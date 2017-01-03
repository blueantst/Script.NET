#################################################################
# Script.NET 选择打开文件的模块脚本
# 作者:Blueant
# 版本:1.0
# 日期:2007-01-01
#################################################################

# 创建一个列表,添加根节点
LangStringEC STR_GROUP_TEXT "Select Module" "选择打开文档的模块"
set opt [list "-OPT_GROUP -ICO_GROUP T\"$STR_GROUP_TEXT\""]

set lsVci [vci_info -listinstalled]
set lsOwm {}
foreach vciId $lsVci {
	set vcitype [vci_exec plat GetVciType $vciId]
	if {$vcitype == "owm"} {
		lappend lsOwm $vciId
	}
}

# 添加各个子节点
foreach owmId $lsOwm {
	set owmName [vci_exec plat GetVciName $owmId]
	lappend opt "-OPT_RADIOBUTTON(radio($owmId)) T\"$owmName\[$owmId\]\""
}

# 配置标题和信息栏
LangStringEC STR_MSG_WIN_TITLE "Select Open File Module" "请选择模块"
LangStringEC STR_MSG_TITLE1 "Select module to open file:" "选择打开文档"
LangStringEC STR_MSG_TITLE2 "" "的模块"
lappend opt "-OPT_MSG T\"$STR_MSG_WIN_TITLE\" E\"$STR_MSG_TITLE1 $argv $STR_MSG_TITLE2\""
# 执行属性树对话框
set ret [OptionTree $opt];
if { $ret != -1 } {
	set ls [array get radio]
	foreach {owmId select} $ls {
		if {$select ==1} {
			owm -sendcmd "" 86 "<string>$argv" "<string>$owmId"
		}
	}
}