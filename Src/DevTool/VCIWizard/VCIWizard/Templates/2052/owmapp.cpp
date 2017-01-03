// [!output PROJECT_NAME].cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "[!output PROJECT_NAME].h"
#include "C[!output INTERFACE_NAME].h"
[!if OWM_DOC]
#include "ChildFrame.h"
#include "[!output PROJECT_NAME]View.h"
#include "[!output PROJECT_NAME]Doc.h"
[!endif]
[!if OWM_CONTROLBAR || OWM_TABCTRLBAR]
#include "[!output PROJECT_NAME]Bar.h"
[!endif]

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

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// C[!output PROJECT_NAME]App

BEGIN_MESSAGE_MAP(C[!output PROJECT_NAME]App, CWinApp)
	//{{AFX_MSG_MAP(C[!output PROJECT_NAME]App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C[!output PROJECT_NAME]App construction

C[!output PROJECT_NAME]App::C[!output PROJECT_NAME]App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bDebug	= FALSE;
	m_pDocTemplate = NULL;
	m_pIPlatUI = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 获取平台所在路径
/////////////////////////////////////////////////////////////////////////////
CString C[!output PROJECT_NAME]App::GetPlatRootPath()
{
	//获取平台路径
	TCHAR szFullPath[MAX_PATH];
	TCHAR szdrive[_MAX_DRIVE];
	TCHAR szdir[_MAX_DIR];
	//::GetModuleFileName(m_hInstance, szFullPath, MAX_PATH);
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

/////////////////////////////////////////////////////////////////////////////
// 获取模块所在路径
/////////////////////////////////////////////////////////////////////////////
CString C[!output PROJECT_NAME]App::GetModulePath()
{
	//获取平台路径
	TCHAR szFullPath[MAX_PATH];
	TCHAR szdrive[_MAX_DRIVE];
	TCHAR szdir[_MAX_DIR];
	::GetModuleFileName(AfxGetStaticModuleState()->m_hCurrentInstanceHandle, szFullPath, MAX_PATH);
	_splitpath(szFullPath, szdrive, szdir, NULL, NULL);
	CString szPath;
	szPath.Format(_T("%s%s"), szdrive, szdir);
	szPath = szPath.Left(szPath.GetLength() - 1);

	return szPath;
}

/////////////////////////////////////////////////////////////////////////////
// 读平台参数(字符型)
/////////////////////////////////////////////////////////////////////////////
CString C[!output PROJECT_NAME]App::GetIniString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault)
{
	struct INIUNIT
	{
		CString strEntry;
		CString strRet;
		int nRet;
	};
	INIUNIT IniUnit;
	IniUnit.strEntry = lpszEntry;
	IniUnit.strRet = lpszDefault;
	m_ol.CommandPlat(OC_GETINISTRING, (WPARAM)lpszSection, (LPARAM)(&IniUnit));
	return IniUnit.strRet;
}

/////////////////////////////////////////////////////////////////////////////
// 读平台参数(整型)
/////////////////////////////////////////////////////////////////////////////
int C[!output PROJECT_NAME]App::GetIniInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault)
{
	struct INIUNIT
	{
		CString strEntry;
		CString strRet;
		int nRet;
	};
	INIUNIT IniUnit;
	IniUnit.strEntry = lpszEntry;
	IniUnit.nRet = nDefault;
	return m_ol.CommandPlat(OC_GETINIINT, (WPARAM)lpszSection, (LPARAM)(&IniUnit));
}

/////////////////////////////////////////////////////////////////////////////
// 写平台参数(字符型)
/////////////////////////////////////////////////////////////////////////////
CString C[!output PROJECT_NAME]App::WriteIniString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue)
{
	struct INIUNIT
	{
		CString strEntry;
		CString strRet;
		int nRet;
	};
	INIUNIT IniUnit;
	IniUnit.strEntry = lpszEntry;
	IniUnit.strRet = lpszValue;
	m_ol.CommandPlat(OC_SETINISTRING, (WPARAM)lpszSection, (LPARAM)(&IniUnit));
	return IniUnit.strRet;
}

