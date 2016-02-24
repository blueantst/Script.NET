// DuiVisionDesignerView.cpp : implementation of the CDuiVisionDesignerView class
//

#include "stdafx.h"
#include "DuiVisionDesigner.h"
#include "DuiVisionDesignerDoc.h"
#include "DuiVisionDesignerView.h"
#include "GlobalFunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDuiVisionDesignerView

IMPLEMENT_DYNCREATE(CDuiVisionDesignerView, CView)

BEGIN_MESSAGE_MAP(CDuiVisionDesignerView, CView)
	//{{AFX_MSG_MAP(CDuiVisionDesignerView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDuiVisionDesignerView construction/destruction

CDuiVisionDesignerView::CDuiVisionDesignerView()
{
	// TODO: add construction code here
	m_ulRefCount = 0;
	m_pDuiPluginPanelObject = NULL;
	m_uTimerAnimation = 0;
	m_uTimerAutoClose = 0;
	m_nTooltipCtrlID = 0;

	m_MinSize.cx = 0;
	m_MinSize.cy = 0;
	m_bChangeSize = FALSE;

	m_nOverRegioX = 100;
	m_nOverRegioY = 100;
	m_sizeBKImage.cx = 100;
	m_sizeBKImage.cy = 100;

	m_clrBK = RGB(186, 226, 239);
	m_crlBackTransParent = RGB(255, 255, 255);

	m_strBkImg = _T("");
	m_crlBack = RGB(0,0,0);
	m_nBackTranslucent = 255;	// 背景透明度,255表示不透明,1表示全透明
}

CDuiVisionDesignerView::~CDuiVisionDesignerView()
{
	CTimer::KillTimer(m_uTimerAnimation);

	if(m_BKImage.m_hObject != NULL)
	{
		m_BKImage.DeleteObject();
		m_MemBKDC.SelectObject(m_pOldMemBK);
		m_MemBK.DeleteObject();
	}

	// 释放DuiVision插件的Panel对象实例
	if((theApp.m_pIPlatUI != NULL) && (m_pDuiPluginPanelObject != NULL))
	{
		theApp.m_pIPlatUI->ReleaseVciObject(m_pDuiPluginPanelObject, FALSE);
	}
}

BOOL CDuiVisionDesignerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// 获取DuiVision插件的路径
CString CDuiVisionDesignerView::GetDuiVisionPluginPath()
{
	if(theApp.m_pIPlatUI != NULL)
	{
		CString strPath = theApp.m_pIPlatUI->GetPlatPath();
		strPath += _T("Plugins\\org.vci.duivision\\");
		return strPath;
	}

	return _T("");
}

// 定时器消息
void CDuiVisionDesignerView::OnTimer(UINT uTimerID)
{
	if(m_uTimerAnimation == uTimerID)	// 动画定时器
	{
		if(m_pDuiPluginPanelObject)
		{
			m_pDuiPluginPanelObject->OnTimer(uTimerID, NULL);
		}
	}else
	if(m_uTimerAutoClose == uTimerID)	// 窗口自动关闭的定时器
	{
		CTimer::KillTimer(m_uTimerAutoClose);
		m_uTimerAutoClose = 0;
		/*if(m_bAutoHide)
		{
			ShowWindow(SW_HIDE);	// 隐藏窗口模式
		}else
		{
			DoClose();	// 关闭窗口模式
		}*/
	}
}

// 定时器消息(带定时器名字的定时函数)
void CDuiVisionDesignerView::OnTimer(UINT uTimerID, CString strTimerName)
{
	// 应用创建的定时器都调用事件处理对象的定时处理函数
	//DuiSystem::Instance()->CallDuiHandlerTimer(uTimerID, strTimerName);
}

