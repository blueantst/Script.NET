// BlueUIDoc.cpp : implementation of the CBlueUIDoc class
//

#include "stdafx.h"
#include "BlueUI.h"

#include "BlueUIDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlueUIDoc

IMPLEMENT_DYNCREATE(CBlueUIDoc, CDocument)

BEGIN_MESSAGE_MAP(CBlueUIDoc, CDocument)
	//{{AFX_MSG_MAP(CBlueUIDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlueUIDoc construction/destruction

CBlueUIDoc::CBlueUIDoc()
{
	// TODO: add one-time construction code here

}

CBlueUIDoc::~CBlueUIDoc()
{
}

BOOL CBlueUIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CBlueUIDoc serialization

void CBlueUIDoc::Serialize(CArchive& ar)
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
// CBlueUIDoc diagnostics

#ifdef _DEBUG
void CBlueUIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBlueUIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBlueUIDoc commands
