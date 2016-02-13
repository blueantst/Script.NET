// DuiVisionDesignerDoc.cpp : implementation file
//

#include "stdafx.h"
#include "DuiVisionDesigner.h"
#include "DuiVisionDesignerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDuiVisionDesignerDoc

IMPLEMENT_DYNCREATE(CDuiVisionDesignerDoc, CDocument)

CDuiVisionDesignerDoc::CDuiVisionDesignerDoc()
{
	m_pMenu = NULL;
}

BOOL CDuiVisionDesignerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CDuiVisionDesignerDoc::~CDuiVisionDesignerDoc()
{
}


BEGIN_MESSAGE_MAP(CDuiVisionDesignerDoc, CDocument)
	//{{AFX_MSG_MAP(CDuiVisionDesignerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDuiVisionDesignerDoc diagnostics

#ifdef _DEBUG
void CDuiVisionDesignerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDuiVisionDesignerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDuiVisionDesignerDoc serialization

void CDuiVisionDesignerDoc::Serialize(CArchive& ar)
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
// CDuiVisionDesignerDoc commands
static int nDocCount = 0;
void CDuiVisionDesignerDoc::SetTitle(LPCTSTR lpszTitle) 
{
	//CString strTitle;
	//strTitle.Format("DuiVisionDesigner OWM:%d", nDocCount++);
	
	CDocument::SetTitle(lpszTitle);
}

void CDuiVisionDesignerDoc::OnCloseDocument() 
{
	if(m_pMenu)
	{
		delete m_pMenu;
		m_pMenu = NULL;
	}
	
	CDocument::OnCloseDocument();
}

// 刷新属性树
void CDuiVisionDesignerDoc::RefreshDocProperty()
{
	if(!theApp.m_pIPlatUI || !theApp.m_pIPlatUI->EnableUpdatePropertyGridContent())
	{
		return;
	}

	// 更新属性树
	theApp.m_pIPlatUI->UpdatePropertyGridContent();
}
