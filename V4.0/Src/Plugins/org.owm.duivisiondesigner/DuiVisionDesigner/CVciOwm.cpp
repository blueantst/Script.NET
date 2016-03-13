#include "stdafx.h"
#include "CVciOwm.h"
#include "DuiVisionDesigner.h"

/////////////////////////////////////////////////////////////////////////////
// 获取平台路径
/////////////////////////////////////////////////////////////////////////////
CString GetPlatRootPath()
{
	//获取平台路径
	TCHAR szFullPath[MAX_PATH];
	TCHAR szdrive[_MAX_DRIVE];
	TCHAR szdir[_MAX_DIR];
	::GetModuleFileName(NULL, szFullPath, MAX_PATH);
	_splitpath(szFullPath, szdrive, szdir, NULL, NULL);
	CString szPath;
	szPath.Format(_T("%s%s"), szdrive, szdir);
	szPath = szPath.Left(szPath.GetLength() - 1);
	int nPos = szPath.ReverseFind('\\');
	if(nPos >= 0)
	{
		szPath = szPath.Left(nPos+1);
	}

	return szPath;
}


//////////////////////////////////////////////////////////////////////////
// 功能实现
//////////////////////////////////////////////////////////////////////////
CVciOwm::CVciOwm()
{
}

CVciOwm::~CVciOwm()
{
}

