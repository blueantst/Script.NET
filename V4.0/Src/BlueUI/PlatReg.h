////////////////////////////////////////////////////////////////////////////
//	File:		platreg.h
//	Version:	1.0.0.0
//	Created:	2000-06-01
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	Comm define of platform register.
////////////////////////////////////////////////////////////////////////////
#ifndef __PLATREG_H_
#define __PLATREG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 定义版本信息和注册信息子键
#define REG_VERSION_SUBKEY		_T("Version")		// 版本信息
#define REG_VER_PLATVERSION		_T("CurVersion")	// 平台版本
#define REG_VER_NEWVERSION		_T("NewVersion")	// 最新版本
#define REG_REGISTER			_T("Register")		// 是否已经注册
#define REG_REG_USER			_T("RegUser")		// 注册用户名
#define REG_REG_USERID			_T("RegUserID")		// 注册用户ID
#define REG_REG_EDITION			_T("Edition")		// 版本类型(0=用户版本,1=开发版本)
#define REG_REG_USE				_T("Use")			// 用途(开发/测试)
#define REG_REG_PRODUCTLINE		_T("Category")		// 分类(大类)
#define REG_REG_PRODUCT			_T("Class")			// 种类(小类)
#define REG_REG_LANGUAGE		_T("Language")		// 当前语言

// 定义平台统一标识子键
#define REG_GUID_SUBKEY			_T("EquipInfo")		// 统一标识
#define REG_GUID_PLATGUID		_T("GUID")		// 平台统一标识

// 定义Path子键
#define REG_PATH_SUBKEY			_T("Settings")		// 路径子键
#define REG_PATH_INSTALL		_T("Install")		// 安装路径
#define REG_PATH_PROJECT		_T("Project")		// 保存的工程路径(v1.0用)

// 定义帮助子键
#define REG_HELP_SUBKEY			_T("Help")
#define REG_HELP_USEAGENT		_T("UseAgent")		// 是否启用助手
#define REG_HELP_DAYTIP			_T("DayTip")		// 每日一帖文件
#define REG_HELP_OPHANDBOOK		_T("OperatorBook")	// 操作手册
#define REG_HELP_STARTPAGE		_T("Option")		// 起始页

// 定义配置信息子键
#define REG_CONFIG_SUBKEY		_T("Settings")
#define REG_CONFIG_DEFFILEEXT	_T("DefaultFileExt")// 缺省的打开文件的后缀
#define REG_CONFIG_DEFAULTTREE	_T("DefaultTree")	// 缺省的工作树页面
#define REG_CONFIG_SPLASHTIME	_T("SplashTime")	// 启动画面的延迟时间
#define REG_CONFIG_SPLASH		_T("Splash")		// 启动画面文件
#define REG_CONFIG_AUTOLOGIN	_T("AutoLogin")		// 自动登录
#define REG_CONFIG_STARTRCSVR	_T("StartRCS")		// 是否启动远程管理服务
#define REC_CONFIG_ENABLERCADMIN	_T("EnableRCA")	// 是否允许远程管理服务
#define REG_CONFIG_SHOWADDRESSBAR	_T("ShowAddressBar")// 是否显示HTML页面的地址输入条

#define REG_CONFIG_AUTO_RELOAD_DOCUMENT	_T("AutoReloadDocument")// 自动加载上一次打开的文档
#define REG_CONFIG_AUTO_OPEN_STARTPAGE	_T("AutoOpenStartPage")// 自动打开起始页

#define REG_CONFIG_IMAGESTYLE	_T("ImageStyle")		// 图标风格

#define REG_CONFIG_SKIN_TYPE			_T("SkinType")			// 当前皮肤类型
#define REG_CONFIG_SKIN_FILE			_T("SkinFile")			// 使用扩展皮肤文件
#define REG_CONFIG_SKIN_INI				_T("SkinIni")			// 皮肤文件INI
#define REG_CONFIG_SKIN_THEME			_T("Theme")				// 主题
#define REG_CONFIG_SKIN_THEMECOLOR		_T("ThemeColor")		// 主题颜色
#define REG_CONFIG_SKIN_COLORSCHEMES	_T("SkinColorSchemes")	// 颜色
#define REG_CONFIG_SKIN_FONTSIZE		_T("SkinFontSize")		// 文字大小
#define REG_CONFIG_SKIN_APPLY_FRAMESKIN	_T("SkinApplyFrameSkin")// 应用FrameSkin
#define REG_CONFIG_SKIN_APPLY_METRICS	_T("SkinApplyMetrics")	// 应用Metrics
#define REG_CONFIG_SKIN_APPLY_COLORS	_T("SkinApplyColors")	// 应用Colors

// 定义皮肤类型
enum {
	SKIN_NONE,
	SKIN_OFFICE2007,	// CodeJock Office2007 DLL File
	SKIN_CJINI,		// CodeJock INI File
	SKIN_CJSTYLE,	// CodeJock Style File
	SKIN_MSSTYLE	// Microsoft Style File
};

