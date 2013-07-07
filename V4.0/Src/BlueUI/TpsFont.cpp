// TpsFont.cpp : implementation file
//

#include "stdafx.h"
#include "BlueUI.h"
#include "TpsFont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
/////////////////////////////////////////////////////////////////////////////
// CLabel

CLabel::CLabel()
{
	m_crText = GetSysColor(COLOR_WINDOWTEXT);
	m_hBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));

	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT),sizeof(m_lf),&m_lf);

	m_font.CreateFontIndirect(&m_lf);
	m_bTimer = FALSE;
	m_bState = FALSE;
	m_bLink = TRUE;
	m_hCursor = NULL;
	m_Type = None;

	m_hwndBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));
}


CLabel::~CLabel()
{
	m_font.DeleteObject();
	::DeleteObject(m_hBrush);
}

void CLabel::SetFont( CFont* pFont, BOOL bRedraw )
{
	LOGFONT LogFont;
	pFont->GetLogFont(&LogFont);
	m_font.DeleteObject();
	BOOL bCreated = m_font.CreateFontIndirect(&LogFont);
	CStatic::SetFont(pFont, bRedraw);
}

CLabel& CLabel::SetText(const CString& strText)
{
	SetWindowText(strText);
	return *this;
}

CLabel& CLabel::SetTextColor(COLORREF crText)
{
	m_crText = crText;
	RedrawWindow();
	return *this;
}

CLabel& CLabel::SetFontBold(BOOL bBold)
{	
	m_lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
	ReconstructFont();
	RedrawWindow();
	return *this;
}

CLabel& CLabel::SetFontUnderline(BOOL bSet)
{	
	m_lf.lfUnderline = bSet;
	ReconstructFont();
	RedrawWindow();
	return *this;
}

CLabel& CLabel::SetFontStrikeOut(BOOL bSet)
{	
	m_lf.lfStrikeOut = bSet;
	ReconstructFont();
	RedrawWindow();
	return *this;
}

CLabel& CLabel::SetFontItalic(BOOL bSet)
{
	m_lf.lfItalic = bSet;
	ReconstructFont();
	RedrawWindow();
	return *this;	
}

CLabel& CLabel::SetSunken(BOOL bSet)
{
	if (!bSet)
		ModifyStyleEx(WS_EX_STATICEDGE,0,SWP_DRAWFRAME);
	else
		ModifyStyleEx(0,WS_EX_STATICEDGE,SWP_DRAWFRAME);
		
	return *this;	
}

CLabel& CLabel::SetBorder(BOOL bSet)
{
	if (!bSet)
		ModifyStyle(WS_BORDER,0,SWP_DRAWFRAME);
	else
		ModifyStyle(0,WS_BORDER,SWP_DRAWFRAME);
		
	return *this;	
}

CLabel& CLabel::SetFontSize(int nSize)
{
	nSize*=-1;
	m_lf.lfHeight = nSize;
	ReconstructFont();
	RedrawWindow();
	return *this;
}


CLabel& CLabel::SetBkColor(COLORREF crBkgnd)
{
	if (m_hBrush)
		::DeleteObject(m_hBrush);
	
	m_hBrush = ::CreateSolidBrush(crBkgnd);
	return *this;
}

CLabel& CLabel::SetFontName(const CString& strFont)
{	
	strcpy(m_lf.lfFaceName,strFont);
	ReconstructFont();
	RedrawWindow();
	return *this;
}


BEGIN_MESSAGE_MAP(CLabel, CStatic)
	//{{AFX_MSG_MAP(CLabel)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLabel message handlers

HBRUSH CLabel::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called

	if (CTLCOLOR_STATIC == nCtlColor)
	{
		pDC->SelectObject(&m_font);
		pDC->SetTextColor(m_crText);
		pDC->SetBkMode(TRANSPARENT);
	}


	if (m_Type == Background)
	{
		if (!m_bState)
			return m_hwndBrush;
	}

	return m_hBrush;
}

void CLabel::ReconstructFont()
{
	m_font.DeleteObject();
	BOOL bCreated = m_font.CreateFontIndirect(&m_lf);

	ASSERT(bCreated);
}


