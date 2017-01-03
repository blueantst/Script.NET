/****************************************************************************
 *	class		:	CKCSideBannerWnd
 *	author		:	Peter Mares / kinkycode.com (gui@kinkycode.com)
 *	base class	:	CWnd (MFC)
 *	notes		:	A control to act as a banner to a window. More of a look'n'feel
 *					control.
 *					NOTE: Remember to link to MSIMG32.LIB to enable GradientFill()
 *
 *	Blurb		:	Its free, it feels good and its from South Africa :)
 ****************************************************************************
 *	Version History:
 *
 *	v0.1 (20-Oct-2003)
 *
 *	- First public release
 *
 *	v0.2 (24-Oct-2003)
 *
 *	- Added (Set)(Get)ColTxtTitle() and (Set)(Get)ColTxtCaption() functions
 *	- Revised all other updates since initial release
 *
 *	v0.3 (25-Oct-2003)
 *
 *	- Changed the SetIcon() function to support a delete flag
 *	- Added SetTexture() function
 *	- Fixed a resource leak
 *
 *	v0.31 (26-Oct-2003)
 *
 *	- Added UNICODE support to the control - thanks to Abraxas23
 *
 *	v0.32 (29-Oct-2003)
 *
 *	- Added support for gradient drawing without the dependancy on MSIMG32.LIB
 *    Used John A. Johnson's GradientFill() function, and used dynamic linking
 *	  of the MSIMG32.DLL (thanks to Irek Zielinski's code)
 *
 ****************************************************************************/

#include "stdafx.h"
#include "KCSideBannerWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKCSideBannerWnd

CKCSideBannerWnd::CKCSideBannerWnd()
: CWndUtil(KCSB_CLASSNAME)
, m_pOwner(NULL)
, m_nSize(50)
, m_uPosFlag(0)
, m_uFillFlag(KCSB_FILL_GRADIENT)
, m_colBkg(RGB(255,255,255))
, m_colBkg2(RGB(128,128,192))
, m_colTxtTitle(RGB(0,0,0))
, m_colTxtCaption(RGB(0,0,0))
, m_colEdge(RGB(0,0,0))
, m_strTitle(_T("This is the sample title"))
, m_strCaption(_T("This is the sample caption..."))
, m_szEdgeOffset(10,5)
, m_szCaptionOffset(0, 8)
, m_uIconPos(KCSB_ICON_RIGHT)
, m_hIcon(0)
, m_bIconDelete(false)
, m_hBkgBitmap(0)
, m_bBmpDelete(false)
, m_pGradFill(NULL)
, m_hGradMod(0)
{
	RegisterWndClass();

	CFont			font;

	PLAT_LOADSTRING(strFont, IDS_BANNER_FONT)
	//font.CreatePointFont(110, _T("Tahoma Bold"));
	font.CreatePointFont(100, strFont);
	font.GetLogFont(&m_lfTitle);
	font.DeleteObject();
	//font.CreatePointFont(85, _T("Tahoma"));
	font.CreatePointFont(90, strFont);
	font.GetLogFont(&m_lfCaption);
	font.DeleteObject();

	// try and load the MSIMG32.LIB
	if ( (m_hGradMod = LoadLibrary(_T("MSIMG32.DLL"))) )
		m_pGradFill = (PFNGRADFILL) GetProcAddress( m_hGradMod, "GradientFill" );
}

/////////////////////////////////////////////////////////////////////////////

