////////////////////////////////////////////////////////////////////////////
//	File:		Tools.h
//	Version:	1.0.0.0
//	Created:	2007-08-26
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	Tools manager
////////////////////////////////////////////////////////////////////////////
#ifndef __TOOLS_H_
#define __TOOLS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
// 工具状态行Pane类
//
class CToolStatusPane : public CObject
{
public:
	CToolStatusPane();
	~CToolStatusPane();

	CToolStatusPane(const CToolStatusPane& other)
	{
		m_nID			= other.m_nID;
		m_nWidth		= other.m_nWidth;
		m_strText		= other.m_strText;
		m_strTooltip	= other.m_strTooltip;
		m_strIcon		= other.m_strIcon;
		m_strAction		= other.m_strAction;
	}

	CToolStatusPane& operator = (const CToolStatusPane& other)
	{
		m_nID			= other.m_nID;
		m_nWidth		= other.m_nWidth;
		m_strText		= other.m_strText;
		m_strTooltip	= other.m_strTooltip;
		m_strIcon		= other.m_strIcon;
		m_strAction		= other.m_strAction;
		return *this;
	}

public:
	UINT	m_nID;				// Pane ID
	int		m_nWidth;			// Pane宽度
	CString	m_strText;			// Pane文字
	CString	m_strTooltip;		// Pane提示信息
	CString	m_strIcon;			// Pane图标
	CString	m_strAction;		// 动作
};

typedef CArray<CToolStatusPane, CToolStatusPane&> CToolStatusPaneArray;

//
// DockingPane信息类
//
class CToolDockingPane : public CObject
{
public:
	CToolDockingPane()
	{
		m_bFileDefined = FALSE;
		m_strName	= "";
		m_nId		= 0;
		m_nDirection= 0;
		m_strIcon	= "";
		m_nAttachType= PANE_ATTACH_WND;
		m_bAutoCreate= FALSE;
		m_bSave		= FALSE;
		m_strOwmId	= "";
	}
	~CToolDockingPane() {};

	CToolDockingPane(const CToolDockingPane& other)
	{
		m_bFileDefined	= other.m_bFileDefined;
		m_strName		= other.m_strName;
		m_nId			= other.m_nId;
		m_nDirection	= other.m_nDirection;
		m_strIcon		= other.m_strIcon;
		m_nAttachType	= other.m_nAttachType;
		m_bAutoCreate	= other.m_bAutoCreate;
		m_bSave			= other.m_bSave;
		m_strOwmId		= other.m_strOwmId;
	}

	CToolDockingPane& operator = (const CToolDockingPane& other)
	{
		m_bFileDefined	= other.m_bFileDefined;
		m_strName		= other.m_strName;
		m_nId			= other.m_nId;
		m_nDirection	= other.m_nDirection;
		m_strIcon		= other.m_strIcon;
		m_nAttachType	= other.m_nAttachType;
		m_bAutoCreate	= other.m_bAutoCreate;
		m_bSave			= other.m_bSave;
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
	BOOL	m_bSave;		// 是否在平台关闭时保存位置
	CWnd*	m_pWnd;
	CString	m_strOwmId;		// 所属OWM模块ID
};

typedef CArray<CToolDockingPane, CToolDockingPane&> CToolDockingPaneArray;

//
// 工具信息类
//
class CTool : public CObject
{
public:
	CTool();	
	~CTool();

	CTool(CString strPath);

	CTool& operator = (const CTool& other)
	{
		m_nMenuIndex= other.m_nMenuIndex;
		m_strId		= other.m_strId;
		m_strName	= other.m_strName;
		m_strVersion= other.m_strVersion;
		m_strProvider= other.m_strProvider;
		m_strPath	= other.m_strPath;
		m_strCategory= other.m_strCategory;
		m_bShowMenu	= other.m_bShowMenu;
		m_bShowTask	= other.m_bShowTask;
		m_bShowToolbar = other.m_bShowToolbar;
		m_bShowStatusbar = other.m_bShowStatusbar;
		m_strType	= other.m_strType;
		m_strVciId	= other.m_strVciId;
		m_strInstanceName	= other.m_strInstanceName;
		m_strExecFile= other.m_strExecFile;
		m_strExecCommand	= other.m_strExecCommand;
		for(int i=0; i<other.m_arStatusPane.GetSize(); i++)
		{
			m_arStatusPane.Add((CToolStatusPane &)other.m_arStatusPane[i]);
		}
		for(int i=0; i<other.m_arDockingPane.GetSize(); i++)
		{
			m_arDockingPane.Add((CToolDockingPane &)other.m_arDockingPane[i]);
		}
		return *this;
	}

	BOOL LoadInfo();
	void ShowAboutBox();
	BOOL Init();
	BOOL LoadDockingPane(int nID);
	BOOL Run();

public:
	int		m_nMenuIndex;	// 菜单的索引位置
	CString	m_strId;		// 工具ID
	CString	m_strName;		// 工具名
	CString	m_strVersion;	// 工具版本
	CString	m_strProvider;	// 开发商
	CString	m_strPath;		// 工具路径
	CString	m_strCategory;	// 分类
	BOOL	m_bShowMenu;	// 是否在菜单中显示
	BOOL	m_bShowTask;	// 是否在任务栏显示
	BOOL	m_bShowToolbar;	// 是否在工具栏显示
	BOOL	m_bShowStatusbar;	// 是否在状态栏显示
	CString	m_strType;		// 执行类型
	CString	m_strVciId;		// 执行VCI id
	CString	m_strInstanceName;// 执行实例名
	CString	m_strExecFile;	// 执行文件
	CString	m_strExecCommand;// 执行命令
	CToolStatusPaneArray m_arStatusPane; // 状态行Pane数组
	CToolDockingPaneArray m_arDockingPane;	// DockingPane数组
};

typedef CArray<CTool, CTool&> CToolArray;




#endif //__TOOLS_H_