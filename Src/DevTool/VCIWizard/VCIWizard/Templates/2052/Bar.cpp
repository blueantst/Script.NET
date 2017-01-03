// [!output PROJECT_NAME]Bar.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "[!output PROJECT_NAME]Bar.h"
[!if OWM_TABCTRLBAR]
#include "[!output PROJECT_NAME]View.h"
#include "[!output PROJECT_NAME]Doc.h"
[!endif]

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_[!output PROJECT_NAME]_BAR	2000
#define IDC_[!output PROJECT_NAME]_VIEW	2001
/////////////////////////////////////////////////////////////////////////////
// C[!output PROJECT_NAME]Bar

C[!output PROJECT_NAME]Bar::C[!output PROJECT_NAME]Bar()
{
}

C[!output PROJECT_NAME]Bar::~C[!output PROJECT_NAME]Bar()
{
}

CObject* PASCAL C[!output PROJECT_NAME]Bar::CreateObject()
{
	C[!output PROJECT_NAME]Bar* pBar = new C[!output PROJECT_NAME]Bar;

	if(!pBar->Create(AfxGetApp()->m_pMainWnd, IDC_[!output PROJECT_NAME]_BAR,
		_T("[!output PROJECT_NAME]Bar"), CSize(200, 200), CBRS_LEFT))
	{
		TRACE0("Failed to create dialog bar C[!output PROJECT_NAME]Bar\n");
		return NULL;		// fail to create
	}

[!if OWM_TABCTRLBAR]
	C[!output PROJECT_NAME]Doc* pDoc = new C[!output PROJECT_NAME]Doc;
	ASSERT_KINDOF(CDocument, pDoc);
	pBar->AddView(_T( "[!output PROJECT_NAME]View" ), RUNTIME_CLASS(C[!output PROJECT_NAME]View), pDoc);
[!endif]
	pBar->EnableDockingEx(CBRS_ALIGN_ANY, CBRS_XT_ALL_FLAT);

	return pBar;
}

[!if OWM_CONTROLBAR]
IMPLEMENT_RUNTIMECLASS(C[!output PROJECT_NAME]Bar, CXTDockWindow, 0xFFFF, C[!output PROJECT_NAME]Bar::CreateObject)
[!else]
IMPLEMENT_RUNTIMECLASS(C[!output PROJECT_NAME]Bar, CXTTabCtrlBar, 0xFFFF, C[!output PROJECT_NAME]Bar::CreateObject)
[!endif]

[!if OWM_CONTROLBAR]
BEGIN_MESSAGE_MAP(C[!output PROJECT_NAME]Bar, CXTDockWindow)
[!else]
BEGIN_MESSAGE_MAP(C[!output PROJECT_NAME]Bar, CXTTabCtrlBar)
[!endif]
	//{{AFX_MSG_MAP(C[!output PROJECT_NAME]Bar)
[!if OWM_CONTROLBAR]
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
[!endif]
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C[!output PROJECT_NAME]Bar message handlers

[!if OWM_CONTROLBAR]
int C[!output PROJECT_NAME]Bar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTDockWindow::OnCreate(lpCreateStruct) == -1)
		return -1;

	// define the style used by the bar.
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL |
		ES_AUTOHSCROLL | WS_TABSTOP;

	if (!m_[!output PROJECT_NAME]View.Create(NULL, NULL, 
		dwStyle, CRect(0,0,0,0), this, IDC_[!output PROJECT_NAME]_VIEW))
	{
		TRACE(_T("Failed to create controlbar window.\n"));
		return -1;
	}

	return 0;
}

void C[!output PROJECT_NAME]Bar::OnWindowPosChanged(WINDOWPOS FAR* lpWP) 
{
	CXTDockWindow::OnWindowPosChanged(lpWP);
	
	CRect rc;
	GetInsideRect( rc );
	if( IsFloating()) 
	{
		rc.DeflateRect(2,2);
	}
	if( m_[!output PROJECT_NAME]View.GetSafeHwnd( )) 
	{
		m_[!output PROJECT_NAME]View.MoveWindow(rc.left, rc.top, rc.Width(), rc.Height());
	}
}
[!endif]
