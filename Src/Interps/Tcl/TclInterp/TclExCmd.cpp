////////////////////////////////////////////////////////////////////////////
//	File:		TclExCmd.cpp
//	Version:	1.0.0.0
//	Created:	2006-05-27
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	implementation of Tcl Ex cmds
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CTclInterp.h"
#include "wait.h"
#include "TclExCmd.h"
#include "IProject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Tcl command

//TCL函数定义宏
#define TCL_CMDEF(fname) int fname (ClientData cd, Tcl_Interp *pInterp, int objc, Tcl_Obj *CONST objv[])
#define TCL_CMD_PARAM    ClientData cd, Tcl_Interp *pInterp, int objc, Tcl_Obj *CONST objv[]

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:License
// GetESN		: 获取硬件ID
// GenerateLID	: 获取License ID
// ImportLicense: 导入License信息到注册表
// GetLicense	: 获取License ID
// VerifyLID	: 校验License ID
// GetLicenseInfo: 获取某个功能项的License信息
// GetCustomerInfo: 获取授权用户信息
// GetOrderInfo: 获取订单信息
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_Plat_License(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc < 2)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "option");
		return TCL_ERROR;
	}

	CTclInterp* pTclInterp = GetTclInterp((LPVOID)interp);
	if(pTclInterp == NULL)
	{
		Tcl_AppendResult(interp, "Get tcl interp pointer failed!", (char *)NULL);
		return TCL_ERROR;
	}

	ILicense* pILicense = pTclInterp->GetILicense();
	if(pILicense == NULL)
	{
		Tcl_AppendResult(interp, "Get license interface failed!", (char *)NULL);
		return TCL_ERROR;
	}

	CString strOption = Tcl_GetString(objv[1]);

	if(strOption == "GetESN")	// 获取硬件ID
	{
		CString strESN = "";
		pILicense->GetESN(strESN);
		Tcl_AppendResult(interp, strESN, (char *)NULL);
	}else
#ifdef _DEBUG
	if(strOption == "GenerateLID")	// 获取License ID
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "GenerateLID ESN MaskCode");
			return TCL_ERROR;
		}

		CString strESN = Tcl_GetString(objv[2]);
		CString strMaskCode = Tcl_GetString(objv[3]);
		CString strLicenseID = "";
		pILicense->GenerateLicenseID(strESN, strMaskCode, strLicenseID);
		Tcl_AppendResult(interp, strLicenseID, (char *)NULL);
	}else
#endif
	if(strOption == "ImportLicense")	// 导入License信息到注册表
	{
		if(objc < 3)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "ImportLicense LID");
			return TCL_ERROR;
		}

		CString strLicenseID = Tcl_GetString(objv[2]);
		if(pILicense->ImportLicenseID(strLicenseID) != License::trOk)
		{
			Tcl_AppendResult(interp, "Import License ID failed!", (char *)NULL);
			return TCL_ERROR;
		}
	}else
	if(strOption == "GetLicense")	// 获取License ID
	{
		CString strLicenseID = "";
		if(pILicense->GetLicenseID(strLicenseID) == License::trOk)
		{
			Tcl_AppendResult(interp, strLicenseID, (char *)NULL);
		}
	}else
	if(strOption == "VerifyLID")	// 校验LicenseID
	{
		CString strMaskCode = Tcl_GetString(objv[2]);
		if(pILicense->VerifyLicenseID(strMaskCode) != License::trOk)
		{
			Tcl_AppendResult(interp, "Verify License ID failed!", (char *)NULL);
			return TCL_ERROR;
		}
		Tcl_AppendResult(interp, "1", (char *)NULL);
	}else
	if(strOption == "GetLicenseInfo")	// 获取某个功能项的License信息
	{
		if(objc < 3)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "GetLicenseInfo Function");
			return TCL_ERROR;
		}

		CString strFuncID = Tcl_GetString(objv[2]);
		int nLicenseType;
		CTime tUpdateLimit;
		int nUserCount;
		CTime tTrialTime;
		if(pILicense->GetPluginLicenseInfo(strFuncID, nLicenseType, tUpdateLimit, nUserCount, tTrialTime) != License::trOk)
		{
			//Tcl_AppendResult(interp, "Not found license info of " + strFuncID + "!", (char *)NULL);
			Tcl_AppendResult(interp, "Free", (char *)NULL);
			return TCL_OK;
		}
		CString strTmp;
		strTmp.Format("%d", nLicenseType);
		Tcl_AppendElement(interp, strTmp);
		strTmp.Format("%d", tUpdateLimit);
		Tcl_AppendElement(interp, strTmp);
		strTmp.Format("%d", nUserCount);
		Tcl_AppendElement(interp, strTmp);
		strTmp.Format("%d", tTrialTime);
		Tcl_AppendElement(interp, strTmp);
	}else
	if(strOption == "GetCustomerInfo")	// 获取授权用户信息
	{
		CString strCustomerInfo = "";
		if(pILicense->GetCustomerInfo(strCustomerInfo) == License::trOk)
		{
			Tcl_AppendResult(interp, strCustomerInfo, (char *)NULL);
		}
	}else
	if(strOption == "GetOrderInfo")	// 获取订单信息
	{
		CString strOrderInfo = "";
		if(pILicense->GetOrderInfo(strOrderInfo) == License::trOk)
		{
			Tcl_AppendResult(interp, strOrderInfo, (char *)NULL);
		}
	}else
	{
		return TCL_ERROR;
	}

	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:信息提示对话框
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_Plat_MessageBox(ClientData clientData, Tcl_Interp* interp, int argc, const char* argv[])
{
	if(argc < 2)
	{
		interp->result = "wrong # args: should be \"MessageBox ?-option? ?-title? message\"";
		return TCL_ERROR;
	}

	CString strTitle = _T("Platform");
	CString strMsg;
	UINT uType = 0;
	for(int i=1; i<argc; i++)
	{
		CString strParam = ConvertUTFParam(argv[i]);
		if((strParam.GetLength() > 0) && (strParam[0] == '-'))
			strParam.Delete(0, 1);
		strParam.MakeUpper();

		if(strParam == "TITLE")	// 标题
		{
			if((argc-i) < 2)
			{
				interp->result = "wrong # args: lost title param";
				return TCL_ERROR;
			}
			strTitle = ConvertUTFParam(argv[i+1]);
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
			strMsg = ConvertUTFParam(argv[i]);
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
	sprintf(interp->result, "%d", nRet);	// 返回值
	// IDYES	= 7
	// IDNO		= 6

	ConvertResultToUTF(interp);
	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:文件对话框
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_Plat_FileDlg(ClientData clientData, Tcl_Interp* interp, int argc, const char* argv[])
{
	if(argc < 2)
	{
		interp->result = "wrong # args: should be \"FileDlg ?-option?...";
		return TCL_ERROR;
	}

	BOOL bOpen = TRUE;
	CString strExt = "";
	CString strFileName = "";
	CString strFilter = "";
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

	// 参数解析
	for(int i=1; i<argc; i++)
	{
		CString strParam = ConvertUTFParam(argv[i]);
		if((strParam.GetLength() > 0) && (strParam[0] == '-'))
			strParam.Delete(0, 1);
		strParam.MakeUpper();

		if(strParam == "EXT")	// File Ext
		{
			if((argc-i) < 2)
			{
				interp->result = "wrong # args: lost file ext param";
				return TCL_ERROR;
			}
			strExt = ConvertUTFParam(argv[i+1]);
			i++;
		}else
		if(strParam == "NAME")	// File Name
		{
			if((argc-i) < 2)
			{
				interp->result = "wrong # args: lost file name param";
				return TCL_ERROR;
			}
			strFileName = ConvertUTFParam(argv[i+1]);
			i++;
		}else
		if(strParam == "FILTER")	// Filter
		{
			if((argc-i) < 2)
			{
				interp->result = "wrong # args: lost filter param";
				return TCL_ERROR;
			}
			strFilter = ConvertUTFParam(argv[i+1]);
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
			while(pos != NULL)
			{
				strPathName = dlg.GetNextPathName(pos);
				Tcl_AppendElement(interp, strPathName);
			}			
		}else
		{
			sprintf(interp->result, "%s", dlg.GetPathName());	// 返回路径
		}		
	}

	ConvertResultToUTF(interp);
	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:路径选择对话框
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_Plat_PathSelectDlg(ClientData clientData, Tcl_Interp* interp, int argc, const char* argv[])
{
	if(argc < 2)
	{
		interp->result = "wrong # args: should be \"PathSelect title ?InitPath?\"";
		return TCL_ERROR;
	}

	CTclInterp* pTclInterp = GetTclInterp((LPVOID)interp);
	IPlatUI* pIPlatUI = NULL;
	if(pTclInterp)
	{
		pIPlatUI = pTclInterp->pIPlatUI;
	}
	if(pIPlatUI == NULL)
	{
		Tcl_AppendResult(interp, "Get platform interface fail.", (char *) NULL);
		return TCL_ERROR;
	}

	CString strTitle = ConvertUTFParam(argv[1]);
	CString strPath = "";
	if(argc >= 3)
	{
		strPath = ConvertUTFParam(argv[2]);
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

	sprintf(interp->result, "%s", strRet);	// 返回路径
	ConvertResultToUTF(interp);
	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:通用进度对话框
// create ?caption? ?status? ?range? ?cancelscript?	: 创建进度对话框
// setpos pos							: 设置进度
// setstatus msg						: 设置状态信息
// end									: 结束
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_ProgressDlg(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc < 2)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "create|setpos|setstatus|end");
		return TCL_ERROR;
	}

	CTclInterp* pTclInterp = GetTclInterp((LPVOID)interp);
	IPlatUI* pIPlatUI = NULL;
	if(pTclInterp)
	{
		pIPlatUI = pTclInterp->pIPlatUI;
	}
	if(pIPlatUI == NULL)
	{
		Tcl_AppendResult(interp, "Get platform interface fail.", (char *) NULL);
		return TCL_ERROR;
	}

	if(strcmp(Tcl_GetString(objv[1]), "create") == 0)	// 创建进度对话框
	{
		CString strCaption = "";
		CString strStatus = "";
		int nRange = 0;
		CString strCancelScript = "";
		if(objc > 2)
			strCaption = ConvertUTFParam(Tcl_GetString(objv[2]));
		if(objc > 3)
			strStatus = ConvertUTFParam(Tcl_GetString(objv[3]));
		if(objc > 4)
			nRange = atoi(ConvertUTFParam(Tcl_GetString(objv[4])));
		if(objc > 5)
			strCancelScript = ConvertUTFParam(Tcl_GetString(objv[5]));
		if(strStatus != "")
		{
			strCaption += "\n";
			strCaption += strStatus;
		}
		int nRet;
		//RCOM_CMD(0, OC_PROGDLG_CREATE, (WPARAM)(LPCTSTR)strCaption, nRange, &nRet);
		pIPlatUI->SendOwmCmd("", OC_PROGDLG_CREATE, (WPARAM)(LPCTSTR)strCaption, (LPARAM)nRange, &nRet);
		CString strRet;
		strRet.Format("%d", nRet);
		Tcl_AppendResult(interp, strRet,  (char *) NULL);
		if(strCancelScript != "")
		{
			//COM_CMD(0, OC_PROGDLG_CANCELSCRIPT, (WPARAM)(LPCTSTR)strCancelScript, 0);
			pIPlatUI->SendOwmCmd("", OC_PROGDLG_CANCELSCRIPT, (WPARAM)(LPCTSTR)strCancelScript, 0, NULL);
		}
	}else
	if(strcmp(Tcl_GetString(objv[1]), "setpos") == 0)	// 设置进度
	{
		if(objc < 3)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "setpos pos");
			return TCL_ERROR;
		}
		int nRange = atoi(ConvertUTFParam(Tcl_GetString(objv[2])));
		//COM_CMD(0, OC_PROGDLG_SETPOS, nRange, 0);
		pIPlatUI->SendOwmCmd("", OC_PROGDLG_SETPOS, nRange, 0, NULL);
	}else
	if(strcmp(Tcl_GetString(objv[1]), "setstatus") == 0)	// 设置状态信息
	{
		if(objc < 3)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "setstatus msg");
			return TCL_ERROR;
		}
		CString strStatus = ConvertUTFParam(Tcl_GetString(objv[2]));
		//COM_CMD(0, OC_PROGDLG_SETSTATUS, (WPARAM)(LPCTSTR)strStatus, 0);
		pIPlatUI->SendOwmCmd("", OC_PROGDLG_SETSTATUS, (WPARAM)(LPCTSTR)strStatus, 0, NULL);
	}else
	if(strcmp(Tcl_GetString(objv[1]), "end") == 0)	// 结束
	{
		//COM_CMD(0, OC_PROGDLG_END, 0, 0);
		pIPlatUI->SendOwmCmd("", OC_PROGDLG_END, 0, 0, NULL);
	}else
	{
		Tcl_AppendResult(interp, "operater must be create, setpos, setstatus or end.",  (char *) NULL);
		return TCL_ERROR;
	}

	ConvertResultToUTF(interp);
    return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 解析List中的每个项
