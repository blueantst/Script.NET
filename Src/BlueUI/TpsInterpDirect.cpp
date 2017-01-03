// TpsInterpDirect.cpp : implementation file
//

#include "stdafx.h"
#include "blueui.h"
#include "TpsInterpDirect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsInterpDirect dialog


CTpsInterpDirect::CTpsInterpDirect(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsInterpDirect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsInterpDirect)
	m_bDirectOutput = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_OUTPUT, TRUE);
	m_bDirectFile = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_FILE, FALSE);
	m_bDirectResult = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_RESULT, TRUE);
	m_bDirectPipe = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_PIPE, FALSE);
	m_bDirectMemory = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_MEMORY, FALSE);
	m_bDirectAdmin = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_ADMIN, TRUE);
	m_nDirectOutputID = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_OUTPUT_ID, 2);

	//}}AFX_DATA_INIT
}


void CTpsInterpDirect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsInterpDirect)
	DDX_Control(pDX, IDC_SPIN_TCL_DIRECT_OUTPUTID, m_SpinDirectOutputID);
	DDX_Check(pDX, IDC_CHECK_DIRECT_OUTPUT, m_bDirectOutput);
	DDX_Check(pDX, IDC_CHECK_DIRECT_FILE, m_bDirectFile);
	DDX_Check(pDX, IDC_CHECK_DIRECT_RESULT, m_bDirectResult);
	DDX_Check(pDX, IDC_CHECK_DIRECT_PIPE, m_bDirectPipe);
	DDX_Check(pDX, IDC_CHECK_DIRECT_MEMORY, m_bDirectMemory);
	DDX_Check(pDX, IDC_CHECK_DIRECT_ADMIN, m_bDirectAdmin);
	DDX_Text(pDX, IDC_EDIT_DIRECT_OUTPUTID, m_nDirectOutputID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsInterpDirect, CDialog)
	//{{AFX_MSG_MAP(CTpsInterpDirect)
	ON_BN_CLICKED(IDC_CHECK_DIRECT_OUTPUT, OnCheckDirectOutput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsInterpDirect message handlers

BOOL CTpsInterpDirect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_SpinDirectOutputID.SetRange(1, 1000);
	m_SpinDirectOutputID.SetRange32(1, 1000);

	GetDlgItem(IDC_EDIT_DIRECT_OUTPUTID)->EnableWindow(m_bDirectOutput);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTpsInterpDirect::OnOK() 
{
	UpdateData(TRUE);

	AfxGetApp()->WriteProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_OUTPUT, m_bDirectOutput);
	AfxGetApp()->WriteProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_FILE, m_bDirectFile);
	AfxGetApp()->WriteProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_RESULT, m_bDirectResult);
	AfxGetApp()->WriteProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_PIPE, m_bDirectPipe);
	AfxGetApp()->WriteProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_MEMORY, m_bDirectMemory);
	AfxGetApp()->WriteProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_ADMIN, m_bDirectAdmin);
	AfxGetApp()->WriteProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_OUTPUT_ID, m_nDirectOutputID);

	CDialog::OnOK();
}

void CTpsInterpDirect::OnCheckDirectOutput() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_DIRECT_OUTPUTID)->EnableWindow(m_bDirectOutput);
}
