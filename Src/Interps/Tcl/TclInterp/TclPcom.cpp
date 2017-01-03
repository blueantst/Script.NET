////////////////////////////////////////////////////////////////////////////
//	File:		TclPcom.cpp
//	Version:	1.0.0.0
//	Created:	2002-04-03
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	implementation of use PCOM in TCL script.
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TclPcom.h"
#include "CTclInterp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define VCI_NAME_PLATUI _T("org.blueui.platui")

// 验证TCL扩展命令权限宏
#define VERIFY_TCLCMD_RIGHT(right)	\
/*	if(right < theApp.m_CurUser.nUserRight)	\
	{	\
		interp->result = "This tcl command request higher user rights.";	\
		return TCL_ERROR;	\
	}else	\
	if(interp == theApp.m_pRCTclInterp && right < theApp.m_nRCUserRights)	\
	{	\
		interp->result = "This tcl command request higher rights for remote user.";	\
		return TCL_ERROR;	\
	}	\
*/

static IPlatUI* g_pIPlatUI = NULL;		// 平台接口指针
static CTclPcomObj* g_pTclPcomObjPlatUI = NULL;	// 平台接口TclVci对象
static CPtrList g_TclPcomCallBackList;	// PCOM回调对象列表

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTclPcomParam::CTclPcomParam()
{
	m_strParamName	= "";
	m_nType			= PTYPE_INT;
	m_nMode			= PMODE_DIRECT;
	m_strDefault	= "";
	//
	m_strRefVar		= "";
	m_nCommVar		= 0;
	m_chChar		= 0;
	m_byByte		= 0;
	m_sShort		= 0;
	m_wWord			= 0;
	m_nInt			= 0;
	m_uiUint		= 0;
	m_tTime			= CTime(0);
	m_ptTime		= NULL;
	m_lpVoid		= 0;
	m_fFloat		= 0;
	m_lpTSTR		= NULL;
	m_strString		= "";
	m_psString		= NULL;
	m_pasString		= NULL;
	m_pauUInt		= NULL;
}

CTclPcomParam::~CTclPcomParam()
{
}

CTclPcomParam::CTclPcomParam(const CTclPcomParam& other)
{
	m_strParamName	= other.m_strParamName;
	m_nType			= other.m_nType;
	m_nMode			= other.m_nMode;
	m_strDefault	= other.m_strDefault;
	//
	m_strRefVar		= other.m_strRefVar;
	m_nCommVar		= other.m_nCommVar;
	m_chChar		= other.m_chChar;
	m_byByte		= other.m_byByte;
	m_sShort		= other.m_sShort;
	m_wWord			= other.m_wWord;
	m_nInt			= other.m_nInt;
	m_uiUint		= other.m_uiUint;
	m_tTime			= other.m_tTime;
	m_ptTime = NULL;
	if(other.m_ptTime != NULL)
	{
		m_ptTime		= new CTime(*(other.m_ptTime));
	}
	m_lpVoid		= other.m_lpVoid;
	m_fFloat		= other.m_fFloat;
	m_lpTSTR		= NULL;
	if(other.m_lpTSTR != NULL)
	{
		m_lpTSTR		= new char[strlen(other.m_lpTSTR)];
		strcpy(m_lpTSTR, other.m_lpTSTR);
	}
	m_strString		= other.m_strString;
	m_psString = NULL;
	if(other.m_psString != NULL)
	{
		m_psString		= new CString(*(other.m_psString));
	}
	m_asString.RemoveAll();
	int i;
	for(i=0; i<other.m_asString.GetSize(); i++)
	{
		m_asString.Add(other.m_asString[i]);
	}
	m_pasString = NULL;
	if(other.m_pasString != NULL)
	{
		m_pasString = new CStringArray();
		for(i=0; i<other.m_pasString->GetSize(); i++)
		{
			m_pasString->Add(other.m_pasString->GetAt(i));
		}
	}
	m_auUInt.RemoveAll();
	for(i=0; i<other.m_auUInt.GetSize(); i++)
	{
		m_auUInt.Add(other.m_auUInt[i]);
	}
	m_pauUInt = NULL;
	if(other.m_pauUInt != NULL)
	{
		m_pauUInt = new CUIntArray();
		for(i=0; i<other.m_pauUInt->GetSize(); i++)
		{
			m_pauUInt->Add(other.m_pauUInt->GetAt(i));
		}
	}
}

//////////////////////////////////////////////////////////////////////
// 重载赋值操作符
//////////////////////////////////////////////////////////////////////
CTclPcomParam& CTclPcomParam::operator = (const CTclPcomParam& other)
{
	m_strParamName	= other.m_strParamName;
	m_nType			= other.m_nType;
	m_nMode			= other.m_nMode;
	m_strDefault	= other.m_strDefault;
	//
	m_strRefVar		= other.m_strRefVar;
	m_nCommVar		= other.m_nCommVar;
	m_chChar		= other.m_chChar;
	m_byByte		= other.m_byByte;
	m_sShort		= other.m_sShort;
	m_wWord			= other.m_wWord;
	m_nInt			= other.m_nInt;
	m_uiUint		= other.m_uiUint;
	m_tTime			= other.m_tTime;
	m_ptTime = NULL;
	if(other.m_ptTime != NULL)
	{
		m_ptTime		= new CTime(*(other.m_ptTime));
	}
	m_lpVoid		= other.m_lpVoid;
	m_fFloat		= other.m_fFloat;
	if(other.m_lpTSTR != NULL)
	{
		m_lpTSTR		= new char[strlen(other.m_lpTSTR)];
		strcpy(m_lpTSTR, other.m_lpTSTR);
	}
	m_strString		= other.m_strString;
	if(other.m_psString != NULL)
	{
		m_psString		= new CString(*(other.m_psString));
	}
	m_asString.RemoveAll();
	int i;
	for(i=0; i<other.m_asString.GetSize(); i++)
	{
		m_asString.Add(other.m_asString[i]);
	}
	if(other.m_pasString != NULL)
	{
		m_pasString = new CStringArray();
		for(i=0; i<other.m_pasString->GetSize(); i++)
		{
			m_pasString->Add(other.m_pasString->GetAt(i));
		}
	}
	m_auUInt.RemoveAll();
	for(i=0; i<other.m_auUInt.GetSize(); i++)
	{
		m_auUInt.Add(other.m_auUInt[i]);
	}
	m_pauUInt = NULL;
	if(other.m_pauUInt != NULL)
	{
		m_pauUInt = new CUIntArray();
		for(i=0; i<other.m_pauUInt->GetSize(); i++)
		{
			m_pauUInt->Add(other.m_pauUInt->GetAt(i));
		}
	}

	return *this;
};

//////////////////////////////////////////////////////////////////////
// 获取参数占用空间
//////////////////////////////////////////////////////////////////////
int CTclPcomParam::GetParamSize()
{
	switch(m_nType)
	{
	case PTYPE_VOID: return 0;
	case PTYPE_CHAR: return sizeof(char);
	case PTYPE_BYTE: return sizeof(BYTE);
	case PTYPE_SHORT: return sizeof(short);
	case PTYPE_WORD: return sizeof(WORD);
	case PTYPE_INT: return sizeof(int);
	case PTYPE_UINT: return sizeof(UINT);
	case PTYPE_CTIME: return sizeof(LPVOID);
	case PTYPE_POINTER: return sizeof(LPVOID);
	case PTYEP_FLOAT: return sizeof(float);
	case PTYPE_STRING: return sizeof(LPCTSTR);
	case PTYPE_CSTRING: return sizeof(LPVOID);
	case PTYPE_CSTRINGARRAY: return sizeof(LPVOID);
	case PTYPE_UINTARRAY: return sizeof(LPVOID);
	default: return 0;
	}
};

//////////////////////////////////////////////////////////////////////
// 判断是否需要用于回传变量的TCL变量名
//////////////////////////////////////////////////////////////////////
BOOL CTclPcomParam::NeedRefVar()
{
	// 引用类型必须要,指针类型可能需要
	return (PMODE_DIRECT != m_nMode);
}

