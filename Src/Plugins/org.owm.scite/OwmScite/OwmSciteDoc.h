#ifndef __OWMSCITEDOC_H__
#define __OWMSCITEDOC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OwmSciteDoc.h : header file
//

const int blockSize = 128 * 1024;

/////////////////////////////////////////////////////////////////////////////
// COwmSciteDoc document

class COwmSciteDoc : public CDocument, public CFileEvent
{
	DECLARE_DYNCREATE(COwmSciteDoc)

// Attributes
public:
	COwmSciteDoc();
	virtual ~COwmSciteDoc();

	CMenu* m_pMenu;

protected:
	// The file change event procedure, a call back
	virtual void OnFileEvent(WPARAM wEvent, LPCTSTR pszPathName);

// Operations
public:
	void RefreshDocProperty();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COwmSciteDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void SetTitle(LPCTSTR lpszTitle);
	virtual void OnCloseDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void SetModifiedFlag(BOOL bModified = TRUE);
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
	//{{AFX_MSG(COwmSciteDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__OWMSCITEDOC_H__)
