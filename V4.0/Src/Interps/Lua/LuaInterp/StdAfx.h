// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A81570F9_8FF3_11D5_9949_0050BA0C48D0__INCLUDED_)
#define AFX_STDAFX_H__A81570F9_8FF3_11D5_9949_0050BA0C48D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

/*
#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT
*/
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>

HINSTANCE GetDllInstance();

// 资源切换函数
void get_dll_resource(void);
void reset_dll_resource(void);

#define LOADRCSTRING(str,strID) \
	get_dll_resource();\
	str.LoadString(strID);\
	reset_dll_resource();

#include "platdef.h"		// platform common define

#include "IPlatUI.h"	// 平台功能接口
#include "owm.h"

#include "XMLDom.h"

#define USE_LUA_51
//#define USE_LUA_52

//#include <lua.hpp>

#ifdef USE_LUA_51
#include "../lua5.1/include/lua.hpp"
#pragma comment(lib, "lua5.1.lib")		// 自动链接Lua库
#pragma message("Automatically linking with lua5.1.lib")
#endif

#ifdef USE_LUA_52
#include "../lua5.2/include/lua.hpp"
#pragma comment(lib, "lua5.2.lib")		// 自动链接Lua库
#pragma message("Automatically linking with lua5.2.lib")
#endif

// 定义注册表键
#define REG_INTERP_SUBKEY			_T("interp\\lua\\")

// 重定向参数
#define REG_DIRECT_SUBKEY	_T("interp\\Redirect")			// 重定向子键
// 以下为标准输出重定向
#define REG_DIRECT_STDOUT_OUTPUT	_T("stdout_output")		// 输出窗口
#define REG_DIRECT_STDOUT_FILE		_T("stdout_file")		// 文件
#define REG_DIRECT_STDOUT_RESULT	_T("stdout_result")		// 脚本标准输出
#define REG_DIRECT_STDOUT_PIPE		_T("stdout_pipe")		// 管道
#define REG_DIRECT_STDOUT_MEMORY	_T("stdout_memory")		// 共享内存
#define REG_DIRECT_STDOUT_ADMIN		_T("stdout_admin")		// 管理员用户
// 输出参数
#define REG_DIRECT_STDOUT_OUTPUT_ID	_T("stdout_output_id")	// 输出窗口ID

// LUA编译选项
#define REG_LUA_COMPILE_SUBKEY	_T("interp\\lua\\Compile")	// LUA编译选项子键
#define REG_LUA_COMPILE_AUTO_OUTFILE	_T("AutoOutFile")	// 自动设置输出文件
#define REG_LUA_COMPILE_FORCE_OVERWRITE	_T("ForceOverwrite")// 强制覆盖

// LUA构建选项
#define REG_LUA_BUILD_SUBKEY		_T("interp\\lua\\Build")	// Lua构建选项子键
#define REG_LUA_BUILD_AUTO_OUTFILE	_T("AutoOutFile")	// 自动设置输出文件
#define REG_LUA_BUILD_ADD_LUA_LIB	_T("AddLuaLib")	// 是否选择Lua扩展包添加到生成文件中
#define REG_LUA_BUILD_ADD_SRC_PATHFILES	_T("AddSourcePathFiles")// 是否添加源文件目录下的所有文件
#define REG_LUA_BUILD_ADD_SRC_DATA	_T("AddSourceData")	// 是否添加源文件目录下的data目录所有内容
#define REG_LUA_BUILD_PROCESS_WXKIT	_T("AutoProcessWxLua")	// wxLua脚本自动识别和处理
#define REG_LUA_BUILD_SELECT_TYPE	_T("SelectType")	// 选择可执行文件类型
#define REG_LUA_BUILD_REPLACE_ICON	_T("ReplaceIcon")	// 替换可执行文件的图标
#define REG_LUA_BUILD_SELECT_ICON_TYPE	_T("SelectIconType")	// 用户选择图标的方式

// 定义调试信息
#define _WND_DEBUG	1		// 定义调试窗口

CString GetValueString(lua_State *pLState, int nIndex);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A81570F9_8FF3_11D5_9949_0050BA0C48D0__INCLUDED_)
