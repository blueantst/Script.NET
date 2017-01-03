////////////////////////////////////////////////////////////////////////////////
//	File:		CPopupListBox.cpp
//	Version:	1.0.0.0
//	Created:	13-Nov-2000
//
//	Author:		   TextCenter Project Team.
//	Website:	   http://www.ToolsCenter.org/TextCenter
//  Mailing List:  http://www.eGroups.com/Group/TextCenter
//
//	Implementation of the CPopupListBox class, a part of TextCenter -  a syntax
//	coloring MFC-based text editor component.
//
//  Purpose: 
//      This class, derived from standard MFC CListBox class, is the base class
//      for list of items in the auto-completion popup window.
//      It is a owner-drawn list box, with images to indicate the type of list
//      item, and implements the CPopupEventListener interface to process events
//      sent from the popup window.
//
//  Credits:
//
//	You are free to use, distribute or modify this code.
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CPopupListBox.h"
#include "PTerminal.h"
#include "resource.h" //TODO


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int CPopupListBox::s_nLeftMargin = 24;

/////////////////////////////////////////////////////////////////////////////
// CPopupListBox

CPopupListBox::CPopupListBox()
{
	// create font -- use system tooltip font
	//CPopupNonClientMetrics ncm;
	//m_font.CreateFontIndirect(&ncm.lfStatusFont);

	// create font -- 使用标准字体 modified by xieyongfang
	LOGFONT lfFont;
	memset(&lfFont, 0, sizeof(lfFont));
	lstrcpy(lfFont.lfFaceName, _T("FixedSys"));
	lfFont.lfHeight = 0;
	lfFont.lfWeight = FW_NORMAL;
	lfFont.lfItalic = FALSE;
	lfFont.lfCharSet = GB2312_CHARSET;
	lfFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lfFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lfFont.lfQuality = DEFAULT_QUALITY;
	lfFont.lfPitchAndFamily = DEFAULT_PITCH;
	m_font.CreateFontIndirect(&lfFont);

	// Just make sure there is not gabbage
	m_saTooltips.RemoveAll();
	m_aACIIndex.RemoveAll();
	m_bDoTooltips   = FALSE;
	m_nLastSelected = 0;
	m_pPopWnd       = NULL;
	m_bResetOnHide  = TRUE;

//	m_bFirstTime = TRUE;
}


CPopupListBox::~CPopupListBox()
{
	// Clean up the tooltip list
	m_saTooltips.RemoveAll();
	m_aACIIndex.RemoveAll();
	m_bDoTooltips           = FALSE;
	m_nLastSelected         = 0;
//	m_ilBitmaps.
}


BEGIN_MESSAGE_MAP(CPopupListBox, CListBox)
	//{{AFX_MSG_MAP(CPopupListBox)
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelchange)
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopupListBox message handlers

void CPopupListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	ASSERT(lpDIS->CtlType == ODT_LISTBOX);	 // We must be drawing a listbox
	if (lpDIS->itemID == LB_ERR) 			 // The item must be valid
		return; 			   
   	
	CDC dc;
	CBrush	brBk;
	CRect rcTemp(lpDIS->rcItem); 
	dc.Attach(lpDIS->hDC);	   			    // To do the drawing in MFC's CDC

	// Save these value to restore them when done drawing.
	COLORREF crOldTextColor = dc.GetTextColor();
	COLORREF crOldBkColor   = dc.GetBkColor();
	
	COLORREF clText; 					   // For the item text color
	
	// Set the background and text color for the selected item...
	if (lpDIS->itemState & ODS_SELECTED)  
	{ 
		// I prefer the VC++ style black highlighting, 
		// you may use the system color
		brBk.CreateSolidBrush(RGB(0, 0, 0)); 
		// Or, uncomment this and comment the above for the system color
		// brBk.CreateSysColorBrush(COLOR_HIGHLIGHT); 
		clText = ::GetSysColor(COLOR_HIGHLIGHTTEXT); 
	} 
	else  // ... or the non-selected item
	{ 
		brBk.CreateSysColorBrush(COLOR_WINDOW); 
		clText = ::GetSysColor(COLOR_WINDOWTEXT); 
	} 
	
	CRect rcBk(rcTemp);
	rcBk.left = rcBk.left + s_nLeftMargin;	     // Leave magin for the bitmap

	// For either the entire or the selection...
	if (lpDIS->itemAction & (ODA_DRAWENTIRE | ODA_SELECT))
		dc.FillRect(&rcBk, &brBk); 

	// For the focused item...draw the focus rectangle
	if (lpDIS->itemAction & ODA_FOCUS)
	{
		CRect rcBk2(lpDIS->rcItem);
		rcBk2.left = rcBk2.left + s_nLeftMargin;
		dc.DrawFocusRect(&rcBk2); 
	}

	int		 OldBkMode = dc.SetBkMode(TRANSPARENT); 
	COLORREF clOldText = dc.SetTextColor(clText); 
	
	CString strText;
	GetText(lpDIS->itemID, strText);
	
	int h = rcTemp.bottom - rcTemp.top;
	rcTemp.left += h + 8;//4;

	// Draw the indented text of the item...
	HFONT OldFont = 
		static_cast<HFONT>(dc.SelectObject(static_cast<HFONT>(m_font)));
	dc.DrawText(strText, -1, &rcTemp, DT_LEFT | DT_VCENTER |	
		DT_NOPREFIX | DT_SINGLELINE);
	dc.SelectObject(OldFont);	            // Restore the old font 
	
	rcTemp.left = lpDIS->rcItem.left;	    // Bitmap to the extreme left
	// Obtain the data attached to this item being drawn, 
	// it contains the bitmap information
	int nData = static_cast<int>(GetItemData(lpDIS->itemID));

	int nImageIndex = ACI_NONE;			   // Default to plain bitmap
	if (nData != LB_ERR)
		nImageIndex = m_aACIIndex[nData];

	// Now draw the bitmap, at the extreme left
	m_ilBitmaps.Draw(&dc, nImageIndex, CPoint(rcTemp.left + 2, rcTemp.top), ILD_NORMAL);
	
	dc.SetTextColor(clOldText);			  // Just being a good citizen...
	dc.SetBkMode(OldBkMode);			  // ...not really needed.
	
	// Reset the background color and the text color back to their
	// original values.
	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);

	dc.Detach();
}

