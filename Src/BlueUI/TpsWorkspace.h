#if !defined(AFX_TPSWORKSPACE_H__31DF416E_CDB1_46CD_A58C_4107ABFC2851__INCLUDED_)
#define AFX_TPSWORKSPACE_H__31DF416E_CDB1_46CD_A58C_4107ABFC2851__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsWorkspace.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTpsWorkspace dialog

class CTpsWorkspace : public CDialog
{
// Construction
public:
	CTpsWorkspace(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsWorkspace)
	enum { IDD = IDD_TPS_WORKSPACE };
	BOOL	m_bAutoReloadDocument;
	BOOL	m_bAutoOpenStartPage;
	BOOL	m_bAddShellContextMenu;
	//}}AFX_DATA

	virtual void OnOK();

	BOOL	m_bShellContextMenuExist;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsWorkspace)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsWorkspace)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSWORKSPACE_H__31DF416E_CDB1_46CD_A58C_4107ABFC2851__INCLUDED_)
