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

#if !defined(_KCSIDEBANNERWND_H_)
#define _KCSIDEBANNERWND_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWndUtil  
{
protected:
	CWndUtil(LPCTSTR lpszClassName)
		: m_strClassName(lpszClassName)
	{
	}

	virtual ~CWndUtil()
	{
	}

public:

	virtual BOOL			RegisterWndClass()
	{
		WNDCLASS			wnd;
		HINSTANCE			hInst = AfxGetInstanceHandle();

		if ( !(::GetClassInfo( hInst, m_strClassName, &wnd )) )
		{
			wnd.style				= CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
			wnd.lpfnWndProc			= ::DefWindowProc;
			wnd.cbClsExtra			= wnd.cbWndExtra = 0;
			wnd.hInstance			= hInst;
			wnd.hIcon				= NULL;
			wnd.hCursor				= AfxGetApp()->LoadStandardCursor(IDC_ARROW);
			wnd.hbrBackground		= NULL;
			wnd.lpszMenuName		= NULL;
			wnd.lpszClassName		= m_strClassName;

			if ( !AfxRegisterClass(&wnd) )
			{
				AfxThrowResourceException();
				return FALSE;
			}
		}

		return TRUE;
	}

protected:
	// class registration/information
	CString					m_strClassName;
};

// classname definition
#define KCSB_CLASSNAME		_T("KCSideBanner")

// positioning defines
#define	KCSB_ATTACH_LEFT		1
#define	KCSB_ATTACH_TOP			2
#define	KCSB_ATTACH_RIGHT		4
#define	KCSB_ATTACH_BOTTOM		8

// icon position defines
#define	KCSB_ICON_LEFT			1
#define	KCSB_ICON_RIGHT			2
#define	KCSB_ICON_VCENTER		4
#define	KCSB_ICON_TOP			8
#define	KCSB_ICON_BOTTOM		16

// drawing defines
#define	KCSB_FILL_FLAT			1
#define KCSB_FILL_GRADIENT		2
#define	KCSB_FILL_TEXTURE		4

typedef BOOL (CALLBACK*PFNGRADFILL)(HDC, PTRIVERTEX, ULONG, PVOID, ULONG, ULONG);

