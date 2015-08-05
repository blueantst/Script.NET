#include "stdafx.h"
#include "CLuaInterp.h"
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

CString LuaMakeNumber(lua_State *pLState, double dnum)
{
	CString strRet;
    long num = (long) dnum;
    if ((double) num == dnum)
	{
        strRet.Format("%ld (0x%lx)", num, num);
	}else
	{
		strRet.Format("%g", dnum);
	}

    return strRet;
}

CString LuaMakeBoolean(lua_State *pLState, int num)
{
    return num != 0 ? "true" : "false";
}

CString LuaGetTableInfo(lua_State *pLState, int nIndex)
{
	CString strRet;
    //int         nTag   = wxlua_ttag(pLState, nIndex);
	#ifdef USE_LUA_51
	int         nItems = luaL_getn(pLState, nIndex);
	#endif
	#ifdef USE_LUA_52
    int         nItems = lua_objlen(pLState, nIndex);
	#endif
    const void *pItem  = lua_topointer(pLState, nIndex);

    strRet.Format("0x%p", pItem);

    if (nItems > 0)
        strRet.Format("0x%p (approx %d items)", pItem, nItems);

    //if (nTag != WXLUA_NOTAG)
    //    strRet.Format("0x%p (tag %d)"), pItem, nTag);
	strRet = "table:" + strRet;
	return strRet;
}

// 获取指定变量的值(转换为字符串)
CString GetValueString(lua_State *pLState, int nIndex)
{
	CString strValue = "";
	int ntype = lua_type(pLState, nIndex);
	switch(ntype)
	{
	case LUA_TNIL:
		strValue = "nil";
		break;
	case LUA_TBOOLEAN:
		strValue = LuaMakeBoolean(pLState, lua_toboolean(pLState, nIndex));
		break;
	case LUA_TNUMBER:
		strValue = LuaMakeNumber(pLState, lua_tonumber(pLState, nIndex));
		break;
	case LUA_TSTRING:
		strValue.Format("%s", lua_tostring(pLState, nIndex));
		break;
	case LUA_TTABLE:
		strValue = LuaGetTableInfo(pLState, nIndex);
		break;
	case LUA_TFUNCTION:
		if (lua_iscfunction(pLState, nIndex))
		{
			strValue.Format("%p", lua_tocfunction(pLState, nIndex));
		}else
		{
			strValue.Format("%p", lua_topointer(pLState, nIndex));
		}
		break;
	case LUA_TTHREAD:
		strValue.Format("%p", lua_topointer(pLState, nIndex));
		break;
	default:
		break;
	}
	return strValue;
}

// 重载的print函数
static int print_glue(lua_State *L)
{
	CLuaInterp* pInterp = GetLuaInterp(L);
	if(pInterp == NULL)
	{
		return 0;
	}

	IPlatUI* pIPlatUI = pInterp->pIPlatUI;
	if(pIPlatUI == NULL)
	{
		return 0;
	}

	BOOL bDirectOutput = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_OUTPUT, TRUE);
	int nOT = 2;
	if(bDirectOutput)
	{
		nOT = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_OUTPUT_ID, 2);	// 信息窗口
	}

	// 输出到脚本标准输出
	//BOOL bDirectResult = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_RESULT, TRUE);

	if(bDirectOutput)
	{
		pIPlatUI->OutputSet(nOT, "COLOR=0,0,255");	// 设置输出颜色
	}

	// Get the number of parameters
	int n = lua_gettop(L);
	for(int i = 1; i <= n; i ++)
	{
		//const char *s = lua_tostring(L, i);
		CString s = GetValueString(L, i);
		if(bDirectOutput)// && (s != NULL))
		{
			pIPlatUI->Output(nOT, s);
			pIPlatUI->Output(nOT, "\t");
		}
	}

	// 增加换行
	if((n > 0) && bDirectOutput)
	{
		pIPlatUI->Output(nOT, "\r\n");
	}

	// No results
	return 0;
}

// 输出到结果字符串的函数
static int put_result_glue(lua_State *L)
{
	CLuaInterp* pInterp = GetLuaInterp(L);
	if(pInterp == NULL)
	{
		return 0;
	}

	// Get the number of parameters
	int n = lua_gettop(L);
	for(int i = 1; i <= n; i ++)
	{
		//const char *s = lua_tostring(L, i);
		CString s = GetValueString(L, i);
		pInterp->m_strResult += s;
	}

	// No results
	return 0;
}

static int lua_traceback (lua_State *L)
{
	#ifdef USE_LUA_51
	lua_getfield(L, LUA_GLOBALSINDEX, "debug");
	#endif
	#ifdef USE_LUA_52
	lua_getglobal(L, "debug");	// Lua 5.2没有global
	#endif

	if (!lua_istable(L, -1))
	{
		lua_pop(L, 1);
		return 1;
	}
	lua_getfield(L, -1, "traceback");
	if (!lua_isfunction(L, -1))
	{
		lua_pop(L, 2);
		return 1;
	}
	lua_pushvalue(L, 1);  /* pass error message */
	lua_pushinteger(L, 2);  /* skip this function and traceback */
	lua_call(L, 2, 1);  /* call debug.traceback */
	return 1;
}

/*
// 重载的加载扩展库的函数
typedef int (*LuaRegister)(lua_State*, HWND hWnd);
static int lua_loadlib(lua_State *L)
{
	CString strLibPath = lua_tostring(L, 1);
	strLibPath.Replace("/", "\\");
	if(strLibPath.Find("\\") == -1)
	{
		strLibPath = GetInterpRootPath() + "bin\\" + strLibPath;
	}

	HMODULE hMod = LoadLibrary(strLibPath);
	if ( hMod )
	{
		LuaRegister lua_reg = (LuaRegister)GetProcAddress(hMod, "LuaRegister");
		if ( lua_reg )
		{
			lua_reg(L, AfxGetMainWnd()->GetSafeHwnd());
			CLuaInterp* pInterp = GetLuaInterp(L);
			if(pInterp)
			{
				pInterp->m_hLoaded[pInterp->m_nLoaded++] = hMod;
			}
			lua_pushboolean(L, TRUE);
			return 1;
		}
		else
		{
			luaL_argerror(L, 1, "Dll has no LuaRegister entry");
			lua_pushboolean(L, FALSE);
			return 1;
		}	
	}
	else
	{
		luaL_argerror(L, 1, (LPCTSTR)"Load library failed!");
		lua_pushboolean(L, FALSE);
		return 1;
	}
}
*/
//////////////////////////////////////////////////////////////////////
// 解释器操作
//////////////////////////////////////////////////////////////////////
static CPtrList	g_InterpList;	// 解释器对象列表
static int g_InterpID = 0x100;	// 解释器ID计数器

// 获取Lua解释器列表指针
CPtrList* GetInterpList()
{
	return &g_InterpList;
}

