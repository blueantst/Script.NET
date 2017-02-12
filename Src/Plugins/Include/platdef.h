////////////////////////////////////////////////////////////////////////////
//	File:		platdef.h
//	Version:	2.0.0.0
//	Created:	2000-07-20
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	Comm define of platform.
////////////////////////////////////////////////////////////////////////////
#ifndef __PLATDEF_H_
#define __PLATDEF_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 多语言定义
#ifndef LANGUAGE_PAGE_ENGLISH
#define LANGUAGE_PAGE_ENGLISH	1033
#define LANGUAGE_PAGE_CHINESE	2052
#endif

// 语言类型
enum {
	LANGUAGE_ENGLISH,
	LANGUAGE_CHINESE,
};

// DockingPane类型
enum{
	PANE_ATTACH_WND,	// 控件类型的停靠窗口
	PANE_ATTACH_VIEW,	// 视图类型的停靠窗口
};

// 几个预定义组件的ID
#define VCIID_PLATFORM			_T("org.platform")			// 平台
#define VCIID_EDITOR			_T("org.owm.edit")			// 编辑器
#define VCIID_HTMLVIEW			_T("org.owm.htmlview")		// HtmlView组件
#define VCIID_PROJECTMANAGER	_T("org.vci.projectmanager")// 工程管理组件
#define VCIID_COMMUNICATE		_T("org.vci.communicate")	// 通信组件
#define VCIID_IPC				_T("org.vci.ipc")			// IPC组件
#define VCIID_MESSAGEQUEUE		_T("org.vci.messagequeue")	// 消息队列组件
#define VCIID_WORKSPACE			_T("org.owm.workspace")		// WorkSpace组件
#define VCIID_GREP				_T("org.owm.grep")			// 查找组件
#define VCIID_DUIVISION			_T("org.vci.duivision")		// DuiVision组件

////////////////////////////////////////////////////////////////////////////
// User define

// 定义用户权限
enum
{
	USER_SUPER,			// 超级用户
	USER_ADMIN,			// 管理员
	USER_DEVELOP,		// 开发人员
	USER_OPERATOR,		// 操作人员
	USER_NONE,			// 空
};

////////////////////////////////////////////////////////////////////////////
// Log define

// 日志操作命令
#define OC_LOG_ADDITEM			100	// 添加日志项
#define OC_LOG_DELITEM			101	// 删除日志项
#define OC_LOG_CLEAR			102	// 清空日志
#define OC_LOG_QUERY			103	// 查询日志

// 定义日志项结构
typedef struct _LOGITEM
{
	int		nLogID;			// 事件ID
	int		nLogType;		// 事件类型
	time_t	tTime;			// 时间
	CString	strFrom;		// 来源
	CString	strUser;		// 用户
	CString	strDesc;		// 描述
} LOGITEM, FAR* LPLOGITEM;

// 定义日志事件类型
enum
{
	SL_INFOMATION,			// 信息
	SL_WARNING,				// 警告
	SL_ERROR,				// 错误
	SL_AUDIT_SUCC,			// 成功审核
	SL_AUDIT_FAIL,			// 失败审核
};

//日志信息级别定义
#define LOG_LEVEL_DEBUG		0x0001		//调试信息
#define LOG_LEVEL_INFO		0x0002		//一般信息
#define LOG_LEVEL_ERROR		0x0004		//错误信息
#define LOG_LEVEL_CRITICAL	0x0008		//致命信息

////////////////////////////////////////////////////////////////////////////
// Output define

// 树形输出窗口行结构
typedef struct _TTREEITEM
{
	LPVOID		hItem;		// 树节点句柄(内部使用)
	int			nType;		// 节点类型
	int			nImage;		// 图标
	int			nParent;	// 父节点索引
	int			nCheck;		// 检查标记
	int			nBold;		// 是否粗体
	COLORREF	cText;		// 文字颜色
	COLORREF	cBack;		// 背景颜色
	int			nExpand;	// 是否展开
	int			nClickFunc;	// 双击回调函数索引
	CString		strLine;	// 内容
public:
	_TTREEITEM& operator = (const _TTREEITEM& other)
	{
		hItem	= other.hItem;
		nType	= other.nType;
		nImage	= other.nImage;
		nParent	= other.nParent;
		nCheck	= other.nCheck;
		nBold	= other.nBold;
		cText	= other.cText;
		cBack	= other.cBack;
		nExpand	= other.nExpand;
		nClickFunc=other.nClickFunc;
		strLine	=other.strLine;
		return *this;
	};
} TTREEITEM, FAR* LPTTREEITEM;

