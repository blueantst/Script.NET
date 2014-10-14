////////////////////////////////////////////////////////////////////////////
//	File:		PlugIn.h
//	Version:	1.0.0.0
//	Created:	2005-07-08
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	PlugIn Interface
////////////////////////////////////////////////////////////////////////////
#ifndef __PLUGIN_H_
#define __PLUGIN_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 插件当前的加载状态
enum {
	PLUGIN_UNLOAD,		// 插件未加载任何信息
	PLUGIN_LOAD_NAME,	// 插件加载了名字和路径
	PLUGIN_LOAD_INFO,	// 插件加载了配置文件的信息
	PLUGIN_LOAD_LIB,	// 插件加载了执行体
};

//
// VCI实体对象
//
class CVciObject : public CObject
{
public:
	CVciObject()
	{
		m_pVciObj		= NULL;
		m_pIVciControl	= NULL;
		m_strInstName	= "";
		m_nRefCount		= 0;
	}
	~CVciObject(){};

	CVciObject(const CVciObject& other)
	{
		m_pVciObj		= other.m_pVciObj;
		m_pIVciControl	= other.m_pIVciControl;
		m_strInstName	= other.m_strInstName;
		m_nRefCount		= other.m_nRefCount;
	}

	CVciObject& operator = (const CVciObject& other)
	{
		m_pVciObj		= other.m_pVciObj;
		m_pIVciControl	= other.m_pIVciControl;
		m_strInstName	= other.m_strInstName;
		m_nRefCount		= other.m_nRefCount;
		return *this;
	}

public:
	LPVOID	m_pVciObj;		// VCI对象指针
	LPVOID	m_pIVciControl;	// VCI控制接口
	CString	m_strInstName;	// VCI对象实例名
	int		m_nRefCount;	// 对象引用计数
};

typedef CArray<CVciObject, CVciObject&> CVciObjectArray;

//
// 文档扩展点信息类
//
class COwmDocExtend : public CObject
{
public:
	COwmDocExtend();
	~COwmDocExtend();

	COwmDocExtend(const COwmDocExtend& other)
	{
		m_strDocName	= other.m_strDocName;
		m_strDocId		= other.m_strDocId;
		m_strOwmId		= other.m_strOwmId;
		m_strIcon		= other.m_strIcon;
		m_bEnableLink	= other.m_bEnableLink;
		m_arDocExt.RemoveAll();
		m_arExecuteModuleType.RemoveAll();
		m_arExecuteModule.RemoveAll();
		m_arDocIconType.RemoveAll();
		m_arDocIconFile.RemoveAll();
		int i;
		for(i=0; i<other.m_arDocExt.GetSize(); i++)
		{
			m_arDocExt.Add(other.m_arDocExt[i]);
		}
		for(i=0; i<other.m_arExecuteModuleType.GetSize(); i++)
		{
			m_arExecuteModuleType.Add(other.m_arExecuteModuleType[i]);
		}
		for(i=0; i<other.m_arExecuteModule.GetSize(); i++)
		{
			m_arExecuteModule.Add(other.m_arExecuteModule[i]);
		}
		for(i=0; i<other.m_arDocIconType.GetSize(); i++)
		{
			m_arDocIconType.Add(other.m_arDocIconType[i]);
		}
		for(i=0; i<other.m_arDocIconFile.GetSize(); i++)
		{
			m_arDocIconFile.Add(other.m_arDocIconFile[i]);
		}
	}

	COwmDocExtend& operator = (const COwmDocExtend& other)
	{
		m_strDocName	= other.m_strDocName;
		m_strDocId		= other.m_strDocId;
		m_strOwmId		= other.m_strOwmId;
		m_strIcon		= other.m_strIcon;
		m_bEnableLink	= other.m_bEnableLink;
		m_arDocExt.RemoveAll();
		m_arExecuteModuleType.RemoveAll();
		m_arExecuteModule.RemoveAll();
		m_arDocIconType.RemoveAll();
		m_arDocIconFile.RemoveAll();
		int i;
		for(i=0; i<other.m_arDocExt.GetSize(); i++)
		{
			m_arDocExt.Add(other.m_arDocExt[i]);
		}
		for(i=0; i<other.m_arExecuteModuleType.GetSize(); i++)
		{
			m_arExecuteModuleType.Add(other.m_arExecuteModuleType[i]);
		}
		for(i=0; i<other.m_arExecuteModule.GetSize(); i++)
		{
			m_arExecuteModule.Add(other.m_arExecuteModule[i]);
		}
		for(i=0; i<other.m_arDocIconType.GetSize(); i++)
		{
			m_arDocIconType.Add(other.m_arDocIconType[i]);
		}
		for(i=0; i<other.m_arDocIconFile.GetSize(); i++)
		{
			m_arDocIconFile.Add(other.m_arDocIconFile[i]);
		}
		return *this;
	}

public:
	CString	m_strDocName;	// 文档名
	CString	m_strDocId;		// 文档ID
	CString	m_strOwmId;		// 所属OWM模块ID
	CString	m_strIcon;		// 图标文件
	BOOL	m_bEnableLink;	// 是否可以建立关联
	CStringArray m_arDocExt;// 文件后缀列表
	CStringArray m_arExecuteModuleType;	// 可以执行此文件的其他模块类型
	CStringArray m_arExecuteModule;	// 可以执行此文件的其他模块
	CStringArray m_arDocIconType;	// 文档图标类型
	CStringArray m_arDocIconFile;	// 文档图标文件
};

