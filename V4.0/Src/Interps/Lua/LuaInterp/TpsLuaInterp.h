#if !defined(AFX_TPSLUAINTERP_H__B6EF2B88_1CD2_4ABB_A2A3_29E2AC9B3C22__INCLUDED_)
#define AFX_TPSLUAINTERP_H__B6EF2B88_1CD2_4ABB_A2A3_29E2AC9B3C22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsLuaInterp.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTpsLuaInterp dialog

class CTpsLuaInterp : public CDialog
{
// Construction
public:
	CTpsLuaInterp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsLuaInterp)
	enum { IDD = IDD_TPS_LUAINTERP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsLuaInterp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsLuaInterp)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSLUAINTERP_H__B6EF2B88_1CD2_4ABB_A2A3_29E2AC9B3C22__INCLUDED_)