/////////////////////////////////////////////////////////////////////////////
int ParseListItem(CString strList, CStringArray& asItem)
{
	int nPosStart = 0;
	int nPos = 0;
	while((nPos = strList.Find(" ", 0)) > 0)
	{
		// 如果以"开头，则找到"的结尾，算作一个元素
		if( (strList[0]=='"') || (strList.Find("T\"")==0) ||
			(strList.Find("E\"")==0) || (strList.Find("L\"")==0) )
		{
			int nPos1 = 1;
			if(strList[0] != '"')
				nPos1 = 2;
			while((nPos1<strList.GetLength()) && (strList[nPos1]!='"'))
			{
				nPos1++;
			}
			if(nPos1 < strList.GetLength())
			{
				nPos = nPos1+1;
			}
		}
		asItem.Add(strList.Left(nPos));
		strList.Delete(0, nPos);
		strList.TrimLeft();
	}

	if(strList.GetLength() > 0)
	{
		asItem.Add(strList);
	}

	return asItem.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
// 解析括号中的内容
/////////////////////////////////////////////////////////////////////////////
CString ParseParenthesis(CString strItem)
{
	int nPos = strItem.Find("(");
	if(nPos >= 0)
		strItem.Delete(0, nPos+1);
	nPos = strItem.ReverseFind(')');
	if(nPos >= 0)
		strItem = strItem.Left(nPos);
	return strItem;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:属性树对话框,未完成
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_Plat_OptionTreeDlg(ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc < 2)
	{
		interp->result = "wrong # args: should be \"OptionTree optionList\"";
		return TCL_ERROR;
	}

	CTclInterp* pTclInterp = GetTclInterp((LPVOID)interp);
	IPlatUI* pIPlatUI = NULL;
	if(pTclInterp)
	{
		pIPlatUI = pTclInterp->pIPlatUI;
	}
	if(pIPlatUI == NULL)
	{
		Tcl_AppendResult(interp, "Get platform interface fail.", (char *) NULL);
		return TCL_ERROR;
	}

	int nListCount = 0;
	Tcl_Obj** paList;
	if(TCL_ERROR == Tcl_ListObjGetElements(interp, objv[1], &nListCount, &paList))
	{
		return TCL_ERROR;
	}

	if(0 == nListCount)
	{
		return TCL_OK;
	}

	CTOptArray aopt;
	TOPTITEM oi;
	oi.hItem	= NULL;
	oi.iParent	= -1;
	for(int i=0; i<nListCount; i++)
	{
		// 初始化项
		oi.iType	= TOPT_ITEM;
		oi.bCheck	= FALSE;
		oi.iImage	= TOPTI_INFO;
		if(i == 0)
			oi.iParent	= -1;
		else
			oi.iParent	= 0;
		oi.lpszText	= "";
		oi.lpszEdit	= "";
		oi.lpszList	= "";
		oi.strTclVar = "";

		// 参数解析
		Tcl_Obj* list = (paList[i]);
		CString strList = ConvertUTFParam(list->bytes);
		CStringArray asItem;
		int nSubCount = ParseListItem(strList, asItem);
		for(int j=0; j<nSubCount; j++)
		{
			CString strItem = asItem[j];
			if(strItem[0] == '-')	// 类型、图标等参数
			{
				strItem.Delete(0, 1);
				//strItem.MakeUpper();
				if(strItem[0] == 'P')
				{
					strItem.Delete(0, 1);
					strItem = ParseParenthesis(strItem);
					oi.iParent	= atoi(strItem);
				}else
				if(strItem == "CHECK")	// 设置检查标记
				{
					oi.bCheck	= TRUE;
				}else
				if(strItem == "OPT_MSG")	// 设置标题和信息
				{
					oi.iType	= TOPT_MSG;
				}else
				if(strItem == "OPT_HELP")	// 设置帮助文件
				{
					oi.iType	= TOPT_HELP;
				}else
				if(strItem == "OPT_ITEM")	// 添加项
				{
					oi.iType	= TOPT_ITEM;
				}else
				if(strItem == "OPT_GROUP")	// 添加组
				{
					oi.iType	= TOPT_GROUP;
				}else
				if(strItem.Find("OPT_CHECKBOX") == 0)	// 添加检查框
				{
					oi.iType	= TOPT_CHECKBOX;
					strItem.Delete(0, 12);
					strItem = ParseParenthesis(strItem);
					oi.strTclVar = strItem;
					CString strValue = Tcl_GetVar(interp, strItem.GetBuffer(strItem.GetLength()), 0);
					strItem.ReleaseBuffer();
					oi.bCheck = (strValue=="1");
				}else
				if(strItem.Find("OPT_RADIOBUTTON") == 0)	// 添加广播按钮
				{
					oi.iType	= TOPT_RADIOBUTTON;
					strItem.Delete(0, 15);
					strItem = ParseParenthesis(strItem);
					oi.strTclVar = strItem;
					CString strValue = Tcl_GetVar(interp, strItem.GetBuffer(strItem.GetLength()), 0);
					strItem.ReleaseBuffer();
					oi.bCheck = (strValue=="1");
				}else
				if(strItem.Find("OPT_EDIT") == 0)	// 添加编辑框
				{
					oi.iType	= TOPT_EDIT;
					strItem.Delete(0, 8);
					strItem = ParseParenthesis(strItem);
					oi.strTclVar = strItem;
					CString strValue = Tcl_GetVar(interp, strItem.GetBuffer(strItem.GetLength()), 0);
					strItem.ReleaseBuffer();
					oi.lpszEdit	= strValue;
				}else
				if(strItem.Find("OPT_COMBO") == 0)	// 添加组合框
				{
					oi.iType	= TOPT_COMBO;
					strItem.Delete(0, 9);
					strItem = ParseParenthesis(strItem);
					oi.strTclVar = strItem;
					CString strValue = Tcl_GetVar(interp, strItem.GetBuffer(strItem.GetLength()), 0);
					strItem.ReleaseBuffer();
					oi.lpszEdit	= strValue;
				}else
				if(strItem.Find("OPT_FONT") == 0)	// 选择字体
				{
				}else
				if(strItem.Find("OPT_TIME") == 0)	// 日期时间输入
				{
					oi.iType	= TOPT_TIME;
					strItem.Delete(0, 8);
					strItem = ParseParenthesis(strItem);
					oi.strTclVar = strItem;
					CString strValue = Tcl_GetVar(interp, strItem.GetBuffer(strItem.GetLength()), 0);
					strItem.ReleaseBuffer();
					oi.lpszEdit	= strValue;
				}else
				if(strItem.Find("OPT_COLOR") == 0)	// 选择颜色
				{
				}else
				if(strItem.Find("OPT_CHOOSEDIR") == 0)	// 选择路径
				{
				}else
				if(strItem.Find("OPT_CHOOSEFILE") == 0)	// 文件输入
				{
				}else
				if(strItem == "ICO_FOLDER")	// 以下为设置图标
				{
					oi.iImage	= TOPTI_FOLDER;
				}else
				if(strItem == "ICO_GROUP")
				{
					oi.iImage	= TOPTI_GROUP;
				}else
				if(strItem == "ICO_EDIT")
				{
					oi.iImage	= TOPTI_EDIT;
				}else
				if(strItem == "ICO_COMBO")
				{
					oi.iImage	= TOPTI_COMBO;
				}else
				if(strItem == "ICO_MFOLDER")
				{
					oi.iImage	= TOPTI_M_FOLDER;
				}else
				if(strItem == "ICO_MGROUP")
				{
					oi.iImage	= TOPTI_M_GROUP;
				}else
				if(strItem == "ICO_MEDIT")
				{
					oi.iImage	= TOPTI_M_EDIT;
				}else
				if(strItem == "ICO_MCOMBO")
				{
					oi.iImage	= TOPTI_M_COMBO;
				}else
				if(strItem == "ICO_INFO")
				{
					oi.iImage	= TOPTI_INFO;
				}else
				if(strItem == "ICO_PROJECT")
				{
					oi.iImage	= TOPTI_PROJECT;
				}else
				if(strItem == "ICO_FILE")
				{
					oi.iImage	= TOPTI_FILE;
				}else
				if(strItem == "ICO_VERSION")
				{
					oi.iImage	= TOPTI_VERSION;
				}else
				if(strItem == "ICO_INTERFACE")
				{
					oi.iImage	= TOPTI_INTERFACE;
				}else
				if(strItem == "ICO_ITEM")
				{
					oi.iImage	= TOPTI_ITEM;
				}else
				if(strItem == "ICO_HELP")
				{
					oi.iImage	= TOPTI_HELP;
				}else
				if(strItem == "ICO_BOARD")
				{
					oi.iImage	= TOPTI_BOARD;
				}else
				if(strItem == "ICO_WORNING")
				{
					oi.iImage	= TOPTI_WORNING;
				}else
				if(strItem == "ICO_CONNECT")
				{
					oi.iImage	= TOPTI_CONNECT;
				}else
				if(strItem == "ICO_USER")
				{
					oi.iImage	= TOPTI_USER;
				}else
				if(strItem == "ICO_FONT")
				{
					oi.iImage	= TOPTI_FONT;
				}else
				if(strItem == "ICO_TIME")
				{
					oi.iImage	= TOPTI_TIME;
				}else
				if(strItem == "ICO_COLOR")
				{
					oi.iImage	= TOPTI_COLOR;
				}
			}else
			if(strItem.Find("T\"")==0)	// Text
			{
				strItem.Delete(0, 2);
				strItem.Delete(strItem.GetLength()-1, 1);
				oi.lpszText	= strItem;
			}else
			if(strItem.Find("E\"")==0)	// Edit
			{
				strItem.Delete(0, 2);
				strItem.Delete(strItem.GetLength()-1, 1);
				oi.lpszEdit	= strItem;
			}else
			if(strItem.Find("L\"")==0)	// List
			{
				strItem.Delete(0, 2);
				strItem.Delete(strItem.GetLength()-1, 1);
				while(strItem.Find("\\n") >= 0)
				{
					strItem.Replace("\\n", "\n");
				}
				oi.lpszList	= strItem;
			}
		}

		aopt.Add(oi);
	}

	// 执行OptionTree对话框
/*	CTreeOptionsDlg option;
	option.SetTOptArrayPoint(&aopt);
	if(option.DoModal() == IDOK)
	{
		sprintf(interp->result, "1");	// 返回值
	}
*/
	int nRet = FALSE;

	try{
	//RCOM_CMD(-1, OC_EXECTREEOPTION, 0, (LPARAM)(&aopt), &nRet);
	pIPlatUI->SendOwmCmd("", OC_EXECTREEOPTION, 0, (LPARAM)(&aopt), &nRet);
	}catch(...)
	{
		Tcl_ResetResult(interp);
		Tcl_AppendResult(interp, "Catch a unknown exception while execute OptionTree command.", (char *) NULL);
		return TCL_ERROR;
	}

	if(nRet == IDOK)
	{
		// 设置各个项的TCL变量
		for(int j=0; j<nListCount; j++)
		{
			// 变量名不为空表示需要设置
			if(aopt[j].strTclVar != "")
			{
				CString strVar = aopt[j].strTclVar;
				CString strValue = "";

				if(TOPT_CHECKBOX == aopt[j].iType)
				{
					strValue = aopt[j].bCheck ? "1" : "0";
				}else
				if(TOPT_RADIOBUTTON == aopt[j].iType)
				{
					strValue = aopt[j].bCheck ? "1" : "0";
				}else
				if(TOPT_EDIT == aopt[j].iType)
				{
					strValue = aopt[j].lpszEdit;
				}else
				if(TOPT_COMBO == aopt[j].iType)
				{
					strValue = aopt[j].lpszEdit;
				}else
				if(TOPT_TIME == aopt[j].iType)
				{
					strValue = aopt[j].lpszEdit;
				}

				strVar = ConvertStringToUTF(strVar);
				strValue = ConvertStringToUTF(strValue);
				char* szVar = strVar.GetBuffer(strVar.GetLength());
				char* szValue = strValue.GetBuffer(strValue.GetLength());
				Tcl_SetVar(interp, szVar, szValue, TCL_LEAVE_ERR_MSG);
				strVar.ReleaseBuffer();
				strValue.ReleaseBuffer();
			}
		}

		int nSel = -1;
		if(nListCount > 0)
		{
			nSel = (int)(aopt[0].hItem);
		}
		CString strSel;
		strSel.Format("%d", nSel);
		sprintf(interp->result, strSel);	// 返回值
	}else
	{
		sprintf(interp->result, "-1");	// 返回值
	}

	ConvertResultToUTF(interp);
	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:通用登录对话框
// -title text							: 设置对话框标题
// -info text							: 设置信息内容
// -laddress label						: 设置地址标签
// -lusername label						: 设置用户名标签
// -lpassword label						: 设置口令标签
// -ldomain label						: 设置域标签
// -address var default					: 地址输入
// -username var default				: 用户名输入
// -password var default				: 口令输入
// -domain var default					: 域输入
// -domainlist list						: 域列表
// -check var text default(true|false)	: 检查框输入
// -focus inputcontrol					: 设置初始输入框焦点
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_tclLogin(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	CTclInterp* pTclInterp = GetTclInterp((LPVOID)interp);
	IPlatUI* pIPlatUI = NULL;
	if(pTclInterp)
	{
		pIPlatUI = pTclInterp->pIPlatUI;
	}
	if(pIPlatUI == NULL)
	{
		Tcl_AppendResult(interp, "Get platform interface fail.", (char *) NULL);
		return TCL_ERROR;
	}

	TCommLoginInfo LoginInfo;

	for(int i=1; i<objc; i++)
	{
		CString optionName = Tcl_GetString(objv[i]);

		if(strcmp(optionName, "-title") == 0)	// 标题
		{			
			i++;
			if(objc <= i)
			{
				Tcl_WrongNumArgs(interp, 1, objv, "-title title");
				return TCL_ERROR;
			}
			CString strDefault = ConvertUTFParam(Tcl_GetString(objv[i]));
			LoginInfo.strTitle = strDefault;
		}else
		if(strcmp(optionName, "-info") == 0)	// 信息
		{			
			i++;
			if(objc <= i)
			{
				Tcl_WrongNumArgs(interp, 1, objv, "-info info");
				return TCL_ERROR;
			}
			CString strDefault = ConvertUTFParam(Tcl_GetString(objv[i]));
			LoginInfo.strInfo = strDefault;
		}else
		if(strcmp(optionName, "-laddress") == 0)	// 设置地址输入标签
		{			
			i++;
			if(objc <= i)
			{
				Tcl_WrongNumArgs(interp, 1, objv, "-laddress label");
				return TCL_ERROR;
			}
			LoginInfo.strLAddress = ConvertUTFParam(Tcl_GetString(objv[i]));
		}else
		if(strcmp(optionName, "-lusername") == 0)	// 设置用户名输入标签
		{			
			i++;
			if(objc <= i)
			{
				Tcl_WrongNumArgs(interp, 1, objv, "-lusername label");
				return TCL_ERROR;
			}
			LoginInfo.strLUserName = ConvertUTFParam(Tcl_GetString(objv[i]));
		}else
		if(strcmp(optionName, "-lpassword") == 0)	// 设置口令输入标签
		{			
			i++;
			if(objc <= i)
			{
				Tcl_WrongNumArgs(interp, 1, objv, "-lpassword label");
				return TCL_ERROR;
			}
			LoginInfo.strLPassword = ConvertUTFParam(Tcl_GetString(objv[i]));
		}else
		if(strcmp(optionName, "-ldomain") == 0)	// 设置域输入标签
		{			
			i++;
			if(objc <= i)
			{
				Tcl_WrongNumArgs(interp, 1, objv, "-ldomain label");
				return TCL_ERROR;
			}
			LoginInfo.strLDomain = ConvertUTFParam(Tcl_GetString(objv[i]));
		}else
		if(strcmp(optionName, "-address") == 0)	// 登录地址
		{			
			if((objc+2) <= i)
			{
				Tcl_WrongNumArgs(interp, 1, objv, "-address var default");
				return TCL_ERROR;
			}
			CString valueName = ConvertUTFParam(Tcl_GetString(objv[i+1]));
			CString strDefault = ConvertUTFParam(Tcl_GetString(objv[i+2]));
			i += 2;
			LoginInfo.strAddress = strDefault;
			if(valueName != "disable")
			{
				LoginInfo.bUseAddress = TRUE;
				LoginInfo.varAddress = valueName;
			}
		}else
		if(strcmp(optionName, "-username") == 0)	// 用户名
		{			
			if((objc+2) <= i)
			{
				Tcl_WrongNumArgs(interp, 1, objv, "-username var default");
				return TCL_ERROR;
			}
			CString valueName = ConvertUTFParam(Tcl_GetString(objv[i+1]));
			CString strDefault = ConvertUTFParam(Tcl_GetString(objv[i+2]));
			i += 2;
			LoginInfo.strUserName = strDefault;
			if(valueName != "disable")
			{
				LoginInfo.bUseUserName = TRUE;
				LoginInfo.varUserName = valueName;
			}
		}else
		if(strcmp(optionName, "-password") == 0)	// 口令
		{			
			if((objc+2) <= i)
			{
				Tcl_WrongNumArgs(interp, 1, objv, "-password var default");
				return TCL_ERROR;
			}
			CString valueName = ConvertUTFParam(Tcl_GetString(objv[i+1]));
			CString strDefault = ConvertUTFParam(Tcl_GetString(objv[i+2]));
			i += 2;
			LoginInfo.strPassword = strDefault;
			if(valueName != "disable")
			{
				LoginInfo.bUsePassword = TRUE;
				LoginInfo.varPassword = valueName;
			}
		}else
		if(strcmp(optionName, "-domain") == 0)	// 登录域
		{			
			if((objc+2) <= i)
			{
				Tcl_WrongNumArgs(interp, 1, objv, "-domain var default");
				return TCL_ERROR;
			}
			CString valueName = ConvertUTFParam(Tcl_GetString(objv[i+1]));
			CString strDefault = ConvertUTFParam(Tcl_GetString(objv[i+2]));
			i += 2;
			LoginInfo.strDomain = strDefault;
			if(valueName != "disable")
			{
				LoginInfo.bUseDomain = TRUE;
				LoginInfo.varDomain = valueName;
			}
		}else
		if(strcmp(optionName, "-domainlist") == 0)	// 域列表
		{
			i++;
			if(objc <= i)
			{
				Tcl_WrongNumArgs(interp, 1, objv, "-domainlist domains");
				return TCL_ERROR;
			}
			CString strDefault = ConvertUTFParam(Tcl_GetString(objv[i]));
			LoginInfo.strDomainList = strDefault;
		}else
		if(strcmp(optionName, "-check") == 0)	// 检查框
		{
			if((objc+3) <= i)
			{
				Tcl_WrongNumArgs(interp, 1, objv, "-check var text default");
				return TCL_ERROR;
			}
			CString valueName = ConvertUTFParam(Tcl_GetString(objv[i+1]));
			CString strCheckName = ConvertUTFParam(Tcl_GetString(objv[i+2]));
			CString strDefault = ConvertUTFParam(Tcl_GetString(objv[i+3]));
			i += 3;
			LoginInfo.strCheck = strCheckName;
			LoginInfo.varCheck = valueName;
			if(strDefault == "true")
			{
				LoginInfo.bCheck = TRUE;
			}
		}else
		if(strcmp(optionName, "-focus") == 0)	// 缺省焦点位置
		{
			i++;
			if(objc <= i)
			{
				Tcl_WrongNumArgs(interp, 1, objv, "-focus inputcontrol");
				return TCL_ERROR;
			}
			CString strFocus = ConvertUTFParam(Tcl_GetString(objv[i]));
			LoginInfo.strFocus = strFocus;
		}else
		{
			Tcl_WrongNumArgs(interp, 1, objv, "?-optionName? ?value? ?default?...");
			return TCL_ERROR;
		}
	}

	int nRet = 0;
	//RCOM_CMD(0, OC_LOGIN_DIALOG, WPARAM(&LoginInfo), 0, &nRet);
	pIPlatUI->SendOwmCmd("", OC_LOGIN_DIALOG, WPARAM(&LoginInfo), 0, &nRet);

	if(nRet)	// 确认输入，给TCL变量赋值
	{
		if(LoginInfo.bUseAddress)
		{
			char* varAddress = LoginInfo.varAddress.GetBuffer(LoginInfo.varAddress.GetLength());
			char* strAddress = LoginInfo.strAddress.GetBuffer(LoginInfo.strAddress.GetLength());
			Tcl_SetVar(interp, varAddress, strAddress, TCL_LEAVE_ERR_MSG);
			LoginInfo.varAddress.ReleaseBuffer();
			LoginInfo.strAddress.ReleaseBuffer();
		}
		if(LoginInfo.bUseUserName)
		{
			char* varUserName = LoginInfo.varUserName.GetBuffer(LoginInfo.varUserName.GetLength());
			char* strUserName = LoginInfo.strUserName.GetBuffer(LoginInfo.strUserName.GetLength());
			Tcl_SetVar(interp, varUserName, strUserName, TCL_LEAVE_ERR_MSG);
			LoginInfo.varUserName.ReleaseBuffer();
			LoginInfo.strUserName.ReleaseBuffer();
		}
		if(LoginInfo.bUsePassword)
		{
			char* varPassword = LoginInfo.varPassword.GetBuffer(LoginInfo.varPassword.GetLength());
			char* strPassword = LoginInfo.strPassword.GetBuffer(LoginInfo.strPassword.GetLength());
			Tcl_SetVar(interp, varPassword, strPassword, TCL_LEAVE_ERR_MSG);
			LoginInfo.varPassword.ReleaseBuffer();
			LoginInfo.strPassword.ReleaseBuffer();
		}
		if(LoginInfo.bUseDomain)
		{
			char* varDomain = LoginInfo.varDomain.GetBuffer(LoginInfo.varDomain.GetLength());
			char* strDomain = LoginInfo.strDomain.GetBuffer(LoginInfo.strDomain.GetLength());
			Tcl_SetVar(interp, varDomain, strDomain, TCL_LEAVE_ERR_MSG);
			LoginInfo.varDomain.ReleaseBuffer();
			LoginInfo.strDomain.ReleaseBuffer();
		}
		if(LoginInfo.strCheck != "")
		{
			char* varCheck = LoginInfo.varCheck.GetBuffer(LoginInfo.varCheck.GetLength());
			Tcl_SetVar(interp, varCheck, LoginInfo.bCheck ? "1" : "0", TCL_LEAVE_ERR_MSG);
			LoginInfo.varCheck.ReleaseBuffer();
		}
	}

	sprintf(interp->result, "%d", nRet);	// 返回值
	ConvertResultToUTF(interp);

    return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:平台输出窗口操作
// -format	: 格式化控制
// -create	: 创建输出窗口
// -close	: 关闭输出窗口
// -active	: 激活输出窗口
// -clear	: 清空输出窗口
// -getinfo	: 获取信息
// -id|name	: 输出
// -direct	: 直接输出,无格式化
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_Output(ClientData clientData, Tcl_Interp* interp, int argc, const char* argv[])
{
	if(argc < 2)
	{
		interp->result = "wrong # args: should be \"output ?-option? ?-id|title?...?-direct?\"";
		return TCL_ERROR;
	}

	CTclInterp* pTclInterp = GetTclInterp((LPVOID)interp);
	IPlatUI* pIPlatUI = NULL;
	if(pTclInterp)
	{
		pIPlatUI = pTclInterp->pIPlatUI;
	}
	if(pIPlatUI == NULL)
	{
		Tcl_AppendResult(interp, "Get platform interface fail.", (char *) NULL);
		return TCL_ERROR;
	}

	int nOT = 2;
	CString strOT;

	if(argc == 2)	// 只有一个参数,表示输出到Trace窗口
	{
		pIPlatUI->Output(nOT, ConvertUTFParam(argv[1]));
	}else	// 有两个参数,参数1表示输出窗口名或ID
	{
		CString strOption = ConvertUTFParam(argv[1]);
		if((strOption.GetLength() > 0) && (strOption[0] == '-'))
		{
			strOption.Delete(0, 1);
			strOption.MakeUpper();
			if(strOption == "FORMAT")	// format -id formatstr
			{
				if(argc < 4)
				{
					interp->result = "wrong # args: should be \"output -format -id|title FormatStr\"";
					return TCL_ERROR;
				}

				strOT = ConvertUTFParam(argv[2]);
				CString strSet = ConvertUTFParam(argv[3]);
				if(strOT[0] == '-')
				{	// 使用窗口ID作为参数
					strOT.Delete(0, 1);
					nOT = atoi(strOT);
				}else
				{
					nOT = atoi(pIPlatUI->OutputGet(-1, strOT));
				}
				pIPlatUI->OutputSet(nOT, strSet);
			}else
			if(strOption == "CREATE")	// create title ?id?
			{
				if(argc < 3)
				{
					interp->result = "wrong # args: should be \"output -create title ?-id?\"";
					return TCL_ERROR;
				}

				int nID = 0;	// 缺省是自动分配ID
				if(argc > 3)
				{	// 有两个参数，窗口名和指定的ID
					strOT = ConvertUTFParam(argv[3]);
					if(strOT[0] == '-')
					{
						strOT.Delete(0, 1);
						nID = atoi(strOT);
					}
				}
				CString strTitle = ConvertUTFParam(argv[2]);
				nID = pIPlatUI->AddOutput(strTitle);

				sprintf(interp->result, "%d", nID);	// 返回值
			}else
			if(strOption == "CLOSE")
			{
				if(argc < 3)
				{
					interp->result = "wrong # args: should be \"output -close -id|title\"";
					return TCL_ERROR;
				}

				strOT = ConvertUTFParam(argv[2]);
				if(strOT[0] == '-')
				{	// 使用窗口ID作为参数
					strOT.Delete(0, 1);
					nOT = atoi(strOT);
				}else
				{
					nOT = atoi(pIPlatUI->OutputGet(-1, strOT));
				}
				pIPlatUI->DelOutput(nOT);
			}else
			if(strOption == "ACTIVE")
			{
				if(argc < 3)
				{
					interp->result = "wrong # args: should be \"output -active -id|title\"";
					return TCL_ERROR;
				}

				strOT = ConvertUTFParam(argv[2]);
				if(strOT[0] == '-')
				{	// 使用窗口ID作为参数
					strOT.Delete(0, 1);
					nOT = atoi(strOT);
				}else
				{
					nOT = atoi(pIPlatUI->OutputGet(-1, strOT));
				}
				pIPlatUI->ActiveOutput(nOT);
			}else
			if(strOption == "CLEAR")
			{
				if(argc < 3)
				{
					interp->result = "wrong # args: should be \"output -clear -id|title\"";
					return TCL_ERROR;
				}

				strOT = ConvertUTFParam(argv[2]);
				if(strOT[0] == '-')
				{	// 使用窗口ID作为参数
					strOT.Delete(0, 1);
					nOT = atoi(strOT);
				}else
				{
					nOT = atoi(pIPlatUI->OutputGet(-1, strOT));
				}
				pIPlatUI->OutputClear(nOT);
			}else
			if(strOption == "GETINFO")
			{
				if(argc < 4)
				{
					interp->result = "wrong # args: should be \"output -getinfo -id|title param\"";
					return TCL_ERROR;
				}

				strOT = ConvertUTFParam(argv[2]);
				if(strOT[0] == '-')
				{	// 使用窗口ID作为参数
					strOT.Delete(0, 1);
					nOT = atoi(strOT);
				}else
				{
					nOT = atoi(pIPlatUI->OutputGet(-1, strOT));
				}

				CString strParam = ConvertUTFParam(argv[3]);
				CString strInfo = pIPlatUI->OutputGet(nOT, strParam);
				if(strParam == "window-id")
				{
					strInfo.Format("%d", nOT);
				}

				sprintf(interp->result, "%s", strInfo);
			}else
			{	// 窗口ID
				strOT = strOption;
				pIPlatUI->Output(atoi(strOT), ConvertUTFParam(argv[2]));
			}
		}else
		{
			nOT = atoi(pIPlatUI->OutputGet(-1, strOption));
			pIPlatUI->Output(nOT, ConvertUTFParam(argv[2]));
		}
	}

	ConvertResultToUTF(interp);
	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:状态行Pane操作
// -createpane	: 创建Pane
// -delpane		: 删除Pane
// -settext		: 设置Pane文本
// -setwidth	: 设置Pane宽度
// -seticon		: 设置Pane图标
// -setbutton	: 设置Pane按钮
// -settooltip	: 设置Pane提示信息
// -progress	: 进度条操作
/////////////////////////////////////////////////////////////////////////////
static int g_nPaneID = 700;	// Status Pane的起始分配ID
int Tcl_Cmd_StatusBar(ClientData clientData, Tcl_Interp* interp, int argc, const char* argv[])
{
	if(argc < 2)
	{
		interp->result = "wrong # args: should be \"statusbar ?-option? ...\"";
		return TCL_ERROR;
	}

	CTclInterp* pTclInterp = GetTclInterp((LPVOID)interp);
	IPlatUI* pIPlatUI = NULL;
	if(pTclInterp)
	{
		pIPlatUI = pTclInterp->pIPlatUI;
	}
	if(pIPlatUI == NULL)
	{
		Tcl_AppendResult(interp, "Get platform interface fail.", (char *) NULL);
		return TCL_ERROR;
	}

	CString strOption = ConvertUTFParam(argv[1]);
	if(strOption[0] == '-')
		strOption.Delete(0, 1);
	strOption.MakeUpper();
	if(strOption == "CREATEPANE")	// 创建Pane
	{
		int nWidth = 100;
		int nID = 0;

		if(argc < 3)
		{
			interp->result = "wrong # args: should be \"statusbar -createpane ?-width width? ?-id id?\"";
			return TCL_ERROR;
		}

		for (int i = 3; i < argc; i += 2)
		{
			CString optionName = ConvertUTFParam(argv[i-1]);
			optionName.MakeUpper();
			if(optionName == "-WIDTH")	// 参数1:宽度
				nWidth = atoi(ConvertUTFParam(argv[i]));
			else if(optionName == "-ID")	// 参数2:指定的ID
				nID = atoi(ConvertUTFParam(argv[i]));
		}

		//if((nID>10) && (nID<100))
		if(nID != 0)	// 取消自定义ID的范围限制,因为Button类型的Pane范围是在500-549之间
		{
			//COM_CMD(-1, OC_ADDPANE, nID, nWidth);
			pIPlatUI->SendOwmCmd("", OC_ADDPANE, nID, nWidth, NULL);
		}else
		{
			//COM_CMD(-1, OC_ADDPANE, g_nPaneID, nWidth);
			pIPlatUI->SendOwmCmd("", OC_ADDPANE, g_nPaneID, nWidth, NULL);
			nID = g_nPaneID;
			g_nPaneID++;		
		}

		sprintf(interp->result, "%d", nID);	// 返回值
		// 注：Status Pane的ID小于500表示是平台保留的,大于500
		// 表示是由平台动态分配给用户的
	}else
	if(strOption == "DELPANE")	// 删除Pane
	{
		if(argc < 3)
		{
			interp->result = "wrong # args: should be \"statusbar -delpane ID\"";
			return TCL_ERROR;
		}

		//COM_CMD(-1, OC_REMOVEPANE, atoi(argv[2]), 0);
		pIPlatUI->SendOwmCmd("", OC_REMOVEPANE, atoi(argv[2]), 0, NULL);
	}else
	if(strOption == "SETTEXT")	// 设置Pane文字
	{
		if(argc < 4)
		{
			interp->result = "wrong # args: should be \"statusbar -settext PaneID Text\"";
			return TCL_ERROR;
		}

		CString strText = ConvertUTFParam(argv[3]);
		//COM_CMD(-1, OC_SETPANETEXT, atoi(argv[2]), (LPARAM)(LPCTSTR)strText);
		pIPlatUI->SendOwmCmd("", OC_SETPANETEXT, atoi(argv[2]), (LPARAM)(LPCTSTR)strText, NULL);
	}else
	if(strOption == "SETWIDTH")	// 设置状态行Pane宽度
	{
		if(argc < 4)
		{
			interp->result = "wrong # args: should be \"statusbar -setwidth PaneID Width\"";
			return TCL_ERROR;
		}

		//COM_CMD(-1, OC_SETPANEWIDTH, atoi(argv[2]), atoi(argv[3]));
		pIPlatUI->SendOwmCmd("", OC_SETPANEWIDTH, atoi(argv[2]), atoi(argv[3]), NULL);
	}else
	if(strOption == "SETICON")	// 设置状态行Pane图标
	{
		if(argc < 5)
		{
			interp->result = "wrong # args: should be \"statusbar -seticon PaneID GroupID ImageIndex\"";
			return TCL_ERROR;
		}

		//COM_CMD(-1, OC_SETPANEIMAGE, atoi(argv[2]), atoi(argv[3]));
		pIPlatUI->SendOwmCmd("", OC_SETPANEIMAGE, atoi(argv[2]), atoi(argv[3]), NULL);
		//COM_CMD(-1, OC_SETPANEIMAGEITEM, atoi(argv[2]), atoi(argv[4]));
		pIPlatUI->SendOwmCmd("", OC_SETPANEIMAGEITEM, atoi(argv[2]), atoi(argv[4]), NULL);
	}else
	if(strOption == "SETBUTTON")	// 设置状态行Pane按钮
	{
		if(argc < 4)
		{
			interp->result = "wrong # args: should be \"statusbar -setbutton PaneID action\"";
			return TCL_ERROR;
		}

		CString strAction = ConvertUTFParam(argv[3]);

		CExtMenuInfo extMenuInfo;
		extMenuInfo.nId = atoi(argv[2]);
		extMenuInfo.m_strAction = strAction;
			
		pIPlatUI->SendOwmCmd("", OC_SETPANEBUTTON, atoi(argv[2]), LPARAM(&extMenuInfo), NULL);
	}else
	if(strOption == "SETTOOLTIP")	// 设置状态行Pane提示信息
	{
		if(argc < 4)
		{
			interp->result = "wrong # args: should be \"statusbar -settooltip PaneID tipinfo\"";
			return TCL_ERROR;
		}

		CString strTip = ConvertUTFParam(argv[3]);
		//COM_CMD(-1, OC_SETPANETOOLTIP, atoi(argv[2]), (LPARAM)(LPCTSTR)strTip);
		pIPlatUI->SendOwmCmd("", OC_SETPANETOOLTIP, atoi(argv[2]), (LPARAM)(LPCTSTR)strTip, NULL);
	}else
	if(strOption == "PROGRESS")
	{
		if(argc < 4)
		{
			interp->result = "wrong # args: should be \"statusbar -progress -operator -ID ...\"";
			return TCL_ERROR;
		}

		// 解析操作项参数
		CString strOperator = ConvertUTFParam(argv[2]);
		if(strOperator[0] == '-')
			strOperator.Delete(0, 1);
		strOperator.MakeUpper();

		// 解析Pane ID参数
		CString strID = ConvertUTFParam(argv[3]);
		if(strID[0] == '-')
			strID.Delete(0, 1);
		int nPaneID = atoi(strID);

		// 处理具体的进度条操作
		if(strOperator == "ATTACH")	// 和某个Pane建立关联
		{
			CString strMsg = "";
			int nMaxValue = 100;
			if(argc > 4)			// Pane信息参数
				strMsg = ConvertUTFParam(argv[4]);
			if(argc > 5)			// 进度条最大值参数
				nMaxValue = atoi(ConvertUTFParam(argv[5]));
			//COM_CMD(-1, OC_ATTACHPANEPROGRESS, MAKELONG((WORD)nPaneID, (WORD)nMaxValue), (LPARAM)(LPCTSTR)strMsg);
			pIPlatUI->SendOwmCmd("", OC_ATTACHPANEPROGRESS, MAKELONG((WORD)nPaneID, (WORD)nMaxValue), (LPARAM)(LPCTSTR)strMsg, NULL);
		}else
		if(strOperator == "SHOW")	// 显示进度
		{
			//COM_CMD(-1, OC_SHOWPANEPROGRESS, nPaneID, TRUE);
			pIPlatUI->SendOwmCmd("", OC_SHOWPANEPROGRESS, nPaneID, TRUE, NULL);
		}else
		if(strOperator == "HIDE")	// 隐藏进度
		{
			//COM_CMD(-1, OC_SHOWPANEPROGRESS, nPaneID, FALSE);
			pIPlatUI->SendOwmCmd("", OC_SHOWPANEPROGRESS, nPaneID, FALSE, NULL);
		}else
		if(strOperator == "POS")	// 设置进度
		{
			int nPos = 0;
			if(argc > 4)	// 解析进度参数,缺省为0
			{
				nPos = atoi(argv[4]);
			}
			//COM_CMD(-1, OC_SETPANEPROGRESSPOS, nPaneID, nPos);
			pIPlatUI->SendOwmCmd("", OC_SETPANEPROGRESSPOS, nPaneID, nPos, NULL);
		}
	}else
	{
		Tcl_AppendResult(interp, "bad argument \"", argv[1], "\" : must be CreatePane, DelPane, SetText, SetWidth, SetIcon, SetButton, SetTooltip or Progress", (char *) NULL);
		return TCL_ERROR;
	}

	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:消息方式延时
// pwait time(ms)
// pwait CreateEvent ?id?
// pwait WaitEvent id ?timeout?
// pwait AwakeEvent id
// pwait DeleteEvent id
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_Peek_Wait(ClientData clientData, Tcl_Interp* interp, int argc, const char* argv[])
{
	if(argc < 2)
	{
		interp->result = "wrong # args: should be \"pwait time(ms)\"";
		return TCL_ERROR;
	}

	if(strcmp(argv[1], "CreateEvent") == 0)	// 创建等待对象
	{
		int nID = 0;
		if(argc > 2)
		{
			nID = atoi(argv[2]);
		}

		CWait* pWait = CreateWaitObject(nID);
		if(pWait == NULL)
		{
			Tcl_AppendResult(interp, "Create wait event fail.", (char *) NULL);
			return TCL_ERROR;
		}

		CString strRet;
		strRet.Format("%d", pWait->GetWaitID());
		Tcl_AppendResult(interp, strRet, (char *) NULL);
		return TCL_OK;
	}else
	if(strcmp(argv[1], "WaitEvent") == 0)	// 开始等待
	{
		if(argc < 3)
		{
			interp->result = "wrong # args: should be \"pwait WaitEvent event ?timeout?\"";
			return TCL_ERROR;
		}
		CWait* pWait = GetWaitObject(atoi(argv[2]));
		if(pWait)
		{
			int nTimeOut = 0;
			if(argc > 3)
			{
				nTimeOut = atoi(argv[3]);
			}
			int nRet = pWait->Wait(nTimeOut);
			CString strRet;
			strRet.Format("%d", nRet);
			Tcl_AppendResult(interp, strRet, (char *) NULL);
		}
		return TCL_OK;
	}else
	if(strcmp(argv[1], "AwakeEvent") == 0)	// 唤醒等待对象
	{
		if(argc < 3)
		{
			interp->result = "wrong # args: should be \"pwait AwakeEvent event\"";
			return TCL_ERROR;
		}
		AwakeWaitObject(atoi(argv[2]));
		return TCL_OK;
	}else
	if(strcmp(argv[1], "DeleteEvent") == 0)	// 删除等待对象
	{
		if(argc < 3)
		{
			interp->result = "wrong # args: should be \"pwait DeleteEvent event\"";
			return TCL_ERROR;
		}
		DeleteWaitObject(atoi(argv[2]));
		return TCL_OK;
	}

	// 非以上操作,则直接执行消息方式等待一段时间
	MSG msg;
	DWORD dwTimeOut = atoi(argv[1]);
	DWORD tick = ::GetTickCount();
	while((::GetTickCount()-tick) < dwTimeOut)
	{
		if(::PeekMessage(&msg, NULL, NULL, NULL,PM_REMOVE))
		//if(::GetMessage(&msg, NULL, NULL, NULL))
		{															
			::TranslateMessage(&msg);										
			::DispatchMessage(&msg);
		}
	}

	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:TaskDialog
// TaskDialog create optionList
// TaskDialog refresh optionList
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_Plat_TaskDialog(ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc < 3)
	{
		interp->result = "wrong # args: should be \"TaskDialog create|refresh optionList\"";
		return TCL_ERROR;
	}

	CTclInterp* pTclInterp = GetTclInterp((LPVOID)interp);
	IPlatUI* pIPlatUI = NULL;
	if(pTclInterp)
	{
		pIPlatUI = pTclInterp->pIPlatUI;
	}
	if(pIPlatUI == NULL)
	{
		Tcl_AppendResult(interp, "Get platform interface fail.", (char *) NULL);
		return TCL_ERROR;
	}

	CString strOperate = Tcl_GetString(objv[1]);
	int nOperate = 0;
	if(strOperate == "create")	// 执行TaskDialog
	{
		nOperate = 0;
	}else
	if(strOperate == "refresh")	// 刷新TaskDialog
	{
		nOperate = 1;
	}else
	if(strOperate == "xml")	// 从XML加载并执行TaskDialog
	{
		nOperate = 2;
	}else
	if(strOperate == "clear")	// 删除所有向导页面
	{
		nOperate = 3;
		try{
		pIPlatUI->SendOwmCmd("", OC_TASK_DIALOG, nOperate, 0, 0);
		}catch(...)
		{
			Tcl_ResetResult(interp);
			Tcl_AppendResult(interp, "Catch a unknown exception while execute TaskDialog command.", (char *) NULL);
			return TCL_ERROR;
		}
		return TCL_OK;
	}else
	if(strOperate == "addwizard")	// 添加向导页面
	{
		nOperate = 4;
	}else
	if(strOperate == "dowizard")	// 执行向导
	{
		nOperate = 5;
	}

	int nListCount = 0;
	Tcl_Obj** paList;
	if(TCL_ERROR == Tcl_ListObjGetElements(interp, objv[2], &nListCount, &paList))
	{
		return TCL_ERROR;
	}

	if(0 == nListCount)
	{
		return TCL_OK;
	}

	TTaskDialogInfo info;
	info.pInterp = (IInterp*)(pTclInterp->ExternalQueryInterface());
	CString strVarSelectedRadioButtonId = "";
	CString strVarIsVerificiationChecked = "";
	CString strVarInputBoxText = "";
	for(int i=0; i<nListCount; i++)
	{
		// 参数解析
		Tcl_Obj* list = (paList[i]);

		int nSubCount = 0;
		Tcl_Obj** paSubList;
		if(TCL_ERROR == Tcl_ListObjGetElements(interp, list, &nSubCount, &paSubList))
		{
			continue;
		}

		if(0 == nSubCount)
		{
			continue;
		}

		CStringArray asItem;
		for(int j=0; j<nSubCount; j++)
		{
			asItem.Add(ConvertUTFParam(paSubList[j]->bytes));
		}
		CString strItemName = "";
		CString strItemValue = "";
		CString strItemExplanation = "";
		CString strItemScript = "";
		int nItemID = 0;
		if(nSubCount > 0)
		{
			strItemName = asItem[0];
		}
		if(nSubCount > 1)
		{
			strItemValue = asItem[1];
		}
		if(nSubCount == 3)
		{
			nItemID = atoi(asItem[2]);
		}else
		if(nSubCount > 3)
		{
			strItemExplanation = asItem[2];
			nItemID = atoi(asItem[3]);
		}
		if(nSubCount > 4)
		{
			strItemScript = asItem[4];
		}

		if(strItemName == "WindowTitle")
		{
			info.strWindowTitle = strItemValue;
		}else
		if(strItemName == "VerificationText")
		{
			info.strVerificationText = strItemValue;
		}else
		if(strItemName == "ExpandedControlText")
		{
			info.strExpandedControlText = strItemValue;
		}else
		if(strItemName == "CollapsedControlText")
		{
			info.strCollapsedControlText = strItemValue;
		}else
		if(strItemName == "MainInstruction")
		{
			info.strMainInstruction = strItemValue;
		}else
		if(strItemName == "Content")
		{
			info.strContent = strItemValue;
		}else
		if(strItemName == "ExpandedInformation")
		{
			info.strExpandedInformation = strItemValue;
		}else
		if(strItemName == "Footer")
		{
			info.strFooter = strItemValue;
		}else
		if(strItemName == "MainIcon")
		{
			info.strMainIcon = strItemValue;
		}else
		if(strItemName == "FooterIcon")
		{
			info.strFooterIcon = strItemValue;
		}else
		if(strItemName == "Width")
		{
			info.nWidth = atoi(strItemValue);
		}else
		if(strItemName == "Button")
		{
			if(strItemExplanation == "")
			{
				info.asButtonText.Add(strItemValue);
			}else
			{
				info.asButtonText.Add(strItemValue + "\n" + strItemExplanation);
			}
			info.auButtonID.Add(nItemID);
			info.asButtonScript.Add(strItemScript);
		}else
		if(strItemName == "RadioButton")
		{
			info.asRadioButtonText.Add(strItemValue);
			info.auRadioButtonID.Add(nItemID);
		}else
		if(strItemName == "EnableCommandLinks")
		{
			info.bEnableCommandLinks = (strItemValue == "true");
		}else
		if(strItemName == "EnableHyperlinks")
		{
			info.bEnableHyperlinks = (strItemValue == "true");
		}else
		if(strItemName == "EnableRelativePosition")
		{
			info.bEnableRelativePosition = (strItemValue == "true");
		}else
		if(strItemName == "ShowProgressBar")
		{
			info.bShowProgressBar = (strItemValue == "true");
		}else
		if(strItemName == "ProgressBarPos")
		{
			info.nProgressBarPos = atoi(strItemValue);
		}else
		if(strItemName == "ProgressBarRangeMin")
		{
			info.nProgressBarRangeMin = atoi(strItemValue);
		}else
		if(strItemName == "ProgressBarRangeMax")
		{
			info.nProgressBarRangeMax = atoi(strItemValue);
		}else
		if(strItemName == "Marquee")
		{
			info.bMarquee = (strItemValue == "true");
		}else
		if(strItemName == "ExpandedByDefault")
		{
			info.bExpandedByDefault = (strItemValue == "true");
		}else
		if(strItemName == "ExpandFooterArea")
		{
			info.bExpandFooterArea = (strItemValue == "true");
		}else
		if(strItemName == "DefaultButton")
		{
			info.nDefaultButton = atoi(strItemValue);
		}else
		if(strItemName == "DefaultRadioButton")
		{
			info.nDefaultRadioButton = atoi(strItemValue);
		}else
		if(strItemName == "NoDefaultRadioButton")
		{
			info.bNoDefaultRadioButton = (strItemValue == "true");
		}else
		if(strItemName == "VerifyCheckState")
		{
			info.bVerifyCheckState = (strItemValue == "true");
		}else
		if(strItemName == "EnableCallbackTimer")
		{
			info.bEnableCallbackTimer = (strItemValue == "true");
		}else
		if(strItemName == "TimerTimeout")
		{
			info.nTimerTimeout = atoi(strItemValue);
		}else
		if(strItemName == "TimerScript")
		{
			info.strTimerScript = strItemValue;
		}else
		if(strItemName == "ShowButtonOk")
		{
			info.bCommonButtonOk = (strItemValue == "true");
		}else
		if(strItemName == "ShowButtonYes")
		{
			info.bCommonButtonYes = (strItemValue == "true");
		}else
		if(strItemName == "ShowButtonNo")
		{
			info.bCommonButtonNo = (strItemValue == "true");
		}else
		if(strItemName == "ShowButtonCancel")
		{
			info.bCommonButtonCancel = (strItemValue == "true");
		}else
		if(strItemName == "ShowButtonRetry")
		{
			info.bCommonButtonRetry = (strItemValue == "true");
		}else
		if(strItemName == "ShowButtonClose")
		{
			info.bCommonButtonClose = (strItemValue == "true");
		}else
		if(strItemName == "SelectedRadioButton")
		{
			strVarSelectedRadioButtonId = strItemValue;
		}else
		if(strItemName == "VerificiationChecked")
		{
			strVarIsVerificiationChecked = strItemValue;
		}else
		if(strItemName == "InputBox")
		{
			info.bEnableInputBox = TRUE;
			strVarInputBoxText = strItemValue;
			info.strInputBoxText = asItem[2];
		}else
		if(strItemName == "Replace")
		{
			info.asReplace.Add(strItemValue);
		}
	}

	// 执行TaskDialog对话框
	int nRet = IDCANCEL;

	try{
	//RCOM_CMD(-1, OC_EXECTREEOPTION, 0, (LPARAM)(&aopt), &nRet);
	pIPlatUI->SendOwmCmd("", OC_TASK_DIALOG, nOperate, (LPARAM)(&info), &nRet);
	}catch(...)
	{
		Tcl_ResetResult(interp);
		Tcl_AppendResult(interp, "Catch a unknown exception while execute TaskDialog command.", (char *) NULL);
		return TCL_ERROR;
	}

	if(((nOperate == 0) || (nOperate == 2)) && (nRet != IDCANCEL) && (nRet != IDNO))
	{
		// 设置各个项的TCL变量
		// RadioButton
		CString strVar = strVarSelectedRadioButtonId;
		CString strValue;
		strValue.Format("%d", info.nSelectedRadioButtonId);

		strVar = ConvertStringToUTF(strVar);
		strValue = ConvertStringToUTF(strValue);
		char* szVar = strVar.GetBuffer(strVar.GetLength());
		char* szValue = strValue.GetBuffer(strValue.GetLength());
		Tcl_SetVar(interp, szVar, szValue, TCL_LEAVE_ERR_MSG);
		strVar.ReleaseBuffer();
		strValue.ReleaseBuffer();

		// 检查框
		strVar = strVarIsVerificiationChecked;
		strValue = info.bIsVerificiationChecked ? "true" : "false";

		strVar = ConvertStringToUTF(strVar);
		strValue = ConvertStringToUTF(strValue);
		szVar = strVar.GetBuffer(strVar.GetLength());
		szValue = strValue.GetBuffer(strValue.GetLength());
		Tcl_SetVar(interp, szVar, szValue, TCL_LEAVE_ERR_MSG);
		strVar.ReleaseBuffer();
		strValue.ReleaseBuffer();

		// 输入框
		strVar = strVarInputBoxText;
		strValue = info.strInputBoxText;

		strVar = ConvertStringToUTF(strVar);
		strValue = ConvertStringToUTF(strValue);
		szVar = strVar.GetBuffer(strVar.GetLength());
		szValue = strValue.GetBuffer(strValue.GetLength());
		Tcl_SetVar(interp, szVar, szValue, TCL_LEAVE_ERR_MSG);
		strVar.ReleaseBuffer();
		strValue.ReleaseBuffer();
	}

	CString strRet;
	strRet.Format("%d", nRet);
	sprintf(interp->result, strRet);	// 返回值

	// 前面调用接口查询函数,需要释放
	((IInterp*)(info.pInterp))->Release();

	ConvertResultToUTF(interp);
	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:PopupControl
// PopupControl show optionList
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_Plat_PopupControl(ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc < 3)
	{
		interp->result = "wrong # args: should be \"PopupControl show optionList\"";
		return TCL_ERROR;
	}

	CTclInterp* pTclInterp = GetTclInterp((LPVOID)interp);
	IPlatUI* pIPlatUI = NULL;
	if(pTclInterp)
	{
		pIPlatUI = pTclInterp->pIPlatUI;
	}
	if(pIPlatUI == NULL)
	{
		Tcl_AppendResult(interp, "Get platform interface fail.", (char *) NULL);
		return TCL_ERROR;
	}

	CString strOperate = Tcl_GetString(objv[1]);
	if(strOperate != "show")
	{
		interp->result = "wrong # args: should be \"PopupControl show optionList\"";
		return TCL_ERROR;
	}

	int nListCount = 0;
	Tcl_Obj** paList;
	if(TCL_ERROR == Tcl_ListObjGetElements(interp, objv[2], &nListCount, &paList))
	{
		return TCL_ERROR;
	}

	if(0 == nListCount)
	{
		return TCL_OK;
	}

	TPopupControlInfo info;
	info.pInterp = (IInterp*)(pTclInterp->ExternalQueryInterface());
	CString strVarSelectedRadioButtonId = "";
	CString strVarIsVerificiationChecked = "";
	for(int i=0; i<nListCount; i++)
	{
		// 参数解析
		Tcl_Obj* list = (paList[i]);

		int nSubCount = 0;
		Tcl_Obj** paSubList;
		if(TCL_ERROR == Tcl_ListObjGetElements(interp, list, &nSubCount, &paSubList))
		{
			continue;
		}

		if(0 == nSubCount)
		{
			continue;
		}

		CStringArray asItem;
		for(int j=0; j<nSubCount; j++)
		{
			asItem.Add(ConvertUTFParam(paSubList[j]->bytes));
		}
		CString strItemName = "";
		CString strItemValue = "";
		CString strItemExplanation = "";
		CString strItemScript = "";
		if(nSubCount > 0)
		{
			strItemName = asItem[0];
		}
		if(nSubCount > 1)
		{
			strItemValue = asItem[1];
		}
		if(nSubCount > 2)
		{
			strItemExplanation = asItem[2];
		}
		if(nSubCount > 3)
		{
			strItemScript = asItem[3];
		}

		if(strItemName == "Theme")
		{
			info.strTheme = strItemValue;
		}else
		if(strItemName == "Icon")
		{
			info.strIcon = strItemValue;
		}else
		if(strItemName == "Caption")
		{
			info.strCaption = strItemValue;
		}else
		if(strItemName == "Title")
		{
			info.strTitle = strItemValue;
			info.strLinkTitle = strItemExplanation;
		}else
		if(strItemName == "Text")
		{
			info.strText = strItemValue;
			info.strLinkText = strItemExplanation;
			if(strItemScript == "RTF")
			{
				info.nItemType = POPUP_ITEM_RTF;
			}else
			if(strItemScript == "Markup")
			{
				info.nItemType = POPUP_ITEM_MARKUP;
			}
		}else
		if(strItemName == "Background")
		{
			info.strBackgroundImage = strItemValue;
		}else
		if(strItemName == "ShowDelay")
		{
			info.nShowDelay = atoi(strItemValue);
		}else
		if(strItemName == "Transparency")
		{
			info.nTransparency = atoi(strItemValue);
		}else
		if(strItemName == "Width")
		{
			info.nWidth = atoi(strItemValue);
		}else
		if(strItemName == "Hight")
		{
			info.nHight = atoi(strItemValue);
		}else
		if(strItemName == "AddHight")
		{
			info.nAddHight = atoi(strItemValue);
		}else
		if(strItemName == "Animation")
		{
			info.nAnimation = atoi(strItemValue);
		}else
		if(strItemName == "Button")
		{
			info.asButtonText.Add(strItemValue);
			info.asButtonScript.Add(strItemExplanation);
		}
	}


	// 执行PopupControl对话框
	try{
	pIPlatUI->SendOwmCmd("", OC_POPUP_CONTROL, 0, (LPARAM)(&info), NULL);
	}catch(...)
	{
		Tcl_ResetResult(interp);
		Tcl_AppendResult(interp, "Catch a unknown exception while execute PopupControl command.", (char *) NULL);
		return TCL_ERROR;
	}

	// 前面调用接口查询函数,需要释放
	((IInterp*)(info.pInterp))->Release();

	ConvertResultToUTF(interp);
	return TCL_OK;
}

static int g_nVciHandleTcl = 10000;	// CVciMessage的handle变量,每次加1,仅针对Tcl发送的msg
/////////////////////////////////////////////////////////////////////////////
// 扩展命令:SendVciMessage
// SendVciMessage sendMsg ?recvMsg?
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_Plat_SendVciMessage(ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc < 2)
	{
		interp->result = "wrong # args: should be \"SendVciMessage sendMsg ?recvMsg?\"";
		return TCL_ERROR;
	}

	CTclInterp* pTclInterp = GetTclInterp((LPVOID)interp);
	IPlatUI* pIPlatUI = NULL;
	if(pTclInterp)
	{
		pIPlatUI = pTclInterp->pIPlatUI;
	}
	if(pIPlatUI == NULL)
	{
		Tcl_AppendResult(interp, "Get platform interface fail.", (char *) NULL);
		return TCL_ERROR;
	}

	int nListCount = 0;
	Tcl_Obj** paList;
	if(TCL_ERROR == Tcl_ListObjGetElements(interp, objv[1], &nListCount, &paList))
	{
		return TCL_ERROR;
	}

	if(0 == nListCount)
	{
		return TCL_OK;
	}

	CVciMessage msgSend;
	for(int i=0; i<nListCount; i++)
	{
		// 参数解析
		Tcl_Obj* list = (paList[i]);

		int nSubCount = 0;
		Tcl_Obj** paSubList;
		if(TCL_ERROR == Tcl_ListObjGetElements(interp, list, &nSubCount, &paSubList))
		{
			continue;
		}

		if(0 == nSubCount)
		{
			continue;
		}

		CStringArray asItem;
		for(int j=0; j<nSubCount; j++)
		{
			asItem.Add(ConvertUTFParam(paSubList[j]->bytes));
		}
		CString strItemName = "";
		CString strItemValue = "";
		CString strItemContent = "";
		CString strItemParam1 = "";
		CString strItemParam2 = "";
		if(nSubCount > 0)
		{
			strItemName = asItem[0];
		}
		if(nSubCount > 1)
		{
			strItemValue = asItem[1];
		}
		if(nSubCount > 2)
		{
			strItemContent = asItem[2];
		}
		if(nSubCount > 3)
		{
			strItemParam1 = asItem[3];
		}
		if(nSubCount > 4)
		{
			strItemParam2 = asItem[3];
		}

		if(strItemName == "Encrypt")
		{
			if(strItemValue == "base64")
			{
				msgSend.nEncrypt = VME_BASE64;
			}else
			if(strItemValue == "DES")
			{
				msgSend.nEncrypt = VME_DES;
			}
		}else
		if(strItemName == "MsgID")
		{
			msgSend.szMsgId = strItemValue;
		}else
		if(strItemName == "From")
		{
			msgSend.szFrom = strItemValue;
		}else
		if(strItemName == "Dest")
		{
			msgSend.szDest = strItemValue;
		}else
		if(strItemName == "UserFrom")
		{
			msgSend.szUserFrom = strItemValue;
		}else
		if(strItemName == "UserTo")
		{
			msgSend.szUserTo = strItemValue;
		}else
		if(strItemName == "CmdCode")
		{
			msgSend.nCmdCode = atoi(strItemValue);
		}else
		if(strItemName == "Handle")
		{
			msgSend.nHandle = atoi(strItemValue);
		}else
		if(strItemName == "Type")
		{
			if(strItemValue == "Normal")
			{
				msgSend.nMsgType = VMT_NORMAL;
			}else
			if(strItemValue == "Broadcast")
			{
				msgSend.nMsgType = VMT_BROADCAST;
			}else
			if(strItemValue == "Multicast")
			{
				msgSend.nMsgType = VMT_MULTICAST;
			}
		}else
		if(strItemName == "Request")
		{
			if(strItemValue == "Request")
			{
				msgSend.nRequest = VMT_REQUEST;
			}else
			if(strItemValue == "Response")
			{
				msgSend.nRequest = VMT_RESPONSE;
			}
		}else
		if(strItemName == "Sync")
		{
			if(strItemValue == "Sync")
			{
				msgSend.nSync = VMT_SYNC;
			}else
			if(strItemValue == "Async")
			{
				msgSend.nSync = VMT_ASYNC;
			}
		}else
		if(strItemName == "TimeOut")
		{
			msgSend.nTimeOut = atoi(strItemValue);
		}else
		if(strItemName == "Result")
		{
			if(strItemValue == "OK")
			{
				msgSend.nResult = VMR_OK;
			}else
			if(strItemValue == "Fail")
			{
				msgSend.nResult = VMR_FAIL;
			}else
			{
				msgSend.nResult = atoi(strItemValue);
			}
		}else
		if(strItemName == "Data")
		{
			if(strItemValue == "string")
			{
				msgSend.SetString(strItemContent);
			}else
			if(strItemValue == "xml")
			{
				msgSend.SetXML(strItemContent);
			}else
			if(strItemValue == "owm")
			{
				msgSend.SetOwmCmd(atoi(strItemContent), atoi(strItemParam1), atoi(strItemParam2));
			}
		}
	}

	if(msgSend.nHandle == VMH_NULL)
	{
		if(g_nVciHandleTcl > 11000)
		{
			g_nVciHandleTcl = 10000;
		}
		msgSend.nHandle = g_nVciHandleTcl++;
	}

	CVciMessage msgRecv;

	// 执行SendMessage
	try{
	pIPlatUI->SendMessage(&msgSend, &msgRecv);
	}catch(...)
	{
		Tcl_ResetResult(interp);
		Tcl_AppendResult(interp, "Catch a unknown exception while execute SendVciMessage command.", (char *) NULL);
		return TCL_ERROR;
	}

	if((msgRecv.GetHandle() == msgSend.GetHandle()) && (objc > 2))
	{
		// 收到了返回消息,并且命令参数中有返回消息变量名,进行处理,根据返回消息生成列表并设置到变量
		CString strRetVar = ConvertUTFParam(Tcl_GetString(objv[2]));
		Tcl_Obj* listObj = Tcl_GetVar2Ex(interp, strRetVar.GetBuffer(strRetVar.GetLength()), NULL, TCL_LIST_ELEMENT);
		strRetVar.ReleaseBuffer();
		int nLength = 0;
		int nResult = Tcl_ListObjLength(interp, listObj, &nLength);
		if(nLength == 0)
		{
			// 如果列表长度为0,需要新创建一个列表,避免后面添加操作产生异常
			listObj = Tcl_NewListObj(0, NULL);
			listObj = Tcl_SetVar2Ex(interp, strRetVar.GetBuffer(strRetVar.GetLength()), NULL, listObj, TCL_LIST_ELEMENT);
			strRetVar.ReleaseBuffer();
		}

		// 添加返回消息的内容到列表
		CStringArray asItem;
		CString strTemp;

		asItem.RemoveAll();
		asItem.Add("Encrypt");
		strTemp.Format("%d", msgRecv.GetEncrypt());
		asItem.Add(strTemp);
		Tcl_Obj* objPtr = CreateTclList(interp, asItem);
		Tcl_ListObjAppendElement(interp, listObj, objPtr);

		asItem.RemoveAll();
		asItem.Add("Priority");
		strTemp.Format("%d", msgRecv.GetPriority());
		asItem.Add(strTemp);
		objPtr = CreateTclList(interp, asItem);
		Tcl_ListObjAppendElement(interp, listObj, objPtr);

		asItem.RemoveAll();
		asItem.Add("MsgID");
		asItem.Add(msgRecv.GetMsgId());
		objPtr = CreateTclList(interp, asItem);
		Tcl_ListObjAppendElement(interp, listObj, objPtr);

		asItem.RemoveAll();
		asItem.Add("From");
		asItem.Add(msgRecv.GetFrom());
		objPtr = CreateTclList(interp, asItem);
		Tcl_ListObjAppendElement(interp, listObj, objPtr);

		asItem.RemoveAll();
		asItem.Add("Dest");
		asItem.Add(msgRecv.GetDest());
		objPtr = CreateTclList(interp, asItem);
		Tcl_ListObjAppendElement(interp, listObj, objPtr);

		asItem.RemoveAll();
		asItem.Add("UserFrom");
		asItem.Add(msgRecv.GetUserFrom());
		objPtr = CreateTclList(interp, asItem);
		Tcl_ListObjAppendElement(interp, listObj, objPtr);

		asItem.RemoveAll();
		asItem.Add("UserTo");
		asItem.Add(msgRecv.GetUserTo());
		objPtr = CreateTclList(interp, asItem);
		Tcl_ListObjAppendElement(interp, listObj, objPtr);

		asItem.RemoveAll();
		asItem.Add("CmdCode");
		strTemp.Format("%d", msgRecv.GetCmdCode());
		asItem.Add(strTemp);
		objPtr = CreateTclList(interp, asItem);
		Tcl_ListObjAppendElement(interp, listObj, objPtr);

		asItem.RemoveAll();
		asItem.Add("Handle");
		strTemp.Format("%d", msgRecv.GetHandle());
		asItem.Add(strTemp);
		objPtr = CreateTclList(interp, asItem);
		Tcl_ListObjAppendElement(interp, listObj, objPtr);

		asItem.RemoveAll();
		asItem.Add("Request");
		if(msgRecv.GetRequest() == VMT_REQUEST)
		{
			asItem.Add("Request");
		}else
		if(msgRecv.GetRequest() == VMT_RESPONSE)
		{
			asItem.Add("Response");
		}
		objPtr = CreateTclList(interp, asItem);
		Tcl_ListObjAppendElement(interp, listObj, objPtr);

		asItem.RemoveAll();
		asItem.Add("Sync");
		if(msgRecv.GetSync() == VMT_SYNC)
		{
			asItem.Add("Sync");
		}else
		if(msgRecv.GetSync() == VMT_ASYNC)
		{
			asItem.Add("Async");
		}
		objPtr = CreateTclList(interp, asItem);
		Tcl_ListObjAppendElement(interp, listObj, objPtr);

		asItem.RemoveAll();
		asItem.Add("Result");
		strTemp.Format("%d", msgRecv.GetResult());
		asItem.Add(strTemp);
		objPtr = CreateTclList(interp, asItem);
		Tcl_ListObjAppendElement(interp, listObj, objPtr);

		asItem.RemoveAll();
		asItem.Add("Data");
		if(msgRecv.GetDataType() == VMD_STRING)
		{
			asItem.Add("string");
			asItem.Add(msgRecv.GetString());
		}else
		if(msgRecv.GetDataType() == VMD_XML)
		{
			asItem.Add("xml");
			asItem.Add(msgRecv.GetXML());
		}
		objPtr = CreateTclList(interp, asItem);
		Tcl_ListObjAppendElement(interp, listObj, objPtr);
	}

	ConvertResultToUTF(interp);
	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:ImageManager
// ImageManager seticon command-id icon-file icon-index
// ImageManager seticons bmp-file id-list
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_Plat_ImageManager(ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc < 3)
	{
		interp->result = "wrong # args: should be \"ImageManager operate params\"";
		return TCL_ERROR;
	}

	CTclInterp* pTclInterp = GetTclInterp((LPVOID)interp);
	IPlatUI* pIPlatUI = NULL;
	if(pTclInterp)
	{
		pIPlatUI = pTclInterp->pIPlatUI;
	}
	if(pIPlatUI == NULL)
	{
		Tcl_AppendResult(interp, "Get platform interface fail.", (char *) NULL);
		return TCL_ERROR;
	}

	CString strOperate = Tcl_GetString(objv[1]);
	if(strOperate == "seticon")
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "seticon command-id icon-file");
			return TCL_ERROR;
		}

		int nID = atoi(ConvertUTFParam(Tcl_GetString(objv[2])));
		CString strIconFile = ConvertUTFParam(Tcl_GetString(objv[3]));

		pIPlatUI->SendOwmCmd("", OC_SETCOMMANDICON, nID, (LPARAM)(LPCTSTR)strIconFile, NULL);
	}else
	if(strOperate == "seticons")
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "seticons bmp-file id-list");
			return TCL_ERROR;
		}

		CString strBmpFile = ConvertUTFParam(Tcl_GetString(objv[2]));

		CUIntArray auID;

		int nListCount = 0;
		Tcl_Obj** paList;
		if(TCL_ERROR == Tcl_ListObjGetElements(interp, objv[3], &nListCount, &paList))
		{
			return TCL_ERROR;
		}

		if(0 == nListCount)
		{
			return TCL_OK;
		}

		for(int i=0; i<nListCount; i++)
		{
			// 参数解析
			Tcl_Obj* list = (paList[i]);

			auID.Add(atoi(list->bytes));
		}

		pIPlatUI->SendOwmCmd("", OC_SETCOMMANDICONS, (WPARAM)(&auID), (LPARAM)(LPCTSTR)strBmpFile, NULL);
	}else
	{
		Tcl_AppendResult(interp, "operater must be seticon or seticons.",  (char *) NULL);
		return TCL_ERROR;
	}


	ConvertResultToUTF(interp);
	return TCL_OK;
}

