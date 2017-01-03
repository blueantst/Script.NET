////////////////////////////////////////////////////////////////////////////
//	File:		PlatUI.cpp
//	Version:	1.0.0.0
//	Created:	2005-07-08
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	Implementation of Platform UI interface object.
////////////////////////////////////////////////////////////////////////////
// PlatUI.cpp : implementation file
//

#include "stdafx.h"
#include "BlueUI.h"
#include "PlatUI.h"
#include "MainFrm.h"
#include "FileVersion.h"		// Get version of file

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlatUI

CPlatUI::CPlatUI()
{
}

CPlatUI::~CPlatUI()
{
}

//////////////////////////////////////////////////////////////////////////
// 接口实现
//////////////////////////////////////////////////////////////////////////
INTERFACE_IMPLEMENT(PlatUI)
CONTROL_INTERFACE_IMPLEMENT(PlatUI)

/////////////////////////////////////////////////////////////////////////////
// CPlatUI Interface handlers 具体操作

// 注:此接口中的大部分函数都会存在多线程冲突问题,因为此接口许多
// 函数都有可能同时被多个线程同时调用.
// 目前采用的防止冲突的方法是通过一个消息代理来间接的执行操作,
// 每个调用都通过消息方式发给MainFrame,MainFrame再调用PlatForm
// 中的函数来执行实际的操作.因为消息是串行处理的,因此就可以保
// 证同时只有一个调用.
// 一般通过SendMessage方式来同步调用,但有时候需要异步调用,必须
// 通过PostMessage方式调用,因此提供了两个宏COM_CMD和PCOM_CMD来
// 提供同步和异步调用操作.异步方式,如果消息的某个参数是指向一个
// 结构的指针,则需要注意,必须在消息的接受方执行删除操作。
// RCOM_CMD和PRCOM_CMD宏增加了返回值,方法是传递一个变量的地址。
// 有些情况下仅用SendMessage和PostMessage的两个参数是不够的,可能
// 需要传递更多的数据,因此提供了PDCOM_CMD和PRDCOM_CMD宏,允许携带
// 一个结构,此结构在接收方负责删除。

/////////////////////////////////////////////////////////////////////////////
// 获取应用程序名
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::GetAppName()
{
	return theApp.m_xmlPlat.GetNodeText("application\\appName");
}

/////////////////////////////////////////////////////////////////////////////
// 获取平台路径
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::GetPlatPath()
{
	return theApp.GetPlatRootPath();
}

/////////////////////////////////////////////////////////////////////////////
// 获取平台版本
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::GetPlatVersion()
{
	// 获取平台程序版本,改为使用程序版本而不是注册表版本
	CString strVersion;
	CFileVersion fileversion;
	TCHAR szFullPath[MAX_PATH];
	::GetModuleFileName(NULL, szFullPath, MAX_PATH);
	// Get version number
	if (!fileversion.Open(szFullPath))
		return _T("0.0.0.0");
	strVersion = fileversion.GetFileVersion();
	fileversion.Close();
	strVersion.Replace(" ", "");
	strVersion.Replace(",", ".");
/*
	// 获取注册表中程序版本
	CXTRegistryManager registry(HKEY_LOCAL_MACHINE);
	CString strSection = "Setup";
	// 注：CXTRegistryManager的Section都是针对当前应用程序的,因此不需要再增加应用程序注册表路径,只要把Section设置为Setup就可以
	//strSection.Format("Software\\%s\\%s\\Setup",
	//		theApp.m_xmlPlat.GetNodeText("application\\RegCompany"),
	//		theApp.m_xmlPlat.GetNodeText("application\\RegProfile"));
	CString strVersion = registry.GetProfileString(strSection, "Version", "unknown");
*/
	return strVersion;
}

/////////////////////////////////////////////////////////////////////////////
// 获取当前语言
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(int)
CPlatUI::XPlatUI::GetCurrentLanguage()
{
	return (LANGUAGE_PAGE_CHINESE == theApp.m_curLanguage) ? LANGUAGE_CHINESE : LANGUAGE_ENGLISH;
}

/////////////////////////////////////////////////////////////////////////////
// 获取平台注册表根路径
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::GetPlatRegistry()
{
	CString strSection;
	strSection.Format("Software\\%s\\%s",
			theApp.m_xmlPlat.GetNodeText("application\\RegCompany"),
			theApp.m_xmlPlat.GetNodeText("application\\RegProfile"));

	return strSection;
}

/////////////////////////////////////////////////////////////////////////////
// 获取平台版权字符串
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::GetPlatCopyRight()
{
	return theApp.m_xmlPlat.GetNodeText("application\\CopyRight");
}

/////////////////////////////////////////////////////////////////////////////
// 获取主页URL
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::GetPlatHomeURL()
{
	return theApp.m_xmlPlat.GetNodeText("application\\HomeURL");
}

/////////////////////////////////////////////////////////////////////////////
// 获取下载URL
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::GetPlatDownloadURL()
{
	return theApp.m_xmlPlat.GetNodeText("application\\DownloadURL");
}

