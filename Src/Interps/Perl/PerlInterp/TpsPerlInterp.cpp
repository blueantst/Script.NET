// TpsPerlInterp.cpp : implementation file
//

#include "stdafx.h"
#include "TpsPerlInterp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsPerlInterp dialog


CTpsPerlInterp::CTpsPerlInterp(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsPerlInterp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsPerlInterp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTpsPerlInterp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsPerlInterp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsPerlInterp, CDialog)
	//{{AFX_MSG_MAP(CTpsPerlInterp)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsPerlInterp message handlers
