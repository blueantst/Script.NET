#if !defined(AFX_WORKTASKPANEL_H__489D691F_E9A5_41E9_9A60_0744DD249E69__INCLUDED_)
#define AFX_WORKTASKPANEL_H__489D691F_E9A5_41E9_9A60_0744DD249E69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorkTaskPanel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWorkTaskPanel window

class CWorkTaskPanel : public CXTPTaskPanel
{
	class CTaskPanelCmdUI;
// Construction
public:
	CWorkTaskPanel();

	void CreateFromMenu(CMenu* pMenu);

// Attributes
public:
	BOOL					m_bUIMode;
	int						m_nTaskPanelItemID;	// TaskPanel Item ID
	int						m_nTaskPanelImageID;// TaskPanel Image ID
	CTaskPanelItemArray		m_aTaskPanelItem;
	CStringArray			m_asIcons;

// Operations
public:
	virtual void OnClick(CXTPTaskPanelGroupItem* pItem);

	void SetTaskPanelTheme(XTPTaskPanelPaintTheme taskPanelTheme);
	int SetIcons(CString strIcons);
	int SetIcon(CString strIcon);

	int GetTaskItemIDByItem(CXTPTaskPanelItem* pTaskItem);
	int GetTaskItemIDByCaption(CString strCaption, int nType);
	CXTPTaskPanelGroupItem* GetTaskItemByID(int nID);
	CXTPTaskPanelGroupItem* AddTaskItem(HWORKTASKITEM* pItem);
	void DelTaskItem(HWORKTASKITEM* pItem);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkTaskPanel)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWorkTaskPanel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWorkTaskPanel)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM);
	void OnUpdateCmdUI(CWnd* pTarget, BOOL bDisableIfNoHndler);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKTASKPANEL_H__489D691F_E9A5_41E9_9A60_0744DD249E69__INCLUDED_)
