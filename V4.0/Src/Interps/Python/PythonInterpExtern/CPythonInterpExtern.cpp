#include "stdafx.h"
#include "CPythonInterpExtern.h"

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

// 获取键值对中指定的值
BOOL GetStringKeyValue(CString strStr, CString strKey, CString& strValue, CString strSep = ",")
{
	CStringArray asSubStr;
	int nPos = strStr.Find(strSep);
	while(nPos != -1)
	{
		asSubStr.Add(strStr.Left(nPos));
		strStr.Delete(0, nPos+1);
		nPos = strStr.Find(strSep);
	}
	asSubStr.Add(strStr);

	for(int i=0; i<asSubStr.GetSize(); i++)
	{
		CString strSubStr = asSubStr[i];
		int nPos = strSubStr.Find("=");
		if(nPos != -1)
		{
			CString _strKey = strSubStr.Left(nPos);
			strSubStr.Delete(0, nPos+1);
			CString _strValue = strSubStr;
			if(_strKey == strKey)
			{
				strValue = _strValue;
				return TRUE;
			}
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// 解释器操作
//////////////////////////////////////////////////////////////////////
static CPtrList	g_InterpList;	// 解释器对象列表
static int g_InterpID = 0x100;	// 解释器ID计数器
static CPythonInterpExtern* g_interp_instance = NULL;

//////////////////////////////////////////////////////////////////////////
// 功能实现
//////////////////////////////////////////////////////////////////////////
CPythonInterpExtern::CPythonInterpExtern()
{
	g_interp_instance = this;
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
	m_hPipeWrite	= NULL;
	m_hPipeRead		= NULL;
	g_InterpList.AddTail(this);
}

CPythonInterpExtern::~CPythonInterpExtern()
{
	// 清空覆盖率分析哈希表
	m_ProfileMap.RemoveAll();

	try {
	}catch(...)
	{
	}

	// 删除解释器对象列表中的项
	int count = g_InterpList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_InterpList.FindIndex(i);
		CPythonInterpExtern* pInterp = (CPythonInterpExtern*)g_InterpList.GetAt(pos);
		if(pInterp == this) 
		{
			g_InterpList.RemoveAt(pos);
			return;
		}
	}
}

CPythonInterpExtern* CPythonInterpExtern::Instance()
{
	return g_interp_instance;
}

/////////////////////////////////////////////////////////////////////////////
// 安装回调函数
/////////////////////////////////////////////////////////////////////////////
int CPythonInterpExtern::InstallHandler(DWORD nEventID, LPVOID lpHandler, DWORD dwRefData)
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
int CPythonInterpExtern::UninstallHandler(DWORD nEventID, LPVOID lpHandler)
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

/////////////////////////////////////////////////////////////////////////////
// 消息处理
/////////////////////////////////////////////////////////////////////////////
int CPythonInterpExtern::ProcessMessage(CVciMessage* pIn, CVciMessage* ppOut)
{
	return 0;
}


//{{VCI_IMPLEMENT_BEGIN
int __stdcall CPythonInterpExtern::SetIPlatUI(LPVOID lpIPlatUI)
{
	pIPlatUI = (IPlatUI*)lpIPlatUI;

	// 设置语言变量
	CString strCurrentLanguage = _T("english");
	if(VCI_LANGUAGE_CHINESE == GetLanguage())
	{
		strCurrentLanguage = _T("chinese");
	}

	// TODO: Add your code here

	return 0;
}

// 设置库路径
int __stdcall CPythonInterpExtern::SetLibPath(LPCTSTR lpszPath)
{
	// TODO: Add your code here

	return 0;
}

// 解释器是否在运行
int __stdcall CPythonInterpExtern::IsInterpRunning()
{
	// TODO: Add your code here
	// 判断所有解释器是否都已经终止
	int count = g_InterpList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_InterpList.FindIndex(i);
		CPythonInterpExtern* pInterp = (CPythonInterpExtern*)g_InterpList.GetAt(pos);
		int nRunState = pInterp->GetRunState();
		if( (SCRIPT_STATE_RUN == nRunState) ||
			(SCRIPT_STATE_BREAK == nRunState) ||
			(SCRIPT_STATE_STEP == nRunState) )
			return TRUE;
	}

	return FALSE;
}

// 设置脚本文件名
int __stdcall CPythonInterpExtern::SetScriptFile(LPCTSTR lpszPathName)
{
	// TODO: Add your code here
	m_strScriptFile = lpszPathName;

	return TRUE;
}

// 获取脚本文件名
CString __stdcall CPythonInterpExtern::GetScriptFile()
{
	// TODO: Add your code here

	return m_strScriptFile;
}

// 运行进程并挂接自定义输出管道
BOOL CPythonInterpExtern::RunProgramWithPipe(CString strExePath, CString strParam, int nOutputId, BOOL bActiveOutput)
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
	m_hPipeWrite = hWrite;
	m_hPipeRead = hRead;

	// 组合命令行
	CString strCmd;
	strCmd.Format("%s %s", strExePath, strParam);
	char cscmd[512];
	strcpy(cscmd, strCmd);

	m_bPipeProcessTerminated = FALSE;
	if (!CreateProcess(NULL, cscmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		m_hPipeWrite = NULL;
		m_hPipeRead = NULL;
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

	m_hPipeWrite = NULL;
	m_hPipeRead = NULL;
	m_hPipeProcess = NULL;

	// 如果管道进程是被终止的,返回失败
	if(m_bPipeProcessTerminated)
	{
		return FALSE;
	}

	return TRUE;
}

// 运行调试服务进程并挂接自定义输出管道
BOOL CPythonInterpExtern::RunDebugServerWithPipe(CString strExePath, CString strParam, int nOutputId, BOOL bActiveOutput)
{
	m_hPipeProcess = NULL;

	SECURITY_ATTRIBUTES sa;
	HANDLE hRead,hWrite;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead,&hWrite,&sa,0))
	{
		//AfxMessageBox("创建运行管道失败！");
		DWORD dwError = GetLastError();
		CPythonInterpExtern::Instance()->DEBUG_OUTF(LOG_LEVEL_ERROR, "RunDebugServerWithPipe, CreatePipe failed, errorcode=%d", dwError);
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
	m_hPipeWrite = hWrite;
	m_hPipeRead = hRead;

	// 组合命令行
	CString strCmd;
	strCmd.Format("%s %s", strExePath, strParam);
	//strCmd.Format("%s", strExePath);
	char cscmd[512];
	strcpy(cscmd, strCmd);

	m_bPipeProcessTerminated = FALSE;
	if (!CreateProcess(NULL, cscmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		m_hPipeWrite = NULL;
		m_hPipeRead = NULL;
		DWORD dwError = GetLastError();
		//AfxMessageBox("创建管道进程失败！");
		CPythonInterpExtern::Instance()->DEBUG_OUTF(LOG_LEVEL_ERROR, "RunDebugServerWithPipe, CreateProcess & pipe failed, errorcode=%d", dwError);
		return FALSE;
	}
	//CloseHandle(hWrite);

	m_hPipeProcess = pi.hProcess;

	Sleep(200);

	if(pIPlatUI && bActiveOutput)
	{
		pIPlatUI->ActiveOutput(nOutputId);
		pIPlatUI->OutputClear(nOutputId);
		pIPlatUI->OutputSet(nOutputId, "COLOR=0,0,255");	// Set output color
	}

	// 首先等待socket连接
	int nWaitTime = 10;
	while(!CDebugSocket::Instance()->IsConnect() && (nWaitTime > 0))
	{
		CPythonInterpExtern::Instance()->DEBUG_OUTF(LOG_LEVEL_DEBUG, "RunDebugServerWithPipe, wait debug socket connect");
		Sleep(100);
		nWaitTime--;
	}

	// 如果socket一直未连接,则杀掉python进程,并返回错误
	if(!CDebugSocket::Instance()->IsConnect())
	{
		CPythonInterpExtern::Instance()->DEBUG_OUTF(LOG_LEVEL_ERROR, "RunDebugServerWithPipe, debug socket is not connect");
		TerminatePipeProcess();
		return FALSE;
	}

	//SendCmdToDebugProcess("l");
	//SendCmdToDebugProcess("w");

	//设置当前状态为中断状态
	//SetRunState(SCRIPT_STATE_BREAK);

	char buffer[4096] = {0};
	DWORD bytesRead;
	while (!m_bPipeProcessTerminated
		&& CDebugSocket::Instance()->IsConnect())
	{
		CPythonInterpExtern::Instance()->DEBUG_OUT(LOG_LEVEL_DEBUG, "wait pipe recv...");
		memset(buffer, 0, 4096);
		// 需要有方法可以在进程结束之后,或者socket中断,或者人工终止时候,能够触发函数返回,否则会一直停留在这个地方不退出
		if (!ReadFile(hRead, buffer, 4095, &bytesRead, NULL))
			break;

		if(pIPlatUI)
		{
			CString strOutput = buffer;
			CPythonInterpExtern::Instance()->DEBUG_OUT(LOG_LEVEL_DEBUG, "pipe recv:");
			CPythonInterpExtern::Instance()->DEBUG_OUT(LOG_LEVEL_DEBUG, buffer);

			int nPos = strOutput.Find("[RMXDB::SYSEXIT]");
			if(nPos == 0)
			{
				// 收到退出标识
				break;
			}

			BOOL bExit = FALSE;
			if(nPos == (strOutput.GetLength() - 18))
			{
				// 执行结束,需要把最后的退出标识删除,显示前面部分的输出
				strOutput = strOutput.Left(nPos);
				bExit = TRUE;
			}

			pIPlatUI->Output(nOutputId, strOutput);

			if(bExit)
			{
				break;
			}
		}
		Sleep(100);
	}

	CPythonInterpExtern::Instance()->DEBUG_OUTF(LOG_LEVEL_DEBUG, "RunDebugServerWithPipe loop end, term is %d, sock connect is %d", m_bPipeProcessTerminated, CDebugSocket::Instance()->IsConnect());

	CloseHandle(hWrite);

	m_hPipeWrite = NULL;
	m_hPipeRead = NULL;
	m_hPipeProcess = NULL;

	// 如果管道进程是被终止的,返回失败
	if(m_bPipeProcessTerminated)
	{
		return FALSE;
	}

	return TRUE;
}

// 向进程管道发送命令(不使用)
BOOL CPythonInterpExtern::SendCmdToProgramPipe(CString strCmd)
{
	if(m_hPipeWrite == NULL)
	{
		return FALSE;
	}

	// 发送消息到进程管道
	// Prepare the response.
	strCmd += "\r\n";
	int nLen = strCmd.GetLength();
	BYTE* pBuffer = new BYTE[nLen];
	memset(pBuffer, 0, nLen);
	memcpy(pBuffer, strCmd.GetBuffer(0), nLen);
	strCmd.ReleaseBuffer();

	DWORD cbBytesWritten = nLen;
	int cbBufferBytes = nLen;

	// Write the response to the pipe.
	BOOL bResult = WriteFile(		// Write to the pipe.
		m_hPipeWrite,				// Handle of the pipe
		pBuffer,				// Buffer to write to 
		cbBufferBytes,			// Number of bytes to write 
		&cbBytesWritten,		// Number of bytes written 
		NULL);					// Not overlapped I/O

	delete []pBuffer;

	CPythonInterpExtern::Instance()->DEBUG_OUTF(LOG_LEVEL_DEBUG, "SendCmd:%s, result=%d", strCmd, bResult);

	return bResult;
}

// 终止管道进程
BOOL CPythonInterpExtern::TerminatePipeProcess()
{
	if(m_hPipeProcess != NULL)
	{
		CPythonInterpExtern::Instance()->DEBUG_OUTF(LOG_LEVEL_DEBUG, "TerminatePipeProcess...");
		m_bPipeProcessTerminated = TRUE;
		if(!::TerminateProcess(m_hPipeProcess, 0))
		{
			CPythonInterpExtern::Instance()->DEBUG_OUTF(LOG_LEVEL_ERROR, "TerminatePipeProcess failed");
			return FALSE;
		}
		m_hPipeProcess = NULL;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 执行外部进程(不使用)
/////////////////////////////////////////////////////////////////////////////
BOOL CPythonInterpExtern::ExecProcess(CString strExePath, CString strParam, BOOL bWaitEnd)
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
	
	// 创建进程
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

// 处理socket消息回调
int CPythonInterpExtern::DbgSocketEventProcess(int nEventCode)
{
	if (TCP_CONNECT == nEventCode)	// 建立连接
	{
		CPythonInterpExtern::Instance()->DEBUG_OUTF(LOG_LEVEL_DEBUG, "CPythonInterpExtern::DbgSocketEventProcess eventCode=%d", nEventCode);
	}
	else if (TCP_ERROR == nEventCode)	// 连接失败
	{
		CPythonInterpExtern::Instance()->DEBUG_OUTF(LOG_LEVEL_DEBUG, "CPythonInterpExtern::DbgSocketEventProcess eventCode=%d", nEventCode);
	}

	return 0;
}

// socket接收的消息回调,对命令的解析都在此函数进行
int CPythonInterpExtern::DbgSocketMsgProcess(void *pData)
{
	CPythonInterpExtern::Instance()->DEBUG_OUT(LOG_LEVEL_DEBUG, "socket recv msg:");
	CPythonInterpExtern::Instance()->DEBUG_OUT(LOG_LEVEL_DEBUG, (char *)pData);

	CString strDbgMsg = (char*)pData;
	strDbgMsg.TrimLeft();
	if(strDbgMsg.Find("[RMDBG][LINE]") == 0)	// 显示当前行
	{
		strDbgMsg.Delete(0, 13);
		CString strFileName,strLineNo;
		GetStringKeyValue(strDbgMsg, "f", strFileName);
		GetStringKeyValue(strDbgMsg, "l", strLineNo);

		CString strFile1 = strFileName;
		strFile1.Replace("\\", "/");
		strFile1.MakeLower();
		CString strFile2 = CPythonInterpExtern::Instance()->GetScriptFile();
		strFile2.Replace("\\", "/");
		strFile2.MakeLower();
		CPythonInterpExtern::Instance()->DEBUG_OUTF(LOG_LEVEL_DEBUG, "socket msg:[RMDBG][LINE],%s,%s,%s", strFileName, strFile2, strLineNo);
		if(strcmp(strFile1, strFile2) != 0)
		{
			if(strcmp(strFileName, "<string>") == 0)
			{
				return 0;
			}
			// 文件未打开,则打开
			/*if(AddDebugFile(strFileName) != -1)
			{
				// 向OwmEdit发送打开文件命令
				//COM_CMD(OWM_OWMEDIT, OC_OPENFILE, (WPARAM)(LPCTSTR)strFileName, 0);
				CPythonInterpExtern::Instance()->pIPlatUI->OpenDocumentFile(strFileName, "");

				// 向OwmEdit发送设置运行模式为从调试模式的命令
				//COM_CMD(OWM_OWMEDIT, OC_SETSLAVEDEBUG, (WPARAM)(LPCTSTR)strFileName, TRUE);
				CPythonInterpExtern::Instance()->pIPlatUI->SendOwmCmd("org.owm.edit", OC_SETSLAVEDEBUG, (WPARAM)(LPTSTR)(LPCTSTR)strFileName, 1, NULL);
			}*/
		}else
		{
			//设置当前状态为中断状态
			CPythonInterpExtern::Instance()->SetRunState(SCRIPT_STATE_BREAK);
			//设置当前行
			if(CPythonInterpExtern::Instance()->m_lpfnDebug)
			{
				CPythonInterpExtern::Instance()->m_lpfnDebug(IDB_USER_SETCURRENTLINE, strFileName, atoi(strLineNo));
			}
		}
	}

	return 0;
}

// socket打印日志回调
void CPythonInterpExtern::DbgSocketLog(LPCSTR lpData)
{
	CPythonInterpExtern::Instance()->DEBUG_OUT(LOG_LEVEL_DEBUG, "socket log:");
	CPythonInterpExtern::Instance()->DEBUG_OUT(LOG_LEVEL_DEBUG, lpData);
}

// 向调试进程发送命令
BOOL CPythonInterpExtern::SendCmdToDebugProcess(CString strCmd)
{
	// 发送消息到Socket客户端
	strCmd += "\n";
	int nLen = strCmd.GetLength();
	BYTE* pBuffer = new BYTE[nLen+1];
	memset(pBuffer, 0, nLen+1);
	memcpy(pBuffer, strCmd.GetBuffer(0), nLen);
	strCmd.ReleaseBuffer();

	BOOL bResult = CDebugSocket::Instance()->SendCmdToClient((const char*)pBuffer);

	delete []pBuffer;

	CPythonInterpExtern::Instance()->DEBUG_OUTF(LOG_LEVEL_DEBUG, "SendCmd:%s, result=%d", strCmd, bResult);

	return bResult;
}

// 运行脚本文件
int __stdcall CPythonInterpExtern::RunScriptFile(LPCTSTR lpszPathName)
{
	// TODO: Add your code here
	if(lpszPathName != NULL)
	{
		if(!SetScriptFile(lpszPathName))
			return FALSE;
	}

	m_dwRunThreadId = ::GetCurrentThreadId();

	m_nRunState = SCRIPT_STATE_RUN;

	DEBUG_OUTF(LOG_LEVEL_DEBUG, "CPythonInterpExtern::RunScriptFile %s", lpszPathName);

	////////////////////////////////////////////
	// 执行脚本文件

	CString strPythonExeFile	= "c:\\python27\\";
	strPythonExeFile	+= "python.exe";

	//CString strScript = GetInterpRootPath();
	//strScript += "bin\\rm_pdb.py";

	CString strScriptCmd;
	//strScriptCmd.Format("-c \"import pdb;import rm_pdb;rmpdb=rm_pdb.pdb();rmpdb.run(execfile('%s'));\"", m_strScriptFile);
	strScriptCmd.Format("-c \"import pdb;import rm_pdb;rmpdb=rm_pdb.pdb();rmpdb._runscript('%s');print '[RMXDB::SYSEXIT]'\"", m_strScriptFile);
	//strScriptCmd.Format("-c \"import rm_pdb;pdb=rm_pdb.pdb();\"");

	if(SCRIPT_RUN_NORMAL != m_nRunMode)
	{
		// 调试模式
		// 启动调试服务线程
		CDebugSocket::Instance()->RegEventCallBack(CPythonInterpExtern::DbgSocketEventProcess);
		CDebugSocket::Instance()->RegRecvMsgCallBack(CPythonInterpExtern::DbgSocketMsgProcess);
		CDebugSocket::Instance()->RegPrintLog(CPythonInterpExtern::DbgSocketLog);
		//CDebugSocket::Instance()->StartServer();
		CDebugSocket::Instance()->StartServerThread();

		Sleep(100);
		//Sleep(30000);

		if(!RunDebugServerWithPipe(strPythonExeFile, strScriptCmd))
		{
			CDebugSocket::Instance()->StopServer();
			m_nRunState = SCRIPT_STATE_END;
			return FALSE;
		}

		CDebugSocket::Instance()->StopServer();

		Sleep(100);
	}else
	{
		// 普通运行模式
		if(!RunProgramWithPipe(strPythonExeFile, m_strScriptFile))
		{
			m_nRunState = SCRIPT_STATE_END;
			return FALSE;
		}
	}

	//if(TCL_OK != Tcl_EvalFile(pInterp, script))
	//{
	//	m_nRunState = SCRIPT_STATE_END;
	//	m_nEvalResult = TCL_ERROR;
	//	return FALSE;
	//}
	////////////////////////////////////////////

	m_nRunState = SCRIPT_STATE_END;

	m_dwRunThreadId = NULL;
	//m_nEvalResult	= TCL_OK;
	return TRUE;
}

// 执行脚本命令
int __stdcall CPythonInterpExtern::RunScriptCommand(LPCTSTR lpszTclCmd)
{
	// TODO: Add your code here

	//m_nEvalResult = TCL_OK;
	return TRUE;
}

// 停止脚本运行
int __stdcall CPythonInterpExtern::StopScript()
{
	// TODO: Add your code here
	m_bStopSignal	= TRUE;	// 设置中止标志

	// 终止管道进程
	//TerminatePipeProcess();

	// 关闭管道读句柄,这样可以使ReadFile函数立即返回
	if(m_hPipeRead != NULL)
	{
		DEBUG_OUTF(LOG_LEVEL_DEBUG, "StopScript, close read handle");
		CloseHandle(m_hPipeRead);
		m_hPipeRead = NULL;
	}
	if(m_hPipeWrite != NULL)
	{
		DEBUG_OUTF(LOG_LEVEL_DEBUG, "StopScript, close write handle");
		CloseHandle(m_hPipeWrite);
		m_hPipeWrite = NULL;
	}
/*
	// 发消息激活消息循环
	// 注：m_dwRunThreadId是保存的脚本运行时的线程ID，如果在其他线程中
	// 调用此函数,就会通过线程消息去激活运行线程中的td命令
	if(m_dwRunThreadId)
	{
		::PostThreadMessage(m_dwRunThreadId, WM_USER, 0, 0);
	}
*/
	return TRUE;
}

// 获取解释器指针
LPVOID __stdcall CPythonInterpExtern::GetInterp()
{
	// TODO: Add your code here

	return NULL;
}

// 获取解释器ID
int __stdcall CPythonInterpExtern::GetInterpID()
{
	// TODO: Add your code here

	return m_nInterpID;
}

// 设置解释器名
void __stdcall CPythonInterpExtern::SetInterpName(LPCTSTR lpszInterptName)
{
	// TODO: Add your code here
	m_strInterpName = lpszInterptName;
}

// 获取解释器名
CString __stdcall CPythonInterpExtern::GetInterpName()
{
	// TODO: Add your code here

	return m_strInterpName;
}

// 获取错误行号
int __stdcall CPythonInterpExtern::GetErrorLine()
{
	// TODO: Add your code here
	return 0;
}

// 获取结果信息
CString __stdcall CPythonInterpExtern::GetResult()
{
	// TODO: Add your code here
	return "";
}

// 设置变量值
int __stdcall CPythonInterpExtern::SetVar(LPCTSTR lpszVarName, LPCTSTR lpszVarValue)
{
	// TODO: Add your code here

	return 0;
}

// 获取变量值
CString __stdcall CPythonInterpExtern::GetVar(LPCTSTR lpszVarName)
{
	// TODO: Add your code here

	return "";
}

// 注册扩展命令
int __stdcall CPythonInterpExtern::RegisterCommand(LPVOID pInterp, LPCTSTR lpszCmd, LPVOID lpFunc)
{
	// TODO: Add your code here

	return 0;
}

// 设置扩展参数
int __stdcall CPythonInterpExtern::SetInterpParam(LPCTSTR lpszParamName, LPCTSTR lpszParamValue)
{
	// TODO: Add your code here

	return 0;
}

// 获取扩展参数
CString __stdcall CPythonInterpExtern::GetInterpParam(LPCTSTR lpszParamName)
{
	// TODO: Add your code here

	return "";
}

// 设置运行模式
int __stdcall CPythonInterpExtern::SetRunMode(int nRunMode)
{
	m_nRunMode = nRunMode;
	return true;
}

// 获取运行模式
int __stdcall CPythonInterpExtern::GetRunMode()
{
	return m_nRunMode;
}

// 设置运行状态
int __stdcall CPythonInterpExtern::SetRunState(int nState)
{
	m_nRunState = nState;
	return true;
}

// 获取运行状态
int __stdcall CPythonInterpExtern::GetRunState()
{
	return m_nRunState;
}

// 获取当前堆栈层数
int __stdcall CPythonInterpExtern::GetCurStackLevel()
{
	// TODO: Add your code here

	return 0;
}

// 获取关键字列表
int __stdcall CPythonInterpExtern::GetKeyWordList(LPCTSTR lpszType, CStringArray& asKeyWord)
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
int __stdcall CPythonInterpExtern::SetDebugEvent(int nEvent, int nParam, LPCTSTR lpszParam)
{
	// TODO: Add your code here
	CString strParam = lpszParam;

	if(nEvent == IDB_STOP)
	{
		//sprintf(szTclCmd, "td -stop");
		// 通过socket发送命令给调试进程
		SendCmdToDebugProcess("sysexit");
		Sleep(200);
		// 终止调试进程,并关闭管道句柄
		//StopScript();
	}else
	if(nEvent == IDB_NEXT)
	{
		//sprintf(szTclCmd, "td -next");
		SendCmdToDebugProcess("continue");
	}else
	if(nEvent == IDB_GOTO)
	{
		if(strParam.GetLength() == 0)
		{
			// 行断点
			//sprintf(szTclCmd, "td -goto *%d", nParam);
		}else
		{
			// 命名断点
			//sprintf(szTclCmd, "td -goto #%s", lpszParam);
		}
	}else
	if(nEvent == IDB_STEPINTO)
	{
		//sprintf(szTclCmd, "td -step into");
		SendCmdToDebugProcess("step");
	}else
	if(nEvent == IDB_SETPOUT)
	{
		//sprintf(szTclCmd, "td -step return");
		SendCmdToDebugProcess("next");
	}else
	if(nEvent == IDB_STEP)
	{
		//sprintf(szTclCmd, "td -step");
		SendCmdToDebugProcess("next");
	}else
	if(nEvent == IDB_END)
	{
		return TRUE;
	}

	// 设置当前状态为运行状态
	SetRunState(SCRIPT_STATE_RUN);
	// 向Edit发送当前行清除命令
	if(m_lpfnDebug)
	{
		m_lpfnDebug(IDB_USER_SETCURRENTLINE, m_strScriptFile, 0);
	}

	// 执行断点操作

	return TRUE;
}

// 设置断点列表
int __stdcall CPythonInterpExtern::SetBreakList(LPCTSTR lpszFile, CUIntArray& auBreaks)
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
int __stdcall CPythonInterpExtern::GetBreakList(LPCTSTR lpszFile, CUIntArray& auBreaks)
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
int __stdcall CPythonInterpExtern::GetEnableProfileAnalyse()
{
	// TODO: Add your code here
	return m_bProfileEnable;
}

// 获取有效代码行数
int __stdcall CPythonInterpExtern::GetAvailableLineCount()
{
	// TODO: Add your code here
	return m_nTDLineCount;
}

// 获取代码运行覆盖表
LPVOID __stdcall CPythonInterpExtern::GetProfileMap()
{
	// TODO: Add your code here
	return (void*)(&m_ProfileMap);
}

// 判断指定方法是否被支持
int __stdcall CPythonInterpExtern::MethodIsSupport(LPCTSTR lpszMethod)
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
int __stdcall CPythonInterpExtern::InitialDebugMode(CString& strResult)
{
	// TODO: Add your code here
	// 初始化各个调试窗口
	CString strWinVar = "变量", strWinObj = "对象", strWinFunc = "函数", strWinStack = "堆栈";
	//LOADRCSTRING( strWinVar, IDS_WINDOW_VAR );
	//LOADRCSTRING( strWinObj, IDS_WINDOW_OBJ );
	//LOADRCSTRING( strWinFunc, IDS_WINDOW_FUNC );
	//LOADRCSTRING( strWinStack, IDS_WINDOW_STACK );

	CString strColName = "名字", strColValue = "值", strColType = "类型", strColSource = "文件", strColLine = "行";
	//LOADRCSTRING( strColName, IDS_COLUMN_NAME );
	//LOADRCSTRING( strColValue, IDS_COLUMN_VALUE );
	//LOADRCSTRING( strColType, IDS_COLUMN_TYPE );
	//LOADRCSTRING( strColSource, IDS_COLUMN_SOURCE );
	//LOADRCSTRING( strColLine, IDS_COLUMN_LINE );

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

	// 初始化堆栈窗口
	m_nOutputStack = ::atoi(pIPlatUI->OutputGet(-1, strWinStack));
	if(m_nOutputStack <= 0)
	{
		//新窗口，创建
		m_nOutputStack = pIPlatUI->AddOutput(strWinStack + "##GRID##" + strWinVar);
	}
	pIPlatUI->OutputSet(m_nOutputStack, "DelColumn ALL;DelItem ALL");
	pIPlatUI->OutputSet(m_nOutputStack, "AddColumn " + strColName + ",100");
	pIPlatUI->OutputSet(m_nOutputStack, "AddColumn " + strColSource + ",150");
	pIPlatUI->OutputSet(m_nOutputStack, "AddColumn " + strColLine + ",50,AutoSize");
	pIPlatUI->OutputSet(m_nOutputStack, "SetTreeColumn 0");
	pIPlatUI->OutputSet(m_nOutputStack, "SetRowColors RGB(0,0,128) RGB(255,255,205)");
	pIPlatUI->OutputSet(m_nOutputStack, "SetGridColor RGB(192,192,192)");
	pIPlatUI->OutputSet(m_nOutputStack, "ModifyStyle Horizontal Solid;ModifyStyle Vertical Solid;NoSort");
	pIPlatUI->OutputSet(m_nOutputStack, "ModifyStyle Add(NoSort)");
	pIPlatUI->OutputSet(m_nOutputStack, "SEPARATOR=" SEPATATOE); //设置分隔符
	m_nOutputStack = pIPlatUI->AddOutput(strWinStack + "##GRID");

	// 激活变量窗口
	pIPlatUI->ActiveOutput(m_nOutputVar);

	return TRUE;
}

// 更新调试窗口信息
int __stdcall CPythonInterpExtern::RefreshDebugWindow(LPCTSTR lpszWindow, BOOL bActive, CString& strResult)
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
	}else
	if(strWindow == "object")
	{
		bWindowObject = TRUE;
	}else
	if(strWindow == "proc")
	{
		bWindowProc = TRUE;
	}else
	if(strWindow == "stack")
	{
		bWindowStack = TRUE;
	}else
	if(strWindow == "all")
	{
		bWindowVar = TRUE;
		bWindowObject = TRUE;
		//bWindowProc = TRUE;
		bWindowStack = TRUE;
	}

	if(bWindowVar)
	{
		//RunCommand(_T("global _plat_Tcl_WatchVariables;eval $_plat_Tcl_WatchVariables"));
		if(bActive)
		{
			//RunCommand(_T("_plat_Tcl_ActiveDebugWatch var"));
		}
	}
	if(bWindowObject)
	{
	}
	if(bWindowProc)
	{
	}
	if(bWindowStack)
	{
	}

	return TRUE;
}

// 调用脚本方法
int __stdcall CPythonInterpExtern::CallMethod(LPCTSTR lpszMethod, CStringArray& asParam)
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
int __stdcall CPythonInterpExtern::Compile(LPCTSTR lpszSrcFile, CString& strDestFile, LPCTSTR lpszParams)
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

// 构建
int __stdcall CPythonInterpExtern::Build(LPCTSTR lpszSrcFile, CString& strDestFile, LPCTSTR lpszParams)
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
//}}VCI_IMPLEMENT_END

//////////////////////////////////////////////////////////////////////////
// 接口实现
//////////////////////////////////////////////////////////////////////////
INTERFACE_IMPLEMENT(PythonInterpExtern)
CONTROL_INTERFACE_IMPLEMENT(PythonInterpExtern)

//{{ 注意：!!!这里是保留的重要函数，不可删除!!!
// 获取接口的额外信息
LPVOID __stdcall CPythonInterpExtern::XPythonInterpExtern::GetExtraInfo(LPVOID lpRefData)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj;
}
//}} 注意：!!!这里是保留的重要函数，不可删除!!!

//{{VCI_INTERFACE_IMPLEMENT_BEGIN
// 设置平台UI接口指针
int __stdcall CPythonInterpExtern::XPythonInterpExtern::SetIPlatUI(LPVOID lpIPlatUI)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->SetIPlatUI(lpIPlatUI);
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::SetLibPath(LPCTSTR lpszPath)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->SetLibPath(lpszPath);
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::IsInterpRunning()
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->IsInterpRunning();
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::SetScriptFile(LPCTSTR lpszPathName)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->SetScriptFile(lpszPathName);
}

