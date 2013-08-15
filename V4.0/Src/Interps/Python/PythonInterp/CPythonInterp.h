#if !defined(__VCI_INTERFACE_CPythonInterp_H_INCLUDED__)
#define __VCI_INTERFACE_CPythonInterp_H_INCLUDED__



#include "vcicomm.h"
#include "IInterp.h"

#define INF_IInterp \
{\
	"IInterp",\
	"1.0.0.0",\
	THIS_REVISION_TYPE,\
	"{87AA1D66-6BC3-4C12-9459-E70C6DB08349}",\
	"Python Interp",\
	"Blueant",\
	__DATE__\
}
const TInterfaceInfo INTERFACE_INFO_IInterp = INF_IInterp;


// Python调试事件类型
enum{
	// 以下为Python调试器回调事件
	PDB_USER_CALL,		// 调用函数
	PDB_USER_LINE,		// 执行一行
	PDB_USER_RETURN,	// 函数返回
	PDB_USER_EXCEPTION,	// 发生异常

	// 以下为平台调试命令事件
	PDB_STOP,			// 中止
	PDB_NEXT,			// 执行到下一个断点
	PDB_GOTO,			// 执行到某个行断点或命名断点
	PDB_STEPINTO,		// 单步进入函数
	PDB_SETPOUT,		// 从函数返回
	PDB_STEP,			// 单步不进入函数
	PDB_END,			// 通知解释器文件调试结束,可以执行善后处理

	PDB_SETFILE = 20,	// 设置被调试的脚本文件
};


typedef struct _ts PyThreadState;
typedef struct _object PyObject;

////////////////////////////////add by lcmao
typedef struct _frame PyFrameObject;
/////////////////////////////////end by lcmao

class CPythonInterp : public CVisualComponent
{
public:
	CPythonInterp();
	~CPythonInterp();

	// tells if Python is available
	static bool IsAvail(const bool forceReload = false);
	
	static void InitPythonSupport(void);
	static void ClosePythonSupport(void);

	inline operator PyThreadState *(void) {return m_myTS;}

	// 多文件调试
	CStringArray* GetDebugScriptList() { return &m_asDebugScript; }
	int FindDebugFile(CString strFileName);
	int AddDebugFile(CString strFileName);
	BOOL ClearDebugFile();

//////////////////////////////////////////////////////////////////////////
// 提供给接口的功能函数
//////////////////////////////////////////////////////////////////////////
public:
	//{{VCI_IMPLEMENT_DEFINE_BEGIN
	virtual int __stdcall SetIPlatUI(LPVOID lpIPlatUI);// 设置平台UI接口指针
	virtual int __stdcall SetLibPath(LPCTSTR lpszPath);
	virtual int __stdcall IsInterpRunning();
	virtual int __stdcall SetScriptFile(LPCTSTR lpszPathName);
	virtual CString __stdcall GetScriptFile();
	virtual int __stdcall RunScriptFile(LPCTSTR lpszPathName);
	virtual int __stdcall RunScriptCommand(LPCTSTR lpszTclCmd);
	virtual int __stdcall StopScript();
	virtual LPVOID __stdcall GetInterp();
	virtual int __stdcall GetInterpID();
	virtual void __stdcall SetInterpName(LPCTSTR lpszInterptName);
	virtual CString __stdcall GetInterpName();
	virtual int __stdcall GetErrorLine();
	virtual CString __stdcall GetResult();
	virtual int __stdcall SetVar(LPCTSTR lpszVarName, LPCTSTR lpszVarValue);
	virtual CString __stdcall GetVar(LPCTSTR lpszVarName);
	virtual int __stdcall RegisterCommand(LPVOID pInterp, LPCTSTR lpszCmd, LPVOID lpFunc);
	virtual int __stdcall SetInterpParam(LPCTSTR lpszParamName, LPCTSTR lpszParamValue);
	virtual CString __stdcall GetInterpParam(LPCTSTR lpszParamName);
	virtual int __stdcall SetRunMode(int nRunMode);
	virtual int __stdcall GetRunMode();
	virtual int __stdcall SetRunState(int nRunState);
	virtual int __stdcall GetRunState();
	virtual int __stdcall GetCurStackLevel();
	virtual int __stdcall GetKeyWordList(LPCTSTR lpszType, CStringArray& asKeyWord);
	virtual int __stdcall SetDebugEvent(int nEvent, int nParam, LPCTSTR lpszParam);
	virtual int __stdcall SetBreakList(LPCTSTR lpszFile, CUIntArray& auBreaks);
	virtual int __stdcall GetBreakList(LPCTSTR lpszFile, CUIntArray& auBreaks);
	virtual int __stdcall GetEnableProfileAnalyse();
	virtual int __stdcall GetAvailableLineCount();
	virtual LPVOID __stdcall GetProfileMap();
	virtual int __stdcall MethodIsSupport(LPCTSTR lpszMethod);
	virtual int __stdcall InitialDebugMode(CString& strResult);
	virtual int __stdcall RefreshDebugWindow(LPCTSTR lpszWindow, BOOL bActive, CString& strResult);
	virtual int __stdcall CallMethod(LPCTSTR lpszMethod, CStringArray& asParam);
	virtual int __stdcall Compile(LPCTSTR lpszSrcFile, CString& strDestFile, LPCTSTR lpszParams);
	virtual int __stdcall Build(LPCTSTR lpszSrcFile, CString& strDestFile, LPCTSTR lpszParams);
	//}}VCI_IMPLEMENT_DEFINE_END

