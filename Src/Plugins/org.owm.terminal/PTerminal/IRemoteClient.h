#if !defined(__PCOM_INTERFACE_IRemoteClient_H_INCLUDED__)
#define __PCOM_INTERFACE_IRemoteClient_H_INCLUDED__

#include "RcResult.h"

#define IID_IRemoteClient     "IRemoteClient"

#define    RC_SOCKET_SERVER              1116 // 远程服务端口

interface IRemoteClient : public IUnknown
{
	//{{ 注意：!!!这里是保留的重要函数，不可删除!!!
	// 获取接口的额外信息
	virtual LPVOID __stdcall GetExtraInfo(LPVOID lpRefData) = 0;
	//}} 注意：!!!这里是保留的重要函数，不可删除!!!

	//设置最大网络处理时间
	virtual STDMETHODIMP_(void) SetMaxProcTime(DWORD dwMaxProcTime) = 0;

	//连接并登录服务器
	virtual STDMETHODIMP_(ERCResult) ConnectAndLogin(CString strServerAddr, CString strUser, CString strPassword, DWORD dwPort = RC_SOCKET_SERVER) = 0;
	//断开服务器连接
	virtual STDMETHODIMP_(ERCResult) DisconnectServer(void) = 0;

	//获取平台版本
	virtual STDMETHODIMP_(ERCResult) ProcessGetPlatVersion(CString& strReturn) = 0;
	//设置平台升级新版本
	virtual STDMETHODIMP_(ERCResult) ProcessSetPlatNewVersion(CString strVer, CString strSubVer) = 0;
	//执行TCL命令
	virtual STDMETHODIMP_(ERCResult) ProcessTclCommand(CString strCommand, CString& strReturn) = 0;
	//创建解释器
	virtual STDMETHODIMP_(ERCResult) ProcessOpenInterpreter(CString strName, int& nInterpreterID) = 0;
	//关闭脚本解释器
	virtual STDMETHODIMP_(ERCResult) ProcessCloseInterpreter(int nInterpreterID) = 0;
	//获取解释器ID
	virtual STDMETHODIMP_(ERCResult) ProcessGetInterpreterID(CString strName, int& nInterpreterID) = 0;
	//执行脚本文件
	virtual STDMETHODIMP_(ERCResult) ProcessEvalFile(int nInterpreterID, CString strFileName, CString& strReturn) = 0;
	//执行脚本命令
	virtual STDMETHODIMP_(ERCResult) ProcessEvalCommand(int nInterpreterID, CString strCommand, CString& strReturn) = 0;
	//获取错误行号
	virtual STDMETHODIMP_(ERCResult) ProcessGetErrorLine(int nInterpreterID, int& nErrorLine) = 0;

};

#endif // !defined(__PCOM_INTERFACE_IRemoteClient_H_INCLUDED__)
