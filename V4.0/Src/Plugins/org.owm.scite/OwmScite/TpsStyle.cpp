// TpsStyle.cpp : implementation file
//

#include "stdafx.h"
#include "OwmScite.h"
#include "TpsStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsStyle dialog


CTpsStyle::CTpsStyle(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsStyle::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsStyle)
	m_sFont = _T("");
	m_sSize = _T("");
	m_bBold = FALSE;
	m_bItalic = FALSE;
	m_bUnderLine = FALSE;
	m_bUseGlobalFg = FALSE;
	m_bUseGlobalBg = FALSE;
	m_bUseGlobalFont = FALSE;
	//}}AFX_DATA_INIT

	_isDirty = false;
	_isSync = true;
	_isShownGOCtrls = false;

	LOGFONT* plogfont = NULL;

	LOGFONT logfont;
	CWindowDC dc(GetDesktopWindow() );
	m_cyPixelsPerInch = GetDeviceCaps(dc, LOGPIXELSY);
	if( plogfont == NULL )
	{
		CFont *pfont = dc.GetCurrentFont();
		pfont->GetLogFont( &logfont );
		plogfont = &logfont;
	}
	m_fontSample.CreateFontIndirect( plogfont );

	m_bUnderLine = plogfont->lfUnderline;
	m_sFont = plogfont->lfFaceName;
	m_sSize.Format( "%d", MulDiv(plogfont->lfHeight, 72, m_cyPixelsPerInch) );
	m_clrFg = RGB(0,0,0);
}


void CTpsStyle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsStyle)
	DDX_Control(pDX, IDC_SAMPLEBOX, m_staticSample);
	DDX_Control(pDX, IDC_LIST_ITEM, m_ListItem);
	DDX_Control(pDX, IDC_COLORBK_PICKER, m_ColorbkPicker);
	DDX_Control(pDX, IDC_COLORFG_PICKER, m_ColorfgPicker);
	DDX_Control(pDX, IDC_COMBO_SIZE, m_comboSize);
	DDX_Control(pDX, IDC_COMBO_FONT, m_comboFont);
	DDX_Control(pDX, IDC_COMBO_LANGUAGE, m_comboLanguage);
	DDX_Control(pDX, IDC_COMBO_THEME, m_comboTheme);
	DDX_CBString(pDX, IDC_COMBO_FONT, m_sFont);
	DDX_CBString(pDX, IDC_COMBO_SIZE, m_sSize);
	DDX_Check(pDX, IDC_CHECK_BOLD, m_bBold);
	DDX_Check(pDX, IDC_CHECK_ITALIC, m_bItalic);
	DDX_Check(pDX, IDC_CHECK_UNDERLINE, m_bUnderLine);
	DDX_Check(pDX, IDC_CHECK_GLOBAL_FG, m_bUseGlobalFg);
	DDX_Check(pDX, IDC_CHECK_GLOBAL_BG, m_bUseGlobalBg);
	DDX_Check(pDX, IDC_CHECK_GLOBAL_FONT, m_bUseGlobalFont);
	//}}AFX_DATA_MAP
	DDX_XTColorPicker(pDX, IDC_COLORFG_PICKER, m_clrFg);
	DDX_XTColorPicker(pDX, IDC_COLORBK_PICKER, m_clrBk);
}


