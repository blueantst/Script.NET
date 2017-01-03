// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A8E73573_3C54_415C_9986_6D8DF4D5883A__INCLUDED_)
#define AFX_STDAFX_H__A8E73573_3C54_415C_9986_6D8DF4D5883A__INCLUDED_

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


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>

#include "platdef.h"		// platform common defin

#include "vcicomm.h"
#include "owm.h"		// OWM公共头文件
#include "IPlatUI.h"	// 平台功能接口
//#include "ILicense.h"	// License接口
#include "IInterp.h"	// 解释器接口
#include "IOwmGrep.h"	// Grep Owm命令定义

#include <XTToolkitPro.h>		// X~treme Toolkit MFC extensions

#include "OwmSciteView.h"
#include "editcmd.h"
#include "editreg.h"
#include "FileEvent.h"

// 解释器名
#define INTERP_NAME		_T("Interp_Script")


#define VIEW_ID_BOOKMARK			30010	// 书签窗口
#define VIEW_ID_BREAKPOINT			30011	// 断点窗口
#define OUTPUT_ID_BOOKMARK			10		// 书签对应的输出窗口ID
#define OUTPUT_ID_BREAKPOINT		11		// 断点对应的输出窗口ID
#define VIEW_ID_DYNHELP				10014	// 动态帮助窗口

#pragma   comment(lib,"Shlwapi.lib")

// 资源切换函数
void get_dll_resource(void);
void reset_dll_resource(void);

// OWM调试宏定义
#define _IF_OWM_DEBUG		if(theApp.m_bDebug){
#define _ENDIF_OWM_DEBUG	}
#define _IF_OWM_NDEBUG		if(!theApp.m_bDebug){
#define _ENDIF_OWM_NDEBUG	}

// 向平台查询OWM安装信息
COWMArray* GetOwmInfo(int nId);

// 定义字体格式子键
#define REG_FORMAT_SUBKEY		_T("Format")
#define REG_FMFONT				_T("Font")			// 字体名
#define REG_FMSIZE				_T("Size")			// 大小
#define REG_FMCOLORFG			_T("ColorFg")		// 前景色
#define REG_FMCOLORBK			_T("ColorBk")		// 背景色
#define REG_FMBOLD				_T("Bold")			// 是否粗体
#define REG_FMITALIC			_T("Italic")		// 是否斜体
#define REG_FMUNDERLINE			_T("UnderLine")		// 下划线
#define REG_FMSTRIKEOUT			_T("StrikeOut")		// 删除线


// 线程多语言支持
#define REG_VERSION_SUBKEY		_T("Version")		// 版本信息
#define REG_REG_LANGUAGE		_T("Language")		// 当前语言
// 多语言定义
#define LANGUAGE_PAGE_ENGLISH	1033
#define LANGUAGE_PAGE_CHINESE	2052

void SetLocateInThread();	// 设置线程中的语言类型

// 重定向参数
#define REG_DIRECT_SUBKEY	_T("interp\\Redirect")	// 重定向子键
// 以下为标准输出重定向
#define REG_DIRECT_STDOUT_OUTPUT_ID	_T("stdout_output_id")	// 输出窗口ID

// 控制台OWM命令定义
#define OC_TERM_SWITCHSHELL			110	// 切换终端外壳
#define OC_TERM_DOCK_SWITCHSHELL	111	// 切换Docking窗口终端外壳(通过解释器名字)

//HINSTANCE gGetScintillaLibHandle();

// 自动链接TinyXML库
#ifdef _DEBUG
#pragma comment(linker,"/nodefaultlib:LIBCMTd.lib")
#pragma comment(lib, "tinyxmld.lib")
#else
#pragma comment(linker,"/nodefaultlib:LIBCMT.lib")
#pragma comment(lib, "tinyxml.lib")
#endif
#pragma message("Automatically linking with TinyXML library")

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A8E73573_3C54_415C_9986_6D8DF4D5883A__INCLUDED_)
