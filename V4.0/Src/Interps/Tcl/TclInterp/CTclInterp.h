#if !defined(__VCI_INTERFACE_CTclInterp_H_INCLUDED__)
#define __VCI_INTERFACE_CTclInterp_H_INCLUDED__


#include "vcicomm.h"
#include "IInterp.h"

#define INF_IInterp \
{\
	"IInterp",\
	"1.0.0.0",\
	THIS_REVISION_TYPE,\
	"{72423E82-311F-4AD6-B791-9AD110B9F6C0}",\
	"Tcl Interp",\
	"Blueant",\
	__DATE__\
}
const TInterfaceInfo INTERFACE_INFO_IInterp = INF_IInterp;

// 多文件调试的文件类
class CDebugFile
{
public:
	CDebugFile();
	~CDebugFile();

	// 拷贝构造函数
	CDebugFile(const CDebugFile& other);
	// 重载赋值运算符
	CDebugFile& operator = (const CDebugFile& other);

// function
public:
	int ProcessRunBuffer();
	int RunScriptBuffer(Tcl_Interp*	pInterp);

// attribute
public:
	int				m_nFileIndex;	// 文件索引
	CString			m_strFile;		// 文件名
	char*			m_pRunBuffer;	// 脚本运行缓冲区
	int				m_nTDLineCount;	// 加td标记的行数
	CMap<int,int,int,int> m_ProfileMap;	// 存储覆盖行的哈希表
};

typedef CArray<CDebugFile, CDebugFile&> CDebugFileArray;


// Tcl解释器封装
class CTclInterp : public CVisualComponent
{
public:
	CTclInterp();
	~CTclInterp();

	virtual int Exit();

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

	int InitTclBaseInterp();
	int PreScript();
	BOOL CheckIsTkFile();
	BOOL CheckIsTkvFile();
	int ProcessRunBuffer();
	int RunScriptBuffer();
	void TkMainLoop();
	void TkDestroyWindow();

	void DebugCallback(int nEvent, int nLine, CString strFile = "");

	int Tcl_RegisterCmd(Tcl_Interp* pInterp);

	int RunCommand(CString strCmd);
	int RunCommandEx(CString strCmd, int nFlag);

	BOOL RunProgramWithPipe(CString strExePath, CString strParam, int nOutputId = 1, BOOL bActiveOutput = FALSE);
	BOOL TerminatePipeProcess();
	BOOL ExecProcess(CString strExePath, CString strParam, BOOL bWaitEnd = FALSE);

	int  GetPackageRequire(LPCTSTR lpszFile, CStringArray& asPackage);
	BOOL CheckPackageProvider(LPCTSTR lpszIndexFile, CStringArray& asPackage);
	BOOL SelectLibPath(CStringArray& asLibPath, CStringArray& asPackage);

	virtual int __stdcall Init(LPCSTR lpcsParams);
	virtual int __stdcall Done(LPCSTR lpcsParams);
	virtual int __stdcall InstallHandler(DWORD nEventID, LPVOID lpHandler, DWORD dwRefData);
	virtual int __stdcall UninstallHandler(DWORD nEventID, LPVOID lpHandler);

// attribute
public:
	Tcl_Interp*	pInterp;		// 解释器
	int			m_nInterpID;	// 解释器ID(解释器唯一标识)
	IPlatUI*	pIPlatUI;		// 平台界面接口
	CString		m_strAppProcess;// 平台进程名
	DWORD		m_dwRunThreadId;// 脚本运行的线程ID
	void*		m_TclNotifier;	// 
	BOOL		m_nSupportTk;	// 是否支持TK(0:不支持,1:支持(自动判断),2:强制支持,不管系统设置情况)
	BOOL		m_bIsTkFile;	// 是否是Tk脚本
	BOOL		m_bHide;		// 解释器是否隐含(在导航树中不可见)
	CString		m_strInterpName;// 解释名(用于命名解释器)
	CString		m_strTclLibPath;// Tcl库路径
	CString		m_strScriptFile;// 脚本文件
	char*		m_pRunBuffer;	// 脚本运行缓冲区
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
	HANDLE		m_hPipeProcess;	// 管道进程句柄
	BOOL		m_bPipeProcessTerminated;	// 管道进程是否被终止的
	CDebugFileArray	m_aDebugFile;	// 多文件调试的文件信息数组
	BOOL		m_bMultiDebugKeepSelect;	// 多文件调试时候是否保持选择
	BOOL		m_bMultiDebugLoadSource;	// 多文件调试时候是否加载调试文件

	int			m_nOutputVar;      // 变量窗口ID
	int			m_nOutputObject;   // 对象窗口ID
	int			m_nOutputFunc;	   // 函数窗口ID
	int			m_nOutputStack;    // 堆栈窗口ID

//////////////////////////////////////////////////////////////////////////
// 要输出的功能接口
//////////////////////////////////////////////////////////////////////////
protected:
	// 接口定义
	BEGIN_INTERFACE_PART(VciControl, IVciControl)
		CONTROL_INTERFACE_DEFINE
	END_INTERFACE_PART(VciControl)
	EXPORT_CONTROL_INTERFACE_PART(VciControl)

	BEGIN_INTERFACE_PART(TclInterp, IInterp)
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
		virtual int __stdcall Compile(LPCTSTR lpszSrcFile, CString& strDestFil, LPCTSTR lpszParams);
		virtual int __stdcall Build(LPCTSTR lpszSrcFile, CString& strDestFile, LPCTSTR lpszParams);
	//}}VCI_INTERFACE_PART_END
	END_INTERFACE_PART(TclInterp)
	EXPORT_INTERFACE_PART(TclInterp)
};

// UTF-8和普通字符串之间的转换函数
void ConvertResultToUTF(Tcl_Interp* interp);
CString ConvertStringToUTF(CString strParam);
void ConvertUTFToString(int argc, char* argv[]);
CString ConvertUTFParam(CString strParam);
Tcl_Obj* CreateTclList(Tcl_Interp* interp, CStringArray& asItem);


// 解释器操作函数
CPtrList* GetTclInterpList();
int  OpenTclInterp(LPCTSTR lpszInterptName, BOOL bSupportTk=FALSE);
CTclInterp* GetTclInterp(LPCTSTR lpszInterptName);
CTclInterp* GetTclInterp(int nInterpID);
CTclInterp* GetTclInterp(LPVOID lpTclInterp);
BOOL CloseTclInterp(LPCTSTR lpszInterptName);
BOOL CloseTclInterp(int nInterpID);
void CloseAllTclInterp();

// 初始化扩展命令
int TclExCmd_Init(Tcl_Interp *interp);

// TclWorkTree扩展命令
int TclWorkTree_Init(Tcl_Interp *interp);

#endif // !defined(__VCI_INTERFACE_CTclInterp_H_INCLUDED__)