void CPopupListBox::OnSelchange() 
{
	//ASSERT(m_pPopWnd != NULL);
	if(m_pPopWnd == NULL)
	{
		return;
	}

	int nIndex = GetSelectedItem();
	// If valid update the last selected index.
	if (nIndex != -1)
		m_nLastSelected = nIndex;
	else
	{
		m_pPopWnd->DoToolTip(_T(""));
		return;
	}

	// Display the tooltip, if supported and there is there in the buffer
	if (m_bDoTooltips && m_saTooltips.GetSize() != 0)
	{
		int nData = GetSelectedItemData();
		if (nData != -1)
		{
			CString strTooltip = m_saTooltips[nData];
			//TRACE("The tooltip index is %d, the text is %s\n", nData, strTooltip);
			// Determine whether the tooltips display should be a single or
			// multiple lines---try to find the occurance of the line breaks "\r\n"
			BOOL bSingleLine = (strTooltip.Find(_T("\r\n")) == -1);
			m_pPopWnd->DoToolTip(strTooltip, bSingleLine);
		}
		else //...if selected item has not tooltip text, hide the tooltip window
		{
			m_pPopWnd->DoToolTip(_T(""));
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//

int CPopupListBox::GetSelectedItem() const
{
	int nCurSel = GetCurSel();
	if (nCurSel != LB_ERR)	
		return nCurSel;

	return -1;
}

int CPopupListBox::GetSelectedItemData() const
{
	int nCurSel = GetCurSel();
	if (nCurSel != LB_ERR)
	{
		int nItemData = static_cast<int>(GetItemData(nCurSel));
		if (nItemData != LB_ERR)
			return nItemData;
	}

	return -1;
}

int CPopupListBox::InternalAddString(const CString &strText)
{
	int nIndex = CListBox::AddString(strText);

//	if (nIndex != LB_ERR)
//	{
//		TRACE(_T("CPopupListBox: \'%s\' added at index %d\n"), strText, nIndex);
//	}

	
	// We support vertical scroll bar, however, hide it unless
	// there is a need to scroll
//	SCROLLINFO scrollInfo;
//	memset(&scrollInfo, 0, sizeof(SCROLLINFO));
//	scrollInfo.cbSize = sizeof(SCROLLINFO);
//	scrollInfo.fMask  = SIF_ALL;
//	GetScrollInfo(SB_VERT, &scrollInfo, SIF_ALL);
//	
//	int nScrollWidth = 0;
//	if (GetCount() > 1 && ((int)scrollInfo.nMax >= (int)scrollInfo.nPage))
//	{
//		nScrollWidth = GetSystemMetrics(SM_CXVSCROLL);
//	}
	
	SIZE      sSize;
	CClientDC dcClient(this);
	
	// Determine the extent of the longest text in the list
	CFont* pOldFont = dcClient.SelectObject(GetFont());
	::GetTextExtentPoint32(dcClient.m_hDC, strText, strText.GetLength(), &sSize);
	m_nMaxWidth = max(m_nMaxWidth, (int)sSize.cx);
	CRect rcWin;
	rcWin.SetRectEmpty();
	GetWindowRect(&rcWin);

	int nScrollLen = ::GetSystemMetrics(SM_CXVSCROLL)/2;
	if (rcWin.Width() < (m_nMaxWidth + nScrollLen))
	{
		rcWin.right = rcWin.left + m_nMaxWidth + nScrollLen; // Allow an extract width
		MoveWindow(&rcWin);
	}
//	SetHorizontalExtent(m_nMaxWidth + 6);
	dcClient.SelectObject(pOldFont);
	
	return nIndex;
}

int CPopupListBox::AddString(LPCTSTR lpszItem, int nImage/* = ACI_NONE*/)
{
	ASSERT(m_bDoTooltips == FALSE);  // You cannot support tooltip by this method, use
	                         // the other AddString(..., ...) always for this.

	int nItemIndx = InternalAddString(lpszItem);
	if (nItemIndx != LB_ERR)
	{
		int nImageIndx = m_aACIIndex.Add(nImage);
		
		// Since the list box may be sorted, we are using the item index returned
		// from adding the item to add tooltip tracking index
		int nSuccess = SetItemData(nItemIndx, nImageIndx);
		ASSERT(nSuccess != LB_ERR);  // An error occured when adding the item data
	}

	return nItemIndx;

}

int CPopupListBox::AddString(LPCTSTR lpszItem, LPCTSTR lpszToolTip, 
							 int nImage/* = ACI_NONE*/)
{
	// We are supporting tooltips, so indicate this
	if (!m_bDoTooltips)
		m_bDoTooltips = TRUE;

	if (m_saTooltips.GetSize() != 0)
	{
		// Since tooltip text are already added to the list, it should be
		// indicated that we are support tooltips by now...
		ASSERT(m_bDoTooltips);
	}

	int nItemIndx = InternalAddString(lpszItem);
	if (nItemIndx != LB_ERR)
	{
		int nTipIndx   = m_saTooltips.Add(lpszToolTip);
		int nImageIndx = m_aACIIndex.Add(nImage);
		
		ASSERT(nImageIndx == nTipIndx);  // Must be the same
		
		// Since the list box may be sorted, we are using the item index returned
		// from adding the item to add tooltip tracking index
		int nSuccess = SetItemData(nItemIndx, nTipIndx);
		ASSERT(nSuccess != LB_ERR);  // An error occured when adding the item data
	}

	return nItemIndx;
}

void CPopupListBox::ResetContent()
{
	CListBox::ResetContent();
	
	m_nMaxWidth = 0;
//	SetHorizontalExtent(0);
	CRect rcWin = CRect(0, 0, 150, 200);
	MoveWindow(&rcWin);

	// Clean up the tooltip list
	m_saTooltips.RemoveAll();
	m_aACIIndex.RemoveAll();
	m_bDoTooltips   = FALSE;
	m_nLastSelected = 0;
	m_pPopWnd       = NULL;
	m_bResetOnHide  = TRUE;
}

int CPopupListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
	// TODO: Add your code to determine the sorting order of the specified items
	// return -1 = item 1 sorts before item 2
	// return 0 = item 1 and item 2 sort the same
	// return 1 = item 1 sorts after item 2

   ASSERT(lpCompareItemStruct->CtlType == ODT_LISTBOX);
   LPCTSTR lpszText1 = (LPCTSTR) lpCompareItemStruct->itemData1;
   ASSERT(lpszText1 != NULL);
   LPCTSTR lpszText2 = (LPCTSTR) lpCompareItemStruct->itemData2;
   ASSERT(lpszText2 != NULL);

//   TRACE0("CPopupListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct)\n");

   return _tcscmp(lpszText1, lpszText2); 
   
//	return 0;
}

void CPopupListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// Get the default list height...
	lpMeasureItemStruct->itemHeight = 18;	 //TODO--should be customizable
}

void CPopupListBox::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CListBox::DeleteItem(lpDeleteItemStruct);
}

int CPopupListBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	get_dll_resource();
	VERIFY(m_ilBitmaps.Create(IDB_EDIT_AUTOCOMPLETE, 16, 4, /*ILC_COLOR8*/ILC_COLOR32));
	reset_dll_resource();

	return 0;
}

void CPopupListBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
//	TRACE0("CPopupListBox::OnKeyDown(nChar, nRepCnt, nFlags)\n");

	// Prevent scrolling with the left and right arrow keys.
	if ((nChar == VK_LEFT) || (nChar == VK_RIGHT))
		return;
	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CPopupListBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	UNREFERENCED_PARAMETER(nFlags);
	UNREFERENCED_PARAMETER(nRepCnt);
	UNREFERENCED_PARAMETER(nChar);
	//TODO--There could be a better way to do the following...find out!

	// Prevent the list box control from processing the character messages
	// This is to prevent the default incremental search provided by the
	// standard list box, when a character is type while the control has a focus.

	// CListBox::OnChar(nChar, nRepCnt, nFlags);
}

void CPopupListBox::DoIncrementalSearch(LPCTSTR strWord)
{
	ASSERT(m_pPopWnd != NULL);
	int nLength = lstrlen(strWord);
	//TRACE1("CPopupListBox::DoIncrementalSearch-strWord: %s\n", strWord);
	if (nLength != 0)
	{
			// Search and select the match for the given word...
			int nReturn = LB_ERR; //SelectString(-1, strWord);	//TODO-search not accurate
			for (int i = 0; i < GetCount(); i++)
			{
				TCHAR szBuffer[_MAX_PATH] = {0};
				GetText(i, szBuffer);
				//TRACE1("CPopupListBox::DoIncrementalSearch-szBuffer: %s\n", szBuffer);
				if (_tcsnccmp(szBuffer, strWord, nLength) == 0)
				{
					nReturn = i;
					//TRACE1("CPopupListBox::DoIncrementalSearch-nReturn: %d\n", nReturn);
					break;
				}
			}
			// ...if not found, deselect the current item.
			if (nReturn == LB_ERR)
				SetCurSel(-1);
			else
				SetCurSel(nReturn);
	}
	else
	{
		// ...if the search string is empty, deselect the current item.
		SetCurSel(-1);
	}

	// The following will force the tooltips to be displayed if required.
	OnSelchange();
}

void CPopupListBox::AutoSizeWindow()
{
	CRect rcWin;
	GetWindowRect(&rcWin);

	// Client area must be large enough to display about 10 items, 
	// unless there are fewer than 10 items.
	rcWin.bottom = rcWin.top + GetItemHeight(0) * min(10, GetCount()) + 
		::GetSystemMetrics(SM_CYHSCROLL);

	// Resize the window
	MoveWindow(&rcWin);
}

///////////////////////////////////////////////////////////////////////////
//

