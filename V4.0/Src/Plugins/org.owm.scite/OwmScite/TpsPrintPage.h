#if !defined(AFX_TPSPRINTPAGE_H__24FC1A19_4BA3_4C7E_B8F2_A5DA594FC959__INCLUDED_)
#define AFX_TPSPRINTPAGE_H__24FC1A19_4BA3_4C7E_B8F2_A5DA594FC959__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsPrintPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTpsPrintPage dialog

class CTpsPrintPage : public CDialog
{
// Construction
public:
	CTpsPrintPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsPrintPage)
	enum { IDD = IDD_TPS_PRINTPAGE };
	CXTFlatComboBox	m_comboColorMode;
	CXTFlatComboBox	m_comboSize;
	CXTFontCombo	m_comboFont;
	CString	m_sFont;
	CString	m_sSize;
	CString	m_strPageHeader;
	CString	m_strPageFooter;
	int		m_nMarginLeft;
	int		m_nMarginRight;
	int		m_nMarginTop;
	int		m_nMarginBottom;
	BOOL	m_bPrintLineNumber;
	BOOL	m_bBold;
	BOOL	m_bItalic;
	int		m_nColorMode;
	//}}AFX_DATA

	virtual void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsPrintPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsPrintPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSPRINTPAGE_H__24FC1A19_4BA3_4C7E_B8F2_A5DA594FC959__INCLUDED_)
