// StylerView.h : interface of the CStylerView class
//


#pragma once

#include "StylerDoc.h"

static const CLSID CGID_IWebBrowser = 
	{0xED016940L,0xBD5B,0x11cf,0xBA, 0x4E,0x00,0xC0,0x4F,0xD7,0x08,0x16};

#define HTMLID_FIND					1
#define HTMLID_VIEWSOURCE			2
#define HTMLID_OPTIONS				3 

#define DISPID_AMBIENT_DLCONTROL    (-5512)
#define DLCTL_DLIMAGES              0x00000010
#define DLCTL_VIDEOS                0x00000020
#define DLCTL_BGSOUNDS              0x00000040

#define IDM_COPY                    15
#define IDM_CUT                     16
#define IDM_PASTE                   26


class CStylerView : public CHtmlView
{
protected: // create from serialization only

	CStylerView();
	DECLARE_DYNCREATE(CStylerView)

// Attributes
public:
	static BOOL m_bFilterAll;
	static BOOL m_bFilterList;


protected:
	BOOL m_bBack, m_bForward;
	BOOL m_bActive;
	int m_nLoadingState; 
	int m_nProgress;

	BOOL m_bPopup;
	BOOL m_bLock;

	LPDISPATCH m_lpDisp;

	CString	m_strURL;
	
	//static CImageList m_lstTabImages;
	static HICON m_arrIcons[5];

	static long m_nFontSize;
	static BOOL m_bShowImages;
	
	UINT m_nEncoding;
	
	struct MOUSEINFO
	{
		long nLClick;
		long nRClick;
		CPoint ptClick;

		void Clear()
		{
			nLClick = 0;
			nRClick = 0;
		}
	} m_infMouse;
	

	char	m_szOldPath[256];
	DWORD	m_dwPathLen;

// Operations
public:
	BOOL GetTabIcon(CImageList** imgList, int* nIndex);
 	void SetLoading(BOOL bLoading);
	CStylerDoc* GetDocument() const;

	CString GetCurrentURL() {return GetDocument()->GetCurrentUrl();}//m_strURL;}

	void SaveAndSetCurPath(CString strURL);
	void RestoreCurPath();

	static void Load();
	static void Save();

protected:
	BOOL IsLoading() {return m_nLoadingState < 3;}
	void RefreshExplorer();

	BOOL QueryFormsCommand(DWORD dwCommandID);
	void ExecFormsCommand(DWORD dwCommandID);

	BOOL PopupFilter(CString str);
	void SetAddress();
	CComPtr<IHTMLDocument2> GetHtmlDocument2();	


// Overrides
public:


// Implementation
public:
	virtual ~CStylerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:
//{{AFX_MSG(CChildFrame)

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);


	afx_msg void OnToolsOptions();
	afx_msg void OnUpdateGotoBack(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGotoForward(CCmdUI *pCmdUI);
	afx_msg void OnFileProperties();
	afx_msg void OnEditSelectall();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFileSaveAs();
	afx_msg void OnEditFind();
	afx_msg void OnViewSource();

	afx_msg void OnAddToFavorites();

	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	
	afx_msg void OnTextSizeRange(UINT nID);
	afx_msg void OnUpdateTextSizeRange(CCmdUI* pCmdUI);

	afx_msg void OnViewEncodingRange(UINT);
	afx_msg void OnUpdateViewEncodingRange(CCmdUI *pCmdUI);

	afx_msg LRESULT OnGetTabIcon(WPARAM, LPARAM);

//}}AFX_MSG
	//afx_msg void OnAddPageHere(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnUpdateAddPageHere(CCmdUI *pCmdUI);

	//afx_msg void OnTextSize();

	afx_msg void OnUpdateIndicator(CCmdUI *pCmdUI);

	//afx_msg void OnShowImages();
	//afx_msg void OnUpdateShowImages(CCmdUI *pCmdUI);

public:
//{{AFX_VIRTUAL(CChildFrame)	
	virtual void OnActivateView( BOOL bActivate, CView* pActivateView, CView* pDeactiveView );

	virtual void OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel);
	virtual void OnTitleChange(LPCTSTR lpszText);

	virtual void OnCommandStateChange(long nCommand, BOOL bEnable);
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags,
		LPCTSTR lpszTargetFrameName, CByteArray& baPostedData,
		LPCTSTR lpszHeaders, BOOL* pbCancel);

	virtual void OnProgressChange(long nProgress, long nProgressMax);
	virtual void PostNcDestroy();

	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
						DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
						UINT nID, CCreateContext* pContext);

	virtual void NavigateComplete2(LPDISPATCH pDisp, VARIANT* URL);
	virtual void DocumentComplete(LPDISPATCH pDisp, VARIANT* URL);
	virtual void OnLinkStartDrag(NMHDR* pNMHDR, LRESULT* pRes);

	afx_msg void OnGoStartPage();
	afx_msg void OnUpdateGoStartPage(CCmdUI* pCmdUI);
	afx_msg void OnGoSearchTheWeb();
	afx_msg void OnUpdateGoSearchTheWeb(CCmdUI* pCmdUI);
	afx_msg void OnViewFolders();
	afx_msg void OnUpdateViewFolders(CCmdUI* pCmdUI);
	afx_msg void OnViewHistory();
	afx_msg void OnUpdateViewHistory(CCmdUI* pCmdUI);
	afx_msg void OnFavorite(NMHDR* pNMHDR, LRESULT* pResult);