	virtual int __stdcall InstallHandler(DWORD nEventID, LPVOID lpHandler, DWORD dwRefData);
	virtual int __stdcall UninstallHandler(DWORD nEventID, LPVOID lpHandler);

	void SetHide(BOOL bHide) { m_bHide = bHide; }
	BOOL GetHide() { return m_bHide; }
	void SetRunLevel(int nLevel) { m_nRunLevel = nLevel; }
	int	 GetRunLevel() { return m_nRunLevel; }
	void SetStopSignal(BOOL bStop) { m_bStopSignal = bStop; }
	BOOL GetStopSignal() { return m_bStopSignal; }
	void SetBreakSignal(BOOL bBreak);
	BOOL GetBreakSignal() { return m_bBreakSignal; }
	void SetBreakMode(int nMode) { m_nBreakMode = nMode; }
	int  GetBreakMode() { return m_nBreakMode; }
	void SetBreakLineNo(int nLineNo) { m_nBreakLineNo = nLineNo; }
	int  GetBreakLineNo() { return m_nBreakLineNo; }
	void SetBreakName(LPCTSTR lpszName) { m_strBreakName = lpszName; }
	CString GetBreakName() { return m_strBreakName; }
	void SetBreakLevel(int nLevel) { m_nBreakLevel = nLevel; }
	int  GetBreakLevel() { return m_nBreakLevel; }
	void SetStackLevel(int nLevel) { m_nStackLevel = nLevel; }
	int	 GetStackLevel() { return m_nStackLevel; }

	//////////////////////////////////////////add by lcmao
	//添加打开和关闭文件的函数
public:
	static void* GetFuncAddress(const char* funcName);
	FILE* OpenFile(const char* fileName, const char* mode);
	int CloseFile(FILE* );
	//MyPy_tracefunc回调这个函数
	int PythonTrace(PyFrameObject *frame, int what, PyObject *arg);
	int PyTraceLine(PyFrameObject *frame);
	//获取PyFrameObject所在的层次，底层为1，frame是null是返回0
	int GetStackLevel(PyFrameObject *frame);
	//断点，这个函数会阻塞，直到下一个指令到来
	int BreakHear(PyFrameObject *frame, int stackLevel, CString& fileName, int line);
	//输出当前帧可以获取到的变量和值,如果acquireLock为true，此函数将会获取python的线程锁
	//如果在已经占有线程锁的时候调用这个函数，一定要设置acquireLock为FALSE，否则会死锁
	void OutPutVars(int outputWin, PyFrameObject *frame,BOOLEAN acquireLock = TRUE);
	//在新的解析器上面运行一条命令
	int RunScriptCommandWithNewInt(LPCTSTR lpszCmd);

	static CString m_strPythonHome;
	static HANDLE m_hStdoutPipeRead;
	static HANDLE m_hStdoutPipeWrite;
	static HANDLE m_hStderrPipeRead;
	static HANDLE m_hStderrPipeWrite;
	static UINT __cdecl RedirectOuputThread(LPVOID parameter);
	static UINT __cdecl RedirectErrorThread(LPVOID parameter);
	//获取python对象的属性
	static PyObject* GetPythonAttr(PyObject* obj, char* attrName);
	//调试函数，提供给python回调
	static int __cdecl MyPy_tracefunc(PyObject *obj, PyFrameObject *frame, int what, PyObject *arg);

