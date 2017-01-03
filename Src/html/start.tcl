#################################################################
# File       : start.tcl										#
# Author     : Blueant Software									#
# Version    : 1.0												#
# Date       : 2006-05-04										#
# Description: start page script								#
#################################################################

package require TclFace

#-------------------------------------------------------------
# TLFAppPage class define
#-------------------------------------------------------------
class  TLFAppPage {
	inherit TLFPage
	
	### data member ###
	
	### public methods ###
	public method  CreateMenu {};
	public method  OnCreate {};
	public method  CheckInstalledInterpPackage {};	# 判断是否安装了解释器开发包
}

#-------------------------------------------------------------
#	创建菜单
#-------------------------------------------------------------
body TLFAppPage::CreateMenu {} {
	global platform_path platform_language
	
	# 主菜单
	LangString STR_MENU_1 chinese "管理"
	LangString STR_MENU_1 english "Manager"
	LangString STR_MENU_2 chinese "文档"
	LangString STR_MENU_2 english "Document"
	LangString STR_MENU_3 chinese "演示"
	LangString STR_MENU_3 english "Samples"
	
	TLFToggleMenu ::mainmenu
	
	# 获取安装的解释器
	set lsVci [vci_info -listinstalled]
	set lsInterp {}
	foreach vciId $lsVci {
		set vcitype [vci_exec plat GetVciType $vciId]
		if {$vcitype == "interp"} {
			lappend lsInterp $vciId
		}
	}
	
	# 文档
	LangString STR_DOCUMENT chinese "文档"
	LangString STR_DOCUMENT english "Document"
	set lsDoc {}
	foreach interpId $lsInterp {
		set interpName [vci_exec plat GetVciName $interpId]
		if {($platform_language == "chinese") && ([file exist "$platform_path/doc/$interpName/index_cn.html"])} {
			lappend lsDoc [list "$interpName $STR_DOCUMENT" "$platform_path/doc/$interpName/index_cn.html"]
		} elseif {[file exists "$platform_path/doc/$interpName/index.html"]} {
			lappend lsDoc [list "$interpName $STR_DOCUMENT" "$platform_path/doc/$interpName/index.html"]
		} 
	}
	::mainmenu AddMenu m2 $STR_MENU_2 "image/icon_32_doc.jpg" "" $lsDoc
	
	# 演示
	LangString STR_SAMPLES chinese "演示"
	LangString STR_SAMPLES english "Samples"
	set lsSamples {}
	foreach interpId $lsInterp {
		set interpName [vci_exec plat GetVciName $interpId]
		if {[file exists "$platform_path/samples/$interpName"]} {
			lappend lsSamples [list "$interpName $STR_SAMPLES" "tcl:<file>PLAT://\\html\\\\samples.tcl?dir=$platform_path/samples/$interpName"]
		}
	}
	::mainmenu AddMenu m3 $STR_MENU_3 "image/icon_32_samples.jpg" "none" $lsSamples

	# 管理
	LangString STR_MENU1_VCIMANAGER chinese "VCI组件管理"
	LangString STR_MENU1_VCIMANAGER english "Component Manager"
	LangString STR_MENU1_OWMMANAGER chinese "界面组件管理"
	LangString STR_MENU1_OWMMANAGER english "UI Plugin Manager"
	LangString STR_MENU1_INTERPMANAGER chinese "解释器管理"
	LangString STR_MENU1_INTERPMANAGER english "Interpert Manager"
	
	::mainmenu AddMenu m1 $STR_MENU_1 "image/icon_32x32_config.gif" "none" [list \
			[list $STR_MENU1_VCIMANAGER "tcl:<file>PLAT:///html/vcimanager.tcl"] \
			[list $STR_MENU1_OWMMANAGER "tcl:<file>PLAT:///html/owmmanager.tcl"] \
			[list $STR_MENU1_INTERPMANAGER "tcl:<file>PLAT:///html/interpmanager.tcl"] ]
	
	SetTagObject "LEFT_MENU" ::mainmenu
}

