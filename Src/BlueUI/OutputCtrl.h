#if !defined(AFX_OUTPUTCTRL_H__8BF933E2_9C05_11D2_AA7D_0000216359D4__INCLUDED_)
#define AFX_OUTPUTCTRL_H__8BF933E2_9C05_11D2_AA7D_0000216359D4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// COutputCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COutputCtrl window
#define MAX_CHAR_FORMAT	5

class COutputCtrl : public CRichEditCtrl
{
	DECLARE_DYNAMIC(COutputCtrl)
// Construction
public:
	COutputCtrl();

// Attributes
public:
	COLORREF	m_Color;
	DWORD		m_Effect;
	LONG		m_Height;
	TCHAR		m_FaceName[LF_FACESIZE]; 	

// Operations
public:
	void SetParentBar(CWnd* pWnd) { m_pParentBar = pWnd; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputCtrl)
	//}}AFX_VIRTUAL
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

// Implementation
public:
	void SetMaxLine(DWORD dwLine=1000);
	void ArrangeLine();
	void ClearLineFrom(INT dlineFrom , INT dlineTo);
	void Clear();
	void vsWrite(LPCTSTR format, va_list apList);
	void WriteDirect(LPCTSTR lpszOutput);
	void SetFormat(int nFmt = 0);
	void SetTextColor(COLORREF color);
	void SetCurFormat();
	void ResetFormat();
	void SetCtrlFormat(LPCTSTR lpszSet);
	void ParseLines(CString strSet, CStringArray& aLines);
	void ParseALine(CString strLine);
	void SaveToFile(CString strFile, int nFormat);

	void OutputFormat(int nColor, LPCTSTR format, ...);
	void Output(LPCTSTR format, ...);

	static DWORD CALLBACK EditStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG FAR *pcb); 
	static DWORD CALLBACK EditStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG FAR *pcb); 

	virtual ~COutputCtrl();

	// Generated message map functions
protected:
	CHARFORMAT	m_fmtCur;		// 当前使用的字体
	DWORD		dwMaxLine;		// 最大行数
	CWnd*		m_pParentBar;	// 记录父Bar窗口
	//{{AFX_MSG(COutputCtrl)
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnOeditCopy();
	afx_msg void OnOeditClear();
	afx_msg void OnOfileSave();
	afx_msg void OnOeditSelectall();
	afx_msg void OnUpdateOeditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOeditClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOfileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOeditSelectall(CCmdUI* pCmdUI);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTCTRL_H__8BF933E2_9C05_11D2_AA7D_0000216359D4__INCLUDED_)
