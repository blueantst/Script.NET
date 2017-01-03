////////////////////////////////////////////////////////////////////////////
//	File:		vcimessage.h
//	Version:	1.0.0.0
//	Created:	2005-08-23
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	Comm define of VCI Message.
////////////////////////////////////////////////////////////////////////////
#ifndef __VCIMESSAGE_H_
#define __VCIMESSAGE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 定义数据加密类型
enum {
	VME_NONE,		// 不加密
	VME_BASE64,		// Base64编码
	VME_NOT,		// 取反加密
	VME_XOR,		// 异或加密
	VME_DES,		// DES加密
};

// 定义消息优先级
#define VMP_HIGH	1		// 高优先级
#define VMP_NORMAL	3		// 普通优先级
#define VMP_LOW		5		// 低优先级

// 定义命令句柄
#define VMH_NULL	0x0000	// 空句柄

// 定义消息类型
enum {
	VMT_NORMAL,		// 普通消息
	VMT_BROADCAST,	// 广播消息
	VMT_MULTICAST,	// 组播消息
	VMT_FILTER,		// 过滤消息
};

// 定义请求/应答类型
enum {
	VMT_REQUEST,	// 请求
	VMT_RESPONSE,	// 应答
};

// 定义同步/异步消息
enum {
	VMT_SYNC,		// 同步消息
	VMT_ASYNC,		// 异步消息
};

// 定义消息结果
enum {
	VMR_OK,			// 执行成功
	VMR_FAIL,		// 执行失败
};

// 定义数据类型
enum {
	VMD_NULL,		// 空数据类型
	VMD_DATA,		// 二进制数据流
	VMD_STRING,		// 字符串数据
	VMD_XML,		// XML数据
};

// 定义消息数据的最大长度
#define VMD_MAX_SIZE			0x100000

// 定义命令码
// 基本连接命令
#define VMC_NULL				0x0000	// 空命令
#define VMC_CHECK_LINK			0x0001	// 检查联接
#define VMC_RELINK				0x0002	// 重连
#define VMC_CONNECT				0x0003	// 连接
#define VMC_CONNECT_CONFIRM		0x0004	// 连接证实
#define VMC_CLOSE				0x0005	// 关闭
#define VMC_CLOSE_CONFIRM		0x0006	// 关闭证实

// 口令组
#define VMC_CHECKPWD			0x0010	// 口令验证请求
#define VMC_CHECKPWD_ACK		0x0011	// 口令验证应答
#define VMC_CHECKPWD_CONFIRM	0x0012	// 口令验证证实
#define VMC_RIGHT_SETRIGHT		0x0015	// 设置远程用户权限
#define VMC_RIGHT_GETRIGHT		0x0016	// 获取远程用户权限

// 基本命令
#define VMC_RUNCOMMAND			0x0020	// 执行命令
#define VMC_RUNSCRIPT			0x0021	// 执行脚本
#define VMC_SENDOWMCMD			0x0022	// 发送OWM命令
#define VMC_SHOWMESSAGE			0x0023	// 显示信息
#define VMC_SENDDATA			0x0024	// 传送数据
#define VMC_GETCONFIG			0x0025	// 获取配置信息
#define VMC_CONFIG				0x0026	// 设置配置信息

// 在线升级
#define VMC_GETVERSION			0x0030	// 获取平台版本
#define VMC_SETNEWVER			0x0031	// 设置新版本号
#define VMC_DOWMLOAD_PLAT		0x0032	// 下载平台安装包
#define VMC_DOWNLOAD_PLUGIN		0x0033	// 下载插件

// 脚本命令
#define VMC_INTERP_OPEN			0x0040	// 打开脚本解释器
#define VMC_INTERP_CLOSE		0x0041	// 关闭脚本解释器
#define VMC_INTERP_EVALFILE		0x0042	// 执行脚本文件
#define VMC_INTERP_EVALCOMMAND	0x0043	// 执行脚本命令
#define VMC_INTERP_GETERRORLINE	0x0044	// 获取错误行号
#define VMC_INTERP_GETINTERPID	0x0045	// 获取解释器ID

