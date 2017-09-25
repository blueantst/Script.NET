////////////////////////////////////////////////////////////////////////////
//	File:		TclDuiVision.cpp
//	Version:	1.0.0.0
//	Created:	2017-01-21
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	implementation of Tcl DuiVision cmds
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CTclInterp.h"
#include "TclDuiVision.h"
#include "IDuiVisionApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static IDuiVisionApp* g_pIDuiVisionApp = NULL;		// 平台DuiVision接口指针

/////////////////////////////////////////////////////////////////////////////
// 获取IDuiVisionApp接口
/////////////////////////////////////////////////////////////////////////////
IDuiVisionApp* GetIDuiVisionApp(Tcl_Interp *interp)
{
	if(g_pIDuiVisionApp == NULL)
	{
		CTclInterp* pTclInterp = GetTclInterp((LPVOID)interp);
		if(pTclInterp)
		{
			g_pIDuiVisionApp = (IDuiVisionApp*)(pTclInterp->getPlatInterface(PLAT_INTERFACE_DUIVISIONAPP));
		}
	}
	return g_pIDuiVisionApp;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static CPtrList	g_TclDuiDialogObjList;	// DUI对话框对象列表

CTclDuiDialogObj::CTclDuiDialogObj(Tcl_Interp* interp)
{
	m_pDialogObj	= NULL;
	m_Interp		= interp;
	m_strName		= "";
	m_strErrorInfo	= "";
	g_TclDuiDialogObjList.AddTail(this);
}

CTclDuiDialogObj::~CTclDuiDialogObj()
{
	// 删除对话框
	IDuiVisionApp* pIDuiVisionApp = GetIDuiVisionApp(m_Interp);
	if(pIDuiVisionApp)
	{
		pIDuiVisionApp->RemoveDuiDialog((CDlgBase*)(m_pDialogObj));
	}

	m_pDialogObj = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 创建一个DUI对话框
/////////////////////////////////////////////////////////////////////////////
void* CTclDuiDialogObj::CreateDuiDialog(CString strXmlTemplate, CString strDlgName, BOOL bModule)
{
	IDuiVisionApp* pIDuiVisionApp = GetIDuiVisionApp(m_Interp);
	if(pIDuiVisionApp == NULL)
	{
		m_strErrorInfo = _T("Get duivisionapp interface failed.");
		return FALSE;
	}

	CDlgBase* pDlg = pIDuiVisionApp->CreateDuiDialog(strXmlTemplate, NULL, strDlgName, bModule, TRUE);

	m_pDialogObj = (void*)pDlg;
	m_strName = strDlgName;


	if(m_pDialogObj == NULL)
	{
		m_strErrorInfo = _T("Create duivision dialog failed.");
	}

	return m_pDialogObj;
}

/////////////////////////////////////////////////////////////////////////////
// 创建一个DUI对话框,并注册事件处理对象
/////////////////////////////////////////////////////////////////////////////
void* CTclDuiDialogObj::CreateDuiDialogWithHandler(CString strXmlTemplate, CString strDlgName, BOOL bModule,
				CTclDuiHandler* pDuiHandler, CString strTDuiHandlerObj, CString strControlName, BOOL bInit)
{
	IDuiVisionApp* pIDuiVisionApp = GetIDuiVisionApp(m_Interp);
	if(pIDuiVisionApp == NULL)
	{
		m_strErrorInfo = _T("Get duivisionapp interface failed.");
		return FALSE;
	}

	CDlgBase* pDlg = pIDuiVisionApp->CreateDuiDialog(strXmlTemplate, NULL, strDlgName, bModule, TRUE);

	m_pDialogObj = (void*)pDlg;
	m_strName = strDlgName;

	if(m_pDialogObj == NULL)
	{
		m_strErrorInfo = _T("Create duivision dialog failed.");
		return NULL;
	}

	// 创建并关联事件处理对象
	CTclDuiHandler::RegisterDuiHandler(pDlg, pDuiHandler,
					strTDuiHandlerObj, strControlName, bInit, m_Interp);

	// 调用iTcl Handler对象的设置DuiVision对象指针函数
	if(!strTDuiHandlerObj.IsEmpty())
	{
		CTclInterp* pTclInterp = GetTclInterp((LPVOID)m_Interp);
		if(pTclInterp)
		{
			CString strTemp;
			strTemp.Format("%s SetDuiObject %d", strTDuiHandlerObj, m_pDialogObj);
			if(!pTclInterp->RunCommandEx(strTemp, TCL_EVAL_DIRECT))
			{
				return NULL;
			}
		}
	}

	// 如果是模态对话框,则直接显示对话框
	if(bModule)
	{
		pDlg->DoModal();
	}

	return m_pDialogObj;
}

/////////////////////////////////////////////////////////////////////////////
// 获取DUI对话框对象
/////////////////////////////////////////////////////////////////////////////
CTclDuiDialogObj* CTclDuiDialogObj::GetDuiDialog(void* pDialogObj)
{
	// 获取对象列表中的项
	int count = g_TclDuiDialogObjList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_TclDuiDialogObjList.FindIndex(i);
		CTclDuiDialogObj* pTclDialogObj = (CTclDuiDialogObj*)g_TclDuiDialogObjList.GetAt(pos);
		if(pDialogObj == pTclDialogObj->GetDialogObj()) 
		{
			return pTclDialogObj;
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 添加DUI对话框对象
/////////////////////////////////////////////////////////////////////////////
void CTclDuiDialogObj::AddDuiDialog(CString strDlgName, void* pDialogObj, Tcl_Interp* interp)
{
	CTclDuiDialogObj* pTclDialogObj = CTclDuiDialogObj::GetDuiDialog(pDialogObj);
	if(pTclDialogObj != NULL)
	{
		return;
	}

	pTclDialogObj = new CTclDuiDialogObj(interp);
	pTclDialogObj->SetDialogObj(pDialogObj);
	if(!strDlgName.IsEmpty())
	{
		pTclDialogObj->SetName(strDlgName);
	}

	IDuiVisionApp* pIDuiVisionApp = GetIDuiVisionApp(interp);
	if(pIDuiVisionApp)
	{
		pIDuiVisionApp->AddDuiDialog((CDlgBase*)pDialogObj);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 释放DUI对话框对象
/////////////////////////////////////////////////////////////////////////////
void CTclDuiDialogObj::RemoveDuiDialog(void* pDialogObj, Tcl_Interp* interp)
{
	// 删除对话框
	int count = g_TclDuiDialogObjList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_TclDuiDialogObjList.FindIndex(i);
		CTclDuiDialogObj* pTclDialogObj = (CTclDuiDialogObj*)g_TclDuiDialogObjList.GetAt(pos);
		if(pDialogObj == pTclDialogObj->GetDialogObj()) 
		{
			g_TclDuiDialogObjList.RemoveAt(pos);
			delete pTclDialogObj;
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static CPtrList	g_TclDuiHandlerList;	// DUI Handler对象列表

CTclDuiHandler::CTclDuiHandler(Tcl_Interp* interp) : CDuiHandler()
{
	m_pDlg = NULL;
	m_Interp = interp;
	m_strTDuiHandlerObj = "";
	m_strErrorInfo	= "";
	g_TclDuiHandlerList.AddTail(this);
}

CTclDuiHandler::~CTclDuiHandler(void)
{
}

// 初始化
void CTclDuiHandler::OnInit()
{
	// 调用iTcl对象的OnInit函数
	if(!m_strTDuiHandlerObj.IsEmpty())
	{
		CTclInterp* pTclInterp = GetTclInterp((LPVOID)m_Interp);
		if(pTclInterp)
		{
			CString strTemp;
			strTemp.Format("%s OnInit", m_strTDuiHandlerObj);
			if(!pTclInterp->RunCommandEx(strTemp, TCL_EVAL_DIRECT))
			{
				return;
			}
		}
	}
}

// DUI事件处理
LRESULT CTclDuiHandler::OnDuiMessage(UINT uID, CString strName, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	// 调用iTcl对象的OnInit函数
	if(!m_strTDuiHandlerObj.IsEmpty())
	{
		CTclInterp* pTclInterp = GetTclInterp((LPVOID)m_Interp);
		if(pTclInterp)
		{
			CString strTemp;
			strTemp.Format("%s OnMessage %s %d %d %d",
					m_strTDuiHandlerObj, strName, Msg, wParam, lParam);
			if(!pTclInterp->RunCommandEx(strTemp, TCL_EVAL_DIRECT))
			{
				//AfxMessageBox(pTclInterp->GetResult());
				return FALSE;
			}
		}
	}

    return 0;
}

// DUI定时器事件处理
void CTclDuiHandler::OnTimer(UINT uTimerID, CString strTimerName)
{
	// 如果strTimerName非空，则调用iTcl对象的OnTimer函数
	if(!strTimerName.IsEmpty())
	{
		CTclInterp* pTclInterp = GetTclInterp((LPVOID)m_Interp);
		if(pTclInterp)
		{
			CString strTemp;
			strTemp.Format("%s OnTimer %s",
					m_strTDuiHandlerObj, strTimerName);
			if(!pTclInterp->RunCommandEx(strTemp, TCL_EVAL_DIRECT))
			{
				return;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 创建一个DUI Handler对象,并进行注册
/////////////////////////////////////////////////////////////////////////////
CTclDuiHandler* CTclDuiHandler::RegisterDuiHandler(CDuiObject* pDuiObject, CTclDuiHandler* pDuiHandler, CString strTDuiHandlerObj, CString strControlName, BOOL bInit, Tcl_Interp* interp)
{
	IDuiVisionApp* pIDuiVisionApp = GetIDuiVisionApp(interp);
	if(pIDuiVisionApp == NULL)
	{
		return NULL;
	}

	CTclDuiHandler* pTclDuiHandler = NULL;
	if(pDuiHandler != NULL)
	{
		pTclDuiHandler = pDuiHandler;
	}else
	{
		pTclDuiHandler = new CTclDuiHandler(interp);
	}
	pTclDuiHandler->SetTDuiHandlerObj(strTDuiHandlerObj);
	BOOL bRet = pIDuiVisionApp->RegisterDuiHandler(pDuiObject, pTclDuiHandler, strControlName, bInit);

	if(!bRet)
	{
		pTclDuiHandler->SetErrorInfo(_T("Register duivision handler failed."));
	}

	return pTclDuiHandler;
}

/////////////////////////////////////////////////////////////////////////////
// 查找DUI Handler对象是否存在
/////////////////////////////////////////////////////////////////////////////
BOOL CTclDuiHandler::IsDuiHandlerExist(CTclDuiHandler* pTclDuiHandler)
{
	// 查找对象列表中的项
	int count = g_TclDuiHandlerList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_TclDuiHandlerList.FindIndex(i);
		CTclDuiHandler* _pTclDuiHandler = (CTclDuiHandler*)g_TclDuiHandlerList.GetAt(pos);
		if(_pTclDuiHandler == pTclDuiHandler) 
		{
			return TRUE;
		}
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 添加一个DUI Handler对象
/////////////////////////////////////////////////////////////////////////////
CTclDuiHandler* CTclDuiHandler::AddDuiHandler(CTclDuiHandler* pTclDuiHandler, Tcl_Interp* interp)
{
	IDuiVisionApp* pIDuiVisionApp = GetIDuiVisionApp(interp);
	if(pIDuiVisionApp == NULL)
	{
		return NULL;
	}

	pIDuiVisionApp->AddDuiHandler(pTclDuiHandler);

	return pTclDuiHandler;
}

/////////////////////////////////////////////////////////////////////////////
// 删除一个DUI Handler对象
/////////////////////////////////////////////////////////////////////////////
void CTclDuiHandler::RemoveDuiHandler(CTclDuiHandler* pTclDuiHandler, Tcl_Interp* interp)
{
	IDuiVisionApp* pIDuiVisionApp = GetIDuiVisionApp(interp);
	if(pIDuiVisionApp == NULL)
	{
		return;
	}

	pIDuiVisionApp->RemoveDuiHandler(pTclDuiHandler);
}

/////////////////////////////////////////////////////////////////////////////
// 关闭指定解释器创建的所有DUI对象
/////////////////////////////////////////////////////////////////////////////
void CloseInterpAllDuiObject(Tcl_Interp* interp)
{
	// 删除指定解释器所有Tcl脚本创建的Dui对话框
	int count = g_TclDuiDialogObjList.GetCount();
	for(int i = count-1; i >= 0; i --)
	{
		POSITION pos = g_TclDuiDialogObjList.FindIndex(i);
		CTclDuiDialogObj* pTclDialogObj = (CTclDuiDialogObj*)g_TclDuiDialogObjList.GetAt(pos);
		if((pTclDialogObj != NULL) && (pTclDialogObj->GetInterp() == interp))
		{
			g_TclDuiDialogObjList.RemoveAt(pos);
			delete pTclDialogObj;
		}
	}

	// 删除所有Tcl脚本创建的DuiHandler对象
	count = g_TclDuiHandlerList.GetCount();
	for(int i = count-1; i >= 0; i --)
	{
		POSITION pos = g_TclDuiHandlerList.FindIndex(i);
		CTclDuiHandler* pTclDuiHandler = (CTclDuiHandler*)g_TclDuiHandlerList.GetAt(pos);
		if((pTclDuiHandler != NULL) && (pTclDuiHandler->GetInterp() == interp))
		{
			g_TclDuiHandlerList.RemoveAt(pos);
			// 必须调用平台的删除接口进行删除
			CTclDuiHandler::RemoveDuiHandler(pTclDuiHandler, interp);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 关闭所有DUI对象
/////////////////////////////////////////////////////////////////////////////
void CloseAllDuiObject()
{
	// 删除所有Tcl脚本创建的Dui对话框
	int count = g_TclDuiDialogObjList.GetCount();
	for(int i = count-1; i >= 0; i --)
	{
		POSITION pos = g_TclDuiDialogObjList.FindIndex(i);
		CTclDuiDialogObj* pTclDialogObj = (CTclDuiDialogObj*)g_TclDuiDialogObjList.GetAt(pos);
		if(pTclDialogObj != NULL)
		{
			g_TclDuiDialogObjList.RemoveAt(pos);
			delete pTclDialogObj;
		}
	}

	// 删除所有Tcl脚本创建的DuiHandler对象
	count = g_TclDuiHandlerList.GetCount();
	for(int i = count-1; i >= 0; i --)
	{
		POSITION pos = g_TclDuiHandlerList.FindIndex(i);
		CTclDuiHandler* pTclDuiHandler = (CTclDuiHandler*)g_TclDuiHandlerList.GetAt(pos);
		if(pTclDuiHandler != NULL) 
		{
			g_TclDuiHandlerList.RemoveAt(pos);
			// 必须调用平台的删除接口进行删除
			CTclDuiHandler::RemoveDuiHandler(pTclDuiHandler, pTclDuiHandler->GetInterp());
		}
	}

	// 释放此解释器中创建的DuiSystem实例
	DuiSystem::Release();
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:dui::resource
// load xmlFile
// getconfig name
// getxmlfile name
// getskin name
// getstring name
// setstring name value
// parsestring value
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_DUI_Resource(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc < 3)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "operate ...");
		return TCL_ERROR;
	}

	IDuiVisionApp* pIDuiVisionApp = GetIDuiVisionApp(interp);
	if(pIDuiVisionApp == NULL)
	{
		Tcl_AppendResult(interp, "Get duivisionapp interface fail!", (char *) NULL);
		return TCL_ERROR;
	}

	CString strOpr = ConvertUTFParam(Tcl_GetString(objv[1]));

	if(strOpr == "load")	// 加载资源文件
	{
		CString strXmlFile = ConvertUTFParam(Tcl_GetString(objv[2]));
		CString strStyle = _T("");
		if(objc > 3)
		{
			strStyle = ConvertUTFParam(Tcl_GetString(objv[3]));
		}
		if(!pIDuiVisionApp->DuiLoadResourceXml(strXmlFile, strStyle))
		{
			Tcl_AppendResult(interp, "Load resource file fail.", (char *) NULL);
			return TCL_ERROR;
		}
	}else
	if(strOpr == "getconfig")	// 获取系统配置信息
	{
		CString strName = ConvertUTFParam(Tcl_GetString(objv[2]));
		CString strValue = pIDuiVisionApp->DuiGetConfig(strName);
		Tcl_AppendResult(interp, strValue, (char *)NULL);
	}else
	if(strOpr == "getxmlfile")	// 获取XML文件
	{
		CString strName = ConvertUTFParam(Tcl_GetString(objv[2]));
		CString strValue = pIDuiVisionApp->DuiGetXmlFile(strName);
		Tcl_AppendResult(interp, strValue, (char *)NULL);
	}else
	if(strOpr == "getskin")	// 获取Skin
	{
		CString strName = ConvertUTFParam(Tcl_GetString(objv[2]));
		CString strValue = pIDuiVisionApp->DuiGetSkin(strName);
		Tcl_AppendResult(interp, strValue, (char *)NULL);
	}else
	if(strOpr == "getstring")	// 获取字符串值
	{
		CString strName = ConvertUTFParam(Tcl_GetString(objv[2]));
		CString strValue = pIDuiVisionApp->DuiGetString(strName);
		Tcl_AppendResult(interp, strValue, (char *)NULL);
	}else
	if(strOpr == "setstring")	// 设置字符串值
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "setstring name value");
			return TCL_ERROR;
		}
		CString strName = ConvertUTFParam(Tcl_GetString(objv[2]));
		CString strValue = ConvertUTFParam(Tcl_GetString(objv[3]));
		pIDuiVisionApp->DuiSetString(strName, strValue);
	}else
	if(strOpr == "parsestring")	// 解析字符串，替换其中的替换内容
	{
		// 获取tcl变量名
		CString strVarName = ConvertUTFParam(Tcl_GetString(objv[2]));
		// 获取变量值
		CString strValue = ConvertUTFParam(Tcl_GetVar(interp, strVarName.GetBuffer(strVarName.GetLength()), 0));
		pIDuiVisionApp->ParseDuiString(strValue);
		strVarName.ReleaseBuffer();

		strValue = ConvertStringToUTF(strValue);
		char* szVar = strVarName.GetBuffer(strVarName.GetLength());
		char* szValue = strValue.GetBuffer(strValue.GetLength());
		Tcl_SetVar(interp, szVar, szValue, TCL_LEAVE_ERR_MSG);
		strVarName.ReleaseBuffer();
		strValue.ReleaseBuffer();
	}else
	{
		Tcl_AppendResult(interp, "Unknown operation " + strOpr, (char *) NULL);
		return TCL_ERROR;
	}

	ConvertResultToUTF(interp);
	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:dui::control
// getcontrol controlName ?-parentDialog parentDialog?
// getcontrol controlName ?-parentPopup parentPopup?
// getcontrol controlName ?-parent parentControl?
// getcontrol controlName -dialog dialogName
// getvalue controlObj
// setvalue controlObj value
// setattribute controlObj attrName attrValue
// getposstr controlObj
// setposstr controlObj pos
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_DUI_Control(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc < 3)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "operate ...");
		return TCL_ERROR;
	}

	IDuiVisionApp* pIDuiVisionApp = GetIDuiVisionApp(interp);
	if(pIDuiVisionApp == NULL)
	{
		Tcl_AppendResult(interp, "Get duivisionapp interface fail!", (char *) NULL);
		return TCL_ERROR;
	}

	CString strOpr = ConvertUTFParam(Tcl_GetString(objv[1]));

	if(strOpr == "getcontrol")	// 获取DUI控件对象句柄
	{
		CString strControlName = ConvertUTFParam(Tcl_GetString(objv[2]));
		CControlBase* pControl = NULL;
		CString strType = _T("");
		if(objc > 3)
		{
			strType = ConvertUTFParam(Tcl_GetString(objv[3]));
			strType.MakeUpper();
		}
		if(strType == _T("-PARENT"))
		{
			CString strParentControl = ConvertUTFParam(Tcl_GetString(objv[4]));
			CDuiObject* pDuiObject = (CDuiObject*)(atoi(strParentControl));
			pControl = pIDuiVisionApp->DuiGetControl(pDuiObject, strControlName);
		}else
		if(strType == _T("-PARENTDIALOG"))
		{
			CString strParentDialog = ConvertUTFParam(Tcl_GetString(objv[4]));
			CDlgBase* pDuiDlg = (CDlgBase*)(atoi(strParentDialog));
			pControl = pIDuiVisionApp->DuiGetControl(pDuiDlg, strControlName);
		}else
		if(strType == _T("-PARENTPOPUP"))
		{
			CString strParentPopup = ConvertUTFParam(Tcl_GetString(objv[4]));
			CDlgPopup* pDuiPopup = (CDlgPopup*)(atoi(strParentPopup));
			pControl = pIDuiVisionApp->DuiGetControl(pDuiPopup, strControlName);
		}else
		if(strType == _T("-DIALOG"))
		{
			CString strDialogName = ConvertUTFParam(Tcl_GetString(objv[4]));
			pControl = pIDuiVisionApp->DuiGetDialogControl(strDialogName, strControlName);
		}else
		{
			pControl = pIDuiVisionApp->DuiGetControl(NULL, strControlName);
		}

		CString strResult;
		strResult.Format("%d", (int)pControl);
		Tcl_AppendResult(interp, strResult, (char *)NULL);
	}else
	if(strOpr == "getvalue")	// 获取DUI控件值
	{
		CString strControlObj = ConvertUTFParam(Tcl_GetString(objv[2]));
		CControlBase* pControl = (CControlBase*)(atoi(strControlObj));
		if(pControl == NULL)
		{
			Tcl_AppendResult(interp, "Control object is null!", (char *) NULL);
			return TCL_ERROR;
		}

		CString strResult = _T("");
		if(pControl->IsClass(CControlBaseFont::GetClassName()))
		{
			strResult = ((CControlBaseFont*)pControl)->GetTitle();
			Tcl_AppendResult(interp, strResult, (char *)NULL);
		}else
		if(pControl->IsClass(CDuiCheckButton::GetClassName()))
		{
			strResult = ((CDuiCheckButton*)pControl)->GetCheck() ? _T("1") : _T("0");
			Tcl_AppendResult(interp, strResult, (char *)NULL);
		}else
		if(pControl->IsClass(CDuiRadioButton::GetClassName()))
		{
			strResult = ((CDuiRadioButton*)pControl)->GetValue();
			Tcl_AppendResult(interp, strResult, (char *)NULL);
		}
	}else
	if(strOpr == "setvalue")	// 设置DUI控件值
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "setvalue controlObj value ?-update?");
			return TCL_ERROR;
		}

		CString strControlObj = ConvertUTFParam(Tcl_GetString(objv[2]));
		CString strValue = ConvertUTFParam(Tcl_GetString(objv[3]));
		BOOL bUpdateControl = FALSE;
		if(objc > 4)
		{
			CString strUpdate = ConvertUTFParam(Tcl_GetString(objv[4]));
			if(strUpdate == "-update")
			{
				bUpdateControl = TRUE;
			}
		}

		CControlBase* pControl = (CControlBase*)(atoi(strControlObj));
		if(pControl == NULL)
		{
			Tcl_AppendResult(interp, "Control object is null!", (char *) NULL);
			return TCL_ERROR;
		}

		if(pControl->IsClass(CControlBaseFont::GetClassName()))
		{
			((CControlBaseFont*)pControl)->SetTitle(strValue);
		}else
		if(pControl->IsClass(CDuiCheckButton::GetClassName()))
		{
			((CDuiCheckButton*)pControl)->SetCheck((strValue == _T("1")) ? TRUE : FALSE);
		}else
		if(pControl->IsClass(CDuiRadioButton::GetClassName()))
		{
			((CDuiRadioButton*)pControl)->SetCheck((strValue == _T("1")) ? TRUE : FALSE);
		}else
		{
			Tcl_AppendResult(interp, "Control is not support get value!", (char *) NULL);
			return TCL_ERROR;
		}

		// 刷新控件
		if(bUpdateControl)
		{
			pControl->UpdateControl(true);
		}
	}else
	if(strOpr == "setattribute")	// 设置DUI控件属性值
	{
		if(objc < 5)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "setattribute controlObj attrName attrValue ?-update?");
			return TCL_ERROR;
		}

		CString strControlObj = ConvertUTFParam(Tcl_GetString(objv[2]));
		CString strAttrName = ConvertUTFParam(Tcl_GetString(objv[3]));
		CString strAttrValue = ConvertUTFParam(Tcl_GetString(objv[4]));
		BOOL bUpdateControl = FALSE;
		if(objc > 5)
		{
			CString strUpdate = ConvertUTFParam(Tcl_GetString(objv[5]));
			if(strUpdate == "-update")
			{
				bUpdateControl = TRUE;
			}
		}

		CControlBase* pControl = (CControlBase*)(atoi(strControlObj));
		if(pControl == NULL)
		{
			Tcl_AppendResult(interp, "Control object is null!", (char *) NULL);
			return TCL_ERROR;
		}

		// 需要通过控件函数刷新的一些属性值需要特殊处理,其他的直接调用控件的设置属性值函数
		if(strAttrName == _T("show"))
		{
			pControl->SetVisible(_ttoi(strAttrValue));
		}else
		if(strAttrName == _T("title"))
		{
			((CControlBaseFont*)pControl)->SetTitle(strAttrValue);
		}else
		if(strAttrName == _T("value"))
		{
			if(pControl->IsClass(CDuiComboBox::GetClassName()))
			{
				((CDuiComboBox*)pControl)->SetComboValue(strAttrValue);
			}else
			{
				pControl->SetAttribute(strAttrName, strAttrValue, TRUE);
			}
		}else
		{
			pControl->SetAttribute(strAttrName, strAttrValue, TRUE);
		}

		// 刷新控件
		if(bUpdateControl)
		{
			pControl->UpdateControl(true);
		}
	}else
	if(strOpr == "getposstr")	// 获取DUI控件值的位置字符串
	{
		CString strControlObj = ConvertUTFParam(Tcl_GetString(objv[2]));
		CControlBase* pControl = (CControlBase*)(atoi(strControlObj));
		if(pControl == NULL)
		{
			Tcl_AppendResult(interp, "Control object is null!", (char *) NULL);
			return TCL_ERROR;
		}

		Tcl_AppendResult(interp, pControl->GetPosStr(), (char *)NULL);
	}else
	if(strOpr == "setposstr")	// 设置DUI控件值的位置字符串
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "setposstr controlObj pos");
			return TCL_ERROR;
		}

		CString strControlObj = ConvertUTFParam(Tcl_GetString(objv[2]));
		CString strPos = ConvertUTFParam(Tcl_GetString(objv[3]));
		CControlBase* pControl = (CControlBase*)(atoi(strControlObj));
		if(pControl == NULL)
		{
			Tcl_AppendResult(interp, "Control object is null!", (char *) NULL);
			return TCL_ERROR;
		}

		pControl->SetPosStr(strPos);
	}else
	{
		Tcl_AppendResult(interp, "Unknown operation " + strOpr, (char *) NULL);
		return TCL_ERROR;
	}

	ConvertResultToUTF(interp);
	return TCL_OK;
}

// GridCtrl单元格信息
struct _GridItemInfo
{
	CString strTitle;		// 文字
	CString strContent;		// 内容
	int		nImageIndex;	// 图片索引
	CString strImage;		// 图片文件
	Color	clrText;		// 文字颜色
	CString strLink;		// 链接的文字
	CString strLinkAction;	// 链接的动作
	BOOL	bUseTitleFont;	// 是否使用标题字体显示标题
	BOOL	bLink;			// 是否是链接单元格
};

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:dui::gridctrl
// addrow controlObj row ?-id id? ?-image image? ?-rightImage rightImage? ?-check check? ?-textColor textColor? ?-backColor backColor? -items itemList
// deleterow controlObj row
// addcontrol controlObj row item controlClass controlName ?-dialog dialogObj?
// getrowcount controlObj
// getrowcheck controlObj row
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_DUI_GridCtrl(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc < 3)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "operate ...");
		return TCL_ERROR;
	}

	IDuiVisionApp* pIDuiVisionApp = GetIDuiVisionApp(interp);
	if(pIDuiVisionApp == NULL)
	{
		Tcl_AppendResult(interp, "Get duivisionapp interface fail!", (char *) NULL);
		return TCL_ERROR;
	}

	// 初始化DuiSystem的资源文件根目录
	DuiSystem::SetRootPath(pIDuiVisionApp->GetDuiResourcePath());

	CString strOpr = ConvertUTFParam(Tcl_GetString(objv[1]));
	CString strDuiObject = ConvertUTFParam(Tcl_GetString(objv[2]));
	CDuiGridCtrl* pGridCtrl = (CDuiGridCtrl*)(atoi(strDuiObject));
	if(pGridCtrl == NULL)
	{
		Tcl_AppendResult(interp, "Get gridctrl object fail!", (char *) NULL);
		return TCL_ERROR;
	}

	if(strOpr == "addrow")	// 添加行
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "addrow controlObj row ?-image image? ?-check check? -items itemList");
			return TCL_ERROR;
		}

		int nRow = -1;
		CString strRow = ConvertUTFParam(Tcl_GetString(objv[3]));
		if(!strRow.IsEmpty())
		{
			nRow = atoi(strRow);
		}

		CString strId = "";
		int nImageIndex = -1;
		CString strImage = "";
		int nRightImageIndex = -1;
		CString strRightImage = "";
		Color clrText = Color(0,0,0,0);
		Color clrBack = Color(0,0,0,0);
		int nCheck = -1;
		vector<_GridItemInfo> vecItemInfo;
		for(int i = 4; i < objc; i++)
		{
			CString strParam = ConvertUTFParam(Tcl_GetString(objv[i]));
			if((strParam.GetLength() > 0) && (strParam[0] == _T('-')))
				strParam.Delete(0, 1);
			strParam.MakeUpper();

			if(strParam == "ID")	// 设置id属性
			{
				if((objc - i) < 2)
				{
					Tcl_WrongNumArgs(interp, 1, objv, "wrong # args: lost id param");
					return TCL_ERROR;
				}
				strId = ConvertUTFParam(Tcl_GetString(objv[i+1]));
				i++;
			}else
			if(strParam == "IMAGE")	// 设置image属性
			{
				if((objc - i) < 2)
				{
					Tcl_WrongNumArgs(interp, 1, objv, "wrong # args: lost image param");
					return TCL_ERROR;
				}
				CString strImageTmp = ConvertUTFParam(Tcl_GetString(objv[i+1]));
				if((strImageTmp.Find(_T("skin:")) == 0) || (strImageTmp.Find(_T(".")) != -1))
				{
					strImage = strImageTmp;
				}else
				{
					nImageIndex = atoi(strImageTmp);
				}
				i++;
			}else
			if(strParam == "RIGHTIMAGE")	// 设置right-image属性
			{
				if((objc - i) < 2)
				{
					Tcl_WrongNumArgs(interp, 1, objv, "wrong # args: lost right-image param");
					return TCL_ERROR;
				}
				CString strImageTmp = ConvertUTFParam(Tcl_GetString(objv[i+1]));
				if((strImageTmp.Find(_T("skin:")) == 0) || (strImageTmp.Find(_T(".")) != -1))
				{
					strRightImage = strImageTmp;
				}else
				{
					nRightImageIndex = atoi(strImageTmp);
				}
				i++;
			}else
			if(strParam == "CHECK")	// 设置check属性
			{
				if((objc - i) < 2)
				{
					Tcl_WrongNumArgs(interp, 1, objv, "wrong # args: lost check param");
					return TCL_ERROR;
				}
				CString strCheck = ConvertUTFParam(Tcl_GetString(objv[i+1]));
				if(!strCheck.IsEmpty())
				{
					nCheck = atoi(strCheck);
				}
				i++;
			}else
			if(strParam == "TEXTCOLOR")	// 设置text-color属性
			{
				if((objc - i) < 2)
				{
					Tcl_WrongNumArgs(interp, 1, objv, "wrong # args: lost text-color param");
					return TCL_ERROR;
				}
				clrText = CDuiObject::StringToColor(ConvertUTFParam(Tcl_GetString(objv[i+1])));
				i++;
			}else
			if(strParam == "BACKCOLOR")	// 设置back-color属性
			{
				if((objc - i) < 2)
				{
					Tcl_WrongNumArgs(interp, 1, objv, "wrong # args: lost back-color param");
					return TCL_ERROR;
				}
				clrBack = CDuiObject::StringToColor(ConvertUTFParam(Tcl_GetString(objv[i+1])));
				i++;
			}else
			if(strParam == "ITEMS")	// 设置items属性
			{
				if((objc - i) < 2)
				{
					Tcl_WrongNumArgs(interp, 1, objv, "wrong # args: lost itemList param");
					return TCL_ERROR;
				}
				
				// 读取item列表
				int nListCount = 0;
				Tcl_Obj** paList;
				if(TCL_ERROR == Tcl_ListObjGetElements(interp, objv[i+1], &nListCount, &paList))
				{
					return TCL_ERROR;
				}

				if(0 == nListCount)
				{
					return TCL_OK;
				}

				for(int j=0; j<nListCount; j++)
				{
					Tcl_Obj* listItem = (paList[j]);
					// 读取每个item的属性列表
					int nListCountAttr = 0;
					Tcl_Obj** paListAttr;
					if(TCL_ERROR == Tcl_ListObjGetElements(interp, listItem, &nListCountAttr, &paListAttr))
					{
						return TCL_ERROR;
					}

					if(nListCountAttr < 1)
					{
						return TCL_OK;
					}

					_GridItemInfo itemInfo;
					itemInfo.bLink = FALSE;
					itemInfo.strTitle = ConvertUTFParam(paListAttr[0]->bytes);
					itemInfo.strContent = "";
					itemInfo.bUseTitleFont = 0;
					itemInfo.nImageIndex = -1;
					itemInfo.clrText = Color(0,0,0,0);
					itemInfo.strImage = "";
					itemInfo.strLink = "";
					itemInfo.strLinkAction = "";
					if(nListCountAttr > 1)
					{
						itemInfo.strContent = ConvertUTFParam(paListAttr[1]->bytes);
					}
					if(nListCountAttr > 2)
					{
						CString strTemp = ConvertUTFParam(paListAttr[2]->bytes);
						if(strTemp == "-link")
						{
							// 表示是链接单元格
							itemInfo.bLink = TRUE;
							itemInfo.strLink = itemInfo.strTitle;
							itemInfo.strLinkAction = itemInfo.strContent;
						}else
						{
							itemInfo.bUseTitleFont = atoi(ConvertUTFParam(paListAttr[2]->bytes));
						}
					}
					if(nListCountAttr > 3)
					{
						itemInfo.nImageIndex = atoi(ConvertUTFParam(paListAttr[3]->bytes));
					}
					if(nListCountAttr > 4)
					{
						itemInfo.clrText = CDuiObject::StringToColor(ConvertUTFParam(paListAttr[4]->bytes));
					}
					if(nListCountAttr > 5)
					{
						itemInfo.strImage = ConvertUTFParam(paListAttr[5]->bytes);
					}

					vecItemInfo.push_back(itemInfo);
				}

				i++;
			}
		}

		// 添加行
		nRow = pGridCtrl->InsertRow(nRow, strId, nImageIndex, clrText, strImage,
					nRightImageIndex, strRightImage, nCheck, clrBack);
		// 添加行的单元格内容
		for(size_t i = 0; i < vecItemInfo.size(); i++)
		{
			_GridItemInfo &itemInfo = vecItemInfo.at(i);
			if(itemInfo.bLink)
			{
				pGridCtrl->SetSubItemLink(nRow, i,
					itemInfo.strLink, itemInfo.strLinkAction,
					itemInfo.nImageIndex,
					itemInfo.clrText, itemInfo.strImage);
			}else
			{
				pGridCtrl->SetSubItem(nRow, i,
					itemInfo.strTitle, itemInfo.strContent,
					itemInfo.bUseTitleFont, itemInfo.nImageIndex,
					itemInfo.clrText, itemInfo.strImage);
			}
		}

		CString strResult;
		strResult.Format("%d", nRow);
		Tcl_AppendResult(interp, strResult, (char *)NULL);
	}else
	if(strOpr == "deleterow")	// 删除行
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "deleterow controlObj row");
			return TCL_ERROR;
		}

		int nRow = -1;
		CString strRow = ConvertUTFParam(Tcl_GetString(objv[3]));
		if(!strRow.IsEmpty())
		{
			nRow = atoi(strRow);
		}

		BOOL bRet = pGridCtrl->DeleteRow(nRow);

		CString strResult;
		strResult.Format("%d", bRet);
		Tcl_AppendResult(interp, strResult, (char *)NULL);
	}else
	if(strOpr == "addcontrol")	// 添加单元格子控件
	{
		if(objc < 8)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "addcontrol controlObj row item controlClass controlName pos ?-dialog dialogObj?");
			return TCL_ERROR;
		}

		int nRow = atoi(ConvertUTFParam(Tcl_GetString(objv[3])));
		int nItem = atoi(ConvertUTFParam(Tcl_GetString(objv[4])));
		CString strControlClass = ConvertUTFParam(Tcl_GetString(objv[5]));
		CString strControlName = ConvertUTFParam(Tcl_GetString(objv[6]));
		CString strControlPos = ConvertUTFParam(Tcl_GetString(objv[7]));
		HWND hWnd = NULL;
		if(objc > 9)
		{
			CString strDialog = ConvertUTFParam(Tcl_GetString(objv[8]));
			if(strDialog == "-dialog")
			{
				CString strDialogObj = ConvertUTFParam(Tcl_GetString(objv[9]));
				CDlgBase* pDlg = (CDlgBase*)(atoi(strDialogObj));
				if(pDlg)
				{
					hWnd = pDlg->GetSafeHwnd();
				}
			}
		}

		CControlBase* pControl = pIDuiVisionApp->DuiCreateControlByName(strControlClass, hWnd, NULL);
		if(pControl)
		{
			pControl->SetName(strControlName);
			pControl->SetPosStr(strControlPos);
			BOOL bRet = pGridCtrl->AddSubItemControl(nRow, nItem, pControl);
			if(!bRet)
			{
				delete pControl;
				Tcl_AppendResult(interp, "Create control " + strControlClass + " faild.", (char *) NULL);
				return TCL_ERROR;
			}

			CString strResult;
			strResult.Format("%d", pControl);
			Tcl_AppendResult(interp, strResult, (char *)NULL);
		}
	}else
	if(strOpr == "getrowcount")	// 获取行数
	{
		int nRowCount = pGridCtrl->GetRowCount();

		CString strResult;
		strResult.Format("%d", nRowCount);
		Tcl_AppendResult(interp, strResult, (char *)NULL);
	}else
	if(strOpr == "getrowcheck")	// 获取行的检查框状态
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "deleterow controlObj row");
			return TCL_ERROR;
		}

		int nRow = -1;
		CString strRow = ConvertUTFParam(Tcl_GetString(objv[3]));
		if(!strRow.IsEmpty())
		{
			nRow = atoi(strRow);
		}

		int nCheck = pGridCtrl->GetRowCheck(nRow);

		CString strResult;
		strResult.Format("%d", nCheck);
		Tcl_AppendResult(interp, strResult, (char *)NULL);
	}else
	{
		Tcl_AppendResult(interp, "Unknown operation " + strOpr, (char *) NULL);
		return TCL_ERROR;
	}

	ConvertResultToUTF(interp);
	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:dui::handler
