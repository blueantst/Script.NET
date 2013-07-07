////////////////////////////////////////////////////////////////////////////
//	File:		LuaExCmd.h
//	Version:	1.0.0.0
//	Created:	2007-12-20
//
//	Author:		xieyongfang
//	E-mail:		script.net@gmail.com
//
//	Interface define of LuaExCmd.
////////////////////////////////////////////////////////////////////////////
#ifndef __LUAEXCMD_H_
#define __LUAEXCMD_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


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


// 通用登录信息结构
struct TCommLoginInfo
{
	CString	strAddress;		// 登录地址
	CString	strUserName;	// 用户名
	CString	strPassword;	// 口令
	CString	strDomain;		// 登录域
	CString	strDomainList;	// 登录域列表
	CString	strTitle;		// 标题
	CString	strInfo;		// 提示信息
	CString	strLAddress;	// 地址输入标签
	CString	strLUserName;	// 用户输入标签
	CString	strLPassword;	// 口令输入标签
	CString	strLDomain;		// 域输入标签
	CString	strCheck;		// 检查框文字
	BOOL	bCheck;			// 检查框值
	BOOL	bUseAddress;	// 使用地址字段
	BOOL	bUseUserName;	// 使用用户名字段
	BOOL	bUsePassword;	// 使用口令字段
	BOOL	bUseDomain;		// 使用登录域字段
	CString	varAddress;		// 登录地址TCL变量
	CString	varUserName;	// 用户名TCL变量
	CString	varPassword;	// 口令TCL变量
	CString	varDomain;		// 登录域TCL变量
	CString	varCheck;		// 检查框TCL变量
	CString	strFocus;		// 输入焦点

public:
	TCommLoginInfo()
	{
		strAddress		= "";
		strUserName		= "";
		strPassword		= "";
		strDomain		= "";
		strDomainList	= "";
		strTitle		= "";
		strInfo			= "";
		strLAddress		= "";
		strLUserName	= "";
		strLPassword	= "";
		strLDomain		= "";
		strCheck		= "";
		bCheck			= FALSE;
		bUseAddress		= FALSE;
		bUseUserName	= FALSE;
		bUsePassword	= FALSE;
		bUseDomain		= FALSE;
		varAddress		= "";
		varUserName		= "";
		varPassword		= "";
		varDomain		= "";
		varCheck		= "";
		strFocus		= "";
	};
};


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
};


/////////////////////////////////////////////////////////////////
// Popup control struct
struct TPopupControlInfo
{
	IInterp*	pInterp;	// 解释器指针
	CString	strTheme;		// 风格
	CString	strIcon;		// 图标
	CString	strCaption;		// 窗口标题
	CString	strTitle;		// 标题行文本
	CString	strText;		// 主文字
	CString	strLinkTitle;	// 标题行链接
	CString	strLinkText;	// 文字链接
	BOOL	bRTFText;		// 是否RTF格式文本
	int		nShowDelay;		// 显示时间
	int		nTransparency;	// 透明度
	int		nWidth;			// 窗口宽度
	int		nHight;			// 窗口高度
	int		nAddHight;		// 增加的高度
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
		bRTFText		= FALSE;
		nShowDelay		= 0;
		nTransparency	= 0;
		nWidth			= 0;
		nHight			= 0;
		nAddHight		= 0;
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
		bRTFText		= other.bRTFText;
		nShowDelay		= other.nShowDelay;
		nTransparency	= other.nTransparency;
		nWidth			= other.nWidth;
		nHight			= other.nHight;
		nAddHight		= other.nAddHight;
		asButtonText.RemoveAll();
		for(int i=0; i<other.asButtonText.GetSize(); i++)
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
		nProcType		= 0;
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
	TYPEOF_ClientMenu *pProc;	// 菜单函数
	TYPEOF_UpdateClientMenu	*pUpdateProc;	// 刷新菜单函数
	int nId;					// 菜单ID
	int nGrade;					// 权限
	int nProcType;				// 菜单函数类型(0表示普通类型,1表示带一个参数的菜单函数)
	CString	m_strAction;	// 菜单执行的动作
};


#endif	// __LUAEXCMD_H_