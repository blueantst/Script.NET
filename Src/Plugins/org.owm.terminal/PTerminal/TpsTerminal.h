#if !defined(AFX_TPSTERMINAL_H__1BAF6C12_D186_4E79_AF63_546116E193B7__INCLUDED_)
#define AFX_TPSTERMINAL_H__1BAF6C12_D186_4E79_AF63_546116E193B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsTerminal.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTpsTerminal dialog

class CTpsTerminal : public CDialog
{
// Construction
public:
	CTpsTerminal(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsTerminal)
	enum { IDD = IDD_TPS_PTERMINAL };
	int		m_nDefaultShell;
	CString	m_strTclInterpName;
	BOOL	m_bEnableListDlg;
	//}}AFX_DATA

	virtual void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsTerminal)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsTerminal)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSTERMINAL_H__1BAF6C12_D186_4E79_AF63_546116E193B7__INCLUDED_)
