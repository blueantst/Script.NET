// WorkTaskPanel.cpp : implementation file
//

#include "stdafx.h"
#include "workspace.h"
#include "WorkTaskPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Separator Item class
class CTaskPanelSeparatorItem : public CXTPTaskPanelGroupItem
{
public:
	CTaskPanelSeparatorItem()
	{
	}

	virtual CRect OnReposition(CRect rc)
	{
		rc.bottom = rc.top + 4;
		m_rcItem = rc;

		return m_rcItem;
	}

	virtual void OnDrawItem(CDC* pDC, CRect rc)
	{
		rc.DeflateRect(0, 1);
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));		
	}
};

/////////////////////////////////////////////////////////////////////////////
// CWorkTaskPanel

CWorkTaskPanel::CWorkTaskPanel()
{
	m_bUIMode = FALSE;
	m_nTaskPanelItemID = 40000;
	m_nTaskPanelImageID = 0;
}

CWorkTaskPanel::~CWorkTaskPanel()
{
}


BEGIN_MESSAGE_MAP(CWorkTaskPanel, CXTPTaskPanel)
	//{{AFX_MSG_MAP(CWorkTaskPanel)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
END_MESSAGE_MAP()

CString StripMnemonics(CString str)
{
	XTPDrawHelpers()->StripMnemonics(str);
	int nIndex = str.Find(_T('\t'));
	if (nIndex > 0)
	{
		return str.Left(nIndex);
	}
	return str;
}

void AddGroupItems(CXTPTaskPanelGroup* pGroup,  CMenu* pMenu)
{
	for (UINT i = 0; i < pMenu->GetMenuItemCount(); i++)
	{
		CMenu* pMenuPopup = pMenu->GetSubMenu(i);

		if (pMenuPopup)
		{
			AddGroupItems(pGroup, pMenuPopup);
		}
		else
		{	
			MENUITEMINFO info = { sizeof(MENUITEMINFO), MIIM_TYPE | MIIM_STATE};
			::GetMenuItemInfo(pMenu->m_hMenu, i, TRUE, &info);
	
			if (((info.fType & MFT_SEPARATOR) == MFT_SEPARATOR) || (pMenu->GetMenuItemID(i) == 0))
			{
				pGroup->GetItems()->Add(new CTaskPanelSeparatorItem(), 0);
				continue;
			}

			CString strCaption;
			pMenu->GetMenuString(i, strCaption, MF_BYPOSITION);			;
			
			UINT nID = pMenu->GetMenuItemID(i);
			CXTPTaskPanelGroupItem* pItem = pGroup->AddLinkItem(nID, nID);
			pItem->SetCaption(StripMnemonics(strCaption));
		}		
	}
}

void CWorkTaskPanel::CreateFromMenu(CMenu* pMenu)
{
	GetGroups()->Clear();


	for (UINT i = 0; i < pMenu->GetMenuItemCount(); i++)
	{
		CMenu* pMenuPopup = pMenu->GetSubMenu(i);
		if (!pMenuPopup)
			continue;

		CString strCaption;
		pMenu->GetMenuString(i, strCaption, MF_BYPOSITION);

		CXTPTaskPanelGroup* pGroup = AddGroup(0);
		pGroup->SetCaption(StripMnemonics(strCaption));


		AddGroupItems(pGroup, pMenuPopup);
	}

	m_bUIMode = TRUE;
}

class CWorkTaskPanel::CTaskPanelCmdUI : public CCmdUI
{
public: // re-implementations only
	virtual void Enable(BOOL bOn);
	virtual void SetCheck(int nCheck);
	virtual void SetText(LPCTSTR lpszText);

public:
	CXTPTaskPanelGroupItem* m_pItem;
};

void CWorkTaskPanel::CTaskPanelCmdUI::Enable(BOOL bOn)
{
	m_bEnableChanged = TRUE;

	ASSERT(m_pItem != NULL);

	m_pItem->SetEnabled(bOn);
}

