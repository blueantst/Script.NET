// DuiVisionDesignerBar.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __DuiVisionDesignerBAR_H__
#define __DuiVisionDesignerBAR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "DuiVisionDesignerView.h"
/////////////////////////////////////////////////////////////////////////////
// CDuiVisionDesignerBar class
class CDuiVisionDesignerBar : public CXTDockWindow
{
protected:
	static CRuntimeClass* PASCAL _GetBaseClass();
public:
	static const AFX_DATA CRuntimeClass classCDuiVisionDesignerBar;
	virtual CRuntimeClass* GetRuntimeClass() const;
	static CObject* PASCAL CreateObject();

	
// Construction
public:
	CDuiVisionDesignerBar();
	virtual ~CDuiVisionDesignerBar();

// Attributes
public:
	CDuiVisionDesignerView	m_DuiVisionDesignerView;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDuiVisionDesignerBar)
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CDuiVisionDesignerBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __DuiVisionDesignerBAR_H__

