// HtmlViewOWMView.h : interface of the CHtmlViewOWMView class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __HTMLVIEWOWMVIEW_H__
#define __HTMLVIEWOWMVIEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mshtmhst.h"

/////////////////////////////////////////////////////////////////////////////
// CHtmlViewOWMView view
class CHtmlViewOWMDoc;
class CHtmlViewOWMView : public CXTHtmlView
{
protected: // create from serialization only
	CHtmlViewOWMView();
	DECLARE_DYNCREATE(CHtmlViewOWMView)

// Attributes
public:
	CHtmlViewOWMDoc* GetDocument();

	char	m_szOldPath[256];
	DWORD	m_dwPathLen;

	void*	m_pUI;	// 扩展UI处理器对象
	IDocHostUIHandler* m_IExUIHandler;	// 扩展UI处理器接口

// Operations
public:
	void SaveAndSetCurPath(CString strURL);
	void RestoreCurPath();

	IHTMLDocument2Ptr GetHtmlFrameWindowDOM(LPCTSTR lpszFrameId);
	IHTMLElementPtr GetHtmlElement(LPCTSTR lpszId);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHtmlViewOWMView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnTitleChange(LPCTSTR lpszText);
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	virtual void OnDocumentComplete(LPCTSTR lpszURL);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHtmlViewOWMView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CHtmlViewOWMView)
	afx_msg void OnGoBack();
	afx_msg void OnUpdateGoBack(CCmdUI* pCmdUI);
	afx_msg void OnGoForward();
	afx_msg void OnUpdateGoForward(CCmdUI* pCmdUI);
	afx_msg void OnViewStop();
	afx_msg void OnUpdateViewStop(CCmdUI* pCmdUI);
	afx_msg void OnViewRefresh();
	afx_msg void OnUpdateViewRefresh(CCmdUI* pCmdUI);
	afx_msg void OnGoStartPage();
	afx_msg void OnUpdateGoStartPage(CCmdUI* pCmdUI);
	afx_msg void OnGoSearchTheWeb();
	afx_msg void OnUpdateGoSearchTheWeb(CCmdUI* pCmdUI);
	afx_msg void OnViewFolders();
	afx_msg void OnUpdateViewFolders(CCmdUI* pCmdUI);
	afx_msg void OnViewHistory();
	afx_msg void OnUpdateViewHistory(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnFilePrintPreview();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditUndo();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in HtmlViewOWMView.cpp
inline CHtmlViewOWMDoc* CHtmlViewOWMView::GetDocument()
   { return (CHtmlViewOWMDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__HTMLVIEWOWMVIEW_H__)