/////////////////////////////////////////////////////////////////////////////
// 安装回调函数
/////////////////////////////////////////////////////////////////////////////
int CVciOwm::InstallHandler(DWORD nEventID, LPVOID lpHandler, DWORD dwRefData)
{
	CVisualComponent::InstallHandler(nEventID, lpHandler, dwRefData);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 卸载回调函数
/////////////////////////////////////////////////////////////////////////////
int CVciOwm::UninstallHandler(DWORD nEventID, LPVOID lpHandler)
{

	CVisualComponent::UninstallHandler(nEventID, lpHandler);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 消息处理
/////////////////////////////////////////////////////////////////////////////
int CVciOwm::ProcessMessage(CVciMessage* pIn, CVciMessage* ppOut)
{
	return 0;
}

// 设置调试模式
int CVciOwm::SetDebugMode(BOOL bDebug)
{
	theApp.m_bDebug	= bDebug;
	return CVisualComponent::SetDebugMode(bDebug);
}

// 设置语言
int CVciOwm::SetLanguage(int nLanguage)
{
	theApp.m_nLanguage = nLanguage;
	return CVisualComponent::SetLanguage(nLanguage);
}

// 初始化组件
int CVciOwm::Init(LPCSTR lpcsParams)
{
	return InitOWM();
}

// 结束组件
int CVciOwm::Done(LPCSTR lpcsParams)
{
	return DoneOWM();
}

//{{VCI_IMPLEMENT_BEGIN
//}}VCI_IMPLEMENT_END

//////////////////////////////////////////////////////////////////////////
// 接口实现
//////////////////////////////////////////////////////////////////////////
INTERFACE_IMPLEMENT(VciOwm)
CONTROL_INTERFACE_IMPLEMENT(VciOwm)

//{{ 注意：!!!这里是保留的重要函数，不可删除!!!
// 获取接口的额外信息
LPVOID __stdcall CVciOwm::XVciOwm::GetExtraInfo(LPVOID lpRefData)
{
	CVciOwm *pObj = GET_INTERFACE_OBJECT(VciOwm);
	return pObj;
}
//}} 注意：!!!这里是保留的重要函数，不可删除!!!

//{{VCI_INTERFACE_IMPLEMENT_BEGIN
// 获取模块名
int __stdcall CVciOwm::XVciOwm::GetOwmName(CString& strOwmName)
{
	strOwmName = INTERFACE_INFO_IVciOwm.csName;
	return TRUE;
}

// 获取模块描述
int __stdcall CVciOwm::XVciOwm::GetOwmDesc(CString& strOwmDesc)
{
	strOwmDesc = INTERFACE_INFO_IVciOwm.csDescription;
	return TRUE;
}

// 获取模块版本
int __stdcall CVciOwm::XVciOwm::GetOwmVersion(int& nVersion)
{
	nVersion = 0x100;
	return TRUE;
}

// 获取模块类型
int __stdcall CVciOwm::XVciOwm::GetOwmType(UINT& nType)
{
	nType = theApp.m_ol.nOWMType;
	return TRUE;
}

// 获取模块ID
int __stdcall CVciOwm::XVciOwm::GetOwmID(UINT& nID)
{
	nID = theApp.m_ol.nOWMID;
	return TRUE;
}

// 设置平台UI接口指针
int __stdcall CVciOwm::XVciOwm::SetIPlatUI(LPVOID lpIPlatUI)
{
	theApp.m_pIPlatUI = (IPlatUI*)lpIPlatUI;
	return TRUE;
}

// 设置文档模板指针
int __stdcall CVciOwm::XVciOwm::SetDocTemplate(CMultiDocTemplate* pDocTemplate)
{
	theApp.m_pDocTemplate = pDocTemplate;
	return TRUE;
}

// 获取文档模板指针
int __stdcall CVciOwm::XVciOwm::GetDocTemplate(CMultiDocTemplate** ppDocTemplate)
{
	*ppDocTemplate = theApp.m_pDocTemplate;
	return TRUE;
}

// 获取指定运行时类指针
int __stdcall CVciOwm::XVciOwm::GetRuntimeClass(int nType, CRuntimeClass** ppRuntimeClass)
{
	switch(nType)
	{
	case 1: *ppRuntimeClass = theApp.m_ol.pDocClass;break;
	case 2: *ppRuntimeClass = theApp.m_ol.pFrameClass;break;
	case 3: *ppRuntimeClass = theApp.m_ol.pViewClass;break;
	case 4: *ppRuntimeClass = theApp.m_ol.pCtrlBarClass;break;
	}
	return TRUE;
}

// Theme 注册表键
#define REG_CONFIG_SUBKEY		_T("Settings")
#define REG_CONFIG_SKIN_THEME	_T("SkinTheme")			// 主题

// 显示DockingPane窗口时调用的函数
CWnd* __stdcall CVciOwm::XVciOwm::OnShowDockingPane(int nID, CWnd* pParentWnd, LPCTSTR lpszParam)
{
	CVciOwm *pObj = GET_INTERFACE_OBJECT(VciOwm);

	XTPTaskPanelPaintTheme paneTheme = xtpTaskPanelThemeShortcutBarOffice2003;
	
	int nTheme = AfxGetApp()->GetProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_THEME, xtpThemeRibbon);
	switch(nTheme)
	{
	case xtpThemeOffice2000: paneTheme = xtpTaskPanelThemeOffice2000; break;
	case xtpThemeOfficeXP: paneTheme = xtpTaskPanelThemeListViewOfficeXP; break;
	case xtpThemeOffice2003: paneTheme = xtpTaskPanelThemeListViewOffice2003; break;
	case xtpThemeNativeWinXP: paneTheme = xtpTaskPanelThemeNativeWinXP; break;
	case xtpThemeWhidbey: paneTheme = xtpTaskPanelThemeToolboxWhidbey; break;
	case xtpThemeOffice2007: paneTheme = xtpTaskPanelThemeShortcutBarOffice2003; break;
	case xtpThemeRibbon: paneTheme = xtpTaskPanelThemeShortcutBarOffice2007; break;
	}

	CWnd* pWnd = (CWnd*)(new CXTPTaskPanel());
	theApp.m_pToolboxPane = (CXTPTaskPanel*)pWnd;
	theApp.m_pToolboxPane->Create(WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), pParentWnd, nID);//DOCKPANE_ID_TOOLBOX);
	theApp.m_pToolboxPane->SetOwner(pParentWnd);
	theApp.m_pToolboxPane->SetBehaviour(xtpTaskPanelBehaviourToolbox);
	theApp.m_pToolboxPane->SetTheme(paneTheme);
	theApp.m_pToolboxPane->SetSelectItemOnFocus(TRUE);
	theApp.m_pToolboxPane->SetGroupIconSize(CSize(16, 16));
	theApp.m_pToolboxPane->Reposition();
	theApp.m_pToolboxPane->AllowDrag(FALSE);
	theApp.m_pToolboxPane->EnableAnimation(TRUE);

	// 创建工具组
	get_dll_resource();

	// 加载toolbox配置文件
	CString strToolboxPath = theApp.GetModulePath() + "\\toolbox";
	if(LANGUAGE_PAGE_CHINESE == pObj->GetLanguage())
	{
		strToolboxPath += "\\cn";
	}else
	{
		strToolboxPath += "\\cn";
	}

	theApp.m_pToolboxPane->GetGroups()->Clear(FALSE);

	CString strToolboxFile = strToolboxPath + "\\duivision_base_control_cn.xml";
	pObj->LoadToolbox(strToolboxFile);
/*
	// 加载工具组图片
	theApp.SetToolboxIcons(theApp.GetModulePath() + "\\image\\toolbox\\toolbox_uml.bmp");
	theApp.SetToolboxIcons(theApp.GetModulePath() + "\\image\\toolbox\\toolbox_ui.bmp");
	//theApp.SetToolboxIcons(theApp.GetModulePath() + "\\image\\toolbox\\toolbox_flow.bmp");

	theApp.m_pToolboxPane->GetGroups()->Clear(FALSE);

	// UML组
	CXTPTaskPanelGroup* pFolderClass = theApp.CreateToolboxGroup(ID_TOOLBOXGROUP_CLASS);
	pFolderClass->AddLinkItem(ID_BUTTON_CLASS,2);
	pFolderClass->AddLinkItem(ID_BUTTON_TEMPLATE,3);
	pFolderClass->AddLinkItem(ID_BUTTON_PACKAGE,4);
	pFolderClass->AddLinkItem(ID_BUTTON_INTERFACE,5);
	pFolderClass->AddLinkItem(ID_BUTTON_NOTE,6);
	pFolderClass->AddLinkItem(ID_BUTTON_LABEL,7);
	pFolderClass->AddLinkItem(ID_BUTTON_OBJECT,8);
	pFolderClass->AddLinkItem(ID_BUTTON_LINK,9);

	pFolderClass->SetExpanded(TRUE);

	// UI组
	CXTPTaskPanelGroup* pFolderUI = theApp.CreateToolboxGroup(ID_TOOLBOXGROUP_UI);
	pFolderUI->AddLinkItem(ID_BUTTON_UI_BUTTON,10);
	pFolderUI->AddLinkItem(ID_BUTTON_UI_EDIT,11);
	pFolderUI->AddLinkItem(ID_BUTTON_UI_STATIC,12);
	pFolderUI->AddLinkItem(ID_BUTTON_UI_CHECKBOX,13);
	pFolderUI->AddLinkItem(ID_BUTTON_UI_RADIOBUTTON,14);
	pFolderUI->AddLinkItem(ID_BUTTON_UI_COMBOBOX,15);
	pFolderUI->AddLinkItem(ID_BUTTON_UI_LISTBOX,16);
	pFolderUI->AddLinkItem(ID_BUTTON_UI_GROUPBOX,17);

	pFolderUI->SetExpanded(FALSE);
*/
	reset_dll_resource();

	return pWnd;
}

