#################################################################
# File       : TclDuiVision.tcl
# Author     : Blueant
# Version    : 1.0.20170128
# Date       : 2017-01-28
# Description: Tcl DuiVision script library
#################################################################

package provide TclDuiVision 1.0

package require Itcl

catch {namespace import itcl::*}

# DuiVision的相关定义
namespace eval DUIVISION {
# 消息类型类型
set MSG_BUTTON_DOWN				1	;#鼠标或键盘在控件按下
set MSG_BUTTON_UP				2	;#鼠标或键盘在控件放开
set MSG_BUTTON_DBLCLK			3	;#鼠标在控件双击
set MSG_BUTTON_CHECK			4	;#检查框消息
set MSG_SCROLL_CHANGE			5	;#滚动条位置变更事件
set MSG_CONTROL_BUTTON			6	;#控件内的按钮点击事件
set MSG_MOUSE_MOVE				7	;#鼠标移动事件
set MSG_MOUSE_LEAVE				8	;#鼠标离开事件
set MSG_MOUSE_LDOWN				9	;#鼠标左键按下事件
set MSG_MOUSE_LUP				10	;#鼠标左键放开事件
set MSG_MOUSE_RDOWN				11	;#鼠标右键按下事件
set MSG_MOUSE_RUP				12	;#鼠标右键放开事件
set MSG_KEY_DOWN				13	;#键盘按下事件
set MSG_CONTROL_EVENT			14	;#控件的自定义事件
set MSG_MOUSE_RDBLCLK			15	;#鼠标右键双击
set MSG_CONTROL_SELECT			16	;#控件内的选择事件
set MSG_CONTROL_DELETE			17	;#控件内的删除事件
set MSG_DROP_FILE				18	;#拖拽文件事件
set MSG_FOCUS_CHANGE			19	;#控件的焦点状态变更事件
set MSG_KEY_UP					20	;#键盘放开事件
}

