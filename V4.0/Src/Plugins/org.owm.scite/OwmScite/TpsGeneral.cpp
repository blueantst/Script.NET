// TpsGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "OwmScite.h"
#include "TpsGeneral.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsGeneral dialog


CTpsGeneral::CTpsGeneral(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsGeneral::IDD, pParent)
{
	NppParameters *pNppParam = NppParameters::getInstance();
	const ScintillaViewParams & svp = pNppParam->getSVP(SCIV_PRIMARY);
	NppGUI & nppGUI = (NppGUI &)pNppParam->getNppGUI();

	//{{AFX_DATA_INIT(CTpsGeneral)
	m_bLineNumberMarginShow = svp._lineNumberMarginShow;
	m_bCurrentLineHilitingShow = svp._currentLineHilitingShow;
	m_bIndentMaintain = nppGUI._maitainIndent;
	m_bFileAutoDetection = (nppGUI._fileAutoDetection == cdEnabled);
	m_bEnableTagsMatchHilite = nppGUI._enableTagsMatchHilite;
	m_bReplaceTabBySpace = nppGUI._tabReplacedBySpace;
	m_bAutocStatus = (nppGUI._autocStatus == nppGUI.autoc_func_and_word);
	m_bFuncParamsHint = nppGUI._funcParams;
	m_nTabSize = nppGUI._tabSize;
	m_bWhiteSpaceShow = svp._whiteSpaceShow;
	m_bEolShow = svp._eolShow;
	m_bIndentGuideLineShow = svp._indentGuideLineShow;
	m_bDoWrap = svp._doWrap;
	m_bWrapSymbolShow = svp._wrapSymbolShow;
	m_nDefaultZoom = svp._zoom;
	m_bEnableWordTooltip = AfxGetApp()->GetProfileInt(REG_HELP_SUBKEY, REG_HELP_WORD_TOOLTIP, TRUE);
	m_bEnableVarTooltip = AfxGetApp()->GetProfileInt(REG_HELP_SUBKEY, REG_HELP_VAR_TOOLTIP, TRUE);
	m_bSearchWordAccurate = AfxGetApp()->GetProfileInt(REG_HELP_SUBKEY, REG_HELP_SEARCHWORD_ACCURATE, TRUE);
	m_strInterpName = AfxGetApp()->GetProfileString(REG_OWMEDIT, REG_INTERPNAME, INTERP_NAME);
	m_bAutoResetInterp = AfxGetApp()->GetProfileInt(REG_OWMEDIT, REG_AUTORESET_INTERP, TRUE);
	m_bShowWordHelpInDynamicHelp = AfxGetApp()->GetProfileInt(REG_HELP_SUBKEY, REG_HELP_WORDHELP_IN_DYNHELP, TRUE);
	//}}AFX_DATA_INIT
}


