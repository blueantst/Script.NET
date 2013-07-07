// CCrystalPopupWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CCrystalPopupWnd.h"
#include "OwmSciteView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCrystalPopupWnd

IMPLEMENT_DYNCREATE(CCrystalPopupWnd, CMiniFrameWnd)

CCrystalPopupWnd::CCrystalPopupWnd() : m_pCaller(NULL), m_pControl(NULL), 
	m_pEvents(NULL), m_hPrevParentCtrl(NULL), 
	m_bDestroy(FALSE), m_bResizable(FALSE)
{
	m_rcPopup.SetRectEmpty();
	m_bDoingToolTips = FALSE;
	m_nTextHeight    = 16;
	m_bViewCaller    = TRUE;      // Default to true, the common use
	m_bSendViewKeys  = FALSE;
	m_xInsertType    = PLIT_NONE;  // Defaults insert action to NONE!
}

CCrystalPopupWnd::~CCrystalPopupWnd()
{
}


BEGIN_MESSAGE_MAP(CCrystalPopupWnd, CMiniFrameWnd)
	//{{AFX_MSG_MAP(CCrystalPopupWnd)
	ON_WM_SIZE()
	ON_WM_ACTIVATE()
	ON_WM_CANCELMODE()
	ON_WM_GETMINMAXINFO()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCrystalPopupWnd message handlers

void CCrystalPopupWnd::OnSize(UINT nType, int cx, int cy) 
{
	CMiniFrameWnd::OnSize(nType, cx, cy);
	
	if (m_bResizable && m_pControl != NULL)
	{
		CRect rcChild;
		GetClientRect(&rcChild);
		rcChild.InflateRect(1, 1);
		m_pControl->MoveWindow(rcChild);
		Invalidate();
	}	
}

