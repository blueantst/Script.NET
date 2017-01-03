#if !defined(AFX_TPSPERLINTERP_H__B84B631B_BC12_4B29_B5C9_E06D9F2419CD__INCLUDED_)
#define AFX_TPSPERLINTERP_H__B84B631B_BC12_4B29_B5C9_E06D9F2419CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsPerlInterp.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTpsPerlInterp dialog

class CTpsPerlInterp : public CDialog
{
// Construction
public:
	CTpsPerlInterp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsPerlInterp)
	enum { IDD = IDD_TPS_PERLINTERP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsPerlInterp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsPerlInterp)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSPERLINTERP_H__B84B631B_BC12_4B29_B5C9_E06D9F2419CD__INCLUDED_)