	static IPlatUI*	m_pCurPlatUI;  //当前的平台UI
	static PyObject* m_stdout;     //重定向输出
	static PyObject* m_stderr;     //重定向输出

	PyObject* m_debugArg;  //传给python用于debug的参数
	HANDLE m_hDebugEvent;     //调试事件，用于断点，调试模式下面才会产生
	int m_nCurCommand;     //当前收到的调试指令，IDB_STEPINTO，IDB_NEXT等
	PyFrameObject* m_currentframe;	// 当前调试时候的frame

	int			m_nOutputVar;      // 变量窗口ID
	int			m_nOutputObject;   // 对象窗口ID
	int			m_nOutputFunc;	   // 函数窗口ID
	int			m_nOutputStack;    // 堆栈窗口ID
	/////////////////////////////////////////end add

public:
	void DebugCallback(int nEvent, int nLine);

	BOOL RunProgramWithPipe(CString strExePath, CString strParam, int nOutputId = 1, BOOL bActiveOutput = FALSE);
	BOOL TerminatePipeProcess();
	BOOL ExecProcess(CString strExePath, CString strParam, BOOL bWaitEnd = FALSE);

	void SetVarInPlatModule(CString strVarName, CString strVarValue);
	CString GetVarInPlatModule(CString strVarName);

	int  RefreshVarWindow();
	int  RefreshStackWindow();

	int	 SelectSetupType();

// attribute
public:
	PyThreadState * m_myTS;        //创建解析器时第一个线程状态（单线程情况下只有一个）
	//static PyThreadState *m_mainInterpreter;
	PyObject*	pyPlatModule;	// _platform模块的对象指针
	CStringArray m_asDebugScript;	// 调试过程中跟踪进去的文件

	//Tcl_Interp*	pInterp;		// 解释器
	int			m_nInterpID;	// 解释器ID(解释器唯一标识)
	IPlatUI*	pIPlatUI;		// 平台界面接口
	DWORD		m_dwRunThreadId;// 脚本运行的线程ID
	HANDLE		m_hPipeProcess;	// 管道进程句柄
	BOOL		m_bPipeProcessTerminated;	// 管道进程是否被终止的
	BOOL		m_bHide;		// 解释器是否隐含(在导航树中不可见)
	CString		m_strInterpName;// 解释名(用于命名解释器)
	CString		m_strScriptFile;// 脚本文件
	int			m_nRunMode;		// 脚本运行模式
	int			m_nRunState;	// 脚本运行状态
	int			m_nRunLevel;	// 脚本运行级别(权限)
	BOOL		m_bStopSignal;	// 脚本中止信号
	BOOL		m_bBreakSignal;	// 断点激活信号
	int			m_nEvalResult;	// 脚本运行结果
	int			m_nBreakMode;	// 当前断点模式
	int			m_nBreakLineNo;	// 断点行号
	CString		m_strBreakName;	// 命名断点名
	int			m_nBreakLevel;	// 当前堆栈深度
	CUIntArray	m_auBreakList;	// 断点列表
	int			m_nStackLevel;	// 保存的堆栈层数
	BOOL		m_bProfileEnable;	// 是否允许进行覆盖率分析
	int			m_nTDLineCount;	// 加td标记的行数
	CMap<int,int,int,int> m_ProfileMap;	// 存储覆盖行的哈希表
	INTERP_EXIT_FUNC*	m_lpfnExit;	// 退出回调函数
	INTERP_DEBUG_FUNC*	m_lpfnDebug;// 调试回调函数

	CString		strErrorInfo;	// 错误信息
	CString		strStdOut;		// 标准输出
	CString		strIntervalOut;	// 内部输出信息
//////////////////////////////////////////////////////////////////////////
// 要输出的功能接口
//////////////////////////////////////////////////////////////////////////
protected:
	// 接口定义
	BEGIN_INTERFACE_PART(VciControl, IVciControl)
		CONTROL_INTERFACE_DEFINE
	END_INTERFACE_PART(VciControl)
	EXPORT_CONTROL_INTERFACE_PART(VciControl)