BOOL CCrystalPopupWnd::PreTranslateMessage(MSG* pMsg) 
{
    BOOL bEnd  = FALSE, bAbort = FALSE, bResult = FALSE;
    int  nCode = PU_NOACTION;
	
    switch (pMsg->message)
    {
		case WM_CHAR:
			if (m_pEvents != NULL)
			{
//				CPopupEventListener* pEvents = m_pEvents;
//				
//				// Search for the interface for the correct object 
//				// (referenced by pMsg->hwnd)
//				if (pMsg->hwnd != m_pControl->m_hWnd)
//				{
//					pEvents = m_pEvents->GetInterfaceOf(pMsg->hwnd);
//				}
//				if (pEvents != NULL)
//				{
//					nCode = pEvents->OnChar(pMsg->wParam, LOWORD(pMsg->lParam), 
//						HIWORD(pMsg->lParam));
//				}
				if (m_bViewCaller && m_bSendViewKeys)
				{
					if ((m_pCaller != NULL) && 
						((pMsg->wParam != VK_TAB) && (pMsg->wParam != VK_SPACE) &&
						(pMsg->wParam != VK_UP)   && (pMsg->wParam != VK_DOWN)  && 
						(pMsg->wParam != VK_RETURN)))
					{
						//m_pCaller->PostMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
						TRACE0("::SendMessage(...WM_CHAR...)\n");
						::SendMessage(m_pCaller->m_hWnd, pMsg->message, 
							pMsg->wParam, pMsg->lParam);
					}
				}
			}
			break;

		case WM_KEYDOWN:
			{
				// Default action for <Escape> key
				if (pMsg->wParam == VK_ESCAPE)
				{
					bEnd = bAbort = bResult = TRUE;
					break;
				}

				if (m_pEvents != NULL)
				{
					CPopupEventListener* pEvents = m_pEvents;
					
					// Search for the interface for the correct object 
					// (referenced by pMsg->hwnd)
					if (pMsg->hwnd != m_pControl->m_hWnd)
					{
						pEvents = m_pEvents->GetInterfaceOf(pMsg->hwnd);
					}
					if (pEvents != NULL)
					{
						nCode = pEvents->OnKeyDown(pMsg->wParam, LOWORD(pMsg->lParam), 
							HIWORD(pMsg->lParam));
					}
				}         
				// Inform the view about the key down message...
				// Enter and Space are special functions which ends the popup,
				// these must not be sent to the view
				if (m_bViewCaller && m_bSendViewKeys)
				{
					if ((m_pCaller != NULL)  && 
						((pMsg->wParam != VK_TAB) && (pMsg->wParam != VK_SPACE) &&
						(pMsg->wParam != VK_UP)   && (pMsg->wParam != VK_DOWN)  && 
						(pMsg->wParam != VK_RETURN)))
					{
						TRACE0("m_pCaller->PostMessage-WM_KEYDOWN\n");
						if (!_istprint(pMsg->wParam) || 
							((pMsg->wParam == VK_LEFT) || (pMsg->wParam == VK_RIGHT)))
						{
							m_pCaller->PostMessage(pMsg->message, 
							pMsg->wParam, pMsg->lParam);
						}
					}
				}
			}
			break;
			
		case WM_LBUTTONDBLCLK:
			if (m_pEvents != NULL)
			{
				CPopupEventListener* pEvents = m_pEvents;
				
				// Search for the interface for the correct object
				if (pMsg->hwnd != m_pControl->m_hWnd)
				{
					pEvents = m_pEvents->GetInterfaceOf(pMsg->hwnd);
				}
				if (pEvents != NULL)
				{
					nCode = pEvents->OnLButtonDblClk(pMsg->wParam, 
						CPoint(static_cast<short>(LOWORD(pMsg->lParam)), 
						static_cast<short>(HIWORD(pMsg->lParam))));
				}
			}
			break;
			
		case WM_LBUTTONDOWN:
			if (m_pEvents != NULL)
			{
				CPopupEventListener* pEvents = m_pEvents;
				
				// Search for the interface for the correct object
				if (pMsg->hwnd != m_pControl->m_hWnd)
				{
					pEvents = m_pEvents->GetInterfaceOf(pMsg->hwnd);
				}
				if (pEvents != NULL)
				{
					nCode = pEvents->OnLButtonDown(pMsg->wParam, 
						CPoint(static_cast<short>(LOWORD(pMsg->lParam)), 
						static_cast<short>(HIWORD(pMsg->lParam))));
				}
			}
			break;
			
		case WM_LBUTTONUP:
			if (m_pEvents != NULL)
			{
				CPopupEventListener* pEvents = m_pEvents;
				
				// Search for the interface for the correct object
				if (pMsg->hwnd != m_pControl->m_hWnd)
				{
					pEvents = m_pEvents->GetInterfaceOf(pMsg->hwnd);
				}
				if (pEvents != NULL)
				{
					nCode = pEvents->OnLButtonUp(pMsg->wParam, 
						CPoint(static_cast<short>(LOWORD(pMsg->lParam)), 
						static_cast<short>(HIWORD(pMsg->lParam))));
				}
			}
			break;
    }

    switch (nCode)
    {
		case PU_NOSEND:
			bResult = TRUE;
			break;
			
		case PU_END:
			bEnd = bResult = TRUE;
			break;
			
		case PU_ABORT:
			bEnd = bAbort = bResult = TRUE;
			break;
    }
    if (!bResult)
    {
        bResult = CMiniFrameWnd::PreTranslateMessage(pMsg) != 0;
    }
    if (bEnd)
    {
        EndPopup(bAbort);
    }
    return bResult;
}

void CCrystalPopupWnd::PostNcDestroy() 
{
    delete this;
}
								 
void CCrystalPopupWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
    CMiniFrameWnd::OnActivate(nState, pWndOther, bMinimized);

    if (nState == WA_INACTIVE && !m_bDestroy)
    {
        CFrameWnd* pFrame    = NULL;
        BOOL       bSetFocus = FALSE;
		
        if (m_pCaller != NULL && (pFrame = m_pCaller->GetParentFrame()) != NULL 
			&& pWndOther != NULL)
        {
            bSetFocus = (pWndOther->m_hWnd == pFrame->m_hWnd);
        }
        EndPopup(TRUE, bSetFocus);
    }
}

