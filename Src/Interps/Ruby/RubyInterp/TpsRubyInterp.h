#if !defined(AFX_TPSRUBYINTERP_H__6A58265C_2CD0_409B_BD09_3EC77C584342__INCLUDED_)
#define AFX_TPSRUBYINTERP_H__6A58265C_2CD0_409B_BD09_3EC77C584342__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsRubyInterp.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTpsRubyInterp dialog

class CTpsRubyInterp : public CDialog
{
// Construction
public:
	CTpsRubyInterp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsRubyInterp)
	enum { IDD = IDD_TPS_RUBYINTERP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsRubyInterp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsRubyInterp)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSRUBYINTERP_H__6A58265C_2CD0_409B_BD09_3EC77C584342__INCLUDED_)