typedef CArray<TTREEITEM, TTREEITEM&> CTTreeItemArray;


////////////////////////////////////////////////////////////////////////////
// Tree Option Dialog define

typedef struct _TOPTITEM
{
	HTREEITEM	hItem;		// 树节点句柄(内部使用)
	INT			iType;		// 节点类型
	CString		lpszText;	// 信息
	CString		lpszEdit;	// 可编辑字符串
	CString		lpszList;	// Combo列表
	BOOL		bCheck;		// 检查标记
	INT			iImage;		// 图标
	INT			iParent;	// 父节点索引
	CString		strTclVar;	// TCL中变量的名字
public:
	_TOPTITEM& operator = (const _TOPTITEM& other)
	{
		hItem	= other.hItem;
		iType	= other.iType;
		lpszText= other.lpszText;
		lpszEdit= other.lpszEdit;
		lpszList= other.lpszList;
		bCheck	= other.bCheck;
		iImage	= other.iImage;
		iParent	= other.iParent;
		strTclVar=other.strTclVar;
		return *this;
	};
} TOPTITEM, FAR* LPTOPTITEM;

typedef CArray<TOPTITEM, TOPTITEM&> CTOptArray;

// 定义属性项类型
enum {
	TOPT_MSG,			// 设置标题和信息
	TOPT_HELP,			// 设置帮助文件
	TOPT_ITEM,			// 添加项
	TOPT_GROUP,			// 添加组
	TOPT_CHECKBOX,		// 添加检查框
	TOPT_RADIOBUTTON,	// 添加广播按钮
	TOPT_EDIT,			// 添加编辑框
	TOPT_COMBO,			// 添加组合框
	TOPT_FONT,			// 选择字体
	TOPT_TIME,			// 日期时间输入
	TOPT_COLOR,			// 选择颜色
	TOPT_CHOOSEDIR,		// 选择路径
	TOPT_CHOOSEFILE,	// 文件输入
};

// 定义属性项图标
enum {
	TOPTI_FOLDER,		// 文件组
	TOPTI_GROUP,		// 组
	TOPTI_EDIT,			// 编辑框
	TOPTI_COMBO,		// 组合框
	TOPTI_M_FOLDER,		// 文件组(必填项)
	TOPTI_M_GROUP,		// 组(必填项)
	TOPTI_M_EDIT,		// 编辑框(必填项)
	TOPTI_M_COMBO,		// 组合框(必填项)
	TOPTI_INFO,			// 信息
	TOPTI_PROJECT,		// 工程
	TOPTI_FILE,			// 文件
	TOPTI_VERSION,		// 版本
	TOPTI_INTERFACE,	// 接口
	TOPTI_ITEM,			// 项目
	TOPTI_HELP,			// 帮助
	TOPTI_BOARD,		// 单板
	TOPTI_WORNING,		// 告警
	TOPTI_CONNECT,		// 网络连接
	TOPTI_USER,			// 用户
	TOPTI_FONT,			// 字体
	TOPTI_TIME,			// 时间
	TOPTI_COLOR,		// 颜色
};

////////////////////////////////////////////////////////////////////////////
// CExtMenuInfo define

// 菜单函数类型
enum {
	MENU_PROC_TYPE_NORMAL,		// 普通类型菜单函数
	MENU_PROC_TYPE_PARAM_ID,	// 带一个参数（ID）的菜单函数
};

// 定义菜单项结构
typedef int (TYPEOF_EXT_ClientMenu)();
typedef void (TYPEOF_EXT_UpdateClientMenu)(CCmdUI* pCmdUI);
typedef int (TYPEOF_EXT_ClientMenuParamId)(int nId);

//
// 扩展菜单信息类
//
class CExtMenuInfo : public CObject
{
public:
	CExtMenuInfo()
	{
		m_strVciId		= "";
		m_strMainMenu	= "";
		m_strSubMenu	= "";
		pProc			= NULL;
		pUpdateProc		= NULL;
		nId				= 0;
		nGrade			= 0;
		//--------扩展成员变量----------
		nProcType		= MENU_PROC_TYPE_NORMAL;
		m_strAction		= "";
	}
	~CExtMenuInfo() {};

