// FileUtils.cpp: implementation of the CFileUtils class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// A cool utility function that MS is hiding from you! --- MFC FILE: appinit.cpp
UINT AFXAPI AfxGetFileName(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax)
{
	ASSERT(lpszTitle == NULL ||
		AfxIsValidAddress(lpszTitle, _MAX_FNAME));
	ASSERT(AfxIsValidString(lpszPathName));

	// always capture the complete file name including extension (if present)
	LPTSTR lpszTemp = (LPTSTR)lpszPathName;
	for (LPCTSTR lpsz = lpszPathName; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
	{
		// remember last directory/drive separator
		if (*lpsz == '\\' || *lpsz == '/' || *lpsz == ':')
			lpszTemp = (LPTSTR)_tcsinc(lpsz);
	}

	// lpszTitle can be NULL which just returns the number of bytes
	if (lpszTitle == NULL)
		return lstrlen(lpszTemp)+1;

	// otherwise copy it into the buffer provided
	lstrcpyn(lpszTitle, lpszTemp, nMax);
	return 0;
}

// A cool utility function that MS is hiding from you! --- MFC FILE: filelist.cpp
/////////////////////////////////////////////////////////////////////////////
// lpszCanon = C:\MYAPP\DEBUGS\C\TESWIN.C
//
// cchMax   b   Result
// ------   -   ---------
//  1- 7    F   <empty>
//  1- 7    T   TESWIN.C
//  8-14    x   TESWIN.C
// 15-16    x   C:\...\TESWIN.C
// 17-23    x   C:\...\C\TESWIN.C
// 24-25    x   C:\...\DEBUGS\C\TESWIN.C
// 26+      x   C:\MYAPP\DEBUGS\C\TESWIN.C

/*AFX_STATIC*/ void AFXAPI _AfxAbbreviateName(LPTSTR lpszCanon, int cchMax, BOOL bAtLeastName)
{
	TCHAR ChNil = '\0';
	int cchFullPath, cchFileName, cchVolName;
	const TCHAR* lpszCur;
	const TCHAR* lpszBase;
	const TCHAR* lpszFileName;

	lpszBase = lpszCanon;
	cchFullPath = lstrlen(lpszCanon);

	cchFileName = AfxGetFileName(lpszCanon, NULL, 0) - 1;
	lpszFileName = lpszBase + (cchFullPath-cchFileName);

	// If cchMax is more than enough to hold the full path name, we're done.
	// This is probably a pretty common case, so we'll put it first.
	if (cchMax >= cchFullPath)
		return;

	// If cchMax isn't enough to hold at least the basename, we're done
	if (cchMax < cchFileName)
	{
		lstrcpy(lpszCanon, (bAtLeastName) ? lpszFileName : &ChNil);
		return;
	}

	// Calculate the length of the volume name.  Normally, this is two characters
	// (e.g., "C:", "D:", etc.), but for a UNC name, it could be more (e.g.,
	// "\\server\share").
	//
	// If cchMax isn't enough to hold at least <volume_name>\...\<base_name>, the
	// result is the base filename.

	lpszCur = lpszBase + 2;                 // Skip "C:" or leading "\\"

	if (lpszBase[0] == '\\' && lpszBase[1] == '\\') // UNC pathname
	{
		// First skip to the '\' between the server name and the share name,
		while (*lpszCur != '\\')
		{
			lpszCur = _tcsinc(lpszCur);
			ASSERT(*lpszCur != '\0');
		}
	}
	// if a UNC get the share name, if a drive get at least one directory
	ASSERT(*lpszCur == '\\');
	// make sure there is another directory, not just c:\filename.ext
	if (cchFullPath - cchFileName > 3)
	{
		lpszCur = _tcsinc(lpszCur);
		while (*lpszCur != '\\')
		{
			lpszCur = _tcsinc(lpszCur);
			ASSERT(*lpszCur != '\0');
		}
	}
	ASSERT(*lpszCur == '\\');

	cchVolName = lpszCur - lpszBase;
	if (cchMax < cchVolName + 5 + cchFileName)
	{
		lstrcpy(lpszCanon, lpszFileName);
		return;
	}

	// Now loop through the remaining directory components until something
	// of the form <volume_name>\...\<one_or_more_dirs>\<base_name> fits.
	//
	// Assert that the whole filename doesn't fit -- this should have been
	// handled earlier.

	ASSERT(cchVolName + (int)lstrlen(lpszCur) > cchMax);
	while (cchVolName + 4 + (int)lstrlen(lpszCur) > cchMax)
	{
		do
		{
			lpszCur = _tcsinc(lpszCur);
			ASSERT(*lpszCur != '\0');
		}
		while (*lpszCur != '\\');
	}

	// Form the resultant string and we're done.
	lpszCanon[cchVolName] = '\0';
	lstrcat(lpszCanon, _T("\\..."));
	lstrcat(lpszCanon, lpszCur);
}

/////////////////////////////////////////////////////////////////////////////
// CCFileSpec

CCFileSpec::CCFileSpec(FS_BUILTINS spec)
{
	Initialise(spec);
}

CCFileSpec::CCFileSpec(LPCTSTR lpszSpec)
{
	SetFullSpec(lpszSpec);
}

CCFileSpec::~CCFileSpec()
{
	m_strDrive.Empty();
	m_strPath.Empty();
	m_strFileName.Empty();
	m_strExt.Empty();
}

void CCFileSpec::Initialise(void)
{
	m_strDrive.GetBufferSetLength(_MAX_DRIVE);
	m_strPath.GetBufferSetLength(_MAX_PATH);
	m_strFileName.GetBufferSetLength(_MAX_FNAME);
	m_strExt.GetBufferSetLength(_MAX_EXT);
}

void CCFileSpec::UnLock(void)
{
	m_strDrive.ReleaseBuffer();
	m_strPath.ReleaseBuffer();
	m_strFileName.ReleaseBuffer();
	m_strExt.ReleaseBuffer();
	
	m_strDrive.FreeExtra();
	m_strPath.FreeExtra();
	m_strFileName.FreeExtra();
	m_strExt.FreeExtra();
}

void CCFileSpec::Initialise(FS_BUILTINS Spec)
{
	TCHAR szPath[_MAX_PATH] = {0};
	TCHAR* ptr = NULL;
	
	Initialise();
	
	switch (Spec)
	{
		case FS_EMPTY:							//	Nothing
			break;
			
		case FS_APP:							//	Full application path and name
			GetModuleFileName(NULL, szPath, sizeof(szPath));
			SetFullSpec((LPCTSTR) szPath);
			break;
			
		case FS_APPDIR: 						//	Application folder
			GetModuleFileName(NULL, szPath, sizeof(szPath));
			SetFullSpec((LPCTSTR) szPath);
			m_strExt      = _T("");
			m_strFileName = _T("");
			break;
			
		case FS_WINDIR: 						//	Windows folder
			GetWindowsDirectory(szPath, sizeof(szPath));
			
			if (szPath[lstrlen((LPCTSTR) szPath)] != _T('\\'))
				lstrcat(szPath, _T("\\"));
			
			SetFullSpec((LPCTSTR) szPath);
			break;
			
		case FS_SYSDIR: 						//	System folder
			GetSystemDirectory(szPath, sizeof(szPath));
			
			if (szPath[lstrlen((LPCTSTR) szPath)] != _T('\\'))
				lstrcat(szPath, _T("\\"));
			
			SetFullSpec((LPCTSTR) szPath);
			break;
			
		case FS_TMPDIR: 						//	Temporary folder
			GetTempPath(sizeof(szPath), szPath);
			SetFullSpec((LPCTSTR) szPath);
			break;
			
		case FS_DESKTOP:						//	Desktop folder
			GetShellFolder(_T("Desktop"));
			break;
			
		case FS_FAVOURITES:						//	Favourites folder
			GetShellFolder(_T("Favorites"));
			break;
			
		case FS_TEMPNAME:
			GetTempPath(sizeof(szPath), szPath);
			lstrcpy(szPath, ptr = _ttempnam(szPath, _T("~")));
			SetFullSpec((LPCTSTR) szPath);
			free(ptr);
			break;
			
		default:
	#ifdef _DEBUG
			afxDump << "Invalid initialisation spec for CCFileSpec, " << Spec
				<< "\n";
	#endif
			ASSERT(NULL);
	}
}

void CCFileSpec::GetShellFolder(LPCTSTR lpszFolder)
{
	HKEY  hKey;
	TCHAR szPath[_MAX_PATH];
	DWORD dwDataType,	dwDataSize = sizeof(szPath);
	
	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"),
		0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		if (RegQueryValueEx(hKey, lpszFolder, NULL, &dwDataType, 
			(UCHAR*) szPath, &dwDataSize) == ERROR_SUCCESS)
		{
			//	We need to append a \\ here to ensure we get the full path 
			//  into the path member.  If not
			//	_splitpath will take the last part of the path and 
			//  think it's a filename
			if (szPath[lstrlen((LPCTSTR) szPath)] != _T('\\'))
				lstrcat(szPath, _T("\\"));
			
			SetFullSpec((LPCTSTR) szPath);
		}
		
		RegCloseKey(hKey);
	}
}

