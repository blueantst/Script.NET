// OwmGrepDoc.cpp : implementation file
//

#include "stdafx.h"
#include "OwmGrep.h"
#include "OwmGrepDoc.h"
#include "OwmGrepView.h"
#include "ChildFrame.h"
#include "SearchThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COwmGrepDoc

IMPLEMENT_DYNCREATE(COwmGrepDoc, CDocument)

COwmGrepDoc::COwmGrepDoc()
{
	m_pMenu = NULL;
}

BOOL COwmGrepDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

COwmGrepDoc::~COwmGrepDoc()
{
}


BEGIN_MESSAGE_MAP(COwmGrepDoc, CDocument)
	//{{AFX_MSG_MAP(COwmGrepDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COwmGrepDoc diagnostics

#ifdef _DEBUG
void COwmGrepDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COwmGrepDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COwmGrepDoc serialization

void COwmGrepDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// COwmGrepDoc commands
//static int nDocCount = 0;
void COwmGrepDoc::SetTitle(LPCTSTR lpszTitle) 
{
	CString strTitle = "Grep";
	
	CDocument::SetTitle(strTitle);
}

void COwmGrepDoc::OnCloseDocument() 
{
	if(m_pMenu)
	{
		delete m_pMenu;
		m_pMenu = NULL;
	}
	
	CDocument::OnCloseDocument();
}

// 刷新属性树
void COwmGrepDoc::RefreshDocProperty()
{
	if(!theApp.m_pIPlatUI || !theApp.m_pIPlatUI->EnableUpdatePropertyGridContent())
	{
		return;
	}
/*
	COwmGrepView* pGrepView = GetFirstOwmGrepView();
	if(!pGrepView)
	{
		return;
	}

	// 开始属性页更新
	theApp.m_pIPlatUI->BeginUpdatePropertyGridContent();

	CXTPPropertyGrid* pPropertyGrid = (CXTPPropertyGrid*)(theApp.m_pIPlatUI->GetPropertyGridWnd());
	if(!pPropertyGrid->m_hWnd)
	{
		return;
	}

	get_dll_resource();
	CXTPPropertyGridItem* pCategoryFindReplace = pPropertyGrid->AddCategory(ID_PROP_FINDANDREPLACE);
	CXTPPropertyGridItem* pItem = NULL;
	CString strTmp;

	{
		pItem = pCategoryFindReplace->AddChildItem(
			new CXTPPropertyGridItemNumber(_T("Matching lines"),
				pFrame->m_searchResult.nMatchingLines));
		pItem->SetReadOnly(TRUE);

		pItem = pCategoryFindReplace->AddChildItem(
			new CXTPPropertyGridItemNumber(_T("Matching files"),
				pFrame->m_searchResult.nMatchingFiles));
		pItem->SetReadOnly(TRUE);

		pItem = pCategoryFindReplace->AddChildItem(
			new CXTPPropertyGridItemNumber(_T("Total files searched"),
				pFrame->m_searchResult.nTotalFiles));
		pItem->SetReadOnly(TRUE);
	}

	pCategoryFindReplace->Expand();
	reset_dll_resource();

	// 添加快捷链接
	//pPropertyGrid->GetVerbs()->Add(_T("属性"), ID_CXIMAGE_OPTIONS);
*/
	// 结束属性页更新
	theApp.m_pIPlatUI->EndUpdatePropertyGridContent();
}