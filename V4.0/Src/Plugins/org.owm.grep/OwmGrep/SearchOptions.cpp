// SearchOptions.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OwmGrep.h"
#include "SearchOptions.h"
#include "OwmGrepView.h"
#include "SearchThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CSearchBrowseButton::Initialize(CComboBox* pParentWnd, DWORD dwBStyle, UINT nMenu, int nSubMenuIndx)
{
	m_nMenu = nMenu;
	m_nSubMenuIndx = nSubMenuIndx;
	m_dwBStyle = dwBStyle;


	m_pComboBox = pParentWnd;
}

void CSearchBrowseButton::OnClicked()
{
	static BOOL m_bExecute = FALSE;
	if (m_bExecute)
		return;
	m_bExecute = TRUE;

	if (m_dwBStyle == BES_XT_CHOOSEDIR)
	{
		CString strPath;
		m_pComboBox->GetWindowText(strPath);

		// Instantiate a browse for folder dialog object
		CXTBrowseDialog browseDlg;
		browseDlg.SetOwner(GetParent()->GetSafeHwnd());
		browseDlg.SetSelPath((TCHAR*)(LPCTSTR)strPath);

		if( browseDlg.DoModal() == IDOK ) {
			m_pComboBox->SetWindowText( browseDlg.GetSelPath() );
			CSearchOptionsView* pWnd = (CSearchOptionsView*)(theApp.m_pIPlatUI->GetDockingPaneWnd(VIEW_ID_GREP));
			if(pWnd)
			{
				CSearchOptionsView* pView = (CSearchOptionsView*)(pWnd->GetDescendantWindow(AFX_IDW_PANE_FIRST));
				if(pView)
				{
					pView->UpdateOptions();
				}
			}
		}

		m_bExecute = FALSE;
		return;
	}

	SetState(TRUE);

	CRect rect;
	GetWindowRect(&rect);

	get_dll_resource();
	// loading a user resource.
	CMenu menu;
	menu.LoadMenu(m_nMenu);
	reset_dll_resource();

	CMenu* pPopup = menu.GetSubMenu(m_nSubMenuIndx);
	ASSERT(pPopup != NULL);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	int nCmd = XTFuncContextMenu(pPopup, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL |TPM_RETURNCMD,
		rect.right, rect.top, pWndPopupOwner, 0 );

	if (nCmd > 0)
	{
		get_dll_resource();
		CString str;
		str.LoadString(nCmd);
		reset_dll_resource();

		CString strPrev;
		m_pComboBox->GetWindowText(strPrev);
		m_pComboBox->SetWindowText(strPrev + str);
	}

	// Return the button state to normal.
	SetState(FALSE);

	m_bExecute = FALSE;
};
/////////////////////////////////////////////////////////////////////////////
// CSearchOptionsView

IMPLEMENT_DYNCREATE(CSearchOptionsView, CXTResizeFormView)

CSearchOptions::CSearchOptions()
{
	m_bIncludeSubFolders = TRUE;
	m_bRegularExpressions = TRUE;
	m_bMatchCase = FALSE;
	m_bMatchWholeWords = FALSE;

	m_strFind = _T("");
	m_strReplace = _T("");
	m_strPath = _T("");
	m_strFileTypes = _T("*.*");

	pRegExp = 0;
}

void ExchangeList(CXTPPropExchange* pPX,  LPCTSTR pszPropName, CStringList& lst)
{
	CXTPPropExchangeSection secItems(pPX->GetSection(pszPropName));
	CString str, strSection;

	if (pPX->IsStoring())
	{
		DWORD dwCount = (DWORD)lst.GetCount();
		secItems->WriteCount(dwCount);
		
		POSITION pos = lst.GetHeadPosition();
		int i = 0;
		while (pos)
		{
			str = lst.GetNext(pos);				
			strSection.Format(_T("Item%i"), i++);
			PX_String(&secItems, strSection, str, _T(""));			
		}	
	}
	else
	{
		DWORD nNewCount = secItems->ReadCount();
		for (DWORD i = 0; i < nNewCount; i++)
		{
			strSection.Format(_T("Item%i"), i);

			PX_String(&secItems, strSection, str, _T(""));
			lst.AddTail(str);
		}
	}
}

