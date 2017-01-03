// InetTools.cpp: implementation of the CInetTools class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlueUI.h"
#include "InetTools.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInetTools InetTools;

void CInetTools::SetStyleSheet(BOOL bSet, BOOL bUpdate)
{
	CString strEntry;
	HKEY hRegRoot, hReg;

	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		_T("Software\\Microsoft\\Internet Explorer"), 0, KEY_WRITE|KEY_CREATE_SUB_KEY, &hRegRoot) != ERROR_SUCCESS)
		return;

	if (RegCreateKeyEx(hRegRoot, _T("Styles"), 0, 0, 0, KEY_WRITE, NULL, &hReg, NULL) != ERROR_SUCCESS)
	{
		RegCloseKey(hRegRoot);
		return;
	}

	
	RegSetValueEx(hReg, _T("Use My Stylesheet"), NULL, REG_DWORD , (LPBYTE)&bSet, sizeof(BOOL));
	
	if (bSet)
	{
		//CString strPath = GetModuleDir() + _T("tables.css");
		CString strPath = theApp.GetPlatRootPath();
		strPath += "\\bin\\tables.css";
		RegSetValueEx(hReg, _T("User Stylesheet"), NULL, REG_SZ, (LPBYTE)(LPCTSTR)strPath, (strPath.GetLength() + 1) * sizeof(TCHAR));
	}
	
	RegCloseKey(hReg);
	RegCloseKey(hRegRoot);

	if (bUpdate)
	{
		const TCHAR pKey[] = _T("Software\\Microsoft\\Internet Explorer");
		DWORD_PTR dwResult;
		SendMessageTimeout(HWND_BROADCAST, WM_WININICHANGE, 0x1F, (LPARAM)pKey, 0, 0x64, &dwResult);
		
	}
	
}
	