void CCrystalPopupWnd::OnCancelMode() 
{
	PostMessage(WM_DESTROY);
}

void CCrystalPopupWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CMiniFrameWnd::OnGetMinMaxInfo(lpMMI);
    lpMMI->ptMinTrackSize.x = m_rcPopup.Width();
    lpMMI->ptMinTrackSize.y = m_rcPopup.Height();
}

void CCrystalPopupWnd::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	if (!m_bResizable)
		lpwndpos->flags |= SWP_NOSIZE;
    CMiniFrameWnd::OnWindowPosChanging(lpwndpos);
}


BOOL CCrystalPopupWnd::Display(CWnd* pControl, CWnd* pCaller, CRect& rcCaller,
							   CURSORALIGN xAlignment/*= CA_TOPLEFT*/,
							   CPopupEventListener* pEvents, 
							   BOOL bButtomAlign, BOOL bResizable)
{
	// 1. This window cannot work without a child control, so if one is not
	// provided, delete the C++ object and return.
    if (pControl == NULL)
    {
        delete this;
        return FALSE;
    }
    m_pControl   = pControl;
    m_pCaller    = pCaller;
    m_pEvents    = pEvents;
	m_bResizable = bResizable;
	
	// 2. If the event handler is not property initialized, then do not proceed
    if (m_pEvents != NULL && !m_pEvents->OnInit())
    {
        delete this;
        return FALSE;

    }
    // 3. Calculate the most appropriate position for the left-top corner 
	//    of control. By default, at left and bottom of the caller.
	//    Then based on the start point and the screen parameters, make necessary
	//    adjustments to make sure the window does not go of the screen.
	CPoint pt(0, 0);
	CWindowRect rcCtrl(m_pControl);
	if ((xAlignment == CA_NONE) || (xAlignment == CA_TOPLEFT))
	{
		if (bButtomAlign)
			pt = CPoint(rcCaller.left, rcCaller.bottom);
		else
			pt = CPoint(rcCaller.left, rcCaller.top);
		
		// Alignment at right if necessary
		if (pt.x + rcCtrl.Width() > ::GetSystemMetrics(SM_CXSCREEN))
		{
			pt.x = rcCaller.right - rcCtrl.Width();
		}
		// Alignment at top if necessary
		if (pt.y + rcCtrl.Height() > ::GetSystemMetrics(SM_CYSCREEN))
		{
			pt.y = rcCaller.top - rcCtrl.Height();
		}
		// Adjustments to keep control into screen
		if (pt.x + rcCtrl.Width() > ::GetSystemMetrics(SM_CXSCREEN))
		{
			pt.x = ::GetSystemMetrics(SM_CXSCREEN) - rcCtrl.Width();
		}
		if (pt.y + rcCtrl.Height() > ::GetSystemMetrics(SM_CYSCREEN))
		{
			pt.y = ::GetSystemMetrics(SM_CYSCREEN) - rcCtrl.Height();
		}
		if (pt.x < 0)
			pt.x = 0;
		if (pt.y < 0)
			pt.y = 0;
	}
	else
	{
		pt.x = rcCaller.left;
		pt.y = rcCaller.top;
	}
	
	//	m_rcPopup = CRect(pt.x, pt.y, 
	//	pt.x + rcCtrl.Width() + ::GetSystemMetrics(SM_CXBORDER)*6, 
	//		pt.y + rcCtrl.Height() + ::GetSystemMetrics(SM_CYBORDER)*6);
	
	m_rcPopup = CRect(pt.x, pt.y, 
		pt.x + rcCtrl.Width() + ::GetSystemMetrics(SM_CXBORDER)*6, 
		pt.y + rcCtrl.Height() + ::GetSystemMetrics(SM_CYBORDER)*6);
	
	CRect rcPopupAlign  = m_rcPopup;
	// 4. Select the popup window- and cursor-alignment if requested.
	//    This is to make sure that the window does not cover the entering point,
	//    for instance in its use for autocomplete in the editor.
	switch (xAlignment)
	{
	case CA_NONE:
	case CA_TOPLEFT:     // Top-Left: The default
		break;
	case CA_TOPRIGHT:    // Top-Right
		{
			rcPopupAlign.left  = m_rcPopup.left - m_rcPopup.Width();
			rcPopupAlign.right = m_rcPopup.left - m_rcPopup.Width();
		}
		break;
	case CA_BOTTOMLEFT:  // Bottom-Left
		{
			rcPopupAlign.top    = m_rcPopup.top - m_rcPopup.Height() 
				- (m_nTextHeight + ::GetSystemMetrics(SM_CYBORDER)*3);
			rcPopupAlign.bottom = m_rcPopup.top - m_rcPopup.Height() 
				- (m_nTextHeight + ::GetSystemMetrics(SM_CYBORDER)*3);
		}
		break;
	case CA_BOTTOMRIGHT: // Bottom-Right
		{
			// Move the popup window (this object) to the up...
			rcPopupAlign.top    = m_rcPopup.top - m_rcPopup.Height() 
				- (m_nTextHeight + ::GetSystemMetrics(SM_CYBORDER)*3);
			rcPopupAlign.bottom = m_rcPopup.top - m_rcPopup.Height() 
				- (m_nTextHeight + ::GetSystemMetrics(SM_CYBORDER)*3);
			
			// ... and left
			rcPopupAlign.left  = m_rcPopup.left - m_rcPopup.Width();
			rcPopupAlign.right = m_rcPopup.left - m_rcPopup.Width();
		}
		break;
	}
	
	CString strClass;
	// 5. Register a window class. Basically, we need only the background brush here
	try
	{
		strClass = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS,
			::LoadCursor(NULL, IDC_ARROW),
			(HBRUSH)::GetStockObject(WHITE_BRUSH),
			::LoadIcon(NULL, IDI_APPLICATION));
	}
	catch (CResourceException* pEx)
	{
		AfxMessageBox(_T("Couldn't register class! (Already registered?)"));  //TODO
		pEx->Delete();
	}
	
	TCHAR ChNil = '\0';
	// 6. And, create the actual window based on registered class
    if (!CMiniFrameWnd::Create(strClass, &ChNil, WS_BORDER | WS_POPUP |
		(m_pCaller != NULL ? MFS_SYNCACTIVE | MFS_THICKFRAME : 0), 
		/*m_rcPopup*/rcPopupAlign,
		m_pControl->GetParentFrame()))
    {
        return FALSE;
    }
	
	// 7. Set the auto-completion mode for the caret to be visible, if the caller
	//    is a view.
	if (m_bViewCaller && m_pCaller != NULL)
	{
		COwmSciteView* pView = reinterpret_cast<COwmSciteView*>(m_pCaller);
		ASSERT(pView);						// It must be valid!
		pView->SetExtAutoCompletionMode(TRUE); // Tell the view autocomplete is started!
	}
	
	// 8. The control is normally created as a child of another window, usually the
	//    caller. However, we need to make it a child of the popup window, so set
	//    its parent to *this* window, and make it visible now.
    m_hPrevParentCtrl = m_pControl->SetParent(this)->GetSafeHwnd();
    m_pControl->SetWindowPos(NULL, 0, 0, 0, 0, 
		SWP_NOZORDER | SWP_NOSIZE | SWP_SHOWWINDOW);
	
	// 9. Make this window also visible.
    ShowWindow(SW_SHOW);
	
	// 10. Tell the event handler that we are not visible
    if (m_pEvents != NULL)
    {
        m_pEvents->OnShow();
    }
	
	// Finally! we must be up there beautifully displayed :-)
    return TRUE;
}

