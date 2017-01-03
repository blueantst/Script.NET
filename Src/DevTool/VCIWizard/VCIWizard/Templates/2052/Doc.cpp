// [!output PROJECT_NAME]Doc.cpp : implementation file
//

#include "stdafx.h"
#include "[!output PROJECT_NAME].h"
#include "[!output PROJECT_NAME]Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C[!output PROJECT_NAME]Doc

IMPLEMENT_DYNCREATE(C[!output PROJECT_NAME]Doc, CDocument)

C[!output PROJECT_NAME]Doc::C[!output PROJECT_NAME]Doc()
{
	m_pMenu = NULL;
}

BOOL C[!output PROJECT_NAME]Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

C[!output PROJECT_NAME]Doc::~C[!output PROJECT_NAME]Doc()
{
}


BEGIN_MESSAGE_MAP(C[!output PROJECT_NAME]Doc, CDocument)
	//{{AFX_MSG_MAP(C[!output PROJECT_NAME]Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C[!output PROJECT_NAME]Doc diagnostics

#ifdef _DEBUG
void C[!output PROJECT_NAME]Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void C[!output PROJECT_NAME]Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// C[!output PROJECT_NAME]Doc serialization

void C[!output PROJECT_NAME]Doc::Serialize(CArchive& ar)
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
// C[!output PROJECT_NAME]Doc commands
static int nDocCount = 0;
void C[!output PROJECT_NAME]Doc::SetTitle(LPCTSTR lpszTitle) 
{
	CString strTitle;
	strTitle.Format("[!output PROJECT_NAME] OWM:%d", nDocCount++);
	
	CDocument::SetTitle(strTitle);
}

void C[!output PROJECT_NAME]Doc::OnCloseDocument() 
{
	if(m_pMenu)
	{
		delete m_pMenu;
		m_pMenu = NULL;
	}
	
	CDocument::OnCloseDocument();
}

// 刷新属性树
void C[!output PROJECT_NAME]Doc::RefreshDocProperty()
{
	if(!theApp.m_pIPlatUI || !theApp.m_pIPlatUI->EnableUpdatePropertyGridContent())
	{
		return;
	}

	// 更新属性树
	theApp.m_pIPlatUI->UpdatePropertyGridContent();
}
