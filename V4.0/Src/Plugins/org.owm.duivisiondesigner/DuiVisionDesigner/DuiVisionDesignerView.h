// DuiVisionDesignerView.h : interface of the CDuiVisionDesignerView class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __DuiVisionDesignerVIEW_H__
#define __DuiVisionDesignerVIEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "timer.h"
#include "IDuiHostWnd.h"

// 窗口背景模式
enum
{
	BKTYPE_IMAGE_RESOURCE	= 0,	// 图片资源
	BKTYPE_COLOR,					// 指定颜色
	BKTYPE_IMAGE_FILE,				// 图片文件
};

/////////////////////////////////////////////////////////////////////////////
// CDuiVisionDesignerView view
class CDuiVisionDesignerDoc;
class CDuiVisionDesignerView : public CView, public CTimer
{
protected: // create from serialization only
	CDuiVisionDesignerView();
	DECLARE_DYNCREATE(CDuiVisionDesignerView)

// Attributes
public:
	CDuiVisionDesignerDoc* GetDocument();

	ULONG			m_ulRefCount;			// 内嵌接口类使用的引用计数,vcicomm使用
	IDuiPluginPanel* m_pDuiPluginPanelObject; // DUI界面插件Panel对象

	UINT			m_uTimerAnimation;			// 动画定时器
	UINT			m_uTimerAutoClose;			// 用于窗口自动关闭的定时器ID

	CToolTipCtrl	m_wndToolTip;				// Tooltip
	int				m_nTooltipCtrlID;			// 当前Tooltip显示的控件ID

	BOOL			m_bChangeSize;				// 改变窗口大小
	CSize			m_MinSize;					// 窗口限定最小大小

	COLORREF		m_clrBK;					// 背景颜色,由背景图片计算出的平均色,当背景图片不够大时可以达到渐变色效果
	CBitmap			m_BKImage;					// 框架背景图片对象
	CSize			m_sizeBKImage;				// 背景图片大小
	//CString			m_strFramePicture;			// 背景图片
	CDC				m_MemBKDC;					// 背景dc(由背景图片或背景颜色生成)
	CBitmap			*m_pOldMemBK;
	CBitmap			m_MemBK;
	BOOL			m_bDrawImage;				// 图片或纯色背景

	CString			m_strBkImg;					// 背景图片
	COLORREF		m_crlBack;					// 背景颜色
	COLORREF		m_crlBackTransParent;		// 背景透明颜色
	int				m_nBackTranslucent;			// 背景透明度

	int				m_nOverRegioX;				// 过度的大小
	int				m_nOverRegioY;				// 过度的大小

// Operations
public:
	CString GetDuiVisionPluginPath();

	// 定时器消息
	virtual void OnTimer(UINT uTimerID);
	virtual void OnTimer(UINT uTimerID, CString strTimerName);

	// 初始化窗口背景皮肤(加载到背景内存dc)
	void InitWindowBkSkin();
	// 加载窗口背景图片
	void LoadBackgroundImage(CString strFileName);
	// 设置窗口背景透明度
	void SetupBackTranslucent();
	// 画背景图片
	void DrawBackground(CBitmap &bitBackground);
	// 画背景颜色
	void DrawBackground(COLORREF clr);
	// 画Area控件的半透明层
	void DrawAreaControl(CDC &dc, CRect rcArea, COLORREF clr, int nBeginTransparent, int nEndTransparent);
	// 画背景和Area控件到内存dc
	void DrawBackgroundAndAreas(CDC &dc, const CRect &rcClient, const CRect &rcUpdate);

	// Tooltip操作
	void SetTooltip(int nCtrlID, LPCTSTR lpszTooltip, CRect rect, int nTipWidth);
	void ClearTooltip();
	void  SetTooltipCtrlID(int nTooltipCtrlID) { m_nTooltipCtrlID = nTooltipCtrlID; }
	int  GetTooltipCtrlID() { return m_nTooltipCtrlID; }

protected:
	// 导出的插件宿主窗口功能接口
    BEGIN_INTERFACE_PART(DuiVisionDesignerView, IDuiHostWnd)
		// 平台操作
		STDMETHOD_( CString , GetAppName) ();				// 获取应用程序名
		STDMETHOD_( CString , GetPlatPath) ();				// 获取平台路径
		STDMETHOD_( CString , GetPlatVersion) ();			// 获取平台版本
		STDMETHOD_( int , GetCurrentLanguage) ();			// 获取当前语言
		STDMETHOD_( CString , GetPlatRegistry) ();			// 获取平台注册表根路径
		STDMETHOD_( CString , GetPlatCopyRight) ();			// 获取平台版权字符串
		STDMETHOD_( CString , GetPlatHomeURL) ();			// 获取主页URL
		STDMETHOD_( CString , GetPlatDownloadURL) ();		// 获取下载URL
		STDMETHOD_( int  , SendMessage) (CVciMessage* pIn, CVciMessage* ppOut);	// 发送消息
		STDMETHOD_( int  , ProcessMessage) (CVciMessage* pIn, CVciMessage* ppOut);	// 平台的消息处理
		STDMETHOD_( int  , SendCommand) (int nCmd, WPARAM wParam, LPARAM lParam);	// 发送平台命令
		STDMETHOD_( BOOL , SendCommand) (int nCmd, WPARAM wParam, LPARAM lParam, LPVOID lpResult);	// 发送平台命令

		// DuiVision系统功能
		STDMETHOD_( int  , GetAppID) ();	// 获取DuiVision应用ID

		// 窗口操作
		STDMETHOD_( BOOL , GetWindowBkInfo) (int& nType, int& nIDResource, COLORREF& clr, CString& strImgFile);	// 获取窗口背景信息
		STDMETHOD_( BOOL , SetWindowBkInfo) (int nType, int nIDResource, COLORREF clr, LPCTSTR lpszImgFile);	// 设置窗口背景信息
		STDMETHOD_( void  , ClientToScreen) (LPPOINT lpPoint);	// 坐标转换为屏幕坐标
		STDMETHOD_( HWND  , GetPaintHWnd) ();	// 获取宿主窗口的句柄

		// Tooltip操作
		STDMETHOD_( void  , SetTooltip) (int nCtrlID, LPCTSTR lpszTooltip, CRect rect, int nTipWidth);	// 设置Tooltip
		STDMETHOD_( void , ClearTooltip) ();			// 清除Tooltip
		STDMETHOD_( void , SetTooltipCtrlID) (int nTooltipCtrlID);	// 设置当前Tooltip控件ID
		STDMETHOD_( int , GetTooltipCtrlID) ();		// 获取当前Tooltip控件ID
	END_INTERFACE_PART(DuiVisionDesignerView)
	EXPORT_INTERFACE_PART(DuiVisionDesignerView)

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDuiVisionDesignerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
 //CFormView
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
 //!CRichEditView||CHtmlView
 //CFormView
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDuiVisionDesignerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDuiVisionDesignerView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnFilePrintPreview();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnViewToolbox();
	afx_msg void OnDuiControlCmd(UINT nID);
	afx_msg void OnUpdateDuiControlCmd(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // debug version in DuiVisionDesignerView.cpp
inline CDuiVisionDesignerDoc* CDuiVisionDesignerView::GetDocument()
   { return (CDuiVisionDesignerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !defined(__DuiVisionDesignerVIEW_H__)
