////////////////////////////////////////////////////////////////////////////
//	File:		LuaExCmd.cpp
//	Version:	1.0.0.0
//	Created:	2007-12-20
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	implementation of Lua Ex cmds
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CLuaInterp.h"
#include "wait.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// output函数
static int lua_glue_output(lua_State *L)
{
	if(lua_gettop(L) < 2)
	{
		lua_pushstring(L, "Output: parameter mismatch");
		lua_error(L);
		return 0;
	}

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

	int nOT = lua_tonumber(L, 1);

	// Get the number of parameters
	int n = lua_gettop(L);
	for(int i = 2; i <= n; i ++)
	{
		//const char *s = lua_tostring(L, i);
		CString s = GetValueString(L, i);
		pIPlatUI->Output(nOT, s);
	}

	// No results
	return 0;
}

// outputset函数
static int lua_glue_outputset(lua_State *L)
{
	if(lua_gettop(L) < 2)
	{
		lua_pushstring(L, "OutputSet: parameter mismatch");
		lua_error(L);
		return 0;
	}

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

	int nOT = lua_tonumber(L, 1);

	// Get the number of parameters
	int n = lua_gettop(L);
	for(int i = 2; i <= n; i ++)
	{
		CString s = GetValueString(L, i);
		pIPlatUI->OutputSet(nOT, "COLOR="+s);	// 设置输出颜色
	}

	// No results
	return 0;
}

// outputget函数
static int lua_glue_outputget(lua_State *L)
{
	if(lua_gettop(L) < 2)
	{
		lua_pushstring(L, "OutputGet: parameter mismatch");
		lua_error(L);
		return 0;
	}

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

	int nOT = lua_tonumber(L, 1);
	CString strParam = GetValueString(L, 2);
	CString strRet = pIPlatUI->OutputGet(nOT, strParam);

	// result
	lua_pushstring(L, strRet);
	return 1;
}

// addoutput函数
static int lua_glue_addoutput(lua_State *L)
{
	if(lua_gettop(L) < 1)
	{
		lua_pushstring(L, "AddOutput: parameter mismatch");
		lua_error(L);
		return 0;
	}

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

	CString strName = GetValueString(L, 1);
	int nOT = pIPlatUI->AddOutput(strName);

	// result
	lua_pushnumber(L, nOT);
	return 1;
}

// deloutput函数
static int lua_glue_deloutput(lua_State *L)
{
	if(lua_gettop(L) < 1)
	{
		lua_pushstring(L, "DelOutput: parameter mismatch");
		lua_error(L);
		return 0;
	}

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

	int nOT = lua_tonumber(L, 1);
	pIPlatUI->DelOutput(nOT);

	// no result
	return 0;
}

// activeoutput函数
static int lua_glue_activeoutput(lua_State *L)
{
	if(lua_gettop(L) < 1)
	{
		lua_pushstring(L, "ActiveOutput: parameter mismatch");
		lua_error(L);
		return 0;
	}

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

	int nOT = lua_tonumber(L, 1);
	pIPlatUI->ActiveOutput(nOT);

	// no result
	return 0;
}

