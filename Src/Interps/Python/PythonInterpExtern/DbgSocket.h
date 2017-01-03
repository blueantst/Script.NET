#pragma once
#include <atlstr.h>
#include "sysobj.h"

// 消息接收回调
typedef int (*RECV_CALLBACK) (void *pData);

// 事件处理回调
typedef int (*EVENT_CALLBACK) (int nEventCode);

// 日志回调
typedef void (*PRINT_LOG_CALLBACK) (LPCSTR lpData);

enum EVENT_CODE
{
	TCP_CONNECT = 0x00, // 连接成功
	TCP_DISCONNECT,     // 主动关闭连接
	TCP_ERROR,          // 连接异常中断 
};

#define DEFAULT_BUFLEN 1024 // 默认接收数据包文大小

class CDebugSocket
{
public:
	~CDebugSocket(void);

	static CDebugSocket *Instance();

	// socket服务端
	// 启动服务
	int StartServer(unsigned short uPort = 17664);

	// 停止服务
	BOOL StopServer();

	// 启动服务线程
	int StartServerThread(unsigned short uPort = 17664);

	// 发送消息给Socket客户端
	int SendCmdToClient(const char *pMsg);

	// socket客户端
	// 建立连接
	int Connect(const char *pszIP, unsigned short uPort);

	// 关闭连接
	int Disconnect();

	// 发送消息
	int SendMsg(const char *pData);

	 // 获取当前连接服务器IP地址
	CString GetLocalIP();

	CString GetServerAddr();

	// 注册消息接收回调
	void RegRecvMsgCallBack(RECV_CALLBACK pRecv);

	// 注册状态通告回调
	void RegEventCallBack(EVENT_CALLBACK pEvent);

	// 注册日志打印接口
	void RegPrintLog(PRINT_LOG_CALLBACK pLog);

	bool IsConnect() 
	{
		CAutoLockWrap<CCSWrap> guard(m_lockConnect);
		return m_bConnct;
	};

	void Destroy();

private:
	void SetConnctState(bool bState) 
	{
		CAutoLockWrap<CCSWrap> guard(m_lockConnect);
		m_bConnct = bState;
	};

	// 服务线程
	static DWORD ServerThreadProc(LPVOID lpParam);

	// 数据接收线程
	static DWORD RecvThreadProc(LPVOID lpParam);

	CString GetStrByINT(int nData);

	void GetLocalIP(const char *pszIP, unsigned short uPort);

	void PrintLog(LPCSTR lpData);
	void SetCallbackEvent(int nEventCode);

	CString GetErrorCode(DWORD dwErrorCode);

private:
	CDebugSocket(void);

	static CDebugSocket		m_Instance;

	SOCKET					m_Socket;	// 服务端的监听socket
	SOCKET					m_SockConn;	// 服务端的连接socket
	bool					m_bConnct;
    CCSWrap					m_lockConnect;
	CCSWrap					m_lockSend;
 
	bool					m_bRecvThreadRun;

	HANDLE					m_ServerThreadHandle;
	HANDLE					m_RecvThreadHandle;

	RECV_CALLBACK			m_lpfnRecvMsg;		// 接收消息回调
	EVENT_CALLBACK			m_lpfnEvent;		// 事件回调
	PRINT_LOG_CALLBACK		m_lpfnLogPrint;		// 日志打印

	CString					m_strServerAddr;	// 服务器地址
	CString					m_strLocalIP;		// 获取本地IP 
};