typedef CArray<COwmDocExtend, COwmDocExtend&> COwmDocExtendArray;

//
// DockingPane信息类
//
class COwmDockingPane : public CObject
{
public:
	COwmDockingPane()
	{
		m_bFileDefined = FALSE;
		m_strName	= "";
		m_nId		= 0;
		m_nDirection= 0;
		m_strIcon	= "";
		m_nAttachType= PANE_ATTACH_WND;
		m_bAutoCreate= FALSE;
		m_bProjectCreate= FALSE;
		m_bSave		= FALSE;
		m_pWnd		= NULL;
		m_strOwmId	= "";
	}
	~COwmDockingPane() {};

	COwmDockingPane(const COwmDockingPane& other)
	{
		m_bFileDefined	= other.m_bFileDefined;
		m_strName		= other.m_strName;
		m_nId			= other.m_nId;
		m_nDirection	= other.m_nDirection;
		m_strIcon		= other.m_strIcon;
		m_nAttachType	= other.m_nAttachType;
		m_bAutoCreate	= other.m_bAutoCreate;
		m_bProjectCreate= other.m_bProjectCreate;
		m_bSave			= other.m_bSave;
		m_pWnd			= other.m_pWnd;
		m_strOwmId		= other.m_strOwmId;
	}

	COwmDockingPane& operator = (const COwmDockingPane& other)
	{
		m_bFileDefined	= other.m_bFileDefined;
		m_strName		= other.m_strName;
		m_nId			= other.m_nId;
		m_nDirection	= other.m_nDirection;
		m_strIcon		= other.m_strIcon;
		m_nAttachType	= other.m_nAttachType;
		m_bAutoCreate	= other.m_bAutoCreate;
		m_bProjectCreate= other.m_bProjectCreate;
		m_bSave			= other.m_bSave;
		m_pWnd			= other.m_pWnd;
		m_strOwmId		= other.m_strOwmId;
		return *this;
	}

public:
	BOOL	m_bFileDefined;	// 是否在配置文件中预定义的(有些DockingPane是动态创建的,例如Tree窗口)
	CString	m_strName;		// Pane名
	int		m_nId;			// PaneID
	int		m_nDirection;	// 显示位置
	CString	m_strIcon;		// 图标文件位置
	int		m_nAttachType;	// 关联的窗口类型
	BOOL	m_bAutoCreate;	// 是否在模块加载时自动创建
	BOOL	m_bProjectCreate;// 是否在打开工程时自动创建
	BOOL	m_bSave;		// 是否在平台关闭时保存位置
	CWnd*	m_pWnd;
	CString	m_strOwmId;		// 所属OWM模块ID
};

typedef CArray<COwmDockingPane, COwmDockingPane&> COwmDockingPaneArray;

//
// 透视图信息类
//
class COwmPerspective : public CObject
{
public:
	COwmPerspective();
	~COwmPerspective();

	COwmPerspective(const COwmPerspective& other)
	{
		m_strName		= other.m_strName;
		m_strId			= other.m_strId;
		m_strOwmId		= other.m_strOwmId;
		m_strViewName	= other.m_strViewName;
		m_arDockView.RemoveAll();
		for(int i=0; i<other.m_arDockView.GetSize(); i++)
		{
			m_arDockView.Add(other.m_arDockView[i]);
		}
	}

