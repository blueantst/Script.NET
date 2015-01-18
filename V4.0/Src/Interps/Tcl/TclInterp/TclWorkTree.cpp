////////////////////////////////////////////////////////////////////////////
//	File:		TclCmd.cpp
//	Version:	1.0.0.0
//	Created:	2000-01-06
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	Implementation of TCL extern command.
////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CTclInterp.h"
#include "WorkTree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


////////////////////////////////////////////////////////////////////////////
// COM Message define

// 调用OWM_WORKTREE功能
#define WORKTREE_CMD(cmd, w, l)	\
	{	\
		CTclInterp* pTclInterp = GetTclInterp((LPVOID)interp);	\
		if(pTclInterp)	\
		{	\
			IPlatUI* pIPlatUI = pTclInterp->pIPlatUI;	\
			if(pIPlatUI)	\
			{	\
				pIPlatUI->SendOwmCmd("org.owm.workspace", cmd, (WPARAM)w, (LPARAM)l, NULL);	\
			}	\
		}	\
	}	\

// 调用OWM_WORKTREE功能(带返回值的)
#define RWORKTREE_CMD(cmd, w, l, r)	\
	{	\
		CTclInterp* pTclInterp = GetTclInterp((LPVOID)interp);	\
		if(pTclInterp)	\
		{	\
			IPlatUI* pIPlatUI = pTclInterp->pIPlatUI;	\
			if(pIPlatUI)	\
			{	\
				pIPlatUI->SendOwmCmd("org.owm.workspace", cmd, (WPARAM)w, (LPARAM)l, r);	\
			}	\
		}	\
	}	\


