#include "StdAfx.h"
#include "HtmlViewOWM.h"
#include "explorerbar.h"
 
#include "StylerDoc.h"
#include "StylerView.h"



CExplorerBar::CExplorerBar(void)
{
	m_nId = 0;
	m_pIECtrl = 0;
	m_pBrowserApp = NULL;
}

CExplorerBar::~CExplorerBar(void)
{
	delete m_pIECtrl;
}


int CExplorerBar::OnViewHtml(CString strURL)
{
	if (!m_pIECtrl)
	{
		try
		{	
			m_pIECtrl = new CWebBrowserCtrl;
			if (!m_pIECtrl->Create(NULL, WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE,	CRect(0,0,0,0), this, AFX_IDW_PANE_FIRST))
				return -1;
			
			AfxEnableControlContainer();
			
			m_pIECtrl->ModifyStyle(0, WS_BORDER);
			
			LPDISPATCH lpDisp = m_pIECtrl->get_Application();
			m_pBrowserApp = (IWebBrowser2*)lpDisp;
			if (lpDisp)
			{
				CComPtr<IOleObject> spOleObj;
				
				lpDisp->QueryInterface(IID_IOleObject, (void**)&spOleObj);
				if (spOleObj)
					spOleObj->SetClientSite(&m_xOleClientSite );
				lpDisp->Release();		
			}		
			
			m_pIECtrl->Navigate2(strURL);
		}
		catch (...) 
		{
		}
	}

	return 0;
}