CLabel& CLabel::FlashText(BOOL bActivate)
{
	if (m_bTimer)
	{
		SetWindowText(m_strText);
		KillTimer(1);
	}

	if (bActivate)
	{
		GetWindowText(m_strText);
		m_bState = FALSE;
		
		m_bTimer = TRUE;
		SetTimer(1,500,NULL);
		m_Type = Text;
	}

	return *this;
}

CLabel& CLabel::FlashBackground(BOOL bActivate)
{

	if (m_bTimer)
		KillTimer(1);

	if (bActivate)
	{
		m_bState = FALSE;

		m_bTimer = TRUE;
		SetTimer(1,500,NULL);

		m_Type = Background;
	}

	return *this;
}


void CLabel::OnTimer(UINT nIDEvent) 
{
	m_bState = !m_bState;

	switch (m_Type)
	{
		case Text:
			if (m_bState)
				SetWindowText("");
			else
				SetWindowText(m_strText);
		break;

		case Background:
			InvalidateRect(NULL,FALSE);
			UpdateWindow();
		break;
	}
	
	CStatic::OnTimer(nIDEvent);
}

CLabel& CLabel::SetLink(BOOL bLink)
{
	m_bLink = bLink;

	if (bLink)
		ModifyStyle(0,SS_NOTIFY);
	else
		ModifyStyle(SS_NOTIFY,0);

	return *this;
}

void CLabel::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CString strLink;

	GetWindowText(strLink);
	ShellExecute(NULL,"open",strLink,NULL,NULL,SW_SHOWNORMAL);
		
	CStatic::OnLButtonDown(nFlags, point);
}

BOOL CLabel::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_hCursor)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}

	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

CLabel& CLabel::SetLinkCursor(HCURSOR hCursor)
{
	m_hCursor = hCursor;
	return *this;
}
*/

/////////////////////////////////////////////////////////////////////////////
// CTpsFont dialog


CTpsFont::CTpsFont(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsFont::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsFont)
	m_bBold = FALSE;
	m_bItalic = FALSE;
	m_bStrikeOut = FALSE;
	m_bUnderLine = FALSE;
	m_sFont = _T("");
	m_sSize = _T("");
	//}}AFX_DATA_INIT

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

	m_bStrikeOut = plogfont->lfStrikeOut;
	m_bUnderLine = plogfont->lfUnderline;
	m_sFont = plogfont->lfFaceName;
	m_sSize.Format( "%d", MulDiv(plogfont->lfHeight, 72, m_cyPixelsPerInch) );
	m_clrFg = RGB(0,0,0);

	PLAT_LOADSTRING(strTpsFontOutput, IDS_TPS_FONT_OUTPUT); // 输出窗口字体

	m_nFontItemCount = 1;
	m_FontItems[0] = FontItem(FON_FONT | FON_SIZE | FON_TYPE,
			strTpsFontOutput,	REG_FMT_OUTPUT, "FixedSys", 16,
			RGB(0,0,0), RGB(255,255,255),
			FALSE, FALSE, FALSE, FALSE);
}

