//-----------------------------------------------------------------------------------------------------------
// Script.NET版权声明
// 
// 此软件的个人版本为免费、开源软件，可以任意下载和使用，并可以用于商业目的。
// 
// 此软件的部分代码使用了一些开源软件，请参考相关开源代码的版权信息。
// 此软件的代码需要依赖于一些商业软件，此部分不包含在此软件中，如需获取相关商业软件代码，请自行从官方渠道获取。
// 此软件的手册中部分内容为网上收集整理所得，此部分版权为原作者所有。
//-----------------------------------------------------------------------------------------------------------

// BlueUI.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
//#include "vld.h"
#include "BlueUI.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "BlueUIDoc.h"
#include "BlueUIView.h"
#include "mdump.h"
#include "cgfiltyp.h"
#include "Splash.h"
#include "PictureEx.h"
#include "FlashWin.h"
#include "TreePropertySheet.h"
#include "TpsFont.h"
#include "TpsAssociate.h"
#include "TpsLanguage.h"
#include "TpsUIStyle.h"
#include "TpsWorkspace.h"
#include "TpsVci.h"
#include "TpsInterpDirect.h"

#include <locale.h>
#include "winnls.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlueUIApp

BEGIN_MESSAGE_MAP(CBlueUIApp, CWinApp)
	//{{AFX_MSG_MAP(CBlueUIApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE1, OnUpdateFileMruFile1)
	ON_COMMAND(ID_OPTION, OnOption)
	ON_UPDATE_COMMAND_UI(ID_OPTION, OnUpdateOption)
	ON_COMMAND(ID_VCI_MANAGER, OnVciManager)
	ON_UPDATE_COMMAND_UI(ID_VCI_MANAGER, OnUpdateVciManager)
	ON_COMMAND(ID_PROJECT_NEWFILE, OnProjectNewfile)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_NEWFILE, OnUpdateProjectNewfile)
	ON_COMMAND(ID_HELP_TOPICS, OnHelpTopics)
	ON_UPDATE_COMMAND_UI(ID_HELP_TOPICS, OnUpdateHelpTopics)
	ON_COMMAND(ID_HELP_REGISTERATE, OnHelpRegisterate)
	ON_UPDATE_COMMAND_UI(ID_HELP_REGISTERATE, OnUpdateHelpRegisterate)
	ON_COMMAND(ID_HELP_UPGRADE, OnHelpUpgrade)
	ON_UPDATE_COMMAND_UI(ID_HELP_UPGRADE, OnUpdateHelpUpgrade)
	ON_COMMAND(ID_HELP_HOMEPAGE, OnHelpHomepage)
	ON_UPDATE_COMMAND_UI(ID_HELP_HOMEPAGE, OnUpdateHelpHomepage)
	ON_COMMAND(ID_HELP_BUGREPORT, OnHelpBugreport)
	ON_UPDATE_COMMAND_UI(ID_HELP_BUGREPORT, OnUpdateHelpBugreport)
	ON_COMMAND(ID_OPEN_WITH, OnOpenWith)
	ON_UPDATE_COMMAND_UI(ID_OPEN_WITH, OnUpdateOpenWith)
	ON_COMMAND(ID_TOOLS_EXTEND, OnToolsExtend)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_EXTEND, OnUpdateToolsExtend)
	ON_COMMAND(ID_HELP_DOWNLOAD, OnHelpDownload)
	ON_UPDATE_COMMAND_UI(ID_HELP_DOWNLOAD, OnUpdateHelpDownload)
	ON_COMMAND(ID_PROJECT_NEW, OnProjectNew)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_NEW, OnUpdateProjectNew)
	ON_COMMAND(ID_PROJECT_OPEN, OnProjectOpen)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_OPEN, OnUpdateProjectOpen)
	ON_COMMAND(ID_PROJECT_SAVE, OnProjectSave)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_SAVE, OnUpdateProjectSave)
	ON_COMMAND(ID_PROJECT_CLOSE, OnProjectClose)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_CLOSE, OnUpdateProjectClose)
	ON_COMMAND(ID_PROJECT_BUILD, OnProjectBuild)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_BUILD, OnUpdateProjectBuild)
	ON_COMMAND(ID_PROJECT_REBUILD, OnProjectRebuild)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_REBUILD, OnUpdateProjectRebuild)
	ON_COMMAND(ID_PROJECT_CLEAN, OnProjectClean)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_CLEAN, OnUpdateProjectClean)
	ON_COMMAND(ID_PROJECT_INFO, OnProjectInfo)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_INFO, OnUpdateProjectInfo)
	ON_COMMAND(ID_PROJECT_PROPERTY, OnProjectProperty)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_PROPERTY, OnUpdateProjectProperty)
	ON_COMMAND(ID_PROJECT_RUN, OnProjectRun)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_RUN, OnUpdateProjectRun)
	ON_COMMAND(ID_FILE_LOADSESSION, OnFileLoadsession)
	ON_UPDATE_COMMAND_UI(ID_FILE_LOADSESSION, OnUpdateFileLoadsession)
	ON_COMMAND(ID_FILE_SAVESESSION, OnFileSavesession)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVESESSION, OnUpdateFileSavesession)
	ON_COMMAND(ID_FILE_PRINT_PAGESETUP, OnFilePrintPagesetup)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PAGESETUP, OnUpdateFilePrintPagesetup)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlueUIApp construction

CBlueUIApp::CBlueUIApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pIProject = NULL;
	m_pIMessageQueue = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBlueUIApp object

CBlueUIApp theApp;

/////////////////////////////////////////////////////////////////////////////
// 进程间通信,传递命令行字符串
/////////////////////////////////////////////////////////////////////////////
BOOL CBlueUIApp::InterprocessCommunication()
{
	// number of characters in memory-mapped file
	const DWORD dwMemoryFileSize = 1024;

	// memory-mapped file name
	const LPCTSTR sMemoryFileName = _T("DF034858-1608-4147-9025-4A0CD86F6C9F");

	// 在Win7下ParseCommandLine不能正确解析命令行参数,因此改为自己解析参数
	// Parse command line for standard shell commands, DDE, file open
	//CCommandLineInfo cmdInfo;
	//ParseCommandLine(cmdInfo);
	//if(cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen)

	int nShellCommand = CCommandLineInfo::FileNew;
	if(__argc > 1)
	{
		nShellCommand = CCommandLineInfo::FileOpen;
	}
	CString strFileNames = _T("");
	for(int i=1; i<__argc; i++)
	{
		if(i != 1)
		{
			strFileNames += _T(" ");
		}
		strFileNames += _T("\"");
		strFileNames += __targv[i];
		strFileNames += _T("\"");
	}

	{
		//AfxMessageBox(cmdInfo.m_strFileName);

		HANDLE hFileMapping = NULL;
		LPVOID pViewOfFile = NULL;

		// Create file mapping which can contain dwMemoryFileSize characters
		hFileMapping = CreateFileMapping(
			INVALID_HANDLE_VALUE,           // system paging file
			NULL,                           // security attributes
			PAGE_READWRITE,                 // protection
			0,                              // high-order DWORD of size
			dwMemoryFileSize*sizeof(TCHAR), // low-order DWORD of size
			sMemoryFileName);               // name

		DWORD dwError = GetLastError();     // if ERROR_ALREADY_EXISTS 
				// this instance is not first (other instance created file mapping)

		if(! hFileMapping)
		{
			TRACE(_T("Creating of file mapping failed.\n"));
			return FALSE;
		}

        pViewOfFile = MapViewOfFile(
            hFileMapping,               // handle to file-mapping object
            FILE_MAP_ALL_ACCESS,        // desired access
            0,
            0,
            0);                         // map all file

        if(!pViewOfFile)
        {
            TRACE(_T("MapViewOfFile failed.\n"));
			CloseHandle(hFileMapping);
			return FALSE;
        }

		lstrcpy( (LPTSTR) pViewOfFile, strFileNames);//cmdInfo.m_strFileName);

		DWORD result;
		LRESULT ok = SendMessageTimeout(HWND_BROADCAST,
									   WM_CHECK_ITS_ME,
									   //cmdInfo.m_nShellCommand,
									   nShellCommand,		// wParam:命令行操作类型
									   //(LPARAM)(LPCTSTR)(cmdInfo.m_strFileName),
									   __argc-1,				// 文件个数
									   SMTO_BLOCK |
									   SMTO_ABORTIFHUNG,
									   200,
									   &result);

		UnmapViewOfFile(pViewOfFile);
		CloseHandle(hFileMapping);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CBlueUIApp initialization

BOOL CBlueUIApp::InitInstance()
{
	CString strConfigFile = GetPlatRootPath();
	strConfigFile += "conf\\main_cn.xml";
	m_xmlPlat.Open(strConfigFile);

	CString strLicensePluginPath = "";

	CString strAppMutex = m_xmlPlat.GetNodeText("application\\appMutex");
	::CreateMutex(NULL, FALSE, strAppMutex);//_T("##SCRIPT.NETV4##"));
	if(ERROR_ALREADY_EXISTS == GetLastError() || ERROR_ACCESS_DENIED == GetLastError())
	{
		InterprocessCommunication();

		return FALSE; // Here we quit this application
	}

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// 初始化COM库
	::CoInitialize(NULL);

	AfxInitRichEdit();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	CString strRegCompany = m_xmlPlat.GetNodeText("application\\RegCompany");
	SetRegistryKey(strRegCompany);//_T("Blueant"));
	CString strRegProfile = m_xmlPlat.GetNodeText("application\\RegProfile");
	m_pszProfileName = _tcsdup(strRegProfile);//"BlueUI");

	m_xmlPlat.Close();

	LoadStdProfileSettings(8);  // Load standard INI file options (including MRU)

	// 获取当前语言并设置语言
	int nCurrentLanguage = LANGUAGE_PAGE_CHINESE;
	if(LANGUAGE_PAGE_CHINESE == nCurrentLanguage)
	{
		SetLocale(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED),
				"chinese-simplified.2052", FALSE);
	}else
	{
		SetLocale(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
				"English.1252", FALSE);
	}

	strConfigFile = GetPlatRootPath();
	if(LANGUAGE_PAGE_CHINESE == theApp.m_curLanguage)
	{
		if(strLicensePluginPath == "")
		{
			strConfigFile += "conf\\main_cn.xml";
		}else
		{
			strConfigFile = strLicensePluginPath + "\\main_cn.xml";
		}
	}else
	{
		if(strLicensePluginPath == "")
		{
			strConfigFile += "conf\\main.xml";
		}else
		{
			strConfigFile = strLicensePluginPath + "\\main.xml";
		}
	}
	m_xmlPlat.Open(strConfigFile);

	// 初始化MiniDump
	CString strPlatVersion = "";
	IPlatUI* pIPlatUI = GetIPlatUI();
	if(pIPlatUI)
	{
		strPlatVersion = pIPlatUI->GetPlatVersion();
	}
	theCrashDumper.Enable(m_xmlPlat.GetNodeText("application\\appName") + "_" + strPlatVersion, true);

	// 获取进程标识
	m_strProcessId = m_xmlPlat.GetNodeText("application\\Process");

	// 崩溃测试,人为制造一个被0除错误
	//int a = 10;
	//float b = a / 0;

	// Splash
	CString strSplashEnable = m_xmlPlat.GetNodeAttribute("application\\Splash", "enable");
	if(strSplashEnable == "true")
	{
		CSplashWnd::EnableSplashScreen();
		CSplashWnd::ShowSplashScreen();
	}

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_BLUEUITYPE,
		RUNTIME_CLASS(CBlueUIDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CBlueUIView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// 设置窗口标题
	CString strAppName = m_xmlPlat.GetNodeText("application\\appName");
	pMainFrame->SetTitle(strAppName);

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// 获取缺省的编辑器模块名
	m_strDefaultEditor = m_xmlPlat.GetNodeText("Editor\\Default");

#ifndef _DEBUG
	/////////////////////////////////////////////////////////////
	// Regist Shell File Type
	DOMNode* pNode = m_xmlPlat.GetNode("shell\\File");
	while (pNode != NULL)
	{
		CString strFileExtension = m_xmlPlat.GetNodeAttribute(pNode, "Extension");
		CString strDocClassName = m_xmlPlat.GetNodeAttribute(pNode, "Class");
		if(strFileExtension != "")
		{

			CGCFileTypeAccess TheFTA;
			TheFTA.SetApplicationName(strAppName);

			TCHAR	szProgPath[MAX_PATH * 2];
			::GetModuleFileName(NULL, szProgPath, sizeof(szProgPath)/sizeof(TCHAR));

			CString csTempText;

			// 注册Script.NET工程文件后缀
			TheFTA.SetExtension(strFileExtension);//"snp");

			// just pass file path in quotes on command line
			csTempText  = szProgPath;
			csTempText += " \"%1\"";
			TheFTA.SetShellOpenCommand(csTempText);
			TheFTA.SetDocumentShellOpenCommand(csTempText);

			TheFTA.SetDocumentClassName(strDocClassName);//"snp.Document");

			// use first icon in program
			csTempText  = szProgPath;
			csTempText += ",0";
			TheFTA.SetDocumentDefaultIcon(csTempText);
			
			TheFTA.RegSetAllInfo();
		}

		pNode = m_xmlPlat.GetNextNode(pNode, "File");
	}
	/////////////////////////////////////////////////////////////
#endif

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	CString strShellFileName = "";
	if(cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen)
		strShellFileName = cmdInfo.m_strFileName;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindowEx(m_nCmdShow);
	pMainFrame->UpdateWindow();

	// 设置安装路径
	WriteProfileString(REG_PATH_SUBKEY, REG_PATH_INSTALL, GetPlatRootPath());

	// 加载插件
	LoadPlugIns();

	// 创建消息队列组件实例
	m_pIMessageQueue = (IMessageQueue*)(CreateVciObject(VCIID_MESSAGEQUEUE));
	if(m_pIMessageQueue)
	{
		m_pIMessageQueue->SetAppProcessId(m_strProcessId);
	}

	// 是否自动加载起始页
	BOOL bAutoOpenStartPage = theApp.GetProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_AUTO_OPEN_STARTPAGE, TRUE);
	if(bAutoOpenStartPage)
	{
		CString strStartPage;
		GetIPlatUI()->GetVciParameter("", "StartPage", strStartPage, strStartPage);
		OpenHtmlWindow(strStartPage);
	}

	// 隐藏闪烁窗口
	int nTime = atoi(m_xmlPlat.GetNodeAttribute("application\\Splash", "time"));
	if(nTime == 0)
		CSplashWnd::HideSplashScreen();

	int nRegister = GetProfileInt(REG_VERSION_SUBKEY, REG_REGISTER, 0);
	if(nRegister == 0)
	{
		ExecMenuScript("Help\\Register");
	}else
	{
		// 加载保存的桌面文件
		BOOL bAutoReloadDocument = theApp.GetProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_AUTO_RELOAD_DOCUMENT, TRUE);
		if(bAutoReloadDocument)
		{
			CString strFileWorkspace = GetPlatRootPath();
			strFileWorkspace += "conf\\workspace.session";
			pMainFrame->LoadSessionFile(strFileWorkspace);
		}
	}

	// 加载命令行需要打开的文件
	if(strShellFileName != "")
	{
		ShellOpenFile(strShellFileName);
	}else
	if(__argc > 1)	// Win7下可能不能正确解析命令行参数,改为自己解析
	{
		CStringArray asFile;
		CString strCommand = __targv[1];
		if(strCommand.Find(_T("CMD_")) != 0)
		{
			strCommand = _T("");
			asFile.Add(__targv[1]);
		}
		for(int i=2; i<__argc; i++)
		{
			asFile.Add(__targv[i]);
		}
		if(strCommand != _T(""))
		{
			// 处理命令行操作
			ShellCommand(strCommand, asFile);
		}else
		{
			// 没有命令行操作命令表示要打开文件
			for(int i=0; i<asFile.GetSize(); i++)
			{
				ShellOpenFile(asFile[i]);
			}
		}
	}

	// 判断并加载启动时运行的定式任务
	CString strTimerXmlFile = GetPlatRootPath() + "\\conf\\starttimer.xml";
	if(GetFileAttributes(strTimerXmlFile) != 0xFFFFFFFF)
	{
		CXmlParser xmlTimer;
		xmlTimer.Open(strTimerXmlFile);

		DOMNode* pNode = xmlTimer.GetNode("Task");
		while (pNode != NULL)
		{
			CString strTaskName = xmlTimer.GetNodeAttribute(pNode, "Name");
			CString strInterpId = xmlTimer.GetNodeAttribute(pNode, "Interp");
			CString strInterpName = xmlTimer.GetNodeAttribute(pNode, "InterpName");
			CString strCmdType = xmlTimer.GetNodeAttribute(pNode, "CmdType");
			int nSeconds = atoi(xmlTimer.GetNodeAttribute(pNode, "Seconds"));
			int nCount = atoi(xmlTimer.GetNodeAttribute(pNode, "Count"));
			CString strStartTime = xmlTimer.GetNodeAttribute(pNode, "StartTime");
			CString strEndTime = xmlTimer.GetNodeAttribute(pNode, "strEndTime");
			CString strTrigger = xmlTimer.GetNodeAttribute(pNode, "Trigger");

			CString strCommand = xmlTimer.GetNodeText(pNode);
			strCommand.TrimLeft();
			strCommand.TrimRight();
			if(strCmdType == "file")
			{
				strCommand = "<file>" + strCommand;
			}

			CTimerTask* pTask = AddTimerTask(strTaskName, strInterpId, strInterpName, strCommand, nSeconds, FALSE);
			if(pTask)
			{
				pTask->SetExecCount(nCount);
				//pTask->SetStartTime();
				//pTask->SetEndTime();
				if(strTrigger != "")
				{
					pTask->SetTrigger(strTrigger);
				}
				pTask->StartTask();
			}

			pNode = xmlTimer.GetNextNode(pNode, "Task");
		}
	}

	// 加载工具信息(线程方式加载会导致某些工具创建窗口时候异常,暂时先不用线程方式)
	AutoLoadTools("Tools");
	//CreateLoadToolsThread();

	return TRUE;
}

