// InetTools.h: interface for the CInetTools class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INETTOOLS_H__7AED56F9_DE08_412A_AAFB_4E0B00DECB47__INCLUDED_)
#define AFX_INETTOOLS_H__7AED56F9_DE08_412A_AAFB_4E0B00DECB47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "wininet.h"


#pragma comment( lib, "wininet" )

class CInetTools : public CObject  
{
public:
	
	static inline bool MtlIsGlobalOffline()
	{
		DWORD dwState = 0, dwSize = sizeof(DWORD);
		bool bRet = false;
		
		if(::InternetQueryOption(NULL, INTERNET_OPTION_CONNECTED_STATE, &dwState, &dwSize)) {
			if(dwState & INTERNET_STATE_DISCONNECTED_BY_USER)
				bRet = true;
		}
		
		return bRet; 
	}
	static inline void MtlSetGlobalOffline(bool bGoOffline)
	{
		if (MtlIsGlobalOffline() == bGoOffline)
			return;
		
		INTERNET_CONNECTED_INFO ci;
		memset(&ci, 0, sizeof(ci));
		if(bGoOffline) {
			ci.dwConnectedState = INTERNET_STATE_DISCONNECTED_BY_USER;
			ci.dwFlags = ISO_FORCE_DISCONNECTED;
		} 
		else {
			ci.dwConnectedState = INTERNET_STATE_CONNECTED;
		}
		
		::InternetSetOption(NULL, INTERNET_OPTION_CONNECTED_STATE, &ci, sizeof(ci));
	}
	
	
	static BOOL GetFavoriteFolder(CString& strFav)
	{
		
		LPITEMIDLIST pidlFavorites = NULL;
		IMalloc * pMalloc = NULL;
		BOOL bResult = FALSE;
		
		HRESULT hr = SHGetMalloc(&pMalloc);
		if (SUCCEEDED(hr))
		{
			TCHAR	szPath[MAX_PATH];
			HRESULT hr = SHGetSpecialFolderLocation(NULL, CSIDL_FAVORITES, &pidlFavorites);
			if (hr == NOERROR )
			{
				if (SHGetPathFromIDList( pidlFavorites,szPath))
				{
					strFav = szPath;	
					bResult = TRUE;
				}
				
			}
			
			pMalloc->Free(pidlFavorites);
			pMalloc->Release();
			
		}
		
		TCHAR szPath[MAX_PATH];
		if (!bResult)
		{
			HKEY hKey;
			TCHAR sz[MAX_PATH];
			
			// find out from the registry where the favorites are located.
			if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\User Shell Folders"), &hKey) == ERROR_SUCCESS)
			{
				DWORD dwSize = MAX_PATH;
				RegQueryValueEx(hKey, _T("Favorites"), NULL, NULL, (LPBYTE)sz, &dwSize);
				ExpandEnvironmentStrings(sz, szPath, MAX_PATH);
				RegCloseKey(hKey);
				strFav = szPath;			
				bResult = dwSize != 0;		
			}
			
		}
		if (!bResult)
		{
			GetWindowsDirectory (szPath, MAX_PATH); 
			strFav = szPath;
			strFav += "\\Favorites";
		}
		
		return TRUE;
		
	}
	
	
	static inline void MtlOrganizeFavorite(HWND hWnd)
	{
		HINSTANCE hLib = ::LoadLibrary(_T("shdocvw.dll"));
		if (hLib) 
		{
			typedef UINT (CALLBACK* LPFNORGFAV)(HWND, LPTSTR);
#ifdef _UNICODE
			LPFNORGFAV lpfnDoOrganizeFavDlg = (LPFNORGFAV)::GetProcAddress(hLib, "DoOrganizeFavDlgW");
#else
			LPFNORGFAV lpfnDoOrganizeFavDlg = (LPFNORGFAV)::GetProcAddress(hLib, "DoOrganizeFavDlg");
#endif
			if (lpfnDoOrganizeFavDlg) 
			{
				CString strFav;
				if (GetFavoriteFolder(strFav))
				{
					lpfnDoOrganizeFavDlg(hWnd, (LPTSTR)(LPCTSTR)strFav);
				}
			}
			::FreeLibrary(hLib);
		}
	}
	
	
	static void SetStyleSheet(BOOL bSet, BOOL bUpdate);
	
};

extern CInetTools InetTools;

#endif // !defined(AFX_INETTOOLS_H__7AED56F9_DE08_412A_AAFB_4E0B00DECB47__INCLUDED_)
