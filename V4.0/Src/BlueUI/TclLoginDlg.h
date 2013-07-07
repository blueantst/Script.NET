#if !defined(AFX_TCLLOGINDLG_H__13EC914D_5D82_40C9_8029_D9EC299EEDFD__INCLUDED_)
#define AFX_TCLLOGINDLG_H__13EC914D_5D82_40C9_8029_D9EC299EEDFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TclLoginDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTclLoginDlg dialog

class CTclLoginDlg : public CDialog
{
// Construction
public:
	CTclLoginDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTclLoginDlg)
	enum { IDD = IDD_TCL_LOGIN };
	CComboBox	m_comboDomain;
	CString	m_strAddress;
	CString	m_strUserName;
	CString	m_strPassword;
	CString	m_strDomain;
	BOOL	m_bCheck;
	//}}AFX_DATA

	TCommLoginInfo*	m_pInfo;// 登录信息指针

	CString m_strTitle;		// 对话框标题
	CString m_strInfo;		// 提示信息
	CString m_strCheck;		// 检查框文字
	CString m_strDomainList;// 登录域列表

	void SetTitle(CString strTitle);
	void SetInfo(CString strInfo);
	void SetCheck(CString strCheck);
	void SetDomainboList(CString strDomainList);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTclLoginDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTclLoginDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCLLOGINDLG_H__13EC914D_5D82_40C9_8029_D9EC299EEDFD__INCLUDED_)
