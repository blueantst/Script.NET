// CPopupListTips.cpp : implementation file
//

#include "stdafx.h"
#include "CPopupListTips.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CPopupListTips::s_bVScrollFirstTime = TRUE;

/////////////////////////////////////////////////////////////////////////////
// CPopupListTips

CPopupListTips::CPopupListTips()
{
	m_szMargins   = CSize(4, 4);
	// create font -- use system tooltip font
	CNonClientMetrics ncm;
	m_font.CreateFontIndirect(&ncm.lfStatusFont);
	m_strText     = _T("");
	m_bSingleLine = TRUE;

    m_lScrollYPos = 0;
}

CPopupListTips::CPopupListTips(CWnd* pParent)
{
	m_szMargins   = CSize(4, 4);
	// create font -- use system tooltip font
	CNonClientMetrics ncm;
	m_font.CreateFontIndirect(&ncm.lfStatusFont);
	m_strText     = _T("");
	m_bSingleLine = TRUE;
	
	m_lScrollYPos = 0;

	Create(CPoint(20, 20), pParent);
	
}

CPopupListTips::~CPopupListTips()
{
	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CPopupListTips, CWnd)
	//{{AFX_MSG_MAP(CPopupListTips)
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPopupListTips message handlers

BOOL CPopupListTips::PreCreateWindow(CREATESTRUCT& cs) 
{
	static CString sClassName;
	if (sClassName.IsEmpty())
		sClassName = AfxRegisterWndClass(0);
	cs.lpszClass = sClassName;
//	cs.style |= WS_BORDER;
	cs.style |= WS_DLGFRAME;
	cs.style |= WS_THICKFRAME;
	
	return CWnd::PreCreateWindow(cs);
}

void CPopupListTips::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(&rc);
	CBrush b(GetSysColor(COLOR_INFOBK)); // use tooltip bg color
	dc.FillRect(&rc, &b);
	
	// draw text
	dc.SetBkMode(TRANSPARENT);
	CFont* pOldFont = dc.SelectObject(&m_font);
	dc.SetTextColor(GetSysColor(COLOR_INFOTEXT)); // tooltip text color
	if (m_bSingleLine)
	{
		dc.DrawText(m_strText, &rc, DT_SINGLELINE | DT_CENTER | 
		DT_VCENTER | DT_EDITCONTROL);
	}
	else
	{
		// Using the extended draw function, fill the parameters to set the tab size
		// to 4 and the left/right margins to 2 units.
		DRAWTEXTPARAMS xDParam;
		xDParam.cbSize       = sizeof(DRAWTEXTPARAMS);
		xDParam.iLeftMargin  = 4;
		xDParam.iRightMargin = 4;
		xDParam.iTabLength   = 4;
		
		// Draw using the extended draw function, it is not supported by the MFC CDC.
		::DrawTextEx(dc.m_hDC, (LPTSTR)(LPCTSTR)m_strText, -1, rc, 
			DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_WORDBREAK | DT_EXPANDTABS | 
			DT_TABSTOP | DT_EDITCONTROL, &xDParam);
		
		//	dc.DrawText(m_strText, &rc, /*DT_CENTER*/DT_LEFT | DT_VCENTER | 
		//		DT_NOPREFIX | DT_WORDBREAK | DT_EDITCONTROL);
	}

	dc.SelectObject(pOldFont);	
	// Do not call CWnd::OnPaint() for painting messages
}

// Create the window. 
//    pt is upper left or upper right corner depending on nStyle.
BOOL CPopupListTips::Create(CPoint pt, CWnd* pParentWnd, UINT nStyle, UINT nID)
{
	m_nStyle = nStyle;
	return CreateEx(0,
		NULL,
		NULL,
		WS_POPUP,// | WS_VISIBLE,
		CRect(pt, CSize(0, 0)),
		pParentWnd,
		nID);
}

// The text changed: resize to fit new text
BOOL CPopupListTips::SetText(const CString& strText, BOOL bSingleLine/* = TRUE*/)
{
	m_bSingleLine   = bSingleLine;
	m_strText       = strText;
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&m_font);
	CRect rc;
	GetWindowRect(&rc);
	int x = (m_nStyle & JUSTIFYRIGHT) ? rc.right : rc.left;
	int y = rc.top;
	dc.DrawText(strText, &rc, DT_CALCRECT);
	dc.SelectObject(pOldFont);
	Invalidate();
	rc.InflateRect(m_szMargins);
	if (m_nStyle & JUSTIFYRIGHT)
		x -= rc.Width();
	SetWindowPos(NULL, x, y, rc.Width(), rc.Height(), SWP_NOZORDER);
	return TRUE;
}

void CPopupListTips::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// destroy old region
	m_rgn.DeleteObject();

	// create rounded rect region based on new window size
	CRect rc;					// rounded rectangle 
	GetWindowRect(&rc);			// window rect in screen coords
	rc -= rc.TopLeft();			// convert to window coords: top left = (0,0)
	m_rgn.CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom, 
		20, 20);				// rounded rect w/20 pixel corners
	SetWindowRgn(m_rgn,TRUE);	// set window region to make rounded window
}

void CPopupListTips::DoScrollTip(UINT nTopLine)
{
	CWnd* pWnd = GetParent();
	
	if (pWnd)
	{
		// set y-position of the page number window
		if (s_bVScrollFirstTime)  // Comment out this line to tract the mouse.
		{
			POINT ptCursorPos;
			GetCursorPos(&ptCursorPos);
			m_lScrollYPos = ptCursorPos.y;
		}
		
		// determine size and position of the view
		CRect rect;
//		pWnd->GetClientRect(rect);
//		pWnd->ClientToScreen(rect);
		pWnd->GetWindowRect(rect);
//		pWnd->ClientToScreen(rect);
		
		// build the string that will be displayed
		m_strText.Format(_T("Topmost Line: %d"), nTopLine); //TODO--to resource
		
		// adjust size and position of the window
		CPaintDC dc(this);
		int iWidth, iHeight;
		int xPos, yPos;
		
		CSize size = dc.GetTextExtent(m_strText);
		iWidth     = size.cx + 10;
		iHeight    = size.cy + 6;
		
		xPos = rect.right - iWidth - 80;
		yPos = m_lScrollYPos    - 10;
		
		SetWindowPos(NULL, xPos, yPos, iWidth, iHeight, 
			SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
		
		RedrawWindow();
		
		s_bVScrollFirstTime = FALSE;
	}
}  

void CPopupListTips::EndScrollTip()
{
   ShowWindow(SW_HIDE);
   s_bVScrollFirstTime = TRUE;
}
