// TpsLuaInterp.cpp : implementation file
//

#include "stdafx.h"
#include "TpsLuaInterp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsLuaInterp dialog


CTpsLuaInterp::CTpsLuaInterp(CWnd* pParent /*=NULL*/)
	: CDialog(CTpsLuaInterp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsLuaInterp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTpsLuaInterp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsLuaInterp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsLuaInterp, CDialog)
	//{{AFX_MSG_MAP(CTpsLuaInterp)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsLuaInterp message handlers
