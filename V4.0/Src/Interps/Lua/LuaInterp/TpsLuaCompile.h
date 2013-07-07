#if !defined(AFX_TPSLUACOMPILE_H__457CB1CA_1949_4BA0_8B8F_93AB4CB8505B__INCLUDED_)
#define AFX_TPSLUACOMPILE_H__457CB1CA_1949_4BA0_8B8F_93AB4CB8505B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsLuaCompile.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTpsLuaCompile dialog

class CTpsLuaCompile : public CDialog
{
// Construction
public:
	CTpsLuaCompile(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsLuaCompile)
	enum { IDD = IDD_TPS_LUA_COMPILE };
	BOOL	m_bAutoOutFile;
	BOOL	m_bForceOverwrite;
	//}}AFX_DATA

	virtual void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsLuaCompile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsLuaCompile)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSLUACOMPILE_H__457CB1CA_1949_4BA0_8B8F_93AB4CB8505B__INCLUDED_)
