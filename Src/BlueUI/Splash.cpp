// CG: This file was added by the Splash Screen component.
// Splash.cpp : implementation file
//

#include "stdafx.h"  // e. g. stdafx.h
#include "resource.h"  // e.g. resource.h
#include "BlueUI.h"
#include "Splash.h"  // e.g. splash.h
#include "dib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//   Splash Screen class
//文件名：Splash.cpp
//文件功能：显示闪烁窗口
/////////////////////////////////////////////////////////////////////
BOOL CSplashWnd::c_bShowSplashWnd;
static BOOL g_bSplashTimer = FALSE;
CSplashWnd* CSplashWnd::c_pSplashWnd;
CSplashWnd::CSplashWnd()
{
}

CSplashWnd::~CSplashWnd()
{
	// Clear the static window pointer.
	ASSERT(c_pSplashWnd == this);
	c_pSplashWnd = NULL;
}

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	//{{AFX_MSG_MAP(CSplashWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//允许显示闪动窗口
/////////////////////////////////////////////////////////////////////////////
void CSplashWnd::EnableSplashScreen(BOOL bEnable /*= TRUE*/)
{
	c_bShowSplashWnd = bEnable;
}

/////////////////////////////////////////////////////////////////////////////
//显示闪动窗口
/////////////////////////////////////////////////////////////////////////////
void CSplashWnd::ShowSplashScreen(CWnd* pParentWnd /*= NULL*/)
{
	if (!c_bShowSplashWnd || c_pSplashWnd != NULL)
		return;

	// Allocate a new splash screen, and create the window.
	c_pSplashWnd = new CSplashWnd;
	if (!c_pSplashWnd->Create(pParentWnd))
		delete c_pSplashWnd;
	else
	{
		c_pSplashWnd->UpdateWindow();
		c_pSplashWnd->SetWindowPos( & CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	}
}

/////////////////////////////////////////////////////////////////////////////
//按键隐藏闪动窗口
/////////////////////////////////////////////////////////////////////////////
BOOL CSplashWnd::PreTranslateAppMessage(MSG* pMsg)
{
	if (c_pSplashWnd == NULL)
		return FALSE;

	// If we get a keyboard or mouse message, hide the splash screen.
	if (pMsg->message == WM_KEYDOWN ||
	    pMsg->message == WM_SYSKEYDOWN)// ||
//	    pMsg->message == WM_LBUTTONDOWN ||
//	    pMsg->message == WM_RBUTTONDOWN ||
//	    pMsg->message == WM_MBUTTONDOWN ||
//	    pMsg->message == WM_NCLBUTTONDOWN ||
//	    pMsg->message == WM_NCRBUTTONDOWN ||
//	    pMsg->message == WM_NCMBUTTONDOWN)
	{
		c_pSplashWnd->HideSplashScreen();
		return TRUE;	// message handled here
	}

	return FALSE;	// message not handled
}

/////////////////////////////////////////////////////////////////////////////
// 创建窗口
/////////////////////////////////////////////////////////////////////////////
BOOL CSplashWnd::Create(CWnd* pParentWnd /*= NULL*/)
{
	CDib	ShowDib;
	CString	FileInput;
	BOOL	bFlag;
	bFlag=TRUE;
	FileInput = theApp.m_xmlPlat.GetNodeText("application\\Splash");
	if(FileInput!="") {
		FileInput = theApp.GetPlatRootPath() + FileInput;
		CFile	fileRead;
		bFlag=fileRead.Open(FileInput,CFile::modeRead);
		if(bFlag)
		{
			ShowDib.Read(&fileRead);
			fileRead.Close();

			HDC		hDC=::GetDC(NULL);
			CDC *pDC=CDC::FromHandle(hDC);
			CSize bmSize;
			HBITMAP	hBmp;
			hBmp=ShowDib.MakeHBitmap(pDC,bmSize);
			::ReleaseDC(NULL,hDC);
			m_bitmap.Attach(hBmp);
		}
	}
	else
		bFlag=FALSE;

	if(!bFlag){
		//if (!m_bitmap.LoadBitmap(IDB_SPLASH))
			return FALSE;
	}

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);

	return CreateEx(0,
		AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
		NULL, WS_POPUP | WS_VISIBLE, 0, 0, bm.bmWidth, bm.bmHeight, pParentWnd->GetSafeHwnd(), NULL);
}

/////////////////////////////////////////////////////////////////////////////
//隐藏闪动窗口
/////////////////////////////////////////////////////////////////////////////
void CSplashWnd::HideSplashScreen()
{
	// Destroy the window, and update the mainframe.
	if ( c_pSplashWnd == NULL )
		return;
	if(g_bSplashTimer)
	{
		c_pSplashWnd->KillTimer(13);
		g_bSplashTimer = FALSE;
	}
	c_pSplashWnd->DestroyWindow();
	c_pSplashWnd = NULL;
	AfxGetMainWnd()->UpdateWindow();
}

/////////////////////////////////////////////////////////////////////////////
// 非客户区的析构，释放对象
/////////////////////////////////////////////////////////////////////////////
void CSplashWnd::PostNcDestroy()
{
	// Free the C++ class.
	delete this;
}

int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Center the window.
	CenterWindow();

	// Set a timer to destroy the splash screen.
	int nTime = atoi(theApp.m_xmlPlat.GetNodeAttribute("application\\Splash", "time"));
	if(nTime > 0)
	{
		SetTimer(13, nTime, NULL);
		g_bSplashTimer = TRUE;
	}

	return 0;
}

void CSplashWnd::OnPaint()
{
	CPaintDC dc(this);

	CDC dcImage;
	if (!dcImage.CreateCompatibleDC(&dc))
		return;

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);

	// Paint the image.
	CBitmap* pOldBitmap = dcImage.SelectObject(&m_bitmap);
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);
	dcImage.SelectObject(pOldBitmap);
}

void CSplashWnd::OnTimer(UINT nIDEvent)
{
	// Destroy the splash screen window.
	HideSplashScreen();
}
