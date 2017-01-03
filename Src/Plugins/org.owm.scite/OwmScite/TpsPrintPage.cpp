// TpsPrintPage.cpp : implementation file
//

#include "stdafx.h"
#include "OwmScite.h"
#include "TpsPrintPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsPrintPage dialog


CTpsPrintPage::CTpsPrintPage(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsPrintPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsPrintPage)
	m_sFont = _T("");
	m_sSize = _T("");
	m_strPageHeader = _T("");
	m_strPageFooter = _T("");
	m_nMarginLeft = 0;
	m_nMarginRight = 0;
	m_nMarginTop = 0;
	m_nMarginBottom = 0;
	m_bPrintLineNumber = FALSE;
	m_bBold = FALSE;
	m_bItalic = FALSE;
	m_nColorMode = 0;
	//}}AFX_DATA_INIT
}


void CTpsPrintPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsPrintPage)
	DDX_Control(pDX, IDC_COMBO_COLOR_MODE, m_comboColorMode);
	DDX_Control(pDX, IDC_COMBO_SIZE, m_comboSize);
	DDX_Control(pDX, IDC_COMBO_FONT, m_comboFont);
	DDX_CBString(pDX, IDC_COMBO_FONT, m_sFont);
	DDX_CBString(pDX, IDC_COMBO_SIZE, m_sSize);
	DDX_Text(pDX, IDC_EDIT_PAGE_HEADER, m_strPageHeader);
	DDX_Text(pDX, IDC_EDIT_PAGE_FOOTER, m_strPageFooter);
	DDX_Text(pDX, IDC_EDIT_PAGEMARGIN_LEFT, m_nMarginLeft);
	DDV_MinMaxInt(pDX, m_nMarginLeft, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_PAGEMARGIN_RIGHT, m_nMarginRight);
	DDV_MinMaxInt(pDX, m_nMarginRight, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_PAGEMARGIN_TOP, m_nMarginTop);
	DDV_MinMaxInt(pDX, m_nMarginTop, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_PAGEMARGIN_BOTTOM, m_nMarginBottom);
	DDV_MinMaxInt(pDX, m_nMarginBottom, 0, 1000);
	DDX_Check(pDX, IDC_CHECK_PRINT_LINENUM, m_bPrintLineNumber);
	DDX_Check(pDX, IDC_CHECK_BOLD, m_bBold);
	DDX_Check(pDX, IDC_CHECK_ITALIC, m_bItalic);
	DDX_CBIndex(pDX, IDC_COMBO_COLOR_MODE, m_nColorMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsPrintPage, CDialog)
	//{{AFX_MSG_MAP(CTpsPrintPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsPrintPage message handlers

