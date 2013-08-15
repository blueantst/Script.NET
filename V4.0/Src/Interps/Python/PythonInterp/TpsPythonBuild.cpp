// TpsPythonBuild.cpp : implementation file
//

#include "stdafx.h"
#include "TpsPythonBuild.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsPythonBuild dialog


CTpsPythonBuild::CTpsPythonBuild(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsPythonBuild::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsPythonBuild)
	m_bAutoOutFile = AfxGetApp()->GetProfileInt(REG_PY_BUILD_SUBKEY, REG_PY_BUILD_AUTO_OUTFILE, TRUE);
	m_bAddPythonLib = AfxGetApp()->GetProfileInt(REG_PY_BUILD_SUBKEY, REG_PY_BUILD_ADD_PYTHON_LIB, FALSE);
	m_bAddSourcePathFiles = AfxGetApp()->GetProfileInt(REG_PY_BUILD_SUBKEY, REG_PY_BUILD_ADD_SRC_PATHFILES, FALSE);
	m_bAddSourceData = AfxGetApp()->GetProfileInt(REG_PY_BUILD_SUBKEY, REG_PY_BUILD_ADD_SRC_DATA, FALSE);
	m_bSelectType = AfxGetApp()->GetProfileInt(REG_PY_BUILD_SUBKEY, REG_PY_BUILD_SELECT_TYPE, TRUE);
	m_bReplaceIcon = AfxGetApp()->GetProfileInt(REG_PY_BUILD_SUBKEY, REG_PY_BUILD_REPLACE_ICON, FALSE);
	m_nSelectIconType = AfxGetApp()->GetProfileInt(REG_PY_BUILD_SUBKEY, REG_PY_BUILD_SELECT_ICON_TYPE, 0);
	//}}AFX_DATA_INIT
}


void CTpsPythonBuild::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsPythonBuild)
	DDX_Check(pDX, IDC_CHECK_BUILD_AUTO_OUTFILE, m_bAutoOutFile);
	DDX_Check(pDX, IDC_CHECK_BUILD_ADD_PYTHON_LIB, m_bAddPythonLib);
	DDX_Check(pDX, IDC_CHECK_BUILD_ADD_SOURCE_PATHFILES, m_bAddSourcePathFiles);
	DDX_Check(pDX, IDC_CHECK_BUILD_ADD_SOURCE_DATA, m_bAddSourceData);
	DDX_Check(pDX, IDC_CHECK_BUILD_SELECT_TYPE, m_bSelectType);
	DDX_Check(pDX, IDC_CHECK_BUILD_REPLACE_ICON, m_bReplaceIcon);
	DDX_Radio(pDX, IDC_RADIO_BUILD_USE_SOURCEICON, m_nSelectIconType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsPythonBuild, CDialog)
	//{{AFX_MSG_MAP(CTpsPythonBuild)
	ON_BN_CLICKED(IDC_CHECK_BUILD_REPLACE_ICON, OnCheckBuildReplaceIcon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsPythonBuild message handlers
void CTpsPythonBuild::OnOK() 
{
	UpdateData(TRUE);

	AfxGetApp()->WriteProfileInt(REG_PY_BUILD_SUBKEY, REG_PY_BUILD_AUTO_OUTFILE, m_bAutoOutFile);
	AfxGetApp()->WriteProfileInt(REG_PY_BUILD_SUBKEY, REG_PY_BUILD_ADD_PYTHON_LIB, m_bAddPythonLib);
	AfxGetApp()->WriteProfileInt(REG_PY_BUILD_SUBKEY, REG_PY_BUILD_ADD_SRC_PATHFILES, m_bAddSourcePathFiles);
	AfxGetApp()->WriteProfileInt(REG_PY_BUILD_SUBKEY, REG_PY_BUILD_ADD_SRC_DATA, m_bAddSourceData);
	AfxGetApp()->WriteProfileInt(REG_PY_BUILD_SUBKEY, REG_PY_BUILD_SELECT_TYPE, m_bSelectType);
	AfxGetApp()->WriteProfileInt(REG_PY_BUILD_SUBKEY, REG_PY_BUILD_REPLACE_ICON, m_bReplaceIcon);
	AfxGetApp()->WriteProfileInt(REG_PY_BUILD_SUBKEY, REG_PY_BUILD_SELECT_ICON_TYPE, m_nSelectIconType);

	CDialog::OnOK();
}

BOOL CTpsPythonBuild::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_RADIO_BUILD_USE_SOURCEICON)->EnableWindow(m_bReplaceIcon);
	GetDlgItem(IDC_RADIO_BUILD_USER_SELECT_ICON)->EnableWindow(m_bReplaceIcon);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTpsPythonBuild::OnCheckBuildReplaceIcon() 
{
	UpdateData(TRUE);

	GetDlgItem(IDC_RADIO_BUILD_USE_SOURCEICON)->EnableWindow(m_bReplaceIcon);
	GetDlgItem(IDC_RADIO_BUILD_USER_SELECT_ICON)->EnableWindow(m_bReplaceIcon);
}
