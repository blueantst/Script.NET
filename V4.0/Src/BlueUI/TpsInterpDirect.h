#if !defined(AFX_TPSINTERPDIRECT_H__E48BB4AA_9EDE_46D0_873A_08C309472BB9__INCLUDED_)
#define AFX_TPSINTERPDIRECT_H__E48BB4AA_9EDE_46D0_873A_08C309472BB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsInterpDirect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTpsInterpDirect dialog

class CTpsInterpDirect : public CDialog
{
// Construction
public:
	CTpsInterpDirect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsInterpDirect)
	enum { IDD = IDD_TPS_INTERP_DIRECT };
	CSpinButtonCtrl	m_SpinDirectOutputID;
	BOOL	m_bDirectOutput;
	BOOL	m_bDirectFile;
	BOOL	m_bDirectResult;
	BOOL	m_bDirectPipe;
	BOOL	m_bDirectMemory;
	BOOL	m_bDirectAdmin;
	int		m_nDirectOutputID;
	//}}AFX_DATA

	virtual void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsInterpDirect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsInterpDirect)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckDirectOutput();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSINTERPDIRECT_H__E48BB4AA_9EDE_46D0_873A_08C309472BB9__INCLUDED_)
