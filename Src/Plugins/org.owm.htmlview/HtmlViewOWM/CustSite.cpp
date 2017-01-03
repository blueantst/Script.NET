
//=--------------------------------------------------------------------------=
//  (C) Copyright 1996-1999 Microsoft Corporation. All Rights Reserved.
//=--------------------------------------------------------------------------=


// 
// NOTE: 
// Some of the code in this file is MFC implementation specific.
// Changes in future versions of MFC implementation may require
// the code to be changed. Please check the readme of this
// sample for more information 
// 

#include "stdafx.h"
#include "StylerView.h"
#include "HtmlViewOWM.h"
#include "ExplorerBar.h"


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


//DEFINE_GUID(CGID_DocHostCommandHandler,0xf38bc242,0xb950,0x11d1,0x89,0x18,0x00,0xc0,0x4f,0xc2,0xc8,0x36);

//static CLSID const CGID_DocHostCommandHandler
//	= { 0xf38bc242, 0xb950, 0x11d1, { 0x89,0x18,0x00,0xc0,0x4f,0xc2,0xc8,0x36 } };

BEGIN_INTERFACE_MAP(CStylerView, CCmdTarget)
	INTERFACE_PART(CStylerView, IID_IDocHostUIHandler, DocHostUIHandler)
	INTERFACE_PART(CStylerView, IID_IOleClientSite, OleClientSite)
	INTERFACE_PART(CStylerView, IID_IDispatch, Dispatch)
END_INTERFACE_MAP()


#define DOCHOSTUIFLAG_ENABLE_FORMS_AUTOCOMPLETE 16384


#define IMPLEMENT_INTERFACE_PART(theClass, localClass)\
STDMETHODIMP_(ULONG) theClass::X##localClass::AddRef()\
{\
	METHOD_PROLOGUE(theClass, localClass)\
	return pThis->ExternalAddRef();\
}\
STDMETHODIMP_(ULONG) theClass::X##localClass::Release()\
{\
    METHOD_PROLOGUE(theClass, localClass)\
	return pThis->ExternalRelease();\
}\
STDMETHODIMP theClass::X##localClass::QueryInterface(REFIID riid, void **ppvObj)\
{\
	METHOD_PROLOGUE(theClass, localClass)\
    HRESULT hr = (HRESULT)pThis->ExternalQueryInterface(&riid, ppvObj);\
	return hr;\
}


IMPLEMENT_INTERFACE_PART(CStylerView, DocHostUIHandler)
IMPLEMENT_INTERFACE_PART(CStylerView, OleClientSite)
IMPLEMENT_INTERFACE_PART(CStylerView, Dispatch)

STDMETHODIMP  CStylerView::XDocHostUIHandler::GetHostInfo( DOCHOSTUIINFO* pInfo )
{
	// 显示为新式用户界面
	pInfo->dwFlags = DOCHOSTUIFLAG_ENABLE_FORMS_AUTOCOMPLETE |DOCHOSTUIFLAG_FLAT_SCROLLBAR|0x40000;
    pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
    return S_OK;
}

STDMETHODIMP  CStylerView::XDocHostUIHandler::ShowUI(
				DWORD /*dwID*/, 
				IOleInPlaceActiveObject * /*pActiveObject*/,
				IOleCommandTarget * /*pCommandTarget*/,
				IOleInPlaceFrame * /*pFrame*/,
				IOleInPlaceUIWindow * /*pDoc*/)
{
    return S_OK;
}

STDMETHODIMP  CStylerView::XDocHostUIHandler::HideUI(void)
{
    return S_OK;
}

STDMETHODIMP  CStylerView::XDocHostUIHandler::UpdateUI(void)
{
	return S_OK;
}

STDMETHODIMP  CStylerView::XDocHostUIHandler::EnableModeless(BOOL /*fEnable*/)
{
    return E_NOTIMPL;
}

STDMETHODIMP  CStylerView::XDocHostUIHandler::OnDocWindowActivate(BOOL /*fActivate*/)
{
    return E_NOTIMPL;
}

