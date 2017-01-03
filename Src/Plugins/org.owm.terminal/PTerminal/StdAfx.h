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

#include <afxsock.h>		// MFC socket extensions

#include <afxrich.h>

#include <afxtempl.h>

#include "owm.h"		// OWM公共头文件
#include "IPlatUI.h"	// 平台功能接口
#include "IInterp.h"	// 解释器接口

#include <XTToolkitPro.h> // Xtreme Toolkit Pro components

#include "terminal.h"

#include "TermReg.h"

// 判断是否Tcl解释器
inline BOOL IsTclInterp(int nInterpID) { return (nInterpID < 0x01000000); }

/////////////////////////////////////////////////////////////////////////////
// Telnet支持部分的头文件
#include "SocketRx.h"
#include "SocketDx.h"
#include "SocketTx.h"

/////////////////////////////////////////////////////////////////////////////
// 定义远程管理编译开关，如果打开，则将RCAdmin部分管理代码编译进来，
// 使控制台支持远程程序管理
/////////////////////////////////////////////////////////////////////////////
#define INCLUDE_RCADMIN

// 窗口ID定义
#define VIEW_ID_TERMINAL	10013	// 控制台窗口

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

int AboutOWM(void);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A8E73573_3C54_415C_9986_6D8DF4D5883A__INCLUDED_)
