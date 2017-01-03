#include "stdafx.h"
#include "CRubyInterp.h"
#include "resource.h"

////////////////////////////////////////////////////////////////////
// OwmEdit扩展命令
#define OC_OPENFILE				102	// 打开一个文档(to owm)
#define OC_OPENFILEFIRST		103	// 在第一个位置打开文档(to owm)
#define OC_SETREADONLY			104	// 设置文档的只读属性
#define OC_GETOPENFILE			105	// 获取打开的文件名

#define OC_SETEXECUTION			110	// 设置错误标记
#define OC_SETCURENTLINE		111	// 设置当前行
#define OC_SETCURENTEXECLINE	112	// 设置当前执行行
#define OC_SEARCHLINE			113	// 查找指定内容的行并设置为当前行
#define OC_GETBREAKLIST			115	// 获取断点列表
#define OC_SETSLAVEDEBUG		116	// 设置从调试模式(调试模式,但非主调试程序)

#define OC_GET_LINECOUNT		117	// 获取总行数
#define OC_GET_LINE				118	// 获取行
#define OC_SET_LINE				119	// 设置行
#define OC_GET_SELECTLINE		120	// 获取当前选择的行范围
#define OC_SET_SELECTLINE		121	// 设置选择的行
#define OC_GET_CURSORPOS		122	// 获取光标位置
#define OC_SET_CURSORPOS		123	// 设置光标位置
#define OC_SET_BREAKLIST		124	// 设置断点列表
#define OC_CLEAR_BREAK			125	// 清空断点

/////////////////////////////////////////////////////////////////////////////
// 获取平台路径
/////////////////////////////////////////////////////////////////////////////
CString GetPlatRootPath()
{
	//获取平台路径
	TCHAR szFullPath[MAX_PATH];
	TCHAR szdrive[_MAX_DRIVE];
	TCHAR szdir[_MAX_DIR];
	::GetModuleFileName(NULL, szFullPath, MAX_PATH);
	_splitpath(szFullPath, szdrive, szdir, NULL, NULL);
	CString szPath;
	szPath.Format(_T("%s%s"), szdrive, szdir);
	szPath = szPath.Left(szPath.GetLength() - 1);
	int nPos = szPath.ReverseFind('\\');
	if(nPos >= 0)
	{
		szPath = szPath.Left(nPos+1);
	}

	return szPath;
}

/////////////////////////////////////////////////////////////////////////////
// 获取解释器路径
/////////////////////////////////////////////////////////////////////////////
CString GetInterpRootPath()
{
	//获取平台路径
	TCHAR szFullPath[MAX_PATH];
	TCHAR szdrive[_MAX_DRIVE];
	TCHAR szdir[_MAX_DIR];
	::GetModuleFileName(GetDllInstance(), szFullPath, MAX_PATH);
	_splitpath(szFullPath, szdrive, szdir, NULL, NULL);
	CString szPath;
	szPath.Format(_T("%s%s"), szdrive, szdir);
	szPath = szPath.Left(szPath.GetLength() - 1);
	int nPos = szPath.ReverseFind('\\');
	if(nPos >= 0)
	{
		szPath = szPath.Left(nPos+1);
	}

	return szPath;
}

//////////////////////////////////////////////////////////////////////
// 解释器操作
//////////////////////////////////////////////////////////////////////
static int g_InterpID = 0x100;	// 解释器ID计数器
static CRubyInterp* g_pRubyInterp = NULL;

static VALUE g_hCaptureOutIO;	// 重载的标准输出Ruby对象
static VALUE g_hCaptureErrorIO;	// 重载的标准错误输出Ruby对象

// 获取重载的标准输出Ruby对象
VALUE get_CaptureOutIO()
{
	return g_hCaptureOutIO;
}

// 获取重载的标准错误输出Ruby对象
VALUE get_CaptureErrorIO()
{
	return g_hCaptureErrorIO;
}

// 重载的标准输出Ruby类的write函数
static VALUE _CaptureOutIO_Write(VALUE self, VALUE text)
{
	try {
	//////////////////////////////
	VALUE strtmp = rb_obj_as_string(text);	// 不论是什么类型，都先转换为字符串类型
	VALUE str = StringValue(strtmp);
    CString strText = RSTRING(str)->ptr;

	// 非string类型需要进行转换
	//CString strText = NUM2CHR(text);
    
	if(g_pRubyInterp && g_pRubyInterp->pIPlatUI)
	{
		IPlatUI* pIPlatUI = g_pRubyInterp->pIPlatUI;

		BOOL bDirectOutput = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_OUTPUT, TRUE);
		int nOT = 2;
		if(bDirectOutput)
		{
			nOT = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_OUTPUT_ID, 2);	// 信息窗口
		}
		pIPlatUI->OutputSet(nOT, "COLOR=0,0,255");	// 设置输出颜色
		pIPlatUI->Output(nOT, strText);
	}

	//TRACE(strText);
	//////////////////////////////
	}catch(...)
	{
	}
    
    return self;
}

// 重载的标准错误输出Ruby类的write函数
static VALUE _CaptureErrorIO_Write(VALUE self, VALUE text)
{
	try {
	//////////////////////////////
	VALUE strtmp = rb_obj_as_string(text);	// 不论是什么类型，都先转换为字符串类型
	VALUE str = StringValue(strtmp);
    CString strText = RSTRING(str)->ptr;

	if(g_pRubyInterp && g_pRubyInterp->pIPlatUI)
	{
		IPlatUI* pIPlatUI = g_pRubyInterp->pIPlatUI;

		BOOL bDirectOutput = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_OUTPUT, TRUE);
		int nOT = 2;
		if(bDirectOutput)
		{
			nOT = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_OUTPUT_ID, 2);	// 信息窗口
		}
		pIPlatUI->OutputSet(nOT, "COLOR=255,0,0");	// 设置输出颜色
		pIPlatUI->Output(nOT, strText);
	}

	//TRACE(strText);
	//////////////////////////////
	}catch(...)
	{
	}
    
    return self;
}

// 调试回调函数
static VALUE _Ruby_Debug_Callback_Line(VALUE self, VALUE file, VALUE line, VALUE id, VALUE binding, VALUE classname)
{
	try {
	//////////////////////////////
	VALUE strfile = StringValue(file);
    CString strFile = RSTRING(strfile)->ptr;
	if(strFile == "(eval)")
	{
		return self;
	}
	int nLine = NUM2INT(line);

	VALUE strid = rb_obj_as_string(id);
	strid = StringValue(strid);
    CString strId = RSTRING(strid)->ptr;
	VALUE strclassname = rb_obj_as_string(classname);
	strclassname = StringValue(strclassname);
    CString strClassName = RSTRING(strclassname)->ptr;
   
	if(g_pRubyInterp)
	{
		g_pRubyInterp->LineHook(strFile, nLine, strId, binding, strClassName);
	}
	//////////////////////////////
	}catch(...)
	{
	}
    
    return self;
}

// 初始化重载的标准输入输出
void RubyEmbedCaptureOutIOInit()
{
	try {
	//////////////////////////////
	// 标准输出
	VALUE h = rb_define_class("CaptureOutIO", rb_cIO);
	typedef VALUE (*HOOK)(...);
	rb_define_method(h, "write", reinterpret_cast<HOOK>(_CaptureOutIO_Write), 1);

	VALUE type = INT2NUM(1);
	VALUE mode = rb_str_new2("w");
	VALUE args[2];
	args[0] = type;
	args[1] = mode;

	g_hCaptureOutIO = rb_class_new_instance(2, args, rb_path2class("CaptureOutIO"));
	rb_stdout = g_hCaptureOutIO;

	// 标准错误输出
	VALUE hError = rb_define_class("CaptureErrorIO", rb_cIO);
	rb_define_method(hError, "write", reinterpret_cast<HOOK>(_CaptureErrorIO_Write), 1);

	VALUE typeError = INT2NUM(2);
	VALUE modeError = rb_str_new2("w");
	VALUE argsError[2];
	argsError[0] = typeError;
	argsError[1] = modeError;

	g_hCaptureErrorIO = rb_class_new_instance(2, argsError, rb_path2class("CaptureErrorIO"));
	rb_stderr = g_hCaptureErrorIO;

	// 注册调试回调函数
	rb_define_global_function("debug_callback_line", reinterpret_cast<HOOK>(_Ruby_Debug_Callback_Line), 5);
	//////////////////////////////
	}catch(...)
	{
	}
}

//////////////////////////////////////////////////////////////////////////
// 功能实现
//////////////////////////////////////////////////////////////////////////
CRubyInterp::CRubyInterp()
{
	m_nInterpID		= g_InterpID++;
	if(g_InterpID > 0x01000000)
		g_InterpID  = 0x100;
	pIPlatUI		= NULL;
	m_dwRunThreadId	= NULL;
	m_strInterpName	= _T("");
	m_strScriptFile	= _T("");
	m_nRunMode		= SCRIPT_RUN_NORMAL;
	m_nRunState		= SCRIPT_STATE_IDLE;
	m_nRunLevel		= SCRIPT_LEVEL_NONE;
	m_nEvalResult	= 0;
	m_bStopSignal	= FALSE;
	m_bBreakSignal	= FALSE;
	m_nBreakMode	= SCRIPT_BREAK_NORMAL;
	m_nBreakLineNo	= 0;
	m_strBreakName	= _T("");
	m_nBreakLevel		= 0;
	m_nStackLevel	= 0;
	m_bProfileEnable= FALSE;
	m_nTDLineCount	= 0;
	m_ProfileMap.InitHashTable(128);	// 初始化覆盖率分析哈希表
	m_bHide			= FALSE;
	m_lpfnExit		= NULL;
	m_hPipeProcess	= NULL;
	m_bPipeProcessTerminated = FALSE;

	m_strCurRunCommand = "";
	m_strScriptResult = "";

	g_pRubyInterp	= this;
}

CRubyInterp::~CRubyInterp()
{
	// 清空覆盖率分析哈希表
	m_ProfileMap.RemoveAll();

	try {
	}catch(...)
	{
	}

	g_pRubyInterp = NULL;
}


