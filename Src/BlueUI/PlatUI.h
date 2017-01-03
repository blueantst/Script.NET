#if !defined(AFX_PLATUI_H__48B86E81_729F_11D4_9671_00C0DF258498__INCLUDED_)
#define AFX_PLATUI_H__48B86E81_729F_11D4_9671_00C0DF258498__INCLUDED_
////////////////////////////////////////////////////////////////////////////
//	File:		PlatUI.h
//	Version:	1.0.0.0
//	Created:	2005-07-08
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	header of Platform UI interface object.
////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlatUI.h : header file
//

#include "IPlatUI.h"

/////////////////////////////////////////////////////////////////////////////
// CPlatUI command target

#define INF_IPlatUI \
{\
	"IPlatUI",\
	"1.0.0.0",\
	THIS_REVISION_TYPE,\
	"org.blueui.platui",\
	"PlatUI VCI",\
	"Blueant",\
	__DATE__\
}
const TInterfaceInfo INTERFACE_INFO_IPlatUI = INF_IPlatUI;

class CPlatUI : public CVisualComponent
{
public:
	CPlatUI();           // protected constructor used by dynamic creation
	virtual ~CPlatUI();

// Attributes
public:

// Interface define
public:
	BEGIN_INTERFACE_PART(VciControl, IVciControl)
		CONTROL_INTERFACE_DEFINE
	END_INTERFACE_PART(VciControl)
	EXPORT_CONTROL_INTERFACE_PART(VciControl)

    BEGIN_INTERFACE_PART(PlatUI, IPlatUI)

		// 平台操作
		STDMETHOD_( CString , GetAppName) ();				// 获取应用程序名
		STDMETHOD_( CString , GetPlatPath) ();				// 获取平台路径
		STDMETHOD_( CString , GetPlatVersion) ();			// 获取平台版本
		STDMETHOD_( int , GetCurrentLanguage) ();			// 获取当前语言
		STDMETHOD_( CString , GetPlatRegistry) ();			// 获取平台注册表根路径
		STDMETHOD_( CString , GetPlatCopyRight) ();			// 获取平台版权字符串
		STDMETHOD_( CString , GetPlatHomeURL) ();			// 获取主页URL
		STDMETHOD_( CString , GetPlatDownloadURL) ();		// 获取下载URL
		STDMETHOD_( int  , SendMessage) (CVciMessage* pIn, CVciMessage* ppOut);	// 发送消息
		STDMETHOD_( int  , ProcessMessage) (CVciMessage* pIn, CVciMessage* ppOut);	// 平台的消息处理
		STDMETHOD_( int  , SendCommand) (int nCmd, WPARAM wParam, LPARAM lParam);	// 发送平台命令
		STDMETHOD_( BOOL , SendCommand) (int nCmd, WPARAM wParam, LPARAM lParam, LPVOID lpResult);	// 发送平台命令

		// 输出窗口操作
		STDMETHOD_( int  , AddOutput) (CString strName);	//添加输出窗口
		STDMETHOD_( BOOL , DelOutput) (int nOT);			//删除输出窗口
		STDMETHOD_( BOOL , ActiveOutput) (int nOT);		//激活输出窗口
		STDMETHOD_( BOOL , Output) (int nOT, LPCTSTR lpszOut);//输出信息
		STDMETHOD_( BOOL , OutputFormat) (int nOT, int nFmt, LPCTSTR lpszOut);//输出格式信息
		STDMETHOD_( BOOL , OutputVerify) (int nOT, LPCTSTR lpszOut, LPCTSTR lpszVerify);//输出信息并和校验字符串比较,不一致的字符显示为红色
		STDMETHOD_( BOOL , OutputSet) (int nOT, LPCTSTR lpszSet);//设置输出窗口
		STDMETHOD_( CString , OutputGet) (int nOT, LPCTSTR lpszParam);// 获取输出窗口信息
		STDMETHOD_( BOOL , OutputClear) (int nOT);			//清除输出窗口
		STDMETHOD_( BOOL , OutputAddTreeItems) (int nOT, CTTreeItemArray_* pItem);// 添加树输出窗口内容

		// 用户菜单操作
		STDMETHOD_( void* , GetCommandBars) ();			// 获取CommandBars
		STDMETHOD_( BOOL , AddExtMenu) (CMenu*& pMenu);	// 添加扩展菜单
		STDMETHOD_( BOOL , DelExtMenu) (CMenu*& pMenu);	// 删除扩展菜单
		STDMETHOD_( BOOL , AddSubMenu) (CString strMainMenu, int nIndex, CMenu*& pMenu, LPVOID pProc, LPVOID pUpdateProc);	// 添加扩展子菜单
		STDMETHOD_( BOOL , DelSubMenu) (CString strMainMenu, int nIndex, CMenu*& pMenu);	// 删除扩展子菜单
		STDMETHOD_( BOOL , TrackPopupMenu) (CMenu* pMenu, UINT nFlags, int x, int y, CWnd* pWnd);	// 弹出菜单