BEGIN_MESSAGE_MAP(CTpsStyle, CDialog)
	//{{AFX_MSG_MAP(CTpsStyle)
	ON_WM_DRAWITEM()
	ON_CBN_SELENDOK(IDC_COMBO_FONT, OnSelendokFontCombo)
	ON_LBN_SELCHANGE(IDC_LIST_ITEM, OnSelchangeListItem)
	ON_CBN_SELCHANGE(IDC_COMBO_LANGUAGE, OnSelchangeComboLanguage)
	ON_BN_CLICKED(IDC_BTN_STYLERESET, OnBtnStylereset)
	ON_BN_CLICKED(IDC_CHECK_BOLD, OnCheckBold)
	ON_BN_CLICKED(IDC_CHECK_ITALIC, OnCheckItalic)
	ON_BN_CLICKED(IDC_CHECK_UNDERLINE, OnCheckUnderline)
	ON_CBN_SELCHANGE(IDC_COMBO_SIZE, OnSelchangeComboSize)
	ON_BN_CLICKED(IDC_CHECK_GLOBAL_FG, OnCheckGlobalFg)
	ON_BN_CLICKED(IDC_CHECK_GLOBAL_BG, OnCheckGlobalBg)
	ON_BN_CLICKED(IDC_CHECK_GLOBAL_FONT, OnCheckGlobalFont)
	ON_CBN_KILLFOCUS(IDC_COMBO_SIZE, OnSelchangeComboSize)
	ON_EN_CHANGE(IDC_USER_KEYWORDS_EDIT, OnChangeUserKeywordsEdit)
	//}}AFX_MSG_MAP
	ON_CPN_XT_SELENDOK(IDC_COLORFG_PICKER, OnFgColorChange)
	ON_CPN_XT_SELENDOK(IDC_COLORBK_PICKER, OnBkColorChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsStyle message handlers

BOOL CTpsStyle::OnInitDialog() 
{
	CDialog::OnInitDialog();

	prepare2Cancel();

	m_comboTheme.DisableFlatLook(TRUE);
	m_comboTheme.SetCurSel(0);
	m_comboLanguage.DisableFlatLook(TRUE);

	//m_comboFont.DisableFlatLook(TRUE);
	m_comboFont.SetListStyle(xtFontSample);
	m_comboFont.InitControl();

	m_comboSize.DisableFlatLook(TRUE);

	// Fill Size combobox with "common" sizes
	TCHAR* Defaults[] = { _T(""), _T("8"), _T("9"), _T("10"), _T("11"), _T("12"), _T("14"),
				_T("16"), _T("18"), _T("20"), _T("22"), _T("24"), _T("26"), 
				_T("28"), _T("36"), _T("48") };
	for (int i = 0; i < (sizeof(Defaults)/sizeof(Defaults[0])); i++)
	{
		m_comboSize.AddString(Defaults[i]);
	}

	UpdateData(FALSE);

	_lsArray = (NppParameters::getInstance())->getLStylerArray();
    _globalStyles = (NppParameters::getInstance())->getGlobalStylers();
	
	m_comboLanguage.AddString("Global Styles");
	// All the lexers
    for (int i = 0 ; i < _lsArray.getNbLexer() ; i++)
    {
		m_comboLanguage.AddString(_lsArray.getLexerDescFromIndex(i));
    }
	m_comboLanguage.SetCurSel(0);

	const int index2Begin = 0;
	setStyleListFromLexer(index2Begin);

	setVisualFromStyleList();

	OnSelchangeListItem();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTpsStyle::OnOK() 
{
	if (_isDirty)
	{
		LexerStylerArray & lsa = (NppParameters::getInstance())->getLStylerArray();
		StyleArray & globalStyles = (NppParameters::getInstance())->getGlobalStylers();

		lsa = _lsArray;
		globalStyles = _globalStyles;

		//::EnableWindow(::GetDlgItem(_hSelf, IDOK), FALSE);
		_isDirty = false;
	}
	(NppParameters::getInstance())->writeStyles(_lsArray, _globalStyles);
	//::EnableWindow(::GetDlgItem(_hSelf, IDC_SAVECLOSE_BUTTON), FALSE);
	_isSync = true;

	CDialog::OnOK();
}

// 取消
void CTpsStyle::OnCancel() 
{
	if (_isDirty)
	{
		LexerStylerArray & lsArray = (NppParameters::getInstance())->getLStylerArray();
		StyleArray & globalStyles = (NppParameters::getInstance())->getGlobalStylers();
		
		globalStyles = _globalStyles = _gstyles2restored;
		lsArray = _lsArray = _styles2restored;
		restoreGlobalOverrideValues();

		_isDirty = false;
		//setVisualFromStyleList();
		OnSelchangeListItem();
		theApp.RefreshAllSciteView();
	}

	CDialog::OnCancel();
}

// 恢复为缺省
void CTpsStyle::OnBtnStylereset() 
{
	if (_isDirty)
	{
		LexerStylerArray & lsArray = (NppParameters::getInstance())->getLStylerArray();
		StyleArray & globalStyles = (NppParameters::getInstance())->getGlobalStylers();
		
		globalStyles = _globalStyles = _gstyles2restored;
		lsArray = _lsArray = _styles2restored;
		restoreGlobalOverrideValues();

		_isDirty = false;
		//setVisualFromStyleList();
		OnSelchangeListItem();
		theApp.RefreshAllSciteView();
	}
	//::EnableWindow(::GetDlgItem(_hSelf, IDC_SAVECLOSE_BUTTON), !_isSync);
}

void CTpsStyle::prepare2Cancel()
{
	_styles2restored = (NppParameters::getInstance())->getLStylerArray();
	_gstyles2restored = (NppParameters::getInstance())->getGlobalStylers();
	_gOverride2restored = (NppParameters::getInstance())->getGlobalOverrideStyle();
}

void CTpsStyle::apply()
{
	LexerStylerArray & lsa = (NppParameters::getInstance())->getLStylerArray();
	StyleArray & globalStyles = (NppParameters::getInstance())->getGlobalStylers();

	lsa = _lsArray;
	globalStyles = _globalStyles;

	//::EnableWindow(::GetDlgItem(_hSelf, IDOK), FALSE);
	_isSync = false;
	theApp.RefreshAllSciteView();
	_isDirty = true;
}

// 获取当前格式项
Style & CTpsStyle::getCurrentStyler()
{
	int styleIndex = m_ListItem.GetCurSel();
    if (_currentLexerIndex == 0)
        return _globalStyles.getStyler(styleIndex);
    else
    {
		LexerStyler & lexerStyler = _lsArray.getLexerFromIndex(_currentLexerIndex - 1);
		return lexerStyler.getStyler(styleIndex);
    }
}

// 获取全局格式项
Style & CTpsStyle::getGlobalStyler(int nIndex)
{
    return _globalStyles.getStyler(nIndex);
}

void CTpsStyle::setVisualFromStyleList() 
{
//	showGlobalOverrideCtrls(false);

    Style & style = getCurrentStyler();
/*
	// Global override style
	if (lstrcmp(style._styleDesc, TEXT("Global override")) == 0)
	{
		showGlobalOverrideCtrls(true);
	}

	COLORREF c = c = RGB(0x00, 0x00, 0xFF);
	TCHAR str[256];

	str[0] = '\0';
	
	int i = m_comboLanguage.GetCurSel();
	if (i == LB_ERR)
		return;
	m_comboLanguage.GetLBText(i, str);

	i = m_ListItem.GetCurSel();
	if (i == LB_ERR)
		return;
	TCHAR styleName[64];
	m_ListItem.GetText(i, styleName);

	lstrcat(lstrcat(str, TEXT(" : ")), styleName);

	// PAD for fix a display glitch
	lstrcat(str, TEXT("          "));
	//colourHooker.setColour(c);
	//::SetWindowText(_hStyleInfoStaticText, str);

	//-- 2 couleurs : fg et bg
	bool isEnable = false;
	if (HIBYTE(HIWORD(style._fgColor)) != 0xFF)
	{
		m_clrFg		= style._fgColor;
		m_staticSample.SetTextColor(style._fgColor);
		isEnable = true;
	}
	GetDlgItem(IDC_STATIC_COLORFG)->EnableWindow(isEnable);
	GetDlgItem(IDC_COLORFG_PICKER)->EnableWindow(isEnable);

	isEnable = false;
	if (HIBYTE(HIWORD(style._bgColor)) != 0xFF)
	{
		m_clrBk		= style._bgColor;
		m_staticSample.SetBkColor(style._bgColor);
		isEnable = true;
	}
	GetDlgItem(IDC_STATIC_COLORBK)->EnableWindow(isEnable);
	GetDlgItem(IDC_COLORBK_PICKER)->EnableWindow(isEnable);

	//-- font name
	isEnable = false;
	if (style._fontName != NULL)
	{
		m_sFont		= style._fontName;
		m_staticSample.SetFontName(m_sFont);
		isEnable = true;
	}
	GetDlgItem(IDC_STATIC_FONT)->EnableWindow(isEnable);
	GetDlgItem(IDC_COMBO_FONT)->EnableWindow(isEnable);

	//-- font size
	isEnable = false;
	TCHAR intStr[5] = TEXT("");
	int iFontSize = 0;
	if (style._fontSize != -1)
	{
		m_sSize.Format("%d", style._fontSize);
		m_staticSample.SetFontSize(atoi(m_sSize));
		isEnable = true;
	}
	GetDlgItem(IDC_STATIC_SIZE)->EnableWindow(isEnable);
	GetDlgItem(IDC_COMBO_SIZE)->EnableWindow(isEnable);

	//-- font style : bold et italic
	isEnable = false;
	if (style._fontStyle != -1)
	{
		m_bBold = (style._fontStyle & FONTSTYLE_BOLD) != 0;
		m_bItalic = (style._fontStyle & FONTSTYLE_ITALIC) != 0;
		m_bUnderLine = (style._fontStyle & FONTSTYLE_UNDERLINE) != 0;
		isEnable = true;
	}
    GetDlgItem(IDC_CHECK_BOLD)->EnableWindow(style._fontStyle != -1);
	GetDlgItem(IDC_CHECK_ITALIC)->EnableWindow(style._fontStyle != -1);
	GetDlgItem(IDC_CHECK_UNDERLINE)->EnableWindow(style._fontStyle != -1);
*/
	//-- Default Keywords
	bool shouldBeDisplayed = style._keywordClass != -1;
	if (shouldBeDisplayed)
	{
		LexerStyler & lexerStyler = _lsArray.getLexerFromIndex(_currentLexerIndex - 1);

		NppParameters *pNppParams = NppParameters::getInstance();
		LangType lType = pNppParams->getLangIDFromStr(lexerStyler.getLexerName());
		if (lType == L_TXT)
		{
			generic_string str = lexerStyler.getLexerName();
			str += TEXT(" is not defined in NppParameters::getLangIDFromStr()");
				printStr(str.c_str());
		}
		const TCHAR *kws = pNppParams->getWordList(lType, style._keywordClass);
		if (!kws)
			kws = TEXT("");
		::SendDlgItemMessage(m_hWnd, IDC_DEF_KEYWORDS_EDIT, WM_SETTEXT, 0, (LPARAM)(kws));

		const TCHAR *ckwStr = (style._keywords)?style._keywords->c_str():TEXT("");
		::SendDlgItemMessage(m_hWnd, IDC_USER_KEYWORDS_EDIT, WM_SETTEXT, 0, (LPARAM)(ckwStr));
	}
	int showOption = shouldBeDisplayed?SW_SHOW:SW_HIDE;
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_DEF_KEYWORDS_EDIT), showOption);
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_USER_KEYWORDS_EDIT),showOption);
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_DEF_KEYWORDS_STATIC), showOption);
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_USER_KEYWORDS_STATIC),showOption);
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_PLUSSYMBOL_STATIC),showOption);

    //redraw();

	UpdateData(FALSE); // 传递数据
}