/////////////////////////////////////////////////////////////////////////////
// 写平台参数(整型)
/////////////////////////////////////////////////////////////////////////////
int C[!output PROJECT_NAME]App::WriteIniInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue)
{
	struct INIUNIT
	{
		CString strEntry;
		CString strRet;
		int nRet;
	};
	INIUNIT IniUnit;
	IniUnit.strEntry = lpszEntry;
	IniUnit.nRet = nValue;
	return m_ol.CommandPlat(OC_SETINIINT, (WPARAM)lpszSection, (LPARAM)(&IniUnit));
}

/////////////////////////////////////////////////////////////////////////////
// The one and only C[!output PROJECT_NAME]App object

C[!output PROJECT_NAME]App theApp;

/////////////////////////////////////////////////////////////////////////////
// 资源切换函数
// get_dll_resource   切换到DLL的资源
// reset_dll_resource 恢复以前的资源
/////////////////////////////////////////////////////////////////////////////
static int resource_counter = 0;		// 资源计数
static HINSTANCE save_hInstance = NULL;	// 资源保存

void get_dll_resource(void)
{
   /* this function changes the resource handle to that of the DLL */
   if (resource_counter == 0)
   {
	   AFX_MODULE_STATE* pModule= AfxGetStaticModuleState();
	   save_hInstance = AfxGetResourceHandle();
	   AfxSetResourceHandle(pModule->m_hCurrentResourceHandle);
   }

   resource_counter++;
}

