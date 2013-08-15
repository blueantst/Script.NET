#if !defined(AFX_TPSPYTHONBUILD_H__BCBD9C68_AE0F_420D_9CC3_EAA40A3530F4__INCLUDED_)
#define AFX_TPSPYTHONBUILD_H__BCBD9C68_AE0F_420D_9CC3_EAA40A3530F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsPythonBuild.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTpsPythonBuild dialog

class CTpsPythonBuild : public CDialog
{
// Construction
public:
	CTpsPythonBuild(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsPythonBuild)
	enum { IDD = IDD_TPS_PYTHON_BUILD };
	BOOL	m_bAutoOutFile;
	BOOL	m_bAddPythonLib;
	BOOL	m_bAddSourcePathFiles;
	BOOL	m_bAddSourceData;
	BOOL	m_bSelectType;
	BOOL	m_bReplaceIcon;
	int		m_nSelectIconType;
	//}}AFX_DATA

	virtual void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsPythonBuild)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsPythonBuild)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckBuildReplaceIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSPYTHONBUILD_H__BCBD9C68_AE0F_420D_9CC3_EAA40A3530F4__INCLUDED_)
