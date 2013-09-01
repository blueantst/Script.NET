// [!output PROJECT_NAME]Bar.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __[!output PROJECT_NAME]BAR_H__
#define __[!output PROJECT_NAME]BAR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

[!if OWM_CONTROLBAR]
#include "[!output PROJECT_NAME]View.h"
[!endif]
/////////////////////////////////////////////////////////////////////////////
// C[!output PROJECT_NAME]Bar class
[!if OWM_CONTROLBAR]
class C[!output PROJECT_NAME]Bar : public CXTDockWindow
[!else]
class C[!output PROJECT_NAME]Bar : public CXTTabCtrlBar
[!endif]
{
protected:
	static CRuntimeClass* PASCAL _GetBaseClass();
public:
	static const AFX_DATA CRuntimeClass classC[!output PROJECT_NAME]Bar;
	virtual CRuntimeClass* GetRuntimeClass() const;
	static CObject* PASCAL CreateObject();

	
// Construction
public:
	C[!output PROJECT_NAME]Bar();
	virtual ~C[!output PROJECT_NAME]Bar();

// Attributes
public:
[!if OWM_CONTROLBAR]
	C[!output PROJECT_NAME]View	m_[!output PROJECT_NAME]View;
[!endif]

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C[!output PROJECT_NAME]Bar)
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(C[!output PROJECT_NAME]Bar)
[!if OWM_CONTROLBAR]
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
[!endif]
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __[!output PROJECT_NAME]BAR_H__

