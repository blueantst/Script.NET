// OwmGrep.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "OwmGrep.h"
#include "ChildFrame.h"
#include "OwmGrepView.h"
#include "OwmGrepDoc.h"
#include "SearchOptions.h"
#include "TpsGrep.h"

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
// COwmGrepApp

BEGIN_MESSAGE_MAP(COwmGrepApp, CWinApp)
	//{{AFX_MSG_MAP(COwmGrepApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COwmGrepApp construction

COwmGrepApp::COwmGrepApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bDebug	= FALSE;
	m_pDocTemplate = NULL;
	m_pIPlatUI = NULL;
	m_pSearchOptionsView = NULL;
	m_pSearchResultView = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 获取模块所在路径
/////////////////////////////////////////////////////////////////////////////
CString COwmGrepApp::GetModulePath()
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
CString COwmGrepApp::GetIniString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault)
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
int COwmGrepApp::GetIniInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault)
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
CString COwmGrepApp::WriteIniString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue)
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
int COwmGrepApp::WriteIniInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue)
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

CSearchOptionsView* COwmGrepApp::GetSearchOptionsView()
{
	CSearchOptionsView* pWnd = (CSearchOptionsView*)(theApp.m_pIPlatUI->GetDockingPaneWnd(VIEW_ID_GREP));
	if(pWnd)
	{
		CSearchOptionsView* pView = (CSearchOptionsView*)(pWnd->GetDescendantWindow(AFX_IDW_PANE_FIRST));
		return pView;
	}

	return NULL;
}

