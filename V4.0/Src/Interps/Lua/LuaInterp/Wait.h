////////////////////////////////////////////////////////////////////////////
//	File:		Temsp.h
//	Version:	1.0.0.0
//	Created:	6-20-2002
//
//	Author:		xieyongfang
//	E-mail:		script.net@gmail.com
//
//	Comm define of wait object and wait list.
////////////////////////////////////////////////////////////////////////////
#ifndef __WAIT_H_
#define __WAIT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CWait  
{
private:
	BOOL	m_bWait;	// 是否处于Wait状态
	int		m_nID;		// Wait对象ID
//	DWORD	m_dwThreadID;// 记录线程ID
//	HANDLE	m_hWait;	// 等待事件量句柄
	
public:
	CWait(int nID = 0);
	virtual ~CWait();

public:
	void Awake();						// 唤醒等待
	int  Wait(DWORD dwTimeOut = 0);		//等待指定时间
	BOOL IsWaiting() { return m_bWait; };	//是否正在等待
	int  GetWaitID() { return m_nID; }	// 获取对象ID
};


void DeleteAllWaitObject();
CWait* CreateWaitObject(int nID = 0);
BOOL DeleteWaitObject(int nID);
BOOL AwakeWaitObject(int nID);
CWait* GetWaitObject(int nID);


#endif	// __WAIT_H_
