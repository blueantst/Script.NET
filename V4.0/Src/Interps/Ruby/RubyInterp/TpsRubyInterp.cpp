// TpsRubyInterp.cpp : implementation file
//

#include "stdafx.h"
#include "TpsRubyInterp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsRubyInterp dialog


CTpsRubyInterp::CTpsRubyInterp(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsRubyInterp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsRubyInterp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTpsRubyInterp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsRubyInterp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsRubyInterp, CDialog)
	//{{AFX_MSG_MAP(CTpsRubyInterp)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsRubyInterp message handlers
