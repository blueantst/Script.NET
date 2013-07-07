////////////////////////////////////////////////////////////////////////////
//	File:		Wait.cpp
//	Version:	1.0.0.0
//	Created:	6-20-2002
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	implementation of wait object and wait list
////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Wait.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static int g_WaitID = 10;

CWait::CWait(int nID)
{
	m_bWait = FALSE;
	if(nID == 0)
	{
		m_nID = g_WaitID++;
	}else
	{
		m_nID = nID;
	}
//	m_hWait = ::CreateEvent(NULL, TRUE, TRUE, NULL);
	// 保存创建时候的线程ID
//	m_dwThreadID = ::GetCurrentThreadId();
}

CWait::~CWait()
{
//	if(m_hWait)
//		::CloseHandle(m_hWait);
	m_bWait = FALSE;
}

//////////////////////////////////////////////////////////////////////
//等待指定时间或者被唤醒
//////////////////////////////////////////////////////////////////////
int CWait::Wait(DWORD dwTimeOut)
{
	MSG msg;
	BOOL bTimeOut = FALSE;
	DWORD tick = ::GetTickCount();
	if(!m_bWait)  m_bWait = TRUE;
	//if(m_hWait != NULL)
	//	::ResetEvent(m_hWait);
	//DWORD dwWait = (dwTimeOut>0) ? dwTimeOut : INFINITE;
	while(m_bWait && !bTimeOut)
	{
		// 判断超时,如果超时时间为0表示永远等
		if(dwTimeOut > 0 && ::GetTickCount()-tick >= dwTimeOut)
			bTimeOut = TRUE;
		if(::GetMessage(&msg, NULL, NULL, NULL))
		{															
			TranslateMessage(&msg);										
			DispatchMessage(&msg);
		}
		/*int rc = ::MsgWaitForMultipleObjects(1, &m_hWait, FALSE, dwWait, QS_ALLINPUT);
		if(rc == WAIT_OBJECT_0 + 1)
		{
			while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{															
				TranslateMessage(&msg);										
				DispatchMessage(&msg);
			}
		}*/
	}

	m_bWait = FALSE;
	if(bTimeOut)	// 超时退出
	{
		return 0;
	}

	// 被唤醒而退出
	return 1;
}

//////////////////////////////////////////////////////////////////////
// 唤醒等待
//////////////////////////////////////////////////////////////////////
void CWait::Awake()
{
	m_bWait = FALSE;
//	if(m_hWait)
//		::SetEvent(m_hWait);
	// 如果不在同一个线程中,则需要发送线程消息来触发消息循环
//	if(::GetCurrentThreadId() != m_dwThreadID)
//	{		
//		::PostThreadMessage(m_dwThreadID, 0, 0, 0);
//	}
}

//////////////////////////////////////////////////////////////////////
// Wait Object List function
//////////////////////////////////////////////////////////////////////
static CPtrList g_WaitObjList;	// Wait对象列表

/////////////////////////////////////////////////////////////////////////////
// 删除所有Wait对象
/////////////////////////////////////////////////////////////////////////////
void DeleteAllWaitObject()
{
	int count = g_WaitObjList.GetCount();
	for(int i = count-1; i >= 0; i --)
	{
		POSITION pos = g_WaitObjList.FindIndex(i);
		CWait* pWait = (CWait*)(g_WaitObjList.GetAt(pos));
		if(pWait != NULL)
		{
			delete pWait;
		}
	}

	g_WaitObjList.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
// 创建一个Wait对象,如果存在相同ID的对象,则直接返回此对象
/////////////////////////////////////////////////////////////////////////////
CWait* CreateWaitObject(int nID)
{
	int count = g_WaitObjList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_WaitObjList.FindIndex(i);
		CWait* pWait = (CWait*)(g_WaitObjList.GetAt(pos));
		if(pWait->GetWaitID() == nID)
		{
			return pWait;
		}
	}

	CWait* pWait = new CWait(nID);
	g_WaitObjList.AddTail(pWait);
	return pWait;
}

/////////////////////////////////////////////////////////////////////////////
// 删除一个Wait对象
/////////////////////////////////////////////////////////////////////////////
BOOL DeleteWaitObject(int nID)
{
	int count = g_WaitObjList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_WaitObjList.FindIndex(i);
		CWait* pWait = (CWait*)(g_WaitObjList.GetAt(pos));
		if(pWait->GetWaitID() == nID)
		{
			g_WaitObjList.RemoveAt(pos);
			delete pWait;
			return TRUE;
		}
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 唤醒一个Wait对象
/////////////////////////////////////////////////////////////////////////////
BOOL AwakeWaitObject(int nID)
{
	int count = g_WaitObjList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_WaitObjList.FindIndex(i);
		CWait* pWait = (CWait*)(g_WaitObjList.GetAt(pos));
		if(pWait->GetWaitID() == nID)
		{
			pWait->Awake();
			return TRUE;
		}
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 获取Wait对象
/////////////////////////////////////////////////////////////////////////////
CWait* GetWaitObject(int nID)
{
	int count = g_WaitObjList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_WaitObjList.FindIndex(i);
		CWait* pWait = (CWait*)(g_WaitObjList.GetAt(pos));
		if(pWait->GetWaitID() == nID)
		{
			return pWait;
		}
	}

	return NULL;
}

