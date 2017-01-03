// TpsUIStyle.cpp : implementation file
//

#include "stdafx.h"
#include "BlueUI.h"
#include "TpsUIStyle.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsUIStyle dialog


CTpsUIStyle::CTpsUIStyle(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsUIStyle::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsUIStyle)
	m_bApplyMetrics = theApp.GetProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_APPLY_METRICS, TRUE);
	m_bApplyFrame = theApp.GetProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_APPLY_FRAMESKIN, TRUE);
	m_bApplyColors = theApp.GetProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_APPLY_COLORS, TRUE);
	m_nTheme = theApp.GetProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_THEME, themeVisualStudio2010);
	m_nThemeColor = theApp.GetProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_THEMECOLOR, themeColorBlue);
	//}}AFX_DATA_INIT
}


void CTpsUIStyle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsUIStyle)
	DDX_Control(pDX, IDC_COMBO_HTML_STYLE, m_cmbHtmlStyle);
	DDX_Control(pDX, IDC_COMBO_COLORS, m_cmbColors);
	DDX_Control(pDX, IDC_COMBO_FONTS, m_cmbFonts);
	DDX_Control(pDX, IDC_LIST_THEMES, m_lboxSkins);
	DDX_Check(pDX, IDC_CHECK_APPLYMETRICS, m_bApplyMetrics);
	DDX_Check(pDX, IDC_CHECK_APPLYFRAME, m_bApplyFrame);
	DDX_Check(pDX, IDC_CHECK_APPLYCOLORS, m_bApplyColors);
	DDX_Radio(pDX, IDC_RADIO_RIBBON, m_nTheme);
	DDX_Radio(pDX, IDC_RADIO_OFFICE_BLUE, m_nThemeColor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsUIStyle, CDialog)
	//{{AFX_MSG_MAP(CTpsUIStyle)
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_LIST_THEMES, OnSelChangeListThemes)
	ON_CBN_SELCHANGE(IDC_COMBO_COLORS, OnSelChangeComboColors)
	ON_CBN_SELCHANGE(IDC_COMBO_FONTS, OnSelChangeComboFonts)
	ON_BN_CLICKED(IDC_CHECK_APPLYMETRICS, OnCheckApplyMetrics)
	ON_BN_CLICKED(IDC_CHECK_APPLYFRAME, OnCheckApplyFrame)
	ON_BN_CLICKED(IDC_CHECK_APPLYCOLORS, OnCheckApplyColors)
	ON_BN_CLICKED(IDC_RADIO_RIBBON, OnThemeChanged)
	ON_BN_CLICKED(IDC_RADIO_OFFICE2003, OnThemeChanged)
	ON_BN_CLICKED(IDC_RADIO_OFFICE2007, OnThemeChanged)
	ON_BN_CLICKED(IDC_RADIO_OFFICE2010, OnThemeChanged)
	ON_BN_CLICKED(IDC_RADIO_VS2005, OnThemeChanged)
	ON_BN_CLICKED(IDC_RADIO_VS2008, OnThemeChanged)
	ON_BN_CLICKED(IDC_RADIO_VS2010, OnThemeChanged)
	ON_BN_CLICKED(IDC_RADIO_OFFICE_BLUE, OnThemeColorChanged)
	ON_BN_CLICKED(IDC_RADIO_OFFICE_SILVER, OnThemeColorChanged)
	ON_BN_CLICKED(IDC_RADIO_OFFICE_BLACK, OnThemeColorChanged)
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsUIStyle message handlers