// 加载Toolbox配置文件
BOOL CVciOwm::LoadToolbox(LPCTSTR lpszToolboxFile)
{
	if(GetFileAttributes(lpszToolboxFile) == 0xFFFFFFFF)
	{
		return FALSE;
	}

	CXmlParser parser;
	if(parser.Open(lpszToolboxFile))
	{
		DEBUG_OUTF(LOG_LEVEL_ERROR, "Load toolbox file %s fail!", lpszToolboxFile);
		return FALSE;
	}

	CString strImagePath = theApp.GetModulePath();

	BOOL bRet = TRUE;
	DOMNode* pNode = parser.GetRootNode();

	// 加载图片资源
	DOMNode* pImageNode = parser.GetChildNode(pNode, "image");
	while (pImageNode != NULL)
	{
		CString strIconFile = strImagePath + "\\" + parser.GetNodeAttribute(pImageNode, "file");
		int nImageId = atoi(parser.GetNodeAttribute(pImageNode, "id"));

		// 加载单个图片
		if(!theApp.SetToolboxIcon(strIconFile, nImageId))
		{
			bRet = FALSE;
		}

		pImageNode = parser.GetNextNode(pImageNode, "image");
	}

	DOMNode* pImageListNode = parser.GetChildNode(pNode, "imagelist");
	while (pImageListNode != NULL)
	{
		CString strBmpFile = strImagePath + "\\" + parser.GetNodeAttribute(pImageListNode, "file");
		CString strIDs = parser.GetNodeText(pImageListNode);
		CUIntArray auID;
		int nPos = strIDs.Find(",");
		while(nPos != -1)
		{
			auID.Add(atoi(strIDs.Left(nPos)));
			strIDs.Delete(0, nPos+1);
			nPos = strIDs.Find(",");
		}
		auID.Add(atoi(strIDs));

		// 加载图片列表
		if(!theApp.SetToolboxIcons(strBmpFile, &auID))
		{
			bRet = FALSE;
		}

		pImageListNode = parser.GetNextNode(pImageListNode, "imagelist");
	}

	// 加载控件组和控件
	DOMNode* pGroupNode = parser.GetChildNode(pNode, "group");
	while (pGroupNode != NULL)
	{
		CString strGroupName = parser.GetNodeAttribute(pGroupNode, "name");
		int nGroupId = atoi(parser.GetNodeAttribute(pGroupNode, "id"));
		BOOL bExpand = (parser.GetNodeAttribute(pGroupNode, "expand") == "true");
		CXTPTaskPanelGroup* pFolderClass = theApp.CreateToolboxGroup(nGroupId, strGroupName);

		// 加载控件
		DOMNode* pItemNode = parser.GetChildNode(pGroupNode, "control");
		while (pItemNode != NULL)
		{
			CString strControlName = parser.GetNodeAttribute(pItemNode, "name");
			int nControlImageId = atoi(parser.GetNodeAttribute(pItemNode, "image-id"));
			CXTPTaskPanelGroupItem* pItem = pFolderClass->AddLinkItem(nControlImageId, nControlImageId);
			pItem->SetCaption(theApp.StripMnemonics(strControlName));

			pItemNode = parser.GetNextNode(pItemNode, "control");
		}

		pFolderClass->SetExpanded(bExpand);

		pGroupNode = parser.GetNextNode(pGroupNode, "group");
	}

	parser.Close();

	return TRUE;
}

