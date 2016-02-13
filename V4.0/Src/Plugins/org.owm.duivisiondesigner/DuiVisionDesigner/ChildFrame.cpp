// ChildFrame.cpp : implementation file
//

#include "stdafx.h"
#include "DuiVisionDesigner.h"
#include "ChildFrame.h"

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
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 设置图标(需要切换资源)
	get_dll_resource();
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_OWM);
	SetIcon(hIcon, TRUE);
	reset_dll_resource();
	
	return 0;
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
