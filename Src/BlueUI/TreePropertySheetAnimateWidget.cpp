// TreePropertySheetAnimateWidget.cpp : implementation file
//

#include "stdafx.h"
#include "TreePropertySheet.h"
#include "TreePropertySheetAnimateWidget.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreePropertySheetAnimateWidget

CTreePropertySheetAnimateWidget::CTreePropertySheetAnimateWidget(CWnd *pParentWnd,COLORREF clrLeft1,COLORREF clrLeft2,COLORREF clrRight1,COLORREF clrRight2,int nSteps)
{
	Create(NULL,NULL,WS_CHILD,CRect(0,0,0,0),pParentWnd,(UINT)IDC_STATIC);
	m_clrLeft1	=clrLeft1;
	m_clrLeft2	=clrLeft2;
	m_clrRight1	=clrRight1;
	m_clrRight2	=clrRight2;
	m_nSteps	=nSteps;
	m_nStep		=0;
}

CTreePropertySheetAnimateWidget::~CTreePropertySheetAnimateWidget()
{
}


BEGIN_MESSAGE_MAP(CTreePropertySheetAnimateWidget, CWnd)
	//{{AFX_MSG_MAP(CTreePropertySheetAnimateWidget)
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTreePropertySheetAnimateWidget message handlers

int CTreePropertySheetAnimateWidget::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	SetTimer(0x1234,100,NULL);
	
	return 0;
}

void CTreePropertySheetAnimateWidget::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==0x1234) {
		CTreePropertySheet *pSheet;
		pSheet		=GetSheet(this);

		if(pSheet!=NULL) {
			COLORREF	clrLeft,clrRight;
			float		fVal=(float)(::GetTickCount());
			float		nStep=(float)(sin((fVal/2000.0)*3.14159265)*128.0);

			nStep		+=128;

			clrLeft	=interpolate(m_clrLeft1,m_clrLeft2,(int)nStep,256);
			clrRight=interpolate(m_clrRight1,m_clrRight2,(int)nStep,256);

			m_nStep++;
			if(m_nStep==(2*m_nSteps))
				m_nStep=0;

			GetSheet(this)->m_clrCaptionLeft=clrLeft;
			GetSheet(this)->m_clrCaptionRight=clrRight;
			GetSheet(this)->InvalidateCaption(TRUE);
			GetSheet(this)->UpdateWindow();
		}
	}
}