/////////////////////////////////////////////////////////////////////////////
// 安装回调函数
/////////////////////////////////////////////////////////////////////////////
int CRubyInterp::InstallHandler(DWORD nEventID, LPVOID lpHandler, DWORD dwRefData)
{
	CVisualComponent::InstallHandler(nEventID, lpHandler, dwRefData);

	if(nEventID == CALLBACK_EVENT_INTERP_EXIT)
	{
		if(lpHandler == NULL)
		{
			return -1;
		}

		// 安装退出回调函数
		m_lpfnExit = (INTERP_EXIT_FUNC*)lpHandler;
		TRACE("InstallHandler: Event=CALLBACK_EVENT_INTERP_EXIT, FunctionEntry=0x%08X, DeviceID=0x%04X\n",
			lpHandler, dwRefData);
	}else
	if(nEventID == CALLBACK_EVENT_INTERP_DEBUG)
	{
		if(lpHandler == NULL)
		{
			return -1;
		}

		// 安装退出回调函数
		m_lpfnDebug = (INTERP_DEBUG_FUNC*)lpHandler;
		TRACE("InstallHandler: Event=CALLBACK_EVENT_INTERP_DEBUG, FunctionEntry=0x%08X, DeviceID=0x%04X\n",
			lpHandler, dwRefData);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 卸载回调函数
/////////////////////////////////////////////////////////////////////////////
int CRubyInterp::UninstallHandler(DWORD nEventID, LPVOID lpHandler)
{
	if(nEventID == CALLBACK_EVENT_INTERP_EXIT)
	{
		m_lpfnExit = NULL;
	}else
	if(nEventID == CALLBACK_EVENT_INTERP_DEBUG)
	{
		m_lpfnDebug = NULL;
	}

	CVisualComponent::UninstallHandler(nEventID, lpHandler);

	return 0;
}

//{{VCI_IMPLEMENT_BEGIN
int __stdcall CRubyInterp::SetIPlatUI(LPVOID lpIPlatUI)
{
	pIPlatUI = (IPlatUI*)lpIPlatUI;

	// 设置语言变量
	CString strCurrentLanguage = _T("english");
	if(VCI_LANGUAGE_CHINESE == GetLanguage())
	{
		strCurrentLanguage = _T("chinese");
	}

	// TODO: Add your code here
	if(pIPlatUI)
	{
		ruby_script(pIPlatUI->GetAppName());
	}

	return 0;
}

// 设置库路径
int __stdcall CRubyInterp::SetLibPath(LPCTSTR lpszPath)
{
	// TODO: Add your code here

	return 0;
}

// 解释器是否在运行
int __stdcall CRubyInterp::IsInterpRunning()
{
	// TODO: Add your code here

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 获取调试文件名的索引,用于判断文件是否已经打开了
/////////////////////////////////////////////////////////////////////////////
int CRubyInterp::FindDebugFile(CString strFileName)
{
	for(int i=0; i<m_asDebugScript.GetSize(); i++)
	{
		if(strFileName == m_asDebugScript[i])
		{
			return i;
		}
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// 在调试文件列表中增加文件
/////////////////////////////////////////////////////////////////////////////
int CRubyInterp::AddDebugFile(CString strFileName)
{
	if(FindDebugFile(strFileName) == -1)
	{
		return m_asDebugScript.Add(strFileName);
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// 清空调试文件列表
/////////////////////////////////////////////////////////////////////////////
BOOL CRubyInterp::ClearDebugFile()
{
	m_asDebugScript.RemoveAll();
	return TRUE;
}

// 设置脚本文件名
int __stdcall CRubyInterp::SetScriptFile(LPCTSTR lpszPathName)
{
	// TODO: Add your code here
	m_strScriptFile = lpszPathName;

	return TRUE;
}

// 获取脚本文件名
CString __stdcall CRubyInterp::GetScriptFile()
{
	// TODO: Add your code here

	return m_strScriptFile;
}

// 运行进程并挂接自定义输出管道
BOOL CRubyInterp::RunProgramWithPipe(CString strExePath, CString strParam, int nOutputId, BOOL bActiveOutput)
{
	m_hPipeProcess = NULL;

	SECURITY_ATTRIBUTES sa;
	HANDLE hRead,hWrite;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead,&hWrite,&sa,0))
	{
		AfxMessageBox("创建运行管道失败！");
		DWORD dwError = GetLastError();
		return FALSE;
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	//si.hStdInput = hRead;
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	// 组合命令行
	CString strCmd;
	strCmd.Format("%s %s", strExePath, strParam);
	char cscmd[512];
	strcpy(cscmd, strCmd);

	m_bPipeProcessTerminated = FALSE;
	if (!CreateProcess(NULL, cscmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		DWORD dwError = GetLastError();
		AfxMessageBox("创建管道进程失败！");
		return FALSE;
	}
	CloseHandle(hWrite);

	m_hPipeProcess = pi.hProcess;

	if(pIPlatUI && bActiveOutput)
	{
		pIPlatUI->ActiveOutput(nOutputId);
		pIPlatUI->OutputClear(nOutputId);
		pIPlatUI->OutputSet(nOutputId, "COLOR=0,0,255");	// Set output color
	}

	char buffer[4096] = {0};
	DWORD bytesRead;
	while (true)
	{
		memset(buffer, 0, 4096);
		if (ReadFile(hRead, buffer, 4095, &bytesRead, NULL) == NULL)
			break;
		if(pIPlatUI)
		{
			pIPlatUI->Output(nOutputId, buffer);
		}
		Sleep(200);
	}

	m_hPipeProcess = NULL;

	// 如果管道进程是被终止的,返回失败
	if(m_bPipeProcessTerminated)
	{
		return FALSE;
	}

	return TRUE;
}

// 终止管道进程
BOOL CRubyInterp::TerminatePipeProcess()
{
	if(m_hPipeProcess != NULL)
	{
		m_bPipeProcessTerminated = TRUE;
		if(!::TerminateProcess(m_hPipeProcess, 0))
		{
			return FALSE;
		}
		m_hPipeProcess = NULL;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 执行外部进程
/////////////////////////////////////////////////////////////////////////////
BOOL CRubyInterp::ExecProcess(CString strExePath, CString strParam, BOOL bWaitEnd)
{
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
 
	// Set up members of the PROCESS_INFORMATION structure. 
	ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );

	// Set up members of the STARTUPINFO structure. 
	ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
	siStartInfo.cb = sizeof(STARTUPINFO); 
	siStartInfo.wShowWindow=SW_HIDE ;
	siStartInfo.dwFlags=STARTF_USESHOWWINDOW;

	// 组合命令行
	CString strCmd;
	strCmd.Format("%s %s", strExePath, strParam);
	char cscmd[255];
	strcpy(cscmd, strCmd);
	
	// 创建报表进程
	BOOL bRet=CreateProcess(NULL,
      cscmd,		  // command line 
      NULL,          // process security attributes 
      NULL,          // primary thread security attributes 
      TRUE,          // handles are inherited 
      0,             // creation flags 
      NULL,          // use parent's environment 
      NULL,          // use parent's current directory 
      &siStartInfo,  // STARTUPINFO pointer 
      &piProcInfo);  // receives PROCESS_INFORMATION
	
	if(bWaitEnd)
	{
		// 等待进程结束(超时时间20秒)
		WaitForSingleObject(piProcInfo.hProcess, 20000);
	}
	return bRet;
}
/*
#include <fcntl.h>
int rb_w32_redirect_stdin(HANDLE pipe)
{
	return dup2(_open_osfhandle((long)pipe, _O_TEXT), 0);
}

int rb_w32_redirect_stdout(HANDLE pipe)
{
	return dup2(_open_osfhandle((long)pipe, _O_TEXT), 1);
}

int rb_w32_redirect_stderr(HANDLE pipe)
{
	return dup2(_open_osfhandle((long)pipe, _O_TEXT), 2);
}

#define READ_THREAD_BUFFER_SIZE 1024
// thread that continually grabs Ruby's stdout output
unsigned long __stdcall stdioReadThread(void* parameter)
{
	DWORD dwRead;
	char chBuf[READ_THREAD_BUFFER_SIZE];

	for( ;; )
	{
		if(!ReadFile(stdioReadHandle, chBuf, READ_THREAD_BUFFER_SIZE - 1, &dwRead, NULL) || dwRead == 0)
			break;
		chBuf[dwRead / sizeof(char)] = '\0';
		// DO SOMETHING WITH THE BUFFER HERE
	}

	return 0;
}
*/

// 调试回调
void CRubyInterp::DebugBreak(const char *szFile, int nStackLevel, int nLine)
{
	// 保存堆栈层数，需要计算
	SetStackLevel(nStackLevel);

	//看看是否需要打开新文件
	CString file1 = szFile;
	file1.Replace('\\', '/');
	CString file2 = m_strScriptFile;
	file2.Replace('\\', '/');
	if(file1 != file2)
	{
		if(FindDebugFile(szFile) < 0)
		{
			//添加文件
			if(AddDebugFile(szFile) >= 0)
			{
				pIPlatUI->OpenDocumentFile(szFile, "");
			}
			//添加调试文件失败，不进行断点
			else
			{
				//TODO
				return;
			}
		}
		// 向OwmEdit发送设置运行模式为从调试模式的命令
		pIPlatUI->SendOwmCmd("org.owm.edit", OC_SETSLAVEDEBUG,
			(WPARAM)(LPTSTR)(LPCTSTR)szFile, 1, NULL);
	}
	else
	{
		// 向OwmEdit发送设置运行模式为非从调试模式的命令
		pIPlatUI->SendOwmCmd("org.owm.edit", OC_SETSLAVEDEBUG,
			(WPARAM)(LPTSTR)(LPCTSTR)szFile, 0, NULL);
	}

	//设置当前行
	if(m_lpfnDebug)
	{
		m_lpfnDebug(IDB_USER_SETCURRENTLINE, szFile, nLine);
	}
label_waitevent:
	//设置当前状态为中断状态
	SetRunState(SCRIPT_STATE_BREAK);
	//重新设置事件
	::ResetEvent(m_hDebugEvent);
	
	//等待调试命令
	::WaitForSingleObject(m_hDebugEvent, INFINITE);

	if(m_nCurCommand == IDB_REFRESH_DEBUGWINDOW)	// 刷新调试窗口
	{
		switch(m_nRefreshDebugWindow)
		{
		case 1:RefreshVarWindow();break;
		case 2:break;
		case 3:RefreshFuncWindow();break;
		case 4:RefreshStackWindow();break;
		}

		//重新跳转到等待
		goto label_waitevent;
	}else
	if(m_nCurCommand == IDB_GET_VAR)	// 获取变量信息
	{
		m_strDebugSetGetVarValue = InternalGetVar(m_strDebugSetGetVarName);
		//TRACE("--GetVar:%s=%s\n", m_strDebugSetGetVarName, m_strDebugSetGetVarValue);
		::SetEvent(m_hDebugSetGetEvent);
		//重新跳转到等待
		goto label_waitevent;
	}else
	if(m_nCurCommand == IDB_SET_VAR)	// 设置变量值
	{
		m_nDebugSetVarResult = InternalSetVar(m_strDebugSetGetVarName, m_strDebugSetGetVarValue);
		::SetEvent(m_hDebugSetGetEvent);
		//重新跳转到等待
		goto label_waitevent;
	}else
	if(m_nCurCommand == IDB_USER_CALL)	// 执行脚本命令
	{
		if(m_strCurRunCommand != "")
		{
			m_strScriptResult = "";
			int nError;
			VALUE vret = rb_eval_string_protect(m_strCurRunCommand, &nError);
			VALUE vartype = rb_funcall(vret,  rb_intern("class"), 0);
			if(vartype == rb_cArray)
			{
				// 数组类型
				VALUE vsize = rb_funcall(vret,  rb_intern("size"), 0);
				int nSize = NUM2INT(vsize);
				for(int i=0; i<nSize; i++)
				{
					VALUE vitem = rb_funcall(vret,  rb_intern("at"), 1, INT2NUM(i));
					VALUE strtmp = rb_obj_as_string(vitem);
					VALUE str = StringValue(strtmp);
					m_strScriptResult += RSTRING(str)->ptr;
					m_strScriptResult += " ";
				}
				m_strScriptResult.TrimRight();
			}else
			{
				// 其他类型
				VALUE strtmp = rb_obj_as_string(vret);
				VALUE str = StringValue(strtmp);
				m_strScriptResult = RSTRING(str)->ptr;
			}
		}

		//重新跳转到等待
		goto label_waitevent;
	}

	// 设置当前状态为运行状态
	SetRunState(SCRIPT_STATE_RUN);
	// 向Edit发送当前行清除命令
	if(m_lpfnDebug)
	{
		m_lpfnDebug(IDB_USER_SETCURRENTLINE, szFile, 0);
	}
}

// 行回调
void CRubyInterp::LineHook(const char *szFile, int nLine, CString strId, VALUE binding, CString strClassName)
{
	m_strDebugCurId = strId;
	m_vDebugCurBinding = binding;
	m_strDebugCurClassName = strClassName;

	//判断当前模式
	if(m_nCurCommand == IDB_STOP)	// 终止程序运行
	{
		StopScript();
	}else
	if(m_nCurCommand == IDB_STEPINTO) //单步进入函数，每步必停
	{
		DebugBreak(szFile, GetCurStackLevel(), nLine);
		return;
	}
	else
	{
		if(m_nCurCommand == IDB_STEP) //单步（不进入函数），同一帧的下一个步骤停止
		{
			int nStackLevel = GetCurStackLevel();
			if(GetStackLevel() >= nStackLevel)
			{
				DebugBreak(szFile, nStackLevel, nLine);
				return;
			}
		}else
		if(m_nCurCommand == IDB_GOTO) //执行到指定行
		{
			if(nLine == GetBreakLineNo())
			{
				DebugBreak(szFile, GetCurStackLevel(), nLine);
				return;
			}
		}
		
		//判断是否到了设置的断点位置，是的话断点，暂时只支持单文件断点
		CString file1 = szFile;
		file1.Replace('\\', '/');
		CString file2 = m_strScriptFile;
		file2.Replace('\\', '/');
		if(file1 == file2)
		{
			for(int i=0; i<m_auBreakList.GetSize(); i++)
			{
				if(nLine == m_auBreakList.GetAt(i))
				{
					DebugBreak(szFile, GetCurStackLevel(), nLine);
					return;
				}
			}
		}

		if(m_nCurCommand == IDB_SETPOUT) //退出函数，返回到上一层
		{
			int nStackLevel = GetCurStackLevel();
			if(nStackLevel < GetStackLevel())
			{
				DebugBreak(szFile, nStackLevel, nLine);
				return;
			}
		}
	}
}

// 运行脚本文件
int __stdcall CRubyInterp::RunScriptFile(LPCTSTR lpszPathName)
{
	// TODO: Add your code here
	if(lpszPathName != NULL)
	{
		if(!SetScriptFile(lpszPathName))
			return FALSE;
	}

	m_dwRunThreadId = ::GetCurrentThreadId();

	m_nRunState = SCRIPT_STATE_RUN;

	try {

	ruby_init();

	// 调试模式
	if(SCRIPT_RUN_NORMAL != m_nRunMode)
	{
		// 创建调试事件
		static int eventNum = 0;
		CString eventName;
		eventName.Format("RubyDebug%d", eventNum);
		m_hDebugEvent = ::CreateEvent(NULL, FALSE, FALSE, eventName);		
		if(!m_hDebugEvent)
		{
			::AfxMessageBox("Create Debug Event Failed, Can not debug");
			m_nRunState = SCRIPT_STATE_END;
			return FALSE;
		}

		// 创建调试时的获取/设置变量的事件
		eventName.Format("RubyDebugGetSetVar%d", eventNum);
		m_hDebugSetGetEvent = ::CreateEvent(NULL, FALSE, FALSE, eventName);
		if(!m_hDebugSetGetEvent)
		{
			::AfxMessageBox("Create Debug Set/Get Var Event Failed, Can not debug");
			m_nRunState = SCRIPT_STATE_END;
			return FALSE;
		}
		eventNum++;

		// 设置Ruby程序跟踪回调函数
		int nError;
		/*
		CString strCmd = "set_trace_func proc { |platform_temp_event, platform_temp_file, platform_temp_line, platform_temp_id, platform_temp_binding, platform_temp_classname|\n\
							if platform_temp_event == \"line\"\n\
							eval(\"puts instance_variables\",platform_temp_binding)\n\
							puts platform_temp_file, platform_temp_line, platform_temp_id, instance_variables, platform_temp_classname\n\
							debug_callback_line(platform_temp_file, platform_temp_line, platform_temp_id, instance_variables, platform_temp_classname)\n\
							end\n\
						}";
		*/
		CString strCmd = "set_trace_func proc { |platform_temp_event, platform_temp_file, platform_temp_line, platform_temp_id, platform_temp_binding, platform_temp_classname|\n\
							if platform_temp_event == \"line\"\n\
							debug_callback_line(platform_temp_file, platform_temp_line, platform_temp_id, platform_temp_binding, platform_temp_classname)\n\
							end\n\
						}";
		rb_eval_string_protect(strCmd, &nError);
		TRACE("1.rb_eval_string_protect ret:%d\n", nError);
	}

/*
	// create pipe that replaces Ruby's stdout
	if(CreatePipe(&stdioReadHandle, &stdioWriteHandle, NULL, 0))
	{
		dup2(_open_osfhandle((long)stdioWriteHandle, _O_TEXT), 1);
		dup2(_open_osfhandle((long)stdioWriteHandle, _O_TEXT), 2);

		stdioReadThreadHandle = CreateThread(NULL, 0, stdioReadThread, outputTextBox, 0, &stdioReadThreadID);
	}
*/
/*
	int argc = 0;
	char **argv = 0;
	NtInitialize(&argc,&argv);

	ruby_init();
	ruby_init_loadpath();
	ruby_script("embedded");
*/
	//RubyEmbedCaptureOutIOInit();

	// 清空异常描述和异常堆栈变量	
	//rb_gv_set("$!", rb_str_new2(""));
	//rb_gv_set("$@", rb_str_new2(""));
	int nError;
	CString strCmd = "$@ = {}";
	rb_eval_string_protect(strCmd, &nError);
	TRACE("2.$! = NIL ret:%d\n", nError);

	rb_load_file(m_strScriptFile);

	/*
	//rb_eval_string(strExecCmd);
	int nError;
	rb_eval_string_protect(strExecCmd, &nError);
	TRACE("rb_eval_string_protect ret:%d\n", nError);
	*/

	//ruby_run();
	int status = ruby_exec();

	//ID id_close = rb_intern("close");
	//rb_funcall(g_hIO, id_close, 0);

	//ruby_cleanup(status);
	//ruby_finalize();

	if(SCRIPT_RUN_NORMAL != m_nRunMode)
	{
		// 清除Ruby程序跟踪回调函数
		int nError;
		CString strCmd = "set_trace_func(nil)";
		rb_eval_string_protect(strCmd, &nError);
		TRACE("3.rb_eval_string_protect ret:%d\n", nError);

		//清除从debug文件
		for(int i=0; i<m_asDebugScript.GetSize(); i++)
		{
			CString strFileName = m_asDebugScript.GetAt(i);
			// 向OwmEdit发送设置运行模式为非从调试模式的命令
			pIPlatUI->SendOwmCmd("org.owm.edit", OC_SETSLAVEDEBUG, (WPARAM)(LPTSTR)(LPCTSTR)strFileName, -1, NULL);
			//清除从文件的调试箭头
			if(m_lpfnDebug)
			{
				m_lpfnDebug(IDB_USER_SETCURRENTLINE, strFileName, 0);
			}
		}
		m_asDebugScript.RemoveAll();

		if(m_lpfnDebug)
		{
			//清除主调试文件的调试箭头
			m_lpfnDebug(IDB_USER_SETCURRENTLINE, m_strScriptFile, 0);
		}

		// 关闭调试事件
		if(m_hDebugEvent)
		{
			CloseHandle(m_hDebugEvent);
			m_hDebugEvent = NULL;
		}
		if(m_hDebugSetGetEvent)
		{
			CloseHandle(m_hDebugSetGetEvent);
			m_hDebugSetGetEvent = NULL;
		}
	}

	////////////////////////////////////////////
	}catch(...)
	{
	}

	m_nRunState = SCRIPT_STATE_END;

	m_dwRunThreadId = NULL;
	//m_nEvalResult	= TCL_OK;
	return TRUE;
}

// 执行脚本命令
int __stdcall CRubyInterp::RunScriptCommand(LPCTSTR lpszTclCmd)
{
	// TODO: Add your code here

	try {

	// 如果是正在运行或调试一个脚本文件,就发调试消息,由调试线程来执行
	if(SCRIPT_STATE_RUN == m_nRunState)
	{
		// 如果非调试状态就退出
		if(SCRIPT_RUN_NORMAL != m_nRunMode)
		{
			return TRUE;
		}
		m_strCurRunCommand = lpszTclCmd;
		m_nCurCommand = IDB_USER_CALL;
		::SetEvent(m_hDebugEvent);
		return TRUE;
	}

	if(m_dwRunThreadId == NULL)
	{
		// 如果当前没有脚本文件在运行,就设置当前的线程为ruby原始线程
		ruby_init();
	}

	// 清空异常描述和异常堆栈变量
	//rb_gv_set("$!", rb_str_new2(""));
	//rb_gv_set("$@", rb_str_new2(""));
	int nError;
	CString strCmd = "$@ = {}";
	rb_eval_string_protect(strCmd, &nError);
	TRACE("$! = NIL ret:%d\n", nError);

	m_strScriptResult = "";
	VALUE vret = rb_eval_string_protect(lpszTclCmd, &nError);
	VALUE vartype = rb_funcall(vret,  rb_intern("class"), 0);
	if(vartype == rb_cArray)
	{
		// 数组类型
		VALUE vsize = rb_funcall(vret,  rb_intern("size"), 0);
		int nSize = NUM2INT(vsize);
		for(int i=0; i<nSize; i++)
		{
			VALUE vitem = rb_funcall(vret,  rb_intern("at"), 1, INT2NUM(i));
			VALUE strtmp = rb_obj_as_string(vitem);
			VALUE str = StringValue(strtmp);
			m_strScriptResult += RSTRING(str)->ptr;
			m_strScriptResult += " ";
		}
		m_strScriptResult.TrimRight();
	}else
	{
		// 其他类型
		VALUE strtmp = rb_obj_as_string(vret);
		VALUE str = StringValue(strtmp);
		m_strScriptResult = RSTRING(str)->ptr;
	}

	TRACE("rb_eval_string_protect ret:%d\n", nError);

	//int status = ruby_exec();
	////////////////////////////////////////////
	}catch(...)
	{
	}

	//m_nEvalResult = TCL_OK;
	return TRUE;
}

// 停止脚本运行
int __stdcall CRubyInterp::StopScript()
{
	// TODO: Add your code here
	m_bStopSignal	= TRUE;	// 设置中止标志

	// 终止管道进程
	TerminatePipeProcess();

	try {
	//////////////////////////////

	// 目前没有很好的终止的方法
	// 调用Kernel模块的abort函数终止脚本执行
	rb_funcall(rb_mKernel,  rb_intern("abort"), 1, rb_str_new2("script terminated at user request."));

	//////////////////////////////
	}catch(...)
	{
	}

	return TRUE;
}

// 获取解释器指针
LPVOID __stdcall CRubyInterp::GetInterp()
{
	// TODO: Add your code here

	return NULL;
}

// 获取解释器ID
int __stdcall CRubyInterp::GetInterpID()
{
	// TODO: Add your code here

	return m_nInterpID;
}

// 设置解释器名
void __stdcall CRubyInterp::SetInterpName(LPCTSTR lpszInterptName)
{
	// TODO: Add your code here
	m_strInterpName = lpszInterptName;
}

// 获取解释器名
CString __stdcall CRubyInterp::GetInterpName()
{
	// TODO: Add your code here

	return m_strInterpName;
}

// 解析错误堆栈行数组
int CRubyInterp::ParseErrorStack(CString strStack, CStringArray& asStack, BOOL bDelIn)
{
	int nPos = strStack.Find(":in ");
	while((nPos = strStack.Find(":in ")) != -1)
	{
		asStack.Add(strStack.Left(nPos));
		strStack.Delete(0, nPos+1);
		if(bDelIn)
		{
			// 删除in `functionname`
			if(strStack.Find("`") == 3)
			{
				strStack.Delete(0, 4);
				int nPos1 = strStack.Find("'");
				if(nPos1 != -1)
				{
					strStack.Delete(0, nPos1+1);
				}
			}
		}
	}
	if(strStack.GetLength() > 0)
	{
		asStack.Add(strStack);
	}

	return asStack.GetSize();
}

// 获取错误行号
int __stdcall CRubyInterp::GetErrorLine()
{
	// TODO: Add your code here
	// $!存放异常的原因描述
	VALUE exception_instance = rb_gv_get("$!");
	// class
	VALUE klass = rb_class_path(CLASS_OF(exception_instance));
	CString sclass = RSTRING(klass)->ptr; 
	// message
	VALUE message = rb_obj_as_string(exception_instance);
	CString smessage = RSTRING(message)->ptr;

	// $@存放异常的堆栈信息
	VALUE exception_stack = rb_gv_get("$@");
	// class
	VALUE klass_stack = rb_class_path(CLASS_OF(exception_stack));
	CString sclass_stack = RSTRING(klass_stack)->ptr; 
	// message
	VALUE message_stack = rb_obj_as_string(exception_stack);
	CString smessage_stack = RSTRING(message_stack)->ptr;

	int nErrorLine = 0;
	if(klass != rb_cNilClass)
	{
		CStringArray asStackLine;
		int nStackSize = ParseErrorStack(smessage_stack, asStackLine, TRUE);
		for(int i=0; i<nStackSize; i++)
		{
			CString strStack = asStackLine.GetAt(i);
			int nPos = strStack.Find(":");
			if(nPos == 1)
			{
				nPos = strStack.Find(":", 2);
			}
			if(nPos != -1)
			{
				CString strStackFile = strStack.Left(nPos);
				strStackFile.Replace('\\', '/');
				CString strScriptFile = m_strScriptFile;
				strScriptFile.Replace('\\', '/');
				if(strStackFile == strScriptFile)
				{
					strStack.Delete(0, nPos+1);
					nPos = strStack.Find(":");
					if(nPos != -1)
					{
						nErrorLine = atoi(strStack.Left(nPos));
					}else
					{
						nErrorLine = atoi(strStack);
					}
					return nErrorLine;
				}
			}
		}
	}

	return nErrorLine;
}

// 获取结果信息
CString __stdcall CRubyInterp::GetResult()
{
	// TODO: Add your code here
	if(GetErrorLine() != 0)
	{
		// $!存放异常的原因描述
		VALUE exception_instance = rb_gv_get("$!");
		// class
		VALUE klass = rb_class_path(CLASS_OF(exception_instance));
		CString sclass = RSTRING(klass)->ptr; 
		// message
		VALUE message = rb_obj_as_string(exception_instance);
		CString smessage = RSTRING(message)->ptr;

		// $@存放异常的堆栈信息
		VALUE exception_stack = rb_gv_get("$@");
		// class
		VALUE klass_stack = rb_class_path(CLASS_OF(exception_stack));
		CString sclass_stack = RSTRING(klass_stack)->ptr; 
		// message
		VALUE message_stack = rb_obj_as_string(exception_stack);
		CString smessage_stack = RSTRING(message_stack)->ptr;

		if(klass != rb_cNilClass)
		{
			CStringArray asStackLine;
			int nStackSize = ParseErrorStack(smessage_stack, asStackLine, FALSE);
			CString strResult = smessage + "\r\nat ";
			for(int i=0; i<nStackSize; i++)
			{
				strResult += asStackLine.GetAt(i);
				strResult += "\r\n";
			}
			return strResult;
		}
	}

	return m_strScriptResult;
}

// 内部函数:设置变量值
int CRubyInterp::InternalSetVar(LPCTSTR lpszVarName, LPCTSTR lpszVarValue)
{
	CString strCmd;
	strCmd.Format("%s", lpszVarName);
	int nError;
	//VALUE vvar = rb_eval_string(strCmd);
	VALUE vvar = rb_eval_string_protect(strCmd, &nError);
	if(nError != 0)
	{
		return 0;
	}

	VALUE vartype = rb_funcall(vvar,  rb_intern("class"), 0);
	VALUE strtmp = rb_obj_as_string(vartype);
	VALUE str = StringValue(strtmp);
	CString strType = RSTRING(str)->ptr;
	if(strType == "String")
	{
		strCmd.Format("%s=\"%s\"", lpszVarName, lpszVarValue);
		rb_eval_string(strCmd);
	}else
	{
		strCmd.Format("%s=%s", lpszVarName, lpszVarValue);
		rb_eval_string(strCmd);
	}

	return 0;
}

// 获取变量值
CString CRubyInterp::InternalGetVar(LPCTSTR lpszVarName)
{
	// 首先判断变量名是否和内核的所有函数名同名,如果有同名的就退出
	VALUE vmethods = rb_funcall(rb_mKernel,  rb_intern("methods"), 0);
	VALUE vsize = rb_funcall(vmethods, rb_intern("size"), 0);
	int nSize = NUM2INT(vsize);
	for(int i=0; i<nSize; i++)
	{
		VALUE vmethod = rb_funcall(vmethods,  rb_intern("at"), 1, INT2NUM(i));
		VALUE strtmp = rb_obj_as_string(vmethod);
		VALUE str = StringValue(strtmp);
		CString strMethod = RSTRING(str)->ptr;
		if(strMethod == lpszVarName)
		{
			return "";
		}
	}

	CString strCmd;
	strCmd.Format("%s", lpszVarName);
	int nError;
	//VALUE vvar = rb_eval_string(strCmd);
	VALUE vvar = rb_eval_string_protect(strCmd, &nError);
	if(nError != 0)
	{
		return "";
	}
	//VALUE strtmp = rb_obj_as_string(vvar);
	//VALUE str = StringValue(strtmp);
	CString strValue = "";

	VALUE vartype = rb_funcall(vvar,  rb_intern("class"), 0);
	if(vartype == rb_cArray)
	{
		// 数组类型
		VALUE vsize = rb_funcall(vvar,  rb_intern("size"), 0);
		int nSize = NUM2INT(vsize);
		for(int i=0; i<nSize; i++)
		{
			VALUE vitem = rb_funcall(vvar,  rb_intern("at"), 1, INT2NUM(i));
			VALUE strtmp = rb_obj_as_string(vitem);
			VALUE str = StringValue(strtmp);
			strValue += RSTRING(str)->ptr;
			strValue += " ";
		}
		strValue.TrimRight();
	}else
	/*if(vartype == rb_cHash)
	{
		// 哈希类型
	}else*/
	{
		// 其他类型
		VALUE strtmp = rb_obj_as_string(vvar);
		VALUE str = StringValue(strtmp);
		strValue = RSTRING(str)->ptr;
	}

	return strValue;
}

// 设置变量值
int __stdcall CRubyInterp::SetVar(LPCTSTR lpszVarName, LPCTSTR lpszVarValue)
{
	// TODO: Add your code here
	// 如果是正在运行或调试一个脚本文件,就发调试消息,由调试线程来执行
	if((SCRIPT_STATE_BREAK == m_nRunState) || (SCRIPT_STATE_STEP == m_nRunState))
	{
		::ResetEvent(m_hDebugSetGetEvent);
		m_strDebugSetGetVarName = lpszVarName;
		m_strDebugSetGetVarValue = lpszVarValue;
		m_nDebugSetVarResult = 0;

		m_nCurCommand = IDB_SET_VAR;
		::SetEvent(m_hDebugEvent);

		//等待设置完变量信息
		::WaitForSingleObject(m_hDebugSetGetEvent, INFINITE);

		return m_nDebugSetVarResult;
	}

	return InternalSetVar(lpszVarName, lpszVarValue);
}

// 获取变量值
CString __stdcall CRubyInterp::GetVar(LPCTSTR lpszVarName)
{
	// TODO: Add your code here
	// 必须先排除一些关键字,因为目前获取变量的方法是执行脚本,如果是关键字,则执行时候会异常
	static CString g_strKeywordCheck = " __FILE__ and def end in or self unless __LINE__ begin defined ensure module redo super until BEGIN break do false next rescue then when END case else for nil retry true while alias class elsif if not return undef yield puts ";
	CString strVarNameCheck;
	strVarNameCheck.Format(" %s ", lpszVarName);
	if(g_strKeywordCheck.Find(strVarNameCheck) != -1)
	{
		return "";
	}

	// 如果是正在运行或调试一个脚本文件,就发调试消息,由调试线程来执行
	if((SCRIPT_STATE_BREAK == m_nRunState) || (SCRIPT_STATE_STEP == m_nRunState))
	{
		::ResetEvent(m_hDebugSetGetEvent);
		m_strDebugSetGetVarName = lpszVarName;
		m_strDebugSetGetVarValue = "";
		//TRACE("--Enter GetVar:%s\n", m_strDebugSetGetVarName);

		m_nCurCommand = IDB_GET_VAR;
		::SetEvent(m_hDebugEvent);

		//等待获取到变量信息
		::WaitForSingleObject(m_hDebugSetGetEvent, 100);//INFINITE);
		//TRACE("--GetVar Result:%s\n", m_strDebugSetGetVarValue);

		return m_strDebugSetGetVarValue;
	}

	return InternalGetVar(lpszVarName);
}

// 注册扩展命令
int __stdcall CRubyInterp::RegisterCommand(LPVOID pInterp, LPCTSTR lpszCmd, LPVOID lpFunc)
{
	// TODO: Add your code here

	return 0;
}

// 设置扩展参数
int __stdcall CRubyInterp::SetInterpParam(LPCTSTR lpszParamName, LPCTSTR lpszParamValue)
{
	// TODO: Add your code here

	return 0;
}

// 获取扩展参数
CString __stdcall CRubyInterp::GetInterpParam(LPCTSTR lpszParamName)
{
	// TODO: Add your code here

	return "";
}

// 设置运行模式
int __stdcall CRubyInterp::SetRunMode(int nRunMode)
{
	m_nRunMode = nRunMode;
	//根据运行模式设置当前调试指令，主要用于单步时进入主函数可以断点
	switch(m_nRunMode)
	{
	case SCRIPT_RUN_DEBUG_GO:  //go，相当于运行到断点位置停止
		m_nCurCommand = IDB_GOTO;
		break;
	case SCRIPT_RUN_DEBUG_STEP:  //单步执行，一开始代码立刻停止，相当于单步进入函数
		m_nCurCommand = IDB_STEPINTO;
		break;
	default:  //其它的是普通运行，不进行操作(SCRIPT_RUN_NORMAL,SCRIPT_RUN_CANSTOP)
		break;
	}

	return true;
}

// 获取运行模式
int __stdcall CRubyInterp::GetRunMode()
{
	return m_nRunMode;
}

// 设置运行状态
int __stdcall CRubyInterp::SetRunState(int nState)
{
	m_nRunState = nState;
	return true;
}

// 获取运行状态
int __stdcall CRubyInterp::GetRunState()
{
	return m_nRunState;
}

// 获取当前堆栈层数
int __stdcall CRubyInterp::GetCurStackLevel()
{
	// TODO: Add your code here
	int level = 1;
	try{
	//////////////////////////////
	VALUE vcaller = rb_funcall(rb_mKernel,  rb_intern("caller"), 0);

	VALUE vsize = rb_funcall(vcaller, rb_intern("size"), 0);

	level = NUM2INT(vsize);
	//////////////////////////////
	}catch(...)
	{
	}

	return level;
}

// 获取关键字列表
int __stdcall CRubyInterp::GetKeyWordList(LPCTSTR lpszType, CStringArray& asKeyWord)
{
	// TODO: Add your code here
	CString strType = lpszType;
	if(strType == "platform")	// 平台扩展关键字
	{
		//asKeyWord.Add("LangString");
	}else
	if(strType == "platform_keyword_tooltip")	// 平台扩展关键字使用说明
	{
		//asKeyWord.Add("DESCRIPTION:\r\n Set a language depend string\r\nSYNOPSIS:\r\n LangString strid language string");
	}

	return asKeyWord.GetSize();
}

// 设置调试事件
int __stdcall CRubyInterp::SetDebugEvent(int nEvent, int nParam, LPCTSTR lpszParam)
{
	// TODO: Add your code here
	CString strParam = lpszParam;

	m_nCurCommand = nEvent;
	::SetEvent(m_hDebugEvent);

	return TRUE;
}

// 设置断点列表
int __stdcall CRubyInterp::SetBreakList(LPCTSTR lpszFile, CUIntArray& auBreaks)
{
	// TODO: Add your code here
	m_auBreakList.RemoveAll();
	for(int i=0; i<auBreaks.GetSize(); i++)
	{
		m_auBreakList.Add(auBreaks[i]);
	}

	return 0;
}

// 获取断点列表
int __stdcall CRubyInterp::GetBreakList(LPCTSTR lpszFile, CUIntArray& auBreaks)
{
	// TODO: Add your code here
	auBreaks.RemoveAll();
	for(int i=0; i<m_auBreakList.GetSize(); i++)
	{
		auBreaks.Add(m_auBreakList[i]);
	}

	return 0;
}

// 获取是否允许覆盖率分析
int __stdcall CRubyInterp::GetEnableProfileAnalyse()
{
	// TODO: Add your code here
	return m_bProfileEnable;
}

// 获取有效代码行数
int __stdcall CRubyInterp::GetAvailableLineCount()
{
	// TODO: Add your code here
	return m_nTDLineCount;
}

// 获取代码运行覆盖表
LPVOID __stdcall CRubyInterp::GetProfileMap()
{
	// TODO: Add your code here
	return (void*)(&m_ProfileMap);
}

// 判断指定方法是否被支持
int __stdcall CRubyInterp::MethodIsSupport(LPCTSTR lpszMethod)
{
	// TODO: Add your code here
	CString strMethod = lpszMethod;
	if( (strMethod == "run") ||		// 支持运行
		(strMethod == "debug") ||	// 支持调试
		(strMethod == "break") ||	// 支持断点
		(strMethod == "compile") ||	// 支持编译
		(strMethod == "build") )	// 支持构建可执行程序
	{
		return TRUE;
	}

	return FALSE;
}

#define SEPATATOE "<<&@&>>"
// 初始化调试模式
int __stdcall CRubyInterp::InitialDebugMode(CString& strResult)
{
	// TODO: Add your code here
	// 初始化各个调试窗口
	CString strWinVar, strWinObj, strWinFunc, strWinStack;
	LOADRCSTRING( strWinVar, IDS_WINDOW_VAR );
	LOADRCSTRING( strWinObj, IDS_WINDOW_OBJ );
	LOADRCSTRING( strWinFunc, IDS_WINDOW_FUNC );
	LOADRCSTRING( strWinStack, IDS_WINDOW_STACK );

	CString strColName, strColValue, strColType, strColSource, strColLine, strColWhat;
	LOADRCSTRING( strColName, IDS_COLUMN_NAME );
	LOADRCSTRING( strColValue, IDS_COLUMN_VALUE );
	LOADRCSTRING( strColType, IDS_COLUMN_TYPE );
	LOADRCSTRING( strColSource, IDS_COLUMN_SOURCE );
	LOADRCSTRING( strColLine, IDS_COLUMN_LINE );
	LOADRCSTRING( strColWhat, IDS_COLUMN_WHAT );

	// 初始化变量窗口
	m_nOutputVar = ::atoi(pIPlatUI->OutputGet(-1, strWinVar));
	if(m_nOutputVar <= 0)
	{
		//新窗口，创建
		m_nOutputVar = pIPlatUI->AddOutput(strWinVar + "##GRID##" + strWinVar);
	}
	pIPlatUI->OutputSet(m_nOutputVar, "DelColumn ALL;DelItem ALL");
	pIPlatUI->OutputSet(m_nOutputVar, "AddColumn " + strColName + ",120");
	pIPlatUI->OutputSet(m_nOutputVar, "AddColumn " + strColValue + ",100");
	pIPlatUI->OutputSet(m_nOutputVar, "AddColumn " + strColType + ",60,AutoSize");
	pIPlatUI->OutputSet(m_nOutputVar, "SetTreeColumn 0");
	pIPlatUI->OutputSet(m_nOutputVar, "SetRowColors RGB(0,0,128) RGB(255,255,205)");
	pIPlatUI->OutputSet(m_nOutputVar, "SetGridColor RGB(192,192,192)");
	pIPlatUI->OutputSet(m_nOutputVar, "ModifyStyle Horizontal Solid;ModifyStyle Vertical Solid");
	pIPlatUI->OutputSet(m_nOutputVar, "SEPARATOR=" SEPATATOE); //设置分隔符
	m_nOutputVar = pIPlatUI->AddOutput(strWinVar + "##GRID");

	//pIPlatUI->OutputSet(m_nOutputVar, "DelItem ALL");

	// 初始化对象窗口
	m_nOutputObject = ::atoi(pIPlatUI->OutputGet(-1, strWinObj));
	if(m_nOutputObject <= 0)
	{
		//新窗口，创建
		m_nOutputObject = pIPlatUI->AddOutput(strWinObj + "##GRID##" + strWinVar);
	}
	pIPlatUI->OutputSet(m_nOutputObject, "DelColumn ALL;DelItem ALL");
	pIPlatUI->OutputSet(m_nOutputObject, "AddColumn " + strColName + ",120");
	pIPlatUI->OutputSet(m_nOutputObject, "AddColumn " + strColValue + ",100");
	pIPlatUI->OutputSet(m_nOutputObject, "AddColumn " + strColType + ",60,AutoSize");
	pIPlatUI->OutputSet(m_nOutputObject, "SetTreeColumn 0");
	pIPlatUI->OutputSet(m_nOutputObject, "SetRowColors RGB(0,0,128) RGB(255,255,205)");
	pIPlatUI->OutputSet(m_nOutputObject, "SetGridColor RGB(192,192,192)");
	pIPlatUI->OutputSet(m_nOutputObject, "ModifyStyle Horizontal Solid;ModifyStyle Vertical Solid");
	pIPlatUI->OutputSet(m_nOutputObject, "SEPARATOR=" SEPATATOE); //设置分隔符
	m_nOutputObject = pIPlatUI->AddOutput(strWinObj + "##GRID");

	//pIPlatUI->OutputSet(m_nOutputObject, "DelItem ALL");

	// 初始化函数窗口
	m_nOutputFunc = ::atoi(pIPlatUI->OutputGet(-1, strWinFunc));
	if(m_nOutputFunc <= 0)
	{
		//新窗口，创建
		m_nOutputFunc = pIPlatUI->AddOutput(strWinFunc + "##GRID##" + strWinVar);
	}
	pIPlatUI->OutputSet(m_nOutputFunc, "DelColumn ALL;DelItem ALL");
	pIPlatUI->OutputSet(m_nOutputFunc, "AddColumn " + strColName + ",200");
	pIPlatUI->OutputSet(m_nOutputFunc, "AddColumn " + strColType + ",80,AutoSize");
	pIPlatUI->OutputSet(m_nOutputFunc, "SetTreeColumn 0");
	pIPlatUI->OutputSet(m_nOutputFunc, "SetRowColors RGB(0,0,128) RGB(255,255,205)");
	pIPlatUI->OutputSet(m_nOutputFunc, "SetGridColor RGB(192,192,192)");
	pIPlatUI->OutputSet(m_nOutputFunc, "ModifyStyle Horizontal Solid;ModifyStyle Vertical Solid");
	pIPlatUI->OutputSet(m_nOutputFunc, "SEPARATOR=" SEPATATOE); //设置分隔符
	m_nOutputFunc = pIPlatUI->AddOutput(strWinFunc + "##GRID");

	//pIPlatUI->OutputSet(m_nOutputFunc, "DelItem ALL");

	// 初始化堆栈窗口
	m_nOutputStack = ::atoi(pIPlatUI->OutputGet(-1, strWinStack));
	if(m_nOutputStack <= 0)
	{
		//新窗口，创建
		m_nOutputStack = pIPlatUI->AddOutput(strWinStack + "##GRID##" + strWinVar);
	}
	pIPlatUI->OutputSet(m_nOutputStack, "DelColumn ALL;DelItem ALL");
	//pIPlatUI->OutputSet(m_nOutputStack, "AddColumn " + strColName + ",100");
	pIPlatUI->OutputSet(m_nOutputStack, "AddColumn " + strColSource + ",160");
	pIPlatUI->OutputSet(m_nOutputStack, "AddColumn " + strColLine + ",50");
	pIPlatUI->OutputSet(m_nOutputStack, "AddColumn " + strColWhat + ",80,AutoSize");
	//pIPlatUI->OutputSet(m_nOutputStack, "AddColumn " + strColType + ",60,AutoSize");
	pIPlatUI->OutputSet(m_nOutputStack, "SetTreeColumn 0");
	pIPlatUI->OutputSet(m_nOutputStack, "SetRowColors RGB(0,0,128) RGB(255,255,205)");
	pIPlatUI->OutputSet(m_nOutputStack, "SetGridColor RGB(192,192,192)");
	pIPlatUI->OutputSet(m_nOutputStack, "ModifyStyle Horizontal Solid;ModifyStyle Vertical Solid");
	pIPlatUI->OutputSet(m_nOutputStack, "ModifyStyle Add(NoSort)");
	pIPlatUI->OutputSet(m_nOutputStack, "SEPARATOR=" SEPATATOE); //设置分隔符
	m_nOutputStack = pIPlatUI->AddOutput(strWinStack + "##GRID");

	//pIPlatUI->OutputSet(m_nOutputStack, "DelItem ALL");

	// 激活变量窗口
	pIPlatUI->ActiveOutput(m_nOutputVar);

	return TRUE;
}

// 获取指定数组或哈希变量的完整路径名
CString get_var_fullname(CRubyInterp* pInterp, CString strhItem)
{
	CString strArrayHashName = "";
	CString strTmp;
	CString strhParent = "";
	while((strhParent = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, "ItemParent " + strhItem)) != "ROOT")
	{
		strTmp.Format("<#%s,2>", strhItem);
		CString strItemType = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, strTmp);
		if((strItemType == "Array") || (strItemType == "Hash"))
		{
			strTmp.Format("<#%s,0>", strhItem);
			strArrayHashName = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, strTmp) + strArrayHashName;
		}
		strhItem = strhParent;
	}

	strTmp.Format("<#%s,2>", strhItem);
	CString strItemType = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, strTmp);
	if((strItemType == "Array") || (strItemType == "Hash"))
	{
		strTmp.Format("<#%s,0>", strhItem);
		strArrayHashName = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, strTmp) + strArrayHashName;
	}

	return strArrayHashName;
}

// 变量窗口双击处理
void callback_ruby_ldclick(CRubyInterp* pInterp, LPCTSTR lpszInfo)
{
	TRACE("callback info=%s\n", lpszInfo);
	CString strInfo = lpszInfo;

	if(pInterp == NULL || g_pRubyInterp == NULL)
	{
		return;
	}

	int nPos = strInfo.Find(",");
	if(nPos == -1)
	{
		return;
	}

	LPVOID lpItem = (LPVOID)(atoi(strInfo.Left(nPos)));

	CString strTmp;
	strTmp.Format("<%d,0>", lpItem);
	CString strVarName = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, strTmp);
	strTmp.Format("<%d,1>", lpItem);
	CString strVarValue = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, strTmp);
	strTmp.Format("<%d,2>", lpItem);
	CString strVarType = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, strTmp);

	if(strVarType == "")
	{
		return;
	}

	if((strVarType == "Array") || (strVarType == "Hash"))
	{
		// 如果是Array或Hash,就展开
		CString strhItem = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, "SelItem");

		// 判断是局部变量还是全局变量
		CString strParentItem = strhItem;
		while(pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, "ItemParent " + strParentItem) != "ROOT")
		{
			strParentItem = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, "ItemParent " + strParentItem);
		}
		CString strLocalGlobal = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, "<#" + strParentItem + ",0>");
		BOOL bIsLocal = (strLocalGlobal == "Locals");

		//CString strArrayHashName = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, "<#" + strhItem + ",0>");
		CString strArrayHashName = get_var_fullname(pInterp, strhItem);
		int nChildCount = atoi(pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, "ChildCount " + strhItem));
		if(nChildCount == 0)
		{
			CString strTmp;

			int nError;
			//VALUE vvar = rb_eval_string(strArrayHashName);
			VALUE vvar = rb_eval_string_protect(strArrayHashName, &nError);
			if(nError != 0)
			{
				return;
			}

			CString strValue = "";

			VALUE vartype = rb_funcall(vvar,  rb_intern("class"), 0);
			if(vartype == rb_cArray)
			{
				VALUE vsize = rb_funcall(vvar,  rb_intern("size"), 0);
				int nSize = NUM2INT(vsize);
				for(int i=0; i<nSize; i++)
				{
					// 数组元素值
					VALUE vitem = rb_funcall(vvar,  rb_intern("at"), 1, INT2NUM(i));
					VALUE strtmp = rb_obj_as_string(vitem);
					VALUE str = StringValue(strtmp);
					CString strValue = RSTRING(str)->ptr;

					// 数组元素类型
					VALUE vartype = rb_funcall(vitem,  rb_intern("class"), 0);
					strtmp = rb_obj_as_string(vartype);
					str = StringValue(strtmp);
					CString strType = RSTRING(str)->ptr;

					strTmp.Format("<%s,7>[%d]%s%s%s%s", strhItem,
							i, SEPATATOE,
							strValue, SEPATATOE,
							strType);
					pInterp->pIPlatUI->Output(pInterp->m_nOutputVar, strTmp);

					// 设置双击处理函数
					CString strlasthItem = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, "LastItem");
					pInterp->pIPlatUI->Output(pInterp->m_nOutputVar, "<I>Set Script<#" + strlasthItem + "> <1>");
				}
				pInterp->pIPlatUI->Output(pInterp->m_nOutputVar, "<T>ExpandItem<#" + strhItem + ">");
			}else
			if(vartype == rb_cHash)
			{
				VALUE vkeyarray = rb_funcall(vvar,  rb_intern("keys"), 0);
				VALUE vsize = rb_funcall(vkeyarray,  rb_intern("size"), 0);
				int nSize = NUM2INT(vsize);
				for(int i=nSize-1; i>=0; i--)
				{
					// Key
					VALUE vitem = rb_funcall(vkeyarray,  rb_intern("at"), 1, INT2NUM(i));
					VALUE strtmp = rb_obj_as_string(vitem);
					VALUE str = StringValue(strtmp);
					CString strKey = RSTRING(str)->ptr;
					CString strKeyShow = strKey;	// 显示的Key

					// Key类型
					VALUE vartype = rb_funcall(vitem,  rb_intern("class"), 0);
					strtmp = rb_obj_as_string(vartype);
					str = StringValue(strtmp);
					CString strKeyType = RSTRING(str)->ptr;
					if(strKeyType == "String")
					{
						strKeyShow.Format("'%s'", strKey);
					}

					// Value
					VALUE varvalue = rb_funcall(vvar,  rb_intern("fetch"), 1, vitem);
					strtmp = rb_obj_as_string(varvalue);
					str = StringValue(strtmp);
					CString strValue = RSTRING(str)->ptr;

					// Value类型
					VALUE valuetype = rb_funcall(varvalue,  rb_intern("class"), 0);
					strtmp = rb_obj_as_string(valuetype);
					str = StringValue(strtmp);
					CString strValueType = RSTRING(str)->ptr;

					strTmp.Format("<%s,7>[%s]%s%s%s%s", strhItem,
							strKeyShow, SEPATATOE,
							strValue, SEPATATOE,
							strValueType);
					pInterp->pIPlatUI->Output(pInterp->m_nOutputVar, strTmp);

					// 设置双击处理函数
					CString strlasthItem = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, "LastItem");
					pInterp->pIPlatUI->Output(pInterp->m_nOutputVar, "<I>Set Script<#" + strlasthItem + "> <1>");
				}
				pInterp->pIPlatUI->Output(pInterp->m_nOutputVar, "<T>ExpandItem<#" + strhItem + ">");
			}
		}
	}else
	{
		DLL_LOADSTRING(strWatchVarTitle, IDS_WATCH_VAR_TITLE);
		DLL_LOADSTRING(strWatchVarName, IDS_WATCH_VAR_NAME);
		DLL_LOADSTRING(strWatchVarValue, IDS_WATCH_VAR_VALUE);
		DLL_LOADSTRING(strWatchVarType, IDS_WATCH_VAR_TYPE);
		CString strhItem = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, "SelItem");
		strVarName = get_var_fullname(pInterp, strhItem) + strVarName;
		CString strMsg;
		strMsg.Format("%s:%s\n%s:%s\n%s:%s", strWatchVarName, strVarName, strWatchVarValue, strVarValue, strWatchVarType, strVarType);
		::MessageBox(AfxGetMainWnd()->m_hWnd, strMsg, strWatchVarTitle, MB_OK | MB_ICONINFORMATION);
	}
}

