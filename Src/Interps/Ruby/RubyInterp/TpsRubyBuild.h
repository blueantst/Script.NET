#if !defined(AFX_TPSRUBYBUILD_H__5F5F6168_1A73_4058_B5C8_B5AFFC116B50__INCLUDED_)
#define AFX_TPSRUBYBUILD_H__5F5F6168_1A73_4058_B5C8_B5AFFC116B50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsRubyBuild.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CTpsRubyBuild dialog

class CTpsRubyBuild : public CDialog
{
// Construction
public:
	CTpsRubyBuild(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsRubyBuild)
	enum { IDD = IDD_TPS_RUBY_BUILD };
	BOOL	m_bAutoOutFile;
	BOOL	m_bAddRubyLib;
	BOOL	m_bAddSourcePathFiles;
	BOOL	m_bSelectBuildTool;
	BOOL	m_bSelectType;
	BOOL	m_bReplaceIcon;
	int		m_nSelectIconType;
	//}}AFX_DATA

	virtual void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsRubyBuild)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsRubyBuild)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckBuildReplaceIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSRUBYBUILD_H__5F5F6168_1A73_4058_B5C8_B5AFFC116B50__INCLUDED_)