void CTpsUIStyle::EnumerateThemeColors(CXTPSkinManagerResourceFile* pFile, LPCTSTR lpszResourcePath, LPCTSTR lpszThemeName) 
{
	THEME* pTheme= new THEME;
	pTheme->strResourcePath = lpszResourcePath;

	CString str;
	CString strIniFile;
	CString csColorSchemes;
	CString csSizes;

	while(pFile->ReadString(str))
	{
		str.TrimLeft();
		if (str.IsEmpty())
			continue;
		
		if (str[1] == _T(';'))
			continue;

		if (str.Left(5).CompareNoCase(_T("[file")) == 0)
		{
			strIniFile = str.Mid(6, str.GetLength() - 7) + _T(".ini");
			csColorSchemes.Empty();
			csSizes.Empty();
		}
		else if (str[1] == _T('['))
		{
			strIniFile.Empty();
			csColorSchemes.Empty();
			csSizes.Empty();
		}
		else if (!strIniFile.IsEmpty())
		{
			int nPos = str.Find(_T('='));
			if (nPos > 0 && str.Left(12).CompareNoCase(_T("ColorSchemes")) == 0)
			{
				csColorSchemes = str.Mid(nPos + 1);
				csColorSchemes.TrimLeft();
			}
			if (nPos > 0 && str.Left(5).CompareNoCase(_T("Sizes")) == 0)
			{
				csSizes = str.Mid(nPos + 1);
				csSizes.TrimLeft();
			}

			if (!csSizes.IsEmpty() && !csColorSchemes.IsEmpty())
			{
				THEMEINFO* pThemeInfo = new THEMEINFO();
				pThemeInfo->csIniFileName = strIniFile;
				pThemeInfo->csColorSchemes = csColorSchemes;
				pThemeInfo->csSizes = csSizes;
				pThemeInfo->pTheme = pTheme;
				pTheme->m_arrThemes.Add(pThemeInfo);

				strIniFile.Empty();
			}
		}
	}

	if (pTheme->m_arrThemes.GetSize() == 0)
	{
		delete pTheme;
		return;
	}
	
	int nIndex = m_lboxSkins.AddString(lpszThemeName);
	m_lboxSkins.SetItemDataPtr(nIndex, pTheme);
	m_asSkinName.Add(lpszThemeName);
}

void CTpsUIStyle::EnumerateThemes(CString strResourcePath) 
{
	CXTPSkinManagerResourceFile f;
	if (!f.Open(strResourcePath, _T("Themes.ini")))
		return;

	TCHAR szSkinName[256];
	if (::LoadString(f.GetModuleHandle(), 5000, szSkinName, 256))
	{
		EnumerateThemeColors(&f, strResourcePath, szSkinName);
		return;
	}
}

void CTpsUIStyle::EnumerateIniThemes(CString strResourcePath)
{
	THEME* pTheme= new THEME;
	pTheme->strResourcePath = strResourcePath;

	CString strThemeName = strResourcePath;
	strThemeName = strThemeName.Left(strThemeName.GetLength()-10);
	int nPos = strThemeName.ReverseFind('\\');
	if(nPos !=-1)
	{
		strThemeName.Delete(0, nPos+1);
	}

	CFileFind f;
	BOOL bNext = f.FindFile(strResourcePath + _T("*.ini"));
	while(bNext)
	{
		bNext = f.FindNextFile();
		if (f.IsDots())
			continue;

		if (f.IsDirectory())
			continue;

		THEMEINFO* pThemeInfo = new THEMEINFO();
		pThemeInfo->csIniFileName = f.GetFileName();
		pThemeInfo->csColorSchemes = f.GetFileTitle();
		pThemeInfo->csSizes = "";
		pThemeInfo->pTheme = pTheme;
		pTheme->m_arrThemes.Add(pThemeInfo);
	}

	if (pTheme->m_arrThemes.GetSize() == 0)
	{
		delete pTheme;
		return;
	}
	
	int nIndex = m_lboxSkins.AddString(strThemeName);
	m_lboxSkins.SetItemDataPtr(nIndex, pTheme);
	m_asSkinName.Add(strThemeName);
}

void CTpsUIStyle::FindThemes(CString strPath, BOOL bRecurse)
{
	CFileFind f;
	BOOL bNext = f.FindFile(strPath + _T("*.*"));
	while(bNext)
	{
		bNext = f.FindNextFile();
		if (f.IsDots())
			continue;

		if (f.IsDirectory())
		{
			if (bRecurse)
			{
				FindThemes(f.GetFilePath() + "\\", TRUE);
			}else
			{
				// 路径------------------------------
				CString strExtension = f.GetFilePath().Right(9);
				if (strExtension.CompareNoCase(_T(".cjstyles")) == 0)
				{
					EnumerateIniThemes(f.GetFilePath() + "\\");
				}
			}
		}
		else 
		{
			CString strExtension = f.GetFileName().Right(9);

			if ((strExtension.CompareNoCase(_T(".msstyles")) == 0) || (strExtension.CompareNoCase(_T(".cjstyles")) == 0))
			{
				EnumerateThemes(f.GetFilePath());
			}
		}
	}

	// 添加一个不使用皮肤的纪录
	THEME* pTheme= new THEME;
	pTheme->strResourcePath = "";
	CString strThemeName = "Not use skin";
	THEMEINFO* pThemeInfo = new THEMEINFO();
	pThemeInfo->csIniFileName = "";
	pThemeInfo->csColorSchemes = "";
	pThemeInfo->csSizes = "";
	pThemeInfo->pTheme = pTheme;
	pTheme->m_arrThemes.Add(pThemeInfo);
	int nIndex = m_lboxSkins.AddString(strThemeName);
	m_lboxSkins.SetItemDataPtr(nIndex, pTheme);
	m_asSkinName.Add(strThemeName);
	
	m_lboxSkins.SetCurSel(-1);
}