//	Operations
BOOL CCFileSpec::FileExists(void) const
{
	OFSTRUCT ofs;
	return (OpenFile(GetFullSpec(), &ofs, OF_EXIST) != HFILE_ERROR);
}

//	Access functions : Get title + ext
CString CCFileSpec::GetFileNameEx(void) const
{
	return m_strFileName + m_strExt;
}

void CCFileSpec::SetFileNameEx(LPCTSTR lpszSpec)
{
	m_strFileName.GetBufferSetLength(_MAX_FNAME);
	m_strExt.GetBufferSetLength(_MAX_EXT);

#ifdef _UNICODE
	_wsplitpath(lpszSpec, NULL, NULL, (LPTSTR)(LPCTSTR) m_strFileName, 
		(LPTSTR)(LPCTSTR) m_strExt);
#else
	_splitpath(lpszSpec, NULL, NULL, (LPTSTR)(LPCTSTR) m_strFileName, 
		(LPTSTR)(LPCTSTR) m_strExt);
#endif

	m_strFileName.ReleaseBuffer();
	m_strExt.ReleaseBuffer();
	m_strFileName.FreeExtra();
	m_strExt.FreeExtra();
}

CString	CCFileSpec::GetFullPathNoExtension(void) const
{
	return m_strDrive + m_strPath + m_strFileName;
}

