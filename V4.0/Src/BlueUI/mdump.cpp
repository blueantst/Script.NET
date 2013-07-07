//this file is part of eMule
//Copyright (C)2002 Merkur ( devs@emule-project.net / http://www.emule-project.net )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#include "stdafx.h"
typedef unsigned long ULONG_PTR, *PULONG_PTR;
#include <dbghelp.h>
#include "mdump.h"

#define ARRSIZE(x)	(sizeof(x)/sizeof(x[0]))

typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
										 CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
										 CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
										 CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

CMiniDumper theCrashDumper;
TCHAR CMiniDumper::m_szAppName[MAX_PATH] = {0};

void CMiniDumper::Enable(LPCTSTR pszAppName, bool bShowErrors)
{
	// if this assert fires then you have two instances of CMiniDumper which is not allowed
	ASSERT( m_szAppName[0] == _T('\0') );
	_tcsncpy(m_szAppName, pszAppName, ARRSIZE(m_szAppName));

	MINIDUMPWRITEDUMP pfnMiniDumpWriteDump = NULL;
	HMODULE hDbgHelpDll = GetDebugHelperDll((FARPROC*)&pfnMiniDumpWriteDump, bShowErrors);
	if (hDbgHelpDll)
	{
		if (pfnMiniDumpWriteDump)
			SetUnhandledExceptionFilter(TopLevelFilter);
		FreeLibrary(hDbgHelpDll);
		hDbgHelpDll = NULL;
		pfnMiniDumpWriteDump = NULL;
	}
}

HMODULE CMiniDumper::GetDebugHelperDll(FARPROC* ppfnMiniDumpWriteDump, bool bShowErrors)
{
	*ppfnMiniDumpWriteDump = NULL;
	HMODULE hDll = LoadLibrary(_T("DBGHELP.DLL"));
	if (hDll == NULL)
	{
		if (bShowErrors) {
			// Do *NOT* localize that string (in fact, do not use MFC to load it)!
			MessageBox(NULL, _T("DBGHELP.DLL not found. Please install a DBGHELP.DLL."), m_szAppName, MB_ICONSTOP | MB_OK);
		}
	}
	else
	{
		*ppfnMiniDumpWriteDump = GetProcAddress(hDll, "MiniDumpWriteDump");
		if (*ppfnMiniDumpWriteDump == NULL)
		{
			if (bShowErrors) {
				// Do *NOT* localize that string (in fact, do not use MFC to load it)!
				MessageBox(NULL, _T("DBGHELP.DLL found is too old. Please upgrade to a newer version of DBGHELP.DLL."), m_szAppName, MB_ICONSTOP | MB_OK);
			}
		}
	}
	return hDll;
}

/////////////////////////////////////////////////////////////////////////////
// 执行外部进程
/////////////////////////////////////////////////////////////////////////////
BOOL ExecProcess(LPCTSTR strExePath, LPCTSTR strParam, BOOL bWaitEnd)
{
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
 
	// Set up members of the PROCESS_INFORMATION structure. 
	ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );

	// Set up members of the STARTUPINFO structure. 
	ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
	siStartInfo.cb = sizeof(STARTUPINFO); 
	siStartInfo.wShowWindow=SW_SHOWNORMAL ;
	siStartInfo.dwFlags=STARTF_USESHOWWINDOW;

	// 命令组装
	TCHAR szCmd[_MAX_PATH] = {0};
	_tcsncat(szCmd, strExePath, ARRSIZE(szCmd));
	
	// 创建进程
	BOOL bRet=CreateProcess(NULL,
      szCmd,		  // command line 
      NULL,          // process security attributes 
      NULL,          // primary thread security attributes 
      TRUE,          // handles are inherited 
      0,             // creation flags 
      NULL,          // use parent's environment 
      NULL,          // use parent's current directory 
      &siStartInfo,  // STARTUPINFO pointer 
      &piProcInfo);  // receives PROCESS_INFORMATION
	
	if(bWaitEnd)
	{
		// 等待
		//WaitForSingleObject
	}
	return bRet;
}