// outputclear函数
static int lua_glue_outputclear(lua_State *L)
{
	if(lua_gettop(L) < 1)
	{
		lua_pushstring(L, "OutputClear: parameter mismatch");
		lua_error(L);
		return 0;
	}

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

	int nOT = lua_tonumber(L, 1);
	pIPlatUI->OutputClear(nOT);

	// no result
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:信息提示对话框
/////////////////////////////////////////////////////////////////////////////
static int lua_glue_MessageBox(lua_State *L)
{
	// We need 1-3 parameters
	if(lua_gettop(L) < 2)
	{
		lua_pushstring(L, "MessageBox: parameter mismatch");
		lua_error(L);
		return 0;
	}

	// Get the parameters
	CString strTitle = _T("Platform");
	CString strMsg;
	UINT uType = 0;
	for(int i=1; i<=lua_gettop(L); i++)
	{
		CString strParam = "";
		if(lua_isstring(L, i))
		{
			strParam = lua_tostring(L, i);
		}
		if(strParam[0] == '-')
			strParam.Delete(0, 1);
		strParam.MakeUpper();

		if(strParam == "TITLE")	// 标题
		{
			if((lua_gettop(L)-i) < 2)
			{
				lua_pushstring(L, "wrong # args: lost title param");
				lua_error(L);
				return 0;
			}
			strTitle = lua_tostring(L, i+1);
			i++;
		}else
		if(strParam == "MB_OK")	// 选项
		{
			uType |= MB_OK;
		}else
		if(strParam == "MB_OKCANCEL")
		{
			uType |= MB_OKCANCEL;
		}else
		if(strParam == "MB_RETRYCANCEL")
		{
			uType |= MB_RETRYCANCEL;
		}else
		if(strParam == "MB_YESNO")
		{
			uType |= MB_YESNO;
		}else
		if(strParam == "MB_ICONWARNING")
		{
			uType |= MB_ICONWARNING;
		}else
		if(strParam == "MB_ICONINFORMATION")
		{
			uType |= MB_ICONINFORMATION;
		}else
		if(strParam == "MB_ICONQUESTION")
		{
			uType |= MB_ICONQUESTION;
		}else if(strParam == "MB_ICONERROR")
		{
			uType |= MB_ICONERROR;
		}else
		{	// 信息
			strMsg = lua_tostring(L, i);
		}
	}

	if(uType == 0)
	{	// 缺省的类型
		uType = MB_OK;
	}

	HWND hWndParent = NULL;
	if(AfxGetMainWnd())
		hWndParent = AfxGetMainWnd()->m_hWnd;
	int nRet = ::MessageBox(hWndParent, strMsg, strTitle, uType);
	// Push the number on the stack, result
	lua_pushnumber(L, nRet);
	// IDYES	= 7
	// IDNO		= 6

	// 1 results
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:文件对话框
/////////////////////////////////////////////////////////////////////////////
static int lua_glue_FileDlg(lua_State *L)
{
	// We need 1-3 parameters
	if(lua_gettop(L) < 2)
	{
		lua_pushstring(L, "FileDlg: parameter mismatch");
		lua_error(L);
		return 0;
	}

	BOOL bOpen = TRUE;
	CString strExt = "";
	CString strFileName = "";
	CString strFilter = "";
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

	// 参数解析
	for(int i=1; i<=lua_gettop(L); i++)
	{
		CString strParam = "";
		if(lua_isstring(L, i))
		{
			strParam = lua_tostring(L, i);
		}
		if((strParam.GetLength() > 0) && (strParam[0] == '-'))
			strParam.Delete(0, 1);
		strParam.MakeUpper();

		if(strParam == "EXT")	// File Ext
		{
			if((lua_gettop(L)-i) < 1)
			{
				lua_pushstring(L, "wrong # args: lost file ext param");
				lua_error(L);
				return 0;
			}
			strExt = lua_tostring(L, i+1);
			i++;
		}else
		if(strParam == "NAME")	// File Name
		{
			if((lua_gettop(L)-i) < 1)
			{
				lua_pushstring(L, "wrong # args: lost file name param");
				lua_error(L);
				return 0;
			}
			strFileName = lua_tostring(L, i+1);
			i++;
		}else
		if(strParam == "FILTER")	// Filter
		{
			if((lua_gettop(L)-i) < 1)
			{
				lua_pushstring(L, "wrong # args: lost file filter param");
				lua_error(L);
				return 0;
			}
			strFilter = lua_tostring(L, i+1);
			i++;
		}else
		if(strParam == "SAVETO")	// saveto param
		{
			bOpen = FALSE;
		}else
		if(strParam == "OFN_ALLOWMULTISELECT")	// allow multi select
		{
			dwFlags |= OFN_ALLOWMULTISELECT;
		}
	}

	CFileDialog dlg(bOpen, strExt, strFileName, dwFlags, strFilter);
	if(dlg.DoModal() == IDOK)
	{
		if(dwFlags & OFN_ALLOWMULTISELECT)
		{
			POSITION pos = dlg.GetStartPosition();
			CString strPathName;
			int nCount = 0;
			while(pos != NULL)
			{
				strPathName = dlg.GetNextPathName(pos);
				lua_pushstring(L, strPathName);
				nCount++;
			}

			return nCount;
		}else
		{
			lua_pushstring(L, dlg.GetPathName());	// 返回路径
		}		
	}else
	{
		lua_pushstring(L, "");
	}

	// 1 results
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:路径选择对话框
/////////////////////////////////////////////////////////////////////////////
static int lua_glue_PathSelectDlg(lua_State *L)
{
	// We need 1-3 parameters
	if(lua_gettop(L) < 1)
	{
		lua_pushstring(L, "wrong # args: should be \"PathSelect(title, ?InitPath?)\"");
		lua_error(L);
		return 0;
	}

	CLuaInterp* pInterp = GetLuaInterp((LPVOID)L);
	IPlatUI* pIPlatUI = NULL;
	if(pInterp)
	{
		pIPlatUI = pInterp->pIPlatUI;
	}
	if(pIPlatUI == NULL)
	{
		lua_pushstring(L, "Get platform interface fail.");
		lua_error(L);
		return 0;
	}

	CString strTitle = lua_tostring(L, 1);
	CString strPath = "";
	if(lua_gettop(L) >= 2)
	{
		strPath = lua_tostring(L, 2);
	}

	int nRet;
	pIPlatUI->SendOwmCmd("", OC_PATHSELECT_DIALOG, (WPARAM)(LPCTSTR)strTitle, (LPARAM)(LPCTSTR)strPath, &nRet);

	CString strRet = "";
	if(nRet)
	{
		CString* pStr = (CString*)nRet;
		strRet = *pStr;
		delete pStr;
	}

	lua_pushstring(L, strRet);

	// 1 results
	return 1;
}

// Lua扩展函数注册函数
int LuaExCmd_Register(lua_State *L)
{
	// 注册命令

	// output
	lua_register(L, "Output",  lua_glue_output);
	lua_register(L, "OutputSet",  lua_glue_outputset);
	lua_register(L, "OutputGet",  lua_glue_outputget);
	lua_register(L, "AddOutput",  lua_glue_addoutput);
	lua_register(L, "DelOutput",  lua_glue_deloutput);
	lua_register(L, "ActiveOutput",  lua_glue_activeoutput);
	lua_register(L, "OutputClear",  lua_glue_outputclear);

	// 信息提示对话框
	lua_register(L, "MessageBox",  lua_glue_MessageBox);

	// 文件对话框
	lua_register(L, "FileDlg",  lua_glue_FileDlg);

	// 路径选择对话框
	lua_register(L, "PathSelect",  lua_glue_PathSelectDlg);

	// 通用进度对话框

	// 属性树对话框

	// 登录对话框

	// 输出窗口操作

	// 状态行操作

	// 消息方式延时

	// TaskDialog操作

	// PopupControl操作

	// ImageManager操作
	
    return 0;
}