/////////////////////////////////////////////////////////////////////////////
// 扩展命令:WorkSpace命令
// WorkTree addtree TreeType TreeName ?IconFile? ?BmpFile? ?AttachPaneId? ?PaneId?
// WorkTree closetree -TreeID|TreeName
// WorkTree settreetype TreeID TreeType
// WorkTree settreeimage TreeID BmpFile
// WorkTree gettreeid TreeName
// WorkTree activetree -TreeID|TreeName
// WorkTree additem TreeID hParent ItemName ?-param(value)?
// WorkTree delitem TreeID id ?-TIM_ID|TIM_ITEM? ?-TIM_CHILDONLY?
// WorkTree getitem info TreeID id ?-TIM_ID|TIM_ITEM?
// WorkTree setitem TreeID id ?-TIM_ID|TIM_ITEM? ?-param(value)?
// WorkTree getnextitem TreeID id code ?-TIM_ID|TIM_ITEM?
// WorkTree searchitem TreeID info
// WorkTree expanditem TreeID id ?-TIM_ID|TIM_ITEM?
// WorkTree collapseitem TreeID id ?-TIM_ID|TIM_ITEM?
// WorkTree sortchildren TreeID id ?-TIM_ID|TIM_ITEM?
// WorkTree setdclickscript TreeID script
// WorkTree addmenuitem TreeID type string command ?menuid?
//
// WorkSpace settasktheme TreeID theme
// WorkSpace addtaskitem TreeID caption command ?-param(value)?
// WorkSpace deltaskitem TreeID ?-param(value)?
// WorkSpace settaskicon TreeID ico|bmpfile
/////////////////////////////////////////////////////////////////////////////
int Tcl_Cmd_WorkSpace(ClientData dummy, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
	if(objc < 2)
	{
		Tcl_WrongNumArgs(interp, 1, objv, "operate ...");
		return TCL_ERROR;
	}

	// 获取参数
	CString strOpr = ConvertUTFParam(Tcl_GetString(objv[1]));

	if(strOpr == "addtree")	// 添加树
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "addtree TreeType TreeName ?IconFile? ?BmpFile? ?AttachPaneId? ?PaneId?");
			return TCL_ERROR;
		}
		CString strTreeType = ConvertUTFParam(Tcl_GetString(objv[2]));
		CString strTreeName = ConvertUTFParam(Tcl_GetString(objv[3]));
		int nTreeType = atoi(strTreeType);//PGI_FILE;
		CString strIconFile = "";
		CString strBmpFile = "";
		int nAttachPaneId = 0;
		int nPaneId = 0;
		if(objc > 4)
		{
			strIconFile = ConvertUTFParam(Tcl_GetString(objv[4]));
		}
		if(objc > 5)
		{
			strBmpFile = ConvertUTFParam(Tcl_GetString(objv[5]));
		}
		if(objc > 6)
		{
			nAttachPaneId = atoi(ConvertUTFParam(Tcl_GetString(objv[6])));
		}
		if(objc > 7)
		{
			nPaneId = atoi(ConvertUTFParam(Tcl_GetString(objv[7])));
		}

		STRU_ADDTREEEX struTree;
		struTree.strTreeName = strTreeName;
		struTree.nTreeType = nTreeType;
		struTree.strIconFile = strIconFile;
		struTree.nAttachPaneId = nAttachPaneId;
		struTree.strBmpFile = strBmpFile;
		struTree.nPaneId = nPaneId;

		int nTreeID;
		//RWORKTREE_CMD(OC_WS_ADDTREE, (WPARAM)(LPCTSTR)strTreeName, nTreeType, &nTreeID);
		RWORKTREE_CMD(OC_WS_ADDTREEEX, (WPARAM)(&struTree), 0, &nTreeID);

		CString strTreeID;
		strTreeID.Format("%d", nTreeID);
		Tcl_AppendResult(interp, strTreeID, (char *) NULL);
		// 因为创建可能需要一段时间(等消息调度完成),因此等待一段时间,避免立即操作会不成功
		Sleep(100);
	}else
	if(strOpr == "closetree")	// 关闭树
	{
		if(objc < 3)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "closetree -TreeID|TreeName");
			return TCL_ERROR;
		}
		CString strTree = ConvertUTFParam(Tcl_GetString(objv[2]));
		int nTreeID = -1;
		if(strTree.Find("-") == 0)
		{	// 页面ID
			strTree.Delete(0, 1);
			nTreeID = atoi(strTree);
		}else
		{	// 页面名字
			RWORKTREE_CMD(OC_WS_GETTREEINDEX, (WPARAM)(LPCTSTR)strTree, 0, &nTreeID);
			if(nTreeID == 0)
			{
				Tcl_AppendResult(interp, "Tree name '", strTree, "' not exist.", (char *) NULL);
				//return TCL_ERROR;
				return TCL_OK;
			}
		}

		WORKTREE_CMD(OC_WS_CLOSETREE, nTreeID, 0);
	}else
	if(strOpr == "settreetype")	// 设置窗口类型
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "settreetype TreeID TreeType");
			return TCL_ERROR;
		}

		// 必须输入的参数
		CString strTreeID = ConvertUTFParam(Tcl_GetString(objv[2]));
		int nTreeID = atoi(strTreeID);
		CString strTreeType = ConvertUTFParam(Tcl_GetString(objv[3]));
		int nTreeType = atoi(strTreeType);

		WORKTREE_CMD(OC_WS_SETTREETYPE, nTreeID, nTreeType);
	}else
	if(strOpr == "settreeimage")	// 设置树的图标组
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "settreeimage TreeID BmpFile");
			return TCL_ERROR;
		}

		// 必须输入的参数
		CString strTreeID = ConvertUTFParam(Tcl_GetString(objv[2]));
		int nTreeID = atoi(strTreeID);
		CString strBmpFile = ConvertUTFParam(Tcl_GetString(objv[3]));

		WORKTREE_CMD(OC_WS_SETTREEIMAGE, nTreeID, (LPARAM)(LPCTSTR)strBmpFile);
	}else
	if(strOpr == "treeexist")	// 判断指定ID的树窗口是否已经存在
	{
		if(objc < 3)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "treeexist TreeId");
			return TCL_ERROR;
		}
		int nTreeId = atoi(ConvertUTFParam(Tcl_GetString(objv[2])));
		int nTreeExist = 0;
		RWORKTREE_CMD(OC_WS_TREEEXIST, (WPARAM)nTreeId, 0, &nTreeExist);

		CString strTreeExist;
		strTreeExist.Format("%d", nTreeExist);
		Tcl_AppendResult(interp, strTreeExist, (char *) NULL);
	}else
	if(strOpr == "gettreeid")	// 根据树名字获取ID
	{
		if(objc < 3)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "gettreeid TreeName");
			return TCL_ERROR;
		}
		CString strTreeName = ConvertUTFParam(Tcl_GetString(objv[2]));
		int nTreeID = -1;
		RWORKTREE_CMD(OC_WS_GETTREEINDEX, (WPARAM)(LPCTSTR)strTreeName, 0, &nTreeID);

		CString strTreeID;
		strTreeID.Format("%d", nTreeID);
		Tcl_AppendResult(interp, strTreeID, (char *) NULL);
	}else
	if(strOpr == "activetree")	// 激活树
	{
		if(objc < 3)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "activetree -TreeID|TreeName");
			return TCL_ERROR;
		}
		CString strTree = ConvertUTFParam(Tcl_GetString(objv[2]));
		int nTreeID = -1;
		if(strTree.Find("-") == 0)
		{	// 页面ID
			strTree.Delete(0, 1);
			nTreeID = atoi(strTree);
		}else
		{	// 页面名字
			RWORKTREE_CMD(OC_WS_GETTREEINDEX, (WPARAM)(LPCTSTR)strTree, 0, &nTreeID);
			if(nTreeID == 0)
			{
				Tcl_AppendResult(interp, "Tree name '", strTree, "' not exist.", (char *) NULL);
				return TCL_ERROR;
			}
		}

		WORKTREE_CMD(OC_WS_SETACTIVETREE, nTreeID, 0);
	}else
	if(strOpr == "additem")	// 添加树节点
	{
		if(objc < 5)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "additem TreeID hParent ItemName ?-param(value)?");
			return TCL_ERROR;
		}
		// 必须输入的参数
		CString strTreeID = ConvertUTFParam(Tcl_GetString(objv[2]));
		int nTreeID = atoi(strTreeID);
		CString strhParent = ConvertUTFParam(Tcl_GetString(objv[3]));
		int hParent = NULL;
		if(strhParent != "ROOT")
			hParent = atoi(strhParent);
		CString strItemName = ConvertUTFParam(Tcl_GetString(objv[4]));

		// 其它可选参数
		DWORD dwMask = TIM_PARENT;	// 缺省为使用父节点句柄
		int nID = 0;
		BYTE byType = 0;
		BYTE byState = WTS_NORMAL;
		BYTE byImage = 0;
		CString strInfo = "";
		COLORREF crColor = RGB(0,0,0);
		int nMenuIndex = 0;
		for(int i=5; i<objc; i++)
		{
			CString strParam = ConvertUTFParam(Tcl_GetString(objv[i]));
			if(strParam == "-TIM_ID")	// 使用父节点ID
			{
				dwMask &= ~TIM_PARENT;
				dwMask |= TIM_PARENTID;
			}else
			if(strParam == "-TIM_ITEM")	// 使用父节点句柄
			{
				dwMask &= ~TIM_PARENTID;
				dwMask |= TIM_PARENT;
			}else
			if((strParam.Find("-id(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 4);
				strParam.Delete(strParam.GetLength()-1, 1);
				nID = atoi(strParam);
				dwMask |= TIM_ID;
			}else
			if((strParam.Find("-type(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 6);
				strParam.Delete(strParam.GetLength()-1, 1);
				byType = atoi(strParam);
				dwMask |= TIM_TYPE;
			}else
			if((strParam.Find("-state(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 7);
				strParam.Delete(strParam.GetLength()-1, 1);
				byState = atoi(strParam);
				dwMask |= TIM_STATE;
			}else
			if((strParam.Find("-image(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 7);
				strParam.Delete(strParam.GetLength()-1, 1);
				byImage = atoi(strParam);
				dwMask |= TIM_IMAGE;
			}else
			if((strParam.Find("-info(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 6);
				strParam.Delete(strParam.GetLength()-1, 1);
				strInfo = strParam;
				dwMask |= TIM_INFO;
			}else
			if((strParam.Find("-color(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 7);
				strParam.Delete(strParam.GetLength()-1, 1);
				int r = 0, g=0, b=0;
				int nPos = strParam.Find(",");
				if(nPos > 0)
				{
					r = atoi(strParam.Left(nPos));
					strParam.Delete(0, nPos+1);
					nPos = strParam.Find(",");
					if(nPos > 0)
					{
						g = atoi(strParam.Left(nPos));
						strParam.Delete(0, nPos+1);
						b = atoi(strParam);
					}
				}
				crColor = RGB(r, g, b);
				dwMask |= TIM_COLOR;
			}else
			if((strParam.Find("-bold(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 6);
				strParam.Delete(strParam.GetLength()-1, 1);
				if(strParam == "true")
				{
					dwMask |= TIM_BOLD;
				}
			}else
			if((strParam.Find("-font(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				// 暂不支持
				strParam.Delete(0, 6);
				strParam.Delete(strParam.GetLength()-1, 1);
				//dwMask |= TIM_FONT;
			}else
			if((strParam.Find("-menu(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 6);
				strParam.Delete(strParam.GetLength()-1, 1);
				nMenuIndex = atoi(strParam);
				dwMask |= TIM_MENU;
			}
		}

		HWORKTREEITEM item;
		item.dwMask		= dwMask;
		item.byState	= byState;
		item.byType		= byType;
		item.hParent	= (HTREEITEM)hParent;
		item.nParentID	= hParent;
		item.nID		= nID;
		item.lpszName	= strItemName;
		item.nImage		= byImage;
		item.pMenu		= (CMenu*)nMenuIndex;
		item.strInfo	= strInfo;
		item.crColor	= crColor;
		int hItem;
		RWORKTREE_CMD(OC_WS_ADDTREEITEM, nTreeID, (LPARAM)(&item), &hItem);

		CString strhItem;
		strhItem.Format("%d", hItem);
		Tcl_AppendResult(interp, strhItem, (char *) NULL);
	}else
	if(strOpr == "delitem")	// 删除树节点
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "delitem TreeID id ?-TIM_ID|TIM_ITEM? ?-TIM_CHILDONLY?");
			return TCL_ERROR;
		}

		// 必须输入的参数
		CString strTreeID = ConvertUTFParam(Tcl_GetString(objv[2]));
		int nTreeID = atoi(strTreeID);
		CString strID = ConvertUTFParam(Tcl_GetString(objv[3]));
		int nID = atoi(strID);
		DWORD dwMask = TIM_ID;

		for(int i=4; i<objc; i++)
		{
			CString strParam = ConvertUTFParam(Tcl_GetString(objv[i]));
			if(strParam == "-TIM_ID")
			{
				dwMask &= ~TIM_ITEM;
				dwMask |= TIM_ID;
			}else
			if(strParam == "-TIM_ITEM")
			{
				dwMask &= ~TIM_ID;
				dwMask |= TIM_ITEM;
			}
			if(strParam == "-TIM_CHILDONLY")
			{
				dwMask |= TIM_CHILDONLY;
			}
		}

		HWORKTREEITEM item;
		item.dwMask		= dwMask;
		item.hItem		= (HTREEITEM)nID;
		item.nID		= nID;		
		WORKTREE_CMD(OC_WS_DELTREEITEM, nTreeID, (LPARAM)(&item));
	}else
	if(strOpr == "getitem")	// 取得节点信息
	{
		if(objc < 5)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "getitem info TreeID id ?-TIM_ID|TIM_ITEM?");
			return TCL_ERROR;
		}

		// 必须输入的参数
		CString strInfo = ConvertUTFParam(Tcl_GetString(objv[2]));
		CString strTreeID = ConvertUTFParam(Tcl_GetString(objv[3]));
		int nTreeID = atoi(strTreeID);
		CString strID = ConvertUTFParam(Tcl_GetString(objv[4]));
		int nID = atoi(strID);
		DWORD dwMask = TIM_ID;
		if(objc >= 6)
		{
			CString strParam = ConvertUTFParam(Tcl_GetString(objv[5]));
			if(strParam == "-TIM_ITEM")
				dwMask = TIM_ITEM;
		}

		if(strInfo == "name")
		{
			dwMask |= TIM_NAME;
		}else
		if(strInfo == "image")
		{
			dwMask |= TIM_IMAGE;
		}else
		if(strInfo == "type")
		{
			dwMask |= TIM_TYPE;
		}else
		if(strInfo == "state")
		{
			dwMask |= TIM_STATE;
		}else
		if(strInfo == "parent")
		{
			dwMask |= TIM_PARENT;
		}else
		if(strInfo == "info")
		{
			dwMask |= TIM_INFO;
		}else
		if(strInfo == "dclick")
		{
			dwMask |= TIM_DCLICK;
		}

		if(strInfo == "child")
		{	// 获取子节点
			HWORKTREEITEM item;
			item.dwMask		= dwMask;
			item.hItem		= (HTREEITEM)nID;
			item.nID		= nID;
			int hChildItem = NULL;
			RWORKTREE_CMD(OC_WS_GETCHILDITEM, nTreeID, (LPARAM)(&item), &hChildItem);
			CString strRes;
			strRes.Format("%d", hChildItem);
			Tcl_AppendResult(interp, strRes, (char *) NULL);
		}else
		{
			HWORKTREEITEM item;
			item.dwMask		= dwMask;
			item.hItem		= (HTREEITEM)nID;
			item.nID		= nID;
			BOOL bRet = 0;
			RWORKTREE_CMD(OC_WS_GETITEMINFO, nTreeID, (LPARAM)(&item), &bRet);
			if(bRet)
			{
				CString strRes;
				if(strInfo == "item")
				{
					strRes.Format("%d", item.hItem);
				}else
				if(strInfo == "name")
				{
					strRes = item.lpszName;
				}else
				if(strInfo == "image")
				{
					strRes.Format("%d", item.nImage);
				}else
				if(strInfo == "type")
				{
					strRes.Format("%d", item.byType);
				}else
				if(strInfo == "state")
				{
					strRes.Format("%d", item.byState);
				}else
				if(strInfo == "parent")
				{
					strRes.Format("%d", item.hParent);
				}else
				if(strInfo == "info")
				{
					strRes = item.strInfo;
				}else
				if(strInfo == "dclick")
				{
					strRes = item.strDClick;
				}
				Tcl_AppendResult(interp, strRes, (char *) NULL);
			}
		}
	}else
	if(strOpr == "setitem")	// 设置节点信息
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "setitem TreeID id ?-TIM_ID|TIM_ITEM? ?-param(value)?");
			return TCL_ERROR;
		}

		// 必须输入的参数
		CString strTreeID = ConvertUTFParam(Tcl_GetString(objv[2]));
		int nTreeID = atoi(strTreeID);
		CString strID = ConvertUTFParam(Tcl_GetString(objv[3]));
		int nID = atoi(strID);
		DWORD dwMask = TIM_ID;
		CString strItemName = "";
		BYTE byType = 0;
		BYTE byState = WTS_NORMAL;
		BYTE byImage = 0;
		CString strInfo = "";
		CString strDClick = "";
		COLORREF crColor = RGB(0,0,0);
		//LOGFONT logFont;
		int nMenuIndex = 0;
		for(int i=4; i<objc; i++)
		{
			CString strParam = ConvertUTFParam(Tcl_GetString(objv[i]));
			if(strParam == "-TIM_ID")
			{
				dwMask &= ~TIM_ITEM;
				dwMask |= TIM_ID;
			}else
			if(strParam == "-TIM_ITEM")
			{
				dwMask &= ~TIM_ID;
				dwMask |= TIM_ITEM;
			}
			if((strParam.Find("-name(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 6);
				strParam.Delete(strParam.GetLength()-1, 1);
				strItemName = strParam;
				dwMask |= TIM_NAME;
			}else
			if((strParam.Find("-type(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 6);
				strParam.Delete(strParam.GetLength()-1, 1);
				byType = atoi(strParam);
				dwMask |= TIM_TYPE;
			}else
			if((strParam.Find("-state(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 7);
				strParam.Delete(strParam.GetLength()-1, 1);
				byState = atoi(strParam);
				dwMask |= TIM_STATE;
			}else
			if((strParam.Find("-image(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 7);
				strParam.Delete(strParam.GetLength()-1, 1);
				byImage = atoi(strParam);
				dwMask |= TIM_IMAGE;
			}else
			if((strParam.Find("-info(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 6);
				strParam.Delete(strParam.GetLength()-1, 1);
				strInfo = strParam;
				dwMask |= TIM_INFO;
			}else
			if((strParam.Find("-dclick(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 8);
				strParam.Delete(strParam.GetLength()-1, 1);
				strDClick = strParam;
				dwMask |= TIM_DCLICK;
			}else
			if((strParam.Find("-color(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 7);
				strParam.Delete(strParam.GetLength()-1, 1);
				int r = 0, g=0, b=0;
				int nPos = strParam.Find(",");
				if(nPos > 0)
				{
					r = atoi(strParam.Left(nPos));
					strParam.Delete(0, nPos+1);
					nPos = strParam.Find(",");
					if(nPos > 0)
					{
						g = atoi(strParam.Left(nPos));
						strParam.Delete(0, nPos+1);
						b = atoi(strParam);
					}
				}
				crColor = RGB(r, g, b);
				dwMask |= TIM_COLOR;
			}else
			if((strParam.Find("-bold(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 6);
				strParam.Delete(strParam.GetLength()-1, 1);
				if(strParam == "true")
				{
					dwMask |= TIM_BOLD;
				}
			}else
			if((strParam.Find("-font(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 6);
				strParam.Delete(strParam.GetLength()-1, 1);
				//dwMask |= TIM_FONT;
			}else
			if((strParam.Find("-menu(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 6);
				strParam.Delete(strParam.GetLength()-1, 1);
				nMenuIndex = atoi(strParam);
				dwMask |= TIM_MENU;
			}
		}

		HWORKTREEITEM item;
		item.dwMask		= dwMask;
		item.hItem		= (HTREEITEM)nID;
		item.nID		= nID;
		item.byState	= byState;
		item.byType		= byType;
		item.lpszName	= strItemName;
		item.nImage		= byImage;
		item.pMenu		= (CMenu*)nMenuIndex;
		item.strInfo	= strInfo;
		item.strDClick	= strDClick;
		item.crColor	= crColor;
		//item.logFont	= logFont;
		BOOL bRet = 0;
		RWORKTREE_CMD(OC_WS_SETITEMINFO, nTreeID, (LPARAM)(&item), &bRet);

	}else
	if(strOpr == "getnextitem")	// 取得下一个节点
	{
		if(objc < 5)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "getnextitem TreeID id code ?-TIM_ID|TIM_ITEM?");
			return TCL_ERROR;
		}

		// 必须输入的参数		
		CString strTreeID = ConvertUTFParam(Tcl_GetString(objv[2]));
		int nTreeID = atoi(strTreeID);
		CString strID = ConvertUTFParam(Tcl_GetString(objv[3]));
		int nID = atoi(strID);
		CString strCode = ConvertUTFParam(Tcl_GetString(objv[4]));
		DWORD dwMask = TIM_ID;
		if(objc >= 6)
		{
			CString strParam = ConvertUTFParam(Tcl_GetString(objv[5]));
			if(strParam == "-TIM_ITEM")
				dwMask = TIM_ITEM;
		}

		HWORKTREEITEM item;
		item.dwMask		= dwMask;
		item.hItem		= (HTREEITEM)nID;
		item.nID		= nID;
		if(strCode == "TVGN_CARET")
		{
			item.byType	= TVGN_CARET;
		}else
		if(strCode == "TVGN_CHILD")
		{
			item.byType	= TVGN_CHILD;
		}else
		if(strCode == "TVGN_DROPHILITE")
		{
			item.byType	= TVGN_DROPHILITE;
		}else
		if(strCode == "TVGN_FIRSTVISIBLE")
		{
			item.byType	= TVGN_FIRSTVISIBLE;
		}else
		if(strCode == "TVGN_NEXT")
		{
			item.byType	= TVGN_NEXT;
		}else
		if(strCode == "TVGN_NEXTVISIBLE")
		{
			item.byType	= TVGN_NEXTVISIBLE;
		}else
		if(strCode == "TVGN_PARENT")
		{
			item.byType	= TVGN_PARENT;
		}else
		if(strCode == "TVGN_PREVIOUS")
		{
			item.byType	= TVGN_PREVIOUS;
		}else
		if(strCode == "TVGN_PREVIOUSVISIBLE")
		{
			item.byType	= TVGN_PREVIOUSVISIBLE;
		}else
		if(strCode == "TVGN_ROOT")
		{
			item.byType	= TVGN_ROOT;
		}

		int hItem = NULL;
		RWORKTREE_CMD(OC_WS_GETNEXTITEM, nTreeID, (LPARAM)(&item), &hItem);
		CString strRes;
		strRes.Format("%d", hItem);
		Tcl_AppendResult(interp, strRes, (char *) NULL);

	}else
	if(strOpr == "searchitem")	// 查找节点
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "searchitem TreeID info");
			return TCL_ERROR;
		}

		// 必须输入的参数
		CString strTreeID = ConvertUTFParam(Tcl_GetString(objv[2]));
		int nTreeID = atoi(strTreeID);
		CString strParam = ConvertUTFParam(Tcl_GetString(objv[3]));
		DWORD dwMask = TIM_INFO;

		HWORKTREEITEM item;
		item.dwMask		= dwMask;
		item.strInfo	= strParam;
		HTREEITEM hItem;
		RWORKTREE_CMD(OC_WS_SEARCHITEM, nTreeID, (LPARAM)(&item), &hItem);

		CString strhItem;
		strhItem.Format("%d", hItem);
		Tcl_AppendResult(interp, strhItem, (char *) NULL);
	}else
	if(strOpr == "expanditem" || strOpr == "collapseitem")	// 展开/收缩树节点
	{
		if(objc < 4)
		{
			if(strOpr == "expanditem")
				Tcl_WrongNumArgs(interp, 1, objv, "expanditem TreeID id ?-TIM_ID|TIM_ITEM?");
			else
				Tcl_WrongNumArgs(interp, 1, objv, "collapseitem TreeID id ?-TIM_ID|TIM_ITEM?");
			return TCL_ERROR;
		}
		BOOL bExpand = (strOpr == "expanditem");

		// 必须输入的参数
		CString strTreeID = ConvertUTFParam(Tcl_GetString(objv[2]));
		int nTreeID = atoi(strTreeID);
		CString strID = ConvertUTFParam(Tcl_GetString(objv[3]));
		int nID = atoi(strID);
		DWORD dwMask = TIM_ID;
		if(objc >= 5)
		{
			CString strParam = ConvertUTFParam(Tcl_GetString(objv[4]));
			if(strParam == "-TIM_ITEM")
				dwMask = TIM_ITEM;
		}

		HWORKTREEITEM item;
		item.dwMask		= dwMask;
		item.hItem		= (HTREEITEM)nID;
		item.nID		= nID;
		if(bExpand)
		{
			WORKTREE_CMD(OC_WS_EXPANDITEM, nTreeID, (LPARAM)(&item));
		}else
		{
			WORKTREE_CMD(OC_WS_COLLAPSEITEM, nTreeID, (LPARAM)(&item));
		}

	}else
	if(strOpr == "sortchildren")	// 展开/收缩树节点
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "sortchildren TreeID id ?-TIM_ID|TIM_ITEM?");
			return TCL_ERROR;
		}

		// 必须输入的参数
		CString strTreeID = ConvertUTFParam(Tcl_GetString(objv[2]));
		int nTreeID = atoi(strTreeID);
		CString strID = ConvertUTFParam(Tcl_GetString(objv[3]));
		int nID = atoi(strID);
		DWORD dwMask = TIM_ID;
		if(objc >= 5)
		{
			CString strParam = ConvertUTFParam(Tcl_GetString(objv[4]));
			if(strParam == "-TIM_ITEM")
				dwMask = TIM_ITEM;
		}

		HWORKTREEITEM item;
		item.dwMask		= dwMask;
		item.hItem		= (HTREEITEM)nID;
		item.nID		= nID;

		BOOL bRet = 0;
		RWORKTREE_CMD(OC_WS_SORTCHILDREN, nTreeID, (LPARAM)(&item), &bRet);

	}else
	if(strOpr == "setdclickscript")	// 设置节点双击的脚本
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "setdclickscript TreeID script");
			return TCL_ERROR;
		}

		// 必须输入的参数
		CString strTreeID = ConvertUTFParam(Tcl_GetString(objv[2]));
		int nTreeID = atoi(strTreeID);
		CString strScript = ConvertUTFParam(Tcl_GetString(objv[3]));

		WORKTREE_CMD(OC_WS_SETDCLICKSCRIPT, nTreeID, (LPARAM)(LPCTSTR)strScript);

	}else
	if(strOpr == "addmenuitem")	// 添加菜单项
	{
		if(objc < 6)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "addmenuitem TreeID type string command ?menuid?");
			return TCL_ERROR;
		}

		// 必须输入的参数
		CString strTreeID = ConvertUTFParam(Tcl_GetString(objv[2]));
		int nTreeID = atoi(strTreeID);
		BYTE byType = atoi(ConvertUTFParam(Tcl_GetString(objv[3])));
		CString strMenuItem = ConvertUTFParam(Tcl_GetString(objv[4]));
		CString strMenuCmd = ConvertUTFParam(Tcl_GetString(objv[5]));

		UINT nID = 0;
		if(objc > 6)
		{
			nID = atoi(ConvertUTFParam(Tcl_GetString(objv[6])));
		}

		HWORKMENUITEM menuitem;
		menuitem.byType = byType;
		menuitem.nID = nID;
		menuitem.strItem = strMenuItem;
		menuitem.strCommand = strMenuCmd;
		menuitem.Proc = NULL;
		menuitem.UpdateProc = NULL;

		WORKTREE_CMD(OC_WS_ADDMENUITEM, nTreeID, (LPARAM)(&menuitem));

	}else
	if(strOpr == "settasktheme")	// 设置TaskPanel的Theme
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "settasktheme TreeID theme");
			return TCL_ERROR;
		}

		// 必须输入的参数
		CString strTreeID = ConvertUTFParam(Tcl_GetString(objv[2]));
		int nTreeID = atoi(strTreeID);
		int nTheme = atoi(ConvertUTFParam(Tcl_GetString(objv[3])));

		WORKTREE_CMD(OC_WS_SETTASKTHEME, nTreeID, nTheme);
	}else
	if(strOpr == "addtaskitem")	// 添加Task项
	{
		if(objc < 5)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "addtaskitem TreeID caption command ?-param(value)?");
			return TCL_ERROR;
		}

		// 必须输入的参数
		CString strTreeID = ConvertUTFParam(Tcl_GetString(objv[2]));
		int nTreeID = atoi(strTreeID);
		CString strCaption = ConvertUTFParam(Tcl_GetString(objv[3]));
		CString strCommand = ConvertUTFParam(Tcl_GetString(objv[4]));

		DWORD dwMask = 0;
		int nType = WTT_GROUP;
		int nImage = -1;
		int nParentID = 0;
		int nID = 0;
		COLORREF crColor = RGB(0,0,0);
		CString strInfo = "";

		for(int i=5; i<objc; i++)
		{
			CString strParam = ConvertUTFParam(Tcl_GetString(objv[i]));
			if((strParam.Find("-id(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 4);
				strParam.Delete(strParam.GetLength()-1, 1);
				nID = atoi(strParam);
				dwMask |= TIM_ID;
			}else
			if((strParam.Find("-parent(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 8);
				strParam.Delete(strParam.GetLength()-1, 1);
				nParentID = atoi(strParam);
				dwMask |= TIM_PARENT;
			}else
			if((strParam.Find("-type(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 6);
				strParam.Delete(strParam.GetLength()-1, 1);
				nType = atoi(strParam);
				dwMask |= TIM_TYPE;
			}else
			if((strParam.Find("-image(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 7);
				strParam.Delete(strParam.GetLength()-1, 1);
				nImage = atoi(strParam);
				dwMask |= TIM_IMAGE;
			}else
			if((strParam.Find("-info(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 6);
				strParam.Delete(strParam.GetLength()-1, 1);
				strInfo = strParam;
				dwMask |= TIM_INFO;
			}else
			if((strParam.Find("-color(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 7);
				strParam.Delete(strParam.GetLength()-1, 1);
				int r = 0, g=0, b=0;
				int nPos = strParam.Find(",");
				if(nPos > 0)
				{
					r = atoi(strParam.Left(nPos));
					strParam.Delete(0, nPos+1);
					nPos = strParam.Find(",");
					if(nPos > 0)
					{
						g = atoi(strParam.Left(nPos));
						strParam.Delete(0, nPos+1);
						b = atoi(strParam);
					}
				}
				crColor = RGB(r, g, b);
				dwMask |= TIM_COLOR;
			}else
			if((strParam.Find("-bold(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 6);
				strParam.Delete(strParam.GetLength()-1, 1);
				if(strParam == "true")
				{
					dwMask |= TIM_BOLD;
				}
			}else
			if(strParam == "-special")
			{
				dwMask |= TIM_SPECIAL;
			}else
			if((strParam.Find("-font(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				// 暂不支持
				strParam.Delete(0, 6);
				strParam.Delete(strParam.GetLength()-1, 1);
				//dwMask |= TIM_FONT;
			}
		}

		HWORKTASKITEM item;
		item.dwMask = dwMask;
		item.nType = nType;
		item.nID = nID;
		item.nImage = nImage;
		item.nParent = nParentID;
		item.strCaption = strCaption;
		item.strDClick = strCommand;
		item.strInfo = strInfo;
		item.crColor = crColor;

		int nItemID = 0;
		RWORKTREE_CMD(OC_WS_ADDTASKITEM, nTreeID, (LPARAM)(&item), &nItemID);

		CString strTemp;
		strTemp.Format("%d", nItemID);
		Tcl_AppendResult(interp, strTemp, (char *) NULL);
	}else
	if(strOpr == "deltaskitem")	// 删除指定TaskItems
	{
		if(objc < 3)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "addtaskitem TreeID ?-param(value)?");
			return TCL_ERROR;
		}

		// 必须输入的参数
		CString strTreeID = ConvertUTFParam(Tcl_GetString(objv[2]));
		int nTreeID = atoi(strTreeID);

		DWORD dwMask = 0;
		int nType = WTT_GROUP;
		int nImage = -1;
		int nParentID = 0;
		int nID = 0;
		CString strCaption = "";
		CString strInfo = "";

		for(int i=3; i<objc; i++)
		{
			CString strParam = ConvertUTFParam(Tcl_GetString(objv[i]));
			if((strParam.Find("-id(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 4);
				strParam.Delete(strParam.GetLength()-1, 1);
				nID = atoi(strParam);
				dwMask |= TIM_ID;
			}
			if((strParam.Find("-type(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 6);
				strParam.Delete(strParam.GetLength()-1, 1);
				nType = atoi(strParam);
				dwMask |= TIM_TYPE;
			}else
			if((strParam.Find("-caption(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 9);
				strParam.Delete(strParam.GetLength()-1, 1);
				strCaption = strParam;
				dwMask |= TIM_NAME;
			}
			if((strParam.Find("-info(") == 0) && (strParam[strParam.GetLength()-1] == ')'))
			{
				strParam.Delete(0, 6);
				strParam.Delete(strParam.GetLength()-1, 1);
				strInfo = strParam;
				dwMask |= TIM_INFO;
			}
		}

		HWORKTASKITEM item;
		item.dwMask = dwMask;
		item.nType = nType;
		item.nID = nID;
		item.nParent = nParentID;
		item.strCaption = strCaption;
		item.strDClick = "";
		item.strInfo = strInfo;

		WORKTREE_CMD(OC_WS_DELTASKITEM, nTreeID, (LPARAM)(&item));
	}else
	if(strOpr == "settaskicon")	// 设置TaskPanel的图标或图标组
	{
		if(objc < 4)
		{
			Tcl_WrongNumArgs(interp, 1, objv, "settaskicon TreeID ico|bmpfile");
			return TCL_ERROR;
		}

		// 必须输入的参数
		CString strTreeID = ConvertUTFParam(Tcl_GetString(objv[2]));
		int nTreeID = atoi(strTreeID);
		CString strIconFile = ConvertUTFParam(Tcl_GetString(objv[3]));

		int nImageIndex = 0;
		RWORKTREE_CMD(OC_WS_SETTASKICON, nTreeID, (LPARAM)(LPCTSTR)strIconFile, &nImageIndex);

		CString strTemp;
		strTemp.Format("%d", nImageIndex);
		Tcl_AppendResult(interp, strTemp, (char *) NULL);
	}else
	{
		Tcl_AppendResult(interp, "operater must be addtree, closetree, treeexist, gettreeid, activetree, additem, delitem, getitem, setitem, searchitem, expanditem, collapseitem, setdclickscript.", (char *) NULL);
		return TCL_ERROR;
	}

	ConvertResultToUTF(interp);
	return TCL_OK;
}

// TCL WorkTree命令声明函数
int TclWorkTree_Init(Tcl_Interp *interp)
{
	// 注册命令

	// worktree命令
	Tcl_CreateObjCommand(interp, "WorkTree", Tcl_Cmd_WorkSpace, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

	// workspace命令
	Tcl_CreateObjCommand(interp, "WorkSpace", Tcl_Cmd_WorkSpace, 
			(ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

    return TCL_OK;
}
