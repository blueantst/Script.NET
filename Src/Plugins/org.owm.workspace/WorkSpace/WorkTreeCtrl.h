#if !defined(AFX_WORKTREECTRL_H__D8058500_582D_11D4_BEF6_000021605F53__INCLUDED_)
#define AFX_WORKTREECTRL_H__D8058500_582D_11D4_BEF6_000021605F53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorkTreeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWorkTreeCtrl window

// 树节点数据结构
struct HWORKTREEDATA
{
	BYTE		byType;		// 节点类型
	BYTE		byState;	// 节点状态
	int			nID;		// 节点ID
	CMenu*		pMenu;		// 节点菜单
	CObject*	pObject;	// 节点对象
	HTREEITEM	hItem;		// 节点Item
	CString		strInfo;	// 节点信息
	CString		strDClick;	// 节点双击脚本
};

class CWorkTreeCtrl : public CXTTreeCtrl
{
// Construction
public:
	CWorkTreeCtrl();

// Attributes
public:
	CImageList		m_ImageList;	// 节点图标数组
	CPtrArray		m_aMenus;		// 菜单数组
	CMenuItemArray	m_aMenuItems;	// 菜单项数组
	CPtrArray		m_aDatas;		// 节点数据树组
	HTREEITEM		m_hCurTreeItem;	// 当前操作的树节点
	int				m_nImageCount;	// 节点图标数
	int				m_nTreeType;	// 树类型
	int				m_nWsNo;		// 任务列表号
	int				m_nTreeID;		// 树ID;
	int				m_nMenuItemID;	// 菜单项ID
	CString			m_strBmpFile;	// 图片文件名
	CString			m_strDClickScript;// 缺省双击操作时候执行的脚本
	CString			m_strCurDClickScript;// 当前节点双击脚本的暂存变量

// Operations
public:
	BOOL		IsWsInit();				// 任务列表是否初始化
	void		SetWsInit(BOOL bInit);	// 设置任务列表初始化标志

	void		InitTree();			// 初始化树
	void		SetTreeImageList(CString strBmpFile);

	// 树菜单
	int			AddTreeMenu(CMenu* pMenu);	// 添加菜单
	CMenu*		GetTreeMenu(int nIndex);	// 取得某个菜单

	// 菜单项操作
	int			AddMenuItem(BYTE byType, UINT nID, CString strItem, CString strCommand, TYPEOF_Proc* Proc, TYPEOF_UpdateProc* UpdateProc=NULL);
	BOOL		GetMenuItem(BYTE byType, UINT nID, CString& strItem, CString& strCommand, TYPEOF_Proc** pProc, TYPEOF_UpdateProc** pUpdateProc);

	HTREEITEM	AddTreeItem(HTREEITEM hParent, CString strText, 
				int iImage = 0, HTREEITEM hInsAfter = TVI_LAST);
	BOOL		DelTreeItem(HTREEITEM hItem, BOOL bChildOnly);
	BOOL		DelTreeItemData(HTREEITEM hItem);

	// 节点数据操作
	void		SetItemType(HTREEITEM hItem, BYTE byType);	// 设置节点类型
	BYTE		GetItemType(HTREEITEM hItem);	// 取得节点类型
	void		SetItemState1(HTREEITEM hItem, BYTE byState);	// 设置节点状态
	void		TraversalSetItemState(HTREEITEM hItem, BYTE byState, BOOL bFreshAll = TRUE);	// 遍历设置节点状态
	void		UpTraversalSetItemState(HTREEITEM hItem, BYTE byState);	// 向上遍历设置节点状态
	BYTE		GetItemState1(HTREEITEM hItem);	// 取得节点状态
	void		SetItemMenu(HTREEITEM hItem, CMenu* pMenu);	// 设置节点菜单
	CMenu*		GetItemMenu(HTREEITEM hItem);	// 取得节点菜单
	void		SetItemObject(HTREEITEM hItem, CObject* pObject);	// 设置节点对象
	CObject*	GetItemObject(HTREEITEM hItem);	// 取得节点对象
	void		SetItemID(HTREEITEM hItem, int nID);	// 设置节点ID
	int			GetItemID(HTREEITEM hItem);		// 取得节点ID
	void		SetItemImage1(HTREEITEM hItem, int nImage);	// 设置节点图标
	int			GetItemImage1(HTREEITEM hItem);		// 取得节点图标
	void		SetItemInfo(HTREEITEM hItem, CString strInfo);	// 设置节点信息
	CString		GetItemInfo(HTREEITEM hItem);	// 获取节点信息
	void		SetItemDClick(HTREEITEM hItem, CString strDClick);	// 设置节点双击脚本
	CString		GetItemDClick(HTREEITEM hItem);	// 取得节点双击脚本

	// 任务列表操作
	int			GetWsID();						// 获取任务列表ID

	// 查询操作
	HTREEITEM	GetItemFromID(int nID);
	HTREEITEM	GetItemFromInfo(CString strInfo);

	// 测试允许操作
	BOOL		EnableTestInit();		// 初始化允许
	BOOL		EnableTestStart();
	BOOL		EnableTestStop();
	BOOL		EnableTestPause();
	BOOL		EnableTestContinue();
	BOOL		EnableTestDisable();
	BOOL		EnableTestFresh();
	BOOL		EnableTestProperty();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWorkTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWorkTreeCtrl)
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTestInit();
	afx_msg void OnTestStart();
	afx_msg void OnTestStop();
	afx_msg void OnTestPause();
	afx_msg void OnTestContinue();
	afx_msg void OnTestDisable();
	afx_msg void OnTestFresh();
	afx_msg void OnTestProperty();
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRsmProperty();
	afx_msg void OnUpdateRsmProperty(CCmdUI* pCmdUI);
	afx_msg void OnRsmRefresh();
	afx_msg void OnUpdateRsmRefresh(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnWorkTreeCmd(UINT nID);
	afx_msg void OnUpdateWorkTreeCmd(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKTREECTRL_H__D8058500_582D_11D4_BEF6_000021605F53__INCLUDED_)