// 设置语言
void CTpsStyle::setStyleListFromLexer(int index)
{
    _currentLexerIndex = index;

    // Fill out Styles listbox
    // Before filling out, we clean it
	//::SendDlgItemMessage(_hSelf, IDC_STYLES_LIST, LB_RESETCONTENT, 0, 0);
/*
	if (index)
	{
		const TCHAR *langName = _lsArray.getLexerNameFromIndex(index - 1);
		const TCHAR *ext = NppParameters::getInstance()->getLangExtFromName(langName);
		const TCHAR *userExt = (_lsArray.getLexerStylerByName(langName))->getLexerUserExt();
		::SendDlgItemMessage(_hSelf, IDC_DEF_EXT_EDIT, WM_SETTEXT, 0, (LPARAM)(ext));
		::SendDlgItemMessage(_hSelf, IDC_USER_EXT_EDIT, WM_SETTEXT, 0, (LPARAM)(userExt));
	}
	::ShowWindow(::GetDlgItem(_hSelf, IDC_DEF_EXT_EDIT), index?SW_SHOW:SW_HIDE);
    ::ShowWindow(::GetDlgItem(_hSelf, IDC_DEF_EXT_STATIC), index?SW_SHOW:SW_HIDE);
	::ShowWindow(::GetDlgItem(_hSelf, IDC_USER_EXT_EDIT), index?SW_SHOW:SW_HIDE);
    ::ShowWindow(::GetDlgItem(_hSelf, IDC_USER_EXT_STATIC), index?SW_SHOW:SW_HIDE);
	::ShowWindow(::GetDlgItem(_hSelf, IDC_PLUSSYMBOL2_STATIC), index?SW_SHOW:SW_HIDE);
*/
	StyleArray & lexerStyler = index?_lsArray.getLexerFromIndex(index-1):_globalStyles;

	m_ListItem.ResetContent();
    for (int i = 0 ; i < lexerStyler.getNbStyler() ; i++)
    {
        Style & style = lexerStyler.getStyler(i);
		CString strTmp = style._styleDesc;
		m_ListItem.AddString(strTmp);
    }
	m_ListItem.SetCurSel(0);
    setVisualFromStyleList();
	OnSelchangeListItem();
}