//////////////////////////////////////////////////////////////////////
// 根据输入的变量信息初始化相应变量
//////////////////////////////////////////////////////////////////////
void CTclPcomParam::VarInit(CString strVar, CString strValue)
{
	// 如果是缺省参数,并且未赋值,则按照缺省参数赋值
	if((strValue == "") && (m_strDefault != ""))
	{
		strValue = m_strDefault;
	}

	// 如果是引用或指针方式,需要保存TCL变量名,用于回传参数
	if(NeedRefVar())
	{
		m_strRefVar = strVar;
	}else
	{
		m_strRefVar = "";
	}

	// 赋初值
	switch(m_nType)
	{
	case PTYPE_CHAR:
		if(strValue.GetLength() > 0)
			m_chChar = (char)(strValue[0]);
		else
			m_chChar = '\0';
		m_nCommVar = m_chChar;
		break;
	case PTYPE_BYTE:
		m_byByte = (BYTE)(atoi(strValue));
		if(NeedRefVar())
			m_nCommVar = (int)(void*)(&m_byByte);	
		else
			m_nCommVar = m_byByte;
		break;
	case PTYPE_SHORT:
		m_sShort = (short)(atoi(strValue));
		if(NeedRefVar())
			m_nCommVar = (int)(void*)(&m_sShort);	
		else
			m_nCommVar = m_sShort;
		break;
	case PTYPE_WORD:
		m_wWord = (WORD)(atoi(strValue));
		if(NeedRefVar())
			m_nCommVar = (int)(void*)(&m_wWord);	
		else
			m_nCommVar = m_wWord;
		break;
	case PTYPE_INT:
		m_nInt = (int)(atoi(strValue));
		if(NeedRefVar())
			m_nCommVar = (int)(void*)(&m_nInt);			
		else
			m_nCommVar = m_nInt;
		break;
	case PTYPE_UINT:
		m_uiUint = (UINT)(atoi(strValue));
		if(NeedRefVar())
			m_nCommVar = (int)(void*)(&m_uiUint);	
		else
			m_nCommVar = m_uiUint;
		break;
	case PTYPE_CTIME:
		{
			if(NeedRefVar())
			{
				m_tTime = CTime((time_t)(atoi(strValue)));
				m_nCommVar = (int)(void*)(&m_tTime);
			}else
			{
				m_ptTime = new CTime((time_t)(atoi(strValue)));
				m_nCommVar = (int)(m_ptTime->GetTime());
			}
		}
		break;
	case PTYPE_POINTER:
		m_lpVoid = (LPVOID)(atoi(strValue));
		if(NeedRefVar())
			m_nCommVar = (int)(void*)(&m_lpVoid);	
		else
			m_nCommVar = (int)m_lpVoid;
		break;
	case PTYEP_FLOAT:
		m_fFloat = atof(strValue);
		if(NeedRefVar())
			m_nCommVar = (int)(void*)(&m_fFloat);	
		else
			m_nCommVar = m_fFloat;
		break;
	case PTYPE_STRING:
		m_lpTSTR = new char[strValue.GetLength()+1];
		strcpy(m_lpTSTR, strValue);
		m_nCommVar = (int)(void*)m_lpTSTR;
		break;
	case PTYPE_CSTRING:
		{
			if(NeedRefVar())
			{
				m_strString = strValue;
				m_nCommVar = (int)(void*)(&m_strString);
			}else
			{
				m_psString = new CString(strValue);
				//m_nCommVar = (int)(void*)m_psString;
				m_nCommVar = (int)(LPCTSTR)(*m_psString);
			}
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////
// 根据输入的变量信息初始化相应变量
//////////////////////////////////////////////////////////////////////
void CTclPcomParam::VarInitArray(CString strVar, CStringArray* pasValue)
{
	// 如果是引用或指针方式,需要保存TCL变量名,用于回传参数
	if(NeedRefVar())
	{
		m_strRefVar = strVar;
	}else
	{
		m_strRefVar = "";
	}
	if(pasValue == NULL)
	{
		return;
	}

	// 赋初值
	if(PTYPE_CSTRINGARRAY == m_nType)
	{
		if(NeedRefVar())
		{
			for(int i=0; i<pasValue->GetSize(); i++)
			{
				m_asString.Add(pasValue->GetAt(i));
			}
			m_nCommVar = (int)(void*)(&m_asString);
		}else
		{
			m_pasString = new CStringArray();
			for(int i=0; i<pasValue->GetSize(); i++)
			{
				m_pasString->Add(pasValue->GetAt(i));
			}
			m_nCommVar = (int)(void*)m_pasString;
		}
	}else
	if(PTYPE_UINTARRAY == m_nType)
	{
		if(NeedRefVar())
		{
			for(int i=0; i<pasValue->GetSize(); i++)
			{
				m_auUInt.Add(atoi(pasValue->GetAt(i)));
			}
			m_nCommVar = (int)(void*)(&m_auUInt);
		}else
		{
			m_pauUInt = new CUIntArray();
			for(int i=0; i<pasValue->GetSize(); i++)
			{
				m_pauUInt->Add(atoi(pasValue->GetAt(i)));
			}
			m_nCommVar = (int)(void*)m_pauUInt;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// 释放变量(主要是动态分配的一些变量),
// 对于引用或指针类型会返回变化后的变量(转换为字符串)
//////////////////////////////////////////////////////////////////////
CString CTclPcomParam::VarRelease()
{
	CString strRet = "";

	// 转换为字符串,并删除动态分配的空间
	switch(m_nType)
	{
	case PTYPE_CTIME:
		{
			if(NeedRefVar())
			{
				strRet.Format("%d", m_tTime.GetTime());
			}else
			{
				delete (void*)m_ptTime;
			}
		}
		break;
	case PTYPE_STRING:
		if(NeedRefVar())
			strRet = m_lpTSTR;
		delete m_lpTSTR;
		break;
	case PTYPE_CSTRING:
		{
			if(NeedRefVar())
			{
				strRet = m_strString;
			}else
			{
				delete (void*)m_psString;
			}
		}
		break;
	case PTYPE_CSTRINGARRAY:
		{
			if(!NeedRefVar())
			{
				delete (void*)m_pasString;
			}
		}
		break;
	case PTYPE_UINTARRAY:
		{
			if(!NeedRefVar())
			{
				delete (void*)m_pauUInt;
			}
		}
		break;
	}

	// 判断是否需要回传变量
	if(!NeedRefVar())
	{
		return strRet;
	}

	// 转换为字符串,并删除动态分配的空间
	switch(m_nType)
	{
	case PTYPE_CHAR:
		strRet = m_chChar;
		break;
	case PTYPE_BYTE:
		strRet.Format("%d", m_byByte);
		break;
	case PTYPE_SHORT:
		strRet.Format("%d", m_sShort);
		break;
	case PTYPE_WORD:
		strRet.Format("%d", m_wWord);
		break;
	case PTYPE_INT:
		strRet.Format("%d", m_nInt);
		break;
	case PTYPE_UINT:
		strRet.Format("%u", m_uiUint);
		break;
	case PTYPE_POINTER:
		strRet.Format("%d", m_lpVoid);
		break;
	case PTYEP_FLOAT:
		if(m_fFloat < 1)
		{
			// 小于0.3的浮点数直接转换为字符串会失败,必须设置为%.6f才可以
			strRet.Format("%.6f", m_fFloat);
		}else
		{
			strRet.Format("%f", m_fFloat);
		}
		break;
	}

	return strRet;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTclPcomFunc::CTclPcomFunc()
{
	m_strFuncName	= "";
	m_strFuncDef	= "";
	m_nLeastParams	= 0;
	m_bEnableExec	= TRUE;
}

CTclPcomFunc::~CTclPcomFunc()
{
}

//////////////////////////////////////////////////////////////////////
// 重载赋值操作符
//////////////////////////////////////////////////////////////////////
CTclPcomFunc::CTclPcomFunc(CTclPcomFunc& other)
{
	m_strFuncName	= other.m_strFuncName;
	m_strFuncDef	= other.m_strFuncDef;
	m_nLeastParams	= other.m_nLeastParams;
	m_bEnableExec	= other.m_bEnableExec;
	m_Ret			= other.m_Ret;
	m_arParams.RemoveAll();
	for(int i=0; i<other.m_arParams.GetSize(); i++)
	{
		m_arParams.Add(other.m_arParams[i]);
	}
};

//////////////////////////////////////////////////////////////////////
// 重载赋值操作符
//////////////////////////////////////////////////////////////////////
CTclPcomFunc& CTclPcomFunc::operator = (CTclPcomFunc& other)
{
	m_strFuncName	= other.m_strFuncName;
	m_strFuncDef	= other.m_strFuncDef;
	m_nLeastParams	= other.m_nLeastParams;
	m_bEnableExec	= other.m_bEnableExec;
	m_Ret			= other.m_Ret;
	m_arParams.RemoveAll();
	for(int i=0; i<other.m_arParams.GetSize(); i++)
	{
		m_arParams.Add(other.m_arParams[i]);
	}

	return *this;
};

//////////////////////////////////////////////////////////////////////
// 解析参数名,参数类型和传递方式
// 返回值: 参数类型
//////////////////////////////////////////////////////////////////////
int	CTclPcomFunc::ParseParamType(CString strParam, CString& strParamName, int& nType, int& nMode, CString& strDefault)
{
	strDefault = "";
	
	strParamName = "";
	// 去除各种干扰
	strParam.Replace("\t", " ");	// 替换Tab符
	strParam.Replace("\r", "");		// 替换\r
	strParam.Replace("\n", "");		// 替换\n
	strParam.TrimLeft();
	strParam.TrimRight();

	// 查找是否有缺省值
	int nPos = strParam.Find("=");
	if(nPos != -1)
	{
		strDefault = strParam.Right(strParam.GetLength()-nPos-1);
		strDefault.TrimLeft();
		strParam = strParam.Left(nPos);
		strParam.TrimRight();
	}

	// 将参数类型和参数名分开
	nPos = strParam.ReverseFind(' ');
	if(nPos != -1)
	{
		strParamName = strParam.Right(strParam.GetLength() - nPos -1);
		strParamName.TrimLeft();
		strParam = strParam.Left(nPos);
		while(strParamName.FindOneOf("*& ") == 0)
		{
			if(('*' == strParamName[0]) || ('&' == strParamName[0]))
			{
				strParam += strParamName[0];
			}
			strParamName.Delete(0, 1);
		}
	}

	// 解析const关键字,目前先去掉const关键字
	if(strParam.Find("const ") == 0)
	{
		strParam.Delete(0, 6);
		strParam.TrimLeft();
	}
	nPos = strParam.Find(" const");
	if((nPos > 0) && (nPos == (strParam.GetLength()-6)))
	{
		strParam = strParam.Left(strParam.GetLength()-6);
		strParam.TrimRight();
	}

	// 解析参数类型
	CString strMode = "";
	while(strParam.FindOneOf("*&") == (strParam.GetLength() - 1))
	{
		strMode += strParam[strParam.GetLength() - 1];
		strParam = strParam.Left(strParam.GetLength()-1);
	}

	// 参数传递类型
	if(strMode == "")
	{
		nMode = PMODE_DIRECT;
	}else
	if(strMode == "&")
	{
		nMode = PMODE_REFERENCE;
	}else
	if(strMode == "*")
	{
		nMode = PMODE_POINTER;
	}else
	if(strMode == "**")
	{
		nMode = PMODE_POINTER2;
	}

	// 参数类型
	if(strParam == "void")
	{
		if(nMode == PMODE_POINTER)
		{
			// 如果为void*则等同于LPVOID
			nType = PTYPE_POINTER;
			nMode = PMODE_DIRECT;
		}else
		{
			nType = PTYPE_VOID;
		}
	}else
	if(strParam == "char" || strParam == "CHAR" || strParam == "bool")
	{
		nType = PTYPE_CHAR;
	}else
	if(strParam == "BYTE" || strParam == "unsigned char")
	{
		nType = PTYPE_BYTE;
	}else
	if(strParam == "short")
	{
		nType = PTYPE_SHORT;
	}else
	if(strParam == "WORD" || strParam == "unsigned short")
	{
		nType = PTYPE_WORD;
	}else
	if(strParam == "int" || strParam == "BOOL" || strParam == "long" || strParam == "time_t"
		|| strParam == "CDocument")
	{
		// 注: CDocument等特殊处理,当作整形看待
		nType = PTYPE_INT;
	}else
	if(strParam == "UINT" || strParam == "unsigned int" || strParam == "DWORD" || strParam == "ULONG" || strParam == "unsigned long" || strParam == "HANDLE" || strParam == "WPARAM" || strParam == "LPARAM")
	{
		nType = PTYPE_UINT;
	}else
	if(strParam == "CTime")
	{
		nType = PTYPE_CTIME;
	}else
	if(strParam == "LPVOID")
	{
		nType = PTYPE_POINTER;
	}else
	if(strParam == "float")
	{
		nType = PTYEP_FLOAT;
	}else
	if(strParam == "LPCSTR" || strParam == "LPCTSTR" || strParam == "LPSTR" || strParam == "PTSTR")
	{
		nType = PTYPE_STRING;
	}else
	if(strParam == "CString")
	{
		nType = PTYPE_CSTRING;
	}else
	if(strParam == "CStringArray")
	{
		nType = PTYPE_CSTRINGARRAY;
	}else
	if((strParam == "CUIntArray") || (strParam == "CPtrArray"))
	{
		nType = PTYPE_UINTARRAY;
	}else
	{
		nType = PTYPE_UNKNOWN;
	}

	return nType;
}

//////////////////////////////////////////////////////////////////////
// 计算函数的参数总长度
//////////////////////////////////////////////////////////////////////
int	CTclPcomFunc::CalcTotalParamSize()
{
	int nSize = 0;
	for(int i=0; i<m_arParams.GetSize(); i++)
	{
		nSize += m_arParams[i].GetParamSize();
	}
	return nSize;
}

//////////////////////////////////////////////////////////////////////
// 添加一个参数
// 返回值: 目前的参数个数
//////////////////////////////////////////////////////////////////////
int	CTclPcomFunc::AddParam(CString strParamName, int nType, int nMode, CString strDefault)
{
	CTclPcomParam PcomParam;
	PcomParam.m_strParamName	= strParamName;
	PcomParam.m_nType			= nType;
	PcomParam.m_nMode			= nMode;
	PcomParam.m_strDefault		= strDefault;
	m_arParams.Add(PcomParam);
	return m_arParams.GetSize();
}

//////////////////////////////////////////////////////////////////////
// 查找参数
//////////////////////////////////////////////////////////////////////
CTclPcomParam* CTclPcomFunc::FindParam(CString strParam)
{
	for(int i=0; i<m_arParams.GetSize(); i++)
	{
		if(strParam == m_arParams[i].m_strParamName)
		{
			return &(m_arParams[i]);
		}
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static CPtrList	g_TclPcomClassList;	// PCOM对象列表

CTclPcomClass::CTclPcomClass()
{
	m_dwRefCount	= 0;
	m_strVciName		= "";
}

CTclPcomClass::~CTclPcomClass()
{
}

//////////////////////////////////////////////////////////////////////
// 尝试加载头文件
//////////////////////////////////////////////////////////////////////
BOOL CTclPcomClass::LoadHeadFile()
{
	if(g_pIPlatUI == NULL)
	{
		return FALSE;
	}

	CString strPlatPath = g_pIPlatUI->GetPlatPath();
	CString strVciType = g_pIPlatUI->GetVciType(m_strVciName);
	CString strVciClass = g_pIPlatUI->GetVciClass(m_strVciName);
	CString strVciInterfaceFile = g_pIPlatUI->GetVciInterfaceFile(m_strVciName);

	// 查找平台的VCI\Include目录,看能否找到头文件
	CString strPath;
	if(m_strVciName == VCI_NAME_PLATUI)
	{
		strVciType = "plat";
		strPath = strPlatPath + "Plugins\\Include\\IPlatUI.h";
		strVciClass = "IPlatUI";
	}else
	if(strVciType == "owm")
	{
		strPath = strPlatPath + "Plugins\\Include\\vciowm.h";
	}else
	if(strVciType == "interp")
	{
		strPath = strPlatPath + "Plugins\\Include\\IInterp.h";
	}else
	if(strVciType == "project")
	{
		strPath = strPlatPath + "Plugins\\Include\\IProject.h";
	}else
	{
		strPath = strVciInterfaceFile;
	}
/*
	if(g_strDevicePath == "")
	{
		strPath = GetPlatRootPath();
		strPath += "VCI\\Include\\";
		strPath += m_strVciName;
		strPath += ".h";
	}else
	{
		strPath = g_strDevicePath;
		strPath.Replace("/", "\\");
		if(strPath.Right(1) != '\\')
		{
			strPath += "\\";
			strPath += "Include\\";
			strPath += m_strVciName;
			strPath += ".h";
		}
	}
*/
	if(GetFileAttributes(strPath) == 0xFFFFFFFF)
	{
		// 未找到头文件,返回失败
		TRACE("TclPcomClass: not find device head file %s!\n", strPath);
		return FALSE;
	}

	// 头文件解析方法:用正则表达式匹配所有虚函数
	strPath.Replace("\\", "/");
	char loadHeadFileScript[500];
	sprintf(loadHeadFileScript, "\n\
		set fi [open \"%s\" r];\n\
		set buf [read $fi];\n\
		close $fi;\n\
		regexp {interface\\s+%s\\s+.*?\\{(.*?)\\}\\s*;} $buf total all;\n\
		regsub -all -line {//.*$} $all {} all1;\n\
		regsub -all -line {/\\*.*\\*/} $all1 {} all;\n\
		set all;\n\
		", strPath, strVciClass);

	CTclInterp interp;
	if(!interp.RunScriptCommand(loadHeadFileScript))
	{
		return FALSE;
	}

	// 获取所有虚函数的串(只是将)
	CString strFuncs = interp.GetResult();
	int nPos = 0;
	int nPosStart = 0;
	int nPosEnd = 0;
	int nPosParam = 0;
	int nPosRet = 0;
	while(TRUE)
	{
		// 函数头
		nPos = strFuncs.Find("virtual ", nPosStart);
		if(nPos == -1)
			break;

		// 函数尾
		nPosEnd = strFuncs.Find(") = 0;", nPos);
		if(nPosEnd == -1)
			break;

		// 参数起始位置
		nPosParam = strFuncs.Find("(", nPos);
		if(nPosParam == -1)
			break;

		// 返回值类型
		nPosRet = strFuncs.Find("__stdcall", nPos);
		if(nPosRet == -1)
			break;
		CString strRetType = strFuncs.Mid(nPos+8, nPosRet-nPos-8);
		CString strParamName = "";	// 参数名
		int nType = -1;				// 参数类型
		int nMode = PMODE_DIRECT;	// 参数传递方式
		CString strDefault = "";	// 参数缺省值

		CTclPcomFunc PcomFunc;
		PcomFunc.ParseParamType(strRetType, strParamName, nType, nMode, strDefault);
		PcomFunc.m_Ret.m_nType	= nType;
		PcomFunc.m_Ret.m_nMode	= nMode;

		// 解析函数原型
		CString strFuncDef = strFuncs.Mid(nPos+8, nPosEnd-nPos-7);
		strFuncDef.Delete(nPosRet-nPos-8, 10);
		strFuncDef.TrimLeft();
		strFuncDef += ";";
		PcomFunc.SetFuncDef(strFuncDef);

		// 解析函数名
		CString strFuncName = strFuncs.Mid(nPosRet+9, nPosParam-nPosRet-9);
		strFuncName.TrimLeft();
		strFuncName.TrimRight();
		PcomFunc.SetFuncName(strFuncName);
		//TRACE(">>>%s\n", strFuncName);

		// 解析各个参数
		CString strParams = strFuncs.Mid(nPosParam+1, nPosEnd-nPosParam-1);
		strParams.TrimLeft();
		strParams.TrimRight();
		int nPosP = -1;
		while((nPosP = strParams.Find(",")) != -1)
		{	// 前面若干个参数
			CString strParam = strParams.Left(nPosP);
			strParams.Delete(0, nPosP+1);
			PcomFunc.ParseParamType(strParam, strParamName, nType, nMode, strDefault);
			PcomFunc.AddParam(strParamName, nType, nMode, strDefault);
			//TRACE("...%s(type=%d,mode=%d,default=%s)\n", strParamName, nType, nMode, strDefault);
			// 有未知类型参数则不可执行
			if(PTYPE_UNKNOWN == nType)
				PcomFunc.m_bEnableExec = FALSE;
		}
		strParams.TrimLeft();
		if(strParams != "")	// 最后一个参数
		{
			PcomFunc.ParseParamType(strParams, strParamName, nType, nMode, strDefault);
			PcomFunc.AddParam(strParamName, nType, nMode, strDefault);
			//TRACE("...%s(type=%d,mode=%d,default=%s)\n", strParamName, nType, nMode, strDefault);
			// 有未知类型参数则不可执行
			if(PTYPE_UNKNOWN == nType)
				PcomFunc.m_bEnableExec = FALSE;
		}

		// 参数太多也不可执行
		if(PcomFunc.m_arParams.GetSize() > TCLPCOM_MAXPARAM)
		{
			PcomFunc.m_bEnableExec = FALSE;
		}

		// 计算除去缺省参数后的最少参数个数
		PcomFunc.m_nLeastParams = PcomFunc.m_arParams.GetSize();
		for(int i=0; i<PcomFunc.m_arParams.GetSize(); i++)
		{
			if(PcomFunc.m_arParams[i].m_strDefault != "")
			{
				PcomFunc.m_nLeastParams = i+1;
				break;
			}
		}

		m_arFuncs.Add(PcomFunc);

		// 下一个函数
		nPosStart = nPosEnd;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// 查找函数
//////////////////////////////////////////////////////////////////////
CTclPcomFunc* CTclPcomClass::FindFunction(CString strFunc)
{
	// 处理重名的函数,通过在函数名后加"(No)"来表示,
	// 例如Send(2)表示第2个名字是Send的函数
	int nFuncNo = 1;	// 对于函数名相同的函数的序号
	int nPos = strFunc.Find("(");
	if(nPos != -1)
	{
		int nPos1 = strFunc.Find(")");
		if(nPos1 > nPos)
		{
			CString strFuncNo = strFunc.Mid(nPos+1, nPos1-nPos-1);
			nFuncNo = atoi(strFuncNo);
			if(nFuncNo <= 0)
				nFuncNo = 1;
			strFunc = strFunc.Left(nPos);
		}
	}

	for(int i=0; i<m_arFuncs.GetSize(); i++)
	{
		if(strFunc == m_arFuncs[i].GetFuncName())
		{
			nFuncNo--;
			if(nFuncNo <= 0)
			{
				return &(m_arFuncs[i]);
			}
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// 获取函数的索引
//////////////////////////////////////////////////////////////////////
int CTclPcomClass::GetFunctionIndex(CString strFunc)
{
	// 处理重名的函数,通过在函数名后加"(No)"来表示,
	// 例如Send(2)表示第2个名字是Send的函数
	int nFuncNo = 1;	// 对于函数名相同的函数的序号
	int nPos = strFunc.Find("(");
	if(nPos != -1)
	{
		int nPos1 = strFunc.Find(")");
		if(nPos1 > nPos)
		{
			CString strFuncNo = strFunc.Mid(nPos+1, nPos1-nPos-1);
			nFuncNo = atoi(strFuncNo);
			if(nFuncNo <= 0)
				nFuncNo = 1;
			strFunc = strFunc.Left(nPos);
		}
	}

	for(int i=0; i<m_arFuncs.GetSize(); i++)
	{
		if(strFunc == m_arFuncs[i].GetFuncName())
		{
			nFuncNo--;
			if(nFuncNo <= 0)
			{
				return i;
			}
		}
	}
	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// 根据组件类名查找类对象
/////////////////////////////////////////////////////////////////////////////
CTclPcomClass* GetTclPcomClass(CString strClassName)
{
	int count = g_TclPcomClassList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_TclPcomClassList.FindIndex(i);
		CTclPcomClass* pTclPcomClass = (CTclPcomClass*)g_TclPcomClassList.GetAt(pos);
		if(strClassName == pTclPcomClass->GetVciName()) 
		{
			return pTclPcomClass;
			break;
		}
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static CPtrList	g_TclPcomObjList;	// PCOM对象列表

CTclPcomObj::CTclPcomObj()
{
	m_pClass	= NULL;
	m_pPcomObj	= NULL;
	m_Interp	= NULL;
	m_strVciName= "";
	m_strShareName = "";
	m_strErrorInfo = "";
	m_nRefCount	= 1;
	g_TclPcomObjList.AddTail(this);
}

CTclPcomObj::~CTclPcomObj()
{
	// 关闭组件
	if(g_pIPlatUI == NULL)
	{
		CTclInterp* pTclInterp = GetTclInterp((LPVOID)m_Interp);
		if(pTclInterp)
		{
			g_pIPlatUI = pTclInterp->pIPlatUI;
		}
	}
	if(g_pIPlatUI && (m_strShareName == ""))	// sharename非空的在CTclPcomObj::CloseVciObject函数已经调用平台的释放函数
	{
		g_pIPlatUI->ReleaseVciObject(m_pPcomObj, FALSE);
	}

	m_pPcomObj = NULL;

	// 释放PcomClass对象
	if(m_pClass != NULL)
	{
		// 判断引用计数
		if(m_pClass->ReleaseRef() == 0)
		{
			// 删除列表中的内容和对象
			int count = g_TclPcomClassList.GetCount();
			for(int i = 0; i < count; i ++)
			{
				POSITION pos = g_TclPcomClassList.FindIndex(i);
				CTclPcomClass* pTclPcomClass = (CTclPcomClass*)g_TclPcomClassList.GetAt(pos);
				if(m_pClass == pTclPcomClass) 
				{
					g_TclPcomClassList.RemoveAt(pos);
					delete m_pClass;
					m_pClass = NULL;
					break;
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 判断PCOM对象是否已经打开
// 参数1: 组件名
// 参数2: 需要的版本
// 参数3: 共享名
/////////////////////////////////////////////////////////////////////////////
BOOL CTclPcomObj::IsOpen(CString strVciName, CString strVer, CString strShareName)
{
	if((strVciName == m_strVciName) && (strShareName != "") && (strShareName == m_strShareName))
	{
		return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 打开一个PCOM组件
// 参数1: 组件名
// 参数2: 需要的版本
// 参数3: 共享名
// 参数4: 指定PcomObj对象指针,默认为空,如果指定就使用指定的指针,不用创建新的对象
/////////////////////////////////////////////////////////////////////////////
BOOL CTclPcomObj::OpenVciObject(CString strVciName, CString strVer, CString strShareName, LPVOID pPcomObj)
{
	if(g_pIPlatUI == NULL)
	{
		CTclInterp* pTclInterp = GetTclInterp((LPVOID)m_Interp);
		IPlatUI* pIPlatUI = NULL;
		if(pTclInterp)
		{
			pIPlatUI = pTclInterp->pIPlatUI;
			g_pIPlatUI = pIPlatUI;
		}
	}
	if(g_pIPlatUI == NULL)
	{
		m_strErrorInfo = _T("Get platform interface failed.");
		return FALSE;
	}

	// 1.首先查找是否存在对应的PcomClass对象
	int count = g_TclPcomClassList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_TclPcomClassList.FindIndex(i);
		CTclPcomClass* pTclPcomClass = (CTclPcomClass*)g_TclPcomClassList.GetAt(pos);
		if(strVciName == pTclPcomClass->GetVciName()) 
		{
			m_pClass = pTclPcomClass;
			pTclPcomClass->AddRef();
			break;
		}
	}

	// 2.未找到则创建一个新的
	if(m_pClass == NULL)
	{
		m_pClass = new CTclPcomClass;
		g_TclPcomClassList.AddTail(m_pClass);
		m_pClass->AddRef();
		m_pClass->SetVciName(strVciName);
		// 让PcomClass对象尝试加载头文件
		m_pClass->LoadHeadFile();
	}

	// 3.调用平台接口打开组件
	m_pPcomObj = NULL;

	if(m_pPcomObj == NULL)
	{
		// 调用平台接口打开组件
		if(strVciName == VCI_NAME_PLATUI)
		{
			m_pPcomObj = g_pIPlatUI;
			m_strVciName = VCI_NAME_PLATUI;
		}else
		{
			if(pPcomObj != NULL)
			{
				// 如果传递的参数中有PcomObj指针,就直接使用指定的指针
				m_pPcomObj = pPcomObj;
			}else
			{
				// 否则创建对象
				m_pPcomObj = g_pIPlatUI->CreateVciObject(strVciName, strShareName);
			}
			m_strVciName = strVciName;
			m_strShareName = strShareName;
		}
		if(m_pPcomObj == NULL)
		{
			m_strErrorInfo = _T("Not find device manager interface or not find this device.");
			return FALSE;
		}
	}else
	if(strShareName != "")	// 如果sharename非空,则增加VCI对象的引用计数
	{
		if(strVciName != VCI_NAME_PLATUI)
		{
			m_pPcomObj = g_pIPlatUI->CreateVciObject(strVciName, strShareName);
			m_nRefCount++;	// 增加引用计数
		}
	}

	if(m_pPcomObj == NULL)
	{
		m_strErrorInfo = _T("Open device failed.");
	}

	return (m_pPcomObj != NULL);
}

/////////////////////////////////////////////////////////////////////////////
// 关闭PCOM组件对象
/////////////////////////////////////////////////////////////////////////////
void CTclPcomObj::CloseVciObject(CTclPcomObj* pPcomObj)
{
	// 如果sharename非空,则判断引用计数是否为0
	if(pPcomObj->m_strShareName != "")
	{
		if(g_pIPlatUI == NULL)
		{
			CTclInterp* pTclInterp = GetTclInterp((LPVOID)(pPcomObj->m_Interp));
			if(pTclInterp)
			{
				g_pIPlatUI = pTclInterp->pIPlatUI;
			}
		}
		if(g_pIPlatUI)
		{
			pPcomObj->m_nRefCount--;
			if(g_pIPlatUI->ReleaseVciObject(pPcomObj->m_pPcomObj, FALSE) && (pPcomObj->m_nRefCount > 0))
			{
				// 返回TRUE表示此VCI对象引用计数还不为0,如果同时TclPcomObj的引用计数也大于0,则不用删除此对象
				return;
			}
		}
	}

	// 卸载回调函数
	int count = g_TclPcomCallBackList.GetCount();
	int i;
	for(i = 0; i < count; i ++)
	{
		POSITION pos = g_TclPcomCallBackList.FindIndex(i);
		CTclPcomCallBack* pTclPcomCallBack = (CTclPcomCallBack*)g_TclPcomCallBackList.GetAt(pos);
		if(pPcomObj==pTclPcomCallBack->m_pPcomObject) 
		{
			CTclPcomCallBack::UnInstallCallBack(pPcomObj, pTclPcomCallBack->m_nEventID);
			break;
		}
	}

	// 删除解释器对象列表中的项
	count = g_TclPcomObjList.GetCount();
	for(i = 0; i < count; i ++)
	{
		POSITION pos = g_TclPcomObjList.FindIndex(i);
		CTclPcomObj* pTclPcomObj = (CTclPcomObj*)g_TclPcomObjList.GetAt(pos);
		if(pPcomObj == pTclPcomObj) 
		{
			g_TclPcomObjList.RemoveAt(pos);
			delete pTclPcomObj;
			break;
		}
	}

	// 删除关联的回调对象
	count = g_TclPcomCallBackList.GetCount();
	for(i = 0; i < count; i ++)
	{
		POSITION pos = g_TclPcomCallBackList.FindIndex(i);
		CTclPcomCallBack* pTclPcomCallBack = (CTclPcomCallBack*)g_TclPcomCallBackList.GetAt(pos);
		if(pPcomObj == pTclPcomCallBack->m_pPcomObject) 
		{
			delete pTclPcomCallBack;
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 释放VCI对象
/////////////////////////////////////////////////////////////////////////////
void CTclPcomObj::ReleaseVciObject(BOOL bForce)
{
	if(g_pIPlatUI == NULL)
	{
		CTclInterp* pTclInterp = GetTclInterp((LPVOID)(m_Interp));
		if(pTclInterp)
		{
			g_pIPlatUI = pTclInterp->pIPlatUI;
		}
	}
	if(g_pIPlatUI)
	{
		m_nRefCount--;
		if(bForce)
		{
			m_nRefCount = 0;
		}
		g_pIPlatUI->ReleaseVciObject(m_pPcomObj, bForce);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 关闭所有PCOM组件对象
/////////////////////////////////////////////////////////////////////////////
void CloseAllVciObject()
{
	// 删除解释器对象列表中的项
	int count = g_TclPcomObjList.GetCount();
	int i;
	for(i = count-1; i >= 0; i --)
	{
		POSITION pos = g_TclPcomObjList.FindIndex(i);
		CTclPcomObj* pTclPcomObj = (CTclPcomObj*)g_TclPcomObjList.GetAt(pos);
		if(pTclPcomObj != NULL)
		{
			pTclPcomObj->ReleaseVciObject(TRUE);
			g_TclPcomObjList.RemoveAt(pos);
			delete pTclPcomObj;
		}
	}

	// 删除所有回调对象
	count = g_TclPcomCallBackList.GetCount();
	for(i = count-1; i >= 0; i --)
	{
		POSITION pos = g_TclPcomCallBackList.FindIndex(i);
		CTclPcomCallBack* pTclPcomCallBack = (CTclPcomCallBack*)g_TclPcomCallBackList.GetAt(pos);
		if(pTclPcomCallBack != NULL) 
		{
			delete pTclPcomCallBack;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 检查组件对象指针是否合法
/////////////////////////////////////////////////////////////////////////////
BOOL IsValidTclPcomObj(CTclPcomObj* pObj)
{
	int count = g_TclPcomObjList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_TclPcomObjList.FindIndex(i);
		CTclPcomObj* pTclPcomObj = (CTclPcomObj*)g_TclPcomObjList.GetAt(pos);
		if(pObj == pTclPcomObj)
		{
			return TRUE;
		}
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 创建平台接口的TclVci对象
/////////////////////////////////////////////////////////////////////////////
BOOL CreatePlatUITclPcomObj()
{
	if(g_pTclPcomObjPlatUI)
	{
		return TRUE;
	}

	g_pTclPcomObjPlatUI = new CTclPcomObj();
	return g_pTclPcomObjPlatUI->OpenVciObject(VCI_NAME_PLATUI, "2.0.0.2", "");
}

/////////////////////////////////////////////////////////////////////////////
// 定义回调函数,用于通过脚本的callback
/////////////////////////////////////////////////////////////////////////////
// 执行TCL脚本的调用
int TclPcomCallBack(CUIntArray& arParams, LPVOID pCallBackFunc)
{
	int count = g_TclPcomCallBackList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_TclPcomCallBackList.FindIndex(i);
		CTclPcomCallBack* pTclPcomCallBack = (CTclPcomCallBack*)g_TclPcomCallBackList.GetAt(pos);
		if(pCallBackFunc==pTclPcomCallBack->m_pCallBackFunc)
		{
			CTclInterp* pTclInterp = GetTclInterp(pTclPcomCallBack->m_pInterp);
			if(pTclInterp == NULL)
				return 0;
			CString strScript = pTclPcomCallBack->m_strCallBackScript;
			for(int j=0; j<arParams.GetSize(); j++)
			{
				CString strParam;
				strParam.Format(" %d", arParams[j]);
				strScript += strParam;
			}
			//TRACE("%s\n", strScript);
			BOOL bRes = pTclInterp->RunScriptCommand(strScript.GetBuffer(strScript.GetLength()));
			strScript.ReleaseBuffer();
			if(bRes)
			{
				return atoi(pTclInterp->GetResult());
			}else
			{
				TRACE("TclCallBack error:%s\n", pTclInterp->GetResult());
				return 0;
			}
		}
	}
	return 0;
}

// 不带参数的回调
int PcomCallBack0()
{
	CUIntArray arParams;
	return TclPcomCallBack(arParams, PcomCallBack0);
}

// 带1个参数的回调
int PcomCallBack1(int nParam1)
{
	CUIntArray arParams;
	arParams.Add(nParam1);
	return TclPcomCallBack(arParams, PcomCallBack1);
}

// 带2个参数的回调
int PcomCallBack2(int nParam1, int nParam2)
{
	CUIntArray arParams;
	arParams.Add(nParam1);
	arParams.Add(nParam2);
	return TclPcomCallBack(arParams, PcomCallBack2);
}

// 带3个参数的回调
int PcomCallBack3(int nParam1, int nParam2, int nParam3)
{
	CUIntArray arParams;
	arParams.Add(nParam1);
	arParams.Add(nParam2);
	arParams.Add(nParam3);
	return TclPcomCallBack(arParams, PcomCallBack3);
}

// 带4个参数的回调
int PcomCallBack4(int nParam1, int nParam2, int nParam3, int nParam4)
{
	CUIntArray arParams;
	arParams.Add(nParam1);
	arParams.Add(nParam2);
	arParams.Add(nParam3);
	arParams.Add(nParam4);
	return TclPcomCallBack(arParams, PcomCallBack4);
}

// 带5个参数的回调
int PcomCallBack5(int nParam1, int nParam2, int nParam3, int nParam4, int nParam5)
{
	CUIntArray arParams;
	arParams.Add(nParam1);
	arParams.Add(nParam2);
	arParams.Add(nParam3);
	arParams.Add(nParam4);
	arParams.Add(nParam5);
	return TclPcomCallBack(arParams, PcomCallBack5);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CTclPcomCallBack::CTclPcomCallBack()
{
	m_pInterp			= NULL;
	m_strCallBackScript	= "";
	m_nParamCount		= -1;
	m_pCallBackFunc		= NULL;
	m_pPcomObject		= NULL;
	m_nEventID			= 0;
	g_TclPcomCallBackList.AddTail(this);
}

CTclPcomCallBack::~CTclPcomCallBack()
{
	// 删除列表中的内容和对象
	int count = g_TclPcomCallBackList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_TclPcomCallBackList.FindIndex(i);
		CTclPcomCallBack* pTclPcomCallBack = (CTclPcomCallBack*)g_TclPcomCallBackList.GetAt(pos);
		if(this == pTclPcomCallBack) 
		{
			g_TclPcomCallBackList.RemoveAt(pos);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// 安装回调函数
//////////////////////////////////////////////////////////////////////
CTclPcomCallBack* CTclPcomCallBack::InstallCallBack(LPVOID pInterp, CString strScript, int nParamCount, CTclPcomObj* pObj, int nEventID)
{
	if(nParamCount<0 || nParamCount>5)
	{
		return NULL;
	}

	int count = g_TclPcomCallBackList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_TclPcomCallBackList.FindIndex(i);
		CTclPcomCallBack* pTclPcomCallBack = (CTclPcomCallBack*)g_TclPcomCallBackList.GetAt(pos);
		if(nParamCount == pTclPcomCallBack->m_nParamCount) 
		{
			if(strScript == pTclPcomCallBack->m_strCallBackScript)
			{
				// 如果已经安装了一个同名的,则直接返回安装过的回调指针
				return pTclPcomCallBack;
			}

			// 每种参数个数只能有一个回调函数
			return NULL;
		}
	}

	// 创建新的回调对象
	CTclPcomCallBack* pCallBack = new CTclPcomCallBack();
	pCallBack->m_pInterp		= pInterp;
	pCallBack->m_strCallBackScript= strScript;
	pCallBack->m_nParamCount	= nParamCount;
	pCallBack->m_pPcomObject	= pObj;
	pCallBack->m_nEventID		= nEventID;
	switch(nParamCount)
	{
	case 0: pCallBack->m_pCallBackFunc = PcomCallBack0; break;
	case 1: pCallBack->m_pCallBackFunc = PcomCallBack1; break;
	case 2: pCallBack->m_pCallBackFunc = PcomCallBack2; break;
	case 3: pCallBack->m_pCallBackFunc = PcomCallBack3; break;
	case 4: pCallBack->m_pCallBackFunc = PcomCallBack4; break;
	case 5: pCallBack->m_pCallBackFunc = PcomCallBack5; break;
	}
	
	return pCallBack;
}

//////////////////////////////////////////////////////////////////////
// 卸载回调函数
//////////////////////////////////////////////////////////////////////
BOOL CTclPcomCallBack::UnInstallCallBack(CTclPcomObj* pObj, int nEventID)
{
	int count = g_TclPcomCallBackList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_TclPcomCallBackList.FindIndex(i);
		CTclPcomCallBack* pTclPcomCallBack = (CTclPcomCallBack*)g_TclPcomCallBackList.GetAt(pos);
		if((pObj==pTclPcomCallBack->m_pPcomObject) && (nEventID==pTclPcomCallBack->m_nEventID)) 
		{
			delete pTclPcomCallBack;
			return TRUE;
		}
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// 获取回调函数对象
//////////////////////////////////////////////////////////////////////
CTclPcomCallBack* CTclPcomCallBack::GetCallBack(CTclPcomObj* pObj, int nEventID)
{
	int count = g_TclPcomCallBackList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_TclPcomCallBackList.FindIndex(i);
		CTclPcomCallBack* pTclPcomCallBack = (CTclPcomCallBack*)g_TclPcomCallBackList.GetAt(pos);
		if((pObj==pTclPcomCallBack->m_pPcomObject) && (nEventID==pTclPcomCallBack->m_nEventID)) 
		{
			return pTclPcomCallBack;
		}
	}
	return NULL;
}


/////////////////////////////////////////////////////////////////////////////
// 用于组件调用的TCL扩展命令

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:打开一个VCI组件
// 用  法: vci::open name ?sharename? ?ver?
// 返回值: 对象句柄
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_DeviceOpen(ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[])
{
	VERIFY_TCLCMD_RIGHT(USER_OPERATOR);

	if(objc < 2)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "name ?sharename? ?ver?");
		return TCL_ERROR;
	}

	// 获取参数
	CString strName = ConvertUTFParam(Tcl_GetString(objv[1]));
	CString strVer = "";
	CString strShareName = "";
	if(objc >= 3)
	{
		strShareName = ConvertUTFParam(Tcl_GetString(objv[2]));
	}
	if(objc >= 4)
	{
		strVer = ConvertUTFParam(Tcl_GetString(objv[3]));
	}

	// 查找是否有id,ver,sharename相同的对象,如果有就不用再打开,直接返回对象指针
	if(strShareName != "")
	{
		int count = g_TclPcomObjList.GetCount();
		for(int i = 0; i < count; i ++)
		{
			POSITION pos = g_TclPcomObjList.FindIndex(i);
			CTclPcomObj* pTclPcomObj = (CTclPcomObj*)g_TclPcomObjList.GetAt(pos);
			if((pTclPcomObj != NULL) && pTclPcomObj->IsOpen(strName, strVer, strShareName))
			{
				pTclPcomObj->OpenVciObject(strName, strVer, strShareName);
				sprintf(interp->result, "%d", pTclPcomObj);
				return TCL_OK;
			}
		}
	}

	CTclPcomObj* pTclPcomObj = new CTclPcomObj;
	pTclPcomObj->SetInterp(interp);
	if(!pTclPcomObj->OpenVciObject(strName, strVer, strShareName))
	{
		sprintf(interp->result, "%s", pTclPcomObj->GetErrorInfo());
		CTclPcomObj::CloseVciObject(pTclPcomObj);
		ConvertResultToUTF(interp);
		return TCL_ERROR;
	}else
	{
		sprintf(interp->result, "%d", pTclPcomObj);
		return TCL_OK;
	}	
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:关闭一个VCI组件
// 用  法: vci::close handle
// 返回值: 是否成功
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_DeviceClose(ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[])
{
	VERIFY_TCLCMD_RIGHT(USER_OPERATOR);

	if(objc < 2)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "handle");
		return TCL_ERROR;
	}

	// 获取参数
	CString strpDev = ConvertUTFParam(Tcl_GetString(objv[1]));
	CTclPcomObj* pTclPcomObj = (CTclPcomObj*)(atoi(strpDev));

	// 检查指针的合法性
	if(!IsValidTclPcomObj(pTclPcomObj))
	{
		Tcl_AppendResult(interp, "Invalid device handle: ", strpDev, (char *) NULL);
		return TCL_ERROR;
	}

	CTclPcomObj::CloseVciObject(pTclPcomObj);

	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:VCI组件的函数操作
// 用  法: vci::info -list | -add | -remove
// -listclass			: 列当前所有VCI类
// -listobject ?vciid?	: 列打开的所有组件对象或某一种对象
// -getvcihandle tclvciobj	: 获取指定TclVci对象的VCI句柄
// -gettclvciobj vciid vcihandle	: 获取指定VCI对象的TclVci封装对象句柄
// -listinstalled ?vciid? ?verlist|version? : 列安装的组件信息
// -listdef vciid		: 列函数声明
// -listfunc vciid		: 列函数名
// -listparam vciid func ?param? : 列函数参数详细信息
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_DeviceInfo(ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[])
{
	VERIFY_TCLCMD_RIGHT(USER_OPERATOR);

	if(objc < 2)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "-opr(listdef|listfunc|listparam|listclass|listobject|listinstalled|getvcihandle|gettclvciobj) ...");
		return TCL_ERROR;
	}

	CTclInterp* pTclInterp = GetTclInterp((LPVOID)interp);
	if(pTclInterp)
	{
		g_pIPlatUI = pTclInterp->pIPlatUI;
	}
	if(g_pIPlatUI == NULL)
	{
		Tcl_AppendResult(interp, "Get platform interface fail.", (char *) NULL);
		return TCL_ERROR;
	}

	// 获取参数
	CString strOpr = ConvertUTFParam(Tcl_GetString(objv[1]));
	CString strVciName = "";
	CTclPcomClass* pTclPcomClass = NULL;
	if(objc >= 3)
	{
		strVciName = ConvertUTFParam(Tcl_GetString(objv[2]));
		pTclPcomClass = GetTclPcomClass(strVciName);
	}

	if(strOpr == "-listclass")	// 列当前所有VCI类
	{
		int count = g_TclPcomClassList.GetCount();
		for(int i = 0; i < count; i ++)
		{
			POSITION pos = g_TclPcomClassList.FindIndex(i);
			CTclPcomClass* pTclPcomClass = (CTclPcomClass*)g_TclPcomClassList.GetAt(pos);
			Tcl_AppendElement(interp, pTclPcomClass->GetVciName());
		}

		ConvertResultToUTF(interp);
		return TCL_OK;
	}else
	if(strOpr == "-listobject")	// 列当前打开的VCI对象
	{
		if(strVciName == "")	// 列所有VCI对象句柄
		{
			int count = g_TclPcomObjList.GetCount();
			for(int i = 0; i < count; i ++)
			{
				POSITION pos = g_TclPcomObjList.FindIndex(i);
				CTclPcomObj* pTclPcomObj = (CTclPcomObj*)g_TclPcomObjList.GetAt(pos);
				if(pTclPcomObj != NULL)
				{
					CString strHandle;
					strHandle.Format("%d", pTclPcomObj);
					Tcl_AppendElement(interp, strHandle);
				}
			}
		}else	// 列指定组件的所有对象实例句柄
		{
			if(!pTclPcomClass)
			{
				Tcl_AppendResult(interp, "Vci class ", strVciName, " not find.", (char *) NULL);
				return TCL_ERROR;
			}

			int count = g_TclPcomObjList.GetCount();
			for(int i = 0; i < count; i ++)
			{
				POSITION pos = g_TclPcomObjList.FindIndex(i);
				CTclPcomObj* pTclPcomObj = (CTclPcomObj*)g_TclPcomObjList.GetAt(pos);
				if(pTclPcomObj->GetTclPcomClass() == pTclPcomClass)
				{
					CString strHandle;
					strHandle.Format("%d", pTclPcomObj);
					Tcl_AppendElement(interp, strHandle);
				}
			}
		}

		ConvertResultToUTF(interp);
		return TCL_OK;
	}else
	if(strOpr == "-getvcihandle")	// 获取指定TclVci对象的VCI句柄
	{
		if(objc < 3)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "-getvcihandle tclvciobj");
			return TCL_ERROR;
		}

		CTclPcomObj* pTclPcomObj = (CTclPcomObj*)(atoi(strVciName));

		int count = g_TclPcomObjList.GetCount();
		for(int i = 0; i < count; i ++)
		{
			POSITION pos = g_TclPcomObjList.FindIndex(i);
			CTclPcomObj* pTclPcomObjTmp = (CTclPcomObj*)g_TclPcomObjList.GetAt(pos);
			if(pTclPcomObjTmp == pTclPcomObj)
			{
				CString strHandle;
				strHandle.Format("%d", pTclPcomObj->GetPcomObj());
				Tcl_AppendElement(interp, strHandle);
				return TCL_OK;
			}
		}

		ConvertResultToUTF(interp);
		return TCL_OK;
	}else
	if(strOpr == "-gettclvciobj")	// 获取指定VCI对象的TclVci封装对象句柄,如果封装不存在就创建
	{
		if(objc < 3)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "-gettclvciobj vciid vcihandle");
			return TCL_ERROR;
		}

		void* pPcomObj = (void*)(atoi(ConvertUTFParam(Tcl_GetString(objv[3]))));

		// 如果找到,则返回指针
		if(pTclPcomClass)
		{
			int count = g_TclPcomObjList.GetCount();
			for(int i = 0; i < count; i ++)
			{
				POSITION pos = g_TclPcomObjList.FindIndex(i);
				CTclPcomObj* pTclPcomObj = (CTclPcomObj*)g_TclPcomObjList.GetAt(pos);
				if( (pTclPcomObj->GetTclPcomClass() == pTclPcomClass) &&
					(pTclPcomObj->GetPcomObj() == pPcomObj) )
				{
					CString strHandle;
					strHandle.Format("%d", pTclPcomObj);
					Tcl_AppendElement(interp, strHandle);
					return TCL_OK;
				}
			}
		}

		// 未找到则创建TclPcom对象
		CString strInstanceName = g_pIPlatUI->GetInstanceNameByObject(strVciName, pPcomObj);
		CTclPcomObj* pTclPcomObj = new CTclPcomObj;
		pTclPcomObj->SetInterp(interp);
		if(!pTclPcomObj->OpenVciObject(strVciName, "", strInstanceName, pPcomObj))
		{
			sprintf(interp->result, "%s", pTclPcomObj->GetErrorInfo());
			CTclPcomObj::CloseVciObject(pTclPcomObj);
			ConvertResultToUTF(interp);
			return TCL_ERROR;
		}

		CString strHandle;
		strHandle.Format("%d", pTclPcomObj);
		Tcl_AppendElement(interp, strHandle);
		ConvertResultToUTF(interp);
		return TCL_OK;
	}else
	if(strOpr == "-listinstalled")	// 列当前所有已经安装的组件接口
	{
		CStringArray asVciId;
		g_pIPlatUI->GetInstalledVci(asVciId);

		if(strVciName == "")	// 没有接口名参数,表示列所有组件接口
		{
			for(int i = 0; i < asVciId.GetSize(); i ++)
			{
				Tcl_AppendElement(interp, asVciId[i]);
			}
		}else
		{
			// 查找此组件是否安装
			int i;
			for(i = 0; i < asVciId.GetSize(); i ++)
			{
				if(strVciName == asVciId[i])
					break;
			}
			if(i >= asVciId.GetSize())
			{
				Tcl_AppendResult(interp, "Not find vci component ", strVciName, ".", (char *) NULL);
				ConvertResultToUTF(interp);
				return TCL_ERROR;
			}

			Tcl_AppendElement(interp, g_pIPlatUI->GetVciVersion(strVciName));
			Tcl_AppendElement(interp, g_pIPlatUI->GetVciPath(strVciName));
			Tcl_AppendElement(interp, g_pIPlatUI->GetVciProvider(strVciName));
			Tcl_AppendElement(interp, g_pIPlatUI->GetVciType(strVciName));
			Tcl_AppendElement(interp, g_pIPlatUI->GetVciInterfaceFile(strVciName));
			Tcl_AppendElement(interp, g_pIPlatUI->GetVciAboutPage(strVciName));
			ConvertResultToUTF(interp);
			return TCL_OK;
		}

		ConvertResultToUTF(interp);
		return TCL_OK;
	}

	if(objc < 3)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "-opr(listdef|listfunc|listparam) vciid");
		return TCL_ERROR;
	}
	if(!pTclPcomClass)
	{
		Tcl_AppendResult(interp, "Vci class ", strVciName, " not find.", (char *) NULL);
		ConvertResultToUTF(interp);
		return TCL_ERROR;
	}

	if(strOpr == "-listdef")	// 列函数声明
	{
		for(int i=0; i<pTclPcomClass->m_arFuncs.GetSize(); i++)
		{
			CString strDef = pTclPcomClass->m_arFuncs[i].GetFuncDef();
			Tcl_AppendElement(interp, strDef);
		}
	}else	
	if(strOpr == "-listfunc")	// 列函数名
	{
		for(int i=0; i<pTclPcomClass->m_arFuncs.GetSize(); i++)
		{
			CString strName = pTclPcomClass->m_arFuncs[i].GetFuncName();
			Tcl_AppendElement(interp, strName);
		}
	}else
	if(strOpr == "-listparam")	// 列函数参数
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "-listparam vciid func ?param?");
			return TCL_ERROR;
		}

		CString strFunc = ConvertUTFParam(Tcl_GetString(objv[3]));
		// 查找函数是否存在
		// 对于名字相同的函数,可以通过在函数名后加"(No)"来表示
		CTclPcomFunc* pTclPcomFunc = pTclPcomClass->FindFunction(strFunc);
		if(!pTclPcomFunc)
		{
			Tcl_AppendResult(interp, "Not find function ", strFunc, " in vci ", pTclPcomClass->GetVciName(), ".", (char *) NULL);
			ConvertResultToUTF(interp);
			return TCL_ERROR;
		}

		if(objc == 4)	// 列参数名
		{
			for(int i=0; i<pTclPcomFunc->m_arParams.GetSize(); i++)
			{
				CString strParamName = pTclPcomFunc->m_arParams[i].m_strParamName;
				Tcl_AppendElement(interp, strParamName);
			}
			ConvertResultToUTF(interp);
			return TCL_OK;
		}

		// 获取参数的详细情况
		CString strParam = ConvertUTFParam(Tcl_GetString(objv[4]));
		// 查找参数是否存在
		CTclPcomParam* pTclPcomParam = pTclPcomFunc->FindParam(strParam);
		if(!pTclPcomParam)
		{
			Tcl_AppendResult(interp, "Not find param ", strParam, " in func ", strFunc, ".", (char *) NULL);
			ConvertResultToUTF(interp);
			return TCL_ERROR;
		}

		CString strDetail;
		strDetail.Format("%d", pTclPcomParam->m_nType);		// 类型
		Tcl_AppendElement(interp, strDetail);
		strDetail.Format("%d", pTclPcomParam->m_nMode);		// 调用方式
		Tcl_AppendElement(interp, strDetail);
		Tcl_AppendElement(interp, pTclPcomParam->m_strDefault);	// 缺省值

	}else
	{
		Tcl_AppendResult(interp, "operater must be -listclass, -listobject, -listinstalled, -getvcihandle, -gettclvciobj, -listdef, -listfunc or -listparam.",  (char *) NULL);
		return TCL_ERROR;
	}

	ConvertResultToUTF(interp);
	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:调用VCI组件对象的函数
// 用  法: vci::exec handle func params...
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_DeviceExec(ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[])
{
	VERIFY_TCLCMD_RIGHT(USER_OPERATOR);

	if(objc < 3)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "handle func params...");
		return TCL_ERROR;
	}

	if(g_pIPlatUI == NULL)
	{
		CTclInterp* pTclInterp = GetTclInterp((LPVOID)interp);
		if(pTclInterp)
		{
			g_pIPlatUI = pTclInterp->pIPlatUI;
		}
	}

	// 获取参数
	CString strpDev = ConvertUTFParam(Tcl_GetString(objv[1]));
	CString strFunc = ConvertUTFParam(Tcl_GetString(objv[2]));
	CTclPcomObj* pTclPcomObj = NULL;
	if(strpDev == "plat")
	{
		// 表示调用平台接口
		CreatePlatUITclPcomObj();
		pTclPcomObj = g_pTclPcomObjPlatUI;
	}else
	{
		pTclPcomObj = (CTclPcomObj*)(atoi(strpDev));
	}
	// 检查指针的合法性
	if(!IsValidTclPcomObj(pTclPcomObj))
	{
		Tcl_AppendResult(interp, "Invalid vci object handle: ", strpDev, (char *) NULL);
		return TCL_ERROR;
	}

	// 检查PCOM类封装对象是否存在
	CTclPcomClass* pTclPcomClass = pTclPcomObj->GetTclPcomClass();
	if(!pTclPcomClass)
	{
		Tcl_AppendResult(interp, "Not find vci define object.", (char *) NULL);
		return TCL_ERROR;
	}

	// 查找函数是否存在
	// 对于名字相同的函数,可以通过在函数名后加"(No)"来表示
	CTclPcomFunc* pTclPcomFunc = pTclPcomClass->FindFunction(strFunc);
	if(!pTclPcomFunc)
	{
		Tcl_AppendResult(interp, "Not find function ", strFunc, " in vci ", pTclPcomClass->GetVciName(), ".", (char *) NULL);
		ConvertResultToUTF(interp);
		return TCL_ERROR;
	}

	// 此函数是否可执行
	if(!pTclPcomFunc->EnableExec())
	{	// 有未知类型参数,函数不可执行
		Tcl_AppendResult(interp, "Can't exec function ", strFunc, ", because have some unknown type paramter, or too many paramters.", (char *) NULL);
		ConvertResultToUTF(interp);
		return TCL_ERROR;
	}

	// 判断参数个数是否够(去掉缺省参数后的最少参数个数)
	int nLeastParams = pTclPcomFunc->m_nLeastParams;
	if(nLeastParams > (objc-3))
	{
		CString strArgNum;
		strArgNum.Format("%d", nLeastParams);
		Tcl_AppendResult(interp, "wrong number of args: need at least ", strArgNum, " parameters of function ", strFunc, ".", (char *) NULL);
		ConvertResultToUTF(interp);
		return TCL_ERROR;
	}

	// 开始执行函数

	// 1.在对应TclPcomClass对象中查找此函数名对应的索引
	// 注:在PCOM封装中,前面有三个隐藏的函数,因此要加3
	int nFuncIndex = pTclPcomClass->GetFunctionIndex(strFunc) + 3;

	// 2.处理参数
	void* pPcomObj = pTclPcomObj->GetPcomObj();

	// 创建一个临时的参数对象数组,用于保存信息
	CTclPcomParamArray arLocalParams;
	int i;
	for(i=0; i<pTclPcomFunc->m_arParams.GetSize(); i++)
	{
		arLocalParams.Add(pTclPcomFunc->m_arParams[i]);
	}

	// 用于参数传递的缓冲区,首先计算所有参数依次放在此缓冲区中
	char bufParam[TCLPCOM_MAXPARAMSIZE];
	memset(bufParam, 0, TCLPCOM_MAXPARAMSIZE);

	// stdcall为从右到左的压栈次序
	int nParamCount = arLocalParams.GetSize();
	int nPos = 0;
	for(i=nParamCount-1; i >= 0; i--)
	{
		// 传入参数
		CString strVar = "";
		CString strValue = "";
		CStringArray asValue;

		if( (PTYPE_CSTRINGARRAY == arLocalParams[i].GetParamType()) ||
			(PTYPE_UINTARRAY == arLocalParams[i].GetParamType()) )
		{
			Tcl_Obj* listObj = NULL;

			if(arLocalParams[i].NeedRefVar())
			{
				if((3+i) < objc)
				{
					strValue = ConvertUTFParam(Tcl_GetString(objv[3+i]));
				}
				// 表示传入的参数是一个列表名,获取列表内容,添加到数组
				strVar = strValue;
				listObj = Tcl_GetVar2Ex(interp, strVar.GetBuffer(strVar.GetLength()), NULL, TCL_LIST_ELEMENT);
				strVar.ReleaseBuffer();
				
			}else
			{
				if((3+i) < objc)
				{
					listObj = objv[3+i];
				}
			}

			if(listObj != NULL)
			{
				int nListCount = 0;
				Tcl_Obj** paList;
				if(TCL_ERROR == Tcl_ListObjGetElements(interp, listObj, &nListCount, &paList))
				{
					Tcl_AppendResult(interp, "get list elements from param error.",  (char *) NULL);
					return TCL_ERROR;
				}
				for(int k=0; k<nListCount; k++)
				{
					Tcl_Obj* list = (paList[k]);
					asValue.Add(ConvertUTFParam(list->bytes));
				}
			}

		}else
		{

			if((3+i) < objc)
			{
				strValue = ConvertUTFParam(Tcl_GetString(objv[3+i]));
			}

			if(arLocalParams[i].NeedRefVar())
			{
				// 表示传入的参数是一个变量名,获取到变量的初值
				strVar = strValue;
				CString szValue	= Tcl_GetVar(interp, strVar.GetBuffer(strVar.GetLength()), TCL_LEAVE_ERR_MSG);
				strVar.ReleaseBuffer();
				strValue = szValue;
			}
		}

		// 如果是安装或卸载回调函数的命令,则参数2不能用传入的,而应该用处理过后对应的C语言函数指针
		if((strFunc=="InstallHandler" || strFunc=="UninstallHandler") && (i==1))
		{
			if(strFunc=="InstallHandler")	// 安装
			{
				// 调用脚本获取参数个数
				char szScript[64];
				sprintf(szScript, "llength [info args %s]", strValue);
				if(TCL_OK != Tcl_Eval(interp, szScript))
				{
					// 查询参数列表错误,可能因为指定的回调脚本函数不存在
					Tcl_AppendResult(interp, "Get device callback func ", strValue, " args list error.", (char *) NULL);
					ConvertResultToUTF(interp);
					return TCL_ERROR;
				}			

				int nArgsCount = atoi(Tcl_GetStringResult(interp));
				Tcl_ResetResult(interp);

				CTclPcomCallBack* pCallBack = 
					CTclPcomCallBack::InstallCallBack(interp, strValue, nArgsCount,
								pTclPcomObj, arLocalParams[0].m_uiUint);
				if(pCallBack == NULL)
				{
					// 安装回调失败,可能是因为回调资源已经被占用
					Tcl_AppendResult(interp, "Tcl vci callback func ", strValue, " install failed.", (char *) NULL);
					ConvertResultToUTF(interp);
					return TCL_ERROR;
				}else
				{
					// 设置strValue为实际的函数地址
					strValue.Format("%d", pCallBack->m_pCallBackFunc);
				}
			}else	// 卸载
			{
				CTclPcomCallBack* pCallBack = CTclPcomCallBack::GetCallBack(pTclPcomObj, arLocalParams[0].m_uiUint);
				if(pCallBack == NULL)
				{
					// 卸载失败,未找到回调对象
					Tcl_AppendResult(interp, "Tcl vci callback func ", strValue, " uninstall failed.", (char *) NULL);
					ConvertResultToUTF(interp);
					return TCL_ERROR;
				}else
				{
					// 设置strValue为实际的函数地址
					strValue.Format("%d", pCallBack->m_pCallBackFunc);
					CTclPcomCallBack::UnInstallCallBack(pTclPcomObj, arLocalParams[0].m_uiUint);
				}
			}
		}

		// 参数初始化(VarInit内部可以处理缺省参数)
		// 注:目前对缺省参数的处理还有问题,对于缺省值是宏定义的,不能有效处理
		if( (PTYPE_CSTRINGARRAY == arLocalParams[i].GetParamType()) ||
			(PTYPE_UINTARRAY == arLocalParams[i].GetParamType()) )
		{
			arLocalParams[i].VarInitArray(strVar, &asValue);
		}else
		{
			arLocalParams[i].VarInit(strVar, strValue);
		}

		// 先按照最简单的,认为每个变量都占4个字节的方法实现,以后再增强
		// 缓冲区内按照变量大小/变量值这样成对放置
		//(int*) pSize = (int*)((char*)bufParam+nPos);
		//memcpy(bufParam+nPos, ,arLocalParams[i].GetParamSize());
		//nPos += arLocalParams[i].GetParamSize();
		memcpy(bufParam+nPos, &(arLocalParams[i].m_nCommVar), sizeof(int));
		nPos += sizeof(int);
	}

	// 如果返回值为CString类型,实际上传递参数时候要多传递一个
	int nParam_RetCount = nParamCount;

	// 初始化返回值对象
	int nTmpRet = 0; // 用于存储返回值的临时变量,只用于返回CString的情况,
					 // 因为这种情况下返回的eax没有用处,需要找个临时变量存储,
					 // 以免覆盖掉正确的结果
	void* pTmpRet = &nTmpRet;
	if(PTYPE_CSTRING == pTclPcomFunc->m_Ret.m_nType)
	{
		// 返回值为CString的情况,需要多传递一个参数,即一个临时CString变量的地址
		pTclPcomFunc->m_Ret.m_nMode	= PMODE_REFERENCE;
		pTclPcomFunc->m_Ret.VarInit("", "");
		memcpy(bufParam+nPos, &(pTclPcomFunc->m_Ret.m_nCommVar), sizeof(int));
		nParam_RetCount++;
		nPos += sizeof(int);
		memcpy(bufParam+nPos, &(pTmpRet), sizeof(int));
	}else
	if(PTYPE_CTIME == pTclPcomFunc->m_Ret.m_nType)
	{
		// 返回值为CTime的情况,需要多传递一个参数,即一个临时CTime变量的地址
		pTclPcomFunc->m_Ret.m_nMode	= PMODE_REFERENCE;
		pTclPcomFunc->m_Ret.VarInit("", "");
		memcpy(bufParam+nPos, &(pTclPcomFunc->m_Ret.m_nCommVar), sizeof(int));
		nParam_RetCount++;
		nPos += sizeof(int);
		memcpy(bufParam+nPos, &(pTmpRet), sizeof(int));
	}else
	{
		pTclPcomFunc->m_Ret.m_nMode	= PMODE_REFERENCE;
		pTclPcomFunc->m_Ret.VarInit("", "");
		memcpy(bufParam+nPos, &(pTclPcomFunc->m_Ret.m_nCommVar), sizeof(int));
	}

	int nRetTypeIsFloat = 0;	// 返回值类型是否float的标志
	if(PTYEP_FLOAT == pTclPcomFunc->m_Ret.m_nType)
	{
		nRetTypeIsFloat = 1;
	}

	// 3.通过汇编代码调用封装
	//int nRet;
	//void* pRet = (void*)(&nRet);
	try
	{
		_asm
		{
			// 保存环境寄存器
			push	eax;
			push	edx;
			push	ecx;

			// 从参数缓冲区中循环取参数并压栈
			mov		ecx, nParam_RetCount;

			// 判断参数个数,如果为0则不用执行下面的压栈操作
			cmp		ecx, 0;
			jz		next;

			lea		edx, bufParam;
	again:	mov		eax, [edx];
			push	eax;
			add		edx, 4;
			dec		ecx
			jecxz	next;
			jmp		again;

			// 对象首地址压栈
	next:	mov		eax, pPcomObj;
			push	eax;

			// 根据函数索引值计算函数在虚函数表中的位置
			mov		edx, [eax];
			mov		eax, nFuncIndex;
			shl		eax, 2;
			// 调用虚函数表中对应位置的函数
			call	dword ptr [edx+eax];

			// 传递返回值
			lea		edx, bufParam;
			mov		ecx, nParam_RetCount;
			shl		ecx, 2;
			add		edx, ecx;

			// 判断返回值类型是否float,如果是就从协处理器栈顶取数据,否则还是从eax取
			mov		ecx, nRetTypeIsFloat;
			jecxz	reteax;
			mov		ecx, dword ptr [edx];
			fst		dword ptr [ecx];
			jmp		end;

	reteax:	mov		ecx, dword ptr [edx];	// 正常的获取返回值方法(取eax的值)
			mov		dword ptr [ecx], eax;

//			lea		edx, nRet;				// 设置返回值(取消这种方法)
//			mov		dword ptr [edx], eax;

			// 恢复环境寄存器
	end:	pop		ecx;
			pop		edx;
			pop		eax;
		}

		// 4.回传参数
		for(i=0; i<nParamCount; i++)
		{
			CString strVarRet = arLocalParams[i].VarRelease();
			if(arLocalParams[i].NeedRefVar())
			{
				if(PTYPE_CSTRINGARRAY == arLocalParams[i].GetParamType())
				{
					CString strVar = arLocalParams[i].m_strRefVar;
					Tcl_Obj* listObj = Tcl_GetVar2Ex(interp, strVar.GetBuffer(strVar.GetLength()), NULL, TCL_LIST_ELEMENT);
					strVar.ReleaseBuffer();
					int nLength = 0;
					int nResult = Tcl_ListObjLength(interp, listObj, &nLength);
					if(nLength == 0)
					{
						// 如果列表长度为0,需要新创建一个列表,避免后面添加操作产生异常
						listObj = Tcl_NewListObj(0, NULL);
						listObj = Tcl_SetVar2Ex(interp, strVar.GetBuffer(strVar.GetLength()), NULL, listObj, TCL_LIST_ELEMENT);
						strVar.ReleaseBuffer();
					}

					for(int el = 0; el < arLocalParams[i].m_asString.GetSize(); el++)
					{
						CString strElement = arLocalParams[i].m_asString[el];
						strElement = ConvertStringToUTF(strElement);
						Tcl_Obj* objPtr = Tcl_NewStringObj(strElement.GetBuffer(strElement.GetLength()), strElement.GetLength());
						strElement.ReleaseBuffer();
						Tcl_ListObjAppendElement(interp, listObj, objPtr);
					}
				}else
				if(PTYPE_UINTARRAY == arLocalParams[i].GetParamType())
				{
					CString strVar = arLocalParams[i].m_strRefVar;
					Tcl_Obj* listObj = Tcl_GetVar2Ex(interp, strVar.GetBuffer(strVar.GetLength()), NULL, TCL_LIST_ELEMENT);
					strVar.ReleaseBuffer();
					int nLength = 0;
					int nResult = Tcl_ListObjLength(interp, listObj, &nLength);
					if(nLength == 0)
					{
						// 如果列表长度为0,需要新创建一个列表,避免后面添加操作产生异常
						listObj = Tcl_NewListObj(0, NULL);
						listObj = Tcl_SetVar2Ex(interp, strVar.GetBuffer(strVar.GetLength()), NULL, listObj, TCL_LIST_ELEMENT);
						strVar.ReleaseBuffer();
					}

					for(int el = 0; el < arLocalParams[i].m_auUInt.GetSize(); el++)
					{
						CString strElement;
						strElement.Format("%u", arLocalParams[i].m_auUInt[el]);
						strElement = ConvertStringToUTF(strElement);
						Tcl_Obj* objPtr = Tcl_NewStringObj(strElement.GetBuffer(strElement.GetLength()), strElement.GetLength());
						strElement.ReleaseBuffer();
						Tcl_ListObjAppendElement(interp, listObj, objPtr);
					}
				}else
				{
					strVarRet = ConvertStringToUTF(strVarRet);
					CString strVar = arLocalParams[i].m_strRefVar;
					char* szVar = strVar.GetBuffer(strVar.GetLength());
					char* szVarRet = strVarRet.GetBuffer(strVarRet.GetLength());
					Tcl_SetVar(interp, szVar, szVarRet, TCL_LEAVE_ERR_MSG);
					strVar.ReleaseBuffer();
					strVarRet.ReleaseBuffer();
				}
			}
		}

		// 5.设置返回值
		if( (PTYPE_VOID != pTclPcomFunc->m_Ret.m_nType) &&
			(PTYPE_UNKNOWN != pTclPcomFunc->m_Ret.m_nType) )
		{
			Tcl_ResetResult(interp);
			CString strRet = pTclPcomFunc->m_Ret.VarRelease();
			Tcl_AppendResult(interp, strRet, (char *) NULL);
		}
	}
	catch(...)
	{
		Tcl_AppendResult(interp, "function exec failed, catch a unknown exception.", (char *) NULL);
		return TCL_ERROR;
	}

	ConvertResultToUTF(interp);
	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:调用VCI组件对象的函数
// 用  法: 
//	vci::callback handle install eventId command
//	vci::callback handle uninstall eventId command
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_DeviceCallBack(ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[])
{
	VERIFY_TCLCMD_RIGHT(USER_OPERATOR);

	if(objc < 5)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "handle install|uninstall eventId command");
		return TCL_ERROR;
	}

	if(g_pIPlatUI == NULL)
	{
		CTclInterp* pTclInterp = GetTclInterp((LPVOID)interp);
		if(pTclInterp)
		{
			g_pIPlatUI = pTclInterp->pIPlatUI;
		}
	}

	// 获取参数
	CString strpDev = ConvertUTFParam(Tcl_GetString(objv[1]));
	CString strOperate = ConvertUTFParam(Tcl_GetString(objv[2]));
	int nEventId = atoi(ConvertUTFParam(Tcl_GetString(objv[3])));
	CString strCommand = ConvertUTFParam(Tcl_GetString(objv[4]));
	CTclPcomObj* pTclPcomObj = (CTclPcomObj*)(atoi(strpDev));
	// 检查指针的合法性
	if(!IsValidTclPcomObj(pTclPcomObj))
	{
		Tcl_AppendResult(interp, "Invalid vci object handle: ", strpDev, (char *) NULL);
		return TCL_ERROR;
	}

	// 检查PCOM类封装对象是否存在
	CTclPcomClass* pTclPcomClass = pTclPcomObj->GetTclPcomClass();
	if(!pTclPcomClass)
	{
		Tcl_AppendResult(interp, "Not find vci define object.", (char *) NULL);
		return TCL_ERROR;
	}

	// 获取IVciControl接口指针
	IVciControl* pVciControl = (IVciControl*)(g_pIPlatUI->GetObjectControlPtrByInstancePtr(pTclPcomObj->GetPcomObj()));
	if(pVciControl == NULL)
	{
		Tcl_AppendResult(interp, "Invalid vci control interface.", (char *) NULL);
		return TCL_ERROR;
	}

	try
	{
	///////////////////////////////////////////////////////

	if(strOperate=="install")	// 安装
	{
		// 调用脚本获取参数个数
		char szScript[64];
		sprintf(szScript, "llength [info args %s]", strCommand);
		if(TCL_OK != Tcl_Eval(interp, szScript))
		{
			// 查询参数列表错误,可能因为指定的回调脚本函数不存在
			Tcl_AppendResult(interp, "Get device callback func ", strCommand, " args list error.", (char *) NULL);
			ConvertResultToUTF(interp);
			return TCL_ERROR;
		}			

		int nArgsCount = atoi(Tcl_GetStringResult(interp));
		Tcl_ResetResult(interp);

		CTclPcomCallBack* pCallBack = 
			CTclPcomCallBack::InstallCallBack(interp, strCommand, nArgsCount,
						pTclPcomObj, nEventId);
		if(pCallBack == NULL)
		{
			// 安装回调失败,可能是因为回调资源已经被占用
			Tcl_AppendResult(interp, "Tcl vci callback func ", strCommand, " install failed.", (char *) NULL);
			ConvertResultToUTF(interp);
			return TCL_ERROR;
		}else
		{
			pVciControl->InstallHandler(nEventId, pCallBack->m_pCallBackFunc, 0);
		}
	}else
	if(strOperate=="uninstall")	// 卸载
	{
		CTclPcomCallBack* pCallBack = CTclPcomCallBack::GetCallBack(pTclPcomObj, nEventId);
		if(pCallBack == NULL)
		{
			// 卸载失败,未找到回调对象
			Tcl_AppendResult(interp, "Tcl vci callback func ", strCommand, " uninstall failed.", (char *) NULL);
			ConvertResultToUTF(interp);
			return TCL_ERROR;
		}else
		{
			pVciControl->UninstallHandler(nEventId, pCallBack->m_pCallBackFunc);
			CTclPcomCallBack::UnInstallCallBack(pTclPcomObj, nEventId);
		}
	}else
	{
		Tcl_AppendResult(interp, "operater must be install or uninstall.",  (char *) NULL);
		return TCL_ERROR;
	}

	//////////////////////////////////////////////////////////
	}
	catch(...)
	{
		Tcl_AppendResult(interp, "install/uninstall callback failed, catch a unknown exception.", (char *) NULL);
		return TCL_ERROR;
	}

	ConvertResultToUTF(interp);
	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:OWM扩展命令
// owm showview owmid ?active?							- 显示OWM View
// owm closeview owmid									- 关闭OWM View
// owm sendcmd owmid CmdCode wParam lParam ?returntype?	- 发送OWM命令
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_OWM(ClientData clientData, Tcl_Interp* interp, int argc, const char* argv[])
{
	VERIFY_TCLCMD_RIGHT(USER_OPERATOR);

	if(argc < 2)
	{
		interp->result = "wrong # args: should be \"owm operate params\"";
		return TCL_ERROR;
	}

	if(g_pIPlatUI == NULL)
	{
		CTclInterp* pTclInterp = GetTclInterp((LPVOID)interp);
		if(pTclInterp)
		{
			g_pIPlatUI = pTclInterp->pIPlatUI;
		}
	}
	if(g_pIPlatUI == NULL)
	{
		Tcl_AppendResult(interp, "Get platform interface fail.", (char *) NULL);
		return TCL_ERROR;
	}

	CString strOption = ConvertUTFParam(argv[1]);
	if(strOption[0] == '-')
		strOption.Delete(0, 1);
	strOption.MakeUpper();

	if(strOption == "SHOWVIEW")	// 显示OWM View
	{
		if(argc < 3)
		{
			interp->result = "wrong # args: should be \"owm showview owmid ?active?\"";
			return TCL_ERROR;
		}
		CString strOwmId = ConvertUTFParam(argv[2]);

		BOOL bActive = FALSE;
		if(argc > 3)
		{
			bActive = (ConvertUTFParam(argv[3]) == "active");
		}

		int nRet;
		g_pIPlatUI->SendOwmCmd("", OC_OPENVIEW, (WPARAM)(LPCTSTR)strOwmId, bActive, &nRet);

	}else
	if(strOption == "CLOSEVIEW")	// 关闭OWM View
	{
		if(argc < 3)
		{
			interp->result = "wrong # args: should be \"owm closeview owmid\"";
			return TCL_ERROR;
		}
		CString strOwmId = ConvertUTFParam(argv[2]);

		int nRet;
		g_pIPlatUI->SendOwmCmd("", OC_CLOSEVIEW, (WPARAM)(LPCTSTR)strOwmId, 0, &nRet);

	}else
	if(strOption == "SENDCMD")	// 发送OWM命令
	{
		if(argc < 6)
		{
			interp->result = "wrong # args: should be \"owm sendcmd owmid cmdcode wparam lparam ?returntype?\"";
			return TCL_ERROR;
		}

		CString strReturnType = "";

		CString strOwmId = ConvertUTFParam(argv[2]);
		int nCmdCode = atoi(ConvertUTFParam(argv[3]));
		WPARAM wParam = 0;
		LPARAM lParam = 0;
		CString strwParam = ConvertUTFParam(argv[4]);

		CUIntArray auwParam;
		CUIntArray aulParam;
		BOOL bwUIntArray = FALSE;	// 整形数组标志
		BOOL blUIntArray = FALSE;
		int  nwRef;					// 整形引用类型的变量
		int  nlRef;
		BOOL bwIRef	= FALSE;		// 引用类型标志
		BOOL blIRef	= FALSE;

		if(strwParam.Find("<string>") == 0)	// 字符串参数
		{
			strwParam.Delete(0, 8);
			wParam = (WPARAM)(LPCTSTR)strwParam;
		}else
		if(strwParam.Find("<uintarray>") == 0)	// 整型数组参数(可以回传信息,输入变量为列表变量)
		{
			strwParam.Delete(0, 11);
			// 获取列表内容,添加到数组
			Tcl_Obj* listObj = Tcl_GetVar2Ex(interp, strwParam.GetBuffer(strwParam.GetLength()), NULL, TCL_LIST_ELEMENT);
			if(listObj != NULL)
			{
				int nListCount = 0;
				Tcl_Obj** paList;
				if(TCL_ERROR == Tcl_ListObjGetElements(interp, listObj, &nListCount, &paList))
				{
					Tcl_AppendResult(interp, "get list elements from wparam error.",  (char *) NULL);
					return TCL_ERROR;
				}
				for(int k=0; k<nListCount; k++)
				{
					Tcl_Obj* list = (paList[k]);
					auwParam.Add(atoi(ConvertUTFParam(list->bytes)));
				}
			}
			wParam = (WPARAM)(&auwParam);
			bwUIntArray = TRUE;
		}else
		if(strwParam.Find("<&int>") == 0)	// 引用类型的整形
		{
			strwParam.Delete(0, 6);
			CString szVar = Tcl_GetVar(interp, strwParam.GetBuffer(strwParam.GetLength()), 0);
			nwRef = atoi(szVar);
			wParam = (WPARAM)(&nwRef);
			bwIRef = TRUE;
		}else
		{
			wParam = atoi(ConvertUTFParam(argv[4]));
		}
		CString strlParam = ConvertUTFParam(argv[5]);
		if(strlParam.Find("<string>") == 0)
		{
			strlParam.Delete(0, 8);
			lParam = (WPARAM)(LPCTSTR)strlParam;
		}else
		if(strlParam.Find("<uintarray>") == 0)	// 整型数组参数(可以回传信息,输入变量为列表变量)
		{
			strlParam.Delete(0, 11);
			// 获取列表内容,添加到数组
			Tcl_Obj* listObj = Tcl_GetVar2Ex(interp, strlParam.GetBuffer(strlParam.GetLength()), NULL, TCL_LIST_ELEMENT);
			if(listObj != NULL)
			{
				int nListCount = 0;
				Tcl_Obj** paList;
				if(TCL_ERROR == Tcl_ListObjGetElements(interp, listObj, &nListCount, &paList))
				{
					Tcl_AppendResult(interp, "get list elements from lparam error.",  (char *) NULL);
					return TCL_ERROR;
				}
				for(int k=0; k<nListCount; k++)
				{
					Tcl_Obj* list = (paList[k]);
					aulParam.Add(atoi(ConvertUTFParam(list->bytes)));
				}
			}
			lParam = (LPARAM)(&aulParam);
			blUIntArray = TRUE;
		}else
		if(strlParam.Find("<&int>") == 0)	// 引用类型的整形
		{
			strlParam.Delete(0, 6);
			CString szVar = Tcl_GetVar(interp, strlParam.GetBuffer(strlParam.GetLength()), 0);
			nlRef = atoi(szVar);
			lParam = (WPARAM)(&nlRef);
			blIRef = TRUE;
		}else
		{
			lParam = atoi(ConvertUTFParam(argv[5]));
		}

		if(argc > 6)
		{
			strReturnType = ConvertUTFParam(argv[6]);
		}

		int nRet;
		g_pIPlatUI->SendOwmCmd(strOwmId, nCmdCode, wParam, lParam, &nRet);

		// 参数是CUIntArray时候的返回列表变量设置
		if(bwUIntArray && (strwParam.GetLength() > 0))
		{
			CString strCmd;
			strCmd.Format("set %s [list];", strwParam);
			Tcl_Eval(interp, strCmd.GetBuffer(0));
			strCmd.ReleaseBuffer();
			for(int i=0; i<auwParam.GetSize(); i++)
			{
				strCmd.Format("lappend %s %d;", strwParam, auwParam[i]);
				Tcl_Eval(interp, strCmd.GetBuffer(0));
				strCmd.ReleaseBuffer();
			}
		}
		if(blUIntArray && (strlParam.GetLength() > 0))
		{
			CString strCmd;
			strCmd.Format("set %s [list];", strlParam);
			Tcl_Eval(interp, strCmd.GetBuffer(0));
			strCmd.ReleaseBuffer();
			for(int i=0; i<aulParam.GetSize(); i++)
			{
				strCmd.Format("lappend %s %d;", strlParam, aulParam[i]);
				Tcl_Eval(interp, strCmd.GetBuffer(0));
				strCmd.ReleaseBuffer();
			}
		}
		// 参数是整形引用类型时的返回值设置到Tcl变量中
		if(bwIRef)
		{
			CString strTmp;
			strTmp.Format("%d", nwRef);
			Tcl_SetVar(interp, strwParam.GetBuffer(0), strTmp.GetBuffer(0), TCL_LEAVE_ERR_MSG);
			strwParam.ReleaseBuffer();
			strTmp.ReleaseBuffer();
		}
		if(blIRef)
		{
			CString strTmp;
			strTmp.Format("%d", nlRef);
			Tcl_SetVar(interp, strlParam.GetBuffer(0), strTmp.GetBuffer(0), TCL_LEAVE_ERR_MSG);
			strlParam.ReleaseBuffer();
			strTmp.ReleaseBuffer();
		}

		CString strRet;
		if(strReturnType == "<string>")
		{
			strRet = (LPCTSTR)nRet;
		}else
		{
			strRet.Format("%d", nRet);
		}
		Tcl_AppendResult(interp, strRet, (char *) NULL);
	}else
	{
		Tcl_AppendResult(interp, "bad argument \"", argv[1], "\" : must be showview, closeview or sendcmd", (char *) NULL);
		return TCL_ERROR;
	}

	ConvertResultToUTF(interp);
	return TCL_OK;
}

//TCL命令声明函数
int Tclvci_Init(Tcl_Interp *interp)
{
	// 注册VCI组件调用类命令
	// 打开一个VCI组件
	Tcl_CreateObjCommand(interp, "vci_open", Tcl_Cmd_DeviceOpen, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 关闭一个VCI组件
	Tcl_CreateObjCommand(interp, "vci_close", Tcl_Cmd_DeviceClose, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// VCI组件的函数操作
	Tcl_CreateObjCommand(interp, "vci_info", Tcl_Cmd_DeviceInfo, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 调用VCI组件对象的函数
	Tcl_CreateObjCommand(interp, "vci_exec", Tcl_Cmd_DeviceExec, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 安装/卸载VCI组件对象的回调函数
	Tcl_CreateObjCommand(interp, "vci_callback", Tcl_Cmd_DeviceCallBack, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// OWM扩展命令
	Tcl_CreateCommand(interp, "owm", Tcl_Cmd_OWM, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);
	
    return TCL_OK;
}