// 定义主题
enum {
	//themeNone,
	themeRibbon,
	themeOffice2003,
	themeOffice2007,
	themeOffice2010,
	themeVisualStudio2005,
	themeVisualStudio2008,
	themeVisualStudio2010,
	themeVisualStudio2012,
};

// 定义主题颜色
enum {
	themeColorBlue,
	themeColorSilver,
	themeColorBlack,
};

// 定义VCI子键
#define REG_VCI_SUBKEY			_T("VCI")		// VCI子键
#define REG_VCI_ENABLE_DEBUGOUT	_T("EnableDebugOut")// 允许输出组件调试信息

// 定义输出窗口子键
#define REG_OUTPUT_SUBKEY		_T("Output")
#define REG_OUTPUT_COUNT		_T("Count")			// 平台输出窗口个数
#define REG_OUTPUT_DEFAULT		_T("Default")		// 平台缺省输出窗口
#define REG_OUTPUT_LIMITTEXT	_T("LimitText")		// 输出窗口行数限制
#define REG_OUTPUT_SAVETYPE		_T("SaveType")		// 保存文件类型
#define REG_OUTPUT_TRACEID		_T("TraceID")		// Trace窗口的ID
#define REG_GRIDOUT_SUBKEY		_T("Output\\GridOut")// Grid输出窗口子键

// 定义状态行子键
#define REG_STATUSBAR_SUBKEY	_T("StatusBar")
#define REG_VIEW_DATE			_T("ViewDate")		// 显示日期
#define REG_VIEW_TIME			_T("ViewTime")		// 显示时间
#define REG_VIEW_WEEK			_T("ViewWeek")		// 显示星期
#define REG_VIEW_USER			_T("ViewUser")		// 显示登录用户
#define REG_VIEW_INFO			_T("ViewInfo")		// 显示信息
#define REG_ENABLE_SCROLL		_T("EnableScroll")	// 允许信息滚动
#define REG_SETTINGS_SUBKEY		_T("Settings")		// 设置子键
#define REG_SHOWMDITAB			_T("ShowMDITab")	// 是否显示MDITab
#define REG_MDITABALIGN			_T("MDITabAlign")	// MDITab对齐方式
#define REG_XPMODE				_T("XpMode")		// 是否XP风格界面

// 定义字体格式子键
#define REG_FORMAT_SUBKEY		_T("Format")
#define REG_FMFONT				_T("Font")			// 字体名
#define REG_FMSIZE				_T("Size")			// 大小
#define REG_FMCOLORFG			_T("ColorFg")		// 前景色
#define REG_FMCOLORBK			_T("ColorBk")		// 背景色
#define REG_FMBOLD				_T("Bold")			// 是否粗体
#define REG_FMITALIC			_T("Italic")		// 是否斜体
#define REG_FMUNDERLINE			_T("UnderLine")		// 下划线
#define REG_FMSTRIKEOUT			_T("StrikeOut")		// 删除线

#define REG_FMT_OUTPUT			_T("Plat.Output")	// 输出窗口

// 重定向参数
#define REG_DIRECT_SUBKEY	_T("interp\\Redirect")			// 重定向子键
// 以下为标准输出重定向
#define REG_DIRECT_STDOUT_OUTPUT	_T("stdout_output")		// 输出窗口
#define REG_DIRECT_STDOUT_FILE		_T("stdout_file")		// 文件
#define REG_DIRECT_STDOUT_RESULT	_T("stdout_result")		// 脚本标准输出
#define REG_DIRECT_STDOUT_PIPE		_T("stdout_pipe")		// 管道
#define REG_DIRECT_STDOUT_MEMORY	_T("stdout_memory")		// 共享内存
#define REG_DIRECT_STDOUT_ADMIN		_T("stdout_admin")		// 管理员用户
// 输出参数
#define REG_DIRECT_STDOUT_OUTPUT_ID	_T("stdout_output_id")	// 输出窗口ID

// 定义登录子键
#define REG_LOGIN_SUBKEY		_T("Login")
#define REG_LG_USERNAME			_T("UserName")		// 用户名
#define REG_LG_PASSWORD			_T("Key")			// 用户口令
#define REG_LG_DOMAIN			_T("Domain")		// 登录域
#define REG_LG_RCADMIN			_T("RCAdmin")		// 远程管理员用户名
#define REG_LG_RCPWD			_T("RCPWD")			// 远程管理员口令

// 定义OWM编辑器子键
#define REG_OWMEDIT				_T("OwmEdit")
#define REG_DEFAULTDOC			_T("DefaultDoc")	// 缺省编辑类型

