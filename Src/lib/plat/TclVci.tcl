#################################################################
# File       : TclVci.tcl
# Author     : Blueant Software
# Version    : 1.0
# Date       : 2001-02-04
# Description: Script.NET TclVci support script
#################################################################

#-------------------------------------------------------------
# TclVci提供的命令如下
#-------------------------------------------------------------
# vci_open name ?ver? ?sharename?
# 功能  : 打开一个VCI组件封装
# 参数  : name		- 组件名
#		  ver		- 组件版本,缺省为最新版本
#		  sharename	- 共享名,如果提供则通过命名方式打开仪器,
#					  这样就可以实现多个实例间的共享
# 返回值: 打开的组件实例的句柄,如果出错,则返回错误信息
#-------------------------------------------------------------
# vci_close handle
# 功能  : 关闭一个VCI组件实例
# 参数  : handle	- 实例的句柄
# 返回值: 如果成功没有返回值,如果失败则返回错误信息
# 说明  : 如果一个封装被打开了多个实例,只有最后一个实例关闭
#		  之后,此封装的声明等信息才会完全卸载,可以通过下面的
#		  device::func命令查询目前所有在内存中的封装信息
#-------------------------------------------------------------
# vci_info -operator devicename ?...?
# 功能  : 组件封装操作和查询
# 参数  : -operator	- 子功能,具体可用的子功能如下
#			1)listdef	- 列函数声明清单(列表形式)
#			2)listfunc	- 列函数名清单(列表形式)
#			3)listparam	- 列函数的参数,如果最后一个参数为
#						  参数名,则可以列出此参数的详细信息
#			4)listdevice- 列目前加载的所有封装类名
#			5)listobject ?devicename?
#						- 列某设备的所有实例句柄,如果不指定
#						  设备,则列所有设备的所有实例句柄
#			6)listinstalled ?device? ?verlist|ver?
#						- 列安装的组件信息
#			7)getvcihandle tclvciobj
#						- 获取指定TclVci对象的VCI句柄
#			8)gettclvciobj vciid vcihandle
#						- 获取指定VCI对象的TclVci封装对象句柄
#-------------------------------------------------------------
# vci_exec handle func parameters
# 功能  : 执行组件对象的函数
# 参数  : handle		- 实例的句柄
#		  func			- 函数名
#		  parameters	- 所有参数,如果为引用类型参数,则必须用
#						  一个TCL变量名
# 返回值: 函数执行的返回值
#-------------------------------------------------------------
# TclVci常用参数类型定义如下
#-------------------------------------------------------------
#	定义			值		说明
#	PTYPE_VOID		0		空
#	PTYPE_CHAR		1		有符号单字节
#	PTYPE_BYTE		2		无符号单字节
#	PTYPE_SHORT		3		有符号双字节
#	PTYPE_WORD		4		无符号双字节
#	PTYPE_INT		5		有符号四字节
#	PTYPE_UINT		6		无符号四字节
#	PTYPE_POINTER	7		指针
#	PTYEP_FLOAT		8		浮点
#	PTYPE_STRING	9		字符串
#	PTYPE_CSTRING	0		CString类型
#	PTYPE_UNKNOWN	-1		未知类型
#-------------------------------------------------------------
# TclVci参数传递方式定义
#-------------------------------------------------------------
#	定义			值		说明
#	PMODE_DIRECT	0		直接传递
#	PMODE_REFERENCE	1		引用&
#	PMODE_POINTER	2		指针*
#	PMODE_POINTER2	3		双重指针**
#	PMODE_CONST		4		常量,不能修改const
#-------------------------------------------------------------

#-------------------------------------------------------------
# 实现字符串到列表的转换
#-------------------------------------------------------------
proc _plat_Tcl_Vci_StrToList {str} {
	set strlength [string length $str]
	set DList ""
	for {set i 0} {$i < $strlength} {incr i} {
		binary scan [string index $str $i] H2 X1
		set DList "$DList $X1"
	}
	return $DList
}

#-------------------------------------------------------------
# 实现列表到字符串的转换
#-------------------------------------------------------------
proc _plat_Tcl_Vci_ListToStr {listA} {
	set strlength [llength $listA]

	set formatStr [string repeat "%c" $strlength]
	return [eval "format $formatStr $listA"]
}

#-------------------------------------------------------------
# 重载缺省的DEBUG_OUT回调函数,指向平台的输出
# 参数含义:
# nDeviceID		- 仪器ID
# nType			- 调试信息类型
#					TEXT_RECEIVE=0x10
#					TEXT_SEND=0x20
#					TEXT_DEBUG=0x30
# nMode			- 调试信息显示方式
#					MODE_HEX=0x10
#					MODE_ASCII=0x20
#					MODE_USER=0x30
# lpData		- 输出的调试信息
# nLen			- 调试信息长度
#-------------------------------------------------------------
proc _plat_Tcl_Vci_DebugOut {nDeviceID nType nMode lpData nLen} {
	set strInfo [varconvert itos $lpData];
	if { [expr $nMode == 0x20] } {
		puts -nonewline "DEBUG:$strInfo";
	} elseif { [expr $nMode == 0x10] } {
		set index 0;
		while {$index < [string length $strInfo]} {
			if {$index == 0} {
				puts "DEBUG:[_plat_Tcl_Vci_StrToList [string range $strInfo $index [expr $index+17]]]";
			} else {
				puts "      [_plat_Tcl_Vci_StrToList [string range $strInfo $index [expr $index+17]]]";
			}
			incr index 18;
		}
	}
}
