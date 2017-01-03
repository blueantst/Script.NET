////////////////////////////////////////////////////////////////////////////
//	File:		TclXml.cpp
//	Version:	1.0.0.0
//	Created:	2003-04-20
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	implementation of use Microsoft XML API in TCL script.
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CTclInterp.h"
#include "XMLDom.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// TclXml文件对象列表
static CPtrList	g_TclXmlFileList;	// XML文件对象列表

/////////////////////////////////////////////////////////////////////////////
// 关闭所有TclXml打开的Xml文件
/////////////////////////////////////////////////////////////////////////////
void CloseAllTclXmlFile()
{
	int count = g_TclXmlFileList.GetCount();
	for(int i = count-1; i >= 0; i --)
	{
		POSITION pos = g_TclXmlFileList.FindIndex(i);
		CXmlParser* pTclXmlFile = (CXmlParser*)(g_TclXmlFileList.GetAt(pos));
		if(pTclXmlFile != NULL)
		{
			delete pTclXmlFile;			
		}
		g_TclXmlFileList.RemoveAt(pos);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 关闭指定的TclXml文件对象
/////////////////////////////////////////////////////////////////////////////
BOOL CloseTclXmlFile(void* pTclXmlFile)
{
	if(pTclXmlFile == NULL)
		return FALSE;

	int count = g_TclXmlFileList.GetCount();
	for(int i = count-1; i >= 0; i --)
	{
		POSITION pos = g_TclXmlFileList.FindIndex(i);
		CXmlParser* pXmlFile = (CXmlParser*)(g_TclXmlFileList.GetAt(pos));
		if(pTclXmlFile == pXmlFile) 
		{
			g_TclXmlFileList.RemoveAt(pos);
			delete pXmlFile;
			return TRUE;
		}		
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 判断指定的Xml文件对象是否存在
/////////////////////////////////////////////////////////////////////////////
BOOL IsXmlFileExist(void* pTclXmlFile)
{
	int count = g_TclXmlFileList.GetCount();
	for(int i = 0; i < count; i ++)
	{
		POSITION pos = g_TclXmlFileList.FindIndex(i);
		void* pXmlFile = (void*)g_TclXmlFileList.GetAt(pos);
		if(pTclXmlFile == pXmlFile) 
		{
			return TRUE;
		}
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 用于XML调用的TCL扩展命令

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:打开一个XML文件或流
// 用  法: xml::open file name ?encoding?
//		   xml::open stream string ?encoding?
// 返回值: 对象句柄
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_XmlOpen(ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc < 2)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "file|stream ...");
		return TCL_ERROR;
	}

	// 获取参数
	CString strOpObj = ConvertUTFParam(Tcl_GetString(objv[1]));	// 操作对象
	if(strOpObj == "file")
	{
		if(objc < 3)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "file name");
			return TCL_ERROR;
		}

		CString strName = ConvertUTFParam(Tcl_GetString(objv[2]));	// 文件名

		CString strEncoding = "";
		if(objc > 3)
		{
			strEncoding = ConvertUTFParam(Tcl_GetString(objv[3]));
		}

		///////////////////catch exception/////////////////
		try{

		// 检查XML文件是否存在
		if(GetFileAttributes(strName) == 0xFFFFFFFF)
		{
			Tcl_AppendResult(interp, "Xml file ", strName, " not exist.", (char *) NULL);
			ConvertResultToUTF(interp);
			return TCL_ERROR;
		}

		// 创建XML文件对象
		CXmlParser* pXmlFile = new CXmlParser();
		pXmlFile->Open(strName, strEncoding);
		//CXmlParser* pXmlFile = NULL;
		//RCOM_CMD(0, OC_XML_OPENFILE, (WPARAM)(LPCTSTR)strName, 0, &pXmlFile);

		// 添加到文件对象列表中
		g_TclXmlFileList.AddTail(pXmlFile);

		// 返回文件对象指针
		CString strRet;
		strRet.Format("%d", pXmlFile);
		Tcl_AppendResult(interp, strRet, (char *) NULL);

		///////////////////catch exception/////////////////
		}catch(...)
		{
			Tcl_ResetResult(interp);
			Tcl_AppendResult(interp, "TclXml catch an unknown exception while open xml file.",  (char *) NULL);
			return TCL_ERROR;
		}
	}else
	if(strOpObj == "stream")
	{
		if(objc < 3)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "stream string");
			return TCL_ERROR;
		}

		CString strStream = ConvertUTFParam(Tcl_GetString(objv[2]));	// XML流

		CString strEncoding = "";
		if(objc > 3)
		{
			strEncoding = ConvertUTFParam(Tcl_GetString(objv[3]));
		}

		///////////////////catch exception/////////////////
		try{

		// 创建XML流对象
		CXmlParser* pXmlStream = new CXmlParser();
		pXmlStream->SetEncoding(strEncoding);

		if(strStream != "")
		{
			pXmlStream->ParseXMLStream(strStream);
		}

		// 添加到文件对象列表中
		g_TclXmlFileList.AddTail(pXmlStream);

		// 返回流对象指针
		CString strRet;
		strRet.Format("%d", pXmlStream);
		Tcl_AppendResult(interp, strRet, (char *) NULL);

		///////////////////catch exception/////////////////
		}catch(...)
		{
			Tcl_ResetResult(interp);
			Tcl_AppendResult(interp, "TclXml catch an unknown exception while open xml stream.",  (char *) NULL);
			return TCL_ERROR;
		}
	}else
	{
		Tcl_AppendResult(interp, "operater must be file or stream.", (char *) NULL);
		return TCL_ERROR;
	}

	ConvertResultToUTF(interp);
	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:写一个XML文件或流
