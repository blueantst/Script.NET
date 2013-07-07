////////////////////////////////////////////////////////////////////////////////
//	File:		CPopupListBox.h
//	Version:	1.0.0.0
//	Created:	16-May-2000
//
//	Author:		   TextCenter Update Team
//	Website:	   http://www.ToolsCenter.org/TextCenter
//  Mailing List:  http://www.eGroups.com/Group/TextCenter
//
//	Interface of the CPopupListBox class, a part of TextCenter - syntax
//	coloring text editor.
//
//  Purpose: 
//      This class, derived from standard MFC CListBox class, is the base class
//      for list of items in the auto-completion popup window.
//      It is a owner-drawn list box, with images to indicate the type of list
//      item, and implements the CPopupEventListener interface to process events
//      sent from the popup window.
//
//  Credit:
//
//	You are free to use, distribute or modify this code.
////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CPOPUPLISTBOX_H__A7F18664_FE00_11D3_8624_955EDCC5764F__INCLUDED_)
#define AFX_CPOPUPLISTBOX_H__A7F18664_FE00_11D3_8624_955EDCC5764F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPopupListBox.h : header file
//

#include "CCrystalPopupWnd.h"

enum
{
	ACI_NONE,				// 空
	ACI_KEYWORDEX,			// 扩展关键字
	ACI_FILE,				// 文件
	ACI_CLASS,				// 类
	ACI_KEYWORD,			// 关键字
	ACI_CLASS_FUNC,			// 方法
	ACI_CLASS_PRIVFUNC,		// 私有方法
	ACI_CLASS_VAR,			// 变量
	ACI_CLASS_PRIVVAR,		// 私有变量
};

/////////////////////////////////////////////////////////////////////////////
// CPopupListBox window

class CPopupListBox : public CListBox
{
// Construction
public:
	CPopupListBox();

// Attributes
public:
	// A pointer to the parent popup window, except when being used for
	// other purposes
	CCrystalPopupWnd* m_pPopWnd;
	BOOL              m_bResetOnHide;

// Operations
public:
	// Methods to obtained information about the current selected item
    int GetSelectedItem() const;		    //... the selected item index
	int GetSelectedItemData() const;		//... the selected item data
	// Searches and selects the string "strWord" march in the list box control
	void DoIncrementalSearch(LPCTSTR strWord);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopupListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPopupListBox();

	// Overriden to take care of some internal clean ups.
	void ResetContent();
	// Use this if tooltip support is not required for the listbox
	int AddString(LPCTSTR lpszItem, int nImage = ACI_NONE);
	// For support of tooltips of items, use the following...
	int AddString(LPCTSTR lpszItem, LPCTSTR lpszTooltip, int nImage = ACI_NONE);
	// Returns the current maximum width of the list box control
	int GetMaxWidth() const;
    void AutoSizeWindow();

	// Generated message map functions
protected:
	int InternalAddString(const CString& strText);

	// Get NONCLIENTMETRICS info: ctor calls SystemParametersInfo.
	class CPopupNonClientMetrics : public NONCLIENTMETRICS 
	{
	public:
		CPopupNonClientMetrics() 
		{
			cbSize = sizeof(NONCLIENTMETRICS);
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, this, 0);
		}
	};

	CFont             m_font;
	int               m_nMaxWidth;		// The maximum width of the list control
	int               m_nMaxWidthIdx;	// Index of the item with the maximum width
	CStringArray      m_saTooltips;     // container of the items tooltip texts
	BOOL              m_bDoTooltips;    // TRUE if supporting tooltip, FALSE otherwise
	CArray<int, int>  m_aACIIndex;      // Image index for the items
	CImageList        m_ilBitmaps;      //	The images for the owner-drawn list items
	int               m_nLastSelected;	// Index of the last selected item in the list
	static const int  s_nLeftMargin;    // Margin for the bitmap

//	BOOL    m_bFirstTime;

	//{{AFX_MSG(CPopupListBox)
	afx_msg void OnSelchange();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

    // CCrystalPopupWnd events
public:
    class XPopupCtrlEvent : public CPopupEventListener
    {
    public:
        virtual BOOL OnInit();
        virtual void OnShow();
        virtual BOOL OnHide(BOOL bAbort, CString& strSel, int& nIndex);
        virtual int  OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
        virtual int  OnLButtonDblClk(UINT nFlags, CPoint point);
        virtual int  OnLButtonDown(UINT nFlags, CPoint point);
        virtual int  OnLButtonUp(UINT nFlags, CPoint point);

        virtual CPopupEventListener* GetInterfaceOf(HWND hWnd);

    } m_xPopupCtrlEvent;

	// Make the event interface a friend for *easy* access to the protected
	// and private methods and members
	friend class XPopupCtrlEvent;
};

////////////////////////////////////////////////////////////////////////////////
// CPopupListBox inlines

inline int CPopupListBox::GetMaxWidth() const 
{ 
	return m_nMaxWidth; 
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPOPUPLISTBOX_H__A7F18664_FE00_11D3_8624_955EDCC5764F__INCLUDED_)
