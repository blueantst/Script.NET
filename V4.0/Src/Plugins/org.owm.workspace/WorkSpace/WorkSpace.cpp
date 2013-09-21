// WorkSpace.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "WorkSpace.h"
#include "ChildFrame.h"
#include "WorkSpaceView.h"
#include "WorkSpaceDoc.h"
//#include "WorkSpaceBar.h"

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

static CString g_strOWMDescription(_T(""));

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
// CWorkSpaceApp

BEGIN_MESSAGE_MAP(CWorkSpaceApp, CWinApp)
	//{{AFX_MSG_MAP(CWorkSpaceApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceApp construction

CWorkSpaceApp::CWorkSpaceApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bDebug	= FALSE;
	m_pDocTemplate = NULL;
	m_pIPlatUI = NULL;
	m_nLastTreePaneId = 0;
	m_nLastTreeType = PGI_FILE;
	m_strLastBmpFile = "";
}
/*
/////////////////////////////////////////////////////////////////////////////
// 读INI
/////////////////////////////////////////////////////////////////////////////
CString CWorkSpaceApp::GetIniString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault)
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
// 读INI
/////////////////////////////////////////////////////////////////////////////
int CWorkSpaceApp::GetIniInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault)
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
// 写INI
/////////////////////////////////////////////////////////////////////////////
CString CWorkSpaceApp::WriteIniString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue)
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
// 写INI
/////////////////////////////////////////////////////////////////////////////
int CWorkSpaceApp::WriteIniInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue)
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
*/
/////////////////////////////////////////////////////////////////////////////
// The one and only CWorkSpaceApp object

