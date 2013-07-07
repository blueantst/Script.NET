// DynamicHelpBar.cpp : implementation file
//

#include "stdafx.h"
#include "htmlviewowm.h"
#include "DynamicHelpBar.h"
#include "ComboBoxWord.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynamicHelpBar

CDynamicHelpBar::CDynamicHelpBar()
{
	m_pComboBoxSearchWord = NULL;
}

CDynamicHelpBar::~CDynamicHelpBar()
{
}


BEGIN_MESSAGE_MAP(CDynamicHelpBar, CExplorerBar)
	//{{AFX_MSG_MAP(CDynamicHelpBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_HELP_HOME, OnHelpHome)
	ON_COMMAND(ID_HELP_SEARCH, OnHelpSearch)
	ON_UPDATE_COMMAND_UI(ID_HELP_SEARCH, OnUpdateHelpSearch)
	//}}AFX_MSG_MAP
	ON_XTP_CREATECONTROL()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDynamicHelpBar message handlers

int CDynamicHelpBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CExplorerBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	get_dll_resource();
	VERIFY(m_wndToolBar.CreateToolBar(WS_TABSTOP|WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS, this));
	VERIFY(m_wndToolBar.LoadToolBar(IDR_DYNAMIC_HELP));
	reset_dll_resource();

	return 0;
}

void CDynamicHelpBar::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	int nTop = 0;

	if (m_wndToolBar.GetSafeHwnd())
	{
		CSize sz = m_wndToolBar.CalcDockingLayout(cx, /*LM_HIDEWRAP|*/ LM_HORZDOCK|LM_HORZ | LM_COMMIT);

		m_wndToolBar.MoveWindow(0, nTop, cx, sz.cy);
		m_wndToolBar.Invalidate(FALSE);
		nTop += sz.cy;
	}
	if (m_pIECtrl)
	{
		m_pIECtrl->MoveWindow(0, nTop, cx, cy - nTop);
		//m_wndPropertyGrid.Invalidate(FALSE);
	}
}

int CDynamicHelpBar::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	if (lpCreateControl->bToolBar)
	{
		CXTPToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTPToolBar, lpCreateControl->pCommandBar);
		if (!pToolBar)
			return FALSE;

		if (lpCreateControl->nID == ID_FIND_INPUT && pToolBar->GetBarID() == IDR_DYNAMIC_HELP)
		{
			CComboBoxWord* pComboSearchWord = new CComboBoxWord();
			pComboSearchWord->UpdateComboBox();
			pComboSearchWord->SetWidth(150);
			pComboSearchWord->SetDropDownListStyle();
			pComboSearchWord->SetCurSel(0);
			//pComboSearchWord->EnableShellAutoComplete();

			m_pComboBoxSearchWord = pComboSearchWord;
		
			lpCreateControl->pControl = pComboSearchWord;
			return TRUE;
		}
	}

	return FALSE;
}

// 显示帮助首页
void CDynamicHelpBar::OnHelpHome() 
{
	if(theApp.m_pIPlatUI == NULL)
	{
		return;
	}

	CString strScript = "";
	CString strDefaultValue;
	theApp.m_pIPlatUI->GetVciParameter("org.owm.htmlview", "DynHelpPage", strScript, strDefaultValue);
	strScript.MakeUpper();
	theApp.ProcessPath(strScript);

	if(m_pIECtrl)
	{
		m_pIECtrl->Navigate2(strScript);
	}
}

#define OC_WORD_HELP			140	// 单词帮助

// 查找
void CDynamicHelpBar::OnHelpSearch() 
{
	if(theApp.m_pIPlatUI == NULL)
	{
		return;
	}

	CComboBoxWord* pCombo = (CComboBoxWord*)m_pComboBoxSearchWord;
	ASSERT(pCombo->GetType() == xtpControlComboBox);

	CString strWord = pCombo->GetEditText();
	
	strWord.TrimLeft();
	strWord.TrimRight();
	
	if (strWord.IsEmpty())
		return;

	pCombo->SaveWordItem();

	// 调用编辑器的查找功能
	theApp.m_pIPlatUI->SendOwmCmd("org.owm.edit", OC_WORD_HELP, (WPARAM)(LPCTSTR)strWord, NULL, NULL);
}

void CDynamicHelpBar::OnUpdateHelpSearch(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}
