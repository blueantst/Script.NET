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

#include "platdef.h"		// platform common define

#include "owm.h"		// OWM公共头文件
#include "IPlatUI.h"	// 平台功能接口
#include "IOwmGrep.h"	// Grep Owm命令定义

#import "RegExp.tlb"
using namespace VBScript_RegExp_10;

#include <XTToolkitPro.h>   // Xtreme Toolkit Pro components

// 窗口ID定义
#define VIEW_ID_GREP			10002	// 查找替换窗口
#define VIEW_ID_SEARCH_RESULT	10003	// 查找结果窗口

#include "SearchOptions.h"
CSearchOptions* GetSearchOptions();

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

// 定义GREP子键
#define REG_GREP				_T("Grep")
#define REG_SHOW_REPLACE_LINE	_T("ShowReplaceLine")	// 是否显示替换后的行


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

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A8E73573_3C54_415C_9986_6D8DF4D5883A__INCLUDED_)
