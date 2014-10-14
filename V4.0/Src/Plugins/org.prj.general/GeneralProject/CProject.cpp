#include "stdafx.h"
#include "CProject.h"

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
// 获取插件路径
/////////////////////////////////////////////////////////////////////////////
CString GetPlugInRootPath()
{
	//获取平台路径
	TCHAR szFullPath[MAX_PATH];
	TCHAR szdrive[_MAX_DRIVE];
	TCHAR szdir[_MAX_DIR];
	::GetModuleFileName(GetDllInstance(), szFullPath, MAX_PATH);
	_splitpath(szFullPath, szdrive, szdir, NULL, NULL);
	CString szPath;
	szPath.Format(_T("%s%s"), szdrive, szdir);

	return szPath;
}

static CProject* g_Project = NULL;

IPlatUI* gGetIPlatUI()
{
	if(g_Project)
	{
		IPlatUI* pIPlatUI = (IPlatUI*)(g_Project->getIPlatUI());
		return pIPlatUI;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// 功能实现
//////////////////////////////////////////////////////////////////////////
CProject::CProject()
{
	g_Project = this;
}

CProject::~CProject()
{
	g_Project = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 安装回调函数
/////////////////////////////////////////////////////////////////////////////
int CProject::InstallHandler(DWORD nEventID, LPVOID lpHandler, DWORD dwRefData)
{
	CVisualComponent::InstallHandler(nEventID, lpHandler, dwRefData);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 卸载回调函数
/////////////////////////////////////////////////////////////////////////////
int CProject::UninstallHandler(DWORD nEventID, LPVOID lpHandler)
{

	CVisualComponent::UninstallHandler(nEventID, lpHandler);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 消息处理
/////////////////////////////////////////////////////////////////////////////
int CProject::ProcessMessage(CVciMessage* pIn, CVciMessage* ppOut)
{
	return 0;
}


//{{VCI_IMPLEMENT_BEGIN
// 新建工程
int __stdcall CProject::NewProject(LPCTSTR lpszProjectName, LPCTSTR lpszProjectPath, LPCTSTR lpszProjectDesc, LPCTSTR lpszAuthor)
{
	// TODO: Add your code here

	return trpOk;
}

// 打开工程
int __stdcall CProject::OpenProject(LPCTSTR lpszProjectFile)
{
	// TODO: Add your code here

	return trpOk;
}

// 保存工程
int __stdcall CProject::SaveProject()
{
	// TODO: Add your code here

	return trpOk;
}

// 执行工程
int __stdcall CProject::RunProject()
{
	// TODO: Add your code here

	return trpOk;
}

// 关闭工程
int __stdcall CProject::CloseProject(BOOL bForce)
{
	// TODO: Add your code here

	return trpOk;
}

// 编译工程
int __stdcall CProject::BuildProject()
{
	// TODO: Add your code here

	return trpOk;
}

// 重新编译工程
int __stdcall CProject::RebuildProject()
{
	// TODO: Add your code here

	return trpOk;
}

// 清除工程编译信息
int __stdcall CProject::CleanProject()
{
	// TODO: Add your code here

	return trpOk;
}

// 显示工程信息
int __stdcall CProject::ProjectInformation()
{
	// TODO: Add your code here

	return trpOk;
}

// 刷新属性窗口的工程信息
int __stdcall CProject::RefreshProjectProperty()
{
	// TODO: Add your code here

	return trpOk;
}

// 工程属性设置
int __stdcall CProject::ProjectPropertyPage(CVciPropertyPageArray& aPropertyPage)
{
	// TODO: Add your code here

	return aPropertyPage.GetSize();
}

// 获取打开的工程列表
int __stdcall CProject::GetOpenProjectList(CStringArray& asProjectFile)
{
	// TODO: Add your code here

	return 1;
}

// 获取当前激活的工程
int __stdcall CProject::GetActiveProject(CString& strProject)
{
	// TODO: Add your code here

	return trpOk;
}

// 设置激活的工程
int __stdcall CProject::SetActiveProject(LPCTSTR lpszProject)
{
	// TODO: Add your code here

	return trpOk;
}

// 获取工程信息
int __stdcall CProject::GetProjectInfo(LPCTSTR lpszProject, CString& strProjectDesc, CString& strProjectFile, CString& strProjectPath)
{
	// TODO: Add your code here

	return trpOk;
}

// 获取工程状态
int __stdcall CProject::GetProjectState(LPCTSTR lpszProject)
{
	// TODO: Add your code here

	return PROJECT_IDLE;
}

// 注册菜单
int __stdcall CProject::RegisterMenu(CMenu* pMenu, COWMMenuArray& aOWMMenus, int nOWMIdStart)
{
	// TODO: Add your code here

	return 0;
}

// 卸载菜单
int __stdcall CProject::UnregisterMenu(COWMMenuArray& aOWMMenus)
{
	// TODO: Add your code here

	return 0;
}

// 显示DockingPane窗口时调用的函数
CWnd* __stdcall CProject::OnShowDockingPane(int nID, CWnd* pParentWnd, LPCTSTR lpszParam)
{
	// TODO: Add your code here
	
	return NULL;
}

// 初始化代码定义库
int __stdcall CProject::InitCodeDefineLibrary(LPCTSTR lpszProject)
{
	// TODO: Add your code here

	return 0;
}

// 加载代码定义库
int __stdcall CProject::LoadCodeDefineLibrary(LPCTSTR lpszProject)
{
	// TODO: Add your code here
	// 如果库不存在,则先进行库的初始化

	return 0;
}

// 查找代码定义列表(返回值表示个数)
int __stdcall CProject::GetCodeDefine(LPCTSTR lpszCode, CCodePositionArray& asCodePosition)
{
	// TODO: Add your code here

	return 0;
}

// 查找代码引用列表(返回值表示个数)
int __stdcall CProject::GetCodeReference(LPCTSTR lpszCode, CCodePositionArray& asCodePosition)
{
	// TODO: Add your code here

	return 0;
}
//}}VCI_IMPLEMENT_END

//////////////////////////////////////////////////////////////////////////
// 接口实现
//////////////////////////////////////////////////////////////////////////
INTERFACE_IMPLEMENT(Project)
CONTROL_INTERFACE_IMPLEMENT(Project)

//{{ 注意：!!!这里是保留的重要函数，不可删除!!!
// 获取接口的额外信息
LPVOID __stdcall CProject::XProject::GetExtraInfo(LPVOID lpRefData)
{
	CProject *pObj = GET_INTERFACE_OBJECT(Project);
	return pObj;
}
//}} 注意：!!!这里是保留的重要函数，不可删除!!!

//{{VCI_INTERFACE_IMPLEMENT_BEGIN
	PROJECT_INTERFACE_IMPLEMENT
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