STDMETHODIMP  CStylerView::XDocHostUIHandler::OnFrameWindowActivate(BOOL /*fActivate*/)
{
    return E_NOTIMPL;
}

STDMETHODIMP CStylerView::XDocHostUIHandler::ResizeBorder(
				LPCRECT /*prcBorder*/, 
				IOleInPlaceUIWindow* /*pUIWindow*/,
				BOOL /*fRameWindow*/)
{
    return E_NOTIMPL;
}

STDMETHODIMP  CStylerView::XDocHostUIHandler::ShowContextMenu(
				DWORD /*dwID*/, 
				POINT* /*pptPosition*/,
				IUnknown* /*pCommandTarget*/,
				IDispatch* /*pDispatchObjectHit*/)
{

    return E_NOTIMPL; // We've shown our own context menu. MSHTML.DLL will no longer try to show its own.
}

STDMETHODIMP  CStylerView::XDocHostUIHandler::TranslateAccelerator(LPMSG /*lpMsg*/,
            /* [in] */ const GUID __RPC_FAR* /*pguidCmdGroup*/,
            /* [in] */ DWORD /*nCmdID*/)
{
    return S_FALSE;
}

STDMETHODIMP  CStylerView::XDocHostUIHandler::GetOptionKeyPath(BSTR* /*pbstrKey*/, DWORD)
{
	return E_NOTIMPL;
}

STDMETHODIMP CStylerView::XDocHostUIHandler::GetDropTarget( 
            /* [in] */ IDropTarget __RPC_FAR* /*pDropTarget*/,
            /* [out] */ IDropTarget __RPC_FAR *__RPC_FAR* /*ppDropTarget*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CStylerView::XDocHostUIHandler::GetExternal( 
            /* [out] */ IDispatch __RPC_FAR *__RPC_FAR* ppDispatch)
{
	// 创建扩展命令处理器对象
	CExCommandHandler* pHandler = new CExCommandHandler();
	pHandler->QueryInterface(IID_IDispatch, (void**)ppDispatch);
	return S_OK;
	//return E_NOTIMPL;
}
        
STDMETHODIMP CStylerView::XDocHostUIHandler::TranslateUrl( 
            /* [in] */ DWORD /*dwTranslate*/,
            /* [in] */ OLECHAR __RPC_FAR* /*pchURLIn*/,
            /* [out] */ OLECHAR __RPC_FAR *__RPC_FAR* /*ppchURLOut*/)
{
    return E_NOTIMPL;
}
        
STDMETHODIMP CStylerView::XDocHostUIHandler::FilterDataObject( 
            /* [in] */ IDataObject __RPC_FAR* /*pDO*/,
            /* [out] */ IDataObject __RPC_FAR *__RPC_FAR* /*ppDORet*/)
{
    return E_NOTIMPL;
}



STDMETHODIMP CStylerView::XDispatch::GetTypeInfoCount(unsigned int*)
{
	return E_NOTIMPL;
}

STDMETHODIMP CStylerView::XDispatch::GetTypeInfo(
	unsigned int, LCID, ITypeInfo**)
{
	return E_NOTIMPL;
}

STDMETHODIMP CStylerView::XDispatch::GetIDsOfNames(
	REFIID, LPOLESTR*, unsigned int, LCID, DISPID*)
{
	return E_NOTIMPL;
}

STDMETHODIMP CStylerView::XDispatch::Invoke(
	DISPID dispid, REFIID, LCID, unsigned short /*wFlags*/,
	DISPPARAMS* /*pDispParams*/, VARIANT* pvarResult,
	EXCEPINFO*, unsigned int*)
{
    switch (dispid)
    {
	case DISPID_AMBIENT_DLCONTROL:
		pvarResult->lVal =  DLCTL_VIDEOS | DLCTL_BGSOUNDS;
		if (m_bShowImages) pvarResult->lVal |= DLCTL_DLIMAGES;
		
		pvarResult->vt = VT_I4;
		
		break;
		
	default:
		return DISP_E_MEMBERNOTFOUND;
    }
	
    return S_OK;
}