/////////////////////////////////////////////////////////////////////////////
// 发送消息
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(int)
CPlatUI::XPlatUI::SendMessage(CVciMessage* pIn, CVciMessage* ppOut)
{
	if(pIn == NULL)
	{
		return 0;
	}

	if((pIn->GetMsgType() == VMT_BROADCAST) || (pIn->GetMsgType() == VMT_MULTICAST))
	{
		// 如果是广播消息或组播消息,则插入平台的消息队列进行广播调用
		if(theApp.m_pIMessageQueue)
		{
			theApp.m_pIMessageQueue->PostMessage(pIn, "");
		}
		return 0;
	}

	CPlugIn* pPlugIn = theApp.FindPlugIn(pIn->GetFirstNodeId());
	if(pPlugIn)	// 发给组件的消息
	{
		if((pPlugIn->m_strProcess == theApp.m_strProcessId) || (pPlugIn->m_strProcess == ""))
		{
			// 目的组件能找到并且进程相同或者进程为空,则直接调用
			return pPlugIn->SendMessage(pIn, ppOut);
		}else
		{
			// 如果目的组件能找到但进程不同,则送给IPC组件处理
			pPlugIn = theApp.FindPlugIn(VCIID_IPC);
			if(pPlugIn)
			{
				return pPlugIn->SendMessage(pIn, ppOut);
			}
		}
	}else
	if(pIn->GetFirstNodeId() == VCIID_PLATFORM)	// 发给平台的消息
	{
		if(pIn->GetSync() == VMT_SYNC)
		{
			// 同步消息,直接调用平台的消息处理函数
			return ProcessMessage(pIn, ppOut);
		}else
		{
			// 异步消息,插入消息队列
			if(theApp.m_pIMessageQueue)
			{
				theApp.m_pIMessageQueue->PostMessage(pIn, "");
			}
		}
	}else
	{
		// 目的组件找不到则送给通信组件发送
		pPlugIn = theApp.FindPlugIn(VCIID_COMMUNICATE);
		if(pPlugIn)
		{
			if((pPlugIn->m_strProcess == theApp.m_strProcessId) || (pPlugIn->m_strProcess == ""))
			{
				// 如果通信组件在当前进程则送给通信组件
				return pPlugIn->SendMessage(pIn, ppOut);
			}else
			{
				// 通信组件不在当前进程则送给IPC组件
				pPlugIn = theApp.FindPlugIn(VCIID_IPC);
				if(pPlugIn)
				{
					return pPlugIn->SendMessage(pIn, ppOut);
				}
			}
		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 平台的消息处理
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(int)
CPlatUI::XPlatUI::ProcessMessage(CVciMessage* pIn, CVciMessage* ppOut)
{
	if(pIn == NULL)
	{
		return 0;
	}

	if(pIn->GetMsgType() == VMT_BROADCAST)
	{
		// 如果是广播消息,则调用所有组件的ProcessMessage函数
		for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
		{
			if(theApp.m_aPlugIns[i].m_pIVciControl == NULL)
			{
				// 如果组件实例未创建,则创建实例对象
				theApp.m_aPlugIns[i].CreateObject();
			}
			if(theApp.m_aPlugIns[i].m_pIVciControl != NULL)
			{
				#ifdef _DEBUG
				TRACE("Send broadcast message to %s\n", theApp.m_aPlugIns[i].m_strId);
				#endif
				// 目的ID设置为对应组件的ID
				CVciMessage msg = *pIn;
				msg.SetDest(theApp.m_aPlugIns[i].m_strId);
				((IVciControl*)(theApp.m_aPlugIns[i].m_pIVciControl))->ProcessMessage(&msg, NULL);
			}
		}
		return 0;
	}else
	if(pIn->GetMsgType() == VMT_MULTICAST)
	{
		// 如果是组播消息,则解析目的组件,获取所有要发送的目的组件(目的组件字段用,进行分隔)
		CString strMultiDest = pIn->GetDest();
		CStringArray asDest;
		int nPos = -1;
		while((nPos = strMultiDest.Find(",")) != -1)
		{
			asDest.Add(strMultiDest.Left(nPos));
			strMultiDest.Delete(0, nPos+1);
		}
		if(strMultiDest != "")
		{
			asDest.Add(strMultiDest);
		}

		// 调用所有目的组件的ProcessMessage函数
		for(int i=0; i<asDest.GetSize(); i++)
		{
			CPlugIn* pPlugIn = theApp.FindPlugIn(asDest[i]);
			if(pPlugIn)
			{
				if(pPlugIn->m_pIVciControl == NULL)
				{
					// 如果组件实例未创建,则创建实例对象
					pPlugIn->CreateObject();
				}
				if(pPlugIn->m_pIVciControl != NULL)
				{
					#ifdef _DEBUG
					TRACE("Send broadcast message to %s\n", pPlugIn->m_strId);
					#endif
					// 目的ID设置为对应组件的ID
					CVciMessage msg = *pIn;
					msg.SetDest(pPlugIn->m_strId);
					((IVciControl*)(pPlugIn->m_pIVciControl))->ProcessMessage(&msg, NULL);
				}
			}
		}
		return 0;		
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 发送平台命令
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(int)
CPlatUI::XPlatUI::SendCommand(int nCmd, WPARAM wParam, LPARAM lParam)
{
	int nRet;
	RCOM_CMD("", nCmd, wParam, lParam, &nRet)
	return nRet;
}

/////////////////////////////////////////////////////////////////////////////
// 发送平台命令
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::SendCommand(int nCmd, WPARAM wParam, LPARAM lParam, LPVOID lpResult)
{
	RCOM_CMD("", nCmd, wParam, lParam, lpResult)
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 添加输出窗口
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(int)
CPlatUI::XPlatUI::AddOutput(CString strName)
{
	CPlatUI *pObj = GET_INTERFACE_OBJECT(PlatUI);

	// 修改为通过消息形式传递,第2个参数用于回传ID
	int nID = 0;
	COM_CMD("", OC_CREATEOUTPUT, (WPARAM)(LPCTSTR)strName, &nID);

	return nID;
}

/////////////////////////////////////////////////////////////////////////////
// 删除输出窗口
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::DelOutput(int nOT)
{
	CPlatUI *pObj = GET_INTERFACE_OBJECT(PlatUI);

	// 修改为通过消息形式传递
	PCOM_CMD("", OC_CLOSEOUTPUT, (WPARAM)nOT, 0);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 激活输出窗口
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::ActiveOutput(int nOT)
{
	CPlatUI *pObj = GET_INTERFACE_OBJECT(PlatUI);

	// 修改为通过消息形式传递,发现一些问题，会丢失字符，因此改回去
	PCOM_CMD("", OC_ACTIVEOUTPUT, (WPARAM)nOT, 0);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 输出信息
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::Output(int nOT, LPCTSTR lpszOut)
{
	CPlatUI *pObj = GET_INTERFACE_OBJECT(PlatUI);

	int nLen = strlen(lpszOut)+1;
	//PDCOM_CMD("", OC_OUTPUT, (WPARAM)nOT, lpszOut, nLen);
	DCOM_CMD("", OC_OUTPUT, (WPARAM)nOT, lpszOut, nLen);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 输出信息，用特定颜色
// nFmt的高字的含义:
// 0x10000   - 快速输出
// 0x20000   - 低位的颜色无效(这样就允许在之前调用OutputSet来设置颜色)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::OutputFormat(int nOT, int nFmt, LPCTSTR lpszOut)
{
	CPlatUI *pObj = GET_INTERFACE_OBJECT(PlatUI);

	CString strSet;
	switch(nFmt & 0x0000FFFF)
	{
	case 0: OutputText(nOT, lpszOut, RGB(0,0,0));break;
	case 1: OutputText(nOT, lpszOut, RGB(0,0,255));break;
	case 2: OutputText(nOT, lpszOut, RGB(255,0,0));break;
	case 3: OutputText(nOT, lpszOut, RGB(0,255,0));break;
	case 4: OutputText(nOT, lpszOut, RGB(128,128,128));break;
	default: OutputText(nOT, lpszOut, RGB(0,0,0));
	}
/*
	BOOL bQuickOutput = (nFmt & 0x10000);	// 快速输出标志

	CString strSet;
	switch(nFmt & 0x0000FFFF)
	{
	case 0: strSet = "COLOR=0,0,0";break;
	case 1: strSet = "COLOR=0,0,255";break;
	case 2: strSet = "COLOR=255,0,0";break;
	case 3: strSet = "COLOR=0,255,0";break;
	case 4: strSet = "COLOR=128,128,128";break;
	default: strSet = "COLOR=0,0,0";
	}

	if((nFmt & 0x20000) == 0)	// 是否设置格式标志
	{
		int nLen = strSet.GetLength()+1;
		LPTSTR lpszSet = strSet.GetBuffer(nLen);
		if(bQuickOutput)	// 快速输出,不用PostMessage
			COM_CMD(0, OC_OUTPUTSET, (WPARAM)nOT, lpszSet)
		else
			PDCOM_CMD(0, OC_OUTPUTSET, (WPARAM)nOT, lpszSet, nLen);
		strSet.ReleaseBuffer();
	}
	
	if(bQuickOutput)	// 快速输出,不用PostMessage
	{
		COM_CMD(0, OC_OUTPUT, (WPARAM)nOT, lpszOut);
	}else
	{
		int nLen1 = strlen(lpszOut)+1;
		PDCOM_CMD(0, OC_OUTPUT, (WPARAM)nOT, lpszOut, nLen1);
	}
*/
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 输出信息并和校验字符串比较,不一致的字符显示为红色
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::OutputVerify(int nOT, LPCTSTR lpszOut, LPCTSTR lpszVerify)
{
	CPlatUI *pObj = GET_INTERFACE_OBJECT(PlatUI);

	int nLen = strlen(lpszOut)+1;
	DCOM_CMD("", OC_OUTPUT, (WPARAM)nOT, lpszOut, nLen);	// 暂未调用校验函数

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 设置输出信息格式
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::OutputSet(int nOT, LPCTSTR lpszSet)
{
	int nLen = strlen(lpszSet)+1;
	//PDCOM_CMD("", OC_OUTPUTSET, (WPARAM)nOT, lpszSet, nLen);
	DCOM_CMD("", OC_OUTPUTSET, (WPARAM)nOT, lpszSet, nLen);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 获取输出窗口信息
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::OutputGet(int nOT, LPCTSTR lpszParam)
{
	int nRet;
	RCOM_CMD("", OC_OUTPUTGET, (WPARAM)nOT, (LPARAM)lpszParam, &nRet);
	CString* pStr = (CString*)nRet;
	CString strRet = *pStr;
	delete pStr;

	return strRet;
}

/////////////////////////////////////////////////////////////////////////////
// 清除输出信息
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::OutputClear(int nOT)
{
	// 修改为通过消息形式传递
	//PCOM_CMD("", OC_CLEAROUTPUT, (WPARAM)nOT, 0);
	COM_CMD("", OC_CLEAROUTPUT, (WPARAM)nOT, 0);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 添加树输出窗口内容
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::OutputAddTreeItems(int nOT, CTTreeItemArray_* pItem)
{
	// 修改为通过消息形式传递
	COM_CMD("", OC_TREEOUTPUT, (WPARAM)nOT, (LPARAM)pItem);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 获取CommandBars
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(void*)
CPlatUI::XPlatUI::GetCommandBars()
{
	CMainFrame* pMainFrame = (CMainFrame*)(theApp.GetMainWnd());
	return (void*)(pMainFrame->GetCommandBars());
}

/////////////////////////////////////////////////////////////////////////////
// 添加扩展菜单
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::AddExtMenu(CMenu*& pMenu)
{
	if(pMenu == NULL)
	{
		return FALSE;
	}

	CString strTitle;
	pMenu->GetMenuString(0, strTitle, MF_BYPOSITION);

	BOOL bLoadMenu = FALSE;
	CMainFrame* pMainFrame = (CMainFrame*)(theApp.GetMainWnd());
	CXTPCommandBars* pCommandBars = pMainFrame->GetCommandBars();
	if(pCommandBars)
	{
		CXTPCommandBar* pMenuBar = pCommandBars->GetMenuBar();

		// 查找菜单是否存在,如果存在就不用创建了
		CXTPControls* pControls = pMenuBar->GetControls();
		CXTPControl* pControl = pControls->GetFirst();
		CXTPControl* pControlExt = NULL;
		while(pControl)
		{
			if(pControl->GetCaption() == strTitle)
			{
				pControlExt = pControl;
				break;
			}
			pControls->GetNext(pControl);
		}

		if(!pControlExt)
		{
			pMenuBar->LoadMenu(pMenu, FALSE);
			bLoadMenu = TRUE;

			pControl = pControls->GetFirst();
			while(pControl)
			{
				if(pControl->GetCaption() == strTitle)
				{
					pControlExt = pControl;
					break;
				}
				pControls->GetNext(pControl);
			}

			CExtMenuInfo extMenuInfo;
			extMenuInfo.m_strMainMenu = strTitle;
			extMenuInfo.m_strSubMenu = "";
			pMainFrame->m_arExtMenuInfo.Add(extMenuInfo);
		}

		if(pControlExt)
		{
			// 将扩展菜单移动到工具菜单之前
			CXTPControl* pControlTool = NULL;
			pControl = pControls->GetFirst();
			while(pControl)
			{
				CString strTools = "&Tools";
				if(LANGUAGE_PAGE_CHINESE == theApp.m_curLanguage)
				{
					strTools = "工具(&T)";
				}
				if(pControl->GetCaption() == strTools)
				{
					pControlTool = pControl;
					break;
				}
				pControls->GetNext(pControl);
			}
			if(pControlTool)
			{
				pControls->MoveBefore(pControlExt, pControlTool->GetIndex());
			}
		}
	}

	delete pMenu;
	pMenu = NULL;

	return bLoadMenu;
}

/////////////////////////////////////////////////////////////////////////////
// 删除扩展菜单
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::DelExtMenu(CMenu*& pMenu)
{
	if(pMenu == NULL)
	{
		return FALSE;
	}

	CString strTitle;
	pMenu->GetMenuString(0, strTitle, MF_BYPOSITION);
	delete pMenu;
	pMenu = NULL;

	CMainFrame* pMainFrame = (CMainFrame*)(theApp.GetMainWnd());
	CXTPCommandBars* pCommandBars = pMainFrame->GetCommandBars();
	if(pCommandBars)
	{
		// 查找并删除创建的菜单
		CXTPCommandBar* pMenuBar = pCommandBars->GetMenuBar();
		CXTPControls* pControls = pMenuBar->GetControls();
		CXTPControl* pControl = pControls->GetFirst();
		while(pControl)
		{
			if(pControl->GetCaption() == strTitle)
			{
				pControls->Remove(pControl);
				for(int i=0; i<pMainFrame->m_arExtMenuInfo.GetSize(); i++)
				{
					if(pMainFrame->m_arExtMenuInfo[i].m_strMainMenu == strTitle)
					{
						pMainFrame->m_arExtMenuInfo.RemoveAt(i);
					}
				}
				return TRUE;
			}
			pControls->GetNext(pControl);
		}
	}	

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 添加扩展子菜单
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::AddSubMenu(CString strMainMenu, int nIndex, CMenu*& pMenu, LPVOID pProc, LPVOID pUpdateProc)
{
	CMainFrame* pMainFrame = (CMainFrame*)(theApp.GetMainWnd());
	CXTPCommandBars* pCommandBars = pMainFrame->GetCommandBars();
	if(!pCommandBars)
	{
		return FALSE;		
	}
	CXTPCommandBar* pMenuBar = pCommandBars->GetMenuBar();
	if(!pMenuBar)
	{
		return FALSE;
	}

	CString strMenuItem;
	pMenu->GetSubMenu(0)->GetMenuString(nIndex, strMenuItem, MF_BYPOSITION);
	CXTPControls* pControls = pMenuBar->GetControls();
	CXTPControl* pControl = pControls->GetFirst();
	CXTPCommandBar* pMenuBarSub = NULL;
	CXTPControl* pControlExtSub = NULL;
	while(pControl)
	{
		if(pControl->GetCaption() == strMainMenu)
		{
			pMenuBarSub = pControl->GetCommandBar();
			CXTPControls* pControlsSub = pMenuBarSub->GetControls();
			pControl = pControlsSub->GetFirst();
			while(pControl)
			{
				if(pControl->GetCaption() == strMenuItem)
				{
					pControlExtSub = pControl;
					break;
				}
				pControlsSub->GetNext(pControl);
			}			
			break;
		}
		pControls->GetNext(pControl);
	}
	if(pMenuBarSub && !pControlExtSub)
	{
		CXTPControls* pControlsSub = pMenuBarSub->GetControls();
		CMenu* pmenu = pMenu->GetSubMenu(0);
		if(pmenu)
		{
			pControlsSub->AddMenuItem(pmenu, nIndex);
			CExtMenuInfo extMenuInfo;
			extMenuInfo.m_strMainMenu = strMainMenu;
			extMenuInfo.m_strSubMenu = strMenuItem;
			extMenuInfo.nId = pmenu->GetMenuItemID(nIndex) - IDM_OWMCMD;
			extMenuInfo.pProc = (TYPEOF_ClientMenu*)pProc;
			extMenuInfo.pUpdateProc = (TYPEOF_UpdateClientMenu*)pUpdateProc;
			pMainFrame->m_arExtMenuInfo.Add(extMenuInfo);
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 删除扩展子菜单
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::DelSubMenu(CString strMainMenu, int nIndex, CMenu*& pMenu)
{
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 弹出菜单
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::TrackPopupMenu(CMenu* pMenu, UINT nFlags, int x, int y, CWnd* pWnd)
{
	return CXTPCommandBars::TrackPopupMenu(pMenu, nFlags, x, y, pWnd);
}

/////////////////////////////////////////////////////////////////////////////
// 添加状态条Pane
/////////////////////////////////////////////////////////////////////////////
static int g_nPaneID = 500;	// Status Pane的起始分配ID

STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::AddStatusPane(int& nID, int nWidth)
{
	if((nID>10) && (nID<100))
	{
		PCOM_CMD("", OC_ADDPANE, nID, nWidth);
	}else
	{
		PCOM_CMD("", OC_ADDPANE, g_nPaneID, nWidth);
		nID = g_nPaneID;
		g_nPaneID++;		
	}

	// 注：Status Pane的ID小于500表示是平台保留的,大于500
	// 表示是由平台动态分配给用户的

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 删除状态条Pane
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::DelStatusPane(int nID)
{
	if(nID != 0)
	{
		PCOM_CMD("", OC_REMOVEPANE, nID, 0);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 设置Pane文字
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::SetPaneText(int nID, CString strPane)
{
	int nLen = strPane.GetLength()+1;
	LPTSTR lpszPane = strPane.GetBuffer(nLen);
	PDCOM_CMD("", OC_SETPANETEXT, nID, lpszPane, nLen);
	strPane.ReleaseBuffer();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 设置Pane宽度
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::SetPaneWidth(int nID, int nWidth)
{
	PCOM_CMD("", OC_SETPANEWIDTH, nID, nWidth);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 设置Pane图标
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::SetPaneImage(int nID, int nGroup, int nImage)
{
/*
	PCOM_CMD(0, OC_SETPANEIMAGE, nID, nGroup);
	PCOM_CMD(0, OC_SETPANEIMAGEITEM, nID, nImage);
*/
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 获取当前激活的文档对象
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CDocument*)
CPlatUI::XPlatUI::GetActiveDocument()
{
	CMainFrame* pMainFrame = (CMainFrame*)(theApp.GetMainWnd());
/*	CFrameWnd* pFrame = pMainFrame->GetActiveFrame();
	if(pFrame == NULL)
	{
		return NULL;
	}
	return pFrame->GetActiveDocument();*/
	return pMainFrame->GetActiveDocument();
/*	CMDIChildWnd* pActiveChildWnd = pMainFrame->MDIGetActive();
	if(pActiveChildWnd == NULL)
	{
		return NULL;
	}
	return pActiveChildWnd->GetActiveDocument();*/
	/*
	CView* pView = reinterpret_cast<CView*>(pActiveChildWnd->GetActiveView());
	if(pView == NULL)
	{
		return NULL;
	}
	return pView->GetDocument();*/
}

/////////////////////////////////////////////////////////////////////////////
// 创建DockingPane
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::CreateDockingPane(CString strOwmId, CString strTitle, int nID, int nDirection, int nAttachType, HICON hIcon, int nAttachPane)
{
	CPlugIn* pPlugIn = theApp.GetOWM(strOwmId);
	if(pPlugIn == NULL)
	{
		return FALSE;
	}

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->GetDockingPaneWnd(nID))
	{
		for(int i=0; i<pPlugIn->m_arDockingPane.GetSize(); i++)
		{
			if(pPlugIn->m_arDockingPane[i].m_nId == nID)
			{
				pMainFrame->CreateDockingBar(nID, nDirection, hIcon, nAttachPane);
				return TRUE;
			}
		}

		CXTPDockingPane* pPane = pMainFrame->CreateDockingBar(nID, nDirection, hIcon, nAttachPane);
		if(pPane == NULL)
		{
			return FALSE;
		}

		return TRUE;
	}

	COwmDockingPane paneInfo;
	paneInfo.m_strName = strTitle;
	paneInfo.m_nId = nID;
	paneInfo.m_strOwmId = strOwmId;
	paneInfo.m_nAttachType = nAttachType;

	// 查找这个PaneID看是否有其他PlugIn或Tool中预定义了这个Pane信息,如果定义了,可以用相关信息补全此处的Pane信息
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		for(int j=0; j<theApp.m_aPlugIns[i].m_arDockingPane.GetSize(); j++)
		{
			if( (theApp.m_aPlugIns[i].m_arDockingPane[j].m_nId == nID) &&
				(theApp.m_aPlugIns[i].m_arDockingPane[j].m_bFileDefined) )
			{
				if((theApp.m_aPlugIns[i].m_strType == "project") &&
					!theApp.m_aPlugIns[i].HasLoad() )
				{
					// 工程插件只有加载了才进行判断
					continue;
				}
				paneInfo.m_nDirection = theApp.m_aPlugIns[i].m_arDockingPane[j].m_nDirection;
				paneInfo.m_strIcon = theApp.m_aPlugIns[i].m_arDockingPane[j].m_strIcon;
				if(theApp.m_aPlugIns[i].m_arDockingPane[j].m_strIcon.Find(":") == -1)
				{
					paneInfo.m_strIcon = theApp.m_aPlugIns[i].m_strPath+"\\"+theApp.m_aPlugIns[i].m_arDockingPane[j].m_strIcon;
				}
				paneInfo.m_bAutoCreate = theApp.m_aPlugIns[i].m_arDockingPane[j].m_bAutoCreate;
				paneInfo.m_bSave = theApp.m_aPlugIns[i].m_arDockingPane[j].m_bSave;
			}
		}
	}
	for(int i=0; i<theApp.m_aTools.GetSize(); i++)
	{
		for(int j=0; j<theApp.m_aTools[i].m_arDockingPane.GetSize(); j++)
		{
			if( (theApp.m_aTools[i].m_arDockingPane[j].m_nId == nID) &&
				(theApp.m_aTools[i].m_arDockingPane[j].m_bFileDefined) )
			{
				paneInfo.m_nDirection = theApp.m_aTools[i].m_arDockingPane[j].m_nDirection;
				paneInfo.m_strIcon = theApp.m_aTools[i].m_arDockingPane[j].m_strIcon;
				if(theApp.m_aTools[i].m_arDockingPane[j].m_strIcon.Find(":") == -1)
				{
					paneInfo.m_strIcon = theApp.m_aTools[i].m_strPath+"\\"+theApp.m_aTools[i].m_arDockingPane[j].m_strIcon;
				}
				paneInfo.m_bAutoCreate = theApp.m_aTools[i].m_arDockingPane[j].m_bAutoCreate;
				paneInfo.m_bSave = theApp.m_aTools[i].m_arDockingPane[j].m_bSave;
			}
		}
	}

	pPlugIn->m_arDockingPane.Add(paneInfo);

	return pPlugIn->LoadDockingPane(nID);
}

/////////////////////////////////////////////////////////////////////////////
// 创建DockingPane
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::CreateDockingPane(CString strOwmId, int nID)
{
	CPlugIn* pPlugIn = theApp.GetOWM(strOwmId);
	if(pPlugIn == NULL)
	{
		return FALSE;
	}
	return pPlugIn->LoadDockingPane(nID);
}

/////////////////////////////////////////////////////////////////////////////
// 获取DockingPane包含的窗口对象
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CWnd*)
CPlatUI::XPlatUI::GetDockingPaneWnd(int nPaneID)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	return pMainFrame->GetDockingPaneWnd(nPaneID);
}

/////////////////////////////////////////////////////////////////////////////
// 获取DockingPane ID(根据名字)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(int)
CPlatUI::XPlatUI::GetDockingPaneIdByName(CString strName)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	return pMainFrame->GetDockingPaneIdByName(strName);
}

/////////////////////////////////////////////////////////////////////////////
// 激活DockingPane
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::ActiveDockingPane(int nPaneID, BOOL bShow)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	return pMainFrame->ActiveDockingPane(nPaneID, bShow);
}

/////////////////////////////////////////////////////////////////////////////
// 关闭DockingPane窗口
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::CloseDockingPane(int nPaneID)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	return pMainFrame->CloseDockingPane(nPaneID);
}

/////////////////////////////////////////////////////////////////////////////
// 添加日志
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::AddSysLogInfo(int nType, CString strFrom, CString strDesc)
{
/*
	LOGITEM LogItem;
	LogItem.nLogType = nType;
	LogItem.tTime = CTime::GetCurrentTime().GetTime();
	LogItem.strFrom = strFrom;
	if(theApp.m_CurUser.bLogined)	// 如果已登录就记录登录用户名
	{
		LogItem.strUser = theApp.m_CurUser.strUserName;
	}else
	{
		LogItem.strUser = "------";
	}
	LogItem.strDesc = strDesc;
	
	// 通知平台执行添加操作
	// 0表示发给平台而不是某个OWM,也就是调用CommandPlat
	COM_CMD(0, OC_LOGADDITEM, (WPARAM)(&LogItem), 0);
*/
	// 注:微软的DAO存在一些问题,在多线程环境下使用会出问题,
	// 此处的日志操作用DAO来操作数据库,为了防止出现多线程冲突,
	// 因此将直接调用方式改为发消息方式,将此消息发给MainFrame,
	// 然后通过MainFrame来间接调用
	// 多线程冲突问题在此接口的其他函数中普遍存在,因此需要将所
	// 有接口函数改为通过MainFrame消息代理的方式来操作

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 判断扩展模块是否存在
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::IsOwmExist(CString strOwmId)
{
	return (theApp.GetOWM(strOwmId) != NULL);
}

/////////////////////////////////////////////////////////////////////////////
// 发送扩展模块命令
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::SendOwmCmd(CString strOwmId, int nCmd, WPARAM wParam, LPARAM lParam, LPVOID lpResult)
{
	// 通知平台执行某个操作
	// 输入参数0表示发给平台，而COM_CMD中-1表示发给平台
	if(strOwmId == "0")
	{	// SendMessage方式向平台发消息
		if(lpResult == NULL)
		{
			COM_CMD("0", nCmd, wParam, lParam);
		}else
		{
			RCOM_CMD("0", nCmd, wParam, lParam, lpResult);
		}
	}else
	if(strOwmId == "-1")
	{	// PostMessage方式向平台发消息
		if(lpResult == NULL)
		{
			PCOM_CMD("0", nCmd, wParam, lParam);
		}else
		{
			PRCOM_CMD("0", nCmd, wParam, lParam, lpResult);
		}
	}else
	{
		//BOOL bPost = ((nOwmID & 0xFFFF0000) != 0);
		// 如果模块ID大于65535,说明是Post方式,真正的ID是低16位
		//int nRealOwm = nOwmID&0x0000FFFF;
		BOOL bPost = (strOwmId.Find("post|") == 0);
		if(bPost)
		{
			strOwmId.Delete(0, 5);
		}

		// 如果模块处于动态为驻留状态,调用此函数可以使其驻留,保证后面可以调用
		theApp.GetDynamicOWM(strOwmId);

		if(!bPost)
		{
			// SendMessage方式向某个扩展模块发消息
			if(lpResult == NULL)
			{
				COM_CMD(strOwmId, nCmd, wParam, lParam);
			}else
			{	// 填充返回结果
				RCOM_CMD(strOwmId, nCmd, wParam, lParam, lpResult);
			}
		}else
		{
			// PostMessage方式向某个扩展模块发消息
			if(lpResult == NULL)
			{
				PCOM_CMD(strOwmId, nCmd, wParam, lParam);
			}else
			{	// 填充返回结果
				PRCOM_CMD(strOwmId, nCmd, wParam, lParam, lpResult);
			}
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 加载工程扩展模块
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::LoadOwm(CString strOwmName, int& nID, BOOL bLoad)
{

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 打开文档
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CDocument*)
CPlatUI::XPlatUI::OpenDocumentFile(LPCTSTR lpszFileName, LPCTSTR lpszOwmId)
{
	//return theApp.OpenDocumentFileWithOwm(lpszFileName, lpszOwmId);
	int nRet;
	RCOM_CMD("", OC_OPENDOCUMENTFILE, (WPARAM)(LPCTSTR)lpszFileName, (WPARAM)(LPCTSTR)lpszOwmId, &nRet);
	return (CDocument*)(nRet);
}

/////////////////////////////////////////////////////////////////////////////
// 获取属性树指针
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CWnd*)
CPlatUI::XPlatUI::GetPropertyGridWnd()
{
	return &GetPropertyGrid(AfxGetMainWnd());
}

/////////////////////////////////////////////////////////////////////////////
// 判断是否可以更新属性树
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::EnableUpdatePropertyGridContent()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	return pMainFrame->m_bEnableUpdatePropertyGrid;
}

/////////////////////////////////////////////////////////////////////////////
// 刷新属性窗口内容
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(void)
CPlatUI::XPlatUI::UpdatePropertyGridContent()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->UpdatePropertyGridContent();
}

/////////////////////////////////////////////////////////////////////////////
// 开始属性窗口刷新
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(void)
CPlatUI::XPlatUI::BeginUpdatePropertyGridContent()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->BeginUpdatePropertyGridContent();
}

/////////////////////////////////////////////////////////////////////////////
// 结束属性窗口刷新
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(void)
CPlatUI::XPlatUI::EndUpdatePropertyGridContent()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->EndUpdatePropertyGridContent();
}

/////////////////////////////////////////////////////////////////////////////
// 创建一个组件对象
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(void*)
CPlatUI::XPlatUI::CreateVciObject(CString strVciId, CString strInstName)
{
	return theApp.CreateVciObject(strVciId, strInstName);
}

/////////////////////////////////////////////////////////////////////////////
// 关闭一个组件对象
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::ReleaseVciObject(void* pObject, BOOL bForceDelete)
{
	return theApp.ReleaseObject(pObject, bForceDelete);
}

/////////////////////////////////////////////////////////////////////////////
// 关闭一个组件对象(指定实例名)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::ReleaseObjectByInstanceName(CString strInstName)
{
	return theApp.ReleaseObjectByInstanceName(strInstName);
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个组件对象(根据实例名)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(void*)
CPlatUI::XPlatUI::GetObjectByInstanceName(CString strInstName)
{
	return theApp.GetObjectByInstanceName(strInstName);
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个组件对象控制接口(根据组件实例接口指针)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(void*)
CPlatUI::XPlatUI::GetObjectControlPtrByInstancePtr(LPVOID pVciObj)
{
	return theApp.GetObjectControlPtrByInstancePtr(pVciObj);
}

/////////////////////////////////////////////////////////////////////////////
// 获取所有安装的组件ID(可以指定组件类型)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(int)
CPlatUI::XPlatUI::GetInstalledVci(CStringArray& asVciId)
{
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		asVciId.Add(theApp.m_aPlugIns[i].m_strId);
	}
	return asVciId.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个组件名(根据组件ID)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::GetVciName(CString strId)
{
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if(theApp.m_aPlugIns[i].m_strId == strId)
		{
			return theApp.m_aPlugIns[i].m_strName;
		}
	}
	return "";
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个组件路径(根据组件ID)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::GetVciPath(CString strId)
{
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if(theApp.m_aPlugIns[i].m_strId == strId)
		{
			return theApp.m_aPlugIns[i].m_strPath;
		}
	}
	return "";
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个组件版本(根据组件ID)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::GetVciVersion(CString strId)
{
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if(theApp.m_aPlugIns[i].m_strId == strId)
		{
			return theApp.m_aPlugIns[i].m_strVersion;
		}
	}
	return "";
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个组件开发商信息(根据组件ID)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::GetVciProvider(CString strId)
{
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if(theApp.m_aPlugIns[i].m_strId == strId)
		{
			return theApp.m_aPlugIns[i].m_strProvider;
		}
	}
	return "";
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个组件类型(根据组件ID)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::GetVciType(CString strId)
{
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if(theApp.m_aPlugIns[i].m_strId == strId)
		{
			return theApp.m_aPlugIns[i].m_strType;
		}
	}
	return "";
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个组件类接口ID(根据组件ID)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::GetVciClass(CString strId)
{
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if(theApp.m_aPlugIns[i].m_strId == strId)
		{
			return theApp.m_aPlugIns[i].m_strClass;
		}
	}
	return "";
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个组件接口定义文件(根据组件ID)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::GetVciInterfaceFile(CString strId)
{
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if(theApp.m_aPlugIns[i].m_strId == strId)
		{
			return theApp.m_aPlugIns[i].m_strInterfaceFile;
		}
	}
	return "";
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个组件编译时间(根据组件ID)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CTime)
CPlatUI::XPlatUI::GetVciBuildTime(CString strId)
{
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if(theApp.m_aPlugIns[i].m_strId == strId)
		{
			return theApp.m_aPlugIns[i].GetPlugInBuildTime();
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个组件GUID(根据组件ID)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::GetVciGUID(CString strId)
{
	CString strVciId = "";
	int nPos = strId.Find("##");
	if(nPos != -1)
	{
		strVciId = strId;
		strVciId.Delete(0, nPos+2);
		strId = strId.Left(nPos);
	}
	if(strVciId != strId)
	{
		return "";
	}

	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if(theApp.m_aPlugIns[i].m_strId == strId)
		{
			return theApp.m_aPlugIns[i].GetPlugInGUID();
		}
	}
	return "";
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个组件关于文件位置(根据组件ID)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::GetVciAboutPage(CString strId)
{
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if(theApp.m_aPlugIns[i].m_strId == strId)
		{
			return theApp.m_aPlugIns[i].m_strAboutPage;
		}
	}
	return "";
}

/////////////////////////////////////////////////////////////////////////////
// 获取可以执行一个文档的扩展模块ID列表(根据组件Id和文档Id)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(int)
CPlatUI::XPlatUI::GetVciDocExecuteModule(CString strId, CString strDocId, CString strExecuteType, CStringArray& arExecuteModule)
{
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if(theApp.m_aPlugIns[i].m_strId == strId)
		{
			for(int j=0; j<theApp.m_aPlugIns[i].m_arDocExtend.GetSize(); j++)
			{
				if(theApp.m_aPlugIns[i].m_arDocExtend[j].m_strDocId == strDocId)
				{
					for(int k=0; k<theApp.m_aPlugIns[i].m_arDocExtend[j].m_arExecuteModule.GetSize(); k++)
					{
						if(strExecuteType == theApp.m_aPlugIns[i].m_arDocExtend[j].m_arExecuteModuleType[k])
						{
							arExecuteModule.Add(theApp.m_aPlugIns[i].m_arDocExtend[j].m_arExecuteModule[k]);
						}
					}
					return arExecuteModule.GetSize();
				}
			}

			return 0;
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 获取文档的图标(根据组件Id,文档Id,图标类型)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::GetVciDocIcon(CString strId, CString strDocId, CString strIconType)
{
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if(theApp.m_aPlugIns[i].m_strId == strId)
		{
			for(int j=0; j<theApp.m_aPlugIns[i].m_arDocExtend.GetSize(); j++)
			{
				if(theApp.m_aPlugIns[i].m_arDocExtend[j].m_strDocId == strDocId)
				{
					for(int k=0; k<theApp.m_aPlugIns[i].m_arDocExtend[j].m_arDocIconType.GetSize(); k++)
					{
						if(strIconType == theApp.m_aPlugIns[i].m_arDocExtend[j].m_arDocIconType[k])
						{
							return theApp.m_aPlugIns[i].m_arDocExtend[j].m_arDocIconFile.GetAt(k);
						}
					}
					if(strIconType == "normal")
					{
						return theApp.m_aPlugIns[i].m_arDocExtend[j].m_strIcon;
					}
					return "";
				}
			}
			return "";
		}
	}
	return "";
}

/////////////////////////////////////////////////////////////////////////////
// 设置文档的图标(根据组件Id,文档Id,图标类型,窗口句柄)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(void)
CPlatUI::XPlatUI::SetVciDocIcon(CString strId, CString strDocId, CString strIconType, HWND hWnd)
{
	CString strIconPath = GetVciPath(strId);
	CString strIconFile = GetVciDocIcon(strId, strDocId, strIconType);
	if(strIconFile != "")
	{
		strIconFile = strIconPath + "\\" + strIconFile;
		if(GetFileAttributes(strIconFile) != 0xFFFFFFFF)
		{
			COM_CMD("", OC_SETDOCICON, (WPARAM)(LPCTSTR)strIconFile, (LPARAM)hWnd);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 设置文档的标题(根据窗口句柄)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(void)
CPlatUI::XPlatUI::SetDocumentTitle(CString strTitle, int nModifyType, HWND hWnd)
{
	if(strTitle != "")
	{
		COM_CMD("", OC_SETDOCTITLE, (WPARAM)(LPCTSTR)strTitle, (LPARAM)hWnd);
	}else
	{
		COM_CMD("", OC_MODIFYDOCTITLE, nModifyType, (LPARAM)hWnd);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个组件的所有实例对象指针
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(int)
CPlatUI::XPlatUI::GetVciObjects(CString strId, CPtrArray& auVciObj)
{
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if(theApp.m_aPlugIns[i].m_strId == strId)
		{
			for(int j=0; j<theApp.m_aPlugIns[i].m_aVciObj.GetSize(); j++)
			{
				auVciObj.Add(theApp.m_aPlugIns[i].m_aVciObj[j].m_pVciObj);
			}
			return theApp.m_aPlugIns[i].m_aVciObj.GetSize();
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个组件实例对象的实例名
// strId = 0表示搜索所有组件
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::GetInstanceNameByObject(CString strId, LPVOID pVciObj)
{
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if((strId != "") && (theApp.m_aPlugIns[i].m_strId == strId))
		{
			for(int j=0; j<theApp.m_aPlugIns[i].m_aVciObj.GetSize(); j++)
			{
				if(theApp.m_aPlugIns[i].m_aVciObj[j].m_pVciObj == pVciObj)
				{
					return theApp.m_aPlugIns[i].m_aVciObj[j].m_strInstName;
				}
			}
			return "";
		}else
		if(strId == "")
		{
			for(int j=0; j<theApp.m_aPlugIns[i].m_aVciObj.GetSize(); j++)
			{
				if(theApp.m_aPlugIns[i].m_aVciObj[j].m_pVciObj == pVciObj)
				{
					return theApp.m_aPlugIns[i].m_aVciObj[j].m_strInstName;
				}
			}
		}
	}
	return "";
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个组件实例对象的引用计数
// strId = 0表示搜索所有组件
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(int)
CPlatUI::XPlatUI::GetInstanceRefCount(CString strId, LPVOID pVciObj)
{
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if((strId != "") && (theApp.m_aPlugIns[i].m_strId == strId))
		{
			for(int j=0; j<theApp.m_aPlugIns[i].m_aVciObj.GetSize(); j++)
			{
				if(theApp.m_aPlugIns[i].m_aVciObj[j].m_pVciObj == pVciObj)
				{
					return theApp.m_aPlugIns[i].m_aVciObj[j].m_nRefCount;
				}
			}
			return 0;
		}else
		if(strId == "")
		{
			for(int j=0; j<theApp.m_aPlugIns[i].m_aVciObj.GetSize(); j++)
			{
				if(theApp.m_aPlugIns[i].m_aVciObj[j].m_pVciObj == pVciObj)
				{
					return theApp.m_aPlugIns[i].m_aVciObj[j].m_nRefCount;
				}
			}
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 枚举一个VCI组件的扩展参数
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(int)
CPlatUI::XPlatUI::EnumVciParameters(CString strId, CStringArray& asParamName)
{
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if(theApp.m_aPlugIns[i].m_strId == strId)
		{
			return theApp.m_aPlugIns[i].EnumParameters(asParamName);
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个VCI组件的参数
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::GetVciParameter(CString strId, CString strName, CString& strValue, CString& strDefaultValue)
{
	if(strId == "")
	{
		// 获取平台参数
		strValue = theApp.m_xmlPlat.GetNodeText("application\\" + strName);
		strDefaultValue = strValue;
		return TRUE;
	}

	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if(theApp.m_aPlugIns[i].m_strId == strId)
		{
			return theApp.m_aPlugIns[i].GetParameter(strName, strValue, strDefaultValue);
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 设置一个VCI组件的参数
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::SetVciParameter(CString strId, CString strName, CString strValue)
{
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if(theApp.m_aPlugIns[i].m_strId == strId)
		{
			return theApp.m_aPlugIns[i].SetParameter(strName, strValue);
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 添加定时器任务
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(void*)
CPlatUI::XPlatUI::AddTimerTask(CString strTaskName, CString strInterpId, CString strInterpName, CString strCommand, int nSeconds, BOOL bStart)
{
	return theApp.AddTimerTask(strTaskName, strInterpId, strInterpName, strCommand, nSeconds, bStart);
}

/////////////////////////////////////////////////////////////////////////////
// 添加定时器任务
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(void*)
	CPlatUI::XPlatUI::AddTimerTask(CString strTaskName, LPVOID lpFunction, int nSeconds, BOOL bStart)
{
	return theApp.AddTimerTask(strTaskName, lpFunction, nSeconds, bStart);
}

/////////////////////////////////////////////////////////////////////////////
// 根据任务名获取定时任务指针
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(void*)
CPlatUI::XPlatUI::GetTimerTask(CString strTaskName)
{
	return theApp.FindTimerTask(strTaskName);
}

/////////////////////////////////////////////////////////////////////////////
// 删除定时任务(根据任务指针)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::DeleteTimerTask(LPVOID lpTimerTask)
{
	return theApp.DeleteTimerTask((CTimerTask*)lpTimerTask);
}

/////////////////////////////////////////////////////////////////////////////
// 删除定时任务(根据任务名)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::DeleteTimerTask(CString strTaskName)
{
	return theApp.DeleteTimerTask(strTaskName);
}

/////////////////////////////////////////////////////////////////////////////
// 启动定时任务
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(void)
CPlatUI::XPlatUI::StartTimerTask(LPVOID lpTimerTask)
{
	CTimerTask* pTimer = (CTimerTask*)lpTimerTask;
	if(pTimer)
	{
		pTimer->StartTask();
	}
}

/////////////////////////////////////////////////////////////////////////////
// 设置定时任务开始时间
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(void)
CPlatUI::XPlatUI::SetTimerStartTime(LPVOID lpTimerTask, int tStart)
{
	CTimerTask* pTimer = (CTimerTask*)lpTimerTask;
	if(pTimer)
	{
		pTimer->SetStartTime(CTime(tStart));
	}
}

/////////////////////////////////////////////////////////////////////////////
// 设置定时任务结束时间
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(void)
CPlatUI::XPlatUI::SetTimerEndTime(LPVOID lpTimerTask, int tEnd)
{
	CTimerTask* pTimer = (CTimerTask*)lpTimerTask;
	if(pTimer)
	{
		pTimer->SetEndTime(CTime(tEnd));
	}
}

/////////////////////////////////////////////////////////////////////////////
// 设置定时任务触发条件脚本
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(void)
CPlatUI::XPlatUI::SetTimerTrigger(LPVOID lpTimerTask, CString strTrigger)
{
	CTimerTask* pTimer = (CTimerTask*)lpTimerTask;
	if(pTimer)
	{
		pTimer->SetTrigger(strTrigger);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 设置定时任务执行次数
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(void)
CPlatUI::XPlatUI::SetTimerExecCount(LPVOID lpTimerTask, int nCount)
{
	CTimerTask* pTimer = (CTimerTask*)lpTimerTask;
	if(pTimer)
	{
		pTimer->SetExecCount(nCount);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 获取所有安装的工具ID
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(int)
CPlatUI::XPlatUI::GetInstalledTool(CStringArray& asToolId)
{
	for(int i=0; i<theApp.m_aTools.GetSize(); i++)
	{
		asToolId.Add(theApp.m_aTools[i].m_strId);
	}
	return asToolId.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个工具名(根据工具ID)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::GetToolName(CString strId)
{
	for(int i=0; i<theApp.m_aTools.GetSize(); i++)
	{
		if(theApp.m_aTools[i].m_strId == strId)
		{
			return theApp.m_aTools[i].m_strName;
		}
	}
	return "";
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个工具路径(根据工具ID)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::GetToolPath(CString strId)
{
	for(int i=0; i<theApp.m_aTools.GetSize(); i++)
	{
		if(theApp.m_aTools[i].m_strId == strId)
		{
			return theApp.m_aTools[i].m_strPath;
		}
	}
	return "";
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个工具版本(根据工具ID)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::GetToolVersion(CString strId)
{
	for(int i=0; i<theApp.m_aTools.GetSize(); i++)
	{
		if(theApp.m_aTools[i].m_strId == strId)
		{
			return theApp.m_aTools[i].m_strVersion;
		}
	}
	return "";
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个工具开发商信息(根据工具ID)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(CString)
CPlatUI::XPlatUI::GetToolProvider(CString strId)
{
	for(int i=0; i<theApp.m_aTools.GetSize(); i++)
	{
		if(theApp.m_aTools[i].m_strId == strId)
		{
			return theApp.m_aTools[i].m_strProvider;
		}
	}
	return "";
}

/////////////////////////////////////////////////////////////////////////////
// 运行工具(根据工具ID)
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(BOOL)
CPlatUI::XPlatUI::RunTool(CString strId)
{
	for(int i=0; i<theApp.m_aTools.GetSize(); i++)
	{
		if(theApp.m_aTools[i].m_strId == strId)
		{
			return theApp.m_aTools[i].Run();
		}
	}
	return FALSE;
}