void CTpsUIStyle::ReleaseThemes()
{
	for (int i = 0; i < m_lboxSkins.GetCount(); i++)
	{
		THEME* pTheme = (THEME*)m_lboxSkins.GetItemDataPtr(i);

		for (int j = 0; j < pTheme->m_arrThemes.GetSize(); j++)
		{	
			delete pTheme->m_arrThemes[j];
		}		

		delete pTheme;
	}		

	m_lboxSkins.ResetContent();
	m_cmbFonts.ResetContent();
	m_cmbColors.ResetContent();
}

void CTpsUIStyle::ReloadThemes()
{
	UpdateData();

	ReleaseThemes();

	CString csStylesPath = theApp.GetPlatRootPath();
	csStylesPath += "\\Skins\\";
	FindThemes(csStylesPath, FALSE);

	CString strSkinFile = theApp.GetProfileString(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_FILE);
	for (int i = 0; i < m_lboxSkins.GetCount(); i++)
	{
		THEME* pTheme = (THEME*)m_lboxSkins.GetItemDataPtr(i);

		if(strSkinFile.CompareNoCase(pTheme->strResourcePath) == 0)
		{
			m_lboxSkins.SetCurSel(i);

			CString strColorSchemes = theApp.GetProfileString(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_COLORSCHEMES);
			CString strSizes = theApp.GetProfileString(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_FONTSIZE);

			// 修改为初始化时候仅加载皮肤信息，不用更新一次皮肤的显示，可以节省时间
			InitListThemes();

			m_cmbColors.SelectString(0, strColorSchemes);
			m_cmbFonts.SelectString(0, strSizes);

			theApp.WriteProfileString(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_COLORSCHEMES, strColorSchemes);
			theApp.WriteProfileString(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_FONTSIZE, strSizes);

			break;
		}
	}
}

void CTpsUIStyle::SetTheme(int nColor, int nFont)
{
	UpdateData();

	if (nColor == LB_ERR)
		return;

	THEMEINFO* pThemeInfo = (THEMEINFO*)m_cmbColors.GetItemDataPtr(nColor);

	CString csColorSchemes = pThemeInfo->csColorSchemes;
	CString csSizes;

	if (nFont != LB_ERR && m_bApplyMetrics)
	{
		THEMEINFO* pThemeInfo = (THEMEINFO*)m_cmbFonts.GetItemDataPtr(nFont);
		csSizes = pThemeInfo->csSizes;
	}

	THEME* pTheme = pThemeInfo->pTheme;

	for (int i = 0; i < pTheme->m_arrThemes.GetSize(); i++)
	{
		THEMEINFO* pThemeInfo = pTheme->m_arrThemes[i];
		if (pThemeInfo->csColorSchemes == csColorSchemes && 
			(csSizes.IsEmpty() || pThemeInfo->csSizes == csSizes))
		{
			XTPSkinManager()->LoadSkin(pTheme->strResourcePath, pThemeInfo->csIniFileName);

			// 保存注册表
			theApp.WriteProfileString(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_FILE, pTheme->strResourcePath);
			theApp.WriteProfileString(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_INI, pThemeInfo->csIniFileName);
			theApp.WriteProfileString(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_COLORSCHEMES, csColorSchemes);
			theApp.WriteProfileString(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_FONTSIZE, csSizes);

			break;
		}		
	}

	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	pWnd->OnSkinChanged();
	pWnd->RedrawFrame(this);	
}