/////////////////////////////////////////////////////////////////////////////
// CKCSideBannerWnd window
/////////////////////////////////////////////////////////////////////////////
class CKCSideBannerWnd : public CWnd,
					     public CWndUtil
{
	// internal structure passed to EnumChildWindows
	struct stEnum
	{
		CWnd*			pBannerWnd;			// pointer to the banner wnd
		CWnd*			pParentWnd;			// pointer to the parent of the banner
		int				nOffset;			// amount to offset siblings
		unsigned int	uFlags;				// flags
	};

public:

	/////////////////////////////////////////////////////////////////////////////
	// PUBLIC: constructor/destructor
	/////////////////////////////////////////////////////////////////////////////
	CKCSideBannerWnd();
	virtual ~CKCSideBannerWnd();

	/////////////////////////////////////////////////////////////////////////////
	// PUBLIC: methods
	/////////////////////////////////////////////////////////////////////////////
	BOOL				Attach(CWnd* pWnd, unsigned int uFlags = KCSB_ATTACH_LEFT, unsigned int uiID = 0xFFF0);
	void				SetSize(int nSize);
	void				UpdateSize();
	int					GetSize()										{ return m_nSize; }
	void				SetPosFlag(unsigned int uFlags);
	unsigned int		GetPosFlag()									{ return m_uPosFlag; }
	void				SetFillFlag(unsigned int uFlags)				{ m_uFillFlag = uFlags; if ( m_hWnd ) Invalidate(); }
	unsigned int		GetFillFlag()									{ return m_uFillFlag; }
	void				SetTitle(LPCTSTR lpszTitle)						{ m_strTitle = lpszTitle; if ( m_hWnd ) Invalidate(); }
	CString				GetTitle()										{ return m_strTitle; }
	void				SetCaption(LPCTSTR lpszCaption)					{ m_strCaption = lpszCaption; if ( m_hWnd ) Invalidate(); }
	CString				GetCaption()									{ return m_strCaption; }
	void				SetColBkg(COLORREF col)							{ m_colBkg = col; }
	COLORREF			GetColBkg()										{ return m_colBkg; }
	void				SetColBkg2(COLORREF col)						{ m_colBkg2 = col; }
	COLORREF			GetColBkg2()									{ return m_colBkg2; }
	void				SetColEdge(COLORREF col)						{ m_colEdge = col; }
	COLORREF			GetColEdge()									{ return m_colEdge; }
	void				SetColTxtTitle(COLORREF	col)					{ m_colTxtTitle = col; }
	COLORREF			GetColTxtTitle()								{ return m_colTxtTitle; }
	void				SetColTxtCaption(COLORREF col)					{ m_colTxtCaption = col; }
	COLORREF			GetColTxtCaption(COLORREF col)					{ return m_colTxtCaption; }
	void				SetEdgeOffset(CSize szOffset)					{ m_szEdgeOffset = szOffset; }
	CSize				GetEdgeOffset()									{ return m_szEdgeOffset; }
	void				SetCaptionOffset(CSize szOffset)				{ m_szCaptionOffset = szOffset; }
	CSize				GetCaptionOffset()								{ return m_szCaptionOffset; }
	void				SetCaptionFont(CFont* pFont);
	void				GetCaptionFont(LOGFONT* pFont);
	void				SetTitleFont(CFont* pFont);
	void				GetTitleFont(LOGFONT* pFont);
	bool				SetIcon(HICON hIcon, UINT uiIconPos = KCSB_ICON_RIGHT | KCSB_ICON_VCENTER, bool bSelfDelete = true );
	void				SetIconPos(UINT uiIconPos)						{ m_uIconPos = uiIconPos; if ( m_hWnd ) Invalidate(); }
	UINT				GetIconPos()									{ return m_uIconPos; }
	// added on 2003-Oct-24
	void				SetTexture(HBITMAP hBitmap, bool bSelfDelete = true);
	HBITMAP				GetTexture()									{ return m_hBkgBitmap; }
	// added on 2003-Oct-29
	void				GradientFill(CDC* pDC, COLORREF col1, COLORREF col2, CRect rect, bool bHor, bool cFlip);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKCSideBannerWnd)
	public:
	protected:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL


	// Generated message map functions
protected:

	/////////////////////////////////////////////////////////////////////////////
	// PROTECTED: methods
	/////////////////////////////////////////////////////////////////////////////
	void				UpdateLayout(int nOffset);
	
	// drawing
	virtual void		DrawBackground(CDC* pDC, CRect rect);
	virtual void		DrawTextFields(CDC* pDC, CRect rect);
	virtual void		DrawEdge(CDC* pDC, CRect rect);
	virtual void		DrawBannerIcon(CDC* pDC, CRect& rect);

	static BOOL CALLBACK	ChildEnumProc(HWND hWndChild, LPARAM lParam);

	/////////////////////////////////////////////////////////////////////////////
	// PROTECTED: attributes
	/////////////////////////////////////////////////////////////////////////////
	CWnd*				m_pOwner;
	int					m_nSize;
	UINT				m_uPosFlag;
	UINT				m_uFillFlag;
	UINT				m_uIconPos;
	CString				m_strTitle;
	CString				m_strCaption;
	HICON				m_hIcon;
	bool				m_bIconDelete;
	BITMAP				m_bmpInfo;
	HBITMAP				m_hBkgBitmap;
	bool				m_bBmpDelete;
	// added 2003-Oct-29
	PFNGRADFILL			m_pGradFill;
	HMODULE				m_hGradMod;

	// padding offsets
	CSize				m_szEdgeOffset;
	CSize				m_szCaptionOffset;

	// font data
	LOGFONT				m_lfTitle;
	LOGFONT				m_lfCaption;

	// colours
	COLORREF			m_colBkg;
	COLORREF			m_colBkg2;
	COLORREF			m_colEdge;
	COLORREF			m_colTxtTitle;
	COLORREF			m_colTxtCaption;

	//{{AFX_MSG(CKCSideBannerWnd)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_KCSIDEBANNERWND_H_)
