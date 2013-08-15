// TpsPythonInterp.cpp : implementation file
//

#include "stdafx.h"
#include "TpsPythonInterp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsPythonInterp dialog


CTpsPythonInterp::CTpsPythonInterp(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsPythonInterp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsPythonInterp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTpsPythonInterp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsPythonInterp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsPythonInterp, CDialog)
	//{{AFX_MSG_MAP(CTpsPythonInterp)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsPythonInterp message handlers
