// FileUtils.h: interface for the CFileUtils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEUTILS_H__C18445C3_2C77_11D4_8649_AB38DB807153__INCLUDED_)
#define AFX_FILEUTILS_H__C18445C3_2C77_11D4_8649_AB38DB807153__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>	  // for CEvent
//#include "cedefs.h"

#define SFBUF_SIZE	       2048
#define MAX_ABBREV_LEN     60

UINT AFXAPI AfxGetFileName(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax);
void AFXAPI _AfxAbbreviateName(LPTSTR lpszCanon, int cchMax, BOOL bAtLeastName);

////////////////////////////////////////////////////////////////////////////////
// CCFileSpec declaration

class CCFileSpec
{
public:
	LPCTSTR operator=(LPCTSTR lpszSpec);
	
	void SetDirectory(LPCTSTR directory);
	LPCTSTR GetShortPathName(void) const;

	void SetTitle(LPCTSTR lpszTitle)
	{
		CString strFileName = lpszTitle;
		strFileName  += m_strExt;
		SetFileNameEx(strFileName);
	}
	
	enum FS_BUILTINS
	{
		FS_EMPTY,              //   Nothing
		FS_APP,                //   Full application path and name
		FS_APPDIR,             //   Application folder
		FS_WINDIR,             //   Windows folder
		FS_SYSDIR,             //   System folder
		FS_TMPDIR,             //   Temporary folder
		FS_DESKTOP,            //   Desktop folder
		FS_FAVOURITES,         //   Favorites folder
		FS_TEMPNAME            //   Create a temporary name
	};
	
	CCFileSpec(FS_BUILTINS Spec = FS_EMPTY);
	CCFileSpec(LPCTSTR lpszSpec);
	virtual ~CCFileSpec();
	
//	Operations
	BOOL  FileExists(void) const;		
	
//	Access functions
	CString&    GetDrive(void) { return m_strDrive; }
	CString&    GetPath(void)  { return m_strPath; }
	CString	    GetDirectory() const { return m_strDrive + m_strPath; }
	CString     GetFileName(void) const	{ return m_strFileName + m_strExt; }
	CString&    GetFileTitle(void)	{ return m_strFileName; }
	CString&    GetExt(void) { return m_strExt; }
	CString	    GetFullPathNoExtension(void) const;
	
	operator LPCTSTR() 
	{
		static CString strTemp;
		strTemp = GetFullSpec(); 
		return strTemp;
	}// as a C string
	
	CString	GetFullSpec(void) const;
	void	SetFullSpec(LPCTSTR lpszSpec);
	void	SetFullSpec(FS_BUILTINS Spec = FS_EMPTY);
	
	CString	GetFileNameEx(void) const;
	void	SetFileNameEx(LPCTSTR lpszSpec);
	void	SetExt(LPCTSTR lpszExt) { m_strExt = lpszExt; }

private:
	void	Initialise(void);
	void	Initialise(FS_BUILTINS Spec);
	void	UnLock(void);
	void	GetShellFolder(LPCTSTR lpszFolder);
	
	CString m_strDrive, m_strPath, m_strFileName, m_strExt;
};

////////////////////////////////////////////////////////////////////////////////
// CCReadFile declaration
// Author	: Frank Driesens
//

class CCReadFile  
{
public:
	CCReadFile(int nBufSize = SFBUF_SIZE);
	virtual ~CCReadFile();
	BOOL Open(LPCTSTR szFile, CFileException *feError = NULL);
	void Close(void);
	void Reset(void);
	void GetErrorText(CString& szMsg);
	DWORD GetNextLine(LPTSTR szLine, int iLineSize);
	DWORD GetNextLine(CString& szLine);

protected:
	int       m_nBufferSize;
	CString   m_strError;
	DWORD	  m_nMaxSize;
	DWORD	  m_dwRead;
	DWORD	  m_dwLine;
	DWORD	  m_dwMasterIndex;
	DWORD	  m_dwIndex;
	int		  m_nSectionCount;
	BYTE	 *m_pBuffer;
	CFile	  m_fFile;
};


////////////////////////////////////////////////////////////////////////////////
// CCFilePath declaration
//

class CCFilePath
{
public:
    CCFilePath(LPCTSTR pszPath) : m_sPath (pszPath), m_nCount (1) {}
    ~CCFilePath() {}

    LPCTSTR GetPath() const { return m_sPath; }
    int GetCount() const { return m_nCount; }
    void Increment() { InterlockedIncrement(&m_nCount); }
    void Decrement() { InterlockedDecrement(&m_nCount); }

protected:
    CString m_sPath;
    long    m_nCount;
};

typedef CList<CCFilePath*, CCFilePath*> CCFilePathList;

////////////////////////////////////////////////////////////////////////////////
// CCFileInfo declaration
//

class CCFileInfo
{
public:
    CCFileInfo(LPCTSTR pszPathName)
	{
		TCHAR szPath[_MAX_PATH]   = {0};
		TCHAR szDrive[_MAX_DRIVE] = {0};
		_tsplitpath(pszPathName, szDrive, szPath, NULL, NULL);
		m_sPathName  = pszPathName;
		m_sPath      = szDrive;
		m_sPath     += szPath;
		HANDLE hFile = CreateFile(pszPathName, GENERIC_READ, FILE_SHARE_READ,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			VERIFY(GetFileTime(hFile, NULL, NULL, &m_ftModified));
			m_dwSize = GetFileSize(hFile, NULL);
			ASSERT(m_dwSize != (DWORD)-1);
			CloseHandle(hFile);
		}
	}

    ~CCFileInfo() {}

    LPCTSTR GetPathName() const { return m_sPathName; }
    LPCTSTR GetPath() const { return m_sPath; }
    const FILETIME &GetModified() const { return m_ftModified; }
    DWORD GetSize() const { return m_dwSize; }
    void SetModified(const FILETIME &ftModified) { m_ftModified = ftModified; }
    void SetSize(DWORD dwSize) { m_dwSize = dwSize; }

protected:
    CString    m_sPathName;
	CString    m_sPath;
    FILETIME   m_ftModified;
    DWORD      m_dwSize;
};

typedef CMap<CString, LPCTSTR, CCFileInfo*, CCFileInfo*> CCFileInfoMap;

#define FE_NONE				0
#define FE_CHANGED_NAME		1
#define FE_CHANGED_SIZE		2
#define FE_CHANGED_TIME		4
#define FE_DELETED          8
//#define FE_MODIFIED			(FE_CHANGED_NAME | FE_CHANGED_SIZE | FE_CHANGED_TIME)

////////////////////////////////////////////////////////////////////////////////
// CCFileChangeEvent declaration
//

class CCFileChangeEvent
{
public:
    CCFileChangeEvent() : m_bEvent (FALSE) {}
    virtual ~CCFileChangeEvent();
    
	void AddFile(LPCTSTR pszPathName);
    void RemoveFile(LPCTSTR pszPathName);
    void StartWatching();
    void StopWatching();
    BOOL IsWatching() const { return m_bEvent; }

protected:
    virtual void OnFileEvent(WPARAM wEvent, LPCTSTR pszPathName) = 0;
    static UINT  FileEventProc(LPVOID lpParam);

    CEvent          m_evWatching;
    CCFileInfoMap   m_mapFileInfo;
    CCFilePathList  m_lstFilePath;
    BOOL            m_bEvent;
};

#endif // !defined(AFX_FILEUTILS_H__C18445C3_2C77_11D4_8649_AB38DB807153__INCLUDED_)