// 初始化窗口背景皮肤(加载到背景内存dc)
void CDuiVisionDesignerView::InitWindowBkSkin()
{
	m_strBkImg = _T("skin:");

	int nType = 0;
	COLORREF clr = RGB(0,0,0);
	CString strImgFile = _T("");

	BOOL bRet = TRUE;
	if(!m_strBkImg.IsEmpty())	// 如果窗口设置了背景图片属性，就用此背景图片
	{
		// 通过Skin读取
		if(m_strBkImg.Find(_T("skin:")) == 0)
		{
			strImgFile = GetDuiVisionPluginPath() + _T("bkimg\\SKIN_PIC_0.png");//DuiSystem::Instance()->GetSkin(m_strBkImg);
		}else
		{
			strImgFile = m_strBkImg;
		}

		if(strImgFile.Find(_T(".")) != -1)	// 加载图片文件
		{
			nType = BKTYPE_IMAGE_FILE;
		}
	}else
	if(m_crlBack != RGB(0,0,0))	// 如果窗口设置了背景颜色属性，就用此背景颜色
	{
		nType = BKTYPE_COLOR;
		clr = m_crlBack;
	}else
	{
		// 调用DuiSystem从应用程序获取背景信息
		bRet = FALSE;//DuiSystem::Instance()->GetWindowBkInfo(nType, nIDResource, clr, strImgFile);
	}

	// 设置窗口背景皮肤
	if(bRet)
	{
		if(nType == BKTYPE_COLOR)	// 颜色
		{
			DrawBackground(clr);
		}else
		if(nType == BKTYPE_IMAGE_FILE)	// 图片文件
		{
			LoadBackgroundImage(strImgFile);
		}
	}else
	{
		// 默认加载第一张背景图片
		strImgFile = GetDuiVisionPluginPath() + _T("bkimg\\SKIN_PIC_0.png");//DuiSystem::Instance()->GetSkin(_T("SKIN_PIC_0"));
		LoadBackgroundImage(strImgFile);
	}
}

// 加载窗口背景图片(从文件或zip资源加载)
void CDuiVisionDesignerView::LoadBackgroundImage(CString strFileName)
{
	CBitmap bitBackground;
	if(LoadBitmapFromFile(strFileName, bitBackground, m_sizeBKImage))
	{
		DrawBackground(bitBackground);
	}
}

