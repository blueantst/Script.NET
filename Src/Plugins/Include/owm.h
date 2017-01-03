////////////////////////////////////////////////////////////////////////////
//	File:		owm.h
//	Version:	2.1.0.1
//	Created:	2000-06-01
//	Last Update:2006-05-25
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	Comm define of objwin module.
////////////////////////////////////////////////////////////////////////////
#ifndef __OWM_H_
#define __OWM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 各OWM模块扩展资源的起始值
#define OWM_SID_FTLIST		11000
#define OWM_SID_LOG			11050
#define OWM_SID_REPORT		11100
#define OWM_SID_OWMEDIT		11150
#define OWM_SID_WORKTREE	11250
#define OWM_SID_RCADMIN		11300
#define OWM_SID_PTERMINAL	11350
#define OWM_SID_UMLEDIT		11400
#define OWM_SID_IMAGE		11500	// 图像编辑器模块

// 定义OWM属性
#define OWM_DOC			0x00000001	// 有文档模板的OWM
#define OWM_TCL			0x00000002	// 有TCL扩展的OWM
#define OWM_MENU		0x00000004	// 有扩展菜单的OWM
#define OWM_TOOLBAR		0x00000008	// 注册工具条
#define OWM_ONEFRAME	0x00000010	// 只允许一个OWM窗口的实例
#define OWM_TABVIEW		0x00000020	// 是否为TabView类型的窗口
#define OWM_OPTION		0x00000040	// 是否有属性页
#define OWM_CONTROLBAR	0x00000080	// 是否包含ControlBar窗口
#define OWM_TABCTRLBAR	0x00000100	// 是否包含TabCtrlBar窗口
#define OWM_AUTOCREATE	0x00000200	// 加载OWM时是否自动创建窗口
#define OWM_DYNLOAD		0x00000400	// 动态加载扩展模块
#define OWM_BROADCAST	0x00010000	// 是否允许接收广播命令
#define OWM_MAXWINDOW	0x00020000	// 创建最大化窗口

// 定义OWM扩展命令
#define OC_CMDALL				0	// 查询所有命令
#define OC_QUERYOWMID			1	// 查询OWM(根据ID)
#define OC_SETOWMLINK			2	// 设置OWM链接(to owm)
#define OC_RUNSCRIPT			3	// 运行脚本(to plat)
#define OC_OPENVIEW				4	// 打开OWM窗口(to owm)
#define OC_CLOSEVIEW			5	// 关闭OWM窗口(to owm)
#define OC_OPENBAR				6	// 打开ControlBar(to owm)
#define OC_CLOSEBAR				7	// 关闭ControlBar(to owm)
#define OC_CMDHELP				8	// 命令帮助
#define OC_GETTCLEXKEYWORD		9	// 获取TCL扩展命令列表指针

// 配置文件操作命令
#define OC_GETINISTRING			10	// 取INI字符
#define OC_GETINIINT			11	// 取INI int
#define OC_SETINISTRING			12	// 设置INI字符
#define OC_SETINIINT			13	// 设置INI int
#define OC_QUERYUSERINFO		14	// 查询用户信息

// 输出窗口操作命令
#define OC_TRACEFMT				20	// 向TRACE窗口输出
#define OC_CREATEOUTPUT			21	// 创建输出窗口
#define OC_CLOSEOUTPUT			22	// 关闭输出窗口
#define OC_OUTPUTSET			23	// 设置输出窗口
#define OC_OUTPUTGET			24	// 获取输出窗口信息
#define OC_OUTPUT				25	// 向输出窗口输出
#define OC_ACTIVEOUTPUT			26	// 激活窗口
#define OC_CLEAROUTPUT			27	// 清除窗口内容
#define OC_TREEOUTPUT			28	// 树输出窗口
#define OC_OUTPUTVERIFY			29	// 输出信息并和校验字符串比较

// 状态条操作命令
#define OC_ADDPANE				30	// 添加Pane
#define OC_REMOVEPANE			31	// 删除Pane
#define OC_SETPANETEXT			32	// 设置Pane文字
#define OC_SETPANEMENU			33	// 设置Pane菜单
#define OC_SETPANECONTROL		34	// 设置Pane控件
#define OC_SETPANEIMAGE			35	// 设置Pane图标组
#define OC_SETPANEIMAGEITEM		36	// 设置Pane图标项
#define OC_SETPANEWIDTH			37	// 设置Pane宽度
#define OC_ATTACHPANEPROGRESS	38	// 建立Pane和进度条的关联
#define OC_SHOWPANEPROGRESS		39	// 显示/隐藏Pane进度条
#define OC_SETPANEPROGRESSPOS	40	// 设置Pane进度条进度
#define OC_SETPANEBUTTON		41	// 设置Pane按钮
#define OC_SETPANETOOLTIP		42	// 设置Pane提示信息

