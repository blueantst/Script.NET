////////////////////////////////////////////////////////////////////////////
//	File:		IPlatUI.h
//	Version:	1.0.0.0
//	Created:	2008-07-08
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	Interface define of VCI message queue
////////////////////////////////////////////////////////////////////////////
#if !defined(__VCI_INTERFACE_IMessageQueue_H_INCLUDED__)
#define __VCI_INTERFACE_IMessageQueue_H_INCLUDED__


#define IID_IMessageQueue "IMessageQueue"

interface IMessageQueue : public IUnknown
{
	//{{ 注意：!!!这里是保留的重要函数，不可删除!!!
	// 获取接口的额外信息
	virtual LPVOID __stdcall GetExtraInfo(LPVOID lpRefData) = 0;
	//}} 注意：!!!这里是保留的重要函数，不可删除!!!

	//{{VCI_INTERFACE_BEGIN
	// 设置当前进程的进程标识
	virtual int __stdcall SetAppProcessId(LPCTSTR lpszProcess) = 0;
	// 将VCI消息插入对应组件的消息队列
	virtual int __stdcall PostMessage(CVciMessage* pIn, LPCTSTR lpszQueueName) = 0;
	//}}VCI_INTERFACE_END
};


#endif // !defined(__VCI_INTERFACE_IMessageQueue_H_INCLUDED__)