	CExtMenuInfo(const CExtMenuInfo& other)
	{
		m_strVciId		= other.m_strVciId;
		m_strMainMenu	= other.m_strMainMenu;
		m_strSubMenu	= other.m_strSubMenu;
		pProc			= other.pProc;
		pUpdateProc		= other.pUpdateProc;
		nId				= other.nId;
		nGrade			= other.nGrade;
		nProcType		= other.nProcType;
		m_strAction		= other.m_strAction;
	}

	CExtMenuInfo& operator = (const CExtMenuInfo& other)
	{
		m_strVciId		= other.m_strVciId;
		m_strMainMenu	= other.m_strMainMenu;
		m_strSubMenu	= other.m_strSubMenu;
		pProc			= other.pProc;
		pUpdateProc		= other.pUpdateProc;
		nId				= other.nId;
		nGrade			= other.nGrade;
		nProcType		= other.nProcType;
		m_strAction		= other.m_strAction;
		return *this;
	}

public:
	CString	m_strVciId;		// 如果是OWM的扩展菜单,则表示OWM的ID
	CString	m_strMainMenu;	// 主菜单名
	CString	m_strSubMenu;	// 子菜单名
	TYPEOF_EXT_ClientMenu *pProc;	// 菜单函数
	TYPEOF_EXT_UpdateClientMenu	*pUpdateProc;	// 刷新菜单函数
	int nId;					// 菜单ID
	int nGrade;					// 权限
	int nProcType;				// 菜单函数类型(0表示普通类型,1表示带一个参数的菜单函数)
	CString	m_strAction;	// 菜单执行的动作
};

typedef CArray<CExtMenuInfo, CExtMenuInfo&> CExtMenuInfoArray;


////////////////////////////////////////////////////////////////////////////
// TTaskDialogInfo define

interface IInterp;
// 通用任务对话框信息结构
struct TTaskDialogInfo
{
	IInterp*	pInterp;		// 解释器指针
	CString	strWindowTitle;		// 窗口标题
	CString	strVerificationText;// 验证文字
	CString	strExpandedControlText;	// 扩展控件文字
	CString	strCollapsedControlText;// 收缩控件文字
	CString	strMainInstruction;	// 主文字
	CString	strContent;			// 内容
	CString	strExpandedInformation;// 展开信息
	CString	strFooter;			// 脚注
	CString	strMainIcon;		// 主图标
	CString	strFooterIcon;		// 脚注图标
	int		nWidth;				// 窗口宽度
	BOOL	bEnableInputBox;	// 允许输入框
	CString	strInputBoxText;	// 输入框内容
	CStringArray	asReplace;	// 替换字符串数组
	CStringArray	asButtonText;// 按钮数组
	CUIntArray		auButtonID;	// 按钮ID数组
	CStringArray	asButtonScript;// 按钮脚本数组
	CStringArray	asRadioButtonText;// Radio按钮数组
	CUIntArray		auRadioButtonID;// Radio按钮ID数组
	BOOL	bEnableCommandLinks;// 允许命令链接
	BOOL	bEnableHyperlinks;	// 允许文字超链接
	BOOL	bEnableRelativePosition;// 允许根据父窗口确定窗口位置
	BOOL	bShowProgressBar;	// 显示进度条
	int		nProgressBarPos;	// 进度条位置
	int		nProgressBarRangeMin;// 进度条最小值
	int		nProgressBarRangeMax;// 进度条最大值
	BOOL	bMarquee;			// 进度条使用滑动模式
	BOOL	bExpandedByDefault;	// 默认情况下是否显示收缩的内容
	BOOL	bExpandFooterArea;	// 收缩内容是否显示在脚注区
	int		nDefaultButton;		// 默认按钮ID
	int		nDefaultRadioButton;// 默认Radio按钮ID
	BOOL	bNoDefaultRadioButton;// 没有默认的radio按钮
	BOOL	bVerifyCheckState;	// check框的状态
	BOOL	bEnableCallbackTimer;// 允许回调时钟
	int		nTimerTimeout;		// 定时器超时时间
	CString	strTimerScript;		// 定时器超时脚本
	BOOL	bCommonButtonOk;	// 显示公共按钮Ok
	BOOL	bCommonButtonYes;	// 显示公共按钮Yes
	BOOL	bCommonButtonNo;	// 显示公共按钮No
	BOOL	bCommonButtonCancel;// 显示公共按钮Cancel
	BOOL	bCommonButtonRetry;	// 显示公共按钮Retry
	BOOL	bCommonButtonClose;	// 显示公共按钮Close
	int		nSelectedButtonId;	// 返回选择的按钮ID
	int		nSelectedRadioButtonId;// 返回选择的Radio按钮ID
	BOOL	bIsVerificiationChecked;// 返回check框的状态

public:
	TTaskDialogInfo()
	{
		pInterp						= NULL;
		strWindowTitle				= "";
		strVerificationText			= "";
		strExpandedControlText		= "";
		strCollapsedControlText		= "";
		strMainInstruction			= "";
		strContent					= "";
		strExpandedInformation		= "";
		strFooter					= "";
		strFooterIcon				= "";
		nWidth						= 0;
		bEnableInputBox				= FALSE;
		strInputBoxText				= "";
		bEnableCommandLinks			= TRUE;
		bEnableHyperlinks			= TRUE;
		bEnableRelativePosition		= FALSE;
		bShowProgressBar			= FALSE;
		nProgressBarPos				= 0;
		nProgressBarRangeMin		= 0;
		nProgressBarRangeMax		= 100;
		bMarquee					= FALSE;
		bExpandedByDefault			= FALSE;
		bExpandFooterArea			= FALSE;
		nDefaultButton				= 0;
		nDefaultRadioButton			= 0;
		bNoDefaultRadioButton		= FALSE;
		bVerifyCheckState			= FALSE;
		bEnableCallbackTimer		= FALSE;
		nTimerTimeout				= 0;
		strTimerScript				= "";
		bCommonButtonOk				= TRUE;
		bCommonButtonYes			= FALSE;
		bCommonButtonNo				= FALSE;
		bCommonButtonCancel			= TRUE;
		bCommonButtonRetry			= FALSE;
		bCommonButtonClose			= FALSE;
		nSelectedButtonId			= 0;
		nSelectedRadioButtonId		= 0;
		bIsVerificiationChecked		= FALSE;
	};

