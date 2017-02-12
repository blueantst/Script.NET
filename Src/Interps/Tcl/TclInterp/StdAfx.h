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

// 获取当前语言
int gGetCurrentLanguage();

// 获取解释器路径
CString GetInterpRootPath();

// 是否支持TK的编译开关
#define _SUPPORT_TK

// 是否使用Tcl8.5版本编译
//#define _USE_TCL84
#define _USE_TCL85
//#define _USE_TCL86

#ifdef _USE_TCL84
#include "../include/tcl.h"
#ifdef _SUPPORT_TK
#include "../include/tk.h"
#endif
#endif

#ifdef _USE_TCL85
#include "../include/tcl8.5/tcl.h"
#ifdef _SUPPORT_TK
#include "../include/tcl8.5/tk.h"
#endif
#endif

#ifdef _USE_TCL86
#define USE_INTERP_RESULT
#define USE_INTERP_ERRORLINE
#include "../include/tcl8.6/tcl.h"
#ifdef _SUPPORT_TK
#include "../include/tcl8.6/tk.h"
#endif
#endif

#include "platdef.h"		// platform common define

#include "IPlatUI.h"	// 平台功能接口
#include "owm.h"

#include "XMLDom.h"

//////////////////////////////////////////////////////////////////////
// 自动链接TCL和TK库
#ifdef _USE_TCL84
#pragma comment(lib, "tcl84.lib")		// 自动链接Tcl库
#pragma message("Automatically linking with tcl84.lib")
#ifdef _SUPPORT_TK
#pragma comment(lib, "tk84.lib")		// 自动链接Tk库
#pragma message("Automatically linking with tk84.lib")
#endif
#endif

#ifdef _USE_TCL85
#pragma comment(lib, "tcl85.lib")		// 自动链接Tcl库
#pragma message("Automatically linking with tcl85.lib")
#ifdef _SUPPORT_TK
#pragma comment(lib, "tk85.lib")		// 自动链接Tk库
#pragma message("Automatically linking with tk84.lib")
#endif
#endif

#ifdef _USE_TCL86
#pragma comment(lib, "tcl86.lib")		// 自动链接Tcl库
#pragma message("Automatically linking with tcl86.lib")
#ifdef _SUPPORT_TK
#pragma comment(lib, "tk86.lib")		// 自动链接Tk库
#pragma message("Automatically linking with tk86.lib")
#endif
#endif

// 定义版本信息和注册信息子键
#define REG_VERSION_SUBKEY		_T("Version")		// 版本信息
#define REG_VER_PLATVERSION		_T("CurVersion")	// 平台版本

// 界面风格设置
#define REG_UISTYLE_SUBKEY		_T("Config\\UIStyle")	// 界面风格子键
#define REG_UISTYLE_HTML		_T("Html")				// Html页面风格

// 定义注册表键
#define REG_INTERP_SUBKEY			_T("interp\\tcl\\")

// 定义加载库子健
#define REG_TCL_LIB_SUBKEY			_T("interp\\tcl\\Lib")
#define REG_TCL_LIB_TK				_T("Tk")
#define REG_TCL_LIB_ITCL			_T("iTcl")
#define REG_TCL_LIB_ITK				_T("iTk")

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

// TCL调试器配置
#define REG_TCL_DEBUG_SUBKEY	_T("interp\\tcl\\Debug")	// TCL调试子键
#define REG_TCL_DEBUG_RTVAR		_T("RTVariable")	// 实时更新变量信息
#define REG_TCL_DEBUG_RTOBJECT	_T("RTObject")		// 实时更新对象信息
#define REG_TCL_DEBUG_RTPROC	_T("RTProcedure")	// 实时更新过程信息
#define REG_TCL_DEBUG_RTSTACK	_T("RTStack")		// 实时更新堆栈信息

#define REG_TCL_ENABLE_PROFILE	_T("EnableProfileAnalysis")	// 允许覆盖率分析
#define REG_TCL_ENABLE_MULTIFILEDEBUG	_T("EnableMultiFileDebug")	// 允许多文件调试
#define REG_TCL_MULTIDEBUG_QUERY	_T("MultiFileDebugQuery")	// 多文件调试时候是否提示加载从调试文件
#define REG_TCL_MULTIDEBUG_EXCLUDE_LIB	_T("MultiDebugExcludeLib")	// 多文件调试时候排除解释器lib目录下的文件

// TCL编译选项
#define REG_TCL_COMPILE_SUBKEY	_T("interp\\tcl\\Compile")	// TCL编译选项子键
#define REG_TCL_COMPILE_AUTO_OUTFILE	_T("AutoOutFile")	// 自动设置输出文件
#define REG_TCL_COMPILE_FORCE_OVERWRITE	_T("ForceOverwrite")// 强制覆盖
#define REG_TCL_COMPILE_ADD_PREFIX		_T("AddPrefix")		// 添加文件前缀

// TCL构建选项
#define REG_TCL_BUILD_SUBKEY	_T("interp\\tcl\\Build")	// TCL构建选项子键
#define REG_TCL_BUILD_AUTO_OUTFILE	_T("AutoOutFile")	// 自动设置输出文件
#define REG_TCL_BUILD_ADD_TCL_LIB	_T("AddTclLib")		// 是否选择Tcl扩展包添加到生成文件中
#define REG_TCL_BUILD_ADD_SRC_PATHFILES	_T("AddSourcePathFiles")// 是否添加源文件目录下的所有文件
#define REG_TCL_BUILD_ADD_SRC_LIB	_T("AddSourceLib")	// 是否添加源文件目录下的Lib目录所有内容
#define REG_TCL_BUILD_FORCE_TKKIT	_T("ForceTkKit")	// 强制按TK脚本生成
#define REG_TCL_BUILD_REPLACE_ICON	_T("ReplaceIcon")	// 替换可执行文件的图标
#define REG_TCL_BUILD_SELECT_ICON_TYPE	_T("SelectIconType")	// 用户选择图标的方式


// 定义平台版本
#define VAR_PLATFORM_VERSION	_T("4.0.0.0")

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A81570F9_8FF3_11D5_9949_0050BA0C48D0__INCLUDED_)
