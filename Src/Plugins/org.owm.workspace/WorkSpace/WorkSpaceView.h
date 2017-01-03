#ifndef __WORKSPACEVIEW_H__
#define __WORKSPACEVIEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorkSpaceView.h : header file
//

#include "WorkTreeCtrl.h"
#include "WorkTaskPanel.h"

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceView view
class CWorkSpaceDoc;
class CWorkSpaceView : public CView
{
	DECLARE_DYNCREATE(CWorkSpaceView)

// Attributes
public:
	CWorkTreeCtrl		m_TreeCtrl;		// 树控件
	CWorkTaskPanel		m_TaskPanel;	// 任务Panel
	TYPEOF_ViewChange*	m_pViewChange;	// 页面改变回调函数
	int					m_nWsNo;		// 任务列表号
	BOOL				m_bTreeInited;	// 树控件是否已经初始化
	int					m_nPageType;	// 页面类型

	CWorkSpaceView();
	virtual ~CWorkSpaceView();

	CWorkSpaceDoc* GetDocument();

// Operations
public:
	// WorkTree
	BOOL SetTreeType(int nTreeType);
	void InitTreeCtrl();
	void InitTaskPanel();
	void SetTreeImageList(CString strBmpFile);
	void InstallVCCallBack(TYPEOF_ViewChange* pViewChange);
	int  AddTreeMenu(CMenu* pMenu);
	int  AddMenuItem(HWORKMENUITEM* pItem);
	HTREEITEM AddTreeItem(HWORKTREEITEM* pItem);
	BOOL DelTreeItem(HWORKTREEITEM* pItem);
	void SetDClickScript(CString strScript);

	// TaskPanel
	CXTPTaskPanelGroupItem* AddTaskItem(HWORKTASKITEM* pItem);
	void DelTaskItem(HWORKTASKITEM* pItem);
	int GetTaskItemIDByItem(CXTPTaskPanelItem* pTaskItem);
	int GetTaskItemIDByCaption(CString strCaption, int nType);
	CXTPTaskPanelGroupItem* GetTaskItemByID(int nID);
	void CreateTaskMenu(CMenu* pMenu);
	void SetTaskPanelTheme(XTPTaskPanelPaintTheme taskPanelTheme);
	int SetTaskPanelIcon(CString strIcon);

	// 节点数据操作
	void		SetItemType(HWORKTREEITEM* pItem);	// 设置节点类型
	BYTE		GetItemType(HWORKTREEITEM* pItem);	// 取得节点类型
	void		SetItemState1(HWORKTREEITEM* pItem);	// 设置节点状态
	BYTE		GetItemState1(HWORKTREEITEM* pItem);	// 取得节点状态
	void		SetItemMenu(HWORKTREEITEM* pItem);	// 设置节点菜单
	CMenu*		GetItemMenu(HWORKTREEITEM* pItem);	// 取得节点菜单
	void		SetItemObject(HWORKTREEITEM* pItem);// 设置节点对象
	CObject*	GetItemObject(HWORKTREEITEM* pItem);// 取得节点对象
	void		SetItemID(HWORKTREEITEM* pItem);	// 设置节点ID
	int			GetItemID(HWORKTREEITEM* pItem);	// 取得节点ID
	void		SetItemName(HWORKTREEITEM* pItem);	// 设置节点名
	CString		GetItemName(HWORKTREEITEM* pItem);	// 取得节点名
	void		SetItemImage(HWORKTREEITEM* pItem);	// 设置节点图标
	int			GetItemImage(HWORKTREEITEM* pItem);	// 取得节点图标
	void		ExpandItem(HWORKTREEITEM* pItem);	// 扩展节点
	void		CollapseItem(HWORKTREEITEM* pItem);	// 收缩节点
	void		EnsureItem(HWORKTREEITEM* pItem);	// 转到某个项目
	BOOL		SetItemInfo(HWORKTREEITEM* pItem);	// 设置树节点信息
	BOOL		GetItemInfo(HWORKTREEITEM* pItem);	// 获取树节点信息
	HTREEITEM	SearchItem(HWORKTREEITEM* pItem);	// 查找节点
	HTREEITEM	GetChildItem(HWORKTREEITEM* pItem);	// 获取第一个子节点
	HTREEITEM	GetNextItem(HWORKTREEITEM* pItem);	// 获取下一个节点
	BOOL		SortChildren(HWORKTREEITEM* pItem);	// 对子节点排序

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkSpaceView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CWorkSpaceView)
//	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in WorkSpaceView.cpp
inline CWorkSpaceDoc* CWorkSpaceView::GetDocument()
   { return (CWorkSpaceDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__WORKSPACEVIEW_H__)