// 消息队列命令
#define VMC_MSGQUEUE_EXIT		0x0050	// 结束消息队列
#define VMC_MSGQUEUE_ABNORMAL	0x0051	// 消息队列发生异常

class CVciMessage
{
// attribuite
public:
	int		nEncrypt;	// 数据加密类型
	int		nPriority;	// 优先级
	CString	szMsgId;	// 消息ID
	CString	szFrom;		// 来源(组件ID)
	CString	szDest;		// 目的地(组件ID)
	CString	szUserFrom;	// 来源用户
	CString	szUserTo;	// 目的用户
	int		nCmdCode;	// 命令码
	int		nHandle;	// 唯一句柄
	int		nMsgType;	// 消息类型(普通/广播)
	int		nRequest;	// 请求/应答消息
	int		nSync;		// 同步/异步消息
	int		nTimeOut;	// 超时时间(毫秒)
	int		nResult;	// 执行结果
	int		nDataType;	// 数据类型
	int		nDataLen;	// 数据长度
	BYTE*	pData;		// 数据指针
	CString	szContent;	// 消息体(XML流)

// Operation
public:
	CVciMessage()
	{
		nEncrypt	= VME_NONE;
		nPriority	= VMP_NORMAL;
		szMsgId		= "";
		szFrom		= "";
		szDest		= "";
		szUserFrom	= "";
		szUserTo	= "";
		nCmdCode	= VMC_NULL;
		nHandle		= VMH_NULL;
		nMsgType	= VMT_NORMAL;
		nRequest	= VMT_REQUEST;
		nSync		= VMT_SYNC;
		nTimeOut	= 100;	// 默认100毫秒
		nResult		= VMR_OK;
		nDataType	= VMD_NULL;
		nDataLen	= 0;
		pData		= NULL;
		szContent	= "";
	}

	CVciMessage(int _nCmdCode, int _nHandle = VMH_NULL)
	{
		nEncrypt	= VME_NONE;
		nPriority	= VMP_NORMAL;
		szMsgId		= "";
		szFrom		= "";
		szDest		= "";
		szUserFrom	= "";
		szUserTo	= "";
		nCmdCode	= _nCmdCode;
		nHandle		= _nHandle;
		nMsgType	= VMT_NORMAL;
		nRequest	= VMT_REQUEST;
		nSync		= VMT_SYNC;
		nTimeOut	= 100;	// 默认100毫秒
		nResult		= VMR_OK;
		nDataType	= VMD_NULL;
		nDataLen	= 0;
		pData		= NULL;
		szContent	= "";
	}

	CVciMessage(LPCTSTR lpszMsgId, LPCTSTR lpszFrom, LPCTSTR lpszDest)
	{
		nEncrypt	= VME_NONE;
		nPriority	= VMP_NORMAL;
		szMsgId		= lpszMsgId;
		szFrom		= lpszFrom;
		szDest		= lpszDest;
		szUserFrom	= "";
		szUserTo	= "";
		nCmdCode	= VMC_NULL;
		nHandle		= VMH_NULL;
		nMsgType	= VMT_NORMAL;
		nRequest	= VMT_REQUEST;
		nSync		= VMT_SYNC;
		nTimeOut	= 100;	// 默认100毫秒
		nResult		= VMR_OK;
		nDataType	= VMD_NULL;
		nDataLen	= 0;
		pData		= NULL;
		szContent	= "";
	}

	virtual ~CVciMessage()
	{
		if(pData != NULL)
		{
			delete pData;
			pData = NULL;
		}
	}