#-------------------------------------------------------------
# TDuiHandler class define
#-------------------------------------------------------------
if {[itcl::find classes TDuiHandler] != "TDuiHandler"} {
class  TDuiHandler {
	constructor {} {};
	destructor {};
	
	### data member ###
	protected variable _logLevel;		# 日志级别
	protected variable _objDuiObject;	# Handler关联的DuiVision对象指针
	protected variable _objHandler;		# C++事件处理对象指针
	protected variable _objTDuiHandler;	# Tcl事件处理对象名
	protected variable _xmlTemplate;	# XML文件名或内容
	protected variable _lsTimer;		# 创建的DUI定时器列表
	protected variable _lsMsgMethod;	# DUI消息和Handler方法的映射表
	
	### public methods ###
	public method  GetClassName {};		# 获取类名
	public method  SetDuiObject {objDuiObject};	# 设置DuiVision对象指针
	public method  SetLogLevel {logLevel};		# 设置日志级别
	public method  StartTimer {timerName time};	# 启动定时器
	public method  StopTimer {timerName};		# 停止定时器
	public method  MsgMethod {name msg method};	# 添加DUI消息的映射方法定义
	public method  OnInit {};			# 事件处理对象初始化函数,C++调用
	public method  OnMessage {name msg wParam lParam};# DUI消息处理函数
	public method  OnTimer {name};		# 定时器处理函数
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
body TDuiHandler::constructor {} {
	chain;
	
	set _logLevel "INFO"
	set _objDuiObject ""
	set _xmlTemplate ""
	set _lsTimer {}
	set _lsMsgMethod {}
	
	# 获取iTcl事件处理对象名,根据类名获取第一个对象名
	set lsObjTDuiHandler [itcl::find objects -class [GetClassName]]
	if {[llength $lsObjTDuiHandler] > 0} {
		set _objTDuiHandler [lindex $lsObjTDuiHandler 0]
	}
}

#-------------------------------------------------------------
#	单元析构函数
#-------------------------------------------------------------
body TDuiHandler::destructor {} {
	foreach timer $_lsTimer {
		dui::timer stop $timer
	}
}

#-------------------------------------------------------------
# 获取类名
#-------------------------------------------------------------
body TDuiHandler::GetClassName {} {
	set classname [$this info class];
	if {[string first "::" $classname] == 0} {
		set classname [string range $classname 2 end];
	}
	return $classname;
}

#-------------------------------------------------------------
# 设置DuiVision对象指针
#-------------------------------------------------------------
body TDuiHandler::SetDuiObject {objDuiObject} {
	set _objDuiObject $objDuiObject
}

#-------------------------------------------------------------
#	设置日志级别
#-------------------------------------------------------------
body TDuiHandler::SetLogLevel {logLevel} {
	set _logLevel $logLevel
}

#-------------------------------------------------------------
#	创建定时器
#-------------------------------------------------------------
body TDuiHandler::StartTimer {timerName time} {
	dui::timer add $timerName $time
	lappend _lsTimer $timerName
}

#-------------------------------------------------------------
#	停止定时器
#-------------------------------------------------------------
body TDuiHandler::StopTimer {timerName} {
	dui::timer stop $timerName
}

#-------------------------------------------------------------
#	添加DUI消息的映射方法定义
#-------------------------------------------------------------
body TDuiHandler::MsgMethod {name msg method} {
	lappend _lsMsgMethod [list $name $msg $method]
}

#-------------------------------------------------------------
#	初始化
#-------------------------------------------------------------
body TDuiHandler::OnInit {} {
}

#-------------------------------------------------------------
#	DUI消息处理
#-------------------------------------------------------------
body TDuiHandler::OnMessage {name msg wParam lParam} {
	if {$_logLevel == "DEBUG"} {
		puts "TDuiHandler::OnMessage $name $msg $wParam $lParam"
	}
	log -level DEBUG "TDuiHandler::OnMessage $name $msg $wParam $lParam"
	foreach msgMethod $_lsMsgMethod {
		set _name [lindex $msgMethod 0]
		set _msg [lindex $msgMethod 1]
		set _method [lindex $msgMethod 2]
		if {($_name == $name) && ($_msg == $msg) && ($_method != "")} {
			$_objTDuiHandler $_method $name $msg $wParam $lParam
		}
	}
}

#-------------------------------------------------------------
#	DUI定时器处理函数
#-------------------------------------------------------------
body TDuiHandler::OnTimer {name} {
}

#-------------------------------------------------------------
# TDuiDialogHandler class define
#-------------------------------------------------------------
if {[itcl::find classes TDuiDialogHandler] != "TDuiDialogHandler"} {
class  TDuiDialogHandler {
	inherit TDuiHandler
	
	constructor {{xmlTemplate ""}} {};
	destructor {};
	
	### data member ###
	
	### public methods ###
	public method  GetControl {name};	# 获取对话框的指定控件指针
	public method  ShowDialog {};	# 显示对话框
	public method  DoOK {};			# 关闭对话框-OK
	public method  DoCancel {};		# 关闭对话框-Cancel
	public method  DoClose {};		# 关闭对话框-Close
	public method  DoYes {};		# 关闭对话框-Yes
	public method  DoNo {};			# 关闭对话框-No
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
body TDuiDialogHandler::constructor {{xmlTemplate ""}} {
	chain;
	set _xmlTemplate $xmlTemplate
}

#-------------------------------------------------------------
#	单元析构函数
#-------------------------------------------------------------
body TDuiDialogHandler::destructor {} {
	chain;
	dui::dialog remove $_objDuiObject
}

#-------------------------------------------------------------
#	获取对话框的指定控件指针
#-------------------------------------------------------------
body TDuiDialogHandler::GetControl {name} {
	return [dui::control getcontrol $name -parentDialog $_objDuiObject]
}

#-------------------------------------------------------------
#	显示对话框
#-------------------------------------------------------------
body TDuiDialogHandler::ShowDialog {} {
	dui::dialog create $_xmlTemplate -handlerName $_objTDuiHandler
}

#-------------------------------------------------------------
#	关闭对话框-OK
#-------------------------------------------------------------
body TDuiDialogHandler::DoOK {} {
	dui::dialog close $_objDuiObject -ok
}

#-------------------------------------------------------------
#	关闭对话框-Cancel
#-------------------------------------------------------------
body TDuiDialogHandler::DoCancel {} {
	dui::dialog close $_objDuiObject -cancel
}

#-------------------------------------------------------------
#	关闭对话框-Close
#-------------------------------------------------------------
body TDuiDialogHandler::DoClose {} {
	dui::dialog close $_objDuiObject -close
}

#-------------------------------------------------------------
#	关闭对话框-Yes
#-------------------------------------------------------------
body TDuiDialogHandler::DoYes {} {
	dui::dialog close $_objDuiObject -yes
}

#-------------------------------------------------------------
#	关闭对话框-No
#-------------------------------------------------------------
body TDuiDialogHandler::DoNo {} {
	dui::dialog close $_objDuiObject -no
}