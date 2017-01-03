// TreePropertySheet.cpp : implementation file
//

#include "stdafx.h"
#include "BlueUI.h"
#include "TreePropertySheet.h"
#include "TreePropertySheetAnimateWidget.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_APPLY	3000

UINT	msgInitializationDone=0;
UINT	msgCmdPass=0;

int		nRef=0;
HHOOK	getmsgHook=NULL;

typedef struct _hookinfo {
	HWND		hwndHook;
	WNDPROC		oldWndProc;
	CTreePropertySheet *pSheet;
	BOOL		bInitialized;
} hookinfo;

CArray<hookinfo,hookinfo&> hooklist;

// delete all items from 'hooklist' with the 'pSheet' parent
void	deleteHooks(CTreePropertySheet *pSheet)
{
	int	i,nSize=hooklist.GetSize();
	for(i=0;i<nSize;i++) {
		if(hooklist[i].pSheet==pSheet) {
			hooklist.RemoveAt(i);
			i--;
			nSize--;
		}
	}
}

// get the 'hookinfo' for hwndHook
hookinfo	*findHookInfo(HWND hwndHook)
{
	int	i,nSize=hooklist.GetSize();
	for(i=0;i<nSize;i++)
		if(hooklist[i].hwndHook==hwndHook)
			return &hooklist[i];
	return NULL;
}

// Search 'hooklist' for a hWnd property sheet
CTreePropertySheet *getPropertySheet(HWND hWnd)
{
	int	i,nSize=hooklist.GetSize();
	for(i=0;i<nSize;i++)
		if(hooklist[i].pSheet->GetSafeHwnd()==hWnd)
			return hooklist[i].pSheet;
	return NULL;
}

// Find parent sheet of a HWND
CTreePropertySheet	*DlgFindSheet(HWND hwnd)
{
	CTreePropertySheet *pSheet;
	for(;;) {
		pSheet	=getPropertySheet(hwnd);
		if(pSheet!=NULL) 
			return pSheet;
		hwnd=::GetParent(hwnd);
		if(hwnd==NULL) break;
	}
	return NULL;
}

// Subclasses dialog WNDPROC
LRESULT CALLBACK DlgSubclassWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	hookinfo	*info=findHookInfo(hWnd);
	if(uMsg==WM_COMMAND && info->bInitialized) {
		if(HIWORD(wParam)==BN_CLICKED) {
			CTreePropertySheet *pSheet;
			pSheet	=info->pSheet;
			if(pSheet!=NULL) {
				if(LOWORD(wParam)==IDOK) {
					pSheet->PostMessage(WM_COMMAND,MAKELONG(IDOK,BN_CLICKED),(LPARAM)hWnd);
					return 0;
				}
				if(LOWORD(wParam)==IDCANCEL) {
					pSheet->PostMessage(WM_COMMAND,MAKELONG(IDCANCEL,BN_CLICKED),(LPARAM)hWnd);
					return 0;
				}
			}
		}
	}
	if(uMsg==msgCmdPass)
		return (*info->oldWndProc)(hWnd,WM_COMMAND,wParam,lParam);

	if(uMsg==msgInitializationDone){
		LRESULT res=(*info->oldWndProc)(hWnd,uMsg,wParam,lParam);
		info->bInitialized=TRUE;
		return res;
	}
	return (*info->oldWndProc)(hWnd,uMsg,wParam,lParam);
}

// Dialog hook procedure
// This is needed to catch TAB/SHIFT-TAB key strokes
LRESULT CALLBACK DlgGetMsgProc(int /*nCode*/,WPARAM /*wParam*/,LPARAM lParam)
{
	MSG				*pMsg=(MSG*)lParam;
	static	DWORD	lastKeyDownTime=0;

	if(pMsg->message==WM_KEYDOWN || pMsg->message==WM_KEYUP || pMsg->message==WM_CHAR) {
		if(pMsg->wParam==VK_TAB) {
			pMsg->wParam=0;
			if(pMsg->message==WM_KEYDOWN) {
				if(	pMsg->hwnd!=::GetFocus())
					return 0;
				if(pMsg->time==lastKeyDownTime)
					return 0;
				lastKeyDownTime=pMsg->time;
				CTreePropertySheet *pSheet;
				pSheet	=DlgFindSheet(pMsg->hwnd);
				if(pSheet!=NULL) {
					SHORT ks=::GetKeyState(VK_SHIFT);
					ks&=~1;
					pSheet->OnNextTabItem(ks?TRUE:FALSE);
					return 0;
				}
			}
			return 0;
		}
		if(pMsg->wParam==VK_ESCAPE) {
			pMsg->wParam=0;
			CTreePropertySheet *pSheet;
			pSheet	=DlgFindSheet(pMsg->hwnd);
			if(pSheet!=NULL) {
				pSheet->PostMessage(WM_COMMAND,MAKELONG(IDCANCEL,BN_CLICKED),(LPARAM)pMsg->hwnd);
				return 0;
			}
		}
		if(pMsg->wParam==VK_RETURN && pMsg->message==WM_KEYDOWN) {
			pMsg->wParam=0;
			CTreePropertySheet *pSheet;
			pSheet	=DlgFindSheet(pMsg->hwnd);
			if(pSheet!=NULL) {
				if(pSheet->m_pCurItem!=NULL) {
					CDialog	*pDlg=pSheet->m_pCurItem->pDialog;
					DWORD defID=pDlg->GetDefID();
					if(defID!=0) {
						defID=LOWORD(defID);
						pDlg->PostMessage(WM_COMMAND,MAKELONG(LOWORD(defID),BN_CLICKED),(LPARAM)pDlg->GetDlgItem(defID)->GetSafeHwnd());
						return 0;
					}
				}
				return 0;
			}
		}
	}
	return 0;
}

void	HookDlgProc(CTreePropertySheet *pSheet,CDialog *pDialog)
{
	hookinfo	hinfo;

	hinfo.hwndHook	=(pDialog->GetSafeHwnd());
	hinfo.pSheet	=pSheet;
	hinfo.oldWndProc=(WNDPROC)GetWindowLong(pDialog->GetSafeHwnd(),GWL_WNDPROC);
	hinfo.bInitialized=FALSE;
	hooklist.Add(hinfo);
	if(nRef==0) {
		getmsgHook=SetWindowsHookEx(WH_GETMESSAGE,DlgGetMsgProc,AfxGetInstanceHandle(),AfxGetApp()->m_nThreadID);
		nRef++;
	}

	SetWindowLong(pDialog->GetSafeHwnd(),GWL_WNDPROC,(LPARAM)DlgSubclassWndProc);
}

/////////////////////////////////////////////////////////////////////////////
// CTreePropertySheet

CTreePropertySheet::CTreePropertySheet(int nTreeWidth,CWnd *pParentWnd)
{
	if(msgInitializationDone==0)
		msgInitializationDone=RegisterWindowMessage("TPM_INITDONE");
	if(msgCmdPass==0)
		msgCmdPass=RegisterWindowMessage("TPM_CMDPASS");

	m_nTreeWidth=nTreeWidth; // Width of the tree control
	m_pParentWnd=pParentWnd; // parent window
	m_szDefault.cx=0;
	m_szDefault.cy=0;

	m_nSpaceLeft	=16;	 // Left border width
	m_nSpaceRight	=16;	 // Right border width
	m_nSpaceTop		=8;
	m_nSpaceBottom	=16;
	m_nSpaceMid		=16;	 // Space between tree and dialog
	m_nCaptionHeight=32;     // Height of caption bar

	m_nAddonSpaceTop=0;		 // Do not change, internally used
	m_nAddonSpaceBottom=0;

	m_pCaptionBackgroundDC=NULL;
	m_pCaptionDC=NULL;
	m_pCaptionFont=NULL;
	m_pCaptionDescrFont=NULL;

	m_clrCaptionLeft=RGB(128,128,128);
	m_clrCaptionRight=RGB(128,128,128);
	m_clrCaptionTextLeft=RGB(255,255,255);
	m_clrCaptionTextRight=RGB(255,255,255);

	m_nCaptionBorder	=tps_capborder_line;
	m_clrCaptionBorderTopLeft=RGB(255,255,255);
	m_clrCaptionBorderBottomRight=RGB(64,64,64);

	m_pBackgroundBmpDC	=NULL;
	m_hBackgroundBmp	=NULL;

	m_bSpecialCaption=FALSE;

	m_pCurItem=NULL;

	PLAT_LOADSTRING(strTitle, IDS_TITLE_TREEPROPSHEET_SYSCFG); // 系统设置
	m_strWinTitle = strTitle;
}