int CBlueUIApp::ExitInstance() 
{
	// 删除所有定时任务
	POSITION pos = m_lsTimerTask.GetHeadPosition();
	while(pos != NULL)
	{
		CTimerTask* pItem = m_lsTimerTask.GetAt(pos);

		delete pItem;

		m_lsTimerTask.GetNext(pos);
	}

	::CoUninitialize();

	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// 设置语言
/////////////////////////////////////////////////////////////////////////////
BOOL CBlueUIApp::SetLocale(LCID Locale, const char* szLocale, BOOL bIsRTL, BOOL bIsTextFile)
{
	if (bIsTextFile)
	{
		CString strPath;
		VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, strPath.GetBufferSetLength(_MAX_PATH), _MAX_PATH));
		strPath.ReleaseBuffer();

		int nIndex  = strPath.ReverseFind(_T('\\'));
		if (nIndex > 0) strPath = strPath.Left(nIndex + 1); else strPath.Empty();

		// 资源文件路径例如:res_English.1252.xml
		strPath = strPath + _T("res_") + szLocale + _T(".xml");

		if (!XTPResourceManager()->SetResourceManager(new CXTPResourceManagerXML(strPath)))
			return FALSE;

	} else
	{
		if (!XTPResourceManager()->SetResourceManager(new CXTPResourceManager()))
			return FALSE;
	}

	::SetThreadLocale(MAKELCID(Locale, SORT_DEFAULT));
	::setlocale(LC_ALL, szLocale);

	m_curLanguage = Locale;
	SetRTLLayout(bIsRTL);

	// 重新加载缺省的菜单
	ReloadDefaultMenu();

	return TRUE;
}

void CBlueUIApp::ReloadDefaultMenu()
{
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, m_pMainWnd);
	if (pMainFrame)
	{
		pMainFrame->ResetCommandBars();
	}
}

void SetRTLLayout(CWnd* pWnd, BOOL bRTLLayout)
{
	pWnd->ModifyStyleEx(bRTLLayout? 0: WS_EX_LAYOUTRTL, !bRTLLayout? 0: WS_EX_LAYOUTRTL);

	// walk through HWNDs to avoid creating temporary CWnd objects
	// unless we need to call this function recursively
	for (CWnd* pChild = pWnd->GetWindow(GW_CHILD); pChild != NULL;
		pChild = pChild->GetWindow(GW_HWNDNEXT))
	{

		// send to child windows after parent
		SetRTLLayout(pChild, bRTLLayout);
	}
}


void CBlueUIApp::SetRTLLayout(BOOL bRTLLayout)
{
	if ((BYTE)::GetVersion() < 4)
		return;

	if (m_bIsRTL == bRTLLayout)
		return;

	m_bIsRTL = bRTLLayout;


	if (m_pMainWnd)
	{
		::SetRTLLayout(m_pMainWnd, bRTLLayout);

		// then update the state of all floating windows owned by the parent
		HWND hWnd = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
		while (hWnd != NULL)
		{
			if (m_pMainWnd->m_hWnd == ::GetParent(hWnd))
				::SetRTLLayout(CWnd::FromHandle(hWnd), bRTLLayout);

			hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
		}
	}

	XTPImageManager()->DrawReverted(bRTLLayout);
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CPictureEx	m_Picture;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_Picture);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strSplashPicture = theApp.GetPlatRootPath();
	strSplashPicture += "conf\\splash.bmp";
	if (m_Picture.Load(strSplashPicture))
		m_Picture.Draw();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// App command to run the dialog
void CBlueUIApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CBlueUIApp message handlers

BOOL CBlueUIApp::PreTranslateMessage(MSG* pMsg) 
{
	CSplashWnd::PreTranslateAppMessage(pMsg);
	return CWinApp::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// 取平台根目录
/////////////////////////////////////////////////////////////////////////////
CString CBlueUIApp::GetPlatRootPath()
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

	return szPath;
}

/////////////////////////////////////////////////////////////////////////////
// 保存并设置当前路径
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::SaveAndSetCurPath(CString strURL)
{
	// 记录当前路径,并设置当前路径
	char szOldPath[256];
	memset(szOldPath, 0, 256);
	DWORD dwPathLen = GetCurrentDirectory(255, szOldPath);
	m_strOldPath = szOldPath;
	CString strPath = strURL;
	strPath.Replace("/", "\\");
	int nPos = strPath.ReverseFind('\\');
	if(nPos >= 0)
	{
		strPath = strPath.Left(nPos);
	}
	SetCurrentDirectory(strPath);
}