CKCSideBannerWnd::~CKCSideBannerWnd()
{
	if ( m_hBkgBitmap && m_bBmpDelete )
		::DeleteObject(m_hBkgBitmap);
	if ( m_hIcon && m_bIconDelete )
		::DeleteObject(m_hIcon);
	if ( m_hGradMod )
		::FreeLibrary(m_hGradMod);
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CKCSideBannerWnd, CWnd)
	//{{AFX_MSG_MAP(CKCSideBannerWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL CKCSideBannerWnd::Attach(CWnd* pWnd, unsigned int uFlags, unsigned int uiID)
{
	ASSERT(pWnd);
	if ( !pWnd )
		return FALSE;
	ASSERT(pWnd->m_hWnd);
	if ( !pWnd->m_hWnd )
		return FALSE;

	CRect				rect;

	m_pOwner = pWnd;
	m_pOwner->GetWindowRect(&rect);
	m_pOwner->ClientToScreen(&rect);

	m_uPosFlag = uFlags;

	// Banner LEFT
	if ( uFlags & KCSB_ATTACH_LEFT )
	{
		rect.left -= m_nSize;
		if ( rect.left < 0 )
			rect.OffsetRect( -rect.left, 0 );
	}
	// Banner RIGHT
	else if ( uFlags & KCSB_ATTACH_RIGHT )
	{
		rect.right += m_nSize;
	}
	// Banner TOP
	else if ( uFlags & KCSB_ATTACH_TOP )
	{
		rect.top -= m_nSize;
		if ( rect.top < 0 )
			rect.OffsetRect( 0, -rect.top );
	}
	// Banner BOTTOM
	else if ( uFlags & KCSB_ATTACH_BOTTOM )
	{
		rect.bottom += m_nSize;
	}
	// update the size of the owner
	m_pOwner->SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER);

	// update the positions of the child controls
	UpdateLayout(m_nSize);

	// attach the banner respective to its positional flag
	m_pOwner->GetClientRect(&rect);
	if ( uFlags & KCSB_ATTACH_LEFT )
		rect.right = rect.left + m_nSize;
	else if ( uFlags & KCSB_ATTACH_RIGHT )
		rect.left = rect.right - m_nSize;
	else if ( uFlags & KCSB_ATTACH_TOP )
		rect.bottom = rect.top + m_nSize;
	else if ( uFlags & KCSB_ATTACH_BOTTOM )
		rect.top = rect.bottom - m_nSize;

	// et voila... create the banner..
	Create(WS_CHILD | WS_VISIBLE, rect, m_pOwner, uiID);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

void CKCSideBannerWnd::SetSize(int nSize)
{
	if ( m_hWnd )
	{
		CRect			rect;

		//
		// offset the controls accordingly
		if ( m_uPosFlag & KCSB_ATTACH_LEFT || m_uPosFlag & KCSB_ATTACH_TOP )
		{
			UpdateLayout( nSize - m_nSize );
		}

		//
		// size the containing window
		m_pOwner->GetWindowRect(&rect);

		if ( m_uPosFlag & KCSB_ATTACH_LEFT )
		{
			rect.left -= (nSize-m_nSize);
			if ( rect.left < 0 )
				rect.OffsetRect( -rect.left, 0 );
			m_pOwner->SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER);
		}
		else if ( m_uPosFlag & KCSB_ATTACH_RIGHT )
		{
			rect.left -= (nSize - m_nSize);
			m_pOwner->SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
		}
		else if ( m_uPosFlag & KCSB_ATTACH_TOP )
		{
			rect.top -= (nSize - m_nSize);
			if ( rect.top < 0 )
				rect.OffsetRect( 0, -rect.top );
			m_pOwner->SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER);
		}
		else if ( m_uPosFlag & KCSB_ATTACH_BOTTOM )
		{
			rect.top -= (nSize - m_nSize );
			m_pOwner->SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
		}
	
		// size the banner
		GetClientRect(&rect);
		if ( m_uPosFlag & KCSB_ATTACH_LEFT )
			rect.right = rect.left + nSize;
		else if ( m_uPosFlag & KCSB_ATTACH_RIGHT )
			rect.right = rect.left + nSize;
		else if ( m_uPosFlag & KCSB_ATTACH_TOP )
			rect.bottom = rect.top + nSize;
		else if ( m_uPosFlag & KCSB_ATTACH_BOTTOM )
			rect.bottom = rect.top + nSize;
		MoveWindow(&rect);

		m_pOwner->Invalidate();

	}

	m_nSize = nSize;
}

/////////////////////////////////////////////////////////////////////////////