	CVciMessage(const CVciMessage& other)
	{
		nEncrypt	= other.nEncrypt;
		nPriority	= other.nPriority;
		szMsgId		= other.szMsgId;
		szFrom		= other.szFrom;
		szDest		= other.szDest;
		szUserFrom	= other.szUserFrom;
		szUserTo	= other.szUserTo;
		nCmdCode	= other.nCmdCode;
		nHandle		= other.nHandle;
		nMsgType	= other.nMsgType;
		nRequest	= other.nRequest;
		nSync		= other.nSync;
		nTimeOut	= other.nTimeOut;
		nResult		= other.nResult;
		nDataType	= other.nDataType;
		nDataLen	= other.nDataLen;
		pData = NULL;
		if(nDataLen > 0)
		{
			pData = new BYTE[nDataLen];
			for(int i=0; i<nDataLen; i++)
				pData[i] = other.pData[i];
		}
		szContent	= other.szContent;
	}

	CVciMessage& operator = (const CVciMessage& other)
	{
		nEncrypt	= other.nEncrypt;
		nPriority	= other.nPriority;
		szMsgId		= other.szMsgId;
		szFrom		= other.szFrom;
		szDest		= other.szDest;
		szUserFrom	= other.szUserFrom;
		szUserTo	= other.szUserTo;
		nCmdCode	= other.nCmdCode;
		nHandle		= other.nHandle;
		nMsgType	= other.nMsgType;
		nRequest	= other.nRequest;
		nSync		= other.nSync;
		nTimeOut	= other.nTimeOut;
		nResult		= other.nResult;
		nDataType	= other.nDataType;
		nDataLen	= other.nDataLen;
		if(pData != NULL)
			delete pData;
		pData = NULL;
		if(nDataLen > 0)
		{
			pData = new BYTE[nDataLen];
			for(int i=0; i<nDataLen; i++)
				pData[i] = other.pData[i];
		}
		szContent	= other.szContent;
		return *this;
	}

	// 根据请求消息生成应答消息的基础内容
	void MakeResponceMessage(CVciMessage* pInMsg)
	{
		if(pInMsg)
		{
			nPriority	= pInMsg->nPriority;
			szMsgId		= pInMsg->szMsgId;
			szFrom		= pInMsg->szDest;
			szDest		= pInMsg->szFrom;
			szUserFrom	= pInMsg->szUserTo;
			szUserTo	= pInMsg->szUserFrom;
			nCmdCode	= pInMsg->nCmdCode;
			nHandle		= pInMsg->nHandle;
			nRequest	= VMT_RESPONSE;
		}
	}

	int GetEncrypt() { return nEncrypt; }
	void SetEncrypt(int _nEncrypt) { nEncrypt = _nEncrypt; }

	int GetPriority() { return nPriority; }
	void SetPriority(int _nPriority) { nPriority = _nPriority; }

	CString GetMsgId() { return szMsgId; }
	void SetMsgId(LPCTSTR lpszMsgId) { szMsgId = lpszMsgId; }

	CString GetFrom() { return szFrom; }
	void SetFrom(LPCTSTR lpszFrom) { szFrom = lpszFrom; }

	CString GetDest() { return szDest; }
	void SetDest(LPCTSTR lpszDest) { szDest = lpszDest; }

	CString GetFirstNodeId()	// 获取第一个路由节点ID
	{
		CString strNodeId = szDest;
		int nPos = strNodeId.Find("|");
		if(nPos != -1)
		{
			strNodeId = strNodeId.Left(nPos);
		}
		nPos = strNodeId.Find(":");
		if(nPos != -1)
		{
			strNodeId = strNodeId.Left(nPos);
		}
		return strNodeId;
	}
	CString GetFirstNode()	// 获取第一个路由节点
	{
		CString strNode = szDest;
		int nPos = strNode.Find("|");
		if(nPos != -1)
		{
			strNode = strNode.Left(nPos);
		}
		return strNode;
	}
	CString GetLastNode()	// 获取最后一个路由节点(目的节点)
	{
		CStringArray asNode;
		if(GetNodeList(asNode) > 0)
		{
			return asNode[asNode.GetSize()-1];
		}else
		{
			return "";
		}
	}
	int GetNodeList(CStringArray& asNode)	// 获取路由节点列表
	{
		CString strNodes = szDest;
		int nPos = -1;
		while((nPos = strNodes.Find("|")) != -1)
		{
			if(strNodes.Left(nPos) != "")
			{
				asNode.Add(strNodes.Left(nPos));
			}
			strNodes.Delete(0, nPos+1);
		}
		if(strNodes != "")
		{
			asNode.Add(strNodes);
		}
		return asNode.GetSize();
	}