CString CCFileSpec::GetFullSpec(void) const
{
	return m_strDrive + m_strPath + m_strFileName + m_strExt;
}

void CCFileSpec::SetFullSpec(FS_BUILTINS Spec)
{
	Initialise(Spec);
}

void CCFileSpec::SetFullSpec(LPCTSTR lpszSpec)
{
	Initialise();
#ifdef _UNICODE
	_wsplitpath(lpszSpec, (LPTSTR)(LPCTSTR) m_strDrive, (LPTSTR)(LPCTSTR) m_strPath, 
		(LPTSTR)(LPCTSTR) m_strFileName, (LPTSTR)(LPCTSTR) m_strExt);
#else
	_splitpath(lpszSpec, (LPTSTR)(LPCTSTR) m_strDrive, (LPTSTR)(LPCTSTR) m_strPath, 
		(LPTSTR)(LPCTSTR) m_strFileName, (LPTSTR)(LPCTSTR) m_strExt);
#endif
	UnLock();
}

LPCTSTR CCFileSpec::GetShortPathName(void) const
{
	static TCHAR szShortPathName[_MAX_PATH-5];
	
	::GetShortPathName(GetFullSpec(), szShortPathName, _MAX_PATH-6);
	
	return szShortPathName;
}

void CCFileSpec::SetDirectory(LPCTSTR lpszDirectory)
{
	CString strFullSpec = lpszDirectory;
	
	if (lstrlen(GetFileName()) == 0)
	{
		// this is section is used in case the Directory is set before the
		// filename
		SetFileNameEx(_T("temp"));
		strFullSpec += GetFileName();
		SetFullSpec(strFullSpec);
		SetFileNameEx(_T(""));
	}
	else
	{
		strFullSpec += GetFileName();
		SetFullSpec(strFullSpec);
	}
}

LPCTSTR CCFileSpec::operator=(LPCTSTR lpszSpec)
{
	SetFullSpec(lpszSpec);
	return GetFullSpec();
}