//////////////////////////////////////////////////////////////////////
// 根据解释器指针获取解释器对象指针
//////////////////////////////////////////////////////////////////////
CLuaInterp* GetLuaInterp(LPVOID lpInterp)
{
	int count = g_InterpList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_InterpList.FindIndex(i);
		CLuaInterp* pInterp = (CLuaInterp*)g_InterpList.GetAt(pos);
		if(lpInterp == pInterp->GetInterp())
		{
			return pInterp;
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// 判断解释器对象是否存在
//////////////////////////////////////////////////////////////////////
BOOL CheckLuaInterpExist(CLuaInterp* pLuaInterp)
{
	int count = g_InterpList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_InterpList.FindIndex(i);
		CLuaInterp* pInterp = (CLuaInterp*)g_InterpList.GetAt(pos);
		if(pLuaInterp == pInterp)
		{
			return TRUE;
		}
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// 功能实现
//////////////////////////////////////////////////////////////////////////
CLuaInterp::CLuaInterp()
{
	m_nInterpID		= g_InterpID++;
	if(g_InterpID > 0x01000000)
		g_InterpID  = 0x100;
	pIPlatUI		= NULL;
	m_dwRunThreadId	= NULL;
	m_strInterpName	= _T("");
	m_strScriptFile	= _T("");

	// init lua base library
	#ifdef USE_LUA_51
	pLState			= lua_open();
	#endif
	#ifdef USE_LUA_52
    pLState			= luaL_newstate();
	#endif

	// 设置lua的库路径(此函数是blueant在lua标准动态库中新增的,如果lua版本升级,需要重新移植代码)
	luaL_setlibpath(pLState, GetInterpRootPath() + "bin");

	luaL_openlibs(pLState);
	/*
	luaopen_base(pLState);
	//luaopen_package(pLState);
	luaopen_table(pLState);
	//luaopen_io(pLState);
	luaopen_os(pLState);
	luaopen_string(pLState);
	luaopen_math(pLState);
	luaopen_debug(pLState);
	//luaopen_loadlib(pLState);
	*/

	lua_register(pLState, "print",  print_glue);
	lua_register(pLState, "put_result",  put_result_glue);
	//lua_register(pLState, "loadlib", lua_loadlib);

	LuaExCmd_Register(pLState);

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
	g_InterpList.AddTail(this);
	m_nLoaded		= 0;

	m_lpfnDebug		= NULL;
}

CLuaInterp::~CLuaInterp()
{
	// 清空覆盖率分析哈希表
	m_ProfileMap.RemoveAll();

	try {
		lua_close(pLState);
	}catch(...)
	{
	}

	// 删除解释器对象列表中的项
	int count = g_InterpList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_InterpList.FindIndex(i);
		CLuaInterp* pInterp = (CLuaInterp*)g_InterpList.GetAt(pos);
		if(pInterp == this) 
		{
			g_InterpList.RemoveAt(pos);
			return;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// 安装回调函数
/////////////////////////////////////////////////////////////////////////////
int CLuaInterp::InstallHandler(DWORD nEventID, LPVOID lpHandler, DWORD dwRefData)
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
int CLuaInterp::UninstallHandler(DWORD nEventID, LPVOID lpHandler)
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
// 初始化Lua的平台变量
/////////////////////////////////////////////////////////////////////////////
int CLuaInterp::LuaSetPlatformVariable()
{
	IPlatUI* pIPlatUI = ((IPlatUI*)getIPlatUI());

	// 设置全局变量platform_language,Lua设置变量的方法是先把值压栈,然后调用设置
	lua_pushstring(pLState, (VCI_LANGUAGE_CHINESE == GetLanguage()) ? "chinese" : "english");
	lua_setglobal(pLState, "platform_language");

	// 设置platform_path
	lua_pushstring(pLState, GetPlatRootPath());
	lua_setglobal(pLState, "platform_path");

	// 设置platform_version
	CString strPlatformVersion = "";
	if(pIPlatUI)
	{
		strPlatformVersion = pIPlatUI->GetPlatVersion();
	}
	lua_pushstring(pLState, strPlatformVersion);
	lua_setglobal(pLState, "platform_version");

	// 设置platform_interface
	lua_pushinteger(pLState, (int)pIPlatUI);
	lua_setglobal(pLState, "platform_interface");

	// 设置platform_type
	#ifdef _DEBUG
	lua_pushstring(pLState, "debug");
	#else
	lua_pushstring(pLState, "release");
	#endif
	lua_setglobal(pLState, "platform_type");

	// 设置interp_path
	lua_pushstring(pLState, GetInterpRootPath());
	lua_setglobal(pLState, "interp_path");

	// 设置interpName
	lua_pushstring(pLState, m_strInterpName);
	lua_setglobal(pLState, "interpName");

	return 0;
}

//{{VCI_IMPLEMENT_BEGIN
int __stdcall CLuaInterp::SetIPlatUI(LPVOID lpIPlatUI)
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
int __stdcall CLuaInterp::SetLibPath(LPCTSTR lpszPath)
{
	// TODO: Add your code here

	return 0;
}

// 解释器是否在运行
int __stdcall CLuaInterp::IsInterpRunning()
{
	// TODO: Add your code here
	// 判断所有解释器是否都已经终止
	int count = g_InterpList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_InterpList.FindIndex(i);
		CLuaInterp* pInterp = (CLuaInterp*)g_InterpList.GetAt(pos);
		int nRunState = pInterp->GetRunState();
		if( (SCRIPT_STATE_RUN == nRunState) ||
			(SCRIPT_STATE_BREAK == nRunState) ||
			(SCRIPT_STATE_STEP == nRunState) )
			return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 获取调试文件名的索引,用于判断文件是否已经打开了
/////////////////////////////////////////////////////////////////////////////
int CLuaInterp::FindDebugFile(CString strFileName)
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
int CLuaInterp::AddDebugFile(CString strFileName)
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
BOOL CLuaInterp::ClearDebugFile()
{
	m_asDebugScript.RemoveAll();
	return TRUE;
}

// 设置脚本文件名
int __stdcall CLuaInterp::SetScriptFile(LPCTSTR lpszPathName)
{
	// TODO: Add your code here
	m_strScriptFile = lpszPathName;

	return TRUE;
}

// 获取脚本文件名
CString __stdcall CLuaInterp::GetScriptFile()
{
	// TODO: Add your code here

	return m_strScriptFile;
}

// 运行进程并挂接自定义输出管道
BOOL CLuaInterp::RunProgramWithPipe(CString strExePath, CString strParam, int nOutputId, BOOL bActiveOutput)
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
BOOL CLuaInterp::TerminatePipeProcess()
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
BOOL CLuaInterp::ExecProcess(CString strExePath, CString strParam, BOOL bWaitEnd)
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

// 调试回调
void CLuaInterp::DebugBreak(const char *szFile, int nStackLevel, int nLine)
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

	// 设置当前状态为中断状态
	SetRunState(SCRIPT_STATE_BREAK);
	//重新设置事件
	::ResetEvent(m_hDebugEvent);
	
	//等待调试命令
	::WaitForSingleObject(m_hDebugEvent, INFINITE);

	// 设置当前状态为运行状态
	SetRunState(SCRIPT_STATE_RUN);
	// 向Edit发送当前行清除命令
	if(m_lpfnDebug)
	{
		m_lpfnDebug(IDB_USER_SETCURRENTLINE, szFile, 0);
	}
}

// 行回调
void CLuaInterp::LineHook(const char *szFile, int nLine)
{
	//判断当前模式
	if(m_nCurCommand == IDB_STOP)	// 终止程序运行
	{
		StopScript();
	}else
	if(m_nCurCommand == IDB_STEPINTO) //单步进入函数，每步必停
	{
		DebugBreak(szFile, GetCurStackLevel(), nLine);
		return;
	}else
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

// 行回调
static void lua_line_hook (lua_State *L, lua_Debug *ar)
{
	lua_getinfo(L, "lnuS", ar);

	if ( ar->source[0] == '@' )
	{
		CLuaInterp* pInterp = GetLuaInterp(L);
		if(pInterp != NULL)
		{
			pInterp->LineHook(ar->source+1, ar->currentline);
		}
	}
}

/*
// 函数回调
static void lua_func_hook (lua_State *L, lua_Debug *ar)
{
	lua_getinfo(L, "lnuS", ar);
	
	if ( ar->source[0] == '@' )
 	{
		CLuaInterp* pInterp = GetLuaInterp(L);
		if(pInterp != NULL)
		{
			pInterp->LineHook(ar->source+1, ar->currentline);
		}
 	}
}*/

// 调试回调函数
static void lua_hook (lua_State *L, lua_Debug *ar)
{
 	switch(ar->event)
 	{
		#ifdef USE_LUA_51
 		case LUA_HOOKTAILRET:
		#endif
		#ifdef USE_LUA_52
		case LUA_HOOKTAILCALL:
		#endif
 		case LUA_HOOKRET:
 						//lua_func_hook(L,ar);
 						break;
 		case LUA_HOOKCALL://lua_func_hook(L,ar);
 						break;
 		case LUA_HOOKLINE:lua_line_hook(L,ar);
 						break;
 	}
}

int CLuaInterp::LuaReport(int status)
{
	if ((status!=0) && !lua_isnil(pLState, -1))
	{
		const char *msg = lua_tostring(pLState, -1);
		if (msg == NULL)
		{
			msg = "(error object is not a string)\n";
		}
		/* 错误信息直接通过结果返回,不用在此处显示
		// output
		if(pIPlatUI)
		{
			BOOL bDirectOutput = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_OUTPUT, TRUE);
			int nOT = 1;
			if(bDirectOutput)
			{
				pIPlatUI->OutputSet(nOT, "COLOR=255,0,0");	// 设置输出颜色
				pIPlatUI->Output(nOT, msg);
				pIPlatUI->Output(nOT, "\r\n");
			}
		}
		*/

		lua_pop(pLState, 1);

		//status = 1;
		m_strResult = msg;
	}

	return status;
}

int CLuaInterp::LuaDocall(int narg, int clear)
{
	//如果是调试模式（非运行模式），那么建立调试事件
	if(m_nRunMode != SCRIPT_RUN_NORMAL)
	{
		static int eventNum = 0;
		CString eventName;
		eventName.Format("LuaDebug%d", eventNum);
		m_hDebugEvent = ::CreateEvent(NULL, FALSE, FALSE, eventName);
		eventNum++;
		if(!m_hDebugEvent)
		{
			::AfxMessageBox("Create Debug Event Failed, Can not debug");
			lua_pushstring(pLState, "Create Debug Event Failed, Can not debug!\n");
			lua_error(pLState);
			m_nRunState = SCRIPT_STATE_END;
			return FALSE;
		}

		// 启动hook
		lua_sethook(pLState, lua_hook, LUA_MASKLINE|LUA_MASKCALL|LUA_MASKRET, 0);
	}

	m_strResult = "";
	int status;
	int base = lua_gettop(pLState) - narg;  /* function index */
	lua_pushcfunction(pLState, lua_traceback);  /* push traceback function */
	lua_insert(pLState, base);  /* put it under chunk and args */
	//signal(SIGINT, laction);
	status = lua_pcall(pLState, narg, (clear ? 0 : LUA_MULTRET), base);
	//signal(SIGINT, SIG_DFL);
	lua_remove(pLState, base);  /* remove traceback function */
	/* force a complete garbage collection in case of errors */
	if (status != 0)
	{
		lua_gc(pLState, LUA_GCCOLLECT, 0);
	}

	lua_sethook(pLState, NULL, 0, 0);  /* turn off hooks */

	status = LuaReport(status);

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

	if((m_nRunMode != SCRIPT_RUN_NORMAL)&&(m_lpfnDebug != NULL))
	{
		//清除主调试文件的调试箭头
		m_lpfnDebug(IDB_USER_SETCURRENTLINE, m_strScriptFile, 0);

		// 清除调试事件
		if(m_hDebugEvent)
		{
			::CloseHandle(m_hDebugEvent);
			m_hDebugEvent = NULL;
		}
	}

	return status;
}

// 运行脚本文件
int __stdcall CLuaInterp::RunScriptFile(LPCTSTR lpszPathName)
{
	// TODO: Add your code here
	m_nEvalResult = 0;
	if(lpszPathName != NULL)
	{
		if(!SetScriptFile(lpszPathName))
			return FALSE;
	}

	m_dwRunThreadId = ::GetCurrentThreadId();

	m_nRunState = SCRIPT_STATE_RUN;

	try {
	////////////////////////////////////////////

	// 设置平台变量
	LuaSetPlatformVariable();

	// 执行脚本文件
	m_strResult = "";
	//m_nEvalResult = luaL_loadfile(pLState, m_strScriptFile) ||
	//				LuaDocall(0, 1);
	m_nEvalResult = luaL_loadfile(pLState, m_strScriptFile);
	if(m_nEvalResult == 0)
	{
		// 如果语法解析没有错误就执行
		m_nEvalResult = LuaDocall(0, 1);
	}else
	{
		//如果语法解析有错误就直接报错
		LuaReport(m_nEvalResult);
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
int __stdcall CLuaInterp::RunScriptCommand(LPCTSTR lpszTclCmd)
{
	// TODO: Add your code here
	if(m_nRunMode != SCRIPT_RUN_NORMAL)
	{
		// 正处于调试模式下,不允许执行命令
		return FALSE;
	}

	try {
	////////////////////////////////////////////

	// 设置平台变量
	LuaSetPlatformVariable();

	m_strResult = "";
	//m_nEvalResult = luaL_loadbuffer(pLState, lpszTclCmd, strlen(lpszTclCmd), "command") ||
	//				LuaDocall(0, 1);
	m_nEvalResult = luaL_loadbuffer(pLState, lpszTclCmd, strlen(lpszTclCmd), "command");
	if(m_nEvalResult == 0)
	{
		// 如果语法解析没有错误就执行
		m_nEvalResult = LuaDocall(0, 1);
	}else
	{
		//如果语法解析有错误就直接报错
		LuaReport(m_nEvalResult);
	}

	////////////////////////////////////////////
	}catch(...)
	{
	}

	return TRUE;
}

// 停止脚本运行
int __stdcall CLuaInterp::StopScript()
{
	// TODO: Add your code here
	m_bStopSignal	= TRUE;	// 设置中止标志

	luaL_error(pLState, "User stop eval script!");

	// 终止管道进程
	TerminatePipeProcess();

	// 发消息激活消息循环
	// 注：m_dwRunThreadId是保存的脚本运行时的线程ID，如果在其他线程中
	// 调用此函数,就会通过线程消息去激活运行线程中的td命令
	if(m_dwRunThreadId)
	{
		::PostThreadMessage(m_dwRunThreadId, WM_USER, 0, 0);
	}

	// 如果停止调试,就释放所有加载库
	if ( m_nLoaded > 0 )
	{
		for ( int i=m_nLoaded-1; i>=0; --i )
			FreeLibrary(m_hLoaded[i]);

		m_nLoaded = 0;
	}

	return TRUE;
}

// 获取解释器指针
LPVOID __stdcall CLuaInterp::GetInterp()
{
	// TODO: Add your code here

	return pLState;
}

// 获取解释器ID
int __stdcall CLuaInterp::GetInterpID()
{
	// TODO: Add your code here

	return m_nInterpID;
}

// 设置解释器名
void __stdcall CLuaInterp::SetInterpName(LPCTSTR lpszInterptName)
{
	// TODO: Add your code here
	m_strInterpName = lpszInterptName;
}

// 获取解释器名
CString __stdcall CLuaInterp::GetInterpName()
{
	// TODO: Add your code here

	return m_strInterpName;
}

// 获取错误行号
int __stdcall CLuaInterp::GetErrorLine()
{
	// TODO: Add your code here
	if(m_nEvalResult != 0)	// 错误信息和错误行解析
	{
		GetResult();
	}
	return m_nEvalResult;
}

// 获取结果信息
CString __stdcall CLuaInterp::GetResult()
{
	// TODO: Add your code here
	CString strResult = m_strResult;
	if(m_nEvalResult != 0)	// 错误信息解析
	{
		if(strResult.Find("[string \"command\"]:") == 0)
		{
			// 解析RunScriptCommand的错误信息,格式为 [string "command"]:lineno:errorinfo
			strResult.Delete(0, 19);
			int nPos = strResult.Find(":");
			if(nPos != -1)
			{
				m_nEvalResult = atoi(strResult.Left(nPos));
				strResult.Delete(0, nPos+1);
				strResult.TrimLeft();
			}
		}else
		{
			int nPosStack = strResult.Find("stack traceback:");
			if(nPosStack != -1)
			{
				// 如果有stack traceback:,需要去掉再进行下面的分析,但最后的结果要加上
				strResult.Delete(0, nPosStack+17);
			}
			
			// 解析RunScriptFile的错误信息,格式为 filepath:lineno:errorinfo
			// 但如果字符串比较长的话,filepath部分可能会缩减掉前面的一些内容,并以...开头
			int nPos = strResult.Find(":");
			int nPos3Dot = strResult.Find("...");
			if(((nPos > 0) && (nPos < 5)) || (nPos3Dot < 3))	// 路径中盘符后面的:
			{
				if((nPos > 0) && (nPos < 5))
				{
					nPos = strResult.Find(":", nPos+1);
				}else
				{
					nPos = strResult.Find(":");
				}
				if(nPos != -1)
				{
					CString strFile = strResult.Left(nPos);
					strResult.Delete(0, nPos+1);
					nPos = strResult.Find(":");
					if(nPos != -1)
					{
						m_nEvalResult = atoi(strResult.Left(nPos));
						strResult.Delete(0, nPos+1);
						strResult.TrimLeft();
					}
				}
			}

			if(nPosStack != -1)
			{
				strResult = m_strResult;
			}
		}
	}

	return strResult;
}

BOOL IsNumber(CString str)
{
	int nLength = str.GetLength();
	if (nLength > 2 && str[0] == '0' && str[1] == 'x')
	{
		for (int I = 2; I < nLength; I ++)
		{
			if (isdigit(str[I]) || (str[I] >= 'A' && str[I] <= 'F') ||
										(str[I] >= 'a' && str[I] <= 'f'))
				continue;
			return FALSE;
		}
		return TRUE;
	}
	if (! isdigit(str[0]))
		return FALSE;
	for (int I = 1; I < nLength; I ++)
	{
		if (! isdigit(str[I]) && str[I] != '+' &&
			str[I] != '-' && str[I] != '.' && str[I] != 'e' &&
			str[I] != 'E')
			return FALSE;
	}
	return TRUE;
}

// 设置变量值
int __stdcall CLuaInterp::SetVar(LPCTSTR lpszVarName, LPCTSTR lpszVarValue)
{
	// TODO: Add your code here
	/*CString strCmd;
	if(IsNumber(lpszVarName))
	{
		strCmd.Format("%s=%s", lpszVarName, lpszVarValue);
	}else
	{
		strCmd.Format("%s=\"%s\"", lpszVarName, lpszVarValue);
	}
	RunScriptCommand(strCmd);*/
	lua_pushstring(pLState, lpszVarValue);
	lua_setglobal(pLState, lpszVarName);

	return 0;
}

// 获取变量值
CString get_table_var_string(CLuaInterp* pInterp, lua_State* pLState)
{
	CString strValue = "{";
	//int t_idx = lua_gettop(pLState);
	lua_pushnil(pLState);
	while (lua_next(pLState, -2) != 0)
	{
		//if(lua_istable(pInterp->pLState, -1))	// 嵌套数组类型
		//{
			// 暂时不支持
			/*CString strVarName = lua_tostring(pInterp->pLState, -2);
			if(strVarName == "1")
			{
				lua_pop(pInterp->pLState, 1);
				lua_pop(pInterp->pLState, 1);
			}*/

			//lua_pop(pInterp->pLState, 1);
		//}else
		{
			strValue += lua_tostring(pLState, -2);
			strValue += "=";
			int ntype = lua_type(pLState, -1);
			if(ntype == LUA_TSTRING)
			{
				strValue += "'";
			}
			strValue += pInterp->GetValueString(-1);
			if(ntype == LUA_TSTRING)
			{
				strValue += "'";
			}
			if(ntype == LUA_TTABLE)
			{
				strValue += "(table)";
			}
			strValue += ", ";
			//lua_typename(pInterp->pLState, lua_type(pInterp->pLState, -1))

			lua_pop(pLState, 1); // pop value, keep key for next iteration
		}
	}
	lua_pop(pLState, 1); // pop table

	if(strValue.GetLength() > 1)
	{
		strValue = strValue.Left(strValue.GetLength()-2);
	}
	strValue += "}";

	return strValue;
}

// 获取变量值
CString __stdcall CLuaInterp::GetVar(LPCTSTR lpszVarName)
{
	// TODO: Add your code here
	/*
	CString strCmd;
	strCmd.Format("put_result(%s)", lpszVarName);
	RunScriptCommand(strCmd);

	if(GetErrorLine() == 0)
	{
		return GetResult();
	}*/

	// 首先获取局部变量
	CString strVarName;
	CString strValue;
	int nLevel = 0;
	lua_Debug ar;
	if ( lua_getstack (pLState, nLevel, &ar) )
	{
		int i = 1;
        const char *name;
        while ((name = lua_getlocal(pLState, &ar, i++)) != NULL) {
			int ntype = lua_type(pLState, -1);
			strVarName = name;
			if(strVarName == lpszVarName)
			{
				if(lua_istable(pLState, -1))
				{
					strValue = get_table_var_string(this, pLState);
				}else
				{
					strValue = GetValueString(-1);
					lua_pop(pLState, 1);  /* remove variable value */
				}
				//strType = lua_typename(pLState, ntype);
				return strValue;
			}
			lua_pop(pLState, 1);  /* remove variable value */
        }
	}

	// 获取全局变量
	lua_getglobal(pLState, lpszVarName);
	if(lua_istable(pLState, -1))
	{
		return get_table_var_string(this, pLState);
	}
	strValue = lua_tostring(pLState,-1);
	lua_pop(pLState, 1);

	return strValue;
}

// 注册扩展命令
int __stdcall CLuaInterp::RegisterCommand(LPVOID pInterp, LPCTSTR lpszCmd, LPVOID lpFunc)
{
	// TODO: Add your code here

	return 0;
}

// 设置扩展参数
int __stdcall CLuaInterp::SetInterpParam(LPCTSTR lpszParamName, LPCTSTR lpszParamValue)
{
	// TODO: Add your code here

	return 0;
}

// 获取扩展参数
CString __stdcall CLuaInterp::GetInterpParam(LPCTSTR lpszParamName)
{
	// TODO: Add your code here

	return "";
}

// 设置运行模式
int __stdcall CLuaInterp::SetRunMode(int nRunMode)
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
int __stdcall CLuaInterp::GetRunMode()
{
	return m_nRunMode;
}

// 设置运行状态
int __stdcall CLuaInterp::SetRunState(int nState)
{
	m_nRunState = nState;
	return true;
}

// 获取运行状态
int __stdcall CLuaInterp::GetRunState()
{
	return m_nRunState;
}

#define LEVELS1	12	/* size of the first part of the stack */
#define LEVELS2	10	/* size of the second part of the stack */

// 获取当前堆栈层数
int __stdcall CLuaInterp::GetCurStackLevel()
{
	// TODO: Add your code here
	int level = 1;  /* skip level 0 (it's this function) */
	int firstpart = 1;  /* still before eventual `...' */
	lua_Debug ar;
	while (lua_getstack(pLState, level++, &ar)) 
	{
		if (level > LEVELS1 && firstpart) 
		{
			/* no more than `LEVELS2' more levels? */
			if (!lua_getstack(pLState, level+LEVELS2, &ar))
				level--;  /* keep going */
			else 
			{
				while (lua_getstack(pLState, level+LEVELS2, &ar))  /* find last levels */
					level++;
			}
			firstpart = 0;
			continue;
		}
	}

	return level;
}

// 获取关键字列表
int __stdcall CLuaInterp::GetKeyWordList(LPCTSTR lpszType, CStringArray& asKeyWord)
{
	// TODO: Add your code here
	CString strType = lpszType;
	if(strType == "platform")	// 平台扩展关键字
	{
		asKeyWord.Add("MessageBox");
		asKeyWord.Add("FileDlg");
		asKeyWord.Add("PathSelect");
		asKeyWord.Add("ProgressDlg");
		asKeyWord.Add("OptionTree");
		asKeyWord.Add("tclLogin");
		asKeyWord.Add("output");
		asKeyWord.Add("StatusBar");
		asKeyWord.Add("pwait");
		asKeyWord.Add("TaskDialog");
		asKeyWord.Add("PopupControl");
		asKeyWord.Add("ImageManager");
	}else
	if(strType == "platform_keyword_tooltip")	// 平台扩展关键字使用说明
	{
		asKeyWord.Add("DESCRIPTION:\r\n MessageBox\r\nSYNOPSIS:\r\n MessageBox(?-option?, ?-title?, message)");
		asKeyWord.Add("DESCRIPTION:\r\n File dialog\r\nSYNOPSIS:\r\n FileDlg ?-option?...");
		asKeyWord.Add("DESCRIPTION:\r\n Path select dialog\r\nSYNOPSIS:\r\n PathSelect title ?InitPath?");
		asKeyWord.Add("DESCRIPTION:\r\n Progress dialog\r\nSYNOPSIS:\r\n ProgressDlg create ?caption? ?status? ?range? ?cancelscript?\r\n ProgressDlg setpos pos\r\n ProgressDlg setstatus msg\r\n ProgressDlg end");
		asKeyWord.Add("DESCRIPTION:\r\n Option tree dialog\r\nSYNOPSIS:\r\n OptionTree -optionList\r\n -- ?-option? ?T\"text\"? ?E\"edit\"? ?L\"list\"?\r\n -- option:\r\n -- -P(parentindex)\r\n -- -CHECK\r\n -- -OPT_MSG\r\n -- -OPT_HELP\r\n -- -OPT_ITEM\r\n -- -OPT_GROUP\r\n -- -OPT_CHECKBOX\r\n -- -OPT_RADIOBUTTON\r\n -- -OPT_EDIT\r\n -- -OPT_COMBO\r\n -- -ICO_FOLDER\r\n -- -ICO_GROUP\r\n -- -ICO_EDIT\r\n -- -ICO_COMBO\r\n -- -ICO_MFOLDER\r\n -- -ICO_MGROUP\r\n -- -ICO_MEDIT\r\n -- -ICO_MCOMBO\r\n -- -ICO_INFO\r\n -- -ICO_DOWNARROW\r\n -- -ICO_UPARROW");
		asKeyWord.Add("DESCRIPTION:\r\n Common Login dialog\r\nSYNOPSIS:\r\n tclLogin ?-title text?\r\n          ?-info text?\r\n          ?-laddress label?\r\n          ?-lusername label?\r\n          ?-lpassword label?\r\n          ?-ldomain label?\r\n          ?-address var default?\r\n          ?-username var default?\r\n          ?-password var default?\r\n          ?-domain var default?\r\n          ?-domainlist list?\r\n          ?-check var text default(true|false)?\r\n          ?-focus inputcontrol?");
		asKeyWord.Add("DESCRIPTION:\r\n Output\r\nSYNOPSIS:\r\n output -format -id|title FormatStr\r\n output -create title\r\n output -close -id|title\r\n output -active -id|title\r\n output -clear -id|title\r\n output -getinfo -id|title param\r\n output -getinfo title window-id\r\n output -direct text");
		asKeyWord.Add("DESCRIPTION:\r\n StatusBar\r\nSYNOPSIS:\r\n StatusBar -createpane\r\n StatusBar -delpane\r\n StatusBar -settext\r\n StatusBar -setwidth\r\n StatusBar -seticon\r\n StatusBar -progress");
		asKeyWord.Add("DESCRIPTION:\r\n Wait with event loop\r\nSYNOPSIS:\r\n pwait time(ms)\r\n pwait CreateEvent ?id?\r\n pwait WaitEvent id ?timeout?\r\n pwait AwakeEvent id\r\n pwait DeleteEvent id");
		asKeyWord.Add("DESCRIPTION:\r\n Task dialog\r\nSYNOPSIS:\r\n TaskDialog create optionList\r\n TaskDialog refresh optionList");
		asKeyWord.Add("DESCRIPTION:\r\n Popup control\r\nSYNOPSIS:\r\n PopupControl show optionList");
		asKeyWord.Add("DESCRIPTION:\r\n Image manager\r\nSYNOPSIS:\r\n ImageManager seticon command-id icon-file");
	}

	return asKeyWord.GetSize();
}

// 设置调试事件
int __stdcall CLuaInterp::SetDebugEvent(int nEvent, int nParam, LPCTSTR lpszParam)
{
	// TODO: Add your code here
	CString strParam = lpszParam;

	m_nCurCommand = nEvent;
	::SetEvent(m_hDebugEvent);

	if(nEvent == IDB_STOP)
	{
		//sprintf(szTclCmd, "td -stop");
	}else
	if(nEvent == IDB_NEXT)
	{
		//sprintf(szTclCmd, "td -next");
	}else
	if(nEvent == IDB_GOTO)
	{
		if(strParam.GetLength() == 0)
		{
			// 行断点
			SetBreakLineNo(nParam);
		}else
		{
			// 命名断点
		}
	}else
	if(nEvent == IDB_STEPINTO)
	{
		//sprintf(szTclCmd, "td -step into");
	}else
	if(nEvent == IDB_SETPOUT)
	{
		//sprintf(szTclCmd, "td -step return");
	}else
	if(nEvent == IDB_STEP)
	{
		//sprintf(szTclCmd, "td -step");
	}else
	if(nEvent == IDB_END)
	{
		return TRUE;
	}

	// 执行断点操作

	return TRUE;
}

// 设置断点列表
int __stdcall CLuaInterp::SetBreakList(LPCTSTR lpszFile, CUIntArray& auBreaks)
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
int __stdcall CLuaInterp::GetBreakList(LPCTSTR lpszFile, CUIntArray& auBreaks)
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
int __stdcall CLuaInterp::GetEnableProfileAnalyse()
{
	// TODO: Add your code here
	return m_bProfileEnable;
}

// 获取有效代码行数
int __stdcall CLuaInterp::GetAvailableLineCount()
{
	// TODO: Add your code here
	return m_nTDLineCount;
}

// 获取代码运行覆盖表
LPVOID __stdcall CLuaInterp::GetProfileMap()
{
	// TODO: Add your code here
	return (void*)(&m_ProfileMap);
}

// 判断指定方法是否被支持
int __stdcall CLuaInterp::MethodIsSupport(LPCTSTR lpszMethod)
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
int __stdcall CLuaInterp::InitialDebugMode(CString& strResult)
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
	pIPlatUI->OutputSet(m_nOutputStack, "AddColumn " + strColName + ",100");
	pIPlatUI->OutputSet(m_nOutputStack, "AddColumn " + strColSource + ",100");
	pIPlatUI->OutputSet(m_nOutputStack, "AddColumn " + strColLine + ",50");
	pIPlatUI->OutputSet(m_nOutputStack, "AddColumn " + strColWhat + ",50");
	pIPlatUI->OutputSet(m_nOutputStack, "AddColumn " + strColType + ",60,AutoSize");
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

CString CLuaInterp::LuaMakeNumber(double dnum)
{
	CString strRet;
    long num = (long) dnum;
    if ((double) num == dnum)
	{
        strRet.Format("%ld (0x%lx)", num, num);
	}else
	{
		strRet.Format("%g", dnum);
	}

    return strRet;
}

CString CLuaInterp::LuaMakeBoolean(int num)
{
    return num != 0 ? "true" : "false";
}

CString CLuaInterp::LuaGetTableInfo(int nIndex)
{
	CString strRet;
    //int         nTag   = wxlua_ttag(pLState, nIndex);
	#ifdef USE_LUA_51
	int         nItems = luaL_getn(pLState, nIndex);
	#endif
	#ifdef USE_LUA_52
    int         nItems = lua_objlen(pLState, nIndex);
	#endif
    const void *pItem  = lua_topointer(pLState, nIndex);

    strRet.Format("0x%p", pItem);

    if (nItems > 0)
        strRet.Format("0x%p (approx %d items)", pItem, nItems);

    //if (nTag != WXLUA_NOTAG)
    //    strRet.Format("0x%p (tag %d)"), pItem, nTag);
	return strRet;
}

int CLuaInterp::EnumerateTable(int nTableRef, int nIndex, CUIntArray& auReferences)
{
    int count = 0;

    CString strType;
    CString strValue;
    CString strName;

    //int flag_type = WXLUA_DEBUGITEM_TUNKNOWN;

    if ((nTableRef == -1) || (nTableRef == LUA_GLOBALSINDEX))
    {
        /*wxlState.GetGlobals();
        GetTypeValue(wxlState, -1, type, value, &flag_type);
        int nRef = wxlua_tinsert(L, -1);
        Add(new wxLuaDebugItem(wxT("Globals"), wxT("Table"), value, wxT(""), nRef, 0, WXLUA_DEBUGITEM_TTABLE|WXLUA_DEBUGITEM_LUAREFED));
        references.Add(nRef);*/
    }
    else if (nTableRef == LUA_REGISTRYINDEX)
    {
        /*wxlState.lua_PushValue(LUA_REGISTRYINDEX);
        GetTypeValue(wxlState, -1, type, value, &flag_type);
        int nRef = wxlua_tinsert(L, -1);
        Add(new wxLuaDebugItem(wxT("Lua Registry"), wxT("Table"), value, wxT(""), nRef, 0, WXLUA_DEBUGITEM_TTABLE|WXLUA_DEBUGITEM_LUAREFED));
        references.Add(nRef);*/
    }
    else
    {
        // push the table onto the stack to iterate through
        //if (wxlState.tget(tableRef))
        {
            int nTop = lua_gettop(pLState);

            //wxlua_pushkey_wxLuaObjects(L);
            lua_rawget(pLState, LUA_REGISTRYINDEX);       // pop key, push result (the obj table)
            bool is_lreg_objTable = (lua_equal(pLState, -1, -2) != 0);
            lua_pop(pLState, 1);
            
            // start iterating
            lua_pushnil(pLState);
            while (lua_next(pLState, nTop) != 0)
            {
                // get the index, just want the name=value, type is dummy here
                //GetTypeValue(wxlState, -2, type, name);
                // get the type and value
                //GetTypeValue(wxlState, -1, type, value, &flag_type);

                if (is_lreg_objTable)
                {
                    long tag = -1;
                    //if (value.BeforeFirst(wxT(' ')).ToLong(&tag))
                    //    value = value.BeforeFirst(wxT(' ')) + wxT(" (") + wxlState.GetLuaTagName(tag) + wxT(")");
                }

                //wxPrintf(wxT("wxLuaDebugData::EnumerateTable %d type='%s' key='%s' val='%s'\n"), count, type.c_str(), name.c_str(), value.c_str());

                int nRef = LUA_NOREF;
                if (lua_istable(pLState, -1))
                {
                    /*nRef = wxlua_tisrefed(L, -1); // don't duplicate refs

                    if (nRef == LUA_NOREF)
                    {
                        flag_type |= WXLUA_DEBUGITEM_LUAREFED;
                        nRef = wxlua_tinsert(L, -1); // pops value
                        references.Add(nRef);
                    }
                    else
                        lua_pop(pLState, 1);
					*/
                }
                else
                    lua_pop(pLState, 1);

                //Add(new wxLuaDebugItem(name, type, value, wxT(""), nRef, nIndex, flag_type));
                count++;
            }

            lua_pop(pLState, 1); // remove reference
        }
    }

    return count;
}

// 获取指定变量的值(转换为字符串)
CString CLuaInterp::GetValueString(int nIndex)
{
	CString strValue = "";
	int ntype = lua_type(pLState, nIndex);
	switch(ntype)
	{
	case LUA_TNIL:
		strValue = "nil";
		break;
	case LUA_TBOOLEAN:
		strValue = LuaMakeBoolean(lua_toboolean(pLState, nIndex));
		break;
	case LUA_TNUMBER:
		strValue = LuaMakeNumber(lua_tonumber(pLState, nIndex));
		break;
	case LUA_TSTRING:
		strValue.Format("%s", lua_tostring(pLState, nIndex));
		break;
	case LUA_TTABLE:
		strValue = LuaGetTableInfo(nIndex);
		break;
	case LUA_TFUNCTION:
		if (lua_iscfunction(pLState, nIndex))
		{
			strValue.Format("%p", lua_tocfunction(pLState, nIndex));
		}else
		{
			strValue.Format("%p", lua_topointer(pLState, nIndex));
		}
		break;
	case LUA_TTHREAD:
		strValue.Format("%p", lua_topointer(pLState, nIndex));
		break;
	default:
		break;
	}
	return strValue;
}

// 获取指定变量的描述信息
CString CLuaInterp::Describe(int nIndex)
{
	CString strRet;
	int ntype = lua_type(pLState, nIndex);
	strRet.Format("%s : %.64s", lua_typename(pLState, ntype), GetValueString(nIndex));
	return strRet;
}

// 获取指定数组变量的完整路径名
CString get_var_fullname(CLuaInterp* pInterp, CString strhItem)
{
	CString strArrayName = "";
	CString strTmp;
	CString strhParent = "";
	while((strhParent = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, "ItemParent " + strhItem)) != "ROOT")
	{
		strTmp.Format("<#%s,2>", strhItem);
		CString strItemType = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, strTmp);
		if(strItemType == "table")
		{
			strTmp.Format("<#%s,0>", strhItem);
			strArrayName = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, strTmp) + strArrayName;
		}
		strhItem = strhParent;
	}

	strTmp.Format("<#%s,2>", strhItem);
	CString strItemType = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, strTmp);
	if(strItemType == "table")
	{
		strTmp.Format("<#%s,0>", strhItem);
		strArrayName = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, strTmp) + strArrayName;
	}

	return strArrayName;
}

// 变量窗口双击处理
void callback_lua_ldclick(CLuaInterp* pInterp, LPCTSTR lpszInfo)
{
	TRACE("callback info=%s\n", lpszInfo);
	CString strInfo = lpszInfo;

	if(pInterp == NULL || !CheckLuaInterpExist(pInterp))
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

	if(strVarType == "table")
	{
		// 如果是table,就展开
		CString strhItem = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, "SelItem");

		// 判断是局部变量还是全局变量
		CString strParentItem = strhItem;
		while(pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, "ItemParent " + strParentItem) != "ROOT")
		{
			strParentItem = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, "ItemParent " + strParentItem);
		}
		CString strLocalGlobal = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, "<#" + strParentItem + ",0>");
		BOOL bIsLocal = (strLocalGlobal == "Locals");

		CString strTableName = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, "<#" + strhItem + ",0>");
		int nChildCount = atoi(pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, "ChildCount " + strhItem));
		if(nChildCount == 0)
		{
			CString strTmp;

			if(bIsLocal)
			{
				// 获取局部变量Table
				int nLevel = 0;
				lua_Debug ar;
				if ( lua_getstack (pInterp->pLState, nLevel, &ar) )
				{
					lua_getlocal(pInterp->pLState, &ar, 1);
				}
			}else
			{
				// 获取全局变量Table
				lua_getglobal(pInterp->pLState, strTableName);
			}
			if(lua_istable(pInterp->pLState, -1))
			{
				lua_pushnil(pInterp->pLState);
				while (lua_next(pInterp->pLState, -2))
				{
					//if(lua_istable(pInterp->pLState, -1))	// 嵌套数组类型
					//{
						// 暂时不支持
						/*CString strVarName = lua_tostring(pInterp->pLState, -2);
						if(strVarName == "1")
						{
							lua_pop(pInterp->pLState, 1);
							lua_pop(pInterp->pLState, 1);
						}*/

						//lua_pop(pInterp->pLState, 1);
					//}else
					{
						strTmp.Format("<%s,7>%s%s%s%s%s", strhItem,
								lua_tostring(pInterp->pLState, -2), SEPATATOE,
								pInterp->GetValueString(-1), SEPATATOE,
								lua_typename(pInterp->pLState, lua_type(pInterp->pLState, -1)));
						pInterp->pIPlatUI->Output(pInterp->m_nOutputVar, strTmp);

						// 设置双击处理函数
						CString strlasthItem = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputVar, "LastItem");
						pInterp->pIPlatUI->Output(pInterp->m_nOutputVar, "<I>Set Script<#" + strlasthItem + "> <1>");

						lua_pop(pInterp->pLState, 1); // pop value, keep key for next iteration
					}
				}
				lua_pop(pInterp->pLState, 1); // pop table

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
		strVarName = get_var_fullname(pInterp, strhItem) + "[" + strVarName + "]";
		CString strMsg;
		strMsg.Format("%s:%s\n%s:%s\n%s:%s", strWatchVarName, strVarName, strWatchVarValue, strVarValue, strWatchVarType, strVarType);
		::MessageBox(AfxGetMainWnd()->m_hWnd, strMsg, strWatchVarTitle, MB_OK | MB_ICONINFORMATION);
	}
}