void CTpsStyle::updateGlobalOverrideCtrls()
{
	const NppGUI & nppGUI = (NppParameters::getInstance())->getNppGUI();
	m_bUseGlobalFg = nppGUI._globalOverride.enableFg;
	m_bUseGlobalBg = nppGUI._globalOverride.enableBg;
	m_bUseGlobalFont = nppGUI._globalOverride.enableFont;
	UpdateData(FALSE);
}

void CTpsStyle::showGlobalOverrideCtrls(bool show)
{
	if (show)
	{
		updateGlobalOverrideCtrls();
	}
	GetDlgItem(IDC_STATIC_GLOBALSET)->ShowWindow(show?SW_SHOW:SW_HIDE);
	GetDlgItem(IDC_CHECK_GLOBAL_FG)->ShowWindow(show?SW_SHOW:SW_HIDE);
	GetDlgItem(IDC_CHECK_GLOBAL_BG)->ShowWindow(show?SW_SHOW:SW_HIDE);
	GetDlgItem(IDC_CHECK_GLOBAL_FONT)->ShowWindow(show?SW_SHOW:SW_HIDE);
	_isShownGOCtrls = show;
}

void CTpsStyle::restoreGlobalOverrideValues()
{
	GlobalOverride & gOverride = (NppParameters::getInstance())->getGlobalOverrideStyle();
	gOverride = _gOverride2restored;
}

