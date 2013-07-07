// HtmlViewOWMDoc.cpp : implementation file
//

#include "stdafx.h"
#include "HtmlViewOWM.h"
#include "HtmlViewOWMDoc.h"
#include "HtmlViewOWMView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHtmlViewOWMDoc

IMPLEMENT_DYNCREATE(CHtmlViewOWMDoc, CDocument)

CHtmlViewOWMDoc::CHtmlViewOWMDoc()
{
	m_pMenu = NULL;
	m_strCurURL = "";
}

BOOL CHtmlViewOWMDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CHtmlViewOWMDoc::~CHtmlViewOWMDoc()
{
}


BEGIN_MESSAGE_MAP(CHtmlViewOWMDoc, CDocument)
	//{{AFX_MSG_MAP(CHtmlViewOWMDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHtmlViewOWMDoc diagnostics

#ifdef _DEBUG
void CHtmlViewOWMDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHtmlViewOWMDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHtmlViewOWMDoc serialization

void CHtmlViewOWMDoc::Serialize(CArchive& ar)
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
// CHtmlViewOWMDoc commands
static int nDocCount = 0;
void CHtmlViewOWMDoc::SetTitle(LPCTSTR lpszTitle) 
{
//	CString strTitle;
//	strTitle.Format("HtmlViewOWM OWM:%d", nDocCount++);
	
	CDocument::SetTitle(lpszTitle);
}

void CHtmlViewOWMDoc::OnCloseDocument() 
{
	if(m_pMenu)
	{
		delete m_pMenu;
		m_pMenu = NULL;
	}
	
	CDocument::OnCloseDocument();
}

BOOL CHtmlViewOWMDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	//if (!CDocument::OnOpenDocument(lpszPathName))
	//	return FALSE;
	
	CString strURL = lpszPathName;

	CString strPath = lpszPathName;
	int posPot = strPath.ReverseFind('.');
	if(posPot > 0)
	{
		CString strExt = strPath.Right(strPath.GetLength() - posPot -1);
		strExt.MakeUpper();
		if(strExt == "BLP")
		{
			strURL = "tcl:<file>";
			strURL += lpszPathName;
		}
	}

	POSITION pos = GetFirstViewPosition();
	CHtmlViewOWMView* pView = (CHtmlViewOWMView*)GetNextView(pos);

	pView->Navigate2(strURL);

	return TRUE;
}

// 刷新属性树
void CHtmlViewOWMDoc::RefreshDocProperty()
{
	if(!theApp.m_pIPlatUI || !theApp.m_pIPlatUI->EnableUpdatePropertyGridContent())
	{
		return;
	}

	// 更新属性树
	theApp.m_pIPlatUI->UpdatePropertyGridContent();
}