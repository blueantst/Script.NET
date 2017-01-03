// WorkSpaceDoc.cpp : implementation file
//

#include "stdafx.h"
#include "WorkSpace.h"
#include "WorkSpaceDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceDoc

IMPLEMENT_DYNCREATE(CWorkSpaceDoc, CDocument)

CWorkSpaceDoc::CWorkSpaceDoc()
{
	m_pMenu = NULL;
}

BOOL CWorkSpaceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CWorkSpaceDoc::~CWorkSpaceDoc()
{
}


BEGIN_MESSAGE_MAP(CWorkSpaceDoc, CDocument)
	//{{AFX_MSG_MAP(CWorkSpaceDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceDoc diagnostics

#ifdef _DEBUG
void CWorkSpaceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWorkSpaceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceDoc serialization

void CWorkSpaceDoc::Serialize(CArchive& ar)
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
// CWorkSpaceDoc commands
static int nDocCount = 0;
void CWorkSpaceDoc::SetTitle(LPCTSTR lpszTitle) 
{
	CString strTitle;
	strTitle.Format("WorkSpace OWM:%d", nDocCount++);
	
	CDocument::SetTitle(strTitle);
}

void CWorkSpaceDoc::OnCloseDocument() 
{
	if(m_pMenu)
	{
		delete m_pMenu;
		m_pMenu = NULL;
	}
	
	CDocument::OnCloseDocument();
}

BOOL CWorkSpaceDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	return FALSE;
	
	//return CDocument::OnSaveDocument(lpszPathName);
}

BOOL CWorkSpaceDoc::DoFileSave()
{
	return FALSE;
}

BOOL CWorkSpaceDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{
	return FALSE;
}

BOOL CWorkSpaceDoc::SaveModified() 
{
	return FALSE;
	
	//return CDocument::SaveModified();
}