// 格式项列表自画
void CTpsStyle::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS) 
{
	if(nIDCtl == IDC_LIST_ITEM)
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
		StyleArray & lexerStyler = _currentLexerIndex?_lsArray.getLexerFromIndex(_currentLexerIndex-1):_globalStyles;
		Style & style = lexerStyler.getStyler(item);
		strcpy(szBuf, style._styleDesc);

		int h=rcTemp.bottom-rcTemp.top;
		rcTemp.left+=h+4;
		DrawText(hDC,szBuf,-1,&rcTemp,DT_LEFT|DT_VCENTER|	
			DT_NOPREFIX| DT_SINGLELINE);

		get_dll_resource();
		HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_FONT);
		rcTemp.left=lpDIS->rcItem.left;
		::DrawIconEx(hDC,rcTemp.left+2,rcTemp.top,			
			hIcon,h,h,0,0,DI_NORMAL);	
		reset_dll_resource();
		
		::SetTextColor(hDC,clOldText);
		::SetBkMode(hDC,OldBkMode);
  
		if(lpDIS->itemAction & ODA_FOCUS)   DrawFocusRect(hDC,&lpDIS->rcItem); 
		return;		
	}

	CDialog::OnDrawItem(nIDCtl, lpDIS);
}

// 字体变更
void CTpsStyle::OnSelendokFontCombo() 
{
	UpdateData(TRUE);
	m_staticSample.SetFontName(m_sFont);
	m_staticSample.Invalidate();
	Style & style = getCurrentStyler();
	CString strOldFont = style._fontName;
	if(strOldFont != m_sFont)
	{
		if(strOldFont == "")
		{
			Style & globalStyle = getGlobalStyler(0);
			if(m_sFont != globalStyle._fontName)
			{
				CXTLogFont* pLf;
				CXTFontEnum& fontEnum = CXTFontEnum::Get(); 
				pLf = fontEnum.GetLogFont(m_sFont);
				style._fontName = pLf->lfFaceName;
				apply();
			}
		}else
		{
			CXTLogFont* pLf;
			CXTFontEnum& fontEnum = CXTFontEnum::Get(); 
			pLf = fontEnum.GetLogFont(m_sFont);
			style._fontName = pLf->lfFaceName;
			apply();
		}
	}
}

