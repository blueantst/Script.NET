#ifndef ___SYSOBJ_H__
#define ___SYSOBJ_H__

class CAutoLockTrait
{
public:
	virtual void Lock() = 0;
	virtual void Unlock() = 0;
};

class CCSWrap : public CAutoLockTrait
{
public:
	CCSWrap()
	{
		m_bInited = TRUE;
		m_bInited = ::InitializeCriticalSectionAndSpinCount(&m_cs, 0x80000400);
	}
	~CCSWrap()
	{
		if (m_bInited)
			::DeleteCriticalSection(&m_cs);
	}

	void Lock()
	{
		if (m_bInited)
			EnterCriticalSection(&m_cs);
	}
	void Unlock()
	{
		if (m_bInited)
			LeaveCriticalSection(&m_cs);
	}

private:
	CRITICAL_SECTION m_cs;
	BOOL m_bInited;
};

class CMutexWrap : public CAutoLockTrait
{
public:
    CMutexWrap(LPCTSTR pszName)
    {
        m_hMutex = CreateMutex(NULL, TRUE, pszName);
    }
    ~CMutexWrap()
    {
        if (NULL != m_hMutex)
        {
            CloseHandle(m_hMutex);
            m_hMutex = NULL;
        }
    }

    void Lock()
    {
        WaitForSingleObject(m_hMutex, INFINITE);
    }
    void Unlock()
    {
        ReleaseMutex(m_hMutex);
    }

private:
    HANDLE m_hMutex;
};

//自动锁对象
template<class T>
class CAutoLockWrap
{
public:
	CAutoLockWrap(T& obj) : m_sysobj(obj)
	{
		m_sysobj.Lock();
	}
	~CAutoLockWrap()
	{
		m_sysobj.Unlock();
	}

private:
	T& m_sysobj;
};

#endif //
