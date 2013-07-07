// PTerminalDoc.cpp : implementation file
//

#include "stdafx.h"
#include "PTerminal.h"
#include "PTerminalDoc.h"
#include "PTerminalView.h"
#include "ChildFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LOADRCSTRING(str,strID) \
	get_dll_resource();\
	str.LoadString(strID);\
	reset_dll_resource();

void get_dll_resource(void);
void reset_dll_resource(void);

/////////////////////////////////////////////////////////////////////////////
// CPTerminalDoc

IMPLEMENT_DYNCREATE(CPTerminalDoc, CDocument)

CPTerminalDoc::CPTerminalDoc()
{
}

BOOL CPTerminalDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	CPTerminalView* pView = ((CPTerminalView*)m_viewList.GetHead());
	pView->m_bHaveDocument = TRUE;

	return TRUE;
}

CPTerminalDoc::~CPTerminalDoc()
{
}


BEGIN_MESSAGE_MAP(CPTerminalDoc, CDocument)
	//{{AFX_MSG_MAP(CPTerminalDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTerminalDoc diagnostics

#ifdef _DEBUG
void CPTerminalDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPTerminalDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTerminalDoc serialization

void CPTerminalDoc::Serialize(CArchive& ar)
{
	((CRichEditView*)m_viewList.GetHead())->Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CPTerminalDoc commands
static int nDocCount = 0;
void CPTerminalDoc::SetTitle(LPCTSTR lpszTitle) 
{
	//CDocument::SetTitle(_T("控制台"));
	CPTerminalView* pView = ((CPTerminalView*)m_viewList.GetHead());
	CString strTitle;
	LOADRCSTRING(strTitle, IDS_STR_CONSOLE)	
	switch(pView->m_nWorkShell)
	{
	case SHELL_TCL:
		LOADRCSTRING(strTitle, IDS_TERM_TCL)
		break;
	case SHELL_PYTHON:
		LOADRCSTRING(strTitle, IDS_TERM_PYTHON)
		break;
	case SHELL_RUBY:
		LOADRCSTRING(strTitle, IDS_TERM_RUBY)
		break;
	case SHELL_LUA:
		LOADRCSTRING(strTitle, IDS_TERM_LUA)
		break;
	case SHELL_PERL:
		LOADRCSTRING(strTitle, IDS_TERM_PERL)
		break;
	case SHELL_TELNET:
		LOADRCSTRING(strTitle, IDS_TERM_TELNET)	
		break;
	case SHELL_COM:
		LOADRCSTRING(strTitle, IDS_TERM_COM)	
		break;
	}
	CDocument::SetTitle(strTitle);
}

void CPTerminalDoc::OnCloseDocument() 
{
	CDocument::OnCloseDocument();
}

void CPTerminalDoc::RefreshDocProperty()
{	
	if(!theApp.m_pIPlatUI || !theApp.m_pIPlatUI->EnableUpdatePropertyGridContent())
	{
		return;
	}

	// 开始属性页更新
	theApp.m_pIPlatUI->BeginUpdatePropertyGridContent();


	// 结束属性页更新
	theApp.m_pIPlatUI->EndUpdatePropertyGridContent();
}
