////////////////////////////////////////////////////////////////////////////
//	File:		CommLogin.h
//	Version:	1.0.0.0
//	Created:	2002-07-04
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	Comm define of platform comm login dialog.
////////////////////////////////////////////////////////////////////////////
#ifndef __PLAT_COMMLOGIN_H_
#define __PLAT_COMMLOGIN_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


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


#endif // !defined(__PLAT_COMMLOGIN_H_)