// 刷新变量窗口
int CRubyInterp::RefreshVarWindow()
{
	CString strOutput, strName, strValue, strType;
	int nParent;

	CTTreeItemArray aItems;

	TTREEITEM item;
	item.hItem = NULL;
	item.nParent = -1;
	item.nExpand = 0;

	// Locals
	strOutput = "Locals";
	item.strLine = strOutput;
	item.nParent = -1;
	item.nImage = 4;
	item.nExpand = 1;
	item.nClickFunc = 0;
	aItems.Add(item);
	nParent = aItems.GetSize()-1;

	VALUE vlocals = rb_funcall(rb_mKernel,  rb_intern("local_variables"), 0);

	VALUE vsize = rb_funcall(vlocals,  rb_intern("size"), 0);
	int nSize = NUM2INT(vsize);
	for(int i=0; i<nSize; i++)
	{
		VALUE varname = rb_funcall(vlocals,  rb_intern("at"), 1, INT2NUM(i));
		VALUE strtmp = rb_obj_as_string(varname);	// 不论是什么类型，都先转换为字符串类型
		VALUE str = StringValue(strtmp);
		strName = RSTRING(str)->ptr;
		if(strName.Find("platform_temp_") == 0)
		{
			// 过滤平台使用的临时变量
			continue;
		}
		CString strCmd;
		strCmd.Format("%s", strName);
		VALUE vvar = rb_eval_string(strCmd);
		strtmp = rb_obj_as_string(vvar);
		str = StringValue(strtmp);
		strValue = RSTRING(str)->ptr;
		VALUE vartype = rb_funcall(vvar,  rb_intern("class"), 0);
		strtmp = rb_obj_as_string(vartype);
		str = StringValue(strtmp);
		strType = RSTRING(str)->ptr;

		strOutput =	strName + SEPATATOE + strValue + SEPATATOE + strType;
		item.strLine = strOutput;
		item.nParent = nParent;
		item.nImage = 5;
		item.nExpand = 0;
		item.nClickFunc = 1;
		aItems.Add(item);
	}
/*
	// 实例变量
	strOutput = "Self";
	item.strLine = strOutput;
	item.nParent = -1;
	item.nImage = 4;
	item.nExpand = 1;
	item.nClickFunc = 0;
	aItems.Add(item);
	nParent = aItems.GetSize()-1;

	VALUE vinstances = rb_eval_string("instance_variables");
	//VALUE vinstances = rb_funcall(m_vDebugCurBinding,  rb_intern("instance_variables"), 0);

	//VALUE vinstances = m_vDebugCurBinding;
	vsize = rb_funcall(vinstances,  rb_intern("size"), 0);
	nSize = NUM2INT(vsize);
	for(i=0; i<nSize; i++)
	{
		VALUE varname = rb_funcall(vinstances,  rb_intern("at"), 1, INT2NUM(i));
		VALUE strtmp = rb_obj_as_string(varname);	// 不论是什么类型，都先转换为字符串类型
		VALUE str = StringValue(strtmp);
		strName = RSTRING(str)->ptr;
		VALUE vvar = rb_eval_string(strName);
		strtmp = rb_obj_as_string(vvar);
		str = StringValue(strtmp);
		strValue = RSTRING(str)->ptr;
		VALUE vartype = rb_funcall(vvar,  rb_intern("class"), 0);
		strtmp = rb_obj_as_string(vartype);
		str = StringValue(strtmp);
		strType = RSTRING(str)->ptr;

		strOutput =	strName + SEPATATOE + strValue + SEPATATOE + strType;
		item.strLine = strOutput;
		item.nParent = nParent;
		item.nImage = 5;
		item.nExpand = 0;
		item.nClickFunc = 1;
		aItems.Add(item);
	}
*/
	// Globals
	strOutput = "Globals";
	item.strLine = strOutput;
	item.nParent = -1;
	item.nImage = 4;
	item.nExpand = 1;
	item.nClickFunc = 0;
	aItems.Add(item);
	nParent = aItems.GetSize()-1;

	VALUE vglobals = rb_funcall(rb_mKernel,  rb_intern("global_variables"), 0);

	vsize = rb_funcall(vglobals,  rb_intern("size"), 0);
	nSize = NUM2INT(vsize);
	for(int i=0; i<nSize; i++)
	{
		VALUE varname = rb_funcall(vglobals,  rb_intern("at"), 1, INT2NUM(i));
		VALUE strtmp = rb_obj_as_string(varname);	// 不论是什么类型，都先转换为字符串类型
		VALUE str = StringValue(strtmp);
		strName = RSTRING(str)->ptr;
		VALUE vvar = rb_gv_get(strName);
		strtmp = rb_obj_as_string(vvar);
		str = StringValue(strtmp);
		strValue = RSTRING(str)->ptr;
		VALUE vartype = rb_funcall(vvar,  rb_intern("class"), 0);
		strtmp = rb_obj_as_string(vartype);
		str = StringValue(strtmp);
		strType = RSTRING(str)->ptr;

		strOutput =	strName + SEPATATOE + strValue + SEPATATOE + strType;
		item.strLine = strOutput;
		item.nParent = nParent;
		item.nImage = 5;
		item.nExpand = 0;
		item.nClickFunc = 1;
		aItems.Add(item);
	}

	pIPlatUI->SendCommand(OC_TREEOUTPUT, m_nOutputVar, (LPARAM)(LPVOID)(&aItems));

	CString strCallback;
	strCallback.Format("<I>Set Script<0> <<cfunction:%d>>%d,%d", (int)callback_ruby_ldclick, (int)this, 1);
	pIPlatUI->Output(m_nOutputVar, strCallback);

	return TRUE;
}

