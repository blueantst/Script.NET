// OwmGrepView.cpp : implementation of the COwmGrepView class
//

#include "stdafx.h"
#include "OwmGrep.h"
#include "OwmGrepDoc.h"
#include "OwmGrepView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COwmGrepView

IMPLEMENT_DYNCREATE(COwmGrepView, CXTPReportView)

BEGIN_MESSAGE_MAP(COwmGrepView, CXTPReportView)
	//{{AFX_MSG_MAP(COwmGrepView)
	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP

	ON_NOTIFY(NM_DBLCLK, XTP_ID_REPORT_CONTROL, OnReportItemDblClick)
	ON_NOTIFY(NM_KEYDOWN, XTP_ID_REPORT_CONTROL, OnReportKeyDown)
	ON_NOTIFY(XTP_NM_REPORT_LBUTTONDOWN, XTP_ID_REPORT_CONTROL, OnReportLButtonDown)

	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COwmGrepView construction/destruction

COwmGrepView::COwmGrepView()
{
	// TODO: add construction code here

}

COwmGrepView::~COwmGrepView()
{
}

BOOL COwmGrepView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	return CXTPReportView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// COwmGrepView drawing

void COwmGrepView::OnDraw(CDC* pDC)
{
	COwmGrepDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// COwmGrepView printing

void COwmGrepView::OnFilePrintPreview() 
{
	//DoPrintPreview (this);
}

BOOL COwmGrepView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return TRUE;
}

void COwmGrepView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COwmGrepView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// COwmGrepView diagnostics

#ifdef _DEBUG
void COwmGrepView::AssertValid() const
{
	CXTPReportView::AssertValid();
}

void COwmGrepView::Dump(CDumpContext& dc) const
{
	CXTPReportView::Dump(dc);
}

COwmGrepDoc* COwmGrepView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COwmGrepDoc)));
	return (COwmGrepDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COwmGrepView message handlers
void COwmGrepView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	get_dll_resource();
	CMenu* pMenu = new CMenu();
	pMenu->LoadMenu(IDR_OWM);
	reset_dll_resource();
	CMenu* pmenu = (CMenu*)pMenu->GetSubMenu(0);
	//CXTPCommandBars::TrackPopupMenu(pmenu, 0, point.x, point.y, this);
	theApp.m_pIPlatUI->TrackPopupMenu(pmenu, 0, point.x, point.y, this);
	delete pMenu;
}

int COwmGrepView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTPReportView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CXTPReportControl& wndReport = GetReportCtrl();

	wndReport.SetGridStyle(FALSE, (XTPReportGridStyle)xtpReportGridSmallDots);
	wndReport.SetGridStyle(TRUE, (XTPReportGridStyle)xtpReportGridSmallDots);

	//GetPaintManager()->m_columnStyle = xtpReportColumnOffice2003;
	m_wndReport.GetPaintManager()->SetColumnStyle(xtpReportColumnOffice2007);

	DLL_LOADSTRING(strColumnFile, IDS_COLUMN_FILE);
	DLL_LOADSTRING(strColumnLine, IDS_COLUMN_LINE);
	DLL_LOADSTRING(strColumnContent, IDS_COLUMN_CONTENT);
	DLL_LOADSTRING(strColumnApply, IDS_COLUMN_APPLY);

	wndReport.AddColumn(new CXTPReportColumn(0, strColumnFile, 60));
	wndReport.AddColumn(new CXTPReportColumn(1, strColumnLine, 20));
	//CXTPReportColumn* pColumnDirectory = wndReport.AddColumn(new CXTPReportColumn(1, _T("Directory"), 100));
	//pColumnDirectory->SetAlignment(DT_LEFT|DT_PATH_ELLIPSIS);

	wndReport.AddColumn(new CXTPReportColumn(2, strColumnContent, 150));
	wndReport.AddColumn(new CXTPReportColumn(3, strColumnApply, 18, FALSE));

	//wndReport.ShowGroupBy();
	wndReport.GetReportHeader()->AllowColumnRemove(FALSE);
	wndReport.EnablePreviewMode(TRUE);
	
	return 0;
}

#define OC_SETCURENTLINE		111	// 设置当前行