// 用  法: xml::write handle file name
//		   xml::write handle stream
// 返回值: 对象句柄或流内容
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_XmlWrite(ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc < 3)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "handle file|stream ...");
		return TCL_ERROR;
	}

	// 获取参数
	CString strHandle = ConvertUTFParam(Tcl_GetString(objv[1]));	// 操作对象

	// 检查XML文件对象是否存在
	CXmlParser* pXmlFile = (CXmlParser*)(atoi(strHandle));
	if(!IsXmlFileExist(pXmlFile))
	{
		Tcl_AppendResult(interp, "Xml file handle ", strHandle, " not exist.", (char *) NULL);
		return TCL_ERROR;
	}

	CString strOpObj = ConvertUTFParam(Tcl_GetString(objv[2]));	// 操作对象
	if(strOpObj == "file")
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "handle file name");
			return TCL_ERROR;
		}

		CString strName = ConvertUTFParam(Tcl_GetString(objv[3]));	// 文件名

		///////////////////catch exception/////////////////
		try{

		if(pXmlFile->Save(strName))
		{
			// 返回true表示保存失败
			Tcl_AppendResult(interp, "Save xml file ", strName, " failed.", (char *) NULL);
			ConvertResultToUTF(interp);
			return TCL_ERROR;
		}

		Tcl_AppendResult(interp, "", (char *) NULL);

		///////////////////catch exception/////////////////
		}catch(...)
		{
			Tcl_ResetResult(interp);
			Tcl_AppendResult(interp, "TclXml catch an unknown exception while write xml.",  (char *) NULL);
			return TCL_ERROR;
		}
	}else
	if(strOpObj == "stream")
	{
		///////////////////catch exception/////////////////
		try{

		CString strStream = pXmlFile->WriteToStr();
		Tcl_AppendResult(interp, strStream, (char *) NULL);

		///////////////////catch exception/////////////////
		}catch(...)
		{
			Tcl_ResetResult(interp);
			Tcl_AppendResult(interp, "TclXml catch an unknown exception while write stream.",  (char *) NULL);
			return TCL_ERROR;
		}
	}else
	{
		Tcl_AppendResult(interp, "operater must be file or stream.", (char *) NULL);
		return TCL_ERROR;
	}

	ConvertResultToUTF(interp);
	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:关闭一个XML文件对象