	CString GetUserFrom() { return szUserFrom; }
	void SetUserFrom(LPCTSTR lpszUserFrom) { szUserFrom = lpszUserFrom; }

	CString GetUserTo() { return szUserTo; }
	void SetUserTo(LPCTSTR lpszUserTo) { szUserTo = lpszUserTo; }

	int GetCmdCode() { return nCmdCode; }
	void SetCmdCode(int _nCmdCode) { nCmdCode = _nCmdCode; }

	int GetHandle() { return nHandle; }
	void SetHandle(int _nHandle) { nHandle = _nHandle; }

	int GetMsgType() { return nMsgType; }
	void SetMsgType(int _nMsgType) { nMsgType = _nMsgType; }

	int GetRequest() { return nRequest; }
	void SetRequest(int _nRequest) { nRequest = _nRequest; }

	int GetSync() { return nSync; }
	void SetSync(int _nSync) { nSync = _nSync; }

	int GetTimeOut() { return nTimeOut; }
	void SetTimeOut(int _nTimeOut) { nTimeOut = _nTimeOut; }

	int GetResult() { return nResult; }
	void SetResult(int _nResult) { nResult = _nResult; }

	int GetDataType() { return nDataType; }
	void SetDataType(int _nDataType) { nDataType = _nDataType; }

	int GetDataLen() { return nDataLen; }

	void SetData(BYTE* _pData, int nLen)
	{
		if(pData != NULL)
		{
			delete pData;
		}
		pData		= NULL;
		nDataLen	= nLen;
		if(nDataLen > 0)
		{
			pData = new BYTE[nDataLen];
			memcpy(pData, _pData, nDataLen);
		}
	}
	BYTE* GetData() { return pData; }

	CString GetContent() { return szContent; }
	void SetContent(CString _szContent) { szContent = _szContent; }

	void SetString(CString str)
	{
		nDataType	= VMD_STRING;
		szContent	= str;
	}
	CString GetString()
	{
		if(nDataType == VMD_STRING)
		{
			return szContent;
		}
		return "";
	}

	void SetXML(CString str)
	{
		nDataType	= VMD_XML;
		szContent	= str;
	}
	CString GetXML()
	{
		if(nDataType == VMD_XML)
		{
			return szContent;
		}
		return "";
	}

	void SetOwmCmd(int nOwmCmdCode, int wParam, int lParam)
	{
		if(pData != NULL)
		{
			delete pData;
		}
		pData		= NULL;
		nDataLen	= 3 * sizeof(int);
		pData = new BYTE[nDataLen];
		int* ppData = (int*)pData;
		ppData[0] = nOwmCmdCode;
		ppData[1] = wParam;
		ppData[2] = lParam;
	}
	BOOL GetOwmCmd(int& nOwmCmdCode, int& wParam, int& lParam)
	{
		if((nCmdCode == VMC_SENDOWMCMD) && (pData != NULL))
		{
			int* ppData = (int*)pData;
			nOwmCmdCode = ppData[0];
			wParam = ppData[1];
			lParam = ppData[2];
			return TRUE;
		}
		return FALSE;
	}

