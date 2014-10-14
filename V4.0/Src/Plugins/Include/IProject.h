////////////////////////////////////////////////////////////////////////////
//	File:		IInterp.h
//	Version:	2.0.0.0
//	Created:	2003-07-20
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	interface of project
////////////////////////////////////////////////////////////////////////////
#if !defined(__VCI_INTERFACE_IProject_H_INCLUDED__)
#define __VCI_INTERFACE_IProject_H_INCLUDED__


#define IID_IProject "IProject"

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

//
// 代码位置
//
class CCodePosition : public CObject
{
public:
	CCodePosition()
	{
		m_strCodeFile = _T("");
		m_nCodeLine = -1;
	}
	~CCodePosition() {};

	CCodePosition(const CCodePosition& other)
	{
		m_strCodeFile	= other.m_strCodeFile;
		m_nCodeLine		= other.m_nCodeLine;
	}

	CCodePosition& operator = (const CCodePosition& other)
	{
		m_strCodeFile	= other.m_strCodeFile;
		m_nCodeLine		= other.m_nCodeLine;
		return *this;
	}

public:
	CString	m_strCodeFile;			// 文件
	int		m_nCodeLine;			// 代码行
};

typedef CArray<CCodePosition, CCodePosition&> CCodePositionArray;


interface IProject : public IUnknown
{
	//{{ 注意：!!!这里是保留的重要函数，不可删除!!!
	// 获取接口的额外信息
	virtual LPVOID __stdcall GetExtraInfo(LPVOID lpRefData) = 0;
	//}} 注意：!!!这里是保留的重要函数，不可删除!!!

	//{{VCI_INTERFACE_BEGIN
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
	// 获取工程信息
	virtual int __stdcall GetProjectInfo(LPCTSTR lpszProject, CString& strProjectDesc, CString& strProjectFile, CString& strProjectPath) = 0;
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
	//}}VCI_INTERFACE_END
};


// Project接口定义辅助宏
#define PROJECT_INTERFACE_DEFINE \
	virtual int __stdcall NewProject(LPCTSTR lpszProjectName, LPCTSTR lpszProjectPath, LPCTSTR lpszProjectDesc, LPCTSTR lpszAuthor);	\
	virtual int __stdcall OpenProject(LPCTSTR lpszProjectFile);	\
	virtual int __stdcall SaveProject();	\
	virtual int __stdcall RunProject();	\
	virtual int __stdcall CloseProject(BOOL bForce);	\
	virtual int __stdcall BuildProject();	\
	virtual int __stdcall RebuildProject();	\
	virtual int __stdcall CleanProject();	\
	virtual int __stdcall ProjectInformation();	\
	virtual int __stdcall RefreshProjectProperty();	\
	virtual int __stdcall ProjectPropertyPage(CVciPropertyPageArray& aPropertyPage);	\
	virtual int __stdcall GetOpenProjectList(CStringArray& asProjectFile);	\
	virtual int __stdcall GetActiveProject(CString& strProject);	\
	virtual int __stdcall SetActiveProject(LPCTSTR lpszProject);	\
	virtual int __stdcall GetProjectInfo(LPCTSTR lpszProject, CString& strProjectDesc, CString& strProjectFile, CString& strProjectPath);	\
	virtual int __stdcall GetProjectState(LPCTSTR lpszProject);	\
	virtual int __stdcall RegisterMenu(CMenu* pMenu, COWMMenuArray& aOWMMenus, int nOWMIdStart);	\
	virtual int __stdcall UnregisterMenu(COWMMenuArray& aOWMMenus);	\
	virtual CWnd* __stdcall OnShowDockingPane(int nID, CWnd* pParentWnd, LPCTSTR lpszParam);	\
	virtual int __stdcall InitCodeDefineLibrary(LPCTSTR lpszProject);	\
	virtual int __stdcall LoadCodeDefineLibrary(LPCTSTR lpszProject);	\
	virtual int __stdcall GetCodeDefine(LPCTSTR lpszCode, CCodePositionArray& asCodePosition);	\
	virtual int __stdcall GetCodeReference(LPCTSTR lpszCode, CCodePositionArray& asCodePosition);


