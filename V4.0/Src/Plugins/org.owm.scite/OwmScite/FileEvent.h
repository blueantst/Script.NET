///////////////////////////////////////////////////////////////////////////
//  File:    filevent.h
//  Version: 1.1.0.4
//  Updated: 19-Jul-1998
//
//  Copyright:  Ferdinand Prantl
//  E-mail:     prantl@ff.cuni.cz
//
//  Some handy stuff to deal with files and their names
//
//  You are free to use or modify this code to the following restrictions:
//  - Acknowledge me somewhere in your about box, simple "Parts of code by.."
//  will be enough. If you can't (or don't want to), contact me personally.
//  - LEAVE THIS HEADER INTACT
////////////////////////////////////////////////////////////////////////////

#ifndef __FILEVENT_H__
#define __FILEVENT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#pragma warning(disable:4786)
//////////////////////////////////////////////////////////////////////

class CFileInfo
  {
  public:
    CFileInfo (LPCTSTR pszPathName);
    ~CFileInfo () {}
    LPCTSTR GetPathName () const { return m_sPathName; }
    LPCTSTR GetPath () const { return m_sPath; }
    const FILETIME &GetModified () const { return m_ftModified; }
    DWORD GetSize () const { return m_dwSize; }
    void SetModified (const FILETIME &ftModified) { m_ftModified = ftModified; }
    void SetSize (DWORD dwSize) { m_dwSize = dwSize; }
  protected:
    CString m_sPathName, m_sPath;
    FILETIME m_ftModified;
    DWORD m_dwSize;
  };

//////////////////////////
class CFilePath
  {
  public:
    CFilePath (LPCTSTR pszPath) : m_sPath (pszPath), m_nCount (1) {}
    ~CFilePath () {}
    LPCTSTR GetPath () const { return m_sPath; }
    int GetCount () const { return m_nCount; }
    void Inc () { m_nCount++; }
    void Dec () { m_nCount--; }
  protected:
    CString m_sPath;
    int m_nCount;
  };

typedef CMap<CString, LPCTSTR, CFileInfo*, CFileInfo*> CFileInfoMap;
typedef CList<CFilePath*, CFilePath*> CFilePathList;

#define FE_NONE 0
#define FE_CHANGED_NAME 1
#define FE_CHANGED_SIZE 2
#define FE_CHANGED_TIME 4
#define FE_MODIFIED (FE_CHANGED_NAME|FE_CHANGED_SIZE|FE_CHANGED_TIME)
#define FE_DELETED 8

/////////////////////////////
class CFileEvent
  {
  public:
    CFileEvent () : m_bEvent (false) {}
    virtual ~CFileEvent ();
    void AddFile (LPCTSTR pszPathName);
    void RemoveFile (LPCTSTR pszPathName);
    void StartWatching ();
    void StopWatching ();
    bool IsWatching () const { return m_bEvent; }
  protected:
    virtual void OnFileEvent (WPARAM wEvent, LPCTSTR pszPathName) = 0;
    CEvent m_evWatching;
    CFileInfoMap m_mapFileInfo;
    CFilePathList m_lstFilePath;
    bool m_bEvent;
    static UINT FileEventProc (LPVOID lpParam);
  };

//////////////////////////////////////////////////////////////////////

#endif // __FILEVENT_H__
