// TpsLuaCompile.cpp : implementation file
//

#include "stdafx.h"
#include "TpsLuaCompile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsLuaCompile dialog


CTpsLuaCompile::CTpsLuaCompile(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsLuaCompile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsLuaCompile)
	m_bAutoOutFile = AfxGetApp()->GetProfileInt(REG_LUA_COMPILE_SUBKEY, REG_LUA_COMPILE_AUTO_OUTFILE, TRUE);
	m_bForceOverwrite = AfxGetApp()->GetProfileInt(REG_LUA_COMPILE_SUBKEY, REG_LUA_COMPILE_FORCE_OVERWRITE, FALSE);
	//}}AFX_DATA_INIT
}


void CTpsLuaCompile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsLuaCompile)
	DDX_Check(pDX, IDC_CHECK_AUTO_OUTFILE, m_bAutoOutFile);
	DDX_Check(pDX, IDC_CHECK_FORCE_OVERWRITE, m_bForceOverwrite);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsLuaCompile, CDialog)
	//{{AFX_MSG_MAP(CTpsLuaCompile)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsLuaCompile message handlers

void CTpsLuaCompile::OnOK() 
{
	UpdateData(TRUE);

	AfxGetApp()->WriteProfileInt(REG_LUA_COMPILE_SUBKEY, REG_LUA_COMPILE_AUTO_OUTFILE, m_bAutoOutFile);
	AfxGetApp()->WriteProfileInt(REG_LUA_COMPILE_SUBKEY, REG_LUA_COMPILE_FORCE_OVERWRITE, m_bForceOverwrite);

	CDialog::OnOK();
}