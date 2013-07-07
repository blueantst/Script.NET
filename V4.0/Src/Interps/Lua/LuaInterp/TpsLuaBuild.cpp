// TpsLuaBuild.cpp : implementation file
//

#include "stdafx.h"
#include "TpsLuaBuild.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsLuaBuild dialog


CTpsLuaBuild::CTpsLuaBuild(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsLuaBuild::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsLuaBuild)
	m_bAutoOutFile = AfxGetApp()->GetProfileInt(REG_LUA_BUILD_SUBKEY, REG_LUA_BUILD_AUTO_OUTFILE, TRUE);
	m_bAddLuaLib = AfxGetApp()->GetProfileInt(REG_LUA_BUILD_SUBKEY, REG_LUA_BUILD_ADD_LUA_LIB, FALSE);
	m_bAddSourcePathFiles = AfxGetApp()->GetProfileInt(REG_LUA_BUILD_SUBKEY, REG_LUA_BUILD_ADD_SRC_PATHFILES, FALSE);
	m_bAddSourceLib = AfxGetApp()->GetProfileInt(REG_LUA_BUILD_SUBKEY, REG_LUA_BUILD_ADD_SRC_DATA, FALSE);
	m_bWxScriptProcess = AfxGetApp()->GetProfileInt(REG_LUA_BUILD_SUBKEY, REG_LUA_BUILD_PROCESS_WXKIT, TRUE);
	m_bReplaceIcon = AfxGetApp()->GetProfileInt(REG_LUA_BUILD_SUBKEY, REG_LUA_BUILD_REPLACE_ICON, FALSE);
	m_nSelectIconType = AfxGetApp()->GetProfileInt(REG_LUA_BUILD_SUBKEY, REG_LUA_BUILD_SELECT_ICON_TYPE, 0);
	//}}AFX_DATA_INIT
}


void CTpsLuaBuild::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsLuaBuild)
	DDX_Check(pDX, IDC_CHECK_BUILD_AUTO_OUTFILE, m_bAutoOutFile);
	DDX_Check(pDX, IDC_CHECK_BUILD_ADD_LUA_LIB, m_bAddLuaLib);
	DDX_Check(pDX, IDC_CHECK_BUILD_ADD_SOURCE_PATHFILES, m_bAddSourcePathFiles);
	DDX_Check(pDX, IDC_CHECK_BUILD_ADD_SOURCE_LIB, m_bAddSourceLib);
	DDX_Check(pDX, IDC_CHECK_BUILD_WX_KIT, m_bWxScriptProcess);
	DDX_Check(pDX, IDC_CHECK_BUILD_REPLACE_ICON, m_bReplaceIcon);
	DDX_Radio(pDX, IDC_RADIO_BUILD_USE_SOURCEICON, m_nSelectIconType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsLuaBuild, CDialog)
	//{{AFX_MSG_MAP(CTpsLuaBuild)
	ON_BN_CLICKED(IDC_CHECK_BUILD_REPLACE_ICON, OnCheckBuildReplaceIcon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsLuaBuild message handlers
void CTpsLuaBuild::OnOK() 
{
	UpdateData(TRUE);

	AfxGetApp()->WriteProfileInt(REG_LUA_BUILD_SUBKEY, REG_LUA_BUILD_AUTO_OUTFILE, m_bAutoOutFile);
	AfxGetApp()->WriteProfileInt(REG_LUA_BUILD_SUBKEY, REG_LUA_BUILD_ADD_LUA_LIB, m_bAddLuaLib);
	AfxGetApp()->WriteProfileInt(REG_LUA_BUILD_SUBKEY, REG_LUA_BUILD_ADD_SRC_PATHFILES, m_bAddSourcePathFiles);
	AfxGetApp()->WriteProfileInt(REG_LUA_BUILD_SUBKEY, REG_LUA_BUILD_ADD_SRC_DATA, m_bAddSourceLib);
	AfxGetApp()->WriteProfileInt(REG_LUA_BUILD_SUBKEY, REG_LUA_BUILD_PROCESS_WXKIT, m_bWxScriptProcess);
	AfxGetApp()->WriteProfileInt(REG_LUA_BUILD_SUBKEY, REG_LUA_BUILD_REPLACE_ICON, m_bReplaceIcon);
	AfxGetApp()->WriteProfileInt(REG_LUA_BUILD_SUBKEY, REG_LUA_BUILD_SELECT_ICON_TYPE, m_nSelectIconType);

	CDialog::OnOK();
}

BOOL CTpsLuaBuild::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_RADIO_BUILD_USE_SOURCEICON)->EnableWindow(m_bReplaceIcon);
	GetDlgItem(IDC_RADIO_BUILD_USER_SELECT_ICON)->EnableWindow(m_bReplaceIcon);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTpsLuaBuild::OnCheckBuildReplaceIcon() 
{
	UpdateData(TRUE);

	GetDlgItem(IDC_RADIO_BUILD_USE_SOURCEICON)->EnableWindow(m_bReplaceIcon);
	GetDlgItem(IDC_RADIO_BUILD_USER_SELECT_ICON)->EnableWindow(m_bReplaceIcon);
}
