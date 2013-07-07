// SocketTx.cpp: implementation of the CSocketTx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Telnet.h"
#include "conio.h"
#include "SocketTx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern HANDLE stdin1;
extern HANDLE stdout1;
extern HANDLE stderr1;

CSocketTx::CSocketTx(SOCKET hSocket,HANDLE &hThread)
{
	DWORD dwRet;

	m_nExit = 0;
	m_hThread = NULL;
	m_hSocket = hSocket;

	m_hSendEvent = ::CreateEvent(NULL, TRUE, TRUE, NULL);

	m_hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE) SendTh,(LPVOID)this,0,&dwRet);
	if ( m_hThread == NULL ) return;
	hThread = m_hThread;

	m_strSend = "";
}

CSocketTx::~CSocketTx()
{
	if(m_hSendEvent) ::CloseHandle(m_hSendEvent);
	m_nExit = 1;
	TRACE("CSocketTx::~CSocketTx()\n");
}

DWORD CSocketTx::SendTh(CSocketTx *pSocketTx)
{
	int nRet;

	::ResetEvent(pSocketTx->m_hSendEvent);

	while(1)
	{
		if ( pSocketTx->m_nExit == 1 )
		{
			ExitThread(0);
			return 0;
		}

		DWORD dwWait = WaitForSingleObject(pSocketTx->m_hSendEvent, 100);
		if(dwWait == WAIT_OBJECT_0)
		{
			if ( pSocketTx->m_nExit == 1 )
			{
				ExitThread(0);
				return 0;
			}

			nRet = send(pSocketTx->m_hSocket,(LPCTSTR)(pSocketTx->m_strSend),pSocketTx->m_strSend.GetLength(),0);
			pSocketTx->m_strSend = "";
			if ( nRet == SOCKET_ERROR )
			{
				::ResetEvent(pSocketTx->m_hSendEvent);
				TRACE("\nSend Fail........!\n");
				pSocketTx->m_nExit = 0;
				continue;
			}

			::ResetEvent(pSocketTx->m_hSendEvent);
		}
	}
	return 0;
}

void CSocketTx::SetSendEvent()
{
	::SetEvent(m_hSendEvent);
}