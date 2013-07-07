#if !defined(AFX_TPSVCI_H__DB6A90DB_3F8D_4E17_9526_6FD0CDB13634__INCLUDED_)
#define AFX_TPSVCI_H__DB6A90DB_3F8D_4E17_9526_6FD0CDB13634__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsVci.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTpsVci dialog

class CTpsVci : public CDialog
{
// Construction
public:
	CTpsVci(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsVci)
	enum { IDD = IDD_TPS_VCI };
	BOOL	m_bEnableDebugOut;
	//}}AFX_DATA

	virtual void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsVci)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsVci)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSVCI_H__DB6A90DB_3F8D_4E17_9526_6FD0CDB13634__INCLUDED_)