void CSearchOptions::DoPropExchange(CXTPPropExchange* pPX)
{
	pPX->ExchangeSchema();

	PX_Bool(pPX, _T("MatchCase"), m_bMatchCase, FALSE);
	PX_Bool(pPX, _T("MatchWholeWords"), m_bMatchWholeWords, FALSE);
	PX_Bool(pPX, _T("RegularExpressions"), m_bRegularExpressions, TRUE);
	PX_Bool(pPX, _T("IncludeSubFolders"), m_bIncludeSubFolders, TRUE);

	PX_String(pPX, _T("FindString"), m_strFind, _T(""));
	PX_String(pPX, _T("ReplaceString"), m_strReplace, _T(""));
	PX_String(pPX, _T("Path"), m_strPath, _T(""));
	PX_String(pPX, _T("FileTypes"), m_strFileTypes, _T("*.*"));

	ExchangeList(pPX, _T("FileTypesHistory"),m_lstFileTypesHistory);
	ExchangeList(pPX, _T("ReplaceHistory"),m_lstReplaceHistory);
	ExchangeList(pPX, _T("FindHistory"),m_lstFindHistory);
	if (pPX->IsStoring())
	{
		POSITION pos = m_lstPathHistory.Find(LOC_CURRENTDOC);
		m_lstPathHistory.RemoveAt(pos);
		pos = m_lstPathHistory.Find(LOC_OPENDOCS);
		m_lstPathHistory.RemoveAt(pos);
		pos = m_lstPathHistory.Find(LOC_CURRENTPRJ);
		m_lstPathHistory.RemoveAt(pos);
		ExchangeList(pPX, _T("PathHistory"),m_lstPathHistory);
	}else
	{
		m_lstPathHistory.AddTail(LOC_CURRENTDOC);
		m_lstPathHistory.AddTail(LOC_OPENDOCS);
		m_lstPathHistory.AddTail(LOC_CURRENTPRJ);
		ExchangeList(pPX, _T("PathHistory"),m_lstPathHistory);
	}
}

CSearchOptions* GetSearchOptions()
{
	static CSearchOptions options;
	return &options;
}

CSearchOptionsView::CSearchOptionsView()
	: CXTResizeFormView(CSearchOptionsView::IDD)
{
	//{{AFX_DATA_INIT(CSearchOptionsView)
	//}}AFX_DATA_INIT
	m_bSearchMode = FALSE;
	m_bInitialized = FALSE;

	m_pSearchThread = NULL;
	m_searchResult.nMatchingLines = 0;
	m_searchResult.nMatchingFiles = 0;
	m_searchResult.nTotalFiles = 0;
}

CSearchOptionsView::~CSearchOptionsView()
{
	// 保存配置文件
	CXTPPropExchangeXMLNode px(FALSE, 0, _T("Settings"));
	GetSearchOptions()->DoPropExchange(&px);
	CString strIniFileName = theApp.GetModulePath();
	strIniFileName += "\\GrepSetting.xml";
	px.SaveToFile(strIniFileName);
}

void CSearchOptionsView::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchOptionsView)
	DDX_Control(pDX, IDC_COMBO_REPLACE, m_cmbReplace);
	DDX_Control(pDX, IDC_COMBO_PATH, m_cmbPath);
	DDX_Control(pDX, IDC_COMBO_FIND, m_cmbFind);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_cmbFileTypes);
	DDX_Control(pDX, IDC_BUTTON_REPLACE_EXPAND, m_btnReplaceExpand);
	DDX_Control(pDX, IDC_BUTTON_FIND_EXPAND, m_btnFindExpand);
	DDX_Control(pDX, IDC_BUTTON_PATH_EXPAND, m_btnPathExpand);
	DDX_Control(pDX, IDC_GROUP_FINDOPTIONS, m_wndGroupFindOptions);
	//}}AFX_DATA_MAP
	DDX_CBString(pDX, IDC_COMBO_FIND, GetSearchOptions()->m_strFind);
	DDX_CBString(pDX, IDC_COMBO_REPLACE, GetSearchOptions()->m_strReplace);
	DDX_CBString(pDX, IDC_COMBO_PATH, GetSearchOptions()->m_strPath);
	DDX_Check(pDX, IDC_CHECK_INCLUDE_SUBFOLDERS, GetSearchOptions()->m_bIncludeSubFolders);
	DDX_Check(pDX, IDC_CHECK_REGULAR, GetSearchOptions()->m_bRegularExpressions);
	DDX_Check(pDX, IDC_CHECK_MATCH_CASE, GetSearchOptions()->m_bMatchCase);
	DDX_Check(pDX, IDC_CHECK_MATCH_WHOLE_WORDS, GetSearchOptions()->m_bMatchWholeWords);
	DDX_CBString(pDX, IDC_COMBO_TYPE, GetSearchOptions()->m_strFileTypes);
}