/////////////////////////////////////////////////////////////////////////////
// CCReadFile

// 
// FUNCTION	:  CCReadFile::CCReadFile()
// 
// Description: This is the contructor for this class. It is possible
//	the set the size of the internal buffer in the constructor.
// 
// Params	: 
//		  [int nBufSize] - size of the used buffer
//

CCReadFile::CCReadFile(int nBufSize)
{
	m_dwRead        = nBufSize;
	m_nBufferSize   = nBufSize;
	m_pBuffer       = new BYTE[nBufSize];
	m_dwMasterIndex = 0;
	m_dwIndex       = 0;
	m_dwLine        = 0; 
	m_fFile.m_hFile = NULL;
}

CCReadFile::~CCReadFile()
{
	if (m_fFile.m_hFile != NULL)
		m_fFile.Close();
	delete[] m_pBuffer;
}

// 
// FUNCTION	:  CCReadFile::Open()
// 
// Description:This function opens a file for reading. The file is 
//			opened with readonly reads...and shared only when read
// 
// Returns	: [BOOL] - FALSE on open failure (you may check 
//							optional feError)
//					   TRUE on successfull opening of this file
// Params	: 
//			  [LPCTSTR szFile] - The wanted file
//			  [CFileException *feError] -(optional) this provides for
//						means of extended error detection

BOOL CCReadFile::Open(LPCTSTR szFile, CFileException* feError)
{
	CFileStatus	rStatus;
	if(!m_fFile.Open(szFile, CFile::modeRead | CFile::shareDenyWrite, feError))
		return FALSE;
	m_fFile.GetStatus(rStatus);
	m_nMaxSize = rStatus.m_size;
	return TRUE;
}

// 
// FUNCTION	:  CCReadFile::Close()
// 
// Description:Closes our previously opened file
// 

void CCReadFile::Close(void)
{
	m_fFile.Close();
	m_dwMasterIndex = 0;
	m_dwIndex       = 0;
	m_dwLine        = 0;
}

// 
// FUNCTION	:  CCReadFile::GetNextLine()
// 
// Description	: Read the next line from a file
// 
// Returns	: [DWORD] - 0 nothing read because of an error or EOF
//						non 0...index of line first line has line 
//						number 1
// Params	: 
//			  [CString &szLine] - The read line return in a CString
// 

DWORD CCReadFile::GetNextLine(CString& szLine)
{
	DWORD	dwReturn;
	TCHAR*  szBuffer = new TCHAR[m_nBufferSize];
	ASSERT(szBuffer != NULL);
	dwReturn = this->GetNextLine(szBuffer, m_nBufferSize);
	if (dwReturn != 0)
		szLine = szBuffer;
	else 
		szLine = _T("");	//Empty

	delete[] szBuffer;
	return dwReturn;
}

// 
// FUNCTION	:  CCReadFile::GetNextLine()
// 
// Description	: Read the next line from a file
// 
// Returns	: [DWORD] - 0 nothing read because of an error or EOF
//						non 0...index of line first line has line 
//						number 1
// Params	: 
//			  [LPTSTR szLine] - Pointer to a buffer which recieves the 
//								read line
//			  [int iLineSize] - Size of the supplied line buffer

