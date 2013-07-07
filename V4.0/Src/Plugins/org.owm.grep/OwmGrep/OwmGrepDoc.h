#ifndef __OWMGREPDOC_H__
#define __OWMGREPDOC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OwmGrepDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COwmGrepDoc document

class COwmGrepDoc : public CDocument
{
	DECLARE_DYNCREATE(COwmGrepDoc)

// Attributes
public:
	COwmGrepDoc();
	virtual ~COwmGrepDoc();

	CMenu* m_pMenu;

// Operations
public:
	void RefreshDocProperty();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COwmGrepDoc)
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
	//{{AFX_MSG(COwmGrepDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__OWMGREPDOC_H__)
