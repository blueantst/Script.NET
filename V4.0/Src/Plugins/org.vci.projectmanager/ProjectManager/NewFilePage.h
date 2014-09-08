#if !defined(AFX_NEWFILEPAGE_H__4E7E8A47_A92A_44A8_8685_326582C85FDF__INCLUDED_)
#define AFX_NEWFILEPAGE_H__4E7E8A47_A92A_44A8_8685_326582C85FDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewFilePage.h : header file
//

#include "kcsidebannerwnd.h"

/////////////////////////////////////////////////////////////////////////////
// CNewFilePage dialog

class CNewFilePage : public CDialog
{
	//DECLARE_DYNCREATE(CNewFilePage)

// Construction
public:
	CNewFilePage(CWnd* pParent = NULL);
	~CNewFilePage();

// Dialog Data
	//{{AFX_DATA(CNewFilePage)
	enum { IDD = IDD_NEW_FILE };
	CListBox	m_listFileType;
	BOOL	m_bAddToProject;
	CString	m_strFileName;
	CString	m_strFilePath;
	CString	m_strAuthor;
	CString	m_strFileDesc;
	//}}AFX_DATA

	CKCSideBannerWnd  m_banner;

	int				m_nFileType;	// 文件类型
	CString			m_strFileType;	// 文件类型
	CString			m_strTemplate;	// 模板文件或目录
	CString			m_strInterp;	// 解释器
	CString			m_strScript;	// 执行的脚本
	CUIntArray		m_auIcon;		// 图标列表
	CStringArray	m_asIcon;		// 图标列表(图标文件名)
	CStringArray	m_asTemplate;	// 模板列表
	CStringArray	m_asType;		// 文件类型列表
	CStringArray	m_asInterp;		// 脚本解释器列表
	CStringArray	m_asScript;		// 脚本列表(选定一种工程之后执行的脚本)
	CStringArray	m_asExt;		// 扩展名列表

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CNewFilePage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CNewFilePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS);
	afx_msg void OnButtonFilepath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWFILEPAGE_H__4E7E8A47_A92A_44A8_8685_326582C85FDF__INCLUDED_)
