// CWebBrowser2.h  : Declaration of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CWebBrowser2
#include <MsHtmHst.h>

class CWebBrowserCtrl : public CWnd
{
protected:
	DECLARE_DYNCREATE(CWebBrowserCtrl)
public:

	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x8856F961, 0x340A, 0x11D0, { 0xA9, 0x6B, 0x0, 0xC0, 0x4F, 0xD7, 0x5, 0xA2 } };
		return clsid;
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		if (!CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey))
			return FALSE;

		EnableAutomation();

		return TRUE;

	}

// Attributes
public:

// Operations
public:


	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0xc8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	
	void Navigate2(VARIANT * URL, VARIANT * Flags, VARIANT * TargetFrameName, VARIANT * PostData, VARIANT * Headers)
	{
		static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT ;
		InvokeHelper(0x1f4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, URL, Flags, TargetFrameName, PostData, Headers);
	}

	
	void Navigate2(LPCTSTR lpszURL, DWORD dwFlags  = 0 ,
	LPCTSTR lpszTargetFrameName = NULL,
	LPCTSTR lpszHeaders = NULL ,
	LPVOID lpvPostData = NULL , DWORD dwPostDataLen = 0)
	{

		COleSafeArray vPostData;
		if (lpvPostData != NULL)
		{
			if (dwPostDataLen == 0)
				dwPostDataLen = lstrlen((LPCTSTR) lpvPostData);

			vPostData.CreateOneDim(VT_UI1, dwPostDataLen, lpvPostData);
		}

		COleVariant vURL(lpszURL, VT_BSTR);
		COleVariant vHeaders(lpszHeaders, VT_BSTR);
		COleVariant vTargetFrameName(lpszTargetFrameName, VT_BSTR);
		COleVariant vFlags((long) dwFlags, VT_I4);

		Navigate2(vURL,
		vFlags, vTargetFrameName, vPostData, vHeaders);
	}
};
