// TpsPythonCompile.cpp : implementation file
//

#include "stdafx.h"
#include "TpsPythonCompile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsPythonCompile dialog


CTpsPythonCompile::CTpsPythonCompile(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsPythonCompile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsPythonCompile)
	m_bAutoOutFile = AfxGetApp()->GetProfileInt(REG_PY_COMPILE_SUBKEY, REG_PY_COMPILE_AUTO_OUTFILE, TRUE);
	m_bForceOverwrite = AfxGetApp()->GetProfileInt(REG_PY_COMPILE_SUBKEY, REG_PY_COMPILE_FORCE_OVERWRITE, FALSE);
	//}}AFX_DATA_INIT
}


void CTpsPythonCompile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsPythonCompile)
	DDX_Check(pDX, IDC_CHECK_AUTO_OUTFILE, m_bAutoOutFile);
	DDX_Check(pDX, IDC_CHECK_FORCE_OVERWRITE, m_bForceOverwrite);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsPythonCompile, CDialog)
	//{{AFX_MSG_MAP(CTpsPythonCompile)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsPythonCompile message handlers

void CTpsPythonCompile::OnOK() 
{
	UpdateData(TRUE);

	AfxGetApp()->WriteProfileInt(REG_PY_COMPILE_SUBKEY, REG_PY_COMPILE_AUTO_OUTFILE, m_bAutoOutFile);
	AfxGetApp()->WriteProfileInt(REG_PY_COMPILE_SUBKEY, REG_PY_COMPILE_FORCE_OVERWRITE, m_bForceOverwrite);

	CDialog::OnOK();
}