BOOL CPopupListBox::XPopupCtrlEvent::OnInit()
{
    METHOD_PROLOGUE_(CPopupListBox, PopupCtrlEvent);

//    TRACE("CPopupListBox::OnInit ()\n");
//    pThis->ModifyStyle(0, WS_BORDER, SWP_DRAWFRAME);

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//

void CPopupListBox::XPopupCtrlEvent::OnShow()
{
    METHOD_PROLOGUE_(CPopupListBox, PopupCtrlEvent);

//    TRACE("CPopupListBox::OnShow()\n");
    pThis->SetFocus();
	// For a tooltip to be shown, it might be the first time the control
	// is being displayed...
	pThis->OnSelchange();
}

///////////////////////////////////////////////////////////////////////////////
//

BOOL CPopupListBox::XPopupCtrlEvent::OnHide(BOOL bAbort, CString& strSel, int& nIndex)
{
    METHOD_PROLOGUE_(CPopupListBox, PopupCtrlEvent);

//    TRACE(_T("CPopupListBox::OnHide (Abort = %s)\n"), bAbort ? _T("TRUE") : _T("FALSE"));
		
	// For non-abort actions, select the appropriate method and insert
	// the required text into the view.
	if (!bAbort)
	{
		pThis->GetText(pThis->GetSelectedItem(), strSel);
		nIndex = pThis->GetSelectedItemData();
	}
	else
	{
		strSel = _T("");
		nIndex = -1;
	}

	// This is better to release some resources to the program, since each popup
	// process will fill out the contents of the list box.
	if (pThis->m_bResetOnHide)
		pThis->ResetContent();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//

int CPopupListBox::XPopupCtrlEvent::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	UNREFERENCED_PARAMETER(nFlags);
	UNREFERENCED_PARAMETER(nRepCnt);
    METHOD_PROLOGUE_(CPopupListBox, PopupCtrlEvent);

	// Autocomplete hates delete key!
	if (nChar == VK_DELETE)
		return CCrystalPopupWnd::PU_ABORT;

	// For Enter or Space or tab ends the autocompletion process
    if ((nChar == VK_RETURN) || (nChar == VK_SPACE) || (nChar == VK_TAB))
    {
		// First find out if an item is currently selected...
		int nSelItem = pThis->GetSelectedItem();
		//... if no currently selected item exist...
		if (nSelItem == -1)
		{
			//... select the last selected item...
			pThis->SetCurSel(pThis->m_nLastSelected);
			//... and tell the popup container to stay still.
			return CCrystalPopupWnd::PU_NOACTION;
		}

        return CCrystalPopupWnd::PU_END;
    }

	// For others, no special action be default
    return CCrystalPopupWnd::PU_NOACTION;
}
   
///////////////////////////////////////////////////////////////////////////////
//

int CPopupListBox::XPopupCtrlEvent::OnLButtonDblClk(UINT, CPoint pt)
{
	UNREFERENCED_PARAMETER(pt);
    METHOD_PROLOGUE_(CPopupListBox, PopupCtrlEvent);

	// Comment the following lines(5) to close by simple instead of double click
    int  hItem;
    if ((hItem = pThis->GetSelectedItem()) != -1)
    {
        return CCrystalPopupWnd::PU_END;
    }
	
	return CCrystalPopupWnd::PU_NOACTION;
}

///////////////////////////////////////////////////////////////////////////////
//

int CPopupListBox::XPopupCtrlEvent::OnLButtonDown(UINT, CPoint pt)
{
	UNREFERENCED_PARAMETER(pt);
    METHOD_PROLOGUE_(CPopupListBox, PopupCtrlEvent);

    return CCrystalPopupWnd::/*PU_NOSEND*/PU_NOACTION;
}

///////////////////////////////////////////////////////////////////////////////
//

int CPopupListBox::XPopupCtrlEvent::OnLButtonUp(UINT, CPoint pt)
{
	UNREFERENCED_PARAMETER(pt);
    METHOD_PROLOGUE_(CPopupListBox, PopupCtrlEvent);

	// Uncomment next line to close by simple click instead of double click
//    int  hItem;
//    if ((hItem = pThis->GetSelectedItem()) != -1)
//    {
//        return CCrystalPopupWnd::PU_END;
//    }

    return CCrystalPopupWnd::PU_NOACTION;
}

///////////////////////////////////////////////////////////////////////////////
// 

CPopupEventListener* CPopupListBox::XPopupCtrlEvent::GetInterfaceOf(HWND)
{
    // Not used here because this control doesn't include other controls
    return NULL;
}