		// 状态行操作
		STDMETHOD_( BOOL , AddStatusPane) (int& nID, int nWidth);	//添加状态行Pane
		STDMETHOD_( BOOL , DelStatusPane) (int nID);				//删除状态行Pane
		STDMETHOD_( BOOL , SetPaneText) (int nID, CString strPane);	//设置Pane文字
		STDMETHOD_( BOOL , SetPaneWidth) (int nID, int nWidth);		//设置Pane宽度
		STDMETHOD_( BOOL , SetPaneImage) (int nID, int nGroup, int nImage);//设置Pane图像

		// 窗口操作
		STDMETHOD_( CDocument*, GetActiveDocument) ();	// 获取当前激活的文档对象
		STDMETHOD_( BOOL , CreateDockingPane) (CString strOwmId, CString strTitle, int nID, int nDirection, int nAttachType, HICON hIcon, int nAttachPane);	// 创建DockingPane
		STDMETHOD_( BOOL , CreateDockingPane) (CString strOwmId, int nID);	// 创建DockingPane
		STDMETHOD_( CWnd*, GetDockingPaneWnd) (int nPaneID);	// 获取DockingPane包含的窗口对象
		STDMETHOD_( int , GetDockingPaneIdByName) (CString strName);	// 获取DockingPane ID(根据名字)
		STDMETHOD_( BOOL , ActiveDockingPane) (int nPaneID, BOOL bShow = TRUE);	// 激活DockingPane
		STDMETHOD_( BOOL , CloseDockingPane) (int nPaneID);	// 关闭DockingPane窗口

		// 日志操作
		STDMETHOD_( BOOL , AddSysLogInfo) (int nType, CString strFrom, CString strDesc);//增加日志信息

		// 用户管理操作

		// OWM操作
		STDMETHOD_( BOOL , IsOwmExist) (CString strOwmId);				//查询模块是否存在
		STDMETHOD_( BOOL , SendOwmCmd) (CString strOwmId, int nCmd, WPARAM wParam, LPARAM lParam, LPVOID lpResult);//发送OWM命令
		STDMETHOD_( BOOL , LoadOwm) (CString strOwmName, int& nID, BOOL bLoad);//加载扩展模块

		// 文档操作
		STDMETHOD_( CDocument* , OpenDocumentFile) (LPCTSTR lpszFileName, LPCTSTR lpszOwmId);		//打开文档

		// 属性窗口操作
		STDMETHOD_( CWnd* , GetPropertyGridWnd) ();	// 获取属性树指针
		STDMETHOD_( BOOL , EnableUpdatePropertyGridContent) ();	// 判断是否可以更新属性树
		STDMETHOD_( void , UpdatePropertyGridContent) ();	// 刷新属性窗口内容
		STDMETHOD_( void , BeginUpdatePropertyGridContent) ();	// 开始属性窗口刷新
		STDMETHOD_( void , EndUpdatePropertyGridContent) ();	// 结束属性窗口刷新

