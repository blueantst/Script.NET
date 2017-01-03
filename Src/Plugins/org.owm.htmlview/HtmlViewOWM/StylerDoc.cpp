// StylerDoc.cpp : implementation of the CStylerDoc class
//

#include "stdafx.h"
#include "HtmlViewOWM.h"

#include "StylerDoc.h"
#include "StylerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStylerDoc

IMPLEMENT_DYNCREATE(CStylerDoc, CDocument)

BEGIN_MESSAGE_MAP(CStylerDoc, CDocument)
END_MESSAGE_MAP()


// CStylerDoc construction/destruction

CStylerDoc::CStylerDoc()
{
	m_strUrl = "";
}

CStylerDoc::~CStylerDoc()
{
}

BOOL CStylerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_strUrl = "";

	return TRUE;
}




// CStylerDoc serialization

void CStylerDoc::Serialize(CArchive& ar)
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


// CStylerDoc diagnostics

#ifdef _DEBUG
void CStylerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStylerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CStylerDoc commands

BOOL CStylerDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	POSITION pos = GetFirstViewPosition();
	CStylerView* pView = (CStylerView*)GetNextView(pos);
	if (pView)
	{
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
			}else
			{
				strURL = CString(_T("file://")) + lpszPathName;
			}
		}

		try{
		pView->Navigate2(strURL);
		}catch(...){
		}
		
		// Internet Explorer bug 
		if (strURL.Right(4).CompareNoCase(_T(".mht")) == 0 && strURL.Find(_T("#")) != -1)
		{		
			pView->Refresh();
			pView->Navigate2(strURL);
		}

		m_strUrl = strURL;
	}

	// TODO:  Add your specialized creation code here

	return TRUE;
}

// 刷新属性树
void CStylerDoc::RefreshDocProperty()
{
	if(!theApp.m_pIPlatUI || !theApp.m_pIPlatUI->EnableUpdatePropertyGridContent())
	{
		return;
	}

	// 更新属性树
	theApp.m_pIPlatUI->UpdatePropertyGridContent();
}