void reset_dll_resource(void)
{
   /* this function restores the resource handle set by	'get_dll_resource()' */
   if (resource_counter > 0)
      resource_counter--;

   if (resource_counter == 0)
      AfxSetResourceHandle(save_hInstance);
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog

class CAboutDlg : public CDialog
{
// Construction
public:
	CAboutDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog


CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers
BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	reset_dll_resource();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// 关于OWM
/////////////////////////////////////////////////////////////////////////////
int AboutOWM(void)
{
	get_dll_resource();
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 安装OWM模块
/////////////////////////////////////////////////////////////////////////////
BOOL InstallOWM(OWM_LINKAGE& ol)
{
	// 初始化OWM结构
	ol.nOWMType				= [!if OWM_DOC]OWM_DOC[!endif][!if OWM_TABVIEW] | OWM_TABVIEW[!endif][!if OWM_CONTROLBAR] | OWM_CONTROLBAR[!endif][!if OWM_TABCTRLBAR] | OWM_TABCTRLBAR[!endif][!if OWM_MENU] | OWM_MENU[!endif][!if OWM_TOOLBAR] | OWM_TOOLBAR[!endif][!if OWM_OPTION] | OWM_OPTION[!endif][!if OWM_ONEFRAME] | OWM_ONEFRAME[!endif][!if OWM_AUTOCREATE] | OWM_AUTOCREATE[!endif] | OWM_DYNLOAD;
	ol.nOWMID				= 1000;
[!if OWM_DOC]
	ol.pDocClass			= RUNTIME_CLASS(C[!output PROJECT_NAME]Doc);
	ol.pFrameClass			= RUNTIME_CLASS(CChildFrame);
	ol.pViewClass			= RUNTIME_CLASS(C[!output PROJECT_NAME]View);
[!else]
	ol.pDocClass			= NULL
	ol.pFrameClass			= NULL;
	ol.pViewClass			= NULL;
[!endif]
[!if OWM_CONTROLBAR || OWM_TABCTRLBAR]
	ol.pCtrlBarClass		= RUNTIME_CLASS(C[!output PROJECT_NAME]Bar);
[!else]
	ol.pCtrlBarClass		= NULL;
[!endif]
	ol.SetOWM				= NULL;
	ol.InitOWM				= InitOWM;
	ol.DoneOWM				= DoneOWM;
	ol.RegisterTclCmd		= NULL;
	ol.UnregisterTclCmd		= NULL;
	ol.RegisterTclExKeyword	= NULL;
[!if OWM_MENU]
	ol.RegisterClientMenu	= RegisterClientMenu;
	ol.UnregisterClientMenu	= UnregisterClientMenu;
[!else]
	ol.RegisterClientMenu	= NULL;
	ol.UnregisterClientMenu	= NULL;
[!endif]
[!if OWM_TOOLBAR]
	ol.RegisterClientToolBar= RegisterClientToolBar;
	ol.UnregisterClientToolBar= UnregisterClientToolBar;
[!else]
	ol.RegisterClientToolBar= NULL;
	ol.UnregisterClientToolBar= NULL;
[!endif]
[!if OWM_OPTION]
	ol.RegisterOption		= RegisterOption;
[!else]
	ol.RegisterOption		= NULL;
[!endif]
	ol.SetDebugMode			= SetDebugMode;
	ol.RunHelp				= NULL;
	ol.CommandOWM			= CommandOWM;
	ol.GetDllResource		= get_dll_resource;
	ol.ResetDllResource		= reset_dll_resource;

	// 把OWM结构拷贝到本地备份
	memcpy(&(theApp.m_ol), &ol, sizeof(OWM_LINKAGE));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 卸载OWM模块
/////////////////////////////////////////////////////////////////////////////
void TerminateOWM(void)
{
	if(theApp.m_pDocTemplate)
	{
		delete theApp.m_pDocTemplate;
		theApp.m_pDocTemplate = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// 获取OWM版本
/////////////////////////////////////////////////////////////////////////////
BOOL OWMVersion(OWM_VERSION& ov)
{
	ov.svOWMName	= "[!output MODULE_NAME]";
	ov.svDescription= "[!output MODULE_DESC]";
	ov.wVersionHi	= 1;
	ov.wVersionLo	= 0;
	ov.wMVersionHi	= 1;
	ov.wMVersionLo	= 0;
	ov.nLanguage	= 0;
	ov.bDebugVer	= FALSE;
#ifdef _DEBUG
	ov.bDebugVer	= TRUE;
#endif //_DEBUG

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 设置调试模式
/////////////////////////////////////////////////////////////////////////////
int SetDebugMode(BOOL bDebug)
{
	theApp.m_bDebug	= bDebug;

	return bDebug;
}

[!if OWM_DOC]
/////////////////////////////////////////////////////////////////////////////
// 取第一个C[!output PROJECT_NAME]View
/////////////////////////////////////////////////////////////////////////////
C[!output PROJECT_NAME]View* GetFirst[!output PROJECT_NAME]View()
{
	if(!theApp.m_ol.pDocTemplate)
		return NULL;

	POSITION posdoc = theApp.m_ol.pDocTemplate->GetFirstDocPosition();
	if(posdoc)
	{
		CDocument* pDoc = theApp.m_ol.pDocTemplate->GetNextDoc(posdoc);
		POSITION posview = pDoc->GetFirstViewPosition();
		if(posview)
		{
			return (C[!output PROJECT_NAME]View*)(pDoc->GetNextView(posview));
		}
	}
	return NULL;
}

[!endif]
/////////////////////////////////////////////////////////////////////////////
// OWM命令帮助
/////////////////////////////////////////////////////////////////////////////
int OWMCmdHelp(int nCmd, CStringArray* aHelpStr)
{
	switch(nCmd)
	{
	case OC_CMDALL:
		aHelpStr->Add("Help of OWM[[!output PROJECT_NAME]]:");
		aHelpStr->Add("ID      : 1000");
		aHelpStr->Add("Auhtor  : [!output AUTH_NAME]");
		aHelpStr->Add("Version : 1.0");
		aHelpStr->Add("Descript: [!output MODULE_DESC]");
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

// 字体配置表
//static struct FontItem g_FontItems[] =
//{
//	FontItem(FON_BK,					// 掩码
//			"测试单元背景",				// 显示名字
//			"FTList.TestUnit",			// 注册表项名
//			"宋体", 16,					// 字体
//			RGB(0,0,0), CLR_DEFAULT,	// 文字和背景色
//			FALSE, FALSE, FALSE, FALSE),// 风格
//};

/////////////////////////////////////////////////////////////////////////////
// 从注册表获取最新的字体配置表信息
/////////////////////////////////////////////////////////////////////////////
void GetFontFromReg(FontItem* pFontItem)
{
	CString strSubKey;
	strSubKey.Format("%s\\%s", REG_FORMAT_SUBKEY, pFontItem->strSubKey);
	pFontItem->strFont		= AfxGetApp()->GetProfileString(strSubKey, REG_FMFONT, pFontItem->strFont);	// 字体名
	pFontItem->nSize		= AfxGetApp()->GetProfileInt(strSubKey, REG_FMSIZE, pFontItem->nSize);	// 字体大小
	pFontItem->colorFg		= AfxGetApp()->GetProfileInt(strSubKey, REG_FMCOLORFG, pFontItem->colorFg);	// 前景色
	pFontItem->colorBk		= AfxGetApp()->GetProfileInt(strSubKey, REG_FMCOLORBK, pFontItem->colorBk);	// 背景色
	pFontItem->bBold		= AfxGetApp()->GetProfileInt(strSubKey, REG_FMBOLD, pFontItem->bBold);	// 是否粗体
	pFontItem->bItalic		= AfxGetApp()->GetProfileInt(strSubKey, REG_FMITALIC, pFontItem->bItalic);	// 是否斜体
	pFontItem->bUnderLine	= AfxGetApp()->GetProfileInt(strSubKey, REG_FMUNDERLINE, pFontItem->bUnderLine);	// 是否下划线
	pFontItem->bStrikeOut	= AfxGetApp()->GetProfileInt(strSubKey, REG_FMSTRIKEOUT, pFontItem->bStrikeOut);	// 是否删除线
}

/////////////////////////////////////////////////////////////////////////////
// OWM命令处理函数
/////////////////////////////////////////////////////////////////////////////
int CommandOWM(int nCmd, WPARAM wParam, LPARAM lParam)
{
	switch(nCmd)
	{
	case OC_SETOWMLINK:	// 设置OWM链接(系统命令,请勿改动)
		{
			if(wParam == NULL)
				return FALSE;

			// 把OWM结构拷贝到本地备份
			memcpy(&(theApp.m_ol), (OWM_LINKAGE*)wParam, sizeof(OWM_LINKAGE));

			return TRUE;
		}

	case OC_CMDHELP:	// 命令帮助
		{
			return OWMCmdHelp(wParam, (CStringArray*)lParam);
		}

[!if OWM_DOC]
	case OC_CLOSEVIEW:	// 关闭OWM窗口
		{
			C[!output PROJECT_NAME]View* pView = GetFirst[!output PROJECT_NAME]View();
			if(!pView)
				return FALSE;
			CWnd* pFrame = pView->GetParent();
			if(!pFrame)
				return FALSE;
			pFrame->SendMessage(WM_CLOSE, 0, 0);
			return TRUE;
		}
[!endif]
//	注:如果需要配置某些字体或颜色,可以打开下面命令的注释
//	case OC_GETFONTARRAY:	// 获取字体配置表
//		{
//			int* pnItemCount = (int*)wParam;
//			*pnItemCount = 1;	// 配置项个数
//			for(int i=0; i<(*pnItemCount); i++)
//			{
//				GetFontFromReg(g_FontItems+i);
//			}
//
//			if(lParam == 1)	// 1表示还原为缺省值
//			{
//				g_FontItems[0].colorBk = CLR_DEFAULT;
//			}
//
//			return (int)g_FontItems;
//		}

	default:
		break;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 向平台查询OWM安装信息
/////////////////////////////////////////////////////////////////////////////
COWMArray* GetOwmInfo(int nId)
{
	if(!theApp.m_ol.CommandPlat)
		return NULL;

	return (COWMArray*)theApp.m_ol.CommandPlat(OC_QUERYOWMID, nId, 0);
}

[!if OWM_MENU]
/////////////////////////////////////////////////////////////////////////////
// 注册OWM扩展菜单
/////////////////////////////////////////////////////////////////////////////
int RegisterClientMenu(CMenu* pMenu, COWMMenuArray& aOWMMenus, int nOWMIdStart, CDocument* pDoc)
{
	//DLL_LOADSTRING(strEditMenu, IDS_EDIT_MENU);
	//PLATUI_ADDMENUITEM(strEditMenu, IDR_OWM_VIEW, 0, OnView[!output PROJECT_NAME], OnUpdateView[!output PROJECT_NAME]);
	return aOWMMenus.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
// 卸载OWM扩展菜单命令
/////////////////////////////////////////////////////////////////////////////
int UnregisterClientMenu(COWMMenuArray& aOWMMenus)
{
	return aOWMMenus.GetSize();
}

[!endif]
[!if OWM_CONTROLBAR || OWM_TABCTRLBAR]
/////////////////////////////////////////////////////////////////////////////
// 显示/隐藏OWM ControlBar
/////////////////////////////////////////////////////////////////////////////
int OpenOwmBar()
{
	COWMArray* pOWM = GetOwmInfo(theApp.m_ol.nOWMID);
	if(pOWM)
	{
		CControlBar* pBar = (pOWM->GetAt(0)).pBar;
		if(pBar && pBar->IsWindowVisible())
			theApp.m_ol.CommandPlat(OC_CLOSEBAR, theApp.m_ol.nOWMID, 1);
		else
			theApp.m_ol.CommandPlat(OC_OPENBAR, theApp.m_ol.nOWMID, 1);
		delete pOWM;
	}
	return 0;
}

void UpdateOpenOwmBar(CCmdUI* pCmdUI)
{
	int nCheck = 0;
	COWMArray* pOWM = GetOwmInfo(theApp.m_ol.nOWMID);
	if(pOWM && (pOWM->GetAt(0)).pBar)
	{
		if((pOWM->GetAt(0)).pBar->IsWindowVisible())
			nCheck = 1;
	}
	delete pOWM;
	pCmdUI->SetCheck(nCheck);
}

[!endif]
[!if OWM_DOC]
/////////////////////////////////////////////////////////////////////////////
// 显示/隐藏OWM窗口
/////////////////////////////////////////////////////////////////////////////
int OpenOwmView()
{
	if(GetFirst[!output PROJECT_NAME]View())
		theApp.m_ol.CommandPlat(OC_CLOSEVIEW, theApp.m_ol.nOWMID, 0);
	else
		theApp.m_ol.CommandPlat(OC_OPENVIEW, theApp.m_ol.nOWMID, 0);
	return 0;
}

void UpdateOpenOwmView(CCmdUI* pCmdUI)
{
	int nCheck = GetFirst[!output PROJECT_NAME]View() ? 1 : 0;
	pCmdUI->SetCheck(nCheck);
}

[!endif]
[!if OWM_TOOLBAR]
/////////////////////////////////////////////////////////////////////////////
// 注册OWM ToolBar
/////////////////////////////////////////////////////////////////////////////
int RegisterClientToolBar(CWnd* pBar, COWMMenuArray& aOWMMenus, int nOWMIdStart, int nIndex)
{
	CToolBar* pToolBar = (CToolBar*)pBar;

	// 加载ToolBar资源
	CToolBarCtrl* pCtrl = &(pToolBar->GetToolBarCtrl());
	CImageList* pImage = pCtrl->GetImageList();
	int count = pImage->GetImageCount();
	CBitmap bitmap;
	get_dll_resource();
	bitmap.LoadBitmap(IDR_OWM);
	pImage->Add(&bitmap, RGB(0, 128, 128));
	reset_dll_resource();

	// 在此添加ToolBar
[!if OWM_CONTROLBAR || OWM_TABCTRLBAR]
	OWM_REGISTERTOOLBAR(count, OpenOwmBar, UpdateOpenOwmBar, USER_NONE)
[!else]
	OWM_REGISTERTOOLBAR(count, OpenOwmView, UpdateOpenOwmView, USER_NONE)
[!endif]

	return aOWMMenus.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
// 卸载OWM ToolBar
/////////////////////////////////////////////////////////////////////////////
int UnregisterClientToolBar(CWnd* pBar, COWMMenuArray& aOWMMenus, int nOWMIdStart)
{
	CToolBar* pToolBar = (CToolBar*)pBar;

	// 在此注消ToolBar
	OWM_UNREGISTERTOOLBAR(OpenOwmView)

	return aOWMMenus.GetSize();
}

[!endif]
[!if OWM_OPTION]
/////////////////////////////////////////////////////////////////////////////
// 注册OWM属性页
/////////////////////////////////////////////////////////////////////////////
int RegisterOption(COWMOptionArray& aOWMOption)
{
	// 在此添加属性页
	//CString strTemp , strAbout;
	//LOADRCSTRING(strTemp, IDS_PROP_STR_[!output PROJECT_NAME])
	//LOADRCSTRING(strAbout, IDS_PROP_STR_ABOUT_[!output PROJECT_NAME])

	//OWM_REGISTEROPTION("", strTemp, strAbout, IDD_TPS_[!output PROJECT_NAME], CTps[!output PROJECT_NAME], 0)

	return aOWMOption.GetSize();
}

[!endif]
/////////////////////////////////////////////////////////////////////////////
// 初始化OWM
/////////////////////////////////////////////////////////////////////////////
int InitOWM()
{
	// 注:在下面添加你的代码

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 卸载OWM
/////////////////////////////////////////////////////////////////////////////
int DoneOWM()
{
	// 注:在下面添加你的代码

	return 0;
}


//////////////////////////////////////////////////////////////////////////
// 全局变量
//////////////////////////////////////////////////////////////////////////
const TInterfaceInfo INTERFACE_LIST[] =
{
	INF_IVciOwm,
};

//////////////////////////////////////////////////////////////////////////
// OWMVCI输出的函数
//////////////////////////////////////////////////////////////////////////
//
// 查询本 VCI 输出的接口列表，返回接口数目
//
extern "C" __declspec(dllexport)
int GetInterfaceList(const TInterfaceInfo **pInfo)
{
	*pInfo = INTERFACE_LIST;
	return (sizeof(INTERFACE_LIST)/sizeof(INTERFACE_LIST[0]));
}

//
// 产生对象并返回对象的接口
//
extern "C" __declspec(dllexport)
LPVOID CreateObject(LPCSTR lpcsInterfaceName, LPVOID* lppVciControl, LPVOID lpInitData)
{
	TRACE("CreateObject(\"%s\")-OWM-[!output PROJECT_NAME]\n", lpcsInterfaceName);

	CVisualComponent *pObject;
	if(strcmp(lpcsInterfaceName, IID_IVciOwm) == 0)
	{
		pObject = new CVciOwm;
		OWM_LINKAGE ol;
		InstallOWM(ol);
		//get_dll_resource();
		//theApp.m_ol.pDocTemplate = new CMultiDocTemplate(
		//	NULL,
		//	ol.pDocClass,
		//	ol.pFrameClass,
		//	ol.pViewClass);
		//reset_dll_resource();
	}
	else
	{
		TRACE("Can't find Object: %s, CreateObject failed!\n", lpcsInterfaceName);
		return NULL;
	}

	// 为组件安装调试信息输出回调函数
	if(lpInitData)
	{
		TInitData_DebugOut *pInitData_DebugOut = (TInitData_DebugOut *)lpInitData;
		pObject->InstallHandler(DEBUG_OUT_EVENT, pInitData_DebugOut->lpfnDebugOut, pInitData_DebugOut->nDeviceID);
	}

	*lppVciControl = pObject->VciControlQueryInterface();

	return pObject->ExternalQueryInterface();
}