#define REG_FIND_SUBKEY			_T("OwmEdit\\Find")
#define REG_REPLACE_SUBKEY		_T("OwmEdit\\Replace")
#define REG_FIND_HISTORY		_T("OwmEdit\\FindHistory")
#define REG_MATCH_CASE			_T("MatchCase")		// 区分大小写
#define REG_WHOLE_WORD			_T("WholeWord")		// 整字匹配
#define REG_FIND_WHAT			_T("FindWhat")		// 查找字串
#define REG_REPLACE_WITH		_T("ReplaceWith")	// 替换字串

#define REG_PAGE_SUBKEY			_T("OwmEdit\\PageSetup")
#define REG_MARGIN_LEFT			_T("LeftMargin")
#define REG_MARGIN_RIGHT		_T("RightMargin")
#define REG_MARGIN_TOP			_T("TopMargin")
#define REG_MARGIN_BOTTOM		_T("BottomMargin")

// 定义超级管理员信息
#define USER_SUPERADMIN			_T("Administrator")
#define USER_SUPERADMINID		_T("00001")
#define USER_SUPERADMINPWD		_T("")
#define USER_SUPERADMINPWDV		_T("")				// 口令验证
#define USER_SUPERADMINDESC		_T("SuperAdmin")

// 定义工程子键
#define REG_PROJECT_SUBKEY		_T("Project")
#define REG_PROJECT_LOADLAST	_T("LoadLast")		// 自动加载最近工程
#define REG_PROJECT_AUTOSAVE	_T("AutoSave")		// 退出自动保存工程
#define REG_PROJECT_DEFFILEEXT	_T("DefaultProjectExt")// 缺省的打开工程的后缀
#define REG_CUR_CLASS			_T("CurCategory")		// 当前工程大类
#define REG_CUR_SCLASS			_T("CurClass")		// 当前工程小类
#define REG_CUR_PROJECT			_T("CurProject")	// 当前工程
#define REG_PROJECT_DEFAULTWS	_T("DefaultWS")		// 缺省工位

// 定义最近工程子键
#define REG_RECENTPRJ_SUBKEY	_T("RecentProject")
#define REG_RECENT_PROJECT		_T("PROJECT")		// 工程
#define	MAX_RECENT_PROJECT		10					// 最多保存个数
#define ID_RECENT_PROJECT_1		900					// 第一个工程菜单ID
#define ID_RECENT_PROJECT_10	909					// 第10个工程菜单ID

// 定义工程文件标识
#define PRJ_SIGN				_T("Script.NET Project File, Copyright Blueant Software.")
#define PRJ_VERSION				_T("Version=4.0.0.0")
#define PRJ_SERIAL				_T("{6084F200-C7F8-464B-B067-A4AE5B9CE9DD}")

// 定义日志查看参数(存在系统参数表中)
#define REG_LOG_SUBKEY		_T("Log")				// 日志子键
#define REG_LOG_LISTRANGE	_T("ListRange")			// 查看范围
#define REG_LOG_LASTDAY		_T("ListLastDay")		// 最近天数
#define REG_LOG_DATARANGE	_T("ListDataRange")		// 开始日期

// 定义日志保存参数(存在系统参数表中)	
#define REG_LOGSAVE_SUBKEY		_T("Log")			// 日志保存子键
#define REG_LOGSAVE_SAVERANGE	_T("SaveRange")		// 保存范围
#define REG_LOGSAVE_LASTDAY		_T("SaveLastDay")	// 最近天数
#define REG_LOGSAVE_MAXITEM		_T("SaveMaxItem")	// 最大项数
#define REG_LOGSAVE_AUDIT_SUCC	_T("AuditSucc")		// 成功审核
#define REG_LOGSAVE_AUDIT_FAIL	_T("AuditFail")		// 失败审核
#define REG_LOGSAVE_INFOMATION	_T("Information")	// 信息
#define REG_LOGSAVE_WARNING		_T("Warning")		// 警告
#define REG_LOGSAVE_ERROR		_T("Error")			// 错误

// 资源管理配置子键
#define REG_RESMG_SUBKEY		_T("Config\\ResManager")
#define REG_RESMG_SHOWRESMG		_T("ShowResManager")	// 显示资源管理页面
#define REG_RESMG_SHOWPROJECT	_T("ShowProject")		// 显示工程树
#define REG_RESMG_SHOWEQUIP		_T("ShowEquip")			// 显示设备树
#define REG_RESMG_SHOWDEVICE	_T("ShowDevice")		// 显示仪器树
#define REG_RESMG_SHOWOWM		_T("ShowOwm")			// 显示OWM树
#define REG_RESMG_SHOWTCLINTERP	_T("ShowTclInterp")		// 显示TCL解释器树
#define REG_RESMG_SHOWTOOLS		_T("ShowTools")			// 显示工具树
#define REG_RESMG_SHOWDOCUMENT	_T("ShowDocument")		// 显示文档树

// 界面风格设置
#define REG_UISTYLE_SUBKEY		_T("Config\\UIStyle")	// 界面风格子键
#define REG_UISTYLE_HTML		_T("Html")				// Html页面风格

#endif // !defined(__PLATREG_H_)