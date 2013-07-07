#################################################################
# File       : wordhelp.tcl
# Author     : Blueant Software
# Version    : 1.0
# Date       : 2007-06-03
# Description: Script.NET word search support script
#################################################################

#-------------------------------------------------------------
# 选择查找到的主题
#-------------------------------------------------------------
proc _plat_Tcl_SelectSearchResult { word lsIndex } {
	# 创建一个列表,添加根节点
	LangStringEC STR_GROUP_TEXT "Select Help Topic" "选择帮助主题"
	set opt [list "-OPT_GROUP -ICO_GROUP T\"$STR_GROUP_TEXT\""]
	
	# 添加各个子节点
	foreach wordIndex $lsIndex {
		set word_name [mk::get db.keyword!$wordIndex word]
		set word_link [mk::get db.keyword!$wordIndex link]
		lappend opt "-OPT_RADIOBUTTON(radio($wordIndex)) T\"$word_name\[$word_link\]\""
	}
	
	# 配置标题和信息栏
	LangStringEC STR_MSG_WIN_TITLE "Select Help Topic" "请选择帮助主题"
	LangStringEC STR_MSG_TITLE1 "Select help topic of " "选择关键字"
	LangStringEC STR_MSG_TITLE2 " to open" "的帮助主题"
	lappend opt "-OPT_MSG T\"$STR_MSG_WIN_TITLE\" E\"$STR_MSG_TITLE1 $word $STR_MSG_TITLE2\""
	# 执行属性树对话框
	set ret [OptionTree $opt];
	if { $ret != -1 } {
		set ls [array get radio]
		foreach {wordIndex select} $ls {
			if {$select ==1} {
				return $wordIndex
			}
		}
	}
	
	return ""
}

#-------------------------------------------------------------
# 查找关键字
#-------------------------------------------------------------
proc _plat_Tcl_SearchWord { wnd searchmode interpId word } {
	global platform_path platform_language
	
	if {$platform_language == "chinese"} {
		set dynhelppage "$platform_path/doc/dynhelp_cn.htm"
	} else {
		set dynhelppage "$platform_path/doc/dynhelp.htm"
	}
	if {$word == ""} {
		if {$wnd == "htmlview"} {
			owm -sendcmd "org.owm.htmlview" 84 "<string>$dynhelppage" 0
		} else {
			owm -sendcmd "org.owm.htmlview" 91 10014 "<string>$dynhelppage"
		}
	}
	
	set interpName [vci_exec plat GetVciName $interpId]
	if {[file exists "$platform_path/doc/$interpName/keyword.idx"]} {
		package require Mk4tcl
		mk::file open db "$platform_path/doc/$interpName/keyword.idx"
		if {$searchmode == "accurate"} {
			# 精确查找
			set lsIndex [mk::select db.keyword -glob word $word]
		} else {
			# 模糊查找
			set lsIndex [mk::select db.keyword -keyword word $word]
		}
		
		if {[llength $lsIndex] > 0} {
			if {[llength $lsIndex] > 1} {
				# 搜索到大于1个主题,则进行选择
				set word_index [_plat_Tcl_SelectSearchResult $word $lsIndex]
				if {$word_index == ""} {
					if {$wnd == "htmlview"} {
						owm -sendcmd "org.owm.htmlview" 84 "<string>$dynhelppage" 0
					} else {
						owm -sendcmd "org.owm.htmlview" 91 10014 "<string>$dynhelppage"
					}
					mk::file close db
					return
				}				
			} else {
				set word_index [lindex $lsIndex 0]
			}
			set word_link [mk::get db.keyword!$word_index link]
			if {$wnd == "htmlview"} {
				# 在HtmlView窗口中显示帮助
				# 86命令会打开一个新的窗口,84命令只会用现有的第一个浏览器窗口
				#owm -sendcmd "" 86 "<string>$platform_path/doc/$interpName/$word_link" "<string>org.owm.htmlview"
				owm -sendcmd "org.owm.htmlview" 84 "<string>$platform_path/doc/$interpName/$word_link" 0
			} else {
				# 在DynamicHelp窗口中显示帮助
				vci_exec plat ActiveDockingPane 10014
				owm -sendcmd "org.owm.htmlview" 91 10014 "<string>$platform_path/doc/$interpName/$word_link"
			}
		} else {
			if {$wnd == "htmlview"} {
				owm -sendcmd "org.owm.htmlview" 84 "<string>$dynhelppage" 0
			} else {
				owm -sendcmd "org.owm.htmlview" 91 10014 "<string>$dynhelppage"
			}
		}
		mk::file close db
	}
}

#-------------------------------------------------------------
# 主程序
#-------------------------------------------------------------
if {$argc > 0} {
	set opr [lindex $argv 0];
	if {$opr == "SearchWord"} {
		if {$argc > 4} {
			return [_plat_Tcl_SearchWord [lindex $argv 1] [lindex $argv 2] [lindex $argv 3] [lindex $argv 4]];
		}
	}
}