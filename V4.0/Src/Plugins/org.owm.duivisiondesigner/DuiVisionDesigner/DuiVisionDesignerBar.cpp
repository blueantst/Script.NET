// DuiVisionDesignerBar.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "DuiVisionDesignerBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_DuiVisionDesigner_BAR	2000
#define IDC_DuiVisionDesigner_VIEW	2001
/////////////////////////////////////////////////////////////////////////////
// CDuiVisionDesignerBar

CDuiVisionDesignerBar::CDuiVisionDesignerBar()
{
}

CDuiVisionDesignerBar::~CDuiVisionDesignerBar()
{
}

CObject* PASCAL CDuiVisionDesignerBar::CreateObject()
{
	CDuiVisionDesignerBar* pBar = new CDuiVisionDesignerBar;

	if(!pBar->Create(AfxGetApp()->m_pMainWnd, IDC_DuiVisionDesigner_BAR,
		_T("DuiVisionDesignerBar"), CSize(200, 200), CBRS_LEFT))
	{
		TRACE0("Failed to create dialog bar CDuiVisionDesignerBar\n");
		return NULL;		// fail to create
	}

	pBar->EnableDockingEx(CBRS_ALIGN_ANY, CBRS_XT_ALL_FLAT);

	return pBar;
}

IMPLEMENT_RUNTIMECLASS(CDuiVisionDesignerBar, CXTDockWindow, 0xFFFF, CDuiVisionDesignerBar::CreateObject)

BEGIN_MESSAGE_MAP(CDuiVisionDesignerBar, CXTDockWindow)
	//{{AFX_MSG_MAP(CDuiVisionDesignerBar)
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDuiVisionDesignerBar message handlers

int CDuiVisionDesignerBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTDockWindow::OnCreate(lpCreateStruct) == -1)
		return -1;

	// define the style used by the bar.
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL |
		ES_AUTOHSCROLL | WS_TABSTOP;

	if (!m_DuiVisionDesignerView.Create(NULL, NULL, 
		dwStyle, CRect(0,0,0,0), this, IDC_DuiVisionDesigner_VIEW))
	{
		TRACE(_T("Failed to create controlbar window.\n"));
		return -1;
	}

	return 0;
}

void CDuiVisionDesignerBar::OnWindowPosChanged(WINDOWPOS FAR* lpWP) 
{
	CXTDockWindow::OnWindowPosChanged(lpWP);
	
	CRect rc;
	GetInsideRect( rc );
	if( IsFloating()) 
	{
		rc.DeflateRect(2,2);
	}
	if( m_DuiVisionDesignerView.GetSafeHwnd( )) 
	{
		m_DuiVisionDesignerView.MoveWindow(rc.left, rc.top, rc.Width(), rc.Height());
	}
}
