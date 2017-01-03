// StylerDoc.h : interface of the CStylerDoc class
//


#pragma once

class CStylerDoc : public CDocument
{
protected: // create from serialization only
	CStylerDoc();
	DECLARE_DYNCREATE(CStylerDoc)

// Attributes
public:

// Operations
public:
	void RefreshDocProperty();

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CStylerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CString GetCurrentUrl() { return m_strUrl;}
	void SetUrl(CString str) {m_strUrl = str;}
 
protected:
	CString m_strUrl;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};