CWorkSpaceApp theApp;

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
	ol.nOWMType				= OWM_DOC | /*OWM_MENU | OWM_TOOLBAR |*/ OWM_OPTION | OWM_TABCTRLBAR | OWM_AUTOCREATE | OWM_ONEFRAME | OWM_BROADCAST | OWM_TCL;
	ol.nOWMID				= 110;
	ol.pDocClass			= RUNTIME_CLASS(CWorkSpaceDoc);
	ol.pFrameClass			= RUNTIME_CLASS(CChildFrame);
	ol.pViewClass			= RUNTIME_CLASS(CWorkSpaceView);
	ol.pCtrlBarClass		= NULL;
	ol.SetOWM				= NULL;
	ol.InitOWM				= InitOWM;
	ol.DoneOWM				= DoneOWM;
	ol.RegisterTclCmd		= NULL;//Tcl_RegisterCmd;;
	ol.UnregisterTclCmd		= NULL;//Tcl_UnregisterCmd;
	ol.RegisterTclExKeyword	= NULL;//Tcl_RegisterExKeyword;
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
	ov.svOWMName	= "WorkSpace";

	LOADRCSTRING(g_strOWMDescription,IDS_OWM_WS)
	ov.svDescription = g_strOWMDescription.GetBuffer(g_strOWMDescription.GetLength());
       
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
// 取第一个CWorkSpaceView
/////////////////////////////////////////////////////////////////////////////
CWorkSpaceView* GetFirstWorkSpaceView()
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
			return (CWorkSpaceView*)(pDoc->GetNextView(posview));
		}
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// OWM命令帮助
/////////////////////////////////////////////////////////////////////////////
int OWMCmdHelp(int nCmd, CStringArray* aHelpStr)
{
	CString strTemp;
	switch(nCmd)
	{
	case OC_CMDALL:
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 向平台查询OWM安装信息
/////////////////////////////////////////////////////////////////////////////
static OWM_INFO OwmInfo;
OWM_INFO* GetOwmInfo(int nId)
{
	if(!theApp.m_ol.CommandPlat)
		return NULL;

	COWMArray* pOwmArr = (COWMArray*)(theApp.m_ol.CommandPlat(OC_QUERYOWMID, nId, 0));
	if(pOwmArr)
	{
		OwmInfo.ov	= ((*pOwmArr)[0]).ov;
		OwmInfo.ol	= ((*pOwmArr)[0]).ol;
		OwmInfo.ins	= ((*pOwmArr)[0]).ins;
		OwmInfo.pBar= ((*pOwmArr)[0]).pBar;
		delete pOwmArr;
		return &OwmInfo;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 根据ID获取某个树窗口
/////////////////////////////////////////////////////////////////////////////
CWorkSpaceView* GetWorkSpaceView(int nId)
{
	if(theApp.m_pIPlatUI == NULL)
	{
		return NULL;
	}

	CWnd* pWnd = theApp.m_pIPlatUI->GetDockingPaneWnd(nId);
	if(!pWnd)
	{
		// 如果窗口未创建，就先创建
		theApp.m_pIPlatUI->ActiveDockingPane(nId);
		theApp.m_nLastTreePaneId = nId;
		theApp.m_strLastBmpFile = "";
		// 因为此时窗口还不能用,因此依然返回空
		return NULL;
	}

	return (CWorkSpaceView*)(pWnd->GetDescendantWindow(AFX_IDW_PANE_FIRST));
}

// 树窗口的起始ID
static int g_nTreePaneId = 19000;

/////////////////////////////////////////////////////////////////////////////
// OWM命令处理函数
/////////////////////////////////////////////////////////////////////////////
int CommandOWM(int nCmd, WPARAM wParam, LPARAM lParam)
{
	switch(nCmd)
	{
	case OC_SETOWMLINK:	// 设置OWM链接
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
			CWorkSpaceView* pView = GetFirstWorkSpaceView();
			if(!pView)
				return FALSE;
			CWnd* pFrame = pView->GetParent();
			if(!pFrame)
				return FALSE;
			pFrame->SendMessage(WM_CLOSE, 0, 0);
			return TRUE;
		}

	case OC_GOTOITEM:	// 转到指定的项目
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM item;
				item.dwMask	= TIM_ID;
				item.nID	= lParam;
				pView->EnsureItem(&item);
				return TRUE;
			}
			return FALSE;
		}

	// 树功能
	case OC_WS_ADDTREE:	// 添加树窗口
		{
			if(theApp.m_pIPlatUI)
			{
				theApp.m_nLastTreeType = lParam;
				int nID = g_nTreePaneId++;
				theApp.m_pIPlatUI->CreateDockingPane("org.owm.workspace",
					(LPCTSTR)wParam, nID, xtpPaneDockLeft, PANE_ATTACH_VIEW, NULL, 0);
				return nID;
			}
			return 0;
		}

	case OC_WS_ADDTREEEX:	// 添加树窗口(扩展方式,使用结构传递信息)
		{
			if(theApp.m_pIPlatUI)
			{
				STRU_ADDTREEEX* pStruTree = (STRU_ADDTREEEX*)wParam;
				theApp.m_nLastTreeType = pStruTree->nTreeType;
				int nID = 0;
				if(pStruTree->nPaneId == 0)
				{
					nID = g_nTreePaneId++;
				}else
				{
					nID = pStruTree->nPaneId;
				}

				theApp.m_strLastBmpFile = pStruTree->strBmpFile;

				HICON hIcon = NULL;
				if(pStruTree->strIconFile != "")
				{
					WORD wIndex = 0;
					hIcon = ::ExtractAssociatedIcon(AfxGetApp()->m_hInstance,
								pStruTree->strIconFile.GetBuffer(0), &wIndex);
					pStruTree->strIconFile.ReleaseBuffer();
				}else
				{
					//get_dll_resource();
					//hIcon = AfxGetApp()->LoadIcon(IDI_ICON_WORKSPACE);
					//reset_dll_resource();
				}

				theApp.m_pIPlatUI->CreateDockingPane("org.owm.workspace",
					pStruTree->strTreeName, nID, xtpPaneDockLeft, PANE_ATTACH_VIEW,
					hIcon, pStruTree->nAttachPaneId);
				return nID;
			}
			return 0;
		}

	case OC_WS_CLOSETREE:	// 关闭树窗口
		{
			if(theApp.m_pIPlatUI)
			{
				return theApp.m_pIPlatUI->CloseDockingPane(wParam);
			}
			return 0;
		}

	case OC_WS_GETTREEINDEX:	// 根据名字得到树ID
		{
			if(theApp.m_pIPlatUI)
			{
				return theApp.m_pIPlatUI->GetDockingPaneIdByName((LPCTSTR)wParam);
			}
			return -1;
		}

	case OC_WS_SETACTIVETREE:	// 设置活动树
		{
			if(theApp.m_pIPlatUI)
			{
				return theApp.m_pIPlatUI->ActiveDockingPane(wParam);
			}
			return 0;
		}

	case OC_WS_ACTIVECB:	// 安装窗口激活的回调函数
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				pView->InstallVCCallBack((TYPEOF_ViewChange*)lParam);
				return wParam;
			}
			return -1;
		}

	case OC_WS_TREEEXIST:	// 判断指定树窗口是否已经存在
		{
			if(theApp.m_pIPlatUI)
			{
				CWnd* pWnd = theApp.m_pIPlatUI->GetDockingPaneWnd(wParam);
				if(pWnd)
				{
					return TRUE;
				}
			}
			return FALSE;
		}

	case OC_WS_SETTREETYPE:	// 设置窗口类型
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				pView->SetTreeType(lParam);
				return TRUE;
			}
			return FALSE;
		}

	case OC_WS_SETTREEIMAGE:	// 设置树的图标组
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				pView->SetTreeImageList((LPCTSTR)lParam);
				return TRUE;
			}
			return FALSE;
		}

	case OC_WS_ADDMENU:	// 添加菜单
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				CMenu* pMenu = (CMenu*)lParam;
				pView->AddTreeMenu(pMenu);
				return wParam;
			}
			return -1;
		}

	case OC_WS_ADDMENUITEM:	// 添加菜单项
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKMENUITEM* pItem = (HWORKMENUITEM*)lParam;
				pView->AddMenuItem(pItem);
				return wParam;
			}
			return -1;
		}

	case OC_WS_ADDTREEITEM:	// 添加树节点
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				HTREEITEM hItem = pView->AddTreeItem(pItem);
				return (int)hItem;
			}
			return NULL;
		}

	case OC_WS_DELTREEITEM:	// 删除节点
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				BOOL bOK = pView->DelTreeItem(pItem);
				return bOK;
			}
			return FALSE;
		}

	case OC_WS_SETITEMTYPE:	// 设置节点类型
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				pView->SetItemType(pItem);
				return wParam;
			}
			return -1;
		}

	case OC_WS_GETITEMTYPE:	// 取得节点类型
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				BYTE byType = pView->GetItemType(pItem);
				return byType;
			}
			return -1;
		}

	case OC_WS_SETITEMSTATE:	// 设置节点状态
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				BYTE byStateOld = pView->GetItemState1(pItem);
				if(byStateOld != WTS_DISABLE)
				{
					pView->SetItemState1(pItem);
				}
				return wParam;
			}
			return -1;
		}

	case OC_WS_GETITEMSTATE:	// 取得节点状态
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				BYTE byState = pView->GetItemState1(pItem);
				return byState;
			}
			return -1;
		}

	case OC_WS_SETITEMMENU:	// 设置节点菜单
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				pView->SetItemMenu(pItem);
				return wParam;
			}
			return -1;
		}

	case OC_WS_GETITEMMENU:	// 取得节点菜单
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				CMenu* pMenu = pView->GetItemMenu(pItem);
				return (int)pMenu;
			}
			return NULL;
		}

	case OC_WS_SETITEMOBJECT:	// 设置节点对象
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				pView->SetItemObject(pItem);
				return wParam;
			}
			return -1;
		}

	case OC_WS_GETITEMOBJECT:	// 取得节点对象
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				CObject* pObject = pView->GetItemObject(pItem);
				return (int)pObject;
			}
			return NULL;
		}

	case OC_WS_SETITEMID:	// 设置节点ID
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				pView->SetItemID(pItem);
				return wParam;
			}
			return -1;
		}

	case OC_WS_GETITEMID:	// 取得节点ID
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				int nID = pView->GetItemID(pItem);
				return (int)nID;
			}
			return NULL;
		}

	case OC_WS_SETITEMNAME:	// 设置节点名
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				pView->SetItemName(pItem);
				return wParam;
			}
			return -1;
		}

	case OC_WS_GETITEMNAME:	// 取得节点名
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				CString strName = pView->GetItemName(pItem);
				return (int)(LPCTSTR)strName;
			}
			return NULL;
		}

	case OC_WS_SETITEMIMAGE:	// 设置节点图标
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				pView->SetItemImage(pItem);
				return wParam;
			}
			return -1;
		}

	case OC_WS_GETITEMIMAGE:	// 取得节点图标
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				int nImage = pView->GetItemImage(pItem);
				return nImage;
			}
			return NULL;
		}

	case OC_WS_EXPANDITEM:	// 展开节点
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				pView->ExpandItem(pItem);
				return wParam;
			}
			return -1;
		}

	case OC_WS_COLLAPSEITEM:	// 收缩节点
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				pView->CollapseItem(pItem);
				return wParam;
			}
			return -1;
		}

	case OC_WS_SETDCLICKSCRIPT:	// 设置树缺省的节点双击脚本
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				pView->SetDClickScript((LPCTSTR)lParam);
				return 1;
			}
			return 0;
		}

	case OC_WS_SETITEMINFO:	// 设置节点信息
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				pView->SetItemInfo(pItem);
				return 1;
			}
			return 0;
		}

	case OC_WS_GETITEMINFO:	// 获取节点信息
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				pView->GetItemInfo(pItem);
				return 1;
			}
			return 0;
		}

	case OC_WS_SEARCHITEM:	// 查找节点
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				return (int)(pView->SearchItem(pItem));
			}
			return NULL;
		}

	case OC_WS_GETCHILDITEM:	// 获取第一个子节点
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				return (int)(pView->GetChildItem(pItem));
			}
			return 0;
		}

	case OC_WS_GETNEXTITEM:	// 获取下一个节点
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				return (int)(pView->GetNextItem(pItem));
			}
			return 0;
		}

	case OC_WS_SORTCHILDREN:	// 对子节点排序
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTREEITEM* pItem = (HWORKTREEITEM*)lParam;
				return pView->SortChildren(pItem);
			}
			return 0;
		}

	case OC_WS_SETTASKTHEME:	// 设置TaskPanel的Theme
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				pView->SetTaskPanelTheme((XTPTaskPanelPaintTheme)lParam);
				return TRUE;
			}
			return FALSE;
		}

	case OC_WS_ADDTASKITEM:	// 添加一个TaskItem
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTASKITEM* pItem = (HWORKTASKITEM*)lParam;
				CXTPTaskPanelGroupItem* pTaskItem = pView->AddTaskItem(pItem);
				int nID = pView->GetTaskItemIDByItem(pTaskItem);
				return nID;
			}
			return 0;
		}

	case OC_WS_DELTASKITEM:	// 删除指定TaskItems
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				HWORKTASKITEM* pItem = (HWORKTASKITEM*)lParam;
				pView->DelTaskItem(pItem);
				return TRUE;
			}
			return FALSE;
		}

	case OC_WS_SETTASKICON:	// 设置TaskPanel的图标或图标组
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				int nImageIndex = pView->SetTaskPanelIcon((LPCTSTR)lParam);
				return nImageIndex;
			}
			return 0;
		}

	case OC_WS_CREATTASKMENU:	// 从指定菜单创建TaskPanel
		{
			CWorkSpaceView* pView = GetWorkSpaceView(wParam);
			if(pView)
			{
				pView->CreateTaskMenu((CMenu*)lParam);
				return TRUE;
			}
			return FALSE;
		}

	case OC_WS_SETPAGEMENU:	// 设置页面菜单
		{
			return FALSE;
		}

	default:
		break;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 显示导航树
