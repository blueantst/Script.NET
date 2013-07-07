////////////////////////////////////////////////////////////////////////////
//	File:		plat.h
//	Version:	2.0.0.0
//	Created:	2000-07-20
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	Comm define of platform.
////////////////////////////////////////////////////////////////////////////
#ifndef __PLAT_H_
#define __PLAT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////////////////////////////////////////////////////////////////////////////
// COM Message define
#define WM_PLAT_COMCMD		(WM_USER+50)

struct STRU_COMCMD
{
	BOOL	bPost;		// 是否PostMessage
	CString	strOwmId;	// 模块ID(字符串)
	int		nCmd;		// 命令
	WPARAM	wParam;		// 参数1
	LPARAM	lParam;		// 参数2(如果nDataLen>0则表示携带的数据的指针)
	LPVOID	lpResult;	// 返回值
	int		nDataLen;	// 数据长度
};

// 中间层COM接口调用平台功能
#define COM_CMD(owm, cmd, w, l)	\
	{	\
		CMainFrame* pMainFrame = (CMainFrame*)(theApp.m_pMainWnd);	\
		if(pMainFrame)	\
		{	\
			STRU_COMCMD	comcmd;	\
			comcmd.bPost = FALSE;	\
			comcmd.strOwmId = owm;	\
			comcmd.nCmd = cmd;	\
			comcmd.wParam = (WPARAM)w;	\
			comcmd.lParam = (LPARAM)l;	\
			comcmd.lpResult = NULL;	\
			comcmd.nDataLen	= 0;	\
			pMainFrame->SendMessage(WM_PLAT_COMCMD, (WPARAM)0, (LPARAM)(&comcmd));	\
		}	\
	}	\

// 中间层COM接口调用平台功能(带返回值的)
#define RCOM_CMD(owm, cmd, w, l, r)	\
	{	\
		CMainFrame* pMainFrame = (CMainFrame*)(theApp.m_pMainWnd);	\
		if(pMainFrame)	\
		{	\
			STRU_COMCMD	comcmd;	\
			comcmd.bPost = FALSE;	\
			comcmd.strOwmId = owm;	\
			comcmd.nCmd = cmd;	\
			comcmd.wParam = (WPARAM)w;	\
			comcmd.lParam = (LPARAM)l;	\
			comcmd.lpResult = r;	\
			comcmd.nDataLen	= 0;	\
			pMainFrame->SendMessage(WM_PLAT_COMCMD, (WPARAM)0, (LPARAM)(&comcmd));	\
		}	\
	}	\

// 中间层COM接口调用平台功能(PostMessage方式)
#define PCOM_CMD(owm, cmd, w, l)	\
	{	\
		CMainFrame* pMainFrame = (CMainFrame*)(theApp.m_pMainWnd);	\
		if(pMainFrame)	\
		{	\
			STRU_COMCMD* pcomcmd = new STRU_COMCMD;	\
			pcomcmd->bPost = TRUE;	\
			pcomcmd->strOwmId = owm;	\
			pcomcmd->nCmd = cmd;	\
			pcomcmd->wParam = (WPARAM)w;	\
			pcomcmd->lParam = (LPARAM)l;	\
			pcomcmd->lpResult = NULL;	\
			pcomcmd->nDataLen = 0;	\
			if(!pMainFrame->PostMessage(WM_PLAT_COMCMD, (WPARAM)0, (LPARAM)(pcomcmd)))	\
			{	\
				delete pcomcmd;	\
			}	\
		}	\
	}	\