void CTpsFont::LoadFontItems()
{
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if(!theApp.m_aPlugIns[i].IsVciOwm())
			continue;
		IVciOwm* pVciOwm = theApp.m_aPlugIns[i].GetIVciOwm();
		if(pVciOwm == NULL)
			continue;

		int nItemCount = 0;
		FontItem* pItems = (FontItem*)(pVciOwm->Command(OC_GETFONTARRAY, (WPARAM)(&nItemCount), 0));
		if(pItems != NULL)
		{
			for(int j=0; j<nItemCount; j++)
			{
				if(MAX_FONT_ITEMS > m_nFontItemCount)
				{
					m_FontItems[m_nFontItemCount] = *(pItems+j);
					m_nFontItemCount++;
				}
			}
		}
	}

	for(int i=0; i<m_nFontItemCount; i++)
	{
		FontItem* pItem = &(m_FontItems[i]);
		m_ListItem.AddString(pItem->strName);
		CString strSubKey;
		strSubKey.Format("%s\\%s", REG_FORMAT_SUBKEY, pItem->strSubKey);
		pItem->strFont = theApp.GetProfileString(strSubKey, REG_FMFONT, pItem->strFont);
		pItem->nSize = theApp.GetProfileInt(strSubKey, REG_FMSIZE, pItem->nSize);
		pItem->colorFg = theApp.GetProfileInt(strSubKey, REG_FMCOLORFG, pItem->colorFg);
		pItem->colorBk = theApp.GetProfileInt(strSubKey, REG_FMCOLORBK, pItem->colorBk);
		pItem->bBold = theApp.GetProfileInt(strSubKey, REG_FMBOLD, pItem->bBold);
		pItem->bItalic = theApp.GetProfileInt(strSubKey, REG_FMITALIC, pItem->bItalic);
		pItem->bUnderLine = theApp.GetProfileInt(strSubKey, REG_FMUNDERLINE, pItem->bUnderLine);
		pItem->bStrikeOut = theApp.GetProfileInt(strSubKey, REG_FMSTRIKEOUT, pItem->bStrikeOut);
	}
}

void CTpsFont::SaveFontItems()
{
	for(int i=0; i<m_nFontItemCount; i++)
	{
		FontItem* pItem = &(m_FontItems[i]);
		CString strSubKey;
		strSubKey.Format("%s\\%s", REG_FORMAT_SUBKEY, pItem->strSubKey);
		theApp.WriteProfileString(strSubKey, REG_FMFONT, pItem->strFont);
		theApp.WriteProfileInt(strSubKey, REG_FMSIZE, pItem->nSize);
		theApp.WriteProfileInt(strSubKey, REG_FMCOLORFG, pItem->colorFg);
		theApp.WriteProfileInt(strSubKey, REG_FMCOLORBK, pItem->colorBk);
		theApp.WriteProfileInt(strSubKey, REG_FMBOLD, pItem->bBold);
		theApp.WriteProfileInt(strSubKey, REG_FMITALIC, pItem->bItalic);
		theApp.WriteProfileInt(strSubKey, REG_FMUNDERLINE, pItem->bUnderLine);
		theApp.WriteProfileInt(strSubKey, REG_FMSTRIKEOUT, pItem->bStrikeOut);
	}
}

FontItem* CTpsFont::GetCurItem()
{
	int nCurSel = m_ListItem.GetCurSel();
	if((nCurSel != LB_ERR) && (nCurSel < m_nFontItemCount))
	{
		return &(m_FontItems[nCurSel]);
	}else
		return NULL;
}

void CTpsFont::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsFont)
	DDX_Control(pDX, IDC_BTN_FONTRESET, m_btnReset);
	DDX_Control(pDX, IDC_LIST_ITEM, m_ListItem);
	DDX_Control(pDX, IDC_COLORFG_PICKER, m_ColorfgPicker);
	DDX_Control(pDX, IDC_COLORBK_PICKER, m_ColorbkPicker);
	DDX_Control(pDX, IDC_SAMPLEBOX, m_staticSample);
	DDX_Control(pDX, IDC_COMBO_SIZE, m_comboSize);
	DDX_Control(pDX, IDC_COMBO_FONT, m_comboFont);
	DDX_Check(pDX, IDC_CHECK_BOLD, m_bBold);
	DDX_Check(pDX, IDC_CHECK_ITALIC, m_bItalic);
	DDX_Check(pDX, IDC_CHECK_STRIKEOUT, m_bStrikeOut);
	DDX_Check(pDX, IDC_CHECK_UNDERLINE, m_bUnderLine);
	DDX_CBString(pDX, IDC_COMBO_FONT, m_sFont);
	DDX_CBString(pDX, IDC_COMBO_SIZE, m_sSize);
	//}}AFX_DATA_MAP
	DDX_XTColorPicker(pDX, IDC_COLORFG_PICKER, m_clrFg);
	DDX_XTColorPicker(pDX, IDC_COLORBK_PICKER, m_clrBk);
}