// 菜单操作
#define OC_ADDEXTMENU			45	// 添加扩展菜单
#define OC_DELEXTMENU			46	// 删除扩展菜单
#define OC_QUERYEXTMENU			47	// 查询扩展菜单
#define OC_QUERYEXTMENUFROMID	48	// 通过菜单ID查询扩展菜单信息

// 测试操作
#define OC_TESTINIT				50	// 初始化
#define OC_TESTSTART			51	// 启动测试
#define OC_TESTSTOP				52	// 停止测试
#define OC_TESTHOLD				53	// 暂停测试
#define OC_TESTRESUME			54	// 恢复测试
#define OC_TESTDISABLE			55	// 禁止测试
#define OC_TESTRESET			56	// 复位测试项
#define OC_TESTPROPERTY			57	// 属性
#define OC_TESTSELFTEST			58	// 设备自检
#define OC_TESTSTRATEGY			59	// 测试策略调整

// 日志操作
#define OC_LOGADDITEM			60	// 添加日志项
#define OC_LOGDELITEM			61	// 删除日志项
#define OC_LOGCLEAR				62	// 清空日志

// 系统扩展操作
#define OC_SETACTIVEWS			70	// 设置活动工位
#define OC_GOTOITEM				71	// 转到指定的项目
#define OC_GETFONTARRAY			72	// 获取字体配置表
#define OC_DYNCLOSE				73	// 通知平台执行动态关闭
#define OC_OPENPROJECT			74	// 打开工程
#define OC_CLOSEPROJECT			75	// 关闭工程
#define OC_VIEW_HISTORY			76	// 显示历史纪录
#define OC_CHECK_IDENABLE		77	// 判断某个ID是否具有操作权限
#define OC_SETACTIVEGROUP		78	// 设置活动组
#define OC_EXECOPTION			79	// 执行属性页操作
#define OC_EXECTREEOPTION		80	// 执行通用树状属性对话框
#define OC_EXECTCLCMD			81	// 执行TCL命令
#define OC_EXECTCLFILE			82	// 执行TCL脚本文件
#define OC_REGEXP				83	// 使用TCL正则表达式功能进行分析
#define	OC_VIEWHTMLPAGE			84	// 显示Html页面
#define OC_WEBDIALOG			85	// 执行WEB对话框
#define OC_OPENDOCUMENTFILE		86	// 打开一个文件进行编辑
#define OC_SETDOCICON			87	// 设置文档图标
#define OC_SETDOCTITLE			88	// 设置文档标题
#define OC_MODIFYDOCTITLE		89	// 修改文档标题
#define	OC_GETHTMLURL			90	// 获取Html页面的URL
#define	OC_REFRESH_DOCKINGHTML	91	// 刷新Docking Html页面
#define	OC_SETCOMMANDICON		92	// 设置某个命令ID的图标
#define	OC_SETCOMMANDICONS		93	// 设置多个命令ID的图标
#define	OC_SETOWMICONS			94	// 设置某个OWM的图标
#define	OC_REFRESHDEBUGWIN		95	// 刷新调试窗口
#define	OC_CHECK_ACTIVEOWM		96	// 判断指定OWM窗口是否为当前激活窗口
#define	OC_SHOWABOUTBOX			97	// 显示指定模块的About对话框
#define	OC_SENDVCICOMMAND		98	// 发送VCI的命令(ProcessCommand函数)

// 导航树操作
#define OC_NAVTREE_HIDE			120	// 隐藏节点
#define OC_NAVTREE_REFRESH		121	// 刷新节点
#define OC_NAVTREE_PROPERTY		122	// 节点属性

// XML操作
#define OC_XML_OPENFILE			130	// 打开XML文件
#define OC_XML_CLOSEFILE		131	// 关闭XML文件

// 属性窗口操作
#define OC_PROPERTY_CLEAR		140	// 清空属性窗口


////////////////////////////////////////////////////////////////
// 以下是内部命令,不公开
// 测试命令
#define OC_TEST_INTERNALINIT	1010	// 内部的初始化命令
#define OC_TEST_INTERNALSTART	1011	// 内部的启动测试命令

// 配置命令
#define OC_CONFIG_SWITCHBAR		1020	// 切换窗口信息注册表键
#define OC_REFRESH_RSMINTERP	1021	// 刷新资源管理树的解释器节点

