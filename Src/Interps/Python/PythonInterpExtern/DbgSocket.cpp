#include "StdAfx.h"
#include "DbgSocket.h"
#include <list>
#include <string>
#include <vector>

CDebugSocket CDebugSocket::m_Instance;

#pragma comment(lib, "Ws2_32.lib")

CDebugSocket::CDebugSocket(void)
{
	m_lpfnRecvMsg = NULL;
	m_lpfnEvent   = NULL;
	m_lpfnLogPrint = NULL;
	m_Socket = INVALID_SOCKET;
	m_SockConn = INVALID_SOCKET;
	m_bConnct = false;
	m_bRecvThreadRun = true;
	m_ServerThreadHandle = NULL;
	m_RecvThreadHandle = NULL;
	// 初始化socket
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
}

CDebugSocket::~CDebugSocket(void)
{

}

CDebugSocket *CDebugSocket::Instance()
{
	return &m_Instance;
}

void CDebugSocket::Destroy()
{
	try
	{
		StopServer();	// 停止服务线程
		Disconnect();	// 断开socket客户端连接
		m_bRecvThreadRun = false;

		DWORD dwExitCode = 0;
		if (m_RecvThreadHandle)
		{
			TerminateThread(m_RecvThreadHandle, dwExitCode);
			WaitForSingleObject(m_RecvThreadHandle,INFINITE);
			CloseHandle(m_RecvThreadHandle);
		}

		WSACleanup();
	}
	catch (...)
	{
	}
}

void CDebugSocket::RegRecvMsgCallBack(RECV_CALLBACK pRecv)
{
	m_lpfnRecvMsg = pRecv;
}

void CDebugSocket::RegEventCallBack(EVENT_CALLBACK pEvent)
{
	m_lpfnEvent = pEvent;
}

void CDebugSocket::RegPrintLog(PRINT_LOG_CALLBACK pLog)
{
	m_lpfnLogPrint = pLog;
}

// 启动服务
int CDebugSocket::StartServer(unsigned short uPort)
{
	// 初始化Socket
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_Socket == INVALID_SOCKET) 
	{
		return -1;
	}

	// 配置监听地址和端口
	SOCKADDR_IN serverAddr;
	ZeroMemory((char *)&serverAddr,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(uPort);           // 本地监听端口
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //本地监听IP

	// 绑定监听socket
	bind(m_Socket,(struct sockaddr *)&serverAddr,sizeof(serverAddr));

	// 开始监听
	listen(m_Socket,1);

	PrintLog("CDebugSocket::StartServer start listen");

	SOCKADDR_IN addrClient;
	int len=sizeof(SOCKADDR);

	m_SockConn = accept(m_Socket,(SOCKADDR*)&addrClient,&len);
	if(m_SockConn)
	{
		SetConnctState(true);
		//SetCallbackEvent(TCP_CONNECT);
		PrintLog("CDebugSocket::StartServer accept socket conn succ");
	}else
	{
		PrintLog("CDebugSocket::StartServer accept socket conn failed");
		return -2;
	}

	char szRecvbuf[DEFAULT_BUFLEN] = {0x00};
	int nRecvbuflen = DEFAULT_BUFLEN;

	char szTmp[DEFAULT_BUFLEN * 2] = {0x00}; // 数据缓存区
	int  nTmpLen = 0;                        // 数据缓存区中是否有数据未处理
	bool bIsNewMsg = true;                   // 标识是否是一个新的数据包

	unsigned char *pMsgData = NULL;			// 数据

	while(m_SockConn != INVALID_SOCKET)
	{
		// 接收数据
		memset(szRecvbuf, 0x00, DEFAULT_BUFLEN);
		nRecvbuflen = DEFAULT_BUFLEN;
		int iResult = recv(m_SockConn, szRecvbuf, nRecvbuflen, 0);
		if ( iResult < 0)
		{
			CString strLog = "recv error, errorcode:";
			strLog += GetErrorCode(GetLastError());
			PrintLog(strLog.GetBuffer(0));

			// 连接发生错误
			DWORD dwError = GetLastError();
			SetCallbackEvent(TCP_ERROR);
			closesocket(m_SockConn);
			m_SockConn = INVALID_SOCKET;

			closesocket(m_Socket);
			m_Socket = INVALID_SOCKET;
			
			SetConnctState(false);
			nTmpLen = 0; 
			memset(szTmp, 0x00, DEFAULT_BUFLEN * 2);
			bIsNewMsg = true;

			if (NULL != pMsgData)
			{
				delete []pMsgData;
				pMsgData = NULL;
			}

			return -2;
		}

		// 接收成功,数据处理
		if (m_lpfnRecvMsg && (iResult > 0))
		{
			m_lpfnRecvMsg(szRecvbuf); 
		}

		Sleep(10);
	}

	return 0;
}