COwmGrepView* COwmGrepApp::GetSearchResultView(int nIndex)
{
	COwmGrepView* pWnd = (COwmGrepView*)(theApp.m_pIPlatUI->GetDockingPaneWnd(VIEW_ID_SEARCH_RESULT));
	if(pWnd)
	{
		COwmGrepView* pView = (COwmGrepView*)(pWnd->GetDescendantWindow(AFX_IDW_PANE_FIRST));
		return pView;
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only COwmGrepApp object

COwmGrepApp theApp;

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


// 声明OWM函数
int RegisterClientMenu(CMenu* pMenu, COWMMenuArray& aOWMMenus, int nOWMIdStart, CDocument* pDoc);
int UnregisterClientMenu(COWMMenuArray& aOWMMenus);
int RegisterClientToolBar(CWnd* pBar, COWMMenuArray& aOWMMenus, int nOWMIdStart, int nIndex);
int UnregisterClientToolBar(CWnd* pBar, COWMMenuArray& aOWMMenus, int nOWMIdStart);
int RegisterOption(COWMOptionArray& aOWMOption);
int SetDebugMode(BOOL bDebug);
int CommandOWM(int nCmd, WPARAM wParam, LPARAM lParam);
int InitOWM();
int DoneOWM();

/////////////////////////////////////////////////////////////////////////////
// 安装OWM模块
/////////////////////////////////////////////////////////////////////////////
BOOL InstallOWM(OWM_LINKAGE& ol)
{
	// 初始化OWM结构
	ol.nOWMType				= OWM_DOC | OWM_MENU | OWM_TOOLBAR | OWM_OPTION | OWM_DYNLOAD;
	ol.nOWMID				= 1000;
	ol.pDocClass			= RUNTIME_CLASS(COwmGrepDoc);
	ol.pFrameClass			= NULL;//RUNTIME_CLASS(CChildFrame);
	ol.pViewClass			= RUNTIME_CLASS(COwmGrepView);
	ol.pCtrlBarClass		= NULL;
	ol.SetOWM				= NULL;
	ol.InitOWM				= InitOWM;
	ol.DoneOWM				= DoneOWM;
	ol.RegisterTclCmd		= NULL;
	ol.UnregisterTclCmd		= NULL;
	ol.RegisterTclExKeyword	= NULL;
	ol.RegisterClientMenu	= RegisterClientMenu;
	ol.UnregisterClientMenu	= UnregisterClientMenu;
	ol.RegisterClientToolBar= RegisterClientToolBar;
	ol.UnregisterClientToolBar= UnregisterClientToolBar;
	ol.RegisterOption		= RegisterOption;
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
}

/////////////////////////////////////////////////////////////////////////////
// 获取OWM版本
/////////////////////////////////////////////////////////////////////////////
BOOL OWMVersion(OWM_VERSION& ov)
{
	ov.svOWMName	= "OwmGrep";
	ov.svDescription= "OwmGrep OWM";
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

/////////////////////////////////////////////////////////////////////////////
// 取第一个COwmGrepView
/////////////////////////////////////////////////////////////////////////////
COwmGrepView* GetFirstOwmGrepView()
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
			return (COwmGrepView*)(pDoc->GetNextView(posview));
		}
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// OWM命令帮助
/////////////////////////////////////////////////////////////////////////////
int OWMCmdHelp(int nCmd, CStringArray* aHelpStr)
{
	switch(nCmd)
	{
	case OC_CMDALL:
		aHelpStr->Add("Help of OWM[OwmGrep]:");
		aHelpStr->Add("ID      : 1000");
		aHelpStr->Add("Auhtor  : blueant");
		aHelpStr->Add("Version : 1.0");
		aHelpStr->Add("Descript: OwmGrep OWM");
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

	case OC_CLOSEVIEW:	// 关闭OWM窗口
		{
			COwmGrepView* pView = GetFirstOwmGrepView();
			if(!pView)
				return FALSE;
			CWnd* pFrame = pView->GetParent();
			if(!pFrame)
				return FALSE;
			pFrame->SendMessage(WM_CLOSE, 0, 0);
			return TRUE;
		}

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

	case OC_GREP_SETTEXTFIND:	// 设置查找字符串
		{
			CSearchOptionsView* pOptionsView = theApp.GetSearchOptionsView();
			if(pOptionsView)
			{
				pOptionsView->m_cmbFind.SetWindowText((LPCTSTR)wParam);
				pOptionsView->UpdateOptions();
			}
			return TRUE;
		}

	case OC_GREP_GETTEXTFIND:	// 获取查找字符串
		{
			CSearchOptions* pOptions = GetSearchOptions();
			if(pOptions)
			{
				return (int)(LPCTSTR)(pOptions->m_strFind);
			}
			return NULL;
		}

	case OC_GREP_SETTEXTREPLACE:	// 设置替换字符串
		{
			CSearchOptionsView* pOptionsView = theApp.GetSearchOptionsView();
			if(pOptionsView)
			{
				pOptionsView->m_cmbReplace.SetWindowText((LPCTSTR)wParam);
				pOptionsView->UpdateOptions();
			}
			return TRUE;
		}

	case OC_GREP_GETTEXTREPLACE:	// 获取替换字符串
		{
			CSearchOptions* pOptions = GetSearchOptions();
			if(pOptions)
			{
				return (int)(LPCTSTR)(pOptions->m_strReplace);
			}
			return NULL;
		}

	case OC_GREP_SETLOCATION:	// 设置位置
		{
			CSearchOptionsView* pOptionsView = theApp.GetSearchOptionsView();
			if(pOptionsView)
			{
				switch(wParam)
				{
				case GREP_LOC_CURRENTDOC:pOptionsView->m_cmbPath.SetWindowText(LOC_CURRENTDOC);break;
				case GREP_LOC_OPENDOCS:pOptionsView->m_cmbPath.SetWindowText(LOC_OPENDOCS);break;
				case GREP_LOC_CURRENTPRJ:pOptionsView->m_cmbPath.SetWindowText(LOC_CURRENTPRJ);break;
				case GREP_LOC_PATH:pOptionsView->m_cmbPath.SetWindowText((LPCTSTR)lParam);break;
				}
				pOptionsView->UpdateOptions();
			}
			return TRUE;
		}

	case OC_GREP_GETLOCATION:	// 获取位置
		{
			CSearchOptions* pOptions = GetSearchOptions();
			if(pOptions)
			{
				return (int)(LPCTSTR)(pOptions->m_strPath);
			}
			return NULL;
		}

	case OC_GREP_SETOPTION:	// 设置查找替换设置
		{
			SearchOption* pOption = (SearchOption*)wParam;
			CSearchOptions* pOptions = GetSearchOptions();
			if(pOption && pOptions)
			{
				pOptions->m_bIncludeSubFolders = pOption->m_bIncludeSubFolders;
				pOptions->m_bRegularExpressions = pOption->m_bRegularExpressions;
				pOptions->m_strFind = pOption->m_strFind;
				pOptions->m_strReplace = pOption->m_strReplace;
				pOptions->m_strPath = pOption->m_strPath;
				pOptions->m_strFileTypes = pOption->m_strFileTypes;
				pOptions->m_bMatchCase = pOption->m_bMatchCase;
				pOptions->m_bMatchWholeWords = pOption->m_bMatchWholeWords;
			}
			CSearchOptionsView* pOptionsView = theApp.GetSearchOptionsView();
			if(pOptionsView)
			{
				pOptionsView->UpdateOptions();
			}
			return TRUE;
		}

	case OC_GREP_GETOPTION:	// 获取查找替换设置
		{
			CSearchOptions* pOptions = GetSearchOptions();
			if(pOptions)
			{
				static SearchOption option;
				option.m_bIncludeSubFolders = pOptions->m_bIncludeSubFolders;
				option.m_bRegularExpressions = pOptions->m_bRegularExpressions;
				option.m_strFind = pOptions->m_strFind;
				option.m_strReplace = pOptions->m_strReplace;
				option.m_strPath = pOptions->m_strPath;
				option.m_strFileTypes = pOptions->m_strFileTypes;
				option.m_bMatchCase = pOptions->m_bMatchCase;
				option.m_bMatchWholeWords = pOptions->m_bMatchWholeWords;
				return (int)(&option);
			}
			return NULL;
		}

	case OC_GREP_CLEARRESULT:	// 清空结果行
		{
			COwmGrepView* pResultView = theApp.GetSearchResultView();
			if(pResultView)
			{
				pResultView->ClearRecord();
			}
			return TRUE;
		}

	case OC_GREP_ADDRESULTLINE:	// 添加结果行
		{
			int nResultView = wParam;
			SearchResultInfo* pResultInfo = (SearchResultInfo*)lParam;
			COwmGrepView* pResultView = theApp.GetSearchResultView();
			if(pResultView)
			{
				pResultView->AddRecord(pResultInfo->_strPath, pResultInfo->_nLine, pResultInfo->_strLine);
			}
			return TRUE;
		}

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

/////////////////////////////////////////////////////////////////////////////
// 显示查找替换Dockingbar
/////////////////////////////////////////////////////////////////////////////
void OnViewFindSearch() 
{
	if(theApp.m_pIPlatUI)
	{
		theApp.m_pIPlatUI->CreateDockingPane("org.owm.grep", VIEW_ID_GREP);
	}
}

void OnUpdateViewFindSearch(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 显示查找结果窗口
/////////////////////////////////////////////////////////////////////////////
void OnViewFindResult() 
{
	if(theApp.m_pIPlatUI)
	{
		theApp.m_pIPlatUI->CreateDockingPane("org.owm.grep", VIEW_ID_SEARCH_RESULT);
	}
}

void OnUpdateViewFindResult(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 注册OWM扩展菜单
/////////////////////////////////////////////////////////////////////////////
int RegisterClientMenu(CMenu* pMenu, COWMMenuArray& aOWMMenus, int nOWMIdStart, CDocument* pDoc)
{
	DLL_LOADSTRING(strEditMenu, IDS_EDIT_MENU);
	PLATUI_ADDMENUITEM(strEditMenu, IDR_OWM_VIEW, 0, OnViewFindSearch, OnUpdateViewFindSearch);
	return aOWMMenus.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
// 卸载OWM扩展菜单命令
/////////////////////////////////////////////////////////////////////////////
int UnregisterClientMenu(COWMMenuArray& aOWMMenus)
{
	return aOWMMenus.GetSize();
}


/////////////////////////////////////////////////////////////////////////////
// 显示/隐藏OWM窗口
/////////////////////////////////////////////////////////////////////////////
int OpenOwmView()
{
	if(GetFirstOwmGrepView())
		theApp.m_ol.CommandPlat(OC_CLOSEVIEW, theApp.m_ol.nOWMID, 0);
	else
		theApp.m_ol.CommandPlat(OC_OPENVIEW, theApp.m_ol.nOWMID, 0);
	return 0;
}

void UpdateOpenOwmView(CCmdUI* pCmdUI)
{
	int nCheck = GetFirstOwmGrepView() ? 1 : 0;
	pCmdUI->SetCheck(nCheck);
}

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
	OWM_REGISTERTOOLBAR(count, OpenOwmView, UpdateOpenOwmView, USER_NONE)

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

/////////////////////////////////////////////////////////////////////////////
// 注册OWM属性页
/////////////////////////////////////////////////////////////////////////////
int RegisterOption(COWMOptionArray& aOWMOption)
{
	// 在此添加属性页
	CString strTemp , strAbout;
	LOADRCSTRING(strTemp, IDS_PROP_STR_GREP)
	LOADRCSTRING(strAbout, IDS_PROP_STR_ABOUT_GREP)

	OWM_REGISTEROPTION("", strTemp, strAbout, IDD_TPS_GREP, CTpsGrep, 0)

	return aOWMOption.GetSize();
}

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

/////////////////////////////////////////////////////////////////////////////
//

#include "vcicomm.h"
#include "vciowm.h"

#define INF_IVciOwm \
{\
	"IVciOwm",\
	"1.0.0.0",\
	THIS_REVISION_TYPE,\
	"org.owm.grep",\
	"Grep OWM",\
	"Blueant",\
	__DATE__\
}
const TInterfaceInfo INTERFACE_INFO_IVciOwm = INF_IVciOwm;

class CVciOwm : public CVisualComponent
{
public:
	CVciOwm() {};
	~CVciOwm() {};

//////////////////////////////////////////////////////////////////////////
// 提供给接口的功能函数
//////////////////////////////////////////////////////////////////////////
public:
	virtual int __stdcall SetDebugMode(BOOL bDebug);
	virtual int __stdcall SetLanguage(int nLanguage);
	virtual int __stdcall Init(LPCSTR lpcsParams);
	virtual int __stdcall Done(LPCSTR lpcsParams);
	virtual int __stdcall ProcessMessage(CVciMessage* pIn, CVciMessage* ppOut);

private:

//////////////////////////////////////////////////////////////////////////
// 要输出的功能接口
//////////////////////////////////////////////////////////////////////////
protected:
	// 接口定义
	BEGIN_INTERFACE_PART(VciControl, IVciControl)
		CONTROL_INTERFACE_DEFINE
	END_INTERFACE_PART(VciControl)
	EXPORT_CONTROL_INTERFACE_PART(VciControl)

	BEGIN_INTERFACE_PART(VciOwm, IVciOwm)
		OWM_INTERFACE_DEFINE
	END_INTERFACE_PART(VciOwm)
	EXPORT_INTERFACE_PART(VciOwm)
};

//////////////////////////////////////////////////////////////////////////
// 接口实现
//////////////////////////////////////////////////////////////////////////
INTERFACE_IMPLEMENT(VciOwm)
CONTROL_INTERFACE_IMPLEMENT(VciOwm)

// 设置调试模式
int __stdcall CVciOwm::SetDebugMode(BOOL bDebug)
{
	theApp.m_bDebug	= bDebug;
	return CVisualComponent::SetDebugMode(bDebug);
}

// 设置语言
int __stdcall CVciOwm::SetLanguage(int nLanguage)
{
	theApp.m_nLanguage = nLanguage;
	return CVisualComponent::SetLanguage(nLanguage);
}

// 初始化组件
int __stdcall CVciOwm::Init(LPCSTR lpcsParams)
{
	return InitOWM();
}

// 结束组件
int __stdcall CVciOwm::Done(LPCSTR lpcsParams)
{
	return DoneOWM();
}

// 消息处理
int __stdcall CVciOwm::ProcessMessage(CVciMessage* pIn, CVciMessage* ppOut)
{
	return 0;
}

// 查询命令是否支持
int gQueryCommandIsSupport(LPCSTR lpcsCmd)
{
	//if(strcmp(lpcsCmd, "HELLO") == 0)
	//{
	//}
	return 0;
}

//{{ 注意：!!!这里是保留的重要函数，不可删除!!!
// 获取接口的额外信息
LPVOID __stdcall CVciOwm::XVciOwm::GetExtraInfo(LPVOID lpRefData)
{
	CVciOwm *pObj = GET_INTERFACE_OBJECT(VciOwm);
	return pObj;
}
//}} 注意：!!!这里是保留的重要函数，不可删除!!!

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

// 显示DockingPane窗口时调用的函数
CWnd* __stdcall CVciOwm::XVciOwm::OnShowDockingPane(int nID, CWnd* pParentWnd, LPCTSTR lpszParam)
{
	CWnd* pWnd = NULL;
	if(nID == VIEW_ID_GREP)
	{
		get_dll_resource();
		pWnd = (CWnd*)(RUNTIME_CLASS(CSearchOptionsView));
		theApp.m_pSearchOptionsView = pWnd;
	}else
	if(nID == VIEW_ID_SEARCH_RESULT)
	{
		pWnd = (CWnd*)(RUNTIME_CLASS(COwmGrepView));
		theApp.m_pSearchResultView = pWnd;
	}
	
	return pWnd;
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
	TRACE("CreateObject(\"%s\")-OWM-org.owm.grep\n", lpcsInterfaceName);

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