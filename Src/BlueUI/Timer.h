////////////////////////////////////////////////////////////////////////////
//	File:		Timer.h
//	Version:	1.0.0.0
//	Created:	2007-07-08
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	Timer Interface
////////////////////////////////////////////////////////////////////////////
#ifndef __TIMER_H_
#define __TIMER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 定时器状态
enum {
	TIMER_STATE_STOP,		// 未启动
	TIMER_STATE_UNACTIVE,	// 未激活
	TIMER_STATE_ACTIVE,		// 已激活
	TIMER_STATE_END,		// 已结束
};

// 解释器是否释放
enum {
	TIMER_INTERP_DONTRELEASE,	// 不释放
	TIMER_INTERP_REFRELEASE,	// 按引用计数释放
	TIMER_INTERP_FOUCERELEASE,	// 强制释放
};

// 任务类型
#define TIMER_TYPE_NORMAL		0x00000000	// 普通任务
#define TIMER_TYPE_STARTTIME	0x00000001	// 有起始时间
#define TIMER_TYPE_ENDTIME		0x00000002	// 有结束时间
#define TIMER_TYPE_TRIGGER		0x00000004	// 有触发器

//
// 定时器任务对象
//
class CTimerTask : public CObject
{
public:
	CTimerTask();
	CTimerTask(CString strTaskName, CString strInterpId, CString strInterpName, CString strCommand);
	CTimerTask(CString strTaskName, LPVOID lpFunction);
	~CTimerTask();

	CTimerTask(const CTimerTask& other)
	{
		m_strTaskName	= other.m_strTaskName;
		m_strInterpId	= other.m_strInterpId;
		m_strInterpName	= other.m_strInterpName;
		m_strCommand	= other.m_strCommand;
		m_lpInterpObj	= other.m_lpInterpObj;
		m_lpFunction	= other.m_lpFunction;
		m_nReleaseInterp= other.m_nReleaseInterp;
		m_nTaskType		= other.m_nTaskType;
		m_nSeconds		= other.m_nSeconds;
		m_tStartTime	= other.m_tStartTime;
		m_tEndTime		= other.m_tEndTime;
		m_strTrigger	= other.m_strTrigger;
		m_nExecCount	= other.m_nExecCount;
		m_nCurTimer		= other.m_nCurTimer;
		m_nCurCount		= other.m_nCurCount;
		m_nCurStatus	= other.m_nCurStatus;
	}

	CTimerTask& operator = (const CTimerTask& other)
	{
		m_strTaskName	= other.m_strTaskName;
		m_strInterpId	= other.m_strInterpId;
		m_strInterpName	= other.m_strInterpName;
		m_strCommand	= other.m_strCommand;
		m_lpInterpObj	= other.m_lpInterpObj;
		m_lpFunction	= other.m_lpFunction;
		m_nReleaseInterp= other.m_nReleaseInterp;
		m_nTaskType		= other.m_nTaskType;
		m_nSeconds		= other.m_nSeconds;
		m_tStartTime	= other.m_tStartTime;
		m_tEndTime		= other.m_tEndTime;
		m_strTrigger	= other.m_strTrigger;
		m_nExecCount	= other.m_nExecCount;
		m_nCurTimer		= other.m_nCurTimer;
		m_nCurCount		= other.m_nCurCount;
		m_nCurStatus	= other.m_nCurStatus;
		return *this;
	}

	void StartTask() {m_nCurStatus = TIMER_STATE_UNACTIVE;}
	void SetTaskType(int nType) {m_nTaskType = nType;}
	void SetTaskSeconds(int nSeconds) {m_nSeconds = nSeconds;}
	void SetStartTime(CTime tTime);
	void SetEndTime(CTime tTime);
	void SetTrigger(LPCTSTR lpszTrigger);
	void SetExecCount(int nCount);
	int GetStatus() {return m_nCurStatus;}
	int CheckStatus();		// 检查任务状态
	int	RunTask();			// 运行任务

public:
	CString	m_strTaskName;	// 任务名
	CString	m_strInterpId;	// 解释器ID
	CString	m_strInterpName;// 解释器实例名
	CString	m_strCommand;	// 任务执行的命令(如果以<file>开头,则表示执行文件)
	LPVOID	m_lpInterpObj;	// 解释器对象指针
	LPVOID	m_lpFunction;	// 任务执行函数指针
	int		m_nReleaseInterp;// 是否释放解释器(0=不释放,1=按引用计数,2=强制释放)
	int		m_nTaskType;	// 任务类型
	int		m_nSeconds;		// 任务秒数
	CTime	m_tStartTime;	// 任务开始时间
	CTime	m_tEndTime;		// 任务结束时间
	CString	m_strTrigger;	// 任务触发条件
	int		m_nExecCount;	// 执行次数
	int		m_nCurTimer;	// 当前计数
	int		m_nCurCount;	// 当前次数
	int		m_nCurStatus;	// 当前状态(未启动,已启动,已结束)
};

typedef int (TIMERTASK_FUNC)(CTimerTask* pTimerTask);


#endif //__TIMER_H_