void CTpsGeneral::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsGeneral)
	DDX_Check(pDX, IDC_CHECK_VIEW_LINENUM, m_bLineNumberMarginShow);
	DDX_Check(pDX, IDC_CHECK_HILIGHT_CURRENTLINE, m_bCurrentLineHilitingShow);
	DDX_Check(pDX, IDC_CHECK_AUTO_INDENT, m_bIndentMaintain);
	DDX_Check(pDX, IDC_CHECK_DETECT_FILE_CHANGE, m_bFileAutoDetection);
	DDX_Check(pDX, IDC_CHECK_HILIGHT_MATCHING_TAGS, m_bEnableTagsMatchHilite);
	DDX_Check(pDX, IDC_CHECK_REPLACE_TAB_BY_SPACE, m_bReplaceTabBySpace);
	DDX_Check(pDX, IDC_CHECK_AUTO_COMPLETE, m_bAutocStatus);
	DDX_Check(pDX, IDC_CHECK_FUNC_PARAMETER_HINT, m_bFuncParamsHint);
	DDX_Text(pDX, IDC_EDIT_TAB_SIZE, m_nTabSize);
	DDV_MinMaxInt(pDX, m_nTabSize, 0, 8);
	DDX_Check(pDX, IDC_CHECK_SHOW_WHITESPACE, m_bWhiteSpaceShow);
	DDX_Check(pDX, IDC_CHECK_SHOW_EOL, m_bEolShow);
	DDX_Check(pDX, IDC_CHECK_SHOW_INDENT_GUIDELINE, m_bIndentGuideLineShow);
	DDX_Check(pDX, IDC_CHECK_WRAP, m_bDoWrap);
	DDX_Check(pDX, IDC_CHECK_SHOW_WRAP_SYMBOL, m_bWrapSymbolShow);
	DDX_Text(pDX, IDC_EDIT_ZOOM, m_nDefaultZoom);
	DDV_MinMaxInt(pDX, m_nDefaultZoom, -10, 20);
	DDX_Check(pDX, IDC_CHECK_WORD_TOOLTIP, m_bEnableWordTooltip);
	DDX_Check(pDX, IDC_CHECK_SEARCHWORD_ACCURATE, m_bSearchWordAccurate);
	DDX_Text(pDX, IDC_EDIT_INTERPNAME, m_strInterpName);
	DDX_Check(pDX, IDC_CHECK_AUTORESET_INTERP, m_bAutoResetInterp);
	DDX_Check(pDX, IDC_CHECK_WORDHELP_IN_DYNHELP, m_bShowWordHelpInDynamicHelp);
	DDX_Check(pDX, IDC_CHECK_VAR_TOOLTIP, m_bEnableVarTooltip);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsGeneral, CDialog)
	//{{AFX_MSG_MAP(CTpsGeneral)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsGeneral message handlers
void CTpsGeneral::OnOK() 
{
	UpdateData(TRUE);

	NppParameters *pNppParam = NppParameters::getInstance();
	ScintillaViewParams & svp = (ScintillaViewParams &)pNppParam->getSVP(SCIV_PRIMARY);
	NppGUI & nppGUI = (NppGUI &)pNppParam->getNppGUI();

	svp._lineNumberMarginShow = m_bLineNumberMarginShow ? true : false;
	svp._currentLineHilitingShow = m_bCurrentLineHilitingShow;
	nppGUI._maitainIndent = m_bIndentMaintain;
	nppGUI._fileAutoDetection = m_bFileAutoDetection ? cdEnabled : cdDisabled;
	nppGUI._enableTagsMatchHilite = m_bEnableTagsMatchHilite;
	nppGUI._tabReplacedBySpace = m_bReplaceTabBySpace;
	nppGUI._autocStatus = m_bAutocStatus ? nppGUI.autoc_func_and_word : nppGUI.autoc_none;
	nppGUI._funcParams = m_bFuncParamsHint;
	nppGUI._tabSize = m_nTabSize;
	svp._whiteSpaceShow = m_bWhiteSpaceShow;
	svp._eolShow = m_bEolShow;
	svp._indentGuideLineShow = m_bIndentGuideLineShow;
	svp._doWrap = m_bDoWrap;
	svp._wrapSymbolShow = m_bWrapSymbolShow;
	svp._zoom = m_nDefaultZoom;

	AfxGetApp()->WriteProfileInt(REG_HELP_SUBKEY, REG_HELP_WORD_TOOLTIP, m_bEnableWordTooltip);
	AfxGetApp()->WriteProfileInt(REG_HELP_SUBKEY, REG_HELP_VAR_TOOLTIP, m_bEnableVarTooltip);
	AfxGetApp()->WriteProfileInt(REG_HELP_SUBKEY, REG_HELP_WORDHELP_IN_DYNHELP, m_bShowWordHelpInDynamicHelp);
	AfxGetApp()->WriteProfileInt(REG_HELP_SUBKEY, REG_HELP_SEARCHWORD_ACCURATE, m_bSearchWordAccurate);

	AfxGetApp()->WriteProfileString(REG_OWMEDIT, REG_INTERPNAME, m_strInterpName);
	AfxGetApp()->WriteProfileInt(REG_OWMEDIT, REG_AUTORESET_INTERP, m_bAutoResetInterp);
	
	CDialog::OnOK();
}
