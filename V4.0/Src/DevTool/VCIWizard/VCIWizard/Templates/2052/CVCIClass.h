#if !defined(__VCI_INTERFACE_C[!output INTERFACE_NAME]_H_INCLUDED__)
#define __VCI_INTERFACE_C[!output INTERFACE_NAME]_H_INCLUDED__


#include "vcicomm.h"
[!if VCITYPE_OWM]
#include "vciowm.h"
[!else]
#include "I[!output INTERFACE_NAME].h"
[!endif]

#define INF_I[!output INTERFACE_NAME] \
{\
	"I[!output INTERFACE_NAME]",\
	"[!output INTERFACE_VERSION]",\
	THIS_REVISION_TYPE,\
	"{6F20ED82-A398-44CF-A45B-1E1906DA2C6A}",\
	"[!output INTERFACE_DESC]",\
	"[!output AUTH_NAME]",\
	__DATE__\
}
const TInterfaceInfo INTERFACE_INFO_I[!output INTERFACE_NAME] = INF_I[!output INTERFACE_NAME];

class C[!output INTERFACE_NAME] : public CVisualComponent
{
public:
	C[!output INTERFACE_NAME]();
	~C[!output INTERFACE_NAME]();

//////////////////////////////////////////////////////////////////////////
// 提供给接口的功能函数
//////////////////////////////////////////////////////////////////////////
public:
	//{{VCI_IMPLEMENT_DEFINE_BEGIN
[!if VCITYPE_INTERP]
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
[!endif]
[!if VCITYPE_PROJECT]
		PROJECT_INTERFACE_DEFINE
[!endif]
[!if VCITYPE_OWM]
	virtual int __stdcall SetDebugMode(BOOL bDebug);
	virtual int __stdcall SetLanguage(int nLanguage);
	virtual int __stdcall Init(LPCSTR lpcsParams);
	virtual int __stdcall Done(LPCSTR lpcsParams);
[!endif]
	//}}VCI_IMPLEMENT_DEFINE_END

	virtual int __stdcall InstallHandler(DWORD nEventID, LPVOID lpHandler, DWORD dwRefData);
	virtual int __stdcall UninstallHandler(DWORD nEventID, LPVOID lpHandler);
	virtual int __stdcall ProcessMessage(CVciMessage* pIn, CVciMessage* ppOut);

[!if VCITYPE_INTERP]
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

	BOOL RunProgramWithPipe(CString strExePath, CString strParam, int nOutputId = 1, BOOL bActiveOutput = FALSE);
	BOOL TerminatePipeProcess();
	BOOL ExecProcess(CString strExePath, CString strParam, BOOL bWaitEnd = FALSE);

// attribute
public:
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
[!endif]
[!if VCITYPE_POLICY]
	IAgent*		m_pIAgent;		// 策略管理器接口指针
[!endif]
//////////////////////////////////////////////////////////////////////////
// 要输出的功能接口
//////////////////////////////////////////////////////////////////////////
protected:
	// 接口定义
	BEGIN_INTERFACE_PART(VciControl, IVciControl)
		CONTROL_INTERFACE_DEFINE
	END_INTERFACE_PART(VciControl)
	EXPORT_CONTROL_INTERFACE_PART(VciControl)

	BEGIN_INTERFACE_PART([!output INTERFACE_NAME], I[!output INTERFACE_NAME])
[!if !VCITYPE_OWM]
		//{{ 注意：!!!这里是保留的重要函数，不可删除!!!
		// 获取接口的额外信息
		virtual LPVOID __stdcall GetExtraInfo(LPVOID lpRefData);
		//}} 注意：!!!这里是保留的重要函数，不可删除!!!
[!endif]
	//{{VCI_INTERFACE_PART_BEGIN
[!if VCITYPE_INTERP]
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
[!endif]
[!if VCITYPE_PROJECT]
		PROJECT_INTERFACE_DEFINE
[!endif]
[!if VCITYPE_OWM]
		OWM_INTERFACE_DEFINE
[!endif]
	//}}VCI_INTERFACE_PART_END
	END_INTERFACE_PART([!output INTERFACE_NAME])
	EXPORT_INTERFACE_PART([!output INTERFACE_NAME])
};

[!if VCITYPE_OWM]
// 声明OWM函数
[!if OWM_MENU]
int RegisterClientMenu(CMenu* pMenu, COWMMenuArray& aOWMMenus, int nOWMIdStart, CDocument* pDoc);
int UnregisterClientMenu(COWMMenuArray& aOWMMenus);
[!endif]
[!if OWM_TOOLBAR]
int RegisterClientToolBar(CWnd* pBar, COWMMenuArray& aOWMMenus, int nOWMIdStart, int nIndex);
int UnregisterClientToolBar(CWnd* pBar, COWMMenuArray& aOWMMenus, int nOWMIdStart);
[!endif]
[!if OWM_OPTION]
int RegisterOption(COWMOptionArray& aOWMOption);
[!endif]
int SetDebugMode(BOOL bDebug);
int CommandOWM(int nCmd, WPARAM wParam, LPARAM lParam);
int InitOWM();
int DoneOWM();
[!endif]

#endif // !defined(__VCI_INTERFACE_C[!output INTERFACE_NAME]_H_INCLUDED__)