	TTaskDialogInfo& operator = (const TTaskDialogInfo& other)
	{
		pInterp						= other.pInterp;
		strWindowTitle				= other.strWindowTitle;
		strVerificationText			= other.strVerificationText;
		strExpandedControlText		= other.strExpandedControlText;
		strCollapsedControlText		= other.strCollapsedControlText;
		strMainInstruction			= other.strMainInstruction;
		strContent					= other.strContent;
		strExpandedInformation		= other.strExpandedInformation;
		strFooter					= other.strFooter;
		strFooterIcon				= other.strFooterIcon;
		nWidth						= other.nWidth;
		bEnableInputBox				= other.bEnableInputBox;
		strInputBoxText				= other.strInputBoxText;
		bEnableCommandLinks			= other.bEnableCommandLinks;
		bEnableHyperlinks			= other.bEnableHyperlinks;
		bEnableRelativePosition		= other.bEnableRelativePosition;
		bShowProgressBar			= other.bShowProgressBar;
		nProgressBarPos				= other.nProgressBarPos;
		nProgressBarRangeMin		= other.nProgressBarRangeMin;
		nProgressBarRangeMax		= other.nProgressBarRangeMax;
		bMarquee					= other.bMarquee;
		bExpandedByDefault			= other.bExpandedByDefault;
		bExpandFooterArea			= other.bExpandFooterArea;
		nDefaultButton				= other.nDefaultButton;
		nDefaultRadioButton			= other.nDefaultRadioButton;
		bNoDefaultRadioButton		= other.bNoDefaultRadioButton;
		bVerifyCheckState			= other.bVerifyCheckState;
		bEnableCallbackTimer		= other.bEnableCallbackTimer;
		nTimerTimeout				= other.nTimerTimeout;
		strTimerScript				= other.strTimerScript;
		bCommonButtonOk				= other.bCommonButtonOk;
		bCommonButtonYes			= other.bCommonButtonYes;
		bCommonButtonNo				= other.bCommonButtonNo;
		bCommonButtonCancel			= other.bCommonButtonCancel;
		bCommonButtonRetry			= other.bCommonButtonRetry;
		bCommonButtonClose			= other.bCommonButtonClose;
		nSelectedButtonId			= other.nSelectedButtonId;
		nSelectedRadioButtonId		= other.nSelectedRadioButtonId;
		bIsVerificiationChecked		= other.bIsVerificiationChecked;

		int i;
		asReplace.RemoveAll();
		for(i=0; i<other.asReplace.GetSize(); i++)
		{
			asReplace.Add(other.asReplace[i]);
		}
		asButtonText.RemoveAll();
		for(i=0; i<other.asButtonText.GetSize(); i++)
		{
			asButtonText.Add(other.asButtonText[i]);
		}
		auButtonID.RemoveAll();
		for(i=0; i<other.auButtonID.GetSize(); i++)
		{
			auButtonID.Add(other.auButtonID[i]);
		}
		asButtonScript.RemoveAll();
		for(i=0; i<other.asButtonScript.GetSize(); i++)
		{
			asButtonScript.Add(other.asButtonScript[i]);
		}
		asRadioButtonText.RemoveAll();
		for(i=0; i<other.asRadioButtonText.GetSize(); i++)
		{
			asRadioButtonText.Add(other.asRadioButtonText[i]);
		}
		auRadioButtonID.RemoveAll();
		for(i=0; i<other.auRadioButtonID.GetSize(); i++)
		{
			auRadioButtonID.Add(other.auRadioButtonID[i]);
		}
		return *this;
	}
};