	BEGIN_INTERFACE_PART(PythonInterp, IInterp)
		//{{ 注意：!!!这里是保留的重要函数，不可删除!!!
		// 获取接口的额外信息
		virtual LPVOID __stdcall GetExtraInfo(LPVOID lpRefData);
		//}} 注意：!!!这里是保留的重要函数，不可删除!!!

	//{{VCI_INTERFACE_PART_BEGIN
		virtual int __stdcall SetIPlatUI(LPVOID lpIPlatUI);// 设置平台UI接口指针
		virtual int __stdcall SetLibPath(LPCTSTR lpszPath);
		virtual int __stdcall IsInterpRunning();
		virtual int __stdcall SetScriptFile(LPCTSTR lpszPathName);
		virtual CString __stdcall GetScriptFile();
		virtual int __stdcall RunScriptFile(LPCTSTR lpszPathName);
		virtual int __stdcall RunScriptCommand(LPCTSTR lpszTclCmd);
		virtual int __stdcall StopScript();
		virtual LPVOID __stdcall GetInterp();
		virtual int __stdcall GetInterpID();
		virtual void __stdcall SetInterpName(LPCTSTR lpszInterptName);
		virtual CString __stdcall GetInterpName();
		virtual int __stdcall GetErrorLine();
		virtual CString __stdcall GetResult();
		virtual int __stdcall SetVar(LPCTSTR lpszVarName, LPCTSTR lpszVarValue);
		virtual CString __stdcall GetVar(LPCTSTR lpszVarName);
		virtual int __stdcall RegisterCommand(LPVOID pInterp, LPCTSTR lpszCmd, LPVOID lpFunc);
		virtual int __stdcall SetInterpParam(LPCTSTR lpszParamName, LPCTSTR lpszParamValue);
		virtual CString __stdcall GetInterpParam(LPCTSTR lpszParamName);
		virtual int __stdcall SetRunMode(int nRunMode);
		virtual int __stdcall GetRunMode();
		virtual int __stdcall SetRunState(int nRunState);
		virtual int __stdcall GetRunState();
		virtual int __stdcall GetCurStackLevel();
		virtual int __stdcall GetKeyWordList(LPCTSTR lpszType, CStringArray& asKeyWord);
		virtual int __stdcall SetDebugEvent(int nEvent, int nParam, LPCTSTR lpszParam);
		virtual int __stdcall SetBreakList(LPCTSTR lpszFile, CUIntArray& auBreaks);
		virtual int __stdcall GetBreakList(LPCTSTR lpszFile, CUIntArray& auBreaks);
		virtual int __stdcall GetEnableProfileAnalyse();
		virtual int __stdcall GetAvailableLineCount();
		virtual LPVOID __stdcall GetProfileMap();
		virtual int __stdcall MethodIsSupport(LPCTSTR lpszMethod);
		virtual int __stdcall InitialDebugMode(CString& strResult);
		virtual int __stdcall RefreshDebugWindow(LPCTSTR lpszWindow, BOOL bActive, CString& strResult);
		virtual int __stdcall CallMethod(LPCTSTR lpszMethod, CStringArray& asParam);
		virtual int __stdcall Compile(LPCTSTR lpszSrcFile, CString& strDestFile, LPCTSTR lpszParams);
		virtual int __stdcall Build(LPCTSTR lpszSrcFile, CString& strDestFile, LPCTSTR lpszParams);
	//}}VCI_INTERFACE_PART_END
	END_INTERFACE_PART(PythonInterp)
	EXPORT_INTERFACE_PART(PythonInterp)
};

// 解释器操作函数
CPtrList* GetPythonInterpList();
int  OpenPythonInterp(LPCTSTR lpszInterptName);
CPythonInterp* GetPythonInterp(LPCTSTR lpszInterptName);
CPythonInterp* GetPythonInterp(int nInterpID);
CPythonInterp* GetPythonInterp(LPVOID lpPythonInterp);
BOOL ClosePythonInterp(LPCTSTR lpszInterptName);
BOOL ClosePythonInterp(int nInterpID);
void CloseAllPythonInterp();

#endif // !defined(__VCI_INTERFACE_CPythonInterp_H_INCLUDED__)
