// ChildFrame.cpp : implementation file
//

#include "stdafx.h"
#include "HtmlViewOWM.h"
#include "ChildFrame.h"
#include "InetTools.h"
#include "ControlFavorites.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

CChildFrame::CChildFrame()
{
}

CChildFrame::~CChildFrame()
{
}


BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_XTP_CREATECONTROL()
	ON_CBN_SELENDOK(IDC_ADDRESSBAR, OnSelEndOkAddressBar)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	get_dll_resource();
/*	VERIFY(m_wndToolBar.CreateToolBar(WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS|CBRS_GRIPPER, this, AFX_IDW_TOOLBAR));
	VERIFY(m_wndToolBar.LoadToolBar(IDR_EXPLORER_BAR));
	m_wndToolBar.SetFlags(xtpFlagStretched);
*/
	XTPImageManager()->SetIcons(IDR_MENUBAR);

	// 设置图标(需要切换资源)	
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_OWM);
	SetIcon(hIcon, TRUE);
	reset_dll_resource();

	// 刷新自定义图标资源
	theApp.m_ol.CommandPlat(OC_SETOWMICONS, (WPARAM)(LPCTSTR)"org.owm.htmlview", 0);
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 初始化工具条
/////////////////////////////////////////////////////////////////////////////
void CChildFrame::InitializeToolbar()
{
	// Create and set the disabled toolbar image list.
	m_imageList.Create(16, 16, ILC_COLOR24|ILC_MASK, 11, 1);
	m_bitmap.LoadBitmap(IDB_DISABLED_BAR);
	m_imageList.Add(&m_bitmap, RGB(255,0,255));
}

/////////////////////////////////////////////////////////////////////////////
// 初始化地址栏
/////////////////////////////////////////////////////////////////////////////
void CChildFrame::InitializeAddressBar()
{
	// create a combo box for the address bar
	if (!m_wndAddressBar.Create(CBS_DROPDOWN | WS_CHILD,
		CRect(0,0,200,400), this, IDC_ADDRESSBAR))
	{
		TRACE0("Failed to create combobox\n");
		return;      // fail to create
	}

	// set the font for the combo and initialize the toolbar.
//	m_wndAddressBar.SetFont(&xtAfxData.font);

//	m_ImageListHttp.Create(IDB_HTTP_TYPE, 16, 1, RGB(255, 0, 255));
//	m_wndAddressBar.SetImageList(&m_ImageListHttp);
}

int CChildFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	if (lpCreateControl->bToolBar)
	{
		switch (lpCreateControl->nID)
		{
		case ID_GO_BACK:
		case ID_GO_SEARCH_THE_WEB:
		case ID_VIEW_FOLDERS:
		case ID_VIEW_HISTORY:
			{
			CXTPControlButton* pButton = (CXTPControlButton*)CXTPControlButton::CreateObject();		
			lpCreateControl->pControl = pButton;
			pButton->SetID(lpCreateControl->nID);
			pButton->SetStyle(xtpButtonIconAndCaption);
			}
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CChildFrame::OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT*pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);
	
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	
	CString strTipText;
	get_dll_resource();
	strTipText.LoadString(pNMHDR->idFrom);
	reset_dll_resource();
	if ( strTipText.GetLength() > 0 )
	{
#ifndef _UNICODE
		if (pNMHDR->code == TTN_NEEDTEXTA)
			lstrcpyn(pTTTA->szText, strTipText, _countof(pTTTA->szText));
		else
			_mbstowcsz(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#else
		if (pNMHDR->code == TTN_NEEDTEXTA)
			_wcstombsz(pTTTA->szText, strTipText, _countof(pTTTA->szText));
		else
			lstrcpyn(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#endif
		return TRUE;
	}

	return CMDIChildWnd::OnToolTipText( nID, pNMHDR, pResult );
}

/////////////////////////////////////////////////////////////////////////////
// 地址栏输入确认
/////////////////////////////////////////////////////////////////////////////
void CChildFrame::OnSelEndOkAddressBar() 
{
	// 打开浏览器输入的地址
}

void CChildFrame::OnClose() 
{
	// 通知平台执行动态关闭操作
	// 注:只有允许动态加载的模块平台才会响应此消息
	if(theApp.m_ol.nOWMType & OWM_DYNLOAD)
	{
		theApp.m_ol.CommandPlat(OC_DYNCLOSE, theApp.m_ol.nOWMID, 0);
	}

	CMDIChildWnd::OnClose();
}

void CChildFrame::ActivateFrame(int nCmdShow) 
{
	if(theApp.m_ol.nOWMType & OWM_MAXWINDOW)
	{
		nCmdShow = SW_SHOWMAXIMIZED;
	}

	CMDIChildWnd::ActivateFrame(nCmdShow);
}