////////////////////////////////////////////////////////////////////////////
// PopupControl define

// 定义PopupControl内容的类型
enum {
	POPUP_ITEM_TEXT,
	POPUP_ITEM_RTF,
	POPUP_ITEM_MARKUP,
};

// Popup control struct
struct TPopupControlInfo
{
	LPVOID	pInterp;		// 解释器指针
	CString	strTheme;		// 风格
	CString	strIcon;		// 图标
	CString	strCaption;		// 窗口标题
	CString	strTitle;		// 标题行文本
	CString	strText;		// 主文字
	CString	strLinkTitle;	// 标题行链接
	CString	strLinkText;	// 文字链接
	CString	strBackgroundImage;	// 背景图片
	int		nItemType;		// 文本类型(Text,RTF,Markup)
	int		nShowDelay;		// 显示时间
	int		nTransparency;	// 透明度
	int		nWidth;			// 窗口宽度
	int		nHight;			// 窗口高度
	int		nAddHight;		// 增加的高度
	int		nAnimation;		// 动画类型
	CStringArray	asButtonText;// 按钮数组
	CStringArray	asButtonScript;// 按钮脚本数组
	CStringArray	asRadioButtonText;// Radio按钮数组

public:
	TPopupControlInfo()
	{
		pInterp			= NULL;
		strTheme		= "";
		strIcon			= "";
		strCaption		= "";
		strTitle		= "";
		strText			= "";
		strLinkTitle	= "";
		strLinkText		= "";
		strBackgroundImage = "";
		nItemType		= POPUP_ITEM_TEXT;
		nShowDelay		= 0;
		nTransparency	= 0;
		nWidth			= 0;
		nHight			= 0;
		nAddHight		= 0;
		nAnimation		= 2;
	};

	TPopupControlInfo& operator = (const TPopupControlInfo& other)
	{
		pInterp			= other.pInterp;
		strTheme		= other.strTheme;
		strIcon			= other.strIcon;
		strCaption		= other.strCaption;
		strTitle		= other.strTitle;
		strText			= other.strText;
		strLinkTitle	= other.strLinkTitle;
		strLinkText		= other.strLinkText;
		strBackgroundImage = other.strBackgroundImage;
		nItemType		= other.nItemType;
		nShowDelay		= other.nShowDelay;
		nTransparency	= other.nTransparency;
		nWidth			= other.nWidth;
		nHight			= other.nHight;
		nAddHight		= other.nAddHight;
		nAnimation		= other.nAnimation;
		asButtonText.RemoveAll();
		int i;
		for(i=0; i<other.asButtonText.GetSize(); i++)
		{
			asButtonText.Add(other.asButtonText[i]);
		}
		asButtonScript.RemoveAll();
		for(i=0; i<other.asButtonScript.GetSize(); i++)
		{
			asButtonScript.Add(other.asButtonScript[i]);
		}
		return *this;
	};
};


#endif // !defined(__PLATDEF_H_)