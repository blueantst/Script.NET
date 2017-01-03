#if !defined(AFX_CCRYSTALPOPUPWND_H__2A5F3696_EF67_11D3_8620_C575824E504F__INCLUDED_)
#define AFX_CCRYSTALPOPUPWND_H__2A5F3696_EF67_11D3_8620_C575824E504F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CCrystalPopupWnd.h : header file
//

#include "CPopupListTips.h"

// Options to determine the way text is inserted into the view
enum INSERTTYPE
{
	PLIT_NONE,			  // No insert action is performed
	PLIT_COMPLETEWORD,	  // Set the current word, a "Complete Word" support
	PLIT_INSERTWORD,	  // Set the current word, an "Insert Word" support
	PLIT_AUTOCOMPLETE,	  // Insert code snippet, an "Auto-Complete" support
	PLIT_TEMPLATE		  // Insert template, a "Language template" support
};

interface CPopupEventListener;

///////////////////////////////////////////////////////////////////////////////
// Usefull macros
//
#define KEYDOWN(Key)               ((GetKeyState(Key)&0x8000)!=0)
#define VIRTUAL_KEY(nChar,nFlags)  (((nFlags)&0x0100)||(nChar)<=VK_ESCAPE||(nChar)>=VK_F1)
#define ANSI_KEY(nChar)            ((nChar)>VK_ESCAPE)
#define VK_CTRL_A                  1
#define VK_CTRL_Z                  26
#define VK_ALT                     VK_MENU
// To not have to type the 4 fields !
#define EXPAND_RECT(rc)            (rc).left,(rc).top,(rc).right,(rc).bottom


///////////////////////////////////////////////////////////////////////////////
//
class CClientRect : public CRect
{
public:
    CClientRect(HWND hWnd)
    {
        ::GetClientRect(hWnd, this);
    };
    CClientRect(const CWnd* pWnd)
    {
        ::GetClientRect(pWnd->GetSafeHwnd(), this);
    };
};


///////////////////////////////////////////////////////////////////////////////
//
class CWindowRect : public CRect
{
public:
    CWindowRect(HWND hWnd)
    {
        ::GetWindowRect(hWnd, this);
    };
    CWindowRect(const CWnd* pWnd)
    {
        ::GetWindowRect(pWnd->GetSafeHwnd(), this);
    };
};


/////////////////////////////////////////////////////////////////////////////
// CCrystalPopupWnd frame

class CCrystalPopupWnd : public CMiniFrameWnd
{
	DECLARE_DYNCREATE(CCrystalPopupWnd)
protected:
	CCrystalPopupWnd();   // protected constructor used by dynamic creation

// Attributes
public:
    // Return value of OnKeyDown, OnLButtonDown and OnLButtonUp callbacks
    enum
    {
        PU_NOACTION  = 0,  // No action--do nothing
		PU_NOSEND    = 1,  // Do not send the current message to the control
		PU_END       = 2,  // Hide popup normally
		PU_ABORT     = 3,  // Hide popup with flag 'abort'
    };

	// Cursor/Caret position/alignment '@' with respect to popup window
	enum CURSORALIGN
	{
		CA_NONE,			   // Do nothing

		CA_TOPLEFT,			   // Top-Left position @ ____
			                   //                    |    |
							   //				     |____|

		CA_TOPRIGHT,	       //   ____ @	Top-Right position 
			                   //  |    |
							   //  |____|

		CA_BOTTOMLEFT,	       //                         ____
			                   //                        |    |
							   //					     |____|
							   // Bottom-Left position  @

		CA_BOTTOMRIGHT	       //   ____
			                   //  |    |
							   //  |____|
							   //  	     @  Bottom-right position 
	};

	void SetTextHeight(int nHeight);
	int  GetTextHeight() const;

	// The following are used to manage the type of the caller
	// generic window or a view?
	void SetCallerType(BOOL bIsView);
	BOOL IsViewCaller() const;

