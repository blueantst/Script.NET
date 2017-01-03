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

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CChildFrame();

	// Generated message map functions
	//{{AFX_MSG(CChildFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRAME_H__AA80FC30_B33E_4D69_A4F5_E9F8762C465D__INCLUDED_)
