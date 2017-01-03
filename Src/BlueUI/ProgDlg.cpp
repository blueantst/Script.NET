//  ProgDlg.cpp : implementation file
// CG: This file was added by the Progress Dialog component

#include "stdafx.h"
#include "BlueUI.h"
#include "resource.h"
#include "ProgDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog

CProgressDlg::CProgressDlg(CString strCaption)
{
	m_strCaption = strCaption;

    m_bCancel=FALSE;
	m_bEnd=FALSE;
    m_nLower=0;
    m_nUpper=100;
    m_nStep=10;
    //{{AFX_DATA_INIT(CProgressDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_bParentDisabled = FALSE;

	m_strCancelScript = "";
}

CProgressDlg::~CProgressDlg()
{
    if(m_hWnd!=NULL)
      DestroyWindow();
}

BOOL CProgressDlg::DestroyWindow()
{
    ReEnableParent();
    return CDialog::DestroyWindow();
}

void CProgressDlg::ReEnableParent()
{
    if(m_bParentDisabled && (m_pParentWnd!=NULL))
      m_pParentWnd->EnableWindow(TRUE);
    m_bParentDisabled=FALSE;
}

BOOL CProgressDlg::Create(CWnd *pParent)
{
    // Get the true parent of the dialog
    m_pParentWnd = CWnd::GetSafeOwner(pParent);

    // m_bParentDisabled is used to re-enable the parent window
    // when the dialog is destroyed. So we don't want to set
    // it to TRUE unless the parent was already enabled.

    if((m_pParentWnd!=NULL) && m_pParentWnd->IsWindowEnabled())
    {
      m_pParentWnd->EnableWindow(FALSE);
      m_bParentDisabled = TRUE;
    }

    if(!CDialog::Create(CProgressDlg::IDD,pParent))
    {
      ReEnableParent();
      return FALSE;
    }

    return TRUE;
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CProgressDlg)
    DDX_Control(pDX, CG_IDC_PROGDLG_PROGRESS, m_Progress);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
    //{{AFX_MSG_MAP(CProgressDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CProgressDlg::SetStatus(LPCTSTR lpszMessage)
{
    ASSERT(m_hWnd); // Don't call this _before_ the dialog has
                    // been created. Can be called from OnInitDialog
    CWnd *pWndStatus = GetDlgItem(CG_IDC_PROGDLG_STATUS);

    // Verify that the static text control exists
    ASSERT(pWndStatus!=NULL);
    pWndStatus->SetWindowText(lpszMessage);
}

void CProgressDlg::OnCancel()
{
    m_bCancel=TRUE;
	PCOM_CMD("0", OC_PROGDLG_CANCEL, 0, 0);
}

void CProgressDlg::EndDialog()
{
    m_bEnd=TRUE;
}

void CProgressDlg::SetRange(int nLower,int nUpper)
{
    m_nLower = nLower;
    m_nUpper = nUpper;
    m_Progress.SetRange(nLower,nUpper);
}

int CProgressDlg::SetPos(int nPos)
{
//    PumpMessages();
    int iResult = m_Progress.SetPos(nPos);
    return iResult;
}

int CProgressDlg::SetStep(int nStep)
{
    m_nStep = nStep; // Store for later use in calculating percentage
    return m_Progress.SetStep(nStep);
}

int CProgressDlg::OffsetPos(int nPos)
{
    PumpMessages();
    int iResult = m_Progress.OffsetPos(nPos);
    return iResult;
}

int CProgressDlg::StepIt()
{
    PumpMessages();
    int iResult = m_Progress.StepIt();
    return iResult;
}

void CProgressDlg::PumpMessages()
{
    // Must call Create() before using the dialog
    ASSERT(m_hWnd!=NULL);

    MSG msg;
    // Handle dialog messages
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if(!IsDialogMessage(&msg))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);  
      }
    }
}

BOOL CProgressDlg::CheckCancelButton()
{
    // Process all pending messages
    PumpMessages();

    // Reset m_bCancel to FALSE so that
    // CheckCancelButton returns FALSE until the user
    // clicks Cancel again. This will allow you to call
    // CheckCancelButton and still continue the operation.
    // If m_bCancel stayed TRUE, then the next call to
    // CheckCancelButton would always return TRUE

    BOOL bResult = m_bCancel;
    m_bCancel = FALSE;

    return bResult;
}

int CProgressDlg::LoopDialog()
{
	while(!m_bCancel && !m_bEnd)
	{
	    // Process all pending messages
		PumpMessages();
	}

    // Reset m_bCancel to FALSE so that
    // CheckCancelButton returns FALSE until the user
    // clicks Cancel again. This will allow you to call
    // CheckCancelButton and still continue the operation.
    // If m_bCancel stayed TRUE, then the next call to
    // CheckCancelButton would always return TRUE

    int nResult;
	if(m_bCancel)
		nResult = 0;
	else
		nResult = 1;

    m_bCancel = FALSE;
	m_bEnd = FALSE;

    return nResult;
}
    
/////////////////////////////////////////////////////////////////////////////
// CProgressDlg message handlers

BOOL CProgressDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();

	// Flat按钮初始化
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_REPORT_CTRL);
	CImageList imageList;
	imageList.Create(16, 16, ILC_COLOR24|ILC_MASK, 1, 1);
	imageList.Add(&bitmap, RGB(255,0,255));
	m_btnCancel.SetIcon(CSize(16,16), imageList.ExtractIcon(24));
	m_btnCancel.SetXButtonStyle(BS_XT_HILITEPRESSED | BS_XT_SHOWFOCUS | BS_XT_XPFLAT);

	//进度条初始化
	m_Progress.SetShowText(TRUE); 
	m_Progress.SetBkColour(RGB(192,192,192));
	m_Progress.ShowWindow(SW_SHOW);
    m_Progress.SetRange(m_nLower,m_nUpper);
    m_Progress.SetStep(m_nStep);
    m_Progress.SetPos(m_nLower);

	if(m_strCaption == "")
	{
		//PLAT_LOADSTRING(strCaption, IDS_PROGDLG_PROGRESS);
		SetWindowText("Progress");
	}else
	{
		SetWindowText(m_strCaption);
	}

    return TRUE;  
}