// 中间层COM接口调用平台功能(带返回值的,PostMessage方式)
#define PRCOM_CMD(owm, cmd, w, l, r)	\
	{	\
		CMainFrame* pMainFrame = (CMainFrame*)(theApp.m_pMainWnd);	\
		if(pMainFrame)	\
		{	\
			STRU_COMCMD* pcomcmd = new STRU_COMCMD;	\
			pcomcmd->bPost = TRUE;	\
			pcomcmd->strOwmId = owm;	\
			pcomcmd->nCmd = cmd;	\
			pcomcmd->wParam = (WPARAM)w;	\
			pcomcmd->lParam = (LPARAM)l;	\
			pcomcmd->lpResult = r;	\
			pcomcmd->nDataLen = 0;	\
			if(!pMainFrame->PostMessage(WM_PLAT_COMCMD, (WPARAM)0, (LPARAM)(pcomcmd)))	\
			{	\
				delete pcomcmd;	\
			}	\
		}	\
	}	\

// 中间层COM接口调用平台功能(SendMessage方式,可以携带数据)
#define DCOM_CMD(owm, cmd, w, l, dl)	\
	{	\
		CMainFrame* pMainFrame = (CMainFrame*)(theApp.m_pMainWnd);	\
		if(pMainFrame)	\
		{	\
			STRU_COMCMD* pcomcmd = new STRU_COMCMD;	\
			pcomcmd->bPost = TRUE;	\
			pcomcmd->strOwmId = owm;	\
			pcomcmd->nCmd = cmd;	\
			pcomcmd->wParam = (WPARAM)w;	\
			pcomcmd->lpResult = NULL;	\
			BYTE* pData = new BYTE[dl];	\
			pcomcmd->lParam = (LPARAM)pData;	\
			memcpy((BYTE*)(pcomcmd->lParam), l, dl);	\
			pcomcmd->nDataLen = dl;	\
			if(!pMainFrame->SendMessage(WM_PLAT_COMCMD, (WPARAM)(LPCTSTR)(owm), (LPARAM)(pcomcmd)))	\
			{	\
				delete pData;	\
				delete pcomcmd;	\
			}	\
		}	\
	}	\

// 中间层COM接口调用平台功能(PostMessage方式,可以携带数据)
#define PDCOM_CMD(owm, cmd, w, l, dl)	\
	{	\
		CMainFrame* pMainFrame = (CMainFrame*)(theApp.m_pMainWnd);	\
		if(pMainFrame)	\
		{	\
			STRU_COMCMD* pcomcmd = new STRU_COMCMD;	\
			pcomcmd->bPost = TRUE;	\
			pcomcmd->strOwmId = owm;	\
			pcomcmd->nCmd = cmd;	\
			pcomcmd->wParam = (WPARAM)w;	\
			pcomcmd->lpResult = NULL;	\
			BYTE* pData = new BYTE[dl];	\
			pcomcmd->lParam = (LPARAM)pData;	\
			memcpy((BYTE*)(pcomcmd->lParam), l, dl);	\
			pcomcmd->nDataLen = dl;	\
			if(!pMainFrame->PostMessage(WM_PLAT_COMCMD, (WPARAM)(LPCTSTR)(owm), (LPARAM)(pcomcmd)))	\
			{	\
				delete pData;	\
				delete pcomcmd;	\
			}	\
		}	\
	}	\

// 中间层COM接口调用平台功能(带返回值的,PostMessage方式,可以携带数据)
#define PRDCOM_CMD(owm, cmd, w, l, r, dl)	\
	{	\
		CMainFrame* pMainFrame = (CMainFrame*)(theApp.m_pMainWnd);	\
		if(pMainFrame)	\
		{	\
			STRU_COMCMD* pcomcmd = new STRU_COMCMD;	\
			pcomcmd->bPost = TRUE;	\
			pcomcmd->strOwmId = owm;	\
			pcomcmd->nCmd = cmd;	\
			pcomcmd->wParam = (WPARAM)w;	\
			BYTE* pData = new BYTE[dl];	\
			pcomcmd->lParam = (LPARAM)pData;	\
			pcomcmd->lpResult = r;	\
			pcomcmd->nDataLen = dl;	\
			memcpy((BYTE*)(pcomcmd->lParam), l, dl);	\
			if(!pMainFrame->PostMessage(WM_PLAT_COMCMD, (WPARAM)(LPCTSTR)(owm), (LPARAM)(pcomcmd)))	\
			{	\
				delete pData;	\
				delete pcomcmd;	\
			}	\
		}	\
	}	\