/////////////////////////////////////////////////////////////////////////////
void OnViewNavigater() 
{
	if(theApp.m_pIPlatUI)
	{
		theApp.m_pIPlatUI->CreateDockingPane("org.owm.workspace", TREE_ID_NAVIGATER);
		theApp.m_nLastTreeType = PGI_FILE;
		theApp.m_nLastTreePaneId = TREE_ID_NAVIGATER;
		theApp.m_strLastBmpFile = "";
	}
}

void OnUpdateViewNavigater(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 显示文件树
/////////////////////////////////////////////////////////////////////////////
void OnViewFileTree() 
{
	if(theApp.m_pIPlatUI)
	{
		theApp.m_pIPlatUI->CreateDockingPane("org.owm.workspace", TREE_ID_FILE);
		theApp.m_nLastTreeType = PGI_FILE;
		theApp.m_nLastTreePaneId = TREE_ID_FILE;
		theApp.m_strLastBmpFile = "";
	}
}

void OnUpdateViewFileTree(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 注册OWM扩展菜单
/////////////////////////////////////////////////////////////////////////////
int RegisterClientMenu(CMenu* pMenu, COWMMenuArray& aOWMMenus, int nOWMIdStart, CDocument* pDoc)
{
	DLL_LOADSTRING(strViewMenu, IDS_VIEW_MENU);
	//PLATUI_ADDMENUITEM(strViewMenu, IDR_OWM_VIEW, 0, OnViewNavigater, OnUpdateViewNavigater);
	PLATUI_ADDMENUITEM(strViewMenu, IDR_OWM_VIEW, 1, OnViewFileTree, OnUpdateViewFileTree);
	/*
	// 初始化OWM菜单,并添加到主菜单
	get_dll_resource();
	CMenu* pmenu = new CMenu();
	pmenu->LoadMenu(IDR_OWM);
	CMenu* psubmenu = (CMenu*)(pmenu->GetSubMenu(0));
	reset_dll_resource();

	// 检查菜单是否已经存在,如果存在则删除
	int count = pMenu->GetMenuItemCount();
	OWM_CHECKMENUISEXIST(count, _T("WorkSpace"))

	// 在主菜单最后添加扩展菜单
	pMenu->InsertMenu(count, MF_POPUP|MF_STRING|MF_ENABLED, 
				(UINT)psubmenu->GetSafeHmenu(), _T("WorkSpace"));

	// 在此添加扩展菜单
	OWM_REGISTERMENU(ID_OWM_ABOUT, AboutOWM, NULL, USER_NONE)

	// 在DOC中记载扩展菜单指针,文档关闭时清除
	if(pDoc)
	{
		((CWorkSpaceDoc*)pDoc)->m_pMenu = pmenu;
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
// 显示/隐藏OWM ControlBar
/////////////////////////////////////////////////////////////////////////////
int OpenOwmBar()
{
	OWM_INFO* pOWM = GetOwmInfo(theApp.m_ol.nOWMID);
	if(pOWM && pOWM->pBar)
	{
		if(pOWM->pBar->IsWindowVisible())
			theApp.m_ol.CommandPlat(OC_CLOSEBAR, theApp.m_ol.nOWMID, 1);
		else
			theApp.m_ol.CommandPlat(OC_OPENBAR, theApp.m_ol.nOWMID, 1);
	}
	return 0;
}

void UpdateOpenOwmBar(CCmdUI* pCmdUI)
{
	int nCheck = 0;
	OWM_INFO* pOWM = GetOwmInfo(theApp.m_ol.nOWMID);
	if(pOWM && pOWM->pBar)
	{
		if(pOWM->pBar->IsWindowVisible())
			nCheck = 1;
	}
	pCmdUI->SetCheck(nCheck);
}

/////////////////////////////////////////////////////////////////////////////
// 注册OWM ToolBar
/////////////////////////////////////////////////////////////////////////////
int RegisterClientToolBar(CWnd* pBar, COWMMenuArray& aOWMMenus, int nOWMIdStart, int nIndex)
{
	/*
	CToolBar* pToolBar = (CToolBar*)pBar;

	CToolBarCtrl* pCtrl = &(pToolBar->GetToolBarCtrl());
	CImageList* pImage = pCtrl->GetImageList();
	int count = pImage->GetImageCount();
	CBitmap bitmap;
	get_dll_resource();
	bitmap.LoadBitmap(IDR_OWM);
	pImage->Add(&bitmap, RGB(0, 128, 128));
	reset_dll_resource();

	// 在此添加ToolBar
	OWM_REGISTERTOOLBAR(count, OpenOwmBar, UpdateOpenOwmBar, USER_NONE)
*/
	return aOWMMenus.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
// 卸载OWM ToolBar
/////////////////////////////////////////////////////////////////////////////
int UnregisterClientToolBar(CWnd* pBar, COWMMenuArray& aOWMMenus, int nOWMIdStart)
{
	/*
	CToolBar* pToolBar = (CToolBar*)pBar;

	// 在此注消ToolBar
	OWM_UNREGISTERTOOLBAR(OpenOwmBar)
*/
	return aOWMMenus.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
// 注册OWM属性页
/////////////////////////////////////////////////////////////////////////////
int RegisterOption(COWMOptionArray& aOWMOption)
{
	// 在此添加属性页
	//OWM_REGISTEROPTION("", "WorkSpace", "WorkSpace OWM.", IDD_OWM, CDialog, 0)

	return aOWMOption.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
// 初始化OWM
/////////////////////////////////////////////////////////////////////////////
int InitOWM()
{
//	theApp.m_pIPlatUI->ActiveDockingPane(TREE_ID_FILE);
//	theApp.m_nLastTreePaneId = TREE_ID_FILE;
//	theApp.m_pIPlatUI->ActiveDockingPane(TREE_ID_NAVIGATER);
//	theApp.m_nLastTreePaneId = TREE_ID_NAVIGATER;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 卸载OWM
/////////////////////////////////////////////////////////////////////////////
int DoneOWM()
{

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
	"owm.blueui.workspace",\
	"OWM WorkSpace",\
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
	//theApp.m_ol.pDocTemplate = pDocTemplate;
	theApp.m_pDocTemplate = pDocTemplate;
	return TRUE;
}

// 获取文档模板指针
int __stdcall CVciOwm::XVciOwm::GetDocTemplate(CMultiDocTemplate** ppDocTemplate)
{
	//*ppDocTemplate = theApp.m_ol.pDocTemplate;
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
	get_dll_resource();
	CWnd* pWnd = (CWnd*)(RUNTIME_CLASS(CWorkSpaceView));
	theApp.m_nLastTreePaneId = nID;
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
	TRACE("CreateObject(\"%s\")\n", lpcsInterfaceName);

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
