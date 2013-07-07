////////////////////////////////////////////////////////////////////////////
//	File:		PlugIn.cpp
//	Version:	1.0.0.0
//	Created:	2005-07-08
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	PlugIn implement
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlueUI.h"
#include "PlugIn.h"
#include "MainFrm.h"
#include "resource.h"
#include "TaskDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 查询命令是否支持
int gQueryCommandIsSupport(LPCSTR lpcsCmd)
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// 文档扩展点信息类
//////////////////////////////////////////////////////////////////////////
COwmDocExtend::COwmDocExtend()
{
	m_strDocName = "";
	m_strDocId = "";
	m_strOwmId = "";
	m_strIcon = "";
	m_bEnableLink = TRUE;
}

COwmDocExtend::~COwmDocExtend()
{
}


//////////////////////////////////////////////////////////////////////////
// 透视图信息类
//////////////////////////////////////////////////////////////////////////
COwmPerspective::COwmPerspective()
{
	m_strName = "";
	m_strId = "";
	m_strOwmId = "";
	m_strViewName = "";
}

COwmPerspective::~COwmPerspective()
{
}

//////////////////////////////////////////////////////////////////////////
// VCI扩展参数类
//////////////////////////////////////////////////////////////////////////
CVciParameter::CVciParameter()
{
	m_strName = "";
	m_strValue = "";
	m_strDefaultValue = "";
}

CVciParameter::~CVciParameter()
{
}

//////////////////////////////////////////////////////////////////////////
// VCI扩展菜单类
//////////////////////////////////////////////////////////////////////////
CVciMenu::CVciMenu()
{
	m_strMainMenu	= "";
	m_strSubMenu	= "";
	m_strAction		= "";
}

CVciMenu::~CVciMenu()
{
}

//////////////////////////////////////////////////////////////////////////
// 插件信息类
//////////////////////////////////////////////////////////////////////////
CPlugIn::CPlugIn()
{
	m_nLoadState = PLUGIN_UNLOAD;
	m_strPath = "";
	m_nOwmId = 0;
	m_strId = "";
	m_hVciHandle = NULL;
	m_pVciObj = NULL;
	m_pIVciControl = NULL;
	m_nRefCount = 0;
	m_nType = 0;
	m_bAutoCreate = FALSE;
	m_strAutoCreateInstanceName = "";
	m_strClass = "";
	m_strClassVersion = "";
	m_strProcess = "";
	m_strFile = "";
	m_strInterfaceFile = "";
	m_pDocTemplate = NULL;
	m_pControlBar = NULL;
	m_strInitParam = "";
	m_strDoneParam = "";
	m_strAboutPage = "";
}

CPlugIn::CPlugIn(CString strPath)
{
	m_nLoadState = PLUGIN_UNLOAD;
	m_strPath = strPath;
	m_nOwmId = 0;
	m_strId = "";
	m_hVciHandle = NULL;
	m_pVciObj = NULL;
	m_pIVciControl = NULL;
	m_nRefCount = 0;
	m_nType = 0;
	m_bAutoCreate = FALSE;
	m_strAutoCreateInstanceName = "";
	m_strClass = "";
	m_strClassVersion = "";
	m_strProcess = "";
	m_strFile = "";
	m_strInterfaceFile = "";
	m_pDocTemplate = NULL;
	m_pControlBar = NULL;
	m_strInitParam = "";
	m_strDoneParam = "";
	m_strAboutPage = "";
}

CPlugIn::~CPlugIn()
{
}

