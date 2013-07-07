// TpsVci.cpp : implementation file
//

#include "stdafx.h"
#include "blueui.h"
#include "TpsVci.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsVci dialog


CTpsVci::CTpsVci(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsVci::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsVci)
	m_bEnableDebugOut = theApp.GetProfileInt(REG_VCI_SUBKEY, REG_VCI_ENABLE_DEBUGOUT, FALSE);
	//}}AFX_DATA_INIT
}


void CTpsVci::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsVci)
	DDX_Check(pDX, IDC_CHECK_ENABLE_DEBUGOUT, m_bEnableDebugOut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsVci, CDialog)
	//{{AFX_MSG_MAP(CTpsVci)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsVci message handlers
void CTpsVci::OnOK() 
{
	UpdateData(TRUE);

	theApp.WriteProfileInt(REG_VCI_SUBKEY, REG_VCI_ENABLE_DEBUGOUT, m_bEnableDebugOut);

	CDialog::OnOK();
}