BEGIN_MESSAGE_MAP(CTpsFont, CDialog)
	//{{AFX_MSG_MAP(CTpsFont)
	ON_BN_CLICKED(IDC_CHECK_BOLD, OnSelchange)
	ON_LBN_SELCHANGE(IDC_LIST_ITEM, OnSelchangeListItem)
	ON_BN_CLICKED(IDC_BTN_FONTRESET, OnBtnFontreset)
	ON_BN_CLICKED(IDC_CHECK_ITALIC, OnSelchange)
	ON_BN_CLICKED(IDC_CHECK_STRIKEOUT, OnSelchange)
	ON_BN_CLICKED(IDC_CHECK_UNDERLINE, OnSelchange)
	//ON_CBN_KILLFOCUS(IDC_COMBO_FONT, OnSelchange)
	//ON_CBN_SELCHANGE(IDC_COMBO_FONT, OnSelchange)
	ON_CBN_KILLFOCUS(IDC_COMBO_SIZE, OnSelchange)
	ON_CBN_SELCHANGE(IDC_COMBO_SIZE, OnSelchange)
	ON_WM_DRAWITEM()
	ON_CBN_SELENDOK(IDC_COMBO_FONT, OnSelendokFontCombo)
	//}}AFX_MSG_MAP
	ON_CPN_XT_SELENDOK(IDC_COLORFG_PICKER, OnFgColorChange)
	ON_CPN_XT_SELENDOK(IDC_COLORBK_PICKER, OnBkColorChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsFont message handlers

void CTpsFont::OnSelchange() 
{
	// The selection hasn't changed yet, so change it
	if( IsChild( GetFocus() ) && 
			GetFocus()->GetParent()->IsKindOf( RUNTIME_CLASS( CComboBox ) ) )
	{
		CComboBox *cb = (CComboBox *)GetFocus()->GetParent();
		CString sText;
		if( cb->GetCurSel() != CB_ERR )
		{
			cb->GetLBText( cb->GetCurSel(), sText );
			cb->SetWindowText( sText );
		}
	}

	UpdateData(TRUE);

	m_staticSample.SetFontName(m_sFont);
	m_staticSample.SetFontSize(atoi(m_sSize));
	m_staticSample.SetFontBold(m_bBold);
	m_staticSample.SetFontItalic(m_bItalic);
	m_staticSample.SetFontUnderline(m_bUnderLine);
	m_staticSample.SetFontStrikeOut(m_bStrikeOut);

	FontItem* pFontItem = GetCurItem();
	if(pFontItem)
	{
		pFontItem->strFont		= m_sFont;
		pFontItem->nSize		= atoi(m_sSize);
		pFontItem->bBold		= m_bBold;
		pFontItem->bItalic		= m_bItalic;
		pFontItem->bUnderLine	= m_bUnderLine;
		pFontItem->bStrikeOut	= m_bStrikeOut;
	}
}

void CTpsFont::OnSelendokFontCombo() 
{
	CXTLogFont  LogFont;
	m_comboFont.GetSelFont( LogFont );

	m_sFont = LogFont.lfFaceName;
	m_staticSample.SetFontName(m_sFont);
	FontItem* pFontItem = GetCurItem();
	if(pFontItem)
	{
		pFontItem->strFont		= m_sFont;
	}
	m_staticSample.Invalidate();
}

void CTpsFont::OnFgColorChange()
{
	UpdateData();
	m_staticSample.SetTextColor(m_clrFg);
	FontItem* pFontItem = GetCurItem();
	if(pFontItem)
	{
		pFontItem->colorFg = m_clrFg;
	}
	m_staticSample.Invalidate();
}

void CTpsFont::OnBkColorChange()
{
	UpdateData();
	m_staticSample.SetBkColor(m_clrBk);
	FontItem* pFontItem = GetCurItem();
	if(pFontItem)
	{
		pFontItem->colorBk = m_clrBk;
	}
	m_staticSample.Invalidate();
}

BOOL CTpsFont::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//m_comboFont.DisableFlatLook(TRUE);
	m_comboFont.SetListStyle(xtFontSample);
	m_comboFont.InitControl();

	m_comboSize.DisableFlatLook(TRUE);

/*	
	CWindowDC dc(this);
	LOGFONT logfont;
	logfont.lfCharSet = DEFAULT_CHARSET;
	logfont.lfFaceName[0] = '\0';
	logfont.lfPitchAndFamily = 0;
	EnumFontFamiliesEx( dc.m_hDC, &logfont, (FONTENUMPROC)FontEnumProc, 
			(LPARAM) this, 0 );
*/	
	// Fill Size combobox with "common" sizes
	TCHAR* Defaults[] = { _T("8"), _T("9"), _T("10"), _T("11"), _T("12"), _T("14"),
				_T("16"), _T("18"), _T("20"), _T("22"), _T("24"), _T("26"), 
				_T("28"), _T("36"), _T("48") };
	for (int i = 0; i < (sizeof(Defaults)/sizeof(Defaults[0])); i++)
		m_comboSize.AddString(Defaults[i]);

	UpdateData(FALSE);

	LoadFontItems();

	m_ListItem.SetCurSel(0);
	OnSelchangeListItem();

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_OPTIONTREE);
	CImageList imageList;
	imageList.Create(16, 16, ILC_COLOR24|ILC_MASK, 1, 1);
	imageList.Add(&bitmap, CXTPImageManager::GetBitmapMaskColor(bitmap, CPoint(0, 0)));
	m_btnReset.SetIcon(CSize(16,16), imageList.ExtractIcon(10));
	m_btnReset.SetXButtonStyle(BS_XT_HILITEPRESSED | BS_XT_SHOWFOCUS | BS_XT_XPFLAT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/*
int CALLBACK CTpsFont::FontEnumProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, 
		int FontType, CTpsFont* pTpsFont )
{
	if( pTpsFont->m_comboFont.FindStringExact( 0, (LPCTSTR)lpelfe->elfFullName ) == 
					CB_ERR )
	{
		// Add to list
		if(strlen((LPCTSTR)lpelfe->elfFullName) > 0)
			pTpsFont->m_comboFont.AddString( (LPCTSTR)lpelfe->elfFullName );
	}

 	return 1;
}
*/
void CTpsFont::GetCurrentFont(LPLOGFONT lplf)
{
	m_fontSample.GetLogFont( lplf );
}

void CTpsFont::OnSelchangeListItem() 
{
	int nCurSel = m_ListItem.GetCurSel();
	if(nCurSel != LB_ERR)
	{
		FontItem* pFontItem = GetCurItem();
		if(pFontItem)
		{
			m_sFont		= pFontItem->strFont;
			m_sSize.Format("%d", pFontItem->nSize);
			m_bBold		= pFontItem->bBold;
			m_bItalic	= pFontItem->bItalic;
			m_bUnderLine= pFontItem->bUnderLine;
			m_bStrikeOut= pFontItem->bStrikeOut;
			m_clrFg		= pFontItem->colorFg;
			m_clrBk		= pFontItem->colorBk;

			UpdateData(FALSE); // 传递数据

			m_staticSample.SetFontName(m_sFont);
			m_staticSample.SetFontSize(atoi(m_sSize));
			m_staticSample.SetFontBold(m_bBold);
			m_staticSample.SetFontItalic(m_bItalic);
			m_staticSample.SetFontUnderline(m_bUnderLine);
			m_staticSample.SetFontStrikeOut(m_bStrikeOut);
			m_staticSample.SetTextColor(pFontItem->colorFg);
			m_staticSample.SetBkColor(pFontItem->colorBk);
			m_staticSample.Invalidate();

			// 根据掩码确定那些项可以设置
			GetDlgItem(IDC_STATIC_FONT)->EnableWindow(pFontItem->dwMask & FON_FONT);
			GetDlgItem(IDC_COMBO_FONT)->EnableWindow(pFontItem->dwMask & FON_FONT);
			GetDlgItem(IDC_STATIC_SIZE)->EnableWindow(pFontItem->dwMask & FON_SIZE);
			GetDlgItem(IDC_COMBO_SIZE)->EnableWindow(pFontItem->dwMask & FON_SIZE);
			GetDlgItem(IDC_STATIC_COLORFG)->EnableWindow(pFontItem->dwMask & FON_FG);
			GetDlgItem(IDC_COLORFG_PICKER)->EnableWindow(pFontItem->dwMask & FON_FG);
			GetDlgItem(IDC_STATIC_COLORBK)->EnableWindow(pFontItem->dwMask & FON_BK);
			GetDlgItem(IDC_COLORBK_PICKER)->EnableWindow(pFontItem->dwMask & FON_BK);
			GetDlgItem(IDC_CHECK_BOLD)->EnableWindow(pFontItem->dwMask & FON_TYPE);
			GetDlgItem(IDC_CHECK_ITALIC)->EnableWindow(pFontItem->dwMask & FON_TYPE);
			GetDlgItem(IDC_CHECK_UNDERLINE)->EnableWindow(pFontItem->dwMask & FON_TYPE);
			GetDlgItem(IDC_CHECK_STRIKEOUT)->EnableWindow(pFontItem->dwMask & FON_TYPE);
		}
	}
}

void CTpsFont::OnOK() 
{
	SaveFontItems();

	// 使各个OWM模块重新加载一次字体
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if(!theApp.m_aPlugIns[i].IsVciOwm())
			continue;
		IVciOwm* pVciOwm = theApp.m_aPlugIns[i].GetIVciOwm();
		if(pVciOwm == NULL)
			continue;

		int nItemCount = 0;
		pVciOwm->Command(OC_GETFONTARRAY, (WPARAM)(&nItemCount), 0);
	}

	CDialog::OnOK();
}