#define WS_PLAT		-2
#define WS_ALL		-1


////////////////////////////////////////////////////////////////////////////
// PlatForm Message define

#define WM_PLAT_DYNCLOSE		(WM_USER+51)


////////////////////////////////////////////////////////////////////////////
// PropertyPage define

// 定义属性页树节点类型
#define OPT_PLATFORM	0	// 平台
#define OPT_OWM			1	// OWM
#define OPT_VCI			2	// VCI组件
#define OPT_TESTDLL		3	// 测试DLL
#define OPT_INTERP		4	// 解释器
#define OPT_NODE		5	// 一般节点


/////////////////////////////////////////////////////////////////////
// 定义输出窗口控制对象
class COutputWndCtrl : public CObject
{
public:
	COutputWndCtrl(){};
	~COutputWndCtrl(){};

	COutputWndCtrl(const COutputWndCtrl& other)
	{
		nID		= other.nID;
		strTitle= other.strTitle;
		nCtrlType= other.nCtrlType;
		pWnd	= other.pWnd;
		pPane	= other.pPane;
		bSave	= other.bSave;
	}

	COutputWndCtrl& operator = (const COutputWndCtrl& other)
	{
		nID		= other.nID;
		strTitle= other.strTitle;
		nCtrlType= other.nCtrlType;
		pWnd	= other.pWnd;
		pPane	= other.pPane;
		bSave	= other.bSave;
		return *this;
	}

public:
	int		nID;		// 窗口ID
	CString	strTitle;	// 标题
	int		nCtrlType;	// 窗口类型
	CWnd*	pWnd;		// 窗口指针
	CXTPDockingPane* pPane; // Pane
	BOOL	bSave;		// 是否保存状态
};

typedef CArray<COutputWndCtrl, COutputWndCtrl&> COutputCtrlArray;

// 输出窗口类型
enum{
	OUTPUT_RICH,
	OUTPUT_GRID,
	OUTPUT_TREE,
};

// 输出窗口文字颜色
#define COLOR_NORMAL	0
#define COLOR_BLUE		1
#define COLOR_ERROR		2

// 输出窗口操作函数
void OUTPUT_CLEAR(int nID = 0);
int OUTPUT_ACTIVE_ID(int nID);
int OUTPUT_SET_ID(int nID, CString strFormat);
CString OUTPUT_GET_ID(int nID, CString strParam);
int OUTPUT_TREEITEMS_ID(int nID, LPVOID pItems);
int OUTPUT_COLOR(COLORREF color, LPCTSTR format, ...);
int OutputText(int nID, CString strText, COLORREF color);
int OUTPUT(int nFmt, LPCTSTR format, ...);
int OUTPUT_ID(int nID, LPCTSTR format, ...);
int OUTPUT_ID_DIRECT(int nID, LPCTSTR strOut);
BOOL OUTPUT_VERIFY(CString strActual, CString strVerify);
BOOL STRING_VERIFY(CString strActual, CString strVerify);
void StringFillSpace(CString& strLine, int nLen);
CString ReverseString(CString strInput);

// 调试输出
//#define _ENABLE_DEBUG_OUTPUT
#ifdef _ENABLE_DEBUG_OUTPUT
	#define DEBUG_OUTPUT OUTPUT
#else
	#define DEBUG_OUTPUT //
#endif


/////////////////////////////////////////////////////////////////
// PopupControl define

// PopupControl控件的事件ID定义
#define	ID_POPUP_CLOSE			1	// 关闭按钮
#define ID_POPUP_GOTO_SITE1		2	// 标题的链接
#define ID_POPUP_GOTO_SITE2		3	// 文字的链接
#define ID_POPUP_BUTTON_CLICK	10	// 按钮的ID起始值

#endif // !defined(__PLAT_H_)