// 扩展命令
#define OC_LOGIN_DIALOG			1030	// 通用登录对话框
#define OC_PATHSELECT_DIALOG	1031	// 路径选择对话框

// 进度对话框命令
#define OC_PROGDLG_CREATE		1040	// 创建通用进度对话框
#define OC_PROGDLG_SETPOS		1041	// 设置进度
#define OC_PROGDLG_SETSTATUS	1042	// 设置状态信息
#define OC_PROGDLG_END			1043	// 结束进度对话框
#define OC_PROGDLG_CANCEL		1044	// cancel进度对话框
#define OC_PROGDLG_CANCELSCRIPT	1045	// 设置cancel脚本

// TaskDialog命令
#define OC_TASK_DIALOG			1050	// 通用任务对话框

// Popup Control命令
#define OC_POPUP_CONTROL		1060	// 显示Popup窗口

// HtmlView操作命令
#define OC_HTML_SET_ELEMENT_INNERHTML	1070	// 设置Html元素的Html内容
#define OC_HTML_GET_ELEMENT_INNERHTML	1071	// 获取Html元素的Html内容
#define OC_HTML_SET_ELEMENT_INNERTEXT   1072	// 设置Html元素的Text内容
#define OC_HTML_GET_ELEMENT_INNERTEXT   1073	// 获取Html元素的Text内容
#define OC_HTML_SET_ELEMENT_OUTERHTML	1074	// 设置Html元素的外部Html内容
#define OC_HTML_GET_ELEMENT_OUTERHTML	1075	// 获取Html元素的外部Html内容
#define OC_HTML_SET_ELEMENT_OUTERTEXT	1076	// 设置Html元素的外部Text内容
#define OC_HTML_GET_ELEMENT_OUTERTEXT	1077	// 获取Html元素的外部Text内容
#define OC_HTML_SET_ELEMENT_INPUTVALUE	1078	// 设置Html元素的Input Value
#define OC_HTML_GET_ELEMENT_INPUTVALUE	1079	// 获取Html元素的Input Value
#define OC_HTML_CLEAR_MESSAGE_FILTER	1090	// 清空页面消息过滤列表
#define OC_HTML_ADD_MESSAGE_FILTER		1091	// 添加页面过滤消息
#define OC_HTML_PROCESS_MESSAGE			1095	// 界面处理VciMessage

// MarkupDialog命令
#define OC_MARKUP_DIALOG		2000	// 显示Markup对话框

// Tray相关命令
#define OC_TRAY_ADD_ICON		2010	// 添加Tray Icon
#define OC_TRAY_DEL_ICON		2011	// 删除Tray Icon
#define OC_TRAY_SET_ICON		2012	// 设置Tray Icon
#define OC_TRAY_START_ANIMATION	2013	// 启动Tary Icon动画
#define OC_TRAY_STOP_ANIMATION	2014	// 停止Tray Icon动画
#define OC_TRAY_SET_TOOLTIP		2015	// 设置Tray Icon的Tooltip
#define OC_TRAY_BALLOONTIP		2016	// 显示Tary Icon气泡提示

// 工程相关命令
#define OC_TASK_GETSCRIPT		1501	// 获取某个任务的脚本


#pragma pack(push,1)

// 定义OWM菜单项结构
typedef int (TYPEOF_ClientMenu)();
typedef void (TYPEOF_UpdateClientMenu)(CCmdUI* pCmdUI);
typedef int (TYPEOF_ClientMenuParamId)(int nId);

typedef struct 
{
	TYPEOF_ClientMenu *pProc;	// 菜单函数
	TYPEOF_UpdateClientMenu	*pUpdateProc;	// 刷新菜单函数
	int nId;					// 菜单ID
	int nGrade;					// 权限
} OWMMENUITEM;

typedef CArray<OWMMENUITEM, OWMMENUITEM> COWMMenuArray;

// 定义属性页
typedef struct
{
	CString strParent;		// 父节点名
	CString strNode;		// 节点名
	CString strDesc;		// 描述
	CWnd* pOptionPage;		// 属性页指针
	int nGrade;				// 权限
} OWMOPTION;

typedef CArray<OWMOPTION, OWMOPTION> COWMOptionArray;

// 定义字体设置结构
#define FON_FONT		0x00000001	// 字体
#define FON_SIZE		0x00000002	// 大小
#define FON_FG			0x00000004	// 前景色
#define FON_BK			0x00000008	// 背景色
#define FON_TYPE		0x00000010	// 字形