// duiObj ?-handler handlerObj? ?-handlerName handlerTclObjName? ?-control controlName? ?-init?
// add handlerObj
// remove handlerObj
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_DUI_Handler(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc < 3)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "operate ...");
		return TCL_ERROR;
	}

	IDuiVisionApp* pIDuiVisionApp = GetIDuiVisionApp(interp);
	if(pIDuiVisionApp == NULL)
	{
		Tcl_AppendResult(interp, "Get duivisionapp interface fail!", (char *) NULL);
		return TCL_ERROR;
	}

	CString strOpr = ConvertUTFParam(Tcl_GetString(objv[1]));

	if(strOpr == "register")	// 注册事件处理对象
	{
		CString strDuiObject = ConvertUTFParam(Tcl_GetString(objv[2]));
		CDuiObject* pDuiObject = (CDuiObject*)(atoi(strDuiObject));

		CTclDuiHandler* pTclDuiHandler = NULL;
		CString strTDuiHandlerObj = "";
		CString strControlName = "";
		BOOL bInit = FALSE;
		for(int i = 3; i < objc; i++)
		{
			CString strParam = ConvertUTFParam(Tcl_GetString(objv[i]));
			if((strParam.GetLength() > 0) && (strParam[0] == _T('-')))
				strParam.Delete(0, 1);
			strParam.MakeUpper();

			if(strParam == "HANDLER")	// 指定Handler对象
			{
				if((objc - i) < 2)
				{
					Tcl_WrongNumArgs(interp, 1, objv, "wrong # args: lost handlerObj param");
					return TCL_ERROR;
				}
				pTclDuiHandler = (CTclDuiHandler*)(atoi(ConvertUTFParam(Tcl_GetString(objv[i+1]))));
				i++;
			}else
			if(strParam == "HANDLERNAME")	// 指定Tcl事件处理对象名
			{
				if((objc - i) < 2)
				{
					Tcl_WrongNumArgs(interp, 1, objv, "wrong # args: lost handlerName param");
					return TCL_ERROR;
				}
				strTDuiHandlerObj = ConvertUTFParam(Tcl_GetString(objv[i+1]));
				i++;
			}else
			if(strParam == "CONTROL")	// 指定ControlName
			{
				if((objc - i) < 2)
				{
					Tcl_WrongNumArgs(interp, 1, objv, "wrong # args: lost controlName param");
					return TCL_ERROR;
				}
				strControlName = ConvertUTFParam(Tcl_GetString(objv[i+1]));
				i++;
			}else
			if(strParam == "INIT")
			{
				bInit = TRUE;
			}
		}

		pTclDuiHandler = CTclDuiHandler::RegisterDuiHandler(pDuiObject, pTclDuiHandler,
							strTDuiHandlerObj, strControlName, bInit, interp);

		CString strResult;
		strResult.Format("%d", pTclDuiHandler);
		Tcl_AppendResult(interp, strResult, (char *)NULL);
	}else
	if(strOpr == "add")	// 添加事件处理对象
	{
		CString strHandler = ConvertUTFParam(Tcl_GetString(objv[2]));
		CTclDuiHandler::AddDuiHandler((CTclDuiHandler*)(atoi(strHandler)), interp);
	}else
	if(strOpr == "remove")	// 删除事件处理对象
	{
		CString strHandler = ConvertUTFParam(Tcl_GetString(objv[2]));
		CTclDuiHandler::RemoveDuiHandler((CTclDuiHandler*)(atoi(strHandler)), interp);
	}else
	{
		Tcl_AppendResult(interp, "Unknown operation " + strOpr, (char *) NULL);
		return TCL_ERROR;
	}

	ConvertResultToUTF(interp);
	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:dui::dialog
