// TpsTerminal.cpp : implementation file
//

#include "stdafx.h"
#include "PTerminal.h"
#include "TpsTerminal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsTerminal dialog


CTpsTerminal::CTpsTerminal(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsTerminal::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsTerminal)
	m_nDefaultShell = AfxGetApp()->GetProfileInt(REG_PTERMINAL, REG_DEFAULTSHELL, SHELL_TCL);
	m_strTclInterpName = AfxGetApp()->GetProfileString(REG_PTERMINAL, REG_INTERPNAME, INTERP_NAME);
	m_bEnableListDlg = AfxGetApp()->GetProfileInt(REG_PTERMINAL, REG_ENABLE_LISTDLG, TRUE);
	//}}AFX_DATA_INIT
}


void CTpsTerminal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsTerminal)
	DDX_CBIndex(pDX, IDC_COMBO_DEFAULTSHELL, m_nDefaultShell);
	DDX_Text(pDX, IDC_EDIT_TCLINTERPNAME, m_strTclInterpName);
	DDV_MaxChars(pDX, m_strTclInterpName, 50);
	DDX_Check(pDX, IDC_CHECK_ENABLE_LISTDLG, m_bEnableListDlg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsTerminal, CDialog)
	//{{AFX_MSG_MAP(CTpsTerminal)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsTerminal message handlers
void CTpsTerminal::OnOK() 
{
	UpdateData(TRUE);

	AfxGetApp()->WriteProfileInt(REG_PTERMINAL, REG_DEFAULTSHELL, m_nDefaultShell);
	AfxGetApp()->WriteProfileString(REG_PTERMINAL, REG_INTERPNAME, m_strTclInterpName);
	AfxGetApp()->WriteProfileInt(REG_PTERMINAL, REG_ENABLE_LISTDLG, m_bEnableListDlg);
	
	CDialog::OnOK();
}
