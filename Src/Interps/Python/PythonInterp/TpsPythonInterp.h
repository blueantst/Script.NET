#if !defined(AFX_TPSPYTHONINTERP_H__890D9547_BC35_4ED2_BC39_0FE5B1577F9A__INCLUDED_)
#define AFX_TPSPYTHONINTERP_H__890D9547_BC35_4ED2_BC39_0FE5B1577F9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsPythonInterp.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTpsPythonInterp dialog

class CTpsPythonInterp : public CDialog
{
// Construction
public:
	CTpsPythonInterp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsPythonInterp)
	enum { IDD = IDD_TPS_PYTHONINTERP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsPythonInterp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsPythonInterp)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSPYTHONINTERP_H__890D9547_BC35_4ED2_BC39_0FE5B1577F9A__INCLUDED_)
