#if !defined(AFX_TREEOUTCTRL_H__91998913_22CF_479B_92F3_04C24269D1BE__INCLUDED_)
#define AFX_TREEOUTCTRL_H__91998913_22CF_479B_92F3_04C24269D1BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeOutCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReportOutCtrl window

class CReportOutCtrl : public CXTPReportControl
{
// Construction
public:
	CReportOutCtrl();

// Attributes
public:
	CString m_strSeparator;	// 项目分割符
	CString m_strScriptClose;	// 窗口关闭时执行的脚本
	CStringArray m_asItemScript; // 项目脚本数组
	CStringArray m_asExMenu;	// 扩展菜单
	CStringArray m_asExMenuScript;	// 扩展菜单脚本
	CArray<COLORREF, COLORREF&>	m_aColorTable;	// 颜色表
	BOOL		m_bSetColor;
	COLORREF	m_clText;
	COLORREF	m_clBkGnd;
	CXTPReportRecord* m_pLastRecord;	// 最后一次添加的记录

	CImageList m_ilIcons;		// 图标

// Operations
public:

	void AppendColor(COLORREF color);
	void SetColor(int nIndex, COLORREF color);
	COLORREF GetColor(int nIndex);

	void Clear();
	void DoCloseScript();
	XTPReportGridStyle  ParseStyle(CString strStyle);
	void ParseLines(CString strSet, CStringArray& aLines);
	void ParseALine(CString strLine);
	void SetCtrlFormat(CString strSet);
	void Output(CString strOutput);
	void vsWrite(LPCTSTR lpszFormat, va_list argList);
	CString GetInfo(CString strParam);
	CXTPReportRecord* GetItemRecord(CString strItem);
	void AddTreeItems(CTTreeItemArray*& pItems);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportOutCtrl)
	public:
	//}}AFX_VIRTUAL
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

// Implementation
public:
	virtual ~CReportOutCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CReportOutCtrl)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnOeditClear();
	afx_msg void OnOutctrlDelrow();
	afx_msg void OnOutctrlScript();
	afx_msg void OnUpdateOutctrlScript(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOeditClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOutctrlDelrow(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEOUTCTRL_H__91998913_22CF_479B_92F3_04C24269D1BE__INCLUDED_)
