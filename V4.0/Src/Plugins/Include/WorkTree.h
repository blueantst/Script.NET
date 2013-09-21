////////////////////////////////////////////////////////////////////////////
//	File:		WorkTree.h
//	Version:	1.0.0.0
//	Created:	2000-07-01
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	Comm define of Work Tree.
////////////////////////////////////////////////////////////////////////////
#ifndef __WORKTREE_H_
#define __WORKTREE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////////////////////////////////////////////////////////////////////////////
// WorkTree define

// 定义WorkSpace命令
#define OC_WS_ADDTREE			200	// 添加树窗口
#define OC_WS_ADDTREEEX			201	// 添加树窗口(扩展方式,可以设置图标等)
#define OC_WS_CLOSETREE			202	// 关闭树窗口
#define OC_WS_GETTREEINDEX		203	// 根据名字得到树索引
#define OC_WS_SETACTIVETREE		204	// 设置活动树
#define OC_WS_ACTIVECB			205	// 安装窗口激活的回调函数
#define OC_WS_TREEEXIST			206	// 判断指定树窗口是否已经存在
#define OC_WS_SETTREETYPE		207	// 设置窗口类型
#define OC_WS_SETTREEIMAGE		208	// 设置树的图标组
#define OC_WS_ADDMENU			210	// 添加菜单
#define OC_WS_ADDMENUITEM		211	// 添加菜单项
#define OC_WS_ADDTREEITEM		212	// 添加节点
#define OC_WS_DELTREEITEM		213	// 删除节点
#define OC_WS_SETITEMTYPE		214	// 设置节点类型
#define OC_WS_GETITEMTYPE		215	// 取得节点类型
#define OC_WS_SETITEMSTATE		216	// 设置节点状态
#define OC_WS_GETITEMSTATE		217	// 取得节点状态
#define OC_WS_SETITEMMENU		218	// 设置节点菜单
#define OC_WS_GETITEMMENU		219	// 取得节点菜单
#define OC_WS_SETITEMOBJECT		220	// 设置节点对象
#define OC_WS_GETITEMOBJECT		221	// 取得节点对象
#define OC_WS_SETITEMID			222	// 设置节点ID
#define OC_WS_GETITEMID			223	// 取得节点ID
#define OC_WS_SETITEMNAME		224	// 设置节点名
#define OC_WS_GETITEMNAME		225	// 取得节点名
#define OC_WS_SETITEMIMAGE		226	// 设置节点图标
#define OC_WS_GETITEMIMAGE		227	// 取得节点图标
#define OC_WS_EXPANDITEM		228	// 展开节点
#define OC_WS_COLLAPSEITEM		229	// 收缩节点
#define OC_WS_SETDCLICKSCRIPT	230	// 设置节点双击的脚本
#define OC_WS_SETITEMINFO		231	// 设置节点信息
#define OC_WS_GETITEMINFO		232	// 获取节点信息
#define	OC_WS_SEARCHITEM		233	// 查找节点
#define OC_WS_GETCHILDITEM		234	// 获取第一个子节点
#define OC_WS_GETNEXTITEM		235	// 获取下一个节点
#define OC_WS_SORTCHILDREN		236	// 对子节点排序

#define OC_WS_SETTASKTHEME		240	// 设置TaskPanel的Theme
#define OC_WS_ADDTASKITEM		241	// 添加一个TaskItem
#define OC_WS_DELTASKITEM		242	// 删除指定TaskItems
#define OC_WS_SETTASKICON		243	// 设置TaskPanel的图标或图标组
#define OC_WS_CREATTASKMENU		244	// 从指定菜单创建TaskPanel

#define OC_WS_SETPAGEMENU		250	// 设置页面菜单


// 定义根节点ID
#define UNITID_ALL				-1

typedef void (TYPEOF_ViewChange)();	// 切换树窗口回调

// 定义缺省的几个页面ID
#define TREE_ID_NAVIGATER	10005	// 导航树
#define TREE_ID_FILE		10006	// 文件树

// 定义页面图标(类型)
enum{
	PGI_FILE,		// 文件管理页面
	PGI_TEST,		// 测试树页面
	PGI_CUSTOM,		// 自定义页面
	PGI_TASKPANEL,	// TaskPanel页面
};

// 节点状态(测试节点)
enum{
	WTS_NORMAL,		// 正常
	WTS_TESTING,	// 测试
	WTS_SUCC,		// 通过
	WTS_FAIL,		// 失败
	WTS_DISABLE,	// 禁止
};