CTreePropertySheet::~CTreePropertySheet()
{
	RemoveResource();
}

// 释放资源,add by xieyongfang
void CTreePropertySheet::RemoveResource()
{
	int	i,nSize=m_aDlgItems.GetSize();
	for(i=0;i<nSize;i++) {
		CItem	&item=m_aDlgItems[i];
		if(item.pDialog!=NULL)
			delete item.pDialog;
	}
	m_aDlgItems.RemoveAll();

	CArray<CAddonRow,CAddonRow&>	*paRow;
	int	nVertArrange;

	for(nVertArrange=tps_above;nVertArrange<=tps_below;nVertArrange++) {
		if(nVertArrange==tps_above)	paRow=&m_aUpperRows;
		else						paRow=&m_aLowerRows;

		int		nRows=paRow->GetSize();
		int		row;

		for(row=0;row<nRows;row++) {
			CAddonRow	&cRow=paRow->ElementAt(row);
			int			col,nCols=cRow.aColCtrls.GetSize();
			for(col=0;col<nCols;col++) {
				if(cRow.aColCtrls[col].bAutoDelete)
					delete cRow.aColCtrls[col].pWnd;
			}
		}
	}
	m_aUpperRows.RemoveAll();
	m_aLowerRows.RemoveAll();

	if(m_pCaptionBackgroundDC!=NULL)
		delete m_pCaptionBackgroundDC;
	m_pCaptionBackgroundDC = NULL;

	if(m_pCaptionDC!=NULL)
		delete m_pCaptionDC;
	m_pCaptionDC = NULL;

	if(m_pCaptionFont!=NULL)
		delete m_pCaptionFont;
	m_pCaptionFont = NULL;

	if(m_pCaptionDescrFont!=NULL)
		delete m_pCaptionDescrFont;
	m_pCaptionDescrFont = NULL;

	int nFonts=m_apFonts.GetSize();
	for(i=0;i<nFonts;i++)
		delete m_apFonts[i];
	m_apFonts.RemoveAll();
}

BEGIN_MESSAGE_MAP(CTreePropertySheet, CWnd)
	//{{AFX_MSG_MAP(CTreePropertySheet)
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TVN_SELCHANGED,CTreePropertySheet::IDC_TREE,OnSheetChange)
	ON_BN_CLICKED(IDOK,OnOk)
	ON_BN_CLICKED(IDCANCEL,OnCancel)
	ON_BN_CLICKED(ID_APPLY,OnApply)
	ON_MESSAGE(WM_DISPLAYCHANGE,OnDisplayModeChange)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTreePropertySheet methods

int	CTreePropertySheet::DoModal()
{
	// allow OLE servers to disable themselves
	AfxGetApp()->EnableModeless(FALSE);

	// find parent HWND
	HWND hWndTop;
	CWnd* pParentWnd = CWnd::GetSafeOwner(m_pParentWnd, &hWndTop);

	BOOL bEnableParent = FALSE;
	if (pParentWnd != NULL && pParentWnd->IsWindowEnabled())
	{
		pParentWnd->EnableWindow(FALSE);
		bEnableParent = TRUE;
	}
	HWND hWndCapture = ::GetCapture();
	if (hWndCapture != NULL)
		::SendMessage(hWndCapture, WM_CANCELMODE, 0, 0);

	LPCTSTR	lpszClass=AfxRegisterWndClass(CS_OWNDC);

	UINT	nStyle=WS_POPUP|WS_BORDER|WS_CLIPCHILDREN;
	if(!m_bSpecialCaption)
		nStyle|=WS_CAPTION;

	CreateEx(WS_EX_DLGMODALFRAME|WS_EX_WINDOWEDGE,
			 lpszClass,
			 m_strWinTitle,
			 nStyle,
			 200,200,
			 200,200,
			 pParentWnd->GetSafeHwnd(),
			 NULL,
			 NULL);

	int	i,nSize=m_aDlgItems.GetSize();
	for(i=0;i<nSize;i++) {
		CItem	&item=m_aDlgItems[i];
		if(item.pDialog!=NULL) {
			if(m_pCurItem==&item)
				item.pDialog->ShowWindow(SW_SHOW);
			else
				item.pDialog->ShowWindow(SW_HIDE);
		}
	}

	CenterWindow(pParentWnd);

	int nRetValue=RunModalLoop(MLF_SHOWONIDLE);

	if (m_hWnd != NULL)
	{
		SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|
			SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
	}

	if (bEnableParent)
		pParentWnd->EnableWindow(TRUE);
	if (pParentWnd != NULL && ::GetActiveWindow() == m_hWnd)
		pParentWnd->SetActiveWindow();

	// cleanup
	DestroyWindow();

	nRef--;
	if(nRef==0) {
		UnhookWindowsHookEx(getmsgHook);
		getmsgHook=NULL;
	}

	deleteHooks(this);

	return nRetValue;
}

// Add a page to the sheet
// nType==tps_item_branch/tps_item_node/tps_item_endbranch
void CTreePropertySheet::AddPage(const CString& sCaption,int nType,UINT nIDTemplate,const CString& sCaptionDescr,CDialog *pDlg,int nImage,BOOL bDllResource)
{
	CItem	item;
	item.nType	=nType;
	item.nImage	=nImage;
	item.sCaption=sCaption;
	item.sCaptionDescr=sCaptionDescr;
	if(nIDTemplate!=((UINT)-1)) {
		if(pDlg==NULL) pDlg=new CDialog();
	}
	item.pDialog=pDlg;
	item.nIDTemplate=nIDTemplate;
	item.bDllResource=bDllResource;
	m_aDlgItems.Add(item);
}

void CTreePropertySheet::SetLastPageType(int nType)
{
	CItem* pItem = &(m_aDlgItems[m_aDlgItems.GetSize()-1]);
	if(nType == tps_item_endbranch)
	{
		// 多次调用每次向上返回一层
		pItem->nType = (pItem->nType == tps_item_endbranch) ? tps_item_endbranch2 : nType;
	}else
	{
		pItem->nType = nType;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTreePropertySheet message handlers

BOOL CTreePropertySheet::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->hwnd==GetSafeHwnd() && pMsg->message==msgInitializationDone) {
		OnInitDialog();
		return TRUE;
	}
	if(pMsg->hwnd==GetSafeHwnd() && pMsg->message==WM_KEYDOWN) {
		if(pMsg->wParam==VK_TAB) {
			if(m_pCurItem!=NULL) {
				SHORT val=(SHORT)(GetKeyState(VK_SHIFT)&(~1));
				BOOL lShift=val?FALSE:TRUE;
				OnNextTabItem(lShift);
			}
			return TRUE;
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CTreePropertySheet::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	//EndModalLoop(IDABORT);
}

BOOL CTreePropertySheet::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	CRect	rcClient;
	GetClientRect(&rcClient);
	DrawBackground(pDC,rcClient);
	
	return TRUE;//CWnd::OnEraseBkgnd(pDC);
}

void CTreePropertySheet::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	CRect	rcClient;
	GetClientRect(&rcClient);

	//rcClient.left/=2;
	rcClient.top	=m_nSpaceTop+m_nAddonSpaceTop;
	if(m_bSpecialCaption)
		rcClient.top+=m_nCaptionHeight;

	rcClient.bottom-=m_nSpaceBottom+m_nAddonSpaceBottom;
	rcClient.left	=m_nSpaceLeft;
	rcClient.right	=rcClient.left+m_nTreeWidth;

	// 因Skin的Hook对此处有影响,增加窗口句柄有效性判断
	if(m_cTreeCtrl.GetSafeHwnd())
	{
		m_cTreeCtrl.MoveWindow(&rcClient);
	}
	
}

int CTreePropertySheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// If 'special caption'(replaces standard caption) is set,
	// then don't allow any border
	if(m_bSpecialCaption)
		m_nCaptionBorder=tps_capborder_none;

	// Create the tree control
	m_cTreeCtrl.Create(	TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|WS_CHILD|WS_VISIBLE|WS_TABSTOP,
						CRect(0,0,0,0),
						this,
						IDC_TREE);
	//m_cTreeCtrl.ModifyStyleEx(0,WS_EX_CLIENTEDGE);
	m_cTreeCtrl.ModifyStyleEx(0,WS_EX_STATICEDGE);
	m_cTreeCtrl.InitTree();

	// Set up the tree
	HTREEITEM cur=TVI_ROOT;
	int	i,nSize=m_aDlgItems.GetSize();
	for(i=0;i<nSize;i++) {
		HTREEITEM	hcItem;
		CItem	&item=m_aDlgItems[i];
		// item==branch?
		if(item.nType==tps_item_branch)
			cur=hcItem=m_cTreeCtrl.AddTreeItem(item.sCaption,cur,item.nImage,item.nImage);
		// item==node?
		if(item.nType==tps_item_node)
			hcItem=m_cTreeCtrl.AddTreeItem(item.sCaption,cur,item.nImage,item.nImage+1);
		// item==end of branch?
		if(item.nType==tps_item_endbranch) {
			hcItem=m_cTreeCtrl.AddTreeItem(item.sCaption,cur,item.nImage,item.nImage+1);
			if(cur != TVI_ROOT)
				cur=m_cTreeCtrl.GetParentItem(cur);
		}
		// item==end of branch2?
		if(item.nType==tps_item_endbranch2) {
			hcItem=m_cTreeCtrl.AddTreeItem(item.sCaption,cur,item.nImage,item.nImage+1);
			if(cur != TVI_ROOT)
				cur=m_cTreeCtrl.GetParentItem(cur);
			if(cur != TVI_ROOT)
				cur=m_cTreeCtrl.GetParentItem(cur);
		}
		item.hItem=hcItem;
		m_cTreeCtrl.SetItemData(hcItem,(DWORD)&item);
		if(item.pDialog!=NULL) {
			if(m_pCurItem==NULL)
				m_pCurItem=&item;

			CRect rcClient;
			if(item.bDllResource)
				item.pDialog->SetParent(this);
			else
				item.pDialog->Create(item.nIDTemplate,this);

			// Make sure that each dialog has WS_TABSTOP style
			// set
			item.pDialog->ModifyStyle(0,WS_TABSTOP);

			HookDlgProc(this,item.pDialog);

			item.pDialog->GetWindowRect(&rcClient);
			m_szDefault.cx=max(m_szDefault.cx,rcClient.Width());
			m_szDefault.cy=max(m_szDefault.cy,rcClient.Height());
		}
	}

	m_szDialog	=m_szDefault;

	CRect	rcTemp1,rcTemp2;
	GetWindowRect(&rcTemp1);
	GetClientRect(&rcTemp2);

	// Determine total (m_szDefault) client size of dialog
	m_szDefault.cx+=m_nTreeWidth+m_nSpaceLeft+m_nSpaceRight+m_nSpaceMid;
	m_szDefault.cy+=m_nSpaceTop+m_nSpaceBottom+m_nCaptionHeight+m_nAddonSpaceBottom+m_nAddonSpaceTop;

	// Align extra controls
	AlignExtraControls();

	// Place the dialogs
	for(i=0;i<nSize;i++) {
		CItem	&item=m_aDlgItems[i];

		if(item.pDialog!=NULL)
			item.pDialog->SetWindowPos(NULL,m_nTreeWidth+m_nSpaceLeft+m_nSpaceMid,m_nSpaceTop+m_nCaptionHeight+m_nAddonSpaceTop,0,0,SWP_NOZORDER|SWP_NOSIZE);
	}

	// Create background DC (for background bitmaps, currently
	// not full supported)
	if(m_hBackgroundBmp!=NULL)
		CreateBackgroundDC();

	// Adjust window size of 'this'
	SetWindowPos(NULL,0,0,
		m_szDefault.cx+(rcTemp1.Width()-rcTemp2.Width()),
		m_szDefault.cy+(rcTemp1.Height()-rcTemp2.Height()),SWP_NOMOVE|SWP_NOZORDER);

	PostMessage(msgInitializationDone);

	HTREEITEM hItem = m_cTreeCtrl.GetChildItem(TVI_ROOT);
	m_cTreeCtrl.Expand(hItem, TVE_EXPAND);

	return 0;
}

void CTreePropertySheet::OnSheetChange(NMHDR *pNotifyStruct, LRESULT *lResult)
{
	*lResult	=0;
	NM_TREEVIEW	*nmhdr=(NM_TREEVIEW*)pNotifyStruct;

	if(nmhdr->itemNew.hItem!=NULL) {
		// Fetch corresponding CItem for selected HTREEITEM
		DWORD	dwItemData=m_cTreeCtrl.GetItemData(nmhdr->itemNew.hItem);
		CItem	*pItem=(CItem*)dwItemData;

		// Has item a dialog attached?
		if(pItem->pDialog!=NULL) {
			// Yes, lock window upadte
			LockWindowUpdate();
			// Hide current dialog
			if(m_pCurItem->pDialog)
				m_pCurItem->pDialog->ShowWindow(SW_HIDE);
			// Change caption
			SetCaptionText(pItem->sCaption,pItem->sCaptionDescr,m_clrCaptionLeft,m_clrCaptionRight);
			InvalidateCaption();
			m_pCurItem=pItem;
			// Show new dialog
			m_pCurItem->pDialog->ShowWindow(SW_SHOW);
			// Paint new state
			UnlockWindowUpdate();
		}else if(m_pCurItem && m_pCurItem->pDialog)
		{
			m_pCurItem->pDialog->ShowWindow(SW_HIDE);
			// Change caption
			SetCaptionText("","",m_clrCaptionLeft,m_clrCaptionRight);
			InvalidateCaption();
			m_pCurItem=pItem;
		}
	}
}

BOOL CTreePropertySheet::OnInitDialog() 
{
	// TODO: Add extra initialization here

	int	i,nSize=m_aDlgItems.GetSize();
	for(i=0;i<nSize;i++) {
		if(m_aDlgItems[i].pDialog==NULL) continue;
		MSG msg;
		while(PeekMessage(
			  &msg,
			  m_aDlgItems[i].pDialog->GetSafeHwnd(),
			  WM_KEYFIRST,WM_KEYLAST,PM_REMOVE));
		m_aDlgItems[i].pDialog->PostMessage(msgInitializationDone);
	}
	
	// If at least one dialog is attached to a tree item
	// then make this dialog visible
	if(m_pCurItem!=NULL)
		m_cTreeCtrl.SelectItem(m_pCurItem->hItem);
	m_cTreeCtrl.SetFocus();

	ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTreePropertySheet::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_bSpecialCaption)
		dc.BitBlt(0,0,
				  m_szDefault.cx,m_nCaptionHeight,
				  m_pCaptionDC,
				  0,0,SRCCOPY);
	else
		dc.BitBlt(m_nSpaceLeft+m_nTreeWidth+m_nSpaceMid,m_nSpaceTop+m_nAddonSpaceTop,
				  m_szDialog.cx,m_nCaptionHeight,
				  m_pCaptionDC,
				  0,0,SRCCOPY);
	
}

