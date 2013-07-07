// HtmlViewOWMView.cpp : implementation of the CHtmlViewOWMView class
//

#include "stdafx.h"
#include "HtmlViewOWM.h"
#include "HtmlViewOWMDoc.h"
#include "HtmlViewOWMView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 全局变量,记录当前解释器指针
static IInterp* g_pInterp = NULL;

IInterp* GetCurrentInterp()
{
	// 如果当前有打开的解释器,就返回指针
	if(g_pInterp)
	{
		return g_pInterp;
	}
	// 否则就创建一个解释器
	g_pInterp = (IInterp*)(theApp.m_pIPlatUI->CreateVciObject("org.interp.tcl", "htmlview"));

	return g_pInterp;
}

// 定义DISPID
#define DISPID_EX_HANDLER	1

// 定义命令
/////////////////////////////////////////////////////////////////////////////
// CMyUIHandler

class CExCommandHandler : public IDispatch
{
	long		refcount;
	_bstr_t		m_bstrParam;	// 参数名
public:

	CExCommandHandler() {refcount = 0;}
	//virtual ~CExCommandHandler() {}

	STDMETHOD(QueryInterface)(REFIID iid, LPVOID* ppvObj)
	{
		if (iid == IID_IDispatch)
		{
			*ppvObj = (IDispatch*)this;
			AddRef();
		}else
		{
			return E_NOINTERFACE;
			//return E_NOTIMPL;
		}
		return S_OK;
	}

	STDMETHOD_(ULONG,AddRef)()
	{
		InterlockedIncrement(&refcount);
		return refcount;
	}

	STDMETHOD_(ULONG,Release)()
	{
		InterlockedDecrement(&refcount);
		if (refcount == 0)
			delete this;
		return refcount;
	}

	STDMETHOD(GetTypeInfoCount)(/* [out] */ UINT *pctinfo)
	{
		return S_OK;
	}

	STDMETHOD(GetTypeInfo)(
      /* [in] */ UINT iTInfo,
      /* [in] */ LCID lcid,
      /* [out] */ ITypeInfo **ppTInfo)
	{
		return S_OK;
	}

	STDMETHOD(GetIDsOfNames)( 
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId)
	{
		m_bstrParam = *rgszNames;
		*rgDispId = DISPID_EX_HANDLER;
		return S_OK;
	}

	STDMETHOD(Invoke)( 
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr)
	{
		if(dispIdMember==DISPID_EX_HANDLER)
		{
			if(wFlags == DISPATCH_PROPERTYGET)	// 获取属性
			{
				IInterp* pInterp = GetCurrentInterp();
				if(pInterp)
				{
					CString strValue = pInterp->GetVar(m_bstrParam);
					COleVariant* pRetVal = (COleVariant*)pVarResult;
					*pRetVal = strValue;
				}
			}else
			if(wFlags == DISPATCH_PROPERTYPUT)	// 设置属性
			{
				IInterp* pInterp = GetCurrentInterp();
				if(pInterp)
				{
					if(pDispParams->cArgs > 0)
					{
						COleVariant* pRetVal = (COleVariant*)(pDispParams->rgvarg);
						CString str(pRetVal->bstrVal);
						pInterp->SetVar(m_bstrParam, str);
					}
				}
			}else
			if((wFlags & DISPATCH_METHOD) != 0)	// 调用方法
			{
				IInterp* pInterp = GetCurrentInterp();
				if(pInterp)
				{
					CStringArray asParam;
					// 说明:参数数组pDispParams为倒序,且命名参数在前面,此处省略命名参数
					int nArgs = pDispParams->cArgs + pDispParams->cNamedArgs;
					int nNamedArgs = pDispParams->cNamedArgs;
					for(int i = nArgs-1; i >= nNamedArgs; i--)
					{
						COleVariant* pRetVal = (COleVariant*)(&(pDispParams->rgvarg[i]));
						CString str(pRetVal->bstrVal);
						asParam.Add(str);
					}
					CString strRes = "";
					if(pInterp->CallMethod(m_bstrParam, asParam))
					{
						strRes = pInterp->GetResult();
					}else
					{
						strRes = pInterp->GetResult();
					}
					if((wFlags & DISPATCH_PROPERTYGET) != 0)
					{
						// 可以获取返回值
						COleVariant* pRetVal = (COleVariant*)pVarResult;
						*pRetVal = strRes;
					}
				}
			}

              //MessageBox(0,"Hello World","Hello",0); //place your code here
			  //frmweb->Edit1->Text="Hello World(这也可以？)";
		}

		return S_OK;
	}
	
};