void CCrystalPopupWnd::EndPopup(BOOL bAbort, BOOL bSetFocus)
{
	if (m_bDoingToolTips)
		return;

	CString strSelText = _T("");
	int     nSelData   = -1;
    if (m_pEvents != NULL)
    {
        m_pEvents->OnHide(bAbort, strSelText, nSelData);
    }
    ASSERT(m_pControl->m_hWnd != NULL);
	
    m_pControl->ShowWindow(SW_HIDE);
	
    ASSERT(m_hPrevParentCtrl != NULL);
    ::SetParent(m_pControl->m_hWnd, m_hPrevParentCtrl);
	
    // Do a local copy of the pointer because DestroyWindow delete objet
    // and m_pCaller will not be valid after
    CWnd* pCaller = m_pCaller;

	// make a copy...
	INSERTTYPE xInsertType = m_xInsertType;
	
    if (!m_bDestroy)
    {
		m_wndTips.DestroyWindow();
        m_bDestroy = TRUE;
        DestroyWindow();
    }
					  
    if (bSetFocus && pCaller != NULL)
    {
        pCaller->SetFocus();
    }

	// Reset the auto-completion mode
	if (m_bViewCaller && pCaller != NULL)
	{
		COwmSciteView* pView = reinterpret_cast<COwmSciteView*>(pCaller);
		ASSERT(pView);
		if (!bAbort)
		{
			switch(xInsertType)
			{
			case PLIT_NONE:
				break;
				
			/*case PLIT_COMPLETEWORD:
				{
					if (!strSelText.IsEmpty())
					{
						VERIFY(pView->SetCurrentWord(strSelText, FALSE, 
							CE_ACTION_COMPLETEWORD));
					}
				}
				break;
				
			case PLIT_INSERTWORD:
				{
					if (!strSelText.IsEmpty())
					{
						VERIFY(pView->InsertText(strSelText));
					}
				}
				break;
				
			case PLIT_AUTOCOMPLETE:
				{
					if (nSelData != -1)
					{
						VERIFY(pView->InsertAutoCompleteText(nSelData));		
					}
				}
				break;*/
				
			case PLIT_TEMPLATE:
				{
					if (nSelData != -1)
					{
						VERIFY(pView->InsertTemplateText(nSelData));		
					}
				}
				break;

			} //switch
		} //!bAbort 
		
		pView->SetExtAutoCompletionMode(FALSE);
		return;
	}
}

int CCrystalPopupWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMiniFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Create the tooltip windows, it will be invisible initially.
	CRect rcWin;
	GetWindowRect(rcWin);
	//TODO--Create different orientations for the tooltip support
	m_wndTips.Create(CPoint(rcWin.right + 10, rcWin.top), this);
	
	return 0;
}

void CCrystalPopupWnd::DoToolTip(const CString &strTipText, BOOL bSingleLine)
{
	// Prevent the popup window from exiting, tell it we just doing tooltip.
	// This is necessary because with the display of the tooltip, this window
	// will loose the focus and this will normally cause it to terminate.
	m_bDoingToolTips = TRUE;
	// For an empty tooltip text, hide the tooltip window
	if (strTipText.IsEmpty())
	{
		m_wndTips.SetWindowPos(NULL, -1, -1, -1, -1, 
			SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE |
			SWP_NOACTIVATE | SWP_HIDEWINDOW);
		//m_wndTips.ShowWindow(SW_HIDE);
	}
	else
	{
		// ...the tooltip window may have being hidden.
		if (!m_wndTips.IsWindowVisible())
		{
			m_wndTips.SetWindowPos(NULL, -1, -1, -1, -1, 
			SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | 
			SWP_NOACTIVATE | SWP_SHOWWINDOW);
			// m_wndTips.ShowWindow(SW_SHOW);
		}

		if (m_wndTips.SetText(strTipText, bSingleLine))
		{
			// restore the focus to the listbox/listview control
//			m_pEvents->OnShow();
			if (m_pControl != NULL)
				m_pControl->SetFocus();
		}
	}
	
	// Now, the popup window can exit if necessary
	m_bDoingToolTips = FALSE;
}