BEGIN_MESSAGE_MAP(CSearchOptionsView, CXTResizeFormView)
	//{{AFX_MSG_MAP(CSearchOptionsView)
	ON_BN_CLICKED(IDC_BUTTON_FINDALL, OnButtonFindAll)
	ON_BN_CLICKED(IDC_BUTTON_REPLACEALL, OnButtonReplaceAll)
	ON_CBN_EDITCHANGE(IDC_COMBO_FIND, UpdateOptions)
	ON_CBN_EDITCHANGE(IDC_COMBO_PATH, UpdateOptions)
	ON_CBN_EDITCHANGE(IDC_COMBO_REPLACE, UpdateOptions)
	ON_CBN_SELCHANGE(IDC_COMBO_FIND, UpdateOptions)
	ON_CBN_SELCHANGE(IDC_COMBO_PATH, UpdateOptions)
	ON_CBN_SELCHANGE(IDC_COMBO_REPLACE, UpdateOptions)
	ON_BN_CLICKED(IDC_CHECK_REGULAR, UpdateOptions)
	//}}AFX_MSG_MAP
	ON_MESSAGE_VOID(WM_SEARCHFINISHED, OnFindDone)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchOptionsView diagnostics

#ifdef _DEBUG
void CSearchOptionsView::AssertValid() const
{
	CXTResizeFormView::AssertValid();
}

void CSearchOptionsView::Dump(CDumpContext& dc) const
{
	CXTResizeFormView::Dump(dc);
}
#endif //_DEBUG


void RestoryCombo(CComboBox& cmb, CStringList& lstHistory, LPCTSTR lpszDefault = 0)
{
	if (lstHistory.GetCount() > 0)
	{
		POSITION pos = lstHistory.GetHeadPosition();
		while (pos)
		{
			cmb.AddString(lstHistory.GetNext(pos));
		}
	}
	else if (lpszDefault)
	{
		cmb.AddString(lpszDefault);
		lstHistory.AddTail(lpszDefault);
	}

}
/////////////////////////////////////////////////////////////////////////////
// CSearchOptionsView message handlers

void CSearchOptionsView::OnInitialUpdate()
{
	CXTResizeFormView::OnInitialUpdate();

	SetScaleToFitSize(CSize(90, 1));

	if (m_bInitialized)
		return;

	SetResize(IDC_COMBO_FIND, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_FIND_EXPAND, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_COMBO_REPLACE, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_REPLACE_EXPAND, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_COMBO_PATH, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_PATH_EXPAND, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_COMBO_TYPE, SZ_TOP_LEFT, SZ_TOP_RIGHT);

	SetResize(IDC_GROUP_FINDOPTIONS, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_FINDALL, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_REPLACEALL, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	m_bInitialized = TRUE;

	m_btnFindExpand.SetBrowseStyle(BES_XT_POPUPMENU);
	m_btnFindExpand.Initialize(&m_cmbFind, BES_XT_POPUPMENU, IDR_SEARCH_POPUP, 0);

	m_btnReplaceExpand.SetBrowseStyle(BES_XT_POPUPMENU);
	m_btnReplaceExpand.Initialize(&m_cmbReplace, BES_XT_POPUPMENU, IDR_SEARCH_POPUP, 1);

	m_btnPathExpand.Initialize(&m_cmbPath, BES_XT_CHOOSEDIR);

	reset_dll_resource();

	UpdateOptions();

	get_dll_resource();

	IRegExpPtr regExp;
	HRESULT hr = regExp.CreateInstance(__uuidof(RegExp));
	if (FAILED(hr))
	{
		GetDlgItem(IDC_CHECK_REGULAR)->EnableWindow(FALSE);
	}

	// 初始化配置文件
	CXTPPropExchangeXMLNode px(TRUE, 0, _T("Settings"));
	CString strIniFileName = theApp.GetModulePath();
	strIniFileName += "\\GrepSetting.xml";
	if (px.LoadFromFile(strIniFileName))
	{
		GetSearchOptions()->DoPropExchange(&px);
	}

	RestoryCombo(m_cmbFileTypes, GetSearchOptions()->m_lstFileTypesHistory, _T("*.tcl;*.tk"));
	RestoryCombo(m_cmbFind, GetSearchOptions()->m_lstFindHistory);
	RestoryCombo(m_cmbReplace, GetSearchOptions()->m_lstReplaceHistory);
	RestoryCombo(m_cmbPath, GetSearchOptions()->m_lstPathHistory);

	// 判断状态，决定按钮的初始状态
	CSearchOptions* pOptions = GetSearchOptions();
	GetDlgItem(IDC_BUTTON_FINDALL)->EnableWindow(!pOptions->m_strFind.IsEmpty() && !pOptions->m_strPath.IsEmpty());
	GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(CheckReplaceEnable());

	GetDlgItem(IDC_BUTTON_FIND_EXPAND)->EnableWindow(pOptions->m_bRegularExpressions);
	GetDlgItem(IDC_BUTTON_REPLACE_EXPAND)->EnableWindow(pOptions->m_bRegularExpressions);

	reset_dll_resource();
}

BOOL CSearchOptionsView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	return CXTResizeFormView::PreCreateWindow(cs);
}