CString __stdcall CPythonInterpExtern::XPythonInterpExtern::GetScriptFile()
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->GetScriptFile();
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::RunScriptFile(LPCTSTR lpszPathName)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->RunScriptFile(lpszPathName);
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::RunScriptCommand(LPCTSTR lpszTclCmd)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->RunScriptCommand(lpszTclCmd);
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::StopScript()
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->StopScript();
}

LPVOID __stdcall CPythonInterpExtern::XPythonInterpExtern::GetInterp()
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->GetInterp();
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::GetInterpID()
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->GetInterpID();
}

void __stdcall CPythonInterpExtern::XPythonInterpExtern::SetInterpName(LPCTSTR lpszInterptName)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	pObj->SetInterpName(lpszInterptName);
}

CString __stdcall CPythonInterpExtern::XPythonInterpExtern::GetInterpName()
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->GetInterpName();
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::GetErrorLine()
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->GetErrorLine();
}

CString __stdcall CPythonInterpExtern::XPythonInterpExtern::GetResult()
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->GetResult();
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::SetVar(LPCTSTR lpszVarName, LPCTSTR lpszVarValue)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->SetVar(lpszVarName, lpszVarValue);
}

CString __stdcall CPythonInterpExtern::XPythonInterpExtern::GetVar(LPCTSTR lpszVarName)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->GetVar(lpszVarName);
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::RegisterCommand(LPVOID pInterp, LPCTSTR lpszCmd, LPVOID lpFunc)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->RegisterCommand(pInterp, lpszCmd, lpFunc);
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::SetInterpParam(LPCTSTR lpszParamName, LPCTSTR lpszParamValue)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->SetInterpParam(lpszParamName, lpszParamValue);
}