/////////////////////////////////////////////////////////////////////////////
// 恢复保存的当前路径
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::RestoreCurPath()
{
	// 执行结束,恢复当前路径的设置
	if(m_strOldPath.GetLength() > 0)
	{
		SetCurrentDirectory(m_strOldPath);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 取配置文件xml中某个键
/////////////////////////////////////////////////////////////////////////////
CString CBlueUIApp::GetConfText(CString strPath)
{
	return m_xmlPlat.GetNodeText(strPath);
}

/////////////////////////////////////////////////////////////////////////////
// 取平台UI接口
/////////////////////////////////////////////////////////////////////////////
IPlatUI* CBlueUIApp::GetIPlatUI()
{
	return (IPlatUI*)(m_PlatUI.ExternalQueryInterface());
}

/////////////////////////////////////////////////////////////////////////////
// 加载插件
/////////////////////////////////////////////////////////////////////////////
BOOL CBlueUIApp::LoadPlugIns()
{
	// 自动加载解释器组件
	AutoLoadPlugIns("Interps");

	//打开插件XML文件
	CString strXmlFile = theApp.GetPlatRootPath();
	strXmlFile += "plugins\\plugins.xml";

	BOOL bAutoLoadFolder = TRUE;
	if(GetFileAttributes(strXmlFile) != 0xFFFFFFFF)
	{
		// 否则按照plugins.xml文件内容来加载插件
		CString strLicensePlugin = m_xmlPlat.GetNodeText("application\\LicensePlugin");

		CXmlParser parser;
		parser.Open(strXmlFile);
		DOMNode* pNode = parser.GetRootNode();
		pNode = parser.GetChildNode(pNode);
		bAutoLoadFolder = (parser.GetNodeAttribute(pNode, "AutoLoad") == "true");	// 自动加载目录标识
		pNode = parser.GetChildNode(pNode);
		while (pNode != NULL)
		{
			BOOL bEnable = (parser.GetNodeAttribute(pNode, "enable") == "true");
			if(bEnable)
			{
				CString strName = parser.GetNodeText(pNode);
				if(strName.CompareNoCase(strLicensePlugin) == 0)
				{
					// 平台默认License组件在程序启动时已经加载,不需要再次加载
					pNode = parser.GetNextNode(pNode);
					continue;
				}
				//CString strAttr = parser.GetNodeAttribute(pNode, "name");
				CString strPath = theApp.GetPlatRootPath();
				strPath += "plugins\\";
				strPath += strName;
				CPlugIn plugin(strPath);
				if(plugin.LoadInfo() && plugin.LoadPlugIn())
				{
					m_aPlugIns.Add(plugin);

					// 如果VCI组件设置了自动创建实例的属性,则创建实例
					if(plugin.m_bAutoCreate)
					{
						m_aPlugIns[m_aPlugIns.GetSize()-1].CreateObject(plugin.m_strAutoCreateInstanceName);
					}

					// 加载所有需要自动创建的DockingPane
					m_aPlugIns[m_aPlugIns.GetSize()-1].LoadDockingPane(-1);
				}
			}

			pNode = parser.GetNextNode(pNode);
		}
		parser.Close();
	}

	if(bAutoLoadFolder)
	{
		// 如果没有找到plugins.xml,或plugins.xml中说明了还要自动加载目录,就自动加载目录中其余的所有插件
		AutoLoadPlugIns("Plugins");
	}

	// 自动加载工程插件(已经合并到Plugins目录)
	//AutoLoadPlugIns("PrjPlugins");

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 按ID查找PlugIn,如果未加载就在Plugins目录下查找插件,如果找到就加载插件
/////////////////////////////////////////////////////////////////////////////
CPlugIn* CBlueUIApp::FindPlugIn(CString strVciId)
{
	for(int i=0; i<m_aPlugIns.GetSize(); i++)
	{
		//OUTPUT_ID(1, "--%s\r\n", m_aPlugIns[i].m_strId);
		if(m_aPlugIns[i].m_strId == strVciId)
			return &(m_aPlugIns[i]);
	}

	CFileFind finder;

	CString strWildcard = theApp.GetPlatRootPath();
	strWildcard += _T("plugins\\*.*") ;

	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// skip . and .. files; otherwise, we'd
		// recur infinitely!

		if (finder.IsDots())
			continue;

		// if not a directory, skip
		if (!finder.IsDirectory())
			continue;

		if (finder.GetFileName() != strVciId)
			continue;

		// if it's a directory, and id is same, then load it
		CString strPath = finder.GetFilePath();
		CPlugIn plugin(strPath);
		if(plugin.LoadInfo() && plugin.LoadPlugIn())
		{
			m_aPlugIns.Add(plugin);

			// 加载所有需要自动创建的DockingPane
			m_aPlugIns[m_aPlugIns.GetSize()-1].LoadDockingPane(-1);

			finder.Close();
			return &(m_aPlugIns[m_aPlugIns.GetSize()-1]);
		}

		finder.Close();
		return NULL;
	}

	finder.Close();

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 在指定的Plugins目录下查找所有插件,如果找到就加载插件
/////////////////////////////////////////////////////////////////////////////
int CBlueUIApp::AutoLoadPlugIns(CString strPlugInPath)
{
	int nCount = 0;

	//CString strLicensePlugin = m_xmlPlat.GetNodeText("application\\LicensePlugin");
	CString strPlugInBasePath = theApp.GetPlatRootPath();
	strPlugInBasePath += strPlugInPath;

	CFileFind finder;

	CString strWildcard = theApp.GetPlatRootPath();
	strWildcard += strPlugInPath;
	strWildcard += _T("\\*.*") ;

	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// skip . and .. files; otherwise, we'd
		// recur infinitely!

		if (finder.IsDots())
			continue;

		// if not a directory, skip
		if (!finder.IsDirectory())
			continue;

		// if it's a directory, and found plusin.xml, then load it
		CString strPath = finder.GetFilePath();
		if( (GetFileAttributes(strPath+"\\plugin.xml") == 0xFFFFFFFF) &&
			(GetFileAttributes(strPath+"\\plugin_cn.xml") == 0xFFFFFFFF))
			continue;

		/*if(strPath.CompareNoCase(strPlugInBasePath + "\\" + strLicensePlugin) == 0)
		{
			// 平台默认License组件在程序启动时已经加载,不需要再次加载
			continue;
		}*/
		// 判断如果已经加载过就不用再加载
		BOOL bExist = FALSE;
		for(int i=0; i<m_aPlugIns.GetSize(); i++)
		{
			if(strPath.CompareNoCase(m_aPlugIns[i].m_strPath) == 0)
			{
				bExist = TRUE;
				break;
			}
		}
		if(bExist)
		{
			continue;
		}

		CPlugIn plugin(strPath);
		if(plugin.LoadInfo() && plugin.LoadPlugIn())
		{
			m_aPlugIns.Add(plugin);

			// 如果VCI组件设置了自动创建实例的属性,则创建实例
			if(plugin.m_bAutoCreate)
			{
				m_aPlugIns[m_aPlugIns.GetSize()-1].CreateObject(plugin.m_strAutoCreateInstanceName);
			}

			// 加载所有需要自动创建的DockingPane
			m_aPlugIns[m_aPlugIns.GetSize()-1].LoadDockingPane(-1);

			nCount++;
		}
	}

	finder.Close();

	return nCount;
}

/////////////////////////////////////////////////////////////////////////////
// 获取OWM对象指针
/////////////////////////////////////////////////////////////////////////////
CPlugIn* CBlueUIApp::GetOWM(UINT nID)
{
	if(nID == 0)
	{
		return NULL;
	}

	for(int i=0; i<m_aPlugIns.GetSize(); i++)
	{
		if(m_aPlugIns[i].IsVciOwm() && (m_aPlugIns[i].m_nOwmId == nID))
			return &(m_aPlugIns[i]);
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 获取OWM对象指针(从名字ID)
/////////////////////////////////////////////////////////////////////////////
CPlugIn* CBlueUIApp::GetOWM(CString strOwmId)
{
	for(int i=0; i<m_aPlugIns.GetSize(); i++)
	{
		if(m_aPlugIns[i].IsVciOwm() && (m_aPlugIns[i].m_strId == strOwmId))
			return &(m_aPlugIns[i]);
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 获取OWM对象id(从名字ID)
/////////////////////////////////////////////////////////////////////////////
int CBlueUIApp::GetOWMID(CString strOwmId)
{
	for(int i=0; i<m_aPlugIns.GetSize(); i++)
	{
		if(m_aPlugIns[i].m_strId == strOwmId)
			return m_aPlugIns[i].m_nOwmId;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 获取动态OWM指针,如果此OWM当前未驻留,则对其进行动态加载
/////////////////////////////////////////////////////////////////////////////
CPlugIn* CBlueUIApp::GetDynamicOWM(UINT nID)
{
	CPlugIn* pOWM = GetOWM(nID);
	if(pOWM && !pOWM->HasLoad())
	{
		// 如果尚未加载,则加载
		pOWM->LoadPlugIn(TRUE);
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 获取动态OWM指针,如果此OWM当前未驻留,则对其进行动态加载(从名字ID)
/////////////////////////////////////////////////////////////////////////////
CPlugIn* CBlueUIApp::GetDynamicOWM(CString strOwmId)
{
	CPlugIn* pOWM = GetOWM(strOwmId);
	if(pOWM && !pOWM->HasLoad())
	{
		// 如果尚未加载,则加载
		pOWM->LoadPlugIn(TRUE);
	}else
	if(!pOWM)
	{
		// 查找是否为未注册的OWM
		pOWM = FindPlugIn(strOwmId);
		if(pOWM && !pOWM->HasLoad())
		{
			// 如果尚未加载,则加载
			pOWM->LoadPlugIn(TRUE);
		}
	}

	return pOWM;
}

/////////////////////////////////////////////////////////////////////////////
// 判断指定OWM窗口是否为当前激活窗口
/////////////////////////////////////////////////////////////////////////////
BOOL CBlueUIApp::CheckActiveOWM(CString strOwmId, CString strDocumentFile)
{
	for(int i=0; i<m_aPlugIns.GetSize(); i++)
	{
		if(m_aPlugIns[i].m_strId == strOwmId)
		{
			// 获取激活的框架
			CFrameWnd* pActiveFrame = ((CMainFrame*)AfxGetMainWnd())->GetActiveFrame();
			CMultiDocTemplate* pOwmDocTemplate = m_aPlugIns[i].m_pDocTemplate;
			if(pOwmDocTemplate == NULL)
			{
				continue;
			}
			CView* pOwmView = NULL;
			POSITION posdoc = pOwmDocTemplate->GetFirstDocPosition();
			while(posdoc)
			{
				CDocument* pDoc = pOwmDocTemplate->GetNextDoc(posdoc);
				POSITION posview = pDoc->GetFirstViewPosition();
				if(posview)	// 如果视存在
				{
					pOwmView = pDoc->GetNextView(posview);	// 取到视指针
					if( (pActiveFrame == pOwmView->GetParentFrame()) &&
						((strDocumentFile == "") || (pDoc->GetPathName() == strDocumentFile)) )
					{
						return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 创建VCI对象
/////////////////////////////////////////////////////////////////////////////
void* CBlueUIApp::CreateVciObject(CString strVciId, CString strInstName)
{
	//OUTPUT_ID(1, "##%s\r\n", strVciId);
	// 查找组件
	CPlugIn* pPlugIn = FindPlugIn(strVciId);
	if(pPlugIn)
	{
		return pPlugIn->CreateObject(strInstName);
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 释放VCI对象
/////////////////////////////////////////////////////////////////////////////
BOOL CBlueUIApp::ReleaseObject(LPVOID pVciObj, BOOL bForceDelete)
{
	for(int i=0; i<m_aPlugIns.GetSize(); i++)
	{
		if(m_aPlugIns[i].ReleaseObject(pVciObj, bForceDelete))
			return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 释放VCI对象(指定实例名)
/////////////////////////////////////////////////////////////////////////////
BOOL CBlueUIApp::ReleaseObjectByInstanceName(CString strInstName)
{
	for(int i=0; i<m_aPlugIns.GetSize(); i++)
	{
		if(m_aPlugIns[i].ReleaseObject(strInstName))
			return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个组件对象(根据实例名)
/////////////////////////////////////////////////////////////////////////////
void* CBlueUIApp::GetObjectByInstanceName(CString strInstName)
{
	for(int i=0; i<m_aPlugIns.GetSize(); i++)
	{
		void* pIObj = m_aPlugIns[i].GetObjectByInstanceName(strInstName);
		if(pIObj != NULL)
		{
			return pIObj;
		}
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个组件对象控制接口(根据组件实例接口指针)
/////////////////////////////////////////////////////////////////////////////
void* CBlueUIApp::GetObjectControlPtrByInstancePtr(LPVOID pVciObj)
{
	for(int i=0; i<m_aPlugIns.GetSize(); i++)
	{
		void* pIControlObj = m_aPlugIns[i].GetObjectControlPtrByInstancePtr(pVciObj);
		if(pIControlObj != NULL)
		{
			return pIControlObj;
		}
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 获取一个组件ID(根据组件实例接口指针)
/////////////////////////////////////////////////////////////////////////////
CString CBlueUIApp::GetPlugInIdByInstancePtr(LPVOID pVciObj)
{
	for(int i=0; i<m_aPlugIns.GetSize(); i++)
	{
		if(m_aPlugIns[i].IsInstancePtr(pVciObj))
		{
			return m_aPlugIns[i].m_strId;
		}
	}
	return "";
}

/////////////////////////////////////////////////////////////////////////////
// 添加定时任务
/////////////////////////////////////////////////////////////////////////////
CTimerTask* CBlueUIApp::AddTimerTask(CString strTaskName, CString strInterpId, CString strInterpName, CString strCommand, int nSeconds, BOOL bStart)
{
	CTimerTask* pTask = FindTimerTask(strTaskName);
	if(pTask != NULL)
	{
		return pTask;
	}

	CSingleLock SingleLock(&m_MutexTimerTask);
	SingleLock.Lock();

	pTask = new CTimerTask(strTaskName, strInterpId, strInterpName, strCommand);
	pTask->SetTaskSeconds(nSeconds);
	if(bStart)
	{
		pTask->StartTask();
	}

	m_lsTimerTask.AddTail(pTask);

	TRACE("Add Script Timer Task: %s\n", strTaskName);

	return pTask;
}

/////////////////////////////////////////////////////////////////////////////
// 添加定时任务
/////////////////////////////////////////////////////////////////////////////
CTimerTask* CBlueUIApp::AddTimerTask(CString strTaskName, LPVOID lpFunction, int nSeconds, BOOL bStart)
{
	CTimerTask* pTask = FindTimerTask(strTaskName);
	if(pTask != NULL)
	{
		return pTask;
	}

	CSingleLock SingleLock(&m_MutexTimerTask);
	SingleLock.Lock();

	pTask = new CTimerTask(strTaskName, lpFunction);
	pTask->SetTaskSeconds(nSeconds);
	if(bStart)
	{
		pTask->StartTask();
	}

	m_lsTimerTask.AddTail(pTask);

	TRACE("Add timer task function %s success\n", strTaskName);

	return pTask;
}

/////////////////////////////////////////////////////////////////////////////
// 查找定时任务
/////////////////////////////////////////////////////////////////////////////
CTimerTask* CBlueUIApp::FindTimerTask(CString strTaskName)
{
	if(strTaskName == "")
	{
		return NULL;
	}

	CSingleLock SingleLock(&m_MutexTimerTask);
	SingleLock.Lock();

	POSITION pos = m_lsTimerTask.GetHeadPosition();
	while(pos != NULL)
	{
		CTimerTask* pItem = m_lsTimerTask.GetAt(pos);

		if(pItem->m_strTaskName == strTaskName)
		{
			return pItem;
		}

		m_lsTimerTask.GetNext(pos);
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 删除定时任务
/////////////////////////////////////////////////////////////////////////////
BOOL CBlueUIApp::DeleteTimerTask(CTimerTask* pTimerTask)
{
	CSingleLock SingleLock(&m_MutexTimerTask);
	SingleLock.Lock();

	POSITION pos = m_lsTimerTask.GetHeadPosition();
	while(pos != NULL)
	{
		CTimerTask* pItem = m_lsTimerTask.GetAt(pos);

		if(pItem == pTimerTask)
		{
			delete pItem;
			m_lsTimerTask.RemoveAt(pos);
			return TRUE;
		}

		m_lsTimerTask.GetNext(pos);
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 删除定时任务
/////////////////////////////////////////////////////////////////////////////
BOOL CBlueUIApp::DeleteTimerTask(CString strTaskName)
{
	if(strTaskName == "")
	{
		return FALSE;
	}

	CSingleLock SingleLock(&m_MutexTimerTask);
	SingleLock.Lock();

	POSITION pos = m_lsTimerTask.GetHeadPosition();
	while(pos != NULL)
	{
		CTimerTask* pItem = m_lsTimerTask.GetAt(pos);

		if(pItem->m_strTaskName == strTaskName)
		{
			delete pItem;
			m_lsTimerTask.RemoveAt(pos);
			return TRUE;
		}

		m_lsTimerTask.GetNext(pos);
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 运行定时任务
/////////////////////////////////////////////////////////////////////////////
BOOL CBlueUIApp::RunTimerTask()
{
	CSingleLock SingleLock(&m_MutexTimerTask);
	SingleLock.Lock();

	POSITION pos = m_lsTimerTask.GetHeadPosition();
	while(pos != NULL)
	{
		CTimerTask* pItem = m_lsTimerTask.GetAt(pos);

		POSITION posDelete = NULL;

		if(pItem)
		{
			int nStatus = pItem->CheckStatus();
			if((nStatus != TIMER_STATE_END) && (nStatus != TIMER_STATE_STOP))
			{
				//TRACE("Run Timer Task: %s\n", pItem->m_strTaskName);
				pItem->RunTask();
			}

			if(pItem->GetStatus() == TIMER_STATE_END)
			{
				TRACE("Delete Timer Task: %s\n", pItem->m_strTaskName);
				delete pItem;
				posDelete = pos;
			}
		}

		m_lsTimerTask.GetNext(pos);

		if(posDelete != NULL)
		{
			m_lsTimerTask.RemoveAt(posDelete);
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 在指定的Tools目录下查找所有工具,如果找到就加载信息
/////////////////////////////////////////////////////////////////////////////
int CBlueUIApp::AutoLoadTools(CString strToolPath)
{
	int nCount = 0;

	CFileFind finder;

	CString strWildcard = theApp.GetPlatRootPath();
	strWildcard += strToolPath;
	strWildcard += _T("\\*.*") ;

	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// skip . and .. files; otherwise, we'd
		// recur infinitely!

		if (finder.IsDots())
			continue;

		// if not a directory, skip
		if (!finder.IsDirectory())
			continue;

		// if it's a directory, and found plusin.xml, then load it
		CString strPath = finder.GetFilePath();
		if( (GetFileAttributes(strPath+"\\tool.xml") == 0xFFFFFFFF) &&
			(GetFileAttributes(strPath+"\\tool_cn.xml") == 0xFFFFFFFF))
			continue;

		CTool tool(strPath);
		if(tool.LoadInfo())
		{
			m_aTools.Add(tool);

			// 加载所有需要自动创建的DockingPane
			m_aTools[m_aTools.GetSize()-1].LoadDockingPane(-1);

			nCount++;
		}
	}

	finder.Close();

	// 初始化所有工具
	for(int i=0; i<m_aTools.GetSize(); i++)
	{
		m_aTools[i].Init();
	}

	return nCount;
}

//////////////////////////////////////////////////////////////////
// 创建工具信息加载线程
//////////////////////////////////////////////////////////////////
ULONG _stdcall _CreateLoadToolsThread(LPVOID pParam)
{
	CBlueUIApp* pApp = (CBlueUIApp*)pParam;
	pApp->AutoLoadTools("Tools");
	AfxEndThread(10);
	return 10;
}

/////////////////////////////////////////////////////////////////////////////
// 线程方式加载所有工具信息
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::CreateLoadToolsThread()
{
	HANDLE	hThread;
	DWORD dwThreadID;
	hThread = ::CreateThread(NULL
							  ,0
							  ,_CreateLoadToolsThread
							  ,(LPVOID)(this)
							  ,0
							  ,&dwThreadID
							  );
}

/////////////////////////////////////////////////////////////////////////////
// 平台COM接口消息处理函数
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::PlatComCmd(WPARAM wParam, LPARAM lParam)
{
	//int nWs = wParam;
	//CString strOwmId = (LPCTSTR)wParam;
	STRU_COMCMD* pComCmd = (STRU_COMCMD*)lParam;
	if(pComCmd == NULL)
	{
		return;
	}
	CString strOwmId = pComCmd->strOwmId;
	int ncCmd = pComCmd->nCmd;
	WPARAM wcParam = pComCmd->wParam;
	LPARAM lcParam = pComCmd->lParam;
	//if((0 == nWs) || (-1 == nWs))
	if(("" == strOwmId) || ("0" == strOwmId) || ("-1" == strOwmId))
	{
		// 是发给平台而不是某个OWM的
		int nRet = CommandPlat(ncCmd, wcParam, lcParam);
		if(pComCmd->lpResult != NULL)	// 填充返回结果
		{
			int* pnRet = (int*)(pComCmd->lpResult);
			*pnRet = nRet;
		}
	}else
	{
		CPlugIn* pOWM = GetDynamicOWM(strOwmId);
		if(pOWM != NULL)
		{
			IVciOwm* pIVciOwm = pOWM->GetIVciOwm();
			if(pIVciOwm)
			{
				int nRet = pIVciOwm->Command(ncCmd, wcParam, lcParam);
				if(pComCmd->lpResult != NULL)	// 填充返回结果
				{
					int* pnRet = (int*)(pComCmd->lpResult);
					*pnRet = nRet;
				}
			}
		}
	}

	if(pComCmd->nDataLen > 0)	// 删除携带的数据
	{
		delete ((BYTE*)(pComCmd->lParam));
		pComCmd->lParam = NULL;
	}

	if(pComCmd->bPost)	// 如果是PostMessage则删除消息结构
	{
		delete pComCmd;
		pComCmd = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// 处理命令行操作
/////////////////////////////////////////////////////////////////////////////
BOOL CBlueUIApp::ShellCommand(CString strCommand, CStringArray& asFile)
{
	int nPos = strCommand.Find(_T("CMD_"));
	if(nPos != 0)
	{
		return FALSE;
	}

	strCommand.Delete(0, nPos+4);
	int nCmdID = atoi(strCommand);

	int i;
	CString strParams = _T("?files=");
	for(i=0; i<asFile.GetSize(); i++)
	{
		strParams += asFile[i];
		strParams += _T(",");
	}

	// 查找菜单ID对应的动作,在插件的Ribbon和PopupMenu中查找
	/*for(i=0; i<m_aPlugIns.GetSize(); i++)
	{
		for(int j=0; j<m_aPlugIns[i].m_arRibbonTab.GetSize(); j++)
		{
			for(int k=0; k<m_aPlugIns[i].m_arRibbonTab[j].m_arVciRibbonGroup.GetSize(); k++)
			{
				for(int m=0; m<m_aPlugIns[i].m_arRibbonTab[j].m_arVciRibbonGroup[k].m_arVciRibbonControl.GetSize(); m++)
				{
					if(m_aPlugIns[i].m_arRibbonTab[j].m_arVciRibbonGroup[k].m_arVciRibbonControl[m].m_nId == nCmdID)
					{
						if(m_aPlugIns[i].m_arRibbonTab[j].m_arVciRibbonGroup[k].m_arVciRibbonControl[m].m_strAction == _T("htmlview"))
						{
							ShowUIPage(m_aPlugIns[i].m_arRibbonTab[j].m_arVciRibbonGroup[k].m_arVciRibbonControl[m].m_strScript + strParams);
							return TRUE;
						}
					}
				}
			}
		}
	}

	for(i=0; i<m_aPlugIns.GetSize(); i++)
	{
		for(int j=0; j<m_aPlugIns[i].m_arPopupMenu.GetSize(); j++)
		{
			if(m_aPlugIns[i].m_arPopupMenu[j].m_nId == nCmdID)
			{
				if(m_aPlugIns[i].m_arPopupMenu[j].m_strAction == _T("htmlview"))
				{
					ShowUIPage(m_aPlugIns[i].m_arPopupMenu[j].m_strScript + strParams);
					return TRUE;
				}
			}
		}
	}*/

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 处理命令行的打开文件操作
/////////////////////////////////////////////////////////////////////////////
BOOL CBlueUIApp::ShellOpenFile(CString strFileName)
{
	CDocument* pDoc = OpenDocumentFile(strFileName);
	return (NULL != pDoc);
}

/////////////////////////////////////////////////////////////////////////////
// 打开文档(重载)
/////////////////////////////////////////////////////////////////////////////
CDocument* CBlueUIApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{
	if(lpszFileName == NULL)
	{
		CPlugIn* pOwmEdit = GetDynamicOWM(m_strDefaultEditor);//"OwmEdit";
		if(pOwmEdit && pOwmEdit->m_pDocTemplate)
		{
			return pOwmEdit->m_pDocTemplate->OpenDocumentFile(NULL);
		}
	}

	// 打开Recent File时会调用此函数
	if( (lpszFileName == "") ||
		(GetFileAttributes(lpszFileName) == 0xFFFFFFFF))
	{
		return NULL;
	}

	// 挨个OWM查找扩展点,看哪个OWM可以打开此文档
	for(int i=0; i<m_aPlugIns.GetSize(); i++)
	{
		if(!m_aPlugIns[i].IsVciOwm())
			continue;

		CDocument* pDoc = m_aPlugIns[i].OpenDocument(lpszFileName);
		if(pDoc)
		{
			return pDoc;
		}
	}
/*
	// 查找是否工程文件
	CString strFileName = lpszFileName;
	strFileName.MakeLower();
	int nPos = strFileName.ReverseFind('.');
	if(nPos != -1)
	{
		CString strFileExt = strFileName;
		strFileExt.Delete(0, nPos+1);
		for(int i=0; i<m_aPlugIns.GetSize(); i++)
		{
			if(m_aPlugIns[i].m_strType != "project")
				continue;

			for(int j=0; j<m_aPlugIns[i].m_arDocExtend.GetSize(); j++)
			{
				for(int k=0; k<m_aPlugIns[i].m_arDocExtend[j].m_arDocExt.GetSize(); k++)
				{
					if(strFileExt == m_aPlugIns[i].m_arDocExtend[j].m_arDocExt[k])
					{
						if(OpenProjectFile(&(m_aPlugIns[i]), lpszFileName))
						{
							return NULL;
						}
					}
				}
			}
		}
	}
*/
	// 如果没有找到可以打开文档的模块,就用缺省编辑器打开
	CPlugIn* pOwmEdit = GetDynamicOWM(m_strDefaultEditor);
	if(pOwmEdit && pOwmEdit->m_pDocTemplate)
	{
		return pOwmEdit->m_pDocTemplate->OpenDocumentFile(lpszFileName);
	}

	// 打开Script.NET工程
	//OpenProject(lpszFileName);

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 打开文档(指定用哪种OWM模块)
/////////////////////////////////////////////////////////////////////////////
CDocument* CBlueUIApp::OpenDocumentFileWithOwm(LPCTSTR lpszFileName, LPCTSTR lpszOwmId) 
{
	CString strOwmId = lpszOwmId;
	if(strOwmId == "")
	{
		if(strcmp(lpszFileName, "<null>") == 0)
		{
			return OpenDocumentFile(NULL);
		}else
		{
			return OpenDocumentFile(lpszFileName);
		}
	}

	if( (lpszFileName == NULL) ||
		(strcmp(lpszFileName, "<null>") == 0) )
	{
		CPlugIn* pOwm = GetDynamicOWM(strOwmId);
		if(pOwm && pOwm->m_pDocTemplate)
		{
			return pOwm->m_pDocTemplate->OpenDocumentFile(NULL);
		}
	}

	if( (lpszFileName == "") ||
		(GetFileAttributes(lpszFileName) == 0xFFFFFFFF))
	{
		return NULL;
	}

	CString strFilePath = lpszFileName;
	strFilePath.Replace("/", "\\");
	strFilePath.Replace("\\.\\", "\\");	// 将无用的路径去掉

	CPlugIn* pOwm = GetDynamicOWM(strOwmId);
	if(pOwm && pOwm->m_pDocTemplate)
	{
		// 防止打开已经打开的文档,因此先搜一遍是否已经打开了,如果是就直接激活
		POSITION posdoc = pOwm->m_pDocTemplate->GetFirstDocPosition();
		while(posdoc)
		{
			CDocument* pDoc = pOwm->m_pDocTemplate->GetNextDoc(posdoc);
			CString strPathName = pDoc->GetPathName();
			strPathName.MakeUpper();
			strFilePath.MakeUpper();
			if(strFilePath == strPathName)
			{
				POSITION posview = pDoc->GetFirstViewPosition();
				if(posview)
				{
					CView* pView = pDoc->GetNextView(posview);
					pView->GetParentFrame()->ActivateFrame(); // 激活框架
				}
				return pDoc;
			}
		}

		return pOwm->m_pDocTemplate->OpenDocumentFile(lpszFileName);
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 打开Html窗口
/////////////////////////////////////////////////////////////////////////////
CDocument* CBlueUIApp::OpenHtmlWindow(CString strUrl, BOOL bActivate, BOOL bReplaceBlank)
{
	CPlugIn* pOwm = GetDynamicOWM(VCIID_HTMLVIEW);
	if((pOwm == NULL) || (pOwm->m_pDocTemplate == NULL))
	{
		return NULL;
	}

	if (bReplaceBlank)
	{
		CChildFrame* pFrame = (CChildFrame*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive();

		if(pFrame)
		{
			CView* pActiveView = pFrame->GetActiveView();

			POSITION posdoc = pOwm->m_pDocTemplate->GetFirstDocPosition();
			while(posdoc)
			{
				CDocument* pDoc = pOwm->m_pDocTemplate->GetNextDoc(posdoc);
				POSITION posview = pDoc->GetFirstViewPosition();
				if(posview)
				{
					CView* pView = pDoc->GetNextView(posview);
					if(pView->GetParentFrame()->GetActiveView() == pActiveView)
					{
						COM_CMD(VCIID_HTMLVIEW, OC_VIEWHTMLPAGE, (WPARAM)(LPCTSTR)strUrl, (int)pActiveView);
						return pDoc;
					}
				}
			}
		}
		return NULL;
	}
	
	//COM_CMD(VCIID_HTMLVIEW, OC_VIEWHTMLPAGE, (WPARAM)(LPCTSTR)strUrl, 0);
	//return NULL;
	pOwm->m_pDocTemplate->OpenDocumentFile(NULL);
	CChildFrame* pFrame = (CChildFrame*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive();
	if(pFrame)
	{
		CView* pActiveView = pFrame->GetActiveView();
		if(pActiveView)
		{
			COM_CMD(VCIID_HTMLVIEW, OC_VIEWHTMLPAGE, (WPARAM)(LPCTSTR)strUrl, (int)pActiveView);
			return NULL;
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 新建文件
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnFileNew() 
{
	OpenDocumentFile(NULL);
}

void CBlueUIApp::OnUpdateFileNew(CCmdUI* pCmdUI) 
{
	
}

/////////////////////////////////////////////////////////////////////////////
// 打开文件
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnFileOpen() 
{
	CString strFilter;
	CString strTemp;

	CString strDefaultExt = GetProfileString(REG_CONFIG_SUBKEY, REG_CONFIG_DEFFILEEXT, "");
	strDefaultExt.MakeLower();
	BOOL bFoundDefaultExt = FALSE;

	//strFilter = "Script.NET project file (*.snp)|*.snp|";
	// 挨个OWM查找扩展点,看哪个OWM可以打开此文档
	for(int i=0; i<m_aPlugIns.GetSize(); i++)
	{
		if(!m_aPlugIns[i].IsVciOwm())
			continue;

		for(int j=0; j<m_aPlugIns[i].m_arDocExtend.GetSize(); j++)
		{
			BOOL bDefaultExt = FALSE;
			CString strTemp1;
			for(int k=0; k<m_aPlugIns[i].m_arDocExtend[j].m_arDocExt.GetSize(); k++)
			{
				strTemp1 += "*.";
				strTemp1 += m_aPlugIns[i].m_arDocExtend[j].m_arDocExt[k];
				if(k != (m_aPlugIns[i].m_arDocExtend[j].m_arDocExt.GetSize()-1))
				{
					strTemp1 += ";";
				}

				CString strDocExt = m_aPlugIns[i].m_arDocExtend[j].m_arDocExt[k];
				strDocExt.MakeLower();
				if(strDefaultExt == strDocExt)
				{
					// 是上次保存的后缀名
					bDefaultExt = TRUE;
				}
			}

			strTemp.Format("%s (%s)|%s|", m_aPlugIns[i].m_arDocExtend[j].m_strDocName, strTemp1, strTemp1);
			if(bDefaultExt && !bFoundDefaultExt)
			{
				strFilter = strTemp + strFilter;
				bFoundDefaultExt = TRUE;
			}else
			{				
				strFilter += strTemp;
			}
		}
	}
	strFilter += "All files(*.*)|*.*||";

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,	strFilter);
	if (dlg.DoModal() == IDOK)
	{
		CString strExt = dlg.GetFileExt();
		WriteProfileString(REG_CONFIG_SUBKEY, REG_CONFIG_DEFFILEEXT, strExt);
		OpenDocumentFile(dlg.GetPathName());
	}
}

void CBlueUIApp::OnUpdateFileOpen(CCmdUI* pCmdUI) 
{
	
}

/////////////////////////////////////////////////////////////////////////////
// 使最近打开的文件放在子菜单上
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnUpdateFileMruFile1(CCmdUI* pCmdUI) 
{
	if(pCmdUI->m_nIndex == 0)
	{
		OnUpdateRecentFileMenu(pCmdUI);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 系统设置
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnOption() 
{
	// 显示Flash窗口
	FlashWin FlashDlg;
	FlashDlg.Create(IDD_DIALOGFLASH);
	FlashDlg.ShowWindow(SW_SHOW);
	FlashDlg.CenterWindow();
	FlashDlg.Invalidate(TRUE);

	CTreePropertySheet tps(150);

	// 从License组件中获取平台关于对话框
	CDialog* pLicenseTpsPlatform = NULL;
	CDialog* pLicenseTpsOwm = NULL;
	CDialog* pLicenseTpsInterp = NULL;
	CString strLicensePlugin = m_xmlPlat.GetNodeText("application\\LicensePlugin");
	CPlugIn* pLicensePlugIn = FindPlugIn(strLicensePlugin);
	if(pLicensePlugIn)
	{
		if(!(pLicensePlugIn->HasLoad()))
		{
			pLicensePlugIn->LoadPlugIn(TRUE);
		}
		CVciPropertyPageArray aLicensePropertyPage;
		pLicensePlugIn->RegisterPropertyPage(aLicensePropertyPage);
		pLicensePlugIn->RegisterPropertyPage(aLicensePropertyPage);
		pLicensePlugIn->RegisterPropertyPage(aLicensePropertyPage);
		if(aLicensePropertyPage.GetSize() > 2)
		{
			pLicenseTpsPlatform = (CDialog*)(aLicensePropertyPage[0].pPage);
			pLicenseTpsOwm = (CDialog*)(aLicensePropertyPage[1].pPage);
			pLicenseTpsInterp = (CDialog*)(aLicensePropertyPage[2].pPage);
		}
	}

	// 平台配置
	PLAT_LOADSTRING(strtpsPlatform, IDS_TPS_PLATFORM); // 平台
	PLAT_LOADSTRING(strtpsAboutPlatform, IDS_TPS_ABOUT_PLATFORM); // 关于平台 ...
	if(pLicenseTpsPlatform)
	{
		tps.AddPage(strtpsPlatform,tps_item_branch,-1,strtpsAboutPlatform,pLicenseTpsPlatform,OPT_PLATFORM,TRUE);
	}else
	{
		tps.AddPage(strtpsPlatform,tps_item_branch,IDD_TPS_PLATFORM,strtpsAboutPlatform,NULL,OPT_PLATFORM);
	}

	if(m_xmlPlat.GetNodeAttribute("options\\Language", "enable") != "false")
	{
		CTpsLanguage* pTpsLanguage = new CTpsLanguage();
		PLAT_LOADSTRING(strtpsLanguage, IDS_TPS_LANGUAGE); // 语言设置
		PLAT_LOADSTRING(strtpsAboutLanguage, IDS_TPS_ABOUT_LANGUAGE); // 设置平台语言...
		tps.AddPage(strtpsLanguage,tps_item_node,IDD_TPS_LANGUAGE,strtpsAboutLanguage,pTpsLanguage,OPT_NODE);
	}

	if(m_xmlPlat.GetNodeAttribute("options\\UIStyle", "enable") != "false")
	{
		CTpsUIStyle* pTpsUIStyle = new CTpsUIStyle();
		PLAT_LOADSTRING(strtpsUIStyle, IDS_TPS_UISTYLE); // 界面风格
		PLAT_LOADSTRING(strtpsAboutUIStyle, IDS_TPS_ABOUT_UISTYLE); // 设置界面风格...
		tps.AddPage(strtpsUIStyle,tps_item_node,IDD_TPS_UISTYLE,strtpsAboutUIStyle,pTpsUIStyle,OPT_NODE);
	}

	if(m_xmlPlat.GetNodeAttribute("options\\Font", "enable") != "false")
	{
		CTpsFont* pTpsFont = new CTpsFont();
		PLAT_LOADSTRING(strtpsFont, IDS_TPS_FONT); // 字体设置
		PLAT_LOADSTRING(strtpsAboutFont, IDS_TPS_ABOUT_FONT); // 设置平台字体和颜色...
		tps.AddPage(strtpsFont,tps_item_node,IDD_TPS_FONT,strtpsAboutFont,pTpsFont,OPT_NODE);
	}

	if(m_xmlPlat.GetNodeAttribute("options\\Associate", "enable") != "false")
	{
		CTpsAssociate* pTpsAssociate = new CTpsAssociate();
		PLAT_LOADSTRING(strtpsAssociate, IDS_TPS_ASSOCIATE); // 文件关联
		PLAT_LOADSTRING(strtpsAboutAssociate, IDS_TPS_ABOUT_ASSOCIATE); // 设置文件关联...
		tps.AddPage(strtpsAssociate,tps_item_node,IDD_TPS_ASSOCIATE,strtpsAboutAssociate,pTpsAssociate,OPT_NODE);
	}

	if(m_xmlPlat.GetNodeAttribute("options\\Workspace", "enable") != "false")
	{
		CTpsWorkspace* pTpsWorkspace = new CTpsWorkspace();
		PLAT_LOADSTRING(strtpsWorkspace, IDS_TPS_WORKSPACE); // 工作区设置
		PLAT_LOADSTRING(strtpsAboutWorkspace, IDS_TPS_ABOUT_WORKSPACE); // 设置工作空间...
		tps.AddPage(strtpsWorkspace,tps_item_node,IDD_TPS_WORKSPACE,strtpsAboutWorkspace,pTpsWorkspace,OPT_NODE);
	}

	tps.SetLastPageType(tps_item_endbranch);

	// OWM模块属性
	PLAT_LOADSTRING(strtpsOwm, IDS_TPS_OWM); // 扩展模块
	PLAT_LOADSTRING(strtpsAboutOwm, IDS_TPS_ABOUT_OWM); // 关于扩展模块 ...
	if(pLicenseTpsOwm)
	{
		tps.AddPage(strtpsOwm,tps_item_branch,-1,strtpsAboutOwm,pLicenseTpsOwm,OPT_OWM,TRUE);
	}else
	{
		tps.AddPage(strtpsOwm,tps_item_branch,IDD_TPS_PLATFORM,strtpsAboutOwm,NULL,OPT_OWM);
	}

	CDWordArray aOWMDynUnTsrs;	// 处于动态卸载状态的OWM列表
	// 注册OWM属性页
	COWMOptionArray aOWMOptions;	// OWM属性页列表
	for(int i=0; i<m_aPlugIns.GetSize(); i++)
	{
		if(!m_aPlugIns[i].IsVciOwm())
		{
			continue;
		}

		// 此模块是否允许显示属性页
		if((m_aPlugIns[i].m_nType & OWM_OPTION) == 0)
		{
			continue;
		}

		if(!m_aPlugIns[i].HasLoad())
		{
			// 如果处于动态卸载状态就暂时加载,结束后再恢复
			m_aPlugIns[i].LoadPlugIn(TRUE);
			aOWMDynUnTsrs.Add(i);
		}

		if(!m_aPlugIns[i].HasLoad())
		{
			continue;
		}

		int oldcount = aOWMOptions.GetSize();
		m_aPlugIns[i].RegisterOption(aOWMOptions);
		int count = aOWMOptions.GetSize() - oldcount;
		if(count > 0)
		{
			for(int j=0; j<count; j++)
			{
				int index = oldcount+j;
				CString strNode = aOWMOptions[index].strNode;
				CString strDesc = aOWMOptions[index].strDesc;
				CDialog* pDialog = (CDialog*)(aOWMOptions[index].pOptionPage);
				int nType;
				if(count == 1)
					nType = tps_item_node;
				else
				{
					nType = tps_item_node;
					if(j == 0)
						nType = tps_item_branch;
					if(j+1 == count)
						nType = tps_item_endbranch;
				}
				tps.AddPage(strNode,nType,-1,strDesc,pDialog,OPT_NODE,TRUE);
			}
		}
	}

	tps.SetLastPageType(tps_item_endbranch);

	// 解释器属性
	PLAT_LOADSTRING(strtpsInterp, IDS_TPS_INTERP); // 解释器
	PLAT_LOADSTRING(strtpsAboutInterp, IDS_TPS_ABOUT_INTERP); // 关于解释器 ...
	if(pLicenseTpsInterp)
	{
		tps.AddPage(strtpsInterp,tps_item_branch,-1,strtpsAboutInterp,pLicenseTpsInterp,OPT_INTERP,TRUE);
	}else
	{
		tps.AddPage(strtpsInterp,tps_item_branch,IDD_TPS_PLATFORM,strtpsAboutInterp,NULL,OPT_INTERP);
	}

	// 重定向参数设置
	CTpsInterpDirect* pTpsInterpDirect = new CTpsInterpDirect();
	PLAT_LOADSTRING(strtpsInterpDirect, IDS_TPS_INTERP_DIRECT); // 输出重定向
	PLAT_LOADSTRING(strtpsAboutInterpDirect, IDS_TPS_ABOUT_INTERP_DIRECT); // 设置输出重定向参数...
	tps.AddPage(strtpsInterpDirect,tps_item_node,IDD_TPS_INTERP_DIRECT,strtpsAboutInterpDirect,pTpsInterpDirect,OPT_NODE);

	CDWordArray aInterpDynUnTsrs;	// 处于动态卸载状态的解释器列表
	// 注册解释器属性页
	CVciPropertyPageArray aInterpPropertyPage;	// 解释器属性页列表
	for(int i=0; i<m_aPlugIns.GetSize(); i++)
	{
		if(!m_aPlugIns[i].IsVciInterp())
		{
			continue;
		}

		if(!m_aPlugIns[i].HasLoad())
		{
			// 如果处于动态卸载状态就暂时加载,结束后再恢复
			m_aPlugIns[i].LoadPlugIn(TRUE);
			aInterpDynUnTsrs.Add(i);
		}

		if(!m_aPlugIns[i].HasLoad())
		{
			continue;
		}

		int oldcount = aInterpPropertyPage.GetSize();
		m_aPlugIns[i].RegisterPropertyPage(aInterpPropertyPage);
		int count = aInterpPropertyPage.GetSize() - oldcount;
		if(count > 0)
		{
			for(int j=0; j<count; j++)
			{
				int index = oldcount+j;
				CString strNode = aInterpPropertyPage[index].strNode;
				CString strDesc = aInterpPropertyPage[index].strDesc;
				CDialog* pDialog = (CDialog*)(aInterpPropertyPage[index].pPage);
				int nType;
				if(count == 1)
					nType = tps_item_node;
				else
				{
					nType = tps_item_node;
					if(j == 0)
						nType = tps_item_branch;
					if(j+1 == count)
						nType = tps_item_endbranch;
				}
				tps.AddPage(strNode,nType,-1,strDesc,pDialog,OPT_NODE,TRUE);
			}
		}
	}

	tps.SetLastPageType(tps_item_endbranch);

	// 组件属性
	CTpsVci* pTpsVci = new CTpsVci();
	PLAT_LOADSTRING(strtpsVci, IDS_TPS_VCI); // VCI组件
	PLAT_LOADSTRING(strtpsAboutVci, IDS_TPS_ABOUT_VCI); // 关于VCI组件 ...
	tps.AddPage(strtpsVci,tps_item_branch,IDD_TPS_VCI,strtpsAboutVci,pTpsVci,OPT_VCI);

	CDWordArray aVciDynUnTsrs;	// 处于动态卸载状态的解释器列表
	// 注册VCI组件属性页
	CVciPropertyPageArray aVciPropertyPage;	// VCI组件属性页列表
	for(int i=0; i<m_aPlugIns.GetSize(); i++)
	{
		if(m_aPlugIns[i].IsVciOwm() || m_aPlugIns[i].IsVciInterp() || m_aPlugIns[i].IsVciLicense())
		{
			continue;
		}

		if(!m_aPlugIns[i].HasLoad())
		{
			// 如果处于动态卸载状态就暂时加载,结束后再恢复
			m_aPlugIns[i].LoadPlugIn(TRUE);
			aVciDynUnTsrs.Add(i);
		}

		if(!m_aPlugIns[i].HasLoad())
		{
			continue;
		}

		int oldcount = aVciPropertyPage.GetSize();
		m_aPlugIns[i].RegisterPropertyPage(aVciPropertyPage);
		int count = aVciPropertyPage.GetSize() - oldcount;
		if(count > 0)
		{
			for(int j=0; j<count; j++)
			{
				int index = oldcount+j;
				if((j == 0) && (aVciPropertyPage[index].strParent == "[]"))
				{
					break;
				}
				CString strNode = aVciPropertyPage[index].strNode;
				CString strDesc = aVciPropertyPage[index].strDesc;
				CDialog* pDialog = (CDialog*)(aVciPropertyPage[index].pPage);
				int nType;
				if(count == 1)
					nType = tps_item_node;
				else
				{
					nType = tps_item_node;
					if(j == 0)
						nType = tps_item_branch;
					if(j+1 == count)
						nType = tps_item_endbranch;
				}
				tps.AddPage(strNode,nType,-1,strDesc,pDialog,OPT_NODE,TRUE);
			}
		}
	}

	tps.SetLastPageType(tps_item_endbranch);

	for(int i=0; i<aVciPropertyPage.GetSize(); i++)
	{
		CString strParent = aVciPropertyPage[i].strParent;
		if(strParent.Find("[]") != 0)
		{
			continue;
		}
		strParent.Delete(0, 2);
		CString strNode = aVciPropertyPage[i].strNode;
		CString strDesc = aVciPropertyPage[i].strDesc;
		CDialog* pDialog = (CDialog*)(aVciPropertyPage[i].pPage);
		int nType;
		nType = tps_item_node;
		if(strParent == "")
		{
			nType = tps_item_branch;
		}
		tps.AddPage(strNode,nType,-1,strDesc,pDialog,OPT_NODE,TRUE);
	}

	tps.SetLastPageType(tps_item_endbranch);


	tps.SetFancy(FALSE);
	//tps.SetLikeNetscape(FALSE);

	// 隐藏Flash窗口
	FlashDlg.ShowWindow(SW_HIDE);

	// 执行属性设置对话框(注:将原来中间层结果保存去掉了,因为此处关于中间层的设置已经移走了)
	tps.DoModal();

	// 释放资源,防止因下面的操作而引起析构函数出错
	tps.RemoveResource();

	// 恢复动态卸载的OWM模块到动态卸载状态
	for(int i=0; i< aOWMDynUnTsrs.GetSize(); i++)
	{
		m_aPlugIns[aOWMDynUnTsrs[i]].CloseVci();
	}
}

void CBlueUIApp::OnUpdateOption(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 组件管理器
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnVciManager() 
{
	ExecMenuScript("Tools\\PluginManager");
}

void CBlueUIApp::OnUpdateVciManager(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 对路径字符串进行处理
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::ProcessPath(CString& strPath, CString strPlugInId)
{
	CString strTemp;
	if(strPath.Find(_T("PLAT://")) == 0)
	{
		strTemp = strPath.Right(strPath.GetLength()-7);
		strTemp.Replace("/", "\\");
		if(strTemp.Find(_T("\\")) == 0)
		{	// GetPlatRootPath已经包含了\,因此可以去掉
			strTemp = strTemp.Right(strTemp.GetLength()-1);
		}
		strPath = GetPlatRootPath() + strTemp;
	}else
	if((strPath.Find(_T("PLUGIN://")) != -1) && (strPlugInId != ""))
	{
		int nPos  = strPath.Find(_T("PLUGIN://"));
		strTemp = strPath.Right(strPath.GetLength()-9-nPos);
		strTemp.Replace("/", "\\");
		if(strTemp.Find(_T("\\")) == 0)
		{	// GetPlatRootPath已经包含了\,因此可以去掉
			strTemp = strTemp.Right(strTemp.GetLength()-1);
		}
		strPath = strPath.Left(nPos) + GetPlatRootPath() + "Plugins\\" + strPlugInId + "\\" + strTemp;
	}
}

/////////////////////////////////////////////////////////////////////////////
// 从xml配置文件中查找并执行脚本(对应某个菜单命令)
/////////////////////////////////////////////////////////////////////////////
CString CBlueUIApp::ExecMenuScript(CString strMenuName)
{
	CString strXmlNodeName = _T("menus\\");
	strXmlNodeName += strMenuName;
	CString strInterp = m_xmlPlat.GetNodeText(strXmlNodeName + "\\Interp");
	CString strInterpInstance = m_xmlPlat.GetNodeAttribute(strXmlNodeName + "\\Interp" , "Name");
	CString strScriptFile = m_xmlPlat.GetNodeText(strXmlNodeName + "\\script");
	if(strInterp == VCIID_HTMLVIEW)
	{
		COM_CMD(VCIID_HTMLVIEW, OC_VIEWHTMLPAGE, (WPARAM)(LPCTSTR)strScriptFile, 0);
		return "";
	}else
	if(strInterp == "shellfile")
	{
		ProcessPath(strScriptFile);
		ShellExecute(NULL,"open",strScriptFile,NULL,NULL,SW_SHOWNORMAL);
		return "";
	}

	CString strResult = "";
	ProcessPath(strScriptFile);
	IInterp* pInterp = (IInterp*)CreateVciObject(strInterp, strInterpInstance);
	if(pInterp)
	{
		if(!pInterp->RunScriptFile(strScriptFile))
		{
			AfxMessageBox(pInterp->GetResult());
		}
		strResult = pInterp->GetResult();
		ReleaseObject(pInterp);
	}

	return strResult;
}

/////////////////////////////////////////////////////////////////////////////
// 新建文件
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnProjectNewfile() 
{
	ExecMenuScript("files\\newfile");
}

void CBlueUIApp::OnUpdateProjectNewfile(CCmdUI* pCmdUI) 
{
	
}

/////////////////////////////////////////////////////////////////////////////
// 帮助主体
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnHelpTopics() 
{
	ExecMenuScript("Help\\Topic");
}

void CBlueUIApp::OnUpdateHelpTopics(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 注册
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnHelpRegisterate() 
{
	ExecMenuScript("Help\\Register");
}

void CBlueUIApp::OnUpdateHelpRegisterate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 升级
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnHelpUpgrade() 
{
	ExecMenuScript("Help\\Update");
}

void CBlueUIApp::OnUpdateHelpUpgrade(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 下载
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnHelpDownload() 
{
	ExecMenuScript("Help\\Download");
}

void CBlueUIApp::OnUpdateHelpDownload(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 主页
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnHelpHomepage() 
{
	ExecMenuScript("Help\\Homepage");
}

void CBlueUIApp::OnUpdateHelpHomepage(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 反馈
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnHelpBugreport() 
{
	ExecMenuScript("Help\\BugReport");
}

void CBlueUIApp::OnUpdateHelpBugreport(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 扩展工具
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnToolsExtend() 
{
	ExecMenuScript("Tools\\Extends");
}

void CBlueUIApp::OnUpdateToolsExtend(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 选择OWM模块打开文档
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnOpenWith() 
{
	CChildFrame* pFrame = (CChildFrame*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive();
	if(pFrame)
	{
		CView* pActiveView = pFrame->GetActiveView();
		if(pActiveView)
		{
			CDocument* pDoc = pActiveView->GetDocument();
			if(pDoc)
			{
				CString strFilePath = pDoc->GetPathName();
				IInterp* pInterp = (IInterp*)(CreateVciObject("org.interp.tcl"));
				if(pInterp)
				{
					CString strTclScript = theApp.GetPlatRootPath();
					strTclScript += "lib/plat/openfilewith.tcl?";
					strTclScript += strFilePath;
					//strTclScript += "\"";
					pInterp->RunScriptFile(strTclScript);
					//CString strRes = pInterp->GetResult();
					ReleaseObject(pInterp);
				}
			}
		}
	}
}

void CBlueUIApp::OnUpdateOpenWith(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 显示Popup窗口
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::ShowPopupControl(TPopupControlInfo* pInfo)
{
	if (m_wndPopupControl.GetSafeHwnd())
		return;

	m_infoPopupControl = *pInfo;

	XTPPopupPaintTheme theme = xtpPopupThemeOffice2007;
	if(pInfo->strTheme == "msn")
	{
		theme = xtpPopupThemeMSN;
	}else
	if(pInfo->strTheme == "office2003")
	{
		theme = xtpPopupThemeOffice2003;
	}

	m_wndPopupControl.RemoveAllItems();

	CSize szPopup;
	CSize szTextItem;

	int nWidth = 220;
	if(pInfo->nWidth != 0)
	{
		nWidth = pInfo->nWidth;
	}

	int nHight = (pInfo->strText.GetLength() / ((nWidth-66) / 5) +1) * 16;
	if(pInfo->asButtonText.GetSize() > 0)
	{
		// 留出按钮行的位置
		nHight += 30;
	}

	int nAddHight = pInfo->nAddHight;
	CString strIcon = pInfo->strIcon;

	HICON hIcon = NULL;
	if(strIcon.Find(":") != -1)
	{
		WORD wIndex = 0;
		hIcon = ::ExtractAssociatedIcon(theApp.m_hInstance,
				strIcon.GetBuffer(0), &wIndex);
		strIcon.ReleaseBuffer();
	}

	if(theme == xtpPopupThemeOffice2003 || theme == xtpPopupThemeOffice2007)
	{
		// add text items.
		CXTPPopupItem* pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
			new CXTPPopupItem(CRect(46, 16, nWidth-20, 41), pInfo->strCaption));
		pItemText->SetTextColor(RGB(0, 61, 128));
		pItemText->SetBold(TRUE);
		pItemText->SetHyperLink(FALSE);

		pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
			new CXTPPopupItem(CRect(46, 30, nWidth-20, 46), pInfo->strTitle));
		pItemText->SetTextColor(RGB(0, 61, 128));
		pItemText->SetBold(TRUE);
		pItemText->SetHyperLink(FALSE);
		if(pInfo->strLinkTitle != "")
		{
			pItemText->SetID(ID_POPUP_GOTO_SITE1);
			pItemText->SetHyperLink(TRUE);
		}

		if(pInfo->nItemType == POPUP_ITEM_TEXT)
		{
			pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
				new CXTPPopupItem(CRect(46, 50, nWidth-20, nHight+60+nAddHight), pInfo->strText));
		}else
		{
			pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
				new CXTPPopupItem(CRect(46, 50, nWidth-20, nHight+60+nAddHight)));
		}
		pItemText->SetTextColor(RGB(0, 61, 128));
		pItemText->SetTextAlignment(DT_LEFT|DT_WORDBREAK);
		pItemText->SetHyperLink(FALSE);
		if(pInfo->strLinkText != "")
		{
			pItemText->SetID(ID_POPUP_GOTO_SITE2);
			pItemText->SetHyperLink(TRUE);
		}
		if(pInfo->nItemType == POPUP_ITEM_RTF)
		{
			pItemText->SetRTFText(pInfo->strText);
		}else
		if(pInfo->nItemType == POPUP_ITEM_MARKUP)
		{
			pItemText->SetMarkupText(pInfo->strText);
		}
		pItemText->FitToContent();

		szTextItem = CSize(pItemText->GetRect().Size());

		if((pInfo->nItemType == POPUP_ITEM_MARKUP) && (pInfo->nWidth == 0))
		{
			nWidth = szTextItem.cx + 70;
		}

		// left icon.
		CXTPPopupItem* pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
			new CXTPPopupItem(CRect(5, 12, 17, 24)));
		if (strIcon == "info")
		{
			pItemIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
				MAKEINTRESOURCE(IDI_ICON_INFO), IMAGE_ICON, 32, 32, 0));
		}else
		if (strIcon == "warn")
		{
			pItemIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
				MAKEINTRESOURCE(IDI_ICON_WARN), IMAGE_ICON, 32, 32, 0));
		}else
		if (strIcon == "error")
		{
			pItemIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
				MAKEINTRESOURCE(IDI_ICON_ERROR), IMAGE_ICON, 32, 32, 0));
		}else
		if (strIcon == "plat")
		{
			pItemIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
				MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 32, 32, 0));
		}else
		{
			pItemIcon->SetIcon(hIcon);
		}
		pItemIcon->SetButton(FALSE);

		// close icon.
		CXTPPopupItem* pItemCloseIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
			new CXTPPopupItem(CRect(nWidth-22, 14, nWidth-6, 30)));
		//pItemCloseIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
		//	MAKEINTRESOURCE(IDI_CLOSE), IMAGE_ICON, 16, 16, 0));
		pItemCloseIcon->SetIcons(IDB_CLOSEMSN, 0, xtpPopupItemIconNormal|xtpPopupItemIconSelected|xtpPopupItemIconPressed);
		pItemCloseIcon->SetID(ID_POPUP_CLOSE);

		// top icon.//bug:添加的项无法释放,因此移动到看不见的位置
		CXTPPopupItem* pItemTopIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
			new CXTPPopupItem(CRect(-100, -100, -100, -100)));
	}else
	if(theme == xtpPopupThemeMSN)
	{
		CXTPPopupItem* pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
			new CXTPPopupItem(CRect(25, 6, nWidth-20, 19), pInfo->strCaption));
		pItemText->SetTextAlignment(DT_LEFT);
		pItemText->SetBold(TRUE);
		pItemText->SetTextColor(RGB(0, 61, 128));
		pItemText->SetHyperLink(FALSE);

		pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
			new CXTPPopupItem(CRect(46, 32, nWidth-20, 50), pInfo->strTitle));
		pItemText->SetTextAlignment(DT_LEFT);
		//pItemText->CalculateHeight();
		//pItemText->CalculateWidth();
		pItemText->SetBold(TRUE);
		pItemText->SetTextColor(RGB(0, 61, 128));
		pItemText->SetHyperLink(FALSE);
		if(pInfo->strLinkTitle != "")
		{
			pItemText->SetID(ID_POPUP_GOTO_SITE1);
			pItemText->SetHyperLink(TRUE);
		}

		if(pInfo->nItemType == POPUP_ITEM_TEXT)
		{
			pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
				new CXTPPopupItem(CRect(46, 50, nWidth-20, nHight+60+nAddHight), pInfo->strText));
		}else
		{
			pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
				new CXTPPopupItem(CRect(46, 50, nWidth-20, nHight+60+nAddHight)));
		}
		pItemText->SetTextAlignment(DT_LEFT|DT_WORDBREAK);
		pItemText->SetTextColor(RGB(0, 61, 128));
		pItemText->SetHyperLink(FALSE);
		if(pInfo->strLinkText != "")
		{
			pItemText->SetID(ID_POPUP_GOTO_SITE2);
			pItemText->SetHyperLink(TRUE);
		}
		if(pInfo->nItemType == POPUP_ITEM_RTF)
		{
			pItemText->SetRTFText(pInfo->strText);
		}else
		if(pInfo->nItemType == POPUP_ITEM_MARKUP)
		{
			pItemText->SetMarkupText(pInfo->strText);
		}
		pItemText->FitToContent();

		szTextItem = CSize(pItemText->GetRect().Size());

		if((pInfo->nItemType == POPUP_ITEM_MARKUP) && (pInfo->nWidth == 0))
		{
			nWidth = szTextItem.cx + 70;
		}

		// left icon
		CXTPPopupItem* pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
			new CXTPPopupItem(CRect(7, 30, 19, 44)));
		if (strIcon == "info")
		{
			pItemIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
				MAKEINTRESOURCE(IDI_ICON_INFO), IMAGE_ICON, 32, 32, 0));
		}else
		if (strIcon == "warn")
		{
			pItemIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
				MAKEINTRESOURCE(IDI_ICON_WARN), IMAGE_ICON, 32, 32, 0));
		}else
		if (strIcon == "error")
		{
			pItemIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
				MAKEINTRESOURCE(IDI_ICON_ERROR), IMAGE_ICON, 32, 32, 0));
		}else
		if (strIcon == "plat")
		{
			pItemIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
				MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 32, 32, 0));
		}
		else
		{
			pItemIcon->SetIcon(hIcon);
		}
		pItemIcon->SetButton(FALSE);

		// close icon
		CXTPPopupItem* pItemCloseIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
			new CXTPPopupItem(CRect(nWidth-19, 6, nWidth-6, 19)));
		pItemCloseIcon->SetIcons(IDB_CLOSEMSN, 0, xtpPopupItemIconNormal|xtpPopupItemIconSelected|xtpPopupItemIconPressed);
		pItemCloseIcon->SetID(ID_POPUP_CLOSE);

		// top icon.
		CXTPPopupItem* pItemTopIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
			new CXTPPopupItem(CRect(7, 6, 20, 19)));
		pItemTopIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
			MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16, 0));
		pItemTopIcon->SetButton(FALSE);
	}

	// 按钮
	if(pInfo->asButtonText.GetSize() > 0)
	{
		int nButtonWidth = 70;
		//int nButtonH = nHight + 30 + nAddHight;
		int nButtonH = szTextItem.cy + 65;
		int nButtonL = (nWidth - (pInfo->asButtonText.GetSize() * nButtonWidth)) / 2 + 3;

		for(int i=0; i<pInfo->asButtonText.GetSize(); i++)
		{
			CXTPPopupItem* pItemButton = (CXTPPopupItem*)m_wndPopupControl.AddItem(
				new CXTPPopupItem(CRect(nButtonL, nButtonH, nButtonL+nButtonWidth-2, nButtonH+18), pInfo->asButtonText[i]));
			pItemButton->SetTextAlignment(DT_CENTER|DT_VCENTER|DT_WORDBREAK);
			pItemButton->SetTextColor(RGB(0xA0, 0, 0));
			pItemButton->SetID(ID_POPUP_BUTTON_CLICK + i);
			pItemButton->SetHyperLink(FALSE);
			pItemButton->SetButton(TRUE);

			nButtonL += nButtonWidth;
		}
	}

	m_wndPopupControl.SetTheme(theme);

	m_wndPopupControl.GetPaintManager()->RefreshMetrics();

	DWORD dwShowDelay = 3000;
	DWORD dwAnimateDelay = 500;
	BYTE nTransparency = 230;

	if(theme == xtpPopupThemeOffice2003 || theme == xtpPopupThemeOffice2003)
	{
		dwShowDelay = 3000;
		dwAnimateDelay = 500;
		nTransparency = 230;
	}else
	if(theme == xtpPopupThemeMSN)
	{
		dwShowDelay = 3000;
		dwAnimateDelay = 350;
		nTransparency = 245;
	}

	if(pInfo->nShowDelay != 0)
	{
		dwShowDelay = pInfo->nShowDelay;
	}
	if(pInfo->nTransparency != 0)
	{
		nTransparency = pInfo->nTransparency;
	}

	if(pInfo->nHight != 0)
	{
		szPopup = CSize(nWidth, pInfo->nHight+nAddHight);
	}else
	{
		//szPopup = CSize(nWidth, nHight+70+nAddHight);
		szPopup = CSize(nWidth, szTextItem.cy + ((pInfo->asButtonText.GetSize() > 0) ? 100 : 75));
	}

	///////////////////////////////////////////////////////////////////////////
	// 加载背景图片
	CXTPImageManagerIcon* pImage = NULL;
	if(pInfo->strBackgroundImage != "")
	{
		BOOL bAlphaBitmap = FALSE;
		HBITMAP hBitmap = CXTPImageManagerIcon::LoadBitmapFromFile(pInfo->strBackgroundImage, &bAlphaBitmap);

		if (hBitmap)
		{
			CBitmap bmp;
			bmp.Attach(hBitmap);
			UINT nCommands[] = {100};
			m_wndPopupControl.GetImageManager()->SetIcons(bmp, nCommands, 1, 0, xtpImageNormal, bAlphaBitmap);
			m_wndPopupControl.SetBackgroundBitmap(nCommands[0]);

			BITMAP bmpInfo;
			::GetObject(hBitmap, sizeof(BITMAP), &bmpInfo);
			szPopup = CSize(bmpInfo.bmHeight,bmpInfo.bmHeight);
		}
		pImage = m_wndPopupControl.GetImageManager()->GetImage(0, 0);
		//CMDTARGET_ADDREF(pImage);
	}else
	{
		m_wndPopupControl.SetBackgroundBitmap(0);
	}

	m_wndPopupControl.SetPopupAnimation((XTPPopupAnimation)(pInfo->nAnimation));
	m_wndPopupControl.SetShowDelay(dwShowDelay);
	m_wndPopupControl.SetAnimateDelay(dwAnimateDelay);
	m_wndPopupControl.SetTransparency(nTransparency);
	m_wndPopupControl.SetPopupSize(szPopup);
	m_wndPopupControl.AllowMove(TRUE);
	m_wndPopupControl.Show(AfxGetMainWnd());

	//CMDTARGET_RELEASE(pImage);
}

/////////////////////////////////////////////////////////////////////////////
// 获取弹出窗口状态
/////////////////////////////////////////////////////////////////////////////
BOOL CBlueUIApp::RunPopupControlLink(CString strLink)
{
	if(strLink.Find("link:") == 0)
	{
		strLink.Delete(0, 5);
		::ShellExecute(NULL, _T("open"), strLink, NULL, NULL, SW_SHOW);
		return TRUE;
	}else
	if(strLink.Find("html:") == 0)
	{
		strLink.Delete(0, 5);
		OpenHtmlWindow(strLink, TRUE, FALSE);
		return TRUE;
	}

	IInterp* pInterp = (IInterp*)(m_infoPopupControl.pInterp);
	if(pInterp == NULL)
	{
		return FALSE;
	}

	BOOL bCreateInterp = FALSE;
	if(strLink.Find("org.interp.") == 0)
	{
		int nPos = strLink.Find(":");
		CString strInterpId = strLink.Left(nPos);
		strLink.Delete(0, nPos+1);
		pInterp = (IInterp*)(CreateVciObject(strInterpId, ""));
		bCreateInterp = TRUE;
	}

	BOOL bRet = TRUE;
	int nPos = strLink.Find("<file>");
	if(nPos == 0)
	{
		strLink.Delete(0, 6);
		ProcessPath(strLink);

		// 保存并设置当前路径
		SaveAndSetCurPath(strLink);

		bRet = pInterp->RunScriptFile(strLink);

		// 恢复当前路径
		RestoreCurPath();

	}else
	{
		bRet =  pInterp->RunScriptCommand(strLink);
	}

	if(bCreateInterp)
	{
		ReleaseObject(pInterp);
	}

	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// 获取弹出窗口状态
/////////////////////////////////////////////////////////////////////////////
BOOL CBlueUIApp::GetPopupControlState()
{
	return (xtpPopupStateClosed != m_wndPopupControl.GetPopupState());
}

/////////////////////////////////////////////////////////////////////////////
// 关闭弹出窗口
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::ClosePopupControl()
{
	m_wndPopupControl.Close();
}

/////////////////////////////////////////////////////////////////////////////
// 显示About对话框
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::ShowAboutBox(CString strPluginId)
{
	for(int i=0; i<m_aPlugIns.GetSize(); i++)
	{
		if(strPluginId == m_aPlugIns[i].m_strId)
		{
			m_aPlugIns[i].ShowAboutBox();
			return;
		}
	}

	for(int i=0; i<m_aTools.GetSize(); i++)
	{
		if(strPluginId == m_aTools[i].m_strId)
		{
			m_aTools[i].ShowAboutBox();
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 根据ChildFrame指针获取对应的OWM插件对象和文档指针
/////////////////////////////////////////////////////////////////////////////
CDocument* CBlueUIApp::GetOwmDocumentFromFrame(CWnd* pFrame, CPlugIn*& pPlugIn)
{
	for(int i=theApp.m_aPlugIns.GetSize()-1; i>=0; i--)
	{
		if(theApp.m_aPlugIns[i].IsVciOwm() && theApp.m_aPlugIns[i].m_pDocTemplate)
		{
			POSITION posdoc = theApp.m_aPlugIns[i].m_pDocTemplate->GetFirstDocPosition();
			while(posdoc)
			{
				CDocument* pDoc = theApp.m_aPlugIns[i].m_pDocTemplate->GetNextDoc(posdoc);
				if(pDoc)
				{
					CString strPathName = pDoc->GetPathName();
					POSITION posview = pDoc->GetFirstViewPosition();
					if(posview)
					{
						CView* pView = pDoc->GetNextView(posview);
						if(pView && (pView->GetParent() == pFrame))
						{
							pPlugIn = &(theApp.m_aPlugIns[i]);
							return pDoc;
						}
					}
				}
			}
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 加载会话
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnFileLoadsession() 
{
	CString strFilter = "Session files(*.session)|*.session|All files(*.*)|*.*||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,	strFilter);
	if (dlg.DoModal() == IDOK)
	{
		CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, m_pMainWnd);
		pMainFrame->LoadSessionFile(dlg.GetPathName());
	}
}

void CBlueUIApp::OnUpdateFileLoadsession(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 保存会话
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnFileSavesession() 
{
	CString strFilter = "Session files(*.session)|*.session|All files(*.*)|*.*||";
	CFileDialog dlg(FALSE, "session", NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, strFilter);
	if (dlg.DoModal() == IDOK)
	{
		CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, m_pMainWnd);
		pMainFrame->SaveSessionFile(dlg.GetPathName());
	}
}

void CBlueUIApp::OnUpdateFileSavesession(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 打印页面设置
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnFilePrintPagesetup() 
{
	CVciPropertyPageArray aPropertyPage;

	CTreePropertySheet tps(150);
	PLAT_LOADSTRING(strTitle, IDS_TITLE_TREEPROPSHEET_PRINTPAGESETUP); // 打印页面设置
	tps.m_strWinTitle = strTitle;

	// 找到当前视图对应的插件对象
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, m_pMainWnd);
	CPlugIn* pPlugIn = NULL;
	GetOwmDocumentFromFrame(pMainFrame->MDIGetActive(), pPlugIn);
	if(pPlugIn == NULL)
	{
		return;
	}
	IVciOwm* pIVciOwm = pPlugIn->GetIVciOwm();
	if(pIVciOwm == NULL)
	{
		return;
	}

	// 获取对话框列表
	COWMOptionArray aOWMOptions;
	pIVciOwm->RegisterPrintPageOption(aOWMOptions);
	if(aOWMOptions.GetSize() == 0)
	{
		return;
	}

	for(int i=0; i<aOWMOptions.GetSize(); i++)
	{
		CString strNode = aOWMOptions[i].strNode;
		CString strDesc = aOWMOptions[i].strDesc;
		CDialog* pDialog = (CDialog*)(aOWMOptions[i].pOptionPage);
		tps.AddPage(strNode,tps_item_node,-1,strDesc,pDialog,OPT_NODE,TRUE);
	}

	tps.SetLastPageType(tps_item_endbranch);

	tps.SetFancy(FALSE);

	// 执行属性设置对话框
	tps.DoModal();

	// 释放资源,防止因下面的操作而引起析构函数出错
	tps.RemoveResource();
}

void CBlueUIApp::OnUpdateFilePrintPagesetup(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}
