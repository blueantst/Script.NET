// [!output PROJECT_NAME]View.cpp : implementation of the C[!output PROJECT_NAME]View class
//

#include "stdafx.h"
#include "[!output PROJECT_NAME].h"
#include "[!output PROJECT_NAME]Doc.h"
#include "[!output PROJECT_NAME]View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C[!output PROJECT_NAME]View

[!if OWM_TABVIEW]
IMPLEMENT_DYNCREATE(C[!output PROJECT_NAME]View, CXTTabView)
[!else]
IMPLEMENT_DYNCREATE(C[!output PROJECT_NAME]View, [!output VIEW_BASE_CLASS])
[!endif]

[!if OWM_TABVIEW]
BEGIN_MESSAGE_MAP(C[!output PROJECT_NAME]View, CXTTabView)
[!else]
BEGIN_MESSAGE_MAP(C[!output PROJECT_NAME]View, CView)
[!endif]
	//{{AFX_MSG_MAP(C[!output PROJECT_NAME]View)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
[!if OWM_MENU]
	ON_WM_CONTEXTMENU()
[!endif]
[!if OWM_TABVIEW]
	ON_WM_CREATE()
[!endif]
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, [!output VIEW_BASE_CLASS]::OnFilePrint)
[!if !CHtmlView]
	ON_COMMAND(ID_FILE_PRINT_DIRECT, [!output VIEW_BASE_CLASS]::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
[!endif]
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C[!output PROJECT_NAME]View construction/destruction

C[!output PROJECT_NAME]View::C[!output PROJECT_NAME]View()
[!if CFormView]
	: [!output VIEW_BASE_CLASS](C[!output PROJECT_NAME]View::IDD)
[!endif]
{
[!if CFormView]
	//{{AFX_DATA_INIT(C[!output PROJECT_NAME]View)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
[!endif]
	// TODO: add construction code here

}

C[!output PROJECT_NAME]View::~C[!output PROJECT_NAME]View()
{
}
[!if CFormView]

void C[!output PROJECT_NAME]View::DoDataExchange(CDataExchange* pDX)
{
	[!output VIEW_BASE_CLASS]::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C[!output PROJECT_NAME]View)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
[!endif]

BOOL C[!output PROJECT_NAME]View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

[!if CEditView]
	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
[!else]
	return [!output VIEW_BASE_CLASS]::PreCreateWindow(cs);
[!endif]
}
[!if !CRichEditView]

/////////////////////////////////////////////////////////////////////////////
// C[!output PROJECT_NAME]View drawing

void C[!output PROJECT_NAME]View::OnDraw(CDC* pDC)
{
	C[!output PROJECT_NAME]Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}
[!endif]
[!if CScrollView || CListView || CTreeView || CFormView || CHtmlView]

void C[!output PROJECT_NAME]View::OnInitialUpdate()
{
	[!output VIEW_BASE_CLASS]::OnInitialUpdate();
[!if CFormView]
	ResizeParentToFit();
[!endif]

[!if CListView]

	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
[!endif]
[!if CTreeView]

	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().
[!endif]
[!if CHtmlView]
	// TODO: This code navigates to a popular spot on the web.
	//  change the code to go where you'd like.
[!endif]
[!if CHtmlView]
	Navigate2(_T("http://www.blueantstudio.net"),NULL,NULL);
[!endif]
[!if CScrollView]
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
[!endif]
}
[!endif]

/////////////////////////////////////////////////////////////////////////////
// C[!output PROJECT_NAME]View printing

[!if !CHtmlView]
void C[!output PROJECT_NAME]View::OnFilePrintPreview() 
{
	//DoPrintPreview (this);
}

BOOL C[!output PROJECT_NAME]View::OnPreparePrinting(CPrintInfo* pInfo)
{
[!if CEditView]
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
[!else]
	return TRUE;
[!endif]
}

[!if !CRichEditView]
[!if CEditView]
void C[!output PROJECT_NAME]View::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
[!else]
void C[!output PROJECT_NAME]View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
[!endif]
{
[!if CEditView]
	// Default CEditView begin printing.
	CEditView::OnBeginPrinting(pDC, pInfo);
[!else]
	// TODO: add extra initialization before printing
[!endif]
}

[!if CEditView]
void C[!output PROJECT_NAME]View::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
[!else]
void C[!output PROJECT_NAME]View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
[!endif]
{
[!if CEditView]
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
[!else]
	// TODO: add cleanup after printing
[!endif]
}
[!endif]
[!endif]
[!if CFormView]

void C[!output PROJECT_NAME]View::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}
[!endif]

/////////////////////////////////////////////////////////////////////////////
// C[!output PROJECT_NAME]View diagnostics

#ifdef _DEBUG
void C[!output PROJECT_NAME]View::AssertValid() const
{
[!if OWM_TABVIEW]
	CXTTabView::AssertValid();
[!else]
	[!output VIEW_BASE_CLASS]::AssertValid();
[!endif]
}

void C[!output PROJECT_NAME]View::Dump(CDumpContext& dc) const
{
[!if OWM_TABVIEW]
	CXTTabView::Dump(dc);
[!else]
	[!output VIEW_BASE_CLASS]::Dump(dc);
[!endif]
}

C[!output PROJECT_NAME]Doc* C[!output PROJECT_NAME]View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(C[!output PROJECT_NAME]Doc)));
	return (C[!output PROJECT_NAME]Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// C[!output PROJECT_NAME]View message handlers
[!if OWM_MENU]
void C[!output PROJECT_NAME]View::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	get_dll_resource();
	CMenu* pMenu = new CMenu();
	pMenu->LoadMenu(IDR_OWM);
	reset_dll_resource();
	CMenu* pmenu = (CMenu*)pMenu->GetSubMenu(0);
	pmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
			point.x, point.y, this);
	delete pMenu;
}
[!endif]

[!if OWM_TABVIEW]
int C[!output PROJECT_NAME]View::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTTabView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 添加TabView中的视,可以用AddView
	//AddView("ViewName", RUNTIME_CLASS(CxxxView), pDoc);
	
	return 0;
}
[!endif]

//////////////////////////////////////////////////////////////////////////////
// 窗口切换时调用
void C[!output PROJECT_NAME]View::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if (bActivate)
	{
		// 在属性窗口显示图像信息
		GetDocument()->RefreshDocProperty();
	}
/*
	if (bActivate)
	{
		// 窗口激活时显示OWM菜单
		get_dll_resource();
		CMenu* pMenuTerm = new CMenu();
		pMenuTerm->LoadMenu(IDR_MENU_[!output PROJECT_NAME]);
		reset_dll_resource();
		theApp.m_pIPlatUI->AddExtMenu(pMenuTerm);
	}else
	{
		// 窗口去激活时删除 OWM菜单
		get_dll_resource();
		CMenu* pMenuTerm = new CMenu();
		pMenuTerm->LoadMenu(IDR_MENU_[!output PROJECT_NAME]);
		reset_dll_resource();
		theApp.m_pIPlatUI->DelExtMenu(pMenuTerm);
	}
*/	
	[!output VIEW_BASE_CLASS]::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
