#if !defined(AFX_NEWPRJPAGE_H__B99FAEF7_24EC_40DF_88E5_3E099C2F5D55__INCLUDED_)
#define AFX_NEWPRJPAGE_H__B99FAEF7_24EC_40DF_88E5_3E099C2F5D55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewPrjPage.h : header file
//

#include "kcsidebannerwnd.h"

/////////////////////////////////////////////////////////////////////////////
// CNewPrjPage dialog

class CNewPrjPage : public CDialog
{
	//DECLARE_DYNCREATE(CNewPrjPage)

// Construction
public:
	CNewPrjPage(CWnd* pParent = NULL);
	~CNewPrjPage();

// Dialog Data
	//{{AFX_DATA(CNewPrjPage)
	enum { IDD = IDD_NEW_PROJECT };
	CListBox	m_listPrjType;
	CString	m_strPrjName;
	CString	m_strPrjDesc;
	CString	m_strAuthor;
	CString	m_strPrjPath;
	//}}AFX_DATA

	CKCSideBannerWnd  m_banner;

	int				m_nPrjType;		// 工程类型
	CString			m_strPrjType;	// 工程类型
	CString			m_strPrjVci;	// 工程组件名
	CString			m_strPrjExt;	// 工程组件名
	CString			m_strTemplate;	// 模板文件或目录
	CString			m_strInterp;	// 解释器
	CString			m_strScript;	// 执行的脚本
	CUIntArray		m_auIcon;		// 图标列表
	CStringArray	m_asIcon;		// 图标列表(图标文件名)
	CStringArray	m_asTemplate;	// 模板列表
	CStringArray	m_asType;		// 工程类型列表
	CStringArray	m_asPrjVci;		// 工程VCI组件名列表
	CStringArray	m_asExt;		// 工程文件后缀列表
	CStringArray	m_asInterp;		// 脚本解释器列表
	CStringArray	m_asScript;		// 脚本列表(选定一种工程之后执行的脚本)

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CNewPrjPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CNewPrjPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS);
	afx_msg void OnButtonPrjpath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPRJPAGE_H__B99FAEF7_24EC_40DF_88E5_3E099C2F5D55__INCLUDED_)
