#include "stdafx.h"
#include "CTclInterp.h"
#include "TclPcom.h"
#include "TclXml.h"
#include "TclExCmd.h"
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

// 当前语言
static int g_nCurrentLanguage = VCI_LANGUAGE_ENGLISH;
int gGetCurrentLanguage()
{
	return g_nCurrentLanguage;
}

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

/////////////////////////////////////////////////////////////////////////////
// 转换结果,普通字符串转换为UTF-8编码
/////////////////////////////////////////////////////////////////////////////
void ConvertResultToUTF(Tcl_Interp* interp)
{
	Tcl_DString dst;

	//Tcl_ExternalToUtfDString(NULL, interp->result, -1, &dst);
	Tcl_ExternalToUtfDString(NULL, Tcl_GetStringResult(interp), -1, &dst);

	CString szResult = dst.string;
	//strcpy(interp->result, szResult);
	Tcl_ResetResult(interp);
	Tcl_AppendResult(interp, szResult, (char*) NULL);
	Tcl_DStringFree(&dst);
}

/////////////////////////////////////////////////////////////////////////////
// 转换字符串,普通字符串转换为UTF-8编码
/////////////////////////////////////////////////////////////////////////////
CString ConvertStringToUTF(CString strParam)
{
	Tcl_DString dst;
	
	Tcl_ExternalToUtfDString(NULL, strParam, -1, &dst);
	
	CString szResult = dst.string;
	Tcl_DStringFree(&dst);

	return szResult;
}

/////////////////////////////////////////////////////////////////////////////
// 转换参数,UTF-8编码字符串转换为普通字符串
/////////////////////////////////////////////////////////////////////////////
void ConvertUTFToString(int argc, char* argv[])
{
	int i;
	Tcl_DString dst;
	char* result;

	for(i = 0; i < argc; i++ )
	{
		result = Tcl_UtfToExternalDString(NULL, argv[i], strlen(argv[i]), &dst);
		strcpy(argv[i], result);
		Tcl_DStringFree(&dst);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 转换单个参数,UTF-8编码字符串转换为普通字符串
/////////////////////////////////////////////////////////////////////////////
CString ConvertUTFParam(CString strParam)
{
	Tcl_DString dst;

	Tcl_UtfToExternalDString(NULL, strParam, strlen(strParam), &dst);
	CString szResult = dst.string;

	Tcl_DStringFree(&dst);

	return szResult;
}

/////////////////////////////////////////////////////////////////////////////
// 创建一个Tcl的List对象,并添加字符串数组的内容,返回Tcl对象指针
/////////////////////////////////////////////////////////////////////////////
Tcl_Obj* CreateTclList(Tcl_Interp* interp, CStringArray& asItem)
{
	Tcl_Obj* listObj = Tcl_NewListObj(0, NULL);
	for(int el = 0; el < asItem.GetSize(); el++)
	{
		CString strElement = asItem[el];
		strElement = ConvertStringToUTF(strElement);
		Tcl_Obj* objPtr = Tcl_NewStringObj(strElement.GetBuffer(strElement.GetLength()), strElement.GetLength());
		strElement.ReleaseBuffer();
		Tcl_ListObjAppendElement(interp, listObj, objPtr);
	}

	return listObj;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:TCL脚本调试扩展命令
// 参数1:TCL解释对象指针
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_Plat_Debug(ClientData clientData, Tcl_Interp* interp, int argc, const char* argv[])
{
	// 获取TCL解释器对象
	CTclInterp* pTclInterp = (CTclInterp*)(GetTclInterp(interp));
	if(pTclInterp == NULL)
	{
		Tcl_AppendResult(interp, "tcl debug error, tclinterp pointer invalid", (char *) NULL);
		return TCL_ERROR;
	}

	// 如果是普通运行模式,则不用理会此调试命令
	if(SCRIPT_RUN_NORMAL == pTclInterp->GetRunMode())
	{
		return TCL_OK;
	}

	IPlatUI* pIPlatUI = pTclInterp->pIPlatUI;

	int nBreakLineNo = 0;
	CString strBreakName = "";
	CString strBreakFile = "";
	CDebugFile* pDebugFile = NULL;

	for(int i=1; i<argc; i++)
	{
		CString strOption = argv[i];
		if(strOption[0] == '*')	// 处理行断点
		{
			strOption.Delete(0, 1);
			nBreakLineNo = atoi(strOption);	// 断点行号

			// 如果后面还有一个参数,表示是多文件调试的文件名信息
			if((i+1) < argc)
			{
				CString strDebugFileIndex = argv[i+1];
				int nDebugFileIndex = atoi(strDebugFileIndex);
				if(nDebugFileIndex < pTclInterp->m_aDebugFile.GetSize())
				{
					pDebugFile = &(pTclInterp->m_aDebugFile[nDebugFileIndex]);
				}
				if(pDebugFile)
				{
					strBreakFile = pDebugFile->m_strFile;
					if(pTclInterp->GetEnableProfileAnalyse())
					{
						CMap<int,int,int,int>* pProfileMap = (CMap<int,int,int,int>*)(&(pDebugFile->m_ProfileMap));
						pProfileMap->SetAt(nBreakLineNo, 1);
					}
				}

				i++;
			}else
			{
				if(pTclInterp->GetEnableProfileAnalyse())
				{
					CMap<int,int,int,int>* pProfileMap = (CMap<int,int,int,int>*)(pTclInterp->GetProfileMap());
					pProfileMap->SetAt(nBreakLineNo, 1);
				}
			}
		}else
		if(strOption[0] == '#')	// 处理命名断点
		{
			strOption.Delete(0, 1);
			strBreakName = strOption;	// 断点名
		}
		if(strOption[0] == '-')	// 处理调试命令
		{
			strOption.Delete(0, 1);
			strOption.MakeUpper();
			if(strOption == "STOP")	// 中止脚本
			{
				pTclInterp->StopScript();
			}else
			if(strOption == "NEXT")	// 到下一个断点
			{
				pTclInterp->SetBreakMode(SCRIPT_BREAK_NORMAL);
				pTclInterp->SetBreakSignal(TRUE);	// 激活脚本
				pTclInterp->SetRunMode(SCRIPT_RUN_DEBUG_GO);	// 设置为断点模式
			}else
			if(strOption == "GOTO")	// 执行到某个行断点或命名断点
			{
				if(argc < (i+2))
				{
					interp->result = "wrong # args: should be \"td -goto *lineno|#breakname\"";
					return TCL_ERROR;
				}
				// 分析后面一个参数获取行号或断点名
				CString strBreak = argv[i+1];
				if(strBreak[0] == '*')
				{
					strBreak.Delete(0, 1);
					pTclInterp->SetBreakMode(SCRIPT_BREAK_LINE);
					pTclInterp->SetBreakLineNo(atoi(strBreak));	// 断点行号
					pTclInterp->SetBreakSignal(TRUE);	// 激活脚本
				}else
				if(strBreak[0] == '#')
				{
					strBreak.Delete(0, 1);
					pTclInterp->SetBreakMode(SCRIPT_BREAK_NAME);
					pTclInterp->SetBreakName(strBreak);	// 断点名
					pTclInterp->SetBreakSignal(TRUE);	// 激活脚本
				}
				pTclInterp->SetRunMode(SCRIPT_RUN_DEBUG_GO);	// 设置为断点模式
			}else
			if(strOption == "STEP")	// 处理单步命令
			{	// 设置单步的模式
				if((argc > (i+1)) && (strcmp(argv[i+1], "into") == 0))
				{	// 进入过程的单步
					pTclInterp->SetBreakMode(SCRIPT_STEP_INTO);
				}else
				if((argc > (i+1)) && (strcmp(argv[i+1], "return") == 0))
				{	// 由过程返回
					pTclInterp->SetBreakMode(SCRIPT_STEP_RETURN);
				}else
				{	// 不进入过程的单步
					pTclInterp->SetBreakMode(SCRIPT_STEP_NEXT);
				}
				// 设置当前堆栈深度
				//pTclInterp->SetBreakLevel();
				pTclInterp->SetBreakSignal(TRUE);	// 激活脚本
				pTclInterp->SetRunMode(SCRIPT_RUN_DEBUG_STEP);	// 设置为单步模式
			}
			if(strOption == "LOAD")	// 用OwmEdit加载一个脚本进行调试
			{
				if(argc < (i+2))
				{
					interp->result = "wrong # args: should be \"td -load tclfile\"";
					return TCL_ERROR;
				}
				// 打开OwmEdit,并加载脚本文件
				// 未完成
			}
			return TCL_OK;
		}
	}

	// 检测脚本中止信号,判断是否结束脚本运行
	if(pTclInterp->GetStopSignal())
	{
		// 向OwmEdit发送当前行清除命令信息
		pTclInterp->DebugCallback(IDB_USER_SETCURRENTLINE, 0, strBreakFile);
		pTclInterp->SetStopSignal(FALSE);
		pTclInterp->SetRunState(SCRIPT_STATE_STOP);
		Tcl_AppendResult(interp, "script terminated at user request.", (char *) NULL);
		pTclInterp->TkDestroyWindow();
		return TCL_ERROR;
	}

	// 断点运行模式
	if(SCRIPT_RUN_DEBUG_GO == pTclInterp->GetRunMode())
	{
		// 检查是否符合断点条件
		if(SCRIPT_BREAK_LINE == pTclInterp->GetBreakMode())	// 行断点
		{
			if(nBreakLineNo != pTclInterp->GetBreakLineNo())
			{
				// 行断点条件不符合,不进入断点
				// 向OwmEdit发送当前行清除命令信息
				pTclInterp->DebugCallback(IDB_USER_SETCURRENTLINE, 0, strBreakFile);
				return TCL_OK;
			}
		}else
		if(SCRIPT_BREAK_NAME == pTclInterp->GetBreakMode())	// 命名断点
		{
			if(strBreakName != pTclInterp->GetBreakName())
			{
				// 命名断点条件不符合,不进入断点
				// 向OwmEdit发送当前行清除命令信息
				pTclInterp->DebugCallback(IDB_USER_SETCURRENTLINE, 0, strBreakFile);
				return TCL_OK;
			}
		}else
		{	// 普通断点
			CUIntArray auBreakList;
			pTclInterp->GetBreakList("", auBreakList);
			BOOL bFind = FALSE;
			for(int i=0; i<auBreakList.GetSize(); i++)
			{
				if((UINT)nBreakLineNo == auBreakList[i])
				{
					bFind = TRUE;
					break;
				}
			}
			if(!bFind)
			{	// 普通断点条件不符合,不进入断点
				// 向OwmEdit发送当前行清除命令信息
				pTclInterp->DebugCallback(IDB_USER_SETCURRENTLINE, 0, strBreakFile);
				return TCL_OK;
			}
		}
	}else
	if(SCRIPT_RUN_DEBUG_STEP == pTclInterp->GetRunMode())	// 单步
	{
		int nLevel = pTclInterp->GetCurStackLevel();
		int nOldLevel = pTclInterp->GetStackLevel();
		if(SCRIPT_STEP_NEXT == pTclInterp->GetBreakMode())
		{	// 单步不进入过程
			if(nLevel > nOldLevel)
			{
				return TCL_OK;
			}
		}else
		if(SCRIPT_STEP_RETURN == pTclInterp->GetBreakMode())
		{	// 由过程返回
			if(nLevel >= nOldLevel && nOldLevel != -1)
			{
				return TCL_OK;
			}
		}else
		if(SCRIPT_STEP_INTO == pTclInterp->GetBreakMode())
		{	// 单步进入过程
		}
		//pTclInterp->SetStackLevel(nLevel);
	}

	// 保存堆栈深度参数,用于下一次的比较
	pTclInterp->SetStackLevel(pTclInterp->GetCurStackLevel());

	if(pDebugFile && pIPlatUI)
	{
		pIPlatUI->OpenDocumentFile(strBreakFile, "");
		// 向OwmEdit发送设置运行模式为从调试模式的命令
		pIPlatUI->SendOwmCmd("org.owm.edit", OC_SETSLAVEDEBUG,
			(WPARAM)(LPTSTR)(LPCTSTR)strBreakFile, 1, NULL);
	}

	// 向OwmEdit发送断点行号信息
	pTclInterp->DebugCallback(IDB_USER_SETCURRENTLINE, nBreakLineNo, strBreakFile);

	pTclInterp->SetRunState(SCRIPT_STATE_BREAK);

	MSG msg;
	while(TRUE)
	{
		//if(::PeekMessage(&msg, NULL, NULL, NULL,PM_REMOVE))
		// 使用GetMessage比PeekMessage好,不会出现系统反映变慢地情况
		if(::GetMessage(&msg, NULL, NULL, NULL))
		{															
			::TranslateMessage(&msg);										
			::DispatchMessage(&msg);
		}

		// 再次检测脚本中止信号,判断是否结束脚本运行
		if(pTclInterp->GetStopSignal())
		{
			// 向OwmEdit发送当前行清除命令信息
			pTclInterp->DebugCallback(IDB_USER_SETCURRENTLINE, 0, strBreakFile);
			pTclInterp->SetStopSignal(FALSE);
			pTclInterp->SetBreakSignal(FALSE);
			pTclInterp->SetRunState(SCRIPT_STATE_STOP);
			Tcl_AppendResult(interp, "script terminated at user request.", (char *) NULL);
			pTclInterp->TkDestroyWindow();
			return TCL_ERROR;
		}

		// 检测断点激活信号
		if(pTclInterp->GetBreakSignal())
		{
			pTclInterp->SetBreakSignal(FALSE);
			break;
		}
	}

	pTclInterp->SetRunState(SCRIPT_STATE_RUN);

	// 向OwmEdit发送当前行清除命令信息
	pTclInterp->DebugCallback(IDB_USER_SETCURRENTLINE, 0, strBreakFile);

	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:替换TCL自身的source命令
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_Source(ClientData clientData, Tcl_Interp* interp, int argc, const char* argv[])
{
	if(argc < 2)
	{
		interp->result = "wrong # args: should be \"source file\"";
		return TCL_ERROR;
	}

	CTclInterp* pTclInterp = (CTclInterp*)(GetTclInterp(interp));
	if(pTclInterp == NULL)
	{
		Tcl_AppendResult(interp, "source error, tclinterp pointer invalid", (char *) NULL);
		return TCL_ERROR;
	}

	CString strFile = ConvertUTFParam(argv[1]);
	strFile.MakeLower();
	strFile.Replace("\\", "/");
	int nPos = strFile.ReverseFind('/');
	if(nPos != -1)
	{
		CString strFileName = strFile;
		CString strDir = strFile.Left(nPos);
		strFileName.Delete(0, nPos+1);
		if(strFileName == "pkgindex.tcl")
		{
			// pkgindex.tcl文件不需要调试加载,使用原始的source命令加载
			// 注意要设置dir变量为pkgindex.tcl的文件路径
			CString strTemp;
			strTemp.Format("uplevel #0 {set dir %s;source_std \"%s\"};", strDir, strFile);
			if(!pTclInterp->RunCommandEx(strTemp, TCL_EVAL_DIRECT))
			{
				return TCL_ERROR;
			}
			return TCL_OK;
		}
	}

	// 判断如果加载过就不用再加载了
	for(int i=0; i<pTclInterp->m_aDebugFile.GetSize(); i++)
	{
		if(strFile == pTclInterp->m_aDebugFile[i].m_strFile)
		{
			return TCL_OK;
		}
	}

	// 多文件调试时候排除解释器lib目录下的文件
	BOOL bMultiDebugExcludeLib = AfxGetApp()->GetProfileInt(REG_TCL_DEBUG_SUBKEY, REG_TCL_MULTIDEBUG_EXCLUDE_LIB, TRUE);
	if(bMultiDebugExcludeLib)
	{
		CString strLibPath = GetInterpRootPath() + "lib/";
		strLibPath.MakeLower();
		strLibPath.Replace("\\", "/");
		if(strFile.Find(strLibPath) == 0)
		{
			// 不进行调试加载,则使用原始的source命令加载
			CString strTemp;
			strTemp.Format("uplevel #0 {source_std \"%s\"};", strFile);
			if(!pTclInterp->RunCommandEx(strTemp, TCL_EVAL_DIRECT))
			{
				return TCL_ERROR;
			}
			return TCL_OK;
		}
	}

	// 确认是否加载从调试文件
	BOOL bMultiDebugQuery = AfxGetApp()->GetProfileInt(REG_TCL_DEBUG_SUBKEY, REG_TCL_MULTIDEBUG_QUERY, FALSE);
	if(bMultiDebugQuery)
	{
		CString strTemp, strTemp1;
		LOADRCSTRING( strTemp1, IDS_MULTIDEBUG_QUERY);
		strTemp.Format(strTemp1, strFile);

		if(pTclInterp->m_bMultiDebugKeepSelect && !pTclInterp->m_bMultiDebugLoadSource)
		{
			// 不进行调试加载,则使用原始的source命令加载
			strTemp.Format("uplevel #0 {source_std \"%s\"};", strFile);
			if(!pTclInterp->RunCommandEx(strTemp, TCL_EVAL_DIRECT))
			{
				return TCL_ERROR;
			}
			return TCL_OK;
		}else
		if(!pTclInterp->m_bMultiDebugKeepSelect)
		{
			TTaskDialogInfo info;
			int nRet;
			LOADRCSTRING( info.strWindowTitle, IDS_MULTIDEBUG_QUERY_TITLE );
			//info.strMainInstruction = "";
			info.strContent = strTemp;
			info.strMainIcon = "Information";
			LOADRCSTRING( info.strVerificationText, IDS_MULTIDEBUG_QUERY_INHERIT );
			info.bCommonButtonYes = TRUE;
			info.bCommonButtonNo = TRUE;
			info.bCommonButtonOk = FALSE;
			info.bCommonButtonCancel = FALSE;
			pTclInterp->pIPlatUI->SendOwmCmd("", OC_TASK_DIALOG, 0, (LPARAM)(&info), &nRet);
			pTclInterp->m_bMultiDebugKeepSelect = info.bIsVerificiationChecked;
			pTclInterp->m_bMultiDebugLoadSource = (nRet != IDNO);
			if(nRet != IDYES)
			{
				// 不进行调试加载,则使用原始的source命令加载
				strTemp.Format("uplevel #0 {source_std \"%s\"};", strFile);
				if(!pTclInterp->RunCommandEx(strTemp, TCL_EVAL_DIRECT))
				{
					return TCL_ERROR;
				}
				return TCL_OK;
			}
		}
	}

	int nFileIndex = pTclInterp->m_aDebugFile.Add(CDebugFile());
	CDebugFile* pDebugFile = &(pTclInterp->m_aDebugFile[nFileIndex]);
	pDebugFile->m_nFileIndex = nFileIndex;
	pDebugFile->m_strFile = strFile;
	if(!pDebugFile->ProcessRunBuffer())
	{
		Tcl_AppendResult(interp, "source error, process debug file ", strFile, "failed", (char *) NULL);
		ConvertResultToUTF(interp);
		return TCL_ERROR;
	}

	IPlatUI* pIPlatUI = pTclInterp->pIPlatUI;
	if(pIPlatUI)
	{
		int nOT = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_OUTPUT_ID, 2);
		CString strTmp;
		strTmp.Format("[DEBUG]processed file %s for debug.\r\n", strFile);
		pIPlatUI->Output(nOT, strTmp);
	}

	if(!pDebugFile->RunScriptBuffer(interp))
	{
		return TCL_ERROR;
	}

	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:获取平台信息
// platform_info application-name
// platform_info copyright
// platform_info homepage
// platform_info download-url
// platform_info startpage
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_PlatformInfo(ClientData clientData, Tcl_Interp* interp, int argc, const char* argv[])
{
	if(argc < 2)
	{
		interp->result = "wrong # args: should be \"platform_info option\"";
		return TCL_ERROR;
	}

	CTclInterp* pTclInterp = (CTclInterp*)(GetTclInterp(interp));
	if(pTclInterp == NULL)
	{
		Tcl_AppendResult(interp, "platform_info error, tclinterp pointer invalid", (char *) NULL);
		return TCL_ERROR;
	}

	IPlatUI* pIPlatUI = pTclInterp->pIPlatUI;
	if(pIPlatUI == NULL)
	{
		Tcl_AppendResult(interp, "platform_info error, get platform interface failed", (char *) NULL);
		return TCL_ERROR;
	}

	CString strOption = ConvertUTFParam(argv[1]);
	CString strValue;
	if(strOption == "application-name")
	{
		strValue = pIPlatUI->GetAppName();
	}else
	if(strOption == "copyright")
	{
		strValue = pIPlatUI->GetPlatCopyRight();
	}else
	if(strOption == "homepage")
	{
		strValue = pIPlatUI->GetPlatHomeURL();
	}else
	if(strOption == "download-url")
	{
		strValue = pIPlatUI->GetPlatDownloadURL();
	}else
	if(strOption == "startpage")
	{
		pIPlatUI->GetVciParameter("", "StartPage", strValue, strValue);
	}else
	{
		Tcl_AppendResult(interp, "option must be application-name, copyright, homepage, download-url or startpage.",  (char *) NULL);
	}	

	Tcl_AppendResult(interp, ConvertStringToUTF(strValue), (char *)NULL);

	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:替换TCL自身的exit命令
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_Exit(ClientData clientData, Tcl_Interp* interp, int argc, const char* argv[])
{
	//VERIFY_TCLCMD_RIGHT(USER_NONE);

	CTclInterp* pTclInterp = (CTclInterp*)(GetTclInterp(interp));
	if(pTclInterp == NULL)
	{
		Tcl_AppendResult(interp, "exit error, tclinterp pointer invalid", (char *) NULL);
		return TCL_ERROR;
	}

	return pTclInterp->Exit();
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:获取最近打开的文件列表
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_RecentFileList(ClientData clientData, Tcl_Interp* interp, int argc, const char* argv[])
{
	for(int i=0; i<10; i++)
	{
		CString strTmp;
		strTmp.Format("File%d", i+1);
		CString strFile = AfxGetApp()->GetProfileString("Recent File List", strTmp, "");
		if(strFile != "")
		{
			Tcl_AppendElement(interp, ConvertStringToUTF(strFile));
		}
	}

	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:获取最近打开的工程列表
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_RecentProjectList(ClientData clientData, Tcl_Interp* interp, int argc, const char* argv[])
{
	for(int i=0; i<10; i++)
	{
		CString strTmp;
		strTmp.Format("PROJECT%d", i+1);
		CString strProject = AfxGetApp()->GetProfileString("RecentProject", strTmp, "");
		if(strProject != "")
		{
			Tcl_AppendElement(interp, ConvertStringToUTF(strProject));
		}
	}

	return TCL_OK;
}

CString GetOutputIdByName(Tcl_Interp *interp, CString strName)
{
	CTclInterp* pTclInterp = GetTclInterp((LPVOID)interp);
	IPlatUI* pIPlatUI = NULL;
	if(pTclInterp)
	{
		pIPlatUI = pTclInterp->pIPlatUI;
	}
	if(pIPlatUI == NULL)
	{
		return "";
	}

	if((strName.Find("<") == 0) && (strName.Right(1) == '>'))
	{
		strName.Delete(0, 1);
		strName = strName.Left(strName.GetLength()-1);
		strName = ConvertUTFParam(strName);
		CString strId = "-1";
		strId = pIPlatUI->OutputGet(-1, strName);
		return strId;
	}
	return "";
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_PutsObjCmd_Ex --
 *
 *	This procedure is invoked to process the "puts" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	Produces output on a channel.
 *
 *----------------------------------------------------------------------
 */
// 对标准puts命令的扩展
int Tcl_PutsObjCmd_Ex(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
    Tcl_Channel chan;			/* The channel to puts on. */
    int i;				/* Counter. */
    int newline;			/* Add a newline at end? */
    char *channelId;			/* Name of channel for puts. */
    int result;				/* Result of puts operation. */
    int mode;				/* Mode in which channel is opened. */
    char *arg;
    int length;

    i = 1;
    newline = 1;
    if ((objc >= 2) && (strcmp(Tcl_GetString(objv[1]), "-nonewline") == 0)) 
	{
		newline = 0;
		i++;
    }
    if ((i < (objc-3)) || (i >= objc)) 
	{
		Tcl_WrongNumArgs(interp, 1, objv, "?-nonewline? ?channelId? string");
		return TCL_ERROR;
    }

    /*
     * The code below provides backwards compatibility with an old
     * form of the command that is no longer recommended or documented.
     */

    if (i == (objc-3)) 
	{
		arg = Tcl_GetStringFromObj(objv[i + 2], &length);
		if (strncmp(arg, "nonewline", (size_t) length) != 0) 
		{
			Tcl_AppendResult(interp, "bad argument \"", arg,
				"\": should be \"nonewline\"", (char *) NULL);
			return TCL_ERROR;
		}
		newline = 0;
    }
    if (i == (objc - 1)) 
	{
		channelId = "stdout";	// 修改,标准的是输出到stdout
								// 此处改为缺省输出到平台某个输出窗口或其他位置
								// 此窗口号可以改
    } else 
	{
		channelId = Tcl_GetString(objv[i]);
		i++;
    }

	CTclInterp* pTclInterp = GetTclInterp((LPVOID)interp);
	IPlatUI* pIPlatUI = NULL;
	if(pTclInterp)
	{
		pIPlatUI = pTclInterp->pIPlatUI;
	}

	CString strChannelId = channelId;
	if(strcmp(channelId, "stdout") == 0)	// 标准输出重定向到平台指定窗口
	{
		// 输出到平台输出窗口
		if(pIPlatUI)
		{
			BOOL bTclDirectOutput = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_OUTPUT, TRUE);
			if(bTclDirectOutput)
			{
				int nOT = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_OUTPUT_ID, 2);	// 信息窗口
				pIPlatUI->OutputSet(nOT, "COLOR=0,0,255");	// 设置输出颜色
				pIPlatUI->Output(nOT, ConvertUTFParam(Tcl_GetString(objv[i])));
				if(newline)	// 对newline参数的处理
				{
					CString strNewLine = "\r\n";
					pIPlatUI->Output(nOT, strNewLine);
				}
			}
		}
		// 输出到TCL标准输出
		BOOL bTclDirectResult = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_RESULT, TRUE);
		if(bTclDirectResult)
		{	// 向结果中添加
			Tcl_AppendResult(interp, Tcl_GetString(objv[i]), (char *)NULL);
			if(newline)	// 对newline参数的处理
			{
				Tcl_AppendResult(interp, "\r\n", (char *)NULL);
			}
			// 判断是否结果信息重置的标志
			if(strcmp(Tcl_GetString(objv[i]), "-reset") == 0)
			{
				Tcl_ResetResult(interp);
			}
		}
		return TCL_OK;
	}else
	if(strcmp(channelId, "stderr") == 0)	// 标准错误重定向到平台指定窗口,并使用特殊颜色显示
	{
		if(pIPlatUI)
		{
			int nOT = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_OUTPUT_ID, 2);	// 信息窗口
			pIPlatUI->OutputFormat(nOT, 2, ConvertUTFParam(Tcl_GetString(objv[i])));
			if(newline)	// 对newline参数的处理
			{
				CString strNewLine = "\r\n";
				pIPlatUI->OutputFormat(nOT, 2, strNewLine);
			}
		}
		return TCL_OK;
	}else
	if(strChannelId.Find("format") == 0)	// 设置窗口格式
	{
		int nOT = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_OUTPUT_ID, 2);	// 信息窗口
		CString strOT = channelId;
		CString strFormat = "";
		if(strOT.Find("format,") == 0)
		{
			strOT.Delete(0, 7);
			if((strOT.Find("<") == 0) && (strOT.Right(1) == '>'))
			{
				nOT = atoi(GetOutputIdByName(interp, strOT));
			}else
			{
				nOT = atoi(strOT);
			}
		}
		strFormat = ConvertUTFParam(Tcl_GetString(objv[i]));

		if(pIPlatUI)
		{
			pIPlatUI->OutputSet(nOT, strFormat);
		}
		return TCL_OK;
	}else
	if(strChannelId.Find("getinfo") == 0)	// 获取窗口信息
	{
		CString strParam = ConvertUTFParam(Tcl_GetString(objv[i]));

		int nOT = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_OUTPUT_ID, 2);	// 信息窗口
		CString strOT = channelId;
		if(strOT.Find("getinfo,") == 0)
		{
			strOT.Delete(0, 8);
			if((strOT == "id") && (strParam.Find("<") == 0) && (strParam.Right(1) == '>'))
			{
				Tcl_AppendResult(interp, GetOutputIdByName(interp, Tcl_GetString(objv[i])), (char *) NULL);
			}else
			if((strOT.Find("<") == 0) && (strOT.Right(1) == '>'))
			{
				nOT = atoi(GetOutputIdByName(interp, strOT));
			}else
			{
				nOT = atoi(strOT);
			}
		}

		if(pIPlatUI)
		{
			CString strInfo = pIPlatUI->OutputGet(nOT, strParam);
			Tcl_AppendResult(interp, strInfo, (char *) NULL);
		}
		return TCL_OK;
	}else
	if(strcmp(channelId, "clear") == 0)	// 清空窗口内容
	{
		int nOT = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_OUTPUT_ID, 2);	// 信息窗口
		CString strOT = ConvertUTFParam(Tcl_GetString(objv[i]));
		if((strOT.Find("<") == 0) && (strOT.Right(1) == '>'))
		{
			nOT = atoi(GetOutputIdByName(interp, strOT));
		}else
		if(strOT.Find("-") == 0)
		{
			strOT.Delete(0, 1);
			nOT = atoi(strOT);			
		}else
		{
			nOT = atoi(strOT);
		}

		if(pIPlatUI)
		{
			pIPlatUI->OutputClear(nOT);
		}
		return TCL_OK;
	}else
	if(strcmp(channelId, "create") == 0)	// 创建窗口
	{
		CString strOT = ConvertUTFParam(Tcl_GetString(objv[i]));
		int nPos = strOT.Find("##");

		if(pIPlatUI)
		{
			int nOT = pIPlatUI->AddOutput(strOT);
			CString strTmp;
			strTmp.Format("%d", nOT);
			Tcl_AppendResult(interp, strTmp, (char *) NULL);
		}
		return TCL_OK;
	}else
	if(strcmp(channelId, "active") == 0)	// 激活窗口
	{
		int nOT = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_OUTPUT_ID, 2);	// 信息窗口
		CString strOT = ConvertUTFParam(Tcl_GetString(objv[i]));
		if((strOT.Find("<") == 0) && (strOT.Right(1) == '>'))
		{
			nOT = atoi(GetOutputIdByName(interp, strOT));
		}else
		if(strOT.Find("-") == 0)
		{
			strOT.Delete(0, 1);
			nOT = atoi(strOT);
		}else
		{
			nOT = atoi(strOT);
		}

		if(pIPlatUI)
		{
			pIPlatUI->ActiveOutput(nOT);
		}
		return TCL_OK;
	}else
	{
		CString strChannel = channelId;
		if(strChannel.Find("-") == 0)
		{
			strChannel.Delete(0, 1);
			int nOT = atoi(strChannel);
			if(pIPlatUI)
			{
				pIPlatUI->Output(nOT, ConvertUTFParam(Tcl_GetString(objv[i])));
				if(newline)	// 对newline参数的处理
				{
					CString strNewLine = "\r\n";
					pIPlatUI->Output(nOT, strNewLine);
				}
			}
			return TCL_OK;
		}
	}

    chan = Tcl_GetChannel(interp, channelId, &mode);
    if (chan == (Tcl_Channel) NULL) 
	{
        return TCL_ERROR;
    }
    if ((mode & TCL_WRITABLE) == 0) 
	{
		Tcl_AppendResult(interp, "channel \"", channelId,
                "\" wasn't opened for writing", (char *) NULL);
        return TCL_ERROR;
    }

    result = Tcl_WriteObj(chan, objv[i]);
    if (result < 0) 
	{
        goto error;
    }
    if (newline != 0) 
	{
        result = Tcl_WriteChars(chan, "\n", 1);
        if (result < 0) 
		{
            goto error;
        }
    }
    return TCL_OK;

error:
    Tcl_AppendResult(interp, "error writing \"", channelId, "\": ",
	    Tcl_PosixError(interp), (char *) NULL);

    return TCL_ERROR;
}

