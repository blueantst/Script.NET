// WorkSpace.h : main header file for the WORKSPACE DLL
//

#ifndef __WORKSPACE_H__
#define __WORKSPACE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceApp
// See WorkSpace.cpp for the implementation of this class
//

class CWorkSpaceApp : public CWinApp
{
public:
	CWorkSpaceApp();

//	CString GetIniString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault);
//	int  GetIniInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault);
//	CString WriteIniString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue);
//	int  WriteIniInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkSpaceApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CWorkSpaceApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	OWM_LINKAGE	m_ol;		// OWM接口的拷贝
	BOOL		m_bDebug;	// 调试开关
	CMultiDocTemplate*	m_pDocTemplate;	// 文档模板
	int			m_nLanguage;// 语言类型
	IPlatUI*	m_pIPlatUI;	// 平台接口指针
	int			m_nLastTreePaneId;	// 最近一次创建的Tree Pane的ID
	int			m_nLastTreeType;	// 最近一次创建的Tree Pane类型
	CString		m_strLastBmpFile;	// 最近一次创建的Tree图片文件名
};

extern class CWorkSpaceApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__WORKSPACE_H__)
