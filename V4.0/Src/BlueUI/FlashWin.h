#if !defined(AFX_FLASHWIN_H__52744201_F9B3_11D4_922F_000021E2956D__INCLUDED_)
#define AFX_FLASHWIN_H__52744201_F9B3_11D4_922F_000021E2956D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlashWin.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// FlashWin dialog

class FlashWin : public CDialog
{
// Construction
public:
	FlashWin(CWnd* pParent = NULL);   // standard constructor

// attribute
public:
	int	m_nIcon;

// Dialog Data
	//{{AFX_DATA(FlashWin)
	enum { IDD = IDD_DIALOGFLASH };
	CStatic	m_csTitle;
	//}}AFX_DATA

	void SetMessage(CString strMsg);
	void StartTimer();
	void StopTimer();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FlashWin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(FlashWin)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLASHWIN_H__52744201_F9B3_11D4_922F_000021E2956D__INCLUDED_)
