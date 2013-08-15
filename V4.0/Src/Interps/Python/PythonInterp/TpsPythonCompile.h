#if !defined(AFX_TPSPYTHONCOMPILE_H__EF83A75F_BF9A_42DA_A9E9_56ECE6E1BE87__INCLUDED_)
#define AFX_TPSPYTHONCOMPILE_H__EF83A75F_BF9A_42DA_A9E9_56ECE6E1BE87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsPythonCompile.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTpsPythonCompile dialog

class CTpsPythonCompile : public CDialog
{
// Construction
public:
	CTpsPythonCompile(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsPythonCompile)
	enum { IDD = IDD_TPS_PYTHON_COMPILE };
	BOOL	m_bAutoOutFile;
	BOOL	m_bForceOverwrite;
	//}}AFX_DATA

	virtual void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsPythonCompile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsPythonCompile)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSPYTHONCOMPILE_H__EF83A75F_BF9A_42DA_A9E9_56ECE6E1BE87__INCLUDED_)