// 刷新函数窗口
int CRubyInterp::RefreshFuncWindow()
{
	CString strOutput, strName, strType;

	CTTreeItemArray aItems;

	TTREEITEM item;
	item.hItem = NULL;
	item.nParent = -1;
	item.nExpand = 0;

	VALUE vmethods = rb_funcall(rb_mKernel,  rb_intern("methods"), 0);

	VALUE vsize = rb_funcall(vmethods, rb_intern("size"), 0);

	int nSize = NUM2INT(vsize);
	for(int i=0; i<nSize; i++)
	{
		VALUE vmethod = rb_funcall(vmethods,  rb_intern("at"), 1, INT2NUM(i));
		VALUE strtmp = rb_obj_as_string(vmethod);
		VALUE str = StringValue(strtmp);
		CString strMethod = RSTRING(str)->ptr;

		CString strType = "public";

		item.strLine = strMethod + SEPATATOE + strType;
		aItems.Add(item);
	}

	pIPlatUI->SendCommand(OC_TREEOUTPUT, m_nOutputFunc, (LPARAM)(LPVOID)(&aItems));

	return TRUE;
}

// Ruby堆栈窗口行双击处理
void callback_ruby_stack_ldclick(CRubyInterp* pInterp, LPCTSTR lpszInfo)
{
	TRACE("callback info=%s\n", lpszInfo);
	CString strInfo = lpszInfo;

	if(pInterp == NULL || g_pRubyInterp == NULL)
	{
		return;
	}

	int nPos = strInfo.Find(",");
	if(nPos == -1)
	{
		return;
	}

	LPVOID lpItem = (LPVOID)(atoi(strInfo.Left(nPos)));

	CString strTmp;
	strTmp.Format("<%d,0>", lpItem);
	CString strStackSource = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputStack, strTmp);
	strTmp.Format("<%d,1>", lpItem);
	int nStackLine = atoi(pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputStack, strTmp));

	if(GetFileAttributes(strStackSource) == 0xFFFFFFFF)
	{
		return;
	}

	pInterp->pIPlatUI->SendOwmCmd("org.owm.edit", OC_OPENFILE, (WPARAM)(LPTSTR)(LPCTSTR)strStackSource, 0, NULL);
	pInterp->pIPlatUI->SendOwmCmd("org.owm.edit", OC_SETCURENTLINE, (WPARAM)(LPTSTR)(LPCTSTR)strStackSource, nStackLine, NULL);
}

