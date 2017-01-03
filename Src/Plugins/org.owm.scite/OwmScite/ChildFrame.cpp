// ChildFrame.cpp : implementation file
//

#include "stdafx.h"
#include "OwmScite.h"
#include "ChildFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_EDIT_INDICATOR_READONLY,
	ID_EDIT_INDICATOR_POSITION,
	ID_EDIT_INDICATOR_CRLF,
	ID_EDIT_INDICATOR_UNIMODE,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_OVR,
	ID_INDICATOR_REC,
};

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

	get_dll_resource();

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		reset_dll_resource();
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	VERIFY(m_wndToolBar.CreateToolBar(WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS|CBRS_GRIPPER, this, AFX_IDW_TOOLBAR));
	VERIFY(m_wndToolBar.LoadToolBar(IDR_OWM));
	m_wndToolBar.SetFlags(xtpFlagStretched);
//	m_wndToolBar.AddDropDownButton(ID_EDIT_UNDO);
//	m_wndToolBar.AddDropDownButton(ID_EDIT_REDO);
	//VERIFY(m_wndToolBarDebug.CreateToolBar(WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS|CBRS_GRIPPER, this, AFX_IDW_TOOLBAR+1));
	//VERIFY(m_wndToolBarDebug.LoadToolBar(IDR_DEBUG));
	//m_wndToolBarDebug.SetFlags(xtpFlagStretched);

	// 设置图标(需要切换资源)
	//HICON hIcon = AfxGetApp()->LoadIcon(IDR_OWMEDIT);
	//SetIcon(hIcon, TRUE);
	reset_dll_resource();

	// 刷新自定义图标资源
	theApp.m_ol.CommandPlat(OC_SETOWMICONS, (WPARAM)(LPCTSTR)"org.owm.edit", 0);
	
	return 0;
}

void CChildFrame::OnClose() 
{
	// 获取解释器状态
	int nRunState = SCRIPT_STATE_IDLE;

	COwmSciteView* pView = (COwmSciteView*)GetActiveView();
	if(pView)
	{
		nRunState = pView->GetRunState();
	}

	// 如果处于调试或运行状态,则不关闭
	if( (SCRIPT_STATE_IDLE != nRunState) &&
		(SCRIPT_STATE_END != nRunState) &&
		(SCRIPT_STATE_STOP!= nRunState) )
	{
		DLL_LOADSTRING(strTmp, IDS_PROMPT_SCRIPT_CANTSTOP);	// 已经有脚本正在调试运行，请等待完成或终止任务！
		AfxMessageBox(strTmp, MB_ICONINFORMATION);
		return;
	}

	CMDIChildWnd::OnClose();

	// 通知平台执行动态关闭操作
	theApp.m_ol.CommandPlat(OC_DYNCLOSE, theApp.m_ol.nOWMID, 0);
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	//DWORD dwStyle = WS_CHILD | WS_VISIBLE |	/*WS_HSCROLL |*/ WS_VSCROLL | SPLS_DYNAMIC_SPLIT;
	/*return m_wndSplitter.Create (this,
                               2, 1,    // TODO: adjust the number of rows, columns
                               CSize (10, 10),     // TODO: adjust the minimum pane size
                               pContext, dwStyle);*/
	/*
	if(m_wndSplitter.CreateStatic(this,2,1) == NULL)
		return FALSE;
	m_wndSplitter.CreateView(1,0,RUNTIME_CLASS(COwmSciteView),CSize(0,0),pContext);
	m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(COwmSciteView),CSize(0,0),pContext);
	return TRUE;
	*/
	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CChildFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(theApp.m_ol.nOWMType & OWM_MAXWINDOW)
	{
		nCmdShow = SW_SHOWMAXIMIZED;
	}
	
	CMDIChildWnd::ActivateFrame(nCmdShow);
}
