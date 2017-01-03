#if !defined(AFX_TPSPERLDEBUG_H__DD66F2BE_E3F1_480F_ACA7_8D2C8EE91C25__INCLUDED_)
#define AFX_TPSPERLDEBUG_H__DD66F2BE_E3F1_480F_ACA7_8D2C8EE91C25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsPerlDebug.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTpsPerlDebug dialog

class CTpsPerlDebug : public CDialog
{
// Construction
public:
	CTpsPerlDebug(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsPerlDebug)
	enum { IDD = IDD_TPS_PERL_DEBUG };
	BOOL	m_bEnableMulitFileDebug;
	BOOL	m_bEnableProfileAnalysis;
	BOOL	m_bMultiDebugQuery;
	BOOL	m_bMultiDebugExcludeLib;
	//}}AFX_DATA

	virtual void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsPerlDebug)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsPerlDebug)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckEnableMulitfiledebug();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSPERLDEBUG_H__DD66F2BE_E3F1_480F_ACA7_8D2C8EE91C25__INCLUDED_)