LRESULT CTreePropertySheet::OnDisplayModeChange(WPARAM,LPARAM)
{
	if(m_pCaptionBackgroundDC!=NULL)
		delete m_pCaptionBackgroundDC;
	if(m_pCaptionDC!=NULL)
		delete m_pCaptionDC;
	m_pCaptionBackgroundDC=NULL;
	m_pCaptionDC=NULL;
	InvalidateCaption();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CTreePropertySheet caption methods

void CTreePropertySheet::SetCaptionText(const CString& sCaption,const CString& sCaptionDescr,COLORREF clrLeft, COLORREF clrRight,BOOL bForce)
{
	int		endX;

	// Special caption -> whole client width
	if(m_bSpecialCaption)
		endX	=m_szDefault.cx;
	else
		endX	=m_szDialog.cx;

	m_sCaptionText=sCaption;
	m_sCaptionDescrText=sCaptionDescr;

	PLAT_LOADSTRING(strFont, IDS_FONT_TREEPROPSHEET); // 宋体
	// If fonts not yet set, use defaults
	if(m_pCaptionFont==NULL)
		//SetCaptionFont(strFont,18,TRUE);
		SetCaptionFont(strFont,14,TRUE);
		//SetCaptionFont("MS Sans Serif",24,TRUE);
	if(m_pCaptionDescrFont==NULL)
		SetCaptionDescrFont(strFont,12,TRUE);
		//SetCaptionDescrFont("MS Sans Serif",18,TRUE);

	// Background DC already created? (or redraw force?)
	if(m_pCaptionBackgroundDC==NULL || bForce) {
		if(m_pCaptionBackgroundDC==NULL) {
			m_pCaptionBackgroundDC=new CMemDC(m_szDefault.cx,m_nCaptionHeight);
		}

		int		i;
		CBrush	*pOldBrush=NULL;
		CPen	*pOldPen,nullPen;

		nullPen.CreateStockObject(NULL_PEN);
		pOldPen	=m_pCaptionBackgroundDC->SelectObject(&nullPen);

		// Paint graded caption background
		for(i=0;i<endX;i++) {
			COLORREF clr;
			clr		=interpolate(clrLeft,clrRight,i,endX);
			CBrush	brush(clr);

			if(pOldBrush==NULL)
				pOldBrush	=m_pCaptionBackgroundDC->SelectObject(&brush);
			else
				m_pCaptionBackgroundDC->SelectObject(&brush);

			m_pCaptionBackgroundDC->Rectangle(i,0,i+2,m_nCaptionHeight+1);

			m_pCaptionBackgroundDC->SelectObject(pOldBrush);
		}

		m_pCaptionBackgroundDC->SelectObject(pOldBrush);
		m_pCaptionBackgroundDC->SelectObject(pOldPen);

		// Paint border if any
		if(m_nCaptionBorder==tps_capborder_line)
		{
			m_pCaptionBackgroundDC->Draw3dRect(0,0,endX,m_nCaptionHeight,
									 m_clrCaptionBorderTopLeft,
									 m_clrCaptionBorderBottomRight);
		}else
		if(m_nCaptionBorder==tps_capborder_rect)
		{
			//m_pCaptionBackgroundDC->Rectangle(0,0,endX,m_nCaptionHeight);
			m_pCaptionBackgroundDC->MoveTo(0,0);
			m_pCaptionBackgroundDC->LineTo(endX,0);
			m_pCaptionBackgroundDC->LineTo(endX,m_nCaptionHeight);
			m_pCaptionBackgroundDC->LineTo(0,m_nCaptionHeight);
			m_pCaptionBackgroundDC->LineTo(0,0);
		}

	}
	// DC for caption already created?
	if(m_pCaptionDC==NULL) {
		m_pCaptionDC=new CMemDC(m_szDefault.cx,m_nCaptionHeight);
		m_pCaptionDC->SetBkMode(TRANSPARENT);
	}
	TEXTMETRIC	tm;
	m_pCaptionDC->SelectObject(m_pCaptionFont);
	m_pCaptionDC->GetTextMetrics(&tm);
	m_pCaptionDC->BitBlt(0,0,endX,m_nCaptionHeight,
						 m_pCaptionBackgroundDC,0,0,SRCCOPY);
	m_pCaptionDC->SetTextColor(m_clrCaptionTextLeft);
	m_pCaptionDC->TextOut(4,m_nCaptionHeight-tm.tmAscent-tm.tmDescent-4,sCaption);


	m_pCaptionDC->SelectObject(m_pCaptionDescrFont);
	m_pCaptionDC->SetTextColor(m_clrCaptionTextRight);
	m_pCaptionDC->GetTextMetrics(&tm);

	CSize		extent;
	extent		=m_pCaptionDC->GetTextExtent(sCaptionDescr);
	m_pCaptionDC->TextOut(endX-4-extent.cx,m_nCaptionHeight-tm.tmAscent-tm.tmDescent-4,sCaptionDescr);

}

void CTreePropertySheet::InvalidateCaption(BOOL bForce)
{
	// 1st check if window is already valid
	if(GetSafeHwnd()!=NULL) {
		CRect	rcInv;

		// Determine caption rectangle
		if(m_bSpecialCaption) {
			rcInv.left	=0;
			rcInv.top	=0;
			rcInv.right	=m_szDefault.cx;
			rcInv.bottom=m_nCaptionHeight;
		}	else
		{
			rcInv.left	=m_nSpaceLeft+m_nTreeWidth+m_nSpaceMid;
			rcInv.top	=m_nSpaceTop+m_nAddonSpaceTop;
			rcInv.right	=rcInv.left+m_szDialog.cx;
			rcInv.bottom=rcInv.top+m_nCaptionHeight;
		}
		// Force redrawing of caption background if necessary
		// (bForce==TRUE), paint new caption text
		SetCaptionText(m_sCaptionText,m_sCaptionDescrText,m_clrCaptionLeft,m_clrCaptionRight,bForce);
		// Invalidate
		InvalidateRect(&rcInv,FALSE);
	}
}

void CTreePropertySheet::SetCaptionFont(const CString& sFaceName,int nHeight,BOOL bNoInvalidate)
{
	if(m_pCaptionFont!=NULL) {
		delete m_pCaptionFont;
		m_pCaptionFont=NULL;
	}
	m_pCaptionFont	=new CFont();
	m_pCaptionFont->CreateFont(	nHeight,0,
								0,0,FW_BOLD,//FW_NORMAL,
								FALSE,FALSE,FALSE,
								DEFAULT_CHARSET,
								OUT_DEFAULT_PRECIS,
								CLIP_DEFAULT_PRECIS,
								DEFAULT_QUALITY,
								FF_DONTCARE|DEFAULT_PITCH,
								sFaceName);
	if(!bNoInvalidate)
		InvalidateCaption();
}

void CTreePropertySheet::SetCaptionDescrFont(const CString& sFaceName,int nHeight,BOOL bNoInvalidate)
{
	if(m_pCaptionDescrFont!=NULL) {
		delete m_pCaptionDescrFont;
		m_pCaptionDescrFont=NULL;
	}
	m_pCaptionDescrFont	=new CFont();
	m_pCaptionDescrFont->CreateFont(	nHeight,0,
								0,0,FW_NORMAL,
								FALSE,FALSE,FALSE,
								DEFAULT_CHARSET,
								OUT_DEFAULT_PRECIS,
								CLIP_DEFAULT_PRECIS,
								DEFAULT_QUALITY,
								FF_DONTCARE|DEFAULT_PITCH,
								sFaceName);
	if(!bNoInvalidate)
		InvalidateCaption();
}

CFont *CTreePropertySheet::GetFont(const CString& sFacename,int nHeight)
{
	CFont *temp;
	temp=new CFont();
	temp->CreateFont( nHeight,0,
					  0,0,FW_NORMAL,
					  FALSE,FALSE,FALSE,
					  DEFAULT_CHARSET,
					  OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS,
					  DEFAULT_QUALITY,
					  FF_DONTCARE|DEFAULT_PITCH,
					  sFacename);
	m_apFonts.Add(temp);
	return temp;
}

/////////////////////////////////////////////////////////////////////////////
// CTreePropertySheet extra control methods


// Set no. of extra rows in part 'nVertArrange'
// (nVertArrange==tps_above: above standard controls,
//  nVertArrange==tps_below: below ...)
void CTreePropertySheet::SetExtraRows(int nVertArrange,int nRows)
{
	CArray<CAddonRow,CAddonRow&>	*paRow;

	if(nVertArrange==tps_above)	paRow=&m_aUpperRows;
	else						paRow=&m_aLowerRows;

	paRow->SetSize(nRows);
}

// Set top/bottom empty space for the given row
void CTreePropertySheet::SetExtraRowSpace(int nVertArrange,int nRow,int nSpaceTop,int nSpaceBottom)
{
	ASSERT(nVertArrange==tps_above || nVertArrange==tps_below);
	CArray<CAddonRow,CAddonRow&>	*paRow;

	if(nVertArrange==tps_above)	paRow=&m_aUpperRows;
	else						paRow=&m_aLowerRows;

	ASSERT(nRow>=0);
	ASSERT(nRow<=paRow->GetSize());

	(paRow->ElementAt(nRow)).nSpaceTop=nSpaceTop;
	(paRow->ElementAt(nRow)).nSpaceBottom=nSpaceBottom;
}

// Set no. of columns for given row
void CTreePropertySheet::SetExtraColumns(int nVertArrange,int nRow,int nCols)
{
	ASSERT(nVertArrange==tps_above || nVertArrange==tps_below);
	CArray<CAddonRow,CAddonRow&>	*paRow;

	if(nVertArrange==tps_above)	paRow=&m_aUpperRows;
	else						paRow=&m_aLowerRows;

	ASSERT(nRow>=0);
	ASSERT(nRow<=paRow->GetSize());

	(paRow->ElementAt(nRow)).aColCtrls.SetSize(nCols);
}

void CTreePropertySheet::InsertExtraControl(int nVertArrange,int zOrder,CWnd *pWnd,int row,int col,int nWidth,int nHeight,int nArrange,BOOL bAutoDelete,BOOL bSupportsInterface)
{
	ASSERT(nVertArrange==tps_above || nVertArrange==tps_below);
	CArray<CAddonRow,CAddonRow&>	*paRow;

	if(nVertArrange==tps_above)	paRow=&m_aUpperRows;
	else						paRow=&m_aLowerRows;

	int		nRows=paRow->GetSize();
	ASSERT(row>=0);
	if(row>=nRows) {
		row=nRows;
		SetExtraRows(nVertArrange,++nRows);
	}
	
	CAddonRow	&cRow=paRow->ElementAt(row);
	int		nCols=cRow.aColCtrls.GetSize();
	ASSERT(col>=0);
	if(col>=nCols) {
		col=nCols;
		SetExtraColumns(nVertArrange,row,++nCols);
	}
	AddExtraControl(nVertArrange,zOrder,pWnd,row,col,nWidth,nHeight,nArrange,bAutoDelete,bSupportsInterface);
}

// Add control (zOrder==0: topmost)
// bAutoDelete: Delete on destruction of CTreePropertySheet
// nVertArrange,row,col: Position
void CTreePropertySheet::AddExtraControl(int nVertArrange,int zOrder,CWnd *pWnd,int row,int col,int nWidth,int nHeight,int nArrange,BOOL bAutoDelete,BOOL bSupportsInterface)
{
	CArray<CAddonRow,CAddonRow&>	*paRow;

	if(nVertArrange==tps_above)	paRow=&m_aUpperRows;
	else						paRow=&m_aLowerRows;

	CAddonControl	ctrl;
	CRect			rcTemp;

	pWnd->GetWindowRect(&rcTemp);

	ctrl.zOrder	=zOrder;
	ctrl.pWnd	=pWnd;
	ctrl.nArrange=nArrange;
	ctrl.nWidth	=(nWidth==0)?rcTemp.Width():nWidth;
	ctrl.nHeight=(nHeight==0)?rcTemp.Height():nHeight;
	ctrl.szCtrl	=rcTemp.Size();
	ctrl.bSupportsInterface=bSupportsInterface;
	ctrl.bAutoDelete=bAutoDelete;

	(paRow->ElementAt(row)).aColCtrls[col]=ctrl;
}

// Add space filler 'control' at row,col,nVertArrange
// nWidth=0: Fill unused space(takes up as many space as available)
// nWidth!=0: With exactly defined.
void CTreePropertySheet::AddExtraSpace(int nVertArrange,int row,int col,int nWidth,int nHeight)
{
	CArray<CAddonRow,CAddonRow&>	*paRow;

	if(nVertArrange==tps_above)	paRow=&m_aUpperRows;
	else						paRow=&m_aLowerRows;

	CAddonControl	ctrl;
	CRect			rcTemp;

	ctrl.zOrder	=-1;
	ctrl.pWnd	=NULL;
	ctrl.nArrange=tps_horz_left|tps_vert_top;
	ctrl.nWidth	=nWidth;
	ctrl.nHeight=nHeight;
	ctrl.szCtrl.cx=nWidth;
	ctrl.szCtrl.cy=nHeight;
	ctrl.bAutoDelete=FALSE;

	(paRow->ElementAt(row)).aColCtrls[col]=ctrl;
}

void CTreePropertySheet::InsertExtraSpace(int nVertArrange,int row,int col,int nWidth,int nHeight)
{
	ASSERT(nVertArrange==tps_above || nVertArrange==tps_below);
	CArray<CAddonRow,CAddonRow&>	*paRow;

	if(nVertArrange==tps_above)	paRow=&m_aUpperRows;
	else						paRow=&m_aLowerRows;

	int		nRows=paRow->GetSize();
	ASSERT(row>=0);
	if(row>=nRows) {
		row=nRows;
		SetExtraRows(nVertArrange,++nRows);
	}
	
	CAddonRow	&cRow=paRow->ElementAt(row);
	int		nCols=cRow.aColCtrls.GetSize();
	ASSERT(col>=0);
	if(col>=nCols) {
		col=nCols;
		SetExtraColumns(nVertArrange,row,++nCols);
	}
	AddExtraSpace(nVertArrange,row,col,nWidth,nHeight);
}

// Helper functions

// Get height for given row(pixels)
int CTreePropertySheet::GetExtraRowHeight(int nVertArrange,int nRow)
{
	CArray<CAddonRow,CAddonRow&>	*paRow;

	if(nVertArrange==tps_above)	paRow=&m_aUpperRows;
	else						paRow=&m_aLowerRows;

	CAddonRow	&cRow=paRow->ElementAt(nRow);
	int			col,nCols=cRow.aColCtrls.GetSize();
	int			nHeight=0;
	for(col=0;col<nCols;col++)
		nHeight	=max(max(cRow.aColCtrls[col].szCtrl.cy,cRow.aColCtrls[col].nHeight),nHeight);

	return nHeight+cRow.nSpaceTop+cRow.nSpaceBottom;
}

// Position all extra controls
// (called from within OnCreate)
void CTreePropertySheet::AlignExtraControls()
{
	CArray<CAddonRow,CAddonRow&>	*paRow;
	int	nVertArrange;
	int	cyPos=0;
	int nLastZOrder=0;

	// Do this for both halfs (tps_above, tps_below)
	for(nVertArrange=tps_above;nVertArrange<=tps_below;nVertArrange++) {
		if(nVertArrange==tps_above)	paRow=&m_aUpperRows;
		else						paRow=&m_aLowerRows;

		int		nRows=paRow->GetSize();
		int		row;

		// Enumerate over all rows in actual half
		for(row=0;row<nRows;row++) {
			int			nRowHeight=GetExtraRowHeight(nVertArrange,row);
			CAddonRow	&cRow=paRow->ElementAt(row);

			cRow.nPosY	=cyPos;
			cyPos		+=nRowHeight;

			int	col,nCols=cRow.aColCtrls.GetSize();
			int xLength=0;
			int nSpaceFillers=0;

			// 1st: Calculate how many 'space fillers'
			// exist(AddExtraSpace with nWidth=0),
			// and calculate total width of non-space fillers
			for(col=0;col<nCols;col++) {
				CAddonControl &ctrl=cRow.aColCtrls[col];
				if(ctrl.pWnd==NULL && ctrl.nWidth==-1)
					nSpaceFillers++;
				else
					xLength	+=max(ctrl.nWidth,ctrl.szCtrl.cx);
			}

			// More than zero space fillers?
			// Then divide the remaining space between these
			if(nSpaceFillers!=0) {
				for(col=0;col<nCols;col++) {
					CAddonControl &ctrl=cRow.aColCtrls[col];
					if(ctrl.nWidth==-1) {
						if(ctrl.pWnd==NULL)
							ctrl.szCtrl.cx=(m_szDefault.cx-xLength)/nSpaceFillers;
						ctrl.nWidth=(m_szDefault.cx-xLength)/nSpaceFillers;
					}
				}
			}

			// Align controls
			int	xPos=0;
			for(col=0;col<nCols;col++) {
				CAddonControl &ctrl=cRow.aColCtrls[col];
				int		xPosMin,xPosMax,xTemp;
				int		yPosMin,yPosMax,yTemp;

				// Determine vertical position
				yPosMin	=cRow.nPosY+cRow.nSpaceTop;
				yPosMax	=cRow.nPosY+nRowHeight-cRow.nSpaceBottom;

				if(nVertArrange==tps_above && m_bSpecialCaption) {
					yPosMin+=m_nCaptionHeight;
					yPosMax+=m_nCaptionHeight;
				}

				// Determine horz. position
				xPosMin	=xPos;
				xPosMax	=xPos+max(ctrl.nWidth,ctrl.szCtrl.cx);
				xPos	+=max(ctrl.nWidth,ctrl.szCtrl.cx);

				// Align control vertically
				int		yAlign=ctrl.nArrange&tps_vert_mask;
				if(yAlign==tps_vert_top)
					yTemp	=yPosMin;
				if(yAlign==tps_vert_bottom)
					yTemp	=yPosMax-ctrl.szCtrl.cy;
				if(yAlign==tps_vert_center)
					yTemp	=(yPosMin+yPosMax-ctrl.szCtrl.cy)/2;

				// Align control horizontally
				int		xAlign=ctrl.nArrange&tps_horz_mask;
				if(xAlign==tps_horz_left)
					xTemp	=xPosMin;
				if(xAlign==tps_horz_right)
					xTemp	=xPosMax-ctrl.szCtrl.cx;
				if(xAlign==tps_horz_center)
					xTemp	=(xPosMin+xPosMax-ctrl.szCtrl.cx)/2;

				// Move control to its position
				if(ctrl.pWnd!=NULL) {
					ctrl.pWnd->SetParent(this);
					ctrl.pWnd->MoveWindow(xTemp,yTemp,ctrl.szCtrl.cx,ctrl.szCtrl.cy);
					ctrl.pWnd->ShowWindow(SW_SHOW);
					
					// m_apCtlOrder contains the z-Order
					// of the controls
					m_apCtlOrder.SetAtGrow(ctrl.zOrder,&ctrl);
					nLastZOrder=max(ctrl.zOrder,nLastZOrder);
				}
			}
		}
		if(nVertArrange==tps_above) {
			m_nAddonSpaceTop	=cyPos;
			cyPos	+=m_szDefault.cy;
		}	else
		{
			m_nAddonSpaceBottom	=cyPos-m_nAddonSpaceTop-m_szDefault.cy;
		}
	}

	m_szDefault.cy	=cyPos;

	// Bring controls in the right z-Order
	int	i;
	for(i=(nLastZOrder-1);i>=0;i--) {
		if(m_apCtlOrder[i]==NULL) continue;
		m_apCtlOrder[i]->pWnd->SetWindowPos(&wndTop,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	}
}

void CTreePropertySheet::OnOk()
{
	// 属性页数据校验,如果出错,则返回出错的控件ID
	int	i,nSize=m_aDlgItems.GetSize();
	for(i=0;i<nSize;i++) 
	{
		if(m_aDlgItems[i].pDialog==NULL) continue;
		int nID = m_aDlgItems[i].pDialog->SendMessage(WM_VARIFYPARAM,0,0);
		if(nID != 0)
		{	// 数据校验失败!
			AfxMessageBox(IDS_TREEPROPSHEET_DATAVERIFYFAIL);
			return;
		}
	}

	EnableWindow(FALSE);
	EndModalLoop(IDOK);
/*
	if(m_pCurItem!=NULL && m_pCurItem->pDialog!=NULL) {
		HWND hwndOkBtn=m_pCurItem->pDialog->GetDlgItem(IDOK)->GetSafeHwnd();
		m_pCurItem->pDialog->SendMessage(msgCmdPass,MAKELONG(IDOK,BN_CLICKED),(LPARAM)hwndOkBtn);
	}
*/
	//int	i,nSize=m_aDlgItems.GetSize();
	for(i=0;i<nSize;i++) {
		if(m_aDlgItems[i].pDialog==NULL) continue;
		HWND hwndOkBtn=m_aDlgItems[i].pDialog->GetDlgItem(IDOK)->GetSafeHwnd();
		m_aDlgItems[i].pDialog->SendMessage(msgCmdPass,MAKELONG(IDOK,BN_CLICKED),(LPARAM)hwndOkBtn);
	}
}

void CTreePropertySheet::OnCancel()
{
	EnableWindow(FALSE);
	EndModalLoop(IDCANCEL);
	if(m_pCurItem!=NULL && m_pCurItem->pDialog!=NULL) {
		HWND hwndCancelBtn=m_pCurItem->pDialog->GetDlgItem(IDCANCEL)->GetSafeHwnd();
		m_pCurItem->pDialog->SendMessage(msgCmdPass,MAKELONG(IDCANCEL,BN_CLICKED),(LPARAM)hwndCancelBtn);
	}
}

void CTreePropertySheet::OnApply()
{
	int	i,nSize=m_aDlgItems.GetSize();

	// 属性页数据校验,如果出错,则返回出错的控件ID
	for(i=0;i<nSize;i++) 
	{
		if(m_aDlgItems[i].pDialog==NULL) continue;
		int nID = m_aDlgItems[i].pDialog->SendMessage(WM_VARIFYPARAM,0,0);
		if(nID != 0)
		{	// 数据校验失败!
			AfxMessageBox(IDS_TREEPROPSHEET_DATAVERIFYFAIL);
			return;
		}
	}

	for(i=0;i<nSize;i++) {
		if(m_aDlgItems[i].pDialog==NULL) continue;
		HWND hwndOkBtn=m_aDlgItems[i].pDialog->GetDlgItem(IDOK)->GetSafeHwnd();
		m_aDlgItems[i].pDialog->SendMessage(msgCmdPass,MAKELONG(IDOK,BN_CLICKED),(LPARAM)hwndOkBtn);
		CItem	&item=m_aDlgItems[i];
		if(item.pDialog!=NULL) {
			if(m_pCurItem==&item)
				item.pDialog->ShowWindow(SW_SHOW);
			else
				item.pDialog->ShowWindow(SW_HIDE);
		}
	}
}

void CTreePropertySheet::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(nFlags&MK_LBUTTON) {
		if(m_bSpecialCaption && m_bLButtonDown) {
			DWORD delay=GetTickCount()-m_dwTicksLButtonDown;
			if(delay>200) {
				CRect	rcWnd;
				int		dx,dy;

				ClientToScreen(&point);

				dx		=m_pntLButtonDown.x-point.x;
				dy		=m_pntLButtonDown.y-point.y;
				m_pntLButtonDown=point;

				GetWindowRect(&rcWnd);
				rcWnd.OffsetRect(-dx,-dy);
				MoveWindow(&rcWnd);
			}
		}
	}	else
	{
		::ReleaseCapture();
		m_bLButtonDown=FALSE;
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void CTreePropertySheet::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_bSpecialCaption) {
		if(point.y<m_nCaptionHeight) {
			ClientToScreen(&point);
			SetCapture();
			m_bLButtonDown	=TRUE;
			m_dwTicksLButtonDown=GetTickCount();
			m_pntLButtonDown=point;
			return;
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CTreePropertySheet::OnLButtonUp(UINT nFlags, CPoint point) 
{
	::ReleaseCapture();
	m_bLButtonDown=FALSE;
	
	CWnd::OnLButtonUp(nFlags, point);
}


/////////////////////////////////////////////////////////////////////////////
// CTreePropertySheet 'style sheets'

// Netscape communicator-like outfit
void CTreePropertySheet::SetLikeNetscape(BOOL bSpecialCaption)
{
	CXTButton *okBtn=new CXTButton();
	CXTButton *cancelBtn=new CXTButton();
	CXTButton *applyBtn=new CXTButton();
	CXTButton *helpBtn=new CXTButton();

	PLAT_LOADSTRING(strCtrlOK, IDS_TREEPROPSHEET_CTRL_OK); // 确定
	PLAT_LOADSTRING(strCtrlCancel, IDS_TREEPROPSHEET_CTRL_CANCEL); // 取消
	PLAT_LOADSTRING(strCtrlApply, IDS_TREEPROPSHEET_CTRL_APPLY); // 应用
	PLAT_LOADSTRING(strCtrlHelp, IDS_TREEPROPSHEET_CTRL_HELP); // 帮助

	okBtn->Create(strCtrlOK,WS_CHILD|WS_TABSTOP,CRect(0,0,70,20),GetDesktopWindow(),IDOK);
	cancelBtn->Create(strCtrlCancel,WS_CHILD|WS_TABSTOP,CRect(0,0,70,20),GetDesktopWindow(),IDCANCEL);
	applyBtn->Create(strCtrlApply,WS_CHILD|WS_TABSTOP,CRect(0,0,70,20),GetDesktopWindow(),ID_APPLY);
	helpBtn->Create(strCtrlHelp,WS_CHILD|WS_TABSTOP,CRect(0,0,70,20),GetDesktopWindow(),IDHELP);

	UINT style = okBtn->GetButtonStyle();
	okBtn->SetButtonStyle(style | BS_DEFPUSHBUTTON);
	helpBtn->EnableWindow(FALSE);

	okBtn->SetXButtonStyle(BS_XT_HILITEPRESSED | BS_XT_SHOWFOCUS | BS_XT_XPFLAT);
	cancelBtn->SetXButtonStyle(BS_XT_HILITEPRESSED | BS_XT_SHOWFOCUS | BS_XT_XPFLAT);
	applyBtn->SetXButtonStyle(BS_XT_HILITEPRESSED | BS_XT_SHOWFOCUS | BS_XT_XPFLAT);
	helpBtn->SetXButtonStyle(BS_XT_HILITEPRESSED | BS_XT_SHOWFOCUS | BS_XT_XPFLAT);

	PLAT_LOADSTRING(strFont, IDS_FONT_TREEPROPSHEET); // 宋体

	CFont *tempF=GetFont(strFont,14);
	tempF=GetFont(strFont,12);
	okBtn->SetFont(tempF);
	cancelBtn->SetFont(tempF);
	applyBtn->SetFont(tempF);
	helpBtn->SetFont(tempF);

	m_clrCaptionLeft	=RGB(128,128,128);
	m_clrCaptionRight	=RGB(192,192,192);
	m_clrCaptionTextLeft=RGB(0,0,0);
	m_clrCaptionTextRight=RGB(0,0,0);

	m_nCaptionBorder	=tps_capborder_none;
	m_bSpecialCaption	=bSpecialCaption;

	InsertExtraSpace(tps_below,0,0);
	InsertExtraControl(tps_below,0,okBtn,0,1,75,8,tps_horz_right|tps_vert_center,TRUE);
	InsertExtraControl(tps_below,1,cancelBtn,0,2,75,8,tps_horz_right|tps_vert_center,TRUE);
	InsertExtraControl(tps_below,2,applyBtn,0,3,75,8,tps_horz_right|tps_vert_center,TRUE);
	InsertExtraControl(tps_below,3,helpBtn,0,4,75,16,tps_horz_right|tps_vert_center,TRUE);
	InsertExtraSpace(tps_below,0,5,8);

	//InsertExtraControl(tps_above,3,info,0,0,116,14,tps_horz_right|tps_vert_center,TRUE);
	//SetExtraRowSpace(tps_above,0,8,0);
	SetExtraRowSpace(tps_below,0,0,16);
}

// 'Fancy' outfit
void CTreePropertySheet::SetFancy(BOOL bSpecialCaption)
{
	CButton *okBtn=new CButton();
	CButton *cancelBtn=new CButton();
	CButton *applyBtn=new CButton();
	CButton *helpBtn=new CButton();

//	CTreePropertySheetAnimateWidget *fancyCtl=
//		new CTreePropertySheetAnimateWidget(GetDesktopWindow(),
//											RGB(128,128,128),
//											RGB(128,128,255),
//											RGB(128,128,255),
//											RGB(128,255,255),10);

	PLAT_LOADSTRING(strCtrlOK, IDS_TREEPROPSHEET_CTRL_OK); // 确定
	PLAT_LOADSTRING(strCtrlCancel, IDS_TREEPROPSHEET_CTRL_CANCEL); // 取消
	PLAT_LOADSTRING(strCtrlApply, IDS_TREEPROPSHEET_CTRL_APPLY); // 应用
	PLAT_LOADSTRING(strCtrlHelp, IDS_TREEPROPSHEET_CTRL_HELP); // 帮助

	okBtn->Create(strCtrlOK,WS_CHILD|WS_TABSTOP,CRect(0,0,70,20),GetDesktopWindow(),IDOK);
	cancelBtn->Create(strCtrlCancel,WS_CHILD|WS_TABSTOP,CRect(0,0,70,20),GetDesktopWindow(),IDCANCEL);
	applyBtn->Create(strCtrlApply,WS_CHILD|WS_TABSTOP,CRect(0,0,70,20),GetDesktopWindow(),ID_APPLY);
	helpBtn->Create(strCtrlHelp,WS_CHILD|WS_TABSTOP,CRect(0,0,70,20),GetDesktopWindow(),IDHELP);

	UINT style = okBtn->GetButtonStyle();
	okBtn->SetButtonStyle(style | BS_DEFPUSHBUTTON);
	helpBtn->EnableWindow(FALSE);

	//okBtn->SetXButtonStyle(BS_XT_HILITEPRESSED | BS_XT_SHOWFOCUS | BS_XT_XPFLAT);
	//cancelBtn->SetXButtonStyle(BS_XT_HILITEPRESSED | BS_XT_SHOWFOCUS | BS_XT_XPFLAT);
	//applyBtn->SetXButtonStyle(BS_XT_HILITEPRESSED | BS_XT_SHOWFOCUS | BS_XT_XPFLAT);
	//helpBtn->SetXButtonStyle(BS_XT_HILITEPRESSED | BS_XT_SHOWFOCUS | BS_XT_XPFLAT);
	//okBtn->SetBitmap(CSize(16, 16), IDB_OK);

	PLAT_LOADSTRING(strFont, IDS_FONT_TREEPROPSHEET); // 宋体

	CFont *tempF=GetFont(strFont,14);
	tempF=GetFont(strFont,12);
	okBtn->SetFont(tempF);
	cancelBtn->SetFont(tempF);
	applyBtn->SetFont(tempF);
	helpBtn->SetFont(tempF);

	//m_clrCaptionLeft	=RGB(128,192,225);
	//m_clrCaptionRight	=RGB(192,255,192);
	//m_clrCaptionTextLeft=RGB(0,0,0);
	//m_clrCaptionTextRight=RGB(0,0,0);

	//m_clrCaptionLeft	=RGB(225,192,128);
	//m_clrCaptionRight	=RGB(192,255,192);
	m_clrCaptionLeft	=RGB(50,210,50);
	m_clrCaptionRight	=RGB(235,235,15);
	m_clrCaptionTextLeft=RGB(0,0,255);
	m_clrCaptionTextRight=RGB(255,0,0);
	
	m_nCaptionHeight	=24;
	m_nCaptionBorder	=tps_capborder_none;//tps_capborder_rect;
	m_bSpecialCaption	=bSpecialCaption;

	InsertExtraSpace(tps_below,0,0);
	InsertExtraControl(tps_below,0,okBtn,0,1,75,8,tps_horz_right|tps_vert_center,TRUE);
	InsertExtraControl(tps_below,1,cancelBtn,0,2,75,8,tps_horz_right|tps_vert_center,TRUE);
	InsertExtraControl(tps_below,2,applyBtn,0,3,75,8,tps_horz_right|tps_vert_center,TRUE);
	InsertExtraControl(tps_below,3,helpBtn,0,4,75,16,tps_horz_right|tps_vert_center,TRUE);
	InsertExtraSpace(tps_below,0,5,8);

	//InsertExtraControl(tps_above,3,info,0,0,116,14,tps_horz_right|tps_vert_center,TRUE);
	//SetExtraRowSpace(tps_above,0,8,0);
	SetExtraRowSpace(tps_below,0,0,16);

	m_nSpaceTop		=16;
}

// Simple outfit
void CTreePropertySheet::SetSimple(BOOL bSpecialCaption)
{
	CStatic *info=new CStatic();
	CButton	*okBtn=new CButton();
	CButton *cancelBtn=new CButton();
	CButton *helpBtn=new CButton();

	info->Create("Please select from below:",WS_CHILD,CRect(0,0,200,16),GetDesktopWindow());
	okBtn->Create("OK",WS_CHILD|WS_TABSTOP,CRect(0,0,100,28),GetDesktopWindow(),IDOK);
	cancelBtn->Create("Cancel",WS_CHILD|WS_TABSTOP,CRect(0,0,100,28),GetDesktopWindow(),IDCANCEL);
	helpBtn->Create("Help",WS_CHILD|WS_TABSTOP,CRect(0,0,100,28),GetDesktopWindow(),IDHELP);

	CFont *tempF=GetFont("MS Sans Serif",18);
	info->SetFont(tempF);
	okBtn->SetFont(tempF);
	cancelBtn->SetFont(tempF);
	helpBtn->SetFont(tempF);

	if(bSpecialCaption) {
		m_clrCaptionLeft	=RGB(0,0,128);
		m_clrCaptionRight	=RGB(0,0,128);
	}	else
	{
		m_clrCaptionLeft	=RGB(128,128,128);
		m_clrCaptionRight	=RGB(128,128,128);
		m_clrCaptionBorderTopLeft=RGB(64,64,64);
		m_clrCaptionBorderBottomRight=RGB(255,255,255);
	}
	m_clrCaptionTextLeft=RGB(255,255,255);
	m_clrCaptionTextRight=RGB(255,255,255);

	m_nCaptionHeight	=28;
	m_bSpecialCaption	=bSpecialCaption;

	InsertExtraSpace(tps_below,0,0);
	InsertExtraControl(tps_below,0,okBtn,0,1,116,0,tps_horz_center|tps_vert_center,TRUE);
	InsertExtraControl(tps_below,1,cancelBtn,0,2,116,0,tps_horz_center|tps_vert_center,TRUE);
	InsertExtraControl(tps_below,2,helpBtn,0,3,108,0,tps_horz_right|tps_vert_center,TRUE);
	InsertExtraSpace(tps_below,0,4,16);

	InsertExtraControl(tps_above,3,info,0,0,216,0,tps_horz_right|tps_vert_center,TRUE);
	SetExtraRowSpace(tps_above,0,8,0);
	SetExtraRowSpace(tps_below,0,0,16);
}

// Support for background bitmaps, not fully implemented yet
void CTreePropertySheet::DrawBackground(CDC* pDC, CRect& rcItem)
{
	if(m_pBackgroundBmpDC!=NULL) {
		pDC->BitBlt(0,0,m_szDefault.cx,m_szDefault.cy,
					m_pBackgroundBmpDC,0,0,SRCCOPY);
	}	else
	{
		pDC->FillSolidRect(rcItem,GetSysColor(COLOR_3DFACE));
	}
}

// Support for background bitmaps, not fully implemented yet
void CTreePropertySheet::SetBackgroundBitmap(HBITMAP hbm)
{
	m_hBackgroundBmp	=hbm;
}

// Support for background bitmaps, not fully implemented yet
void CTreePropertySheet::CreateBackgroundDC()
{
	CMemDC		*tempDC;
	CBitmap		*pBmp=CBitmap::FromHandle(m_hBackgroundBmp);
	BITMAP		bmp;
	pBmp->GetBitmap(&bmp);

	m_pBackgroundBmpDC=new CMemDC(m_szDefault.cx,m_szDefault.cy);
	tempDC		=new CMemDC(pBmp);
	int			x,y;
	for(x=0;x<m_szDefault.cx;x+=bmp.bmWidth) {
		for(y=0;y<m_szDefault.cy;y+=bmp.bmHeight) {
			m_pBackgroundBmpDC->BitBlt(
				x,y,bmp.bmWidth,bmp.bmHeight,
				tempDC,0,0,SRCCOPY);
		}
	}
	delete tempDC;
}

// search currently selected dialog for the next
// tab control following to 'hwndCtl'
// (some bugs in windows require some workarounds :-)
// (f.ex. GetNextDlgTabItem(hDlg,NULL,TRUE) returns NULL ...)
HWND CTreePropertySheet::SearchNextTabItemDialog(HWND hwndCtl,BOOL bPrevious)
{
	if(m_pCurItem==NULL || m_pCurItem->pDialog==NULL)
		return NULL;
	
	if(::IsChild(m_pCurItem->pDialog->GetSafeHwnd(),hwndCtl)==FALSE)
		return NULL;

	// manually search for the next tab item
	// (GetNextDlgTabItem() loops thru the items, but we need
	// the end ...)
	HWND	hwndFirst=::GetWindow(m_pCurItem->pDialog->GetSafeHwnd(),GW_CHILD);
	HWND	hwndCur=hwndFirst;
	HWND	hwndNextTab=NULL;
	// 1st: Find hwndCtl(100% secure as IsChild()==TRUE)
	for(;;) {
		if(hwndCur==hwndCtl) break;
		// add by xieyongfang // 以前没有判断IsChild,当不是子窗口时会死循环(例如DropDown ComboBox)
		if(::IsChild(m_pCurItem->pDialog->GetSafeHwnd(),hwndCur))
			hwndCur=::GetWindow(hwndCur,GW_HWNDNEXT);
		else
			break;
	}

	// 2nd: Find next control with WS_TABSTOP set
	if(bPrevious) {
		for(;;) {
			hwndCur=::GetWindow(hwndCur,GW_HWNDPREV);
			if(hwndCur==NULL) break;
			if( (::GetWindowLong(hwndCur,GWL_STYLE)&WS_TABSTOP)==WS_TABSTOP) {
				hwndNextTab=hwndCur;
				break;
			}
		}
	}	else
	{
		for(;;) {
			hwndCur=::GetWindow(hwndCur,GW_HWNDNEXT);
			if(hwndCur==NULL) break;
			if( (::GetWindowLong(hwndCur,GWL_STYLE)&WS_TABSTOP)==WS_TABSTOP) {
				hwndNextTab=hwndCur;
				break;
			}
		}
	}

	return hwndNextTab;
}

// search sheet for the next
// tab control following to 'hwndCtl'
HWND CTreePropertySheet::SearchNextTabItem(HWND hwndCtl, BOOL bPrevious)
{
	if(::IsChild(GetSafeHwnd(),hwndCtl)==FALSE)
		return m_cTreeCtrl.GetSafeHwnd();

	return ::GetNextDlgTabItem(GetSafeHwnd(),hwndCtl,bPrevious);
}

void CTreePropertySheet::OnNextTabItem(BOOL bPrevious)
{
	HWND	hwndCtl=::GetFocus();
	HWND	hwndNext;

	hwndNext=SearchNextTabItemDialog(hwndCtl,bPrevious);

	if(hwndNext==hwndCtl || hwndNext==NULL) {
		hwndNext=SearchNextTabItem(hwndCtl,bPrevious);

		if(m_pCurItem!=NULL && m_pCurItem->pDialog!=NULL) {
			if(m_pCurItem->pDialog->GetSafeHwnd()==hwndNext) {
				if(bPrevious) // workaround
					hwndNext=::GetNextDlgTabItem(hwndNext,::GetWindow(hwndNext,GW_CHILD),bPrevious);
				else
					hwndNext=::GetNextDlgTabItem(hwndNext,NULL,bPrevious);
			}
		}

		if(hwndNext==hwndCtl || hwndNext==NULL)
			hwndNext=m_cTreeCtrl.GetSafeHwnd();
	}

	::SetFocus(hwndNext);
}


/////////////////////////////////////////////////////////////////////////////
// CTreePropertySheetInterface

CTreePropertySheet *CTreePropertySheetInterface::GetSheet(CWnd *pWndThis)
{
	CWnd *pParent=pWndThis->GetParent();
	if(pParent==CWnd::GetDesktopWindow()) return NULL;
	return (CTreePropertySheet*)pWndThis->GetParent();
}



COLORREF interpolate(COLORREF clrLeft, COLORREF clrRight, int nStep, int nSteps)
{
	DWORD	r0,g0,b0;
	DWORD	r1,g1,b1;
	DWORD	rc,gc,bc;

	r0		=(DWORD)(clrLeft&0xFFL);
	g0		=(DWORD)(clrLeft&0xFF00L)>>8;
	b0		=(DWORD)(clrLeft&0xFF0000L)>>16;
	r1		=(DWORD)(clrRight&0xFFL);
	g1		=(DWORD)(clrRight&0xFF00L)>>8;
	b1		=(DWORD)(clrRight&0xFF0000L)>>16;
	rc		=((r1*nStep)+(r0*(nSteps-nStep)))/(nSteps);
	gc		=((g1*nStep)+(g0*(nSteps-nStep)))/(nSteps);
	bc		=((b1*nStep)+(b0*(nSteps-nStep)))/(nSteps);

	return RGB(rc,gc,bc);
}

