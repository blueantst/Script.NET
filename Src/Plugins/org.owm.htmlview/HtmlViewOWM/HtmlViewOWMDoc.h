#ifndef __HTMLVIEWOWMDOC_H__
#define __HTMLVIEWOWMDOC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HtmlViewOWMDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHtmlViewOWMDoc document

class CHtmlViewOWMDoc : public CDocument
{
	DECLARE_DYNCREATE(CHtmlViewOWMDoc)

// Attributes
public:
	CHtmlViewOWMDoc();
	virtual ~CHtmlViewOWMDoc();

	CMenu* m_pMenu;

	CString m_strCurURL;	// ±£´æµ±Ç°URL

// Operations
public:
	void RefreshDocProperty();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHtmlViewOWMDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void SetTitle(LPCTSTR lpszTitle);
	virtual void OnCloseDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
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
	//{{AFX_MSG(CHtmlViewOWMDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__HTMLVIEWOWMDOC_H__)
