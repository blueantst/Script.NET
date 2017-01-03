// OwmGrepView.h : interface of the COwmGrepView class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __OWMGREPVIEW_H__
#define __OWMGREPVIEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGrepRecordPreview : public CXTPReportRecordItemPreview
{
public:
	CGrepRecordPreview(CString strFindLine, CString strReplaceLine)
		:CXTPReportRecordItemPreview(0)
	{
		m_strFindLine = strFindLine;
		m_strReplaceLine = strReplaceLine;

		m_bChanged = strFindLine != strReplaceLine;

		m_strFindLine.Replace(_T("\t"), _T(" "));
		m_strReplaceLine.Replace(_T("\t"), _T(" "));

	}
	CString m_strFindLine;
	CString m_strReplaceLine;
	BOOL m_bChanged;

protected:
	int GetPreviewHeight(CDC* pDC, CXTPReportRow* pRow, int nWidth)
	{
		CXTPReportControl* pControl = pRow->GetControl();

		int nIndentWidth = pControl->GetHeaderIndent();
		CRect& rcIndent = pControl->GetPaintManager()->m_rcPreviewIndent;
		CRect rcPreviewItem(nIndentWidth + rcIndent.left, 0, nWidth - rcIndent.right, 0);


		CXTPFontDC font(pDC, &pControl->GetPaintManager()->m_fontPreview);

		CRect rcFind(rcPreviewItem.left, 0, rcPreviewItem.right, 0);
		pDC->DrawText(m_strFindLine, rcFind, DT_WORDBREAK|DT_CALCRECT|DT_NOPREFIX);

		BOOL bShowReplaceLine = AfxGetApp()->GetProfileInt(REG_GREP, REG_SHOW_REPLACE_LINE, FALSE);
		if(bShowReplaceLine)
		{
			CRect rcReplace(rcPreviewItem.left, 0, rcPreviewItem.right, 0);
			pDC->DrawText(m_strReplaceLine, rcReplace, DT_WORDBREAK|DT_CALCRECT|DT_NOPREFIX);


			return rcFind.Height() + rcReplace.Height() + rcIndent.top + rcIndent.bottom;
		}

		return rcFind.Height() + rcIndent.top + rcIndent.bottom;
	}

	void OnDrawCaption(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* /*pMetrics*/)
	{

		CRect rcItem(pDrawArgs->rcItem);
		CDC* pDC = pDrawArgs->pDC;

		CRect& rcIndent = pDrawArgs->pControl->GetPaintManager()->m_rcPreviewIndent;
		rcItem.DeflateRect(rcIndent.left, rcIndent.top, rcIndent.right, rcIndent.bottom);

		CRect rcFind(rcItem);
		pDC->DrawText(m_strFindLine, rcFind, DT_WORDBREAK|DT_LEFT|DT_NOPREFIX);
		rcFind.bottom = rcFind.top;

		pDC->DrawText(m_strFindLine, rcFind, DT_WORDBREAK|DT_LEFT|DT_NOPREFIX|DT_CALCRECT);

		BOOL bShowReplaceLine = AfxGetApp()->GetProfileInt(REG_GREP, REG_SHOW_REPLACE_LINE, FALSE);
		if(bShowReplaceLine)
		{
			CRect rcReplace(rcItem);
			rcReplace.top += rcFind.Height();
			if (m_bChanged) if (pDC->GetTextColor() == 0xFF0000) pDC->SetTextColor(0xFF);
			pDC->DrawText(m_strReplaceLine, rcReplace, DT_WORDBREAK|DT_LEFT|DT_NOPREFIX);
		}
	}
};

class CGrepRecord : public CXTPReportRecord
{
public:
	CGrepRecord(CString strPath, CString strDirectory, CString strName,
		CString strFind, long nLine, CString strFindLine, CString strReplaceLine, long nIndex, long nLength, CString strReplace )
	{
		BOOL bShowReplaceLine = AfxGetApp()->GetProfileInt(REG_GREP, REG_SHOW_REPLACE_LINE, FALSE);

		CXTPReportRecordItem* pItem = AddItem(new CXTPReportRecordItemText(strPath));//strName));
		pItem->SetBackgroundColor(RGB(255,225,255));
		//pItem = AddItem(new CXTPReportRecordItemText(strDirectory));
		//pItem->SetBackgroundColor(RGB(255,225,255));

		pItem = AddItem(new CXTPReportRecordItemNumber(nLine));
		pItem->SetBackgroundColor(RGB(255,225,255));

		if(bShowReplaceLine)
		{
			pItem = AddItem(new CXTPReportRecordItemText(strFind));
			pItem->SetBackgroundColor(RGB(255,225,255));
		}else
		{
			pItem = AddItem(new CXTPReportRecordItemText(strFindLine));
			pItem->SetBackgroundColor(RGB(225,255,255));
			pItem->SetTextColor(RGB(0,0,255));
		}

		pItem = AddItem(new CXTPReportRecordItem());
		pItem->SetBackgroundColor(RGB(255,225,255));
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(strFindLine != strReplaceLine);
		pItem->SetSortPriority(strFindLine != strReplaceLine? 1: 0);
		pItem->SetGroupPriority(strFindLine != strReplaceLine? 1: 0);

		/*
		CString strExt;
		int nExt = strName.ReverseFind(_T('.'));
		if (nExt > 0) strExt = strName.Mid(nExt + 1);

		pItem = AddItem(new CXTPReportRecordItemText(strExt));
		pItem->SetBackgroundColor(RGB(255,225,255));
		*/

		if(bShowReplaceLine)
		{
			CXTPReportRecordItemPreview* pItemPreview = new CGrepRecordPreview(strFindLine, strReplaceLine);
			SetPreviewItem(pItemPreview);
			pItemPreview->SetBackgroundColor(RGB(225,255,255));
		}

		m_strPath = strPath;
		m_nIndex = nIndex;
		m_nLength = nLength;
		m_nLineNo = nLine;
		m_strReplace = strReplace;
	}
	BOOL IsChecked()
	{
		return GetItem(4)->IsChecked();
	}
	void SetChecked(BOOL bChecked)
	{
		GetItem(4)->SetChecked(bChecked);
	}
	BOOL IsChanged()
	{
		return ((CGrepRecordPreview*)GetItemPreview())->m_bChanged;
	}

public:
	CString m_strPath;
	int m_nIndex;
	int m_nLength;
	int m_nLineNo;
	CString m_strReplace;
};

/////////////////////////////////////////////////////////////////////////////
// COwmGrepView view
class COwmGrepDoc;
class COwmGrepView : public CXTPReportView
{
protected: // create from serialization only
	COwmGrepView();
	DECLARE_DYNCREATE(COwmGrepView)

// Attributes
public:
	COwmGrepDoc* GetDocument();

// Operations
public:
	void CheckSelected(BOOL bChecked);

	void AddRecord(LPCTSTR lpszPath, int nLine, LPCTSTR lpszLine);
	void ClearRecord();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COwmGrepView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COwmGrepView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void OnReportItemDblClick(NMHDR * pNotifyStruct, LRESULT * /*result*/);
	void OnReportKeyDown(NMHDR * pNotifyStruct, LRESULT * /*result*/);
	void OnReportLButtonDown(NMHDR * pNotifyStruct, LRESULT * /*result*/);

// Generated message map functions
protected:
	//{{AFX_MSG(COwmGrepView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnFilePrintPreview();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in OwmGrepView.cpp
inline COwmGrepDoc* COwmGrepView::GetDocument()
   { return (COwmGrepDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__OWMGREPVIEW_H__)