struct FontItem
{
	DWORD	dwMask;			// 掩码
	CString strName;		// 配置项名
	CString strSubKey;		// 子键名
	CString strFont;		// 字体
	int		nSize;			// 大小
	COLORREF colorFg;		// 前景色
	COLORREF colorBk;		// 背景色
	BOOL	bBold;			// 粗体
	BOOL	bItalic;		// 斜体
	BOOL	bUnderLine;		// 下划线
	BOOL	bStrikeOut;		// 删除线

	FontItem() {;};
	FontItem(DWORD _dwMask, CString _strName, CString _strSubKey, CString _strFont,
			int _nSize, COLORREF _colorFg, COLORREF _colorBk, BOOL _bBold,
			BOOL _bItalic, BOOL _bUnderLine, BOOL _bStrikeOut)
	{
		dwMask		= _dwMask;
		strName		= _strName;
		strSubKey	= _strSubKey;
		strFont		= _strFont;
		nSize		= _nSize;
		colorFg		= _colorFg;
		colorBk		= _colorBk;
		bBold		= _bBold;
		bItalic		= _bItalic;
		bUnderLine	= _bUnderLine;
		bStrikeOut	= _bStrikeOut;
	};

	FontItem& operator = (const FontItem& other)
	{
		dwMask		= other.dwMask;
		strName		= other.strName;
		strSubKey	= other.strSubKey;
		strFont		= other.strFont;
		nSize		= other.nSize;
		colorFg		= other.colorFg;
		colorBk		= other.colorBk;
		bBold		= other.bBold;
		bItalic		= other.bItalic;
		bUnderLine	= other.bUnderLine;
		bStrikeOut	= other.bStrikeOut;
		return *this;
	}
};

// 定义OWM接口函数类型
typedef int (TYPEOF_InitOWM)();
typedef int (TYPEOF_DoneOWM)();
typedef int (TYPEOF_SetOWM)(LPCTSTR lpszOption);
typedef int (TYPEOF_RegisterTclCmd)(void* interp);
typedef int (TYPEOF_UnregisterTclCmd)();
typedef int (TYPEOF_RegisterTclExKeyword)(CStringArray& aKeyList);
typedef int (TYPEOF_RunTclScript)(LPTSTR lpszPathName, int& nLine);
typedef int (TYPEOF_RegisterClientMenu)(CMenu* pMenu, COWMMenuArray& aOWMMenus, int nOWMIdStart, CDocument* pDoc);
typedef int (TYPEOF_UnregisterClientMenu)(COWMMenuArray& aOWMMenus);
typedef int (TYPEOF_RegisterClientToolBar)(CWnd* pBar, COWMMenuArray& aOWMMenus, int nOWMIdStart, int nIndex);
typedef int (TYPEOF_UnregisterClientToolBar)(CWnd* pBar, COWMMenuArray& aOWMMenus, int nOWMIdStart);
typedef int (TYPEOF_RegisterOption)(COWMOptionArray& aOWMOption);
typedef int (TYPEOF_SetDebugMode)(BOOL bDebug);
typedef int (TYPEOF_RunHelp)();
typedef int (TYPEOF_CommandOWM)(int nCmd, WPARAM wParam, LPARAM lParam);
typedef int (TYPEOF_CommandPlat)(int nCmd, WPARAM wParam, LPARAM lParam);
typedef int (TYPEOF_TraceFmt)(int nFmt, LPCTSTR format, ...);
typedef int (TYPEOF_CreateOutput)(LPCTSTR lpszName);
typedef int (TYPEOF_CloseOutput)(LPCTSTR lpszName);
typedef int (TYPEOF_OutputSet)(int nWinID, LPCTSTR lpszOption);
typedef int (TYPEOF_Output)(int nWinID, LPCTSTR format, ...);
typedef void (TYPEOF_GetDllResource)();
typedef void (TYPEOF_ResetDllResource)();


