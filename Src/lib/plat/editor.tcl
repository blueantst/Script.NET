#################################################################
# File       : editor.tcl
# Author     : Blueant Software
# Version    : 1.0
# Date       : 2004-08-20
# Description: Script.NET editor encap script
#################################################################

package require Itcl

#-------------------------------------------------------------
# TEditor class define
#-------------------------------------------------------------
if {[itcl::find classes TEditor] != "TEditor"} {
::itcl::class  TEditor {
	constructor { {fname ""} } {};
	destructor {};
	
	### data member ###
	private variable _filename;		#文件名
	private variable _owm_id;		#模板ID
	
	### public methods ###
	public method  Open { {fname ""} };	#打开文件
	public method  Attach { {no -1} };	#建立文件关联
	public method  GetOpenCount {};		#获取打开的文件数
	public method  GetOpenFile { no };	#获取打开的文件名
	public method  SetReadOnly { {bReadOnly 1} };#设置只读属性
	public method  SetCurrentLine { lineno };#设置当前行
	public method  SetCurrentExecLine { lineno };#设置当前执行行
	public method  SearchLine { line };	#查找行,并设置为当前行
	public method  GetBreakList {};		#获取断点列表
	public method  AddBreakList { lsbreak };#增加断点列表
	public method  ClearBreakList {};		#清空断点列表
	public method  GetLineCount {};		#获取行数
	public method  GetLine { lineno };	#获取行
	public method  SetLine { lineno text };	#设置行
	public method  GetSelectLine { startline endline };	#获取当前选择的行
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TEditor::constructor { {fname "" } } {
	set _filename $fname
	set _owm_id "org.owm.edit"
}

#-------------------------------------------------------------
#	单元析构函数
#-------------------------------------------------------------
::itcl::body TEditor::destructor {} {
}

#-------------------------------------------------------------
#	打开文件
#	参数:
#	fname	: 文件名,为空表示用保存的文件名
#-------------------------------------------------------------
::itcl::body TEditor::Open { {fname ""} } {
	if {$fname != ""} {
		set _filename $fname
	}
	
	return [owm -sendcmd $_owm_id 102 "<string>$_filename" 0]
}

#-------------------------------------------------------------
#	建立文件关联
#	参数:
#	no	: 文件序号,为-1表示关联到当前激活的文件
#-------------------------------------------------------------
::itcl::body TEditor::Attach { {no -1} } {
	if {$no == -1} {
		set no -2
	}
	
	set _filename [GetOpenFile $no]
}

#-------------------------------------------------------------
#	获取打开的文件数
#	参数:
#-------------------------------------------------------------
::itcl::body TEditor::GetOpenCount {} {
	return [owm -sendcmd $_owm_id 105 -1 0]
}

#-------------------------------------------------------------
#	获取打开的文件名
#	参数:
#	no	: 文件序号,为-1表示获取文件数,-2表示获取当前文件名
#-------------------------------------------------------------
::itcl::body TEditor::GetOpenFile { no } {
	return [owm -sendcmd $_owm_id 105 $no 0 "<string>"]
}

#-------------------------------------------------------------
#	设置只读属性
#	参数:
#	bReadOnly	: 只读属性,1-只读,0-读写,缺省为只读
#-------------------------------------------------------------
::itcl::body TEditor::SetReadOnly { {bReadOnly 1} } {
	owm -sendcmd $_owm_id 104 "<string>$_filename" $bReadOnly
}

#-------------------------------------------------------------
#	设置当前行
#	参数:
#	lineno	: 行号
#-------------------------------------------------------------
::itcl::body TEditor::SetCurrentLine { lineno } {
	owm -sendcmd $_owm_id 111 "<string>$_filename" $lineno
}

#-------------------------------------------------------------
#	设置当前执行行
#	参数:
#	lineno	: 行号
#-------------------------------------------------------------
::itcl::body TEditor::SetCurrentExecLine { lineno } {
	owm -sendcmd $_owm_id 112 "<string>$_filename" $lineno
}

#-------------------------------------------------------------
#	查找行,并设置为当前行
#	参数:
#	line	: 行内容
#-------------------------------------------------------------
::itcl::body TEditor::SearchLine { line } {
	owm -sendcmd $_owm_id 113 "<string>$_filename" "<string>$line"
}

#-------------------------------------------------------------
#	获取断点列表
#-------------------------------------------------------------
::itcl::body TEditor::GetBreakList {} {
	owm -sendcmd $_owm_id 115 "<string>$_filename" "<uintarray>lsbreak"
	return $lsbreak
}

#-------------------------------------------------------------
#	增加断点列表
#-------------------------------------------------------------
::itcl::body TEditor::AddBreakList { lsbreak } {
	owm -sendcmd $_owm_id 124 "<string>$_filename" "<uintarray>lsbreak"
}

#-------------------------------------------------------------
#	清空断点列表
#-------------------------------------------------------------
::itcl::body TEditor::ClearBreakList {} {
	owm -sendcmd $_owm_id 125 "<string>$_filename" 0
}

#-------------------------------------------------------------
#	获取行数
#-------------------------------------------------------------
::itcl::body TEditor::GetLineCount {} {
	return [owm -sendcmd $_owm_id 117 "<string>$_filename" 0]
}

#-------------------------------------------------------------
#	获取行
#-------------------------------------------------------------
::itcl::body TEditor::GetLine { lineno } {
	return [owm -sendcmd $_owm_id 118 "<string>$_filename" $lineno "<string>"]
}

#-------------------------------------------------------------
#	设置行
#-------------------------------------------------------------
::itcl::body TEditor::SetLine { lineno text } {
	return [owm -sendcmd $_owm_id 119 "<string>$_filename" "<string><$lineno>$text"]
}

#-------------------------------------------------------------
#	获取当前选择的行
#-------------------------------------------------------------
::itcl::body TEditor::GetSelectLine { startline endline } {
	upvar $startline _startline
	upvar $endline _endline
	set linecount [owm -sendcmd $_owm_id 120 "<string>$_filename" "<&int>_startline"]
	set _endline [expr $_startline + $linecount -1]
}
