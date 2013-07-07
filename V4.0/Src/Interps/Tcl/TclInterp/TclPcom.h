////////////////////////////////////////////////////////////////////////////
//	File:		TclPcom.h
//	Version:	1.0.0.0
//	Created:	2002-03-04
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	Interface define of TclPcom.
////////////////////////////////////////////////////////////////////////////
#ifndef __TCLPCOM_H_
#define __TCLPCOM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 定义常用的类型
enum{
	PTYPE_VOID,		// 空
	PTYPE_CHAR,		// 有符号单字节
	PTYPE_BYTE,		// 无符号单字节
	PTYPE_SHORT,	// 有符号双字节
	PTYPE_WORD,		// 无符号双字节
	PTYPE_INT,		// 有符号四字节
	PTYPE_UINT,		// 无符号四字节
	PTYPE_CTIME,	// CTime类型
	PTYPE_POINTER,	// 指针
	PTYEP_FLOAT,	// 浮点
	PTYPE_STRING,	// 字符串
	PTYPE_CSTRING,	// CString类型
	PTYPE_CSTRINGARRAY,	// CStringArray类型
	PTYPE_UINTARRAY,	// CUIntArray类型
};

#define PTYPE_UNKNOWN	-1	// 未知类型

// 定义参数传递方式
enum{
	PMODE_DIRECT,	// 直接传递
	PMODE_REFERENCE,// 引用&
	PMODE_POINTER,	// 指针*
	PMODE_POINTER2,	// 双重指针**
	PMODE_CONST,	// 常量const
};

// 定义最大参数空间
#define	TCLPCOM_MAXPARAM		15	// 最大参数个数
#define TCLPCOM_MAXPARAMSIZE	64	// 占用空间


// 封装PCOM参数
class CTclPcomParam
{
public:
	CTclPcomParam();
	~CTclPcomParam();

	// 拷贝构造函数
	CTclPcomParam(const CTclPcomParam& other);
	// 重载赋值运算符
	CTclPcomParam& operator = (const CTclPcomParam& other);

	int	GetParamSize();
	int	GetParamType() {return m_nType;}
	BOOL NeedRefVar();
	void VarInit(CString strVar, CString strValue);
	void VarInitArray(CString strVar, CStringArray* pasValue);
	CString VarRelease();

// function
public:

// attribute
public:
	CString		m_strParamName;	// 参数名
	int			m_nType;		// 参数类型
	int			m_nMode;		// 参数传递类型
	CString		m_strDefault;	// 此参数的缺省值

	// 以下用于执行时候的临时变量,对于函数说明没有用处
	CString		m_strRefVar;	// 参数是引用的情况记录传入的TCL变量名
	int			m_nCommVar;		// 公共变量(暂时用这种简单方法)
	char		m_chChar;
	BYTE		m_byByte;
	short		m_sShort;
	WORD		m_wWord;
	int			m_nInt;
	UINT		m_uiUint;
	CTime		m_tTime;
	CTime*		m_ptTime;
	LPVOID		m_lpVoid;
	float		m_fFloat;
	LPTSTR		m_lpTSTR;
	CString		m_strString;	// 普通或指针类型的CString
	CString*	m_psString;		// 引用类型的CString
	CStringArray	m_asString;	// 引用类型的CStringArray
	CStringArray*	m_pasString;	// 指针类型的CStringArray
	CUIntArray	m_auUInt;		// 引用类型的CUIntArray
	CUIntArray*	m_pauUInt;		// 指针类型的CUIntArray
};

typedef CArray<CTclPcomParam, CTclPcomParam&> CTclPcomParamArray;


// 封装PCOM类中的函数
class CTclPcomFunc
{
public:
	CTclPcomFunc();
	~CTclPcomFunc();

	// 拷贝构造函数
	CTclPcomFunc(CTclPcomFunc& other);
	// 重载赋值运算符
	CTclPcomFunc& operator = (CTclPcomFunc& other);

// function
public:
	int	ParseParamType(CString strParam, CString& strParamName, int& nType, int& nMode, CString& strDefault);
	int	CalcTotalParamSize();
	int	AddParam(CString strParamName, int nType, int nMode, CString strDefault);
	CTclPcomParam* FindParam(CString strParam);