// 定义OWM接口结构
typedef struct 
{
	UINT							nOWMType;				// OWM类型
	UINT							nOWMID;					// OWM标识
	CRuntimeClass					*pDocClass;				// OWM文档
	CRuntimeClass					*pFrameClass;			// OWM框架
	CRuntimeClass					*pViewClass;			// OWM视
	CRuntimeClass					*pCtrlBarClass;			// 作为工具条的应用
	CMultiDocTemplate				*pDocTemplate;			// OWM文档模板
	TYPEOF_InitOWM					*InitOWM;				// 初始化OWM
	TYPEOF_DoneOWM					*DoneOWM;				// 卸载OWM
	TYPEOF_SetOWM					*SetOWM;				// 设置OWM;
	TYPEOF_RegisterTclCmd			*RegisterTclCmd;		// TCL命令注册函数
	TYPEOF_UnregisterTclCmd			*UnregisterTclCmd;		// TCL命令卸载函数
	TYPEOF_RegisterTclExKeyword		*RegisterTclExKeyword;	// TCL扩展关键字注册函数
	TYPEOF_RunTclScript				*RunTclScript;			// 运行TCL脚本,由平台提供
	TYPEOF_RegisterClientMenu		*RegisterClientMenu;	// 注册菜单
	TYPEOF_UnregisterClientMenu		*UnregisterClientMenu;	// 卸载菜单
	TYPEOF_RegisterClientToolBar	*RegisterClientToolBar;	// 注册ToolBar
	TYPEOF_UnregisterClientToolBar	*UnregisterClientToolBar;// 卸载ToolBar
	TYPEOF_RegisterOption			*RegisterOption;		// 注册OWM属性页
	TYPEOF_SetDebugMode				*SetDebugMode;			// 设置调试模式
	TYPEOF_RunHelp					*RunHelp;				// 运行OWM的帮助
	TYPEOF_CommandOWM				*CommandOWM;			// OWM命令处理函数
	TYPEOF_CommandPlat				*CommandPlat;			// 平台命令处理函数
	TYPEOF_TraceFmt					*TraceFmt;				// TRACE函数,由平台提供
	TYPEOF_CreateOutput				*CreateOutput;			// 创建输出窗口,由平台提供
	TYPEOF_CloseOutput				*CloseOutput;			// 关闭输出窗口,由平台提供
	TYPEOF_OutputSet				*OutputSet;				// 设置输出窗口,由平台提供
	TYPEOF_Output					*Output;				// 向输出窗口输出信息,由平台提供
	TYPEOF_GetDllResource			*GetDllResource;		// 切换到DLL的资源
	TYPEOF_ResetDllResource			*ResetDllResource;		// 切换回以前的资源
} OWM_LINKAGE;

// 定义OWM版本结构
typedef struct 
{
	char *svOWMName;		// OWM名
	char *svDescription;	// OWM描述
	WORD wVersionLo;		// OWM版本低位
	WORD wVersionHi;		// OWM版本高位
	WORD wMVersionLo;		// 所需平台版本低位
	WORD wMVersionHi;		// 所需平台版本高位
	int  nLanguage;			// 语言(0-中文,1-英文)
	BOOL bDebugVer;			// 是否DEBUG版本
} OWM_VERSION;

// 定义OWM模块接口函数
typedef BOOL (TYPEOF_InstallOWM)(OWM_LINKAGE& ol);
typedef BOOL (TYPEOF_OWMVersion)(OWM_VERSION& pov);
typedef void (TYPEOF_TerminateOWM)(void);

// OWM动态加载的状态
#define OWMDYN_TSR		0	// 加载状态
#define OWMDYN_UNTSR	1	// 卸载状态

#define _MAX_OWM_NAME	50	// OWM名字最大长度

// OWM模块信息结构
typedef struct
{
	OWM_VERSION		ov;			// 版本信息
	OWM_LINKAGE		ol;			// 接口信息
	HINSTANCE		ins;		// 模块句柄
	CControlBar*	pBar;		// 记录ControlBar指针
	char			szName[_MAX_OWM_NAME];	// OWM名字
	char			szPath[_MAX_PATH];	// DLL路径信息
	int				nLoader;	// 由谁加载(0=平台注册,1=平台OWM目录,2=工程)
	int				nDnyLoad;	// 动态加载状态
	BOOL			bEnableDynLoad;	// 是否允许动态加载
	BOOL			bEnable;	// 此模块当前是否使用
} OWM_INFO;

typedef CArray<OWM_INFO, OWM_INFO> COWMArray;
/*
// 定义用户权限
enum
{
	USER_SUPER,			// 超级用户
	USER_ADMIN,			// 管理员
	USER_DEVELOP,		// 开发人员
	USER_OPERATOR,		// 操作人员
	USER_NONE,			// 空
};
*/
#pragma pack(pop)