void CWorkTaskPanel::CTaskPanelCmdUI::SetCheck(int nCheck)
{
	ASSERT(nCheck >= 0 && nCheck <= 2); // 0 =>off, 1 =>on, 2 =>indeterminate
	ASSERT(m_pItem != NULL);

	m_pItem->SetItemSelected(nCheck);
}

void CWorkTaskPanel::CTaskPanelCmdUI::SetText(LPCTSTR lpszCaption)
{
	ASSERT(m_pItem != NULL);

	m_pItem->SetCaption(lpszCaption);
}


LRESULT CWorkTaskPanel::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM)
{
	
	if (m_bUIMode && GetSafeHwnd() && (GetStyle() & WS_VISIBLE))
	{
		CWnd* pTarget = GetOwner();

		if (pTarget != NULL)
			OnUpdateCmdUI(pTarget, (BOOL)wParam);
	}
	return 0L;
}

void CWorkTaskPanel::OnUpdateCmdUI(CWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CTaskPanelCmdUI state;
	state.m_pOther = this;

	for (int i = 0; i < GetGroupCount(); i++)
	{
		CXTPTaskPanelGroup* pGroup = GetAt(i);
		
		for (state.m_nIndex = 0; (int)state.m_nIndex < pGroup->GetItemCount(); state.m_nIndex++)
		{
			CXTPTaskPanelGroupItem* pItem = pGroup->GetAt(state.m_nIndex);
			
			state.m_nID = pItem->GetID();
			state.m_pItem = pItem;
			
			if (CWnd::OnCmdMsg(0,
				MAKELONG((int)CN_UPDATE_COMMAND_UI, WM_COMMAND + WM_REFLECT_BASE),
				&state, NULL))
				continue;
			
			// allow the toolbar itself to have update handlers
			if (CWnd::OnCmdMsg(state.m_nID, (int)CN_UPDATE_COMMAND_UI, &state, NULL))
				continue;
			
			// allow the owner to process the update
			state.DoUpdate(pTarget, bDisableIfNoHndler);
		}
	}

	// update the dialog controls added to the toolbar
	UpdateDialogControls(pTarget, bDisableIfNoHndler);

}

/////////////////////////////////////////////////////////////////////////////
// CWorkTaskPanel message handlers
void CWorkTaskPanel::OnClick(CXTPTaskPanelGroupItem* pItem)
{
	for(int i=0; i<m_aTaskPanelItem.GetSize(); i++)
	{
		if(m_aTaskPanelItem[i].lpItem == (LPVOID)pItem)
		{
			CString strDClick = m_aTaskPanelItem[i].strDClick;
			if(strDClick.Find("html:") == 0)
			{
				strDClick.Delete(0, 5);
				theApp.m_pIPlatUI->SendOwmCmd("org.owm.htmlview", OC_VIEWHTMLPAGE, ((WPARAM)(LPCTSTR)strDClick), 0, NULL);
			}else
			{
				ExecScript(strDClick);
			}
			return;
		}
	}
}

void CWorkTaskPanel::SetTaskPanelTheme(XTPTaskPanelPaintTheme taskPanelTheme)
{
	SetTheme(taskPanelTheme);
	//RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
}

int CWorkTaskPanel::SetIcons(CString strIcons)
{
	// 判断是否已经加载此图片
	strIcons.MakeLower();
	for(int i=0; i<m_asIcons.GetSize(); i++)
	{
		if(strIcons == m_asIcons[i])
		{
			return -1;
		}
	}

	int nImage = m_nTaskPanelImageID;

	get_dll_resource();

	CBitmap bmp;

	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetApp()->m_hInstance, strIcons,
        IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if(hBitmap == NULL)
		return m_nTaskPanelImageID;

	BITMAP bmpInfo;
	::GetObject(hBitmap, sizeof(BITMAP), &bmpInfo);

	bmp.Attach(hBitmap);

	int nBmpCount = bmpInfo.bmWidth / bmpInfo.bmHeight;
	UINT uIDs[100];
	for(int i=0; i<nBmpCount; i++)
	{
		if(i < 100)
		{
			uIDs[i] = m_nTaskPanelImageID++;
		}
	}

	BOOL bRet = GetImageManager()->SetIcons(bmp, uIDs, nBmpCount, CSize(bmpInfo.bmHeight,bmpInfo.bmHeight), xtpImageNormal);

	reset_dll_resource();

	m_asIcons.Add(strIcons);

	return nImage;
}

