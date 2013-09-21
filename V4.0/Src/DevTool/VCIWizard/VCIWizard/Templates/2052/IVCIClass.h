#if !defined(__VCI_INTERFACE_I[!output INTERFACE_NAME]_H_INCLUDED__)
#define __VCI_INTERFACE_I[!output INTERFACE_NAME]_H_INCLUDED__


[!if VCITYPE_PROJECT]
// 返回结果
enum{
	trpOk,					// 正常
	trpFail,				// 打开工程失败
	trpCancel,				// 操作取消
	trpWorking,				// 工程正在运行
};

// 工程状态状态
enum{
	PROJECT_CLOSE,			// 关闭
	PROJECT_IDLE,			// 空闲
	PROJECT_WORKING,		// 工作
};
[!endif]

#define IID_I[!output INTERFACE_NAME] "I[!output INTERFACE_NAME]"

interface I[!output INTERFACE_NAME] : public IUnknown
{
	//{{ 注意：!!!这里是保留的重要函数，不可删除!!!
	// 获取接口的额外信息
	virtual LPVOID __stdcall GetExtraInfo(LPVOID lpRefData) = 0;
	//}} 注意：!!!这里是保留的重要函数，不可删除!!!

	//{{VCI_INTERFACE_BEGIN
[!if VCITYPE_INTERP]
	// 设置平台UI接口指针
	virtual int __stdcall SetIPlatUI(LPVOID lpIPlatUI) = 0;
	// 设置库路径
	virtual int __stdcall SetLibPath(LPCTSTR lpszPath) = 0;
	// 解释器是否在运行
	virtual int __stdcall IsInterpRunning() = 0;
	// 设置脚本文件名
	virtual int __stdcall SetScriptFile(LPCTSTR lpszPathName) = 0;
	// 获取脚本文件名
	virtual CString __stdcall GetScriptFile() = 0;
	// 运行脚本文件
	virtual int __stdcall RunScriptFile(LPCTSTR lpszPathName) = 0;
	// 执行脚本命令
	virtual int __stdcall RunScriptCommand(LPCTSTR lpszTclCmd) = 0;
	// 停止脚本运行
	virtual int __stdcall StopScript() = 0;
	// 获取解释器指针
	virtual LPVOID __stdcall GetInterp() = 0;
	// 获取解释器ID
	virtual int __stdcall GetInterpID() = 0;
	// 设置解释器名
	virtual void __stdcall SetInterpName(LPCTSTR lpszInterptName) = 0;
	// 获取解释器名
	virtual CString __stdcall GetInterpName() = 0;
	// 获取错误行号
	virtual int __stdcall GetErrorLine() = 0;
	// 获取结果信息
	virtual CString __stdcall GetResult() = 0;
	// 设置变量值
	virtual int __stdcall SetVar(LPCTSTR lpszVarName, LPCTSTR lpszVarValue) = 0;
	// 获取变量值
	virtual CString __stdcall GetVar(LPCTSTR lpszVarName) = 0;
	// 注册扩展命令
	virtual int __stdcall RegisterCommand(LPVOID pInterp, LPCTSTR lpszCmd, LPVOID lpFunc) = 0;
	// 设置扩展参数
	virtual int __stdcall SetInterpParam(LPCTSTR lpszParamName, LPCTSTR lpszParamValue) = 0;
	// 获取扩展参数
	virtual CString __stdcall GetInterpParam(LPCTSTR lpszParamName) = 0;
	// 设置运行模式
	virtual int __stdcall SetRunMode(int nRunMode) = 0;
	// 获取运行模式
	virtual int __stdcall GetRunMode() = 0;
	// 设置运行状态
	virtual int __stdcall SetRunState(int nRunState) = 0;
	// 获取运行状态
	virtual int __stdcall GetRunState() = 0;
	// 获取当前堆栈层数
	virtual int __stdcall GetCurStackLevel() = 0;
	// 获取关键字列表
	virtual int __stdcall GetKeyWordList(LPCTSTR lpszType, CStringArray& asKeyWord) = 0;
	// 设置调试事件
	virtual int __stdcall SetDebugEvent(int nEvent, int nParam, LPCTSTR lpszParam) = 0;
	// 设置断点列表
	virtual int __stdcall SetBreakList(LPCTSTR lpszFile, CUIntArray& auBreaks) = 0;
	// 获取断点列表
	virtual int __stdcall GetBreakList(LPCTSTR lpszFile, CUIntArray& auBreaks) = 0;
	// 获取是否允许覆盖率分析
	virtual int __stdcall GetEnableProfileAnalyse() = 0;
	// 获取有效代码行数
	virtual int __stdcall GetAvailableLineCount() = 0;
	// 获取代码运行覆盖表
	virtual LPVOID __stdcall GetProfileMap() = 0;
	// 判断指定方法是否被支持
	virtual int __stdcall MethodIsSupport(LPCTSTR lpszMethod) = 0;
	// 初始化调试模式
	virtual int __stdcall InitialDebugMode(CString& strResult) = 0;
	// 更新调试窗口信息
	virtual int __stdcall RefreshDebugWindow(LPCTSTR lpszWindow, BOOL bActive, CString& strResult) = 0;
	// 调用脚本方法
	virtual int __stdcall CallMethod(LPCTSTR lpszMethod, CStringArray& asParam) = 0;
[!endif]
[!if VCITYPE_PROJECT]
	// 新建工程
	virtual int __stdcall NewProject(LPCTSTR lpszProjectName, LPCTSTR lpszProjectPath, LPCTSTR lpszProjectDesc, LPCTSTR lpszAuthor) = 0;
	// 打开工程
	virtual int __stdcall OpenProject(LPCTSTR lpszProjectFile) = 0;
	// 保存工程
	virtual int __stdcall SaveProject() = 0;
	// 执行工程
	virtual int __stdcall RunProject() = 0;
	// 关闭工程
	virtual int __stdcall CloseProject(BOOL bForce) = 0;
	// 编译工程
	virtual int __stdcall BuildProject() = 0;
	// 重新编译工程
	virtual int __stdcall RebuildProject() = 0;
	// 清除工程编译信息
	virtual int __stdcall CleanProject() = 0;
	// 显示工程信息
	virtual int __stdcall ProjectInformation() = 0;
	// 刷新属性窗口的工程信息
	virtual int __stdcall RefreshProjectProperty() = 0;
	// 工程属性设置
	virtual int __stdcall ProjectPropertyPage(CVciPropertyPageArray& aPropertyPage) = 0;
	// 获取打开的工程列表
	virtual int __stdcall GetOpenProjectList(CStringArray& asProjectFile) = 0;
	// 获取当前激活的工程
	virtual int __stdcall GetActiveProject(CString& strProject) = 0;
	// 设置激活的工程
	virtual int __stdcall SetActiveProject(LPCTSTR lpszProject) = 0;
	// 获取工程状态
	virtual int __stdcall GetProjectState(LPCTSTR lpszProject) = 0;
	// 注册菜单
	virtual int __stdcall RegisterMenu(CMenu* pMenu, COWMMenuArray& aOWMMenus, int nOWMIdStart) = 0;
	// 卸载菜单
	virtual int __stdcall UnregisterMenu(COWMMenuArray& aOWMMenus) = 0;
	// 显示DockingPane窗口时调用的函数
	virtual CWnd* __stdcall OnShowDockingPane(int nID, CWnd* pParentWnd, LPCTSTR lpszParam) = 0;
	// 初始化代码定义库
	virtual int __stdcall InitCodeDefineLibrary(LPCTSTR lpszProject) = 0;
	// 加载代码定义库
	virtual int __stdcall LoadCodeDefineLibrary(LPCTSTR lpszProject) = 0;
	// 查找代码定义列表
	virtual int __stdcall GetCodeDefine(LPCTSTR lpszCode, CCodePositionArray& asCodePosition) = 0;
	// 查找代码引用列表
	virtual int __stdcall GetCodeReference(LPCTSTR lpszCode, CCodePositionArray& asCodePosition) = 0;
[!endif]
[!if VCITYPE_POLICY]
	// 设置策略管理器对外接口指针
	virtual void __stdcall SetIAgent(LPVOID pIAgent) = 0;
	// 策略初始化,加载时调用
	virtual int	__stdcall init(LPCSTR lpszParameter) = 0;
	// 策略中止时调用
	virtual int	__stdcall done(LPCSTR lpszParameter) = 0;
	// 策略合法性校验
	virtual int	__stdcall IsValid(LPCSTR lpszConfig) = 0;
	// 策略执行
	virtual int	__stdcall Exec(LPCSTR lpszConfig) = 0;
	// 策略修复
	virtual int __stdcall Repair(LPCSTR lpszConfig) = 0;
	// 更新策略执行参数
	virtual int	__stdcall UpdateExecParam(LPCSTR lpszConfig) = 0;
	// 策略备份
    virtual int	__stdcall Backup(LPCSTR lpszConfig) = 0;	
    // 策略恢复
    virtual int	__stdcall Restore(LPCSTR lpszConfig) = 0;	
[!endif]
	//}}VCI_INTERFACE_END
};

[!if VCITYPE_INTERP]
// 定义退出回调函数
#define CALLBACK_EVENT_INTERP_EXIT	0x1000
typedef int (INTERP_EXIT_FUNC)(void* lpInterp);

// 定义调试回调函数
#define CALLBACK_EVENT_INTERP_DEBUG	0x1001
typedef int (INTERP_DEBUG_FUNC)(int nEvent, LPCTSTR lpszScriptFile, int nLine);
[!endif]

#endif // !defined(__VCI_INTERFACE_I[!output INTERFACE_NAME]_H_INCLUDED__)