// Project接口实现辅助宏
#define PROJECT_INTERFACE_IMPLEMENT \
int __stdcall CProject::XProject::NewProject(LPCTSTR lpszProjectName, LPCTSTR lpszProjectPath, LPCTSTR lpszProjectDesc, LPCTSTR lpszAuthor)	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->NewProject(lpszProjectName, lpszProjectPath, lpszProjectDesc, lpszAuthor);	\
}	\
int __stdcall CProject::XProject::OpenProject(LPCTSTR lpszProjectFile)	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->OpenProject(lpszProjectFile);	\
}	\
int __stdcall CProject::XProject::SaveProject()	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->SaveProject();	\
}	\
int __stdcall CProject::XProject::RunProject()	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->RunProject();	\
}	\
int __stdcall CProject::XProject::CloseProject(BOOL bForce)	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->CloseProject(bForce);	\
}	\
int __stdcall CProject::XProject::BuildProject()	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->BuildProject();	\
}	\
int __stdcall CProject::XProject::RebuildProject()	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->RebuildProject();	\
}	\
int __stdcall CProject::XProject::CleanProject()	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->CleanProject();	\
}	\
int __stdcall CProject::XProject::ProjectInformation()	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->ProjectInformation();	\
}	\
int __stdcall CProject::XProject::RefreshProjectProperty()	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->RefreshProjectProperty();	\
}	\
int __stdcall CProject::XProject::ProjectPropertyPage(CVciPropertyPageArray& aPropertyPage)	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->ProjectPropertyPage(aPropertyPage);	\
}	\
int __stdcall CProject::XProject::GetOpenProjectList(CStringArray& asProjectFile)	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->GetOpenProjectList(asProjectFile);	\
}	\
int __stdcall CProject::XProject::GetActiveProject(CString& strProject)	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->GetActiveProject(strProject);	\
}	\
int __stdcall CProject::XProject::SetActiveProject(LPCTSTR lpszProject)	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->SetActiveProject(lpszProject);	\
}	\
int __stdcall CProject::XProject::GetProjectInfo(LPCTSTR lpszProject, CString& strProjectDesc, CString& strProjectFile, CString& strProjectPath)	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->GetProjectInfo(lpszProject, strProjectDesc, strProjectFile, strProjectPath);	\
}	\
int __stdcall CProject::XProject::GetProjectState(LPCTSTR lpszProject)	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->GetProjectState(lpszProject);	\
}	\
int __stdcall CProject::XProject::RegisterMenu(CMenu* pMenu, COWMMenuArray& aOWMMenus, int nOWMIdStart)	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->RegisterMenu(pMenu, aOWMMenus, nOWMIdStart);	\
}	\
int __stdcall CProject::XProject::UnregisterMenu(COWMMenuArray& aOWMMenus)	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->UnregisterMenu(aOWMMenus);	\
}	\
CWnd* __stdcall CProject::XProject::OnShowDockingPane(int nID, CWnd* pParentWnd, LPCTSTR lpszParam)	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->OnShowDockingPane(nID, pParentWnd, lpszParam);	\
}	\
int __stdcall CProject::XProject::InitCodeDefineLibrary(LPCTSTR lpszProject)	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->InitCodeDefineLibrary(lpszProject);	\
}	\
int __stdcall CProject::XProject::LoadCodeDefineLibrary(LPCTSTR lpszProject)	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->LoadCodeDefineLibrary(lpszProject);	\
}	\
int __stdcall CProject::XProject::GetCodeDefine(LPCTSTR lpszCode, CCodePositionArray& asCodePosition)	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->GetCodeDefine(lpszCode, asCodePosition);	\
}	\
int __stdcall CProject::XProject::GetCodeReference(LPCTSTR lpszCode, CCodePositionArray& asCodePosition)	\
{	\
	CProject *pObj = GET_INTERFACE_OBJECT(Project);	\
	return pObj->GetCodeReference(lpszCode, asCodePosition);	\
}


// 添加一个扩展菜单项在主菜单的某个菜单项下面(调用平台接口)
#define	PLATUI_ADDPRJMENUITEM(mainmenu, menuid, menuindex, proc, updateproc)	\
	{	\
		get_dll_resource();	\
		CMenu* pmenu = new CMenu();	\
		pmenu->LoadMenu(menuid);	\
		reset_dll_resource();	\
		IPlatUI* pIPlatUI = gGetIPlatUI();	\
		pIPlatUI->AddSubMenu(mainmenu, menuindex, pmenu, proc, updateproc);	\
		delete pmenu;	\
	}	\


#endif // !defined(__VCI_INTERFACE_IProject_H_INCLUDED__)