int CWorkTaskPanel::SetIcon(CString strIcon)
{
	// 判断是否已经加载此图片
	strIcon.MakeLower();
	for(int i=0; i<m_asIcons.GetSize(); i++)
	{
		if(strIcon == m_asIcons[i])
		{
			return -1;
		}
	}

	get_dll_resource();

	WORD wIndex = 0;
	HICON hIcon = ::ExtractAssociatedIcon(AfxGetApp()->m_hInstance,	strIcon.GetBuffer(0), &wIndex);
	strIcon.ReleaseBuffer();
	GetImageManager()->SetIcon(CXTPImageManagerIconHandle(hIcon), m_nTaskPanelImageID);//, CSize(32, 32));

	reset_dll_resource();

	m_asIcons.Add(strIcon);

	return m_nTaskPanelImageID++;
}

int CWorkTaskPanel::GetTaskItemIDByItem(CXTPTaskPanelItem* pTaskItem)
{
	if(pTaskItem == NULL)
	{
		return 0;
	}

	for(int i=0; i<m_aTaskPanelItem.GetSize(); i++)
	{
		if(m_aTaskPanelItem[i].lpItem == pTaskItem)
		{
			return m_aTaskPanelItem[i].nID;
		}
	}

	return 0;
}

int CWorkTaskPanel::GetTaskItemIDByCaption(CString strCaption, int nType)
{
	if(strCaption == "")
	{
		return 0;
	}

	for(int i=0; i<m_aTaskPanelItem.GetSize(); i++)
	{
		if((m_aTaskPanelItem[i].strCaption == strCaption) && (m_aTaskPanelItem[i].nType == nType))
		{
			return m_aTaskPanelItem[i].nID;
		}
	}

	return 0;
}

CXTPTaskPanelGroupItem* CWorkTaskPanel::GetTaskItemByID(int nID)
{
	if(nID == 0)
	{
		return NULL;
	}

	for(int i=0; i<m_aTaskPanelItem.GetSize(); i++)
	{
		if(m_aTaskPanelItem[i].nID == nID)
		{
			return (CXTPTaskPanelGroupItem*)(m_aTaskPanelItem[i].lpItem);
		}
	}

	return NULL;
}

