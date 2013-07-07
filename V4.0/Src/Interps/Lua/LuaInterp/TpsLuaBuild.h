#if !defined(AFX_TPSLUABUILD_H__CC6C4D1F_9D84_4C10_AF29_25F9FB05E924__INCLUDED_)
#define AFX_TPSLUABUILD_H__CC6C4D1F_9D84_4C10_AF29_25F9FB05E924__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsLuaBuild.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CTpsLuaBuild dialog

class CTpsLuaBuild : public CDialog
{
// Construction
public:
	CTpsLuaBuild(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsLuaBuild)
	enum { IDD = IDD_TPS_LUA_BUILD };
	BOOL	m_bAutoOutFile;
	BOOL	m_bAddLuaLib;
	BOOL	m_bAddSourcePathFiles;
	BOOL	m_bAddSourceLib;
	BOOL	m_bWxScriptProcess;
	BOOL	m_bReplaceIcon;
	int		m_nSelectIconType;
	//}}AFX_DATA

	virtual void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsLuaBuild)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsLuaBuild)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckBuildReplaceIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSLUABUILD_H__CC6C4D1F_9D84_4C10_AF29_25F9FB05E924__INCLUDED_)