BEGIN_EVENTSINK_MAP(CExplorerBar, CWnd)
	ON_EVENT(CExplorerBar, AFX_IDW_PANE_FIRST, 250, BeforeNavigate2, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	ON_EVENT(CExplorerBar, AFX_IDW_PANE_FIRST, 251 /* NewWindow2 */, OnNewWindow2, VTS_PDISPATCH VTS_PBOOL)
END_EVENTSINK_MAP()

void CExplorerBar::BeforeNavigate2(LPDISPATCH /* pDisp */, VARIANT* URL,
        VARIANT* Flags, VARIANT* TargetFrameName,
        VARIANT* PostData, VARIANT* Headers, BOOL* pfCancel)
{
    ASSERT(V_VT(URL) == VT_BSTR);
    ASSERT(V_VT(TargetFrameName) == VT_BSTR);
    ASSERT(V_VT(PostData) == (VT_VARIANT | VT_BYREF));
    ASSERT(V_VT(Headers) == VT_BSTR);
    ASSERT(pfCancel != NULL);

    // USES_CONVERSION;

    VARIANT* vtPostedData = V_VARIANTREF(PostData);
    CByteArray array;
    if (V_VT(vtPostedData) & VT_ARRAY) {
        // must be a vector of bytes
        ASSERT(vtPostedData->parray->cDims == 1 && vtPostedData->parray->cbElements == 1);

        vtPostedData->vt |= VT_UI1;
        COleSafeArray safe(vtPostedData);

        DWORD dwSize = safe.GetOneDimSize();
        LPVOID pVoid;
        safe.AccessData(&pVoid);

        array.SetSize(dwSize);
        LPBYTE lpByte = array.GetData();

        memcpy(lpByte, pVoid, dwSize);
        safe.UnaccessData();
    }
    // make real parameters out of the notification

    CString strTargetFrameName(V_BSTR(TargetFrameName));
    CString strURL = V_BSTR(URL);
    CString strHeaders = V_BSTR(Headers);
    DWORD nFlags = V_I4(Flags);

    // notify the user's class
    OnBeforeNavigate2(strURL, nFlags, strTargetFrameName, array, strHeaders, pfCancel);
}

/////////////////////////////////////////////////////////////////////////////
// 保存并设置当前路径
/////////////////////////////////////////////////////////////////////////////
void CExplorerBar::SaveAndSetCurPath(CString strURL)
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
void CExplorerBar::RestoreCurPath()
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
void ReplaceHtmlStrToAscii1(CString& strFormParam)
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

void CExplorerBar::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags,
		LPCTSTR lpszTargetFrameName, CByteArray& baPostedData,
		LPCTSTR lpszHeaders, BOOL* pbCancel)
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

		m_pIECtrl->Navigate2(strURL);

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
		for(int i=0; i<asInterpName.GetSize(); i++)
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

			IInterp* g_pInterp = GetCurrentInterp();
			if((g_pInterp != pInterp) && (g_pInterp != NULL))
			{
				// 如果解释器已经改变,需要先释放以前的解释器
				// 但要先查一下以前的解释器是否驻留的解释器,非驻留的才释放
				CString strTSRInterp = g_pInterp->GetInterpParam("TSRInterp");
				if(strTSRInterp != "true")
				{
					theApp.m_pIPlatUI->ReleaseVciObject(g_pInterp, TRUE);
				}
			}
			SetCurrentInterp(pInterp);

			// 在执行脚本之前先设置表示当前页面文件的TCL变量
			//CStylerDoc* pDoc = GetDocument();
			//ASSERT_VALID(pDoc);
			//CString strHtmlPath = pDoc->GetCurrentUrl();
			//strHtmlPath.Replace('\\', '/');
			//pInterp->SetVar("_htmlCurrentFile", strHtmlPath);

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
						ReplaceHtmlStrToAscii1(strFormParam);
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
				bRet = pInterp->RunScriptCommand(strURL);	// 执行脚本命令
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
					try {
					m_pIECtrl->Navigate2(strHtmlNewURL);
					}catch(...){
					}
				}else
				{
					// 检查HTML刷新标记变量
					CString strHtmlRefreshSign = pInterp->GetVar(_T("_htmlRefreshSign"));
					if(strHtmlRefreshSign == "true")
					{	// 表示有页面变化,需要刷新
						try {
						//Refresh();
						}catch(...){
						}
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
}

void CExplorerBar::OnNewWindow2(LPDISPATCH* ppDisp, BOOL* /*bCancel*/)
{
//	if(theApp.m_pIPlatUI)
//	{
//		theApp.m_pIPlatUI->OpenDocumentFile("", "org.owm.htmlview");
//	}

	// Get the correct document template.
	CDocTemplate* pDocTemplate = theApp.m_pDocTemplate;
	
	CStylerDoc* pDoc = (CStylerDoc*)pDocTemplate->CreateNewDocument();
	pDoc->SetUrl("");
	
	// Create a new frame.
	CFrameWnd* pFrame = pDocTemplate->CreateNewFrame(pDoc, (CFrameWnd*)AfxGetMainWnd() );
	
	// Activate the frame.
	
	//if (m_bLock) ((CChildFrame*)pFrame)->m_bActivate = FALSE;	
	pDocTemplate->InitialUpdateFrame( pFrame, NULL );
	CStylerView* pView = (CStylerView*)pFrame->GetActiveView();
	
	// Pass pointer of WebBrowser object.
	pView->SetRegisterAsBrowser( TRUE );   
	*ppDisp = pView->GetApplication();   
}

BOOL CExplorerBar::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg == NULL || m_hWnd == NULL)
		return TRUE;

	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
//		m_infMouse.nLClick = GetTickCount();
//		m_infMouse.nRClick = 0;
	}

	if (CWnd::PreTranslateMessage(pMsg))
		return TRUE;

	// don't translate dialog messages when in Shift+F1 help mode
/*	CFrameWnd* pFrameWnd = GetTopLevelFrame();
	if (pFrameWnd != NULL && pFrameWnd->m_bHelpMode)
		return FALSE;

	if (pFrameWnd->PreTranslateMessage(pMsg))
			return TRUE;
*/
	if (pMsg->message == WM_KEYDOWN && m_pBrowserApp)
	{		
		CComQIPtr<IOleInPlaceActiveObject> spActiveObj(m_pBrowserApp);
		if (spActiveObj && spActiveObj->TranslateAccelerator(pMsg) == S_OK)
			return TRUE;
	}

	return FALSE;
}



BEGIN_INTERFACE_MAP(CExplorerBar, CCmdTarget)
	INTERFACE_PART(CExplorerBar, IID_IDocHostUIHandler, DocHostUIHandler)
	INTERFACE_PART(CExplorerBar, IID_IOleClientSite, OleClientSite)
END_INTERFACE_MAP()