// 节点图标(文件节点)
enum{
	WTI_FILE_ALL,			// 所有工程
	WTI_FILE_PRJ,			// 工程
	WTI_FILE_SOURCE,		// 源文件
	WTI_FOLDER_OPEN,		// 打开的包
	WTI_FOLDER,				// 包
	WTI_RES_ACCEL,			// 快捷键
	WTI_RES_DLG,			// 对话框
	WTI_RES_ICON,			// 图标
	WTI_RES_MENU,			// 菜单
	WTI_RES_STRING,			// 字符串
	WTI_RES_TOOLBAR,		// 工具条
	WTI_RES_VERSION,		// 版本
	WTI_CLASS,				// 类
	WTI_CLASS_PRIVFUNC,		// 私有方法
	WTI_CLASS_FUNC,			// 方法
	WTI_CLASS_PRIVVAR,		// 私有变量
	WTI_CLASS_VAR,			// 变量
	WTI_RESOURCE,			// 资源
	WTI_DOCUMENT,			// 文档
	WTI_DEVICE,				// 仪器
	WTI_RES_MODULE,			// 模块
	WTI_TCL_LIB,			// TCL加载库
	WTI_TCL_INTERP,			// TCL解释器
	WTI_RES_ITEM,			// 元素
	WTI_DOC_HELP,			// 帮助文档
	WTI_RES_BOARD,			// 单板
	WTI_RES_SYSTEM,			// 整机
	WTI_RES_PROJECT,		// 工程
	WTI_RES_INFO,			// 信息
	WTI_DATABASE,			// 数据库
	WTI_DB_LINK,			// 已连接的数据库
	WTI_NET_DISCONNECT,		// 网络断开
	WTI_NET_CONNECT,		// 网络连接
	WTI_USER,				// 用户
	WTI_USER_LOGIN,			// 用户已登录
	WTI_SELF_CHECKOUT,		// 自己checkout的文件
	WTI_OTHER_CHECKOUT,		// 其他用户checkout的文件
	WTI_THIS,				// this
	WTI_PACKAGE_MULTI,		// 多个包
	WTI_PACKAGE_SINGLE,		// 单个包
};

#define WTT_NULL	255		// 空类型

// 树节点
struct HWORKTREEITEM
{
	DWORD		dwMask;		// 掩码
	HTREEITEM	hItem;		// 节点句柄
	HTREEITEM	hParent;	// 父节点句柄
	int			nParentID;	// 父节点ID
	int			nID;		// 节点ID
	CString		lpszName;	// 节点名
	BYTE		nImage;		// 节点图标
	BYTE		byType;		// 节点类型
	BYTE		byState;	// 节点状态
	CMenu*		pMenu;		// 节点菜单
	CObject*	pObject;	// 节点对象
	CString		strInfo;	// 节点信息
	CString		strDClick;	// 节点双击脚本
	COLORREF	crColor;	// 节点颜色
	LOGFONT		logFont;	// 节点字体
};

// 定义掩码
#define TIM_ITEM		0x00000001	// 节点句柄
#define TIM_PARENT		0x00000002	// 父节点句柄
#define TIM_PARENTID	0x00000004	// 父节点句柄
#define TIM_ID			0x00000008	// 节点ID
#define TIM_NAME		0x00000010	// 节点名
#define TIM_IMAGE		0x00000020	// 节点图标
#define TIM_TYPE		0x00000040	// 节点类型
#define TIM_STATE		0x00000080	// 节点状态
#define TIM_MENU		0x00000100	// 节点菜单
#define TIM_OBJECT		0x00000200	// 节点对象
#define	TIM_INFO		0x00000400	// 节点信息串
#define TIM_DCLICK		0x00000800	// 节点双击脚本
#define TIM_COLOR		0x01000000	// 节点颜色
#define TIM_BOLD		0x02000000	// 节点是否粗体
#define TIM_FONT		0x04000000	// 节点字体
#define TIM_SPECIAL		0x08000000	// 特殊节点或组
#define TIM_TRAVERSAL	0x00001000	// 是否递归遍历
#define TIM_UPTRAVERSAL	0x00002000	// 向上遍历,用于设置父节点状态
#define TIM_FRESHALL	0x00004000	// 是否复位所有节点(FALSE=不复位禁止节点)
#define TIM_CHILDONLY	0x00008000	// 仅对子节点操作


// 定义菜单函数
typedef void (TYPEOF_Proc)(CObject* pThis);
typedef void (TYPEOF_UpdateProc)(CCmdUI* pCmdUI, CObject* pThis);

// 菜单项ID与处理函数结构
struct HWORKMENUITEM
{
	BYTE				byType;		// 节点类型(和HWORKTREEDATA对应)
	UINT				nID;		// 菜单项ID
	CString				strItem;	// 菜单显示字符串
	CString				strCommand;	// 菜单脚本
	TYPEOF_Proc			*Proc;		// 菜单函数
	TYPEOF_UpdateProc	*UpdateProc;// 菜单刷新函数
};

typedef CArray<HWORKMENUITEM, HWORKMENUITEM> CMenuItemArray;

/////////////////////////////////////////////////////////////////
// TaskPanel define

// TaskPanel Item类型
enum{
	WTT_GROUP,
	WTT_TEXT,
	WTT_LINK,
	WTT_MASHUP,
};

// TaskPanel项
struct HWORKTASKITEM
{
	DWORD		dwMask;		// 掩码
	int			nType;		// 类型(组/文字/链接/Mashup)
	int			nID;		// ID
	int			nImage;		// 图标
	int			nParent;	// 父节点ID
	CString		strCaption;	// 标题
	CString		strTooltip;	// Tooltip
	CString		strInfo;	// 信息
	CString		strDClick;	// 双击脚本
	LPVOID		lpItem;		// PanelItem对象
	COLORREF	crColor;	// 颜色
	LOGFONT		logFont;	// 字体
};

typedef CArray<HWORKTASKITEM, HWORKTASKITEM> CTaskPanelItemArray;


// 创建树的命令结构
struct STRU_ADDTREEEX
{
	CString	strTreeName;	// 窗口名
	CString	strIconFile;	// 图标文件
	int		nTreeType;		// 类型
	int		nAttachPaneId;	// 创建在哪个窗口后面
	CString	strBmpFile;		// 图片文件名
	int		nPaneId;		// 指定的窗口PandeId
};

#endif // !defined(__WORKTREE_H_)