// 刷新堆栈窗口
int CRubyInterp::RefreshStackWindow()
{
	CString strOutput;

	CTTreeItemArray aItems;

	TTREEITEM item;
	item.hItem = NULL;
	item.nParent = -1;
	item.nExpand = 0;
	item.nImage = 27;
	item.nClickFunc = 1;

	VALUE vcaller = rb_funcall(rb_mKernel,  rb_intern("caller"), 0);

	VALUE vsize = rb_funcall(vcaller, rb_intern("size"), 0);

	int nLevel = NUM2INT(vsize);
	for(int i=0; i<nLevel; i++)
	{
		VALUE vstack = rb_funcall(vcaller,  rb_intern("at"), 1, INT2NUM(i));
		VALUE strtmp = rb_obj_as_string(vstack);
		VALUE str = StringValue(strtmp);
		CString strStack = RSTRING(str)->ptr;

		strOutput = "";

		int nPos = strStack.Find(":");
		if(nPos == 1)
		{
			nPos = strStack.Find(":", 2);
		}
		if(nPos != -1)
		{
			strOutput += strStack.Left(nPos);
			strStack.Delete(0, nPos+1);
			strOutput += SEPATATOE;
			nPos = strStack.Find(":");
			if(nPos != -1)
			{
				strOutput += strStack.Left(nPos);
				strStack.Delete(0, nPos+1);
				strOutput += SEPATATOE;
				strOutput += strStack;
			}else
			{
				strOutput += strStack;
				strOutput += SEPATATOE;
			}
		}else
		{
			strOutput += strStack;
			strOutput += SEPATATOE;
			strOutput += SEPATATOE;
		}

		item.strLine = strOutput;
		aItems.Add(item);
	}

	pIPlatUI->SendCommand(OC_TREEOUTPUT, m_nOutputStack, (LPARAM)(LPVOID)(&aItems));

	CString strCallback;
	strCallback.Format("<I>Set Script<0> <<cfunction:%d>>%d,%d", (int)callback_ruby_stack_ldclick, (int)this, 1);
	pIPlatUI->Output(m_nOutputStack, strCallback);

	return TRUE;
}