// 用  法: xml::close handle
// 返回值: 是否成功
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_XmlClose(ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc < 2)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "handle");
		return TCL_ERROR;
	}

	// 获取参数
	CString strHandle = ConvertUTFParam(Tcl_GetString(objv[1]));

	///////////////////catch exception/////////////////
	try{

	if(!CloseTclXmlFile((void*)(atoi(strHandle))))
	{
		Tcl_AppendResult(interp, "Close handle ", strHandle, " failed.", (char *) NULL);
		return TCL_ERROR;
	}

	///////////////////catch exception/////////////////
	}catch(...)
	{
		Tcl_ResetResult(interp);
		Tcl_AppendResult(interp, "TclXml catch an unknown exception while close handle.",  (char *) NULL);
		return TCL_ERROR;
	}

	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:获取节点信息
// 用  法:
//			xml::get handle rootNode
//			xml::get handle node path
//			xml::get handle childNode node ?name?
//			xmm::get handle childNodes node ?name?
//			xml::get handle nextNode node ?name?
//			xml::get handle nodeName node
//			xml::get handle nodeText -node node ?-cdata?
//			xml::get handle nodeText -path nodePath ?-cdata?
//			xml::get handle nodeAttr -node node attr
//			xml::get handle nodeAttr -path nodePath attr
//			xml::get handle nodeAttrNames node
// 返回值: 对象句柄
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_XmlGetNode(ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc < 3)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "handle option ...");
		return TCL_ERROR;
	}

	CXmlParser* pXmlFile = (CXmlParser*)(atoi(ConvertUTFParam(Tcl_GetString(objv[1]))));	// XML文件对象句柄
	if(!IsXmlFileExist(pXmlFile))
	{
		Tcl_AppendResult(interp, "Invalid handle when xml_get.", (char *) NULL);
		return TCL_ERROR;
	}

	CString strRet;

	// 获取参数
	CString strOp = ConvertUTFParam(Tcl_GetString(objv[2]));	// 操作对象
	if(strOp == "rootNode")	// 获取根节点
	{
		strRet.Format("%d", pXmlFile->GetRootNode());
		Tcl_AppendResult(interp, strRet, (char *) NULL);
	}else
	if(strOp == "node")	// 获取指定节点
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "handle node path");
			return TCL_ERROR;
		}

		CString strNodePath = ConvertUTFParam(Tcl_GetString(objv[3]));

		strRet.Format("%d", pXmlFile->GetNode(strNodePath));
		Tcl_AppendResult(interp, strRet, (char *) NULL);
	}else
	if(strOp == "childNode")	// 获取第一个子节点
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "handle childNode node ?name?");
			return TCL_ERROR;
		}

		DOMNode* pNode = (DOMNode*)(atoi(ConvertUTFParam(Tcl_GetString(objv[3]))));

		CString strName = "";
		if(objc > 4)
		{
			strName = ConvertUTFParam(Tcl_GetString(objv[4]));
		}

		if(strName.GetLength() > 0)
		{
			strRet.Format("%d", pXmlFile->GetChildNode(pNode, strName));
		}else
		{
			strRet.Format("%d", pXmlFile->GetChildNode(pNode));
		}
		
		Tcl_AppendResult(interp, strRet, (char *) NULL);
	}else
	if(strOp == "childNodes")	// 获取子节点列表
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "handle childNodes node ?name?");
			return TCL_ERROR;
		}

		CString strNode = ConvertUTFParam(Tcl_GetString(objv[3]));
		DOMNode* pNode = NULL;
		if(strNode == "root")
		{
			pNode = pXmlFile->GetRootNode();
		}else
		{
			pNode = (DOMNode*)(atoi(strNode));
		}

		CString strName = "";
		if(objc > 4)
		{
			strName = ConvertUTFParam(Tcl_GetString(objv[4]));
		}

		DOMNode* pChildNode = NULL;
		if(strName.GetLength() > 0)
		{
			pChildNode = pXmlFile->GetChildNode(pNode, strName);
		}else
		{
			pChildNode = pXmlFile->GetChildNode(pNode);
		}

		while(pChildNode)
		{
			CString strChildNode;
			strChildNode.Format("%d", pChildNode);
			Tcl_AppendElement(interp, strChildNode);
			if(strName.GetLength() > 0)
			{
				pChildNode = pXmlFile->GetNextNode(pChildNode, strName);
			}else
			{
				pChildNode = pXmlFile->GetNextNode(pChildNode);
			}
		}
	}else
	if(strOp == "nextNode")	// 获取下一个节点
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "handle nextNode node ?name?");
			return TCL_ERROR;
		}

		DOMNode* pNode = (DOMNode*)(atoi(ConvertUTFParam(Tcl_GetString(objv[3]))));

		CString strName = "";
		if(objc > 4)
		{
			strName = ConvertUTFParam(Tcl_GetString(objv[4]));
		}

		if(strName.GetLength() > 0)
		{
			strRet.Format("%d", pXmlFile->GetNextNode(pNode, strName));
		}else
		{
			strRet.Format("%d", pXmlFile->GetNextNode(pNode));
		}
		
		Tcl_AppendResult(interp, strRet, (char *) NULL);
	}else
	if(strOp == "nodeName")	// 获取节点名
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "handle nodeName node");
			return TCL_ERROR;
		}

		DOMNode* pNode = (DOMNode*)(atoi(ConvertUTFParam(Tcl_GetString(objv[3]))));

		strRet = pXmlFile->GetNodeName(pNode);
		Tcl_AppendResult(interp, strRet, (char *) NULL);
	}else
	if(strOp == "nodeText")	// 获取节点内容
	{
		if(objc < 5)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "handle nodeText -node|path node|path ?-cdata?");
			return TCL_ERROR;
		}

		CString strSubOp = ConvertUTFParam(Tcl_GetString(objv[3]));

		int nType = XMLNODE_TEXT;
		if(objc > 5)
		{
			nType = (ConvertUTFParam(Tcl_GetString(objv[5])) == "-cdata") ? XMLNODE_CDATA : XMLNODE_TEXT;
		}

		if(strSubOp == "-node")
		{
			DOMNode* pNode = (DOMNode*)(atoi(ConvertUTFParam(Tcl_GetString(objv[4]))));
			strRet = pXmlFile->GetNodeText(pNode, nType);
		}else
		if(strSubOp == "-path")
		{
			CString strNodePath = ConvertUTFParam(Tcl_GetString(objv[4]));
			strRet = pXmlFile->GetNodeText(strNodePath, nType);
		}

		Tcl_AppendResult(interp, strRet, (char *) NULL);
	}else
	if(strOp == "nodeAttr")	// 获取节点属性
	{
		if(objc < 6)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "handle nodeAttr -node|path node|path attr");
			return TCL_ERROR;
		}

		CString strSubOp = ConvertUTFParam(Tcl_GetString(objv[3]));
		CString strAttr = ConvertUTFParam(Tcl_GetString(objv[5]));

		if(strSubOp == "-node")
		{
			DOMNode* pNode = (DOMNode*)(atoi(ConvertUTFParam(Tcl_GetString(objv[4]))));
			strRet = pXmlFile->GetNodeAttribute(pNode, strAttr);
		}else
		if(strSubOp == "-path")
		{
			CString strNodePath = ConvertUTFParam(Tcl_GetString(objv[4]));
			strRet = pXmlFile->GetNodeAttribute(strNodePath, strAttr);
		}

		Tcl_AppendResult(interp, strRet, (char *) NULL);
	}else
	if(strOp == "nodeAttrNames")	// 获取节点属性名列表
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "handle nodeAttrNames node");
			return TCL_ERROR;
		}

		DOMNode* pNode = (DOMNode*)(atoi(ConvertUTFParam(Tcl_GetString(objv[3]))));

		CStringArray arAttrName;
		pXmlFile->GetNodeAttributeNames(pNode, arAttrName);
		for(int i=0; i<arAttrName.GetSize(); i++)
		{
			Tcl_AppendElement(interp, arAttrName[i]);
		}

	}else
	{
		Tcl_AppendResult(interp, "operater must be rootNode, node, childNode, childNodes, nextNode, nodeName, nodeText, nodeAttr or nodeAttrNames.", (char *) NULL);
		return TCL_ERROR;
	}

	ConvertResultToUTF(interp);
	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:设置节点信息