// 刷新变量窗口
int CLuaInterp::RefreshVarWindow()
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

	int nLevel = 0;
	lua_Debug ar;
	if ( lua_getstack (pLState, nLevel, &ar) )
	{
		int i = 1;
        const char *name;
        while ((name = lua_getlocal(pLState, &ar, i++)) != NULL) {
			int ntype = lua_type(pLState, -1);
			strName = name;
			if(strName == "(*temporary)")	// 过滤掉临时变量
			{
				lua_pop(pLState, 1);  /* remove variable value */
				continue;
			}

			strValue = GetValueString(-1);
			strType = lua_typename(pLState, ntype);

			strOutput =	strName + SEPATATOE + strValue + SEPATATOE + strType;
			item.strLine = strOutput;
			item.nParent = nParent;
			item.nImage = 5;
			item.nExpand = 0;
			item.nClickFunc = 1;
			aItems.Add(item);

			lua_pop(pLState, 1);  /* remove variable value */
        }
	}

	// Globals
	strOutput = "Globals";
	item.strLine = strOutput;
	item.nParent = -1;
	item.nImage = 4;
	item.nExpand = 1;
	item.nClickFunc = 0;
	aItems.Add(item);
	nParent = aItems.GetSize()-1;

	#ifdef USE_LUA_51
	lua_pushvalue(pLState, LUA_GLOBALSINDEX);
	#endif
	#ifdef USE_LUA_52
	//lua_pushvalue(pLState, LUA_GLOBALSINDEX);	// Lua 5.2没有global
	#endif

	lua_pushnil(pLState);  /* first key */
	while (lua_next(pLState, -2))
	{
		// 过滤掉函数
		if(LUA_TFUNCTION == lua_type(pLState, -1))
		{
			lua_pop(pLState, 1); // pop value, keep key for next iteration;
			continue;
		}

		// 获取每个变量信息
		strName = lua_tostring(pLState, -2);
		strValue = GetValueString(-1);
		strType = lua_typename(pLState, lua_type(pLState, -1));
		strOutput =	strName + SEPATATOE + strValue + SEPATATOE + strType;
		item.strLine = strOutput;
		item.nParent = nParent;
		item.nImage = 5;
		item.nExpand = 0;
		item.nClickFunc = 1;
		aItems.Add(item);
		
		lua_pop(pLState, 1); // pop value, keep key for next iteration;
	}
	lua_pop(pLState, 1); // pop table of globals;

	pIPlatUI->SendCommand(OC_TREEOUTPUT, m_nOutputVar, (LPARAM)(LPVOID)(&aItems));

	CString strCallback;
	strCallback.Format("<I>Set Script<0> <<cfunction:%d>>%d,%d", (int)callback_lua_ldclick, (int)this, 1);
	pIPlatUI->Output(m_nOutputVar, strCallback);

	return TRUE;
}