#-------------------------------------------------------------
#	创建页面
#-------------------------------------------------------------
body TLFAppPage::OnCreate {} {
	global platform_language platform_path

	# 创建菜单
	CreateMenu

	if {[CheckInstalledInterpPackage] == -1} {
		set tableInformation [CreateObject TLFTable]
		$tableInformation SetHaveTitle false
		$tableInformation AddColumn ""
		set url_download [vci_exec plat GetPlatDownloadURL]
		LangString STR_INFO chinese "您还没有安装任何脚本语言开发包，点击链接 <a href='$url_download'>$url_download</a> 可以下载安装。"
		$tableInformation AddLine2 [list "<img src='$platform_path/html/image/Info.gif' border=0><b>${STR_INFO}</b>"]
		$tableInformation SetOneColumnStyle 0 #ffffd0
		$tableInformation SetLineColor #008000
	}
	
	# 最近打开文件列表
	set tableRecentFile [CreateObject TLFTable]
	#$tableRecentFile SetTitleColor #008040
	LangString STR_COL_NAME1 chinese "最近打开的文件列表："
	LangString STR_COL_NAME1 english "Recent file list:"
	$tableRecentFile AddColumn $STR_COL_NAME1
	catch {
		set ls [recent-file-list]
		foreach recentfile $ls {
			# 如果是英文模式,则需要转换为UTF-8编码
			set recentfile [varconvert strtoutf8 $recentfile]
			$tableRecentFile AddLine2 [list $recentfile "" "pfe:open $recentfile"]
		}
	}

	# 最近打开工程列表
	if {[llength [recent-project-list]] > 0} {
		set tableRecentProject [CreateObject TLFTable]
		LangString STR_COL_NAME1 chinese "最近打开的工程列表："
		LangString STR_COL_NAME1 english "Recent project list:"
		$tableRecentProject AddColumn $STR_COL_NAME1
		catch {
			set ls [recent-project-list]
			foreach recentproject $ls {
				# 如果是英文模式,则需要转换为UTF-8编码
				set recentproject [varconvert strtoutf8 $recentproject]
				regsub -all {\\} $recentproject {\\\\} projectfile
				$tableRecentProject AddLine2 [list $recentproject "" "tcl:owm -sendcmd 0 74 \"<string>$projectfile\" 0"]
			}
		}
	}
	
	# 工程操作链接
	LangString STR_NEW_PROJECT chinese "新建工程"
	LangString STR_NEW_PROJECT english "New Project"
	LangString STR_OPEN_PROJECT chinese "打开工程"
	LangString STR_OPEN_PROJECT english "Open Project"
	LangString STR_CLOSE_PROJECT chinese "关闭工程"
	LangString STR_CLOSE_PROJECT english "Close Project"
	set objText [CreateObject TLFText "\[ <b><a href='tcl:<file>PLAT://\\conf\\Menu\\NewPrj.tcl'>$STR_NEW_PROJECT</a> | <a href='tcl:owm -sendcmd -1 74 0 0'>$STR_OPEN_PROJECT</a> | <a href='tcl:owm -sendcmd -1 75 0 0'>$STR_CLOSE_PROJECT</a></b> \]"]
	$objText SetColor #000040
}

#-------------------------------------------------------------
#	判断是否安装了解释器开发包
#-------------------------------------------------------------
body TLFAppPage::CheckInstalledInterpPackage {} {
	global platform_path
	set result -1
	
	catch {
	
	set fxml [open "$platform_path/conf/install.xml"]
	set xmlbuf [read $fxml]
	set result [string first "</component>" $xmlbuf]
	close $fxml
	
	}
	
	return $result
}

#-------------------------------------------------------------
#	main
#-------------------------------------------------------------
LangString STR_PAGE_TITLE chinese "起始页"
LangString STR_PAGE_TITLE english "Start Page"
set page [TLFCreatePage $STR_PAGE_TITLE TLFAppPage "startpage.tph" "disablemenu"]
delete object $page;
