#if !defined(AFX_DLGCOMPARAM_H__91077744_AE3B_4659_8015_CBA607385EB6__INCLUDED_)
#define AFX_DLGCOMPARAM_H__91077744_AE3B_4659_8015_CBA607385EB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgComParam.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgComParam dialog

class CDlgComParam : public CDialog
{
// Construction
public:
	CDlgComParam(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgComParam)
	enum { IDD = IDD_DIALOG_COMPARAM };
	CString	m_strComPort;
	CString	m_strBaud;
	int		m_nDataBits;
	int		m_nStopBits;
	int		m_nParity;
	BOOL	m_bEcho;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgComParam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgComParam)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCOMPARAM_H__91077744_AE3B_4659_8015_CBA607385EB6__INCLUDED_)