/////////////////////////////////////////////////////////////////////////////
// CExUIHandler

class CExUIHandler : public IDocHostUIHandler
{
public:

	long refcount;

	BOOL m_vbEnableCtxMenus;
	CExUIHandler():m_vbEnableCtxMenus(FALSE)	// FALSE 表示不显示弹出式菜单
	{
		refcount = 0;
	}
	virtual ~CExUIHandler()
	{
	}

	STDMETHOD(QueryInterface)(REFIID iid, LPVOID* ppvObj)
	{
		if (iid == IID_IUnknown)
		{
			*ppvObj = (IUnknown*)this;
			AddRef();
		}else
		if (iid == IID_IDocHostUIHandler)
		{
			*ppvObj = (IDocHostUIHandler*)this;
			AddRef();
		}else
		{
			return E_NOINTERFACE;
		}
		return S_OK;
	}

	STDMETHOD_(ULONG,AddRef)()
	{
		InterlockedIncrement(&refcount);
		return refcount;
	}

	STDMETHOD_(ULONG,Release)()
	{
		InterlockedDecrement(&refcount);
		if (refcount == 0)
			delete this;
		return refcount;
	}

	STDMETHOD(ShowContextMenu)(DWORD dwID, POINT FAR* ppt, IUnknown FAR* pcmdtReserved,
                              IDispatch FAR* pdispReserved)
	{
		return E_NOTIMPL;
		if (m_vbEnableCtxMenus == TRUE)  // Show Context Menu
			return S_FALSE;
		else
			return S_OK;
	}

	STDMETHOD(GetHostInfo)(DOCHOSTUIINFO FAR *pInfo)
	{
		// 注:此处如果设置为E_NOTIMPL,则页面不会相应任何鼠标和键盘操作
		return S_OK;
		//return E_NOTIMPL;
	}

	STDMETHOD(ShowUI)(DWORD dwID, IOleInPlaceActiveObject FAR* pActiveObject,
                    IOleCommandTarget FAR* pCommandTarget,
                    IOleInPlaceFrame  FAR* pFrame,
                    IOleInPlaceUIWindow FAR* pDoc)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(HideUI)(void)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(UpdateUI)(void)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(EnableModeless)(BOOL fEnable)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnDocWindowActivate)(BOOL fActivate)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnFrameWindowActivate)(BOOL fActivate)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(ResizeBorder)(LPCRECT prcBorder, IOleInPlaceUIWindow FAR* pUIWindow,
                           BOOL fRameWindow)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(TranslateAccelerator)(LPMSG lpMsg, const GUID FAR* pguidCmdGroup,
                                   DWORD nCmdID)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(GetOptionKeyPath)(LPOLESTR FAR* pchKey, DWORD dw)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(GetDropTarget)(IDropTarget* pDropTarget,
				IDropTarget** ppDropTarget)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(GetExternal)(IDispatch** ppDispatch)
	{
		// 创建扩展命令处理器对象
		CExCommandHandler* pHandler = new CExCommandHandler();
		pHandler->QueryInterface(IID_IDispatch, (void**)ppDispatch);
		//*ppDispatch = new CExCommandHandler();
		return S_OK;
	}

	STDMETHOD(TranslateUrl)(DWORD dwTranslate, OLECHAR* pchURLIn,
				OLECHAR** ppchURLOut)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(FilterDataObject)(IDataObject* pDO, IDataObject** ppDORet)
	{
		return E_NOTIMPL;
	}
};



/////////////////////////////////////////////////////////////////////////////
// CHtmlViewOWMView

IMPLEMENT_DYNCREATE(CHtmlViewOWMView, CXTHtmlView)

