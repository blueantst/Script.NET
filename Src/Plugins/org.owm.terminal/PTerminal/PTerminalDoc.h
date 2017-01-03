#ifndef __PTERMINALDOC_H__
#define __PTERMINALDOC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTerminalDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPTerminalDoc document

class CPTerminalDoc : public CDocument
{
	DECLARE_DYNCREATE(CPTerminalDoc)

// Attributes
public:
	CPTerminalDoc();
	virtual ~CPTerminalDoc();

// Attributes
public:

// Functions
public:
	void SetWorkShell(int nShell, BOOL bSwitch = FALSE);

	void RefreshDocProperty();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTerminalDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void SetTitle(LPCTSTR lpszTitle);
	virtual void OnCloseDocument();
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CPTerminalDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__PTERMINALDOC_H__)