// 多语言定义
#define LANGUAGE_PAGE_ENGLISH	1033
#define LANGUAGE_PAGE_CHINESE	2052
#define REG_REG_LANGUAGE		_T("Language")		// 当前语言

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:用于TCL变量的类型转换
// itos		: int到char*类型的转换
// dtot		: double到OleDateTime类型的转换
// utf8tostr: UTF8字符串到普通字符串的转换
// strtoutf8: 普通字符串到UTF8字符串到的转换
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_Plat_VarConvert(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc < 3)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "converttype var");
		return TCL_ERROR;
	}

	CString strConvertType = Tcl_GetString(objv[1]);
	CString strVar = Tcl_GetString(objv[2]);

	if(strConvertType == "itos")	// int到char*类型的转换
	{
		char* str = (char*)(atoi(strVar));
		Tcl_AppendResult(interp, str, (char *)NULL);
	}else
	if(strConvertType == "dtot")	// double到OleDateTime类型的转换
	{
		double d = atof(strVar);
		COleDateTime t = COleVariant(d);
		Tcl_AppendResult(interp, t.Format(), (char *)NULL);
	}else
	if(strConvertType == "utf8tostr")	// UTF8字符串到普通字符串的转换,仅中文模式有效
	{
		// 注:不能用平台传递的语言信息,因为平台启动时因为组件启动顺序的问题,语言信息可能还没有设置
		int nCurrentLanguage = AfxGetApp()->GetProfileInt(REG_VERSION_SUBKEY, REG_REG_LANGUAGE, ::GetSystemDefaultLangID());
		if(LANGUAGE_PAGE_CHINESE == nCurrentLanguage)
		//if(gGetCurrentLanguage() == VCI_LANGUAGE_CHINESE)
		{
			Tcl_AppendResult(interp, ConvertUTFParam(strVar), (char *)NULL);
		}else
		{
			Tcl_AppendResult(interp, strVar, (char *)NULL);
		}
	}else
	if(strConvertType == "strtoutf8")	// 普通字符串到UTF8字符串到的转换,仅英文模式有效
	{
		// 注:不能用平台传递的语言信息,因为平台启动时因为组件启动顺序的问题,语言信息可能还没有设置
		int nCurrentLanguage = AfxGetApp()->GetProfileInt(REG_VERSION_SUBKEY, REG_REG_LANGUAGE, ::GetSystemDefaultLangID());
		if(LANGUAGE_PAGE_ENGLISH == nCurrentLanguage)
		//if(gGetCurrentLanguage() == VCI_LANGUAGE_ENGLISH)
		{
			Tcl_AppendResult(interp, ConvertStringToUTF(strVar), (char *)NULL);
		}else
		{
			Tcl_AppendResult(interp, strVar, (char *)NULL);
		}
	}else
	{
		Tcl_AppendResult(interp, "Unknown convert type", strConvertType, ", only support itos, dtot...", (char *)NULL);
		return TCL_ERROR;
	}

	return TCL_OK;
}

