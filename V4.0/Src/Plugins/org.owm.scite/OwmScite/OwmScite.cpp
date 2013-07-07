// OwmScite.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "OwmScite.h"
#include "ChildFrame.h"
#include "OwmSciteView.h"
#include "OwmSciteDoc.h"
#include "TpsGeneral.h"
#include "TpsNewDocument.h"
#include "TpsStyle.h"
#include "TpsPrintPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// COwmSciteApp

BEGIN_MESSAGE_MAP(COwmSciteApp, CWinApp)
	//{{AFX_MSG_MAP(COwmSciteApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COwmSciteApp construction

COwmSciteApp::COwmSciteApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bDebug	= FALSE;
	m_pDocTemplate = NULL;
	m_pIPlatUI = NULL;
	m_bScriptDebuging = FALSE;

	m_hScintillaDll = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 读平台参数(字符型)
/////////////////////////////////////////////////////////////////////////////
CString COwmSciteApp::GetIniString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault)
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
int COwmSciteApp::GetIniInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault)
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
CString COwmSciteApp::WriteIniString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue)
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
int COwmSciteApp::WriteIniInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue)
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
// 获取平台所在路径
/////////////////////////////////////////////////////////////////////////////
CString COwmSciteApp::GetPlatRootPath()
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
// 取平台Lib目录
/////////////////////////////////////////////////////////////////////////////
CString COwmSciteApp::GetPlatLibPath()
{
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

	szPath += _T("Lib\\");

	return szPath;
}