// 文字色变更
void CTpsStyle::OnFgColorChange()
{
	UpdateData();
	m_staticSample.SetTextColor(m_clrFg);
	m_staticSample.Invalidate();
	Style & style = getCurrentStyler();
	if((m_clrFg & 0xFF000000) != 0)
	{
		m_clrFg &= 0x00FFFFFF;
	}
	style._fgColor = m_clrFg;

	apply();
}

// 背景色变更
void CTpsStyle::OnBkColorChange()
{
	UpdateData();
	m_staticSample.SetBkColor(m_clrBk);
	m_staticSample.Invalidate();
	Style & style = getCurrentStyler();
	if((m_clrBk & 0xFF000000) != 0)
	{
		m_clrBk &= 0x00FFFFFF;
	}
	style._bgColor = m_clrBk;

	apply();
}

// 粗体变更
void CTpsStyle::OnCheckBold() 
{
	UpdateData(TRUE);
	m_staticSample.SetFontBold(m_bBold);
	m_staticSample.Invalidate();
	Style & style = getCurrentStyler();
	if(m_bBold)
		style._fontStyle |= FONTSTYLE_BOLD;
	else
		style._fontStyle &= ~FONTSTYLE_BOLD;

	apply();
}

// 斜体变更
void CTpsStyle::OnCheckItalic() 
{
	UpdateData(TRUE);
	m_staticSample.SetFontItalic(m_bItalic);
	m_staticSample.Invalidate();
	Style & style = getCurrentStyler();
	if(m_bItalic)
		style._fontStyle |= FONTSTYLE_ITALIC;
	else
		style._fontStyle &= ~FONTSTYLE_ITALIC;

	apply();
}

// 下划线变更
void CTpsStyle::OnCheckUnderline() 
{
	UpdateData(TRUE);
	m_staticSample.SetFontUnderline(m_bUnderLine);
	m_staticSample.Invalidate();
	Style & style = getCurrentStyler();
	if(m_bUnderLine)
		style._fontStyle |= FONTSTYLE_UNDERLINE;
	else
		style._fontStyle &= ~FONTSTYLE_UNDERLINE;

	apply();
}

// 文字大小变更
void CTpsStyle::OnSelchangeComboSize() 
{
	CString strOldSize = m_sSize;
	UpdateData(TRUE);
	if((m_sSize != "0") && (m_sSize != "") && (m_sSize != strOldSize))
	{
		m_staticSample.SetFontSize(atoi(m_sSize));
		m_staticSample.Invalidate();
		Style & style = getCurrentStyler();
		if(style._fontSize == 0)
		{
			Style & globalStyle = getGlobalStyler(0);
			if(atoi(m_sSize) != globalStyle._fontSize)
			{
				style._fontSize = atoi(m_sSize);
				apply();
			}
		}else
		{
			style._fontSize = atoi(m_sSize);
			apply();
		}
	}
}

// 全局前景色变更
void CTpsStyle::OnCheckGlobalFg() 
{
	UpdateData(TRUE);
	GlobalOverride & glo = (NppParameters::getInstance())->getGlobalOverrideStyle();
	glo.enableFg = m_bUseGlobalFg;
	apply();
}

// 全局背景色变更
void CTpsStyle::OnCheckGlobalBg() 
{
	UpdateData(TRUE);
	GlobalOverride & glo = (NppParameters::getInstance())->getGlobalOverrideStyle();
	glo.enableBg = m_bUseGlobalBg;
	apply();
}

// 全局字体变更
void CTpsStyle::OnCheckGlobalFont() 
{
	UpdateData(TRUE);
	GlobalOverride & glo = (NppParameters::getInstance())->getGlobalOverrideStyle();
	glo.enableFont = m_bUseGlobalFont;
	glo.enableFontSize = m_bUseGlobalFont;
	glo.enableBold = m_bUseGlobalFont;
	glo.enableItalic = m_bUseGlobalFont;
	glo.enableUnderLine = m_bUseGlobalFont;
	apply();
}