/////////////////////////////////////////////////////////////////////////////
// 加载插件信息
/////////////////////////////////////////////////////////////////////////////
BOOL CPlugIn::LoadInfo()
{
	// 已经加载过信息,就不用重复加载了
	if(GetLoadState() >= PLUGIN_LOAD_INFO)
	{
		return TRUE;
	}

	CString strXmlFile = m_strPath;
	if(LANGUAGE_PAGE_CHINESE == theApp.m_curLanguage)
	{
		strXmlFile += "\\plugin_cn.xml";
		// 如果不存在中文的配置文件,就用缺省的
		if(GetFileAttributes(strXmlFile) == 0xFFFFFFFF)
		{
			strXmlFile = m_strPath + "\\plugin.xml";
		}
	}else
	{
		strXmlFile += "\\plugin.xml";
	}
	CXmlParser parser;
	if(parser.Open(strXmlFile))
	{
		OUTPUT(COLOR_ERROR, "Load %s fail!\r\n", strXmlFile);
		return FALSE;
	}

	DOMNode* pNode = parser.GetRootNode();
	m_strId = parser.GetNodeAttribute(pNode, "id");
	m_nOwmId = atoi(parser.GetNodeAttribute(pNode, "owmid"));
	m_strName = parser.GetNodeAttribute(pNode, "name");
	m_strVersion = parser.GetNodeAttribute(pNode, "version");
	m_strProvider = parser.GetNodeAttribute(pNode, "provider-name");
	m_strType = parser.GetNodeAttribute(pNode, "type");
	m_strClass = parser.GetNodeAttribute(pNode, "class");
	m_strClassVersion = parser.GetNodeAttribute(pNode, "class-version");
	m_strProcess = parser.GetNodeAttribute(pNode, "process");

	// 读取运行信息
	DOMNode* pSubNode = parser.GetChildNode(pNode, "runtime");
	m_strInitParam = parser.GetNodeText(parser.GetChildNode(pSubNode, "init"));
	m_strDoneParam = parser.GetNodeText(parser.GetChildNode(pSubNode, "done"));
	m_strInterfaceFile = parser.GetNodeText(parser.GetChildNode(pSubNode, "interface-define"));
	m_strInterfaceFile = m_strPath + "\\" + m_strInterfaceFile;
	m_strInterfaceFile.Replace("/", "\\");
	#ifdef _DEBUG
	pSubNode = parser.GetChildNode(pSubNode, "library-debug");
	#else
	pSubNode = parser.GetChildNode(pSubNode, "library");
	#endif
	CString strLibrary = parser.GetNodeAttribute(pSubNode, "name");
	m_strFile = m_strPath + "\\" + strLibrary;
	m_strFile.Replace("/", "\\");	

	// 读取OWM属性信息
	m_nType = 0;
	DOMNode* pNodeProperty = parser.GetChildNode(pNode, "property");
	if(IsVciOwm())
	{
		// OWM的属性
		pSubNode = parser.GetChildNode(pNodeProperty, "owm-type");
		if(parser.GetNodeAttribute(pSubNode, "doc") == "true")
		{
			m_nType |= OWM_DOC;
		}
		if(parser.GetNodeAttribute(pSubNode, "dockbar") == "true")
		{
			m_nType |= OWM_CONTROLBAR;
		}
		if(parser.GetNodeAttribute(pSubNode, "menu") == "true")
		{
			m_nType |= OWM_MENU;
		}
		if(parser.GetNodeAttribute(pSubNode, "toolbar") == "true")
		{
			m_nType |= OWM_TOOLBAR;
		}
		if(parser.GetNodeAttribute(pSubNode, "option") == "true")
		{
			m_nType |= OWM_OPTION;
		}
		if(parser.GetNodeAttribute(pSubNode, "oneframe") == "true")
		{
			m_nType |= OWM_ONEFRAME;
		}
		if(parser.GetNodeAttribute(pSubNode, "autocreate") == "true")
		{
			m_nType |= OWM_AUTOCREATE;
		}
		if(parser.GetNodeAttribute(pSubNode, "dynload") == "true")
		{
			m_nType |= OWM_DYNLOAD;
		}
		if(parser.GetNodeAttribute(pSubNode, "broadcast") == "true")
		{
			m_nType |= OWM_BROADCAST;
		}
		if(parser.GetNodeAttribute(pSubNode, "maxwindow") == "true")
		{
			m_nType |= OWM_MAXWINDOW;
		}
	}else
	{
		// VCI属性
		pSubNode = parser.GetChildNode(pNodeProperty, "autocreate");
		m_bAutoCreate = (parser.GetNodeText(pSubNode) == "true");
		m_strAutoCreateInstanceName = parser.GetNodeAttribute(pSubNode, "instancename");
	}

	// 读取关于页面的位置
	pSubNode = parser.GetChildNode(pNode, "url");
	pSubNode = parser.GetChildNode(pSubNode, "about");
	m_strAboutPage = parser.GetNodeAttribute(pSubNode, "url");

	// 读取文档扩展点信息
	pSubNode = parser.GetNode("doc-extend\\doc");
	while (pSubNode != NULL)
	{
		COwmDocExtend docExtend;

		docExtend.m_strDocName = parser.GetNodeAttribute(pSubNode, "name");
		docExtend.m_strDocId = parser.GetNodeAttribute(pSubNode, "id");
		docExtend.m_strOwmId = m_strId;
		docExtend.m_strIcon = parser.GetNodeAttribute(pSubNode, "icon");
		docExtend.m_bEnableLink = (parser.GetNodeAttribute(pSubNode, "enablelink") != "false");

		// 后缀
		DOMNode* pNodeExts = parser.GetChildNode(pSubNode, "exts");
		if(pNodeExts)
		{
			DOMNode* pNodeExt = parser.GetChildNode(pNodeExts, "ext");
			while (pNodeExt != NULL)
			{
				docExtend.m_arDocExt.Add(parser.GetNodeText(pNodeExt));
				pNodeExt = parser.GetNextNode(pNodeExt, "ext");
			}
		}

		// 执行器
		DOMNode* pNodeExecutes = parser.GetChildNode(pSubNode, "executes");
		if(pNodeExecutes)
		{
			DOMNode* pNodeExecute = parser.GetChildNode(pNodeExecutes);
			while (pNodeExecute != NULL)
			{
				docExtend.m_arExecuteModuleType.Add(parser.GetNodeName(pNodeExecute));
				docExtend.m_arExecuteModule.Add(parser.GetNodeText(pNodeExecute));
				pNodeExecute = parser.GetNextNode(pNodeExecute);
			}
			/*
			DOMNode* pNodeExecuteOwm = parser.GetChildNode(pNodeExecutes, "owm");
			while (pNodeExecuteOwm != NULL)
			{
				docExtend.m_arExecuteModule.Add(parser.GetNodeText(pNodeExecuteOwm));
				pNodeExecuteOwm = parser.GetNextNode(pNodeExecuteOwm, "owm");
			}
			*/
		}

		// 文档图标
		DOMNode* pNodeIcons = parser.GetChildNode(pSubNode, "icons");
		if(pNodeIcons)
		{
			DOMNode* pNodeIcon = parser.GetChildNode(pNodeIcons);
			while (pNodeIcon != NULL)
			{
				docExtend.m_arDocIconType.Add(parser.GetNodeName(pNodeIcon));
				docExtend.m_arDocIconFile.Add(parser.GetNodeText(pNodeIcon));
				pNodeIcon = parser.GetNextNode(pNodeIcon);
			}
		}

		m_arDocExtend.Add(docExtend);

		pSubNode = parser.GetNextNode(pSubNode, "doc");
	}

	// 读取DockingPane信息
	pSubNode = parser.GetNode("dockingpanes\\pane");
	while (pSubNode != NULL)
	{
		COwmDockingPane paneInfo;
		paneInfo.m_bFileDefined = TRUE;	// 配置文件中定义的DockingPane

		paneInfo.m_strName = parser.GetNodeAttribute(pSubNode, "name");
		paneInfo.m_nId = atoi(parser.GetNodeAttribute(pSubNode, "id"));
		CString strDirection = parser.GetNodeAttribute(pSubNode, "direction");
		if(strDirection == "left")
		{
			paneInfo.m_nDirection = xtpPaneDockLeft;
		}else
		if(strDirection == "right")
		{
			paneInfo.m_nDirection = xtpPaneDockRight;
		}else
		if(strDirection == "top")
		{
			paneInfo.m_nDirection = xtpPaneDockTop;
		}else
		if(strDirection == "bottom")
		{
			paneInfo.m_nDirection = xtpPaneDockBottom;
		}else
		{
			paneInfo.m_nDirection = xtpPaneDockLeft;
		}
		paneInfo.m_strIcon = parser.GetNodeAttribute(pSubNode, "icon");
		CString strAttachType = parser.GetNodeAttribute(pSubNode, "type");
		if(strAttachType == "window")
		{
			paneInfo.m_nAttachType = PANE_ATTACH_WND;
		}else
		if(strAttachType == "view")
		{
			paneInfo.m_nAttachType = PANE_ATTACH_VIEW;
		}else
		{
			paneInfo.m_nAttachType = PANE_ATTACH_WND;
		}
		paneInfo.m_bAutoCreate = (parser.GetNodeAttribute(pSubNode, "autocreate") == "true");
		paneInfo.m_bSave = (parser.GetNodeAttribute(pSubNode, "save") == "true");
		paneInfo.m_strOwmId = parser.GetNodeAttribute(pSubNode, "owm");

		m_arDockingPane.Add(paneInfo);

		pSubNode = parser.GetNextNode(pSubNode, "pane");
	}	

	// 读取透视图信息

	// 读取扩展参数信息
	pSubNode = parser.GetNode("parameters\\parameter");
	while (pSubNode != NULL)
	{
		CVciParameter parameterInfo;

		parameterInfo.m_strName = parser.GetNodeAttribute(pSubNode, "name");
		parameterInfo.m_strValue = parser.GetNodeAttribute(pSubNode, "value");
		parameterInfo.m_strDefaultValue = parser.GetNodeAttribute(pSubNode, "defaultName");

		m_arParameter.Add(parameterInfo);

		pSubNode = parser.GetNextNode(pSubNode, "parameter");
	}

	// 读取扩展菜单信息
	pSubNode = parser.GetNode("menus\\menu");
	while (pSubNode != NULL)
	{
		CVciMenu menuInfo;

		menuInfo.m_strMainMenu = parser.GetNodeAttribute(pSubNode, "mainmenu");
		menuInfo.m_strSubMenu = parser.GetNodeAttribute(pSubNode, "submenu");
		menuInfo.m_strAction = parser.GetNodeText(pSubNode);

		m_arMenu.Add(menuInfo);

		if(m_strType == "owm")
		{
			// OWM插件在此处就加载菜单
			AddOwmSubMenu(menuInfo.m_strMainMenu, menuInfo.m_strSubMenu, menuInfo.m_strAction);
		}

		pSubNode = parser.GetNextNode(pSubNode, "menu");
	}

	m_nLoadState = PLUGIN_LOAD_INFO;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 显示About对话框
/////////////////////////////////////////////////////////////////////////////
void CPlugIn::ShowAboutBox()
{
	TTaskDialogInfo taskDialogInfo;
	taskDialogInfo.strWindowTitle = m_strName;
	taskDialogInfo.strMainInstruction = m_strName;
	CString strContent;
	strContent = "Version : " + m_strVersion + "\n";
	strContent += "Author : ";
	strContent += m_strProvider;
	strContent += "\n";

	CString strXmlFile = m_strPath;
	if(LANGUAGE_PAGE_CHINESE == theApp.m_curLanguage)
	{
		strXmlFile += "\\plugin_cn.xml";
		// 如果不存在中文的配置文件,就用缺省的
		if(GetFileAttributes(strXmlFile) == 0xFFFFFFFF)
		{
			strXmlFile = m_strPath + "\\plugin.xml";
		}
	}else
	{
		strXmlFile += "\\plugin.xml";
	}
	CXmlParser parser;
	if(parser.Open(strXmlFile) == 0)
	{
		strContent += "License : ";
		CString strLicense = parser.GetNodeText("license");
		strLicense.TrimLeft();
		strLicense.TrimRight();
		strContent += strLicense;
		strContent += "\n";
		CString strDesc = parser.GetNodeText("description");
		strDesc.TrimLeft();
		strDesc.TrimRight();
		strContent += strDesc;
		strContent += "\n";
		CString strCopyright = parser.GetNodeText("copyright");
		strCopyright.TrimLeft();
		strCopyright.TrimRight();
		strContent += strCopyright;
	}
	taskDialogInfo.strContent = strContent;
	taskDialogInfo.strMainIcon = "Information";
	theApp.DoTaskDialog(&taskDialogInfo);

}

/////////////////////////////////////////////////////////////////////////////
// 获取组件的编译时间
/////////////////////////////////////////////////////////////////////////////
CTime CPlugIn::GetPlugInBuildTime()
{
	typedef int (*FNGetInterfaceList)(TInterfaceInfo **pInfo);
	// 获取函数指针
	FNGetInterfaceList fnGetInterfaceList = (FNGetInterfaceList)GetProcAddress(m_hVciHandle, "GetInterfaceList");
	if(fnGetInterfaceList)
	{
		TInterfaceInfo *pInfo = NULL;
		int nInterfaceCount = fnGetInterfaceList(&pInfo);
		if(pInfo != NULL)
		{
			CString strBuildDate = pInfo->csDate;
			COleDateTime dtTime;
			dtTime.ParseDateTime(strBuildDate);

			SYSTEMTIME st;
			dtTime.GetAsSystemTime(st);
			CTime tTime(st);
			return tTime;
		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 获取组件的GUID
/////////////////////////////////////////////////////////////////////////////
CString CPlugIn::GetPlugInGUID()
{
	typedef int (*FNGetInterfaceList)(TInterfaceInfo **pInfo);
	// 获取函数指针
	FNGetInterfaceList fnGetInterfaceList = (FNGetInterfaceList)GetProcAddress(m_hVciHandle, "GetInterfaceList");
	if(fnGetInterfaceList)
	{
		TInterfaceInfo *pInfo = NULL;
		int nInterfaceCount = fnGetInterfaceList(&pInfo);
		if(pInfo != NULL)
		{
			return pInfo->csGuid;
		}
	}

	return "";
}

/////////////////////////////////////////////////////////////////////////////
// OWM扩展菜单命令处理函数
/////////////////////////////////////////////////////////////////////////////
void OnOwmExtMenuProc(int nID) 
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CExtMenuInfo* pMenuInfo = pMainFrame->GetExtMenuInfo(nID);
	if(pMenuInfo)
	{
		CPlugIn* pPlugIn = theApp.GetDynamicOWM(pMenuInfo->m_strVciId);
		if(pPlugIn == NULL)
		{
			return;
		}

		if(!(pPlugIn->IsVciOwm()))
		{
			return;
		}

		IVciOwm* pIVciOwm = pPlugIn->GetIVciOwm();
		if(pIVciOwm == NULL)
		{
			// 如果OWM组件尚未加载,则先加载
			pPlugIn->LoadOwmPlugIn();
		}

		if(pMenuInfo->m_strAction == "showview")
		{
			// 显示OWM窗口
			pPlugIn->ShowOwmView(TRUE, TRUE);
		}else
		if(pMenuInfo->m_strAction.Find("showpane:") == 0)
		{
			// 显示OWM Docking Pane
			CString strPaneId = pMenuInfo->m_strAction;
			strPaneId.Delete(0, 9);
			int nPaneId = atoi(strPaneId);
			pPlugIn->LoadDockingPane(nPaneId);
		}else
		{
		}
	}
}

static int g_nIdOwmExMenu = 4000;	// OWM的扩展菜单从IDM_OWMCMD+4000开始,最多96个
/////////////////////////////////////////////////////////////////////////////
// 加载扩展菜单
/////////////////////////////////////////////////////////////////////////////
BOOL CPlugIn::AddOwmSubMenu(CString strMainMenu, CString strSubMenu, CString strAction)
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
				if(pControl->GetCaption() == strSubMenu)
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
		CExtMenuInfo extMenuInfo;
		extMenuInfo.m_strVciId = m_strId;
		extMenuInfo.m_strMainMenu = strMainMenu;
		extMenuInfo.m_strSubMenu = strSubMenu;
		extMenuInfo.pProc = (TYPEOF_ClientMenu*)OnOwmExtMenuProc;
		extMenuInfo.pUpdateProc = NULL;
		extMenuInfo.nProcType = MENU_PROC_TYPE_PARAM_ID;
		
		int nExMenuId = IDM_OWMCMD+g_nIdOwmExMenu;
		extMenuInfo.m_strAction = strAction;
		extMenuInfo.nId = nExMenuId-IDM_OWMCMD;
		pMainFrame->m_arExtMenuInfo.Add(extMenuInfo);

		CXTPControls* pControlsSub = pMenuBarSub->GetControls();
		CMenu* pMenu = new CMenu();
		pMenu->CreatePopupMenu();
		pMenu->AppendMenu(MF_STRING, nExMenuId, strSubMenu);
		pControlsSub->AddMenuItem(pMenu, 0);
		delete pMenu;

		g_nIdOwmExMenu++;
	}

	return TRUE;
}

typedef LPVOID (*TYPEOF_CreateObject)(LPCSTR lpcsInterfaceName, LPVOID* lppVciControl, LPVOID lpInitData);

/////////////////////////////////////////////////////////////////////////////
// 加载VCI组件
/////////////////////////////////////////////////////////////////////////////
void* CPlugIn::OpenVciLib(CString strInterface)
{
	// 如果已经加载,则直接返回
	if(HasLoad())
	{
		return m_hVciHandle;
	}

	if(GetFileAttributes(m_strFile) == 0xFFFFFFFF)
	{
		OUTPUT(COLOR_ERROR, "File %s not exist!\r\n", m_strFile);
		return NULL;
	}

	if(m_hVciHandle == NULL)
	{
		// 保存并设置当前路径
		theApp.SaveAndSetCurPath(m_strFile);

		// 加载DLL模块
		m_hVciHandle = LoadLibrary(m_strFile);

		// 恢复当前路径
		theApp.RestoreCurPath();
	}

	if(m_hVciHandle == NULL)
	{
		DWORD dwError = ::GetLastError();
		OUTPUT(COLOR_ERROR, "Load %s fail, error code=%u!\r\n", m_strFile, dwError);
	}

	TRACE("INFO:Load VCI %s success!\n", m_strId);

	m_nLoadState = PLUGIN_LOAD_LIB;

	return m_hVciHandle;
}

/////////////////////////////////////////////////////////////////////////////
// 关闭一个VCI组件
/////////////////////////////////////////////////////////////////////////////
BOOL CPlugIn::CloseVci()
{
	if(m_strType == "owm")
	{
		CloseOwm();
		m_aVciObj.RemoveAll();
		m_pVciObj = NULL;
	}else
	{
		// 执行Done操作
		for(int i=0; i<m_aVciObj.GetSize(); i++)
		{
			if(m_aVciObj[i].m_pIVciControl != NULL)
			{
				((IVciControl*)(m_aVciObj[i].m_pIVciControl))->Done(m_strDoneParam);
			}
			if(m_aVciObj[i].m_pVciObj != NULL)
			{
				((IUnknown *)(m_aVciObj[i].m_pVciObj))->Release();
			}
		}
		m_aVciObj.RemoveAll();
		m_pVciObj = NULL;
	}

	if(m_hVciHandle == NULL)
	{
		return FALSE;
	}

	// 释放DLL
	if(m_strType != "interp")	// 很奇怪的问题,Tcl解释器调用多次会造成释放DLL后程序关闭时异常
	{
		FreeLibrary(m_hVciHandle);
		m_hVciHandle = NULL;
	}

	m_nLoadState = PLUGIN_LOAD_INFO;

	return TRUE;
}

// VCI的调试信息输出回调函数
int callback_Vci_DebugOut(LPCTSTR lpszComponentId, LPVOID pInstance, int nLevel, int nType, int nMode, LPCSTR lpData, int nLen)
{
	//OUTPUT_ID_DIRECT(0, lpData);
	BOOL bEnableDebugOut = theApp.GetProfileInt(REG_VCI_SUBKEY, REG_VCI_ENABLE_DEBUGOUT, FALSE);
	if(bEnableDebugOut)
	{
		OUTPUT(nType, lpData);
	}

	// 调用平台的日志函数
	//theApp.PLATDEBUG(lpszComponentId, nLevel, lpData, nType, nMode);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 创建VCI组件对象
/////////////////////////////////////////////////////////////////////////////
void* CPlugIn::CreateObject(CString strInstName)
{
	// 检查进程标识,判断是否可以在本进程加载
	if((m_strProcess != "") && (theApp.m_strProcessId != "") && (m_strProcess != theApp.m_strProcessId))
	{
		return NULL;
	}

	// 如果未加载动态库,则先加载
	if(!HasLoad())
	{
		OpenVciLib(m_strClass);
	}

	// 动态库加载失败,则退出
	if(m_hVciHandle == NULL)
	{
		return NULL;
	}

	if(strInstName != "")
	{
		for(int i=0; i<m_aVciObj.GetSize(); i++)
		{
			if(m_aVciObj[i].m_strInstName == strInstName)
			{
				m_aVciObj[i].m_nRefCount++;
				if(m_aVciObj[i].m_pVciObj == m_pVciObj)
				{
					m_nRefCount++;
				}
				return m_aVciObj[i].m_pVciObj;
			}
		}
	}

	#ifdef _DEBUG
	DWORD dwRevisionType = REVISION_TYPE_DEBUG;
	#else
	DWORD dwRevisionType = REVISION_TYPE_RELEASE;
	#endif;

	LPVOID	pIVciControl = NULL;

	// 获取函数指针
	TYPEOF_CreateObject fnCreateObject = (TYPEOF_CreateObject)GetProcAddress(m_hVciHandle, "CreateObject");
	LPVOID lpVciObj = fnCreateObject(m_strClass, &pIVciControl, NULL);

	if(lpVciObj == NULL)
	{
		return NULL;
	}

	// 设置平台接口
	if(pIVciControl)
	{
		((IVciControl*)pIVciControl)->setIPlatUI(theApp.GetIPlatUI());
	}

	// 设置语言
	if(pIVciControl)
	{
		((IVciControl*)pIVciControl)->SetLanguage(
			(LANGUAGE_PAGE_CHINESE == theApp.m_curLanguage) ? LANGUAGE_CHINESE : LANGUAGE_ENGLISH);
	}

	// 安装调试回调函数
	if(pIVciControl)
	{
		TInitData_DebugOut InitData_DebugOut;
		InitData_DebugOut.lpfnDebugOut = callback_Vci_DebugOut;
		InitData_DebugOut.strComponentId = m_strId;
		InitData_DebugOut.lpVciInstance = NULL;
		InitData_DebugOut.nDeviceID = 0;

		((IVciControl*)pIVciControl)->InstallHandler(DEBUG_OUT_EVENT, callback_Vci_DebugOut, (DWORD)(&InitData_DebugOut));
	}

	if(m_strType == "interp")
	{
		IInterp* pIInterp = (IInterp*)lpVciObj;
		// 设置平台接口
		pIInterp->SetIPlatUI(theApp.GetIPlatUI());
		// 设置解释器名为VCI实例名
		pIInterp->SetInterpName(strInstName);
	}

	CVciObject obj;
	obj.m_pVciObj		= lpVciObj;
	obj.m_pIVciControl	= pIVciControl;
	obj.m_strInstName	= strInstName;
	obj.m_nRefCount		= 1;
	m_aVciObj.Add(obj);

	// 初始化全局变量
	if(m_pVciObj == NULL)
	{
		m_pVciObj		= lpVciObj;
		m_pIVciControl	= pIVciControl;
		m_nRefCount		= 1;
	}

	// 执行组件的初始化操作(OWM组件在OWM的加载函数中执行初始化)
	if(pIVciControl && !IsVciOwm())
	{
		((IVciControl*)pIVciControl)->Init(m_strInitParam);
	}

	//CTime tBuild = GetPluginBuildTime();
	//TRACE("PlugIn %s build date is %s.\n", m_strId, tBuild.Format("%Y-%m-%d   %H:%M:%S"));

	return lpVciObj;
}

/////////////////////////////////////////////////////////////////////////////
// 释放VCI组件对象
/////////////////////////////////////////////////////////////////////////////
BOOL CPlugIn::ReleaseObject(LPVOID pVciObj, BOOL bForceDelete)
{
	for(int i=0; i<m_aVciObj.GetSize(); i++)
	{
		if(m_aVciObj[i].m_pVciObj == pVciObj)
		{
			m_aVciObj[i].m_nRefCount--;			
			if(m_aVciObj[i].m_pVciObj == m_pVciObj)
			{
				m_nRefCount--;
			}
			if((m_aVciObj[i].m_nRefCount > 0) && (!bForceDelete))
			{
				return TRUE;
			}

			// 执行Done操作
			if(m_aVciObj[i].m_pIVciControl != NULL)
			{
				((IVciControl*)(m_aVciObj[i].m_pIVciControl))->Done(m_strDoneParam);
			}

			if(m_aVciObj[i].m_pVciObj)
			{
				try{
				((IUnknown *)(m_aVciObj[i].m_pVciObj))->Release();
				}catch(...){
					OUTPUT(COLOR_ERROR, "Catch Exception: CPlugIn::ReleaseObject(0x%08X), Release object fail!\r\n", m_pVciObj);
					OUTPUT(COLOR_ERROR, "Object Instance Name: %s\r\n", m_aVciObj[i].m_strInstName);
					OUTPUT(COLOR_ERROR, "Object Refrence Count: %d\r\n", m_aVciObj[i].m_nRefCount);
				}
			}

			m_aVciObj[i].m_nRefCount = 0;
			
			if(m_aVciObj[i].m_pVciObj == m_pVciObj)
			{
				m_pVciObj		= NULL;
				m_pIVciControl	= NULL;
				m_nRefCount		= 0;
			}

			m_aVciObj.RemoveAt(i);
			return TRUE;
		}
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 释放VCI组件对象(指定实例名)
/////////////////////////////////////////////////////////////////////////////
BOOL CPlugIn::ReleaseObject(CString strInstName)
{
	for(int i=0; i<m_aVciObj.GetSize(); i++)
	{
		if(m_aVciObj[i].m_strInstName == strInstName)
		{
			// 执行Done操作
			if(m_aVciObj[i].m_pIVciControl != NULL)
			{
				((IVciControl*)(m_aVciObj[i].m_pIVciControl))->Done(m_strDoneParam);
			}

			if(m_aVciObj[i].m_pVciObj)
			{
				((IUnknown *)(m_aVciObj[i].m_pVciObj))->Release();
			}

			m_aVciObj[i].m_nRefCount = 0;
			
			if(m_aVciObj[i].m_pVciObj == m_pVciObj)
			{
				m_pVciObj		= NULL;
				m_pIVciControl	= NULL;
				m_nRefCount		= 0;
			}

			m_aVciObj.RemoveAt(i);
			return TRUE;
		}
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个组件对象(根据实例名)
/////////////////////////////////////////////////////////////////////////////
void* CPlugIn::GetObjectByInstanceName(CString strInstName)
{
	for(int i=0; i<m_aVciObj.GetSize(); i++)
	{
		if(m_aVciObj[i].m_strInstName == strInstName)
		{
			return (void*)(m_aVciObj[i].m_pVciObj);
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个组件对象控制接口(根据组件实例接口指针)
/////////////////////////////////////////////////////////////////////////////
void* CPlugIn::GetObjectControlPtrByInstancePtr(LPVOID	pVciObj)
{
	for(int i=0; i<m_aVciObj.GetSize(); i++)
	{
		if(m_aVciObj[i].m_pVciObj == pVciObj)
		{
			return (void*)(m_aVciObj[i].m_pIVciControl);
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 判断一个实例接口指针是否是这个组件的
/////////////////////////////////////////////////////////////////////////////
BOOL CPlugIn::IsInstancePtr(LPVOID	pVciObj)
{
	for(int i=0; i<m_aVciObj.GetSize(); i++)
	{
		if(m_aVciObj[i].m_pVciObj == pVciObj)
		{
			return TRUE;
		}
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 询问是否可以关闭,保存组件信息
/////////////////////////////////////////////////////////////////////////////
BOOL CPlugIn::CanClose()
{
	if(m_strType == "owm")
	{
		return CanCloseOwm();
	}else
	if(m_strType == "interp")
	{
		return CanCloseInterp();
	}
	return TRUE;
}

// 加载插件
BOOL CPlugIn::LoadPlugIn(BOOL bForce)
{
	if(m_strType == "owm")
	{
		// 如果是动态加载类型且不强制加载则直接返回
		if(!bForce && (m_nType & OWM_DYNLOAD) && !(m_nType & OWM_AUTOCREATE))
		{
			return TRUE;
		}
		return LoadOwmPlugIn();
	}else
	{
		// 为了加快启动速度,VCI组件只在Create时候加载
		if(bForce)
		{
			return LoadVciPlugIn();
		}
		return TRUE;
	}

	return FALSE;
}

// 加载VCI类型插件
BOOL CPlugIn::LoadVciPlugIn()
{
	return (OpenVciLib(m_strClass) != NULL);
}

/////////////////////////////////////////////////////////////////////////////
// 枚举参数名
/////////////////////////////////////////////////////////////////////////////
int CPlugIn::EnumParameters(CStringArray& asName)
{
	for(int i=0; i<m_arParameter.GetSize(); i++)
	{
		asName.Add(m_arParameter[i].m_strName);
	}
	return asName.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
// 获取参数
/////////////////////////////////////////////////////////////////////////////
BOOL CPlugIn::GetParameter(CString strName, CString& strValue, CString& strDefaultValue)
{
	for(int i=0; i<m_arParameter.GetSize(); i++)
	{
		if(strName == m_arParameter[i].m_strName)
		{
			strValue = m_arParameter[i].m_strValue;
			strDefaultValue = m_arParameter[i].m_strDefaultValue;
			return TRUE;
		}
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 设置参数
/////////////////////////////////////////////////////////////////////////////
BOOL CPlugIn::SetParameter(CString strName, CString strValue)
{
	for(int i=0; i<m_arParameter.GetSize(); i++)
	{
		if(strName == m_arParameter[i].m_strName)
		{
			m_arParameter[i].m_strValue = strValue;
			return TRUE;
		}
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 平台命令处理函数
/////////////////////////////////////////////////////////////////////////////
int gCommandPlat(int nCmd, WPARAM wParam, LPARAM lParam)
{
	return theApp.CommandPlat(nCmd, wParam, lParam);
}

// 加载OWM类型插件
BOOL CPlugIn::LoadOwmPlugIn()
{
	// 检查进程标识,判断是否可以在本进程加载
	if((m_strProcess != "") && (theApp.m_strProcessId != "") && (m_strProcess != theApp.m_strProcessId))
	{
		return NULL;
	}

	OpenVciLib(IID_IVciOwm);
	m_pVciObj = CreateObject();
	if(m_pVciObj == NULL)
	{
		return FALSE;
	}

	IVciOwm* pIVciOwm = (IVciOwm*)m_pVciObj;

	OWM_INFO owminfo;
	memset(&owminfo, 0, sizeof(OWM_INFO));
	owminfo.bEnable = TRUE;

	pIVciOwm->SetIPlatUI(theApp.GetIPlatUI());

	// 创建文档模板
	if(m_nType & OWM_DOC)
	{
		CRuntimeClass* pDocClass = NULL;
		CRuntimeClass* pFrameClass = NULL;
		CRuntimeClass* pViewClass = NULL;
		pIVciOwm->GetRuntimeClass(1, &pDocClass);
		pIVciOwm->GetRuntimeClass(2, &pFrameClass);
		pIVciOwm->GetRuntimeClass(3, &pViewClass);
		m_pDocTemplate =  new CMultiDocTemplate(
			IDR_BLUEUITYPE,
			pDocClass,
			pFrameClass,
			pViewClass);
		pIVciOwm->SetDocTemplate(m_pDocTemplate);
		// 兼容以前的数据结构
		owminfo.ol.pDocClass = pDocClass;
		owminfo.ol.pFrameClass = pFrameClass;
		owminfo.ol.pViewClass = pViewClass;
		owminfo.ol.pDocTemplate = m_pDocTemplate;
	}

	owminfo.ol.nOWMType		= m_nType;
	owminfo.ol.CommandPlat	= gCommandPlat;
//	owminfo.ol.TraceFmt		= PF_TRACE_FMT;
//	owminfo.ol.CreateOutput	= PF_CREATE_OUTPUT;
//	owminfo.ol.CloseOutput	= PF_CLOSE_OUTPUT;
//	owminfo.ol.OutputSet	= PF_TRACE_SET_ID;
//	owminfo.ol.Output		= PF_TRACE_EX_ID;

	// 设置OWM链接
	pIVciOwm->Command(OC_SETOWMLINK, (WPARAM)(&owminfo.ol), NULL);

	// 加载菜单
	// 注册OWM菜单,添加到弹出菜单上
	// 注:在Xtreme中需要通过MenuBar的GetMenu函数来获取主
	// 菜单的资源,而CJLib中直接通过MainFrame的GetMenu就可以了
	//ol.RegisterClientMenu(m_pMainWnd->GetMenu(), m_aOWMMenus, IDM_OWMCMD, pDoc);
	
	// 加载扩展菜单
	if(AfxGetMainWnd()->GetSafeHwnd() != NULL)
	{
		COWMMenuArray aOWMMenus;
		CXTPCommandBars* pCommandBars = ((CMainFrame*)AfxGetMainWnd())->GetCommandBars();
		CXTPCommandBar* pMenuBar = pCommandBars->GetMenuBar();
		pIVciOwm->RegisterClientMenu((CMenu*)pMenuBar,
						aOWMMenus, 40000, NULL);
	}

	// 加载ToolBar
	if(m_nType & OWM_TOOLBAR)
	{/*
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CXTToolBar* pToolBar = &(pMainFrame->m_wndToolBar);
		int nStart = pToolBar->CommandToIndex(ID_VIEW_OUTPUT);

		pIVciOwm->RegisterClientToolBar(pToolBar, m_aOWMMenus, IDM_OWMCMD, nStart);
		pMainFrame->ShowControlBar(pToolBar, TRUE, FALSE);*/
	}

	// 执行OWM的初始化操作
	if(m_pIVciControl == NULL)
	{
		return FALSE;
	}
	((IVciControl*)m_pIVciControl)->Init(m_strInitParam);

	// 创建第一个OWM窗口
	if((m_nType & OWM_DOC) && (m_nType & OWM_AUTOCREATE))
	{
		if((m_nType & OWM_CONTROLBAR) || (m_nType & OWM_TABCTRLBAR))
			//&& owminfo.ol.pCtrlBarClass)
			NewOwmControlBar();
		else
			NewOwmFrame(m_strInitParam);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 加载DockingPane
// nID = -1表示加载所有自动加载的
/////////////////////////////////////////////////////////////////////////////
BOOL CPlugIn::LoadDockingPane(int nID)
{
	CMainFrame* pMainFrame = (CMainFrame*)(AfxGetMainWnd());
	if(nID == -1)
	{
		// 2007-09-09修改,不需要事先加载OWM,在窗口要显示时候会自动加载
		// 此段代码取消--如果组件尚未加载就先加载--
		//if((m_arDockingPane.GetSize() > 0) && (m_pVciObj == NULL))
		//{
		//	LoadOwmPlugIn();
		//}

		for(int i=0; i<m_arDockingPane.GetSize(); i++)
		{
			CXTPDockingPane* pPane = pMainFrame->GetPaneManager()->FindPane(m_arDockingPane[i].m_nId);
			if(m_arDockingPane[i].m_bAutoCreate || pPane)
			{
				CString strIconPath = m_arDockingPane[i].m_strIcon;
				if(strIconPath.Find(":") == -1)
				{
					strIconPath = m_strPath+"\\"+m_arDockingPane[i].m_strIcon;
				}
				WORD wIndex = 0;
				HICON hIcon = ::ExtractAssociatedIcon(theApp.m_hInstance,
					strIconPath.GetBuffer(0), &wIndex);
				strIconPath.ReleaseBuffer();
				if(m_arDockingPane[i].m_bAutoCreate)
				{
					pMainFrame->CreateDockingBar(m_arDockingPane[i].m_nId,
						m_arDockingPane[i].m_nDirection, hIcon);
				}else
				if(hIcon)
				{
					pMainFrame->GetPaneManager()->SetIcon(m_arDockingPane[i].m_nId,
						CXTPImageManagerIconHandle(hIcon));
				}
			}
		}
	}else
	{
		for(int i=0; i<m_arDockingPane.GetSize(); i++)
		{
			if(m_arDockingPane[i].m_nId == nID)
			{
				if(m_pVciObj == NULL)
				{
					LoadOwmPlugIn();
				}

				CString strIconPath = m_arDockingPane[i].m_strIcon;
				if(strIconPath.Find(":") == -1)
				{
					strIconPath = m_strPath+"\\"+m_arDockingPane[i].m_strIcon;
				}
				WORD wIndex = 0;
				HICON hIcon = ::ExtractAssociatedIcon(theApp.m_hInstance,
					strIconPath.GetBuffer(0), &wIndex);
				strIconPath.ReleaseBuffer();
				pMainFrame->CreateDockingBar(m_arDockingPane[i].m_nId,
					m_arDockingPane[i].m_nDirection, hIcon);
				return TRUE;
			}
		}
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 新建OWM对象窗口(for Xtreme)
/////////////////////////////////////////////////////////////////////////////
void CPlugIn::NewOwmFrame(LPCTSTR lpszPath)
{
	if(m_pVciObj == NULL)
	{
		return;
	}

	IVciOwm* pIVciOwm = (IVciOwm*)m_pVciObj;
	CMultiDocTemplate* pDocTemplate = NULL;
	pIVciOwm->GetDocTemplate(&pDocTemplate);

	if((m_nType & OWM_DOC) && pDocTemplate)
	{
		// 如果只允许一个实例，则做相应处理
		if((m_nType & OWM_ONEFRAME) &&
			(pDocTemplate->GetFirstDocPosition()))
			return;

		// 打开一个空OWM文档
		//if(!pDocTemplate->OpenDocumentFile(lpszPath))
		if(!pDocTemplate->OpenDocumentFile(NULL))
		{
			return;
		}

		// 创建OWM扩展菜单
		if(m_nType & OWM_MENU)
		{
			// 取到文档指针,用来将扩展菜单指针纪录在文档中,文档关闭时清除
			POSITION pos = pDocTemplate->GetFirstDocPosition();
			CDocument* pDoc = pDocTemplate->GetNextDoc(pos);
			POSITION posview = pDoc->GetFirstViewPosition();
			if(posview)	// 如果视存在
			{
				CView* pView = pDoc->GetNextView(posview);	// 取到视指针
				//pView->GetParentFrame()->ActivateFrame(); // 激活框架
				if(m_nType & OWM_MAXWINDOW)
				{
					// 将窗口最大化
					pView->GetParentFrame()->ShowWindow(SW_SHOWMAXIMIZED);
				}
			}
/*
			// 注册OWM菜单,添加到弹出菜单上
			// 注:在Xtreme中需要通过MenuBar的GetMenu函数来获取主
			// 菜单的资源,而CJLib中直接通过MainFrame的GetMenu就可以了
			//ol.RegisterClientMenu(m_pMainWnd->GetMenu(), m_aOWMMenus, IDM_OWMCMD, pDoc);
			CXTMenuBar* pMenuBar = ((CMainFrame*)AfxGetMainWnd())->GetMenuBar();
			pIVciOwm->RegisterClientMenu(pMenuBar->GetMenu(),
							m_aOWMMenus, IDM_OWMCMD, pDoc);

			// 刷新菜单条
			// 注:在CJLib中用DrawMenuBar就可以了,在Xtreme中必须用
			// DrawMenuBarX来刷新Xtreme中的MenuBar和Menu资源的关联
			((CMainFrame*)AfxGetMainWnd())->DrawMenuBarX();

			// 注:关于OWM针对Xtreme toolkit的改动
			// Xtreme和CJLibrary的区别比较大,Cool菜单改为统一管理,
			// 通过CXTCoolMenu类来管理,此类可以加载许多图标资源,然后
			// 安装到一个窗口上,安装以后,用户不用再针对菜单做特殊处理
			// 就可以使用Cool菜单了.而以前的CJLib使用CoolMenu的方法是
			// 每个菜单单独加载图标资源.
			// 因此,在OWM中要想实现扩展菜单使用自己的图标资源,必须做
			// 一些特殊处理,因为OWM和主程序必须使用同一个CXTCoolMenu,
			// 这样是很容易造成资源冲突问题的,并且也不能通过资源切换
			// 来解决,那么唯一的办法就是保证OWM中需要添加到主程序的菜
			// 单和主程序中的菜单的资源ID不重合,因此规定OWM中需要扩展
			// 的资源ID必须在10000到20000范围内,其中10000-11999为系统
			// OWM保留,用户自己写的必须从12000开始.
			// 为了保证OWM接口的兼容性,因此对接口定义不做任何改动,在
			// OWM的RegisterClientMenu函数中要想注册自己的图标资源,
			// 可以通过获取MainFrame指针,然后通过MainFrame找到CXTCoolMenu
			// 的指针来操作.
*/
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 关闭OWM对象窗口
/////////////////////////////////////////////////////////////////////////////
void CPlugIn::CloseOwmFrame()
{
	if(m_pVciObj == NULL)
	{
		return;
	}

	IVciOwm* pIVciOwm = (IVciOwm*)m_pVciObj;
	CMultiDocTemplate* pDocTemplate = NULL;
	pIVciOwm->GetDocTemplate(&pDocTemplate);

	if(pDocTemplate && (m_nType & OWM_DOC))
	{
		// 关闭所有OWM文档和窗口
		pDocTemplate->CloseAllDocuments(FALSE);

		// 如果是动态加载OWM,则卸载掉此模块,但保留结构
		if(m_nType & OWM_DYNLOAD)
		{/*
			for(int i=0; i<m_aOWMs.GetSize(); i++)
			{
				if((m_aOWMs[i].ol.nOWMID == ol.nOWMID) && m_aOWMs[i].bEnableDynLoad)
				{	// 只有模块类型设置了OWM_DYNLOAD,且数据库中也配置了允许动态加载,动态加载才真正有效
					TRACE("DynUnLoad a OWM[%d]: %s\n", m_aOWMs[i].ol.nOWMID, m_aOWMs[i].szName);
					UnLoadOWM(i, TRUE);
					break;
				}
			}*/
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 新建OWM对象ControlBar(for Xtreme)
/////////////////////////////////////////////////////////////////////////////
BOOL CPlugIn::NewOwmControlBar()
{
	if(m_pVciObj == NULL)
	{
		return FALSE;
	}

	IVciOwm* pIVciOwm = (IVciOwm*)m_pVciObj;
	CMultiDocTemplate* pDocTemplate = NULL;
	pIVciOwm->GetDocTemplate(&pDocTemplate);

	CRuntimeClass* pCtrlBarClass = NULL;
	pIVciOwm->GetRuntimeClass(4, &pCtrlBarClass);

	if(!pCtrlBarClass)
		return FALSE;

	if(m_pControlBar)
		return FALSE;

	// 此处已经修改为支持Xtreme
	CMainFrame* pMainFrame = (CMainFrame*)(AfxGetMainWnd());

	m_pControlBar = (CControlBar*)(pCtrlBarClass->CreateObject());

//	pMainFrame->EnableDockingEx(CBRS_ALIGN_ANY, CBRS_XT_FLAT);
//	pMainFrame->DockControlBar( pBar );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 关闭OWM对象ControlBar
/////////////////////////////////////////////////////////////////////////////
BOOL CPlugIn::CloseOwmControlBar()
{
	if(m_pVciObj == NULL)
	{
		return FALSE;
	}

	IVciOwm* pIVciOwm = (IVciOwm*)m_pVciObj;
	CMultiDocTemplate* pDocTemplate = NULL;
	pIVciOwm->GetDocTemplate(&pDocTemplate);

	CRuntimeClass* pCtrlBarClass = NULL;
	pIVciOwm->GetRuntimeClass(4, &pCtrlBarClass);

	if(!pCtrlBarClass)
		return FALSE;

	if(!m_pControlBar)
		return FALSE;

	CMainFrame* pMainFrame = (CMainFrame*)(AfxGetMainWnd());
	pMainFrame->ShowControlBar(m_pControlBar, FALSE, FALSE);
	delete m_pControlBar;
	m_pControlBar = NULL;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 显示/隐藏OWM对象ControlBar
/////////////////////////////////////////////////////////////////////////////
BOOL CPlugIn::ShowOwmControlBar(BOOL bShow)
{
	if(m_pVciObj == NULL)
	{
		return FALSE;
	}

	IVciOwm* pIVciOwm = (IVciOwm*)m_pVciObj;
	CMultiDocTemplate* pDocTemplate = NULL;
	pIVciOwm->GetDocTemplate(&pDocTemplate);

	CRuntimeClass* pCtrlBarClass = NULL;
	pIVciOwm->GetRuntimeClass(4, &pCtrlBarClass);

	if(!pCtrlBarClass)
		return FALSE;

	if(!m_pControlBar && !NewOwmControlBar())
		return FALSE;

	CMainFrame* pMainFrame = (CMainFrame*)(AfxGetMainWnd());
	pMainFrame->ShowControlBar(m_pControlBar, bShow, FALSE);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 显示OWM(View形式)
/////////////////////////////////////////////////////////////////////////////
BOOL CPlugIn::ShowOwmView(BOOL bShow, BOOL bActive)
{
	if(m_pVciObj == NULL)
	{
		return FALSE;
	}

	IVciOwm* pIVciOwm = (IVciOwm*)m_pVciObj;
	CMultiDocTemplate* pDocTemplate = NULL;
	pIVciOwm->GetDocTemplate(&pDocTemplate);

	if(pDocTemplate == NULL)
	{
		return FALSE;
	}

	POSITION posdoc = pDocTemplate->GetFirstDocPosition();
	if(!bShow)
	{
		if(posdoc)
		{
			CloseOwmFrame();
		}
		return TRUE;
	}

	if(!posdoc)
	{
		NewOwmFrame();
	}
	if(bActive)
	{
		POSITION posdoc = pDocTemplate->GetFirstDocPosition();
		if(posdoc)
		{
			CDocument* pDoc = pDocTemplate->GetNextDoc(posdoc);
			POSITION posview = pDoc->GetFirstViewPosition();
			if(posview)	// 如果视存在
			{
				CView* pOwmView = pDoc->GetNextView(posview);	// 取到视指针
				pOwmView->GetParentFrame()->ActivateFrame(); // 激活框架
				return TRUE;
			}
		}
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 询问是否可以关闭OWM,保存组件信息
/////////////////////////////////////////////////////////////////////////////
BOOL CPlugIn::CanCloseOwm()
{
	if(m_pVciObj == NULL)
	{
		return TRUE;
	}

	IVciOwm* pIVciOwm = (IVciOwm*)m_pVciObj;
	CMultiDocTemplate* pDocTemplate = NULL;
	pIVciOwm->GetDocTemplate(&pDocTemplate);

	if(pDocTemplate == NULL)
	{
		return TRUE;
	}

	if(!pDocTemplate->SaveAllModified())
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 关闭一个VCI组件
/////////////////////////////////////////////////////////////////////////////
BOOL CPlugIn::CloseOwm()
{
	if(m_pVciObj == NULL)
	{
		return TRUE;
	}

	IVciOwm* pIVciOwm = (IVciOwm*)m_pVciObj;
	//CMultiDocTemplate* pDocTemplate = NULL;
	//pIVciOwm->GetDocTemplate(&pDocTemplate);

	// 执行Done操作
	if(m_pIVciControl == NULL)
	{
		return FALSE;
	}
	((IVciControl*)m_pIVciControl)->Done(m_strDoneParam);

	// 关闭ControlBar
	if((m_nType & OWM_CONTROLBAR) || (m_nType & OWM_TABCTRLBAR))
		CloseOwmControlBar();

	// 卸载ToolBar
	if(m_nType & OWM_TOOLBAR)
	{
		CMainFrame* pMainFrame = (CMainFrame*)(AfxGetMainWnd());
		//CXTToolBar* pToolBar = &(pMainFrame->m_wndToolBar);
		//pIVciOwm->UnregisterClientToolBar(pToolBar, m_aOWMMenus, IDM_OWMCMD);
		//pMainFrame->ShowControlBar(pToolBar, TRUE, FALSE);
	}

	// 重置TCL扩展命令(没有必要在此处注销,因为TCL只在创建解释器时有效)
	if(m_nType & OWM_TCL)
	{
		// 注销TCL扩展命令
		//if(owminfo.ol.UnregisterTclCmd)
		//	owminfo.ol.UnregisterTclCmd();
	}

	// 卸载扩展菜单命令
	if(m_nType & OWM_MENU)
	{
		COWMMenuArray aOWMMenus;
		aOWMMenus.Add(OWMMENUITEM());
		CXTPCommandBars* pCommandBars = ((CMainFrame*)AfxGetMainWnd())->GetCommandBars();
		CXTPCommandBar* pMenuBar = pCommandBars->GetMenuBar();
		aOWMMenus[0].pProc = (TYPEOF_ClientMenu*)pMenuBar;
		pIVciOwm->UnregisterClientMenu(aOWMMenus);
	}

	// 释放OWM文档模板
	if((m_nType & OWM_DOC) && m_pDocTemplate)
	{
		// 关闭所有OWM文档和窗口
		POSITION pos = m_pDocTemplate->GetFirstDocPosition();
		if(pos != 0)
		{
			m_pDocTemplate->CloseAllDocuments(FALSE);
		}
		// 删除OWM文档模板
		delete m_pDocTemplate;
		m_pDocTemplate = NULL;
	}

	pIVciOwm->Release();

	m_pIVciControl = NULL;
	m_pVciObj = NULL;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 打开文档,如果不能打开则返回FALSE
/////////////////////////////////////////////////////////////////////////////
CDocument* CPlugIn::OpenDocument(LPCTSTR lpszFileName)
{
	CString strFileName = lpszFileName;
	strFileName.MakeLower();
	int nPos = strFileName.ReverseFind('.');
	if(nPos == -1)
	{
		return NULL;
	}

	CString strFileExt = strFileName;
	strFileExt.Delete(0, nPos+1);

	for(int i=0; i<m_arDocExtend.GetSize(); i++)
	{
		for(int j=0; j<m_arDocExtend[i].m_arDocExt.GetSize(); j++)
		{
			if(strFileExt == m_arDocExtend[i].m_arDocExt[j])
			{
				// 如果是动态加载的OWM,并且未加载过,就先加载
				if((m_pDocTemplate == NULL) && (m_nType & OWM_DYNLOAD))
				{
					LoadOwmPlugIn();
				}

				// 如果文档模板为空,则此种文档模板不能使用,继续向下搜索
				if(m_pDocTemplate == NULL)
				{
					continue;
				}

				// 先判断是否已经打开了
				strFileName.Replace("/", "\\");

				CView* pView = NULL;
				POSITION posdoc = m_pDocTemplate->GetFirstDocPosition();
				while(posdoc != NULL)
				{
					CDocument* pDoc = m_pDocTemplate->GetNextDoc(posdoc);
					CString strFilePathExist = pDoc->GetPathName();
					strFilePathExist.MakeLower();
					strFilePathExist.Replace("/", "\\");
					if(strFilePathExist == strFileName)
					{
						// 如果已经打开则激活
						POSITION posview = pDoc->GetFirstViewPosition();
						if(posview)	// 如果视存在
						{
							CView* pView = pDoc->GetNextView(posview);	// 取到视指针
							pView->GetParentFrame()->ActivateFrame(); // 激活框架
						}
						return pDoc;
					}
				}

				return m_pDocTemplate->OpenDocumentFile(lpszFileName);
			}
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 获取OWM注册的属性页指针数组
/////////////////////////////////////////////////////////////////////////////
int CPlugIn::RegisterOption(COWMOptionArray& aOWMOption)
{
	IVciOwm* pIVciOwm = (IVciOwm*)m_pVciObj;
	return pIVciOwm->RegisterOption(aOWMOption);
}

/////////////////////////////////////////////////////////////////////////////
// 获取注册的属性页指针数组
/////////////////////////////////////////////////////////////////////////////
int CPlugIn::RegisterPropertyPage(CVciPropertyPageArray& aPropertyPage)
{
	// 获取函数指针
	TYPEOF_RegisterPropertyPage fnRegisterPropertyPage = (TYPEOF_RegisterPropertyPage)GetProcAddress(m_hVciHandle, "gRegisterPropertyPage");
	if(fnRegisterPropertyPage)
	{
		return fnRegisterPropertyPage(aPropertyPage);
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 创建工程接口对象
/////////////////////////////////////////////////////////////////////////////
IProject* CPlugIn::CreateProjectObject(CString strInstName)
{
	IProject* pIProject = (IProject*)(CreateObject(strInstName));
	if(pIProject)
	{
		// 加载扩展菜单
		COWMMenuArray aOWMMenus;
		CXTPCommandBars* pCommandBars = ((CMainFrame*)AfxGetMainWnd())->GetCommandBars();
		CXTPCommandBar* pMenuBar = pCommandBars->GetMenuBar();
		pIProject->RegisterMenu((CMenu*)pMenuBar, aOWMMenus, 40000);
	}

	return pIProject;
}

/////////////////////////////////////////////////////////////////////////////
// 释放工程接口对象
/////////////////////////////////////////////////////////////////////////////
BOOL CPlugIn::ReleaseProjectObject(LPVOID pVciObj, BOOL bForceDelete)
{
	if(m_strType != "project")
	{
		return FALSE;
	}

	IProject* pIProject = (IProject*)pVciObj;
	if(pIProject)
	{
		COWMMenuArray aOWMMenus;
		aOWMMenus.Add(OWMMENUITEM());
		CXTPCommandBars* pCommandBars = ((CMainFrame*)AfxGetMainWnd())->GetCommandBars();
		CXTPCommandBar* pMenuBar = pCommandBars->GetMenuBar();
		aOWMMenus[0].pProc = (TYPEOF_ClientMenu*)pMenuBar;
		pIProject->UnregisterMenu(aOWMMenus);
	}

	return ReleaseObject(pVciObj, bForceDelete);
}

/////////////////////////////////////////////////////////////////////////////
// 询问是否可以关闭Interp
/////////////////////////////////////////////////////////////////////////////
BOOL CPlugIn::CanCloseInterp()
{
	if(m_pVciObj == NULL)
	{
		return TRUE;
	}

	//IInterp* pIInterp = (IInterp*)m_pVciObj;
	//return !(pIInterp->IsInterpRunning());

	for(int i=0; i<m_aVciObj.GetSize(); i++)
	{
		IInterp* pIInterp = (IInterp*)(m_aVciObj[i].m_pVciObj);
		if(pIInterp->IsInterpRunning())
		{
			return FALSE;
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 发送消息
/////////////////////////////////////////////////////////////////////////////
int CPlugIn::SendMessage(CVciMessage* pIn, CVciMessage* ppOut)
{
	if(m_pIVciControl == NULL)
	{
		// 如果组件实例未创建,则创建实例对象
		if(m_strType == "owm")
		{
			// OWM组件需要先进行加载和初始化
			if(!LoadOwmPlugIn())
			{
				return -1;
			}
		}else
		{
			if(CreateObject() == NULL)
			{
				return -1;
			}
		}
	}

	if(pIn->GetSync() == VMT_SYNC)
	{
		// 同步消息,直接调用并返回
		return ((IVciControl*)m_pIVciControl)->ProcessMessage(pIn, ppOut);
	}else
	if(pIn->GetSync() == VMT_ASYNC)
	{
		// 异步消息,插入消息队列
		if(theApp.m_pIMessageQueue)
		{
			theApp.m_pIMessageQueue->PostMessage(pIn, m_strId);
		}
	}

	return 0;
}
