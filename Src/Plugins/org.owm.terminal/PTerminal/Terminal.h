////////////////////////////////////////////////////////////////////////////
//	File:		terminal.h
//	Version:	1.0.0.0
//	Created:	2001-10-05
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	Comm define of P terminal.
////////////////////////////////////////////////////////////////////////////
#ifndef __TERMINAL_H_
#define __TERMINAL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define OC_TERM_SWITCHSHELL			110	// 切换终端外壳
#define OC_TERM_DOCK_SWITCHSHELL	111	// 切换Docking窗口终端外壳(通过解释器名字)

// 定义外壳类型
enum{
	SHELL_TCL,			// 本地TCL控制台
	SHELL_PYTHON,		// 本地Python控制台
	SHELL_LUA,			// 本地Lua控制台
	SHELL_RUBY,			// 本地Ruby控制台
	SHELL_PERL,			// 本地Perl控制台
	SHELL_TELNET,		// TELNET控制台
	SHELL_COM,			// 串口控制台
	SHELL_RCADMIN,		// Script.NET远程控制台
};

// 定义提示符
#define SHELL_PROMPT_LOCAL	_T("%")			// 本地控制台
#define SHELL_PROMPT_TCL	_T("TCL%")		// 本地TCL控制台
#define SHELL_PROMPT_PYTHON	_T(">>>")		// 本地Python控制台
#define SHELL_PROMPT_RUBY	_T("RUBY>")			// 本地Ruby控制台
#define SHELL_PROMPT_LUA	_T("LUA>")			// 本地Lua控制台
#define SHELL_PROMPT_PERL	_T("PERL>")			// 本地Perl控制台
#define SHELL_PROMPT_TELNET	_T("TELNET>")	// TELNET客户端
#define SHELL_PROMPT_COM	_T("COM>")		// 串口控制台
#define SHELL_PROMPT_ADMIN	_T("Admin#")	// ATEStudio远程控制台
// 定义扩展提示符
#define SHELL_EXTPROMPT_PYTHON	_T("...")	// Python扩展提示符
#define SHELL_EXTPROMPTLINE_PYTHON	_T("\r\n...")// 带换行符的Python扩展提示符

// 解释器相关定义
#define INTERP_NAME		_T("Interp_Script")	// 解释器名

// 定义颜色
#define SHELL_COLOR_PROMPT	RGB(0, 0, 0)	// 提示符颜色
#define SHELL_COLOR_INPUT	RGB(0, 0, 255)	// 输入字符颜色
#define SHELL_COLOR_RESOK	RGB(128, 0, 255)// 正常结果输出
#define SHELL_COLOR_RESFAIL	RGB(255, 0, 0)	// 失败结果输出

//	Registry keys & values
#define REG_PTERMINAL		_T("owm\\PTerminal")
#define REG_DEFAULTSHELL	_T("DefaultShell")
#define REG_INTERPNAME		_T("InterpName")
#define REG_RCADMIN_TCLINTERPNAME	_T("RemoteInterpName")
#define REG_RCADMIN_IP      _T("RemoteServerIP")
#define REG_RCADMIN_USER    _T("RemoteClientUser")
#define REG_ENABLE_LISTDLG	_T("EnableListDlg")	// 是否允许以对话框方式显示各种列表

// 定义控制台登录域名
//#define IDS_DOMAIN_TELNET	_T("Telnet服务器")

#endif // !defined(__TERMINAL_H_)