#include "stdafx.h"
#include "CProjectManager.h"
#include "NewPrjPage.h"
#include "NewFilePage.h"

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
// 对路径字符串进行处理
/////////////////////////////////////////////////////////////////////////////
void ProcessPath(CString& strPath, CString strBasePath)
{
	CString strTemp;
	if(strPath.Find(_T("PLAT://")) == 0)
	{
		strTemp = strPath.Right(strPath.GetLength()-7);
		//strTemp.Replace("/", "\\");
		if(strTemp.Find(_T("\\")) == 0)
		{	// GetPlatRootPath已经包含了\,因此可以去掉
			strTemp = strTemp.Right(strTemp.GetLength()-1);
		}
		strPath = GetPlatRootPath() + strTemp;
	}else
	{
		strPath.Replace("/", "\\");
		if(strPath.Find(_T(":")) < 0)
		{	// 如果没有路径,表示用平台目录
			strTemp = strPath;
			if(strBasePath == "")
			{
				strPath = GetPlatRootPath() + "Bin\\" + strTemp;
			}else
			{
				// 如果strBasePath非空，则用此作为基础路径
				strPath = strBasePath + "\\" + strTemp;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 功能实现
//////////////////////////////////////////////////////////////////////////
CProjectManager::CProjectManager()
{
}

CProjectManager::~CProjectManager()
{
}

/////////////////////////////////////////////////////////////////////////////
// 安装回调函数
/////////////////////////////////////////////////////////////////////////////
int CProjectManager::InstallHandler(DWORD nEventID, LPVOID lpHandler, DWORD dwRefData)
{
	CVisualComponent::InstallHandler(nEventID, lpHandler, dwRefData);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 卸载回调函数
/////////////////////////////////////////////////////////////////////////////
int CProjectManager::UninstallHandler(DWORD nEventID, LPVOID lpHandler)
{

	CVisualComponent::UninstallHandler(nEventID, lpHandler);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 消息处理
/////////////////////////////////////////////////////////////////////////////
int CProjectManager::ProcessMessage(CVciMessage* pIn, CVciMessage* ppOut)
{
	return 0;
}


//{{VCI_IMPLEMENT_BEGIN
/////////////////////////////////////////////////////////////////////////////
// 启动新建工程向导
/////////////////////////////////////////////////////////////////////////////
int CProjectManager::NewProjectWizard(CString& strPrjVci, CString& strPrjFile)
{
	IPlatUI* pIPlatUI = (IPlatUI*)getIPlatUI();
	if(pIPlatUI == NULL)
	{
		return FALSE;
	}

	// 首先关闭当前工程
	IProject* pOldIProject = (IProject*)pIPlatUI->GetObjectByInstanceName("###project###");
	if(pOldIProject != NULL)
	{
		DLL_LOADSTRING(strConfirm, IDS_CONFIRM_CLOSEPROJECT)
		if(AfxMessageBox(strConfirm, MB_YESNO | MB_ICONQUESTION) != IDYES)
		{
			return FALSE;
		}
		pIPlatUI->ReleaseObjectByInstanceName("###project###");
	}

	// 新建工程对话框
	get_dll_resource();
	CNewPrjPage newPrjDlg;
	if(newPrjDlg.DoModal() != IDOK)
	{
		return FALSE;
	}

	// 工程组件的路径
	CString _strPrjVci = newPrjDlg.m_strPrjVci;
	CString strPrjVciPath = GetPlatRootPath();
	strPrjVciPath += "\\plugins\\";
	strPrjVciPath += _strPrjVci;

	// 创建工程目录
	CString strProjectPath = newPrjDlg.m_strPrjPath;
	CString strProjectName = newPrjDlg.m_strPrjName;
	CreateDirectory(strProjectPath + "\\" + strProjectName, 0);

	CString strTemplate = newPrjDlg.m_strTemplate;

	// 如果设置了脚本解释器和工程脚本,则先调用脚本解释器执行新建工程脚本
	IInterp* pInterp = NULL;
	CString strInterp = newPrjDlg.m_strInterp;
	CString strScript = newPrjDlg.m_strScript;
	if((strInterp.GetLength() != 0) && (strScript.GetLength() != 0))
	{
		if(strInterp == "org.interp.tcl")
		{
			pInterp = (IInterp*)(pIPlatUI->CreateVciObject(strInterp, ""));
			if(pInterp)
			{
				int nRet = pInterp->RunScriptFile(strScript);
				if(!nRet)
				{
					AfxMessageBox(pInterp->GetResult());
				}
				if(nRet)
				{
					nRet = pInterp->RunScriptCommand("TProject project");
				}
				if(!nRet)
				{
					AfxMessageBox(pInterp->GetResult());
				}
				CString strCommand;
				strProjectPath.Replace("\\", "/");
				strProjectPath.Replace("\\", "/");
				strTemplate.Replace("\\", "/");
				strCommand.Format("set project_file [project New \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"]",
					strProjectName, strProjectPath + "/" + strProjectName, newPrjDlg.m_strPrjType,
					newPrjDlg.m_strPrjDesc, newPrjDlg.m_strAuthor,
					strTemplate);
				if(nRet)
				{
					nRet = pInterp->RunScriptCommand(strCommand);
				}
				if(!nRet)
				{
					AfxMessageBox(pInterp->GetResult());
				}
			}
		}
	}

	// 调用工程对象的创建接口
	IProject* pIProject = (IProject*)(pIPlatUI->CreateVciObject(_strPrjVci, "###project###"));
	if(pIProject)
	{
		pIProject->NewProject(strProjectName, strProjectPath + "\\" + strProjectName,
							newPrjDlg.m_strPrjDesc, newPrjDlg.m_strAuthor);
	}

	// 如果设置了脚本解释器和工程脚本,则调用脚本的工程创建之后执行的函数
	if((strInterp.GetLength() != 0) && (strScript.GetLength() != 0) && (pInterp != NULL))
	{
		if(strInterp == "org.interp.tcl") 
		{
			CString strCommand;
			strCommand.Format("project AfterNew \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"",
				strProjectName, strProjectPath + "/" + strProjectName, newPrjDlg.m_strPrjType,
				newPrjDlg.m_strPrjDesc, newPrjDlg.m_strAuthor,
				strTemplate);
			if(!pInterp->RunScriptCommand(strCommand))
			{
				AfxMessageBox(pInterp->GetResult());
			}
		}
	}

	strPrjVci = _strPrjVci;
	strPrjFile = strProjectPath + "\\" + strProjectName + "\\" + strProjectName + "." + newPrjDlg.m_strPrjExt;

	// 调用打开工程的接口
	/*if(pIProject)
	{
		pIProject->OpenProject(strProjectPath + "\\" + strProjectName + "\\" + strProjectName + "." + newPrjDlg.m_strPrjExt);
	}*/

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 启动新建文件向导
/////////////////////////////////////////////////////////////////////////////
int CProjectManager::NewFileWizard()
{
	get_dll_resource();
	CNewFilePage newFileDlg;
	if(newFileDlg.DoModal() != IDOK)
	{
		return 0;
	}

	// 如果有打开的工程，则调用工程的新建文件接口

	// 否则直接按照模版来新建文件

	return 0;
}
//}}VCI_IMPLEMENT_END

//////////////////////////////////////////////////////////////////////////
// 接口实现
//////////////////////////////////////////////////////////////////////////
INTERFACE_IMPLEMENT(ProjectManager)
CONTROL_INTERFACE_IMPLEMENT(ProjectManager)

//{{ 注意：!!!这里是保留的重要函数，不可删除!!!
// 获取接口的额外信息
LPVOID __stdcall CProjectManager::XProjectManager::GetExtraInfo(LPVOID lpRefData)
{
	CProjectManager *pObj = GET_INTERFACE_OBJECT(ProjectManager);
	return pObj;
}
//}} 注意：!!!这里是保留的重要函数，不可删除!!!

//{{VCI_INTERFACE_IMPLEMENT_BEGIN
// 启动新建工程向导
int __stdcall CProjectManager::XProjectManager::NewProjectWizard(CString& strPrjVci, CString& strPrjFile)
{
	CProjectManager *pObj = GET_INTERFACE_OBJECT(ProjectManager);
	return pObj->NewProjectWizard(strPrjVci, strPrjFile);
}

// 启动新建文件向导
int __stdcall CProjectManager::XProjectManager::NewFileWizard()
{
	CProjectManager *pObj = GET_INTERFACE_OBJECT(ProjectManager);
	return pObj->NewFileWizard();
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