// OWM接口定义、实现辅助宏
#define OWM_INTERFACE_DEFINE \
	virtual LPVOID __stdcall GetExtraInfo(LPVOID lpRefData); \
	virtual int __stdcall GetOwmName(CString& strOwmName); \
	virtual int __stdcall GetOwmDesc(CString& strOwmDesc); \
	virtual int __stdcall GetOwmVersion(int& nVersion); \
	virtual int __stdcall GetOwmType(UINT& nType); \
	virtual int __stdcall GetOwmID(UINT& nID); \
	virtual int __stdcall SetIPlatUI(LPVOID lpIPlatUI); \
	virtual int __stdcall SetDocTemplate(CMultiDocTemplate* pDocTemplate); \
	virtual int __stdcall GetDocTemplate(CMultiDocTemplate** ppDocTemplate); \
	virtual int __stdcall GetRuntimeClass(int nType, CRuntimeClass** ppRuntimeClass); \
	virtual CWnd* __stdcall OnShowDockingPane(int nID, CWnd* pParentWnd, LPCTSTR lpszParam); \
	virtual int __stdcall Command(int nCmd, WPARAM wParam, LPARAM lParam); \
	virtual int __stdcall RegisterClientMenu(CMenu* pMenu, COWMMenuArray& aOWMMenus, int nOWMIdStart, CDocument* pDoc); \
	virtual int __stdcall UnregisterClientMenu(COWMMenuArray& aOWMMenus); \
	virtual int __stdcall RegisterClientToolBar(CWnd* pBar, COWMMenuArray& aOWMMenus, int nOWMIdStart, int nIndex); \
	virtual int __stdcall UnregisterClientToolBar(CWnd* pBar, COWMMenuArray& aOWMMenus, int nOWMIdStart);	\
	virtual int __stdcall RegisterOption(COWMOptionArray& aOWMOption);	\
	virtual int __stdcall RegisterPrintPageOption(COWMOptionArray& aOWMOption);	\
	virtual int __stdcall LoadDocumentSession(CDocument* pDoc, CStringArray& asSessionLine);	\
	virtual int __stdcall SaveDocumentSession(CDocument* pDoc, CStringArray& asSessionLine);	\
	virtual int __stdcall BackupDocument(CDocument* pDoc, LPCTSTR lpszBackupFile, int nBackupType);	\
	virtual int __stdcall GetImportTypeList(CStringArray& asImportType);	\
	virtual int __stdcall GetExportTypeList(CStringArray& asExportType);	\
	virtual int __stdcall ImportDocument(CDocument* pDoc, LPCTSTR lpszImportSource, int nImportType);	\
	virtual int __stdcall ExportDocument(CDocument* pDoc, LPCTSTR lpszExportDest, int nExportType);	\
	virtual int __stdcall Zoom(float fZoomFactor, CDocument* pDoc);	\
	virtual int __stdcall GetZoomFactor(float& fZoomFactor, CDocument* pDoc);	\
	virtual int __stdcall GetZoomFactorRange(CStringArray& asZoomFactor, float& fZoomMin, float& fZoomMax);


// 注册扩展菜单,参数分别是:菜单项ID,菜单函数,空闲更新函数,权限
#define OWM_REGISTERMENU(nItem, pproc, pupdateproc, grade) \
	{	\
		TYPEOF_ClientMenu* pProc = pproc;	\
		TYPEOF_UpdateClientMenu* pUpdateProc = pupdateproc;	\
		int nId = 0;	\
		BOOL bFind = FALSE;	\
		for(int i=0; i<aOWMMenus.GetSize(); i++)	\
		{	\
			if(aOWMMenus[i].pProc == pProc)	\
			{	\
				nId = aOWMMenus[i].nId;	\
				bFind = TRUE;	\
			}	\
			if((!bFind) && (aOWMMenus[i].nId >= nId))	\
				nId = aOWMMenus[i].nId + 1;	\
		}	\
		if((!bFind) && (pProc != NULL))	\
		{	\
			OWMMENUITEM omi;	\
			omi.nId = nId;	\
			omi.pProc = pProc;	\
			omi.pUpdateProc = pUpdateProc;	\
			omi.nGrade = grade;	\
			aOWMMenus.Add(omi);	\
		}	\
		if(nItem != 0)	\
		{	\
			CString strMenu;	\
			psubmenu->GetMenuString(nItem, strMenu, MF_BYCOMMAND);	\
			psubmenu->ModifyMenu(nItem, MF_BYCOMMAND, nOWMIdStart+nId, strMenu);	\
		}	\
		if(pProc == NULL)	\
		{	\
			psubmenu->EnableMenuItem(nOWMIdStart+nId, MF_GRAYED);	\
		}	\
	}	\


// 卸载扩展菜单,参数是菜单函数
#define OWM_UNREGISTERMENU(pproc) \
	{	\
		TYPEOF_ClientMenu* pProc = pproc;	\
		for(int i=(aOWMMenus.GetSize()-1); i>=0; i--)	\
		{	\
			if(aOWMMenus[i].pProc == pProc)	\
			{	\
				aOWMMenus.RemoveAt(i);	\
			}	\
		}	\
	}	\