// create create xmlTemplate ?-name dlgName? ?-modal 1|0? ?-handler handlerObj? ?-handlerName handlerTclObjName? ?-control controlName? ?-init?
// show xmlTemplate dlgName isModule
// messagebox text caption ?type? ?-width width? ?-height height?
// showwindow dlgObj show|hide
// notifymsg msg -caption caption ?-delay delayTime? ?-width width? ?-height height? ?type?
// add dlgObj
// remove dlgObj
// get dlgName
// close dlgObj ?-ok|cancel|close|yes|no?
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_DUI_Dialog(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc < 3)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "operate ...");
		return TCL_ERROR;
	}

	IDuiVisionApp* pIDuiVisionApp = GetIDuiVisionApp(interp);
	if(pIDuiVisionApp == NULL)
	{
		Tcl_AppendResult(interp, "Get duivisionapp interface fail!", (char *) NULL);
		return TCL_ERROR;
	}

	CString strOpr = ConvertUTFParam(Tcl_GetString(objv[1]));

	if(strOpr == "create")	// 创建对话框
	{
		CString strXmlTemplate = ConvertUTFParam(Tcl_GetString(objv[2]));

		CString strDlgName = "";
		BOOL bModal = TRUE;
		CTclDuiHandler* pTclDuiHandler = NULL;
		CString strTDuiHandlerObj = "";
		CString strControlName = "";
		BOOL bInit = FALSE;
		for(int i = 3; i < objc; i++)
		{
			CString strParam = ConvertUTFParam(Tcl_GetString(objv[i]));
			if((strParam.GetLength() > 0) && (strParam[0] == _T('-')))
				strParam.Delete(0, 1);
			strParam.MakeUpper();

			if(strParam == "NAME")	// 指定Name
			{
				if((objc - i) < 2)
				{
					Tcl_WrongNumArgs(interp, 1, objv, "wrong # args: lost name param");
					return TCL_ERROR;
				}
				strDlgName = ConvertUTFParam(Tcl_GetString(objv[i+1]));
				i++;
			}else
			if(strParam == "MODAL")	// 是否模态对话框
			{
				if((objc - i) < 2)
				{
					Tcl_WrongNumArgs(interp, 1, objv, "wrong # args: lost isModal param");
					return TCL_ERROR;
				}
				bModal = atoi(ConvertUTFParam(Tcl_GetString(objv[i+1])));
				i++;
			}else
			if(strParam == "HANDLER")	// 指定Handler对象
			{
				if((objc - i) < 2)
				{
					Tcl_WrongNumArgs(interp, 1, objv, "wrong # args: lost handlerObj param");
					return TCL_ERROR;
				}
				pTclDuiHandler = (CTclDuiHandler*)(atoi(ConvertUTFParam(Tcl_GetString(objv[i+1]))));
				i++;
			}else
			if(strParam == "HANDLERNAME")	// 指定Tcl事件处理对象名
			{
				if((objc - i) < 2)
				{
					Tcl_WrongNumArgs(interp, 1, objv, "wrong # args: lost handlerName param");
					return TCL_ERROR;
				}
				strTDuiHandlerObj = ConvertUTFParam(Tcl_GetString(objv[i+1]));
				i++;
			}else
			if(strParam == "CONTROL")	// 指定ControlName
			{
				if((objc - i) < 2)
				{
					Tcl_WrongNumArgs(interp, 1, objv, "wrong # args: lost controlName param");
					return TCL_ERROR;
				}
				strControlName = ConvertUTFParam(Tcl_GetString(objv[i+1]));
				i++;
			}else
			if(strParam == "INIT")
			{
				bInit = TRUE;
			}
		}

		CTclDuiDialogObj* pTclDuiDialogObj = new CTclDuiDialogObj(interp);
		void* pDlg = NULL;
		if(!bModal)
		{
			pDlg = pTclDuiDialogObj->CreateDuiDialog(strXmlTemplate, strDlgName, bModal);
		}else
		{
			pDlg = pTclDuiDialogObj->CreateDuiDialogWithHandler(strXmlTemplate, strDlgName, bModal,
				pTclDuiHandler, strTDuiHandlerObj, strControlName, bInit);
		}

		CString strResult;
		strResult.Format("%d", pDlg);
		Tcl_AppendResult(interp, strResult, (char *)NULL);
	}else
	if(strOpr == "show")	// 创建并显示对话框
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "show xmlTemplate dlgName isModule");
			return TCL_ERROR;
		}

		CString strXmlTemplate = ConvertUTFParam(Tcl_GetString(objv[2]));
		CString strDlgName = ConvertUTFParam(Tcl_GetString(objv[3]));
		BOOL bModule = TRUE;
		if(objc > 4)
		{
			bModule = atoi(ConvertUTFParam(Tcl_GetString(objv[4])));
		}

		int nResult = pIDuiVisionApp->ShowDuiDialog(strXmlTemplate, NULL, strDlgName, bModule);

		// 如果是非模态对话框,则添加到对话框对象列表中
		if(!bModule)
		{
			CDlgBase* pDlg = pIDuiVisionApp->GetDuiDialog(strDlgName);
			CTclDuiDialogObj::AddDuiDialog("", (void*)pDlg, interp);
		}

		CString strResult;
		strResult.Format("%d", nResult);
		Tcl_AppendResult(interp, strResult, (char *)NULL);
	}else
	if(strOpr == "messagebox")	// 显示消息对话框
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "messagebox text caption ?type? ?-width width? ?-height height?");
			return TCL_ERROR;
		}

		CString strText = ConvertUTFParam(Tcl_GetString(objv[2]));
		CString strCaption = ConvertUTFParam(Tcl_GetString(objv[3]));
		UINT uType = 0;
		int nWidth = 0;
		int nHeight = 0;
		for(int i = 4; i < objc; i++)
		{
			CString strParam = ConvertUTFParam(Tcl_GetString(objv[i]));
			if((strParam.GetLength() > 0) && (strParam[0] == _T('-')))
				strParam.Delete(0, 1);
			strParam.MakeUpper();

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
			}else
			if(strParam == "MB_ICONERROR")
			{
				uType |= MB_ICONERROR;
			}else
			if(strParam == "WIDTH")
			{
				if((objc - i) < 2)
				{
					Tcl_WrongNumArgs(interp, 1, objv, "wrong # args: lost width param");
					return TCL_ERROR;
				}
				nWidth = atoi(ConvertUTFParam(Tcl_GetString(objv[i+1])));
				i++;
			}else
			if(strParam == "HEIGHT")
			{
				if((objc - i) < 2)
				{
					Tcl_WrongNumArgs(interp, 1, objv, "wrong # args: lost height param");
					return TCL_ERROR;
				}
				nHeight = atoi(ConvertUTFParam(Tcl_GetString(objv[i+1])));
				i++;
			}
		}

		int nResult = pIDuiVisionApp->DuiMessageBox(NULL, strText, strCaption, uType, nWidth, nHeight);
		CString strResult;
		strResult.Format("%d", nResult);
		Tcl_AppendResult(interp, strResult, (char *)NULL);
	}else
	if(strOpr == "showwindow")	// 显示对话框
	{
		if(objc < 3)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "showwindow dlgObj show|hide");
			return TCL_ERROR;
		}

		CString strDlg = ConvertUTFParam(Tcl_GetString(objv[2]));
		CDlgBase* pDlg = (CDlgBase*)atoi(strDlg);
		if(pDlg == NULL)
		{
			Tcl_AppendResult(interp, "Dialog point is null!", (char *) NULL);
			return TCL_ERROR;
		}

		CString strShow = ConvertUTFParam(Tcl_GetString(objv[3]));
		if(strShow == _T("show"))
		{
			pDlg->ShowWindow(SW_SHOW);
		}else
		if(strShow == _T("hide"))
		{
			pDlg->ShowWindow(SW_HIDE);
		}else
		{
			Tcl_AppendResult(interp, "Unknown operation " + strShow, (char *) NULL);
			return TCL_ERROR;
		}
	}else
	if(strOpr == "notifymsg")	// 显示提示信息框
	{
		CString strMsg = ConvertUTFParam(Tcl_GetString(objv[2]));
		
		CString strCaption = "";
		UINT uNotifyType = 2;
		UINT uIconType = 0;
		int nDelayTime = 0;
		int nWidth = 0;
		int nHeight = 0;
		for(int i = 3; i < objc; i++)
		{
			CString strParam = ConvertUTFParam(Tcl_GetString(objv[i]));
			if((strParam.GetLength() > 0) && (strParam[0] == _T('-')))
				strParam.Delete(0, 1);
			strParam.MakeUpper();

			if(strParam == "MB_ICONINFORMATION")	// 图标
			{
				uIconType |= MB_ICONINFORMATION;
			}else
			if(strParam == "MB_ICONQUESTION")
			{
				uIconType |= MB_ICONQUESTION;
			}else
			if(strParam == "MB_ICONWARNING")
			{
				uIconType |= MB_ICONWARNING;
			}else
			if(strParam == "MB_ICONERROR")
			{
				uIconType |= MB_ICONERROR;
			}else
			if(strParam == "TYPE")
			{
				if((objc - i) < 2)
				{
					Tcl_WrongNumArgs(interp, 1, objv, "wrong # args: lost type param");
					return TCL_ERROR;
				}
				uNotifyType = atoi(ConvertUTFParam(Tcl_GetString(objv[i+1])));
				i++;
			}else
			if(strParam == "CAPTION")
			{
				if((objc - i) < 2)
				{
					Tcl_WrongNumArgs(interp, 1, objv, "wrong # args: lost caption param");
					return TCL_ERROR;
				}
				strCaption = ConvertUTFParam(Tcl_GetString(objv[i+1]));
				i++;
			}else
			if(strParam == "DELAY")
			{
				if((objc - i) < 2)
				{
					Tcl_WrongNumArgs(interp, 1, objv, "wrong # args: lost delayTime param");
					return TCL_ERROR;
				}
				nDelayTime = atoi(ConvertUTFParam(Tcl_GetString(objv[i+1])));
				i++;
			}else
			if(strParam == "WIDTH")
			{
				if((objc - i) < 2)
				{
					Tcl_WrongNumArgs(interp, 1, objv, "wrong # args: lost width param");
					return TCL_ERROR;
				}
				nWidth = atoi(ConvertUTFParam(Tcl_GetString(objv[i+1])));
				i++;
			}else
			if(strParam == "HEIGHT")
			{
				if((objc - i) < 2)
				{
					Tcl_WrongNumArgs(interp, 1, objv, "wrong # args: lost height param");
					return TCL_ERROR;
				}
				nHeight = atoi(ConvertUTFParam(Tcl_GetString(objv[i+1])));
				i++;
			}
		}

		pIDuiVisionApp->AddDuiNotifyMsgTask(strMsg, uIconType | uNotifyType, strCaption, nDelayTime, nWidth, nHeight);
	}else
	if(strOpr == "add")	// 添加对话框
	{
		CString strDlg = ConvertUTFParam(Tcl_GetString(objv[2]));
		CTclDuiDialogObj::AddDuiDialog("", (void*)(atoi(strDlg)), interp);
	}else
	if(strOpr == "remove")	// 删除对话框
	{
		CString strDlg = ConvertUTFParam(Tcl_GetString(objv[2]));
		CTclDuiDialogObj::RemoveDuiDialog((void*)(atoi(strDlg)), interp);
	}else
	if(strOpr == "get")	// 获取对话框
	{
		CString strName = ConvertUTFParam(Tcl_GetString(objv[2]));
		CDlgBase* pDlg = pIDuiVisionApp->GetDuiDialog(strName);
		CString strResult;
		strResult.Format("%d", pDlg);
		Tcl_AppendResult(interp, strResult, (char *)NULL);
	}else
	if(strOpr == "close")	// 关闭对话框
	{
		CString strDlg = ConvertUTFParam(Tcl_GetString(objv[2]));
		CDlgBase* pDlg = (CDlgBase*)atoi(strDlg);
		if(pDlg == NULL)
		{
			Tcl_AppendResult(interp, "Dialog point is null!", (char *) NULL);
			return TCL_ERROR;
		}

		CString strCloseType = "CLOSE";
		for(int i = 3; i < objc; i++)
		{
			CString strParam = ConvertUTFParam(Tcl_GetString(objv[i]));
			if((strParam.GetLength() > 0) && (strParam[0] == _T('-')))
				strParam.Delete(0, 1);
			strParam.MakeUpper();

			strCloseType = strParam;
		}

		if(strCloseType == "OK")
		{
			pDlg->DoOK();
		}else
		if(strCloseType == "CANCEL")
		{
			pDlg->DoCancel();
		}else
		if(strCloseType == "CLOSE")
		{
			pDlg->DoClose();
		}else
		if(strCloseType == "YES")
		{
			pDlg->DoYes();
		}else
		if(strCloseType == "NO")
		{
			pDlg->DoNo();
		}
	}else
	{
		Tcl_AppendResult(interp, "Unknown operation " + strOpr, (char *) NULL);
		return TCL_ERROR;
	}

	ConvertResultToUTF(interp);
	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:dui::timer