// 加载Html风格信息
void CTpsUIStyle::ReloadHtmlStyle()
{
	m_asHtmlTemplate.RemoveAll();
	m_cmbHtmlStyle.ResetContent();

	CString strCurrentTemplate = theApp.GetProfileString(REG_UISTYLE_SUBKEY, REG_UISTYLE_HTML, "default");
	CString strCurrentHtmlStyle = "";

	CString strPath = theApp.GetPlatRootPath();
	strPath += "\\template\\html\\";

	CFileFind f;
	BOOL bNext = f.FindFile(strPath + _T("*.*"));
	while(bNext)
	{
		bNext = f.FindNextFile();
		if (f.IsDots())
			continue;

		if (!f.IsDirectory())
			continue;

		CString strPlugin = f.GetFilePath() + "\\";
		if(LANGUAGE_PAGE_CHINESE == theApp.m_curLanguage)
		{
			strPlugin += "plugin_cn.xml";
		}else
		{
			strPlugin += "plugin.xml";
		}

		CXmlParser xmlPlugin;
		xmlPlugin.Open(strPlugin);
		CString strTemplateName = xmlPlugin.GetNodeAttribute("Template", "name");

		m_cmbHtmlStyle.AddString(strTemplateName);
		m_asHtmlTemplate.Add(f.GetFileTitle());

		if(strCurrentTemplate.CompareNoCase(f.GetFileTitle()) == 0)
		{
			strCurrentHtmlStyle = strTemplateName;
		}
	}

	m_cmbHtmlStyle.SelectString(0, strCurrentHtmlStyle);
}

BOOL CTpsUIStyle::OnInitDialog() 
{
	CDialog::OnInitDialog();

	ReloadThemes();

	ReloadHtmlStyle();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTpsUIStyle::OnOK() 
{
	UpdateData(TRUE);

	int nSelectHtmlStyle = m_cmbHtmlStyle.GetCurSel();
	CString strHtmlStyle = m_asHtmlTemplate.GetAt(nSelectHtmlStyle);
	theApp.WriteProfileString(REG_UISTYLE_SUBKEY, REG_UISTYLE_HTML, strHtmlStyle);

	CDialog::OnOK();
}

void CTpsUIStyle::OnDestroy() 
{
	ReleaseThemes();

	CDialog::OnDestroy();	
}

// 仅初始化Skin的Theme列表
void CTpsUIStyle::InitListThemes()
{
	m_cmbColors.ResetContent();
	m_cmbFonts.ResetContent();

	int nCurSel = m_lboxSkins.GetCurSel();
	if (nCurSel == LB_ERR)
		return;

	THEME* pTheme = (THEME*)m_lboxSkins.GetItemDataPtr(nCurSel);

	for (int i = 0; i < pTheme->m_arrThemes.GetSize(); i++)
	{
		THEMEINFO* pThemeInfo = pTheme->m_arrThemes[i];


		if (m_cmbColors.FindStringExact(-1, pThemeInfo->csColorSchemes) == LB_ERR)
		{
			int nIndex = m_cmbColors.AddString(pThemeInfo->csColorSchemes);
			m_cmbColors.SetItemDataPtr(nIndex, pThemeInfo);
		}		

		if (m_cmbFonts.FindStringExact(-1, pThemeInfo->csSizes) == LB_ERR)
		{
			int nIndex = m_cmbFonts.AddString(pThemeInfo->csSizes);
			m_cmbFonts.SetItemDataPtr(nIndex, pThemeInfo);
		}		
	}	
	
	m_cmbColors.SetCurSel(0);
	m_cmbFonts.SetCurSel(0);
}

void CTpsUIStyle::OnSelChangeListThemes() 
{
	// 皮肤列表中选择项变更的时候更新一下皮肤信息,同时更新皮肤的显示
	InitListThemes();
	OnSelChangeComboColors();
}

void CTpsUIStyle::OnSelChangeComboColors() 
{
	SetTheme(m_cmbColors.GetCurSel(), m_cmbFonts.GetCurSel());
}

void CTpsUIStyle::OnSelChangeComboFonts() 
{
	SetTheme(m_cmbColors.GetCurSel(), m_cmbFonts.GetCurSel());
}

void CTpsUIStyle::OnCheckApplyMetrics() 
{
	UpdateData();
	
	DWORD dwApplyOptions = XTPSkinManager()->GetApplyOptions();

	if (m_bApplyMetrics)	
		XTPSkinManager()->SetApplyOptions(dwApplyOptions | xtpSkinApplyMetrics);
	else
		XTPSkinManager()->SetApplyOptions(dwApplyOptions & ~xtpSkinApplyMetrics);

	m_cmbFonts.EnableWindow(m_bApplyMetrics);

	theApp.WriteProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_APPLY_METRICS, m_bApplyMetrics);
}