void CSearchOptionsView::OnButtonFindAll()
{
	UpdateData();

	if(theApp.m_pIPlatUI == NULL)
	{
		return;
	}

	theApp.m_pIPlatUI->CreateDockingPane("org.owm.grep", VIEW_ID_SEARCH_RESULT);
	theApp.m_pSearchResultView = theApp.m_pIPlatUI->GetDockingPaneWnd(VIEW_ID_SEARCH_RESULT);

	if (m_bSearchMode)
	{
		OnCancelFind();
	}
	else
	{
		OnFindAll();
	}
}

BOOL CSearchOptionsView::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
	{
		if ((::GetFocus() == m_hWnd) || (::GetParent(::GetFocus()) == m_hWnd) || (::GetParent(::GetParent(::GetFocus())) == m_hWnd))
		{
			if (IsDialogMessage(pMsg))
				return TRUE;
		}
	}

	return CXTResizeFormView::PreTranslateMessage(pMsg);
}

// 刷新状态
void CSearchOptionsView::UpdateOptions()
{
	UpdateData();

	get_dll_resource();

	CSearchOptions* pOptions = GetSearchOptions();
	GetDlgItem(IDC_BUTTON_FINDALL)->EnableWindow(!pOptions->m_strFind.IsEmpty() && !pOptions->m_strPath.IsEmpty());
	GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(CheckReplaceEnable());

	GetDlgItem(IDC_BUTTON_FIND_EXPAND)->EnableWindow(pOptions->m_bRegularExpressions);
	GetDlgItem(IDC_BUTTON_REPLACE_EXPAND)->EnableWindow(pOptions->m_bRegularExpressions);

	reset_dll_resource();
}