// 刷新函数窗口
int CLuaInterp::RefreshFuncWindow()
{
	CString strOutput, strName, strType;

	CTTreeItemArray aItems;

	TTREEITEM item;
	item.hItem = NULL;
	item.nParent = -1;
	item.nExpand = 0;

	#ifdef USE_LUA_51
	lua_pushvalue(pLState, LUA_GLOBALSINDEX);
	#endif
	#ifdef USE_LUA_52
	//lua_pushvalue(pLState, LUA_GLOBALSINDEX);	// Lua 5.2没有global
	#endif

	lua_pushnil(pLState);  /* first key */
	while (lua_next(pLState, -2))
	{
		int nType = lua_type(pLState, -1);
		if(LUA_TFUNCTION != nType)
		{
			lua_pop(pLState, 1); // pop value, keep key for next iteration;
			continue;
		}

		// 获取每个函数信息
		strName = lua_tostring(pLState, -2);
		if(lua_iscfunction(pLState, -1))
		{
			strType = "cfunction";
		}else
		{
			strType = lua_typename(pLState, nType);
		}
		strOutput =	strName + SEPATATOE + strType;
		item.strLine = strOutput;
		item.nImage = 5;
		aItems.Add(item);
		
		lua_pop(pLState, 1); // pop value, keep key for next iteration;
	}
	lua_pop(pLState, 1); // pop table of globals;

	pIPlatUI->SendCommand(OC_TREEOUTPUT, m_nOutputFunc, (LPARAM)(LPVOID)(&aItems));

	return TRUE;
}