// 停止服务
BOOL CDebugSocket::StopServer()
{
	PrintLog("CDebugSocket::StopServer");

	// 关闭连接socket
	if (m_SockConn != INVALID_SOCKET)
	{
		closesocket(m_SockConn);
		m_SockConn = INVALID_SOCKET;
	}

	// 关闭监听socket
	if (m_Socket != INVALID_SOCKET)
	{
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
	}

	// 修改连接状态信息
	SetConnctState(false);
	SetCallbackEvent(TCP_DISCONNECT);

	return TRUE;
}

static bool bServerThread = false;	// 服务线程是否已创建
// 服务线程
DWORD CDebugSocket::ServerThreadProc(LPVOID lpParam)
{
	// 参数有效性检查
	if (NULL == lpParam)
	{
		return 0;
	}

	CDebugSocket *pDbgSocket = (CDebugSocket*)lpParam;
	pDbgSocket->StartServer();
	bServerThread = false;

	return 1;
}

// 启动socket服务线程
int CDebugSocket::StartServerThread(unsigned short uPort)
{
	if (!bServerThread)
	{
		bServerThread = true;
		DWORD dwThreadId = 0;
		// 创建socket server线程
		PrintLog("Create Server Thread...");
		m_ServerThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&CDebugSocket::ServerThreadProc,
			(LPVOID) this,0, &dwThreadId);
		if (NULL == m_ServerThreadHandle)
		{
			return -1;
		}
	}
	return 0;
}

// 发送命令给Socket客户端
int CDebugSocket::SendCmdToClient(const char *pMsg)
{
	// 构造要发送的数据
	if (m_SockConn != INVALID_SOCKET)
	{
		unsigned char *pData = NULL;
		int nDataLen = strlen(pMsg);
		pData = new BYTE[nDataLen+1];
		memset(pData, 0x00, nDataLen+1);
		memcpy(pData, pMsg, nDataLen);

		int iResult = 0;
		{
			CAutoLockWrap<CCSWrap> guard(m_lockSend);
			iResult = send( m_SockConn, (char*)pData, nDataLen, 0 );
		}

		if (pData != NULL)
		{
			delete [] pData;
			pData = NULL;
		}

		if (iResult == SOCKET_ERROR) 
		{
			SetCallbackEvent(TCP_ERROR);
			closesocket(m_SockConn);
			m_SockConn = INVALID_SOCKET;

			closesocket(m_Socket);
			m_Socket = INVALID_SOCKET;
			SetConnctState(false);
			return -1;
		}

		return 0;
	}
	else
	{
		return -1;
	}
}

