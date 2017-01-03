// DlgComParam.cpp : implementation file
//

#include "stdafx.h"
#include "PTerminal.h"
#include "DlgComParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgComParam dialog


CDlgComParam::CDlgComParam(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgComParam::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgComParam)
	m_strComPort = _T("COM1");
	m_strBaud = _T("115200");
	m_nDataBits = 1;
	m_nStopBits = 0;
	m_nParity = 2;
	m_bEcho = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgComParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgComParam)
	DDX_CBString(pDX, IDC_COMBO_COM, m_strComPort);
	DDX_CBString(pDX, IDC_COMBO_BAUD, m_strBaud);
	DDX_CBIndex(pDX, IDC_COMBO_DATABITS, m_nDataBits);
	DDX_CBIndex(pDX, IDC_COMBO_STOPBITS, m_nStopBits);
	DDX_CBIndex(pDX, IDC_COMBO_PARITY, m_nParity);
	DDX_Check(pDX, IDC_CHECK_ECHO, m_bEcho);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgComParam, CDialog)
	//{{AFX_MSG_MAP(CDlgComParam)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgComParam message handlers