//功能：工程操作
//用法：
//      pproject isopen				-- 是否打开
//		pproject isworking			-- 是否在工作
//		pproject name				-- 获取工程名
//		pproject desc				-- 获取工程描述
//		pproject path				-- 获取工程路径
//		pproject file				-- 获取工程文件
//		pproject open prjfile		-- 打开工程
//		pproject close				-- 关闭工程
//		pproject save				-- 保存工程
//		pproject run				-- 运行工程
//		pproject build				-- 编译工程
//		pproject rebuild			-- 重新编译工程
//		pproject clean				-- 清除编译信息
//		pproject information		-- 显示工程信息页面
//		pproject property			-- 显示工程属性
//参数：
//返回值：
TCL_CMDEF( Tcl_Cmd_Plat_Project )
{
	Tcl_ResetResult(pInterp);

	if( objc < 2 )
	{
		Tcl_WrongNumArgs (pInterp, 1, objv, "operate");
		ConvertResultToUTF(pInterp);
		return TCL_ERROR;
	}

	// 查找平台接口对象
	CTclInterp* pTclInterp = GetTclInterp((LPVOID)pInterp);
	IPlatUI* pIPlatUI = NULL;
	if(pTclInterp)
	{
		pIPlatUI = pTclInterp->pIPlatUI;
	}
	if(pIPlatUI == NULL)
	{
		Tcl_AppendResult(pInterp, "Get platform interface fail.", (char *) NULL);
		return TCL_ERROR;
	}

	CString strOp = ConvertUTFParam(Tcl_GetString(objv[1]));

	CString strProjectName;
	CString strProjectDesc;
	CString strProjectFile;
	CString strProjectPath;
	BOOL bProjectIsOpen = FALSE;

	// 获取工程插件接口
	IProject* pIProject = (IProject*)(pIPlatUI->GetObjectByInstanceName("###project###"));
	if(pIProject)
	{
		bProjectIsOpen = (pIProject->GetActiveProject(strProjectName) == trpOk);	// 获取当前活动工程
		pIProject->GetProjectInfo(strProjectName, strProjectDesc, strProjectFile, strProjectPath);	// 获取活动工程的信息
	}

	if(strOp == "isopen")
	{
		Tcl_AppendResult( pInterp, bProjectIsOpen ? "1" : "0", (char *)NULL);
	}else
	if(strOp == "isworking")
	{
		BOOL bIsWorking = FALSE;
		if(bProjectIsOpen)
		{
			bIsWorking = (pIProject->GetProjectState(strProjectName) == PROJECT_WORKING);
		}
		Tcl_AppendResult( pInterp, bIsWorking ? "1" : "0", (char *)NULL);
	}else
	if(strOp == "name")
	{
		Tcl_AppendResult( pInterp, strProjectName, (char *)NULL);
	}else
	if(strOp == "desc")
	{
		Tcl_AppendResult( pInterp, strProjectDesc, (char *)NULL);
	}else
	if(strOp == "path")
	{
		Tcl_AppendResult( pInterp, strProjectPath, (char *)NULL);
	}else
	if(strOp == "file")
	{
		Tcl_AppendResult( pInterp, strProjectFile, (char *)NULL);
	}else
	if(strOp == "open")
	{
		if( objc < 3 )
		{
			Tcl_WrongNumArgs (pInterp, 1, objv, "open project");
			ConvertResultToUTF(pInterp);
			return TCL_ERROR;
		}
		CString strProjectFile = ConvertUTFParam(Tcl_GetString(objv[2]));	// 要打开的工程名

		pIPlatUI->SendOwmCmd("", OC_OPENPROJECT, NULL, (LPARAM)(LPCTSTR)strProjectFile, NULL);
	}else
	if(strOp == "close")
	{
		if(bProjectIsOpen)
		{
			// 关闭工程
			pIProject->CloseProject(TRUE);
			// 释放工程插件对象
			pIPlatUI->ReleaseVciObject(pIProject, TRUE);
		}
	}else
	if(strOp == "save")
	{
		if(bProjectIsOpen)
		{
			pIProject->SaveProject();
		}
	}else
	if(strOp == "build")
	{
		if(bProjectIsOpen)
		{
			pIProject->BuildProject();
		}
	}else
	if(strOp == "rebuild")
	{
		if(bProjectIsOpen)
		{
			pIProject->RebuildProject();
		}
	}else
	if(strOp == "run")
	{
		if(bProjectIsOpen)
		{
			pIProject->RunProject();
		}
	}else
	if(strOp == "clean")
	{
		if(bProjectIsOpen)
		{
			pIProject->CleanProject();
		}
	}else
	if(strOp == "information")
	{
		if(bProjectIsOpen)
		{
			pIProject->ProjectInformation();
		}
	}else
	if(strOp == "property")
	{
		if(bProjectIsOpen)
		{
			pIProject->RefreshProjectProperty();
		}
	}else
	/*if(strOp == "get")
	{
		if( objc < 3 )
		{
			Tcl_WrongNumArgs (pInterp, 1, objv, "get files");
			ConvertResultToUTF(pInterp);
			return TCL_ERROR;
		}
		CString szType = ConvertUTFParam(Tcl_GetString(objv[2]));
		if(szType == "files")
		{
		}
	}else
	if(strOp == "add")
	{
		if( objc < 4 )
		{
			Tcl_WrongNumArgs (pInterp, 1, objv, "add file");
			ConvertResultToUTF(pInterp);
			return TCL_ERROR;
		}
		CString szType = ConvertUTFParam(Tcl_GetString(objv[2]));
		if(szType == "file")
		{
			int nListCount = 0;
			Tcl_Obj** paList;
			if(TCL_ERROR == Tcl_ListObjGetElements(pInterp, objv[3], &nListCount, &paList))
			{
				Tcl_AppendResult(pInterp, "get bsdlfile list error.",  (char *) NULL);
				return TCL_ERROR;
			}

			if(0 == nListCount)
			{
				// 列表为空则不继续
				return TCL_OK;
			}

			Tcl_AppendResult( pInterp, bRet ? "1" : "0", (char *)NULL);
		}
	}else
	if(strOp == "del")
	{
		if( objc < 4 )
		{
			Tcl_WrongNumArgs (pInterp, 1, objv, "del file");
			ConvertResultToUTF(pInterp);
			return TCL_ERROR;
		}
		CString szType = ConvertUTFParam(Tcl_GetString(objv[2]));
		if(szType == "file")
		{
			int nListCount = 0;
			Tcl_Obj** paList;
			if(TCL_ERROR == Tcl_ListObjGetElements(pInterp, objv[3], &nListCount, &paList))
			{
				Tcl_AppendResult(pInterp, "get bsdlfile list error.",  (char *) NULL);
				return TCL_ERROR;
			}

			if(0 == nListCount)
			{
				// 列表为空则不继续
				return TCL_OK;
			}

			Tcl_AppendResult( pInterp, bRet ? "1" : "0", (char *)NULL);
		}
	}else*/
	{
		Tcl_AppendResult(pInterp, "bad argument \"", Tcl_GetString(objv[1]), "\" : must be isopen, isworking, name, desc, path, file, open, close, save, build, rebuild, run, information, property, add or del", (char *) NULL);
		return TCL_ERROR;
	}

	ConvertResultToUTF(pInterp);
	return TCL_OK;
}

