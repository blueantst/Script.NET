#if !defined(AFX_TPSTCLBUILD_H__88EC7A5C_71EA_4DDF_BBBF_CF8555750943__INCLUDED_)
#define AFX_TPSTCLBUILD_H__88EC7A5C_71EA_4DDF_BBBF_CF8555750943__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsTclBuild.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTpsTclBuild dialog

class CTpsTclBuild : public CDialog
{
// Construction
public:
	CTpsTclBuild(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsTclBuild)
	enum { IDD = IDD_TPS_TCL_BUILD };
	BOOL	m_bAutoOutFile;
	BOOL	m_bAddTclLib;
	BOOL	m_bAddSourcePathFiles;
	BOOL	m_bAddSourceLib;
	BOOL	m_bForceTkKit;
	BOOL	m_bReplaceIcon;
	int		m_nSelectIconType;
	//}}AFX_DATA

	virtual void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsTclBuild)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsTclBuild)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckBuildReplaceIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSTCLBUILD_H__88EC7A5C_71EA_4DDF_BBBF_CF8555750943__INCLUDED_)