//////////////////////////////////////////////////////////////////////
// 解释器操作
//////////////////////////////////////////////////////////////////////
static CPtrList	g_TclInterpList;	// 解释器对象列表
static int g_TclInterpID = 0x100;	// 解释器ID计数器

// 获取TCL解释器列表指针
CPtrList* GetTclInterpList()
{
	return &g_TclInterpList;
}

// 打开一个解释器
int OpenTclInterp(LPCTSTR lpszInterptName, BOOL bSupportTk)
{
	// 查找是否已经存在
	if(lpszInterptName != NULL && lpszInterptName != "")
	{
		int count = g_TclInterpList.GetCount();
		for(int i = 0; i < count; i ++)
		{
			POSITION pos = g_TclInterpList.FindIndex(i);
			CTclInterp* pTclInterp = (CTclInterp*)g_TclInterpList.GetAt(pos);
			if(lpszInterptName == pTclInterp->GetInterpName()) 
			{
				//pTclInterp->AddRef();	// 增加引用计数
				return pTclInterp->GetInterpID();
			}
		}
	}
	// 不存在则新建
	CTclInterp* pTclInterp = new CTclInterp();
	if(pTclInterp)
	{
		pTclInterp->SetInterpName(lpszInterptName);
		return pTclInterp->GetInterpID();
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
// 根据解释器名获取解释器对象指针
//////////////////////////////////////////////////////////////////////
CTclInterp* GetTclInterp(LPCTSTR lpszInterptName)
{
	int count = g_TclInterpList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_TclInterpList.FindIndex(i);
		CTclInterp* pTclInterp = (CTclInterp*)g_TclInterpList.GetAt(pos);
		if(lpszInterptName == pTclInterp->GetInterpName())
		{
			return pTclInterp;
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// 根据解释器ID获取解释器对象指针
//////////////////////////////////////////////////////////////////////
CTclInterp* GetTclInterp(int nInterpID)
{
	int count = g_TclInterpList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_TclInterpList.FindIndex(i);
		CTclInterp* pTclInterp = (CTclInterp*)g_TclInterpList.GetAt(pos);
		if(nInterpID == pTclInterp->GetInterpID())
		{
			return pTclInterp;
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// 根据解释器指针获取解释器对象指针
//////////////////////////////////////////////////////////////////////
CTclInterp* GetTclInterp(LPVOID lpTclInterp)
{
	int count = g_TclInterpList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_TclInterpList.FindIndex(i);
		CTclInterp* pTclInterp = (CTclInterp*)g_TclInterpList.GetAt(pos);
		if(lpTclInterp == pTclInterp->GetInterp())
		{
			return pTclInterp;
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// 关闭解释器(根据解释器名)
//////////////////////////////////////////////////////////////////////
BOOL CloseTclInterp(LPCTSTR lpszInterptName)
{
	if(NULL == lpszInterptName)
		return FALSE;

	int count = g_TclInterpList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_TclInterpList.FindIndex(i);
		CTclInterp* pTclInterp = (CTclInterp*)g_TclInterpList.GetAt(pos);
		if(lpszInterptName == pTclInterp->GetInterpName())
		{
			//if(pTclInterp->ReleaseRef() == 0)	// 引用计数＝0才删除对象
			//	delete pTclInterp;
			return TRUE;
		}
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// 关闭解释器(根据解释器ID)
//////////////////////////////////////////////////////////////////////
BOOL CloseTclInterp(int nInterpID)
{
	int count = g_TclInterpList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_TclInterpList.FindIndex(i);
		CTclInterp* pTclInterp = (CTclInterp*)g_TclInterpList.GetAt(pos);
		if(nInterpID == pTclInterp->GetInterpID())
		{
			//if(pTclInterp->ReleaseRef() == 0)	// 引用计数＝0才删除对象
			//	delete pTclInterp;
			return TRUE;
		}
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// 删除所有未删除的解释器(平台关闭时调用)
//////////////////////////////////////////////////////////////////////
void CloseAllTclInterp()
{
	// 删除解释器对象列表中的项
	int count = g_TclInterpList.GetCount();
	for(int i = count-1; i >= 0; i --)
	{
		POSITION pos = g_TclInterpList.FindIndex(i);
		CTclInterp* pTclInterp = (CTclInterp*)g_TclInterpList.GetAt(pos);
		delete pTclInterp;
	}
}


//////////////////////////////////////////////////////////////////////
// 多文件调试的文件类
//////////////////////////////////////////////////////////////////////
CDebugFile::CDebugFile()
{
	m_nFileIndex	= 0;
	m_strFile		= "";
	m_pRunBuffer	= NULL;
	m_nTDLineCount	= 0;
	m_ProfileMap.InitHashTable(128);	// 初始化覆盖率分析哈希表
}

CDebugFile::~CDebugFile()
{
	if(m_pRunBuffer != NULL)
		delete m_pRunBuffer;
	m_pRunBuffer = NULL;

	// 清空覆盖率分析哈希表
	m_ProfileMap.RemoveAll();
}

CDebugFile::CDebugFile(const CDebugFile& other)
{
	m_nFileIndex	= other.m_nFileIndex;
	m_strFile		= other.m_strFile;
	m_pRunBuffer	= other.m_pRunBuffer;
	m_nTDLineCount	= other.m_nTDLineCount;
}

//////////////////////////////////////////////////////////////////////
// 重载赋值操作符
//////////////////////////////////////////////////////////////////////
CDebugFile& CDebugFile::operator = (const CDebugFile& other)
{
	m_nFileIndex	= other.m_nFileIndex;
	m_strFile		= other.m_strFile;
	m_pRunBuffer	= other.m_pRunBuffer;
	m_nTDLineCount	= other.m_nTDLineCount;

	return *this;
};

/////////////////////////////////////////////////////////////////////////////
// 根据运行模式,进行缓冲区翻译
// 注:此处目前全部改为UNIX风格进行换行符翻译,因为TCL缺省的是这种风格
// 如果改为Windows风格,则BufSize等地方需要注意
/////////////////////////////////////////////////////////////////////////////
int CDebugFile::ProcessRunBuffer()
{
	if(m_pRunBuffer != NULL)
		delete m_pRunBuffer;
	m_pRunBuffer = NULL;

	// 清空覆盖率分析变量
	m_nTDLineCount = 0;
	m_ProfileMap.RemoveAll();

	try
	{
		CStdioFile fi(m_strFile, CFile::modeRead);
		int nLineNo = 0;		// 行号
		int nBufSize = 0;		// 缓冲区大小
		BOOL bEndLine1 = TRUE;	// 是否续行'\'
		BOOL bEndLine2 = TRUE;	// 是否续行'"'
		BOOL bComment = FALSE;	// 注释
		BOOL bVarMultiLine = FALSE;	// 多行的变量(主要指用{续行的变量)
		int  nVarLevel = 0;		// 多行变量中的深度
		BOOL bCmdSwitch = FALSE;// 处于switch命令中
		int  nSwitchLevel = 0;	// switch命令中的深度
		CString strLine;
		CStringArray asScript;	// 行数组

		while(fi.ReadString(strLine))
		{
			nLineNo++;
			CString strLineSrc = strLine;
			strLine.TrimLeft("\t ");
			strLine.TrimRight("\t ");

			if(strLine.GetLength() == 0)	// 空行
			{
				bEndLine1 = TRUE;
				asScript.Add(strLineSrc);
				nBufSize += strLineSrc.GetLength();
				nBufSize += 1;	// "\n"
				continue;
			}else
			if((strLine[0] == '#') && bEndLine2)	// 注释行(需要排除用"续行的情况)
			{
				bEndLine1 = TRUE;
				bEndLine2 = TRUE;
				bComment = TRUE;
				int nLen = strLine.GetLength();
				if( (strLine[nLen-1]=='\\') && ((nLen>1) && (strLine[nLen-2]!='\\')) )
				{	// 续行
					bEndLine1 = FALSE;
				}
				asScript.Add(strLineSrc);
				nBufSize += strLineSrc.GetLength();
				nBufSize += 1;	// "\n"
				continue;
			}else
			if(bComment && !bEndLine1)
			{
				asScript.Add(strLineSrc);
				nBufSize += strLineSrc.GetLength();
				nBufSize += 1;	// "\n"
				int nLen = strLine.GetLength();
				if( (strLine[nLen-1]=='\\') && ((nLen>1) && (strLine[nLen-2]!='\\')) )
				{
					continue;
				}
				bEndLine1 = TRUE;	// 不用续行
				continue;
			}

			bComment = FALSE;

			if(!bVarMultiLine)	// 判断是否多行变量的起始行
			{
				int nLen = strLine.GetLength();
				if(strLine[nLen-1] == '{')
				{
					CString strLineTmp = strLine;
					int nPos = strLineTmp.ReverseFind(';');
					if(nPos != -1)
					{
						strLineTmp.Delete(0, nPos+1);
					}
					strLineTmp.TrimLeft("\t ");
					//if(((strLineTmp.Find("set") == 0) || (strLineTmp.Find("append") == 0) || (strLineTmp.Find("lappend") == 0)) &&
					//	(strLineTmp.GetLength() > 4) &&
					//	((strLineTmp[3] == ' ') || (strLineTmp[3] == '\t')) )
					if(((strLineTmp.Find("set") == 0) &&
						(strLineTmp.GetLength() > 4) &&
						((strLineTmp[3] == ' ') || (strLineTmp[3] == '\t')) ) ||
					   ((strLineTmp.Find("lappend") == 0) &&
						(strLineTmp.GetLength() > 8) &&
						((strLineTmp[7] == ' ') || (strLineTmp[7] == '\t')) ) ||
					   ((strLineTmp.Find("append") == 0) &&
						(strLineTmp.GetLength() > 7) &&
						((strLineTmp[6] == ' ') || (strLineTmp[6] == '\t')) ) ||
					   ((strLineTmp.Find("array") == 0) &&
						(strLineTmp.GetLength() > 12) &&
						((strLineTmp[5] == ' ') || (strLineTmp[5] == '\t')) &&
						(strLineTmp.Find(" set ") > 4) ) )
					{
						bVarMultiLine = TRUE;
						nVarLevel = 1;

						CString strBreak;
						strBreak.Format("td *%d %d;", nLineNo, m_nFileIndex);
						strLineSrc = strBreak + strLineSrc;
						asScript.Add(strLineSrc);
						nBufSize += strLineSrc.GetLength();
						nBufSize += 1;	// "\n"
						m_nTDLineCount ++;
						continue;
					}
				}
			}			

			if(bEndLine1 && bEndLine2 && !bCmdSwitch && !bVarMultiLine)	// 如果不用续行(不处于switch命令中,且不处于多行变量续行状态),则加单步标志
			{
				CString strBreak;
				strBreak.Format("td *%d %d;", nLineNo, m_nFileIndex);
				strLineSrc = strBreak + strLineSrc;
				asScript.Add(strLineSrc);
				nBufSize += strLineSrc.GetLength();
				nBufSize += 1;	// "\n"
				m_nTDLineCount ++;

				int nLen = strLine.GetLength();
				if( (strLine[nLen-1]=='\\') && ((nLen>1) && (strLine[nLen-2]!='\\')) )
				{	// 续行'\'
					bEndLine1 = FALSE;
					continue;
				}
				/*if((strLine[nLen-1]=='"') && (strLine.Find('"')==(nLen-1)))
				{	// 续行'"'
					bEndLine2 = FALSE;
					continue;
				}*/
				// 对于"续行的情况,需要根据"的个数决定最后的状态
				int nPos = -1;
				while((nPos = strLine.Find('"')) != -1)
				{
					if((nPos == 0) || (strLine[nPos-1] != '\\'))
					{
						bEndLine2 = !bEndLine2;
					}
					strLine.Delete(0, nPos+1);
				}
				continue;
			}

			asScript.Add(strLineSrc);
			nBufSize += strLineSrc.GetLength();
			nBufSize += 1;	// "\n"

			if(!bEndLine1)
			{
				int nLen = strLine.GetLength();
				if( (strLine[nLen-1]=='\\') && ((nLen>1) && (strLine[nLen-2]!='\\')) )
				{
					continue;
				}
				bEndLine1 = TRUE;	// 不用续行
			}

			if(!bEndLine2)
			{
				if(strLine.Find('"') == -1)
				{
					continue;
				}
				//bEndLine2 = TRUE;	// 不用续行
				int nPos = -1;
				while((nPos = strLine.Find('"')) != -1)
				{
					if((nPos == 0) || (strLine[nPos-1] != '\\'))
					{
						bEndLine2 = !bEndLine2;
					}
					strLine.Delete(0, nPos+1);
				}
			}

			if( (strLine.Find("switch") == 0) && (strLine[strLine.GetLength()-1] == '{') &&
				((strLine[6] == ' ') || (strLine[6] == '\t')) )	// 进入switch命令状态
			{
				bCmdSwitch = TRUE;
				nSwitchLevel = 0;
			}else
			if(bCmdSwitch)	// 处于switch命令状态中
			{
				if((nSwitchLevel <= 0) && (strLine[0] == '}'))	// 退出switch命令状态
				{
					bCmdSwitch = FALSE;
				}else
				{
					if(strLine.Find('{'))
					{
						nSwitchLevel++;
					}
					if(strLine.Find('}'))
					{
						nSwitchLevel--;
					}
				}
			}

			if(bVarMultiLine)
			{
				// 当前处于多行变量的续行状态,判断什么时候结束这种状态
				int nPosLeft = -1;
				int nPosRight = -1;
				while( ((nPosRight = strLine.Find('}')) != -1) || ((nPosLeft = strLine.Find('{')) != -1) )
				{
					if((nPosLeft != -1) && ((nPosLeft == 0) || (strLine[nPosLeft-1] != '\\')))
					{
						nVarLevel++;
					}
					if((nPosRight != -1) && ((nPosRight == 0) || (strLine[nPosRight-1] != '\\')))
					{
						nVarLevel--;
					}

					if(nVarLevel <= 0)
					{
						bVarMultiLine = FALSE;
						break;
					}

					//if((nPos == 0) || (strLine[nPos-1] != '\\'))
					//{
					//	bVarMultiLine = FALSE;
					//	break;
					//}
					if(nPosLeft != -1)
					{
						strLine.Delete(0, nPosLeft+1);
					}else
					if(nPosRight != -1)
					{
						strLine.Delete(0, nPosRight+1);
					}
				}
			}

		}

		// 创建缓冲区
		m_pRunBuffer = new char[nBufSize+1];
		m_pRunBuffer[nBufSize] = 0;
		if(nBufSize > 0)
		{	// 保证缓冲区的所有字符不是随机的
			m_pRunBuffer[nBufSize-2] = 0;
			m_pRunBuffer[nBufSize-1] = 0;
		}
		int nPos = 0;
		for(int i=0; i<asScript.GetSize(); i++)
		{
			CString strLine = asScript[i];
			//TRACE("%s\n", strLine);
			//pIPlatUI->Output(2, strLine);
			//pIPlatUI->Output(2, "\r\n");
			memcpy(m_pRunBuffer+nPos, strLine.GetBuffer(strLine.GetLength()), strLine.GetLength());
			strLine.ReleaseBuffer();
			nPos += strLine.GetLength();
			if(i < (asScript.GetSize()-1))
			{				
				memcpy(m_pRunBuffer+nPos, "\n", 1);
				nPos +=1;
			}
		}

	}catch(...)
	{
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 执行缓冲区中的TCL命令
/////////////////////////////////////////////////////////////////////////////
int CDebugFile::RunScriptBuffer(Tcl_Interp*	pInterp)
{
	if(NULL == m_pRunBuffer)
		return FALSE;

	// 修改普通字符串为UTF-8编码字符串
	Tcl_DString dst;
	char* script = Tcl_ExternalToUtfDString(NULL, m_pRunBuffer, -1, &dst);
	// 执行脚本命令
	if(TCL_OK != Tcl_Eval(pInterp, script))
	{
		Tcl_DStringFree(&dst);
		//ConvertResultToUTF(pInterp);
		return FALSE;
	}

	Tcl_DStringFree(&dst);

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// 功能实现
//////////////////////////////////////////////////////////////////////////
CTclInterp::CTclInterp()
{
	m_nInterpID		= g_TclInterpID++;
	if(g_TclInterpID > 0x01000000)
		g_TclInterpID = 0x100;
	pIPlatUI		= NULL;
	m_strAppProcess	= _T("");
	m_dwRunThreadId	= NULL;
	#ifdef _USE_TCL84
	Tcl_FindExecutable(Tcl_GetNameOfExecutable());
	#endif
	#ifdef _USE_TCL85
	Tcl_FindExecutable(_T(""));	// Tcl8.5及以上版本不能使用Tcl_GetNameOfExecutable，否则会异常
	#endif
	#ifdef _USE_TCL86
	Tcl_FindExecutable(_T(""));	// Tcl8.5及以上版本不能使用Tcl_GetNameOfExecutable，否则会异常
	#endif

	m_strInterpName	= _T("");
	pInterp			= Tcl_CreateInterp();
	Tcl_SetServiceMode(TCL_SERVICE_NONE);
/*
	// ------------
	m_TclNotifier = Tcl_InitNotifier();
	Tcl_AlertNotifier(m_TclNotifier);
	Tcl_SetServiceMode(TCL_SERVICE_ALL);

	Tcl_Time timeout;
	timeout.sec = 0;
	timeout.usec = 25000;
	Tcl_SetTimer(&timeout);
*/
	m_strTclLibPath	= GetInterpRootPath();
	m_strTclLibPath	+= "Lib\\";
	m_strScriptFile	= _T("");
	m_pRunBuffer	= NULL;
	m_nRunMode		= SCRIPT_RUN_NORMAL;
	m_nRunState		= SCRIPT_STATE_IDLE;
	m_nRunLevel		= SCRIPT_LEVEL_NONE;
	m_nEvalResult	= TCL_OK;
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
	m_nSupportTk	= 1;//2;//nSupportTk;
	m_bIsTkFile		= FALSE;
	m_bHide			= FALSE;
	m_lpfnExit		= NULL;
	m_hPipeProcess	= NULL;
	m_bPipeProcessTerminated = FALSE;
	// 初始化基础Tcl解释器
	InitTclBaseInterp();
	g_TclInterpList.AddTail(this);

	m_pILicense = NULL;
}

CTclInterp::~CTclInterp()
{
	if(m_pRunBuffer != NULL)
		delete m_pRunBuffer;
	m_pRunBuffer = NULL;

	// 清空覆盖率分析哈希表
	m_ProfileMap.RemoveAll();

	try {
	Tcl_DeleteInterp(pInterp);
	}catch(...)
	{
	}

	// 删除解释器对象列表中的项
	int count = g_TclInterpList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_TclInterpList.FindIndex(i);
		CTclInterp* pTclInterp = (CTclInterp*)g_TclInterpList.GetAt(pos);
		if(pTclInterp == this) 
		{
			g_TclInterpList.RemoveAt(pos);
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 初始化组件
/////////////////////////////////////////////////////////////////////////////
int CTclInterp::Init(LPCSTR lpcsParams)
{
	// 执行预处理脚本
	PreScript();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 组件结束
/////////////////////////////////////////////////////////////////////////////
int CTclInterp::Done(LPCSTR lpcsParams)
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 安装回调函数
/////////////////////////////////////////////////////////////////////////////
int CTclInterp::InstallHandler(DWORD nEventID, LPVOID lpHandler, DWORD dwRefData)
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
int CTclInterp::UninstallHandler(DWORD nEventID, LPVOID lpHandler)
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
// Exit虚函数,由脚本命令exit触发
// 之所以重载Tcl缺省的exit命令是因为缺省exit会销毁主程序造成异常
/////////////////////////////////////////////////////////////////////////////
int CTclInterp::Exit()
{
	//Tcl_Exit(0);
	// 执行Tk窗口的销毁操作,使Tk脚本可以正常退出
	TkDestroyWindow();
	TRACE("CTclInterp::Exit\n");

	if(m_lpfnExit)
	{
		m_lpfnExit((void*)(&m_xTclInterp));
	}

	return TCL_OK;
}

//{{VCI_IMPLEMENT_BEGIN
int __stdcall CTclInterp::SetIPlatUI(LPVOID lpIPlatUI)
{
	pIPlatUI = (IPlatUI*)lpIPlatUI;

	// 设置语言变量
	CString strCurrentLanguage = _T("english");
	g_nCurrentLanguage = VCI_LANGUAGE_ENGLISH;
	if(VCI_LANGUAGE_CHINESE == GetLanguage())
	{
		strCurrentLanguage = _T("chinese");
		g_nCurrentLanguage = VCI_LANGUAGE_CHINESE;
	}

	// 设置平台版本变量
	CString strPlatformVersion = "";
	if(pIPlatUI)
	{
		strPlatformVersion = pIPlatUI->GetPlatVersion();
	}else
	{
		strPlatformVersion = AfxGetApp()->GetProfileString(REG_VERSION_SUBKEY, REG_VER_PLATVERSION, VAR_PLATFORM_VERSION);
	}

	CString strCmd;
	strCmd.Format("set platform_language %s;set platform_version %s;set platform_interface %d;",
				strCurrentLanguage, strPlatformVersion, pIPlatUI);

	Tcl_DString dst;
	strCmd = Tcl_ExternalToUtfDString(NULL, strCmd, -1, &dst);
	Tcl_DStringFree(&dst);

	if(TCL_OK != Tcl_Eval(pInterp, strCmd.GetBuffer(strCmd.GetLength())))
	{
		strCmd.ReleaseBuffer();
		return FALSE;
	}
	strCmd.ReleaseBuffer();

	// 获取License接口指针
	if(getIPlatUI())
	{
		m_pILicense = (ILicense*)(((IPlatUI*)getIPlatUI())->GetObjectByInstanceName("###license"));
	}

	return 0;
}

// 设置库路径
int __stdcall CTclInterp::SetLibPath(LPCTSTR lpszPath)
{
	m_strTclLibPath = lpszPath;
	return 0;
}

// 解释器是否在运行
int __stdcall CTclInterp::IsInterpRunning()
{
	// TODO: Add your code here
	// 判断所有工位是否都已经终止
	int count = g_TclInterpList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_TclInterpList.FindIndex(i);
		CTclInterp* pTclInterp = (CTclInterp*)g_TclInterpList.GetAt(pos);
		int nRunState = pTclInterp->GetRunState();
		if( (SCRIPT_STATE_RUN == nRunState) ||
			(SCRIPT_STATE_BREAK == nRunState) ||
			(SCRIPT_STATE_STEP == nRunState) )
			return TRUE;
	}
	return FALSE;
}

// 设置脚本文件名
int __stdcall CTclInterp::SetScriptFile(LPCTSTR lpszPathName)
{
	// TODO: Add your code here
	CString strPathName = lpszPathName;
	m_strScriptFile = "";
	if(lpszPathName != NULL)
	{
		m_strScriptFile = lpszPathName;
		if(m_strScriptFile.GetLength() == 0)
		{
			return FALSE;
		}

		if(m_strScriptFile[0] == '"')
		{	// 参数0有可能用"包围，必须去掉才能解析出真正的脚本文件名
			m_strScriptFile.Delete(0, 1);
			int nPos = m_strScriptFile.Find("\"");
			if(nPos >= 0)
			{
				m_strScriptFile = m_strScriptFile.Left(nPos);
				strPathName.Delete(0, nPos+2);
			}
		}else
		{	// 第一个?之前的是脚本文件名
			int nPos = m_strScriptFile.Find("?");
			if(nPos > 0)
			{
				m_strScriptFile = m_strScriptFile.Left(nPos);
				strPathName.Delete(0, nPos+1);
			}else
			{
				strPathName = "";
			}
		}
		strPathName.TrimLeft();
	}

	// 下面设置的argv中如果存在空格可能造成错误,因此定义了一个特殊变量,仅对Script.NET有效
	// 可以代替上面的argv,但整个是一个变量
	CString strArgvTemp = strPathName;
	if(strArgvTemp.Find('"') == 0)
	{
		strArgvTemp.Delete(0, 1);
	}
	if(strArgvTemp.ReverseFind('"') == (strArgvTemp.GetLength()-1))
	{
		strArgvTemp = strArgvTemp.Left(strArgvTemp.GetLength()-1);
	}
	SetVar("platform_argv", ConvertStringToUTF(strArgvTemp));

	// 文件名需要使用TCL承认的/方式
//	CString strPathName = lpszPathName;
//	strPathName.Replace("\\", "/");

	// 初始化argc和argv
	// 注:某些用\必须用\\\来替代,行尾必须加\n
	//strPathName.Replace("\\", "\\\\");
	strPathName.Replace("\\\"", "\\\\\"");// 将最后的一个\"替换为\\"
	// 注:此处的argv已经符合TCL标准,将被执行文件名从argv中去掉,并赋给变量argv0
	CString strFileName = m_strScriptFile;
	strFileName.Replace("\\", "/");
	CString strPreScript;
	strPreScript.Format("set argv {%s};\n\
						set argc [llength $argv];\n\
						set argv0 \"%s\";",
						strPathName, strFileName);

	Tcl_DString dst;
	strPreScript = Tcl_ExternalToUtfDString(NULL, strPreScript, -1, &dst);
	Tcl_DStringFree(&dst);

	try {
	////////////////////////////////////////////

	if(TCL_OK != Tcl_Eval(pInterp, strPreScript.GetBuffer(strPreScript.GetLength())))
	{
		strPreScript.ReleaseBuffer();
		//PF_TRACE_FMT(2, "TCL script argc&argv error:%s\r\n", Tcl_GetStringResult(pInterp));
		return FALSE;
	}
	strPreScript.ReleaseBuffer();

	////////////////////////////////////////////
	}catch(...)
	{
		TRACE("SetScriptFile Eval Tcl command '%s' exception!\n", strPreScript);
		return FALSE;
	}

	Tcl_ResetResult(pInterp);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 初始化基础Tcl解释器
/////////////////////////////////////////////////////////////////////////////
int CTclInterp::InitTclBaseInterp()
{
	// 初始化脚本(几个标准命令的重命名)
	// 注：某些用\必须用\\\来替代,行尾必须加\r\n
	// tcl_library
	CString strTclLibrary = m_strTclLibPath;
	#ifdef _USE_TCL84
	strTclLibrary += "tcl8.4";
	#endif
	#ifdef _USE_TCL85
	strTclLibrary += "tcl8.5";
	#endif
	#ifdef _USE_TCL86
	strTclLibrary += "tcl8.6";
	#endif
	strTclLibrary.Replace('\\', '/');
	// tk_library
	CString strTkLibrary = m_strTclLibPath;
	#ifdef _USE_TCL84
	strTkLibrary += "tk8.4";
	#endif
	#ifdef _USE_TCL85
	strTkLibrary += "tk8.5";
	#endif
	#ifdef _USE_TCL86
	strTkLibrary += "tk8.6";
	#endif
	strTkLibrary.Replace('\\', '/');

	CString strPlatPath = GetPlatRootPath();
	if(strPlatPath[strPlatPath.GetLength()-1] == '\\')
		strPlatPath.Delete(strPlatPath.GetLength()-1, 1);
	strPlatPath.Replace('\\', '/');

	CString strInterpPath = GetInterpRootPath();
	if(strInterpPath[strInterpPath.GetLength()-1] == '\\')
		strInterpPath.Delete(strInterpPath.GetLength()-1, 1);
	strInterpPath.Replace('\\', '/');

#ifdef _DEBUG
	CString strPlatType = "debug";
#else
	CString strPlatType = "release";
#endif

	CString strPreScript;
	strPreScript.Format("rename puts puts_std;\n\
						rename exit exit_std;\n\
						set platform_path \"%s\";\n\
						set interp_path \"%s\";\n\
						set platform_type %s;\n\
						set tcl_library \"%s\";\n\
						set tk_library \"%s\";\n\
						set thisinterp %d;",
						strPlatPath, strInterpPath, strPlatType,
						strTclLibrary, strTkLibrary, pInterp);

	Tcl_DString dst;
	strPreScript = Tcl_ExternalToUtfDString(NULL, strPreScript, -1, &dst);
	Tcl_DStringFree(&dst);

	if(TCL_OK != Tcl_Eval(pInterp, strPreScript.GetBuffer(strPreScript.GetLength())))
	{
		strPreScript.ReleaseBuffer();
		//PF_TRACE_FMT(2, "TCL script preprocess error:%s\r\n", Tcl_GetStringResult(pInterp));
		return FALSE;
	}
	strPreScript.ReleaseBuffer();

	// 初始化Tcl库
	Tcl_Init(pInterp);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 预处理脚本
/////////////////////////////////////////////////////////////////////////////
int CTclInterp::PreScript()
{
	// 获取平台的Process设置
	if(pIPlatUI)
	{
		CString strDefaultValue;
		pIPlatUI->GetVciParameter("", "Process", m_strAppProcess, strDefaultValue);
	}

	// 获取加载库信息
	BOOL bTclLibTk = FALSE;
	BOOL bTclLibiTcl = TRUE;
	BOOL bTclLibiTk = FALSE;
	if(m_strAppProcess != "Service")
	{
		bTclLibTk = AfxGetApp()->GetProfileInt(REG_TCL_LIB_SUBKEY, REG_TCL_LIB_TK, FALSE);
		bTclLibiTcl = AfxGetApp()->GetProfileInt(REG_TCL_LIB_SUBKEY, REG_TCL_LIB_ITCL, TRUE);
		bTclLibiTk = AfxGetApp()->GetProfileInt(REG_TCL_LIB_SUBKEY, REG_TCL_LIB_ITK, FALSE);
	}

	// 初始化脚本(几个标准命令的重命名)
	// 注：某些用\必须用\\\来替代,行尾必须加\r\n
	// tcl_library
	CString strTclLibrary = m_strTclLibPath;
	#ifdef _USE_TCL84
	strTclLibrary += "tcl8.4";
	#endif
	#ifdef _USE_TCL85
	strTclLibrary += "tcl8.5";
	#endif
	#ifdef _USE_TCL86
	strTclLibrary += "tcl8.6";
	#endif
	strTclLibrary.Replace('\\', '/');
	// tk_library
	CString strTkLibrary = m_strTclLibPath;
	#ifdef _USE_TCL84
	strTkLibrary += "tk8.4";
	#endif
	#ifdef _USE_TCL85
	strTkLibrary += "tk8.5";
	#endif
	#ifdef _USE_TCL86
	strTkLibrary += "tk8.6";
	#endif
	strTkLibrary.Replace('\\', '/');

	CString strPlatPath = GetPlatRootPath();
	if(strPlatPath[strPlatPath.GetLength()-1] == '\\')
		strPlatPath.Delete(strPlatPath.GetLength()-1, 1);
	strPlatPath.Replace('\\', '/');

	CString strInterpPath = GetInterpRootPath();
	if(strInterpPath[strInterpPath.GetLength()-1] == '\\')
		strInterpPath.Delete(strInterpPath.GetLength()-1, 1);
	strInterpPath.Replace('\\', '/');

#ifdef _DEBUG
	CString strPlatType = "debug";
#else
	CString strPlatType = "release";
#endif

	// 注:此处设置的语言变量是无效的,因为这个函数在解释器创建后会立刻调用,
	// 此时平台还没有对解释器设置语言变量,在SetIPlatUI函数中的设置才是真作证有效的,
	// 此处只是设置一个初始值,应该都是english
	CString strCurrentLanguage = _T("english");
	g_nCurrentLanguage = VCI_LANGUAGE_ENGLISH;
	if(VCI_LANGUAGE_CHINESE == GetLanguage())
	{
		strCurrentLanguage = _T("chinese");
		g_nCurrentLanguage = VCI_LANGUAGE_CHINESE;
	}

	// 平台版本
	// modify at 2007-07-12,原来的获取平台版本方法有问题,修改为通过平台提供的接口来获取
	// 注:此处pIPlatUI都为空,因为这个指针还没有初始化,因此要在SetIPlatUI函数中更新相关变量
	CString strPlatformVersion = "";
	if(pIPlatUI)
	{
		strPlatformVersion = pIPlatUI->GetPlatVersion();
	}else
	{
		strPlatformVersion = AfxGetApp()->GetProfileString(REG_VERSION_SUBKEY, REG_VER_PLATVERSION, VAR_PLATFORM_VERSION);
	}

	// 界面风格
	CString strUIStyle = "default";
	if(m_strAppProcess != "Service")
	{
		strUIStyle = AfxGetApp()->GetProfileString(REG_UISTYLE_SUBKEY, REG_UISTYLE_HTML, "default");
	}

	CString strPreScript;
	strPreScript.Format("set platform_version %s;\n\
						set platform_interface %d;\n\
						set interp_path \"%s\";\n\
						set platform_language %s;\n\
						set platform_ui_style %s;\n\
						set interpName \"%s\";\n\
						set thisinterp %d;",
						strPlatformVersion, pIPlatUI, strInterpPath,
						strCurrentLanguage, strUIStyle,
						m_strInterpName, pInterp);

	Tcl_DString dst;
	strPreScript = Tcl_ExternalToUtfDString(NULL, strPreScript, -1, &dst);
	Tcl_DStringFree(&dst);

	if(TCL_OK != Tcl_Eval(pInterp, strPreScript.GetBuffer(strPreScript.GetLength())))
	{
		strPreScript.ReleaseBuffer();
		//PF_TRACE_FMT(2, "TCL script preprocess error:%s\r\n", Tcl_GetStringResult(pInterp));
		return FALSE;
	}
	strPreScript.ReleaseBuffer();

	// 初始化Tcl库
	//Tcl_Init(pInterp);

//	ClientData TclNotifier = Tcl_InitNotifier();
//	Tcl_AlertNotifier(TclNotifier);
//	Tcl_SetServiceMode(TCL_SERVICE_ALL);

	// 初始化Tk库(如果m_nSupportTk == 2,表示强制支持TK,不需要系统配置)
	if((m_nSupportTk == 2) || ((m_nSupportTk == 1) && bTclLibTk))
	{
		m_bIsTkFile = TRUE;
/*
		CWnd* pMainFrame = AfxGetMainWnd();
		char buffer[500];
		sprintf(buffer, "-use 0x%8.8x", pMainFrame->m_hWnd);
		Tcl_SetVar(pInterp, "argv", buffer, TCL_GLOBAL_ONLY);
*/
		#ifdef _SUPPORT_TK
		Tk_Init(pInterp);
		Tcl_StaticPackage(pInterp, "Tk", Tk_Init, Tk_SafeInit);
		#endif
	}

	// 初始化iTcl库
	if(bTclLibiTcl)
	{
		//Itcl_Init(pInterp);
		//Tcl_Eval(pInterp, "package require Itcl");
	}

	// 初始化iTk库
	if(bTclLibiTk)
	{
		//Itk_Init(pInterp);
		//Tcl_Eval(pInterp, "package require Itk");
	}

    /*
     * Specify a user-specific startup file to invoke if the application
     * is run interactively.  Typically the startup file is "~/.apprc"
     * where "app" is the name of the application.  If this line is deleted
     * then no user-specific startup file will be run under any conditions.
     */

    Tcl_SetVar(pInterp, "tcl_rcFileName", "~/.wishrc", TCL_GLOBAL_ONLY);
	Tcl_SetVar(pInterp, "tcl_interactive", "1", TCL_GLOBAL_ONLY);
	Tcl_SourceRCFile(pInterp);

	// 初始化i18n支持脚本
	strPreScript = "source $platform_path/lib/plat/i18n.tcl";
	strPreScript = Tcl_ExternalToUtfDString(NULL, strPreScript, -1, &dst);
	Tcl_DStringFree(&dst);
	Tcl_Eval(pInterp, strPreScript.GetBuffer(strPreScript.GetLength()));
	strPreScript.ReleaseBuffer();

	// 注册平台扩展命令
	Tcl_RegisterCmd(pInterp);

	// 注册扩展命令
	TclExCmd_Init(pInterp);

	// 注册TclWorkTree命令
	TclWorkTree_Init(pInterp);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 自动检查脚本是否是一个Tk脚本,方法是查找是否有package require Tk的行
/////////////////////////////////////////////////////////////////////////////
BOOL CTclInterp::CheckIsTkFile()
{
	BOOL bIsTkFile = FALSE;

	try
	{
		CStdioFile fi(m_strScriptFile, CFile::modeRead);		
		CString strLine;

		// 只检查前100行是否有
		int nCount = 0;
		while(fi.ReadString(strLine) && nCount < 100)
		{
			strLine.TrimLeft("\t ");
			strLine.TrimRight("\t ");

			if(strLine.Find("package require Tk") == 0)	// Tk脚本
			{
				bIsTkFile = TRUE;
				break;
			}

			nCount++;
		}

	}catch(...)
	{
		return FALSE;
	}

	return bIsTkFile;
}

/////////////////////////////////////////////////////////////////////////////
// 自动检查脚本是否是一个TkView脚本,方法是查找是否有package require TkView的行
/////////////////////////////////////////////////////////////////////////////
BOOL CTclInterp::CheckIsTkvFile()
{
	BOOL bIsTkvFile = FALSE;

	try
	{
		CStdioFile fi(m_strScriptFile, CFile::modeRead);		
		CString strLine;

		// 只检查前50行是否有
		int nCount = 0;
		while(fi.ReadString(strLine) && (nCount < 50))
		{
			strLine.TrimLeft("\t ");
			strLine.TrimRight("\t ");

			if(strLine.Find("package require TkView") == 0)	// Tkv脚本
			{
				bIsTkvFile = TRUE;
				break;
			}

			nCount++;
		}

	}catch(...)
	{
		return FALSE;
	}

	return bIsTkvFile;
}

/////////////////////////////////////////////////////////////////////////////
// 根据运行模式,进行缓冲区翻译
// 注:此处目前全部改为UNIX风格进行换行符翻译,因为TCL缺省的是这种风格
// 如果改为Windows风格,则BufSize等地方需要注意
/////////////////////////////////////////////////////////////////////////////
int CTclInterp::ProcessRunBuffer()
{
	if(SCRIPT_RUN_NORMAL == m_nRunMode)
		return TRUE;

	if(m_pRunBuffer != NULL)
		delete m_pRunBuffer;
	m_pRunBuffer = NULL;

	// 清空覆盖率分析变量
	m_nTDLineCount = 0;
	m_ProfileMap.RemoveAll();

	try
	{
		CStdioFile fi(m_strScriptFile, CFile::modeRead);
		int nLineNo = 0;		// 行号
		int nBufSize = 0;		// 缓冲区大小
		BOOL bEndLine1 = TRUE;	// 是否续行'\'
		BOOL bEndLine2 = TRUE;	// 是否续行'"'
		BOOL bComment = FALSE;	// 注释
		BOOL bVarMultiLine = FALSE;	// 多行的变量(主要指用{续行的变量)
		int  nVarLevel = 0;		// 多行变量中的深度
		BOOL bCmdSwitch = FALSE;// 处于switch命令中
		int  nSwitchLevel = 0;	// switch命令中的深度
		CString strLine;
		CStringArray asScript;	// 行数组

		while(fi.ReadString(strLine))
		{
			nLineNo++;
			CString strLineSrc = strLine;
			strLine.TrimLeft("\t ");
			strLine.TrimRight("\t ");

			if(strLine.GetLength() == 0)	// 空行
			{
				bEndLine1 = TRUE;
				asScript.Add(strLineSrc);
				nBufSize += strLineSrc.GetLength();
				nBufSize += 1;	// "\n"
				continue;
			}else
			if((strLine[0] == '#') && bEndLine2)	// 注释行(需要排除用"续行的情况)
			{
				bEndLine1 = TRUE;
				bEndLine2 = TRUE;
				bComment = TRUE;
				int nLen = strLine.GetLength();
				if( (strLine[nLen-1]=='\\') && ((nLen>1) && (strLine[nLen-2]!='\\')) )
				{	// 续行
					bEndLine1 = FALSE;
				}
				asScript.Add(strLineSrc);
				nBufSize += strLineSrc.GetLength();
				nBufSize += 1;	// "\n"
				continue;
			}else
			if(bComment && !bEndLine1)
			{
				asScript.Add(strLineSrc);
				nBufSize += strLineSrc.GetLength();
				nBufSize += 1;	// "\n"
				int nLen = strLine.GetLength();
				if( (strLine[nLen-1]=='\\') && ((nLen>1) && (strLine[nLen-2]!='\\')) )
				{
					continue;
				}
				bEndLine1 = TRUE;	// 不用续行
				continue;
			}

			bComment = FALSE;

			if(!bVarMultiLine)	// 判断是否多行变量的起始行
			{
				int nLen = strLine.GetLength();
				if(strLine[nLen-1] == '{')
				{
					CString strLineTmp = strLine;
					int nPos = strLineTmp.ReverseFind(';');
					if(nPos != -1)
					{
						strLineTmp.Delete(0, nPos+1);
					}
					strLineTmp.TrimLeft("\t ");
					//if(((strLineTmp.Find("set") == 0) || (strLineTmp.Find("append") == 0) || (strLineTmp.Find("lappend") == 0)) &&
					//	(strLineTmp.GetLength() > 4) &&
					//	((strLineTmp[3] == ' ') || (strLineTmp[3] == '\t')) )
					if(((strLineTmp.Find("set") == 0) &&
						(strLineTmp.GetLength() > 4) &&
						((strLineTmp[3] == ' ') || (strLineTmp[3] == '\t')) ) ||
					   ((strLineTmp.Find("lappend") == 0) &&
						(strLineTmp.GetLength() > 8) &&
						((strLineTmp[7] == ' ') || (strLineTmp[7] == '\t')) ) ||
					   ((strLineTmp.Find("append") == 0) &&
						(strLineTmp.GetLength() > 7) &&
						((strLineTmp[6] == ' ') || (strLineTmp[6] == '\t')) ) ||
					   ((strLineTmp.Find("array") == 0) &&
						(strLineTmp.GetLength() > 12) &&
						((strLineTmp[5] == ' ') || (strLineTmp[5] == '\t')) &&
						(strLineTmp.Find(" set ") > 4) ) )
					{
						bVarMultiLine = TRUE;
						nVarLevel = 1;

						CString strBreak;
						strBreak.Format("td *%d;", nLineNo);
						strLineSrc = strBreak + strLineSrc;
						asScript.Add(strLineSrc);
						nBufSize += strLineSrc.GetLength();
						nBufSize += 1;	// "\n"
						m_nTDLineCount ++;
						continue;
					}
				}
			}			

			if(bEndLine1 && bEndLine2 && !bCmdSwitch && !bVarMultiLine)	// 如果不用续行(不处于switch命令中,且不处于多行变量续行状态),则加单步标志
			{
				CString strBreak;
				strBreak.Format("td *%d;", nLineNo);
				strLineSrc = strBreak + strLineSrc;
				asScript.Add(strLineSrc);
				nBufSize += strLineSrc.GetLength();
				nBufSize += 1;	// "\n"
				m_nTDLineCount ++;

				int nLen = strLine.GetLength();
				if( (strLine[nLen-1]=='\\') && ((nLen>1) && (strLine[nLen-2]!='\\')) )
				{	// 续行'\'
					bEndLine1 = FALSE;
					continue;
				}
				/*if((strLine[nLen-1]=='"') && (strLine.Find('"')==(nLen-1)))
				{	// 续行'"'
					bEndLine2 = FALSE;
					continue;
				}*/
				// 对于"续行的情况,需要根据"的个数决定最后的状态
				int nPos = -1;
				while((nPos = strLine.Find('"')) != -1)
				{
					if((nPos == 0) || (strLine[nPos-1] != '\\'))
					{
						bEndLine2 = !bEndLine2;
					}
					strLine.Delete(0, nPos+1);
				}
				continue;
			}

			asScript.Add(strLineSrc);
			nBufSize += strLineSrc.GetLength();
			nBufSize += 1;	// "\n"

			if(!bEndLine1)
			{
				int nLen = strLine.GetLength();
				if( (strLine[nLen-1]=='\\') && ((nLen>1) && (strLine[nLen-2]!='\\')) )
				{
					continue;
				}
				bEndLine1 = TRUE;	// 不用续行
			}

			if(!bEndLine2)
			{
				if(strLine.Find('"') == -1)
				{
					continue;
				}
				//bEndLine2 = TRUE;	// 不用续行
				int nPos = -1;
				while((nPos = strLine.Find('"')) != -1)
				{
					if((nPos == 0) || (strLine[nPos-1] != '\\'))
					{
						bEndLine2 = !bEndLine2;
					}
					strLine.Delete(0, nPos+1);
				}
			}

			if( (strLine.Find("switch") == 0) && (strLine[strLine.GetLength()-1] == '{') &&
				((strLine[6] == ' ') || (strLine[6] == '\t')) )	// 进入switch命令状态
			{
				bCmdSwitch = TRUE;
				nSwitchLevel = 0;
			}else
			if(bCmdSwitch)	// 处于switch命令状态中
			{
				if((nSwitchLevel <= 0) && (strLine[0] == '}'))	// 退出switch命令状态
				{
					bCmdSwitch = FALSE;
				}else
				{
					if(strLine.Find('{'))
					{
						nSwitchLevel++;
					}
					if(strLine.Find('}'))
					{
						nSwitchLevel--;
					}
				}
			}

			if(bVarMultiLine)
			{
				// 当前处于多行变量的续行状态,判断什么时候结束这种状态
				int nPosLeft = -1;
				int nPosRight = -1;
				while( ((nPosRight = strLine.Find('}')) != -1) || ((nPosLeft = strLine.Find('{')) != -1) )
				{
					if((nPosLeft != -1) && ((nPosLeft == 0) || (strLine[nPosLeft-1] != '\\')))
					{
						nVarLevel++;
					}
					if((nPosRight != -1) && ((nPosRight == 0) || (strLine[nPosRight-1] != '\\')))
					{
						nVarLevel--;
					}

					if(nVarLevel <= 0)
					{
						bVarMultiLine = FALSE;
						break;
					}

					//if((nPos == 0) || (strLine[nPos-1] != '\\'))
					//{
					//	bVarMultiLine = FALSE;
					//	break;
					//}
					if(nPosLeft != -1)
					{
						strLine.Delete(0, nPosLeft+1);
					}else
					if(nPosRight != -1)
					{
						strLine.Delete(0, nPosRight+1);
					}
				}
			}

		}

		// 创建缓冲区
		m_pRunBuffer = new char[nBufSize+1];
		m_pRunBuffer[nBufSize] = 0;
		if(nBufSize > 0)
		{	// 保证缓冲区的所有字符不是随机的
			m_pRunBuffer[nBufSize-2] = 0;
			m_pRunBuffer[nBufSize-1] = 0;
		}
		int nPos = 0;
		for(int i=0; i<asScript.GetSize(); i++)
		{
			CString strLine = asScript[i];
			//TRACE("%s\n", strLine);
			//pIPlatUI->Output(2, strLine);
			//pIPlatUI->Output(2, "\r\n");
			memcpy(m_pRunBuffer+nPos, strLine.GetBuffer(strLine.GetLength()), strLine.GetLength());
			strLine.ReleaseBuffer();
			nPos += strLine.GetLength();
			if(i < (asScript.GetSize()-1))
			{				
				memcpy(m_pRunBuffer+nPos, "\n", 1);
				nPos +=1;
			}
		}

	}catch(...)
	{
		return FALSE;
	}

	return TRUE;
}

// 获取脚本文件名
CString __stdcall CTclInterp::GetScriptFile()
{
	// TODO: Add your code here
	return m_strScriptFile;
}

/////////////////////////////////////////////////////////////////////////////
// 执行TK的主循环
/////////////////////////////////////////////////////////////////////////////
void CTclInterp::TkMainLoop()
{
#ifdef _SUPPORT_TK
	if(m_bIsTkFile)
	{
		Tk_Window tkwin = Tk_MainWindow(pInterp);
		if(tkwin)
		{
			Tk_MainLoop();
		}
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
// 销毁TK的主窗口
/////////////////////////////////////////////////////////////////////////////
void CTclInterp::TkDestroyWindow()
{
#ifdef _SUPPORT_TK
	if(m_bIsTkFile)
	{
		Tk_Window tkwin = Tk_MainWindow(pInterp);
		if(tkwin)
		{
			Tk_DestroyWindow(tkwin);
		}
	}
#endif
}

// 运行脚本文件
int __stdcall CTclInterp::RunScriptFile(LPCTSTR lpszPathName)
{
	// TODO: Add your code here
//	if((m_dwRunThreadId != NULL) && (::GetCurrentThreadId() != m_dwRunThreadId))
//	{
//		AfxMessageBox("tcl thread error!");
//		return FALSE;
//	}

	if(lpszPathName != NULL)
	{
		if(!SetScriptFile(lpszPathName))
			return FALSE;
	}

	m_bMultiDebugKeepSelect = FALSE;
	m_bMultiDebugLoadSource = FALSE;

	if(!m_bIsTkFile && (m_nSupportTk == 1))
	{
		// m_nSupportTk==1表示允许自动判断脚本是否支持Tk
		if(CheckIsTkFile())
		{
			m_bIsTkFile = TRUE;
/*
			CWnd* pMainFrame = AfxGetMainWnd();
			char buffer[500];
			sprintf(buffer, "-use 0x%8.8x", pMainFrame->m_hWnd);
			Tcl_SetVar(pInterp, "argv", buffer, TCL_GLOBAL_ONLY);
*/
			#ifdef _SUPPORT_TK
			Tk_Init(pInterp);
			Tcl_StaticPackage(pInterp, "Tk", Tk_Init, Tk_SafeInit);
			#endif
		}
	}

	pInterp->errorLine = 0;	// 清除错误行信息
	//nBreakMode = SCRIPT_BREAK_NORMAL;	// 设置缺省的断点模式
	//nRunMode = SCRIPT_RUN_DEBUG_STEP;

	m_dwRunThreadId = ::GetCurrentThreadId();

	if(SCRIPT_RUN_NORMAL != m_nRunMode)
	{
		if(!ProcessRunBuffer())
		{
			return FALSE;
		}
		return RunScriptBuffer();
	}

	m_nRunState = SCRIPT_STATE_RUN;
	//char szScriptFile[256];
	//strcpy(szScriptFile, m_strScriptFile);

	Tcl_DString dst;
	char* script = Tcl_ExternalToUtfDString(NULL, m_strScriptFile, -1, &dst);

	try {
	////////////////////////////////////////////
	// 执行脚本文件
	if(TCL_OK != Tcl_EvalFile(pInterp, script))
	{
		Tcl_DStringFree(&dst);
		m_nRunState = SCRIPT_STATE_END;
		m_nEvalResult = TCL_ERROR;
		//ConvertResultToUTF(pInterp);
		return FALSE;
	}
	////////////////////////////////////////////
	}catch(...)
	{
		TRACE("RunScriptFile %s exception!\n", m_strScriptFile);
	}

	Tcl_DStringFree(&dst);

	TkMainLoop();

	m_nRunState = SCRIPT_STATE_END;
	m_nEvalResult	= TCL_OK;
	return TRUE;
}

// 执行脚本命令
int __stdcall CTclInterp::RunScriptCommand(LPCTSTR lpszTclCmd)
{
	// TODO: Add your code here
	pInterp->errorLine = 0;	// 清除错误行信息
	//nBreakMode = SCRIPT_BREAK_NORMAL;	// 设置缺省的断点模式

	try {
	////////////////////////////////////////////

	// 修改普通字符串为UTF-8编码字符串
	Tcl_DString dst;
	char* script = Tcl_ExternalToUtfDString(NULL, lpszTclCmd, -1, &dst);
	// 执行脚本命令
	if(TCL_OK != Tcl_Eval(pInterp, script))
	{
		Tcl_DStringFree(&dst);
		m_nEvalResult = TCL_ERROR;
		//ConvertResultToUTF(pInterp);
		return FALSE;
	}

	Tcl_DStringFree(&dst);

	////////////////////////////////////////////
	}catch(...)
	{
		// TRACE最多可以支持512字节,因此需要截断
		CString strTclCmd = lpszTclCmd;
		if(strTclCmd.GetLength() > 400)
		{
			strTclCmd = strTclCmd.Left(400);
		}
		TRACE("RunScriptCommand '%s' exception!\n", strTclCmd);
	}

	m_nEvalResult = TCL_OK;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 执行缓冲区中的TCL命令
/////////////////////////////////////////////////////////////////////////////
int CTclInterp::RunScriptBuffer()
{
	if(NULL == m_pRunBuffer)
		return FALSE;

	m_bProfileEnable = AfxGetApp()->GetProfileInt(REG_TCL_DEBUG_SUBKEY, REG_TCL_ENABLE_PROFILE, FALSE);

	m_nRunState = SCRIPT_STATE_RUN;

	// 修改普通字符串为UTF-8编码字符串
	Tcl_DString dst;
	char* script = Tcl_ExternalToUtfDString(NULL, m_pRunBuffer, -1, &dst);
	// 执行脚本命令
	if(TCL_OK != Tcl_Eval(pInterp, script))
	{
		Tcl_DStringFree(&dst);
		m_nRunState = SCRIPT_STATE_END;
		m_nEvalResult = TCL_ERROR;
		//ConvertResultToUTF(pInterp);

		// 清除从debug文件
		for(int i=0; i<m_aDebugFile.GetSize(); i++)
		{
			CDebugFile* pDebugFile = &(m_aDebugFile[i]);
			CString strFileName = pDebugFile->m_strFile;
			// 向OwmEdit发送设置运行模式为非从调试模式的命令
			if(pIPlatUI)
			{
				pIPlatUI->SendOwmCmd("org.owm.edit", OC_SETSLAVEDEBUG, (WPARAM)(LPTSTR)(LPCTSTR)strFileName, -1, NULL);
			}
			// 清除从文件的调试箭头
			DebugCallback(IDB_USER_SETCURRENTLINE, 0, strFileName);
		}
		m_aDebugFile.RemoveAll();

		return FALSE;
	}

	Tcl_DStringFree(&dst);

	TkMainLoop();

	// 清除从debug文件
	for(int i=0; i<m_aDebugFile.GetSize(); i++)
	{
		CDebugFile* pDebugFile = &(m_aDebugFile[i]);
		CString strFileName = pDebugFile->m_strFile;
		// 向OwmEdit发送设置运行模式为非从调试模式的命令
		if(pIPlatUI)
		{
			pIPlatUI->SendOwmCmd("org.owm.edit", OC_SETSLAVEDEBUG, (WPARAM)(LPTSTR)(LPCTSTR)strFileName, -1, NULL);
		}
		// 清除从文件的调试箭头
		DebugCallback(IDB_USER_SETCURRENTLINE, 0, strFileName);
	}
	m_aDebugFile.RemoveAll();

	m_nRunState = SCRIPT_STATE_END;
	m_nEvalResult	= TCL_OK;
	return TRUE;
}

// 停止脚本运行
int __stdcall CTclInterp::StopScript()
{
	// TODO: Add your code here
	m_bStopSignal	= TRUE;	// 设置中止标志

	// 终止管道进程
	TerminatePipeProcess();

	// 发消息激活消息循环
	// 注：m_dwRunThreadId是保存的脚本运行时的线程ID，如果在其他线程中
	// 调用此函数,就会通过线程消息去激活运行线程中的td命令
	if(m_dwRunThreadId)
	{
		::PostThreadMessage(m_dwRunThreadId, WM_USER, 0, 0);
	}

	return TRUE;
}

// 获取解释器指针
LPVOID __stdcall CTclInterp::GetInterp()
{
	// TODO: Add your code here
	return pInterp;
}

// 获取解释器ID
int __stdcall CTclInterp::GetInterpID()
{
	// TODO: Add your code here
	return m_nInterpID;
}

// 设置解释器名
void __stdcall CTclInterp::SetInterpName(LPCTSTR lpszInterptName)
{
	// TODO: Add your code here
	m_strInterpName = lpszInterptName;

	CString strCmd = "set interpName " + m_strInterpName;
	Tcl_DString dst;
	strCmd = Tcl_ExternalToUtfDString(NULL, strCmd, -1, &dst);
	Tcl_DStringFree(&dst);
	Tcl_Eval(pInterp, strCmd.GetBuffer(0));
	strCmd.ReleaseBuffer();
}

// 获取解释器名
CString __stdcall CTclInterp::GetInterpName()
{
	// TODO: Add your code here

	return m_strInterpName;
}

// 获取错误行号
int __stdcall CTclInterp::GetErrorLine()
{
	// TODO: Add your code here
	if(TCL_ERROR == m_nEvalResult)
		return pInterp->errorLine;
	else
		return 0;
}

// 获取结果信息
CString __stdcall CTclInterp::GetResult()
{
	// TODO: Add your code here
	// 转换UTF-8编码为普通字符串	
	Tcl_DString dst;
	CString strResult = Tcl_UtfToExternalDString(NULL, Tcl_GetStringResult(pInterp), -1, &dst);
	Tcl_DStringFree(&dst);
	return strResult;
}

// 设置变量值
int __stdcall CTclInterp::SetVar(LPCTSTR lpszVarName, LPCTSTR lpszVarValue)
{
	// TODO: Add your code here
	CString strVarName = lpszVarName;
	CString strVarValue = lpszVarValue;
	char* varName = strVarName.GetBuffer(strVarName.GetLength());
	char* strValue = strVarValue.GetBuffer(strVarValue.GetLength());
	Tcl_SetVar(pInterp, varName, strValue, TCL_GLOBAL_ONLY|TCL_LEAVE_ERR_MSG);
	strVarName.ReleaseBuffer();
	strVarValue.ReleaseBuffer();

	return 0;
}

// 获取变量值
CString __stdcall CTclInterp::GetVar(LPCTSTR lpszVarName)
{
	// TODO: Add your code here
	CString strVarName = lpszVarName;
	char* varName = strVarName.GetBuffer(strVarName.GetLength());
	// Tcl_GetVar最后一个参数为0表示先获取局部变量,如果没有再获取全局变量
	CString strVarValue = Tcl_GetVar(pInterp, varName, 0);//TCL_GLOBAL_ONLY|TCL_LEAVE_ERR_MSG);
	strVarName.ReleaseBuffer();
	// 转换UTF-8编码为普通字符串	
	Tcl_DString dst;
	strVarValue = Tcl_UtfToExternalDString(NULL, strVarValue, -1, &dst);
	Tcl_DStringFree(&dst);
	return strVarValue;
}

// 注册扩展命令
int __stdcall CTclInterp::RegisterCommand(LPVOID pInterp, LPCTSTR lpszCmd, LPVOID lpFunc)
{
	// TODO: Add your code here
	Tcl_CreateObjCommand((Tcl_Interp*)pInterp, lpszCmd, (Tcl_ObjCmdProc*)lpFunc, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	return 0;
}

// 设置扩展参数
int __stdcall CTclInterp::SetInterpParam(LPCTSTR lpszParamName, LPCTSTR lpszParamValue)
{
	// TODO: Add your code here
	CString strParamName = lpszParamName;
	CString strParamValue = lpszParamValue;

	if(strParamName == "usetk" && strParamValue == "true")
	{
		if(!m_bIsTkFile)
		{
			m_bIsTkFile = TRUE;
/*
			CWnd* pMainFrame = AfxGetMainWnd();
			char buffer[500];
			sprintf(buffer, "-use 0x%8.8x", pMainFrame->m_hWnd);
			Tcl_SetVar(pInterp, "argv", buffer, TCL_GLOBAL_ONLY);
*/
			#ifdef _SUPPORT_TK
			Tk_Init(pInterp);
			Tcl_StaticPackage(pInterp, "Tk", Tk_Init, Tk_SafeInit);
			#endif
		}
	}else
	if(strParamName == "usetkv" && strParamValue == "true")
	{
		if(!m_bIsTkFile)
		{
			m_bIsTkFile = TRUE;

			#ifdef _SUPPORT_TK
			Tk_Init(pInterp);
			Tcl_StaticPackage(pInterp, "Tk", Tk_Init, Tk_SafeInit);
			#endif
		}

		// 用于TkView的初始化操作
		//m_TclNotifier = Tcl_InitNotifier();
		//Tcl_AlertNotifier(m_TclNotifier);
		Tcl_SetServiceMode(TCL_SERVICE_ALL);

		Tcl_Time timeout;
		timeout.sec = 0;
		timeout.usec = 25000;
		Tcl_SetTimer(&timeout);
	}else
	if(strParamName == "TSRInterp")	// 驻留解释器标志
	{
		SetVar(lpszParamName, lpszParamValue);
	}

	return 0;
}

// 获取扩展参数
CString __stdcall CTclInterp::GetInterpParam(LPCTSTR lpszParamName)
{
	// TODO: Add your code here
	CString strParamName = lpszParamName;
	if(strParamName == "usetk")
	{
		return m_bIsTkFile ? "1" : "0";
	}else
	if(strParamName == "istkview")
	{
		return CheckIsTkvFile() ? "1" : "0";
	}else
	if(strParamName == "TSRInterp")	// 驻留解释器标志
	{
		return GetVar(lpszParamName);
	}

	return "";
}

// 设置运行模式
int __stdcall CTclInterp::SetRunMode(int nRunMode)
{
	m_nRunMode = nRunMode;
	/*if(SCRIPT_RUN_DEBUG_STEP == nRunMode)
	{
		SetBreakMode(SCRIPT_STEP_NEXT);
	}*/
	return true;
}

// 获取运行模式
int __stdcall CTclInterp::GetRunMode()
{
	return m_nRunMode;
}

// 设置运行模式
int __stdcall CTclInterp::SetRunState(int nState)
{
	m_nRunState = nState;
	return true;
}

// 获取运行模式
int __stdcall CTclInterp::GetRunState()
{
	return m_nRunState;
}

// 获取当前堆栈层数
int __stdcall CTclInterp::GetCurStackLevel()
{
	// TODO: Add your code here
	char szTclCmd[128];
	// 命令前需要加::,否则在进入某些脚本后会因为命名空间的问题导致info level命令无法识别
	sprintf(szTclCmd, "::info level");

	if(TCL_OK != Tcl_Eval(pInterp, szTclCmd))
	{
		return -1;
	}

	return atoi(Tcl_GetStringResult(pInterp));
}

// 获取关键字列表
int __stdcall CTclInterp::GetKeyWordList(LPCTSTR lpszType, CStringArray& asKeyWord)
{
	// TODO: Add your code here
	CString strType = lpszType;
	if(strType == "platform")	// 平台扩展关键字
	{
	}else
	if(strType == "platform_keyword_tooltip")	// 平台扩展关键字使用说明
	{
	}

	return asKeyWord.GetSize();
}

// 设置调试事件
int __stdcall CTclInterp::SetDebugEvent(int nEvent, int nParam, LPCTSTR lpszParam)
{
	// TODO: Add your code here
	CString strParam = lpszParam;
	char szTclCmd[128];

	if(nEvent == IDB_STOP)
	{
		sprintf(szTclCmd, "td -stop");
	}else
	if(nEvent == IDB_NEXT)
	{
		sprintf(szTclCmd, "td -next");
	}else
	if(nEvent == IDB_GOTO)
	{
		if(strParam.GetLength() == 0)
		{
			// 行断点
			sprintf(szTclCmd, "td -goto *%d", nParam);
		}else
		{
			// 命名断点
			sprintf(szTclCmd, "td -goto #%s", lpszParam);
		}
	}else
	if(nEvent == IDB_STEPINTO)
	{
		sprintf(szTclCmd, "td -step into");
	}else
	if(nEvent == IDB_SETPOUT)
	{
		sprintf(szTclCmd, "td -step return");
	}else
	if(nEvent == IDB_STEP)
	{
		sprintf(szTclCmd, "td -step");
	}else
	if(nEvent == IDB_END)
	{
		return TRUE;
	}

	if(TCL_OK != Tcl_Eval(pInterp, szTclCmd))
	{
		return FALSE;
	}

	return TRUE;
}

// 设置断点列表
int __stdcall CTclInterp::SetBreakList(LPCTSTR lpszFile, CUIntArray& auBreaks)
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
int __stdcall CTclInterp::GetBreakList(LPCTSTR lpszFile, CUIntArray& auBreaks)
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
int __stdcall CTclInterp::GetEnableProfileAnalyse()
{
	// TODO: Add your code here
	return m_bProfileEnable;
}

// 获取有效代码行数
int __stdcall CTclInterp::GetAvailableLineCount()
{
	// TODO: Add your code here
	return m_nTDLineCount;
}

// 获取代码运行覆盖表
LPVOID __stdcall CTclInterp::GetProfileMap()
{
	// TODO: Add your code here
	return (void*)(&m_ProfileMap);
}

// 判断指定方法是否被支持
int __stdcall CTclInterp::MethodIsSupport(LPCTSTR lpszMethod)
{
	// TODO: Add your code here
	CString strMethod = lpszMethod;
	if( strMethod == "run" )		// 支持运行
	{
		return TRUE;
	}

	if(	(strMethod == "debug") ||	// 支持调试
		(strMethod == "break") )	// 支持断点
	{
		// 检查procomp.exe文件，如果未安装完整包，则Tcl也不允许进行调试
		CString strProCompFile	= GetInterpRootPath();
		strProCompFile	+= "bin\\procomp.exe";
		if(GetFileAttributes(strProCompFile) != 0xFFFFFFFF)
		{
			return TRUE;
		}
	}

	if(strMethod == "compile")		// 支持编译
	{
		// 检查procomp.exe文件
		CString strProCompFile	= GetInterpRootPath();
		strProCompFile	+= "bin\\procomp.exe";
		if(GetFileAttributes(strProCompFile) != 0xFFFFFFFF)
		{
			return TRUE;
		}
	}

	if(strMethod == "build")		// 支持构建可执行程序
	{
		// 检查tclkit.exe文件
		CString strTclKitFile	= GetInterpRootPath();
		strTclKitFile	+= "bin\\tclkit.exe";
		if(GetFileAttributes(strTclKitFile) != 0xFFFFFFFF)
		{
			return TRUE;
		}
	}

	return FALSE;
}

#define SEPATATOE ","
// 初始化调试模式
int __stdcall CTclInterp::InitialDebugMode(CString& strResult)
{
	// TODO: Add your code here
	// 获取平台调试器脚本并加载
	CString strPlatDebugScript = GetPlatRootPath();
	strPlatDebugScript += _T("lib\\plat\\platdbg.tcl");
	strPlatDebugScript.Replace("\\", "/");
	CString strRes;
	CString strCmd;
	strCmd.Format("source %s", strPlatDebugScript);

	if(!RunCommand(strCmd))
	{
		strResult = GetResult();
		return FALSE;
	}

	// 初始化各个调试窗口
	CString strWinVar, strWinObj, strWinFunc, strWinStack;
	LOADRCSTRING( strWinVar, IDS_WINDOW_VAR );
	LOADRCSTRING( strWinObj, IDS_WINDOW_OBJ );
	LOADRCSTRING( strWinFunc, IDS_WINDOW_FUNC );
	LOADRCSTRING( strWinStack, IDS_WINDOW_STACK );

	CString strColName, strColValue, strColParam;
	LOADRCSTRING( strColName, IDS_COLUMN_NAME );
	LOADRCSTRING( strColValue, IDS_COLUMN_VALUE );
	LOADRCSTRING( strColParam, IDS_COLUMN_PARAM );

	// 初始化变量窗口
	m_nOutputVar = ::atoi(pIPlatUI->OutputGet(-1, strWinVar));
	if(m_nOutputVar <= 0)
	{
		//新窗口，创建
		m_nOutputVar = pIPlatUI->AddOutput(strWinVar + "##GRID##" + strWinVar);
	}
	pIPlatUI->OutputSet(m_nOutputVar, "DelColumn ALL;DelItem ALL");
	pIPlatUI->OutputSet(m_nOutputVar, "AddColumn " + strColName + ",150");
	pIPlatUI->OutputSet(m_nOutputVar, "AddColumn " + strColValue + ",150,AutoSize");
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
	pIPlatUI->OutputSet(m_nOutputObject, "AddColumn " + strColName + ",150");
	pIPlatUI->OutputSet(m_nOutputObject, "AddColumn " + strColValue + ",150,AutoSize");
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
	pIPlatUI->OutputSet(m_nOutputFunc, "AddColumn " + strColName + ",300,AutoSize");
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
	pIPlatUI->OutputSet(m_nOutputStack, "AddColumn " + strColName + ",200");
	pIPlatUI->OutputSet(m_nOutputStack, "AddColumn " + strColParam + ",100,AutoSize");
	pIPlatUI->OutputSet(m_nOutputStack, "SetTreeColumn 0");
	pIPlatUI->OutputSet(m_nOutputStack, "SetRowColors RGB(0,0,128) RGB(255,255,205)");
	pIPlatUI->OutputSet(m_nOutputStack, "SetGridColor RGB(192,192,192)");
	pIPlatUI->OutputSet(m_nOutputStack, "ModifyStyle Horizontal Solid;ModifyStyle Vertical Solid");
	pIPlatUI->OutputSet(m_nOutputStack, "ModifyStyle Add(NoSort)");
	pIPlatUI->OutputSet(m_nOutputStack, "SEPARATOR=" SEPATATOE); //设置分隔符
	m_nOutputStack = pIPlatUI->AddOutput(strWinStack + "##GRID");

	// 激活变量窗口
	pIPlatUI->ActiveOutput(m_nOutputVar);

	BOOL bEnableMulitFileDebug = AfxGetApp()->GetProfileInt(REG_TCL_DEBUG_SUBKEY, REG_TCL_ENABLE_MULTIFILEDEBUG, FALSE);
	if(bEnableMulitFileDebug)
	{
		// 重载source命令,用于支持多文件调试
		if(!RunCommand(_T("rename source source_std;rename source_debug source;")))
		{
			strResult = GetResult();
			return FALSE;
		}
	}

	strResult = GetResult();

	return TRUE;
}

// 更新调试窗口信息
int __stdcall CTclInterp::RefreshDebugWindow(LPCTSTR lpszWindow, BOOL bActive, CString& strResult)
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
		RunCommand(_T("global _plat_Tcl_WatchVariables;eval $_plat_Tcl_WatchVariables"));
		if(bActive)
		{
			pIPlatUI->ActiveOutput(m_nOutputVar);
		}
	}
	if(bWindowObject)
	{
		RunCommand(_T("package require Itcl;global _plat_Tcl_WatchObjects;eval $_plat_Tcl_WatchObjects"));
		if(bActive)
		{
			pIPlatUI->ActiveOutput(m_nOutputObject);
		}
	}
	if(bWindowProc)
	{
		RunCommand(_T("global _plat_Tcl_WatchProcedures;eval $_plat_Tcl_WatchProcedures"));
		if(bActive)
		{
			pIPlatUI->ActiveOutput(m_nOutputFunc);
		}
	}
	if(bWindowStack)
	{
		RunCommand(_T("global _plat_Tcl_WatchCallStack;eval $_plat_Tcl_WatchCallStack"));
		if(bActive)
		{
			pIPlatUI->ActiveOutput(m_nOutputStack);
		}
	}

	strResult = GetResult();

	return TRUE;
}

// 调用脚本方法
int __stdcall CTclInterp::CallMethod(LPCTSTR lpszMethod, CStringArray& asParam)
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

// 运行进程并挂接自定义输出管道
BOOL CTclInterp::RunProgramWithPipe(CString strExePath, CString strParam, int nOutputId, BOOL bActiveOutput)
{
	m_hPipeProcess = NULL;

	SECURITY_ATTRIBUTES sa;
	HANDLE hRead,hWrite;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead,&hWrite,&sa,0))
	{
		AfxMessageBox("Create Pipe process failed!");
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
		AfxMessageBox("Create Pipe process failed!");
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

BOOL CTclInterp::TerminatePipeProcess()
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
BOOL CTclInterp::ExecProcess(CString strExePath, CString strParam, BOOL bWaitEnd)
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

/////////////////////////////////////////////////////////////////////////////
// 删除一个指定的目录，包括目录下的文件
// Return Values
// If the function succeeds, the return value is nonzero.
// If the function fails, the return value is zero. 
// To get extended error information, call GetLastError().
/////////////////////////////////////////////////////////////////////////////
int DeleteDir(LPCTSTR strPath)
{
	CFileFind  fileFind;
	int found=0;
	char charPath[MAX_PATH];
	int iDelete=0;
	//删除所有文件
	sprintf(charPath,"%s\\*.*",strPath);
	found = fileFind.FindFile(charPath,0);
	if(!found) return 1 ;//目录不存在

	while(found)
	{
	    found = fileFind.FindNextFile();
		if(fileFind.IsDots() )
			continue;
		if(fileFind.IsDirectory() )
		{
			DeleteDir(fileFind.GetFilePath());
			continue;
		}
		//修改文件属性，让其可以删除
		SetFileAttributes(fileFind.GetFilePath(),FILE_ATTRIBUTE_NORMAL);
	
		iDelete = DeleteFile(fileFind.GetFilePath());
		//TRACE("%d--Delete File:%s\r\n",iDelete,fileFind.GetFilePath());
		if(iDelete==0)
			return iDelete;
	
	}
	fileFind.Close();

	//删除目录
	iDelete = RemoveDirectory(strPath);
	//TRACE("%d--Delete Dir :%s\r\n",iDelete,strPath);
	return iDelete;
}

/////////////////////////////////////////////////////////////////////////////
// 拷贝目录，包括子目录
// exclude为例外的目录
/////////////////////////////////////////////////////////////////////////////
void CopyDirectory(LPCTSTR src,LPCTSTR dst, LPCTSTR exclude)   
{   
	WIN32_FIND_DATA   FindFileData;   
	HANDLE   hFind;   
	char   tmpsrc[256];   
	strcpy(tmpsrc,src);   
	strcat(tmpsrc,"\\*.*");   
	hFind   =   FindFirstFile(tmpsrc,   &FindFileData);   
	if(hFind   ==   INVALID_HANDLE_VALUE)   
		return;

	CreateDirectory(dst,0);

	do   
	{   
		char   newdst[256];   
		strcpy(newdst,dst);   
		if(newdst[strlen(newdst)]!='\\')   
		strcat(newdst,"\\");   
		strcat(newdst,FindFileData.cFileName);   

		char   newsrc[256];   
		strcpy(newsrc,src);   
		if(newsrc[strlen(newsrc)]!='\\')   
		strcat(newsrc,"\\");   
		strcat(newsrc,FindFileData.cFileName);   
		if(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)   
		{
			CString strFileName = FindFileData.cFileName;
			//if( strcmp(FindFileData.cFileName, ".") != 0 &&
			//	strcmp(FindFileData.cFileName, "..") != 0 &&
			//	strcmp(FindFileData.cFileName, exclude) != 0)
			if( strFileName.CompareNoCase(".") != 0 &&
				strFileName.CompareNoCase("..") != 0 &&
				strFileName.CompareNoCase(exclude) != 0)
			{   
				CopyDirectory(newsrc,newdst,exclude);   
			}   
		}else   
		{
			CString strFileName = newsrc;
			strFileName.MakeLower();
			// 不把exe文件打包到输出文件中,避免输出的文件越来越大
			if(strFileName.Find(".exe") != (strFileName.GetLength()-4))
			{
				CopyFile(newsrc,newdst,true);
			}
		}   
	}while(FindNextFile(hFind,&FindFileData));

	FindClose(hFind);   
} 

/////////////////////////////////////////////////////////////////////////////
// 检查指定脚本文件中需要依赖多少个扩展包,重复的不算
/////////////////////////////////////////////////////////////////////////////
int CTclInterp::GetPackageRequire(LPCTSTR lpszFile, CStringArray& asPackage)
{
	int nPackage = 0;

	try
	{
		CStdioFile fi(lpszFile, CFile::modeRead);		
		CString strLine;

		while(fi.ReadString(strLine))
		{
			strLine.TrimLeft("\t ");
			strLine.TrimRight("\t ;");

			int nPos = strLine.Find("package require");
			if(nPos != -1)
			{
				strLine.Delete(0, nPos + 15);
				strLine.TrimLeft("\t ");
				nPos = strLine.Find(";");
				if(nPos != -1)
				{
					strLine = strLine.Left(nPos);
					strLine.TrimRight("\t ");
				}
				nPos = strLine.Find("}");
				if(nPos != -1)
				{
					strLine = strLine.Left(nPos);
					strLine.TrimRight("\t ");
				}
				nPos = strLine.Find(" ");
				if(nPos != -1)
				{
					strLine = strLine.Left(nPos);
					strLine.TrimRight("\t ");
				}

				BOOL bFind = FALSE;
				for(int i=0; i<asPackage.GetSize(); i++)
				{
					if(strLine == asPackage[i])
					{
						bFind = TRUE;
						break;
					}
				}
				if(!bFind)
				{
					asPackage.Add(strLine);
					nPackage++;
				}
			}
		}

	}catch(...)
	{
		return nPackage;
	}

	return nPackage;
}

/////////////////////////////////////////////////////////////////////////////
// 检查指定扩展包目录中是否有需要的扩展包
/////////////////////////////////////////////////////////////////////////////
BOOL CTclInterp::CheckPackageProvider(LPCTSTR lpszIndexFile, CStringArray& asPackage)
{
	BOOL bFind = FALSE;

	try
	{
		CStdioFile fi(lpszIndexFile, CFile::modeRead);		
		CString strLine;

		while(fi.ReadString(strLine))
		{
			strLine.TrimLeft("\t ");

			if(strLine.Find("package ifneeded") == 0)
			{
				strLine.Delete(0, 16);
				strLine.TrimLeft("\t ");
				int nPos = strLine.Find(" ");
				if(nPos != -1)
				{
					strLine = strLine.Left(nPos);
					strLine.TrimRight("\t ");

					for(int i=0; i<asPackage.GetSize(); i++)
					{
						if(strLine == asPackage[i])
						{
							return TRUE;
						}
					}
				}
			}
		}

	}catch(...)
	{
		return FALSE;
	}

	return FALSE;
}

// 选择lib库目录
BOOL CTclInterp::SelectLibPath(CStringArray& asLibPath, CStringArray& asPackage)
{
	CTOptArray aopt;	// 创建属性树元素数组
	TOPTITEM item;		// 属性树元素
	item.hItem		= NULL;
	item.iType		= TOPT_GROUP;
	CString strTemp;
	LOADRCSTRING( strTemp, IDS_OPTDLG_SELECT_TCLLIB_GROUP)
	strTemp = strTemp;//"选择Lib目录";
	item.lpszText	= strTemp;
	item.lpszEdit	= "";
	item.lpszList	= "";
	item.bCheck		= FALSE;
	item.iImage		= TOPTI_FOLDER;
	item.iParent	= -1;
	aopt.Add(item);		// 添加根节点

	item.iParent	= 0;
	item.iType		= TOPT_CHECKBOX;
	item.iImage		= TOPTI_INFO;

	// 找到lib目录
	CString strInterpLibPath = GetInterpRootPath();
	if(strInterpLibPath[strInterpLibPath.GetLength()-1] == '\\')
		strInterpLibPath.Delete(strInterpLibPath.GetLength()-1, 1);
	strInterpLibPath += "\\lib";

	// 遍历lib下的所有子目录
	WIN32_FIND_DATA   FindFileData;
	HANDLE   hFind;
	char   tmpsrc[256];
	strcpy(tmpsrc,strInterpLibPath);
	strcat(tmpsrc,"\\*.*");
	hFind   =   FindFirstFile(tmpsrc,   &FindFileData);
	if(hFind   ==   INVALID_HANDLE_VALUE)
		return FALSE;

	do
	{
		if(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			if( strcmp(FindFileData.cFileName, ".") != 0 &&
				strcmp(FindFileData.cFileName, "..") != 0)
			{
				item.lpszText = FindFileData.cFileName;
				if(strcmp(FindFileData.cFileName, "tcl8.4") == 0)
				{
					// 默认添加tcl8.4目录
					item.bCheck = TRUE;
				}else
				if(CheckPackageProvider(strInterpLibPath + "\\" + FindFileData.cFileName + "\\pkgIndex.tcl", asPackage))
				{
					item.bCheck = TRUE;
				}else
				{
					item.bCheck = FALSE;
				}

				aopt.Add(item);
			}
		}
	}while(FindNextFile(hFind,&FindFileData));

	FindClose(hFind);

	// 添加标题和提示信息
	item.iType		= TOPT_MSG;
	LOADRCSTRING( strTemp, IDS_OPTDLG_SELECT_TCLLIB_TITLE);
	item.lpszText	= strTemp;//"选择Tcl Lib库目录";
	LOADRCSTRING( strTemp, IDS_OPTDLG_SELECT_TCLLIB_MSG);
	item.lpszEdit	= strTemp;//"请选择Tcl Lib库目录，选择的目录会添加到生成的可执行文件中";
	item.iParent	= -1;
	aopt.Add(item);

	int nRet = FALSE;

	try{
		pIPlatUI->SendOwmCmd("", OC_EXECTREEOPTION, 0, (LPARAM)(&aopt), &nRet);
	}catch(...)
	{
		return FALSE;
	}

	if(nRet != IDOK)
	{
		return FALSE;
	}

	for(int i=0; i<aopt.GetSize(); i++)
	{
		if(TOPT_CHECKBOX == aopt[i].iType)
		{
			if(aopt[i].bCheck)
			{
				asLibPath.Add(aopt[i].lpszText);
			}
		}
	}
	
	return TRUE;
}

// 编译
int __stdcall CTclInterp::Compile(LPCTSTR lpszSrcFile, CString& strDestFile, LPCTSTR lpszParams)
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
	strProCompFile	+= "bin\\procomp.exe";

	CString strOutFile = strSrcFile;
	strOutFile.MakeLower();
	int nDotPos = strOutFile.ReverseFind('.');
	if(nDotPos != -1)
	{
		strOutFile.Delete(nDotPos, strOutFile.GetLength() - nDotPos);
	}

	// 自动设置输出文件
	BOOL bAutoOutFile = AfxGetApp()->GetProfileInt(REG_TCL_COMPILE_SUBKEY, REG_TCL_COMPILE_AUTO_OUTFILE, TRUE);
	if(bAutoOutFile)
	{
		strOutFile += ".tbc";
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
	BOOL bForceOverwrite = AfxGetApp()->GetProfileInt(REG_TCL_COMPILE_SUBKEY, REG_TCL_COMPILE_FORCE_OVERWRITE, FALSE);
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

	// 是否添加文件头前缀
	BOOL bAddPrefix = AfxGetApp()->GetProfileInt(REG_TCL_COMPILE_SUBKEY, REG_TCL_COMPILE_ADD_PREFIX, TRUE);
	if(!bAddPrefix)
	{
		strParams += "-prefix none ";
	}

	strParams += "-out \"";
	strParams += strOutFile;
	strParams += "\"";

	strParams += " \"";
	strParams += strSrcFile;
	strParams += "\"";

	strDestFile = strOutFile;

	//return ExecProcess(strProCompFile, strParams, TRUE);

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
int __stdcall CTclInterp::Build(LPCTSTR lpszSrcFile, CString& strDestFile, LPCTSTR lpszParams)
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

	CString strTclKitFile	= GetInterpRootPath();
	strTclKitFile	+= "bin\\tclkit.exe";

	CString strTclKitFile2	= GetInterpRootPath();

	BOOL bIsTkFile = FALSE;	// 是否为Tk脚本

	// 强制按Tk脚本生成
	BOOL bForceTkKit = AfxGetApp()->GetProfileInt(REG_TCL_BUILD_SUBKEY, REG_TCL_BUILD_FORCE_TKKIT, FALSE);
	if(bForceTkKit || CheckIsTkFile())
	{
		strTclKitFile2	+= "bin\\tclkit2_tk.exe";
		bIsTkFile = TRUE;
	}else
	{
		strTclKitFile2	+= "bin\\tclkit2.exe";
		if(GetFileAttributes(strTclKitFile2) == 0xFFFFFFFF)
		{
			CopyFile(strTclKitFile, strTclKitFile2, TRUE);
		}
	}

	CString strSdxFileBin	= GetInterpRootPath();
	strSdxFileBin	+= "bin\\sdx.kit";
	CString strSdxFile = "sdx.kit";

	CString strOutFile = strSrcFile;
	strOutFile.MakeLower();
	int nDotPos = strOutFile.ReverseFind('.');
	if(nDotPos != -1)
	{
		strOutFile.Delete(nDotPos, strOutFile.GetLength() - nDotPos);
	}

	CString strOutFileName = strOutFile;	// 不带后缀的生成文件名

	// 自动设置输出文件
	BOOL bAutoOutFile = AfxGetApp()->GetProfileInt(REG_TCL_BUILD_SUBKEY, REG_TCL_BUILD_AUTO_OUTFILE, TRUE);
	if(bAutoOutFile)
	{
		strOutFile += ".exe";
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

	// 记录当前路径,并设置当前路径
	char szOldPath[256];
	memset(szOldPath, 0, 256);
	DWORD dwLen = GetCurrentDirectory(255, szOldPath);
	CString strPath = strOutFileName;
	strPath.Replace("/", "\\");
	int nPos = strPath.ReverseFind('\\');
	if(nPos >= 0)
	{
		strPath = strPath.Left(nPos);
	}
	SetCurrentDirectory(strPath);

	// 拷贝sdx.kit文件
	CString strSdxFileDest = strPath + "\\" + strSdxFile;
	CopyFile(strSdxFileBin, strSdxFileDest, TRUE);


	CString strCmdBase = "\"";
	strCmdBase += strSdxFile;
	strCmdBase += "\" ";

	CString strCmd = strCmdBase;

	// 1.生成kit文件(tclkit sdx.kit qwrap sourcefile)
	// 文件如果存在可能有影响,因此先删除
	CString strKitFileDest = strOutFileName + ".kit";
	DeleteFile(strKitFileDest);

	strCmd += "qwrap \"";
	strCmd += strSrcFile;
	strCmd += "\"";
	//if(!ExecProcess(strTclKitFile, strCmd, TRUE))
	if(!RunProgramWithPipe(strTclKitFile, strCmd))
	{
		DeleteFile(strSdxFileDest);
		DeleteFile(strKitFileDest);
		m_nRunState = SCRIPT_STATE_END;
		return FALSE;
	}

	// 2.解开kit文件为vsf目录(tclkit sdx.kit unwrap destfile)
	CString strVfsDir = strOutFileName + ".vfs";
	// 目录如果存在可能有影响,因此先删除
	DeleteDir(strVfsDir);

	strOutFileName.Replace("/", "\\");
	nPos = strOutFileName.ReverseFind('\\');
	if(nPos >= 0)
	{
		strOutFileName.Delete(0, nPos+1);
	}
	strCmd = strCmdBase;
	strCmd += "unwrap \"";
	strCmd += strOutFileName;
	strCmd += ".kit\"";
	//if(!ExecProcess(strTclKitFile, strCmd, TRUE))
	if(!RunProgramWithPipe(strTclKitFile, strCmd))
	{
		if(dwLen > 0)
		{
			CString strOldPath = szOldPath;
			SetCurrentDirectory(strOldPath);
		}

		DeleteFile(strSdxFileDest);
		DeleteFile(strKitFileDest);
		DeleteDir(strVfsDir);

		m_nRunState = SCRIPT_STATE_END;
		return FALSE;
	}

	// 3.向vsf中添加扩展的文件
	CStringArray asPackage;
	GetPackageRequire(strSrcFile, asPackage);

	// 3.1 添加Tcl Lib目录
	BOOL bAddTclLib = AfxGetApp()->GetProfileInt(REG_TCL_BUILD_SUBKEY, REG_TCL_BUILD_ADD_TCL_LIB, TRUE);
	if(bAddTclLib)
	{
		CStringArray asLibPath;
		if(SelectLibPath(asLibPath, asPackage))
		{
			CString strInterpLibPath = GetInterpRootPath();
			if(strInterpLibPath[strInterpLibPath.GetLength()-1] == '\\')
				strInterpLibPath.Delete(strInterpLibPath.GetLength()-1, 1);
			strInterpLibPath += "\\lib";

			//Copy the source path files to the vfs lib path
			CString strDestPath = strVfsDir + "\\lib";
			for(int i=0; i<asLibPath.GetSize(); i++)
			{
				CopyDirectory(strInterpLibPath + "\\" + asLibPath[i], strDestPath + "\\" + asLibPath[i], "");
			}
		}
	}

	// 3.2 添加源代码目录所有文件
	CString strSourcePath = strSrcFile;
	strSourcePath.Replace("/", "\\");
	nPos = strSourcePath.ReverseFind('\\');
	if(nPos != -1)
	{
		strSourcePath = strSourcePath.Left(nPos);
	}
	BOOL bAddSourcePathFiles = AfxGetApp()->GetProfileInt(REG_TCL_BUILD_SUBKEY, REG_TCL_BUILD_ADD_SRC_PATHFILES, FALSE);
	if(bAddSourcePathFiles)
	{
		//Copy the source path files to the vfs lib path
		CString strDestPath = strVfsDir + "\\lib\\app-";
		strDestPath += strOutFileName;

		CString strVfsExcludeDir = strOutFileName + ".vfs";
		CopyDirectory(strSourcePath, strDestPath, strVfsExcludeDir);

		// 删除几个特例文件
		DeleteFile(strDestPath + "\\sdx.kit");
		DeleteFile(strDestPath + "\\" + strOutFileName + ".kit");
		DeleteDir(strDestPath + "\\lib");
	}

	// 3.3 添加源代码目录lib目录下的所有子目录到lib目录
	BOOL bAddSourceLib = AfxGetApp()->GetProfileInt(REG_TCL_BUILD_SUBKEY, REG_TCL_BUILD_ADD_SRC_LIB, TRUE);
	if(bAddSourceLib && (GetFileAttributes(strSourcePath + "\\lib") != 0xFFFFFFFF))
	{
		//Copy the source lib path to the vfs lib path
		CString strDestPath = strVfsDir + "\\lib";
		CopyDirectory(strSourcePath + "\\lib", strDestPath, "");
	}

	// 4.生成最终的可执行文件(tclkit sdx.kit wrap destfile -runtime tclkit2.exe)
	strCmd = strCmdBase;
	strCmd += "wrap \"";
	strCmd += strOutFile;
	strCmd += "\" -runtime \"";
	strCmd += strTclKitFile2;
	strCmd += "\"";
	//if(!ExecProcess(strTclKitFile, strCmd, TRUE))
	if(!RunProgramWithPipe(strTclKitFile, strCmd))
	{
		if(dwLen > 0)
		{
			CString strOldPath = szOldPath;
			SetCurrentDirectory(strOldPath);
		}

		DeleteFile(strSdxFileDest);
		DeleteFile(strKitFileDest);
		DeleteDir(strVfsDir);

		m_nRunState = SCRIPT_STATE_END;
		return FALSE;
	}

	// 5.删除vsf目录和kit文件

	// 执行结束,恢复当前路径的设置(add at 2004-09-07)
	if(dwLen > 0)
	{
		CString strOldPath = szOldPath;
		SetCurrentDirectory(strOldPath);
	}

	// 删除临时文件
	DeleteFile(strSdxFileDest);
	DeleteFile(strKitFileDest);
	DeleteDir(strVfsDir);

	strDestFile = strOutFile;

	// 6.替换可执行文件的图标(仅限于Tk脚本)
	BOOL bReplaceIcon = AfxGetApp()->GetProfileInt(REG_TCL_BUILD_SUBKEY, REG_TCL_BUILD_REPLACE_ICON, FALSE);
	if(bIsTkFile && bReplaceIcon)
	{
		// 默认的图标文件
		CString strTkExeIcon = GetInterpRootPath();
		strTkExeIcon += "bin\\tkexe.ico";

		// 图标选择的方式
		CString strNewIcon = "";
		int nSelectIconType = AfxGetApp()->GetProfileInt(REG_TCL_BUILD_SUBKEY, REG_TCL_BUILD_SELECT_ICON_TYPE, 0);
		if(nSelectIconType == 0)	// 使用源代码目录下的同名图标文件
		{
			strNewIcon = strSourcePath + "/" + strOutFileName + ".ico";
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
				m_nRunState = SCRIPT_STATE_END;
				return TRUE;
			}
		}
		if(GetFileAttributes(strNewIcon) == 0xFFFFFFFF)
		{
			m_nRunState = SCRIPT_STATE_END;
			return TRUE;
		}

		// 获取替换图标脚本并加载执行
		CString strIconReplaceScript = GetInterpRootPath();
		strIconReplaceScript += _T("bin\\iconreplace.tcl");
		strIconReplaceScript.Replace("\\", "/");
		CString strRes;
		CString strCmd;
		strCmd.Format("source %s;set filename {%s};set newicon {%s}; set fwname {%s};IconReplace $filename $newicon $fwname",
			strIconReplaceScript,
			strOutFile,
			strNewIcon,
			strTkExeIcon);

		if(!RunCommand(strCmd))
		{
			CString strResult = GetResult();
			AfxMessageBox(strResult);
			m_nRunState = SCRIPT_STATE_END;
			return TRUE;
		}
	}

	m_nRunState = SCRIPT_STATE_END;

	return TRUE;
}

//}}VCI_IMPLEMENT_END

/////////////////////////////////////////////////////////////////////////////
// 设置断点激活信号
/////////////////////////////////////////////////////////////////////////////
void CTclInterp::SetBreakSignal(BOOL bBreak)
{
	m_bBreakSignal = bBreak;	// 设置断点激活标志
	// 发消息激活消息循环
	// 注：m_dwRunThreadId是保存的脚本运行时的线程ID，如果在其他线程中
	// 调用此函数,就会通过线程消息去激活运行线程中的td命令
	if(m_dwRunThreadId)
	{
		::PostThreadMessage(m_dwRunThreadId, WM_USER, 0, 0);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 调用调试回调函数
/////////////////////////////////////////////////////////////////////////////
void CTclInterp::DebugCallback(int nEvent, int nLine, CString strFile)
{
	if(!m_lpfnDebug)
	{
		return;
	}

	if(strFile == "")
	{
		m_lpfnDebug(nEvent, GetScriptFile(), nLine);
	}else
	{
		m_lpfnDebug(nEvent, strFile, nLine);
	}
}

// 执行脚本命令(内部使用)
int CTclInterp::RunCommand(CString strCmd)
{
	Tcl_DString dst;
	strCmd = Tcl_ExternalToUtfDString(NULL, strCmd, -1, &dst);
	Tcl_DStringFree(&dst);
	if(TCL_OK != Tcl_Eval(pInterp, strCmd.GetBuffer(strCmd.GetLength())))
	{
		strCmd.ReleaseBuffer();
		return FALSE;
	}
	strCmd.ReleaseBuffer();

	return TRUE;
}

// 执行脚本命令(内部使用),支持在全局或局部空间运行脚本
int CTclInterp::RunCommandEx(CString strCmd, int nFlag)
{
	Tcl_DString dst;
	strCmd = Tcl_ExternalToUtfDString(NULL, strCmd, -1, &dst);
	Tcl_DStringFree(&dst);
	if(TCL_OK != Tcl_EvalEx(pInterp, strCmd.GetBuffer(strCmd.GetLength()), -1, nFlag))
	{
		strCmd.ReleaseBuffer();
		return FALSE;
	}
	strCmd.ReleaseBuffer();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 注册扩展命令
/////////////////////////////////////////////////////////////////////////////
int CTclInterp::Tcl_RegisterCmd(Tcl_Interp* pInterp)
{
	// 注册脚本调试命令
	Tcl_CreateCommand(pInterp, "td", Tcl_Cmd_Plat_Debug, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 注册调试用的source命令
	Tcl_CreateCommand(pInterp, "plat_debug_source", Tcl_Cmd_Source, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 注册新的exit命令
	Tcl_CreateCommand(pInterp, "exit", Tcl_Cmd_Exit, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 对标准puts命令的扩展
	Tcl_CreateObjCommand(pInterp, "puts", Tcl_PutsObjCmd_Ex, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 获取平台信息
	Tcl_CreateCommand(pInterp, "platform_info", Tcl_Cmd_PlatformInfo, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 获取最近打开的文件列表
	Tcl_CreateCommand(pInterp, "recent-file-list", Tcl_Cmd_RecentFileList, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 获取最近打开的工程列表
	Tcl_CreateCommand(pInterp, "recent-project-list", Tcl_Cmd_RecentProjectList, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 注册TclVci命令
	Tclvci_Init(pInterp);

	// 注册TclXml命令
	TclXml_Init(pInterp);

	return TCL_OK;
}

//////////////////////////////////////////////////////////////////////////
// 接口实现
//////////////////////////////////////////////////////////////////////////
INTERFACE_IMPLEMENT(TclInterp)
CONTROL_INTERFACE_IMPLEMENT(TclInterp)

//{{ 注意：!!!这里是保留的重要函数，不可删除!!!
// 获取接口的额外信息
LPVOID __stdcall CTclInterp::XTclInterp::GetExtraInfo(LPVOID lpRefData)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj;
}
//}} 注意：!!!这里是保留的重要函数，不可删除!!!
//{{VCI_INTERFACE_IMPLEMENT_BEGIN
// 设置平台UI接口指针
int __stdcall CTclInterp::XTclInterp::SetIPlatUI(LPVOID lpIPlatUI)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->SetIPlatUI(lpIPlatUI);
}

int __stdcall CTclInterp::XTclInterp::SetLibPath(LPCTSTR lpszPath)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->SetLibPath(lpszPath);
}

int __stdcall CTclInterp::XTclInterp::IsInterpRunning()
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->IsInterpRunning();
}

int __stdcall CTclInterp::XTclInterp::SetScriptFile(LPCTSTR lpszPathName)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->SetScriptFile(lpszPathName);
}

CString __stdcall CTclInterp::XTclInterp::GetScriptFile()
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->GetScriptFile();
}

int __stdcall CTclInterp::XTclInterp::RunScriptFile(LPCTSTR lpszPathName)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->RunScriptFile(lpszPathName);
}

int __stdcall CTclInterp::XTclInterp::RunScriptCommand(LPCTSTR lpszTclCmd)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->RunScriptCommand(lpszTclCmd);
}

int __stdcall CTclInterp::XTclInterp::StopScript()
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->StopScript();
}

LPVOID __stdcall CTclInterp::XTclInterp::GetInterp()
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->GetInterp();
}

int __stdcall CTclInterp::XTclInterp::GetInterpID()
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->GetInterpID();
}

void __stdcall CTclInterp::XTclInterp::SetInterpName(LPCTSTR lpszInterptName)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	pObj->SetInterpName(lpszInterptName);
}

CString __stdcall CTclInterp::XTclInterp::GetInterpName()
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->GetInterpName();
}

int __stdcall CTclInterp::XTclInterp::GetErrorLine()
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->GetErrorLine();
}

CString __stdcall CTclInterp::XTclInterp::GetResult()
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->GetResult();
}

int __stdcall CTclInterp::XTclInterp::SetVar(LPCTSTR lpszVarName, LPCTSTR lpszVarValue)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->SetVar(lpszVarName, lpszVarValue);
}

CString __stdcall CTclInterp::XTclInterp::GetVar(LPCTSTR lpszVarName)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->GetVar(lpszVarName);
}

int __stdcall CTclInterp::XTclInterp::RegisterCommand(LPVOID pInterp, LPCTSTR lpszCmd, LPVOID lpFunc)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->RegisterCommand(pInterp, lpszCmd, lpFunc);
}

int __stdcall CTclInterp::XTclInterp::SetInterpParam(LPCTSTR lpszParamName, LPCTSTR lpszParamValue)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->SetInterpParam(lpszParamName, lpszParamValue);
}

CString __stdcall CTclInterp::XTclInterp::GetInterpParam(LPCTSTR lpszParamName)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->GetInterpParam(lpszParamName);
}

int __stdcall CTclInterp::XTclInterp::SetRunMode(int nRunMode)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->SetRunMode(nRunMode);
}

int __stdcall CTclInterp::XTclInterp::GetRunMode()
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->GetRunMode();
}

int __stdcall CTclInterp::XTclInterp::SetRunState(int nRunState)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->SetRunState(nRunState);
}

int __stdcall CTclInterp::XTclInterp::GetRunState()
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->GetRunState();
}

int __stdcall CTclInterp::XTclInterp::GetCurStackLevel()
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->GetCurStackLevel();
}

// 获取关键字列表
int __stdcall CTclInterp::XTclInterp::GetKeyWordList(LPCTSTR lpszType, CStringArray& asKeyWord)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->GetKeyWordList(lpszType, asKeyWord);
}

int __stdcall CTclInterp::XTclInterp::SetDebugEvent(int nEvent, int nParam, LPCTSTR lpszParam)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->SetDebugEvent(nEvent, nParam, lpszParam);
}

int __stdcall CTclInterp::XTclInterp::SetBreakList(LPCTSTR lpszFile, CUIntArray& auBreaks)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->SetBreakList(lpszFile, auBreaks);
}

int __stdcall CTclInterp::XTclInterp::GetBreakList(LPCTSTR lpszFile, CUIntArray& auBreaks)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->GetBreakList(lpszFile, auBreaks);
}

int __stdcall CTclInterp::XTclInterp::GetEnableProfileAnalyse()
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->GetEnableProfileAnalyse();
}

