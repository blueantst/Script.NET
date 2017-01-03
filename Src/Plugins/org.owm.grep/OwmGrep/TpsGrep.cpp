// TpsGrep.cpp : implementation file
//

#include "stdafx.h"
#include "OwmGrep.h"
#include "TpsGrep.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsGrep dialog


CTpsGrep::CTpsGrep(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsGrep::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsGrep)
	m_bShowReplaceLine = AfxGetApp()->GetProfileInt(REG_GREP, REG_SHOW_REPLACE_LINE, FALSE);
	//}}AFX_DATA_INIT
}


void CTpsGrep::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsGrep)
	DDX_Check(pDX, IDC_CHECK_SHOW_REPLACE_LINE, m_bShowReplaceLine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsGrep, CDialog)
	//{{AFX_MSG_MAP(CTpsGrep)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsGrep message handlers
void CTpsGrep::OnOK() 
{
	UpdateData(TRUE);

	AfxGetApp()->WriteProfileInt(REG_GREP, REG_SHOW_REPLACE_LINE, m_bShowReplaceLine);
	
	CDialog::OnOK();
}