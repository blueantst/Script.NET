#if !defined(AFX_TPSFONT_H__6E9081C8_C217_4A3F_8317_89DFBB603460__INCLUDED_)
#define AFX_TPSFONT_H__6E9081C8_C217_4A3F_8317_89DFBB603460__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsFont.h : header file
//
#include "label.h"
/*
/////////////////////////////////////////////////////////////////////////////
// CLabel window
enum FlashType {None, Text, Background };

class CLabel : public CStatic
{
// Construction
public:
	CLabel();
	void SetFont( CFont* pFont, BOOL bRedraw = TRUE );
	CLabel& SetBkColor(COLORREF crBkgnd);
	CLabel& SetTextColor(COLORREF crText);
	CLabel& SetText(const CString& strText);
	CLabel& SetFontBold(BOOL bBold);
	CLabel& SetFontName(const CString& strFont);
	CLabel& SetFontUnderline(BOOL bSet);
	CLabel& SetFontStrikeOut(BOOL bSet);
	CLabel& SetFontItalic(BOOL bSet);
	CLabel& SetFontSize(int nSize);
	CLabel& SetSunken(BOOL bSet);
	CLabel& SetBorder(BOOL bSet);
	CLabel& FlashText(BOOL bActivate);
	CLabel& FlashBackground(BOOL bActivate);
	CLabel& SetLink(BOOL bLink);
	CLabel& SetLinkCursor(HCURSOR hCursor);

// Attributes
public:
protected:
	void ReconstructFont();
	COLORREF	m_crText;
	HBRUSH		m_hBrush;
	HBRUSH		m_hwndBrush;
	LOGFONT		m_lf;
	CFont		m_font;
	CString		m_strText;
	BOOL		m_bState;
	BOOL		m_bTimer;
	BOOL		m_bLink;
	FlashType	m_Type;
	HCURSOR		m_hCursor;
			// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLabel)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLabel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLabel)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
*/

/*
#define FON_FONT		0x00000001	// 字体
#define FON_SIZE		0x00000002	// 大小
#define FON_FG			0x00000004	// 前景色
#define FON_BK			0x00000008	// 背景色
#define FON_TYPE		0x00000010	// 字形

struct FontItem
{
	DWORD	dwMask;			// 掩码
	CString strName;		// 配置项名
	CString strSubKey;		// 子键名
	CString strFont;		// 字体
	int		nSize;			// 大小
	COLORREF colorFg;		// 前景色
	COLORREF colorBk;		// 背景色
	BOOL	bBold;			// 粗体
	BOOL	bItalic;		// 斜体
	BOOL	bUnderLine;		// 下划线
	BOOL	bStrikeOut;		// 删除线

	FontItem() {;};
	FontItem(DWORD _dwMask, CString _strName, CString _strSubKey, CString _strFont,
			int _nSize, COLORREF _colorFg, COLORREF _colorBk, BOOL _bBold,
			BOOL _bItalic, BOOL _bUnderLine, BOOL _bStrikeOut)
	{
		dwMask		= _dwMask;
		strName		= _strName;
		strSubKey	= _strSubKey;
		strFont		= _strFont;
		nSize		= _nSize;
		colorFg		= _colorFg;
		colorBk		= _colorBk;
		bBold		= _bBold;
		bItalic		= _bItalic;
		bUnderLine	= _bUnderLine;
		bStrikeOut	= _bStrikeOut;
	};

	FontItem& operator = (const FontItem& other)
	{
		dwMask		= other.dwMask;
		strName		= other.strName;
		strSubKey	= other.strSubKey;
		strFont		= other.strFont;
		nSize		= other.nSize;
		colorFg		= other.colorFg;
		colorBk		= other.colorBk;
		bBold		= other.bBold;
		bItalic		= other.bItalic;
		bUnderLine	= other.bUnderLine;
		bStrikeOut	= other.bStrikeOut;
		return *this;
	}
};
*/
/////////////////////////////////////////////////////////////////////////////
// CTpsFont dialog
#define MAX_FONT_ITEMS	50

class CTpsFont : public CDialog
{
// Construction
public:
	CTpsFont(CWnd* pParent = NULL);   // standard constructor

	void LoadFontItems();
	void SaveFontItems();
	FontItem* GetCurItem();

// Dialog Data
	//{{AFX_DATA(CTpsFont)
	enum { IDD = IDD_TPS_FONT };
	CXTButton	m_btnReset;
	CXTListBox	m_ListItem;
	CXTColorPicker	m_ColorfgPicker;
	CXTColorPicker	m_ColorbkPicker;
	CLabel			m_staticSample;
	CXTFlatComboBox	m_comboSize;
	CXTFontCombo	m_comboFont;
	BOOL	m_bBold;
	BOOL	m_bItalic;
	BOOL	m_bStrikeOut;
	BOOL	m_bUnderLine;
	CString	m_sFont;
	CString	m_sSize;
	//}}AFX_DATA
	COLORREF	m_clrFg;
	COLORREF	m_clrBk;

	virtual void OnOK();
	void GetCurrentFont(LPLOGFONT lplf);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsFont)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsFont)
	afx_msg void OnSelchange();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListItem();
	afx_msg void OnBtnFontreset();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnSelendokFontCombo();
	//}}AFX_MSG
	afx_msg void OnFgColorChange();
	afx_msg void OnBkColorChange();
	DECLARE_MESSAGE_MAP()

private:
	int m_nFontItemCount;
	struct FontItem m_FontItems[MAX_FONT_ITEMS];
	CFont m_fontSample;
	int m_cyPixelsPerInch;
//	static int CALLBACK FontEnumProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, 
//		int FontType, CTpsFont* pTpsFont );
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSFONT_H__6E9081C8_C217_4A3F_8317_89DFBB603460__INCLUDED_)
