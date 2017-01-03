////////////////////////////////////////////////////////////////////////////
//	File:		Timer.cpp
//	Version:	1.0.0.0
//	Created:	2007-07-08
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	Timer implement
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlueUI.h"
#include "Timer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// 定时器任务对象
//////////////////////////////////////////////////////////////////////////
CTimerTask::CTimerTask()
{
	m_strTaskName	= "";
	m_strInterpId	= "";
	m_strInterpName	= "";
	m_strCommand	= "";
	m_lpInterpObj	= NULL;
	m_lpFunction	= NULL;
	m_nReleaseInterp= 0;
	m_nTaskType		= TIMER_TYPE_NORMAL;
	m_nSeconds		= 0;
	m_tStartTime	= CTime::GetCurrentTime();
	m_tEndTime		= m_tStartTime;
	m_strTrigger	= "";
	m_nExecCount	= 0;
	m_nCurTimer		= 0;
	m_nCurCount		= 0;
	m_nCurStatus	= 0;
}

CTimerTask::CTimerTask(CString strTaskName, CString strInterpId, CString strInterpName, CString strCommand)
{
	m_strTaskName	= strTaskName;
	m_strInterpId	= strInterpId;
	m_strInterpName	= strInterpName;
	m_strCommand	= strCommand;
	m_lpInterpObj	= theApp.CreateVciObject(strInterpId, strInterpName);
	m_lpFunction	= NULL;
	if(m_strInterpName != "")
	{
		m_nReleaseInterp= TIMER_INTERP_DONTRELEASE;
	}else
	{
		m_nReleaseInterp= TIMER_INTERP_REFRELEASE;
	}
	m_nTaskType		= TIMER_TYPE_NORMAL;
	m_nSeconds		= 1;
	m_tStartTime	= CTime::GetCurrentTime();
	m_tEndTime		= m_tStartTime;
	m_strTrigger	= "";
	m_nExecCount	= 1;
	m_nCurTimer		= 0;
	m_nCurCount		= 0;
	m_nCurStatus	= TIMER_STATE_STOP;
}

CTimerTask::CTimerTask(CString strTaskName, LPVOID lpFunction)
{
	m_strTaskName	= strTaskName;
	m_strInterpId	= "";
	m_strInterpName	= "";
	m_strCommand	= "";
	m_lpInterpObj	= NULL;
	m_lpFunction	= lpFunction;
	m_nReleaseInterp= TIMER_INTERP_REFRELEASE;
	m_nTaskType		= TIMER_TYPE_NORMAL;
	m_nSeconds		= 1;
	m_tStartTime	= CTime::GetCurrentTime();
	m_tEndTime		= m_tStartTime;
	m_strTrigger	= "";
	m_nExecCount	= 1;
	m_nCurTimer		= 0;
	m_nCurCount		= 0;
	m_nCurStatus	= TIMER_STATE_STOP;
}

CTimerTask::~CTimerTask()
{
	if(m_nReleaseInterp == TIMER_INTERP_FOUCERELEASE)
	{
		theApp.ReleaseObject(m_lpInterpObj, TRUE);
	}else
	if(m_nReleaseInterp == TIMER_INTERP_REFRELEASE)
	{
		theApp.ReleaseObject(m_lpInterpObj);
	}else
	if(m_nReleaseInterp == TIMER_TYPE_TRIGGER)
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// 设置起始时间
/////////////////////////////////////////////////////////////////////////////
void CTimerTask::SetStartTime(CTime tTime)
{
	m_tStartTime = tTime;
	m_nTaskType |= TIMER_TYPE_STARTTIME;
}

/////////////////////////////////////////////////////////////////////////////
// 设置结束时间
/////////////////////////////////////////////////////////////////////////////
void CTimerTask::SetEndTime(CTime tTime)
{
	m_tEndTime = tTime;
	m_nTaskType |= TIMER_TYPE_ENDTIME;
}

/////////////////////////////////////////////////////////////////////////////
// 设置触发条件
/////////////////////////////////////////////////////////////////////////////
void CTimerTask::SetTrigger(LPCTSTR lpszTrigger)
{
	m_strTrigger = lpszTrigger;
	m_nTaskType |= TIMER_TYPE_TRIGGER;
}

/////////////////////////////////////////////////////////////////////////////
// 设置执行次数
/////////////////////////////////////////////////////////////////////////////
void CTimerTask::SetExecCount(int nCount)
{
	m_nExecCount = nCount;
}

/////////////////////////////////////////////////////////////////////////////
// 检查任务状态
/////////////////////////////////////////////////////////////////////////////
int CTimerTask::CheckStatus()
{
	if(m_lpFunction != NULL)
	{
		// 函数类型的执行体
		return ((TIMERTASK_FUNC*)m_lpFunction)(this);
	}
	
	if((m_nCurStatus == TIMER_STATE_END) || (m_nCurStatus == TIMER_STATE_STOP))
	{
		return m_nCurStatus;
	}

	// 检查起始时间
	if((m_nTaskType & TIMER_TYPE_STARTTIME) != 0)
	{
		if(CTime::GetCurrentTime() < m_tStartTime)
		{
			m_nCurStatus = TIMER_STATE_UNACTIVE;
			return m_nCurStatus;
		}
	}

	// 检查结束时间
	if((m_nTaskType & TIMER_TYPE_ENDTIME) != 0)
	{
		if(CTime::GetCurrentTime() > m_tEndTime)
		{
			m_nCurStatus = TIMER_STATE_END;
			return m_nCurStatus;
		}
	}

	if(m_nCurStatus == TIMER_STATE_ACTIVE)
	{
		return m_nCurStatus;
	}

	// 检查触发器
	if((m_nTaskType & TIMER_TYPE_TRIGGER) != 0)
	{
		IInterp* pInterp = (IInterp*)m_lpInterpObj;
		if(pInterp)
		{
			pInterp->RunScriptCommand(m_strTrigger);
			if(pInterp->GetResult() == "true")
			{
				m_nCurStatus = TIMER_STATE_ACTIVE;
				return m_nCurStatus;
			}
		}
	}

	m_nCurStatus = TIMER_STATE_UNACTIVE;
	return m_nCurStatus;
}

/////////////////////////////////////////////////////////////////////////////
// 运行任务
/////////////////////////////////////////////////////////////////////////////
int CTimerTask::RunTask()
{
	IInterp* pInterp = (IInterp*)m_lpInterpObj;
	if((pInterp == NULL) || (m_strCommand == ""))
	{
		return -1;
	}

	if(m_strCommand.Find("<file>") != 0)
	{
		pInterp->RunScriptCommand(m_strCommand);
	}else
	{
		CString strScriptFile = m_strCommand;
		strScriptFile.Delete(0, 6);
		theApp.ProcessPath(strScriptFile);
		pInterp->RunScriptFile(strScriptFile);
	}

	if((m_nExecCount > 0) && (m_nCurCount < m_nExecCount))
	{
		m_nCurCount++;
		if(m_nCurCount == m_nExecCount)
		{
			m_nCurStatus = TIMER_STATE_END;
		}
	}

	return pInterp->GetErrorLine();
}