	// Serialize from buffer
	virtual BOOL SerializeIn(BYTE* pBuffer, int nBufLen)
	{
		int nLen = 0;
		CString strSign;
		if(!SerialReadString(pBuffer, nBufLen, nLen, strSign)) return FALSE;
		if(strSign != "vcimessage") return FALSE;
		if(!SerialReadInt(pBuffer, nBufLen, nLen, nEncrypt)) return FALSE;
		if(!SerialReadInt(pBuffer, nBufLen, nLen, nPriority)) return FALSE;
		if(!SerialReadString(pBuffer, nBufLen, nLen, szMsgId)) return FALSE;
		if(!SerialReadString(pBuffer, nBufLen, nLen, szFrom)) return FALSE;
		if(!SerialReadString(pBuffer, nBufLen, nLen, szDest)) return FALSE;
		if(!SerialReadString(pBuffer, nBufLen, nLen, szUserFrom)) return FALSE;
		if(!SerialReadString(pBuffer, nBufLen, nLen, szUserTo)) return FALSE;
		if(!SerialReadInt(pBuffer, nBufLen, nLen, nCmdCode)) return FALSE;
		if(!SerialReadInt(pBuffer, nBufLen, nLen, nHandle)) return FALSE;
		if(!SerialReadInt(pBuffer, nBufLen, nLen, nMsgType)) return FALSE;
		if(!SerialReadInt(pBuffer, nBufLen, nLen, nRequest)) return FALSE;
		if(!SerialReadInt(pBuffer, nBufLen, nLen, nSync)) return FALSE;
		if(!SerialReadInt(pBuffer, nBufLen, nLen, nTimeOut)) return FALSE;
		if(!SerialReadInt(pBuffer, nBufLen, nLen, nResult)) return FALSE;
		if(!SerialReadInt(pBuffer, nBufLen, nLen, nDataType)) return FALSE;
		if(!SerialReadData(pBuffer, nBufLen, nLen)) return FALSE;
		if(!SerialReadString(pBuffer, nBufLen, nLen, szContent)) return FALSE;

		return TRUE;
	}

	// Serialize to buffer
	virtual BYTE* SerializeOut(int& nBufLen)
	{
		int nMaxLen = 10240;
		BYTE* pBuffer = new BYTE[nMaxLen];
		BYTE* pBufStart = pBuffer;

		nBufLen = 0;

		CString strSign = "vcimessage";
		if(!SerialWriteString(pBuffer, nMaxLen, nBufLen, strSign)) { delete pBuffer; return NULL; }
		if(!SerialWriteInt(pBuffer, nMaxLen, nBufLen, nEncrypt)) { delete pBuffer; return NULL; }
		if(!SerialWriteInt(pBuffer, nMaxLen, nBufLen, nPriority)) { delete pBuffer; return NULL; }
		if(!SerialWriteString(pBuffer, nMaxLen, nBufLen, szMsgId)) { delete pBuffer; return NULL; }
		if(!SerialWriteString(pBuffer, nMaxLen, nBufLen, szFrom)) { delete pBuffer; return NULL; }
		if(!SerialWriteString(pBuffer, nMaxLen, nBufLen, szDest)) { delete pBuffer; return NULL; }
		if(!SerialWriteString(pBuffer, nMaxLen, nBufLen, szUserFrom)) { delete pBuffer; return NULL; }
		if(!SerialWriteString(pBuffer, nMaxLen, nBufLen, szUserTo)) { delete pBuffer; return NULL; }
		if(!SerialWriteInt(pBuffer, nMaxLen, nBufLen, nCmdCode)) { delete pBuffer; return NULL; }
		if(!SerialWriteInt(pBuffer, nMaxLen, nBufLen, nHandle)) { delete pBuffer; return NULL; }
		if(!SerialWriteInt(pBuffer, nMaxLen, nBufLen, nMsgType)) { delete pBuffer; return NULL; }
		if(!SerialWriteInt(pBuffer, nMaxLen, nBufLen, nRequest)) { delete pBuffer; return NULL; }
		if(!SerialWriteInt(pBuffer, nMaxLen, nBufLen, nSync)) { delete pBuffer; return NULL; }
		if(!SerialWriteInt(pBuffer, nMaxLen, nBufLen, nTimeOut)) { delete pBuffer; return NULL; }
		if(!SerialWriteInt(pBuffer, nMaxLen, nBufLen, nResult)) { delete pBuffer; return NULL; }
		if(!SerialWriteInt(pBuffer, nMaxLen, nBufLen, nDataType)) { delete pBuffer; return NULL; }
		if(!SerialWriteData(pBuffer, nMaxLen, nBufLen)) { delete pBuffer; return NULL; }
		if(!SerialWriteString(pBuffer, nMaxLen, nBufLen, szContent)) { delete pBuffer; return NULL; }

		return pBufStart;
	}

