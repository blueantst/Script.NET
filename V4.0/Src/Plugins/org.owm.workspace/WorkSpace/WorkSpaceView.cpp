// WorkSpaceView.cpp : implementation file
//

#include "stdafx.h"
#include "WorkSpace.h"
#include "WorkSpaceView.h"
#include "WorkSpaceDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// 执行脚本命令
// <<instance-name>><interp-name>script
/////////////////////////////////////////////////////////////////////////////
BOOL ExecScript(CString strScript)
{
	if(theApp.m_pIPlatUI == NULL)
	{
		return FALSE;
	}

	// 解释器实例名
	CString strInterpInstance = _T("");

	// 解析解释器名(支持命名方式的解释器)
	if(strScript.Find("<<") == 0)
	{
		int nPos = strScript.Find(">>");
		if(nPos != -1)
		{
			strScript.Delete(0, 2);
			strInterpInstance = strScript.Left(nPos-2);
			strScript.Delete(0, nPos);
		}
	}

	CString strInterpName = "";	// 解释器名
	if(strScript.Find("<") == 0)
	{
		strScript.Delete(0, 1);
		int nPos = strScript.Find(">");
		if(nPos != -1)
		{
			strInterpName = strScript.Left(nPos);
			if(strInterpName.Find("org.interp") != 0)
			{
				strInterpName = "org.interp." + strInterpName;
			}
			strScript.Delete(0, nPos+1);
		}		
	}

	if(strInterpName == "")
	{
		strInterpName = "org.interp.tcl";
	}

	// 创建解释器
	IInterp* pInterp = (IInterp*)(theApp.m_pIPlatUI->CreateVciObject(strInterpName, strInterpInstance));
	if(!pInterp)
	{
		return FALSE;
	}

	pInterp->RunScriptCommand(strScript);

	if(pInterp->GetErrorLine() > 0)
	{
		AfxMessageBox(pInterp->GetResult());
	}

	// 关闭解释器
	theApp.m_pIPlatUI->ReleaseVciObject(pInterp, FALSE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceView

IMPLEMENT_DYNCREATE(CWorkSpaceView, CView)

CWorkSpaceView::CWorkSpaceView()
{
	m_pViewChange = NULL;
	m_bTreeInited = FALSE;
	m_nPageType   = 0;
}

CWorkSpaceView::~CWorkSpaceView()
{
}


BEGIN_MESSAGE_MAP(CWorkSpaceView, CView)
	//{{AFX_MSG_MAP(CWorkSpaceView)
//	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceView drawing

void CWorkSpaceView::OnDraw(CDC* pDC)
{
	//CWorkSpaceDoc* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceView diagnostics

#ifdef _DEBUG
void CWorkSpaceView::AssertValid() const
{
	CView::AssertValid();
}

void CWorkSpaceView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWorkSpaceDoc* CWorkSpaceView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWorkSpaceDoc)));
	return (CWorkSpaceDoc*)m_pDocument;
}
#endif //_DEBUG

void CWorkSpaceView::InstallVCCallBack(TYPEOF_ViewChange* pViewChange)
{
	m_pViewChange = pViewChange;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceView message handlers

/////////////////////////////////////////////////////////////////////////////
// 视图初始化
/////////////////////////////////////////////////////////////////////////////
void CWorkSpaceView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	InitTreeCtrl();	// 初始化图标组和基本菜单
	//InitTaskPanel(); // 初始化TaskPanel

	reset_dll_resource();
}

BOOL CWorkSpaceView::SetTreeType(int nTreeType)
{
	//theApp.m_nLastTreeType = nTreeType;
	m_nPageType = nTreeType;

	if(m_nPageType == PGI_TASKPANEL)
	{
		m_TreeCtrl.MoveWindow(0,0,0,0);

		CRect rect;
		GetWindowRect(rect);
		m_TaskPanel.MoveWindow(0,0,rect.Width(),rect.Height());
	}else
	{
		m_TaskPanel.MoveWindow(0,0,0,0);

		CRect rect;
		GetWindowRect(rect);
		m_TreeCtrl.MoveWindow(0,0,rect.Width(),rect.Height());
	}

	return TRUE;
}