CXTPTaskPanelGroupItem* CWorkTaskPanel::AddTaskItem(HWORKTASKITEM* pItem)
{
	for(int i=0; i<m_aTaskPanelItem.GetSize(); i++)
	{
		if( (m_aTaskPanelItem[i].nType == pItem->nType) && 
			((m_aTaskPanelItem[i].nID == pItem->nID) || (m_aTaskPanelItem[i].strCaption == pItem->strCaption)) )
		{
			return (CXTPTaskPanelGroupItem*)(m_aTaskPanelItem[i].lpItem);
		}
	}

	HWORKTASKITEM item;
	item.dwMask		= pItem->dwMask;
	if(pItem->nID != 0)
	{
		item.nID		= pItem->nID;
	}else
	{
		item.nID		= m_nTaskPanelItemID++;
	}
	if(item.dwMask & TIM_IMAGE)
	{
		item.nImage		= pItem->nImage;
	}else
	{
		item.nImage		= -1;
	}
	item.nParent	= pItem->nParent;
	item.nType		= pItem->nType;
	item.strCaption	= pItem->strCaption;
	item.strDClick	= pItem->strDClick;
	item.strInfo	= pItem->strInfo;
	item.strTooltip	= pItem->strTooltip;
	item.crColor	= pItem->crColor;
	item.logFont	= pItem->logFont;

	CXTPTaskPanelGroup* pTaskGroup = (CXTPTaskPanelGroup*)(GetTaskItemByID(item.nParent));
	CXTPTaskPanelItem* pTaskItem = NULL;
	if(item.nType == WTT_GROUP)
	{
		pTaskItem = AddGroup(item.nID);
		pTaskItem->SetCaption(item.strCaption);
		if(item.dwMask & TIM_SPECIAL)
		{
			((CXTPTaskPanelGroup*)(pTaskItem))->SetSpecialGroup();
		}
	}else
	if(item.nType == WTT_TEXT)
	{
		if(pTaskGroup)
		{
			pTaskItem = pTaskGroup->AddTextItem(item.strCaption);
			pTaskItem->SetID(item.nID);
		}
	}else
	if(item.nType == WTT_LINK)
	{
		if(pTaskGroup)
		{
			pTaskItem = pTaskGroup->AddTextItem(item.strCaption);
			pTaskItem->SetID(item.nID);
			pTaskItem->SetType(xtpTaskItemTypeLink);
		}
	}else
	if(item.nType == WTT_MASHUP)
	{
	}

	if(item.dwMask & TIM_IMAGE)
	{
		pTaskItem->SetIconIndex(item.nImage);
	}
	if((pTaskItem != NULL) && (item.dwMask & TIM_COLOR))
	{
		((CXTPTaskPanelGroupItem*)pTaskItem)->SetTextColor(item.crColor);
	}

	if((pTaskItem != NULL) && (item.dwMask & TIM_BOLD))
	{
		((CXTPTaskPanelGroupItem*)pTaskItem)->SetBold(TRUE);
	}

	if((pTaskItem != NULL) && (item.dwMask & TIM_FONT))
	{
		pTaskItem->SetItemFont(&(item.logFont));
	}

	item.lpItem = (LPVOID)pTaskItem;
	m_aTaskPanelItem.Add(item);

	if(pTaskItem)
	{
		pTaskItem->SetItemData((DWORD_PTR)(&(m_aTaskPanelItem[m_aTaskPanelItem.GetSize()-1])));
	}

	return (CXTPTaskPanelGroupItem*)(m_aTaskPanelItem[m_aTaskPanelItem.GetSize()-1].lpItem);
}

void CWorkTaskPanel::DelTaskItem(HWORKTASKITEM* pItem)
{
	for(int i=m_aTaskPanelItem.GetSize()-1; i>=0; i--)
	{
		if(pItem->dwMask & TIM_ID)
		{
			if(pItem->nID == 0)
			{
				// 删除全部
				((CXTPTaskPanelGroupItem*)(m_aTaskPanelItem[i].lpItem))->Remove();
				m_aTaskPanelItem.RemoveAt(i);
				// 如果删除全部,则Item计数器恢复为初始值
				m_nTaskPanelItemID = 40000;
				continue;
			}else
			if((m_aTaskPanelItem[i].nID == pItem->nID) || (m_aTaskPanelItem[i].nParent == pItem->nID))
			{
				// 删除指定ID和下面的所有ID
				((CXTPTaskPanelGroupItem*)(m_aTaskPanelItem[i].lpItem))->Remove();
				m_aTaskPanelItem.RemoveAt(i);
				continue;
			}
		}
		if(pItem->dwMask & TIM_TYPE)
		{
			if(m_aTaskPanelItem[i].nType == pItem->nType)
			{
				// 删除指定类型的
				((CXTPTaskPanelGroupItem*)(m_aTaskPanelItem[i].lpItem))->Remove();
				m_aTaskPanelItem.RemoveAt(i);
				continue;
			}
		}
		if(pItem->dwMask & TIM_NAME)
		{
			if(m_aTaskPanelItem[i].strCaption == pItem->strCaption)
			{
				// 删除指定标题的
				((CXTPTaskPanelGroupItem*)(m_aTaskPanelItem[i].lpItem))->Remove();
				m_aTaskPanelItem.RemoveAt(i);
				continue;
			}
		}
		if(pItem->dwMask & TIM_INFO)
		{
			if(m_aTaskPanelItem[i].strInfo == pItem->strInfo)
			{
				// 删除指定信息的
				((CXTPTaskPanelGroupItem*)(m_aTaskPanelItem[i].lpItem))->Remove();
				m_aTaskPanelItem.RemoveAt(i);
				continue;
			}
		}
	}
}