// 用  法:
//			xml::set handle nodeText -node node text
//			xml::set handle nodeText -path nodePath text
//			xml::set handle nodeAttr -node node attr value
//			xml::set handle nodeAttr -path nodePath attr value
// 返回值: 无
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_XmlSetNode(ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc < 4)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "handle option ...");
		return TCL_ERROR;
	}

	CXmlParser* pXmlFile = (CXmlParser*)(atoi(ConvertUTFParam(Tcl_GetString(objv[1]))));	// XML文件对象句柄
	if(!IsXmlFileExist(pXmlFile))
	{
		Tcl_AppendResult(interp, "Invalid handle when xml_set.", (char *) NULL);
		return TCL_ERROR;
	}

	// 获取参数
	CString strOp = ConvertUTFParam(Tcl_GetString(objv[2]));	// 操作对象

	if(strOp == "nodeText")	// 设置节点内容
	{
		if(objc < 6)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "handle nodeText -node|path node|path text");
			return TCL_ERROR;
		}

		CString strSubOp = ConvertUTFParam(Tcl_GetString(objv[3]));
		CString strNodeText = ConvertUTFParam(Tcl_GetString(objv[5]));

		if(strSubOp == "-node")
		{
			DOMNode* pNode = (DOMNode*)(atoi(ConvertUTFParam(Tcl_GetString(objv[4]))));
			pXmlFile->SetNodeText(pNode, strNodeText);
		}else
		if(strSubOp == "-path")
		{
			CString strNodePath = ConvertUTFParam(Tcl_GetString(objv[4]));
			pXmlFile->SetNodeText(strNodePath, strNodeText);
		}

		Tcl_AppendResult(interp, "", (char *) NULL);
	}else
	if(strOp == "nodeAttr")	// 设置节点属性
	{
		if(objc < 7)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "handle nodeAttr -node|path node|path attr value");
			return TCL_ERROR;
		}

		CString strSubOp = ConvertUTFParam(Tcl_GetString(objv[3]));
		CString strAttr = ConvertUTFParam(Tcl_GetString(objv[5]));
		CString strValue = ConvertUTFParam(Tcl_GetString(objv[6]));

		if(strSubOp == "-node")
		{
			DOMNode* pNode = (DOMNode*)(atoi(ConvertUTFParam(Tcl_GetString(objv[4]))));
			pXmlFile->SetNodeAttribute(pNode, strAttr, strValue);
		}else
		if(strSubOp == "-path")
		{
			CString strNodePath = ConvertUTFParam(Tcl_GetString(objv[4]));
			pXmlFile->SetNodeAttribute(strNodePath, strAttr, strValue);
		}

		Tcl_AppendResult(interp, "", (char *) NULL);
	}else
	{
		Tcl_AppendResult(interp, "operater must be nodeText or nodeAttr.", (char *) NULL);
		return TCL_ERROR;
	}

	ConvertResultToUTF(interp);
	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:创建节点
