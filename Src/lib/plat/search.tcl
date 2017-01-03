#################################################################
# File       : search.tcl
# Author     : Blueant Software
# Version    : 1.0
# Date       : 2001-02-04
# Description: Script.NET editer search support script
#			 : if reimplement search function, please don't modify
#			 : follow function name.
#################################################################

#-------------------------------------------------------------
# 初始化查找窗口函数
# 入口参数: 文件名
#-------------------------------------------------------------
proc _plat_Tcl_InitSearchWnd { sFilePath } {

	# 创建窗口
	LangStringEC STR_WINNAME "Find" "查找"
	set _nOT [puts create "$STR_WINNAME##GRID"];
	# 激活窗口
	puts active $_nOT;
	# 清除内容
	puts format,$_nOT "DelItem ALL;DelColumn All";
	# 设置颜色
	puts format,$_nOT "SetRowColors RGB(0,0,255) RGB(255,255,205)";
	# 添加列
	LangStringEC STR_COL1 "No" "序号"
	puts format,$_nOT "AddColumn $STR_COL1,60";
	LangStringEC STR_COL2 "Line" "行号"
	puts format,$_nOT "AddColumn $STR_COL2,60";
	LangStringEC STR_COL3 "Content" "内容"
	puts format,$_nOT "AddColumn $STR_COL3,300,AutoSize";

	# 设置双击操作的脚本
	set script "\
		set hItem \[puts getinfo,$_nOT HotItem\];\n\
		set nLineNo \[puts getinfo,$_nOT <#\$hItem,1>\];\n\
		owm -sendcmd \"org.owm.edit\" 111 \{<string>$sFilePath\} \$nLineNo 0;\
	";
	puts -$_nOT "<I>Set Script<0> $script";

	# 返回窗口ID
	return $_nOT;
}

#-------------------------------------------------------------
# 添加查找到的行
#-------------------------------------------------------------
proc _plat_Tcl_InsertSearchLine { index nLineNo sLine } {
	LangStringEC STR_WINNAME "Find" "查找"
	output "$STR_WINNAME" "<14>$index,$nLineNo,$sLine";
	incr index -1;
	output "$STR_WINNAME" "<I>Set Script<$index> <1>";
}

#-------------------------------------------------------------
# 主程序
#-------------------------------------------------------------
if {$argc > 0} {
	set opr [lindex $argv 0];
	if {$opr == "CreateSearchWnd"} {
		if {$argc > 1} {
			return [_plat_Tcl_InitSearchWnd [lindex $argv 1]];
		}
	} elseif {$opr == "InsertSearchLine"} {
		puts "$argv"
		if {$argc > 3} {
			_plat_Tcl_InsertSearchLine [lindex $argv 1] [lindex $argv 2] [lindex $argv 3];
		}
	}
}