// 画背景图片
void CDuiVisionDesignerView::DrawBackground(CBitmap &bitBackground)
{
	if(m_MinSize.cx - 2 > m_sizeBKImage.cx || m_MinSize.cy - 2 > m_sizeBKImage.cy || m_bChangeSize)
	{
		if(m_MinSize.cx - 2 > m_sizeBKImage.cx)
		{
			m_nOverRegioX = __min(100, m_sizeBKImage.cx - 2);
		}
		else
		{
			m_nOverRegioX = 0;
		}

		if(m_MinSize.cy - 2 > m_sizeBKImage.cy)
		{
			m_nOverRegioY = __min(100, m_sizeBKImage.cy - 2);
		}
		else
		{
			m_nOverRegioY = 0;
		}
	}
 	else
 	{
 		m_nOverRegioX = 0;
		m_nOverRegioY = 0;
 	}
	if(bitBackground.m_hObject)
	{
		m_bDrawImage = TRUE;
		int nWidth = m_sizeBKImage.cx;
		int nHeight = m_sizeBKImage.cy;

		if(m_MemBKDC.m_hDC)
		{
			m_MemBKDC.DeleteDC();
		}

		CDC *pDC = GetDC();
		
		::GetAverageColor(pDC, bitBackground, m_sizeBKImage, m_clrBK);

		m_MemBKDC.CreateCompatibleDC(pDC);

		if(m_MemBK.m_hObject)
		{
			m_MemBK.DeleteObject();
		}
		m_MemBK.CreateCompatibleBitmap(pDC, m_sizeBKImage.cx, m_sizeBKImage.cy);
		m_pOldMemBK =  m_MemBKDC.SelectObject(&m_MemBK);

		CDC TempDC;
		TempDC.CreateCompatibleDC(pDC);

		CBitmap* pOldBitmap = TempDC.SelectObject(&bitBackground);

		// 画出平均图片
		m_MemBKDC.FillSolidRect(0, 0, nWidth, nHeight, m_clrBK);
		// 设置文字的背景透明
		//m_MemBKDC.SetBkMode(TRANSPARENT);

		if(m_nOverRegioX > 0 && m_nOverRegioY > 0)
		{
			int nOverRegio = __min(m_nOverRegioX, m_nOverRegioY);

			// 左上
			m_MemBKDC.BitBlt(0, 0, nWidth - nOverRegio, nHeight - nOverRegio, &TempDC, 0, 0, SRCCOPY ); 

			// 中间
			CRect rc(0, 0, nWidth, nHeight);
			DrawRightBottomTransition(m_MemBKDC, TempDC, rc, nOverRegio, m_clrBK);

			// 右上
			rc.SetRect(nWidth - nOverRegio, 0, nWidth, nHeight - nOverRegio);
			DrawHorizontalTransition(m_MemBKDC, TempDC, rc, rc, 0, 100);

			// 左下
			rc.SetRect(0, nHeight - nOverRegio, nWidth - nOverRegio, nHeight);
			DrawVerticalTransition(m_MemBKDC, TempDC, rc, rc, 0, 100);
		}
		else if(m_nOverRegioX > 0 && m_nOverRegioY == 0)
		{
			// 左边
			m_MemBKDC.BitBlt(0, 0, nWidth - m_nOverRegioX, nHeight, &TempDC, 0, 0, SRCCOPY ); 

			// 右边
			CRect rc(nWidth - m_nOverRegioX, 0, nWidth, nHeight);
			DrawHorizontalTransition(m_MemBKDC, TempDC, rc, rc, 0, 100);
		}
		else if(m_nOverRegioX == 0 && m_nOverRegioY > 0)
		{
			// 边上
			m_MemBKDC.BitBlt(0, 0, nWidth, nHeight - m_nOverRegioY, &TempDC, 0, 0, SRCCOPY ); 

			// 下边
			CRect rc(0, nHeight - m_nOverRegioY, nWidth, nHeight);
			DrawVerticalTransition(m_MemBKDC, TempDC, rc, rc, 0, 100);
		}
		else
		{
			m_MemBKDC.BitBlt(0, 0, nWidth, nHeight, &TempDC, 0, 0, SRCCOPY ); 
		}

		TempDC.SelectObject(pOldBitmap);
		TempDC.DeleteDC();
		ReleaseDC(pDC);

		InvalidateRect(NULL);
	}
}

// 画背景颜色
void CDuiVisionDesignerView::DrawBackground(COLORREF clr)
{
	m_clrBK = clr;
	m_bDrawImage = FALSE;
	InvalidateRect(NULL);
}

// 画Area控件的半透明层
void CDuiVisionDesignerView::DrawAreaControl(CDC &dc, CRect rcArea, COLORREF clr, int nBeginTransparent, int nEndTransparent)
{
	CRect  rcTemp(0, 0, rcArea.Width(), rcArea.Height());

	CDC TempDC;
	TempDC.CreateCompatibleDC(&dc);

	CBitmap memBmpTemp;
	memBmpTemp.CreateCompatibleBitmap(&dc, rcArea.Width(), rcArea.Height());
	CBitmap *pOldmap =  TempDC.SelectObject(&memBmpTemp);

	TempDC.FillSolidRect(&rcTemp, clr);		

	DrawVerticalTransition(dc, TempDC, rcArea, rcArea, nBeginTransparent, nEndTransparent);
	
	TempDC.SelectObject(pOldmap);
	memBmpTemp.DeleteObject();
	TempDC.DeleteDC();
}