//TCL命令声明函数
int TclExCmd_Init(Tcl_Interp *interp)
{
	// 注册命令

	// License命令
	Tcl_CreateObjCommand(interp, "License", Tcl_Cmd_Plat_License, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 信息提示对话框
	Tcl_CreateCommand(interp, "MessageBox", Tcl_Cmd_Plat_MessageBox, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 文件对话框
	Tcl_CreateCommand(interp, "FileDlg", Tcl_Cmd_Plat_FileDlg, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 路径选择对话框
	Tcl_CreateCommand(interp, "PathSelect", Tcl_Cmd_Plat_PathSelectDlg, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 通用进度对话框
	Tcl_CreateObjCommand(interp, "ProgressDlg", Tcl_Cmd_ProgressDlg, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 属性树对话框
	Tcl_CreateObjCommand(interp, "OptionTree", Tcl_Cmd_Plat_OptionTreeDlg, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 登录对话框
	Tcl_CreateObjCommand(interp, "tclLogin", Tcl_Cmd_tclLogin, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 输出窗口操作
	Tcl_CreateCommand(interp, "output", Tcl_Cmd_Output, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 状态行操作
	Tcl_CreateCommand(interp, "StatusBar", Tcl_Cmd_StatusBar, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 消息方式延时
	Tcl_CreateCommand(interp, "pwait", Tcl_Cmd_Peek_Wait, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// TaskDialog操作
	Tcl_CreateObjCommand(interp, "TaskDialog", Tcl_Cmd_Plat_TaskDialog, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// PopupControl操作
	Tcl_CreateObjCommand(interp, "PopupControl", Tcl_Cmd_Plat_PopupControl, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// SendVciMessage操作
	Tcl_CreateObjCommand(interp, "SendMessage", Tcl_Cmd_Plat_SendVciMessage, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// ImageManager操作
	Tcl_CreateObjCommand(interp, "ImageManager", Tcl_Cmd_Plat_ImageManager, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// TCL变量的类型转换
	Tcl_CreateObjCommand(interp, "varconvert", Tcl_Cmd_Plat_VarConvert, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);
	
	// 工程操作
	Tcl_CreateObjCommand(interp, "pproject", Tcl_Cmd_Plat_Project, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

    return TCL_OK;
}
