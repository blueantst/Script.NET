// TpsTclBuild.cpp : implementation file
//

#include "stdafx.h"
#include "TpsTclBuild.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsTclBuild dialog


CTpsTclBuild::CTpsTclBuild(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsTclBuild::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsTclBuild)
	m_bAutoOutFile = AfxGetApp()->GetProfileInt(REG_TCL_BUILD_SUBKEY, REG_TCL_BUILD_AUTO_OUTFILE, TRUE);
	m_bAddTclLib = AfxGetApp()->GetProfileInt(REG_TCL_BUILD_SUBKEY, REG_TCL_BUILD_ADD_TCL_LIB, TRUE);
	m_bAddSourcePathFiles = AfxGetApp()->GetProfileInt(REG_TCL_BUILD_SUBKEY, REG_TCL_BUILD_ADD_SRC_PATHFILES, FALSE);
	m_bAddSourceLib = AfxGetApp()->GetProfileInt(REG_TCL_BUILD_SUBKEY, REG_TCL_BUILD_ADD_SRC_LIB, TRUE);
	m_bForceTkKit = AfxGetApp()->GetProfileInt(REG_TCL_BUILD_SUBKEY, REG_TCL_BUILD_FORCE_TKKIT, FALSE);
	m_bReplaceIcon = AfxGetApp()->GetProfileInt(REG_TCL_BUILD_SUBKEY, REG_TCL_BUILD_REPLACE_ICON, FALSE);
	m_nSelectIconType = AfxGetApp()->GetProfileInt(REG_TCL_BUILD_SUBKEY, REG_TCL_BUILD_SELECT_ICON_TYPE, 0);
	//}}AFX_DATA_INIT
}


void CTpsTclBuild::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsTclBuild)
	DDX_Check(pDX, IDC_CHECK_BUILD_AUTO_OUTFILE, m_bAutoOutFile);
	DDX_Check(pDX, IDC_CHECK_BUILD_ADD_TCL_LIB, m_bAddTclLib);
	DDX_Check(pDX, IDC_CHECK_BUILD_ADD_SOURCE_PATHFILES, m_bAddSourcePathFiles);
	DDX_Check(pDX, IDC_CHECK_BUILD_ADD_SOURCE_LIB, m_bAddSourceLib);
	DDX_Check(pDX, IDC_CHECK_BUILD_TK_KIT, m_bForceTkKit);
	DDX_Check(pDX, IDC_CHECK_BUILD_REPLACE_ICON, m_bReplaceIcon);
	DDX_Radio(pDX, IDC_RADIO_BUILD_USE_SOURCEICON, m_nSelectIconType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsTclBuild, CDialog)
	//{{AFX_MSG_MAP(CTpsTclBuild)
	ON_BN_CLICKED(IDC_CHECK_BUILD_REPLACE_ICON, OnCheckBuildReplaceIcon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsTclBuild message handlers

void CTpsTclBuild::OnOK() 
{
	UpdateData(TRUE);

	AfxGetApp()->WriteProfileInt(REG_TCL_BUILD_SUBKEY, REG_TCL_BUILD_AUTO_OUTFILE, m_bAutoOutFile);
	AfxGetApp()->WriteProfileInt(REG_TCL_BUILD_SUBKEY, REG_TCL_BUILD_ADD_TCL_LIB, m_bAddTclLib);
	AfxGetApp()->WriteProfileInt(REG_TCL_BUILD_SUBKEY, REG_TCL_BUILD_ADD_SRC_PATHFILES, m_bAddSourcePathFiles);
	AfxGetApp()->WriteProfileInt(REG_TCL_BUILD_SUBKEY, REG_TCL_BUILD_ADD_SRC_LIB, m_bAddSourceLib);
	AfxGetApp()->WriteProfileInt(REG_TCL_BUILD_SUBKEY, REG_TCL_BUILD_FORCE_TKKIT, m_bForceTkKit);
	AfxGetApp()->WriteProfileInt(REG_TCL_BUILD_SUBKEY, REG_TCL_BUILD_REPLACE_ICON, m_bReplaceIcon);
	AfxGetApp()->WriteProfileInt(REG_TCL_BUILD_SUBKEY, REG_TCL_BUILD_SELECT_ICON_TYPE, m_nSelectIconType);

	CDialog::OnOK();
}

BOOL CTpsTclBuild::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_RADIO_BUILD_USE_SOURCEICON)->EnableWindow(m_bReplaceIcon);
	GetDlgItem(IDC_RADIO_BUILD_USER_SELECT_ICON)->EnableWindow(m_bReplaceIcon);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTpsTclBuild::OnCheckBuildReplaceIcon() 
{
	UpdateData(TRUE);

	GetDlgItem(IDC_RADIO_BUILD_USE_SOURCEICON)->EnableWindow(m_bReplaceIcon);
	GetDlgItem(IDC_RADIO_BUILD_USER_SELECT_ICON)->EnableWindow(m_bReplaceIcon);
}