//}}AFX_VIRTUAL

protected:
	DECLARE_MESSAGE_MAP()


public:

BEGIN_INTERFACE_PART(DocHostUIHandler, IDocHostUIHandler)
	STDMETHOD(ShowContextMenu)(/* [in] */ DWORD dwID,
            /* [in] */ POINT __RPC_FAR *ppt,
            /* [in] */ IUnknown __RPC_FAR *pcmdtReserved,
            /* [in] */ IDispatch __RPC_FAR *pdispReserved);
	STDMETHOD(GetHostInfo)( 
            /* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo);
	STDMETHOD(ShowUI)( 
            /* [in] */ DWORD dwID,
            /* [in] */ IOleInPlaceActiveObject __RPC_FAR *pActiveObject,
            /* [in] */ IOleCommandTarget __RPC_FAR *pCommandTarget,
            /* [in] */ IOleInPlaceFrame __RPC_FAR *pFrame,
            /* [in] */ IOleInPlaceUIWindow __RPC_FAR *pDoc);
	STDMETHOD(HideUI)(void);
	STDMETHOD(UpdateUI)(void);
	STDMETHOD(EnableModeless)(/* [in] */ BOOL fEnable);
	STDMETHOD(OnDocWindowActivate)(/* [in] */ BOOL fEnable);
	STDMETHOD(OnFrameWindowActivate)(/* [in] */ BOOL fEnable);
	STDMETHOD(ResizeBorder)( 
            /* [in] */ LPCRECT prcBorder,
            /* [in] */ IOleInPlaceUIWindow __RPC_FAR *pUIWindow,
            /* [in] */ BOOL fRameWindow);
	STDMETHOD(TranslateAccelerator)( 
            /* [in] */ LPMSG lpMsg,
            /* [in] */ const GUID __RPC_FAR *pguidCmdGroup,
            /* [in] */ DWORD nCmdID);
	STDMETHOD(GetOptionKeyPath)( 
            /* [out] */ LPOLESTR __RPC_FAR *pchKey,
            /* [in] */ DWORD dw);
	STDMETHOD(GetDropTarget)(
            /* [in] */ IDropTarget __RPC_FAR *pDropTarget,
            /* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget);
    STDMETHOD(GetExternal)( 
            /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch);
    STDMETHOD(TranslateUrl)( 
            /* [in] */ DWORD dwTranslate,
            /* [in] */ OLECHAR __RPC_FAR *pchURLIn,
            /* [out] */ OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut);
    STDMETHOD(FilterDataObject)( 
            /* [in] */ IDataObject __RPC_FAR *pDO,
            /* [out] */ IDataObject __RPC_FAR *__RPC_FAR *ppDORet);
END_INTERFACE_PART(DocHostUIHandler)


BEGIN_INTERFACE_PART(OleClientSite, IOleClientSite)

	STDMETHOD(SaveObject)( void) { return E_NOTIMPL;}
       
	STDMETHOD (GetMoniker)( 
            /* [in] */ DWORD /*dwAssign*/,
            /* [in] */ DWORD /*dwWhichMoniker*/,
            /* [out] */ IMoniker ** /*ppmk*/) { return E_NOTIMPL;}
        
    STDMETHOD(GetContainer)( 
            /* [out] */ IOleContainer ** /*ppContainer*/) { return E_NOTIMPL;}
        
    STDMETHOD(ShowObject)( void) { return E_NOTIMPL;}
        
    STDMETHOD(OnShowWindow)( 
            /* [in] */ BOOL /*fShow*/) { return E_NOTIMPL;}
        
    STDMETHOD(RequestNewObjectLayout)( void) { return E_NOTIMPL;}        
END_INTERFACE_PART(OleClientSite) 	

BEGIN_INTERFACE_PART(Dispatch, IDispatch)
	STDMETHOD(Invoke)(DISPID dispid, REFIID riid, LCID lcid,
			WORD wFlags, DISPPARAMS* pDispParams,
            VARIANT* pvarResult, EXCEPINFO*  pExcepInfo,
            UINT* puArgErr);
	
	STDMETHOD(GetTypeInfoCount)(unsigned int*);
	STDMETHOD(GetTypeInfo)(unsigned int, LCID, ITypeInfo**);
	STDMETHOD(GetIDsOfNames)(REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);

END_INTERFACE_PART(Dispatch) 	


DECLARE_INTERFACE_MAP()



};

#ifndef _DEBUG  // debug version in StylerView.cpp
inline CStylerDoc* CStylerView::GetDocument() const
   { return reinterpret_cast<CStylerDoc*>(m_pDocument); }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
