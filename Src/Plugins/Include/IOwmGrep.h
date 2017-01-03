////////////////////////////////////////////////////////////////////////////
//	File:		IPlatUI.h
//	Version:	1.0.0.0
//	Created:	2006-07-08
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	owm grep command define
////////////////////////////////////////////////////////////////////////////
#ifndef __IOWMGREP_H_
#define __IOWMGREP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// OwmGrep命令定义
#define OC_GREP_SETTEXTFIND		100		// 设置查找字符串
#define OC_GREP_GETTEXTFIND		101		// 获取查找字符串
#define OC_GREP_SETTEXTREPLACE	102		// 设置替换字符串
#define OC_GREP_GETTEXTREPLACE	103		// 获取替换字符串
#define OC_GREP_SETLOCATION		104		// 设置位置
#define OC_GREP_GETLOCATION		105		// 获取位置
#define OC_GREP_SETOPTION		106		// 设置查找替换设置
#define OC_GREP_GETOPTION		107		// 获取查找替换设置
#define OC_GREP_CLEARRESULT		108		// 清空结果行
#define OC_GREP_ADDRESULTLINE	109		// 添加结果行

// 查找结果行信息
struct SearchResultInfo {
	SearchResultInfo(LPCTSTR lpszPath, int nLine, LPCTSTR lpszLine)
		: _strPath(lpszPath), _nLine(nLine), _strLine(lpszLine) {};
	CString _strPath;
	int		_nLine;
	CString	_strLine;
};

// 查找设置
struct SearchOption {
	BOOL    m_bIncludeSubFolders;
	BOOL    m_bRegularExpressions;
	CString m_strFind;
	CString m_strReplace;
	CString m_strPath;
	CString m_strFileTypes;
	BOOL    m_bMatchCase;
	BOOL    m_bMatchWholeWords;
};

// 位置类型
enum {GREP_LOC_CURRENTDOC, GREP_LOC_OPENDOCS, GREP_LOC_CURRENTPRJ, GREP_LOC_PATH};

#define LOC_CURRENTDOC	_T("Current Document")
#define LOC_OPENDOCS	_T("All Open Documents")
#define LOC_CURRENTPRJ	_T("Current Project")



#endif // !defined(__IOWMGREP_H_)