// 更新调试窗口信息
int __stdcall CRubyInterp::RefreshDebugWindow(LPCTSTR lpszWindow, BOOL bActive, CString& strResult)
{
	// TODO: Add your code here
	CString strWindow = lpszWindow;

	BOOL bWindowVar = FALSE;
	BOOL bWindowObject = FALSE;
	BOOL bWindowProc = FALSE;
	BOOL bWindowStack = FALSE;
	if(strWindow == "var")
	{
		bWindowVar = TRUE;
		m_nRefreshDebugWindow = 1;
	}else
	if(strWindow == "object")
	{
		bWindowObject = TRUE;
		m_nRefreshDebugWindow = 2;
	}else
	if(strWindow == "proc")
	{
		bWindowProc = TRUE;
		m_nRefreshDebugWindow = 3;
	}else
	if(strWindow == "stack")
	{
		bWindowStack = TRUE;
		m_nRefreshDebugWindow = 4;
	}else
	if(strWindow == "all")
	{
		bWindowVar = TRUE;
		bWindowObject = TRUE;
		bWindowProc = TRUE;
		bWindowStack = TRUE;
	}

	if(bWindowVar)
	{
		if(strWindow == "all")
		{
			RefreshVarWindow();
		}
		if(bActive)
		{
			pIPlatUI->ActiveOutput(m_nOutputVar);
		}
	}
	if(bWindowObject)
	{
		if(strWindow == "all")
		{
		}
		if(bActive)
		{
			pIPlatUI->ActiveOutput(m_nOutputObject);
		}
	}
	if(bWindowProc)
	{
		if(strWindow == "all")
		{
			RefreshFuncWindow();
		}
		if(bActive)
		{
			pIPlatUI->ActiveOutput(m_nOutputFunc);
		}
	}
	if(bWindowStack)
	{
		if(strWindow == "all")
		{
			RefreshStackWindow();
		}
		if(bActive)
		{
			pIPlatUI->ActiveOutput(m_nOutputStack);
		}
	}

	// 如果是刷新某个调试窗口,就发调试消息,由调试线程来刷新
	if(strWindow != "all")
	{
		m_nCurCommand = IDB_REFRESH_DEBUGWINDOW;
		::SetEvent(m_hDebugEvent);
	}

	return TRUE;
}