	BOOL SerialReadInt(BYTE*& pBuffer, int& nBufLen, int& nLen, int& nValue)
	{
		if((nLen + (int)sizeof(int)) > nBufLen)
		{
			return FALSE;
		}

		nValue = *((int*)pBuffer);
		pBuffer += sizeof(int);
		nLen += sizeof(int);

		return TRUE;
	}

	BOOL SerialWriteInt(BYTE*& pBuffer, int& nBufLen, int& nLen, int& nValue)
	{
		if((nLen + (int)sizeof(int)) > nBufLen)
		{
			return FALSE;
		}

		*((int*)pBuffer) = nValue;
		pBuffer += sizeof(int);
		nLen += sizeof(int);

		return TRUE;
	}

	BOOL SerialReadString(BYTE*& pBuffer, int& nBufLen, int& nLen, CString& strValue)
	{
		if((nLen + (int)sizeof(int)) > nBufLen)
		{
			return FALSE;
		}

		int nStrLen = *((int*)pBuffer);
		pBuffer += sizeof(int);
		nLen += sizeof(int);
		
		if((nLen + nStrLen) > nBufLen)
		{
			return FALSE;
		}

		if(nStrLen > 0)
		{
			char* szBuf = new char[nStrLen+1];
			memset(szBuf, 0, nStrLen+1);
			memcpy(szBuf, pBuffer, nStrLen);
			strValue = szBuf;
			delete szBuf;
			pBuffer += nStrLen;
			nLen += nStrLen;
		}

		return TRUE;
	}

	BOOL SerialWriteString(BYTE*& pBuffer, int& nBufLen, int& nLen, CString& strValue)
	{
		if((nLen + (int)sizeof(int)) > nBufLen)
		{
			return FALSE;
		}

		int nStrLen = strValue.GetLength();
		*((int*)pBuffer) = nStrLen;
		pBuffer += sizeof(int);
		nLen += sizeof(int);
		
		if((nLen + nStrLen) > nBufLen)
		{
			return FALSE;
		}

		if(nStrLen > 0)
		{
			memset(pBuffer, 0, nStrLen);
			memcpy(pBuffer, strValue.GetBuffer(0), nStrLen);
			strValue.ReleaseBuffer();
			pBuffer += nStrLen;
			nLen += nStrLen;
		}

		return TRUE;
	}

	BOOL SerialReadData(BYTE*& pBuffer, int& nBufLen, int& nLen)
	{
		if((nLen + (int)sizeof(int)) > nBufLen)
		{
			return FALSE;
		}

		nDataLen = *((int*)pBuffer);
		pBuffer += sizeof(int);
		nLen += sizeof(int);

		if((nLen + nDataLen) > nBufLen)
		{
			return FALSE;
		}

		if(pData != NULL)
		{
			delete pData;
			pData = NULL;
		}

		if(nDataLen > 0)
		{
			pData = new BYTE[nDataLen];
			memcpy(pData, pBuffer, nDataLen);
			pBuffer += nDataLen;
			nLen += nDataLen;
		}

		return TRUE;
	}

