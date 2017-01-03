////////////////////////////////////////////////////////////////////////////
//	File:		termreg.h
//	Version:	1.0.0.0
//	Created:	2000-06-01
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	Comm define of PTerminal register.
////////////////////////////////////////////////////////////////////////////
#ifndef __TERMREG_H_
#define __TERMREG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 定义登录子键
#define REG_LOGIN_SUBKEY		_T("PTerminal\\Login")
#define REG_LG_ADDRESS			_T("Address")		// 登录地址
#define REG_LG_USERNAME			_T("UserName")		// 用户名
#define REG_LG_DOMAIN			_T("Domain")		// 登陆域



#endif // !defined(__TERMREG_H_)