void CTpsFont::OnBtnFontreset() 
{
	PLAT_LOADSTRING(strTpsFontOutput, IDS_TPS_FONT_OUTPUT); // 输出窗口字体

	m_FontItems[0] = FontItem(FON_FONT | FON_SIZE | FON_TYPE,
			strTpsFontOutput,	REG_FMT_OUTPUT, "FixedSys", 16,
			RGB(0,0,0), RGB(255,255,255),
			FALSE, FALSE, FALSE, FALSE);

	m_nFontItemCount = 1;

	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if(!theApp.m_aPlugIns[i].IsVciOwm())
			continue;
		IVciOwm* pVciOwm = theApp.m_aPlugIns[i].GetIVciOwm();
		if(pVciOwm == NULL)
			continue;

		int nItemCount = 0;
/*
		// 判断是否处于动态卸载状态
		BOOL bOwmUnTsr = theApp.OWMIsUnTsr(theApp.m_aOWMs[i].ol.nOWMID);
		if(bOwmUnTsr)
		{	// 如果是,就进行动态加载
			theApp.GetDynamicOWM(theApp.m_aOWMs[i].ol.nOWMID);
		}
*/
		// lParam=1表示恢复为缺省值
		FontItem* pItems = (FontItem*)(pVciOwm->Command(OC_GETFONTARRAY, (WPARAM)(&nItemCount), 1));
		if(pItems != NULL)
		{
			for(int j=0; j<nItemCount; j++)
			{
				if(MAX_FONT_ITEMS > m_nFontItemCount)
				{
					m_FontItems[m_nFontItemCount] = *(pItems+j);
					m_nFontItemCount++;
				}
			}
		}
/*
		if(bOwmUnTsr)
		{	// 恢复动态卸载状态
			theApp.UnTsrOWM(theApp.m_aOWMs[i].ol.nOWMID);
		}
*/
	}

	OnSelchangeListItem();
}

/////////////////////////////////////////////////////////////////////////////
// 自画事件
/////////////////////////////////////////////////////////////////////////////
void CTpsFont::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS) 
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
		strcpy(szBuf, m_FontItems[item].strName);

		int h=rcTemp.bottom-rcTemp.top;
		rcTemp.left+=h+4;
		DrawText(hDC,szBuf,-1,&rcTemp,DT_LEFT|DT_VCENTER|	
			DT_NOPREFIX| DT_SINGLELINE);

		HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_FONT);
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
