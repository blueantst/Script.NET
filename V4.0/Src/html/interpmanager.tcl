#################################################################
# File       : owmmanager.tcl									#
# Author     : Blueant Software									#
# Version    : 1.0												#
# Date       : 2006-05-04										#
# Description: Script.NET owmmanager page script				#
#################################################################

package require TclFace

#-------------------------------------------------------------
# TLFAppPage class define
#-------------------------------------------------------------
class  TLFAppPage {
	inherit TLFPage
	
	### public methods ###
	public method  OnCreate {};
}

#-------------------------------------------------------------
#	创建页面
#-------------------------------------------------------------
body TLFAppPage::OnCreate {} {
	global platform_path

	# 创建解释器管理界面
	
	set lsVci [vci_info -listinstalled]
	set lsInterp {}
	foreach vciId $lsVci {
		set vcitype [vci_exec plat GetVciType $vciId]
		if {$vcitype == "interp"} {
			lappend lsInterp $vciId
		}
	}
	
	LangString STR_LINE1 chinese "一共有[llength $lsInterp]个脚本解释器："
	LangString STR_LINE1 english "Total [llength $lsInterp] Interps:"
	set objTitle [CreateObject TLFText "<b>$STR_LINE1</b>"]
	$objTitle SetColor #000040
	
	# OWM列表
	set tableInterp [CreateObject TLFTable]
	#$tableInterp SetGridColor #000080
		#$tableInterp SetTitleColor #008040
	#$tableInterp SetTitleBgColor #a0f0b0
		#$tableInterp SetLineColor #000040
	#$tableInterp SetLineBgColor #f0f0a0
	LangStringEC STR_COL1 "Interp Id" "组件ID"
	$tableInterp AddColumn $STR_COL1
	LangStringEC STR_COL2 "Name" "解释器名"
	$tableInterp AddColumn $STR_COL2
	LangStringEC STR_COL3 "Version" "解释器版本"
	$tableInterp AddColumn $STR_COL3
	LangStringEC STR_COL4 "Provider" "开发商"
	$tableInterp AddColumn $STR_COL4
	LangStringEC STR_COL5 "Instances" "实例数"
	$tableInterp AddColumn $STR_COL5
	foreach interpId $lsInterp {
		if {$interpId == "org.interp.tcl"} {
			# 对Tcl解释器的特殊处理，如果不存在procomp.exe,就不认为安装了完整版的Tcl解释器
			if {[file exists "$platform_path/interps/tcl/bin/procomp.exe"] != 1} {
				continue
			} 
		} 
		
		set interpName [vci_exec plat GetVciName $interpId]
		set interpVersion [vci_exec plat GetVciVersion $interpId]
		set interpProvider [vci_exec plat GetVciProvider $interpId]
		set lsInstance [list ""]
		#catch {set lsInstance [vci_info -listobject $interpId]}
		#set instanceNumber [llength $lsInstance]
		set instanceNumber [vci_exec plat GetVciObjects $interpId lsInstance]
		$tableInterp AddLine2 [list $interpId "" "tcl:<file>PLAT:///html/interp.tcl?vci=$interpId"] [list $interpName] [list $interpVersion] [list $interpProvider] [list $instanceNumber]
	}

#	set tableInformation [CreateObject TLFTable]
#	$tableInformation SetHaveTitle false
#	$tableInformation AddColumn ""
#	set cmd_uninstall "exec $platform_path/uninst.exe &"
#	LangString STR_INFO chinese "单独卸载某个脚本解释器可以点击<a href='tcl:$cmd_uninstall'>卸载链接</a>，选择“卸载Script.NET组件”。"
#	LangString STR_INFO english "Individual uninstall a script interpreter can click the <a href='tcl:$cmd_uninstall'>uninstall link</a>, select 'Uninstall Script.NET components'."
#	$tableInformation AddLine2 [list "<img src='$platform_path/html/image/Info.gif' border=0><b>${STR_INFO}</b>"]
#	$tableInformation SetOneColumnStyle 0 #ffffd0
#	$tableInformation SetLineColor #008000
}

#-------------------------------------------------------------
#	main
#-------------------------------------------------------------
LangString STR_PAGE_TITLE chinese "脚本解释器管理"
LangString STR_PAGE_TITLE english "Interp Manager"
set page [TLFCreatePage $STR_PAGE_TITLE TLFAppPage]
delete object $page;