// Lua堆栈窗口行双击处理
void callback_lua_stack_ldclick(CLuaInterp* pInterp, LPCTSTR lpszInfo)
{
	TRACE("callback info=%s\n", lpszInfo);
	CString strInfo = lpszInfo;

	if(pInterp == NULL || !CheckLuaInterpExist(pInterp))
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
	strTmp.Format("<%d,1>", lpItem);
	CString strStackSource = pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputStack, strTmp);
	strTmp.Format("<%d,2>", lpItem);
	int nStackLine = atoi(pInterp->pIPlatUI->OutputGet(pInterp->m_nOutputStack, strTmp));

	if(GetFileAttributes(strStackSource) == 0xFFFFFFFF)
	{
		return;
	}

	pInterp->pIPlatUI->SendOwmCmd("org.owm.edit", OC_OPENFILE, (WPARAM)(LPTSTR)(LPCTSTR)strStackSource, 0, NULL);
	pInterp->pIPlatUI->SendOwmCmd("org.owm.edit", OC_SETCURENTLINE, (WPARAM)(LPTSTR)(LPCTSTR)strStackSource, nStackLine, NULL);
}

// 刷新堆栈窗口
int CLuaInterp::RefreshStackWindow()
{
	CString strOutput;

	CTTreeItemArray aItems;

	TTREEITEM item;
	item.hItem = NULL;
	item.nParent = -1;
	item.nExpand = 0;
	item.nImage = 27;
	item.nClickFunc = 1;

	int nLevel = 0;
	lua_Debug ar;
	CString strTmp;
	while ( lua_getstack (pLState, nLevel, &ar) )
	{
		lua_getinfo(pLState, "lnuS", &ar);
		if ( ar.source[0]=='@' )
		{
			// 第一个字符为@表示是一个文件
			strOutput = "";

			if ( ar.name )
				strOutput += ar.name;
			strOutput += SEPATATOE;

			strOutput += ar.source+1;
			strOutput += SEPATATOE;

			strTmp.Format("%d", ar.currentline);
			strOutput += strTmp;
			strOutput += SEPATATOE;

			if ( ar.what )
				strOutput += ar.what;
			strOutput += SEPATATOE;

			if ( ar.namewhat )
				strOutput += ar.namewhat;

			item.strLine = strOutput;
			aItems.Add(item);
		}else
		{
			// 是一个脚本字符串
		}

		++nLevel;
	};

	pIPlatUI->SendCommand(OC_TREEOUTPUT, m_nOutputStack, (LPARAM)(LPVOID)(&aItems));

	CString strCallback;
	strCallback.Format("<I>Set Script<0> <<cfunction:%d>>%d,%d", (int)callback_lua_stack_ldclick, (int)this, 1);
	pIPlatUI->Output(m_nOutputStack, strCallback);

	return TRUE;
}

