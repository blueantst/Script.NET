// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "BlueUI.h"

#include "MainFrm.h"
#include "ComboBoxSearch.h"
#include "InetTools.h"
#include "SkinProgress.h"

#include "childfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlTools - 扩展工具菜单组

class CControlTools : public CXTPControlButton
{
	DECLARE_XTP_CONTROL(CControlTools)

protected:
	virtual void OnCalcDynamicSize(DWORD dwMode);
	virtual BOOL IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint point, DROPEFFECT& dropEffect);
};

IMPLEMENT_XTP_CONTROL(CControlTools, CXTPControlButton)

void CControlTools::OnCalcDynamicSize(DWORD /*dwMode*/)
{
	if (GetParent()->GetType() !=xtpBarTypePopup)
		return;

	ASSERT(m_pControls->GetAt(m_nIndex) == this);

	while (m_nIndex + 1 < m_pControls->GetCount())
	{
		CXTPControl* pControl = m_pControls->GetAt(m_nIndex + 1);
		if (pControl->GetID() >= FIRST_TOOL_COMMAND && pControl->GetID() < LAST_TOOL_COMMAND)
		{
			m_pControls->Remove(pControl);
		}
		else break;
	}

	m_dwHideFlags = 0;

	if (m_pParent->IsCustomizeMode())
		return;

	CMDIFrameWnd* pFrame = DYNAMIC_DOWNCAST(CMDIFrameWnd, AfxGetMainWnd());
	ASSERT(pFrame);

	m_dwHideFlags |= xtpHideGeneric;

	int nCount = theApp.m_aTools.GetSize();
	int nIndex = 0;
	for (int i = 0; i < nCount; i++)
	{
		if(!theApp.m_aTools[i].m_bShowMenu)
		{
			continue;
		}

		CString strTitle;
		strTitle = theApp.m_aTools[i].m_strName;
		theApp.m_aTools[i].m_nMenuIndex = nIndex;

		CXTPControl* pControl = m_pControls->Add(xtpControlButton, FIRST_TOOL_COMMAND + nIndex, _T(""), m_nIndex + 1 + i, TRUE);

		pControl->SetCaption(strTitle);
		pControl->SetFlags(xtpFlagManualUpdate);

		if (GetBeginGroup() && (i == 0))
			pControl->SetBeginGroup(TRUE);

		nIndex++;
	}
}

BOOL CControlTools::IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint /*point*/, DROPEFFECT& dropEffect)
{
	if (pCommandBar->GetType() != xtpBarTypePopup)
	{
		dropEffect = DROPEFFECT_NONE;
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CControlExports - 导出菜单组

class CControlExports : public CXTPControlButton
{
	DECLARE_XTP_CONTROL(CControlExports)

protected:
	virtual void OnCalcDynamicSize(DWORD dwMode);
	virtual BOOL IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint point, DROPEFFECT& dropEffect);
};

IMPLEMENT_XTP_CONTROL(CControlExports, CXTPControlButton)

void CControlExports::OnCalcDynamicSize(DWORD /*dwMode*/)
{
	if (GetParent()->GetType() !=xtpBarTypePopup)
		return;

	ASSERT(m_pControls->GetAt(m_nIndex) == this);

	while (m_nIndex + 1 < m_pControls->GetCount())
	{
		CXTPControl* pControl = m_pControls->GetAt(m_nIndex + 1);
		if (pControl->GetID() >= FIRST_EXPORT_COMMAND && pControl->GetID() < LAST_EXPORT_COMMAND)
		{
			m_pControls->Remove(pControl);
		}
		else break;
	}

	m_dwHideFlags = 0;

	if (m_pParent->IsCustomizeMode())
		return;

	CMDIFrameWnd* pFrame = DYNAMIC_DOWNCAST(CMDIFrameWnd, AfxGetMainWnd());
	ASSERT(pFrame);

	m_dwHideFlags |= xtpHideGeneric;

	// 获取当前文档的导出菜单列表
	CPlugIn* pPlugIn = NULL;
	theApp.GetOwmDocumentFromFrame(pFrame->MDIGetActive(), pPlugIn);
	if(pPlugIn == NULL)
	{
		return;
	}
	IVciOwm* pIVciOwm = pPlugIn->GetIVciOwm();
	if(pIVciOwm == NULL)
	{
		return;
	}

	CStringArray asExportType;
	pIVciOwm->GetExportTypeList(asExportType);
	if(asExportType.GetSize() == 0)
	{
		return;
	}

	for(int i=0; i<asExportType.GetSize(); i++)
	{
		CXTPControl* pControl = m_pControls->Add(xtpControlButton, FIRST_EXPORT_COMMAND + i, _T(""), -1, TRUE);

		pControl->SetCaption(asExportType[i]);
		pControl->SetFlags(xtpFlagManualUpdate);

		if (GetBeginGroup() && (i == 0))
			pControl->SetBeginGroup(TRUE);
	}
}

BOOL CControlExports::IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint /*point*/, DROPEFFECT& dropEffect)
{
	if (pCommandBar->GetType() != xtpBarTypePopup)
	{
		dropEffect = DROPEFFECT_NONE;
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_VIEW_OUTPUT, OnViewOutput)
	ON_COMMAND(ID_VIEW_PROPERTIES, OnViewProperties)
	ON_COMMAND(ID_FILE_NEW_BLANK, OnFileNewBlank)
	ON_COMMAND(ID_REFRESH_GOTO_URL, OnRefreshGotoUrl)
	ON_UPDATE_COMMAND_UI(ID_REFRESH_GOTO_URL, OnUpdateRefreshGotoUrl)
	ON_COMMAND(ID_WINDOW_CLOSE_ALL, OnWindowCloseAll)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_CLOSE_ALL, OnUpdateWindowCloseAll)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_MESSAGE(WM_XTP_PRETRANSLATEMOUSEMSG, OnTabbarMouseMsg)
	ON_XTP_CREATECONTROL()
    ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)

	ON_MESSAGE(XTPWM_POPUPCONTROL_NOTIFY, OnPopUpNotify)
	ON_MESSAGE(XTPWM_TASKPANEL_NOTIFY, OnTaskPanelNotify)

	ON_REGISTERED_MESSAGE( WM_CHECK_ITS_ME, OnCheckItsMe )

	ON_MESSAGE(WM_PLAT_COMCMD, OnPlatComCmd)
	ON_MESSAGE(WM_PLAT_DYNCLOSE, OnPlatOWMDynClose)

	ON_COMMAND_RANGE(IDM_OWMCMD, IDM_OWMCMD+4095, OnOwmExCmd)
	ON_UPDATE_COMMAND_UI_RANGE(IDM_OWMCMD, IDM_OWMCMD+4095, OnUpdateOwmExCmd)

	ON_XTP_EXECUTE(ID_GOTO_URL, OnGotoUrl)
	ON_UPDATE_COMMAND_UI(ID_GOTO_URL, OnUpdateGotoUrl)

	ON_COMMAND(ID_TOGGLE_DOCK, OnToggleDock)

	ON_COMMAND_RANGE(FIRST_STATUS_COMMAND, LAST_STATUS_COMMAND, OnIndicatorButton)
	ON_UPDATE_COMMAND_UI_RANGE(FIRST_STATUS_COMMAND, LAST_STATUS_COMMAND, OnUpdateIndicatorButton)

	ON_COMMAND_RANGE(FIRST_TOOL_COMMAND, LAST_TOOL_COMMAND, OnToolClick)

	ON_COMMAND_RANGE(FIRST_EXPORT_COMMAND, LAST_EXPORT_COMMAND, OnExportClick)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
};

static UINT uHideCmds[] = 
{
	ID_WINDOW_CASCADE,
	ID_FILE_PRINT,
	ID_FILE_PRINT_PREVIEW,
};

static CSkinProgress* g_pSkinProgress = NULL;	// 进度状态条对象

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// 加载皮肤
	LoadSkin();

	m_strIniFileName = theApp.GetPlatRootPath();
#ifdef _DEBUG
	m_strIniFileName += "bind\\";
#else
	m_strIniFileName += "bin\\";
#endif
	if(LANGUAGE_PAGE_CHINESE == theApp.m_curLanguage)
	{
		m_strIniFileName += "BlueUI_cn.xml";
	}else
	{
		m_strIniFileName += "BlueUI.xml";
	}

	m_bPropInUpdate = FALSE;
	m_bEnableUpdatePropertyGrid = TRUE;
	m_pComboBoxURL = NULL;
}

CMainFrame::~CMainFrame()
{
}

// 启动时候调用一次
void CMainFrame::LoadSkin()
{
#ifdef _XTP_INCLUDE_SKINFRAMEWORK
	XTPSkinManager()->GetResourceFile()->SetModuleHandle(AfxGetInstanceHandle());
#endif

	m_nSkinType = theApp.GetProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_TYPE, SKIN_OFFICE2007);

	//m_nSkinType = SKIN_OFFICE2007;

	CString strSkinFile = "";

	strSkinFile = theApp.GetProfileString(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_FILE);
	if(GetFileAttributes(strSkinFile) != 0xFFFFFFFF)
	{
		DWORD dwApplyOptions = xtpSkinApplyMenus;
		BOOL bApplyMetrics = theApp.GetProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_APPLY_METRICS, TRUE);
		dwApplyOptions |= bApplyMetrics ? xtpSkinApplyMetrics: 0;
		BOOL bApplyFrame = theApp.GetProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_APPLY_FRAMESKIN, TRUE);
		dwApplyOptions |= bApplyFrame ? xtpSkinApplyFrame: 0;
		BOOL bApplyColors = theApp.GetProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_APPLY_COLORS, TRUE);
		dwApplyOptions |= bApplyColors ? xtpSkinApplyColors: 0;
		XTPSkinManager()->SetApplyOptions(dwApplyOptions);
	}

	CString strSkinPath = theApp.GetPlatRootPath();
	strSkinPath += "\\Skins\\";

	switch (m_nSkinType)
	{
	case SKIN_OFFICE2007:
		break;
	case SKIN_CJINI:
		strSkinFile = strSkinPath + "Le5";
		XTPSkinManager()->LoadSkin(strSkinFile, _T("NormalBlue.INI"));
		break;
	case SKIN_CJSTYLE:
		strSkinFile = strSkinPath + "Office2010.cjstyles";
		XTPSkinManager()->LoadSkin(strSkinFile, "");
		break;
	case SKIN_MSSTYLE:
		strSkinFile = strSkinPath + "MsStyle\\Tiger.msstyles";
		XTPSkinManager()->LoadSkin(strSkinFile, "");
		break;
	default:
		XTPSkinManager()->LoadSkin(_T(""), _T(""));
	}

	//if (m_hWnd) OnSkinChanged();
}

void CMainFrame::RedrawFrame(CWnd* pWnd)
{
	pWnd->RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);		
}

