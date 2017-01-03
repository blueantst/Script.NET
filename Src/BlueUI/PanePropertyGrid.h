// PanePropertyGrid.h : header file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// 1998-2005 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PANEPROPERTYGRID_H__9B1C5CED_82EC_43A1_BDF6_E70325EA966F__INCLUDED_)
#define AFX_PANEPROPERTYGRID_H__9B1C5CED_82EC_43A1_BDF6_E70325EA966F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPanePropertyGrid window


class CPanePropertyGrid : public CWnd
{
// Construction
public:
	CPanePropertyGrid();

// Attributes
public:
	CXTPPropertyGrid m_wndPropertyGrid;

	CXTPToolBar m_wndToolBar;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanePropertyGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPanePropertyGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPanePropertyGrid)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG

	afx_msg void OnPaneproperiesCategorized();
	afx_msg void OnUpdatePaneproperiesCategorized(CCmdUI* pCmdUI);
	afx_msg void OnPaneproperiesAlphabetic();
	afx_msg void OnUpdatePaneproperiesAlphabetic(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANEPROPERTYGRID_H__9B1C5CED_82EC_43A1_BDF6_E70325EA966F__INCLUDED_)