// 建立新的连接
int CDebugSocket::Connect(const char *pszIP, unsigned short uPort)
{
	static bool bRecvThread = false;
	if (!bRecvThread)
	{
		bRecvThread = true;
		DWORD dwThreadId = 0;
		// 创建数据接收线程

		PrintLog("Create Recv Thread...");
		m_RecvThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&CDebugSocket::RecvThreadProc,
			(LPVOID) this,0, &dwThreadId);
		if (NULL == m_RecvThreadHandle)
		{
			return -1;
		}
	}
	// 先关闭旧的连接，避免一个客户端建立多个连接
	Disconnect();

	// 初始化Socket
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_Socket == INVALID_SOCKET) 
	{
		return -1;
	}

	m_strServerAddr = pszIP;

	struct sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr( pszIP );
	clientService.sin_port = htons( uPort );

	// 建立连接
	int iResult = connect( m_Socket, (SOCKADDR*) &clientService, sizeof(clientService) );
	if (iResult == SOCKET_ERROR) 
	{
		CString strLog = "Connect fail,errorCode:";
		strLog += GetErrorCode(GetLastError());
		PrintLog(strLog.GetBuffer(0));
		closesocket(m_Socket);
		return -1;
	}

	//连接建立成功设置Socket为阻塞模式
	u_long iMode = 0;
	ioctlsocket(m_Socket, FIONBIO, &iMode);

	GetLocalIP(pszIP, uPort);

	SetConnctState(true);
	SetCallbackEvent(TCP_CONNECT);
	return 0;
}

// 关闭连接
int CDebugSocket::Disconnect()
{
	if (IsConnect()) // 如果当前处于连接状态则关闭连接，否则不做处理
	{
		PrintLog("Start close connect.");
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
		SetConnctState(false);
		SetCallbackEvent(TCP_DISCONNECT);
	}

	return 0;
}

CString CDebugSocket::GetStrByINT(int nData)
{
	char szBuf[10] = {0x00};
	itoa(nData, szBuf, 10);

	CString strData = szBuf;
	return strData;
}

// 发送消息
int CDebugSocket::SendMsg(const char *pMsg)
{
	// 构造要发送的数据
	if (IsConnect())
	{
		unsigned char *pData = NULL;
		int nDataLen = 0;
		//CreateSendData(pMsg, &pData, nDataLen);

		int iResult = 0;
		{
			CAutoLockWrap<CCSWrap> guard(m_lockSend);
			iResult = send( m_Socket, (char*)pData, nDataLen, 0 );
		}

		if (pData != NULL)
		{
			delete [] pData;
			pData = NULL;
		}

		if (iResult == SOCKET_ERROR) 
		{
			SetCallbackEvent(TCP_ERROR);
			closesocket(m_Socket);
			m_Socket = INVALID_SOCKET;
			SetConnctState(false);
			return -1;
		}

		return 0;
	}
	else
	{
		return -1;
	}
}

// 作为客户端情况下的接收数据线程
DWORD CDebugSocket::RecvThreadProc(LPVOID lpParam)
{
	// 参数有效性检查
	if (NULL == lpParam)
	{
		return 0;
	}

	CDebugSocket *pDbgSocket = (CDebugSocket*)lpParam;

	char szRecvbuf[DEFAULT_BUFLEN] = {0x00};
	int nRecvbuflen = DEFAULT_BUFLEN;

	char szTmp[DEFAULT_BUFLEN * 2] = {0x00}; // 数据缓存区
	int  nTmpLen = 0;                        // 数据缓存区中是否有数据未处理
	bool bIsNewMsg = true;                   // 标识是否是一个新的数据包

	unsigned char *pMsgData;				// 数据

	// 开始接收数据
	while(pDbgSocket->m_bRecvThreadRun)
	{
		if (pDbgSocket->IsConnect())
		{
			// 接收数据
			memset(szRecvbuf, 0x00, DEFAULT_BUFLEN);
			nRecvbuflen = DEFAULT_BUFLEN;
			int iResult = recv(pDbgSocket->m_Socket, szRecvbuf, nRecvbuflen, 0);
			if ( iResult <= 0)
			{
				CString strLog = "recv error, errorcode:";
				strLog += pDbgSocket->GetErrorCode(GetLastError());
				pDbgSocket->PrintLog(strLog.GetBuffer(0));

				// 连接发生错误
				DWORD dwError = GetLastError();
				pDbgSocket->SetCallbackEvent(TCP_ERROR);
				closesocket(pDbgSocket->m_Socket);
				pDbgSocket->m_Socket = INVALID_SOCKET;
				pDbgSocket->SetConnctState(false);
				nTmpLen = 0; 
				memset(szTmp, 0x00, DEFAULT_BUFLEN * 2);
				bIsNewMsg = true;

				if (NULL != pMsgData)
				{
					delete []pMsgData;
					pMsgData = NULL;
				}
			}
			else
			{
				// 接收成功,数据处理
				
			}
		}
		else
		{
			bIsNewMsg = true;
			nTmpLen = 0; 
			memset(szTmp, 0x00, DEFAULT_BUFLEN * 2);

			if (NULL != pMsgData)
			{
				delete []pMsgData;
				pMsgData = NULL;
			}

			// 当前处于未连接状态，休眠等等连接成功
			Sleep(10);
		}
	}

	return 1;
}