void CTpsUIStyle::OnCheckApplyFrame() 
{
	UpdateData();
	
	DWORD dwApplyOptions = XTPSkinManager()->GetApplyOptions();

	if (m_bApplyFrame)	
		XTPSkinManager()->SetApplyOptions(dwApplyOptions | xtpSkinApplyFrame);
	else
		XTPSkinManager()->SetApplyOptions(dwApplyOptions & ~xtpSkinApplyFrame);

	theApp.WriteProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_APPLY_FRAMESKIN, m_bApplyFrame);
}

void CTpsUIStyle::OnCheckApplyColors() 
{
	UpdateData();
	
	DWORD dwApplyOptions = XTPSkinManager()->GetApplyOptions();

	if (m_bApplyColors)	
		XTPSkinManager()->SetApplyOptions(dwApplyOptions | xtpSkinApplyColors);
	else
		XTPSkinManager()->SetApplyOptions(dwApplyOptions & ~xtpSkinApplyColors);

	theApp.WriteProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_APPLY_COLORS, m_bApplyColors);
}

// 更改界面主题
void CTpsUIStyle::OnThemeChanged() 
{
	UpdateData();
	
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	pWnd->SetTheme(m_nTheme, m_nThemeColor);

	theApp.WriteProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_THEME, m_nTheme);
}

// 更改界面主题颜色
void CTpsUIStyle::OnThemeColorChanged() 
{
	UpdateData();
	
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();	
	pWnd->SetTheme(m_nTheme, m_nThemeColor);

	theApp.WriteProfileInt(REG_CONFIG_SUBKEY, REG_CONFIG_SKIN_THEMECOLOR, m_nThemeColor);
}

void CTpsUIStyle::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS) 
{
	if(nIDCtl == IDC_LIST_THEMES)
	{
		if (lpDIS->itemID == LB_ERR) return; 
  
		HBRUSH		brBackground;
		RECT		rcTemp = lpDIS->rcItem; 
		HDC			hDC=lpDIS->hDC;

		COLORREF	clText; 

		if (lpDIS->itemState & ODS_SELECTED)  { 
			brBackground = GetSysColorBrush (COLOR_HIGHLIGHT); 
			clText = GetSysColor (COLOR_HIGHLIGHTTEXT); 
		} else { 
			brBackground = GetSysColorBrush (COLOR_WINDOW); 
			clText = GetSysColor (COLOR_WINDOWTEXT); 
		} 

		if (lpDIS->itemAction & (ODA_DRAWENTIRE | ODA_SELECT)) 	FillRect(hDC,&rcTemp, brBackground); 
	
		int			OldBkMode=::SetBkMode(hDC,TRANSPARENT); 
		COLORREF	clOldText=::SetTextColor(hDC,clText); 

		// 获取当前项索引
		int item = lpDIS->itemID;

		TCHAR		szBuf[100];
		strcpy(szBuf, m_asSkinName[item]);

		int h=rcTemp.bottom-rcTemp.top;
		rcTemp.left+=h+4;
		DrawText(hDC,szBuf,-1,&rcTemp,DT_LEFT|DT_VCENTER|	
			DT_NOPREFIX| DT_SINGLELINE);

		HICON hIcon = AfxGetApp()->LoadIcon(IDI_DISPLAY);
		rcTemp.left=lpDIS->rcItem.left;
		::DrawIconEx(hDC,rcTemp.left+2,rcTemp.top,			
			hIcon,h,h,0,0,DI_NORMAL);		
		
		::SetTextColor(hDC,clOldText);
		::SetBkMode(hDC,OldBkMode);
  
		if(lpDIS->itemAction & ODA_FOCUS)   DrawFocusRect(hDC,&lpDIS->rcItem); 
		return;		
	}

	CDialog::OnDrawItem(nIDCtl, lpDIS);
}