	void SetFuncName(CString strName) { m_strFuncName = strName; }
	CString	GetFuncName() { return m_strFuncName; }
	void SetFuncDef(CString strDef) { m_strFuncDef = strDef; }
	CString	GetFuncDef() { return m_strFuncDef; }
	BOOL EnableExec() { return m_bEnableExec; }

// attribute
public:
	CTclPcomParamArray	m_arParams;		// 参数列表
	CTclPcomParam		m_Ret;			// 返回值
	CString				m_strFuncName;	// 函数名
	CString				m_strFuncDef;	// 函数原型定义
	int					m_nLeastParams;	// 去除缺省参数后的最少参数个数
	BOOL				m_bEnableExec;	// 此函数是否可正确执行(有未知类型参数则不可执行)
};

typedef CArray<CTclPcomFunc, CTclPcomFunc&> CTclPcomFuncArray;


// 封装PCOM类
class CTclPcomClass
{
public:
	CTclPcomClass();
	~CTclPcomClass();

// function
public:
	DWORD GetRefCount() { return m_dwRefCount; }
	DWORD AddRef() { return ++m_dwRefCount; }
	DWORD ReleaseRef() { return --m_dwRefCount; }
	CString	GetVciName() { return m_strVciName; }
	void  SetVciName(CString strVciName) { m_strVciName = strVciName; }

	BOOL LoadHeadFile();
	CTclPcomFunc* FindFunction(CString strFunc);
	int  GetFunctionIndex(CString strFunc);

// attribute
public:
	DWORD			m_dwRefCount;	// 引用计数,表示当前被多少PcomObj使用
	CString			m_strVciName;	// 组件类名
	CTclPcomFuncArray	m_arFuncs;	// 函数列表
};

typedef CArray<CTclPcomClass, CTclPcomClass&> CTclPcomClassArray;


class CTclPcomObj;
// 定义回调函数信息类
class CTclPcomCallBack
{
public:
	CTclPcomCallBack();
	~CTclPcomCallBack();

// function
public:
	static CTclPcomCallBack* InstallCallBack(LPVOID pInterp, CString strScript, int nParamCount, CTclPcomObj* pObj, int nEventID);
	static BOOL UnInstallCallBack(CTclPcomObj* pObj, int nEventID);
	static CTclPcomCallBack* GetCallBack(CTclPcomObj* pObj, int nEventID);

// attribute
public:
	LPVOID		m_pInterp;			// 解释器指针
	CString		m_strCallBackScript;// 回调的脚本函数名
	int			m_nParamCount;		// 回调函数的参数个数
	LPVOID		m_pCallBackFunc;	// 回调函数指针(目前只能支持一个回调函数)
	CTclPcomObj* m_pPcomObject;		// 保存对应的实例对象指针
	int			m_nEventID;			// 回调函数事件ID
};

typedef CArray<CTclPcomCallBack, CTclPcomCallBack&> CTclPcomCallBackArray;


// 封装PCOM对象
class CTclPcomObj
{
public:
	CTclPcomObj();
	virtual ~CTclPcomObj();

// function
public:
	void* GetPcomObj() { return m_pPcomObj; }
	CTclPcomClass* GetTclPcomClass() { return m_pClass; }
	CString GetErrorInfo() { return m_strErrorInfo; }

	BOOL IsOpen(CString strVciName, CString strVer, CString strShareName);
	BOOL OpenVciObject(CString strVciName, CString strVer, CString strShareName, LPVOID pPcomObj = NULL);
	static void CloseVciObject(CTclPcomObj* pPcomObj);
	void ReleaseVciObject(BOOL bForce);
//	BOOL CallPcomFunction(CString strFuncName, CStringArray& arParams);
	void SetInterp(Tcl_Interp* interp) {m_Interp = interp;}
	Tcl_Interp* GetInterp() {return m_Interp;}

// attribute
private:
	CTclPcomClass*	m_pClass;		// 对应的类对象
	void*			m_pPcomObj;		// 对象指针
	Tcl_Interp*		m_Interp;		// 解释器指针
	CString			m_strVciName;	// 组件类名
	CString			m_strShareName;	// 组件共享名
	CString			m_strErrorInfo;	// 错误信息
	int				m_nRefCount;	// 引用计数
};

typedef CArray<CTclPcomObj, CTclPcomObj&> CTclPcomObjArray;


void CloseAllVciObject();

int Tclvci_Init(Tcl_Interp *interp);

#endif	// __TCLPCOM_H_