// 获取连接成功的本地地址
CString CDebugSocket::GetLocalIP()
{
	return m_strLocalIP;
}

CString CDebugSocket::GetServerAddr()
{
	return m_strServerAddr;
}

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

void CDebugSocket::GetLocalIP(const char *pszIP, unsigned short uPort)
{
	// Declare and initialize variables
	PMIB_TCPTABLE pTcpTable;
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	char szLocalAddr[128] = {0x00};
	char szRemoteAddr[128] = {0x00};

	struct in_addr IpAddr;

	int i;

	pTcpTable = (MIB_TCPTABLE *) MALLOC(sizeof (MIB_TCPTABLE));
	if (pTcpTable == NULL) {
		return;
	}

	dwSize = sizeof (MIB_TCPTABLE);
	// Make an initial call to GetTcpTable to
	// get the necessary size into the dwSize variable
	if ((dwRetVal = GetTcpTable(pTcpTable, &dwSize, TRUE)) ==
		ERROR_INSUFFICIENT_BUFFER) {
			FREE(pTcpTable);
			pTcpTable = (MIB_TCPTABLE *) MALLOC(dwSize);
			if (pTcpTable == NULL) {
				return;
			}
	}
	// Make a second call to GetTcpTable to get
	// the actual data we require
	if ((dwRetVal = GetTcpTable(pTcpTable, &dwSize, TRUE)) == NO_ERROR) 
	{
		printf("\tNumber of entries: %d\n", (int) pTcpTable->dwNumEntries);
		for (i = 0; i < (int) pTcpTable->dwNumEntries; i++) 
		{
			if (MIB_TCP_STATE_ESTAB == pTcpTable->table[i].dwState)
			{
				IpAddr.S_un.S_addr = (u_long) pTcpTable->table[i].dwRemoteAddr;
				strcpy_s(szRemoteAddr, sizeof (szRemoteAddr), inet_ntoa(IpAddr));

				if (ntohs((u_short)pTcpTable->table[i].dwRemotePort) == uPort &&
					stricmp(szRemoteAddr, pszIP) == 0)
				{
					IpAddr.S_un.S_addr = (u_long) pTcpTable->table[i].dwLocalAddr;
					strcpy_s(szLocalAddr, sizeof (szLocalAddr), inet_ntoa(IpAddr));
					m_strLocalIP = szLocalAddr;
					break;
				}

			}
		}
	}

	FREE(pTcpTable);

	CString strLog = "Get local ip:";
	strLog += m_strLocalIP;
	PrintLog(strLog.GetBuffer(0));
}

void CDebugSocket::PrintLog(LPCSTR lpData)
{
	if (m_lpfnLogPrint)
	{
		m_lpfnLogPrint(lpData);
	}
}

void CDebugSocket::SetCallbackEvent(int nEventCode)
{
	if (m_lpfnEvent)
	{
		m_lpfnEvent(nEventCode);
	}
}

CString CDebugSocket::GetErrorCode(DWORD dwErrorCode)
{
	char szBuf[MAX_PATH] = {0x00};
	_sntprintf_s(szBuf, MAX_PATH, _TRUNCATE, "%u", dwErrorCode);
	CString strError = szBuf;
	return strError;
}