BEGIN_MESSAGE_MAP(CHtmlViewOWMView, CXTHtmlView)
	//{{AFX_MSG_MAP(CHtmlViewOWMView)
	ON_COMMAND(ID_GO_BACK, OnGoBack)
	ON_UPDATE_COMMAND_UI(ID_GO_BACK, OnUpdateGoBack)
	ON_COMMAND(ID_GO_FORWARD, OnGoForward)
	ON_UPDATE_COMMAND_UI(ID_GO_FORWARD, OnUpdateGoForward)
	ON_COMMAND(ID_VIEW_STOP, OnViewStop)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STOP, OnUpdateViewStop)
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_UPDATE_COMMAND_UI(ID_VIEW_REFRESH, OnUpdateViewRefresh)
	ON_COMMAND(ID_GO_START_PAGE, OnGoStartPage)
	ON_UPDATE_COMMAND_UI(ID_GO_START_PAGE, OnUpdateGoStartPage)
	ON_COMMAND(ID_GO_SEARCH_THE_WEB, OnGoSearchTheWeb)
	ON_UPDATE_COMMAND_UI(ID_GO_SEARCH_THE_WEB, OnUpdateGoSearchTheWeb)
	ON_COMMAND(ID_VIEW_FOLDERS, OnViewFolders)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FOLDERS, OnUpdateViewFolders)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_VIEW_HISTORY, OnViewHistory)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HISTORY, OnUpdateViewHistory)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CXTHtmlView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CXTHtmlView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	// 剪切，粘贴类操作
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHtmlViewOWMView construction/destruction

CHtmlViewOWMView::CHtmlViewOWMView()
{
	// TODO: add construction code here
	CExUIHandler* pUI = new CExUIHandler();
	pUI->QueryInterface(IID_IDocHostUIHandler, (void**)&m_IExUIHandler);
	m_pUI = pUI;
}

CHtmlViewOWMView::~CHtmlViewOWMView()
{
	if(m_pUI)
	{
		delete (CExUIHandler*)m_pUI;
	}
}

BOOL CHtmlViewOWMView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CXTHtmlView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CHtmlViewOWMView drawing

void CHtmlViewOWMView::OnDraw(CDC* pDC)
{
	CHtmlViewOWMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CHtmlViewOWMView printing

void CHtmlViewOWMView::OnFilePrintPreview() 
{
	//DoPrintPreview (this);
	//CXTHtmlView::OnFilePrintPreview();
}

BOOL CHtmlViewOWMView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
	//return TRUE;
}

void CHtmlViewOWMView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CHtmlViewOWMView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CHtmlViewOWMView diagnostics

#ifdef _DEBUG
void CHtmlViewOWMView::AssertValid() const
{
	CXTHtmlView::AssertValid();
}

void CHtmlViewOWMView::Dump(CDumpContext& dc) const
{
	CXTHtmlView::Dump(dc);
}

CHtmlViewOWMDoc* CHtmlViewOWMView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHtmlViewOWMDoc)));
	return (CHtmlViewOWMDoc*)m_pDocument;
}
#endif //_DEBUG

// 剪切，粘贴类操作
void CHtmlViewOWMView::OnEditCut() 
{
      ExecWB(OLECMDID_CUT, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);  
}

void CHtmlViewOWMView::OnEditCopy() 
{
      ExecWB(OLECMDID_COPY, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);  
}

void CHtmlViewOWMView::OnEditPaste() 
{
      ExecWB(OLECMDID_PASTE, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);  
}

void CHtmlViewOWMView::OnEditSelectAll() 
{
      ExecWB(OLECMDID_SELECTALL, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL); 
}

void CHtmlViewOWMView::OnEditUndo() 
{
      ExecWB(OLECMDID_UNDO, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL); 
}

/////////////////////////////////////////////////////////////////////////////
// CHtmlViewOWMView message handlers
void CHtmlViewOWMView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	get_dll_resource();
	CMenu* pMenu = new CMenu();
	pMenu->LoadMenu(IDR_OWM);
	reset_dll_resource();
	CMenu* pmenu = (CMenu*)pMenu->GetSubMenu(0);
	//pmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
	//		point.x, point.y, this);
	//CXTPCommandBars::TrackPopupMenu(pmenu, 0, point.x, point.y, this);
	theApp.m_pIPlatUI->TrackPopupMenu(pmenu, 0, point.x, point.y, this);
	delete pMenu;
}

void CHtmlViewOWMView::OnTitleChange(LPCTSTR lpszText) 
{
	GetDocument()->SetTitle(lpszText);
}

void CHtmlViewOWMView::OnInitialUpdate() 
{
	CXTHtmlView::OnInitialUpdate();
	
	CString strStartScript = theApp.m_strDefaultPage;//"local:OWM://\\html\\start.htm";
	Navigate2(strStartScript);
}

