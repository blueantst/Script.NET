// TpsTclCompile.cpp : implementation file
//

#include "stdafx.h"
#include "TpsTclCompile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsTclCompile dialog


CTpsTclCompile::CTpsTclCompile(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsTclCompile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsTclCompile)
	m_bAutoOutFile = AfxGetApp()->GetProfileInt(REG_TCL_COMPILE_SUBKEY, REG_TCL_COMPILE_AUTO_OUTFILE, TRUE);
	m_bForceOverwrite = AfxGetApp()->GetProfileInt(REG_TCL_COMPILE_SUBKEY, REG_TCL_COMPILE_FORCE_OVERWRITE, FALSE);
	m_bAddPrefix = AfxGetApp()->GetProfileInt(REG_TCL_COMPILE_SUBKEY, REG_TCL_COMPILE_ADD_PREFIX, TRUE);
	//}}AFX_DATA_INIT
}


void CTpsTclCompile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsTclCompile)
	DDX_Check(pDX, IDC_CHECK_AUTO_OUTFILE, m_bAutoOutFile);
	DDX_Check(pDX, IDC_CHECK_FORCE_OVERWRITE, m_bForceOverwrite);
	DDX_Check(pDX, IDC_CHECK_ADD_PREFIX, m_bAddPrefix);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsTclCompile, CDialog)
	//{{AFX_MSG_MAP(CTpsTclCompile)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsTclCompile message handlers

void CTpsTclCompile::OnOK() 
{
	UpdateData(TRUE);

	AfxGetApp()->WriteProfileInt(REG_TCL_COMPILE_SUBKEY, REG_TCL_COMPILE_AUTO_OUTFILE, m_bAutoOutFile);
	AfxGetApp()->WriteProfileInt(REG_TCL_COMPILE_SUBKEY, REG_TCL_COMPILE_FORCE_OVERWRITE, m_bForceOverwrite);
	AfxGetApp()->WriteProfileInt(REG_TCL_COMPILE_SUBKEY, REG_TCL_COMPILE_ADD_PREFIX, m_bAddPrefix);

	CDialog::OnOK();
}
