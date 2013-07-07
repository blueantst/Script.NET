// ChildFrame.cpp : implementation file
//

#include "stdafx.h"
#include "PTerminal.h"
#include "ChildFrame.h"
#include "OptTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LOADRCSTRING(str,strID) \
	get_dll_resource();\
	str.LoadString(strID);\
	reset_dll_resource();

void get_dll_resource(void);
void reset_dll_resource(void);

static UINT indicators[] =
{
//	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_INFO,
	ID_INDICATOR_TERMTYPE,
	ID_INDICATOR_SHELL,
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
/*
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_TELNET))
	{
		reset_dll_resource();
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
*/
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		reset_dll_resource();
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// 初始化状态栏
	m_wndStatusBar.SetPaneInfo(0, ID_INDICATOR_SHELL, SBPS_NORMAL, 80);
	m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_INFO, SBPS_NORMAL, 350);
	m_wndStatusBar.SetPaneInfo(2, ID_INDICATOR_TERMTYPE, SBPS_NORMAL, 80);

	int nDefaultShell = AfxGetApp()->GetProfileInt(REG_PTERMINAL, REG_DEFAULTSHELL, SHELL_TCL);
	SwitchShell(nDefaultShell);

	VERIFY(m_wndToolBar.CreateToolBar(WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS|CBRS_GRIPPER, this, AFX_IDW_TOOLBAR));
	VERIFY(m_wndToolBar.LoadToolBar(IDR_TELNET));
	m_wndToolBar.SetFlags(xtpFlagStretched);

	// 设置图标(需要切换资源)
	HICON hIcon = AfxGetApp()->LoadIcon(IDI_TELNET);
	SetIcon(hIcon, TRUE);

	reset_dll_resource();

	// 刷新自定义图标资源
	theApp.m_ol.CommandPlat(OC_SETOWMICONS, (WPARAM)(LPCTSTR)"org.owm.terminal", 0);
	
	return 0;
}

void CChildFrame::SwitchShell(int nShellMode)
{
	CString strMsg;
	CString strTemp;
	switch(nShellMode)
	{
	case SHELL_TCL:
		{
		LOADRCSTRING(strTemp, IDS_TERM_TCL)
		SetShell(strTemp);
		CString strTclInterpName = AfxGetApp()->GetProfileString(REG_PTERMINAL, REG_INTERPNAME, INTERP_NAME);
		LOADRCSTRING(strTemp, IDS_TCL_INTERP)
		strMsg.Format(strTemp, strTclInterpName);
		SetConnInfo(strMsg);
		LOADRCSTRING(strTemp, IDS_TERM_TYPE_STD)
		SetTermType(strTemp);
		}
		break;
	case SHELL_PYTHON:
		{
		LOADRCSTRING(strTemp, IDS_TERM_PYTHON)
		SetShell(strTemp);
		CString strPythonInterpName = AfxGetApp()->GetProfileString(REG_PTERMINAL, REG_INTERPNAME, INTERP_NAME);
		LOADRCSTRING(strTemp, IDS_PYTHON_INTERP)
		strMsg.Format(strTemp, strPythonInterpName);
		SetConnInfo(strMsg);
		LOADRCSTRING(strTemp, IDS_TERM_TYPE_STD)
		SetTermType(strTemp);
		}
		break;
	case SHELL_RUBY:
		{
		LOADRCSTRING(strTemp, IDS_TERM_RUBY)
		SetShell(strTemp);
		CString strRubyInterpName = AfxGetApp()->GetProfileString(REG_PTERMINAL, REG_INTERPNAME, INTERP_NAME);
		LOADRCSTRING(strTemp, IDS_RUBY_INTERP)
		strMsg.Format(strTemp, strRubyInterpName);
		SetConnInfo(strMsg);
		LOADRCSTRING(strTemp, IDS_TERM_TYPE_STD)
		SetTermType(strTemp);
		}
		break;
	case SHELL_LUA:
		{
		LOADRCSTRING(strTemp, IDS_TERM_LUA)
		SetShell(strTemp);
		CString strLuaInterpName = AfxGetApp()->GetProfileString(REG_PTERMINAL, REG_INTERPNAME, INTERP_NAME);
		LOADRCSTRING(strTemp, IDS_LUA_INTERP)
		strMsg.Format(strTemp, strLuaInterpName);
		SetConnInfo(strMsg);
		LOADRCSTRING(strTemp, IDS_TERM_TYPE_STD)
		SetTermType(strTemp);
		}
		break;
	case SHELL_PERL:
		{
		LOADRCSTRING(strTemp, IDS_TERM_PERL)
		SetShell(strTemp);
		CString strPerlInterpName = AfxGetApp()->GetProfileString(REG_PTERMINAL, REG_INTERPNAME, INTERP_NAME);
		LOADRCSTRING(strTemp, IDS_PERL_INTERP)
		strMsg.Format(strTemp, strPerlInterpName);
		SetConnInfo(strMsg);
		LOADRCSTRING(strTemp, IDS_TERM_TYPE_STD)
		SetTermType(strTemp);
		}
		break;
	case SHELL_TELNET:
		{
		LOADRCSTRING(strTemp, IDS_TERM_TELNET)
		SetShell(strTemp);		
		SetConnInfo(_T(""));
		LOADRCSTRING(strTemp, IDS_TERM_TYPE_VT)
		SetTermType(strTemp);
		}
		break;
	case SHELL_COM:
		{
		LOADRCSTRING(strTemp, IDS_TERM_COM)
		SetShell(strTemp);		
		SetConnInfo(_T(""));
		LOADRCSTRING(strTemp, IDS_TERM_TYPE_VT)
		SetTermType(strTemp);
		}
		break;
	}
}

void CChildFrame::SetShell(CString strMsg)
{
	m_wndStatusBar.SetPaneText(0, strMsg, TRUE);
}

void CChildFrame::SetConnInfo(CString strMsg)
{
	m_wndStatusBar.SetPaneText(1, strMsg, TRUE);
}

void CChildFrame::SetTermType(CString strType)
{
	m_wndStatusBar.SetPaneText(2, strType, TRUE);
}

void CChildFrame::OnClose() 
{
	CMDIChildWnd::OnClose();

	// 通知平台执行动态关闭操作
	theApp.m_ol.CommandPlat(OC_DYNCLOSE, theApp.m_ol.nOWMID, 0);
}