// 更新调试窗口信息
int __stdcall CLuaInterp::RefreshDebugWindow(LPCTSTR lpszWindow, BOOL bActive, CString& strResult)
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
		bWindowProc = TRUE;
		bWindowStack = TRUE;
	}

	if(bWindowVar)
	{
		RefreshVarWindow();
		if(bActive)
		{
			pIPlatUI->ActiveOutput(m_nOutputVar);
		}
	}
	if(bWindowObject)
	{
		if(bActive)
		{
			pIPlatUI->ActiveOutput(m_nOutputObject);
		}
	}
	if(bWindowProc)
	{
		RefreshFuncWindow();
		if(bActive)
		{
			pIPlatUI->ActiveOutput(m_nOutputFunc);
		}
	}
	if(bWindowStack)
	{
		RefreshStackWindow();
		if(bActive)
		{
			pIPlatUI->ActiveOutput(m_nOutputStack);
		}
	}

	return TRUE;
}

// 调用脚本方法
int __stdcall CLuaInterp::CallMethod(LPCTSTR lpszMethod, CStringArray& asParam)
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
int __stdcall CLuaInterp::Compile(LPCTSTR lpszSrcFile, CString& strDestFile, LPCTSTR lpszParams)
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

	// 运行状态
	m_nRunState = SCRIPT_STATE_RUN;

	CString strProCompFile	= GetInterpRootPath();
	strProCompFile	+= "bin\\luac5.1.exe";

	CString strOutFile = strSrcFile;
	strOutFile.MakeLower();
	int nDotPos = strOutFile.ReverseFind('.');
	if(nDotPos != -1)
	{
		strOutFile.Delete(nDotPos, strOutFile.GetLength() - nDotPos);
	}

	// 自动设置输出文件
	BOOL bAutoOutFile = AfxGetApp()->GetProfileInt(REG_LUA_COMPILE_SUBKEY, REG_LUA_COMPILE_AUTO_OUTFILE, TRUE);
	if(bAutoOutFile)
	{
		strOutFile += ".luac";
	}else
	{
		// 用户输入输出文件
		CFileDialog dialog(FALSE, "*.*", "", OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, "All files(*.*)|*.*||");
		if(IDOK == dialog.DoModal())
		{
			strOutFile = dialog.GetPathName();
		}else
		{
			m_nRunState = SCRIPT_STATE_END;
			return FALSE;
		}
	}

	// 文件如果存在，是否强制覆盖
	BOOL bForceOverwrite = AfxGetApp()->GetProfileInt(REG_LUA_COMPILE_SUBKEY, REG_LUA_COMPILE_FORCE_OVERWRITE, FALSE);
	if(!bForceOverwrite)
	{
		if(GetFileAttributes(strOutFile) != 0xFFFFFFFF)
		{
			if(AfxMessageBox("Compile output file is exist, overwrite it or not?", MB_YESNO | MB_ICONQUESTION) == IDNO)
			{
				m_nRunState = SCRIPT_STATE_END;
				return FALSE;
			}
		}
	}

	CString strParams = "";

	strParams += "-o \"";
	strParams += strOutFile;
	strParams += "\"";

	strParams += " \"";
	strParams += strSrcFile;
	strParams += "\"";

	strDestFile = strOutFile;

	try {
	////////////////////////////////////////////
	if(!RunProgramWithPipe(strProCompFile, strParams))
	{
		m_nRunState = SCRIPT_STATE_END;
		return FALSE;
	}
	////////////////////////////////////////////
	}catch(...)
	{
		m_nRunState = SCRIPT_STATE_END;
	}

	m_nRunState = SCRIPT_STATE_END;
	return TRUE;
}