LONG CMiniDumper::TopLevelFilter(struct _EXCEPTION_POINTERS* pExceptionInfo)
{
	LONG lRetValue = EXCEPTION_CONTINUE_SEARCH;
	TCHAR szResult[_MAX_PATH + 1024] = {0};
	MINIDUMPWRITEDUMP pfnMiniDumpWriteDump = NULL;
	HMODULE hDll = GetDebugHelperDll((FARPROC*)&pfnMiniDumpWriteDump, true);
	if (hDll)
	{
		if (pfnMiniDumpWriteDump)
		{
			// Ask user if they want to save a dump file
			// Do *NOT* localize that string (in fact, do not use MFC to load it)!
			//if (MessageBox(NULL, _T("application crashed :-(\r\n\r\nA diagnostic file can be created which will help the author to resolve this problem. This file will be saved on your Disk (and not sent).\r\n\r\nDo you want to create this file now?"), m_szAppName, MB_ICONSTOP | MB_YESNO) == IDYES)
			{
				// Create full path for DUMP file
				TCHAR szDumpPath[_MAX_PATH] = {0};
				TCHAR szCrashReporterPath[1024] = {0};
				GetModuleFileName(NULL, szDumpPath, ARRSIZE(szDumpPath));
				LPTSTR pszFileName = _tcsrchr(szDumpPath, _T('\\'));
				if (pszFileName) {
					pszFileName++;
					*pszFileName = _T('\0');
				}
				_tcsncat(szCrashReporterPath, "\"", ARRSIZE(szCrashReporterPath));
				_tcsncat(szCrashReporterPath, szDumpPath, ARRSIZE(szCrashReporterPath));
				#ifdef _DEBUG
				_tcsncat(szCrashReporterPath, _T("CrashReporterd.exe"), ARRSIZE(szCrashReporterPath));
				#else
				_tcsncat(szCrashReporterPath, _T("CrashReporter.exe"), ARRSIZE(szCrashReporterPath));
				#endif
				_tcsncat(szCrashReporterPath, "\" ", ARRSIZE(szCrashReporterPath));

				_tcsncat(szDumpPath, m_szAppName, ARRSIZE(szDumpPath));
				_tcsncat(szDumpPath, _T(".dmp"), ARRSIZE(szDumpPath));

				_tcsncat(szCrashReporterPath, szDumpPath, ARRSIZE(szCrashReporterPath));
				_tcsncat(szCrashReporterPath, "\"", ARRSIZE(szCrashReporterPath));

				HANDLE hFile = CreateFile(szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				if (hFile != INVALID_HANDLE_VALUE)
				{
					_MINIDUMP_EXCEPTION_INFORMATION ExInfo = {0};
					ExInfo.ThreadId = GetCurrentThreadId();
					ExInfo.ExceptionPointers = pExceptionInfo;
					ExInfo.ClientPointers = NULL;

					MINIDUMP_TYPE MiniDumpType = MiniDumpNormal;
					BOOL bOK = (*pfnMiniDumpWriteDump)(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpType, &ExInfo, NULL, NULL);
					if (bOK)
					{
						// Do *NOT* localize that string (in fact, do not use MFC to load it)!
						//_sntprintf(szResult, ARRSIZE(szResult), _T("Saved dump file to \"%s\".\r\n\r\nPlease send this file together with a detailed bug report to script.net@gmail.com !\r\n\r\nThank you for helping to improve Script.NET."), szDumpPath);
						ExecProcess(szCrashReporterPath, "", TRUE);
						lRetValue = EXCEPTION_EXECUTE_HANDLER;
					}
					else
					{
						// Do *NOT* localize that string (in fact, do not use MFC to load it)!
						_sntprintf(szResult, ARRSIZE(szResult), _T("Failed to save dump file to \"%s\".\r\n\r\nError code: 0x%X"), szDumpPath, GetLastError());
					}
					CloseHandle(hFile);
				}
				else
				{
					// Do *NOT* localize that string (in fact, do not use MFC to load it)!
					_sntprintf(szResult, ARRSIZE(szResult), _T("Failed to create dump file \"%s\".\r\n\r\nError code: 0x%X"), szDumpPath, GetLastError());
				}
			}
		}
		FreeLibrary(hDll);
		hDll = NULL;
		pfnMiniDumpWriteDump = NULL;
	}

	if (szResult[0] != _T('\0'))
		MessageBox(NULL, szResult, m_szAppName, MB_ICONINFORMATION | MB_OK);

#ifndef _DEBUG
	// Exit the process only in release builds, so that in debug builds the exceptio is passed to a possible
	// installed debugger
	ExitProcess(0);
#else
	return lRetValue;
#endif
}
