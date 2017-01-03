// PTerminal.h : main header file for the PTERMINAL DLL
//

#ifndef __PTERMINAL_H__
#define __PTERMINAL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPTerminalApp
// See PTerminal.cpp for the implementation of this class
//

class CPTerminalApp : public CWinApp
{
public:
	CPTerminalApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTerminalApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CPTerminalApp)
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
	CWnd*		m_pTerminalView;	// 保存控制台窗口的指针
};

extern class CPTerminalApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__PTERMINAL_H__)
