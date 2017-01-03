////////////////////////////////////////////////////////////////////////////
//	File:		editcmd.h
//	Version:	1.0.0.0
//	Created:	29-Dec-1998
//
//	Author:		Stcherbatchenko Andrei
//	E-mail:		windfall@gmx.de
//
//	Command defintions and resource IDs for Crystal Edit - syntax coloring
//	text editor.
//	Include this in .RC file in your project.
//
//	You are free to use, distribute or modify this code
//	but LEAVE THIS HEADER INTACT.
////////////////////////////////////////////////////////////////////////////

#ifndef __EDITCMD_H_INCLUDED_
#define __EDITCMD_H_INCLUDED_

#define ID_MACRO 20000
#define ID_MACRO_LIMIT 20200

#define ID_USER_CMD 21000
#define ID_USER_CMD_LIMIT 21200

#define ID_PLUGINS_CMD 22000
#define ID_PLUGINS_CMD_LIMIT 22500

////////////////////////////////////////////////////////////////////
// OwmEdit扩展命令
#define OC_OPENFILE				102	// 打开一个文档(to owm)
#define OC_OPENFILEFIRST		103	// 在第一个位置打开文档(to owm)
#define OC_SETREADONLY			104	// 设置文档的只读属性
#define OC_GETOPENFILE			105	// 获取打开的文件名

#define OC_SETEXECUTION			110	// 设置错误标记
#define OC_SETCURENTLINE		111	// 设置当前行
#define OC_SETCURENTEXECLINE	112	// 设置当前执行行
#define OC_SEARCHLINE			113	// 查找指定内容的行并设置为当前行
#define OC_GETBREAKLIST			115	// 获取断点列表
#define OC_SETSLAVEDEBUG		116	// 设置从调试模式(调试模式,但非主调试程序)

#define OC_GET_LINECOUNT		117	// 获取总行数
#define OC_GET_LINE				118	// 获取行
#define OC_SET_LINE				119	// 设置行
#define OC_GET_SELECTLINE		120	// 获取当前选择的行范围
#define OC_SET_SELECTLINE		121	// 设置选择的行
#define OC_GET_CURSORPOS		122	// 获取光标位置
#define OC_SET_CURSORPOS		123	// 设置光标位置
#define OC_SET_BREAKLIST		124	// 设置断点列表
#define OC_CLEAR_BREAK			125	// 清空断点

#define OC_EDIT_FIND			130	// 查找
#define OC_EDIT_REPLACE			131	// 替换

#define OC_WORD_HELP			140	// 单词帮助


#endif