int __stdcall CTclInterp::XTclInterp::GetAvailableLineCount()
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->GetAvailableLineCount();
}

LPVOID __stdcall CTclInterp::XTclInterp::GetProfileMap()
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->GetProfileMap();
}

int __stdcall CTclInterp::XTclInterp::MethodIsSupport(LPCTSTR lpszMethod)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->MethodIsSupport(lpszMethod);
}

int __stdcall CTclInterp::XTclInterp::InitialDebugMode(CString& strResult)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->InitialDebugMode(strResult);
}

int __stdcall CTclInterp::XTclInterp::RefreshDebugWindow(LPCTSTR lpszWindow, BOOL bActive, CString& strResult)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->RefreshDebugWindow(lpszWindow, bActive, strResult);
}

int __stdcall CTclInterp::XTclInterp::CallMethod(LPCTSTR lpszMethod, CStringArray& asParam)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->CallMethod(lpszMethod, asParam);
}

int __stdcall CTclInterp::XTclInterp::Compile(LPCTSTR lpszSrcFile, CString& strDestFile, LPCTSTR lpszParams)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
	return pObj->Compile(lpszSrcFile, strDestFile, lpszParams);
}

int __stdcall CTclInterp::XTclInterp::Build(LPCTSTR lpszSrcFile, CString& strDestFile, LPCTSTR lpszParams)
{
	CTclInterp *pObj = GET_INTERFACE_OBJECT(TclInterp);
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