	COwmPerspective& operator = (const COwmPerspective& other)
	{
		m_strName		= other.m_strName;
		m_strId			= other.m_strId;
		m_strOwmId		= other.m_strOwmId;
		m_strViewName	= other.m_strViewName;
		m_arDockView.RemoveAll();
		for(int i=0; i<other.m_arDockView.GetSize(); i++)
		{
			m_arDockView.Add(other.m_arDockView[i]);
		}
		return *this;
	}

public:
	CString	m_strName;		// 透视图名
	CString	m_strId;		// 透视图ID
	CString	m_strOwmId;		// 所属OWM模块ID
	CString	m_strViewName;	// view名
	CStringArray m_arDockView;// 停靠视图列表
};

typedef CArray<COwmPerspective, COwmPerspective&> COwmPerspectiveArray;


//
// VCI扩展参数类
//
class CVciParameter : public CObject
{
public:
	CVciParameter();
	~CVciParameter();

	CVciParameter(const CVciParameter& other)
	{
		m_strName		= other.m_strName;
		m_strValue		= other.m_strValue;
		m_strDefaultValue= other.m_strDefaultValue;
	}

	CVciParameter& operator = (const CVciParameter& other)
	{
		m_strName		= other.m_strName;
		m_strValue		= other.m_strValue;
		m_strDefaultValue= other.m_strDefaultValue;
		return *this;
	}

public:
	CString	m_strName;			// 参数名
	CString	m_strValue;			// 参数值
	CString	m_strDefaultValue;	// 缺省值
};

typedef CArray<CVciParameter, CVciParameter&> CVciParameterArray;

//
// VCI扩展菜单类
//
class CVciMenu : public CObject
{
public:
	CVciMenu();
	~CVciMenu();

	CVciMenu(const CVciMenu& other)
	{
		m_strMainMenu	= other.m_strMainMenu;
		m_strSubMenu	= other.m_strSubMenu;
		m_strAction		= other.m_strAction;
	}

	CVciMenu& operator = (const CVciMenu& other)
	{
		m_strMainMenu	= other.m_strMainMenu;
		m_strSubMenu	= other.m_strSubMenu;
		m_strAction		= other.m_strAction;
		return *this;
	}

public:
	CString	m_strMainMenu;			// 主菜单
	CString	m_strSubMenu;			// 子菜单
	CString	m_strAction;			// 动作
};

typedef CArray<CVciMenu, CVciMenu&> CVciMenuArray;


//
// 插件信息类
//
class CPlugIn : public CObject
{
public:
	CPlugIn();	
	~CPlugIn();

	CPlugIn(CString strPath);

	CPlugIn& operator = (CPlugIn& other)
	{
		m_nLoadState= other.m_nLoadState;
		m_nOwmId	= other.m_nOwmId;
		m_strId		= other.m_strId;
		m_strName	= other.m_strName;
		m_strVersion= other.m_strVersion;
		m_strProvider= other.m_strProvider;
		m_strPath	= other.m_strPath;
		m_strType	= other.m_strType;
		m_strClass	= other.m_strClass;
		m_strClassVersion= other.m_strClassVersion;
		m_strProcess= other.m_strProcess;
		m_strFile	= other.m_strFile;
		m_strInterfaceFile = other.m_strInterfaceFile;
		m_hVciHandle= other.m_hVciHandle;
		m_pVciObj	= other.m_pVciObj;
		m_pIVciControl=other.m_pIVciControl;
		m_nRefCount	= other.m_nRefCount;
		m_nType		= other.m_nType;
		m_bAutoCreate	= other.m_bAutoCreate;
		m_strAutoCreateInstanceName	= other.m_strAutoCreateInstanceName;
		m_pDocTemplate	=other.m_pDocTemplate;
		m_pControlBar	=other.m_pControlBar;
		m_strInitParam	= other.m_strInitParam;
		m_strDoneParam	= other.m_strDoneParam;
		m_strAboutPage	= other.m_strAboutPage;
		int i;
		for(i=0; i<other.m_arDocExtend.GetSize(); i++)
		{
			m_arDocExtend.Add(other.m_arDocExtend[i]);
		}
		for(i=0; i<other.m_arDockingPane.GetSize(); i++)
		{
			m_arDockingPane.Add(other.m_arDockingPane[i]);
		}
		for(i=0; i<other.m_arParameter.GetSize(); i++)
		{
			m_arParameter.Add(other.m_arParameter[i]);
		}
		for(i=0; i<other.m_arMenu.GetSize(); i++)
		{
			m_arMenu.Add(other.m_arMenu[i]);
		}
		return *this;
	}

	int  GetLoadState() { return m_nLoadState; }

	void* OpenVciLib(CString strInterface);
	BOOL CloseVci();
	BOOL CanClose();
	BOOL HasLoad() { return m_hVciHandle != NULL; }

