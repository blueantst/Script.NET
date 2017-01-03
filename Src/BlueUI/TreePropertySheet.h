// TreePropertySheet.h : header file
//
// Written by Alexander Berthold, alexander.berthold@rol3.com
//
/////////////////////////////////////////////////////////////////////////////
//
// For more informations about this file, please visit
// my home page at http://members.xoom.com/softserv/tps_index.html
//
/////////////////////////////////////////////////////////////////////////////
// CTreePropertySheet window

#include <afxtempl.h>
#include "OptionTreeCtrl.h"

#define WM_VARIFYPARAM	(WM_USER+4000)	// 属性页校验消息

enum {
	tps_item_branch,		// 创建子节点
	tps_item_node,			// 保持节点层次
	tps_item_endbranch,		// 返回父节点
	tps_item_endbranch2,	// 向上返回2层
	tps_horz_left=0,
	tps_horz_right=1,
	tps_horz_center=2,
	tps_horz_mask=3,
	tps_vert_top=0,
	tps_vert_bottom=4,
	tps_vert_center=8,
	tps_vert_mask=12,
	tps_above=0,
	tps_below=1,

	tps_capborder_none,
	tps_capborder_line,
	tps_capborder_rect,
	tps_capborder_flow
};

COLORREF interpolate(COLORREF clrLeft, COLORREF clrRight, int nStep, int nSteps);

class CTreePropertySheet;

class CTreePropertySheetInterface {
public:
	CTreePropertySheet*		GetSheet(CWnd *pWndThis);
};

class CTreePropertySheet : public CWnd
{
private:
	class	CItem	{
	public:
		int			nType;
		int			nImage;
		BOOL		bDllResource;
		CString		sCaption;
		CString		sCaptionDescr;
		CDialog		*pDialog;
		UINT		nIDTemplate;
		HTREEITEM	hItem;
	};
	class	CAddonControl {
	public:
		int			zOrder;
		BOOL		bSupportsInterface;
		BOOL		bAutoDelete;
		CWnd		*pWnd;
		int			nArrange;
		CSize		szCtrl;
		int			nWidth;
		int			nHeight;
	};
	class	CAddonRow {
	public:
		int			nSpaceTop;
		int			nSpaceBottom;
		CArray<CAddonControl,CAddonControl&>	aColCtrls;
		int			nPosY;
	};
	class	CMemDC	: public CDC {
	public:
		CBitmap m_MemBmp,*m_pOldBmp;
	public:
		CMemDC(int nWidth,int nHeight) : CDC()
		{
			CDC		*tempDC=GetDesktopWindow()->GetDC();

			CreateCompatibleDC(tempDC);
			m_MemBmp.CreateCompatibleBitmap(tempDC,nWidth,nHeight);
			m_pOldBmp=SelectObject(&m_MemBmp);

			GetDesktopWindow()->ReleaseDC(tempDC);
		}
		CMemDC(CBitmap *bmp) : CDC()
		{
			CDC		*tempDC=GetDesktopWindow()->GetDC();

			CreateCompatibleDC(tempDC);
			m_pOldBmp	=SelectObject(bmp);

			GetDesktopWindow()->ReleaseDC(tempDC);
		}
		~CMemDC()
		{
			SelectObject(m_pOldBmp);
		}
	};
// Construction
public:
	CTreePropertySheet(int nTreeWidth=100,CWnd *pParentWnd=NULL);

	void RemoveResource();

	enum {
		IDC_TREE=1000
	};


// Attributes
protected:
	CWnd		*m_pParentWnd;
	COptionTreeCtrl	m_cTreeCtrl;
	CSize		m_szDefault;
	CSize		m_szDialog;

	int			m_nAddonSpaceTop;
	int			m_nAddonSpaceBottom;

	CMemDC		*m_pCaptionBackgroundDC;
	CMemDC		*m_pCaptionDC;
	CFont		*m_pCaptionFont;
	CFont		*m_pCaptionDescrFont;

	HBITMAP		m_hBackgroundBmp;
	CMemDC		*m_pBackgroundBmpDC;

	CItem		*m_pCurItem;

	CString		m_sCaptionText;
	CString		m_sCaptionDescrText;

// m_bSpecialCaption move window support
	BOOL		m_bLButtonDown;
	DWORD		m_dwTicksLButtonDown;
	CPoint		m_pntLButtonDown;

	CArray<CItem,CItem>	m_aDlgItems;
	CArray<CAddonRow,CAddonRow&>	m_aUpperRows;
	CArray<CAddonRow,CAddonRow&>	m_aLowerRows;
	CArray<CFont*,CFont*>			m_apFonts;
	CArray<CAddonControl*,CAddonControl*> m_apCtlOrder;

public:
	int			m_nTreeWidth;
	int			m_nSpaceLeft;
	int			m_nSpaceRight;
	int			m_nSpaceTop;
	int			m_nSpaceBottom;
	int			m_nSpaceMid;
	int			m_nCaptionHeight;