// add timerName time
// stop timerName
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_DUI_Timer(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc < 3)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "operate ...");
		return TCL_ERROR;
	}

	IDuiVisionApp* pIDuiVisionApp = GetIDuiVisionApp(interp);
	if(pIDuiVisionApp == NULL)
	{
		Tcl_AppendResult(interp, "Get duivisionapp interface fail!", (char *) NULL);
		return TCL_ERROR;
	}

	CString strOpr = ConvertUTFParam(Tcl_GetString(objv[1]));

	if(strOpr == "add")	// 添加定时器
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "add timerName time");
			return TCL_ERROR;
		}
		CString strTimername = ConvertUTFParam(Tcl_GetString(objv[2]));
		UINT uTimerResolution = (UINT)atoi(ConvertUTFParam(Tcl_GetString(objv[3])));
		pIDuiVisionApp->AddDuiTimer(uTimerResolution, strTimername);
	}else
	if(strOpr == "stop")	// 停止定时器
	{
		CString strTimername = ConvertUTFParam(Tcl_GetString(objv[2]));
		pIDuiVisionApp->RemoveDuiTimer(strTimername);
	}else
	{
		Tcl_AppendResult(interp, "Unknown operation " + strOpr, (char *) NULL);
		return TCL_ERROR;
	}

	ConvertResultToUTF(interp);
	return TCL_OK;
}

//TCL命令声明函数
int TclDuiVision_Init(Tcl_Interp *interp)
{
	// 注册命令

	// DUI资源操作
	Tcl_CreateObjCommand(interp, "dui::resource", Tcl_Cmd_DUI_Resource, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);
	
	// DUI控件操作
	Tcl_CreateObjCommand(interp, "dui::control", Tcl_Cmd_DUI_Control, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// DUI GridCtrl控件操作
	Tcl_CreateObjCommand(interp, "dui::gridctrl", Tcl_Cmd_DUI_GridCtrl, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// DUI事件处理对象操作
	Tcl_CreateObjCommand(interp, "dui::handler", Tcl_Cmd_DUI_Handler, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// DUI对话框操作
	Tcl_CreateObjCommand(interp, "dui::dialog", Tcl_Cmd_DUI_Dialog, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// DUI定时器操作
	Tcl_CreateObjCommand(interp, "dui::timer", Tcl_Cmd_DUI_Timer, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

    return TCL_OK;
}