// 更改皮肤时候调用
void CMainFrame::OnSkinChanged()
{
	if (m_nTheme == themeOffice2003)
	{
		if (XTPSkinManager()->GetResourceFile() &&
			XTPSkinManager()->GetResourceFile()->GetResourcePath().Find(_T("Office2007")) > 0)
		{
			XTPPaintManager()->SetTheme(xtpThemeOffice2007);
		}
		else
		{		
			XTPPaintManager()->SetTheme(xtpThemeOffice2003);
		}
	}
	XTPPaintManager()->RefreshMetrics();

	RedrawFrame(this);	
	
	m_wndStatusBar.Invalidate();

	CWnd* pWnd = CWnd::FromHandle(m_hWndMDIClient)->GetWindow(GW_CHILD);
	while(pWnd)
	{
		RedrawFrame(pWnd);
		
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	GetCommandBars()->GetPaintManager()->RefreshMetrics();

	GetCommandBars()->RedrawCommandBars();
}

// 根据配置文件中的主题字符串查找对一个的定义
int CMainFrame::GetThemeString(CString strTheme)
{
	int nTheme = themeVisualStudio2010;
	if(strTheme == "ThemeRibbon")
		nTheme = themeRibbon;
	else if(strTheme == "ThemeOffice2003")
		nTheme = themeOffice2003;
	else if(strTheme == "ThemeOffice2007")
		nTheme = themeOffice2007;
	else if(strTheme == "ThemeOffice2010")
		nTheme = themeOffice2010;
	else if(strTheme == "ThemeVisualStudio2005")
		nTheme = themeVisualStudio2005;
	else if(strTheme == "ThemeVisualStudio2008")
		nTheme = themeVisualStudio2008;
	else if(strTheme == "ThemeVisualStudio2010")
		nTheme = themeVisualStudio2010;
	return nTheme;
}

// 设置当前主题
void CMainFrame::SetTheme(int nTheme, int nThemeColor)
{
	m_nTheme = nTheme;
	m_nThemeColor = nThemeColor;

	// 如果主题是Office2003,需要根据资源文件决定是否换成Office2007
	if (m_nTheme == themeOffice2003)
	{
		//XTPColorManager()->m_bEnableLunaBlueForRoyaleTheme = TRUE;

		if (XTPSkinManager()->GetResourceFile() &&
			XTPSkinManager()->GetResourceFile()->GetResourcePath().Find(_T("Office2007")) > 0)
		{
			nTheme = themeOffice2007;
		}
	}

	HMODULE hModule = AfxGetInstanceHandle();
	CString strSkinPath = theApp.GetPlatRootPath();
	strSkinPath += "\\Skins\\";
	LPCTSTR lpszIniFile = 0;

	switch (nTheme)
	{
	case themeRibbon:
		XTPPaintManager()->SetTheme(xtpThemeRibbon);
		hModule = LoadLibrary(strSkinPath + _T("Office2007.dll"));
		((CXTPOffice2007Theme*)GetCommandBars()->GetPaintManager())->SetImageHandle(hModule, lpszIniFile);
		m_paneManager.SetTheme(xtpPaneThemeResource);
		XTPPaintManager()->m_bEnableAnimation = TRUE;	// 动画效果
		break;

	case themeOffice2003:
		XTPPaintManager()->SetTheme(xtpThemeOffice2003);
		m_paneManager.SetTheme(xtpPaneThemeOffice2003);
		break;

	case themeOffice2007:
		XTPPaintManager()->SetTheme(xtpThemeRibbon);
		hModule = LoadLibrary(strSkinPath + _T("Office2007.dll"));
		if(nThemeColor == themeColorBlue)
		{
			lpszIniFile = _T("OFFICE2007BLUE.INI");
		}else
		if(nThemeColor == themeColorSilver)
		{
			lpszIniFile = _T("OFFICE2007SILVER.INI");
		}else
		if(nThemeColor == themeColorBlack)
		{
			lpszIniFile = _T("OFFICE2007BLACK.INI");
		}
		((CXTPOffice2007Theme*)GetCommandBars()->GetPaintManager())->SetImageHandle(hModule, lpszIniFile);
		m_paneManager.SetTheme(xtpPaneThemeResource);
		XTPPaintManager()->m_bEnableAnimation = TRUE;	// 动画效果
		break;

	case themeOffice2010:
		XTPPaintManager()->SetTheme(xtpThemeRibbon);
		hModule = LoadLibrary(strSkinPath + _T("Office2010.dll"));
		if(nThemeColor == themeColorBlue)
		{
			lpszIniFile = _T("OFFICE2010BLUE.INI");
		}else
		if(nThemeColor == themeColorSilver)
		{
			lpszIniFile = _T("OFFICE2010SILVER.INI");
		}else
		if(nThemeColor == themeColorBlack)
		{
			lpszIniFile = _T("OFFICE2010BLACK.INI");
		}
		((CXTPOffice2007Theme*)GetCommandBars()->GetPaintManager())->SetImageHandle(hModule, lpszIniFile);
		m_paneManager.SetTheme(xtpPaneThemeResource);
		XTPPaintManager()->m_bEnableAnimation = TRUE;	// 动画效果
		break;

	case themeVisualStudio2005:
		XTPPaintManager()->SetTheme(xtpThemeWhidbey);
		m_paneManager.SetTheme(xtpPaneThemeVisualStudio2005);
		//m_wndTaskPanel.SetTheme(xtpTaskPanelThemeToolboxWhidbey);
		//m_wndProperties.m_wndPropertyGrid.SetTheme(xtpGridThemeWhidbey);
		break;

	case themeVisualStudio2008:
		XTPPaintManager()->SetTheme(xtpThemeVisualStudio2008);
		m_paneManager.SetTheme(xtpPaneThemeVisualStudio2008);
		break;

	case themeVisualStudio2010:
		XTPPaintManager()->SetTheme(xtpThemeVisualStudio2010);
		m_paneManager.SetTheme(xtpPaneThemeVisualStudio2010);
		break;

	default:
		XTPPaintManager()->SetTheme(xtpThemeVisualStudio2010);
		m_paneManager.SetTheme(xtpPaneThemeVisualStudio2010);
		break;
	}

	//GetCommandBars()->EnableFrameTheme(TRUE);

	// Pane如果是VS2010风格,需要设置一些特殊参数
	if (m_paneManager.GetCurrentTheme() == xtpPaneThemeVisualStudio2010)
	{
		m_paneManager.SetClientMargin(6);
		m_paneManager.SetThemedFloatingFrames(TRUE);
	}
	else
	{
		m_paneManager.SetClientMargin(0);
		m_paneManager.SetThemedFloatingFrames(FALSE);
	}

	// 工具栏图标的一些选项设置
	XTPPaintManager()->GetIconsInfo()->bUseFadedIcons = FALSE;
	XTPPaintManager()->GetIconsInfo()->bIconsWithShadow = TRUE;	// 鼠标移动到工具栏图标时候,图标会显示阴影效果

	// 允许2007框架的效果是框架窗口边沿是圆滑的,但会影响皮肤的切换
	CString strSkinFile = theApp.GetProfileString(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_FILE);
	if(strSkinFile == "")
	{
		//EnableOffice2007Frame((nTheme == themeRibbon) ? GetCommandBars() : 0);
	}

	GetCommandBars()->RedrawCommandBars();

	m_MTIClientWnd.GetPaintManager()->RefreshMetrics();	
	
	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    // Create Status bar. 
    // Important: All control bars including the Status Bar 
    // must be created before CommandBars....
   if (!m_wndStatusBar.Create(this) ||
        !m_wndStatusBar.SetIndicators(indicators,
        sizeof(indicators)/sizeof(UINT)))
    {
        TRACE0("Failed to create status bar\n");
        return -1;      // fail to create
    }
	//m_wndStatusBar.UseCommandBarsTheme(TRUE);
   m_wndStatusBar.EnableCustomization();

    // Initialize the command bars
    if (!InitCommandBars())
        return -1;

	// 加载和初始化皮肤和主体
	/*CString strSkinFile = theApp.GetProfileString(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_FILE);
	CString strSkinIni = theApp.GetProfileString(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_INI);
	if(GetFileAttributes(strSkinFile) != 0xFFFFFFFF)
	{
		//strSkinFile = theApp.GetPlatRootPath();
		//strSkinFile += "\\Skins\\Office2007.cjstyles";
		DWORD dwApplyOptions = xtpSkinApplyMenus;
		BOOL bApplyMetrics = theApp.GetProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_APPLY_METRICS, TRUE);
		dwApplyOptions |= bApplyMetrics ? xtpSkinApplyMetrics: 0;
		BOOL bApplyFrame = theApp.GetProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_APPLY_FRAMESKIN, TRUE);
		dwApplyOptions |= bApplyFrame ? xtpSkinApplyFrame: 0;
		BOOL bApplyColors = theApp.GetProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_APPLY_COLORS, TRUE);
		dwApplyOptions |= bApplyColors ? xtpSkinApplyColors: 0;
		XTPSkinManager()->SetApplyOptions(dwApplyOptions);
		XTPSkinManager()->LoadSkin(strSkinFile, strSkinIni);
		OnSkinChanged();
	}*/
	m_nTheme = theApp.GetProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_THEME, themeVisualStudio2010);
	m_nThemeColor = theApp.GetProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_THEMECOLOR, themeColorBlue);
	//CString strTheme = theApp.GetConfText("Theme\\CommandBar");
	//m_nTheme = GetThemeString(strTheme);
	//m_nTheme = themeVisualStudio2010;
	SetTheme(m_nTheme, m_nThemeColor);
/*
	//XTPOffice2007Images()->SetHandle(_T("Office2007Blue.dll"));

	// 如果皮肤类型不是DLL类型,则Theme默认设置为2003风格
	if ((m_nSkinType == SKIN_CJINI) || (m_nSkinType == SKIN_CJSTYLE) || (m_nSkinType == SKIN_MSSTYLE))
	{
		strTheme = "ThemeOffice2003";
	}
*/
    // Get a pointer to the command bars object.
    CXTPCommandBars* pCommandBars = GetCommandBars();
    if(pCommandBars == NULL)
    {
        TRACE0("Failed to create command bars object.\n");
        return -1;      // fail to create
    }

	m_wndStatusBar.SetFont(pCommandBars->GetPaintManager()->GetIconFont());
	m_wndStatusBar.SetDrawDisabledText(FALSE);
	m_wndStatusBar.SetCommandBars(pCommandBars);
	m_wndStatusBar.GetStatusBarCtrl().SetMinHeight(22);
	m_wndStatusBar.GetPane(0)->SetMargins(8, 1, 2, 1);
	//m_wndStatusBar.GetImageManager()->SetIcons(IDR_OUTPUT);

	// 添加一个Pane,用于进度显示,Pane ID=1
	AddPane(1);
	SetPaneWidth(1, 0);
	
	// Load menu bar caption.
	CString strCapt;
	strCapt.LoadString(IDS_MENUBAR);

    // Add the menu bar
    CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(
        strCapt, IDR_MAINFRAME);       
    if(pMenuBar == NULL)
    {
        TRACE0("Failed to create menu bar.\n");
        return -1;      // fail to create
    }
	//pMenuBar->SetFlags(xtpFlagHideMinimizeBox | xtpFlagHideMaximizeBox | xtpFlagIgnoreSetMenuMessage);

	// Load command bar caption.
	strCapt.LoadString(IDS_STANDARD);

    // Create Stand ToolBar
    CXTPToolBar* pToolBar = (CXTPToolBar*)
        pCommandBars->Add(strCapt, xtpBarTop);
    if (!pToolBar || !pToolBar->LoadToolBar(IDR_MAINFRAME))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;
    }

	CXTPToolBar* pExplorerBar = (CXTPToolBar*)pCommandBars->Add(_T("Explorer"), xtpBarTop);
	if (!pExplorerBar ||
		!pExplorerBar->LoadToolBar(IDR_EXPLORER))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	DockRightOf(pExplorerBar, pToolBar);

	//CXTPOffice2003Theme::LoadModernToolbarIcons(IDR_MAINFRAME);

	// Hide array of commands
	pCommandBars->HideCommands(uHideCmds, _countof(uHideCmds));

	// Set "Always Show Full Menus" option to the FALSE
	pCommandBars->GetCommandBarsOptions()->bAlwaysShowFullMenus = FALSE;

	// 加载平台图标组
	CString strImagePath = theApp.GetPlatRootPath() + "conf\\image";
	if(!LoadImageStyleIcons(strImagePath))
	{
		// 如果加载失败就加载程序内部的
		XTPImageManager()->SetIcons(IDR_MAINFRAME);
	}

	pCommandBars->GetCommandBarsOptions()->ShowKeyboardCues(xtpKeyboardCuesShowWindowsDefault);
	pCommandBars->GetShortcutManager()->SetAccelerators(IDR_MAINFRAME);

	// toolbar的tooltip设置
	pCommandBars->GetToolTipContext()->SetStyle((XTPToolTipStyle)xtpToolTipOffice2007);
	pCommandBars->GetToolTipContext()->ShowTitleAndDescription(TRUE);
	pCommandBars->GetToolTipContext()->ShowImage(TRUE);	

    // Load the previous state for toolbars and menus.
    //LoadCommandBars(_T("CommandBars"));

	// Initialize the docking pane manager and set the
	// initial them for the docking panes.  Do this only after all
	// control bars objects have been created and docked.
	m_paneManager.InstallDockingPanes(this);

	// dockingpane皮肤模式
	m_paneManager.SetThemedFloatingFrames(!m_paneManager.IsThemedFloatingFrames());
	m_paneManager.SetStickyFloatingFrames(!m_paneManager.IsStickyFloatingFrames());
	// dockingpane设置为透明模式
	m_paneManager.SetFloatingFramesOpacity(m_paneManager.GetFloatingFramesOpacity() != 255 ? 255 : 200);

	// Create docking panes.
	// 创建属性窗口
	m_paneManager.CreatePane(IDR_PANE_PROPERTIES, CRect(0, 0,200, 120), xtpPaneDockLeft);

	// 创建输出窗口(需要保存状态的)
	//CreateOutputWnd("输出窗口");
	DOMNode* pNode = theApp.m_xmlPlat.GetNode("output\\window");
	while (pNode != NULL)
	{
		CString strWindowName = theApp.m_xmlPlat.GetNodeAttribute(pNode, "Name");
		CString strWindowType = theApp.m_xmlPlat.GetNodeAttribute(pNode, "Type");
		int nWindowType = OUTPUT_RICH;
		if(strWindowType == "GRID")
		{
			nWindowType = OUTPUT_GRID;
		}else
		if(strWindowType == "TREE")
		{
			nWindowType = OUTPUT_TREE;
		}
		CString strWindowSave = theApp.m_xmlPlat.GetNodeAttribute(pNode, "Save");
		int nWindowId = -1;
		if(theApp.m_xmlPlat.GetNodeAttribute(pNode, "ID") != "")
		{
			nWindowId = atoi(theApp.m_xmlPlat.GetNodeAttribute(pNode, "ID"));
		}
		if(strWindowSave == "true")
		{
			// 需要保存状态的窗口
			COutputWndCtrl* pOutputWndCtrl = CreateOutputWnd(strWindowName, nWindowType, nWindowId);
			pOutputWndCtrl->bSave = TRUE;
		}
		pNode = theApp.m_xmlPlat.GetNextNode(pNode, "window");
	}

	// Set the icons for the docking pane tabs.
	//int nIDIcons[] = {IDR_PANE_OPTIONS, IDR_PANE_PROPERTIES};
	//	m_paneManager.SetIcons(IDB_BITMAP_ICONS, nIDIcons, 
	//	_countof(nIDIcons), RGB(0, 255, 0));

    // Load the previous state for docking panes.
//    CXTPDockingPaneLayout layoutNormal(&m_paneManager);
//    if (layoutNormal.Load(_T("NormalLayout"))) 
//    {
//        m_paneManager.SetLayout(&layoutNormal); 
//    }

	//CXTPPropExchangeIniFile px(TRUE, 0, _T("Settings")); // To serialize to ini file
	CXTPPropExchangeXMLNode px(TRUE, 0, _T("Settings"));   // To serialize to XML file

	if (px.LoadFromFile(m_strIniFileName))
	{
#ifdef _XTP_INCLUDE_CONTROLS
		CXTPPropExchangeSection pxTaskPanel(px.GetSection(_T("TaskPanel")));
//		m_wndTaskPanel.GetGroups()->DoPropExchange(&pxTaskPanel);
#endif

		CXTPPropExchangeSection pxCommandBars(px.GetSection(_T("CommandBars")));
		XTP_COMMANDBARS_PROPEXCHANGE_PARAM param;
		param.bSerializeControls = TRUE; 
		GetCommandBars()->DoPropExchange(&pxCommandBars, &param);

		CXTPPropExchangeSection pxDockingPane(px.GetSection(_T("DockingPane")));
		CXTPDockingPaneLayout layoutNormal(&m_paneManager);
		if (layoutNormal.DoPropExchange(&pxDockingPane))
		{
			m_paneManager.SetLayout(&layoutNormal);
		}		
	}
	else
	{
		//ResetToolboxItems();
	}

	CXTPImageManager* pImageManager = pCommandBars->GetImageManager();

	pImageManager->InternalAddRef();
	m_paneManager.SetImageManager(pImageManager);

	m_paneManager.SetAlphaDockingContext(TRUE);
	m_paneManager.SetShowDockingContextStickers(TRUE);
	m_paneManager.SetDockingContextStickerStyle(xtpPaneStickerStyleWhidbey);

	// 注：如果要支持分组，就加第二个参数，但加了之后就只能最大化窗口了
	VERIFY(m_MTIClientWnd.Attach(this/*, TRUE*/));
	m_MTIClientWnd.LoadState();
	m_MTIClientWnd.EnableToolTips();
	m_MTIClientWnd.SetFlags(xtpWorkspaceHideClose | xtpWorkspaceHideArrows | xtpWorkspaceShowCloseSelectedTab);

	//m_MTIClientWnd.GetPaintManager()->m_bShowIcons = FALSE;

	// 创建属性窗口的图标
	if(m_paneManager.FindPane(IDR_PANE_PROPERTIES) != NULL)
	{
		// 取消原来通过显示属性Pane来创建图标的方法,改为直接创建图标,避免每次都会把属性Pane设为激活的
		//if(!m_paneManager.IsPaneClosed(IDR_PANE_PROPERTIES))
		//{
		//	m_paneManager.ShowPane(IDR_PANE_PROPERTIES);
		//}
		m_paneManager.SetIcon(IDR_PANE_PROPERTIES, CXTPImageManagerIconHandle(theApp.LoadIcon(IDI_PROPERTY)));
	}
	
	// 创建输出窗口(不需要保存状态的)
	pNode = theApp.m_xmlPlat.GetNode("output\\window");
	while (pNode != NULL)
	{
		CString strWindowName = theApp.m_xmlPlat.GetNodeAttribute(pNode, "Name");
		CString strWindowType = theApp.m_xmlPlat.GetNodeAttribute(pNode, "Type");
		int nWindowType = OUTPUT_RICH;
		if(strWindowType == "GRID")
		{
			nWindowType = OUTPUT_GRID;
		}else
		if(strWindowType == "TREE")
		{
			nWindowType = OUTPUT_TREE;
		}
		CString strWindowSave = theApp.m_xmlPlat.GetNodeAttribute(pNode, "Save");
		int nWindowId = -1;
		if(theApp.m_xmlPlat.GetNodeAttribute(pNode, "ID") != "")
		{
			nWindowId = atoi(theApp.m_xmlPlat.GetNodeAttribute(pNode, "ID"));
		}
		if(strWindowSave != "true")
		{
			// 不需要保存状态的窗口
			COutputWndCtrl* pOutputWndCtrl = CreateOutputWnd(strWindowName, nWindowType, nWindowId);
			pOutputWndCtrl->bSave = FALSE;
		}

		if(nWindowId != -1)
		{
			m_paneManager.SetIcon(IDR_PANE_OUTPUT+nWindowId, CXTPImageManagerIconHandle(theApp.LoadIcon(IDI_DISPLAY)));
		}

		pNode = theApp.m_xmlPlat.GetNextNode(pNode, "window");
	}

	// 启动秒任务定时器
	SetTimer(1, 1000, NULL);

	return 0;
}