		// VCI组件调用接口
		STDMETHOD_( void* , CreateVciObject) (CString strVciId, CString strInstName);	// 创建一个组件对象
		STDMETHOD_( BOOL , ReleaseVciObject) (void* pObject, BOOL bForceDelete);	// 关闭一个组件对象
		STDMETHOD_( BOOL , ReleaseObjectByInstanceName) (CString strInstName);	// 关闭一个组件对象(指定实例名)
		STDMETHOD_( void* , GetObjectByInstanceName) (CString strInstName);	// 获取一个组件对象(根据实例名)
		STDMETHOD_( void* , GetObjectControlPtrByInstancePtr) (LPVOID pVciObj);	// 获取一个组件对象控制接口(根据组件实例接口指针)
		STDMETHOD_( int , GetInstalledVci) (CStringArray& asVciId);	// 获取所有安装的组件ID
		STDMETHOD_( CString , GetVciName) (CString strId);	// 获取一个组件名(根据组件ID)
		STDMETHOD_( CString , GetVciPath) (CString strId);	// 获取一个组件路径(根据组件ID)
		STDMETHOD_( CString , GetVciVersion) (CString strId);	// 获取一个组件版本(根据组件ID)
		STDMETHOD_( CString , GetVciProvider) (CString strId);	// 获取一个组件开发商信息(根据组件ID)
		STDMETHOD_( CString , GetVciType) (CString strId);	// 获取一个组件类型(根据组件ID)
		STDMETHOD_( CString , GetVciClass) (CString strId);	// 获取一个组件类接口ID(根据组件ID)
		STDMETHOD_( CString , GetVciInterfaceFile) (CString strId);	// 获取一个组件接口定义文件(根据组件ID)
		STDMETHOD_( CTime , GetVciBuildTime) (CString strId);	// 获取一个组件编译时间(根据组件ID)
		STDMETHOD_( CString , GetVciGUID) (CString strId);	// 获取一个组件GUID(根据组件ID)
		STDMETHOD_( CString , GetVciAboutPage) (CString strId);	// 获取一个组件关于文件位置(根据组件ID)
		STDMETHOD_( int , GetVciDocExecuteModule) (CString strId, CString strDocId, CString strExecuteType, CStringArray& arExecuteModule);	// 获取可以执行一个文档的扩展模块ID列表(根据组件Id和文档Id)
		STDMETHOD_( CString , GetVciDocIcon) (CString strId, CString strDocId, CString strIconType);	// 获取文档的图标(根据组件Id,文档Id,图标类型)
		STDMETHOD_( void , SetVciDocIcon) (CString strId, CString strDocId, CString strIconType, HWND hWnd);	// 设置文档的图标(根据组件Id,文档Id,图标类型,窗口句柄)
		STDMETHOD_( void , SetDocumentTitle) (CString strTitle, int nModifyType, HWND hWnd);	// 设置文档的标题(根据窗口句柄)
		STDMETHOD_( int , GetVciObjects) (CString strId, CPtrArray& auVciObj);	// 获取一个组件的所有实例对象指针
		STDMETHOD_( CString , GetInstanceNameByObject) (CString strId, LPVOID pVciObj);	// 获取一个组件实例对象的实例名
		STDMETHOD_( int , GetInstanceRefCount) (CString strId, LPVOID pVciObj);	// 获取一个组件实例对象的引用计数
		STDMETHOD_( int , EnumVciParameters) (CString strId, CStringArray& asParamName);	// 枚举一个VCI组件的扩展参数
		STDMETHOD_( BOOL , GetVciParameter) (CString strId, CString strName, CString& strValue, CString& strDefaultValue);	// 获取一个VCI组件的参数
		STDMETHOD_( BOOL , SetVciParameter) (CString strId, CString strName, CString strValue);	// 设置一个VCI组件的参数

		// 定时器操作
		STDMETHOD_( void* , AddTimerTask) (CString strTaskName, CString strInterpId, CString strInterpName, CString strCommand, int nSeconds, BOOL bStart);	// 添加定时器任务
		STDMETHOD_( void* , AddTimerTask) (CString strTaskName, LPVOID lpFunction, int nSeconds, BOOL bStart);	// 添加定时器任务
		STDMETHOD_( void* , GetTimerTask) (CString strTaskName);	// 根据任务名获取定时任务指针
		STDMETHOD_( BOOL , DeleteTimerTask) (LPVOID lpTimerTask);	// 删除定时任务(根据任务指针)
		STDMETHOD_( BOOL , DeleteTimerTask) (CString strTaskName);	// 删除定时任务(根据任务名)
		STDMETHOD_( void , StartTimerTask) (LPVOID lpTimerTask);	// 启动定时任务
		STDMETHOD_( void , SetTimerStartTime) (LPVOID lpTimerTask, int tStart);	// 设置定时任务开始时间
		STDMETHOD_( void , SetTimerEndTime) (LPVOID lpTimerTask, int tEnd);	// 设置定时任务开始时间
		STDMETHOD_( void , SetTimerTrigger) (LPVOID lpTimerTask, CString strTrigger);	// 设置定时任务触发条件脚本
		STDMETHOD_( void , SetTimerExecCount) (LPVOID lpTimerTask, int nCount);	// 设置定时任务执行次数

		// 工具操作
		STDMETHOD_( int , GetInstalledTool) (CStringArray& asToolId);	// 获取所有安装的工具ID
		STDMETHOD_( CString , GetToolName) (CString strId);	// 获取一个工具名(根据工具ID)
		STDMETHOD_( CString , GetToolPath) (CString strId);	// 获取一个工具路径(根据工具ID)
		STDMETHOD_( CString , GetToolVersion) (CString strId);	// 获取一个工具版本(根据工具ID)
		STDMETHOD_( CString , GetToolProvider) (CString strId);	// 获取一个工具开发商信息(根据工具ID)
		STDMETHOD_( BOOL , RunTool) (CString strId);	// 运行工具(根据工具ID)

	END_INTERFACE_PART(PlatUI)
	EXPORT_INTERFACE_PART(PlatUI)

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLATUI_H__48B86E81_729F_11D4_9671_00C0DF258498__INCLUDED_)