void CKCSideBannerWnd::UpdateSize()
{
	if ( !m_hWnd )
		return;

	CRect				rect;

	m_pOwner->GetClientRect(&rect);
	if ( m_uPosFlag & KCSB_ATTACH_LEFT )
		rect.right = rect.left + m_nSize;
	else if ( m_uPosFlag & KCSB_ATTACH_RIGHT )
		rect.left = rect.right - m_nSize;
	else if ( m_uPosFlag & KCSB_ATTACH_TOP )
		rect.bottom = rect.top + m_nSize;
	else if ( m_uPosFlag & KCSB_ATTACH_BOTTOM )
		rect.top = rect.bottom - m_nSize;

	MoveWindow(&rect, TRUE);
}

/////////////////////////////////////////////////////////////////////////////

void CKCSideBannerWnd::SetPosFlag(unsigned int uFlags)
{
	if ( (uFlags & 0xF) == (m_uPosFlag & 0xF) )
		return;

	CRect				rect;
	int					nSize = m_nSize;

	SetSize(0);
	m_uPosFlag = uFlags;
	SetSize(nSize);

	m_pOwner->GetClientRect(&rect);

	if ( uFlags & KCSB_ATTACH_LEFT )
		rect.right = rect.left + nSize;
	else if ( uFlags & KCSB_ATTACH_RIGHT )
		rect.left = rect.right - nSize;
	else if ( uFlags & KCSB_ATTACH_TOP )
		rect.bottom = rect.top + nSize;
	else if ( uFlags & KCSB_ATTACH_BOTTOM )
		rect.top = rect.bottom - nSize;

	MoveWindow(&rect);
	
	m_pOwner->Invalidate();
}

/////////////////////////////////////////////////////////////////////////////

void CKCSideBannerWnd::UpdateLayout(int nOffset)
{
	stEnum				data;

	data.pBannerWnd = this;
	data.pParentWnd = m_pOwner;
	data.nOffset = nOffset;
	data.uFlags = m_uPosFlag;

	::EnumChildWindows( m_pOwner->m_hWnd, ChildEnumProc, (LPARAM)&data );
}

/////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK CKCSideBannerWnd::ChildEnumProc(HWND hWndChild, LPARAM lParam)
{
	stEnum*			pData = (stEnum*) lParam;
	HWND			hWndParent = 0;
	CRect			rect;
	POINT			topLeft, bottomRight;

	// check if the hwnd is valid, and check that its not the HWND to the banner
	if ( hWndChild && hWndChild != pData->pBannerWnd->m_hWnd )
	{
		// now we check if the parent is actually the parent of the banner..
		// this check was put it to deal with composite controls like a combobox..
		// NOTE: For some reason, a combobox's edit control appears to be a child
		// of the parent, until you query for its parent HWND.
		hWndParent = ::GetParent(hWndChild);
		if ( hWndParent == pData->pParentWnd->m_hWnd )
		{
			// we have a valid child window which we can now reposition
			::GetWindowRect(hWndChild, &rect);
			topLeft.x = rect.left;
			topLeft.y = rect.top;
			bottomRight.x = rect.right;
			bottomRight.y = rect.bottom;
			::ScreenToClient( pData->pParentWnd->m_hWnd, &topLeft );
			::ScreenToClient( pData->pParentWnd->m_hWnd, &bottomRight );
			if ( pData->uFlags & KCSB_ATTACH_LEFT )
			{
				topLeft.x += pData->nOffset;
				bottomRight.x += pData->nOffset;
			}
			else if ( pData->uFlags & KCSB_ATTACH_TOP )
			{
				topLeft.y += pData->nOffset;
				bottomRight.y += pData->nOffset;
			}
			::MoveWindow( hWndChild, topLeft.x, topLeft.y, bottomRight.x - topLeft.x, bottomRight.y - topLeft.y, TRUE );
		}
	}

	// return TRUE to continue enumerating...
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CKCSideBannerWnd message handlers
/////////////////////////////////////////////////////////////////////////////

BOOL CKCSideBannerWnd::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	BOOL		bResult = CWnd::Create(KCSB_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID, pContext);

	return bResult;
}