// 设置查找模式(是否正在查找)
void CSearchOptionsView::SetSearchMode(BOOL bSearchMode)
{
	CWnd* pWnd = GetWindow(GW_CHILD);
	while (pWnd)
	{
		pWnd->EnableWindow(!bSearchMode);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
	get_dll_resource();
	GetDlgItem(IDC_BUTTON_FINDALL)->EnableWindow(TRUE);
	CString strFindAll, strCancel;
	strFindAll.LoadString(IDS_FINDALL);
	strCancel.LoadString(IDS_CANCEL);
	reset_dll_resource();
	GetDlgItem(IDC_BUTTON_FINDALL)->SetWindowText(bSearchMode ? strCancel : strFindAll);

	if (!bSearchMode)
	{
		UpdateOptions();
	}

	RedrawWindow(0, 0, RDW_ALLCHILDREN|RDW_INVALIDATE|RDW_UPDATENOW);

	m_bSearchMode = bSearchMode;
}

// 检查替换状态是否可用
BOOL CSearchOptionsView::CheckReplaceEnable()
{
	CSearchOptions* pOptions = GetSearchOptions();
	if(pOptions->m_strFind.IsEmpty())
		return FALSE;
	if(pOptions->m_strPath != LOC_CURRENTDOC)
		return FALSE;
	return TRUE;
}

void CSearchOptionsView::OnButtonReplaceAll()
{
	OnReplaceAll();
}

#define MAX_HISTORY_COUNT	20
// 添加查找历史
void AddComboHistory(CComboBox& cmb, CString strText, CStringList& lstHistory)
{
	if (strText.IsEmpty())
		return;
	
	int nIndex = cmb.FindString(-1, strText);
	if (nIndex == -1)
	{
		cmb.InsertString(0, strText);
		while(cmb.GetCount() > MAX_HISTORY_COUNT)
		{
			cmb.DeleteString(cmb.GetCount()-1);
		}
		lstHistory.AddHead(strText);
		while(lstHistory.GetCount() > MAX_HISTORY_COUNT)
		{
			lstHistory.RemoveTail();
		}
	}
}

// 查找结束
void CSearchOptionsView::OnFindDone()
{
	if(m_pSearchThread)
	{
		m_searchResult.nMatchingLines = m_pSearchThread->m_searchResult.nMatchingLines;
		m_searchResult.nMatchingFiles = m_pSearchThread->m_searchResult.nMatchingFiles;
		m_searchResult.nTotalFiles = m_pSearchThread->m_searchResult.nTotalFiles;
	}

	m_pSearchThread = NULL;

	SetSearchMode(FALSE);

	CXTPReportView* pView = NULL;
	if(theApp.m_pSearchResultView)
	{
		pView = DYNAMIC_DOWNCAST(CXTPReportView, theApp.m_pSearchResultView->GetDescendantWindow(AFX_IDW_PANE_FIRST));
	}
	ASSERT(pView);

	if (!pView)
		return;

	GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(pView->GetReportCtrl().GetRecords()->GetCount() > 0);

	pView->GetReportCtrl().Populate();
}

// 查找中止
void CSearchOptionsView::OnCancelFind()
{
	ASSERT(m_pSearchThread != NULL);
	
	m_pSearchThread->m_bCancel = TRUE;

	m_pSearchThread = NULL;

	get_dll_resource();
	GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(CheckReplaceEnable());
	reset_dll_resource();

	SetSearchMode(FALSE);

	CXTPReportView* pView = NULL;
	if(theApp.m_pSearchResultView)
	{
		pView = DYNAMIC_DOWNCAST(CXTPReportView, theApp.m_pSearchResultView->GetDescendantWindow(AFX_IDW_PANE_FIRST));
	}
	ASSERT(pView);

	if (!pView)
		return;

	pView->GetReportCtrl().GetRecords()->RemoveAll();

	((CFrameWnd*)(AfxGetMainWnd()))->SetMessageText(_T("Canceled"));
}

// 开始查找
void CSearchOptionsView::OnFindAll()
{
	CSearchOptions* pOptions = GetSearchOptions();
	ASSERT(pOptions);

	CXTPReportView* pView = NULL;
	if(theApp.m_pSearchResultView)
	{
		pView = DYNAMIC_DOWNCAST(CXTPReportView, theApp.m_pSearchResultView->GetDescendantWindow(AFX_IDW_PANE_FIRST));
	}
	ASSERT(pView);

	if (!pView)
		return;

	m_searchResult.nMatchingLines = 0;
	m_searchResult.nMatchingFiles = 0;
	m_searchResult.nTotalFiles = 0;

	pView->GetReportCtrl().GetRecords()->RemoveAll();
	pView->GetReportCtrl().Populate();

	//////////////////////////////////////////////////////////////////////////

	if(pOptions->m_strPath == LOC_CURRENTDOC)
	{
		if(theApp.m_pIPlatUI)
		{
			theApp.m_pIPlatUI->SendOwmCmd("org.owm.edit", OC_EDIT_FIND, 0, 0, NULL);
		}
	}else
	{
		m_pSearchThread = (CSearchThread*)AfxBeginThread(RUNTIME_CLASS(CSearchThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);	
		m_pSearchThread->m_pSearchOptionView = this;
		m_pSearchThread->m_pReportControl = &pView->GetReportCtrl();

		m_pSearchThread->ResumeThread();

		SetSearchMode(TRUE);
	}

	AddComboHistory(m_cmbFind, pOptions->m_strFind, pOptions->m_lstFindHistory);
	AddComboHistory(m_cmbReplace, pOptions->m_strReplace, pOptions->m_lstReplaceHistory);
	AddComboHistory(m_cmbPath, pOptions->m_strPath, pOptions->m_lstPathHistory);
	AddComboHistory(m_cmbFileTypes, pOptions->m_strFileTypes, pOptions->m_lstFileTypesHistory);

	// 关闭查找/替换窗口
	if(theApp.m_pIPlatUI)
	{
		theApp.m_pIPlatUI->ActiveDockingPane(10002, FALSE);
	}
}

// 替换所有
void CSearchOptionsView::OnReplaceAll()
{
	USES_CONVERSION;

	CSearchOptions* pOptions = GetSearchOptions();
	ASSERT(pOptions);

	if(pOptions->m_strPath == LOC_CURRENTDOC)
	{
		if(theApp.m_pIPlatUI)
		{
			theApp.m_pIPlatUI->SendOwmCmd("org.owm.edit", OC_EDIT_REPLACE, 0, 0, NULL);
		}
		return;
	}

	CXTPReportView* pView = NULL;
	if(theApp.m_pSearchResultView)
	{
		pView = DYNAMIC_DOWNCAST(CXTPReportView, theApp.m_pSearchResultView->GetDescendantWindow(AFX_IDW_PANE_FIRST));
	}
	ASSERT(pView);

	if (!pView)
		return;

	CXTPReportRecords* pRecords = pView->GetReportCtrl().GetRecords();
	if (!pRecords)
		return;

	int nFiles = 0;
	int nChanges = 0;

	int i;
	for (i = 0; i < pRecords->GetCount(); i++)
	{
		CString strPath = ((CGrepRecord*)pRecords->GetAt(i))->m_strPath;

		BOOL bChanged = FALSE;
		int nSamePathCount = 0;

		while (i + nSamePathCount < pRecords->GetCount() && ((CGrepRecord*)pRecords->GetAt(i + nSamePathCount))->m_strPath ==
			strPath)
		{
			CGrepRecord* pRecord = ((CGrepRecord*)pRecords->GetAt(i + nSamePathCount));

			nSamePathCount++;
			if (pRecord->IsChanged() && pRecord->IsChecked())
			{
				bChanged = TRUE;
				nChanges++;
			}
		}

		if (!bChanged)
		{
			i += nSamePathCount - 1;
			continue;
		}
		i += nSamePathCount - 1;
		nFiles++;
	}

	if (nFiles == 0)
	{
		AfxMessageBox(_T("There are no selected records"));
		return;
	}

	CString str;
	str.Format(_T("Are you sure you want make %i replacement(s) in %i file(s)?"), nChanges, nFiles);
	if (AfxMessageBox(str, MB_YESNO) != IDYES)
		return;

	for (i = 0; i < pRecords->GetCount(); i++)
	{
		CString strPath = ((CGrepRecord*)pRecords->GetAt(i))->m_strPath;

		int nSamePathCount = 0;
		BOOL bChanged = FALSE;

		while (i + nSamePathCount < pRecords->GetCount() && ((CGrepRecord*)pRecords->GetAt(i + nSamePathCount))->m_strPath ==
			strPath)
		{
			CGrepRecord* pRecord = ((CGrepRecord*)pRecords->GetAt(i + nSamePathCount));

			nSamePathCount++;
			if (pRecord->IsChanged() && pRecord->IsChecked())
			{
				bChanged = TRUE;
			}
		}

		CFile file;
		if (!bChanged || !file.Open(strPath, CFile::modeRead))
		{
			i += nSamePathCount - 1;
			continue;
		}

		DWORD dwCount = (DWORD)file.GetLength();
		char* lpszFileBuffer = new char[dwCount];
		file.Read(lpszFileBuffer, dwCount);
		file.Close();

		//CFile::Rename(pRecord->m_strPath, pRecord->m_strPath + _T(".bak"));
		if (!file.Open(strPath, CFile::modeWrite|CFile::modeCreate))
		{
			i += nSamePathCount - 1;

			delete[] lpszFileBuffer;
			continue;
		}

		long nTail = 0;

		int k = 0;
		for  (int j = 0; j < nSamePathCount; j++)
		{
			CGrepRecord* pRecord = ((CGrepRecord*)pRecords->GetAt(i + j - k ));

			if (pRecord->IsChecked())
			{
				file.Write(lpszFileBuffer + nTail, pRecord->m_nIndex - nTail);

				if (!pRecord->m_strReplace.IsEmpty())
				{
					file.Write(T2A((LPTSTR)(LPCTSTR)(pRecord->m_strReplace)), pRecord->m_strReplace.GetLength());
				}
				nTail = pRecord->m_nIndex + pRecord->m_nLength;

				pRecords->RemoveAt(i + j - k);
				k++;
			}
		}
		i += nSamePathCount - k - 1;

		file.Write(lpszFileBuffer + nTail, dwCount - nTail);
		file.Close();

		delete[] lpszFileBuffer;
	}

	pView->GetReportCtrl().Populate();

	// 关闭查找/替换窗口
	if(theApp.m_pIPlatUI)
	{
		theApp.m_pIPlatUI->ActiveDockingPane(10002, FALSE);
	}
}