// OWM命令处理
int __stdcall CVciOwm::XVciOwm::Command(int nCmd, WPARAM wParam, LPARAM lParam)
{
	return ::CommandOWM(nCmd, wParam, lParam);
}

// 注册菜单
int __stdcall CVciOwm::XVciOwm::RegisterClientMenu(CMenu* pMenu, COWMMenuArray& aOWMMenus, int nOWMIdStart, CDocument* pDoc)
{
	return ::RegisterClientMenu(pMenu, aOWMMenus, nOWMIdStart, pDoc);
}

// 卸载菜单
int __stdcall CVciOwm::XVciOwm::UnregisterClientMenu(COWMMenuArray& aOWMMenus)
{	
	return ::UnregisterClientMenu(aOWMMenus);
}

// 注册ToolBar
int __stdcall CVciOwm::XVciOwm::RegisterClientToolBar(CWnd* pBar, COWMMenuArray& aOWMMenus, int nOWMIdStart, int nIndex)
{	
	return ::RegisterClientToolBar(pBar, aOWMMenus, nOWMIdStart, nIndex);
}

// 卸载ToolBar
int __stdcall CVciOwm::XVciOwm::UnregisterClientToolBar(CWnd* pBar, COWMMenuArray& aOWMMenus, int nOWMIdStart)
{	
	return ::UnregisterClientToolBar(pBar, aOWMMenus, nOWMIdStart);
}