// 调用脚本方法
int __stdcall CRubyInterp::CallMethod(LPCTSTR lpszMethod, CStringArray& asParam)
{
	// TODO: Add your code here
	CString strCmd = lpszMethod;
	for(int i=0; i<asParam.GetSize(); i++)
	{
		if((asParam[i].Find('"') != 0) && (asParam[i].ReverseFind('"') != (asParam[i].GetLength()-1)))
		{
			strCmd += " \"";
			strCmd += asParam[i];
			strCmd += "\"";
		}else
		{
			strCmd += " ";
			strCmd += asParam[i];
		}
	}

	return RunScriptCommand(strCmd);
}

// 编译
int __stdcall CRubyInterp::Compile(LPCTSTR lpszSrcFile, CString& strDestFile, LPCTSTR lpszParams)
{
	// TODO: Add your code here
	CString strSrcFile = lpszSrcFile;
	if(strSrcFile == "")
	{
		strSrcFile = m_strScriptFile;
	}

	if(strSrcFile == "")
	{
		return FALSE;
	}

	return 0;
}

// 选择可执行文件类型
int CRubyInterp::SelectSetupType()
{
	CTOptArray aopt;	// 创建属性树元素数组
	TOPTITEM item;		// 属性树元素
	item.hItem		= NULL;
	item.iType		= TOPT_GROUP;
	CString strTemp;
	LOADRCSTRING( strTemp, IDS_OPTDLG_SELECT_SETUP_TYPE)
	strTemp = strTemp;//"选择可执行文件类型";
	item.lpszText	= strTemp;
	item.lpszEdit	= "";
	item.lpszList	= "";
	item.bCheck		= FALSE;
	item.iImage		= TOPTI_FOLDER;
	item.iParent	= -1;
	aopt.Add(item);		// 添加根节点

	item.iParent	= 0;
	item.iType		= TOPT_RADIOBUTTON;
	//item.iImage		= TOPTI_INFO;
	item.bCheck		= TRUE;
	item.lpszText	= "Console Application";
	aopt.Add(item);
	item.bCheck		= FALSE;
	item.lpszText	= "GUI Application";
	aopt.Add(item);
	item.lpszText	= "Tk Application";
	aopt.Add(item);

	// 添加标题和提示信息
	item.iType		= TOPT_MSG;
	LOADRCSTRING( strTemp, IDS_OPTDLG_SELECT_SETUP_TYPE_TITLE);
	item.lpszText	= strTemp;//"选择可执行文件类型";
	LOADRCSTRING( strTemp, IDS_OPTDLG_SELECT_SETUP_TYPE_MSG);
	item.lpszEdit	= strTemp;//"请选择可执行文件类型，会按照的选择的类型生成可执行文件";
	item.iParent	= -1;
	aopt.Add(item);

	int nRet = -1;

	try{
		pIPlatUI->SendOwmCmd("", OC_EXECTREEOPTION, 0, (LPARAM)(&aopt), &nRet);
	}catch(...)
	{
		return -1;
	}

	if(nRet != IDOK)
	{
		return -1;
	}

	for(int i=0; i<aopt.GetSize(); i++)
	{
		if(TOPT_RADIOBUTTON == aopt[i].iType)
		{
			if(aopt[i].bCheck)
			{
				return i-1;
			}
		}
	}
	
	return -1;
}

