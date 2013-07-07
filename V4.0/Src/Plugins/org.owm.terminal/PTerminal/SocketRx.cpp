// SocketRx.cpp: implementation of the CSocketRx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Telnet.h"
#include "ProtocolRx.h"
#include "SocketRx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSocketRx::CSocketRx()
{
	m_nExit = 0;
	m_hThread = NULL;
	m_hSocket = NULL;
}

CSocketRx::CSocketRx(SOCKET hSocket,HANDLE &hThread)
{
	DWORD dwRet;

	m_nExit = 0;
	m_hThread = NULL;
	m_hSocket = hSocket;

	m_hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE) RdTh,(LPVOID)this,0,&dwRet);
	if ( m_hThread == NULL ) return;
	hThread = m_hThread;
}

CSocketRx::~CSocketRx()
{
	m_nExit = 1;
	TRACE("CSocketRx::~CSocketRx()\n");
}

DWORD CSocketRx::RdTh(CSocketRx *pSocketRx)
{
	char pBuf[256];
	char* scan;
	int nRet;

	while(1)
	{
		if ( pSocketRx->m_nExit == 1 )
		{
			ExitThread(0);
			return 0;
		}

		nRet = recv(pSocketRx->m_hSocket,pBuf,sizeof(pBuf),0);
		if ( nRet == SOCKET_ERROR )
		{
			TRACE( "\nRaed Fail.........!\n" );
			pSocketRx->m_nExit = 1;
			continue;
		}

		if ( nRet == 0) Sleep(10);

		scan = pBuf;
		while(nRet--)
		{
			pSocketRx->m_Protocol.TelentProtcol(pSocketRx->m_hSocket,*scan++);
			//TRACE("%c ",*scan);
		}

	}
	return 0;
}
