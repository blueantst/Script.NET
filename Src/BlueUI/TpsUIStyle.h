#if !defined(AFX_TPSUISTYLE_H__3BFE25AA_54ED_49A4_9D8A_8A5941336E03__INCLUDED_)
#define AFX_TPSUISTYLE_H__3BFE25AA_54ED_49A4_9D8A_8A5941336E03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsUIStyle.h : header file
//

struct THEMEINFO
{
	CString csIniFileName;
	CString csColorSchemes;
	CString csSizes;
	struct THEME* pTheme;
};

struct THEME
{
	CString strResourcePath;
	CArray<THEMEINFO*, THEMEINFO*> m_arrThemes;
};

/////////////////////////////////////////////////////////////////////////////
// CTpsUIStyle dialog

class CTpsUIStyle : public CDialog
{
// Construction
public:
	CTpsUIStyle(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsUIStyle)
	enum { IDD = IDD_TPS_UISTYLE };
	CComboBox	m_cmbHtmlStyle;
	CComboBox	m_cmbColors;
	CComboBox	m_cmbFonts;
	CListBox	m_lboxSkins;
	BOOL	m_bApplyMetrics;
	BOOL	m_bApplyFrame;
	BOOL	m_bApplyColors;
	int		m_nTheme;
	int		m_nThemeColor;
	//}}AFX_DATA

	CStringArray m_asSkinName;
	CStringArray m_asHtmlTemplate;

	void FindThemes(CString strPath, BOOL bRecurse);
	void EnumerateIniThemes(CString strResourcePath);
	void EnumerateThemes(CString strResourcePath);
	void EnumerateThemeColors(CXTPSkinManagerResourceFile* pFile, LPCTSTR lpszResourcePath, LPCTSTR lpszThemeName);
	void ReleaseThemes();

	void SetTheme(int nColor, int nFont);

	void ReloadThemes();
	void InitListThemes();

	void ReloadHtmlStyle();

	virtual void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsUIStyle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsUIStyle)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSelChangeListThemes();
	afx_msg void OnSelChangeComboColors();
	afx_msg void OnSelChangeComboFonts();
	afx_msg void OnCheckApplyMetrics();
	afx_msg void OnCheckApplyFrame();
	afx_msg void OnCheckApplyColors();
	afx_msg void OnThemeChanged();
	afx_msg void OnThemeColorChanged();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSUISTYLE_H__3BFE25AA_54ED_49A4_9D8A_8A5941336E03__INCLUDED_)