// 构建
int __stdcall CLuaInterp::Build(LPCTSTR lpszSrcFile, CString& strDestFile, LPCTSTR lpszParams)
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

	// wxluafreeze.lua文件名
	CString strFreezeLua = GetInterpRootPath();
	strFreezeLua += "bin\\wxluafreeze.lua";

	// wxluafreeze.exe文件名
	CString strFreezeLuaApp = GetInterpRootPath();
	strFreezeLuaApp += "bin\\wxluafreeze.exe";

	CString strCmd;
	strCmd.Format("arg={\"%s\",\"%s\",\"%s\"}", strFreezeLuaApp, strSrcFile, strOutFile + "\\" + strOutFileName + ".exe");
	strCmd.Replace("\\", "\\\\");
	RunScriptCommand(strCmd);

	// 执行wxluafreeze.lua
	RunScriptFile(strFreezeLua);

	strDestFile = strOutFile + "\\" + strOutFileName + ".exe";

	// 替换可执行文件的图标
	CString strIconFile = GetInterpRootPath();
	strIconFile += "bin\\luaexe.ico";	// 默认的图标文件
	strIconFile.Replace("/", "\\");
	BOOL bReplaceIcon = AfxGetApp()->GetProfileInt(REG_LUA_BUILD_SUBKEY, REG_LUA_BUILD_REPLACE_ICON, FALSE);
	if(bReplaceIcon)
	{
		// 图标选择的方式
		CString strNewIcon = "";
		int nSelectIconType = AfxGetApp()->GetProfileInt(REG_LUA_BUILD_SUBKEY, REG_LUA_BUILD_SELECT_ICON_TYPE, 0);
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
INTERFACE_IMPLEMENT(LuaInterp)
CONTROL_INTERFACE_IMPLEMENT(LuaInterp)

//{{ 注意：!!!这里是保留的重要函数，不可删除!!!
// 获取接口的额外信息
LPVOID __stdcall CLuaInterp::XLuaInterp::GetExtraInfo(LPVOID lpRefData)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj;
}
//}} 注意：!!!这里是保留的重要函数，不可删除!!!
//{{VCI_INTERFACE_IMPLEMENT_BEGIN
// 设置平台UI接口指针
int __stdcall CLuaInterp::XLuaInterp::SetIPlatUI(LPVOID lpIPlatUI)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->SetIPlatUI(lpIPlatUI);
}