// **************************************************************************
// * IDocHostUIHandler methods
// **************************************************************************
ULONG FAR EXPORT  CExplorerBar::XDocHostUIHandler::AddRef()
{
	METHOD_PROLOGUE(CExplorerBar, DocHostUIHandler)
	return pThis->ExternalAddRef();
}

ULONG FAR EXPORT  CExplorerBar::XDocHostUIHandler::Release()
{                            
    METHOD_PROLOGUE(CExplorerBar, DocHostUIHandler)
    return pThis->ExternalRelease();
}

HRESULT FAR EXPORT  CExplorerBar::XDocHostUIHandler::QueryInterface
(REFIID riid, void **ppvObj)
{
    METHOD_PROLOGUE(CExplorerBar, DocHostUIHandler)
    HRESULT hr = (HRESULT)pThis->ExternalQueryInterface(&riid, ppvObj);
    return hr;
}

// CImpIDocHostUIHandler::GetHostInfo
HRESULT FAR EXPORT  CExplorerBar::XDocHostUIHandler::GetHostInfo
( DOCHOSTUIINFO* pInfo )
{
    METHOD_PROLOGUE(CExplorerBar, DocHostUIHandler)
    //pInfo->dwFlags = DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_SCROLL_NO ;
	// 显示为新式用户界面
	pInfo->dwFlags = DOCHOSTUIFLAG_NO3DBORDER|DOCHOSTUIFLAG_FLAT_SCROLLBAR|0x40000;
    pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
    return S_OK;
}

// CImpIDocHostUIHandler::ShowUI
HRESULT FAR EXPORT  CExplorerBar::XDocHostUIHandler::ShowUI(
				DWORD /*dwID*/, 
				IOleInPlaceActiveObject * /*pActiveObject*/,
				IOleCommandTarget * /*pCommandTarget*/,
				IOleInPlaceFrame * /*pFrame*/,
				IOleInPlaceUIWindow * /*pDoc*/)
{
    METHOD_PROLOGUE(CExplorerBar, DocHostUIHandler)
    return S_OK;
}

// CImpIDocHostUIHandler::HideUI
HRESULT FAR EXPORT  CExplorerBar::XDocHostUIHandler::HideUI(void)
{
    METHOD_PROLOGUE(CExplorerBar, DocHostUIHandler)
    return S_OK;
}

// CImpIDocHostUIHandler::UpdateUI
HRESULT FAR EXPORT  CExplorerBar::XDocHostUIHandler::UpdateUI(void)
{
    METHOD_PROLOGUE(CExplorerBar, DocHostUIHandler)
    return S_OK;
}

// CImpIDocHostUIHandler::EnableModeless
HRESULT FAR EXPORT  CExplorerBar::XDocHostUIHandler::EnableModeless
(BOOL /*fEnable*/)
{
    METHOD_PROLOGUE(CExplorerBar, DocHostUIHandler)
    return E_NOTIMPL;
}

// CImpIDocHostUIHandler::OnDocWindowActivate
HRESULT FAR EXPORT  CExplorerBar::XDocHostUIHandler::OnDocWindowActivate
(BOOL /*fActivate*/)
{
    METHOD_PROLOGUE(CExplorerBar, DocHostUIHandler)
    return E_NOTIMPL;
}

// CImpIDocHostUIHandler::OnFrameWindowActivate
HRESULT FAR EXPORT  CExplorerBar::XDocHostUIHandler::OnFrameWindowActivate
(BOOL /*fActivate*/)
{
    METHOD_PROLOGUE(CExplorerBar, DocHostUIHandler)
    return E_NOTIMPL;
}

// CImpIDocHostUIHandler::ResizeBorder
HRESULT FAR EXPORT  CExplorerBar::XDocHostUIHandler::ResizeBorder(
				LPCRECT /*prcBorder*/, 
				IOleInPlaceUIWindow* /*pUIWindow*/,
				BOOL /*fRameWindow*/)
{
    METHOD_PROLOGUE(CExplorerBar, DocHostUIHandler)
    return E_NOTIMPL;
}

