// [!output PROJECT_NAME].h : main header file for the [!output PROJECT_NAME] DLL
//

#ifndef __[!output PROJECT_NAME]_H__
#define __[!output PROJECT_NAME]_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// C[!output PROJECT_NAME]App
// See [!output PROJECT_NAME].cpp for the implementation of this class
//

class C[!output PROJECT_NAME]View;
class C[!output PROJECT_NAME]App : public CWinApp
{
public:
	C[!output PROJECT_NAME]App();

	CString GetPlatRootPath();
	CString GetModulePath();
	CString	GetIniString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault = "");
	int		GetIniInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault = 0);
	CString WriteIniString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue);
	int		WriteIniInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C[!output PROJECT_NAME]App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(C[!output PROJECT_NAME]App)
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
};

class C[!output PROJECT_NAME]View;
C[!output PROJECT_NAME]View* GetFirst[!output PROJECT_NAME]View();

extern class C[!output PROJECT_NAME]App theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__[!output PROJECT_NAME]_H__)
