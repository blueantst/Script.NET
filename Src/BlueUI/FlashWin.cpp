// FlashWin.cpp : implementation file
//

#include "stdafx.h"
#include "FlashWin.h"
#include "BlueUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FlashWin dialog


FlashWin::FlashWin(CWnd* pParent /*=NULL*/)
	: CDialog(FlashWin::IDD, pParent)
{
	//{{AFX_DATA_INIT(FlashWin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nIcon = 1;
}


void FlashWin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FlashWin)
	DDX_Control(pDX, IDC_TITLE, m_csTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FlashWin, CDialog)
	//{{AFX_MSG_MAP(FlashWin)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FlashWin message handlers

void FlashWin::SetMessage(CString strMsg) 
{
	m_csTitle.SetWindowText(strMsg);
	UpdateData(FALSE);
}

BOOL FlashWin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void FlashWin::OnTimer(UINT nIDEvent)
{
/*
	m_nIcon = (m_nIcon==4) ? 1 : m_nIcon++;
	HICON hIcon;
	switch(m_nIcon)
	{
		case 1: hIcon = theApp.LoadIcon(IDI_ICON_FLASH1); break;
		case 2: hIcon = theApp.LoadIcon(IDI_ICON_FLASH2); break;
		case 3: hIcon = theApp.LoadIcon(IDI_ICON_FLASH3); break;
		case 4: hIcon = theApp.LoadIcon(IDI_ICON_FLASH4); break;
		default: hIcon = theApp.LoadIcon(IDI_ICON_FLASH1); break;
	}
	((CStatic*)GetDlgItem(IDC_STATIC_FLASH))->SetIcon(hIcon);
*/
}

void FlashWin::StartTimer()
{
//	SetTimer(10, 200, NULL);
}

void FlashWin::StopTimer()
{
//	KillTimer(10);
}