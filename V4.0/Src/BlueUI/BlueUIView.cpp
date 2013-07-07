// BlueUIView.cpp : implementation of the CBlueUIView class
//

#include "stdafx.h"
#include "BlueUI.h"

#include "BlueUIDoc.h"
#include "BlueUIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlueUIView

IMPLEMENT_DYNCREATE(CBlueUIView, CView)

BEGIN_MESSAGE_MAP(CBlueUIView, CView)
	//{{AFX_MSG_MAP(CBlueUIView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlueUIView construction/destruction

CBlueUIView::CBlueUIView()
{
	// TODO: add construction code here

}

CBlueUIView::~CBlueUIView()
{
}

BOOL CBlueUIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBlueUIView drawing

void CBlueUIView::OnDraw(CDC* pDC)
{
	CBlueUIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CBlueUIView printing

BOOL CBlueUIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBlueUIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBlueUIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CBlueUIView diagnostics

#ifdef _DEBUG
void CBlueUIView::AssertValid() const
{
	CView::AssertValid();
}

void CBlueUIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBlueUIDoc* CBlueUIView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBlueUIDoc)));
	return (CBlueUIDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBlueUIView message handlers
