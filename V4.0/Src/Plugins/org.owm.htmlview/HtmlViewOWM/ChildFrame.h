#if !defined(AFX_CHILDFRAME_H__AA80FC30_B33E_4D69_A4F5_E9F8762C465D__INCLUDED_)
#define AFX_CHILDFRAME_H__AA80FC30_B33E_4D69_A4F5_E9F8762C465D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChildFrame frame

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
protected:
	CChildFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	CImageList	       m_imageList;
	CBitmap	           m_bitmap;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CXTPToolBar           m_wndToolBar;
	CXTPReBar             m_wndReBar;
	CXTComboBoxEx        m_wndAddressBar;

	void InitializeToolbar();
	void InitializeAddressBar();

	virtual ~CChildFrame();

	// Generated message map functions
	//{{AFX_MSG(CChildFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg void OnSelEndOkAddressBar();
	afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRAME_H__AA80FC30_B33E_4D69_A4F5_E9F8762C465D__INCLUDED_)