/////////////////////////////////////////////////////////////////////////////

void CKCSideBannerWnd::OnPaint() 
{
	CPaintDC			dc(this); // device context for painting
	CDC					memDC;
	CBitmap				memBMP, *pOldBmp = NULL;
	CRect				rect;

	GetClientRect(&rect);

	// create mem DC
	memDC.CreateCompatibleDC(&dc);
	memBMP.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	pOldBmp = memDC.SelectObject(&memBMP);

	// draw the banner
	DrawBackground(&memDC, rect);
	DrawTextFields(&memDC, rect);

	// render the image
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	// deselect object
	memDC.SelectObject(pOldBmp);

	// delete objects
	memBMP.DeleteObject();
	memDC.DeleteDC();
}

/////////////////////////////////////////////////////////////////////////////

BOOL CKCSideBannerWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

void CKCSideBannerWnd::DrawBackground(CDC* pDC, CRect rect)
{
	CBrush				nBrush, *pOldBrush = NULL;
	CPen				nPen;

	// Flat fill...
	if ( m_uFillFlag & KCSB_FILL_FLAT || ( (m_colBkg == m_colBkg2) && (m_uFillFlag & KCSB_FILL_GRADIENT) ))
	{
		pDC->FillSolidRect(&rect, m_colBkg);
		DrawEdge(pDC, rect);
	}
	// Gradient fill
	else if ( m_uFillFlag & KCSB_FILL_GRADIENT )
	{
		TRIVERTEX			vert[2];
		GRADIENT_RECT		gRect;
		ULONG				uGFlag;

		if ( m_pGradFill )		// can we use the MSIMG32.DLL function?
		{
			if ( (m_uPosFlag & KCSB_ATTACH_LEFT) || (m_uPosFlag & KCSB_ATTACH_RIGHT) )
			{
				// set the gradient fill according to position
				if ( m_uPosFlag & KCSB_ATTACH_LEFT )
				{
					vert[0].x = 0;
					vert[0].y = rect.top;
					vert[0].Red = GetRValue(m_colBkg2)<<8;
					vert[0].Green = GetGValue(m_colBkg2)<<8;
					vert[0].Blue = GetBValue(m_colBkg2)<<8;
					vert[0].Alpha = 0;

					vert[1].x = rect.right;
					vert[1].y = rect.bottom;
					vert[1].Red = GetRValue(m_colBkg)<<8;
					vert[1].Green = GetGValue(m_colBkg)<<8;
					vert[1].Blue = GetBValue(m_colBkg)<<8;
					vert[1].Alpha = 0;
				}
				else
				{
					vert[0].x = rect.right;
					vert[0].y = rect.top;
					vert[0].Red = GetRValue(m_colBkg)<<8;
					vert[0].Green = GetGValue(m_colBkg)<<8;
					vert[0].Blue = GetBValue(m_colBkg)<<8;
					vert[0].Alpha = 0;

					vert[1].x = 0;
					vert[1].y = rect.bottom;
					vert[1].Red = GetRValue(m_colBkg2)<<8;
					vert[1].Green = GetGValue(m_colBkg2)<<8;
					vert[1].Blue = GetBValue(m_colBkg2)<<8;
					vert[1].Alpha = 0;
				}
				uGFlag = GRADIENT_FILL_RECT_V;
			}
			else
			{
				vert[0].x = 0;
				vert[0].y = rect.top;
				vert[0].Red = GetRValue(m_colBkg)<<8;
				vert[0].Green = GetGValue(m_colBkg)<<8;
				vert[0].Blue = GetBValue(m_colBkg)<<8;
				vert[0].Alpha = 0;

				vert[1].x = rect.right;
				vert[1].y = rect.bottom;
				vert[1].Red = GetRValue(m_colBkg2)<<8;
				vert[1].Green = GetGValue(m_colBkg2)<<8;
				vert[1].Blue = GetBValue(m_colBkg2)<<8;
				vert[1].Alpha = 0;
				uGFlag = GRADIENT_FILL_RECT_H;
			}

			gRect.UpperLeft = 0;
			gRect.LowerRight = 1;

			// do the fill :)
			m_pGradFill( pDC->m_hDC, vert, 2, &gRect, 1, uGFlag );
		}
		else
		{
			// added 2003-Oct-29
			bool bFlip = false, bHorz = false;

			if ( (m_uPosFlag & KCSB_ATTACH_TOP) || (m_uPosFlag & KCSB_ATTACH_BOTTOM) )
			{
				bFlip = true;
				bHorz = true;
			}
			else if ( m_uPosFlag & KCSB_ATTACH_RIGHT )
			{
				bFlip = true;
			}
 
			GradientFill(pDC, m_colBkg, m_colBkg2, rect, bHorz, bFlip);
		}

		DrawEdge(pDC, rect);
	}
	// Gradient fill
	else if ( m_uFillFlag & KCSB_FILL_TEXTURE )
	{
		CBitmap			bmp;
		
		bmp.Attach(m_hBkgBitmap);

		nBrush.CreatePatternBrush(&bmp);
		pOldBrush = pDC->SelectObject(&nBrush);
		pDC->SelectStockObject(NULL_PEN);
		rect.InflateRect(1,1,1,1);
		pDC->Rectangle( &rect );
		rect.DeflateRect(1,1,1,1);
		DrawEdge(pDC, rect);

		bmp.Detach();
	}
}

