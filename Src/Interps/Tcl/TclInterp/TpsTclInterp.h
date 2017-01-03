#if !defined(AFX_TPSTCLINTERP_H__55A3602B_209D_4254_BD2E_50E185958620__INCLUDED_)
#define AFX_TPSTCLINTERP_H__55A3602B_209D_4254_BD2E_50E185958620__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsTclInterp.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTpsTclInterp dialog

class CTpsTclInterp : public CDialog
{
// Construction
public:
	CTpsTclInterp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsTclInterp)
	enum { IDD = IDD_TPS_TCLINTERP };
	BOOL	m_bLoadLibTk;
	BOOL	m_bLoadLibITcl;
	BOOL	m_bLoadLibITk;
	//}}AFX_DATA

	virtual void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsTclInterp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsTclInterp)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSTCLINTERP_H__55A3602B_209D_4254_BD2E_50E185958620__INCLUDED_)
