#ifndef __DuiVisionDesignerDOC_H__
#define __DuiVisionDesignerDOC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DuiVisionDesignerDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDuiVisionDesignerDoc document

class CDuiVisionDesignerDoc : public CDocument
{
	DECLARE_DYNCREATE(CDuiVisionDesignerDoc)

// Attributes
public:
	CDuiVisionDesignerDoc();
	virtual ~CDuiVisionDesignerDoc();

	CMenu* m_pMenu;

// Operations
public:
	void RefreshDocProperty();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDuiVisionDesignerDoc)
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
	//{{AFX_MSG(CDuiVisionDesignerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DuiVisionDesignerDOC_H__)