BOOL CTpsPrintPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
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

	m_comboColorMode.DisableFlatLook(TRUE);

	NppParameters *pNppParam = NppParameters::getInstance();
	NppGUI & nppGUI = (NppGUI & )pNppParam->getNppGUI();

	m_bPrintLineNumber = nppGUI._printSettings._printLineNumber;
	m_nColorMode = nppGUI._printSettings._printOption;
	m_nMarginLeft = nppGUI._printSettings._marge.left;
	m_nMarginRight = nppGUI._printSettings._marge.right;
	m_nMarginTop = nppGUI._printSettings._marge.top;
	m_nMarginBottom = nppGUI._printSettings._marge.bottom;

	CString strHeaderLeft = (nppGUI._printSettings._headerLeft.length() > 0) ? "&L" : "";
	strHeaderLeft += nppGUI._printSettings._headerLeft.c_str();
	CString strHeaderMiddle = (nppGUI._printSettings._headerMiddle.length() > 0) ? "&C" : "";
	strHeaderMiddle += nppGUI._printSettings._headerMiddle.c_str();
	CString strHeaderRight = (nppGUI._printSettings._headerRight.length() > 0) ? "&R" : "";
	strHeaderRight += nppGUI._printSettings._headerRight.c_str();
	CString strFooterLeft = (nppGUI._printSettings._footerLeft.length() > 0) ? "&L" : "";
	strFooterLeft += nppGUI._printSettings._footerLeft.c_str();
	CString strFooterMiddle = (nppGUI._printSettings._footerMiddle.length() > 0) ? "&C" : "";
	strFooterMiddle += nppGUI._printSettings._footerMiddle.c_str();
	CString strFooterRight = (nppGUI._printSettings._footerRight.length() > 0) ? "&R" : "";
	strFooterRight += nppGUI._printSettings._footerRight.c_str();

	m_strPageHeader.Format("%s%s%s",
					strHeaderLeft,
					strHeaderMiddle,
					strHeaderRight);

	m_strPageFooter.Format("%s%s%s",
					strFooterLeft,
					strFooterMiddle,
					strFooterRight);

	m_sFont = nppGUI._printSettings._headerFontName.c_str();
	m_sSize.Format("%d", nppGUI._printSettings._headerFontSize);

	m_bBold = nppGUI._printSettings._headerFontStyle & FONTSTYLE_BOLD;
	m_bItalic = nppGUI._printSettings._headerFontStyle & FONTSTYLE_ITALIC;

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTpsPrintPage::OnOK() 
{
	UpdateData(TRUE);

	NppParameters *pNppParam = NppParameters::getInstance();
	NppGUI & nppGUI = (NppGUI & )pNppParam->getNppGUI();

	nppGUI._printSettings._printLineNumber = m_bPrintLineNumber;
	nppGUI._printSettings._printOption = m_nColorMode;
	nppGUI._printSettings._marge.left = m_nMarginLeft;
	nppGUI._printSettings._marge.right = m_nMarginRight;
	nppGUI._printSettings._marge.top = m_nMarginTop;
	nppGUI._printSettings._marge.bottom = m_nMarginBottom;

	nppGUI._printSettings._headerFontName = m_sFont;
	nppGUI._printSettings._footerFontName = m_sFont;
	nppGUI._printSettings._headerFontSize = atoi(m_sSize);
	nppGUI._printSettings._footerFontSize = atoi(m_sSize);

	nppGUI._printSettings._headerFontStyle = 0;
	nppGUI._printSettings._headerFontStyle |= (m_bBold ? FONTSTYLE_BOLD : 0);
	nppGUI._printSettings._headerFontStyle |= (m_bItalic ? FONTSTYLE_ITALIC : 0);
	nppGUI._printSettings._footerFontStyle = nppGUI._printSettings._headerFontStyle;

	CString strPageHeader = m_strPageHeader;
	CString strPageFooter = m_strPageFooter;
	CString strHeaderLeft = "";
	CString strHeaderMiddle = "";
	CString strHeaderRight = "";
	CString strFooterLeft = "";
	CString strFooterMiddle = "";
	CString strFooterRight = "";

	int nPos = strPageHeader.Find("&R");
	if(nPos != -1)
	{
		strHeaderRight = strPageHeader;
		strHeaderRight.Delete(0, nPos+2);
		strPageHeader = strPageHeader.Left(nPos);
	}
	nPos = strPageHeader.Find("&C");
	if(nPos != -1)
	{
		strHeaderMiddle = strPageHeader;
		strHeaderMiddle.Delete(0, nPos+2);
		strPageHeader = strPageHeader.Left(nPos);
	}
	nPos = strPageHeader.Find("&L");
	if(nPos != -1)
	{
		strHeaderLeft = strPageHeader;
		strHeaderLeft.Delete(0, nPos+2);
	}else
	{
		strHeaderLeft = strPageHeader;
	}

	nppGUI._printSettings._headerLeft = strHeaderLeft;
	nppGUI._printSettings._headerMiddle = strHeaderMiddle;
	nppGUI._printSettings._headerRight = strHeaderRight;

	nPos = strPageFooter.Find("&R");
	if(nPos != -1)
	{
		strFooterRight = strPageFooter;
		strFooterRight.Delete(0, nPos+2);
		strPageFooter = strPageFooter.Left(nPos);
	}
	nPos = strPageFooter.Find("&C");
	if(nPos != -1)
	{
		strFooterMiddle = strPageFooter;
		strFooterMiddle.Delete(0, nPos+2);
		strPageFooter = strPageFooter.Left(nPos);
	}
	nPos = strPageFooter.Find("&L");
	if(nPos != -1)
	{
		strFooterLeft = strPageFooter;
		strFooterLeft.Delete(0, nPos+2);
	}else
	{
		strFooterLeft = strPageFooter;
	}

	nppGUI._printSettings._footerLeft = strFooterLeft;
	nppGUI._printSettings._footerMiddle = strFooterMiddle;
	nppGUI._printSettings._footerRight = strFooterRight;

	CDialog::OnOK();
}