	void* CreateObject(CString strInstName = "");
	BOOL ReleaseObject(LPVOID pVciObj, BOOL bForceDelete = FALSE);
	BOOL ReleaseObject(CString strInstName);
	void* GetObjectByInstanceName(CString strInstName);
	void* GetObjectControlPtrByInstancePtr(LPVOID	pVciObj);
	BOOL IsInstancePtr(LPVOID	pVciObj);

	BOOL LoadInfo();
	void ShowAboutBox();
	CTime GetPlugInBuildTime();
	CString GetPlugInGUID();
	BOOL LoadPlugIn(BOOL bForce = FALSE);
	BOOL LoadVciPlugIn();
	BOOL LoadOwmPlugIn();
	BOOL LoadDockingPane(int nID, BOOL bProjectLoad = FALSE);
	BOOL IsVciOwm() { return (m_strType == "owm"); }
	IVciOwm* GetIVciOwm() { return (IVciOwm*)m_pVciObj; }
	IProject* GetIProject() { return (IProject*)m_pVciObj; }
	BOOL IsVciInterp() { return (m_strType == "interp"); }
	IInterp* GetIInterp() { return (IInterp*)m_pVciObj; }
	BOOL IsVciLicense() { return (m_strType == "license"); }

	int  EnumParameters(CStringArray& asName);
	BOOL GetParameter(CString strName, CString& strValue, CString& strDefaultValue);
	BOOL SetParameter(CString strName, CString strValue);

	BOOL AddOwmSubMenu(CString strMainMenu, CString strSubMenu, CString strAction);

	// OWM函数
	void NewOwmFrame(LPCTSTR lpszPath = NULL);
	void CloseOwmFrame();
	BOOL NewOwmControlBar();
	BOOL CloseOwmControlBar();
	BOOL ShowOwmControlBar(BOOL bShow);
	BOOL ShowOwmView(BOOL bShow, BOOL bActive);
	BOOL CanCloseOwm();
	BOOL CloseOwm();
	CDocument* OpenDocument(LPCTSTR lpszFileName);
	int  RegisterOption(COWMOptionArray& aOWMOption);
	int	 RegisterPropertyPage(CVciPropertyPageArray& aPropertyPage);

	// Project函数
	IProject* CreateProjectObject(CString strInstName);
	BOOL ReleaseProjectObject(LPVOID pVciObj, BOOL bForceDelete = FALSE);

	// Interp函数
	BOOL CanCloseInterp();

	// 消息处理
	int  SendMessage(CVciMessage* pIn, CVciMessage* ppOut);

public:
	int		m_nLoadState;		// 插件当前的加载状态
	int		m_nOwmId;			// OWM模块ID
	CString	m_strId;			// 插件ID
	CString	m_strName;			// 插件名
	CString	m_strVersion;		// 插件版本
	CString	m_strProvider;		// 开发商
	CString	m_strPath;			// 插件路径
	CString	m_strType;			// 插件类型
	CString	m_strClass;			// 插件类ID
	CString	m_strClassVersion;	// 插件类版本
	CString	m_strProcess;		// 插件所在进程
	CString m_strFile;			// DLL文件
	CString	m_strInterfaceFile;	// 头文件
	HINSTANCE m_hVciHandle;		// 保存VCI dll的句柄
	CVciObjectArray	m_aVciObj;	// VCI实例对象数组
	LPVOID	m_pVciObj;			// VCI组件对象
	LPVOID	m_pIVciControl;		// VCI控制接口
	int		m_nRefCount;		// 引用计数(用于OWM模块的窗口计数)
	UINT	m_nType;			// OWM模块的类型信息(XML中获取)
	BOOL	m_bAutoCreate;		// 是否自动创建实例
	CString	m_strAutoCreateInstanceName;	// 自动创建实例的实例名
	CString	m_strInitParam;		// 初始化参数
	CString	m_strDoneParam;		// 结束参数
	CString	m_strAboutPage;		// 关于页面位置
	CMultiDocTemplate* m_pDocTemplate; // OWM文档模板指针
	CControlBar* m_pControlBar;	// OWM Control Bar
	COwmDocExtendArray m_arDocExtend;	// 文档扩展点数组
	COwmDockingPaneArray m_arDockingPane;	// DockingPane数组
	CVciParameterArray	m_arParameter;	// 扩展参数
	CVciMenuArray		m_arMenu;	// 扩展菜单
};

typedef CArray<CPlugIn, CPlugIn&> CPlugInArray;


#endif //__PLUGIN_H_