// DuiVisionDesigner.h : main header file for the DuiVisionDesigner DLL
//

#ifndef __DuiVisionDesigner_H__
#define __DuiVisionDesigner_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDuiVisionDesignerApp
// See DuiVisionDesigner.cpp for the implementation of this class
//

class CDuiVisionDesignerView;
class CDuiVisionDesignerApp : public CWinApp
{
public:
	CDuiVisionDesignerApp();

	CString GetPlatRootPath();
	CString GetModulePath();

	CString	GetIniString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault = "");
	int		GetIniInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault = 0);
	CString WriteIniString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue);
	int		WriteIniInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDuiVisionDesignerApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDuiVisionDesignerApp)
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
	HINSTANCE	m_hDuiPluginHandle;		// 保存界面插件动态库的句柄
};

class CDuiVisionDesignerView;
CDuiVisionDesignerView* GetFirstDuiVisionDesignerView();

extern class CDuiVisionDesignerApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DuiVisionDesigner_H__)