void COwmGrepView::OnReportItemDblClick(NMHDR * pNotifyStruct, LRESULT * /*result*/)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (pItemNotify->pRow)
	{
		TRACE(_T("Double Click on row %d\n"),
			pItemNotify->pRow->GetIndex());

		CGrepRecord* pRecord = (CGrepRecord*)pItemNotify->pRow->GetRecord();
		if (pRecord)
		{
			//ShellExecute(0, 0, _T("notepad.exe"), (LPTSTR)(LPCTSTR)pRecord->m_strPath, 0, SW_SHOW);
			theApp.m_pIPlatUI->OpenDocumentFile(pRecord->m_strPath, "org.owm.edit");
			theApp.m_pIPlatUI->SendOwmCmd("org.owm.edit", OC_SETCURENTLINE, (WPARAM)(LPTSTR)(LPCTSTR)pRecord->m_strPath, pRecord->m_nLineNo, NULL);
		}
	}
}

void COwmGrepView::CheckSelected(BOOL bChecked)
{
	CXTPReportSelectedRows* pSelectedRows = GetReportCtrl().GetSelectedRows();
	
	POSITION pos = pSelectedRows->GetFirstSelectedRowPosition();
	while (pos)
	{
		CXTPReportRow* pRow = pSelectedRows->GetNextSelectedRow(pos);
		
		if (pRow->GetRecord())
		{
			((CGrepRecord*)pRow->GetRecord())->SetChecked(bChecked);
		}
	}
	
	GetReportCtrl().RedrawControl();
}

void COwmGrepView::OnReportKeyDown(NMHDR * pNotifyStruct, LRESULT * /*result*/)
{
	LPNMKEY lpNMKey = (LPNMKEY)pNotifyStruct;

	if (lpNMKey->nVKey == VK_SPACE)
	{
		BOOL bChecked = FALSE;

		if (GetReportCtrl().GetFocusedRow() && GetReportCtrl().GetFocusedRow()->GetRecord())
			bChecked = ! ((CGrepRecord*)(GetReportCtrl().GetFocusedRow()->GetRecord()))->IsChecked();

		CXTPReportSelectedRows* pSelectedRows = GetReportCtrl().GetSelectedRows();

		CheckSelected(bChecked);
/*
		POSITION pos = pSelectedRows->GetFirstSelectedRowPosition();
		while (pos)
		{
			CXTPReportRow* pRow = pSelectedRows->GetNextSelectedRow(pos);
			
			if (pRow->GetRecord())
			{
				((CGrepRecord*)pRow->GetRecord())->SetChecked(bChecked);
			}
		}

		m_wndReport.RedrawControl();*/
	}
}

void COwmGrepView::OnReportLButtonDown(NMHDR * pNotifyStruct, LRESULT * result)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (pItemNotify->pRow && pItemNotify->pRow->GetRecord() && pItemNotify->pColumn && pItemNotify->pColumn->GetItemIndex() == 4)
	{
		BOOL bChecked = ! ((CGrepRecord*)(pItemNotify->pRow->GetRecord()))->IsChecked();

		if (pItemNotify->pRow->IsSelected())
		{
			CheckSelected(bChecked);		
			*result = TRUE;
		}
		else
		{
			((CGrepRecord*)pItemNotify->pRow->GetRecord())->SetChecked(bChecked);
		}
	}
}

void COwmGrepView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if (bActivate)
	{
		// 在属性窗口显示图像信息
		//GetDocument()->RefreshDocProperty();
	}

	CXTPReportView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void COwmGrepView::AddRecord(LPCTSTR lpszPath, int nLine, LPCTSTR lpszLine) 
{
	CXTPReportControl& wndReport = GetReportCtrl();

	wndReport.AddRecord(new CGrepRecord
		(lpszPath, "", "", "",
		nLine, lpszLine, "", 0, 0, ""));

	// 把内容显示出来
	wndReport.Populate();
}

void COwmGrepView::ClearRecord() 
{
	CXTPReportControl& wndReport = GetReportCtrl();

	wndReport.GetRecords()->RemoveAll();

	// 把内容显示出来
	wndReport.Populate();
}
