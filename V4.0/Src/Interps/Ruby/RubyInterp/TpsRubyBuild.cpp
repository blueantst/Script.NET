// TpsRubyBuild.cpp : implementation file
//

#include "stdafx.h"
#include "TpsRubyBuild.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsRubyBuild dialog


CTpsRubyBuild::CTpsRubyBuild(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsRubyBuild::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsRubyBuild)
	m_bAutoOutFile = AfxGetApp()->GetProfileInt(REG_RUBY_BUILD_SUBKEY, REG_RUBY_BUILD_AUTO_OUTFILE, TRUE);
	m_bAddRubyLib = AfxGetApp()->GetProfileInt(REG_RUBY_BUILD_SUBKEY, REG_RUBY_BUILD_ADD_RUBY_LIB, FALSE);
	m_bAddSourcePathFiles = AfxGetApp()->GetProfileInt(REG_RUBY_BUILD_SUBKEY, REG_RUBY_BUILD_ADD_SRC_PATHFILES, FALSE);
	m_bSelectBuildTool = AfxGetApp()->GetProfileInt(REG_RUBY_BUILD_SUBKEY, REG_RUBY_BUILD_SELECT_TOOL, TRUE);
	m_bSelectType = AfxGetApp()->GetProfileInt(REG_RUBY_BUILD_SUBKEY, REG_RUBY_BUILD_SELECT_TYPE, TRUE);
	m_bReplaceIcon = AfxGetApp()->GetProfileInt(REG_RUBY_BUILD_SUBKEY, REG_RUBY_BUILD_REPLACE_ICON, FALSE);
	m_nSelectIconType = AfxGetApp()->GetProfileInt(REG_RUBY_BUILD_SUBKEY, REG_RUBY_BUILD_SELECT_ICON_TYPE, 0);
	//}}AFX_DATA_INIT
}


void CTpsRubyBuild::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsRubyBuild)
	DDX_Check(pDX, IDC_CHECK_BUILD_AUTO_OUTFILE, m_bAutoOutFile);
	DDX_Check(pDX, IDC_CHECK_BUILD_ADD_RUBY_LIB, m_bAddRubyLib);
	DDX_Check(pDX, IDC_CHECK_BUILD_ADD_SOURCE_PATHFILES, m_bAddSourcePathFiles);
	DDX_Check(pDX, IDC_CHECK_BUILD_SELECT_TOOL, m_bSelectBuildTool);
	DDX_Check(pDX, IDC_CHECK_BUILD_SELECT_TYPE, m_bSelectType);
	DDX_Check(pDX, IDC_CHECK_BUILD_REPLACE_ICON, m_bReplaceIcon);
	DDX_Radio(pDX, IDC_RADIO_BUILD_USE_SOURCEICON, m_nSelectIconType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsRubyBuild, CDialog)
	//{{AFX_MSG_MAP(CTpsRubyBuild)
	ON_BN_CLICKED(IDC_CHECK_BUILD_REPLACE_ICON, OnCheckBuildReplaceIcon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsRubyBuild message handlers
void CTpsRubyBuild::OnOK() 
{
	UpdateData(TRUE);

	AfxGetApp()->WriteProfileInt(REG_RUBY_BUILD_SUBKEY, REG_RUBY_BUILD_AUTO_OUTFILE, m_bAutoOutFile);
	AfxGetApp()->WriteProfileInt(REG_RUBY_BUILD_SUBKEY, REG_RUBY_BUILD_ADD_RUBY_LIB, m_bAddRubyLib);
	AfxGetApp()->WriteProfileInt(REG_RUBY_BUILD_SUBKEY, REG_RUBY_BUILD_ADD_SRC_PATHFILES, m_bAddSourcePathFiles);
	AfxGetApp()->WriteProfileInt(REG_RUBY_BUILD_SUBKEY, REG_RUBY_BUILD_SELECT_TOOL, m_bSelectBuildTool);
	AfxGetApp()->WriteProfileInt(REG_RUBY_BUILD_SUBKEY, REG_RUBY_BUILD_SELECT_TYPE, m_bSelectType);
	AfxGetApp()->WriteProfileInt(REG_RUBY_BUILD_SUBKEY, REG_RUBY_BUILD_REPLACE_ICON, m_bReplaceIcon);
	AfxGetApp()->WriteProfileInt(REG_RUBY_BUILD_SUBKEY, REG_RUBY_BUILD_SELECT_ICON_TYPE, m_nSelectIconType);

	CDialog::OnOK();
}

BOOL CTpsRubyBuild::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_RADIO_BUILD_USE_SOURCEICON)->EnableWindow(m_bReplaceIcon);
	GetDlgItem(IDC_RADIO_BUILD_USER_SELECT_ICON)->EnableWindow(m_bReplaceIcon);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTpsRubyBuild::OnCheckBuildReplaceIcon() 
{
	UpdateData(TRUE);

	GetDlgItem(IDC_RADIO_BUILD_USE_SOURCEICON)->EnableWindow(m_bReplaceIcon);
	GetDlgItem(IDC_RADIO_BUILD_USER_SELECT_ICON)->EnableWindow(m_bReplaceIcon);
}
