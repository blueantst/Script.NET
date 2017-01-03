// TpsPerlDebug.cpp : implementation file
//

#include "stdafx.h"
#include "TpsPerlDebug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsPerlDebug dialog


CTpsPerlDebug::CTpsPerlDebug(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsPerlDebug::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsPerlDebug)
	m_bEnableProfileAnalysis = AfxGetApp()->GetProfileInt(REG_PERL_DEBUG_SUBKEY, REG_PERL_ENABLE_PROFILE, FALSE);
	m_bEnableMulitFileDebug = AfxGetApp()->GetProfileInt(REG_PERL_DEBUG_SUBKEY, REG_PERL_ENABLE_MULTIFILEDEBUG, FALSE);
	m_bMultiDebugQuery = AfxGetApp()->GetProfileInt(REG_PERL_DEBUG_SUBKEY, REG_PERL_MULTIDEBUG_QUERY, FALSE);
	m_bMultiDebugExcludeLib = AfxGetApp()->GetProfileInt(REG_PERL_DEBUG_SUBKEY, REG_PERL_MULTIDEBUG_EXCLUDE_LIB, TRUE);
	//}}AFX_DATA_INIT
}


void CTpsPerlDebug::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsPerlDebug)
	DDX_Check(pDX, IDC_CHECK_ENABLE_MULITFILEDEBUG, m_bEnableMulitFileDebug);
	DDX_Check(pDX, IDC_CHECK_ENABLE_PROFILEANALYSIS, m_bEnableProfileAnalysis);
	DDX_Check(pDX, IDC_CHECK_MULITDEBUG_QUERY, m_bMultiDebugQuery);
	DDX_Check(pDX, IDC_CHECK_MULITDEBUG_EXCLUDE_LIB, m_bMultiDebugExcludeLib);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsPerlDebug, CDialog)
	//{{AFX_MSG_MAP(CTpsPerlDebug)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_MULITFILEDEBUG, OnCheckEnableMulitfiledebug)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsPerlDebug message handlers
void CTpsPerlDebug::OnOK() 
{
	UpdateData(TRUE);

	AfxGetApp()->WriteProfileInt(REG_PERL_DEBUG_SUBKEY, REG_PERL_ENABLE_PROFILE, m_bEnableProfileAnalysis);
	AfxGetApp()->WriteProfileInt(REG_PERL_DEBUG_SUBKEY, REG_PERL_ENABLE_MULTIFILEDEBUG, m_bEnableMulitFileDebug);
	AfxGetApp()->WriteProfileInt(REG_PERL_DEBUG_SUBKEY, REG_PERL_MULTIDEBUG_QUERY, m_bMultiDebugQuery);
	AfxGetApp()->WriteProfileInt(REG_PERL_DEBUG_SUBKEY, REG_PERL_MULTIDEBUG_EXCLUDE_LIB, m_bMultiDebugExcludeLib);

	CDialog::OnOK();
}


BOOL CTpsPerlDebug::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_CHECK_MULITDEBUG_QUERY)->EnableWindow(m_bEnableMulitFileDebug);
	GetDlgItem(IDC_CHECK_MULITDEBUG_EXCLUDE_LIB)->EnableWindow(m_bEnableMulitFileDebug);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTpsPerlDebug::OnCheckEnableMulitfiledebug() 
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CHECK_MULITDEBUG_QUERY)->EnableWindow(m_bEnableMulitFileDebug);
	GetDlgItem(IDC_CHECK_MULITDEBUG_EXCLUDE_LIB)->EnableWindow(m_bEnableMulitFileDebug);
}