void CWorkSpaceView::InitTaskPanel() 
{
	if(m_bTreeInited)
	{
		return;
	}
	m_bTreeInited = TRUE;

	m_TreeCtrl.m_nTreeType = theApp.m_nLastTreeType;	// 树类型
	m_TreeCtrl.m_nTreeID = theApp.m_nLastTreePaneId;
	m_TreeCtrl.m_strBmpFile = theApp.m_strLastBmpFile;
	m_TreeCtrl.m_nWsNo = m_nWsNo;	// 任务列表号
}

void CWorkSpaceView::InitTreeCtrl() 
{
	if(m_bTreeInited)
	{
		return;
	}
	m_bTreeInited = TRUE;

	m_TreeCtrl.m_nTreeType = theApp.m_nLastTreeType;	// 树类型
	m_TreeCtrl.m_nTreeID = theApp.m_nLastTreePaneId;
	m_TreeCtrl.m_strBmpFile = theApp.m_strLastBmpFile;
	theApp.m_strLastBmpFile = "";
	if(TREE_ID_NAVIGATER == theApp.m_nLastTreePaneId)
	{
		m_TreeCtrl.m_nTreeType = PGI_FILE;
	}else
	if(TREE_ID_FILE == theApp.m_nLastTreePaneId)
	{
		m_TreeCtrl.m_nTreeType = PGI_FILE;
	}
	m_TreeCtrl.m_nWsNo = m_nWsNo;	// 任务列表号
	m_TreeCtrl.InitTree();	// 初始化图标组和基本菜单
	
	if(TREE_ID_NAVIGATER == theApp.m_nLastTreePaneId)
	{
		// 初始化导航树
		TCHAR szFullPath[MAX_PATH];
		TCHAR szdrive[_MAX_DRIVE];
		TCHAR szdir[_MAX_DIR];
		::GetModuleFileName(NULL, szFullPath, MAX_PATH);
		_splitpath(szFullPath, szdrive, szdir, NULL, NULL);

		CString szPath;
		szPath.Format(_T("%s%s"), szdrive, szdir);
		szPath = szPath.Left(szPath.GetLength() - 1);
		int nPos = szPath.ReverseFind('\\');
		if(nPos >= 0)
		{
			szPath = szPath.Left(nPos+1);
		}
		szPath += _T("conf\\menu\\initnavigater.tcl");

		// 创建解释器
		IInterp* pInterp = NULL;
		if(theApp.m_pIPlatUI)
		{
			pInterp = (IInterp*)(theApp.m_pIPlatUI->CreateVciObject("org.interp.tcl", ""));
		}
		if(pInterp == NULL)
		{
			return;
		}

		// 执行脚本
		// 传递文件名要进行斜杠替换
		szPath.Replace('\\', '/');
		pInterp->RunScriptFile(szPath);

		CString strRes = pInterp->GetResult();

		// 释放解释器
		theApp.m_pIPlatUI->ReleaseVciObject(pInterp, FALSE);
	}else
	if(TREE_ID_FILE == theApp.m_nLastTreePaneId)
	{
		// 初始化文件树
		TCHAR szFullPath[MAX_PATH];
		TCHAR szdrive[_MAX_DRIVE];
		TCHAR szdir[_MAX_DIR];
		::GetModuleFileName(NULL, szFullPath, MAX_PATH);
		_splitpath(szFullPath, szdrive, szdir, NULL, NULL);

		CString szPath;
		szPath.Format(_T("%s%s"), szdrive, szdir);
		szPath = szPath.Left(szPath.GetLength() - 1);
		int nPos = szPath.ReverseFind('\\');
		if(nPos >= 0)
		{
			szPath = szPath.Left(nPos+1);
		}
		szPath += _T("Lib\\plat\\filemng.tcl");

		// 创建解释器
		IInterp* pInterp = NULL;
		if(theApp.m_pIPlatUI)
		{
			pInterp = (IInterp*)(theApp.m_pIPlatUI->CreateVciObject("org.interp.tcl", ""));
		}
		if(pInterp == NULL)
		{
			return;
		}

		// 执行脚本(创建和初始化文件树)
		CString strCmdLine;
		strCmdLine.Format("\"%s\" CreateFileTree", szPath);
		// 传递文件名要进行斜杠替换
		strCmdLine.Replace('\\', '/');
		pInterp->RunScriptFile(strCmdLine);

		//CString strRes = pInterp->GetResult();

		// 释放解释器
		theApp.m_pIPlatUI->ReleaseVciObject(pInterp, FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 添加树图标组
/////////////////////////////////////////////////////////////////////////////
void CWorkSpaceView::SetTreeImageList(CString strBmpFile)
{
	m_TreeCtrl.SetTreeImageList(strBmpFile);
}

/////////////////////////////////////////////////////////////////////////////
// 添加树菜单
/////////////////////////////////////////////////////////////////////////////
int CWorkSpaceView::AddTreeMenu(CMenu* pMenu)
{
	if(!pMenu)
		return -1;
	return m_TreeCtrl.AddTreeMenu(pMenu);
}

/////////////////////////////////////////////////////////////////////////////
// 添加树菜单项
/////////////////////////////////////////////////////////////////////////////
int CWorkSpaceView::AddMenuItem(HWORKMENUITEM* pItem)
{
	if(!pItem)
		return -1;
	return m_TreeCtrl.AddMenuItem(pItem->byType, pItem->nID, pItem->strItem, pItem->strCommand, pItem->Proc, pItem->UpdateProc);
}

/////////////////////////////////////////////////////////////////////////////
// 设置变量双击脚本
/////////////////////////////////////////////////////////////////////////////
void CWorkSpaceView::SetDClickScript(CString strScript)
{
	m_TreeCtrl.m_strDClickScript = strScript;
}

/////////////////////////////////////////////////////////////////////////////
// 添加树节点
/////////////////////////////////////////////////////////////////////////////
HTREEITEM CWorkSpaceView::AddTreeItem(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return NULL;

	HTREEITEM hParent = pItem->hParent;
	if(pItem->dwMask & TIM_PARENTID)
	{
		hParent = m_TreeCtrl.GetItemFromID(pItem->nParentID);
	}

	HTREEITEM hItem = m_TreeCtrl.AddTreeItem(hParent, pItem->lpszName, pItem->nImage);

	if(pItem->dwMask & TIM_TYPE)
	{
		m_TreeCtrl.SetItemType(hItem, pItem->byType);
	}

	if(pItem->dwMask & TIM_MENU)
	{
		//if(pItem->pMenu != NULL)
		{
			int nMenuIndex = (int)(pItem->pMenu);
			int nMenuCount = m_TreeCtrl.m_aMenus.GetSize();
			if((nMenuIndex >= 0) && (nMenuIndex <nMenuCount))
			{
				m_TreeCtrl.SetItemMenu(hItem, m_TreeCtrl.GetTreeMenu(nMenuIndex));
			}else
			{
				m_TreeCtrl.SetItemMenu(hItem, pItem->pMenu);
			}
		}
	}/*else if(pItem->pMenu == NULL)
	{
		m_TreeCtrl.SetItemMenu(hItem, pItem->pMenu);
	}*/

	if(pItem->dwMask & TIM_OBJECT)
	{
		m_TreeCtrl.SetItemObject(hItem, pItem->pObject);
	}

	if(pItem->dwMask & TIM_STATE)
	{
		m_TreeCtrl.SetItemState1(hItem, pItem->byState);
	}

	if(pItem->dwMask & TIM_ID)
	{
		m_TreeCtrl.SetItemID(hItem, pItem->nID);
	}

	if(pItem->dwMask & TIM_INFO)
	{
		m_TreeCtrl.SetItemInfo(hItem, pItem->strInfo);
	}

	if(pItem->dwMask & TIM_DCLICK)
	{
		m_TreeCtrl.SetItemDClick(hItem, pItem->strDClick);
	}

	if(pItem->dwMask & TIM_COLOR)
	{
		m_TreeCtrl.SetItemColor(hItem, pItem->crColor);
	}
	if(pItem->dwMask & TIM_BOLD)
	{
		m_TreeCtrl.SetItemBold(hItem, TRUE);
	}else
	{
		m_TreeCtrl.SetItemBold(hItem, FALSE);
	}
	if(pItem->dwMask & TIM_FONT)
	{
	}

	return hItem;
}

/////////////////////////////////////////////////////////////////////////////
// 删除树节点
/////////////////////////////////////////////////////////////////////////////
BOOL CWorkSpaceView::DelTreeItem(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return FALSE;

	BOOL bChildOnly = ((pItem->dwMask & TIM_CHILDONLY) != 0);

	if(pItem->dwMask & TIM_ITEM)
	{
		return m_TreeCtrl.DelTreeItem(pItem->hItem, bChildOnly);
	}else
	if(pItem->dwMask & TIM_ID)
	{
		HTREEITEM hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
		if(hItem != NULL)
		{
			return m_TreeCtrl.DelTreeItem(hItem, bChildOnly);
		}
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 设置树节点类型
/////////////////////////////////////////////////////////////////////////////
void CWorkSpaceView::SetItemType(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return;

	if(pItem->dwMask & TIM_ITEM)
	{
		m_TreeCtrl.SetItemType(pItem->hItem, pItem->byType);
	}else
	if(pItem->dwMask & TIM_ID)
	{
		HTREEITEM hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
		if(hItem != NULL)
		{
			m_TreeCtrl.SetItemType(hItem, pItem->byType);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 获取树节点类型
/////////////////////////////////////////////////////////////////////////////
BYTE CWorkSpaceView::GetItemType(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return NULL;

	if(pItem->dwMask & TIM_ITEM)
	{
		return m_TreeCtrl.GetItemType(pItem->hItem);
	}else
	if(pItem->dwMask & TIM_ID)
	{
		HTREEITEM hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
		if(hItem != NULL)
		{
			return m_TreeCtrl.GetItemType(hItem);
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 设置树节点状态
/////////////////////////////////////////////////////////////////////////////
void CWorkSpaceView::SetItemState1(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return;

	// 是否刷新全部,在开始测试之前,如果某一项处于禁止状态,
	// 则不用恢复到未测状态,因此这种情况不用TIM_FRESHALL掩码,
	// 如果人工恢复某一项,则必须可以恢复,这种情况要使用
	// TIM_FRESHALL掩码
	BOOL bFreshAll = (pItem->dwMask & TIM_FRESHALL);

	if(pItem->dwMask & TIM_ITEM)	// 使用节点句柄
	{
		if(pItem->dwMask & TIM_TRAVERSAL)	// 遍历方式
		{
			m_TreeCtrl.TraversalSetItemState(pItem->hItem, pItem->byState, bFreshAll);
		}else
		if(pItem->dwMask & TIM_UPTRAVERSAL)	// 向上遍历方式
		{
			m_TreeCtrl.UpTraversalSetItemState(pItem->hItem, pItem->byState);
		}else	// 普通方式
		{
			m_TreeCtrl.SetItemState1(pItem->hItem, pItem->byState);
		}
	}else
	if(pItem->dwMask & TIM_ID)	// 使用节点ID
	{
		if(pItem->nID == -1)	// ID变换,-1表示是所有节点
		{
			pItem->nID = 0;
		}
		HTREEITEM hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
		if(hItem != NULL)
		{
			if(pItem->dwMask & TIM_TRAVERSAL)	// 遍历方式
			{
				m_TreeCtrl.TraversalSetItemState(hItem, pItem->byState, bFreshAll);
			}else
			if(pItem->dwMask & TIM_UPTRAVERSAL)	// 向上遍历方式
			{
				m_TreeCtrl.UpTraversalSetItemState(hItem, pItem->byState);
			}else	// 普通方式
			{
				m_TreeCtrl.SetItemState1(hItem, pItem->byState);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 获取树节点状态
/////////////////////////////////////////////////////////////////////////////
BYTE CWorkSpaceView::GetItemState1(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return NULL;

	if(pItem->dwMask & TIM_ITEM)
	{
		return m_TreeCtrl.GetItemState1(pItem->hItem);
	}else
	if(pItem->dwMask & TIM_ID)
	{
		HTREEITEM hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
		if(hItem != NULL)
		{
			return m_TreeCtrl.GetItemState1(hItem);
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 设置树节点菜单
/////////////////////////////////////////////////////////////////////////////
void CWorkSpaceView::SetItemMenu(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return;

	if(pItem->dwMask & TIM_ITEM)
	{
		m_TreeCtrl.SetItemMenu(pItem->hItem, pItem->pMenu);
	}else
	if(pItem->dwMask & TIM_ID)
	{
		HTREEITEM hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
		if(hItem != NULL)
		{
			m_TreeCtrl.SetItemMenu(hItem, pItem->pMenu);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 获取树节点菜单
/////////////////////////////////////////////////////////////////////////////
CMenu* CWorkSpaceView::GetItemMenu(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return NULL;

	if(pItem->dwMask & TIM_ITEM)
	{
		return m_TreeCtrl.GetItemMenu(pItem->hItem);
	}else
	if(pItem->dwMask & TIM_ID)
	{
		HTREEITEM hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
		if(hItem != NULL)
		{
			return m_TreeCtrl.GetItemMenu(hItem);
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 设置树节点对象
/////////////////////////////////////////////////////////////////////////////
void CWorkSpaceView::SetItemObject(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return;

	if(pItem->dwMask & TIM_ITEM)
	{
		m_TreeCtrl.SetItemObject(pItem->hItem, pItem->pObject);
	}else
	if(pItem->dwMask & TIM_ID)
	{
		HTREEITEM hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
		if(hItem != NULL)
		{
			m_TreeCtrl.SetItemObject(hItem, pItem->pObject);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 获取树节点对象
/////////////////////////////////////////////////////////////////////////////
CObject* CWorkSpaceView::GetItemObject(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return NULL;

	if(pItem->dwMask & TIM_ITEM)
	{
		return m_TreeCtrl.GetItemObject(pItem->hItem);
	}else
	if(pItem->dwMask & TIM_ID)
	{
		HTREEITEM hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
		if(hItem != NULL)
		{
			return m_TreeCtrl.GetItemObject(hItem);
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 设置树节点ID
/////////////////////////////////////////////////////////////////////////////
void CWorkSpaceView::SetItemID(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return;

	if(pItem->dwMask & TIM_ITEM)
	{
		m_TreeCtrl.SetItemID(pItem->hItem, pItem->nID);
	}else
	if(pItem->dwMask & TIM_ID)
	{
		HTREEITEM hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
		if(hItem != NULL)
		{
			m_TreeCtrl.SetItemID(hItem, pItem->nID);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 获取树节点ID
/////////////////////////////////////////////////////////////////////////////
int CWorkSpaceView::GetItemID(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return NULL;

	if(pItem->dwMask & TIM_ITEM)
	{
		return m_TreeCtrl.GetItemID(pItem->hItem);
	}else
	if(pItem->dwMask & TIM_ID)
	{
		HTREEITEM hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
		if(hItem != NULL)
		{
			return m_TreeCtrl.GetItemID(hItem);
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 设置树节点名
/////////////////////////////////////////////////////////////////////////////
void CWorkSpaceView::SetItemName(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return;

	if(pItem->dwMask & TIM_ITEM)
	{
		m_TreeCtrl.SetItemText(pItem->hItem, pItem->lpszName);
	}else
	if(pItem->dwMask & TIM_ID)
	{
		HTREEITEM hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
		if(hItem != NULL)
		{
			m_TreeCtrl.SetItemText(hItem, pItem->lpszName);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 获取树节点名
/////////////////////////////////////////////////////////////////////////////
CString CWorkSpaceView::GetItemName(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return "";

	if(pItem->dwMask & TIM_ITEM)
	{
		return m_TreeCtrl.GetItemText(pItem->hItem);
	}else
	if(pItem->dwMask & TIM_ID)
	{
		HTREEITEM hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
		if(hItem != NULL)
		{
			return m_TreeCtrl.GetItemText(hItem);
		}
	}

	return "";
}

/////////////////////////////////////////////////////////////////////////////
// 设置树节点图标
/////////////////////////////////////////////////////////////////////////////
void CWorkSpaceView::SetItemImage(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return;

	if(pItem->dwMask & TIM_ITEM)
	{
		m_TreeCtrl.SetItemImage1(pItem->hItem, pItem->nImage);
	}else
	if(pItem->dwMask & TIM_ID)
	{
		HTREEITEM hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
		if(hItem != NULL)
		{
			m_TreeCtrl.SetItemImage1(hItem, pItem->nImage);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 获取树节点图标
/////////////////////////////////////////////////////////////////////////////
int CWorkSpaceView::GetItemImage(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return NULL;

	if(pItem->dwMask & TIM_ITEM)
	{
		return m_TreeCtrl.GetItemImage1(pItem->hItem);
	}else
	if(pItem->dwMask & TIM_ID)
	{
		HTREEITEM hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
		if(hItem != NULL)
		{
			return m_TreeCtrl.GetItemImage1(hItem);
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 展开树节点
/////////////////////////////////////////////////////////////////////////////
void CWorkSpaceView::ExpandItem(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return;

	if(pItem->dwMask & TIM_ITEM)
	{
		m_TreeCtrl.Expand(pItem->hItem, TVE_EXPAND);
	}else
	if(pItem->dwMask & TIM_ID)
	{
		HTREEITEM hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
		if(hItem != NULL)
		{
			m_TreeCtrl.Expand(hItem, TVE_EXPAND);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 收缩树节点
/////////////////////////////////////////////////////////////////////////////
void CWorkSpaceView::CollapseItem(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return;

	if(pItem->dwMask & TIM_ITEM)
	{
		m_TreeCtrl.Expand(pItem->hItem, TVE_COLLAPSE);
	}else
	if(pItem->dwMask & TIM_ID)
	{
		HTREEITEM hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
		if(hItem != NULL)
		{
			m_TreeCtrl.Expand(hItem, TVE_COLLAPSE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 转到某个项目
/////////////////////////////////////////////////////////////////////////////
void CWorkSpaceView::EnsureItem(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return;

	if(pItem->dwMask & TIM_ITEM)
	{
		m_TreeCtrl.EnsureVisible(pItem->hItem);
	}else
	if(pItem->dwMask & TIM_ID)
	{
		HTREEITEM hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
		if(hItem != NULL)
		{
			m_TreeCtrl.EnsureVisible(hItem);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 设置树节点信息
/////////////////////////////////////////////////////////////////////////////
BOOL CWorkSpaceView::SetItemInfo(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return FALSE;

	HTREEITEM hItem = pItem->hItem;
	if(pItem->dwMask & TIM_ID)
	{
		hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
	}
	if(hItem == NULL)
		return FALSE;

	if(pItem->dwMask & TIM_NAME)
	{
		m_TreeCtrl.SetItemText(hItem, pItem->lpszName);
	}
	if(pItem->dwMask & TIM_IMAGE)
	{
		m_TreeCtrl.SetItemImage1(hItem, pItem->nImage);
	}
	if(pItem->dwMask & TIM_TYPE)
	{
		m_TreeCtrl.SetItemType(hItem, pItem->byType);
	}
	if(pItem->dwMask & TIM_STATE)
	{
		m_TreeCtrl.SetItemState1(hItem, pItem->byState);
	}
	if(pItem->dwMask & TIM_INFO)
	{
		m_TreeCtrl.SetItemInfo(hItem, pItem->strInfo);
	}
	if(pItem->dwMask & TIM_DCLICK)
	{
		m_TreeCtrl.SetItemDClick(hItem, pItem->strDClick);
	}
	if(pItem->dwMask & TIM_COLOR)
	{
		m_TreeCtrl.SetItemColor(hItem, pItem->crColor);
	}
	if(pItem->dwMask & TIM_BOLD)
	{
		m_TreeCtrl.SetItemBold(hItem, TRUE);
	}else
	{
		m_TreeCtrl.SetItemBold(hItem, FALSE);
	}
	if(pItem->dwMask & TIM_FONT)
	{
	}
	if(pItem->dwMask & TIM_MENU)
	{
		int nMenuIndex = (int)(pItem->pMenu);
		int nMenuCount = m_TreeCtrl.m_aMenus.GetSize();
		if((nMenuIndex >= 0) && (nMenuIndex <nMenuCount))
		{
			m_TreeCtrl.SetItemMenu(hItem, m_TreeCtrl.GetTreeMenu(nMenuIndex));
		}else
		{
			m_TreeCtrl.SetItemMenu(hItem, pItem->pMenu);
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 获取树节点信息,直接将信息返回到传入的结构中
/////////////////////////////////////////////////////////////////////////////
BOOL CWorkSpaceView::GetItemInfo(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return FALSE;

	HTREEITEM hItem = pItem->hItem;
	if(pItem->dwMask & TIM_ID)
	{
		hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
		pItem->hItem = hItem;
	}
	if(hItem == NULL)
		return FALSE;

	if(pItem->dwMask & TIM_NAME)
	{
		pItem->lpszName = m_TreeCtrl.GetItemText(hItem);
	}
	if(pItem->dwMask & TIM_IMAGE)
	{
		pItem->nImage = m_TreeCtrl.GetItemImage1(hItem);
	}
	if(pItem->dwMask & TIM_TYPE)
	{
		pItem->byType = m_TreeCtrl.GetItemType(hItem);
	}
	if(pItem->dwMask & TIM_STATE)
	{
		pItem->byState = m_TreeCtrl.GetItemState1(hItem);
	}
	if(pItem->dwMask & TIM_PARENT)
	{
		pItem->hParent = m_TreeCtrl.GetParentItem(hItem);
	}
	if(pItem->dwMask & TIM_INFO)
	{
		pItem->strInfo = m_TreeCtrl.GetItemInfo(hItem);
	}
	if(pItem->dwMask & TIM_DCLICK)
	{
		pItem->strDClick = m_TreeCtrl.GetItemDClick(hItem);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 查找节点
/////////////////////////////////////////////////////////////////////////////
HTREEITEM CWorkSpaceView::SearchItem(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return NULL;

	if(pItem->dwMask & TIM_INFO)
	{
		return m_TreeCtrl.GetItemFromInfo(pItem->strInfo);
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 获取第一个子节点
/////////////////////////////////////////////////////////////////////////////
HTREEITEM CWorkSpaceView::GetChildItem(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return NULL;

	HTREEITEM hItem = pItem->hItem;
	if(pItem->dwMask & TIM_ID)
	{
		hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
	}
	if(hItem == NULL)
		return NULL;

	return m_TreeCtrl.GetChildItem(hItem);
}

/////////////////////////////////////////////////////////////////////////////
// 获取下一个节点,pItem->byType表示关系代码,MFC中的定义如下
//#define TVGN_ROOT               0x0000
//#define TVGN_NEXT               0x0001
//#define TVGN_PREVIOUS           0x0002
//#define TVGN_PARENT             0x0003
//#define TVGN_CHILD              0x0004
//#define TVGN_FIRSTVISIBLE       0x0005
//#define TVGN_NEXTVISIBLE        0x0006
//#define TVGN_PREVIOUSVISIBLE    0x0007
//#define TVGN_DROPHILITE         0x0008
//#define TVGN_CARET              0x0009
//#if (_WIN32_IE >= 0x0400)
//#define TVGN_LASTVISIBLE        0x000A
//#endif      // _WIN32_IE >= 0x0400
/////////////////////////////////////////////////////////////////////////////
HTREEITEM CWorkSpaceView::GetNextItem(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return NULL;

	HTREEITEM hItem = pItem->hItem;
	if(pItem->dwMask & TIM_ID)
	{
		hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
	}
	if(hItem == NULL)
		return NULL;

	return m_TreeCtrl.GetNextItem(hItem, pItem->byType);
}

/////////////////////////////////////////////////////////////////////////////
// 对子节点排序
/////////////////////////////////////////////////////////////////////////////
BOOL CWorkSpaceView::SortChildren(HWORKTREEITEM* pItem)
{
	if(!pItem)
		return FALSE;

	HTREEITEM hItem = pItem->hItem;
	if(pItem->dwMask & TIM_ID)
	{
		hItem = m_TreeCtrl.GetItemFromID(pItem->nID);
	}
	if(hItem == NULL)
		return FALSE;

	return m_TreeCtrl.SortChildren(hItem);
}

int CWorkSpaceView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Create the tree control
	if(!m_TreeCtrl.Create( WS_CHILD | WS_VISIBLE | WS_TABSTOP |
		TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT,
		CRect(0,0,0,0), this, IDC_WORKTREE_CLASS))
	{
		TRACE0("Failed to create CWorkTreeCtrl\n");
		return -1;
	}

	// Create task panel control
	if (!m_TaskPanel.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP |
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		CRect(0,0,0,0),	this, IDC_TASKPANEL_CLASS))
	{
		TRACE0("Failed to create taskpanel bar.");
		return -1;
	}

	m_TaskPanel.SetOwner(AfxGetMainWnd());
	m_TaskPanel.EnableMarkup(TRUE);
	
	return 0;
}

void CWorkSpaceView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if(m_nPageType == PGI_TASKPANEL)
	{
		m_TreeCtrl.MoveWindow(0,0,0,0);
		m_TaskPanel.MoveWindow(0,0,cx,cy);
	}else
	{
		m_TaskPanel.MoveWindow(0,0,0,0);
		m_TreeCtrl.MoveWindow(0,0,cx,cy);
	}
}

void CWorkSpaceView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);

	// 如果页面切换,则调用此回调函数
	if(m_pViewChange)
	{
		m_pViewChange();
	}
}

CXTPTaskPanelGroupItem* CWorkSpaceView::AddTaskItem(HWORKTASKITEM* pItem)
{
	return m_TaskPanel.AddTaskItem(pItem);
}

void CWorkSpaceView::DelTaskItem(HWORKTASKITEM* pItem)
{
	m_TaskPanel.DelTaskItem(pItem);
}

int CWorkSpaceView::GetTaskItemIDByItem(CXTPTaskPanelItem* pTaskItem)
{
	return m_TaskPanel.GetTaskItemIDByItem(pTaskItem);
}

int CWorkSpaceView::GetTaskItemIDByCaption(CString strCaption, int nType)
{
	return m_TaskPanel.GetTaskItemIDByCaption(strCaption, nType);
}

CXTPTaskPanelGroupItem* CWorkSpaceView::GetTaskItemByID(int nID)
{
	return m_TaskPanel.GetTaskItemByID(nID);
}

void CWorkSpaceView::CreateTaskMenu(CMenu* pMenu)
{
	m_TaskPanel.CreateFromMenu(pMenu);
}

void CWorkSpaceView::SetTaskPanelTheme(XTPTaskPanelPaintTheme taskPanelTheme)
{
	m_TaskPanel.SetTaskPanelTheme(taskPanelTheme);
}

int CWorkSpaceView::SetTaskPanelIcon(CString strIcon)
{
	strIcon.MakeLower();
	if(strIcon.Find(".ico") != -1)
	{
		return m_TaskPanel.SetIcon(strIcon);
	}else
	if(strIcon.Find(".bmp") != -1)
	{
		return m_TaskPanel.SetIcons(strIcon);
	}
	return -1;
}