/////////////////////////////////////////////////////////////////////////////
// 保存并设置当前路径
/////////////////////////////////////////////////////////////////////////////
void CHtmlViewOWMView::SaveAndSetCurPath(CString strURL)
{
	// 记录当前路径,并设置当前路径
	memset(m_szOldPath, 0, 256);
	m_dwPathLen = GetCurrentDirectory(255, m_szOldPath);
	CString strPath = strURL;
	strPath.Replace("/", "\\");
	int nPos = strPath.ReverseFind('\\');
	if(nPos >= 0)
	{
		strPath = strPath.Left(nPos);
	}
	SetCurrentDirectory(strPath);
}

/////////////////////////////////////////////////////////////////////////////
// 恢复保存的当前路径
/////////////////////////////////////////////////////////////////////////////
void CHtmlViewOWMView::RestoreCurPath()
{
	// 执行结束,恢复当前路径的设置
	if(m_dwPathLen > 0)
	{
		CString strOldPath = m_szOldPath;
		SetCurrentDirectory(strOldPath);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 处理%替换,将%XX替换会ASCII字符
/////////////////////////////////////////////////////////////////////////////
void ReplaceHtmlStrToAscii(CString& strFormParam)
{
	// 处理%替换
	int nPos = -1;
	int nStartPos = 0;
	int nLen = strFormParam.GetLength();
	while(((nPos=strFormParam.Find('%', nStartPos)) != -1) && (nPos<(nLen-2)))
	{
		if(strFormParam[nPos+1] == '%')
		{
			nStartPos = nPos+2;
		}else
		if(((strFormParam.GetAt(nPos+1)>='0' && strFormParam.GetAt(nPos+1)<='9') ||
			(strFormParam.GetAt(nPos+1)>='A' && strFormParam.GetAt(nPos+1)<='F')) &&
			((strFormParam.GetAt(nPos+2)>='0' && strFormParam.GetAt(nPos+2)<='9') ||
			(strFormParam.GetAt(nPos+2)>='A' && strFormParam.GetAt(nPos+2)<='F')) )
		{
			CString ch1 = strFormParam.GetAt(nPos+1);
			CString ch2 = strFormParam.GetAt(nPos+2);
			int n1,n2;
			if(strFormParam.GetAt(nPos+1) >= 'A')
				n1 = strFormParam.GetAt(nPos+1)-'A'+10;
			else
				n1 = atoi(ch1);
			if(strFormParam.GetAt(nPos+2) >= 'A')
				n2 = strFormParam.GetAt(nPos+2)-'A'+10;
			else
				n2 = atoi(ch2);
			int n = n1*16+n2;
			char ch = char(n);
			strFormParam.Delete(nPos, 3);
			//if(ch == '\\')
			//	strFormParam.Insert(nPos, "\\\\");
			//else
				strFormParam.Insert(nPos, ch);
			nStartPos = nPos+1;
			nLen = strFormParam.GetLength();
		}else
		{
			nStartPos = nPos+2;
		}
	}
}

void CHtmlViewOWMView::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel) 
{
	CString strURL = lpszURL;

	// 获取所有安装的解释器信息
	CStringArray asVciId;
	CStringArray asInterpName;
	CStringArray asInterpId;
	theApp.m_pIPlatUI->GetInstalledVci(asVciId);
	for(int i=0; i<asVciId.GetSize(); i++)
	{
		if(theApp.m_pIPlatUI->GetVciType(asVciId[i]) == _T("interp"))
		{
			asInterpName.Add(theApp.m_pIPlatUI->GetVciName(asVciId[i]));
			asInterpId.Add(asVciId[i]);
		}
	}

	// 在一些Jave的嵌入应用中,不能使用:/等字符,因此需要用这个关键字,
	// 此关键字后面的内容再进行下面的解析

	if( strURL.Find("local:") == 0 )
	{
		// 本地文件
		strURL.Delete(0, 6);
		// 浏览器中会将空格变换为%20,此处变回去
		strURL.Replace("%20", " ");
		// 浏览器中无法输入",因此将引号改为~
		strURL.Replace('~', '"');

		theApp.ProcessPath(strURL);

		Navigate2(strURL);

		*pbCancel = TRUE;

	}else
	// 特殊协议
	if(strURL.Find("pfe:") == 0)		// Platform Event
	{
		// 浏览器中会将空格变换为%20,此处变回去
		strURL.Replace("%20", " ");
		// 浏览器中无法输入",因此将引号改为~
		strURL.Replace('~', '"');

		strURL.Delete(0, 4);
		if(strURL.Find("open") == 0)	// 打开文件
		{
			strURL.Delete(0, 4);
			strURL.TrimLeft();
			theApp.m_pIPlatUI->OpenDocumentFile(strURL, "");
		}else
		if(strURL.Find("openwithowm<") == 0)	// 用指定的OWM打开文件
		{
			strURL.Delete(0, 12);
			int nPos = strURL.Find(">");
			if(nPos != -1)
			{
				CString strOwmId = strURL.Left(nPos);
				strURL.Delete(0, nPos+1);
				strURL.TrimLeft();

				theApp.m_pIPlatUI->OpenDocumentFile(strURL, strOwmId);
			}
		}

		*pbCancel = TRUE;

	}else
	{
		// Interp command

		// 浏览器中会将空格变换为%20,此处变回去
		strURL.Replace("%20", " ");
		// 浏览器中无法输入",因此将引号改为~
		strURL.Replace('~', '"');

		CString strInterpId = "";
		CString strInterpName = "";
		for(i=0; i<asInterpName.GetSize(); i++)
		{
			CString strInterpPre = asInterpName[i] + ":";
			if(strURL.Find(strInterpPre) == 0)
			{
				strInterpId = asInterpId[i];
				strInterpName = asInterpName[i];
				break;
			}
		}

		if(strInterpId != "")
		{
			strURL.Delete(0, strInterpName.GetLength()+1);

			CString strInterp = _T("");
			// 解析解释器名(支持命名方式的解释器)
			// 如果最前面是<<>>包围的字符串,表示是命名方式的
			if(strURL.Find("<<") == 0)
			{
				int nPos = strURL.Find(">>");
				if(nPos != -1)
				{
					strURL.Delete(0, 2);
					strInterp = strURL.Left(nPos-2);
					strURL.Delete(0, nPos);
				}
			}

			// 创建解释器
			IInterp* pInterp = (IInterp*)(theApp.m_pIPlatUI->CreateVciObject(strInterpId, strInterp));
			if(pInterp == NULL)
			{
				*pbCancel = TRUE;
				return;
			}

			if((g_pInterp != pInterp) && (g_pInterp != NULL))
			{
				// 如果解释器已经改变,需要先释放以前的解释其
				theApp.m_pIPlatUI->ReleaseVciObject(g_pInterp, TRUE);
			}
			g_pInterp = pInterp;

			// 在执行脚本之前先设置表示当前页面文件的TCL变量
			CHtmlViewOWMDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			CString strHtmlPath = pDoc->m_strCurURL;
			strHtmlPath.Replace('\\', '/');
			pInterp->SetVar("_htmlCurrentFile", strHtmlPath);

			// 解析HTML中FORM提交过来的参数(GET或POST方式)
			CString strFormParam = "";
			if (baPostedData.GetSize())
			{
				// 如果是POST方式,将POST的内容加到参数后面
				strURL += "?";
				// This is a kludgy copy, but you get the idea
				for(int i = 0;i < baPostedData.GetSize();i++)
				{
					if(baPostedData[i] != 0)
					{
						strURL += (char)baPostedData[i];
					}
				}
			}
			if(strURL.Find("<form>") == 0)	// 最前面有<form>表示是一个FORM提交过来的的URL
			{	// 注:这种只适用于可以带参数的<file>形式TCL调用
				strURL.Delete(0, 6);
				int nPosForm = strURL.Find("?");
				if(nPosForm != -1)
				{
					// 解析出传递的参数
					strFormParam = strURL.Right(strURL.GetLength()-nPosForm-1);
					strURL = strURL.Left(nPosForm);
					if(strFormParam.GetLength() > 0)
					{	// 如果参数为空就不继续处理
						// 处理%替换
						ReplaceHtmlStrToAscii(strFormParam);
						// 将参数用""包围起来作为一个参数传递给TCL
						strFormParam.Replace("\"", "\\\"");
						// 因为解释器内部已经改为用?区分,因此此处必须加上问号
						strURL += "?\"";
						strURL += strFormParam;
						strURL += "\"";
					}
				}
			}

			BOOL bRet = TRUE;
			if(strURL.Find("<file>") == 0)	// 明确指出是执行文件,可以带命令行参数
			{
				strURL.Delete(0, 6);
				theApp.ProcessPath(strURL);	// 处理路径

				//TRACE("[%s]\n", strURL);
				// 保存并设置当前路径
				SaveAndSetCurPath(strURL);

				bRet = pInterp->RunScriptFile(strURL);	// 执行脚本文件

				// 恢复当前路径
				RestoreCurPath();

			}else
			{
				//int nPos = strURL.Find(".tcl");
				//if (nPos == (strURL.GetLength()-4))	// 执行TCL文件,不能带命令行参数
				//{
				//	theApp.ProcessPath(strURL);	// 处理路径
				//	bRet = pInterp->RunScriptFile(strURL);	// 执行脚本文件
				//}else	// 执行TCL命令
				{
					bRet = pInterp->RunScriptCommand(strURL);	// 执行脚本命令
				}
			}

			if(!bRet)	// 脚本执行错误
			{
				::MessageBox(AfxGetMainWnd()->m_hWnd, pInterp->GetResult(), "Error", MB_OK | MB_ICONERROR);
			}else
			{
				// 检查新HTML页面URL赋值变量
				CString strHtmlNewURL = pInterp->GetVar(_T("_htmlNewURL"));
				if(strHtmlNewURL != "")
				{	// 表示页面URL被重新赋值
					Navigate2(strHtmlNewURL);
				}else
				{
					// 检查HTML刷新标记变量
					CString strHtmlRefreshSign = pInterp->GetVar(_T("_htmlRefreshSign"));
					if(strHtmlRefreshSign == "true")
					{	// 表示有页面变化,需要刷新
						Refresh();
					}
				}
			}

			// 判断是否驻留解释器(可以在脚本中设置驻留解释器标志TSRInterp)
			CString strTSRInterp = pInterp->GetInterpParam("TSRInterp");
			if(strTSRInterp == "false")
			{
				// 不需要驻留了,则强制释放解释器
				theApp.m_pIPlatUI->ReleaseVciObject(pInterp, TRUE);
				g_pInterp = NULL;
			}else
			if(strTSRInterp != "true")
			{
				// 非驻留解释器,则释放解释器
				theApp.m_pIPlatUI->ReleaseVciObject(pInterp, FALSE);
				g_pInterp = NULL;
			}

			*pbCancel = TRUE;
		}
		
	}

	if(*pbCancel == TRUE)
	{
		return;	// 操作被取消
	}
	
	CXTHtmlView::OnBeforeNavigate2(lpszURL, nFlags,	lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}

void CHtmlViewOWMView::OnDocumentComplete(LPCTSTR lpszURL)
{
	// user will override to handle this notification
	ICustomDoc * pCustdoc = NULL; 
	IHTMLDocument2Ptr pDoc(GetHtmlDocument());
	if (pDoc)
	{
		pDoc->QueryInterface(&pCustdoc);
	}
	//GetHtmlDocument()->QueryInterface(&pCustdoc);
	if (pCustdoc)
	{
		pCustdoc->SetUIHandler(m_IExUIHandler);  
		//if call pCustdoc->SetUIHandler(m_pHandler),then you can't using
		//mouse to drag-select html's text...why?
		pCustdoc->Release();
	}

	IHTMLElementPtr e = GetHtmlElement("button_tlf1");
	if(e)
	{
		IHTMLInputButtonElementPtr btn = (IHTMLInputButtonElementPtr)e;
		btn->put_disabled(1L);
		//btn->put_value(L"jhdsfihf");
	}

	CXTHtmlView::OnDocumentComplete(lpszURL);
}

IHTMLDocument2Ptr CHtmlViewOWMView::GetHtmlFrameWindowDOM(LPCTSTR lpszFrameId)
{
	IHTMLDocument2Ptr pDoc(GetHtmlDocument());
	if(pDoc == NULL)
	{
		return NULL;
	}

	IHTMLFramesCollection2Ptr piframes = NULL;
	pDoc->get_frames(&piframes);
	if(piframes == NULL)
	{
		return NULL;
	}

	IHTMLWindow2Ptr pifrm = NULL;	
	COleVariant varid(lpszFrameId);
	VARIANT frameOut;
	piframes->item(varid, &frameOut);

	frameOut.pdispVal->QueryInterface(IID_IHTMLWindow2, (void**)&pifrm);
	if(pifrm == NULL)
	{
		return NULL;
	}

	IHTMLDocument2Ptr pDocIFrm = NULL;
	pifrm->get_document(&pDocIFrm);

	return pDocIFrm;
}

IHTMLElementPtr CHtmlViewOWMView::GetHtmlElement(LPCTSTR lpszId)
{
	IHTMLDocument2Ptr pDoc(GetHtmlDocument());
	if(pDoc == NULL)
	{
		return NULL;
	}

	IHTMLElementCollectionPtr ec;
	IHTMLElementPtr e;
	pDoc->get_all(&ec);
	COleVariant varid(lpszId);
	COleVariant varIndex((long)0, VT_I4);
	ec->item(varid, varIndex, (IDispatch**)(&e));
	//IHTMLElementPtr e(pDoc->Getall()->item(lpszId));
	return e;
}

void CHtmlViewOWMView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if (bActivate)
	{
		// 在属性窗口显示图像信息
		GetDocument()->RefreshDocProperty();
	}

	CXTHtmlView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/////////////////////////////////////////////////////////////////////////////
// 后退
/////////////////////////////////////////////////////////////////////////////
void CHtmlViewOWMView::OnGoBack() 
{
	GoBack();
}

void CHtmlViewOWMView::OnUpdateGoBack(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 前进
/////////////////////////////////////////////////////////////////////////////
void CHtmlViewOWMView::OnGoForward() 
{
	GoForward();
}

void CHtmlViewOWMView::OnUpdateGoForward(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 停止
/////////////////////////////////////////////////////////////////////////////
void CHtmlViewOWMView::OnViewStop() 
{
	Stop();
}

void CHtmlViewOWMView::OnUpdateViewStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 刷新
/////////////////////////////////////////////////////////////////////////////
void CHtmlViewOWMView::OnViewRefresh() 
{
	Refresh();
}

void CHtmlViewOWMView::OnUpdateViewRefresh(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 起始页
/////////////////////////////////////////////////////////////////////////////
void CHtmlViewOWMView::OnGoStartPage() 
{
	//GoStartPage();
	CString strStartScript = theApp.m_strDefaultPage;
	Navigate2(strStartScript);
}

void CHtmlViewOWMView::OnUpdateGoStartPage(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 搜索
/////////////////////////////////////////////////////////////////////////////
void CHtmlViewOWMView::OnGoSearchTheWeb() 
{
	// 创建解释器
	IInterp* pInterp = (IInterp*)(theApp.m_pIPlatUI->CreateVciObject("org.interp.tcl", ""));
	if(pInterp == NULL)
	{
		return;
	}

	CString strScriptFile;
	strScriptFile.Format("\"%sLib\\Plat\\htmlview.tcl\" search", theApp.GetPlatRootPath());
	pInterp->RunScriptFile(strScriptFile);

	theApp.m_pIPlatUI->ReleaseVciObject(pInterp, FALSE);
}

void CHtmlViewOWMView::OnUpdateGoSearchTheWeb(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 文件夹
/////////////////////////////////////////////////////////////////////////////
void CHtmlViewOWMView::OnViewFolders() 
{
	// 创建解释器
	IInterp* pInterp = (IInterp*)(theApp.m_pIPlatUI->CreateVciObject("org.interp.tcl", ""));
	if(pInterp == NULL)
	{
		return;
	}

	CString strScriptFile;
	strScriptFile.Format("\"%sLib\\Plat\\htmlview.tcl\" folder", theApp.GetPlatRootPath());
	pInterp->RunScriptFile(strScriptFile);

	theApp.m_pIPlatUI->ReleaseVciObject(pInterp, FALSE);
}

void CHtmlViewOWMView::OnUpdateViewFolders(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 文件夹
/////////////////////////////////////////////////////////////////////////////
void CHtmlViewOWMView::OnViewHistory() 
{
	// 创建解释器
	IInterp* pInterp = (IInterp*)(theApp.m_pIPlatUI->CreateVciObject("org.interp.tcl", ""));
	if(pInterp == NULL)
	{
		return;
	}

	CString strScriptFile;
	strScriptFile.Format("\"%sLib\\Plat\\htmlview.tcl\" history", theApp.GetPlatRootPath());
	pInterp->RunScriptFile(strScriptFile);

	theApp.m_pIPlatUI->ReleaseVciObject(pInterp, FALSE);
}

void CHtmlViewOWMView::OnUpdateViewHistory(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}
