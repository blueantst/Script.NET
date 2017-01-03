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

#define qCvsDebug 0

// TCL重定向参数
#define REG_PY_DIRECT_SUBKEY		_T("interp\\python\\Redirect")	// Python重定向子键
// 以下为标准输出重定向
#define REG_PY_DIRECT_STDOUT_OUTPUT	_T("stdout_output")		// 输出窗口
#define REG_PY_DIRECT_STDOUT_FILE	_T("stdout_file")		// 文件
#define REG_PY_DIRECT_STDOUT_RESULT	_T("stdout_result")		// 脚本标准输出
#define REG_PY_DIRECT_STDOUT_PIPE	_T("stdout_pipe")		// 管道
#define REG_PY_DIRECT_STDOUT_MEMORY	_T("stdout_memory")		// 共享内存
#define REG_PY_DIRECT_STDOUT_ADMIN	_T("stdout_admin")		// 管理员用户
// 输出参数
#define REG_PY_DIRECT_STDOUT_OUTPUT_ID	_T("stdout_output_id")	// 输出窗口ID

// Python加载库路径
#define REG_PY_LIB_SUBKEY			_T("interp\\python\\Library")	// Python加载库子键
#define REG_PY_LIB_PATH				_T("Path")				// 加载库路径
#define REG_PY_USE_EXTERIOR_LIB		_T("UseExteriorPythonLib")	// 使用外部Python路径设置
#define REG_PY_LASTLIBVAL			"LastLoadLibrary"		// 上一次加载的库

// Python调试器配置
#define REG_PY_DEBUG_SUBKEY			_T("interp\\python\\Debug")	// Python调试子键
#define REG_PY_DEBUG_RTVAR			_T("RTVariable")		// 实时更新变量信息
#define REG_PY_DEBUG_RTOBJECT		_T("RTObject")			// 实时更新对象信息
#define REG_PY_DEBUG_RTPROC			_T("RTProcedure")		// 实时更新过程信息
#define REG_PY_DEBUG_RTSTACK		_T("RTStack")			// 实时更新堆栈信息

#define REG_PY_ENABLE_PROFILE		_T("EnableProfileAnalysis")	// 允许覆盖率分析

// Python编译选项
#define REG_PY_COMPILE_SUBKEY		_T("interp\\python\\Compile")	// Python编译选项子键
#define REG_PY_COMPILE_AUTO_OUTFILE	_T("AutoOutFile")	// 自动设置输出文件
#define REG_PY_COMPILE_FORCE_OVERWRITE	_T("ForceOverwrite")// 强制覆盖

// Python构建选项
#define REG_PY_BUILD_SUBKEY			_T("interp\\python\\Build")	// Python构建选项子键
#define REG_PY_BUILD_AUTO_OUTFILE	_T("AutoOutFile")	// 自动设置输出文件
#define REG_PY_BUILD_ADD_PYTHON_LIB	_T("AddPythonLib")	// 是否选择Python扩展包添加到生成文件中
#define REG_PY_BUILD_ADD_SRC_PATHFILES	_T("AddSourcePathFiles")// 是否添加源文件目录下的所有文件
#define REG_PY_BUILD_ADD_SRC_DATA	_T("AddSourceData")	// 是否添加源文件目录下的data目录所有内容
#define REG_PY_BUILD_SELECT_TYPE	_T("SelectType")	// 选择可执行文件类型
#define REG_PY_BUILD_REPLACE_ICON	_T("ReplaceIcon")	// 替换可执行文件的图标
#define REG_PY_BUILD_SELECT_ICON_TYPE	_T("SelectIconType")	// 用户选择图标的方式

//////////////////////////////////////////////////////////////////////
//	定义Python加载库路经											//
//////////////////////////////////////////////////////////////////////
#define PYTHON_LIB_PATH	_T("INTERP://\\;INTERP://\\DLLs;INTERP://\\Lib;INTERP://\\Lib\\lib-tk;INTERP://\\Lib\\site-packages;INTERP://\\Lib\\site-packages\\win32;INTERP://\\Lib\\site-packages\\win32\\lib;INTERP://\\Lib\\site-packages\\PIL;INTERP://\\Plat")

// 定义Python根目录(在解释器目录的基础上的子目录)
#define PYTHON_HOME_PATH _T("Python2.7\\")
//#define PYTHON_HOME_PATH _T("")

// 定义调试信息
#define _WND_DEBUG	1		// 定义调试窗口


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A81570F9_8FF3_11D5_9949_0050BA0C48D0__INCLUDED_)
