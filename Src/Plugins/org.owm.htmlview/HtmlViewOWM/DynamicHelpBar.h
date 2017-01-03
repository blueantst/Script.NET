#if !defined(AFX_DYNAMICHELPBAR_H__53DFF0E4_AD52_432D_BD0E_0363501AE154__INCLUDED_)
#define AFX_DYNAMICHELPBAR_H__53DFF0E4_AD52_432D_BD0E_0363501AE154__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DynamicHelpBar.h : header file
//
#include "ExplorerBar.h"
/////////////////////////////////////////////////////////////////////////////
// CDynamicHelpBar window

class CDynamicHelpBar : public CExplorerBar
{
// Construction
public:
	CDynamicHelpBar();

// Attributes
public:
	CXTPToolBar m_wndToolBar;
	CXTPControlComboBox*	m_pComboBoxSearchWord;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynamicHelpBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDynamicHelpBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDynamicHelpBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHelpHome();
	afx_msg void OnHelpSearch();
	afx_msg void OnUpdateHelpSearch(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DYNAMICHELPBAR_H__53DFF0E4_AD52_432D_BD0E_0363501AE154__INCLUDED_)
