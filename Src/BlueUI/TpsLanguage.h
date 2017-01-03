#if !defined(AFX_TPSLANGUAGE_H__3BE992AC_1515_46F8_A292_7A0C8BABDD56__INCLUDED_)
#define AFX_TPSLANGUAGE_H__3BE992AC_1515_46F8_A292_7A0C8BABDD56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsLanguage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTpsLanguage dialog

class CTpsLanguage : public CDialog
{
// Construction
public:
	CTpsLanguage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsLanguage)
	enum { IDD = IDD_TPS_LANGUAGE };
	CComboBox	m_comboLanguage;
	//}}AFX_DATA

	int m_nCurrentLanguageIndex;

	virtual void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsLanguage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsLanguage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSLANGUAGE_H__3BE992AC_1515_46F8_A292_7A0C8BABDD56__INCLUDED_)
