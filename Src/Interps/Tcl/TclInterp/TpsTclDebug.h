#if !defined(AFX_TPSTCLDEBUG_H__E8BEE4F6_E891_4F5B_BF32_058295C16EBC__INCLUDED_)
#define AFX_TPSTCLDEBUG_H__E8BEE4F6_E891_4F5B_BF32_058295C16EBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsTclDebug.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTpsTclDebug dialog

class CTpsTclDebug : public CDialog
{
// Construction
public:
	CTpsTclDebug(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsTclDebug)
	enum { IDD = IDD_TPS_TCL_DEBUG };
	BOOL	m_bEnableProfileAnalysis;
	BOOL	m_bEnableMulitFileDebug;
	BOOL	m_bMultiDebugQuery;
	BOOL	m_bMultiDebugExcludeLib;
	//}}AFX_DATA

	virtual void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsTclDebug)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsTclDebug)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckEnableMulitfiledebug();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSTCLDEBUG_H__E8BEE4F6_E891_4F5B_BF32_058295C16EBC__INCLUDED_)
