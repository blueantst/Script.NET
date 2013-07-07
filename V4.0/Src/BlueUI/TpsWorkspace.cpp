// TpsWorkspace.cpp : implementation file
//

#include "stdafx.h"
#include "blueui.h"
#include "TpsWorkspace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsWorkspace dialog


CTpsWorkspace::CTpsWorkspace(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsWorkspace::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsWorkspace)
	m_bAutoReloadDocument = theApp.GetProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_AUTO_RELOAD_DOCUMENT, TRUE);
	m_bAutoOpenStartPage = theApp.GetProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_AUTO_OPEN_STARTPAGE, TRUE);
	m_bAddShellContextMenu = FALSE;
	//}}AFX_DATA_INIT
	// 查找注册表,判断是否已经注册了Windows Shell右键菜单
	HKEY hKey;
	if(RegOpenKey(HKEY_CLASSES_ROOT, _T("*\\shellex\\ContextMenuHandlers\\")+theApp.m_xmlPlat.GetNodeText("application\\appName"), &hKey) == ERROR_SUCCESS)
	{
		m_bAddShellContextMenu = TRUE;
	}
	m_bShellContextMenuExist = m_bAddShellContextMenu;
}

void CTpsWorkspace::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsWorkspace)
	DDX_Check(pDX, IDC_CHECK_RELOAD_DOCUMENT, m_bAutoReloadDocument);
	DDX_Check(pDX, IDC_CHECK_AUTO_OPEN_STARTPAGE, m_bAutoOpenStartPage);
	DDX_Check(pDX, IDC_CHECK_ADD_SHELL_CONTEXT_MENU, m_bAddShellContextMenu);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsWorkspace, CDialog)
	//{{AFX_MSG_MAP(CTpsWorkspace)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsWorkspace message handlers

void CTpsWorkspace::OnOK() 
{
	UpdateData(TRUE);

	theApp.WriteProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_AUTO_RELOAD_DOCUMENT, m_bAutoReloadDocument);
	theApp.WriteProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_AUTO_OPEN_STARTPAGE, m_bAutoOpenStartPage);

	if(m_bShellContextMenuExist != m_bAddShellContextMenu)
	{
		// 注册右键菜单选项被修改
	#ifdef _DEBUG
		HINSTANCE hLib = ::LoadLibrary(theApp.GetPlatRootPath() + "Bind\\BlueCMd.dll");
	#else
		HINSTANCE hLib = ::LoadLibrary(theApp.GetPlatRootPath() + "Bin\\BlueCM.dll");
	#endif
		if (hLib) 
		{
			HRESULT hResult = E_FAIL;
			typedef UINT (*LPF_DLLREGFUNC)();
			if(m_bAddShellContextMenu)
			{
				LPF_DLLREGFUNC lpfnDllRegisterServer = (LPF_DLLREGFUNC)::GetProcAddress(hLib, "DllRegisterServer");
				hResult = lpfnDllRegisterServer();
			}else
			{
				LPF_DLLREGFUNC lpfnDllUnregisterServer = (LPF_DLLREGFUNC)::GetProcAddress(hLib, "DllUnregisterServer");
				hResult = lpfnDllUnregisterServer();
			}
			if(hResult != S_OK)
			{
				AfxMessageBox("Register/Unregister windows shell context menu failed!");
			}
			::FreeLibrary(hLib);
		}
	}

	CDialog::OnOK();
}