int __stdcall CLuaInterp::XLuaInterp::SetLibPath(LPCTSTR lpszPath)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->SetLibPath(lpszPath);
}

int __stdcall CLuaInterp::XLuaInterp::IsInterpRunning()
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->IsInterpRunning();
}

int __stdcall CLuaInterp::XLuaInterp::SetScriptFile(LPCTSTR lpszPathName)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->SetScriptFile(lpszPathName);
}

CString __stdcall CLuaInterp::XLuaInterp::GetScriptFile()
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->GetScriptFile();
}

int __stdcall CLuaInterp::XLuaInterp::RunScriptFile(LPCTSTR lpszPathName)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->RunScriptFile(lpszPathName);
}

int __stdcall CLuaInterp::XLuaInterp::RunScriptCommand(LPCTSTR lpszTclCmd)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->RunScriptCommand(lpszTclCmd);
}

int __stdcall CLuaInterp::XLuaInterp::StopScript()
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->StopScript();
}

LPVOID __stdcall CLuaInterp::XLuaInterp::GetInterp()
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->GetInterp();
}

int __stdcall CLuaInterp::XLuaInterp::GetInterpID()
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->GetInterpID();
}

void __stdcall CLuaInterp::XLuaInterp::SetInterpName(LPCTSTR lpszInterptName)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	pObj->SetInterpName(lpszInterptName);
}

CString __stdcall CLuaInterp::XLuaInterp::GetInterpName()
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->GetInterpName();
}

int __stdcall CLuaInterp::XLuaInterp::GetErrorLine()
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->GetErrorLine();
}

CString __stdcall CLuaInterp::XLuaInterp::GetResult()
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->GetResult();
}

int __stdcall CLuaInterp::XLuaInterp::SetVar(LPCTSTR lpszVarName, LPCTSTR lpszVarValue)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->SetVar(lpszVarName, lpszVarValue);
}

CString __stdcall CLuaInterp::XLuaInterp::GetVar(LPCTSTR lpszVarName)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->GetVar(lpszVarName);
}

int __stdcall CLuaInterp::XLuaInterp::RegisterCommand(LPVOID pInterp, LPCTSTR lpszCmd, LPVOID lpFunc)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->RegisterCommand(pInterp, lpszCmd, lpFunc);
}

int __stdcall CLuaInterp::XLuaInterp::SetInterpParam(LPCTSTR lpszParamName, LPCTSTR lpszParamValue)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->SetInterpParam(lpszParamName, lpszParamValue);
}

CString __stdcall CLuaInterp::XLuaInterp::GetInterpParam(LPCTSTR lpszParamName)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->GetInterpParam(lpszParamName);
}

int __stdcall CLuaInterp::XLuaInterp::SetRunMode(int nRunMode)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->SetRunMode(nRunMode);
}

int __stdcall CLuaInterp::XLuaInterp::GetRunMode()
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->GetRunMode();
}

int __stdcall CLuaInterp::XLuaInterp::SetRunState(int nRunState)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->SetRunState(nRunState);
}

int __stdcall CLuaInterp::XLuaInterp::GetRunState()
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->GetRunState();
}

int __stdcall CLuaInterp::XLuaInterp::GetCurStackLevel()
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->GetCurStackLevel();
}

// 获取关键字列表
int __stdcall CLuaInterp::XLuaInterp::GetKeyWordList(LPCTSTR lpszType, CStringArray& asKeyWord)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->GetKeyWordList(lpszType, asKeyWord);
}

int __stdcall CLuaInterp::XLuaInterp::SetDebugEvent(int nEvent, int nParam, LPCTSTR lpszParam)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->SetDebugEvent(nEvent, nParam, lpszParam);
}

int __stdcall CLuaInterp::XLuaInterp::SetBreakList(LPCTSTR lpszFile, CUIntArray& auBreaks)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->SetBreakList(lpszFile, auBreaks);
}

int __stdcall CLuaInterp::XLuaInterp::GetBreakList(LPCTSTR lpszFile, CUIntArray& auBreaks)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->GetBreakList(lpszFile, auBreaks);
}

int __stdcall CLuaInterp::XLuaInterp::GetEnableProfileAnalyse()
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->GetEnableProfileAnalyse();
}

int __stdcall CLuaInterp::XLuaInterp::GetAvailableLineCount()
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->GetAvailableLineCount();
}

LPVOID __stdcall CLuaInterp::XLuaInterp::GetProfileMap()
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->GetProfileMap();
}

int __stdcall CLuaInterp::XLuaInterp::MethodIsSupport(LPCTSTR lpszMethod)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->MethodIsSupport(lpszMethod);
}

int __stdcall CLuaInterp::XLuaInterp::InitialDebugMode(CString& strResult)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->InitialDebugMode(strResult);
}

int __stdcall CLuaInterp::XLuaInterp::RefreshDebugWindow(LPCTSTR lpszWindow, BOOL bActive, CString& strResult)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->RefreshDebugWindow(lpszWindow, bActive, strResult);
}

int __stdcall CLuaInterp::XLuaInterp::CallMethod(LPCTSTR lpszMethod, CStringArray& asParam)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->CallMethod(lpszMethod, asParam);
}

int __stdcall CLuaInterp::XLuaInterp::Compile(LPCTSTR lpszSrcFile, CString& strDestFile, LPCTSTR lpszParams)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
	return pObj->Compile(lpszSrcFile, strDestFile, lpszParams);
}

int __stdcall CLuaInterp::XLuaInterp::Build(LPCTSTR lpszSrcFile, CString& strDestFile, LPCTSTR lpszParams)
{
	CLuaInterp *pObj = GET_INTERFACE_OBJECT(LuaInterp);
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
