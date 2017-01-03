#ifndef __WORKSPACEDOC_H__
#define __WORKSPACEDOC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorkSpaceDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceDoc document

class CWorkSpaceDoc : public CDocument
{
	DECLARE_DYNCREATE(CWorkSpaceDoc)

// Attributes
public:
	CWorkSpaceDoc();
	virtual ~CWorkSpaceDoc();

	CMenu* m_pMenu;

// Operations
public:
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);
	virtual BOOL DoFileSave();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkSpaceDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void SetTitle(LPCTSTR lpszTitle);
	virtual void OnCloseDocument();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL OnNewDocument();
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CWorkSpaceDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__WORKSPACEDOC_H__)