	BOOL SerialWriteData(BYTE*& pBuffer, int& nBufLen, int& nLen)
	{
		if((nLen + (int)sizeof(int)) > nBufLen)
		{
			return FALSE;
		}

		*((int*)pBuffer) = nDataLen;
		pBuffer += sizeof(int);
		nLen += sizeof(int);

		if((nLen + nDataLen) > nBufLen)
		{
			return FALSE;
		}

		if((nDataLen > 0) && (pData != NULL))
		{
			memcpy(pBuffer, pData, nDataLen);
			pBuffer += nDataLen;
			nLen += nDataLen;
		}

		return TRUE;
	}
/*
	virtual void Serialize(CArchive& ar)
	{
		if (ar.IsStoring())
		{
			ar << nEncrypt;
			ar << szMsgId;
			ar << szFrom;
			ar << szDest;
			ar << szUserFrom;
			ar << szUserTo;
			ar << nCmdCode;
			ar << nHandle;
			ar << nMsgType;
			ar << nRequest;
			ar << nSync;
			ar << nResult;
			ar << nDataType;
			ar << nDataLen;
			for(int i=0; i<nDataLen; i++)
				ar << (BYTE)pData[i];
			ar << szContent;
		}
		else
		{
			ar >> nEncrypt;
			ar >> szMsgId;
			ar >> szFrom;
			ar >> szDest;
			ar >> szUserFrom;
			ar >> szUserTo;
			ar >> nCmdCode;
			ar >> nHandle;
			ar >> nMsgType;
			ar >> nRequest;
			ar >> nSync;
			ar >> nResult;
			ar >> nDataType;
			ar >> nDataLen;
			if(pData != NULL)
				delete pData;
			pData = NULL;
			if(nDataLen > 0)
			{
				pData = new BYTE[nDataLen];
				for(int i=0; i<nDataLen; i++)
					ar >> pData[i];
			}
		}
	}

#ifdef _DEBUG
	virtual void AssertValid() const
	{
		CObject::AssertValid();
	}
	virtual void Dump(CDumpContext& dc) const
	{
		CObject::Dump(dc);
	}
#endif*/

#ifdef _DEBUG
	virtual CString DumpInfo()
	{
		CString strDump;
		strDump.Format("CVciMessage:\r\n MsgId=%s, From=%s, Dest=%s, UserFrom=%s, UserTo=%s\r\n CmdCode=%d, Handle=%d, MsgType=%d, Request=%d, Sync=%d\r\n Result=%d, DataType=%d\r\n Data=%s\r\n",
				szMsgId, szFrom, szDest, szUserFrom, szUserTo, nCmdCode, nHandle, nMsgType, nRequest, nSync, nResult, nDataType, szContent);
		return strDump;
	}
#endif
};

// 获取返回消息的指针
#define GET_RESPONSE_MSG(msg_rep)	\
	CVciMessage msg_rep;	\
	CVciMessage* p##msg_rep;	\
	if(ppOut != NULL)	\
	{	\
		p##msg_rep = ppOut;	\
	}else	\
	{	\
		p##msg_rep = &msg_rep;	\
		msg_rep.SetSync(VMT_ASYNC);	\
	}	\
	p##msg_rep->MakeResponceMessage(pIn);	\


// 发送返回消息
#define SEND_RESPONSE_MSG(msg_rep)	\
	if(ppOut == NULL)	\
	{	\
		SendMessage(&msg_rep, NULL);	\
	}	\


// 消息过滤类定义
class CMsgFilter : public CVciMessage
{
// attribute
public:
	CString	szName;			// 过滤器名字
	CString	szFilterDest;	// 过滤器目的组件

// Operation
public:
	CMsgFilter()
	{
		CVciMessage();
		szName = _T("");
		szFilterDest = _T("");
	}

	CMsgFilter(const CMsgFilter& other)
	{
		szName		= other.szName;
		szDest		= other.szDest;
		szFilterDest= other.szFilterDest;
	}

	CMsgFilter& operator = (const CMsgFilter& other)
	{
		szName		= other.szName;
		szDest		= other.szDest;
		szFilterDest= other.szFilterDest;
		return *this;
	}

	CString GetName() { return szName; }
	void SetName(LPCTSTR lpszName) { szName = lpszName; }

	CString GetFilterDest() { return szFilterDest; }
	void SetFilterDest(LPCTSTR lpszFilterDest) { szFilterDest = lpszFilterDest; }
};

typedef CArray<CMsgFilter, CMsgFilter&> CMsgFilterArray;

#endif // !defined(__VCIMESSAGE_H_)