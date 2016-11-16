// DuiVisionDesignerView.h : interface of the CDuiVisionDesignerView class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __DuiVisionDesignerVIEW_H__
#define __DuiVisionDesignerVIEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "timer.h"

/////////////////////////////////////////////////////////////////////////////
// CDuiVisionDesignerView view
class CDuiVisionDesignerDoc;
class CDuiVisionDesignerView : public CView, public CTimer
{
protected: // create from serialization only
	CDuiVisionDesignerView();
	DECLARE_DYNCREATE(CDuiVisionDesignerView)

// Attributes
public:
	CDuiVisionDesignerDoc* GetDocument();

	IDuiPluginPanel* m_pDuiPluginPanelObject; // DUI界面插件Panel对象

	UINT			m_uTimerAnimation;			// 动画定时器
	UINT			m_uTimerAutoClose;			// 用于窗口自动关闭的定时器ID

// Operations
public:
	// 定时器消息
	virtual void OnTimer(UINT uTimerID);
	virtual void OnTimer(UINT uTimerID, CString strTimerName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDuiVisionDesignerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
 //CFormView
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
 //!CRichEditView||CHtmlView
 //CFormView
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDuiVisionDesignerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDuiVisionDesignerView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnFilePrintPreview();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#ifndef _DEBUG  // debug version in DuiVisionDesignerView.cpp
inline CDuiVisionDesignerDoc* CDuiVisionDesignerView::GetDocument()
   { return (CDuiVisionDesignerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !defined(__DuiVisionDesignerVIEW_H__)
