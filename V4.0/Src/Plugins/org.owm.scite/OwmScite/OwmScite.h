// OwmScite.h : main header file for the OWMSCITE DLL
//

#ifndef __OWMSCITE_H__
#define __OWMSCITE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// COwmSciteApp
// See OwmScite.cpp for the implementation of this class
//

class COwmSciteApp : public CWinApp
{
public:
	COwmSciteApp();

	CString	GetIniString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault = "");
	int		GetIniInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault = 0);
	CString WriteIniString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue);
	int		WriteIniInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue);

	CString GetPlatRootPath();
	CString GetPlatLibPath();
	CString GetModulePath();

	BOOL ExecFileMngScript(CString strParam);
	CString GetExecuteInterp(CString strDocId);

	void	RefreshAllSciteView();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COwmSciteApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(COwmSciteApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	OWM_LINKAGE	m_ol;		// OWM接口的拷贝
	BOOL		m_bDebug;	// 调试开关
	int			m_nLanguage;// 语言类型
	CMultiDocTemplate*	m_pDocTemplate;	// 文档模板
	IPlatUI*	m_pIPlatUI;	// 平台功能接口
	BOOL		m_bScriptDebuging;// 正处于调试状态

	HMODULE		m_hScintillaDll;
};

extern class COwmSciteApp theApp;

COwmSciteView* GetSciteEditView(CString strFilePath, BOOL bActive = FALSE);
COwmSciteView* GetSciteEditViewWithEditor(ScintillaEditView* pEditor);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__OWMSCITE_H__)