/////////////////////////////////////////////////////////////////////////////

void CKCSideBannerWnd::DrawEdge(CDC* pDC, CRect rect)
{
	CPen				nPen, *pOldPen = NULL;

	nPen.CreatePen( PS_SOLID, 0, m_colEdge );
	pOldPen = pDC->SelectObject(&nPen);

	if ( m_uPosFlag & KCSB_ATTACH_LEFT )
	{
		pDC->MoveTo(rect.right-1, rect.top);
		pDC->LineTo(rect.right-1, rect.bottom);
	}
	else if ( m_uPosFlag & KCSB_ATTACH_RIGHT )
	{
		pDC->MoveTo(rect.left, rect.top);
		pDC->LineTo(rect.left, rect.bottom);
	}
	else if ( m_uPosFlag & KCSB_ATTACH_TOP )
	{
		pDC->MoveTo(rect.left, rect.bottom-1);
		pDC->LineTo(rect.right, rect.bottom-1);
	}
	else if ( m_uPosFlag & KCSB_ATTACH_BOTTOM )
	{
		pDC->MoveTo(rect.left, rect.top);
		pDC->LineTo(rect.right, rect.top);
	}

	pDC->SelectObject(pOldPen);
	nPen.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////

void CKCSideBannerWnd::DrawTextFields(CDC* pDC, CRect rect)
{
	CPoint				pt, pt2;

	if ( m_hIcon )
		DrawBannerIcon(pDC, rect);

	// configure the font
	if ( m_uPosFlag & KCSB_ATTACH_LEFT )
	{
		m_lfTitle.lfEscapement = m_lfTitle.lfOrientation = 
		m_lfCaption.lfEscapement = m_lfCaption.lfOrientation = 900;
		pt.x = rect.left + m_szEdgeOffset.cx;
		pt.y = rect.bottom - m_szEdgeOffset.cy;
		pt2.x = pt.x - m_lfTitle.lfHeight + m_szCaptionOffset.cx;
		pt2.y = pt.y - m_szCaptionOffset.cy;
	}
	else if ( m_uPosFlag & KCSB_ATTACH_RIGHT )
	{
		m_lfTitle.lfEscapement = m_lfTitle.lfOrientation = 
		m_lfCaption.lfEscapement = m_lfCaption.lfOrientation = -900;
		pt.x = rect.right - m_szEdgeOffset.cx;
		pt.y = rect.top + m_szEdgeOffset.cy;
		pt2.x = pt.x + m_lfTitle.lfHeight - m_szCaptionOffset.cx;
		pt2.y = pt.y + m_szCaptionOffset.cy;
	}
	else if ( ( m_uPosFlag & KCSB_ATTACH_TOP ) || ( m_uPosFlag & KCSB_ATTACH_BOTTOM ) )
	{
		m_lfTitle.lfEscapement = m_lfTitle.lfOrientation = 
		m_lfCaption.lfEscapement = m_lfCaption.lfOrientation = 0;
		pt.x = rect.left + m_szEdgeOffset.cx;
		pt.y = rect.top + m_szEdgeOffset.cy;
		pt2.x = pt.x + m_szCaptionOffset.cx;
		pt2.y = pt.y - m_lfTitle.lfHeight + m_szCaptionOffset.cy;
	}

	int					nOldBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont				nFont, *pOldFont = NULL;
	COLORREF			oldTxtCol = pDC->SetTextColor(m_colTxtTitle);

	//
	// draw the title
	nFont.CreateFontIndirect(&m_lfTitle);
	pOldFont = pDC->SelectObject(&nFont);
	pDC->TextOut(pt.x, pt.y, m_strTitle);
	pDC->SelectObject(pOldFont);
	nFont.DeleteObject();

	//
	// draw the caption
	pDC->SetTextColor(m_colTxtCaption);
	nFont.CreateFontIndirect(&m_lfCaption);
	pOldFont = pDC->SelectObject(&nFont);

	pDC->TextOut(pt2.x, pt2.y, m_strCaption);
/*
	int				arrWidths[256];
	pDC->GetCharWidth(0, 255, arrWidths);

	int			nCount = m_strCaption.GetLength(), i, nHeight = 0;
	CPoint		ptDraw = pt2;
	TCHAR		ch;

	for ( i = 0; i < nCount; i++ )
	{
		ch = m_strCaption.GetAt(i);
		if ( ch == '\r' )
			continue;
		if ( ch == '\n' )
		{
			if ( m_uPosFlag & KCSB_ATTACH_LEFT )
			{
				ptDraw = pt2;
				ptDraw.x -= m_lfCaption.lfHeight * (++nHeight);
			}
			else if ( (m_uPosFlag & KCSB_ATTACH_TOP) || (m_uPosFlag & KCSB_ATTACH_BOTTOM) )
			{
				ptDraw = pt2;
				ptDraw.y -= m_lfCaption.lfHeight * (++nHeight);
			}
			else if ( (m_uPosFlag & KCSB_ATTACH_RIGHT) )
			{
				ptDraw = pt2;
				ptDraw.x += m_lfCaption.lfHeight * (++nHeight);
			}
			continue;
		}
		pDC->TextOut( ptDraw.x, ptDraw.y, CString((char)ch) );
		if ( m_uPosFlag & KCSB_ATTACH_LEFT )
		{
			ptDraw.y -= arrWidths[ch];
			if ( (i < nCount-1) && 
				 ptDraw.y - arrWidths[m_strCaption.GetAt(i+1)] < rect.top )
			{
				ptDraw = pt2;
				ptDraw.x -= m_lfCaption.lfHeight * (++nHeight);
			}
		}
		else if ( (m_uPosFlag & KCSB_ATTACH_TOP) || (m_uPosFlag & KCSB_ATTACH_BOTTOM) )
		{
			ptDraw.x += arrWidths[ch];
			if ( (i < nCount-1) &&
				 ptDraw.x + arrWidths[m_strCaption.GetAt(i+1)] > rect.right )
			{
				ptDraw = pt2;
				ptDraw.y -= m_lfCaption.lfHeight * (++nHeight);
			}
		}
		else if ( (m_uPosFlag & KCSB_ATTACH_RIGHT) )
		{
			ptDraw.y += arrWidths[ch];
			if ( (i < nCount-1) && 
				 ptDraw.y + arrWidths[m_strCaption.GetAt(i+1)] > rect.bottom )
			{
				ptDraw = pt2;
				ptDraw.x += m_lfCaption.lfHeight * (++nHeight);
			}		
		}

	}
*/
	pDC->SelectObject(pOldFont);
	nFont.DeleteObject();

	pDC->SetBkMode(nOldBkMode);
	pDC->SetTextColor(oldTxtCol);
}

/////////////////////////////////////////////////////////////////////////////

void CKCSideBannerWnd::DrawBannerIcon(CDC* pDC, CRect& rect)
{
	if ( !m_hIcon )
		return;

	CPoint			pt;

	// Banner on LEFT
	if ( m_uPosFlag & KCSB_ATTACH_LEFT )
	{
		// ICON to LEFT
		if ( m_uIconPos & KCSB_ICON_LEFT )
		{
			rect.bottom -= (m_szEdgeOffset.cy + m_bmpInfo.bmHeight);
			pt.y = rect.bottom;
		}
		// ICON to RIGHT
		else if ( m_uIconPos & KCSB_ICON_RIGHT )
		{
			rect.top += (m_szEdgeOffset.cy);
			pt = rect.TopLeft();
		}

		// VTOP
		if ( m_uIconPos & KCSB_ICON_TOP )
			pt.x = rect.left;
		// VCENTER
		else if ( m_uIconPos & KCSB_ICON_VCENTER )
			pt.x = rect.CenterPoint().x - (m_bmpInfo.bmWidth/2);
		// VBOTTOM
		else if ( m_uIconPos & KCSB_ICON_BOTTOM )
			pt.x = rect.right - m_bmpInfo.bmWidth - 1;

	}
	// Banner on RIGHT
	else if ( m_uPosFlag & KCSB_ATTACH_RIGHT )
	{
		// ICON to LEFT
		if ( m_uIconPos & KCSB_ICON_LEFT )
		{
			rect.top += ( m_szEdgeOffset.cy );
			pt.y = rect.top;
			rect.top += m_bmpInfo.bmHeight;
		}
		// ICON to RIGHT
		else if ( m_uIconPos & KCSB_ICON_RIGHT )
		{
			rect.bottom -= (m_szEdgeOffset.cy + m_bmpInfo.bmHeight);
			pt.x = rect.left;
			pt.y = rect.bottom;
		}

		// VTOP
		if ( m_uIconPos & KCSB_ICON_TOP )
			pt.x = rect.right - m_bmpInfo.bmWidth;
		// VCENTER
		else if ( m_uIconPos & KCSB_ICON_VCENTER )
			pt.x = rect.CenterPoint().x - (m_bmpInfo.bmWidth/2);
		// VBOTTOM
		else if ( m_uIconPos & KCSB_ICON_BOTTOM )
			pt.x = rect.left + 1;
	}
	// Banner on TOP OR BOTTOM
	else if ( (m_uPosFlag & KCSB_ATTACH_TOP) ||
			  (m_uPosFlag & KCSB_ATTACH_BOTTOM) )
	{
		// ICON to LEFT
		if ( m_uIconPos & KCSB_ICON_LEFT )
		{
			rect.left += m_szEdgeOffset.cx;
			pt = rect.TopLeft();
			rect.left += m_bmpInfo.bmWidth;
		}
		// ICON to RIGHT
		else if ( m_uIconPos & KCSB_ICON_RIGHT )
		{
			rect.right -= ( m_szEdgeOffset.cx + m_bmpInfo.bmWidth);
			pt.x = rect.right;
			pt.y = rect.top;
		}

		// VTOP
		if ( m_uIconPos & KCSB_ICON_TOP )
			pt.y = rect.top+1;
		// VCENTER
		else if ( m_uIconPos & KCSB_ICON_VCENTER )
			pt.y = rect.CenterPoint().y - (m_bmpInfo.bmHeight/2);
		// VBOTTOM
		else if ( m_uIconPos & KCSB_ICON_BOTTOM )
			pt.y = rect.bottom - m_bmpInfo.bmHeight-1;
	}
	pDC->DrawIcon( pt.x, pt.y, m_hIcon );
}

/////////////////////////////////////////////////////////////////////////////

void CKCSideBannerWnd::SetCaptionFont(CFont* pFont)
{
	if ( !pFont )
		return;

	pFont->GetLogFont(&m_lfCaption);
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////

void CKCSideBannerWnd::GetCaptionFont(LOGFONT* pFont)
{
	if ( !pFont )
		return;

	memcpy(pFont, &m_lfCaption, sizeof(LOGFONT));
}

/////////////////////////////////////////////////////////////////////////////

void CKCSideBannerWnd::SetTitleFont(CFont* pFont)
{
	if ( !pFont )
		return;

	pFont->GetLogFont(&m_lfTitle);
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////

void CKCSideBannerWnd::GetTitleFont(LOGFONT* pFont)
{
	if ( !pFont )
		return;

	memcpy(pFont, &m_lfTitle, sizeof(LOGFONT));
}

/////////////////////////////////////////////////////////////////////////////

bool CKCSideBannerWnd::SetIcon(HICON hIcon, UINT uiIconPos, bool bSelfDelete)
{
	bool			bResult = true;

	if ( m_bIconDelete && m_hIcon )
		::DeleteObject(m_hIcon);

	m_bIconDelete = bSelfDelete;
	m_uIconPos = uiIconPos;
	m_hIcon = hIcon;

	if ( m_hIcon )
	{
		// calculate the size of the icon
		ICONINFO			ii;

		if ( !GetIconInfo( (HICON)m_hIcon, &ii ) )
			bResult = false;
		if ( !GetObject( ii.hbmColor, sizeof(BITMAP), &m_bmpInfo ) )
			bResult = false;
		else
		{
			DeleteObject(ii.hbmColor);
			DeleteObject(ii.hbmMask);
		}
	}

	if ( IsWindow(m_hWnd) && m_hIcon )
		Invalidate();

	return bResult;
}

/////////////////////////////////////////////////////////////////////////////

void CKCSideBannerWnd::SetTexture(HBITMAP hBitmap, bool bSelfDelete)
{
	if ( m_hBkgBitmap && m_bBmpDelete )
		::DeleteObject(m_hBkgBitmap);
	m_bBmpDelete = bSelfDelete;
	m_hBkgBitmap = hBitmap;
}

/////////////////////////////////////////////////////////////////////////////

// added 2003-Oct-29
void CKCSideBannerWnd::GradientFill(CDC* pDC, COLORREF col1, COLORREF col2, CRect rect, bool bHor, bool bFlip)
{	
	if (rect == CRect(0,0,0,0))  // no rectangle?	
	{		
		// get the clip area of the device context!		
		pDC->GetClipBox(rect);	
	} 	
	
	if (bFlip)	
	{		
		// exhange the 2 colors		
		COLORREF clr = col1;		
		col1 = col2;		
		col2 = clr;	
	} 	
	
	if (pDC->GetDeviceCaps(BITSPIXEL) > 8)	// gradient only for true color displays	
	{		
		// draw the gradient		
		int nLenght = (bHor ? rect.Width() : rect.Height()) - 1;		
		for (int i=0; i<=nLenght; i++)		
		{			
			CRect rcl	= rect; 			
			if (bHor)			
			{				
				rcl.left	+=  i;				
				rcl.right	+= (i + 1);			
			}			
			else			
			{				
				rcl.top		+=  i;				
				rcl.bottom	+= (i + 1);			
			} 			
			
			double dfL = (double)i / (double)nLenght; 			
			pDC->FillSolidRect(rcl, RGB(dfL * GetRValue(col1) + (1.0 - dfL) * GetRValue(col2),
							   dfL * GetGValue(col1) + (1.0 - dfL) * GetGValue(col2),				
							   dfL * GetBValue(col1) + (1.0 - dfL) * GetBValue(col2) ));		
		}	
	}	
	else	
	{		
		// a solid rectangle		
		pDC->FillSolidRect(rect, col1);	
	}
}
