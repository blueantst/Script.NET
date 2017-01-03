// [!output PROJECT_NAME]View.h : interface of the C[!output PROJECT_NAME]View class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __[!output PROJECT_NAME]VIEW_H__
#define __[!output PROJECT_NAME]VIEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// C[!output PROJECT_NAME]View view
class C[!output PROJECT_NAME]Doc;
[!if OWM_TABVIEW]
class C[!output PROJECT_NAME]View : public CXTTabView
[!else]
class C[!output PROJECT_NAME]View : public [!output VIEW_BASE_CLASS]
[!endif]
{
protected: // create from serialization only
	C[!output PROJECT_NAME]View();
	DECLARE_DYNCREATE(C[!output PROJECT_NAME]View)
[!if CFormView]

public:
	//{{AFX_DATA(C[!output PROJECT_NAME]View)
	enum{ IDD = IDD_[!output PROJECT_NAME]_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
[!endif]

// Attributes
public:
	C[!output PROJECT_NAME]Doc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C[!output PROJECT_NAME]View)
	public:
[!if !CRichEditView]
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
[!endif]
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
[!if CFormView]
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
[!endif] //CFormView
[!if CScrollView || CTreeView || CListView || CFormView || CHtmlView]
	virtual void OnInitialUpdate(); // called first time after construct
[!endif]
[!if !CHtmlView]
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
[!endif]
[!if CRichEditView || CHtmlView]
[!else]
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
[!endif] //!CRichEditView||CHtmlView
[!if CFormView]
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
[!endif] //CFormView
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~C[!output PROJECT_NAME]View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(C[!output PROJECT_NAME]View)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
[!if !CHtmlView]
	afx_msg void OnFilePrintPreview();
[!endif]
[!if OWM_MENU]
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
[!endif]
[!if OWM_TABVIEW]
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
[!endif]
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in [!output PROJECT_NAME]View.cpp
inline C[!output PROJECT_NAME]Doc* C[!output PROJECT_NAME]View::GetDocument()
   { return (C[!output PROJECT_NAME]Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !defined(__[!output PROJECT_NAME]VIEW_H__)
