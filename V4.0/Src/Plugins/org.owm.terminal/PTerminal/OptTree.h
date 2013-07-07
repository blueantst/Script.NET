////////////////////////////////////////////////////////////////////////////
//	File:		OptTree.h
//	Version:	1.0.0.0
//	Created:	2002-07-04
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	Comm define of platform optiontree dialog.
////////////////////////////////////////////////////////////////////////////
#ifndef __PLAT_OPTTREE_H_
#define __PLAT_OPTTREE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CTreeOptionsDlg dialog
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
};


#endif // !defined(__PLAT_OPTTREE_H_)