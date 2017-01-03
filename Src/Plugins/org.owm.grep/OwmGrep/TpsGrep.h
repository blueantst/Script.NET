#if !defined(AFX_TPSGREP_H__C6FC95A7_8E70_408A_B353_D695FCFFA29B__INCLUDED_)
#define AFX_TPSGREP_H__C6FC95A7_8E70_408A_B353_D695FCFFA29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsGrep.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTpsGrep dialog

class CTpsGrep : public CDialog
{
// Construction
public:
	CTpsGrep(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsGrep)
	enum { IDD = IDD_TPS_GREP };
	BOOL	m_bShowReplaceLine;
	//}}AFX_DATA

	virtual void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsGrep)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsGrep)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSGREP_H__C6FC95A7_8E70_408A_B353_D695FCFFA29B__INCLUDED_)