// 构建
int __stdcall CRubyInterp::Build(LPCTSTR lpszSrcFile, CString& strDestFile, LPCTSTR lpszParams)
{
	// TODO: Add your code here
	CString strSrcFile = lpszSrcFile;
	if(strSrcFile == "")
	{
		strSrcFile = m_strScriptFile;
	}

	if(strSrcFile == "")
	{
		return FALSE;
	}

	// 输出文件名
	CString strOutFile = strSrcFile;
	CString strOutFileName = ""; // 文件名,不带路径
	strOutFile.MakeLower();
	strOutFile.Replace("/", "\\");
	int nPos = strOutFile.ReverseFind('\\');
	if(nPos != -1)
	{
		strOutFileName = strOutFile;
		strOutFileName.Delete(0, nPos+1);
		strOutFile = strOutFile.Left(nPos);
	}
	int nDotPos = strOutFileName.ReverseFind('.');
	if(nDotPos != -1)
	{
		strOutFileName.Delete(nDotPos, strOutFileName.GetLength() - nDotPos);
	}

	// rubyscript2exe.rb文件名(此脚本中自带了图标文件资源)
	CString strRuby2ExeRb = GetInterpRootPath();
	strRuby2ExeRb += "bin\\rubyscript2exe.rb";

	//CString strCmd;
	//strCmd.Format("ARGV << \"--rubyscript2exe-tk\"");
	CString strParam = "";

	// 选择文件类型
	BOOL bSelectSetupType = AfxGetApp()->GetProfileInt(REG_RUBY_BUILD_SUBKEY, REG_RUBY_BUILD_SELECT_TYPE, TRUE);
	int nSelectSetupType = 0;
	if(bSelectSetupType)
	{
		nSelectSetupType = SelectSetupType();
		if(nSelectSetupType == -1)
		{
			return FALSE;
		}
	}

	switch(nSelectSetupType)
	{
	case 0:break;										// CLI application
	case 1:strParam = "--rubyscript2exe-rubyw";break;	// GUI application
	case 2:strParam = "--rubyscript2exe-rubyw --rubyscript2exe-tk";break;		// Tk application
	}

	// 记录当前路径,并设置当前路径
	char szOldPath[256];
	memset(szOldPath, 0, 256);
	DWORD dwLen = GetCurrentDirectory(255, szOldPath);
	CString strPath = strSrcFile;
	strPath.Replace("/", "\\");
	nPos = strPath.ReverseFind('\\');
	if(nPos >= 0)
	{
		strPath = strPath.Left(nPos);
	}
	SetCurrentDirectory(strPath);

	// 执行rubyscript2exe.rb,使用解释器bin目录下的ruby.exe来运行脚本
	CString strCmd;
	CString strRubyRunFile	= GetInterpRootPath();
	strRubyRunFile	+= RUBY_LIB_PATH;
	strRubyRunFile	+= "\\bin\\ruby.exe";
	strCmd.Format("\"%s\" \"%s\"", strRuby2ExeRb, strSrcFile);
	if(strParam != "")
	{
		strCmd += " ";
		strCmd += strParam;
	}
	if(!RunProgramWithPipe(strRubyRunFile, strCmd))
	{
		return FALSE;
	}

	strDestFile = strOutFile + "\\" + strOutFileName + ".exe";

	// 替换可执行文件的图标
	CString strIconFile = GetInterpRootPath();
	strIconFile += "bin\\rubyscript2exe.ico";	// rubyscript2exe默认的图标文件
	strIconFile.Replace("/", "\\");
	BOOL bReplaceIcon = AfxGetApp()->GetProfileInt(REG_RUBY_BUILD_SUBKEY, REG_RUBY_BUILD_REPLACE_ICON, FALSE);
	if(bReplaceIcon)
	{
		// 图标选择的方式
		CString strNewIcon = "";
		int nSelectIconType = AfxGetApp()->GetProfileInt(REG_RUBY_BUILD_SUBKEY, REG_RUBY_BUILD_SELECT_ICON_TYPE, 0);
		if(nSelectIconType == 0)	// 使用源代码目录下的同名图标文件
		{
			strNewIcon = strOutFile + "\\" + strOutFileName + ".ico";
		}else
		if(nSelectIconType == 1)	// 用户选择图标文件
		{
			// 用户输入输出文件
			CFileDialog dialog(FALSE, "*.ico", "", OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, "Icon files(*.ico)|*.ico||");
			if(IDOK == dialog.DoModal())
			{
				strNewIcon = dialog.GetPathName();
			}else
			{
				if(pIPlatUI)
				{
					pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=255,0,0");
					pIPlatUI->Output(_WND_DEBUG, "Not select program icon, use default icon!\r\n");
					pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=0,0,0");
				}
				return TRUE;
			}
		}
		if(GetFileAttributes(strNewIcon) == 0xFFFFFFFF)
		{
			if(pIPlatUI)
			{
				pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=255,0,0");
				pIPlatUI->Output(_WND_DEBUG, "Not exist specified icon file, use default icon!\r\n");
				pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=0,0,0");
			}
			return TRUE;
		}

		// 获取替换图标脚本并加载执行
		CString strIconReplaceScript = GetPlatRootPath();
		strIconReplaceScript += _T("interps\\tcl\\bin\\iconreplace.tcl");
		strIconReplaceScript.Replace("\\", "/");
		CString strRes;
		CString strCmd;
		strCmd.Format("source %s;set filename {%s};set newicon {%s}; set fwname {%s};IconReplace $filename $newicon $fwname",
			strIconReplaceScript,
			strDestFile,
			strNewIcon,
			strIconFile);

		if(pIPlatUI)
		{
			IInterp* pInterp = (IInterp*)(pIPlatUI->CreateVciObject("org.interp.tcl", ""));
			if(pInterp)
			{
				pInterp->RunScriptCommand(strCmd);
				int nLine = pInterp->GetErrorLine();
				if(nLine != 0)
				{
					CString strResult = pInterp->GetResult();
					AfxMessageBox(strResult);
				}
				pIPlatUI->ReleaseVciObject(pInterp, TRUE);
			}
		}
	}

	return TRUE;
}
//}}VCI_IMPLEMENT_END

//////////////////////////////////////////////////////////////////////////
// 接口实现
//////////////////////////////////////////////////////////////////////////
INTERFACE_IMPLEMENT(RubyInterp)
CONTROL_INTERFACE_IMPLEMENT(RubyInterp)

//{{ 注意：!!!这里是保留的重要函数，不可删除!!!
// 获取接口的额外信息
LPVOID __stdcall CRubyInterp::XRubyInterp::GetExtraInfo(LPVOID lpRefData)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj;
}
//}} 注意：!!!这里是保留的重要函数，不可删除!!!
//{{VCI_INTERFACE_IMPLEMENT_BEGIN
// 设置平台UI接口指针
int __stdcall CRubyInterp::XRubyInterp::SetIPlatUI(LPVOID lpIPlatUI)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->SetIPlatUI(lpIPlatUI);
}

int __stdcall CRubyInterp::XRubyInterp::SetLibPath(LPCTSTR lpszPath)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->SetLibPath(lpszPath);
}

int __stdcall CRubyInterp::XRubyInterp::IsInterpRunning()
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->IsInterpRunning();
}

int __stdcall CRubyInterp::XRubyInterp::SetScriptFile(LPCTSTR lpszPathName)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->SetScriptFile(lpszPathName);
}

CString __stdcall CRubyInterp::XRubyInterp::GetScriptFile()
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->GetScriptFile();
}

int __stdcall CRubyInterp::XRubyInterp::RunScriptFile(LPCTSTR lpszPathName)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->RunScriptFile(lpszPathName);
}

int __stdcall CRubyInterp::XRubyInterp::RunScriptCommand(LPCTSTR lpszTclCmd)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->RunScriptCommand(lpszTclCmd);
}

int __stdcall CRubyInterp::XRubyInterp::StopScript()
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->StopScript();
}

LPVOID __stdcall CRubyInterp::XRubyInterp::GetInterp()
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->GetInterp();
}

int __stdcall CRubyInterp::XRubyInterp::GetInterpID()
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->GetInterpID();
}

void __stdcall CRubyInterp::XRubyInterp::SetInterpName(LPCTSTR lpszInterptName)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	pObj->SetInterpName(lpszInterptName);
}

CString __stdcall CRubyInterp::XRubyInterp::GetInterpName()
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->GetInterpName();
}

int __stdcall CRubyInterp::XRubyInterp::GetErrorLine()
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->GetErrorLine();
}

CString __stdcall CRubyInterp::XRubyInterp::GetResult()
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->GetResult();
}

int __stdcall CRubyInterp::XRubyInterp::SetVar(LPCTSTR lpszVarName, LPCTSTR lpszVarValue)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->SetVar(lpszVarName, lpszVarValue);
}

CString __stdcall CRubyInterp::XRubyInterp::GetVar(LPCTSTR lpszVarName)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->GetVar(lpszVarName);
}

int __stdcall CRubyInterp::XRubyInterp::RegisterCommand(LPVOID pInterp, LPCTSTR lpszCmd, LPVOID lpFunc)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->RegisterCommand(pInterp, lpszCmd, lpFunc);
}

int __stdcall CRubyInterp::XRubyInterp::SetInterpParam(LPCTSTR lpszParamName, LPCTSTR lpszParamValue)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->SetInterpParam(lpszParamName, lpszParamValue);
}

CString __stdcall CRubyInterp::XRubyInterp::GetInterpParam(LPCTSTR lpszParamName)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->GetInterpParam(lpszParamName);
}

int __stdcall CRubyInterp::XRubyInterp::SetRunMode(int nRunMode)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->SetRunMode(nRunMode);
}

int __stdcall CRubyInterp::XRubyInterp::GetRunMode()
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->GetRunMode();
}

int __stdcall CRubyInterp::XRubyInterp::SetRunState(int nRunState)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->SetRunState(nRunState);
}

int __stdcall CRubyInterp::XRubyInterp::GetRunState()
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->GetRunState();
}

int __stdcall CRubyInterp::XRubyInterp::GetCurStackLevel()
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->GetCurStackLevel();
}

// 获取关键字列表
int __stdcall CRubyInterp::XRubyInterp::GetKeyWordList(LPCTSTR lpszType, CStringArray& asKeyWord)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->GetKeyWordList(lpszType, asKeyWord);
}

int __stdcall CRubyInterp::XRubyInterp::SetDebugEvent(int nEvent, int nParam, LPCTSTR lpszParam)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->SetDebugEvent(nEvent, nParam, lpszParam);
}

int __stdcall CRubyInterp::XRubyInterp::SetBreakList(LPCTSTR lpszFile, CUIntArray& auBreaks)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->SetBreakList(lpszFile, auBreaks);
}

int __stdcall CRubyInterp::XRubyInterp::GetBreakList(LPCTSTR lpszFile, CUIntArray& auBreaks)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->GetBreakList(lpszFile, auBreaks);
}

int __stdcall CRubyInterp::XRubyInterp::GetEnableProfileAnalyse()
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->GetEnableProfileAnalyse();
}

int __stdcall CRubyInterp::XRubyInterp::GetAvailableLineCount()
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->GetAvailableLineCount();
}

LPVOID __stdcall CRubyInterp::XRubyInterp::GetProfileMap()
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->GetProfileMap();
}

int __stdcall CRubyInterp::XRubyInterp::MethodIsSupport(LPCTSTR lpszMethod)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->MethodIsSupport(lpszMethod);
}

int __stdcall CRubyInterp::XRubyInterp::InitialDebugMode(CString& strResult)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->InitialDebugMode(strResult);
}

int __stdcall CRubyInterp::XRubyInterp::RefreshDebugWindow(LPCTSTR lpszWindow, BOOL bActive, CString& strResult)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->RefreshDebugWindow(lpszWindow, bActive, strResult);
}

int __stdcall CRubyInterp::XRubyInterp::CallMethod(LPCTSTR lpszMethod, CStringArray& asParam)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->CallMethod(lpszMethod, asParam);
}

int __stdcall CRubyInterp::XRubyInterp::Compile(LPCTSTR lpszSrcFile, CString& strDestFile, LPCTSTR lpszParams)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->Compile(lpszSrcFile, strDestFile, lpszParams);
}

int __stdcall CRubyInterp::XRubyInterp::Build(LPCTSTR lpszSrcFile, CString& strDestFile, LPCTSTR lpszParams)
{
	CRubyInterp *pObj = GET_INTERFACE_OBJECT(RubyInterp);
	return pObj->Build(lpszSrcFile, strDestFile, lpszParams);
}
//}}VCI_INTERFACE_IMPLEMENT_END

// 查询命令是否支持
extern "C" __declspec(dllexport)
int gQueryCommandIsSupport(LPCSTR lpcsCmd)
{
	//if(strcmp(lpcsCmd, "HELLO") == 0)
	//{
	//}
	return 0;
}