	// The following are used to determine whether the view, if is the caller
	// should be send the WM_KEYDOWN AND WM_CHAR messages
	void SetSendViewKeys(BOOL bSendIt);
	int  GetSendViewKeys() const;

// Operations
public:
	INSERTTYPE GetInsertType() const;
	void SetInsertType(INSERTTYPE xType);

	void DoToolTip(const CString& strTipText, BOOL bSingleLine = TRUE);
    BOOL Display(CWnd* pControl, CWnd* pCaller, CRect& rcCaller, 
		CURSORALIGN xAlignment = CA_TOPLEFT,
		CPopupEventListener* pEvents = NULL, 
		BOOL bBottomAlign = TRUE, 
		BOOL bResizable = FALSE);

	CPopupListTips m_wndTips;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCrystalPopupWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
    void EndPopup(BOOL bAbort = FALSE, BOOL bSetFocus = TRUE);

protected:
	virtual ~CCrystalPopupWnd();
//    void EndPopup(BOOL bAbort = FALSE, BOOL bSetFocus = TRUE);

    CWnd*                  m_pCaller;			// A pointer to the caller
    CWnd*                  m_pControl;			// A pointer to the child control
    CPopupEventListener*   m_pEvents;			// The child control event processor
    HWND                   m_hPrevParentCtrl;
    BOOL                   m_bDestroy;
	BOOL                   m_bResizable;
	CRect                  m_rcPopup;
	BOOL                   m_bDoingToolTips;	// Support tooltips for items?
	int                    m_nTextHeight;		// The view text height for align calc
	BOOL                   m_bViewCaller;		// Is the caller a view?
	BOOL                   m_bSendViewKeys;	    // send the WM_KEYDOWN/WM_CHAR to view?

	INSERTTYPE             m_xInsertType; // Determines the form of text insert in view

	// Generated message map functions
	//{{AFX_MSG(CCrystalPopupWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnCancelMode();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);

	DECLARE_MESSAGE_MAP()
};

///////////////////////////////////////////////////////////////////////////////////
// The CCrystalPopupWnd class inline methods

inline void CCrystalPopupWnd::SetTextHeight(int nHeight) 
{
	m_nTextHeight = nHeight;
}

inline int CCrystalPopupWnd::GetTextHeight() const 
{
	return m_nTextHeight;
}

inline void CCrystalPopupWnd::SetCallerType(BOOL bIsView)
{
	m_bViewCaller = bIsView;
}

inline BOOL CCrystalPopupWnd::IsViewCaller() const
{
	return m_bViewCaller;
}

inline void CCrystalPopupWnd::SetSendViewKeys(BOOL bSendIt)
{
	m_bSendViewKeys = bSendIt;
}

inline BOOL CCrystalPopupWnd::GetSendViewKeys() const
{
	return m_bSendViewKeys;
}

inline INSERTTYPE CCrystalPopupWnd::GetInsertType() const
{
	return m_xInsertType;
}

inline void CCrystalPopupWnd::SetInsertType(INSERTTYPE xType)
{
	m_xInsertType = xType;
}

/////////////////////////////////////////////////////////////////////////////
// CPopupEventListener interface for handling events in the control
// NOTE: This has nothing to do with COM! "interface" is defined as "struct"

interface __declspec(novtable) CPopupEventListener
{
    virtual BOOL OnInit()                                           = 0;
    virtual void OnShow()                                           = 0;
    virtual BOOL OnHide(BOOL bAbort, CString& strSel, int& nIndex)  = 0;
    virtual int  OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)   = 0;
    virtual int  OnLButtonDblClk(UINT nFlags, CPoint point)         = 0;
    virtual int  OnLButtonDown(UINT nFlags, CPoint point)           = 0;
    virtual int  OnLButtonUp(UINT nFlags, CPoint point)             = 0;
    virtual CPopupEventListener* GetInterfaceOf(HWND hWnd)          = 0;
};
 

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCRYSTALPOPUPWND_H__2A5F3696_EF67_11D3_8620_C575824E504F__INCLUDED_)