CString __stdcall CPythonInterpExtern::XPythonInterpExtern::GetInterpParam(LPCTSTR lpszParamName)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->GetInterpParam(lpszParamName);
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::SetRunMode(int nRunMode)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->SetRunMode(nRunMode);
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::GetRunMode()
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->GetRunMode();
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::SetRunState(int nRunState)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->SetRunState(nRunState);
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::GetRunState()
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->GetRunState();
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::GetCurStackLevel()
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->GetCurStackLevel();
}

// 获取关键字列表
int __stdcall CPythonInterpExtern::XPythonInterpExtern::GetKeyWordList(LPCTSTR lpszType, CStringArray& asKeyWord)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->GetKeyWordList(lpszType, asKeyWord);
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::SetDebugEvent(int nEvent, int nParam, LPCTSTR lpszParam)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->SetDebugEvent(nEvent, nParam, lpszParam);
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::SetBreakList(LPCTSTR lpszFile, CUIntArray& auBreaks)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->SetBreakList(lpszFile, auBreaks);
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::GetBreakList(LPCTSTR lpszFile, CUIntArray& auBreaks)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->GetBreakList(lpszFile, auBreaks);
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::GetEnableProfileAnalyse()
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->GetEnableProfileAnalyse();
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::GetAvailableLineCount()
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->GetAvailableLineCount();
}

LPVOID __stdcall CPythonInterpExtern::XPythonInterpExtern::GetProfileMap()
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->GetProfileMap();
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::MethodIsSupport(LPCTSTR lpszMethod)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->MethodIsSupport(lpszMethod);
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::InitialDebugMode(CString& strResult)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->InitialDebugMode(strResult);
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::RefreshDebugWindow(LPCTSTR lpszWindow, BOOL bActive, CString& strResult)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->RefreshDebugWindow(lpszWindow, bActive, strResult);
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::CallMethod(LPCTSTR lpszMethod, CStringArray& asParam)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->CallMethod(lpszMethod, asParam);
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::Compile(LPCTSTR lpszSrcFile, CString& strDestFile, LPCTSTR lpszParams)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
	return pObj->Compile(lpszSrcFile, strDestFile, lpszParams);
}

int __stdcall CPythonInterpExtern::XPythonInterpExtern::Build(LPCTSTR lpszSrcFile, CString& strDestFile, LPCTSTR lpszParams)
{
	CPythonInterpExtern *pObj = GET_INTERFACE_OBJECT(PythonInterpExtern);
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