// 画窗口背景和控件
void CDuiVisionDesignerView::DrawBackgroundAndAreas(CDC &dc, const CRect &rcClient, const CRect &rcUpdate)
{
	// 没有指定背景,则填充背景颜色为背景图片的平均色,当背景图片不够大时起到渐变色的效果
	dc.FillSolidRect(rcUpdate.left, rcUpdate.top, rcUpdate.Width(), rcUpdate.Height(), m_clrBK);

	// 如果指定了背景,已经生成了背景的内存dc,则画背景,从背景内存dc输出到当前dc(背景dc的大小可能小于当前dc)
	if(m_bDrawImage)
	{
		CRect rcBk(1, 1, 1 + m_sizeBKImage.cx, 1 + m_sizeBKImage.cy);
		rcBk = rcBk & rcUpdate;
		if(!rcBk.IsRectEmpty())
		{
			dc.BitBlt(rcBk.left, rcBk.top, rcBk.Width() , rcBk.Height(), &m_MemBKDC, rcBk.left, rcBk.top, SRCCOPY ); 
		}	
	}

	// 画所有的Area控件，实现半透明层
	DrawAreaControl(dc, rcClient, m_crlBackTransParent, 40, 40);
}

// 设置当前的Tooltip
void CDuiVisionDesignerView::SetTooltip(int nCtrlID, LPCTSTR lpszTooltip, CRect rect, int nTipWidth)
{
	m_wndToolTip.DelTool(this, 1);
	m_wndToolTip.Activate(FALSE);
	CString strTooltip = lpszTooltip;
	if(!strTooltip.IsEmpty())
	{
		if(nTipWidth > 0)
		{
			// 指定tooltip的宽度
			m_wndToolTip.SetMaxTipWidth(nTipWidth);
		}
		m_wndToolTip.AddTool(this, strTooltip, rect, 1);	//Tooltip默认都用1作为ID
		m_wndToolTip.Activate(TRUE);
		m_nTooltipCtrlID = nCtrlID;
	}
}

// 清除当前的Tooltip
void CDuiVisionDesignerView::ClearTooltip()
{
	m_wndToolTip.DelTool(this, 1);
}

/////////////////////////////////////////////////////////////////////////////
// CDuiVisionDesignerView drawing

void CDuiVisionDesignerView::OnDraw(CDC* pDC)
{
	CDuiVisionDesignerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	//CRect rcUpdate;
	//GetUpdateRect(&rcUpdate);

	CRect rcClient;
	GetClientRect(rcClient);

	CDC memDC ;
	CBitmap memBitmap ;
	CBitmap* oldBitmap ; // bitmap originally found in CMemDC

	// no real plotting work is performed here, 
	// just putting the existing bitmaps on the client
	// to avoid flicker, establish a memory dc, draw to it 
	// and then BitBlt it to the client
	memDC.CreateCompatibleDC(pDC) ;
	memBitmap.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height()) ;
	oldBitmap = (CBitmap *)memDC.SelectObject(&memBitmap) ;

	// 画背景和Area控件到内存dc
	DrawBackgroundAndAreas(memDC, rcClient, rcClient);
	// 背景复制到内存dc
	//memDC.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), pDC, 0, 0, SRCCOPY);
	// 画白色背景
	//memDC.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), pDC, 0, 0, WHITENESS);

	if (memDC.GetSafeHdc() != NULL)
	{
		// first drop the controls on the memory dc
		if(m_pDuiPluginPanelObject)
		{
			// 画控件
			m_pDuiPluginPanelObject->DrawControl(memDC, rcClient);
			// finally send the result to the display
			pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY) ;
		}
	}

	memDC.SelectObject(oldBitmap) ;
	memDC.DeleteDC();
	memBitmap.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
// CDuiVisionDesignerView printing

void CDuiVisionDesignerView::OnFilePrintPreview() 
{
	//DoPrintPreview (this);
}

BOOL CDuiVisionDesignerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return TRUE;
}

void CDuiVisionDesignerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDuiVisionDesignerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDuiVisionDesignerView diagnostics

#ifdef _DEBUG
void CDuiVisionDesignerView::AssertValid() const
{
	CView::AssertValid();
}

void CDuiVisionDesignerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDuiVisionDesignerDoc* CDuiVisionDesignerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDuiVisionDesignerDoc)));
	return (CDuiVisionDesignerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDuiVisionDesignerView message handlers
void CDuiVisionDesignerView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	get_dll_resource();
	CMenu* pMenu = new CMenu();
	pMenu->LoadMenu(IDR_OWM);
	reset_dll_resource();
	CMenu* pmenu = (CMenu*)pMenu->GetSubMenu(0);
	pmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
			point.x, point.y, this);
	delete pMenu;
}


//////////////////////////////////////////////////////////////////////////////
// 窗口切换时调用
void CDuiVisionDesignerView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if (bActivate)
	{
		// 在属性窗口显示文档信息
		GetDocument()->RefreshDocProperty();
	}
/*
	if (bActivate)
	{
		// 窗口激活时显示OWM菜单
		get_dll_resource();
		CMenu* pMenuTerm = new CMenu();
		pMenuTerm->LoadMenu(IDR_MENU_DuiVisionDesigner);
		reset_dll_resource();
		theApp.m_pIPlatUI->AddExtMenu(pMenuTerm);
	}else
	{
		// 窗口去激活时删除 OWM菜单
		get_dll_resource();
		CMenu* pMenuTerm = new CMenu();
		pMenuTerm->LoadMenu(IDR_MENU_DuiVisionDesigner);
		reset_dll_resource();
		theApp.m_pIPlatUI->DelExtMenu(pMenuTerm);
	}
*/	
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

int CDuiVisionDesignerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}

// 窗口初始化
void CDuiVisionDesignerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	if(theApp.m_pIPlatUI == NULL)
	{
		return;
	}

	HWND hWnd = GetSafeHwnd();
	CDuiVisionDesignerDoc* pDoc = GetDocument();
	if(pDoc == NULL)
	{
		return;
	}

	//CString strResourceXml = "";
	CString strFile = pDoc->GetPathName();

	// 每个窗口需要创建一个DuiVision插件的Panel对象实例
	m_pDuiPluginPanelObject = (IDuiPluginPanel*)theApp.m_pIPlatUI->CreateVciObject("org.vci.duivision", strFile);
	if(m_pDuiPluginPanelObject == NULL)
	{
		return;
	}

	CRect rect;
	GetClientRect(rect);
	m_pDuiPluginPanelObject->OnInit(IDD_ABOUTBOX, hWnd, strFile, rect, &m_xDuiVisionDesignerView);

	m_MinSize.cx = rect.Width();
	m_MinSize.cy = rect.Height();

	// 初始化窗口背景
	InitWindowBkSkin();

	//启动动画定时器
	m_uTimerAnimation = CTimer::SetTimer(30);

	// 初始化Tooltip
	m_wndToolTip.Create(this);

	/*strFile.MakeLower();
	strFile.Replace("\\", "/");
	int nPos = strFile.ReverseFind('/');
	if(nPos != -1)
	{
		CString strFileName = strFile;
		CString strDir = strFile.Left(nPos);
		strFileName.Delete(0, nPos+1);
		strResourceXml = strDir;
		strResourceXml += "xml\\resource.xml";
	}*/
}

// 窗口大小变化
void CDuiVisionDesignerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(m_pDuiPluginPanelObject)
	{
		CRect rect;
		GetClientRect(rect);
		m_pDuiPluginPanelObject->SetRect(rect);
	}
}

void CDuiVisionDesignerView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_pDuiPluginPanelObject)
	{
		m_pDuiPluginPanelObject->OnMouseMove(nFlags, point);
	}

	CView::OnMouseMove(nFlags, point);
}

void CDuiVisionDesignerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_pDuiPluginPanelObject)
	{
		m_pDuiPluginPanelObject->OnLButtonDown(nFlags, point);
	}

	CView::OnLButtonDown(nFlags, point);
}

void CDuiVisionDesignerView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_pDuiPluginPanelObject)
	{
		m_pDuiPluginPanelObject->OnLButtonUp(nFlags, point);
	}

	CView::OnLButtonUp(nFlags, point);
}

void CDuiVisionDesignerView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_pDuiPluginPanelObject)
	{
		m_pDuiPluginPanelObject->OnLButtonDblClk(nFlags, point);
	}

	CView::OnLButtonDblClk(nFlags, point);
}

void CDuiVisionDesignerView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_pDuiPluginPanelObject)
	{
		m_pDuiPluginPanelObject->OnRButtonDown(nFlags, point);
	}

	CView::OnRButtonDown(nFlags, point);
}

void CDuiVisionDesignerView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_pDuiPluginPanelObject)
	{
		m_pDuiPluginPanelObject->OnRButtonUp(nFlags, point);
	}

	CView::OnRButtonUp(nFlags, point);
}

void CDuiVisionDesignerView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_pDuiPluginPanelObject)
	{
		m_pDuiPluginPanelObject->OnRButtonDblClk(nFlags, point);
	}

	CView::OnRButtonDblClk(nFlags, point);
}

BOOL CDuiVisionDesignerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ScreenToClient(&pt);
 	zDelta /= WHEEL_DELTA;
	UINT nFlag = (zDelta>0) ? SB_LINEUP : SB_LINEDOWN;
	zDelta = abs(zDelta);

	BOOL bRet = FALSE;
	if(m_pDuiPluginPanelObject)
	{
		bRet = m_pDuiPluginPanelObject->OnScroll(TRUE, nFlag, pt);
	}

	return bRet;//CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CDuiVisionDesignerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_pDuiPluginPanelObject)
	{
		m_pDuiPluginPanelObject->OnKeyDown(nChar, nRepCnt, nFlags);
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CDuiVisionDesignerView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 禁止刷新背景,否则界面会闪烁
	return FALSE;
	//return CView::OnEraseBkgnd(pDC);
}

BOOL CDuiVisionDesignerView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if ( pMsg->message == WM_MOUSEMOVE ||
		 pMsg->message == WM_LBUTTONDOWN ||
		 pMsg->message == WM_LBUTTONUP )
	{
		m_wndToolTip.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// 插件宿主窗口功能实现
INTERFACE_IMPLEMENT(DuiVisionDesignerView)

// 获取应用程序名
STDMETHODIMP_(CString)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetAppName()
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->GetAppName();
	}
	return _T("");
}

// 获取平台路径
STDMETHODIMP_(CString)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetPlatPath()
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->GetPlatPath();
	}
	return _T("");
}

// 获取平台版本
STDMETHODIMP_(CString)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetPlatVersion()
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->GetPlatVersion();
	}
	return _T("");
}

// 获取当前语言
STDMETHODIMP_(int)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetCurrentLanguage()
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->GetCurrentLanguage();
	}
	return 0;
}

// 获取平台注册表根路径
STDMETHODIMP_(CString)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetPlatRegistry()
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->GetPlatRegistry();
	}
	return _T("");
}

// 获取平台版权字符串
STDMETHODIMP_(CString)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetPlatCopyRight()
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->GetPlatCopyRight();
	}
	return _T("");
}

// 获取主页URL
STDMETHODIMP_(CString)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetPlatHomeURL()
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->GetPlatHomeURL();
	}
	return _T("");
}

// 获取下载URL
STDMETHODIMP_(CString)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetPlatDownloadURL()
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->GetPlatDownloadURL();
	}
	return _T("");
}

