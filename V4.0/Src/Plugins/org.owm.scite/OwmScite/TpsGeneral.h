#if !defined(AFX_TPSGENERAL_H__336B3AEB_B435_41A8_BF47_FDA3067C5181__INCLUDED_)
#define AFX_TPSGENERAL_H__336B3AEB_B435_41A8_BF47_FDA3067C5181__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsGeneral.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTpsGeneral dialog

class CTpsGeneral : public CDialog
{
// Construction
public:
	CTpsGeneral(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsGeneral)
	enum { IDD = IDD_TPS_GENERAL };
	BOOL	m_bLineNumberMarginShow;
	BOOL	m_bCurrentLineHilitingShow;
	BOOL	m_bIndentMaintain;
	BOOL	m_bFileAutoDetection;
	BOOL	m_bEnableTagsMatchHilite;
	BOOL	m_bReplaceTabBySpace;
	BOOL	m_bAutocStatus;
	BOOL	m_bFuncParamsHint;
	int		m_nTabSize;
	BOOL	m_bWhiteSpaceShow;
	BOOL	m_bEolShow;
	BOOL	m_bIndentGuideLineShow;
	BOOL	m_bDoWrap;
	BOOL	m_bWrapSymbolShow;
	int		m_nDefaultZoom;
	BOOL	m_bEnableWordTooltip;
	BOOL	m_bSearchWordAccurate;
	CString	m_strInterpName;
	BOOL	m_bAutoResetInterp;
	BOOL	m_bShowWordHelpInDynamicHelp;
	BOOL	m_bEnableVarTooltip;
	//}}AFX_DATA

	virtual void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsGeneral)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsGeneral)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSGENERAL_H__336B3AEB_B435_41A8_BF47_FDA3067C5181__INCLUDED_)
