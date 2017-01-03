// BlueUIDoc.h : interface of the CBlueUIDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLUEUIDOC_H__A163731B_935C_4A7C_9D7D_91CCCEEB3491__INCLUDED_)
#define AFX_BLUEUIDOC_H__A163731B_935C_4A7C_9D7D_91CCCEEB3491__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBlueUIDoc : public CDocument
{
protected: // create from serialization only
	CBlueUIDoc();
	DECLARE_DYNCREATE(CBlueUIDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlueUIDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBlueUIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBlueUIDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLUEUIDOC_H__A163731B_935C_4A7C_9D7D_91CCCEEB3491__INCLUDED_)