// 注册工具条按钮,参数分别是:位置索引,函数,空闲更新函数,权限
#define OWM_REGISTERTOOLBAR(imageindex, pproc, pupdateproc, grade) \
	{	\
		TYPEOF_ClientMenu* pProc = pproc;	\
		TYPEOF_UpdateClientMenu* pUpdateProc = pupdateproc;	\
		int nId = 0;	\
		BOOL bFind = FALSE;	\
		for(int i=0; i<aOWMMenus.GetSize(); i++)	\
		{	\
			if(aOWMMenus[i].pProc == pProc)	\
			{	\
				nId = aOWMMenus[i].nId;	\
				bFind = TRUE;	\
			}	\
			if((!bFind) && (aOWMMenus[i].nId >= nId))	\
				nId = aOWMMenus[i].nId + 1;	\
		}	\
		if((!bFind) && (pProc != NULL))	\
		{	\
			OWMMENUITEM omi;	\
			omi.nId = nId;	\
			omi.pProc = pProc;	\
			omi.pUpdateProc = pUpdateProc;	\
			omi.nGrade = grade;	\
			aOWMMenus.Add(omi);	\
			TBBUTTON but;	\
			VERIFY(pToolBar->SendMessage(TB_GETBUTTON, 0, (LPARAM)(&but)));	\
			but.iBitmap = imageindex;	\
			VERIFY(::SendMessage(pToolBar->m_hWnd, TB_INSERTBUTTON, nIndex, (LPARAM)(&but)));	\
			VERIFY(::SendMessage(pToolBar->m_hWnd, TB_SETCMDID, nIndex, nOWMIdStart+nId));	\
		}	\
	}	\


// 卸载工具条按钮,参数是按钮函数
#define OWM_UNREGISTERTOOLBAR(pproc) \
	{	\
		TYPEOF_ClientMenu* pProc = pproc;	\
		for(int i=(aOWMMenus.GetSize()-1); i>=0; i--)	\
		{	\
			if(aOWMMenus[i].pProc == pProc)	\
			{	\
				int nId = aOWMMenus[i].nId;	\
				int index = pToolBar->CommandToIndex(nOWMIdStart+nId);	\
				if(index >= 0)	\
				{	\
					VERIFY(::SendMessage(pToolBar->m_hWnd, TB_DELETEBUTTON, index, 0));	\
				}	\
				aOWMMenus.RemoveAt(i);	\
			}	\
		}	\
	}	\


// 注册属性页,参数分别是:父节点名,节点名,描述,对话框ID,对话框类名,权限
#define OWM_REGISTEROPTION(szParent, szNode, szDesc, idtemplate, dialog, grade) \
	{	\
		get_dll_resource();	\
		CString lpszParent = szParent;	\
		CString lpszNode = szNode;	\
		CString lpszDesc = szDesc;	\
		CWnd* pOptionPage = new dialog;	\
		if(pOptionPage->IsKindOf(RUNTIME_CLASS(CDialog)))	\
		{	\
			((CDialog*)pOptionPage)->Create(idtemplate);	\
		}	\
		OWMOPTION oo;	\
		oo.strParent = lpszParent;	\
		oo.strNode = lpszNode;	\
		oo.strDesc = lpszDesc;	\
		oo.pOptionPage = pOptionPage;	\
		oo.nGrade = grade;	\
		aOWMOption.Add(oo);	\
		reset_dll_resource();	\
	}	\


// 获取主程序的菜单管理器,把OWM资源添加进去,参数是ToolBar的ID
#define OWM_LOADBARTOMAINFRAME(idbar)	\
	{	\
		get_dll_resource();	\
		CXTMDIFrameWnd* pMainFrame = (CXTMDIFrameWnd*)AfxGetMainWnd();	\
		CXTCoolMenu* pCoolMenu = pMainFrame->GetCoolMenu();	\
		pCoolMenu->LoadToolbar(idbar);	\
		reset_dll_resource();	\
	}	\


// 检查菜单是否已经存在,如果存在则删除,参数是菜单索引和菜单名
#define OWM_CHECKMENUISEXIST(indexmenu,smenuname)	\
	{	\
		CString strMenuName;	\
		pMenu->GetMenuString(indexmenu, strMenuName, MF_BYPOSITION);	\
		if(smenuname == strMenuName)	\
		{	\
			pMenu->RemoveMenu(indexmenu, MF_BYPOSITION);	\
		}	\
	}	\