DWORD CCReadFile::GetNextLine(LPTSTR szLine, int iLineSize)
{
	BYTE	*chTemp;
	BOOL	bStop = FALSE;
	int		nOut;

	chTemp  = (LPBYTE) szLine;
	*chTemp = 0;
	nOut    = 0;
	while (!bStop)
	{
		if (!m_dwLine || m_dwIndex == m_dwRead)
		{
			m_dwMasterIndex = m_fFile.Seek(0, CFile::current);
			m_dwRead        = m_fFile.Read((LPVOID) m_pBuffer, m_nBufferSize);
			m_dwIndex       = 0;
			if (m_dwRead == 0)
			{
				bStop = TRUE; //Error during readfile or END-OF-FILE encountered
				if (nOut > 0)
				{
					chTemp[nOut++] = (TCHAR) 0;
					return m_dwLine;	
				}
				else 
					return m_dwLine = 0; //nix gelezen
			}
			else
			{
				if (m_dwRead != (DWORD) m_nBufferSize)
					bStop = TRUE;	//END-OF-FILE
			}
		}
		for (; m_dwIndex < m_dwRead ; m_dwIndex++)
		{
			if ((nOut+1) == iLineSize)
			{
				m_strError.Format(_T("m_pBuffer overflow in line %u (line length over %d chars)"),
					++m_dwLine, iLineSize);
				::MessageBox(NULL, m_strError, _T("Warning!"), 
					MB_ICONEXCLAMATION | MB_OK);
				chTemp[nOut] = _T('\0');
				return m_dwLine;
			}
			switch (m_pBuffer[m_dwIndex])
			{
			case 0x0d: // End of Line encountered
			case 0x0a:
				// Check we're not on end of m_pBuffer ???
				if ((m_dwIndex + 1) < m_dwRead)
				{
					if (m_pBuffer[m_dwIndex+1] == _T('\n') || 
						m_pBuffer[m_dwIndex+1] == _T('\r'))
					{
						if (!*chTemp)
							m_dwLine++;
						m_dwIndex++;
					}
				}
				if (*chTemp)
				{
					chTemp[nOut++] = _T('\0');
					m_dwLine++;
					return m_dwLine;
				}
				break;
			default: 
				chTemp[nOut++] = m_pBuffer[m_dwIndex];
			}
		}
	}
	if (nOut > 0)
	{
		chTemp[nOut++] = _T('\0');
		return m_dwLine;	
	}
	return m_dwLine = 0; //
}

void CCReadFile::GetErrorText(CString& szMsg)
{
	szMsg = m_strError;
}

// 
// FUNCTION	:  CCReadFile::Reset()
// 
// Description:Reset the linecounter....this function effectively sets
//			the reading pointer to 0, so when the GetNextLine is 
//			executed it will start reading the first line

void CCReadFile::Reset(void)
{
	m_dwIndex = 0;
	m_dwLine  = 0;
	m_fFile.SeekToBegin();
}

/////////////////////////////////////////////////////////////////////////////
// CCFileChangeEvent
//

CCFileChangeEvent::~CCFileChangeEvent()
{
    if (m_bEvent)
		StopWatching();
    POSITION pos = m_mapFileInfo.GetStartPosition();
    CString sPathName;
    CCFileInfo* pFileInfo;
    while (pos)
	{
        m_mapFileInfo.GetNextAssoc(pos, sPathName, pFileInfo);
        ASSERT(pFileInfo);
        delete pFileInfo;
	}
    pos = m_lstFilePath.GetHeadPosition();
    CCFilePath *pFilePath;
    while (pos)
	{
        pFilePath = m_lstFilePath.GetNext(pos);
        ASSERT(pFilePath);
        delete pFilePath;
	}
}

void CCFileChangeEvent::StartWatching()
{
    if (!m_bEvent)
    {
		m_evWatching.ResetEvent();
		m_bEvent = AfxBeginThread(FileEventProc, static_cast<LPVOID>(this)) != NULL;
    }
}

void CCFileChangeEvent::StopWatching()
{
    if (m_bEvent)
	{
        m_bEvent = FALSE;
        m_evWatching.SetEvent();
	}
}

void CCFileChangeEvent::AddFile(LPCTSTR pszPathName)
{
    CCFileInfo* pFileInfo;
    if (m_mapFileInfo.Lookup(pszPathName, pFileInfo))
		return;
    pFileInfo    = new CCFileInfo(pszPathName);
    m_mapFileInfo.SetAt(pszPathName, pFileInfo);
    POSITION pos = m_lstFilePath.GetHeadPosition();
    CCFilePath* pFilePath;
    while (pos)
	{
        pFilePath = m_lstFilePath.GetNext(pos);
        ASSERT(pFilePath);
        if (!_tcscmp(pFilePath->GetPath(), pFileInfo->GetPath()))
		{
            pFilePath->Increment();
            return;
		}
	}
    m_lstFilePath.AddTail(new CCFilePath(pFileInfo->GetPath()));

    if (m_bEvent)
	{
        StopWatching();
        StartWatching();
	}
}

