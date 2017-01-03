///////////////////////////////////////////////////////////////////////////
//  File:    filevent.cpp
//  Version: 1.3.0.6
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

////////////////////////////////////////////////////////////////////////////
//  14-Sep-99
//  + FIX: GetFileSize stuff causing crash on file size comparings (Hans Eckardt)
//  + FIX: dealing with invalid handle during changes notifications (Roberto Lublinerman)
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "fileevent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

CFileInfo::CFileInfo (LPCTSTR pszPathName)
  {
    TCHAR szPath[_MAX_PATH], szDrive[_MAX_DRIVE];
    _tsplitpath (pszPathName, szDrive, szPath, NULL, NULL);
    m_sPathName = pszPathName;
    m_sPath = szDrive;
    m_sPath += szPath;
    HANDLE hFile = CreateFile (pszPathName, GENERIC_READ, FILE_SHARE_READ /*|FILE_SHARE_WRITE|FILE_SHARE_DELETE*/,
      NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
      {
        VERIFY (GetFileTime (hFile, NULL, NULL, &m_ftModified));
		// ->HE
        m_dwSize= GetFileSize (hFile, NULL);
		ASSERT(m_dwSize != (DWORD)-1);
		// <-HE
        CloseHandle (hFile);
      }
  }

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

CFileEvent::~CFileEvent ()
  {
    if (m_bEvent)
      StopWatching ();
    POSITION pos = m_mapFileInfo.GetStartPosition ();
    CString sPathName;
    CFileInfo *pFileInfo;
    while (pos)
      {
        m_mapFileInfo.GetNextAssoc (pos, sPathName, pFileInfo);
        ASSERT (pFileInfo);
        delete pFileInfo;
      }
    pos = m_lstFilePath.GetHeadPosition ();
    CFilePath *pFilePath;
    while (pos)
      {
        pFilePath = m_lstFilePath.GetNext (pos);
        ASSERT (pFilePath);
        delete pFilePath;
      }
  }

void CFileEvent::StartWatching ()
  {
    if (!m_bEvent)
    {
      m_evWatching.ResetEvent ();
      m_bEvent = AfxBeginThread (FileEventProc, (LPVOID) this) != NULL;
    }
  }

void CFileEvent::StopWatching ()
  {
    if (m_bEvent)
      {
        m_bEvent = false;
        m_evWatching.SetEvent ();
      }
  }

void CFileEvent::AddFile (LPCTSTR pszPathName)
  {
    CFileInfo *pFileInfo;
    if (m_mapFileInfo.Lookup (pszPathName, pFileInfo))
      return;
    pFileInfo = new CFileInfo (pszPathName);
    m_mapFileInfo.SetAt (pszPathName, pFileInfo);
    POSITION pos = m_lstFilePath.GetHeadPosition ();
    CFilePath *pFilePath;
    while (pos)
      {
        pFilePath = m_lstFilePath.GetNext (pos);
        ASSERT (pFilePath);
        if (!_tcscmp (pFilePath->GetPath (), pFileInfo->GetPath ()))
          {
            pFilePath->Inc ();
            return;
          }
      }
    m_lstFilePath.AddTail (new CFilePath (pFileInfo->GetPath ()));
    if (m_bEvent)
      {
        StopWatching ();
        StartWatching ();
      }
  }

void CFileEvent::RemoveFile (LPCTSTR pszPathName)
  {
    CFileInfo *pFileInfo;
    if (!m_mapFileInfo.Lookup (pszPathName, pFileInfo))
      return;
    ASSERT (pFileInfo);
    POSITION pos = m_lstFilePath.GetHeadPosition (), posOld;
    CFilePath *pFilePath;
    while (pos)
      {
        posOld = pos;
        pFilePath = m_lstFilePath.GetNext (pos);
        ASSERT (pFilePath);
        if (!_tcscmp (pFilePath->GetPath (), pFileInfo->GetPath ()))
          {
            if (pFilePath->GetCount () == 1)
              {
                delete pFilePath;
                delete pFileInfo;
                m_lstFilePath.RemoveAt (posOld);
                m_mapFileInfo.RemoveKey (pszPathName);
              }
            else
              pFilePath->Dec ();
            return;
          }
      }
  }

UINT CFileEvent::FileEventProc (LPVOID lpParam)
  {
    CFileEvent *pFileEvent = (CFileEvent*) lpParam;
    int nFilePathCount = pFileEvent->m_lstFilePath.GetCount ();
    HANDLE *phChanges = new HANDLE[nFilePathCount + 1];
    for (int i;;)
      {
        *phChanges = pFileEvent->m_evWatching;
        POSITION pos = pos = pFileEvent->m_lstFilePath.GetHeadPosition ();
        CFilePath *pFilePath;
        int j = 0;
        for (i = 0; i < nFilePathCount; i++)
          {
            if (!pos)
              {
                nFilePathCount = i;
                break;
              }
            pFilePath = pFileEvent->m_lstFilePath.GetNext (pos);
            ASSERT (pFilePath);
            HANDLE hFC = FindFirstChangeNotification (pFilePath->GetPath(), FALSE,
              FILE_NOTIFY_CHANGE_LAST_WRITE|FILE_NOTIFY_CHANGE_SIZE|FILE_NOTIFY_CHANGE_FILE_NAME);
            if (hFC != INVALID_HANDLE_VALUE) // if i can gen a notification --
              phChanges[++j] = hFC;
          }
        DWORD dwResult = WaitForMultipleObjects (j + 1, phChanges, FALSE, INFINITE);
        for (i = 1; i < j; i++)
          VERIFY (FindCloseChangeNotification (phChanges[i + 1]));
        if (dwResult == WAIT_OBJECT_0)
          break;
        int nPath = dwResult - WAIT_OBJECT_0 - 1;
        pos = pFileEvent->m_lstFilePath.FindIndex (nPath);
        //ASSERT (pos);
		if(pos == 0)
		{
			break;
		}
        pFilePath = pFileEvent->m_lstFilePath.GetAt (pos);
        //ASSERT (pFilePath);
		if(pFilePath == NULL)
		{
			break;
		}
        CString sPathName;
        CFileInfo *pFileInfo;
        for (pos = pFileEvent->m_mapFileInfo.GetStartPosition (); pos;)
          {
            pFileEvent->m_mapFileInfo.GetNextAssoc (pos, sPathName, pFileInfo);
            ASSERT (pFileInfo);
            if (!_tcscmp (pFilePath->GetPath (), pFileInfo->GetPath ()))
              {
                HANDLE hFile = CreateFile (sPathName, GENERIC_READ, FILE_SHARE_READ /*|FILE_SHARE_WRITE|FILE_SHARE_DELETE*/,
                  NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				// add at 2006-05-06 by blueant
				if (hFile == INVALID_HANDLE_VALUE)
				{
					// 文件保存过程中很大情况下返回的是无效句柄,需要延迟一段时间等保存完毕再打开
					Sleep(50);
					hFile = CreateFile (sPathName, GENERIC_READ, FILE_SHARE_READ /*|FILE_SHARE_WRITE|FILE_SHARE_DELETE*/,
						NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				}
                if (hFile != INVALID_HANDLE_VALUE)
                  {
                    FILETIME ftModified;
                    VERIFY (GetFileTime (hFile, NULL, NULL, &ftModified));
                    DWORD dwSize;
					// ->HE
					dwSize= GetFileSize (hFile, NULL);
					ASSERT(dwSize != (DWORD)-1);
					// <-HE
                   CloseHandle (hFile);
                    WPARAM wEvent = FE_NONE;
                    if (CompareFileTime (&pFileInfo->GetModified (), &ftModified) < 0)
                      {
                        pFileInfo->SetModified (ftModified);
                        wEvent |= FE_CHANGED_TIME;
                      }
                    if (pFileInfo->GetSize () != dwSize)
                      {
                        pFileInfo->SetSize (dwSize);
                        wEvent |= FE_CHANGED_SIZE;
                      }
                    if (wEvent)
                      pFileEvent->OnFileEvent (wEvent, sPathName);
                  }
                else
                  {
                    pFileEvent->OnFileEvent (FE_DELETED, sPathName);
                    pFileEvent->RemoveFile (sPathName);
                  }
              }
          }
      }
    delete [] phChanges;
    return 0;
  }

//////////////////////////////////////////////////////////////////////