// 格式项变更
void CTpsStyle::OnSelchangeListItem() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ListItem.GetCurSel();
	if(nCurSel != LB_ERR)
	{
		showGlobalOverrideCtrls(false);

		Style & style = getCurrentStyler();
		Style & globalStyle = getGlobalStyler(0);

		// Global override style
		if (lstrcmp(style._styleDesc, TEXT("Global override")) == 0)
		{
			showGlobalOverrideCtrls(true);
		}

		m_sFont		= style._fontName;
		if(m_sFont == "")
		{
			m_sFont = globalStyle._fontName;
		}
		m_sSize.Format("%d", style._fontSize);
		/*if(m_sSize == "0")
		{
			m_sSize = globalStyle._fontSize;
		}*/
		m_bBold		= (style._fontStyle & FONTSTYLE_BOLD) != 0;
		m_bItalic	= (style._fontStyle & FONTSTYLE_ITALIC) != 0;
		m_bUnderLine= (style._fontStyle & FONTSTYLE_UNDERLINE) != 0;
		m_clrFg		= style._fgColor;
		m_clrBk		= style._bgColor;

		UpdateData(FALSE); // 传递数据

		/*if(m_sFont == "")
		{
			m_comboFont.SetSelFont( globalStyle._fontName );
		}*/
		if(m_sSize == "0")
		{
			m_comboSize.SetCurSel(0);
		}

		m_staticSample.SetFontName(m_sFont);
		m_staticSample.SetFontSize(atoi(m_sSize));
		m_staticSample.SetFontBold(m_bBold);
		m_staticSample.SetFontItalic(m_bItalic);
		m_staticSample.SetFontUnderline(m_bUnderLine);
		m_staticSample.SetTextColor(m_clrFg);
		m_staticSample.SetBkColor(m_clrBk);
		m_staticSample.Invalidate();

		// 根据掩码确定那些项可以设置
		GetDlgItem(IDC_STATIC_FONT)->EnableWindow((style._fontName != NULL));
		GetDlgItem(IDC_COMBO_FONT)->EnableWindow((style._fontName != NULL));
		GetDlgItem(IDC_STATIC_SIZE)->EnableWindow(style._fontSize != -1);
		GetDlgItem(IDC_COMBO_SIZE)->EnableWindow(style._fontSize != -1);
		GetDlgItem(IDC_STATIC_COLORFG)->EnableWindow(HIBYTE(HIWORD(style._fgColor)) != 0xFF);
		GetDlgItem(IDC_COLORFG_PICKER)->EnableWindow(HIBYTE(HIWORD(style._fgColor)) != 0xFF);
		GetDlgItem(IDC_STATIC_COLORBK)->EnableWindow(HIBYTE(HIWORD(style._bgColor)) != 0xFF);
		GetDlgItem(IDC_COLORBK_PICKER)->EnableWindow(HIBYTE(HIWORD(style._bgColor)) != 0xFF);
		GetDlgItem(IDC_CHECK_BOLD)->EnableWindow(style._fontStyle != -1);
		GetDlgItem(IDC_CHECK_ITALIC)->EnableWindow(style._fontStyle != -1);
		GetDlgItem(IDC_CHECK_UNDERLINE)->EnableWindow(style._fontStyle != -1);

		setVisualFromStyleList();
	}
}

// 语言变更
void CTpsStyle::OnSelchangeComboLanguage() 
{
	int i = m_comboLanguage.GetCurSel();
	if (i != LB_ERR)
		setStyleListFromLexer(i);
}

// 用户自定义关键字变更
void CTpsStyle::OnChangeUserKeywordsEdit() 
{
	Style & style = getCurrentStyler();
	//const int NB_MAX = 2048;
	//TCHAR kw[NB_MAX];
	int len = ::SendDlgItemMessage(m_hWnd, IDC_USER_KEYWORDS_EDIT, WM_GETTEXTLENGTH, 0, 0);
	len +=1;
	TCHAR *kw = new TCHAR[len];
	::SendDlgItemMessage(m_hWnd, IDC_USER_KEYWORDS_EDIT, WM_GETTEXT, len, (LPARAM)kw);
	style.setKeywords(kw);

	delete [] kw;

	apply();
}