/////////////////////////////////////////////////////////////////////////////
// 获取模块所在路径
/////////////////////////////////////////////////////////////////////////////
CString COwmSciteApp::GetModulePath()
{
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
// 执行平台文件管理脚本,可以带参数
/////////////////////////////////////////////////////////////////////////////
BOOL COwmSciteApp::ExecFileMngScript(CString strParam)
{
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
	szPath += _T("Lib\\plat\\filemng.tcl");

	// 创建解释器
	IInterp* pInterp = NULL;
	if(m_pIPlatUI)
	{
		pInterp = (IInterp*)(m_pIPlatUI->CreateVciObject("org.interp.tcl", ""));
	}
	if(pInterp == NULL)
	{
		return FALSE;
	}

	// 执行脚本
	CString strCmdLine;
	strCmdLine.Format("\"%s\" %s", szPath, strParam);
	// 传递文件名要进行斜杠替换
	strCmdLine.Replace('\\', '/');
	pInterp->RunScriptFile(strCmdLine);

	// 释放解释器
	m_pIPlatUI->ReleaseVciObject(pInterp, FALSE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 获取执行当前文件的解释器ID
/////////////////////////////////////////////////////////////////////////////
CString COwmSciteApp::GetExecuteInterp(CString strDocId)
{
	if(!m_pIPlatUI)
	{
		return "";
	}

	CStringArray arExecuteModule;
	int nCount = m_pIPlatUI->GetVciDocExecuteModule("org.owm.edit", strDocId, "interp", arExecuteModule);

	if(nCount > 0)
	{
		return arExecuteModule[0];
	}

	return "";
}

/////////////////////////////////////////////////////////////////////////////
// 刷新所有窗口
/////////////////////////////////////////////////////////////////////////////
void COwmSciteApp::RefreshAllSciteView()
{
	if(!m_ol.pDocTemplate)
		return;

	POSITION posdoc = m_ol.pDocTemplate->GetFirstDocPosition();
	while(posdoc)
	{
		CDocument* pDoc = m_ol.pDocTemplate->GetNextDoc(posdoc);
		if(pDoc)
		{
			POSITION posview = pDoc->GetFirstViewPosition();
			if(posview)
			{
				COwmSciteView* pView = (COwmSciteView*)(pDoc->GetNextView(posview));
				pView->m_SciEdit.defineDocType(pView->getLangType());
				pView->m_SciEdit.performGlobalStyles();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// The one and only COwmSciteApp object

COwmSciteApp theApp;

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

// 设置线程中的语言类型
void SetLocateInThread()
{
	int nCurrentLanguage = AfxGetApp()->GetProfileInt(REG_VERSION_SUBKEY, REG_REG_LANGUAGE, ::GetSystemDefaultLangID());
	if(LANGUAGE_PAGE_CHINESE == nCurrentLanguage)
	{
		::SetThreadLocale(MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_DEFAULT));
	}else
	{
		::SetThreadLocale(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));
	}
}

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
	ol.nOWMID				= 100;
	ol.pDocClass			= RUNTIME_CLASS(COwmSciteDoc);
	ol.pFrameClass			= RUNTIME_CLASS(CChildFrame);
	ol.pViewClass			= RUNTIME_CLASS(COwmSciteView);
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
	ov.svOWMName	= "OwmScite";
	ov.svDescription= "OwmScite OWM";
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
// 取第一个COwmSciteView
/////////////////////////////////////////////////////////////////////////////
COwmSciteView* GetFirstOwmSciteView()
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
			return (COwmSciteView*)(pDoc->GetNextView(posview));
		}
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 根据文件路径，查找视图对象指针
/////////////////////////////////////////////////////////////////////////////
COwmSciteView* GetSciteEditView(CString strFilePath, BOOL bActive)
{
	if(!theApp.m_ol.pDocTemplate)
		return NULL;

	strFilePath.Replace("/", "\\");
	strFilePath.Replace("\\.\\", "\\");	// 将无用的路径去掉

	POSITION posdoc = theApp.m_ol.pDocTemplate->GetFirstDocPosition();
	while(posdoc)
	{
		CDocument* pDoc = theApp.m_ol.pDocTemplate->GetNextDoc(posdoc);
		CString strPathName = pDoc->GetPathName();
		strPathName.MakeUpper();
		strFilePath.MakeUpper();
		if(strFilePath == strPathName)
		{
			POSITION posview = pDoc->GetFirstViewPosition();
			if(posview)
			{
				COwmSciteView* pView = (COwmSciteView*)(pDoc->GetNextView(posview));
				if(bActive)
				{
					theApp.m_ol.pDocTemplate->OpenDocumentFile(pDoc->GetPathName());
				}
				return pView;
			}
			return NULL;
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 根据ScintillaEditView控件指针，查找视图对象指针
/////////////////////////////////////////////////////////////////////////////
COwmSciteView* GetSciteEditViewWithEditor(ScintillaEditView* pEditor)
{
	if(!theApp.m_ol.pDocTemplate)
		return NULL;

	POSITION posdoc = theApp.m_ol.pDocTemplate->GetFirstDocPosition();
	while(posdoc)
	{
		CDocument* pDoc = theApp.m_ol.pDocTemplate->GetNextDoc(posdoc);
		if(pDoc)
		{
			POSITION posview = pDoc->GetFirstViewPosition();
			if(posview)
			{
				COwmSciteView* pView = (COwmSciteView*)(pDoc->GetNextView(posview));
				if((&pView->m_SciEdit) == pEditor)
				{
					return pView;
				}
			}
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 获取当前激活的视图
/////////////////////////////////////////////////////////////////////////////
COwmSciteView* GetActiveSciteEditView()
{
	if(!theApp.m_ol.pDocTemplate)
		return NULL;

	// 获取当前激活窗口Frame
	CChildFrame* pFrame = (CChildFrame*)((CMDIFrameWnd*)AfxGetMainWnd())->MDIGetActive();
	if(pFrame)
	{
		CView* pActiveView = pFrame->GetActiveView();

		POSITION posdoc = theApp.m_ol.pDocTemplate->GetFirstDocPosition();
		while(posdoc)
		{
			CDocument* pDoc = theApp.m_ol.pDocTemplate->GetNextDoc(posdoc);
			POSITION posview = pDoc->GetFirstViewPosition();
			if(posview)
			{
				CView* pView = pDoc->GetNextView(posview);
				if(pView->GetParentFrame()->GetActiveView() == pActiveView)
				{
					return (COwmSciteView*)pView;
				}
			}
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
		aHelpStr->Add("Help of OWM[OwmScite]:");
		aHelpStr->Add("ID      : 100");
		aHelpStr->Add("Auhtor  : blueant");
		aHelpStr->Add("Version : 1.0");
		aHelpStr->Add("Descript: OwmScite OWM");
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
			COwmSciteView* pView = GetFirstOwmSciteView();
			if(!pView)
				return FALSE;
			CWnd* pFrame = pView->GetParent();
			if(!pFrame)
				return FALSE;
			pFrame->SendMessage(WM_CLOSE, 0, 0);
			return TRUE;
		}

	case OC_OPENFILE:	// 打开文档
		{
			LPCTSTR lpszPathName = NULL;
			if(wParam != NULL)
				lpszPathName = (LPCTSTR)wParam;
			COwmSciteView* pView = GetSciteEditView(lpszPathName);
			if(pView)
			{
				// 已经存在,就激活文件,并返回指针
				pView->GetParentFrame()->ActivateFrame();
				return (int)(pView->GetDocument());
			}
			return (int)(theApp.m_ol.pDocTemplate->OpenDocumentFile(lpszPathName));
		}

	case OC_OPENFILEFIRST:	// 在第一个位置打开文档
		{
			return FALSE;
		}

	case OC_SETREADONLY:	// 设置文档的只读属性
		{
			COwmSciteView* pView = GetSciteEditView((LPCTSTR)wParam);
			if(pView == NULL)
				return FALSE;
			pView->m_SciEdit.execute(SCI_SETREADONLY, lParam);
			return TRUE;
		}

	case OC_GETOPENFILE:	// 获取打开的文件名
		{
			if((int)wParam == -1)	// 获取总文件数
			{
				if(!theApp.m_ol.pDocTemplate)
					return 0;
				int nCount = 0;
				POSITION posdoc = theApp.m_ol.pDocTemplate->GetFirstDocPosition();
				while(posdoc)
				{
					theApp.m_ol.pDocTemplate->GetNextDoc(posdoc);
					nCount++;
				}
				return nCount;
			}else
			if((int)wParam == -2)	// 获取当前打开的文件名
			{
				if(!theApp.m_ol.pDocTemplate)
					return 0;
				// 获取当前激活窗口Frame
				CChildFrame* pFrame = (CChildFrame*)((CMDIFrameWnd*)AfxGetMainWnd())->MDIGetActive();
				if(pFrame)
				{
					CView* pActiveView = pFrame->GetActiveView();

					POSITION posdoc = theApp.m_ol.pDocTemplate->GetFirstDocPosition();
					while(posdoc)
					{
						CDocument* pDoc = theApp.m_ol.pDocTemplate->GetNextDoc(posdoc);
						POSITION posview = pDoc->GetFirstViewPosition();
						if(posview)
						{
							CView* pView = pDoc->GetNextView(posview);
							if(pView->GetParentFrame()->GetActiveView() == pActiveView)
							{
								CString strPathName = pDoc->GetPathName();
								return (int)(LPCTSTR)strPathName;
							}
						}
					}
				}
				return (int)(LPCTSTR)"";
			}else
			if((int)wParam >= 0)	// 指定序号的文件
			{
				if(!theApp.m_ol.pDocTemplate)
					return 0;
				int nCount = 0;
				POSITION posdoc = theApp.m_ol.pDocTemplate->GetFirstDocPosition();
				while(posdoc)
				{
					CDocument* pDoc = theApp.m_ol.pDocTemplate->GetNextDoc(posdoc);
					if((int)wParam == nCount)
					{
						POSITION posview = pDoc->GetFirstViewPosition();
						if(posview)
						{
							CString strPathName = pDoc->GetPathName();
							return (int)(LPCTSTR)strPathName;
						}else
						{
							return (int)(LPCTSTR)"";
						}
					}
					nCount++;
				}
				return (int)(LPCTSTR)"";
			}

			return FALSE;
		}

	case OC_SETEXECUTION:	// 设置错误标记
		{
			COwmSciteView* pView = GetSciteEditView((LPCTSTR)wParam);
			if(pView == NULL)
				return FALSE;
			return FALSE;
		}

	case OC_SETCURENTLINE:	// 设置当前行
		{
			COwmSciteView* pView = GetSciteEditView((LPCTSTR)wParam);
			if(pView == NULL)
				return FALSE;
			int nLineNo = lParam-1;
			pView->m_SciEdit.gotoLine(nLineNo);
			pView->linemarkAdd(MARK_CURRENTLINE, nLineNo, true);
			return TRUE;
		}

	case OC_SETCURENTEXECLINE:	// 设置当前执行行
		{
			COwmSciteView* pView = GetSciteEditView((LPCTSTR)wParam);
			if(pView == NULL)
				return FALSE;
			int nLineNo = lParam-1;
			pView->m_SciEdit.gotoLine(nLineNo);
			pView->linemarkAdd(MARK_RUNCURRENT, nLineNo, true);
			return TRUE;
		}

	case OC_SEARCHLINE:	// 查找指定内容的行并设置为当前行
		{
			COwmSciteView* pView = GetSciteEditView((LPCTSTR)wParam);
			if(pView == NULL)
				return FALSE;
			pView->SearchLine((LPCTSTR)lParam, MARK_CURRENTLINE);
			return TRUE;
		}

	case OC_GETBREAKLIST:	// 获取断点列表
		{
			COwmSciteView* pView = GetSciteEditView((LPCTSTR)wParam);
			if(pView == NULL)
				return FALSE;
			pView->linemarkGetAll(MARK_BREAKPOINT, (CUIntArray&)lParam);
			return TRUE;
		}

	case OC_SETSLAVEDEBUG:	// 设置从调试模式(调试模式,但非主调试程序)
		{
			COwmSciteView* pView = GetSciteEditView((LPCTSTR)wParam);
			if(pView == NULL)
				return FALSE;
			pView->SetSlaveDebugMode(lParam);
			return TRUE;
		}

	case OC_GET_LINECOUNT:	// 获取总行数
		{
			COwmSciteView* pView = GetSciteEditView((LPCTSTR)wParam);
			if(pView == NULL)
				return FALSE;
			return pView->m_SciEdit.execute(SCI_GETLINECOUNT);
		}

	case OC_GET_LINE:	// 获取行
		{
			COwmSciteView* pView = GetSciteEditView((LPCTSTR)wParam);
			if(pView == NULL)
				return FALSE;
			int nLineNo = lParam-1;
			static CString g_strLine_GetLine;			
			int nLineLength = pView->m_SciEdit.getLineLength(nLineNo);
			TCHAR* pszLine = new TCHAR[nLineLength+3];	// 长度+3,因为行结束符有可能占用两个位置
			memset(pszLine, 0, nLineLength+3);
			pView->m_SciEdit.getLine(nLineNo, pszLine, nLineLength);
			g_strLine_GetLine = pszLine;
			delete [] pszLine;

			return (int)(LPCTSTR)g_strLine_GetLine;
		}

	case OC_SET_LINE:	// 设置行
		{
			COwmSciteView* pView = GetSciteEditView((LPCTSTR)wParam);
			if(pView == NULL)
				return FALSE;
			CString strLine = (LPCTSTR)lParam;
			int nPos = strLine.Find(">");
			if(nPos == -1)
			{
				return FALSE;
			}
			CString strLineNo = strLine.Left(nPos);
			strLine.Delete(0, nPos+1);
			if(strLineNo.Find("<") == 0)
			{
				strLineNo.Delete(0, 1);
			}
			int nLine = atoi(strLineNo);
			if(nLine > pView->m_SciEdit.execute(SCI_GETLINECOUNT))
			{
				return FALSE;
			}
			//pView->SetLine(nLine-1, strLine);
			return TRUE;
		}

	case OC_GET_SELECTLINE:	// 获取当前选择的行范围
		{/*
			COwmSciteView* pView = GetSciteEditView((LPCTSTR)wParam);
			if(pView == NULL)
				return FALSE;
			int nStartLine, nEndLine;

			CPoint ptStart;
			CPoint ptEnd;
			pView->public_GetSelection(ptStart, ptEnd);
			nStartLine = ptStart.y+1;
			nEndLine = ptEnd.y+1;
			if((ptEnd.x == 0) && (ptEnd.y > ptStart.y))
			{
				nEndLine--;
			}
			int nCount = nEndLine-nStartLine+1;

			int* lpEndLine = (int*)lParam;
			*lpEndLine = nStartLine;
			return nCount;*/
		}

	case OC_SET_BREAKLIST:	// 设置断点列表
		{
			COwmSciteView* pView = GetSciteEditView((LPCTSTR)wParam);
			if(pView == NULL)
				return FALSE;
			pView->linemarkClearAll(MARK_BREAKPOINT);
			CUIntArray* puIBreak = (CUIntArray*)lParam;
			if(puIBreak)
			{
				for(int i=0; i<puIBreak->GetSize(); i++)
				{
					pView->linemarkAdd(MARK_BREAKPOINT, puIBreak->GetAt(i));
				}
			}
			return TRUE;
		}

	case OC_CLEAR_BREAK:	// 清空断点
		{
			COwmSciteView* pView = GetSciteEditView((LPCTSTR)wParam);
			if(pView == NULL)
				return FALSE;
			pView->linemarkClearAll(MARK_BREAKPOINT);
			return TRUE;
		}

	case OC_EDIT_FIND:	// 查找
		{
			COwmSciteView* pView = GetActiveSciteEditView();
			if(pView)
			{
				SearchOption* pOption = NULL;
				theApp.m_pIPlatUI->SendOwmCmd("org.owm.grep", OC_GREP_GETOPTION, 0, 0, &pOption);
				FindOption opt;
				if(pOption)
				{
					opt._isMatchCase = pOption->m_bMatchCase;
					opt._isWholeWord = pOption->m_bMatchWholeWords;
					opt._searchType = pOption->m_bRegularExpressions ? FindRegex : FindNormal;
					opt._whichDirection = DIR_DOWN;
					opt._isIncremental = false;
					opt._isWrapAround = true;
					opt._isInSelection = (pView->m_SciEdit.execute(SCI_GETSELECTIONSTART) != pView->m_SciEdit.execute(SCI_GETSELECTIONEND));
				}

				generic_string str2Search = pView->getTextFromGrep(OC_GREP_GETTEXTFIND);
				int stringSizeFind = str2Search.length();
				TCHAR *pTextFind = new TCHAR[stringSizeFind + 1];
				lstrcpy(pTextFind, str2Search.c_str());

				int nbTotal = 0;
				const bool isEntireDoc = true;
				CString strPathName = pView->GetDocument()->GetPathName();
				nbTotal += pView->processAll(ProcessFindAll, pTextFind, NULL, isEntireDoc, strPathName, pOption ? &opt : NULL);
				delete [] pTextFind;
			}
			return TRUE;
		}

	case OC_EDIT_REPLACE:	// 替换
		{
			COwmSciteView* pView = GetActiveSciteEditView();
			if(pView)
			{
				SearchOption* pOption = NULL;
				theApp.m_pIPlatUI->SendOwmCmd("org.owm.grep", OC_GREP_GETOPTION, 0, 0, &pOption);
				FindOption opt;
				if(pOption)
				{
					opt._isMatchCase = pOption->m_bMatchCase;
					opt._isWholeWord = pOption->m_bMatchWholeWords;
					opt._searchType = pOption->m_bRegularExpressions ? FindRegex : FindNormal;
					opt._whichDirection = DIR_DOWN;
					opt._isIncremental = false;
					opt._isWrapAround = true;
					opt._isInSelection = (pView->m_SciEdit.execute(SCI_GETSELECTIONSTART) != pView->m_SciEdit.execute(SCI_GETSELECTIONEND));
				}

				generic_string str2Search = pView->getTextFromGrep(OC_GREP_GETTEXTFIND);
				int stringSizeFind = str2Search.length();
				TCHAR *pTextFind = new TCHAR[stringSizeFind + 1];
				lstrcpy(pTextFind, str2Search.c_str());

				generic_string str2Replace = pView->getTextFromGrep(OC_GREP_GETTEXTREPLACE);
				int stringSizeReplace = str2Replace.length();
				TCHAR *pTextReplace = new TCHAR[stringSizeReplace + 1];
				lstrcpy(pTextReplace, str2Replace.c_str());

				int nbTotal = 0;
				const bool isEntireDoc = true;
				CString strPathName = pView->GetDocument()->GetPathName();
				nbTotal += pView->processAll(ProcessReplaceAll, pTextFind, pTextReplace, isEntireDoc, strPathName, pOption ? &opt : NULL);
				delete [] pTextFind;
				delete [] pTextReplace;
			}
			return TRUE;
		}

	case OC_WORD_HELP:	// 单词帮助
		{
			COwmSciteView* pView = GetActiveSciteEditView();
			if(pView)
			{
				pView->SearchWordHelp((LPCTSTR)wParam);
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
// 注册OWM扩展菜单
/////////////////////////////////////////////////////////////////////////////
int RegisterClientMenu(CMenu* pMenu, COWMMenuArray& aOWMMenus, int nOWMIdStart, CDocument* pDoc)
{
/*
	// 初始化OWM菜单,并添加到主菜单
	get_dll_resource();
	CMenu* pmenu = new CMenu();
	pmenu->LoadMenu(IDR_OWM);
	CMenu* psubmenu = (CMenu*)(pmenu->GetSubMenu(0));
	reset_dll_resource();

	// 检查菜单是否已经存在,如果存在则删除
	int count = pMenu->GetMenuItemCount();
	OWM_CHECKMENUISEXIST(count-1, _T("OwmScite"))

	// 在主菜单最后添加扩展菜单
	pMenu->InsertMenu(count-1, MF_POPUP|MF_STRING|MF_ENABLED, 
				(UINT)psubmenu->GetSafeHmenu(), _T("OwmScite"));

	// 在此添加扩展菜单
	OWM_REGISTERMENU(ID_OWM_ABOUT, AboutOWM, NULL, USER_NONE)

	// 在DOC中记载扩展菜单指针,文档关闭时清除
	if(pDoc)
	{
		((COwmSciteDoc*)pDoc)->m_pMenu = pmenu;
	}
*/
	return aOWMMenus.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
// 卸载OWM扩展菜单命令
/////////////////////////////////////////////////////////////////////////////
int UnregisterClientMenu(COWMMenuArray& aOWMMenus)
{
	// 在此注消扩展菜单命令
	//OWM_UNREGISTERMENU(AboutOWM)

	return aOWMMenus.GetSize();
}


/////////////////////////////////////////////////////////////////////////////
// 显示/隐藏OWM窗口
/////////////////////////////////////////////////////////////////////////////
int OpenOwmView()
{
	if(GetFirstOwmSciteView())
		theApp.m_ol.CommandPlat(OC_CLOSEVIEW, theApp.m_ol.nOWMID, 0);
	else
		theApp.m_ol.CommandPlat(OC_OPENVIEW, theApp.m_ol.nOWMID, 0);
	return 0;
}

void UpdateOpenOwmView(CCmdUI* pCmdUI)
{
	int nCheck = GetFirstOwmSciteView() ? 1 : 0;
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
	DLL_LOADSTRING(strTpsEdit, IDS_PROP_STR_GENERAL);	// 编辑器
	DLL_LOADSTRING(strTpsAboutEdit, IDS_PROP_STR_ABOUT_GENERAL);	// 编辑器常用设置...
	DLL_LOADSTRING(strTpsNewDoc, IDS_PROP_STR_NEWDOC);	// 新建文档
	DLL_LOADSTRING(strTpsAboutNewDoc, IDS_PROP_STR_ABOUT_NEWDOC);	// 新建文档设置...
	DLL_LOADSTRING(strTpsStyle, IDS_PROP_STR_STYLE);	// 语言格式
	DLL_LOADSTRING(strTpsAboutStyle, IDS_PROP_STR_ABOUT_STYLE);	// 语言格式设置...

	OWM_REGISTEROPTION("", strTpsEdit, strTpsAboutEdit, IDD_TPS_GENERAL, CTpsGeneral, 0)
	OWM_REGISTEROPTION(strTpsEdit, strTpsNewDoc, strTpsAboutNewDoc, IDD_TPS_NEWDOC, CTpsNewDocument, 0)
	OWM_REGISTEROPTION(strTpsEdit, strTpsStyle, strTpsAboutStyle, IDD_TPS_STYLE, CTpsStyle, 0)

	return aOWMOption.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
// 初始化OWM
/////////////////////////////////////////////////////////////////////////////
int InitOWM()
{
	// 注:在下面添加你的代码
/*	CString strSciLexerFile = theApp.GetModulePath();
	strSciLexerFile += "\\SciLexer.DLL";
	theApp.m_hScintillaDll = ::LoadLibrary(strSciLexerFile);
*/
	NppParameters *pNppParameters = NppParameters::getInstance();
	pNppParameters->load();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 卸载OWM
/////////////////////////////////////////////////////////////////////////////
int DoneOWM()
{
	// 注:在下面添加你的代码
	// 保存配置参数
	NppParameters *pNppParam = NppParameters::getInstance();
	(NppParameters::getInstance())->writeScintillaParams(pNppParam->getSVP(SCIV_PRIMARY), SCIV_PRIMARY);
	(NppParameters::getInstance())->writeScintillaParams(pNppParam->getSVP(SCIV_SECOND), SCIV_SECOND);
	(NppParameters::getInstance())->writeGUIParams();
	// 释放配置参数分配的内存
	(NppParameters::getInstance())->destroyInstance();

	// 释放UTF8-多字节转换器静态对象中分配的内存
	WcharMbcsConvertor::destroyInstance();

	if(theApp.m_hScintillaDll)
	{
		//::FreeLibrary(theApp.m_hScintillaDll);
	}

	/*
	HINSTANCE hScintillaDll = gGetScintillaLibHandle();
	if(hScintillaDll)
	{
		::FreeLibrary(hScintillaDll);
	}*/

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// OWM VCI Interface

#include "vcicomm.h"
#include "vciowm.h"

#define INF_IVciOwm \
{\
	"IVciOwm",\
	"1.0.0.0",\
	THIS_REVISION_TYPE,\
	"OwmScite",\
	"OwmScite OWM",\
	"blueant",\
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
	CVciMessage* pMsg = (CVciMessage*)pIn;
	if(pMsg->GetCmdCode() == VMC_SENDOWMCMD)
	{
		CString* pStrFilePath = (CString*)(pMsg->GetHandle());
		COwmSciteView* pSciteView = GetSciteEditView(*pStrFilePath, FALSE);
		if(pSciteView)
		{
			int nOwmCmdCode, wParam, lParam;
			if(pMsg->GetOwmCmd(nOwmCmdCode, wParam, lParam))
			{
				return pSciteView->m_SciEdit.execute(nOwmCmdCode, wParam, lParam);
			}
		}
	}

	return 0;
}

// 查询命令是否支持
extern "C" __declspec(dllexport)
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
	if(nID == VIEW_ID_BOOKMARK)	// 书签窗口
	{
		if(theApp.m_pIPlatUI)
		{
			theApp.m_pIPlatUI->AddOutput("##GRID**10");
			COwmSciteView* pView = GetActiveSciteEditView();
			if(pView)
			{
				pView->ViewBookmarksWindow();
			}
		}
	}else
	if(nID == VIEW_ID_BREAKPOINT)	// 断点窗口
	{
		if(theApp.m_pIPlatUI)
		{
			theApp.m_pIPlatUI->AddOutput("##GRID**11");
			COwmSciteView* pView = GetActiveSciteEditView();
			if(pView)
			{
				pView->ViewBreaksWindow();
			}
		}
	}/*else
	if(nID == VIEW_ID_DYNHELP)	// 动态帮助窗口
	{
		if(theApp.m_pIPlatUI)
		{
			//theApp.m_pIPlatUI->CreateDockingPane("org.owm.htmlview", "动态帮助", VIEW_ID_DYNHELP, xtpPaneDockBottom, PANE_ATTACH_VIEW, NULL, 0);
		}
	}*/
	
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
	DLL_LOADSTRING(strTpsPrintPage, IDS_PROP_STR_PRINTPAGE);	// 打印页面设置
	DLL_LOADSTRING(strTpsAboutPrintPage, IDS_PROP_STR_ABOUT_PRINTPAGE);	// 设置打印页面...
	OWM_REGISTEROPTION("", strTpsPrintPage, strTpsAboutPrintPage, IDD_TPS_PRINTPAGE, CTpsPrintPage, 0);

	return aOWMOption.GetSize();
}

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
	COwmSciteView* pView = NULL;
	POSITION pos = pDoc->GetFirstViewPosition();
	if(pos != NULL)
	{
		pView = (COwmSciteView*)(pDoc->GetNextView(pos));
	}
	if(pView == NULL)
	{
		return FALSE;
	}

	CUIntArray auBreaks;
	CUIntArray auBookmarks;

	for(int i=0; i<asSessionLine.GetSize(); i++)
	{
		CString strSessionLine = asSessionLine[i];
		if(strSessionLine.Find("[[=SCI_POS=]]") == 0)
		{
			// 恢复当前位置
			Position _pos;
			_pos._firstVisibleLine = atoi(GetValueFromSessionLine(strSessionLine, "firstVisibleLine"));
			_pos._xOffset = atoi(GetValueFromSessionLine(strSessionLine, "xOffset"));
			_pos._scrollWidth = atoi(GetValueFromSessionLine(strSessionLine, "scrollWidth"));
			_pos._startPos = atoi(GetValueFromSessionLine(strSessionLine, "startPos"));
			_pos._endPos = atoi(GetValueFromSessionLine(strSessionLine, "endPos"));
			_pos._selMode = atoi(GetValueFromSessionLine(strSessionLine, "selMode"));
			pView->_position = _pos;
			pView->restoreCurrentPos();
		}else
		if(strSessionLine.Find("[[=SCI_BREAK=]]") == 0)
		{
			int nLineNo = atoi(GetValueFromSessionLine(strSessionLine, "lineno"));
			CString strLineContent = GetValueFromSessionLine(strSessionLine, "line", TRUE);
			CString strLine;
			int nLineLength = pView->m_SciEdit.getLineLength(nLineNo-1);
			TCHAR* pszLine = new TCHAR[nLineLength+3];	// 长度+3,因为行结束符有可能占用两个位置
			memset(pszLine, 0, nLineLength+3);
			pView->m_SciEdit.getLine(nLineNo-1, pszLine, nLineLength);
			strLine = pszLine;
			delete [] pszLine;
			// 校验行号对应的行是否正确
			if(strLine.Find(strLineContent) != -1)
			{
				auBreaks.Add(nLineNo);
			}
		}else
		if(strSessionLine.Find("[[=SCI_BOOKMARK=]]") == 0)
		{
			int nLineNo = atoi(GetValueFromSessionLine(strSessionLine, "lineno"));
			CString strLineContent = GetValueFromSessionLine(strSessionLine, "line", TRUE);
			CString strLine;
			int nLineLength = pView->m_SciEdit.getLineLength(nLineNo-1);
			TCHAR* pszLine = new TCHAR[nLineLength+3];	// 长度+3,因为行结束符有可能占用两个位置
			memset(pszLine, 0, nLineLength+3);
			pView->m_SciEdit.getLine(nLineNo-1, pszLine, nLineLength);
			strLine = pszLine;
			delete [] pszLine;
			// 校验行号对应的行是否正确
			if(strLine.Find(strLineContent) != -1)
			{
				auBookmarks.Add(nLineNo);
			}
		}
	}

	// 加载断点列表
	for(int i=0; i<auBreaks.GetSize(); i++)
	{
		pView->linemarkAdd(MARK_BREAKPOINT, auBreaks[i]-1);
	}

	// 加载书签列表
	for(int i=0; i<auBookmarks.GetSize(); i++)
	{
		pView->linemarkAdd(MARK_BOOKMARK, auBookmarks[i]-1);
	}

	return TRUE;
}

// 保存文档Session
int __stdcall CVciOwm::XVciOwm::SaveDocumentSession(CDocument* pDoc, CStringArray& asSessionLine)
{
	COwmSciteView* pView = NULL;
	POSITION pos = pDoc->GetFirstViewPosition();
	if(pos != NULL)
	{
		pView = (COwmSciteView*)(pDoc->GetNextView(pos));
	}
	if(pView == NULL)
	{
		return FALSE;
	}

	// 保存当前位置
	pView->saveCurrentPos();
	Position _pos = pView->_position;
	CString strTmp;
	strTmp.Format("[[=SCI_POS=]]firstVisibleLine=%d,xOffset=%d,scrollWidth=%d,startPos=%d,endPos=%d,selMode=%d,",
			_pos._firstVisibleLine, _pos._xOffset, _pos._scrollWidth,
			_pos._startPos, _pos._endPos, _pos._selMode);
	asSessionLine.Add(strTmp);

	// 保存断点列表
	CUIntArray auBreaks;
	pView->linemarkGetAll(MARK_BREAKPOINT, auBreaks);
	for(int i=0; i<auBreaks.GetSize(); i++)
	{
		CString strLine;
		int nLineLength = pView->m_SciEdit.getLineLength(auBreaks[i]-1);
		TCHAR* pszLine = new TCHAR[nLineLength+3];	// 长度+3,因为行结束符有可能占用两个位置
		memset(pszLine, 0, nLineLength+3);
		pView->m_SciEdit.getLine(auBreaks[i]-1, pszLine, nLineLength);
		strLine = pszLine;
		delete [] pszLine;
		strLine.TrimRight();

		strTmp.Format("[[=SCI_BREAK=]]lineno=%d,line=%s", auBreaks[i], strLine);
		asSessionLine.Add(strTmp);
	}

	// 保存书签列表
	CUIntArray auBookmarks;
	pView->linemarkGetAll(MARK_BOOKMARK, auBookmarks);
	for(int i=0; i<auBookmarks.GetSize(); i++)
	{
		CString strLine;
		int nLineLength = pView->m_SciEdit.getLineLength(auBookmarks[i]-1);
		TCHAR* pszLine = new TCHAR[nLineLength+3];	// 长度+3,因为行结束符有可能占用两个位置
		memset(pszLine, 0, nLineLength+3);
		pView->m_SciEdit.getLine(auBookmarks[i]-1, pszLine, nLineLength);
		strLine = pszLine;
		delete [] pszLine;
		strLine.TrimRight();

		strTmp.Format("[[=SCI_BOOKMARK=]]lineno=%d,line=%s", auBookmarks[i], strLine);
		asSessionLine.Add(strTmp);
	}

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
	DLL_LOADSTRING(strExportHtmlFile, IDS_EXPORT_HTML_FILE);
	DLL_LOADSTRING(strExportRtfFile, IDS_EXPORT_RTF_FILE);
	asExportType.Add(strExportHtmlFile);
	asExportType.Add(strExportRtfFile);
	return asExportType.GetSize();
}

// 导入文档
int __stdcall CVciOwm::XVciOwm::ImportDocument(CDocument* pDoc, LPCTSTR lpszImportSource, int nImportType)
{
	return TRUE;
}

// 导出文档
int __stdcall CVciOwm::XVciOwm::ExportDocument(CDocument* pDoc, LPCTSTR lpszExportDest, int nExportType)
{
	COwmSciteView* pView = NULL;
	POSITION pos = pDoc->GetFirstViewPosition();
	if(pos != NULL)
	{
		pView = (COwmSciteView*)(pDoc->GetNextView(pos));
	}
	if(pView == NULL)
	{
		return FALSE;
	}

	if((nExportType < 0) || (nExportType > 1))
	{
		return FALSE;
	}

	switch(nExportType)
	{
	case 0:pView->exportHtmlFile(lpszExportDest);break;
	case 1:pView->exportRtfFile(lpszExportDest);break;
	}

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
	TRACE("CreateObject(\"%s\")-OWM-OwmScite\n", lpcsInterfaceName);

	CVisualComponent *pObject;
	if(strcmp(lpcsInterfaceName, IID_IVciOwm) == 0)
	{
		pObject = new CVciOwm;
		OWM_LINKAGE ol;
		InstallOWM(ol);
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
