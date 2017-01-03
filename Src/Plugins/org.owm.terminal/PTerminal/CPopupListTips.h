#if !defined(AFX_CPOPUPLISTTIPS_H__A7F18663_FE00_11D3_8624_955EDCC5764F__INCLUDED_)
#define AFX_CPOPUPLISTTIPS_H__A7F18663_FE00_11D3_8624_955EDCC5764F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPopupListTips.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPopupListTips window

class CPopupListTips : public CWnd
{
// Construction
public:
	CPopupListTips();
	CPopupListTips(CWnd* pParent);

	// Get NONCLIENTMETRICS info: ctor calls SystemParametersInfo.
	class CNonClientMetrics : public NONCLIENTMETRICS 
	{
	public:
		CNonClientMetrics() 
		{
			cbSize = sizeof(NONCLIENTMETRICS);
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, this, 0);
		}
	};

	CSize m_szMargins;		// extra space around text: change if you like
	enum
	{
		JUSTIFYLEFT  = 0, 
		JUSTIFYRIGHT = 1
	};
	BOOL Create(CPoint pt, CWnd* pParentWnd, UINT nStyle = 0, UINT nID = 0);

// Attributes
public:

protected:
   LONG        m_lScrollYPos;
   static BOOL s_bVScrollFirstTime;

// Operations
public:

	BOOL SetText(const CString& strText, BOOL bSingleLine = TRUE);
    void DoScrollTip(UINT nLineNumber);
    void EndScrollTip();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopupListTips)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPopupListTips();

	// Generated message map functions
protected:
	CFont	m_font;				// font to use (same as tooltips)
	UINT	m_nStyle;			// style (see enum above)
	CString m_strText;
	BOOL    m_bSingleLine;      // TRUE to display text on single line

	CRgn	m_rgn;

	//{{AFX_MSG(CPopupListTips)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPOPUPLISTTIPS_H__A7F18663_FE00_11D3_8624_955EDCC5764F__INCLUDED_)