// CImpIDocHostUIHandler::ShowContextMenu
HRESULT FAR EXPORT  CExplorerBar::XDocHostUIHandler::ShowContextMenu(
				DWORD /*dwID*/, 
				POINT* /*pptPosition*/,
				IUnknown* /*pCommandTarget*/,
				IDispatch* /*pDispatchObjectHit*/)
{
    METHOD_PROLOGUE(CExplorerBar, DocHostUIHandler)
    //return S_FALSE;
	return E_NOTIMPL; // We've shown our own context menu. MSHTML.DLL will no longer try to show its own.
}

// CImpIDocHostUIHandler::TranslateAccelerator
HRESULT FAR EXPORT  CExplorerBar::XDocHostUIHandler::TranslateAccelerator(LPMSG /*lpMsg*/,
            /* [in] */ const GUID __RPC_FAR* /*pguidCmdGroup*/,
            /* [in] */ DWORD /*nCmdID*/)
{
    METHOD_PROLOGUE(CExplorerBar, DocHostUIHandler)
    return S_FALSE;
}

// CImpIDocHostUIHandler::GetOptionKeyPath
HRESULT FAR EXPORT  CExplorerBar::XDocHostUIHandler::GetOptionKeyPath
(BSTR* /*pbstrKey*/, DWORD)
{	
	METHOD_PROLOGUE(CExplorerBar, DocHostUIHandler)
	return E_NOTIMPL;
}

STDMETHODIMP CExplorerBar::XDocHostUIHandler::GetDropTarget( 
            /* [in] */ IDropTarget __RPC_FAR* /*pDropTarget*/,
            /* [out] */ IDropTarget __RPC_FAR *__RPC_FAR* /*ppDropTarget*/)
{	
	METHOD_PROLOGUE(CExplorerBar, DocHostUIHandler)
	return E_NOTIMPL;
}

STDMETHODIMP CExplorerBar::XDocHostUIHandler::GetExternal( 
            /* [out] */ IDispatch __RPC_FAR *__RPC_FAR* /*ppDispatch*/)
{
	return E_NOTIMPL;
}
        
STDMETHODIMP CExplorerBar::XDocHostUIHandler::TranslateUrl( 
            /* [in] */ DWORD /*dwTranslate*/,
            /* [in] */ OLECHAR __RPC_FAR* /*pchURLIn*/,
            /* [out] */ OLECHAR __RPC_FAR *__RPC_FAR* /*ppchURLOut*/)
{
	METHOD_PROLOGUE(CExplorerBar, DocHostUIHandler)
	return E_NOTIMPL;
}
        
STDMETHODIMP CExplorerBar::XDocHostUIHandler::FilterDataObject( 
            /* [in] */ IDataObject __RPC_FAR* /*pDO*/,
            /* [out] */ IDataObject __RPC_FAR *__RPC_FAR* /*ppDORet*/)
{
	METHOD_PROLOGUE(CExplorerBar, DocHostUIHandler)
	return E_NOTIMPL;
}
STDMETHODIMP CExplorerBar::XDocHostUIHandler::GetOverrideKeyPath(
    LPOLESTR* /*pchKey*/, DWORD /*dw*/)
{
	/*
	 * This is not called due to the bug.
	 */	
	METHOD_PROLOGUE(CExplorerBar, DocHostUIHandler)
	return E_NOTIMPL;
}

ULONG FAR EXPORT  CExplorerBar::XOleClientSite::AddRef()
{
	METHOD_PROLOGUE(CExplorerBar, OleClientSite)
	return pThis->ExternalAddRef();
}

ULONG FAR EXPORT  CExplorerBar::XOleClientSite::Release()
{                            
    METHOD_PROLOGUE(CExplorerBar, OleClientSite)
    return pThis->ExternalRelease();
}

HRESULT FAR EXPORT  CExplorerBar::XOleClientSite::QueryInterface
(REFIID riid, void **ppvObj)
{
    METHOD_PROLOGUE(CExplorerBar, OleClientSite)
    HRESULT hr = (HRESULT)pThis->ExternalQueryInterface(&riid, ppvObj);
    return hr;
}

BEGIN_MESSAGE_MAP(CExplorerBar, CWnd)
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CExplorerBar::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (m_pIECtrl)
	{
		m_pIECtrl->MoveWindow(0, 0, cx, cy);
	}
}
