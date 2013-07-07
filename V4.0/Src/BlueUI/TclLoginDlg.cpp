// TclLoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BlueUI.h"
#include "TclLoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTclLoginDlg dialog


CTclLoginDlg::CTclLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTclLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTclLoginDlg)
	m_strAddress = _T("");
	m_strUserName = _T("");
	m_strPassword = _T("");
	m_strDomain = _T("");
	m_bCheck = FALSE;	
	//}}AFX_DATA_INIT
	PLAT_LOADSTRING(strLoginTitle, IDS_LOGIN_TITLE); // 登录
	PLAT_LOADSTRING(strLoginInfo, IDS_LOGIN_INFO); // 请输入登录地址和用户
	m_strTitle = strLoginTitle;
	m_strInfo = strLoginInfo;
	m_strCheck = _T("");
	m_strDomainList = _T("");
	m_pInfo = NULL;
}


void CTclLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTclLoginDlg)
	DDX_Control(pDX, IDC_COMBO_DOMAIN, m_comboDomain);
	DDX_Text(pDX, IDC_LOGIN_ADDRESS, m_strAddress);
	DDX_Text(pDX, IDC_LOGIN_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_LOGIN_PASSWORD, m_strPassword);
	DDX_Check(pDX, IDC_CHECK_LOGIN, m_bCheck);
	DDX_CBString(pDX, IDC_COMBO_DOMAIN, m_strDomain);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTclLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CTclLoginDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTclLoginDlg message handlers

/////////////////////////////////////////////////////////////////////////////
// 对话框初始化
/////////////////////////////////////////////////////////////////////////////
BOOL CTclLoginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText(m_strTitle);
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText(m_strInfo);

	if(m_strCheck == "")
	{
		GetDlgItem(IDC_CHECK_LOGIN)->ShowWindow(SW_HIDE);
	}else
	{
		GetDlgItem(IDC_CHECK_LOGIN)->SetWindowText(m_strCheck);
		if(m_pInfo && m_pInfo->bCheck)
			m_bCheck = TRUE;
	}

	if(m_strDomainList != "")
	{
		int nPos = -1;
		while((nPos=m_strDomainList.Find("\n")) >= 0)
		{
			m_comboDomain.AddString(m_strDomainList.Left(nPos));
			m_strDomainList.Delete(0, nPos+1);
		}
		if(m_strDomainList != "")
		{
			m_comboDomain.AddString(m_strDomainList);
		}
	}

	if(m_pInfo)
	{
		if(m_pInfo->strLAddress != "")
		{
			GetDlgItem(IDC_LABEL_ADDRESS)->SetWindowText(m_pInfo->strLAddress);
		}
		if(m_pInfo->strLUserName != "")
		{
			GetDlgItem(IDC_LABEL_USERNAME)->SetWindowText(m_pInfo->strLUserName);
		}
		if(m_pInfo->strLPassword != "")
		{
			GetDlgItem(IDC_LABEL_PASSWORD)->SetWindowText(m_pInfo->strLPassword);
		}
		if(m_pInfo->strLDomain != "")
		{
			GetDlgItem(IDC_LABEL_DOMAIN)->SetWindowText(m_pInfo->strLDomain);
		}

		if(!m_pInfo->bUseAddress)
		{
			GetDlgItem(IDC_LOGIN_ADDRESS)->EnableWindow(FALSE);
		}
		if(!m_pInfo->bUseUserName)
		{
			GetDlgItem(IDC_LOGIN_USERNAME)->EnableWindow(FALSE);
		}
		if(!m_pInfo->bUsePassword)
		{
			GetDlgItem(IDC_LOGIN_PASSWORD)->EnableWindow(FALSE);
		}
		if(!m_pInfo->bUseDomain)
		{
			GetDlgItem(IDC_COMBO_DOMAIN)->EnableWindow(FALSE);
		}

		if(m_pInfo->strFocus == "address")
		{
			GetDlgItem(IDC_LOGIN_ADDRESS)->SetFocus();
		}else
		if(m_pInfo->strFocus == "username")
		{
			GetDlgItem(IDC_LOGIN_USERNAME)->SetFocus();
		}else
		if(m_pInfo->strFocus == "password")
		{
			GetDlgItem(IDC_LOGIN_PASSWORD)->SetFocus();
		}else
		if(m_pInfo->strFocus == "domain")
		{
			GetDlgItem(IDC_COMBO_DOMAIN)->SetFocus();
		}
	}

	UpdateData(FALSE);
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTclLoginDlg::OnOK() 
{
	UpdateData(TRUE);

	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// 设置标题
/////////////////////////////////////////////////////////////////////////////
void CTclLoginDlg::SetTitle(CString strTitle)
{
	m_strTitle = strTitle;
}

/////////////////////////////////////////////////////////////////////////////
// 设置提示信息
/////////////////////////////////////////////////////////////////////////////
void CTclLoginDlg::SetInfo(CString strInfo)
{
	m_strInfo = strInfo;
}

/////////////////////////////////////////////////////////////////////////////
// 设置检查框文字
/////////////////////////////////////////////////////////////////////////////
void CTclLoginDlg::SetCheck(CString strCheck)
{
	m_strCheck = strCheck;
}

/////////////////////////////////////////////////////////////////////////////
// 设置域列表
/////////////////////////////////////////////////////////////////////////////
void CTclLoginDlg::SetDomainboList(CString strDomainList)
{
	m_strDomainList = strDomainList;
}

/////////////////////////////////////////////////////////////////////////////
// 通用登录对话框
/////////////////////////////////////////////////////////////////////////////
BOOL CBlueUIApp::CommLoginDialog(void* p)
{
	TCommLoginInfo* pInfo = (TCommLoginInfo*)p;
	CTclLoginDlg LoginDlg;

	LoginDlg.m_pInfo	= pInfo;

	// 给对话框各个变量赋初值
	if(pInfo->strTitle != "")
	{
		LoginDlg.SetTitle(pInfo->strTitle);
	}
	if(pInfo->strInfo != "")
	{
		LoginDlg.SetInfo(pInfo->strInfo);
	}
	if(pInfo->strAddress != "")
	{
		LoginDlg.m_strAddress = pInfo->strAddress;
	}
	if(pInfo->strUserName != "")
	{
		LoginDlg.m_strUserName = pInfo->strUserName;
	}
	if(pInfo->strPassword != "")
	{
		LoginDlg.m_strPassword = pInfo->strPassword;
	}
	if(pInfo->strDomain != "")
	{
		LoginDlg.m_strDomain = pInfo->strDomain;
	}
	if(pInfo->strDomainList != "")
	{
		LoginDlg.SetDomainboList(pInfo->strDomainList);
	}
	LoginDlg.SetCheck(pInfo->strCheck);

	if(LoginDlg.DoModal() == IDOK)
	{	// 回传变量值
		pInfo->strAddress	= LoginDlg.m_strAddress;
		pInfo->strUserName	= LoginDlg.m_strUserName;
		pInfo->strPassword	= LoginDlg.m_strPassword;
		pInfo->strDomain	= LoginDlg.m_strDomain;
		pInfo->bCheck		= LoginDlg.m_bCheck;
		return 1;
	}else
	{
		return 0;
	}
}
