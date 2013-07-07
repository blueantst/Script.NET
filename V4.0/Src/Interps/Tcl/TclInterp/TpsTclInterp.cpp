// TpsTclInterp.cpp : implementation file
//

#include "stdafx.h"
#include "TpsTclInterp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsTclInterp dialog


CTpsTclInterp::CTpsTclInterp(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsTclInterp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsTclInterp)
	m_bLoadLibTk = AfxGetApp()->GetProfileInt(REG_TCL_LIB_SUBKEY, REG_TCL_LIB_TK, FALSE);
	m_bLoadLibITcl = AfxGetApp()->GetProfileInt(REG_TCL_LIB_SUBKEY, REG_TCL_LIB_ITCL, TRUE);
	m_bLoadLibITk = AfxGetApp()->GetProfileInt(REG_TCL_LIB_SUBKEY, REG_TCL_LIB_ITK, FALSE);
	//}}AFX_DATA_INIT
}


void CTpsTclInterp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsTclInterp)
	DDX_Check(pDX, IDC_LOAD_LIB_TK, m_bLoadLibTk);
	DDX_Check(pDX, IDC_LOAD_LIB_ITCL, m_bLoadLibITcl);
	DDX_Check(pDX, IDC_LOAD_LIB_ITK, m_bLoadLibITk);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsTclInterp, CDialog)
	//{{AFX_MSG_MAP(CTpsTclInterp)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsTclInterp message handlers
/////////////////////////////////////////////////////////////////////////////
// CTpsOwmEdit message handlers
void CTpsTclInterp::OnOK() 
{
	UpdateData(TRUE);

	AfxGetApp()->WriteProfileInt(REG_TCL_LIB_SUBKEY, REG_TCL_LIB_TK, m_bLoadLibTk);
	AfxGetApp()->WriteProfileInt(REG_TCL_LIB_SUBKEY, REG_TCL_LIB_ITCL, m_bLoadLibITcl);
	AfxGetApp()->WriteProfileInt(REG_TCL_LIB_SUBKEY, REG_TCL_LIB_ITK, m_bLoadLibITk);

	CDialog::OnOK();
}