// 发送消息
STDMETHODIMP_(int)
CDuiVisionDesignerView::XDuiVisionDesignerView::SendMessage(CVciMessage* pIn, CVciMessage* ppOut)
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->SendMessage(pIn, ppOut);
	}
	return 0;
}

// 平台的消息处理
STDMETHODIMP_(int)
CDuiVisionDesignerView::XDuiVisionDesignerView::ProcessMessage(CVciMessage* pIn, CVciMessage* ppOut)
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->ProcessMessage(pIn, ppOut);
	}
	return 0;
}

// 发送平台命令
STDMETHODIMP_(int)
CDuiVisionDesignerView::XDuiVisionDesignerView::SendCommand(int nCmd, WPARAM wParam, LPARAM lParam)
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->SendCommand(nCmd, wParam, lParam);
	}
	return 0;
}

// 发送平台命令
STDMETHODIMP_(BOOL)
CDuiVisionDesignerView::XDuiVisionDesignerView::SendCommand(int nCmd, WPARAM wParam, LPARAM lParam, LPVOID lpResult)
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->SendCommand(nCmd, wParam, lParam, lpResult);
	}
	return 0;
}

// 获取DuiVision应用ID
STDMETHODIMP_(int)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetAppID()
{
	return 1117;
}

// 获取窗口背景信息
STDMETHODIMP_(BOOL)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetWindowBkInfo(int& nType, int& nIDResource, COLORREF& clr, CString& strImgFile)
{
	return FALSE;
}

// 设置窗口背景信息
STDMETHODIMP_(BOOL)
CDuiVisionDesignerView::XDuiVisionDesignerView::SetWindowBkInfo(int nType, int nIDResource, COLORREF clr, LPCTSTR lpszImgFile)
{
	return FALSE;
}

// 坐标转换为屏幕坐标
STDMETHODIMP_(void)
CDuiVisionDesignerView::XDuiVisionDesignerView::ClientToScreen(LPPOINT lpPoint)
{
	CDuiVisionDesignerView *pObj = GET_INTERFACE_OBJECT(DuiVisionDesignerView);
	pObj->ClientToScreen(lpPoint);
}

// 获取宿主窗口的句柄
STDMETHODIMP_(HWND)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetPaintHWnd()
{
	CDuiVisionDesignerView *pObj = GET_INTERFACE_OBJECT(DuiVisionDesignerView);
	return pObj->GetSafeHwnd();
}

// 设置Tooltip
STDMETHODIMP_(void)
CDuiVisionDesignerView::XDuiVisionDesignerView::SetTooltip(int nCtrlID, LPCTSTR lpszTooltip, CRect rect, int nTipWidth)
{
	CDuiVisionDesignerView *pObj = GET_INTERFACE_OBJECT(DuiVisionDesignerView);
	pObj->SetTooltip(nCtrlID, lpszTooltip, rect, nTipWidth);
}

// 清除Tooltip
STDMETHODIMP_(void)
CDuiVisionDesignerView::XDuiVisionDesignerView::ClearTooltip()
{
	CDuiVisionDesignerView *pObj = GET_INTERFACE_OBJECT(DuiVisionDesignerView);
	pObj->ClearTooltip();
}

// 设置当前Tooltip控件ID
STDMETHODIMP_(void)
CDuiVisionDesignerView::XDuiVisionDesignerView::SetTooltipCtrlID(int nTooltipCtrlID)
{
	CDuiVisionDesignerView *pObj = GET_INTERFACE_OBJECT(DuiVisionDesignerView);
	if(pObj)
	{
		pObj->SetTooltipCtrlID(nTooltipCtrlID);
	}
}

// 获取当前Tooltip控件ID
STDMETHODIMP_(int)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetTooltipCtrlID()
{
	CDuiVisionDesignerView *pObj = GET_INTERFACE_OBJECT(DuiVisionDesignerView);
	if(pObj)
	{
		return pObj->GetTooltipCtrlID();
	}

	return 0;
}