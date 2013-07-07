#if !defined(AFX_TPSSTYLE_H__8B27CC30_B17E_4F88_A5F3_9FA3409D4BB6__INCLUDED_)
#define AFX_TPSSTYLE_H__8B27CC30_B17E_4F88_A5F3_9FA3409D4BB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsStyle.h : header file
//
#include "label.h"
/////////////////////////////////////////////////////////////////////////////
// CTpsStyle dialog

class CTpsStyle : public CDialog
{
// Construction
public:
	CTpsStyle(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsStyle)
	enum { IDD = IDD_TPS_STYLE };
	CLabel	m_staticSample;
	CXTListBox	m_ListItem;
	CXTColorPicker	m_ColorbkPicker;
	CXTColorPicker	m_ColorfgPicker;
	CXTFlatComboBox	m_comboSize;
	CXTFontCombo	m_comboFont;
	CXTFlatComboBox	m_comboLanguage;
	CXTFlatComboBox	m_comboTheme;
	CString	m_sFont;
	CString	m_sSize;
	BOOL	m_bBold;
	BOOL	m_bItalic;
	BOOL	m_bUnderLine;
	BOOL	m_bUseGlobalFg;
	BOOL	m_bUseGlobalBg;
	BOOL	m_bUseGlobalFont;
	//}}AFX_DATA
	COLORREF	m_clrFg;
	COLORREF	m_clrBk;

	int _currentLexerIndex;

	LexerStylerArray _lsArray;
    StyleArray _globalStyles;

	LexerStylerArray _styles2restored;
	StyleArray _gstyles2restored;
	GlobalOverride _gOverride2restored;

	bool _isDirty;
    bool _isSync;
	bool _isShownGOCtrls;

	virtual void OnOK();
	virtual void OnCancel();

	void prepare2Cancel();
	void apply();
	Style & getCurrentStyler();
	Style & getGlobalStyler(int nIndex);

	void setVisualFromStyleList();
	void setStyleListFromLexer(int index);

	void updateGlobalOverrideCtrls();
	void showGlobalOverrideCtrls(bool show);
	void restoreGlobalOverrideValues();

	CFont m_fontSample;
	int m_cyPixelsPerInch;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsStyle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsStyle)
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS);
	afx_msg void OnSelendokFontCombo();
	afx_msg void OnSelchangeListItem();
	afx_msg void OnSelchangeComboLanguage();
	afx_msg void OnBtnStylereset();
	afx_msg void OnCheckBold();
	afx_msg void OnCheckItalic();
	afx_msg void OnCheckUnderline();
	afx_msg void OnSelchangeComboSize();
	afx_msg void OnCheckGlobalFg();
	afx_msg void OnCheckGlobalBg();
	afx_msg void OnCheckGlobalFont();
	afx_msg void OnChangeUserKeywordsEdit();
	//}}AFX_MSG
	afx_msg void OnFgColorChange();
	afx_msg void OnBkColorChange();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSSTYLE_H__8B27CC30_B17E_4F88_A5F3_9FA3409D4BB6__INCLUDED_)