	BOOL		m_bSpecialCaption;
	int			m_nCaptionBorder; // tps_capborder_xxx

	COLORREF	m_clrCaptionLeft;
	COLORREF	m_clrCaptionRight;
	COLORREF	m_clrCaptionTextLeft;
	COLORREF	m_clrCaptionTextRight;
	COLORREF	m_clrCaptionBorderTopLeft;
	COLORREF	m_clrCaptionBorderBottomRight;

	CString		m_strWinTitle;

protected:
// Implementation helper functions
	// Extra control positioning
	void		AlignExtraControls();
	void		CalcExtraSize();
	int			GetExtraRowHeight(int nVertArrange,int row);
	// Background
	void		DrawBackground(CDC* pDC, CRect& rcItem);
	void		CreateBackgroundDC();

// Operations
public:
	// DoModal() - like CDialog::DoModal() or CPropertySheet::DoModal()
	int			DoModal();

	void		AddPage(const CString& sCaption,int nType=tps_item_node,UINT nIDTemplate=((UINT)-1),const CString& sCaptionDescr=CString(""),CDialog *pDlg=NULL,int nImage=OPT_NODE,BOOL bDllResource=FALSE);
	void		SetLastPageType(int nType = tps_item_node);
	void		SetBackgroundBitmap(HBITMAP hbm);
	// InsertExtraControl: Adds a control
	//  row,col,nVertArrange define position,
	//  pWnd defines the control,
	//  nSpaceLeft, nSpaceRight define additional space to the left/right
	//  nArrange defines the alignment (tps_hort_xxx | tps_vert_xxx)
	//  bSupportsInterface defines if this control is derived
	//                     from CTreePropertySheetInterface
	void		InsertExtraControl(	int nVertArrange,int zOrder,
									CWnd *pWnd,
									int row,int col,
									int nWidth=0,int nHeight=0,
									int nArrange=tps_horz_right|tps_vert_center,
									BOOL bAutoDelete=FALSE,
									BOOL bSupportsInterface=FALSE);
	// AddExtraSpace: Inserts a 'space filler' at the specified
	//  position.
	void		InsertExtraSpace(int nVertArrange,int row,int col,int nWidth=-1,int nHeight=0);
	// SetExtraRowSpace: Sets the additional empty space between
	//  the row above and the one below
	void		SetExtraRowSpace(int nVertArrange,int nRow,int nSpaceTop,int nSpaceBottom);


	// Caption
	void		SetCaptionFont(const CString& sFacename,int nHeight,BOOL bNoInvalidate=FALSE);
	void		SetCaptionDescrFont(const CString& sFacename,int nHeight,BOOL bNoInvalidate=FALSE);
	void		SetCaptionText(const CString& sCaption,const CString& sCaptionDescr,COLORREF clrLeft, COLORREF clrRight,BOOL bForceBackgroundRedraw=FALSE);
	void		InvalidateCaption(BOOL bForceBackgroundRedraw=FALSE);

// ADDON(Styles):
	//  SetLikeNetscape() gives this sheet a netscape-like outfit.
	void		SetLikeNetscape(BOOL bSpecialCaption=FALSE);
	//  SetFancy() :)
	void		SetFancy(BOOL bSpecialCaption=FALSE);
	//	SetSimple()
	void		SetSimple(BOOL bSpecialCaption=FALSE);
	// Fonts
	CFont		*GetFont(const CString& sFacename,int nHeight);

protected:
	// SetExtraRows: Sets the number of rows in the
	//  'nVertArrange' area (tps_above, tps_below)
	void		SetExtraRows(int nVertArrange,int nRows);

	// SetExtraColumns: Sets the number of columns in 'nRow'(nVertArrange)
	void		SetExtraColumns(int nVertArrange,int nRow,int nCols);
	void		AddExtraControl(int nVertArrange,int zOrder,CWnd *pWnd,int row,int col,int nWidth=-1,int nHeight=0,int nArrange=tps_horz_right|tps_vert_center,BOOL bAutoDelete=FALSE,BOOL bSupportsInterface=FALSE);
	void		AddExtraSpace(int nVertArrange,int row,int col,int nWidth=-1,int nHeight=0);



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreePropertySheet)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTreePropertySheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTreePropertySheet)
	afx_msg void OnClose();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnSheetChange(NMHDR *pNotifyStruct, LRESULT *lResult);
	afx_msg	LRESULT OnDisplayModeChange(WPARAM wP,LPARAM lP);
public:
	afx_msg	void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnApply();
	HWND		 SearchNextTabItemDialog(HWND hwndCtl,BOOL bPrevious);
	HWND		 SearchNextTabItem(HWND hwndCtl, BOOL bPrevious);
	void		 OnNextTabItem(BOOL bPrevious);
	BOOL		 OnInitDialog();
	DECLARE_MESSAGE_MAP()

	friend LRESULT CALLBACK DlgGetMsgProc(int nCode,WPARAM wParam,LPARAM lParam);
	friend class CTreePropertySheetInterface;
};

/////////////////////////////////////////////////////////////////////////////

