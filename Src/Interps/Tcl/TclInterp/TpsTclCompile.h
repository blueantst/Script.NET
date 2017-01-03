#if !defined(AFX_TPSTCLCOMPILE_H__2A44BCF8_17D6_4B5F_93A3_010411C2067D__INCLUDED_)
#define AFX_TPSTCLCOMPILE_H__2A44BCF8_17D6_4B5F_93A3_010411C2067D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsTclCompile.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTpsTclCompile dialog

class CTpsTclCompile : public CDialog
{
// Construction
public:
	CTpsTclCompile(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsTclCompile)
	enum { IDD = IDD_TPS_TCL_COMPILE };
	BOOL	m_bAutoOutFile;
	BOOL	m_bForceOverwrite;
	BOOL	m_bAddPrefix;
	//}}AFX_DATA

	virtual void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsTclCompile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsTclCompile)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSTCLCOMPILE_H__2A44BCF8_17D6_4B5F_93A3_010411C2067D__INCLUDED_)
