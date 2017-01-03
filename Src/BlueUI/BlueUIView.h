// BlueUIView.h : interface of the CBlueUIView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLUEUIVIEW_H__1E68A083_39B0_4619_8FE7_62F218831E50__INCLUDED_)
#define AFX_BLUEUIVIEW_H__1E68A083_39B0_4619_8FE7_62F218831E50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBlueUIView : public CView
{
protected: // create from serialization only
	CBlueUIView();
	DECLARE_DYNCREATE(CBlueUIView)

// Attributes
public:
	CBlueUIDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlueUIView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBlueUIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBlueUIView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BlueUIView.cpp
inline CBlueUIDoc* CBlueUIView::GetDocument()
   { return (CBlueUIDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLUEUIVIEW_H__1E68A083_39B0_4619_8FE7_62F218831E50__INCLUDED_)