// 用  法:
//			xml::create handle node -root node name
//			xml::create handle node -node node name
//			xml::create handle node -path path name
// 返回值: Node
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_XmlCreateNode(ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc < 4)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "handle option ...");
		return TCL_ERROR;
	}

	CXmlParser* pXmlFile = (CXmlParser*)(atoi(ConvertUTFParam(Tcl_GetString(objv[1]))));	// XML文件对象句柄
	if(!IsXmlFileExist(pXmlFile))
	{
		Tcl_AppendResult(interp, "Invalid handle when xml_create.", (char *) NULL);
		return TCL_ERROR;
	}

	// 获取参数
	CString strOp = ConvertUTFParam(Tcl_GetString(objv[2]));	// 操作对象

	if(strOp == "node")	// 创建节点
	{
		if(objc < 6)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "handle node -node|path node|path name");
			return TCL_ERROR;
		}

		CString strSubOp = ConvertUTFParam(Tcl_GetString(objv[3]));
		CString strNodeName = ConvertUTFParam(Tcl_GetString(objv[5]));

		DOMNode* pNewNode = NULL;
		if(strSubOp == "-root")
		{
			pNewNode = pXmlFile->CreateDomRoot(strNodeName);
		}else
		if(strSubOp == "-node")
		{
			DOMNode* pNode = (DOMNode*)(atoi(ConvertUTFParam(Tcl_GetString(objv[4]))));
			pNewNode = pXmlFile->CreateChildNode(pNode, strNodeName);
		}else
		if(strSubOp == "-path")
		{
			CString strNodePath = ConvertUTFParam(Tcl_GetString(objv[4]));
			DOMNode* pNode = pXmlFile->GetNode(strNodePath);
			if(pNode)
			{
				pNewNode = pXmlFile->CreateChildNode(pNode, strNodeName);
			}
		}

		CString strTmp;
		strTmp.Format("%d", pNewNode);
		Tcl_AppendResult(interp, strTmp, (char *) NULL);
	}else
	{
		Tcl_AppendResult(interp, "operater must be node.", (char *) NULL);
		return TCL_ERROR;
	}

	ConvertResultToUTF(interp);
	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 扩展命令:删除节点