// 加载字符串资源宏,用于国际化,参数为字符串名和资源ID
#define LANG_LOADSTRING(ssname,ids)	\
		CString ssname;	\
		ssname.LoadString(ids);	\


// 添加一个OWM扩展菜单项在主菜单的某个菜单项下面(暂时不用,可以通过平台接口注册的方式添加菜单项)
#define	OWM_ADDMENUITEM(mainmenu, menuid, menuindex)	\
{	\
	CMenu menu;	\
	get_dll_resource();	\
	menu.LoadMenu(menuid);	\
	reset_dll_resource();	\
	CString strMenuItem;	\
	menu.GetSubMenu(0)->GetMenuString(menuindex, strMenuItem, MF_BYPOSITION);	\
	CXTPCommandBar* pMenuBar = (CXTPCommandBar*)pMenu;	\
	CXTPControls* pControls = pMenuBar->GetControls();	\
	CXTPControl* pControl = pControls->GetFirst();	\
	CXTPCommandBar* pMenuBarSub = NULL;	\
	CXTPControl* pControlExtSub = NULL;	\
	while(pControl)	\
	{	\
		if(pControl->GetCaption() == mainmenu)	\
		{	\
			pMenuBarSub = pControl->GetCommandBar();	\
			CXTPControls* pControlsSub = pMenuBarSub->GetControls();	\
			pControl = pControlsSub->GetFirst();	\
			while(pControl)	\
			{	\
				if(pControl->GetCaption() == strMenuItem)	\
				{	\
					pControlExtSub = pControl;	\
					break;	\
				}	\
				pControlsSub->GetNext(pControl);	\
			}	\
			break;	\
		}	\
		pControls->GetNext(pControl);	\
	}	\
	if(pMenuBarSub && !pControlExtSub)	\
	{	\
		CXTPControls* pControlsSub = pMenuBarSub->GetControls();	\
		pControlsSub->AddMenuItem(menu.GetSubMenu(0), menuindex);	\
	}	\
}	\


// 删除一个OWM扩展菜单项(暂时不用,因为通过平台接口注册的方式平台可以自己删除菜单项,不需要人工释放)
#define	OWM_REMOVEMENUITEM(mainmenu, menuid, menuindex)	\
{	\
	if(aOWMMenus.GetSize() == 0)	\
		return 0;	\
	CMenu menu;	\
	get_dll_resource();	\
	menu.LoadMenu(menuid);	\
	reset_dll_resource();	\
	CString strMenuItem;	\
	menu.GetSubMenu(0)->GetMenuString(menuindex, strMenuItem, MF_BYPOSITION);	\
	CXTPCommandBar* pMenuBar = (CXTPCommandBar*)(aOWMMenus[0].pProc);	\
	CXTPControls* pControls = pMenuBar->GetControls();	\
	CXTPControl* pControl = pControls->GetFirst();	\
	CXTPCommandBar* pMenuBarSub = NULL;	\
	while(pControl)	\
	{	\
		if(pControl->GetCaption() == mainmenu)	\
		{	\
			pMenuBarSub = pControl->GetCommandBar();	\
			CXTPControls* pControlsSub = pMenuBarSub->GetControls();	\
			pControl = pControlsSub->GetFirst();	\
			while(pControl)	\
			{	\
				if(pControl->GetCaption() == strMenuItem)	\
				{	\
					pControlsSub->Remove(pControl);	\
					break;	\
				}	\
				pControlsSub->GetNext(pControl);	\
			}	\
			break;	\
		}	\
		pControls->GetNext(pControl);	\
	}	\
}	\


// 添加一个OWM扩展菜单项在主菜单的某个菜单项下面(调用平台接口)
#define	PLATUI_ADDMENUITEM(mainmenu, menuid, menuindex, proc, updateproc)	\
	{	\
		get_dll_resource();	\
		CMenu* pmenu = new CMenu();	\
		pmenu->LoadMenu(menuid);	\
		reset_dll_resource();	\
		theApp.m_pIPlatUI->AddSubMenu(mainmenu, menuindex, pmenu, proc, updateproc);	\
		delete pmenu;	\
	}	\


// 加载字符串资源宏,用于国际化(在OWM模块中使用时可以加上资源切换)
#define PLAT_LOADSTRING(s,id)	\
	CString s;	\
	s.LoadString(id);	\


// 加载字符串资源宏,用于国际化(包含资源切换)
#define DLL_LOADSTRING(s,id)	\
	get_dll_resource();	\
	CString s;	\
	s.LoadString(id);	\
	reset_dll_resource();	\


#endif // !defined(__OWM_H_)