// 注册OWM属性页
int __stdcall CVciOwm::XVciOwm::RegisterOption(COWMOptionArray& aOWMOption)
{	
	return ::RegisterOption(aOWMOption);
}

// 注册打印页面设置属性页
int __stdcall CVciOwm::XVciOwm::RegisterPrintPageOption(COWMOptionArray& aOWMOption)
{	
	return aOWMOption.GetSize();
}

// 从Session行中获取指定名字的变量值(形式为name=value,)
CString GetValueFromSessionLine(CString strLine, CString strName, BOOL bToEnd = FALSE)
{
	int nPos = strLine.Find(strName+"=");
	if(nPos != -1)
	{
		strLine.Delete(0, nPos+strName.GetLength()+1);
		int nPosEnd = strLine.Find(",");
		if((nPosEnd != -1) && (!bToEnd))
		{
			strLine = strLine.Left(nPosEnd);
		}
		strLine.TrimLeft();
		strLine.TrimRight();
		return strLine;
	}
	return "";
}

// 加载文档Session
int __stdcall CVciOwm::XVciOwm::LoadDocumentSession(CDocument* pDoc, CStringArray& asSessionLine)
{
	return TRUE;
}

// 保存文档Session
int __stdcall CVciOwm::XVciOwm::SaveDocumentSession(CDocument* pDoc, CStringArray& asSessionLine)
{
	return TRUE;
}

// 备份文档
int __stdcall CVciOwm::XVciOwm::BackupDocument(CDocument* pDoc, LPCTSTR lpszBackupFile, int nBackupType)
{
	return TRUE;
}

// 获取导入类型
int __stdcall CVciOwm::XVciOwm::GetImportTypeList(CStringArray& asImportType)
{
	return 0;
}

// 获取导出类型
int __stdcall CVciOwm::XVciOwm::GetExportTypeList(CStringArray& asExportType)
{
	return 0;
}

// 导入文档
int __stdcall CVciOwm::XVciOwm::ImportDocument(CDocument* pDoc, LPCTSTR lpszImportSource, int nImportType)
{
	return TRUE;
}

// 导出文档
int __stdcall CVciOwm::XVciOwm::ExportDocument(CDocument* pDoc, LPCTSTR lpszExportDest, int nExportType)
{
	return TRUE;
}

// 缩放(pDoc为空表示对所有文档)
int __stdcall CVciOwm::XVciOwm::Zoom(float fZoomFactor, CDocument* pDoc)
{
	return TRUE;
}

// 获取当前缩放因子(pDoc为空表示对所有文档)
int __stdcall CVciOwm::XVciOwm::GetZoomFactor(float& fZoomFactor, CDocument* pDoc)
{
	return TRUE;
}

// 获取缩放列表
int __stdcall CVciOwm::XVciOwm::GetZoomFactorRange(CStringArray& asZoomFactor, float& fZoomMin, float& fZoomMax)
{
	return 0;
}
//}}VCI_INTERFACE_IMPLEMENT_END

// 查询命令是否支持
extern "C" __declspec(dllexport)
int gQueryCommandIsSupport(LPCSTR lpcsCmd)
{
	//if(strcmp(lpcsCmd, "HELLO") == 0)
	//{
	//}
	return 0;
}