void CCFileChangeEvent::RemoveFile(LPCTSTR pszPathName)
{
    CCFileInfo* pFileInfo = NULL;
    if (!m_mapFileInfo.Lookup(pszPathName, pFileInfo))
		return;
    ASSERT(pFileInfo);
    POSITION pos = m_lstFilePath.GetHeadPosition(), posOld;
    CCFilePath* pFilePath;
    while (pos)
	{
        posOld = pos;
        pFilePath = m_lstFilePath.GetNext(pos);
        ASSERT(pFilePath);
        if (!_tcscmp(pFilePath->GetPath(), pFileInfo->GetPath()))
		{
            if (pFilePath->GetCount() == 1)
			{
                delete pFilePath;
                delete pFileInfo;
                m_lstFilePath.RemoveAt(posOld);
                m_mapFileInfo.RemoveKey(pszPathName);
			}
            else
				pFilePath->Decrement();
            return;
		}
	}
}

UINT CCFileChangeEvent::FileEventProc(LPVOID lpParam)
{
    CCFileChangeEvent* pFileEvent = reinterpret_cast<CCFileChangeEvent*>(lpParam);
    int nFilePathCount = pFileEvent->m_lstFilePath.GetCount();
    HANDLE* phChanges  = new HANDLE[nFilePathCount + 1];
    for (int i; ;)
	{
        *phChanges   = pFileEvent->m_evWatching;
        POSITION pos = pos = pFileEvent->m_lstFilePath.GetHeadPosition();
        CCFilePath* pFilePath;
        int j = 0;
        for (i = 0; i < nFilePathCount; i++)
		{
            if (!pos)
			{
                nFilePathCount = i;
                break;
			}
            pFilePath = pFileEvent->m_lstFilePath.GetNext(pos);
            ASSERT(pFilePath);
            HANDLE hFC = FindFirstChangeNotification(pFilePath->GetPath(), FALSE,
				FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_SIZE | 
				FILE_NOTIFY_CHANGE_FILE_NAME);
            if (hFC != INVALID_HANDLE_VALUE) // if i can gen a notification --
				phChanges[++j] = hFC;
		}
        DWORD dwResult = WaitForMultipleObjects(j + 1, phChanges, FALSE, INFINITE);
        for (i = 1; i < j; i++)
			VERIFY(FindCloseChangeNotification(phChanges[i + 1]));
        if (dwResult == WAIT_OBJECT_0)
			break;
        int nPath = dwResult - WAIT_OBJECT_0 - 1;
        pos       = pFileEvent->m_lstFilePath.FindIndex(nPath);
        ASSERT(pos);
        pFilePath = pFileEvent->m_lstFilePath.GetAt(pos);
        ASSERT(pFilePath);
        CString sPathName;
        CCFileInfo* pFileInfo;
        for (pos = pFileEvent->m_mapFileInfo.GetStartPosition(); pos;)
		{
            pFileEvent->m_mapFileInfo.GetNextAssoc(pos, sPathName, pFileInfo);
            ASSERT(pFileInfo);
            if (!_tcscmp(pFilePath->GetPath(), pFileInfo->GetPath()))
			{
                HANDLE hFile = CreateFile(sPathName, GENERIC_READ, FILE_SHARE_READ,
					NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                if (hFile != INVALID_HANDLE_VALUE)
				{
                    FILETIME ftModified;
                    VERIFY(GetFileTime (hFile, NULL, NULL, &ftModified));
                    DWORD dwSize;
					dwSize = GetFileSize(hFile, NULL);
					ASSERT(dwSize != (DWORD)-1);
					CloseHandle(hFile);
                    WPARAM wEvent = FE_NONE;
                    if (CompareFileTime(&pFileInfo->GetModified(), &ftModified) < 0)
					{
                        pFileInfo->SetModified(ftModified);
                        wEvent |= FE_CHANGED_TIME;
					}
                    if (pFileInfo->GetSize() != dwSize)
					{
                        pFileInfo->SetSize(dwSize);
                        wEvent |= FE_CHANGED_SIZE;
					}
                    if (wEvent)
						pFileEvent->OnFileEvent(wEvent, sPathName);
				}
                else
				{
                    pFileEvent->OnFileEvent(FE_DELETED, sPathName);
                    pFileEvent->RemoveFile(sPathName);
				}
			}
		}
	}
    delete[] phChanges;
    return 0;
}