BOOL CMainFrame::ShowWindowEx(int nCmdShow)
{
	// Restore frame window size and position.
	m_wndPosition.LoadWindowPos(this);
	nCmdShow = m_wndPosition.showCmd;

	return ShowWindow(nCmdShow);
}

/////////////////////////////////////////////////////////////////////////////
// 设置某个命令ID的图标
/////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::ImageManagerSetIcon(int nID, LPCTSTR lpszIconFile)
{
	BOOL bRet = XTPImageManager()->SetIconFromIcoFile(lpszIconFile, nID, 0, xtpImageNormal);
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// 设置多个命令ID的图标
/////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::ImageManagerSetIcons(CUIntArray* paIDs, LPCTSTR lpszBmpFile)
{
	if(paIDs == NULL)
		return FALSE;

	CBitmap bmp;

	HBITMAP hBitmap = (HBITMAP)::LoadImage(::AfxGetInstanceHandle(), lpszBmpFile,
        IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if(hBitmap == NULL)
		return FALSE;

	BITMAP bmpInfo;
	::GetObject(hBitmap, sizeof(BITMAP), &bmpInfo);

	bmp.Attach(hBitmap);

	UINT uIDs[100];
	for(int i=0; i<(*paIDs).GetSize(); i++)
	{
		if(i < 100)
		{
			uIDs[i] = (*paIDs)[i];
		}
	}

	BOOL bRet = XTPImageManager()->SetIcons(bmp, uIDs, (*paIDs).GetSize(), CSize(bmpInfo.bmHeight,bmpInfo.bmHeight), xtpImageNormal);

	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// 加载当前风格的图标文件组
/////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::LoadImageStyleIcons(CString strImagePath)
{
	CString strImageStyle = theApp.GetProfileString(REG_CONFIG_SUBKEY, REG_CONFIG_IMAGESTYLE, "default");
	
	if(GetFileAttributes(strImagePath + "\\" + strImageStyle) == 0xFFFFFFFF)
	{
		// 如果路径不存在就查找有没有名字为AppName的路径
		CString strAppName = theApp.m_xmlPlat.GetNodeText("application\\appName");
		if(GetFileAttributes(strImagePath + "\\" + strAppName) == 0xFFFFFFFF)
		{
			return FALSE;
		}
		strImageStyle = strAppName;
	}

	strImagePath += "\\";
	strImagePath += strImageStyle;

	CString strPlugin = strImagePath;
	if(LANGUAGE_PAGE_CHINESE == theApp.m_curLanguage)
	{
		strPlugin += "\\plugin_cn.xml";
		// 如果不存在中文的配置文件,就用缺省的
		if(GetFileAttributes(strPlugin) == 0xFFFFFFFF)
		{
			strPlugin = strImagePath + "\\plugin.xml";
		}
	}else
	{
		strPlugin += "\\plugin.xml";
	}
	CXmlParser parser;
	if(parser.Open(strPlugin))
	{
		OUTPUT(COLOR_ERROR, "Load %s fail!\r\n", strPlugin);
		return FALSE;
	}

	DOMNode* pNode = parser.GetRootNode();
	DOMNode* pSubNode = parser.GetChildNode(pNode);
	while (pSubNode != NULL)
	{
		CString strBmpFile = strImagePath + "\\" + parser.GetNodeAttribute(pSubNode, "file");
		CString strIDs = parser.GetNodeText(pSubNode);
		CUIntArray auID;
		int nPos = strIDs.Find(",");
		while(nPos != -1)
		{
			auID.Add(atoi(strIDs.Left(nPos)));
			strIDs.Delete(0, nPos+1);
			nPos = strIDs.Find(",");
		}
		auID.Add(atoi(strIDs));

		if(!ImageManagerSetIcons(&auID, strBmpFile))
		{
			return FALSE;
		}

		pSubNode = parser.GetNextNode(pSubNode);
	}

	parser.Close();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 增加Pane
/////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::AddPane(UINT nPaneID)
{
	int nPaneIndex = m_wndStatusBar.CommandToIndex(nPaneID);
	if(nPaneIndex < 0)
	{
		int nPaneCount = m_wndStatusBar.GetPaneCount();
		nPaneIndex = nPaneCount;
		m_wndStatusBar.AddIndicator(nPaneID, nPaneIndex);
		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 关闭Pane
/////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::ClosePaneItem(UINT nPaneID)
{
	int nPaneIndex = m_wndStatusBar.CommandToIndex(nPaneID);
	if(nPaneIndex < 0)
		return FALSE;
	m_wndStatusBar.RemoveIndicator(nPaneID);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 关闭所有用户创建的Pane(ID>=500说明是用户创建的)
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::CloseAllSelfPane()
{
	for(int i = m_wndStatusBar.GetPaneCount()-1; i >=0; i--)
	{
		UINT nPaneID = m_wndStatusBar.GetPane(i)->GetID();
		if(nPaneID >= 500)
		{
			m_wndStatusBar.RemoveIndicator(nPaneID);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 设置Pane文字
/////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::SetPaneText(UINT nPaneID, LPCTSTR lpszText)
{
	int nPaneIndex = m_wndStatusBar.CommandToIndex(nPaneID);
	if(nPaneIndex < 0)
		return FALSE;
	m_wndStatusBar.SetPaneText(nPaneIndex, lpszText);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 设置Pane控件
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::SetPaneControl(UINT nPaneID, CWnd* pWnd)
{
	int nPaneIndex = m_wndStatusBar.CommandToIndex(nPaneID);
	if(nPaneIndex < 0)
		return;
	m_wndStatusBar.AddControl(pWnd, nPaneID);
}

/////////////////////////////////////////////////////////////////////////////
// 设置Pane宽度
/////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::SetPaneWidth(UINT nPaneID, int nWidth)
{
	int nPaneIndex = m_wndStatusBar.CommandToIndex(nPaneID);
	if(nPaneIndex < 0)
		return FALSE;
	m_wndStatusBar.SetPaneWidth(nPaneIndex, nWidth);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 设置Pane图标项
/////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::SetPaneImageItem(UINT nPaneID, int nItem)
{
	int nPaneIndex = m_wndStatusBar.CommandToIndex(nPaneID);
	if(nPaneIndex < 0)
		return FALSE;
	CXTPStatusBarPane* pPane = m_wndStatusBar.GetPane(nPaneIndex);
	if(!pPane)
	{
		return FALSE;
	}

	pPane->SetIconIndex(nItem);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 创建一个进度条并和特定Pane建立关联
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::AttachPaneProgress(UINT nPaneID, CString strMsg, int nMaxValue)
{
	int nPaneIndex = m_wndStatusBar.CommandToIndex(nPaneID);
	if(nPaneIndex < 0)
		return;
	if(g_pSkinProgress !=NULL)
	{
		// 已经创建了就退出
		if(g_pSkinProgress->GetPane() == nPaneIndex)
		{
			return;
		}

		delete g_pSkinProgress;
		g_pSkinProgress = NULL;
	}

	g_pSkinProgress = new CSkinProgress(strMsg, 200, nMaxValue, nPaneIndex, cSPT_PERCENT, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// 显示/隐藏进度条
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::ShowPaneProgress(UINT nPaneID, BOOL bShow)
{
	int nPaneIndex = m_wndStatusBar.CommandToIndex(nPaneID);
	if(nPaneIndex < 0)
		return;

	if(g_pSkinProgress && !bShow)
	{
		delete g_pSkinProgress;
		g_pSkinProgress = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// 进度条进度控制
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::SetPaneProgressPos(UINT nPaneID, int nPos)
{
	int nPaneIndex = m_wndStatusBar.CommandToIndex(nPaneID);
	if(nPaneIndex < 0)
		return;

	if(g_pSkinProgress)
	{
		g_pSkinProgress->SetPos(nPos);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 设置Pane按钮
/////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::SetPaneButton(UINT nPaneID, CExtMenuInfo* pExtMenuInfo)
{
	int nPaneIndex = m_wndStatusBar.CommandToIndex(nPaneID);
	if(nPaneIndex < 0)
		return FALSE;
	CXTPStatusBarPane* pPane = m_wndStatusBar.GetPane(nPaneIndex);
	if(!pPane)
	{
		return FALSE;
	}

	pPane->SetButton(TRUE);

	if(pExtMenuInfo != NULL)
	{
		// 如果有扩展菜单信息,就添加或更新此信息
		CExtMenuInfo* pMenuInfo = GetExtMenuInfo(pExtMenuInfo->nId);
		if(pMenuInfo == NULL)
		{
			m_arExtMenuInfo.Add(*pExtMenuInfo);
		}else
		{
			*pMenuInfo = *pExtMenuInfo;
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 设置Pane Tooltip
/////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::SetPaneTooltip(UINT nPaneID, LPCTSTR lpszTooltip)
{
	int nPaneIndex = m_wndStatusBar.CommandToIndex(nPaneID);
	if(nPaneIndex < 0)
		return FALSE;
	CXTPStatusBarPane* pPane = m_wndStatusBar.GetPane(nPaneIndex);
	if(!pPane)
	{
		return FALSE;
	}

	pPane->SetTooltip(lpszTooltip);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 状态行按钮事件处理
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnIndicatorButton(UINT nID)
{
	CExtMenuInfo* pMenuInfo = GetExtMenuInfo(nID);
	if(pMenuInfo)
	{
		if(pMenuInfo->pProc)
		{
			// 执行按钮函数
			if(pMenuInfo->nProcType == MENU_PROC_TYPE_NORMAL)
			{
				pMenuInfo->pProc();
			}else
			if(pMenuInfo->nProcType == MENU_PROC_TYPE_PARAM_ID)
			{
				TYPEOF_ClientMenuParamId* ProcMenu = (TYPEOF_ClientMenuParamId*)(pMenuInfo->pProc);
				ProcMenu(nID);
			}
		}else
		if(pMenuInfo->m_strAction != "")
		{
			CString strAction = pMenuInfo->m_strAction;
			if(strAction.Find("tool:") == 0)
			{
				// 执行指定的工具
				strAction.Delete(0, 5);
				for(int i=0; i<theApp.m_aTools.GetSize(); i++)
				{
					if(theApp.m_aTools[i].m_strId == strAction)
					{
						theApp.m_aTools[i].Run();
						return;
					}
				}
			}else
			{
				// 执行按钮动作
				// TODO:未完成......,需要在App中添加一个通用的Action动作解析和执行函数,支持interp,html,owm,tool等类型的动作
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 状态行按钮刷新
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateIndicatorButton(CCmdUI* pCmdUI)
{
	CExtMenuInfo* pMenuInfo = GetExtMenuInfo(pCmdUI->m_nID);
	if(pMenuInfo && pMenuInfo->pUpdateProc)
	{
		pMenuInfo->pUpdateProc(pCmdUI);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 设置文档图标
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::SetDocumentIcon(CString strIconFile, HWND hWnd)
{
	CXTPTabManagerItem* pItem = m_MTIClientWnd.FindItem(hWnd);
	if(pItem)
	{
		CWnd* pFrame = CWnd::FromHandle(pItem->GetHandle());
		WORD wIndex = 0;
		CString strIconFileTmp = strIconFile;
		HICON hIcon = ::ExtractAssociatedIcon(AfxGetApp()->m_hInstance,
					strIconFile.GetBuffer(0), &wIndex);
		//strIconFile.ReleaseBuffer();
		pFrame->SetIcon(hIcon, TRUE);
		m_MTIClientWnd.Refresh();
	}
}

/////////////////////////////////////////////////////////////////////////////
// 设置文档标题
// 如果strTitle为空,则根据nModifyType类型,1表示加*,0表示去掉*,2表示加>>>
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::SetDocumentTitle(CString strTitle, int nModifyType, HWND hWnd)
{
	CXTPTabManagerItem* pItem = m_MTIClientWnd.FindItem(hWnd);
	if(pItem)
	{
		CWnd* pFrame = CWnd::FromHandle(pItem->GetHandle());
		CDocument* pDoc = ((CMDIChildWnd*)pFrame)->GetActiveDocument();
		if(pDoc)
		{
			if(strTitle != "")
			{
				pDoc->SetTitle(strTitle);
			}else
			{
				CString strOldTitle = pDoc->GetTitle();
				if(nModifyType == 0)	// 正常模式
				{
					strOldTitle.Replace("*", "");
					strOldTitle.Replace(">", "");
				}else
				if(nModifyType == 1)	// 文件被修改
				{
					strOldTitle.Replace("*", "");
					strOldTitle += "*";
				}else
				if(nModifyType == 2)	// 调试模式
				{
					strOldTitle.Replace(">", "");
					strOldTitle = ">>>" + strOldTitle;
				}else
				if(nModifyType == 3)	// 从调试模式
				{
					strOldTitle.Replace(">", "");
					strOldTitle = ">>" + strOldTitle;
				}
				pDoc->SetTitle(strOldTitle);
			}
		}
		m_MTIClientWnd.Refresh();
	}
}

void CMainFrame::ResetCommandBars()
{
	m_bInRecalcLayout = TRUE;

	CXTPCommandBars* pCommandBars = GetCommandBars();
	for (int i = 0; i < pCommandBars->GetCount(); i++)
	{
		CXTPToolBar* pToolBar = pCommandBars->GetAt(i);
		if (pToolBar->IsBuiltIn())
		{
			if (pToolBar->GetType() != xtpBarTypeMenuBar)
			{
				pToolBar->LoadToolBar(pToolBar->GetBarID(), FALSE);
			}
			else
			{
				CMenu menu;
				XTPResourceManager()->LoadMenu(&menu, IDR_MAINFRAME);

				pToolBar->LoadMenu(&menu);
				pToolBar->GetControls()->CreateOriginalControls();

				((CXTPMenuBar*)pToolBar)->RefreshMenu();
			}
		}
	}

	CString strCapt;
	XTPResourceManager()->LoadString(&strCapt, IDS_MENUBAR);
	pCommandBars->GetMenuBar()->SetTitle(strCapt);

	XTPResourceManager()->LoadString(&strCapt, IDS_STANDARD);
	pCommandBars->GetAt(1)->SetTitle(strCapt);


	m_bInRecalcLayout = FALSE;
	RecalcLayout();
	AfxGetApp()->OnIdle(0);
	UpdateWindow();
}

void CMainFrame::OnClose() 
{
	// 停止秒任务定时器
	KillTimer(1);

	// 关闭工程
	if(theApp.m_pIProject)
	{
		if(PROJECT_WORKING == theApp.m_pIProject->GetProjectState(""))
		{
			// 工程正在运行
			PLAT_LOADSTRING(strProjectWorking, IDS_PROJECT_WORKING);
			AfxMessageBox(strProjectWorking);
			return;
		}
		theApp.OnProjectClose();
		if(theApp.m_pIProject)
		{
			return;
		}
	}

	// 将工程关闭过程中输出等积累的消息处理掉
	if(::IsWindow(m_hWnd))
	{
		PostMessage(WM_PLAT_COMCMD, 1, 1);	// 发送一个标志消息，保证下面的循环能退出
		MSG msg;
		while(::PeekMessage(&msg, m_hWnd, WM_PLAT_COMCMD, WM_PLAT_COMCMD, PM_REMOVE))
		{	// 循环取消息并处理
			if((1==msg.wParam) && (1==msg.lParam))	// 判断退出标志
				break;
			TranslateMessage(&msg);										
			DispatchMessage(&msg);
		}
	}

	// 询问每个VCI组件是否可以关闭
	// 提示保存改动过的文档(目前只支持OwmEdit)
	int i;
	for(i=theApp.m_aPlugIns.GetSize()-1; i>=0; i--)
	{
		if(!theApp.m_aPlugIns[i].CanClose())
		{
			PLAT_LOADSTRING(strStopInterp, IDS_STOP_INTERP_FIRST);
			AfxMessageBox(strStopInterp);
			//AfxMessageBox("有正在运行的窗口或解释器，请先停止其运行！");
			return;
		}
	}

	// 关闭所有不需要保存位置的输出窗口
	for(i=0; i<m_aOutputCtrl.GetSize(); i++)
	{
		if(!m_aOutputCtrl[i].bSave)
		{
			m_paneManager.DestroyPane(m_aOutputCtrl[i].nID);
		}
		delete m_aOutputCtrl[i].pWnd;
	}

	// 关闭所有VCI组件创建的DockingPane(允许保存位置的除外)
	for(i=theApp.m_aPlugIns.GetSize()-1; i>=0; i--)
	{
		for(int j=0; j<theApp.m_aPlugIns[i].m_arDockingPane.GetSize(); j++)
		{
			if(PANE_ATTACH_VIEW == theApp.m_aPlugIns[i].m_arDockingPane[j].m_nAttachType)
			{
				// 创建的View需要先释放,否则MFC会向这个已经删除的窗口发送释放消息,导致退出时异常
				CPlugIn* pPlugIn = &(theApp.m_aPlugIns[i]);
				if(theApp.m_aPlugIns[i].m_arDockingPane[j].m_pWnd)// &&
					//(theApp.m_aPlugIns[i].m_strType != "project"))
				{
					theApp.m_aPlugIns[i].m_arDockingPane[j].m_pWnd->DestroyWindow();
				}
			}
			if(!theApp.m_aPlugIns[i].m_arDockingPane[j].m_bSave)
			{
				// 删除不保存位置的Docking窗口
				m_paneManager.DestroyPane(theApp.m_aPlugIns[i].m_arDockingPane[j].m_nId);
			}
		}
	}

	// 关闭所有Tool创建的DockingPane(允许保存位置的除外)
	for(i=theApp.m_aTools.GetSize()-1; i>=0; i--)
	{
		for(int j=0; j<theApp.m_aTools[i].m_arDockingPane.GetSize(); j++)
		{
			if(PANE_ATTACH_VIEW == theApp.m_aTools[i].m_arDockingPane[j].m_nAttachType)
			{
				// 创建的View需要先释放,否则MFC会向这个已经删除的窗口发送释放消息,导致退出时异常
				if(theApp.m_aTools[i].m_arDockingPane[j].m_pWnd)
				{
					theApp.m_aTools[i].m_arDockingPane[j].m_pWnd->DestroyWindow();
				}
			}
			if(!theApp.m_aTools[i].m_arDockingPane[j].m_bSave)
			{
				// 删除不保存位置的Docking窗口
				m_paneManager.DestroyPane(theApp.m_aTools[i].m_arDockingPane[j].m_nId);
			}
		}
	}

	// 删除所有创建的扩展菜单
	for(i=m_arExtMenuInfo.GetSize()-1; i>=0; i--)
	{
		if(m_arExtMenuInfo[i].m_strMainMenu != "")
		{			
			// 查找并删除创建的菜单
			CXTPCommandBar* pMenuBar = GetCommandBars()->GetMenuBar();
			CXTPControls* pControls = pMenuBar->GetControls();
			CXTPControl* pControl = pControls->GetFirst();
			while(pControl)
			{
				if(pControl->GetCaption() == m_arExtMenuInfo[i].m_strMainMenu)
				{
					if(m_arExtMenuInfo[i].m_strSubMenu == "")
					{
						// 删除主菜单项
						pControls->Remove(pControl);
						m_arExtMenuInfo.RemoveAt(i);
					}else
					{
						// 删除一个子菜单项
						CXTPCommandBar* pMenuBarSub = pControl->GetCommandBar();
						CXTPControls* pControlsSub = pMenuBarSub->GetControls();
						pControl = pControlsSub->GetFirst();
						while(pControl)
						{
							if(pControl->GetCaption() == m_arExtMenuInfo[i].m_strSubMenu)
							{
								pControlsSub->Remove(pControl);
								m_arExtMenuInfo.RemoveAt(i);
								break;
							}
							pControlsSub->GetNext(pControl);
						}
					}
					break;
				}
				pControls->GetNext(pControl);
			}
		}
	}

	//CXTPPropExchangeIniFile px(FALSE, 0, _T("Settings"));    // To serialize to ini file
	CXTPPropExchangeXMLNode px(FALSE, 0, _T("Settings"));      // To serialize to XML file

#ifdef _XTP_INCLUDE_CONTROLS
	CXTPPropExchangeSection pxTaskPanel(px.GetSection(_T("TaskPanel")));
//	m_wndTaskPanel.GetGroups()->DoPropExchange(&pxTaskPanel);
#endif

	CXTPPropExchangeSection pxCommandBars(px.GetSection(_T("CommandBars")));
	XTP_COMMANDBARS_PROPEXCHANGE_PARAM param;
	param.bSerializeControls = TRUE;
	param.bSaveOriginalControls = FALSE;
	GetCommandBars()->DoPropExchange(&pxCommandBars, &param);

	CXTPPropExchangeSection pxDockingPane(px.GetSection(_T("DockingPane")));
	
	CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	m_paneManager.GetLayout(&layoutNormal);
	layoutNormal.DoPropExchange(&pxDockingPane);

	px.SaveToFile(m_strIniFileName);
/*
    // Save the current state for toolbars and menus.
    SaveCommandBars(_T("CommandBars"));

    // Save the current state for docking panes.
    CXTPDockingPaneLayout layoutNormal(&m_paneManager);
    m_paneManager.GetLayout(&layoutNormal); 
    layoutNormal.Save(_T("NormalLayout"));
*/
	// 清空属性树
	UpdatePropertyGridContent();
	m_bEnableUpdatePropertyGrid = FALSE;

	// 释放属性树Pane
	//m_paneManager.ClosePane(IDR_PANE_PROPERTIES);

	// 保存打开的所有文件与链接的文件
	BOOL bAutoReloadDocument = theApp.GetProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_AUTO_RELOAD_DOCUMENT, TRUE);
	if(bAutoReloadDocument)
	{
		CString strFileWorkspace = theApp.GetPlatRootPath();
		strFileWorkspace += "conf\\workspace.session";
		SaveSessionFile(strFileWorkspace);
	}

	// 释放所有VCI组件
	for(i=theApp.m_aPlugIns.GetSize()-1; i>=0; i--)
	{
		theApp.m_aPlugIns[i].CloseVci();
	}
	theApp.m_aPlugIns.RemoveAll();

	// 因为Post的消息优先级比较低,造成中间层的有些消息包丢失,使内存泄漏
	// 因此退出之前必须将消息队列中剩余中间层消息全部处理完.
	if(::IsWindow(m_hWnd))	// 如果处于打印预览状态直接关闭整个程序,必须判断m_hWnd的有效性
	{
		PostMessage(WM_PLAT_COMCMD, 1, 1);	// 发送一个标志消息，保证下面的循环能退出
		MSG msg;
		while(::PeekMessage(&msg, m_hWnd, WM_PLAT_COMCMD, WM_PLAT_COMCMD, PM_REMOVE))
		{	// 循环取消息并处理
			if((1==msg.wParam) && (1==msg.lParam))	// 判断退出标志
				break;
			TranslateMessage(&msg);										
			DispatchMessage(&msg);
		}
	}

	// Save frame window size and position.
	m_wndPosition.SaveWindowPos(this);

	// 删除进度状态条对象
	if(g_pSkinProgress !=NULL)
	{
		delete g_pSkinProgress;
		g_pSkinProgress = NULL;
	}

	if(::IsWindow(m_hWnd))
	{
		CMDIFrameWnd::OnClose();
	}
}

void CMainFrame::OnCustomize()
{
    // Get a pointer to the command bars object.
    CXTPCommandBars* pCommandBars = GetCommandBars();
    if(pCommandBars != NULL)
    {
        // Instanciate the customize dialog object.
        CXTPCustomizeSheet dlg(pCommandBars);

		CXTPCustomizeKeyboardPage pageKeyboard(&dlg);
		dlg.AddPage(&pageKeyboard);
		pageKeyboard.AddCategories(IDR_MAINFRAME);

        // Add the options page to the customize dialog.
        CXTPCustomizeOptionsPage pageOptions(&dlg);
        dlg.AddPage(&pageOptions);

        // Add the commands page to the customize dialog.
        CXTPCustomizeCommandsPage* pCommands = dlg.GetCommandsPage();
        pCommands->AddCategories(IDR_MAINFRAME);

        // Use the command bar manager to initialize the 
        // customize dialog.
        pCommands->InsertAllCommandsCategory();
        pCommands->InsertBuiltInMenus(IDR_MAINFRAME);
        pCommands->InsertNewMenuCategory();

        // Dispaly the dialog.
        dlg.DoModal();
    }
}

/////////////////////////////////////////////////////////////////////////////
// 扩展工具菜单命令处理函数(此函数暂时不用了)
/////////////////////////////////////////////////////////////////////////////
void OnExtToolMenuProc(int nID) 
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CExtMenuInfo* pMenuInfo = pMainFrame->GetExtMenuInfo(nID);
	if(pMenuInfo)
	{
		AfxMessageBox(pMenuInfo->m_strAction);
	}
}

int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	CXTPMenuBar* pMenuBar = DYNAMIC_DOWNCAST(CXTPMenuBar, lpCreateControl->pCommandBar);
	if (pMenuBar)
	{
		ASSERT(lpCreateControl->pMenu);
		CMenu* pPopupMenu = lpCreateControl->pMenu->GetSubMenu(lpCreateControl->nIndex);

		if ((lpCreateControl->strCaption == "窗口(&W)") ||
			(lpCreateControl->strCaption == _T("&Window")))		
		{
			CXTPControlPopup* pControlPopup = CXTPControlPopup::CreateControlPopup(xtpControlPopup);

			// Exclude only IDR_MAINFRAME template
			pControlPopup->ExcludeDocTemplate(IDR_MAINFRAME);

			pControlPopup->SetCommandBar(pPopupMenu);
			
			lpCreateControl->pControl = pControlPopup;
			return TRUE;
		}
		
		if ((lpCreateControl->strCaption == "编辑(&E)") ||
			(lpCreateControl->strCaption == _T("&Edit")))
		{
			CXTPControlPopup* pControlPopup = CXTPControlPopup::CreateControlPopup(xtpControlPopup);

			// Present only in IDR_BLUEUITYPE template
			pControlPopup->AssignDocTemplate(IDR_BLUEUITYPE);

			pControlPopup->SetCommandBar(pPopupMenu);
			
			lpCreateControl->pControl = pControlPopup;
			return TRUE;
		}
		/*
		// 添加工具扩展菜单
		if ((lpCreateControl->strCaption == "工具(&T)") ||
			(lpCreateControl->strCaption == _T("&Tools")))		
		{
			CExtMenuInfo extMenuInfo;
			extMenuInfo.m_strMainMenu = lpCreateControl->strCaption;
			extMenuInfo.pProc = (TYPEOF_ClientMenu*)OnExtToolMenuProc;
			extMenuInfo.pUpdateProc = NULL;
			extMenuInfo.nProcType = MENU_PROC_TYPE_PARAM_ID;
			
			int nExMenuId = IDM_OWMCMD+4000;
			pPopupMenu->AppendMenu(MF_SEPARATOR);
			pPopupMenu->AppendMenu(MF_STRING, nExMenuId, "hdfsdf");
			extMenuInfo.m_strAction = "hello";
			extMenuInfo.nId = nExMenuId-IDM_OWMCMD;
			m_arExtMenuInfo.Add(extMenuInfo);

			nExMenuId++;
			pPopupMenu->AppendMenu(MF_STRING, nExMenuId, "hsdfeeee2");
			extMenuInfo.m_strAction = "hello2";
			extMenuInfo.nId = nExMenuId-IDM_OWMCMD;
			m_arExtMenuInfo.Add(extMenuInfo);

			//return TRUE;
		}
		*/

		/*
		// 查看所有OWM扩展菜单,看有没有需要添加的菜单
		for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
		{
			for(int j=0; j<theApp.m_aPlugIns[i].m_arMenu.GetSize(); j++)
			{
				if(theApp.m_aPlugIns[i].m_arMenu[j].m_strMainMenu == lpCreateControl->strCaption)
				{
					CExtMenuInfo extMenuInfo;
					extMenuInfo.m_strMainMenu = lpCreateControl->strCaption;
					extMenuInfo.pProc = (TYPEOF_ClientMenu*)OnExtToolMenuProc;
					extMenuInfo.pUpdateProc = NULL;
					extMenuInfo.nProcType = MENU_PROC_TYPE_PARAM_ID;
					
					int nExMenuId = IDM_OWMCMD+g_nIdOwmExMenu;
					pPopupMenu->AppendMenu(MF_STRING, nExMenuId, theApp.m_aPlugIns[i].m_arMenu[j].m_strSubMenu);
					extMenuInfo.m_strAction = theApp.m_aPlugIns[i].m_arMenu[j].m_strAction;
					extMenuInfo.nId = nExMenuId-IDM_OWMCMD;
					m_arExtMenuInfo.Add(extMenuInfo);

					g_nIdOwmExMenu++;
				}
			}
		}
		*/

	}else
	if (lpCreateControl->bToolBar)
	{
		CXTPToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTPToolBar, lpCreateControl->pCommandBar);
		if (!pToolBar)
			return FALSE;

		if (lpCreateControl->nID == ID_GOTO_URL && pToolBar->GetBarID() == IDR_EXPLORER)
		{
			CComboBoxURL* pComboAddress = new CComboBoxURL();
			pComboAddress->UpdateComboBox();
			pComboAddress->SetWidth(250);
			pComboAddress->SetDropDownListStyle();
			pComboAddress->EnableShellAutoComplete();

			m_pComboBoxURL = pComboAddress;
		
			lpCreateControl->pControl = pComboAddress;
			return TRUE;
		}
	}else
	if (lpCreateControl->nID == ID_EXTEND_TOOLS)
	{
		lpCreateControl->pControl = new CControlTools();
		return TRUE;
	}else
	if (lpCreateControl->nID == ID_EXPORT_FILES)
	{
		lpCreateControl->pControl = new CControlExports();
		return TRUE;
	}
	
	return FALSE;
}

LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
    if (wParam == XTP_DPN_SHOWWINDOW)
    {
        CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;
        
        if (!pPane->IsValid())
        {
			int nPaneID = pPane->GetID();
			
			if((nPaneID >= IDR_PANE_OUTPUT) && (nPaneID < (IDR_PANE_OUTPUT+10000)))
			{
				// 输出窗口Pane

				// 查找看有没有哪个OWM中创建了此窗口
				IVciOwm* pIVciOwm = NULL;
				BOOL bOwmDockSave = FALSE;
				for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
				{
					for(int j=0; j<theApp.m_aPlugIns[i].m_arDockingPane.GetSize(); j++)
					{
						if(theApp.m_aPlugIns[i].m_arDockingPane[j].m_nId == nPaneID)
						{
							// 找到一个OWM,执行创建函数
							CPlugIn* pPlugIn = &(theApp.m_aPlugIns[i]);
							if(!pPlugIn->IsVciOwm())
							{
								continue;
							}

							pIVciOwm = theApp.m_aPlugIns[i].GetIVciOwm();
							if(pIVciOwm == NULL)
							{
								// 如果OWM组件尚未加载,则先加载
								theApp.m_aPlugIns[i].LoadOwmPlugIn();
								pIVciOwm = theApp.m_aPlugIns[i].GetIVciOwm();
							}
							if(pIVciOwm)
							{
								bOwmDockSave = theApp.m_aPlugIns[i].m_arDockingPane[j].m_bSave;
								break;
							}
						}
					}
					if(pIVciOwm) break;
				}

				COutputWndCtrl* pOutputCtrl = GetOutputCtrl(nPaneID);
				if(pOutputCtrl == NULL)
				{
					if(pIVciOwm)
					{
						pIVciOwm->OnShowDockingPane(nPaneID, this, "");
						pOutputCtrl = GetOutputCtrl(nPaneID);
						if(pOutputCtrl)
						{
							pOutputCtrl->bSave = bOwmDockSave;
						}
					}else
					{
						return FALSE;
					}
				}

				if (pOutputCtrl->pWnd->GetSafeHwnd() == 0)
				{
					try {
					//////////////////////
					if(pOutputCtrl->nCtrlType == OUTPUT_RICH)
					{
						DWORD dwStyle = WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VSCROLL;
						((COutputCtrl*)(pOutputCtrl->pWnd))->Create(dwStyle, CRect(0,0,0,0), this, IDR_PANE_OUTPUT );
					}else
					if(pOutputCtrl->nCtrlType == OUTPUT_GRID)
					{
						DWORD dwStyle = WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
						((CReportOutCtrl*)(pOutputCtrl->pWnd))->Create(dwStyle, CRect(0,0,0,0), this, IDR_PANE_OUTPUT );
					}
					if(pOutputCtrl->nCtrlType == OUTPUT_TREE)
					{
						DWORD dwStyle = WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
						((CTreeOutCtrl*)(pOutputCtrl->pWnd))->Create(dwStyle, CRect(0,0,0,0), this, IDR_PANE_OUTPUT );
						((CTreeOutCtrl*)(pOutputCtrl->pWnd))->Init();
					}
					///////////////////////
					}catch(...)
					{
					}
				}
				// 和Skin的冲突，需要对句柄进行判断
				HWND hSafeWnd = pOutputCtrl->pWnd->GetSafeHwnd();
				if((hSafeWnd != NULL) && (hSafeWnd != (HWND)0xFEEEFEEE) && (hSafeWnd != (HWND)0xCDCDCDCD))
				{
					pPane->Attach(pOutputCtrl->pWnd);
				}
				if(pIVciOwm == NULL)
				{
					// 如果不是OWM中创建的输出窗口才需要设置默认的图标
					m_paneManager.SetIcon(nPaneID, CXTPImageManagerIconHandle(theApp.LoadIcon(IDI_DISPLAY)));
				}
				pPane->SetTitle(pOutputCtrl->strTitle);
			}else
			{
				switch (nPaneID)
				{
				case IDR_PANE_PROPERTIES:
					{                           
						if (m_panePropertyGrid.GetSafeHwnd() == 0)
						{
							m_panePropertyGrid.Create(_T("STATIC"), NULL,
								WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
								CRect(0, 0, 0, 0), this, 0);
						}
						pPane->Attach(&m_panePropertyGrid);
						m_paneManager.SetIcon(nPaneID, CXTPImageManagerIconHandle(theApp.LoadIcon(IDI_PROPERTY)));
						UpdatePropertyGridContent();
						break;
					}

				default:
					{
						// 查找看有没有哪个OWM中创建了此窗口
						BOOL bFind = FALSE;
						for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
						{
							for(int j=0; j<theApp.m_aPlugIns[i].m_arDockingPane.GetSize(); j++)
							{
								if(theApp.m_aPlugIns[i].m_arDockingPane[j].m_nId == nPaneID)
								{
									// 找到一个OWM,执行创建函数
									CPlugIn* pPlugIn = &(theApp.m_aPlugIns[i]);
									if(!pPlugIn->IsVciOwm())
									{
										continue;
									}

									IVciOwm* pIVciOwm = theApp.m_aPlugIns[i].GetIVciOwm();
									if(pIVciOwm == NULL)
									{
										// 如果OWM组件尚未加载,则先加载
										theApp.m_aPlugIns[i].LoadOwmPlugIn();
										pIVciOwm = theApp.m_aPlugIns[i].GetIVciOwm();
									}
									if(pIVciOwm)
									{
										CWnd* pWnd = pIVciOwm->OnShowDockingPane(nPaneID, this, "");
										if(pWnd)
										{
											if(PANE_ATTACH_WND == theApp.m_aPlugIns[i].m_arDockingPane[j].m_nAttachType)
											{
												pPane->Attach(pWnd);
											}else
											{
												CWnd* pView = pPane->AttachView(this, (CRuntimeClass*)pWnd);
												if (pView && ::IsWindow(pView->m_hWnd))
												{
													pView->SendMessage(WM_INITIALUPDATE);
												}
												theApp.m_aPlugIns[i].m_arDockingPane[j].m_pWnd = pView;
											}
											pPane->SetTitle(theApp.m_aPlugIns[i].m_arDockingPane[j].m_strName);
										}
										bFind = TRUE;
										break;
									}
								}
							}
							if(bFind) break;
						}

						// 查找看有没有哪个Project中创建了此窗口
						bFind = FALSE;
						for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
						{
							for(int j=0; j<theApp.m_aPlugIns[i].m_arDockingPane.GetSize(); j++)
							{
								if( (theApp.m_aPlugIns[i].m_arDockingPane[j].m_nId == nPaneID) &&
									(theApp.m_aPlugIns[i].m_strType == "project") &&
									(theApp.m_aPlugIns[i].HasLoad()) &&
									(theApp.m_pIProject != NULL) &&									// 工程必须已经打开
									(theApp.m_aPlugIns[i].GetIProject() == theApp.m_pIProject) )	// 此插件为打开的工程插件
								{
									// 找到一个Project,加载Project对应的OWM,然后创建DockingPane
									CPlugIn* pPlugIn = theApp.GetOWM(theApp.m_aPlugIns[i].m_arDockingPane[j].m_strOwmId);
									if(pPlugIn == NULL)
									{
										continue;
									}
									IVciOwm* pIVciOwm = pPlugIn->GetIVciOwm();
									if(pIVciOwm == NULL)
									{
										// 如果OWM组件尚未加载,则先加载
										pPlugIn->LoadOwmPlugIn();
										pIVciOwm = pPlugIn->GetIVciOwm();
									}
									if(pIVciOwm)
									{
										// 增加第三个参数,用于传递一些参数信息
										CWnd* pWnd = pIVciOwm->OnShowDockingPane(nPaneID, this, "");
										if(pWnd)
										{
											if(PANE_ATTACH_WND == theApp.m_aPlugIns[i].m_arDockingPane[j].m_nAttachType)
											{
												pPane->Attach(pWnd);
											}else
											{
												CWnd* pView = pPane->AttachView(this, (CRuntimeClass*)pWnd);
												if (pView && ::IsWindow(pView->m_hWnd))
												{
													pView->SendMessage(WM_INITIALUPDATE);
												}
												theApp.m_aPlugIns[i].m_arDockingPane[j].m_pWnd = pView;
											}
											pPane->SetTitle(theApp.m_aPlugIns[i].m_arDockingPane[j].m_strName);
										}
										bFind = TRUE;
										// 调用当前打开的Project对象的OnShowDockingPane函数,用于刷新DockingPane
										// 在此处调用有问题,Project的OnShowDockingPane暂时不用
										//theApp.m_pIProject->OnShowDockingPane(nPaneID, this, "");
										// 调用DockingPane的刷新脚本
										CString strTmp;
										strTmp.Format("dockingpane\\pane_%d", nPaneID);
										theApp.ExecProjectScript(strTmp);
										break;
									}
								}
							}
							if(bFind) break;
						}

						// 查找看有没有哪个Tool中创建了此窗口
						bFind = FALSE;
						for(int i=0; i<theApp.m_aTools.GetSize(); i++)
						{
							for(int j=0; j<theApp.m_aTools[i].m_arDockingPane.GetSize(); j++)
							{
								if(theApp.m_aTools[i].m_arDockingPane[j].m_nId == nPaneID)
								{
									// 找到一个Tool,加载Tool对应的OWM,然后创建DockingPane
									CPlugIn* pPlugIn = theApp.GetOWM(theApp.m_aTools[i].m_strVciId);
									if(pPlugIn == NULL)
									{
										continue;
									}
									IVciOwm* pIVciOwm = pPlugIn->GetIVciOwm();
									if(pIVciOwm == NULL)
									{
										// 如果OWM组件尚未加载,则先加载
										pPlugIn->LoadOwmPlugIn();
										pIVciOwm = pPlugIn->GetIVciOwm();
									}
									if(pIVciOwm)
									{
										// 增加第三个参数,用于传递一些参数信息
										CWnd* pWnd = pIVciOwm->OnShowDockingPane(nPaneID, this, theApp.m_aTools[i].m_strExecCommand);
										if(pWnd)
										{
											if(PANE_ATTACH_WND == theApp.m_aTools[i].m_arDockingPane[j].m_nAttachType)
											{
												pPane->Attach(pWnd);
											}else
											{
												CWnd* pView = pPane->AttachView(this, (CRuntimeClass*)pWnd);
												if (pView && ::IsWindow(pView->m_hWnd))
												{
													pView->SendMessage(WM_INITIALUPDATE);
												}
												theApp.m_aTools[i].m_arDockingPane[j].m_pWnd = pView;
											}
											pPane->SetTitle(theApp.m_aTools[i].m_arDockingPane[j].m_strName);
										}
										bFind = TRUE;
										break;
									}
								}
							}
							if(bFind) break;
						}

					}
				}
			}
        }
        return TRUE;
    }
    return FALSE;
}

LRESULT CMainFrame::OnTaskPanelNotify(WPARAM wParam, LPARAM lParam)
{
	switch(wParam) {
	case XTP_TPN_CLICK:
		{
			CXTPTaskPanelGroupItem* pItem = (CXTPTaskPanelGroupItem*)lParam;
			TRACE(_T("Click Event: pItem.Caption = %s, pItem.ID = %i\n"), pItem->GetCaption(), pItem->GetID());
			SendMessage(WM_COMMAND, pItem->GetID(), 0);
		}
		break;

	case XTP_TPN_RCLICK:
		{				
			CXTPTaskPanelItem* pItem = (CXTPTaskPanelItem*)lParam;
			TRACE(_T("RClick Event: pItem.Caption = %s, pItem.ID = %i\n"), pItem->GetCaption(), pItem->GetID());

		}
		break;
	}

	return 0;
}

// Popup control notify
LRESULT CMainFrame::OnPopUpNotify(WPARAM wParam, LPARAM lParam)
{
	// process notify from popup wnd
	if(wParam == XTP_PCN_ITEMCLICK)
	{
		CXTPPopupItem* pItem = (CXTPPopupItem*)lParam;
		ASSERT(pItem);

		theApp.m_wndPopupControl.Close();

		if(pItem->GetID() == ID_POPUP_GOTO_SITE1)
		{
			// 点击标题行链接
			//::ShellExecute(NULL, _T("open"), theApp.m_infoPopupControl.strLinkTitle, NULL, NULL, SW_SHOW);
			theApp.RunPopupControlLink(theApp.m_infoPopupControl.strLinkTitle);
		}else
		if(pItem->GetID() == ID_POPUP_GOTO_SITE2)
		{
			// 点击文本链接
			//::ShellExecute(NULL, _T("open"), theApp.m_infoPopupControl.strLinkText, NULL, NULL, SW_SHOW);
			theApp.RunPopupControlLink(theApp.m_infoPopupControl.strLinkText);
		}else
		if((pItem->GetID() >= ID_POPUP_BUTTON_CLICK) && (pItem->GetID() < (ID_POPUP_BUTTON_CLICK+5)))
		{
			// 点击按钮
			int nIndex = pItem->GetID() - ID_POPUP_BUTTON_CLICK;
			if((nIndex < theApp.m_infoPopupControl.asButtonScript.GetSize()) && (theApp.m_infoPopupControl.asButtonScript[nIndex].GetLength() > 0))
			{
				theApp.RunPopupControlLink(theApp.m_infoPopupControl.asButtonScript[nIndex]);
			}
		}
	}
	else if(wParam == XTP_PCN_STATECHANGED)
	{
		CXTPPopupControl* pCntrol = (CXTPPopupControl*)lParam;
		ASSERT(pCntrol);

		if(pCntrol->GetPopupState() == xtpPopupStateClosed)
		{
			//EnableItems(TRUE);
		}
	}
	return TRUE;
}

LRESULT CMainFrame::OnTabbarMouseMsg(WPARAM wParam,LPARAM lParam)
{
	if (wParam != WM_RBUTTONDOWN)
		return FALSE;

	CPoint point = CPoint((DWORD)lParam);
	CXTPTabManagerItem* pItem =  m_MTIClientWnd.HitTest(point);

	if (pItem)
	{
		CWnd* pFrame = CWnd::FromHandle(pItem->GetHandle());
		MDIActivate(pFrame);

		m_MTIClientWnd.Refresh();

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_POPUP_TABS));
		CMenu* pPopup = menu.GetSubMenu(0);


		m_MTIClientWnd.WorkspaceToScreen(&point);

		CXTPCommandBars::TrackPopupMenu(pPopup, 0, point.x, point.y, this);

		m_MTIClientWnd.Refresh();

		return TRUE;
	}

	if (wParam == WM_RBUTTONDOWN)
	{
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_POPUP_WORKSPACE));
		CMenu* pPopup = menu.GetSubMenu(0);

		m_MTIClientWnd.WorkspaceToScreen(&point);
		CXTPCommandBars::TrackPopupMenu(pPopup, TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
		
		m_MTIClientWnd.Refresh();
		return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 获取扩展菜单信息指针
/////////////////////////////////////////////////////////////////////////////
CExtMenuInfo* CMainFrame::GetExtMenuInfo(UINT nID)
{
	for(int i=0; i<m_arExtMenuInfo.GetSize(); i++)
	{
		if(m_arExtMenuInfo[i].nId == nID)
		{
			return &(m_arExtMenuInfo[i]);
		}
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展菜单命令处理函数(OWM菜单)
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnOwmExCmd(UINT nID) 
{
	for(int i=0; i<m_arExtMenuInfo.GetSize(); i++)
	{
		if(m_arExtMenuInfo[i].nId == (int)(nID-IDM_OWMCMD))
		{
			if(m_arExtMenuInfo[i].pProc)
			{
				if(m_arExtMenuInfo[i].nProcType == MENU_PROC_TYPE_NORMAL)
				{
					m_arExtMenuInfo[i].pProc();
				}else
				if(m_arExtMenuInfo[i].nProcType == MENU_PROC_TYPE_PARAM_ID)
				{
					TYPEOF_ClientMenuParamId* ProcMenu = (TYPEOF_ClientMenuParamId*)(m_arExtMenuInfo[i].pProc);
					ProcMenu(nID - IDM_OWMCMD);
				}
			}
			//TRACE("OnOwmExCmd: id=%d\n", nID);
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 扩展菜单命令刷新函数(OWM菜单)
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateOwmExCmd(CCmdUI* pCmdUI)
{
	UINT nID = pCmdUI->m_nID;
	int nOID = (nID - IDM_OWMCMD);

	for(int i=0; i<m_arExtMenuInfo.GetSize(); i++)
	{
		if(m_arExtMenuInfo[i].nId == nOID)
		{
			/*
			if(m_arExtMenuInfo[i].nGrade < theApp.m_CurUser.nUserRight)
			{	// 权限控制
				pCmdUI->Enable(FALSE);
				return;
			}
			*/
			if(m_arExtMenuInfo[i].pUpdateProc)
				m_arExtMenuInfo[i].pUpdateProc(pCmdUI);
			//TRACE("OnUpdateOwmExCmd: id=%d\n", nID);
			return;
		}
	}
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////
// 平台COM接口消息
/////////////////////////////////////////////////////////////////////
LRESULT CMainFrame::OnPlatComCmd(WPARAM wParam, LPARAM lParam)
{
	theApp.PlatComCmd(wParam, lParam);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
// 平台OWM消息,某个扩展模块的动态卸载请求
/////////////////////////////////////////////////////////////////////
LRESULT CMainFrame::OnPlatOWMDynClose(WPARAM wParam, LPARAM lParam)
{
	CPlugIn* pOWM = theApp.GetOWM(wParam);
	if(pOWM)
	{
		pOWM->CloseOwmFrame();
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
// 用于跨进程传递文件名的注册消息,第二个平台进程通过此消息将文件名
// 传递给第一个平台进程
/////////////////////////////////////////////////////////////////////
LRESULT CMainFrame::OnCheckItsMe(WPARAM wParam, LPARAM lParam)
{
	// number of characters in memory-mapped file
	const DWORD dwMemoryFileSize = 1024;

	// memory-mapped file name
	const LPCTSTR sMemoryFileName = _T("DF034858-1608-4147-9025-4A0CD86F6C9F");

	ShowWindow(SW_SHOW);

	if(CCommandLineInfo::FileOpen == wParam)
	{
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

		CString strFileNames;
		TCHAR s[dwMemoryFileSize];
		lstrcpy( s, (LPTSTR) pViewOfFile);
		strFileNames = s;

		UnmapViewOfFile(pViewOfFile);
		CloseHandle(hFileMapping);

		//theApp.ShellOpenFile(strFileName);
		// 解析命令行参数,需要先解析出每一个文件名,再循环调用打开文件的方法
		CString strCommand = _T("");
		CStringArray asFile;
		int nPos = -1;
		while((nPos = strFileNames.Find("\"")) != -1)
		{
			strFileNames.Delete(0, nPos+1);
			int nPosEnd = strFileNames.Find("\"");
			if(nPosEnd != -1)
			{
				CString strFile = strFileNames.Left(nPosEnd);
				strFileNames.Delete(0, nPosEnd+1);
				if((strCommand == _T("")) && (strFile.Find(_T("CMD_") == 0)))
				{
					// 命令参数必须以CMD_开头,并且后面跟的是命令ID数字
					strCommand = strFile;
				}else
				{
					asFile.Add(strFile);
				}
			}
		}

		if(strCommand != _T(""))
		{
			// 处理命令行操作
			theApp.ShellCommand(strCommand, asFile);
		}else
		{
			// 没有命令行操作命令表示要打开文件
			for(int i=0; i<asFile.GetSize(); i++)
			{
				theApp.ShellOpenFile(asFile[i]);
			}
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnDestroy() 
{
	CMDIFrameWnd::OnDestroy();
	
	m_MTIClientWnd.Detach();	
}

void CMainFrame::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState) 
{
	// Toggle CommandBars
	GetCommandBars()->OnSetPreviewMode(bPreview);
	// Toggle Tab Client
	m_MTIClientWnd.ShowWorkspace(!bPreview);

	// Toggle Docking Panes.
	m_paneManager.OnSetPreviewMode(bPreview);
	
	CMDIFrameWnd::OnSetPreviewMode(bPreview, pState);
}

/////////////////////////////////////////////////////////////////////
// 输出窗口操作
// 0-999 为保留ID,用于指定ID方式创建窗口
// 1000以上为自动分配用的ID
static int g_nOutputID = IDR_PANE_OUTPUT+1000;

/////////////////////////////////////////////////////////////////////////////
// 获取输出窗口结构，通过标题
/////////////////////////////////////////////////////////////////////////////
COutputWndCtrl* CMainFrame::GetOutputCtrl(LPCTSTR lpszTitle)
{
	for(int i=0; i<m_aOutputCtrl.GetSize(); i++)
	{
		if(m_aOutputCtrl[i].strTitle == lpszTitle)
			return &(m_aOutputCtrl[i]);
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 获取输出窗口结构，通过ID
/////////////////////////////////////////////////////////////////////////////
COutputWndCtrl* CMainFrame::GetOutputCtrl(int nID)
{
	for(int i=0; i<m_aOutputCtrl.GetSize(); i++)
	{
		if(m_aOutputCtrl[i].nID == nID)
			return &(m_aOutputCtrl[i]);
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 删除指定输出窗口的结构
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::DeleteOutputCtrl(int nID)
{
	for(int i=0; i<m_aOutputCtrl.GetSize(); i++)
	{
		if(m_aOutputCtrl[i].nID == nID)
		{			
			if(m_aOutputCtrl[i].pWnd)
			{
				delete m_aOutputCtrl[i].pWnd;
			}
			m_aOutputCtrl.RemoveAt(i);
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////
// 创建输出窗口
/////////////////////////////////////////////////////////////////////
COutputWndCtrl* CMainFrame::CreateOutputWnd(CString strTitle, int nType, int nId, int nAttachPane)
{
	COutputWndCtrl* pOutputCtrl = GetOutputCtrl(strTitle);
	if(pOutputCtrl)
	{
		if(m_paneManager.FindPane(pOutputCtrl->nID) != NULL)
		{
			m_paneManager.ShowPane(pOutputCtrl->nID);
		}
		return pOutputCtrl;
	}

	COutputWndCtrl outputCtrl;
	if(nId == -1)
	{
		outputCtrl.nID = g_nOutputID++;
	}else
	{
		outputCtrl.nID = IDR_PANE_OUTPUT+nId;
	}

	if(strTitle == "")
	{
		// 标题为空,查找是否某个插件中创建的窗口,找到就使用
		BOOL bFind = FALSE;
		for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
		{
			for(int j=0; j<theApp.m_aPlugIns[i].m_arDockingPane.GetSize(); j++)
			{
				if(theApp.m_aPlugIns[i].m_arDockingPane[j].m_nId == outputCtrl.nID)
				{
					strTitle = theApp.m_aPlugIns[i].m_arDockingPane[j].m_strName;
					bFind = TRUE;
					break;
				}
			}
			if(bFind) break;
		}
		if(!bFind)
		{
			// 查找是否某个Tool中创建的窗口
			for(int i=0; i<theApp.m_aTools.GetSize(); i++)
			{
				for(int j=0; j<theApp.m_aTools[i].m_arDockingPane.GetSize(); j++)
				{
					if(theApp.m_aTools[i].m_arDockingPane[j].m_nId == outputCtrl.nID)
					{
						strTitle = theApp.m_aTools[i].m_arDockingPane[j].m_strName;
						bFind = TRUE;
						break;
					}
				}
				if(bFind) break;
			}
		}
	}

	outputCtrl.strTitle = strTitle;
	outputCtrl.nCtrlType = nType;
	outputCtrl.bSave = FALSE;

	if(nType == OUTPUT_RICH)
	{
		COutputCtrl* pRunCtrl = new COutputCtrl;
		outputCtrl.pWnd = pRunCtrl;
	}else
	if(nType == OUTPUT_GRID)
	{
		CReportOutCtrl* pRunCtrl = new CReportOutCtrl;
		outputCtrl.pWnd = pRunCtrl;
	}else
	if(nType == OUTPUT_TREE)
	{
		CTreeOutCtrl* pRunCtrl = new CTreeOutCtrl;
		outputCtrl.pWnd = pRunCtrl;
	}else
	{
		return NULL;
	}
	m_aOutputCtrl.Add(outputCtrl);

	if(m_paneManager.FindPane(outputCtrl.nID) != NULL)
	{
		m_paneManager.ShowPane(outputCtrl.nID);
	}

	int nFirstId = m_aOutputCtrl[0].nID;

	if(outputCtrl.nID == nFirstId)//IDR_PANE_OUTPUT)
	{
		m_paneManager.CreatePane(outputCtrl.nID, CRect(0, 0,200, 120), xtpPaneDockBottom);
	}else
	if(nAttachPane != 0)
	{
		// 创建在指定的Pane后面
		CXTPDockingPane* pBasePane = m_paneManager.FindPane(nAttachPane);
		if(pBasePane == NULL)
		{
			// 如果没有就用第一个窗口
			pBasePane = m_paneManager.FindPane(nFirstId);
		}
		CXTPDockingPane* pPane = m_paneManager.CreatePane(
			outputCtrl.nID, CRect(0, 0,300, 120), xtpPaneDockRight, pBasePane);
		if((nAttachPane != -1) && pBasePane && !pBasePane->IsClosed())
		{
			// 如果nAttachPane==-1表示不需要关联在某个窗口后面
			m_paneManager.AttachPane(pPane, pBasePane);
		}
	}else
	{
		CXTPDockingPane* pBasePane = m_paneManager.FindPane(nFirstId);//IDR_PANE_OUTPUT);
		CXTPDockingPane* pPane = m_paneManager.FindPane(outputCtrl.nID);
		if(pPane != NULL)
		{
			m_paneManager.ShowPane(outputCtrl.nID);
		}else
		{
			pPane = m_paneManager.CreatePane(
				outputCtrl.nID, CRect(0, 0,200, 120), xtpPaneDockBottom, pBasePane);
			if(pBasePane && !pBasePane->IsClosed())
			{
				m_paneManager.AttachPane(pPane, pBasePane);
			}
		}
	}

	return GetOutputCtrl(outputCtrl.nID);	
}

/////////////////////////////////////////////////////////////////////
// 创建DockingBar
// nAttachPane : 如果不为0表示创建在此Pane之后
/////////////////////////////////////////////////////////////////////
CXTPDockingPane* CMainFrame::CreateDockingBar(int nID, int nDirection, HICON hIcon, int nAttachPane)
{
	CXTPDockingPane* pPane = m_paneManager.FindPane(nID);
	if(pPane)
	{
		// 已经创建过了,只是显示窗口
		m_paneManager.ShowPane(nID);
		if(hIcon != NULL)
		{
			m_paneManager.SetIcon(nID, CXTPImageManagerIconHandle(hIcon));
		}
		return pPane;
	}

	pPane = m_paneManager.CreatePane(
			nID, CRect(0, 0,200, 120), (XTPDockingPaneDirection)nDirection);

	if(nAttachPane != 0)
	{
		CXTPDockingPane* pBasePane = m_paneManager.FindPane(nAttachPane);
		if(pBasePane && !pBasePane->IsClosed())
		{
			m_paneManager.AttachPane(pPane, pBasePane);
		}
	}

	// Set the icons for the docking pane tabs.
	if(hIcon != NULL)
	{
		m_paneManager.SetIcon(nID, CXTPImageManagerIconHandle(hIcon));
	}

	return pPane;	
}

/////////////////////////////////////////////////////////////////////
// 获取DockingPane包含的窗口对象
/////////////////////////////////////////////////////////////////////
CWnd* CMainFrame::GetDockingPaneWnd(int nPaneID)
{
	CXTPDockingPane* pPane = m_paneManager.FindPane(nPaneID);
	if(!pPane)
	{
		return NULL;
	}
	return pPane->GetChild();
}

/////////////////////////////////////////////////////////////////////
// 获取DockingPane ID(根据名字)
/////////////////////////////////////////////////////////////////////
int CMainFrame::GetDockingPaneIdByName(CString strName)
{
	POSITION pos = m_paneManager.GetPaneList().GetHeadPosition();
	while(pos)
	{
		CXTPDockingPane* pPane = m_paneManager.GetPaneList().GetNext(pos).pPane;
		if(pPane && (pPane->GetTitle() == strName))
		{
			return pPane->GetID();
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////
// 激活DockingPane窗口
/////////////////////////////////////////////////////////////////////
BOOL CMainFrame::ActiveDockingPane(int nPaneID, BOOL bShow)
{
	CXTPDockingPane* pPane = m_paneManager.FindPane(nPaneID);
	if(!pPane)
	{
		return FALSE;
	}

	if(!m_paneManager.IsPaneClosed(nPaneID))
	{
		// bShow参数表示是显示还是关闭这个Pane
		if(bShow)
		{
			m_paneManager.ShowPane(nPaneID);
		}else
		{
			m_paneManager.ClosePane(nPaneID);
		}
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
// 关闭DockingPane窗口
/////////////////////////////////////////////////////////////////////
BOOL CMainFrame::CloseDockingPane(int nPaneID)
{
	CXTPDockingPane* pPane = m_paneManager.FindPane(nPaneID);
	if(!pPane)
	{
		return FALSE;
	}

	// 查找Plugins中的DockingPane
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		for(int j=0; j<theApp.m_aPlugIns[i].m_arDockingPane.GetSize(); j++)
		{
			if(nPaneID == theApp.m_aPlugIns[i].m_arDockingPane[j].m_nId)
			{
				if(PANE_ATTACH_VIEW == theApp.m_aPlugIns[i].m_arDockingPane[j].m_nAttachType)
				{
					// 创建的View需要先释放,否则MFC会向这个已经删除的窗口发送释放消息,导致退出时异常
					if(theApp.m_aPlugIns[i].m_arDockingPane[j].m_pWnd)
					{
						theApp.m_aPlugIns[i].m_arDockingPane[j].m_pWnd->DestroyWindow();
					}
				}
				theApp.m_aPlugIns[i].m_arDockingPane.RemoveAt(j);
				m_paneManager.DestroyPane(nPaneID);
				return TRUE;
			}
		}
	}

	// 查找Tools中的DockingPane
	for(int i=0; i<theApp.m_aTools.GetSize(); i++)
	{
		for(int j=0; j<theApp.m_aTools[i].m_arDockingPane.GetSize(); j++)
		{
			if(nPaneID == theApp.m_aTools[i].m_arDockingPane[j].m_nId)
			{
				if(PANE_ATTACH_VIEW == theApp.m_aTools[i].m_arDockingPane[j].m_nAttachType)
				{
					// 创建的View需要先释放,否则MFC会向这个已经删除的窗口发送释放消息,导致退出时异常
					if(theApp.m_aTools[i].m_arDockingPane[j].m_pWnd)
					{
						theApp.m_aTools[i].m_arDockingPane[j].m_pWnd->DestroyWindow();
					}
				}
				theApp.m_aTools[i].m_arDockingPane.RemoveAt(j);
				m_paneManager.DestroyPane(nPaneID);
				return TRUE;
			}
		}
	}

	m_paneManager.DestroyPane(nPaneID);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
// 刷新属性窗口内容
/////////////////////////////////////////////////////////////////////
void CMainFrame::UpdatePropertyGridContent()
{
	CXTPPropertyGrid& wndPropertyGrid = GetPropertyGrid(this);
	if (!wndPropertyGrid.m_hWnd) return;

	if (m_bPropInUpdate)
		return;

	m_bPropInUpdate = TRUE;

	wndPropertyGrid.BeginUpdate(m_stateExpanding);
/*
	CXTPPropertyGridItem* pCategoryObjects = wndPropertyGrid.AddCategory(IDR_PANE_PROPERTIES);
	CXTPPropertyGridItem* pItemObjectsCount = pCategoryObjects->AddChildItem(new CXTPPropertyGridItemNumber(_T("Objects Count"), 0));
	pItemObjectsCount->SetReadOnly(TRUE);
	pCategoryObjects->Expand();
*/
/*
	CFrameWnd* pFrame = MDIGetActive();
	if (!pFrame)
	{
		wndPropertyGrid.EndUpdate(m_stateExpanding);
		m_bPropInUpdate = FALSE;
		return;
	}
*/
/*
	CDrawObjList* pObjects = &pView->m_selection;

	CXTPPropertyGridItem* pCategoryObjects = wndPropertyGrid.AddCategory(ID_CATEGORY_OBJECTS);
	CXTPPropertyGridItem* pItemObjectsCount = pCategoryObjects->AddChildItem(new CXTPPropertyGridItemNumber(_T("Objects Count"), (long)pView->GetDocument()->GetObjects()->GetCount()));
	pItemObjectsCount->SetReadOnly(TRUE);
	pCategoryObjects->Expand();


	CXTPPropertyGridItem* pCategorySelected = wndPropertyGrid.AddCategory(ID_CATEGORY_SELECTED);

	CXTPPropertyGridItem* pItemSelectedCount =  pCategorySelected->AddChildItem(new CXTPPropertyGridItemNumber(_T("Selected Count"), (long)pObjects->GetCount()));
	pItemSelectedCount->SetReadOnly(TRUE);
	if (pObjects->GetCount() > 0)
	{
		CDrawObj* pSelected = pObjects->GetHead();

		pCategorySelected->AddChildItem(new CXTPPropertyGridItemColor(ID_ITEM_LINECOLOR, pSelected->m_logpen.lopnColor));
		pCategorySelected->AddChildItem(new CXTPPropertyGridItemColor(ID_ITEM_FILLCOLOR, pSelected->m_logbrush.lbColor));

		if (pObjects->GetCount() == 1)
		{

			pCategorySelected->AddChildItem(new CXTPPropertyGridItemBool(ID_ITEM_SOLID, pSelected->m_bBrush));

			int nPenSize = pSelected->m_bPen ? pSelected->m_logpen.lopnWidth.x : 0;
			pCategorySelected->AddChildItem(new CXTPPropertyGridItemNumber(ID_ITEM_PENSIZE, nPenSize));
		}
	}

	pCategorySelected->Expand();

	if (pView->GetDocument()->m_pSummInfo)
	{

		CXTPPropertyGridItem* pCategorySummary = wndPropertyGrid.AddCategory(ID_CATEGORY_SUMMARY);



		pCategorySummary->AddChildItem(new CXTPPropertyGridItem(ID_ITEM_AUTHOR,
			pView->GetDocument()->m_pSummInfo->GetAuthor()));

		pCategorySummary->AddChildItem(new CXTPPropertyGridItem(ID_ITEM_TITLE,
			pView->GetDocument()->m_pSummInfo->GetTitle()));
		pCategorySummary->AddChildItem(new CXTPPropertyGridItem(ID_ITEM_SUBJECT,
			pView->GetDocument()->m_pSummInfo->GetSubject()));

		CXTPPropertyGridItem* pItemCreated =  pCategorySummary->AddChildItem(new CXTPPropertyGridItem(_T("Created"),
			pView->GetDocument()->m_pSummInfo->GetCreateDate()));
		pItemCreated->SetReadOnly(TRUE);

		pCategorySummary->Expand();

	}
*/
/*
	{
		wndPropertyGrid.GetVerbs()->Add(_T("Shape Properties"), 1);
		wndPropertyGrid.GetVerbs()->Add(_T("Paper Color"), 2);
	}
*/
	wndPropertyGrid.EndUpdate(m_stateExpanding);
	m_bPropInUpdate = FALSE;
}

/////////////////////////////////////////////////////////////////////
// 开始属性窗口刷新
/////////////////////////////////////////////////////////////////////
void CMainFrame::BeginUpdatePropertyGridContent()
{
	CXTPPropertyGrid& wndPropertyGrid = GetPropertyGrid(this);
	if (!wndPropertyGrid.m_hWnd) return;

	if (m_bPropInUpdate)
		return;

	m_bPropInUpdate = TRUE;

	wndPropertyGrid.BeginUpdate(m_stateExpanding);
}

/////////////////////////////////////////////////////////////////////
// 结束属性窗口刷新
/////////////////////////////////////////////////////////////////////
void CMainFrame::EndUpdatePropertyGridContent()
{
	CXTPPropertyGrid& wndPropertyGrid = GetPropertyGrid(this);
	if (!wndPropertyGrid.m_hWnd) return;

	if (!m_bPropInUpdate)
		return;

	wndPropertyGrid.EndUpdate(m_stateExpanding);
	m_bPropInUpdate = FALSE;
}

/////////////////////////////////////////////////////////////////////
// 属性窗口内容通知
/////////////////////////////////////////////////////////////////////
LRESULT CMainFrame::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
	CXTPPropertyGrid& wndPropertyGrid = GetPropertyGrid(this);
	if (!wndPropertyGrid.m_hWnd) return FALSE;

	if (wParam == XTP_PGN_VERB_CLICK)
	{
		// Verb栏点击事件通知
		CXTPPropertyGridVerb* pVerb = (CXTPPropertyGridVerb*)lParam;
		if(pVerb->GetID() > 10000)
		{
			// 大于10000表示是一个菜单命令
			OnCommand(pVerb->GetID(), 0);
		}else
		{
		}
		//if (pVerb->GetID() == 1) OnCommand(ID_VIEW_PROPERTIES, 0);
		//if (pVerb->GetID() == 2) OnCommand(ID_VIEW_PAPERCOLOR, 0);
		return TRUE;
	}
	if (wParam == XTP_PGN_ITEMVALUE_CHANGED)
	{
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;

		CChildFrame* pFrame = (CChildFrame*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive();
		if(pFrame)
		{
			CView* pActiveView = pFrame->GetActiveView();
			if(pActiveView)
			{
				pActiveView->SendMessage(XTPWM_PROPERTYGRID_NOTIFY, XTP_PGN_ITEMVALUE_CHANGED, lParam);
			}
		}
		return TRUE;
/*
		switch (pItem->GetID())
		{
			
			case ID_ITEM_AUTHOR: m_pView->GetDocument()->m_pSummInfo->SetAuthor(pItem->GetValue()); break;
			case ID_ITEM_SUBJECT: m_pView->GetDocument()->m_pSummInfo->SetSubject(pItem->GetValue()); break;
			case ID_ITEM_TITLE: m_pView->GetDocument()->m_pSummInfo->SetTitle(pItem->GetValue()); break;
			case ID_ITEM_PENSIZE:
				{
					CDrawObj* pSelected = m_pView->m_selection.GetHead();

					int nPenSize = min(100, ((CXTPPropertyGridItemNumber*)pItem)->GetNumber());
					if (nPenSize > 0)
					{
						pSelected->m_logpen.lopnWidth.x = nPenSize;
						pSelected->m_logpen.lopnWidth.y = nPenSize;
					}
					pSelected->Invalidate();
					pSelected->m_pDocument->SetModifiedFlag();
				}
				break;
			case ID_ITEM_SOLID:
				{
					CDrawObj* pSelected = m_pView->m_selection.GetHead();

					BOOL bSolid = min(100, ((CXTPPropertyGridItemBool*)pItem)->GetBool());
					pSelected->m_bBrush = bSolid;
					pSelected->Invalidate();
					pSelected->m_pDocument->SetModifiedFlag();
				}
				break;

			case ID_ITEM_LINECOLOR:
				{
					COLORREF color = ((CXTPPropertyGridItemColor*)pItem)->GetColor();

					POSITION pos = m_pView->m_selection.GetHeadPosition();
					while (pos != NULL)
					{
						CDrawObj* pObj = m_pView->m_selection.GetNext(pos);
						pObj->SetLineColor(color);
					}
				}
				break;
			case ID_ITEM_FILLCOLOR:
				{
					COLORREF color = ((CXTPPropertyGridItemColor*)pItem)->GetColor();

					POSITION pos = m_pView->m_selection.GetHeadPosition();
					while (pos != NULL)
					{
						CDrawObj* pObj = m_pView->m_selection.GetNext(pos);
						pObj->SetFillColor(color);
					}
				}
				break;

			default:
				TRACE(_T("Value Changed. Caption = %s, ID = %i, Value = %s\n"), pItem->GetCaption(), pItem->GetID(), pItem->GetValue());

		}
*/
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////
// 显示输出窗口
/////////////////////////////////////////////////////////////////////
void CMainFrame::OnViewOutput() 
{
	if(m_aOutputCtrl.GetSize() > 0)
	{
		int nFirstId = m_aOutputCtrl[0].nID;
		m_paneManager.ShowPane(nFirstId);//IDR_PANE_OUTPUT);
		m_paneManager.ShowPane(nFirstId+1);
		m_paneManager.ShowPane(nFirstId+2);
	}
}

/////////////////////////////////////////////////////////////////////
// 显示属性窗口
/////////////////////////////////////////////////////////////////////
void CMainFrame::OnViewProperties() 
{
	m_paneManager.ShowPane(IDR_PANE_PROPERTIES);	
}
/*
/////////////////////////////////////////////////////////////////////
// 显示查找替换窗口
/////////////////////////////////////////////////////////////////////
void CMainFrame::OnFindandreplaceFindinfiles() 
{
	CPlugIn* pPlugIn = theApp.GetOWM("org.owm.grep");
	if(pPlugIn)
	{
		pPlugIn->LoadDockingPane(10002);
	}
}
*/

void CommonNavigate(CMDIChildWnd* pFrame, CString& strUrl)
{
	if (pFrame == NULL)
	{
		//COM_CMD(VCIID_HTMLVIEW, OC_VIEWHTMLPAGE, (WPARAM)(LPCTSTR)strUrl, 0);
		theApp.OpenHtmlWindow(strUrl);
	}
	else
	{
		//COM_CMD(VCIID_HTMLVIEW, OC_VIEWHTMLPAGE, (WPARAM)(LPCTSTR)strUrl, (int)(pFrame->GetActiveView()));
		theApp.OpenHtmlWindow(strUrl, TRUE, TRUE);
	}

}

/////////////////////////////////////////////////////////////////////
// 打开网页
/////////////////////////////////////////////////////////////////////
void CMainFrame::OnGotoUrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;
	
	CXTPControlComboBox* pCombo = (CXTPControlComboBox*)tagNMCONTROL->pControl;
	ASSERT(pCombo);
	ASSERT(pCombo->GetType() == xtpControlComboBox);

	CString strUrl = pCombo->GetEditText();
	
	strUrl.TrimLeft();
	
	if (strUrl.IsEmpty())
		return;

	int nIndex = strUrl.Find(' ');
	if (nIndex != -1)
	{
		
		CString strKeyword = strUrl.Left(nIndex);/*
		CString strSearch = strUrl.Mid(nIndex + 1);
		strSearch.TrimLeft();
		for (int i = 0; i < CPageSearch::m_arrEngines.GetSize(); i++)
		{
			CPageSearch::ENGINE& engine = CPageSearch::m_arrEngines[i];
			if (engine.strKeyword.CompareNoCase(strKeyword) == 0)
			{
				strUrl = engine.strSearch;
				strUrl.Replace(_T("%s"), strSearch);
				CommonNavigate(MDIGetActive(), strUrl);
				pCombo->SetEditText(strUrl);
				return;
			} 			
		}*/
	}

	nIndex = strUrl.Find(':');
	if(nIndex >= 0) strUrl.Replace('\\','/');
	else strUrl = _T("http://") + strUrl;

	CommonNavigate(MDIGetActive(), strUrl);
	
	if (strUrl.Right(1) == "/")
		strUrl.Delete(strUrl.GetLength() - 1);

	if (pCombo->FindStringExact(0, strUrl) == CB_ERR)
		pCombo->InsertString(0, strUrl);

	*pResult = 1;
}

void CMainFrame::OnUpdateGotoUrl(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}

/////////////////////////////////////////////////////////////////////
// 刷新网页
/////////////////////////////////////////////////////////////////////
void CMainFrame::OnRefreshGotoUrl() 
{
	if(m_pComboBoxURL == NULL)
		return;

	CXTPControlComboBox* pCombo = m_pComboBoxURL;
	ASSERT(pCombo->GetType() == xtpControlComboBox);

	CString strUrl = pCombo->GetEditText();
	
	strUrl.TrimLeft();
	
	if (strUrl.IsEmpty())
		return;

	int nIndex = strUrl.Find(' ');
	if (nIndex != -1)
	{
		CString strKeyword = strUrl.Left(nIndex);
	}

	nIndex = strUrl.Find(':');
	if(nIndex >= 0) strUrl.Replace('\\','/');
	else strUrl = _T("http://") + strUrl;

	CommonNavigate(MDIGetActive(), strUrl);
	
	if (strUrl.Right(1) == "/")
		strUrl.Delete(strUrl.GetLength() - 1);

	if (pCombo->FindStringExact(0, strUrl) == CB_ERR)
		pCombo->InsertString(0, strUrl);
}

void CMainFrame::OnUpdateRefreshGotoUrl(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////
// 打开新网页窗口
/////////////////////////////////////////////////////////////////////
void CMainFrame::OnFileNewBlank() 
{
	theApp.OpenHtmlWindow("about:blank");
	CXTPControlComboBox* pCombo = (CXTPControlComboBox*)(GetCommandBars()->FindControl(xtpControlComboBox, ID_GOTO_URL, FALSE, FALSE));
	if (pCombo) pCombo->GetEditCtrl()->SetFocus();
}

/////////////////////////////////////////////////////////////////////
// 切换Tab页的位置
/////////////////////////////////////////////////////////////////////
void CMainFrame::OnToggleDock(void)
{
	if (m_MTIClientWnd.GetPaintManager()->GetPosition() != xtpTabPositionTop)
		m_MTIClientWnd.GetPaintManager()->SetPosition(xtpTabPositionTop);
	else
		m_MTIClientWnd.GetPaintManager()->SetPosition(xtpTabPositionBottom);

	m_MTIClientWnd.SaveState();
	RecalcLayout(TRUE);

}

/////////////////////////////////////////////////////////////////////
// 关闭所有窗口
/////////////////////////////////////////////////////////////////////
void CMainFrame::OnWindowCloseAll() 
{
//	if (AfxMessageBox(IDS_CONFIRM_CLOSEALL,  MB_YESNO | MB_ICONQUESTION ) != IDYES)
//		return;
//	AfxGetApp()->CloseAllDocuments(FALSE);

	// 询问每个VCI组件是否可以关闭
	// 提示保存改动过的文档(目前只支持OwmEdit)
	for(int i=theApp.m_aPlugIns.GetSize()-1; i>=0; i--)
	{
		if(!theApp.m_aPlugIns[i].CanClose())
		{
			PLAT_LOADSTRING(strStopInterp, IDS_STOP_INTERP_FIRST);
			AfxMessageBox(strStopInterp);
			//AfxMessageBox("有正在运行的窗口或解释器，请先停止其运行！");
			return;
		}
	}

	// 关闭所有OWM组件窗口
	for(int i=theApp.m_aPlugIns.GetSize()-1; i>=0; i--)
	{
		if(theApp.m_aPlugIns[i].IsVciOwm() && theApp.m_aPlugIns[i].m_pDocTemplate)
		{
			theApp.m_aPlugIns[i].m_pDocTemplate->CloseAllDocuments(FALSE);
		}
	}
}

void CMainFrame::OnUpdateWindowCloseAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((CChildFrame*)MDIGetActive() != NULL);
}

/////////////////////////////////////////////////////////////////////
// 定时器
/////////////////////////////////////////////////////////////////////
void CMainFrame::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == 1)
	{
		// 秒任务定时器
		theApp.RunTimerTask();
	}

	CMDIFrameWnd::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////
// 扩展工具菜单点击事件
/////////////////////////////////////////////////////////////////////
void CMainFrame::OnToolClick(UINT nID)
{
	int nIndex = nID - FIRST_TOOL_COMMAND;

	if (nIndex >= 0 && nIndex < theApp.m_aTools.GetSize())
	{
		for(int i=0; i<theApp.m_aTools.GetSize(); i++)
		{
			if(theApp.m_aTools[i].m_nMenuIndex == nIndex)
			{
				theApp.m_aTools[i].Run();
				return;
			}
		}		
	}
}

/////////////////////////////////////////////////////////////////////
// 导出菜单点击事件
/////////////////////////////////////////////////////////////////////
void CMainFrame::OnExportClick(UINT nID)
{
	int nIndex = nID - FIRST_EXPORT_COMMAND;

	// 获取当前文档的导出菜单列表
	CPlugIn* pPlugIn = NULL;
	CDocument* pDoc = theApp.GetOwmDocumentFromFrame(MDIGetActive(), pPlugIn);
	if(pPlugIn == NULL)
	{
		return;
	}
	IVciOwm* pIVciOwm = pPlugIn->GetIVciOwm();
	if(pIVciOwm == NULL)
	{
		return;
	}
	if(pDoc == NULL)
	{
		return;
	}

	pIVciOwm->ExportDocument(pDoc, "", nIndex);
}

CString GetValueFromSessionLine(CString strLine, CString strName)
{
	int nPos = strLine.Find(strName+"=");
	if(nPos != -1)
	{
		strLine.Delete(0, nPos+strName.GetLength()+1);
		int nPosEnd = strLine.Find(",");
		if(nPosEnd != -1)
		{
			strLine = strLine.Left(nPosEnd);
		}
		strLine.TrimLeft();
		strLine.TrimRight();
		return strLine;
	}
	return "";
}

/////////////////////////////////////////////////////////////////////////////
// 加载会话文件
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::LoadSessionFile(CString strSessionFile) 
{
	CStdioFile fSessionFile;
	if(fSessionFile.Open( strSessionFile, CFile::modeRead | CFile::typeText ))
	{
		CString strTemp;
		CDocument* pActiveDoc = NULL;
		CString strOwmId = "";
		CDocument* pDoc = NULL;
		CStringArray asSessionLine;
		while(fSessionFile.ReadString(strTemp))
		{
			if(strTemp.Find("[[[=OWMFILE=]]]") == 0)
			{
				if((strOwmId != "") && pDoc && (asSessionLine.GetSize() > 0))
				{
					CPlugIn* pPlugIn = theApp.GetOWM(strOwmId);
					if(pPlugIn)
					{
						IVciOwm* pIVciOwm = pPlugIn->GetIVciOwm();
						if(pIVciOwm)
						{
							pIVciOwm->LoadDocumentSession(pDoc, asSessionLine);
						}
					}
				}
				asSessionLine.RemoveAll();

				strOwmId = GetValueFromSessionLine(strTemp, "owmid");
				CString strFile = GetValueFromSessionLine(strTemp, "file");
				BOOL bActiveDoc = (GetValueFromSessionLine(strTemp, "active") == "true");
				if((strOwmId != "") && (strFile != ""))
				{
					if(strOwmId == VCIID_HTMLVIEW)
					{
						pDoc = theApp.OpenHtmlWindow(strFile, FALSE);
					}else
					{
						pDoc = theApp.OpenDocumentFileWithOwm(strFile, strOwmId);
					}
					if(bActiveDoc)
					{
						pActiveDoc = pDoc;
					}
				}
			}else
			{
				asSessionLine.Add(strTemp);
			}
		}

		if((strOwmId != "") && pDoc && (asSessionLine.GetSize() > 0))
		{
			CPlugIn* pPlugIn = theApp.GetOWM(strOwmId);
			if(pPlugIn)
			{
				IVciOwm* pIVciOwm = pPlugIn->GetIVciOwm();
				if(pIVciOwm)
				{
					pIVciOwm->LoadDocumentSession(pDoc, asSessionLine);
				}
			}
		}

		fSessionFile.Close();

		// 设置当前激活的视图
		if(pActiveDoc)
		{
			POSITION posview = pActiveDoc->GetFirstViewPosition();
			if(posview)
			{
				CView* pView = pActiveDoc->GetNextView(posview);
				pView->GetParentFrame()->ActivateFrame();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 保存会话文件
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::SaveSessionFile(CString strSessionFile) 
{
	CStdioFile fSessionFile;
	fSessionFile.Open( strSessionFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText );

	int nTabCount = m_MTIClientWnd.GetItemCount();
	for(int i=0; i<nTabCount; i++)
	{
		CXTPTabManagerItem* pItem = m_MTIClientWnd.GetItem(i);
		if(pItem)
		{
			CWnd* pFrame = CWnd::FromHandle(pItem->GetHandle());
			CPlugIn* pPlugIn = NULL;
			CDocument* pDoc = theApp.GetOwmDocumentFromFrame(pFrame, pPlugIn);
			if(pDoc)
			{
				IVciOwm* pIVciOwm = pPlugIn->GetIVciOwm();
				if(pIVciOwm)
				{
					CString strDocPathName = pDoc->GetPathName();
					if(pPlugIn->m_strId == VCIID_HTMLVIEW)
					{
						POSITION posview = pDoc->GetFirstViewPosition();
						if(posview)
						{
							CView* pView = pDoc->GetNextView(posview);
							int nRet = 0;
							RCOM_CMD(VCIID_HTMLVIEW, OC_GETHTMLURL, 0, (int)pView, &nRet);
							if(nRet != 0)
							{
								strDocPathName = (LPCTSTR)nRet;
								if(strDocPathName.Find("file:///") == 0)
								{
									strDocPathName.Delete(0, 8);
									strDocPathName.Replace("/", "\\");
								}
							}
						}
						// 过滤掉没有用的链接
						CString strPlatIndexFile = theApp.GetPlatRootPath() + "html\\index.htm";
						if( (strDocPathName.CompareNoCase(strPlatIndexFile) == 0) ||
							(strDocPathName.Find("pfe:open") != -1) )
						{
							strDocPathName = "";
						}
					}
					if(strDocPathName == "")
					{
						continue;
					}

					CString strTmp;
					strTmp.Format("[[[=OWMFILE=]]]owmid=%s,file=%s,%s\n",
						pPlugIn->m_strId, strDocPathName, (MDIGetActive() == pFrame) ? "active=true" : "");
					fSessionFile.WriteString(strTmp);

					CStringArray asSessionLine;
					pIVciOwm->SaveDocumentSession(pDoc, asSessionLine);
					for(int j=0; j<asSessionLine.GetSize(); j++)
					{
						strTmp.Format("%s\n", asSessionLine[j]);
						fSessionFile.WriteString(strTmp);
					}
				}
			}
		}
	}

	fSessionFile.Close();
}
