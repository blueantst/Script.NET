// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__0DE945FF_07E5_45F6_A1CD_A8ED9B283443__INCLUDED_)
#define AFX_MAINFRM_H__0DE945FF_07E5_45F6_A1CD_A8ED9B283443__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DockPaneWnd.h"
#include "OutputCtrl.h"
#include "TreeOutCtrl.h"
#include "ReportOutCtrl.h"
#include "PanePropertyGrid.h"

CXTPPropertyGrid& GetPropertyGrid(CWnd* pFrame = 0);

class CMainFrame : public CXTPMDIFrameWnd, CXTPOffice2007FrameHook
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

	void LoadSkin();

	void OnSkinChanged();
	void RedrawFrame(CWnd* pWnd);
	int  GetThemeString(CString strTheme);
	void SetTheme(int nTheme, int nThemeColor);

// Attributes
public:
	int							m_nSkinType;		// 当前皮肤类型(SKIN_OFFICE2007/SKIN_CJINI/SKIN_CJSTYLE/SKIN_MSSTYLE)
	int							m_nTheme;			// 当前主题
	int							m_nThemeColor;		// 当前主题颜色
	CXTWindowPos				m_wndPosition;		// 窗口位置
	COutputCtrlArray			m_aOutputCtrl;		// 输出窗口信息数组
	CPanePropertyGrid			m_panePropertyGrid;	// 属性窗口
	CXTPPropertyGridUpdateContext m_stateExpanding;
	BOOL						m_bPropInUpdate;	// 属性窗口正在更新标志
	BOOL						m_bEnableUpdatePropertyGrid;	// 是否可以更新属性树
	CExtMenuInfoArray			m_arExtMenuInfo;	// 保存扩展菜单信息
	CXTPControlComboBox*		m_pComboBoxURL;

// Operations
public:
	void ResetCommandBars();

	CXTPDockingPaneManager* GetPaneManager(){return &m_paneManager;}

	BOOL ImageManagerSetIcon(int nID, LPCTSTR lpszIconFile);
	BOOL ImageManagerSetIcons(CUIntArray* paIDs, LPCTSTR lpszBmpFile);
	BOOL LoadImageStyleIcons(CString strImagePath);

	// 平台状态条Pane操作
	BOOL AddPane(UINT nPaneID);
	BOOL ClosePaneItem(UINT nPane);
	void CloseAllSelfPane();
	BOOL SetPaneText(UINT nPaneID, LPCTSTR lpszText);
	void SetPaneControl(UINT nPaneID, CWnd* pWnd);
	BOOL SetPaneWidth(UINT nPaneID, int nWidth);
	BOOL SetPaneImageItem(UINT nPaneID, int nItem);
	void AttachPaneProgress(UINT nPaneID, CString strMsg, int nMaxValue=100);
	void ShowPaneProgress(UINT nPaneID, BOOL bShow);
	void SetPaneProgressPos(UINT nPaneID, int nPos);
	BOOL SetPaneButton(UINT nPaneID, CExtMenuInfo* pExtMenuInfo = NULL);
	BOOL SetPaneTooltip(UINT nPaneID, LPCTSTR lpszTooltip);

	// 平台OWM操作事件
	LRESULT OnPlatComCmd(WPARAM, LPARAM);
	LRESULT OnPlatOWMDynClose(WPARAM, LPARAM);

	// 跨进程通知消息
	LRESULT OnCheckItsMe(WPARAM wParam, LPARAM lParam);

	// 输出窗口操作
	COutputWndCtrl* GetOutputCtrl(LPCTSTR lpszTitle);
	COutputWndCtrl* GetOutputCtrl(int nID);
	void DeleteOutputCtrl(int nID);
	COutputWndCtrl* CreateOutputWnd(CString strTitle, int nType = OUTPUT_RICH, int nId = -1, int nAttachPane = 0);

	// DockingPane操作
	CXTPDockingPane* CreateDockingBar(int nID, int nDirection, HICON hIcon, int nAttachPane = 0);
	CWnd* GetDockingPaneWnd(int nPaneID);
	int GetDockingPaneIdByName(CString strName);
	BOOL ActiveDockingPane(int nPaneID, BOOL bShow = TRUE);
	BOOL CloseDockingPane(int nPaneID);

	// 属性树操作
	void UpdatePropertyGridContent();
	void BeginUpdatePropertyGridContent();
	void EndUpdatePropertyGridContent();

	// 扩展菜单操作
	CExtMenuInfo* GetExtMenuInfo(UINT nID);

	// Overrode CWnd implementation to restore saved window position.
	BOOL ShowWindowEx(int nCmdShow);

	void SetDocumentIcon(CString strIconFile, HWND hWnd);
	void SetDocumentTitle(CString strTitle, int nModifyType, HWND hWnd);

	// Session操作
	void LoadSessionFile(CString strSessionFile);
	void SaveSessionFile(CString strSessionFile);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CXTPStatusBar  m_wndStatusBar;
	CXTPDockingPaneManager m_paneManager;
	CXTPTabClientWnd m_MTIClientWnd;

	CString m_strIniFileName;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose(); 
	afx_msg void OnDestroy(); 
	afx_msg void OnViewOutput();
	afx_msg void OnViewProperties();
	afx_msg void OnFileNewBlank();
	afx_msg void OnRefreshGotoUrl();
	afx_msg void OnUpdateRefreshGotoUrl(CCmdUI* pCmdUI);
	afx_msg void OnWindowCloseAll();
	afx_msg void OnUpdateWindowCloseAll(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnCustomize();
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPopUpNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGridNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTabbarMouseMsg(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnTaskPanelNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnOwmExCmd(UINT nID);
	afx_msg void OnUpdateOwmExCmd(CCmdUI* pCmdUI);

	afx_msg void OnGotoUrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateGotoUrl(CCmdUI *pCmdUI);

	afx_msg void OnToggleDock(void);

	afx_msg void OnIndicatorButton(UINT nID);
	afx_msg void OnUpdateIndicatorButton(CCmdUI* pCmdUI);

	afx_msg void OnToolClick(UINT nID);

	afx_msg void OnExportClick(UINT nID);

	DECLARE_MESSAGE_MAP()
};

inline CXTPPropertyGrid& GetPropertyGrid(CWnd* pFrame) {
	if (pFrame == 0) pFrame = AfxGetMainWnd();
	return ((CMainFrame*)pFrame)->m_panePropertyGrid.m_wndPropertyGrid;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__0DE945FF_07E5_45F6_A1CD_A8ED9B283443__INCLUDED_)
