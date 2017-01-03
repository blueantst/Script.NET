#################################################################
# Script.NET 查看书签列表的Tcl脚本
# 作者:Blueant
# 版本:1.0
# 日期:2009-06-10
#################################################################

#-------------------------------------------------------------
# 创建书签列表窗口
#-------------------------------------------------------------
proc _plat_Tcl_CreateBookmarks { sFilePath } {

	LangStringEC winName "Bookmark" "书签"
	LangStringEC colName1 "No" "序号"
	LangStringEC colName2 "LineNo" "行号"
	LangStringEC colName3 "Content" "内容"
	
	# 创建书签窗口
	set _nOT [output -create "$winName##GRID"];
	output -active -$_nOT;

	# 清除所有项
	output -format -$_nOT "DelItem ALL;DelColumn All";
	
	# 设置颜色
	output -format -$_nOT "SetRowColors RGB(0,0,128) RGB(255,255,205)";
	output -format -$_nOT "SetGridColor RGB(192,192,192)";
	output -format -$_nOT "ModifyStyle Horizontal Solid;ModifyStyle Vertical Solid";

	# 创建列
	output -format -$_nOT "AddColumn $colName1,50";
	output -format -$_nOT "AddColumn $colName2,65";
	output -format -$_nOT "AddColumn $colName3,300,AutoSize";

	# 设置双击操作的脚本
	set script "\
		set hItem \[output -GetInfo -$_nOT HotItem\];\
		set nLineNo \[output -GetInfo -$_nOT <#\$hItem,1>\];\
		owm -sendcmd \"org.owm.edit\" 111 \"<string>$sFilePath\" \$nLineNo 0;\
	";
	output -$_nOT "<I>Set Script<0> $script";
	
	return $_nOT;
}