// 用  法:
//			xml::remove handle node -node node childnode
// 返回值: Node
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_XmlRemoveNode(ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc < 4)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "handle option ...");
		return TCL_ERROR;
	}

	CXmlParser* pXmlFile = (CXmlParser*)(atoi(ConvertUTFParam(Tcl_GetString(objv[1]))));	// XML文件对象句柄
	if(!IsXmlFileExist(pXmlFile))
	{
		Tcl_AppendResult(interp, "Invalid handle when xml_remove.", (char *) NULL);
		return TCL_ERROR;
	}

	// 获取参数
	CString strOp = ConvertUTFParam(Tcl_GetString(objv[2]));	// 操作对象

	if(strOp == "node")	// 删除节点
	{
		if(objc < 6)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "handle node -node|path node|path childnode|childname");
			return TCL_ERROR;
		}

		CString strSubOp = ConvertUTFParam(Tcl_GetString(objv[3]));
		CString strChildNode = ConvertUTFParam(Tcl_GetString(objv[5]));

		DOMNode* pNewNode = NULL;
		if(strSubOp == "-node")
		{
			DOMNode* pNode = (DOMNode*)(atoi(ConvertUTFParam(Tcl_GetString(objv[4]))));
			pXmlFile->RemoveChildNode(pNode, (DOMNode*)(atoi(strChildNode)));
		}else
		if(strSubOp == "-path")
		{
			CString strNodePath = ConvertUTFParam(Tcl_GetString(objv[4]));
			DOMNode* pNode = pXmlFile->GetNode(strNodePath);
			if(pNode)
			{
				DOMNode* pChildNode = pXmlFile->GetChildNode(pNode, strChildNode);
				if(pChildNode)
				{
					pXmlFile->RemoveChildNode(pNode, pChildNode);
				}
			}
		}

		Tcl_AppendResult(interp, "", (char *) NULL);
	}else
	{
		Tcl_AppendResult(interp, "operater must be node.", (char *) NULL);
		return TCL_ERROR;
	}

	ConvertResultToUTF(interp);
	return TCL_OK;
}

//TCL命令声明函数
int TclXml_Init(Tcl_Interp *interp)
{
	// 注册PCOM调用类命令
	// 打开一个XML文件
	Tcl_CreateObjCommand(interp, "xml_open", Tcl_Cmd_XmlOpen, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 保存一个XML文件
	Tcl_CreateObjCommand(interp, "xml_write", Tcl_Cmd_XmlWrite, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 关闭一个XML文件对象
	Tcl_CreateObjCommand(interp, "xml_close", Tcl_Cmd_XmlClose, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 获取节点信息
	Tcl_CreateObjCommand(interp, "xml_get", Tcl_Cmd_XmlGetNode, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 设置节点信息
	Tcl_CreateObjCommand(interp, "xml_set", Tcl_Cmd_XmlSetNode, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 创建节点
	Tcl_CreateObjCommand(interp, "xml_create", Tcl_Cmd_XmlCreateNode, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// 删除节点
	Tcl_CreateObjCommand(interp, "xml_remove", Tcl_Cmd_XmlRemoveNode, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

    return TCL_OK;
}
