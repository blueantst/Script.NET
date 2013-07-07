////////////////////////////////////////////////////////////////////////////
//	File:		Tools.cpp
//	Version:	1.0.0.0
//	Created:	2007-08-26
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	Tools manager implement
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlueUI.h"
#include "Tools.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// 工具状态行Pane类
//////////////////////////////////////////////////////////////////////////
CToolStatusPane::CToolStatusPane()
{
	m_nID			= 0;
	m_nWidth		= 16;
	m_strText		= "";
	m_strTooltip	= "";
	m_strIcon		= "";
	m_strAction		= "";
}

CToolStatusPane::~CToolStatusPane()
{
}


//////////////////////////////////////////////////////////////////////////
// 插件信息类
//////////////////////////////////////////////////////////////////////////
CTool::CTool()
{
	m_nMenuIndex = 0;
	m_strPath = "";
	m_strId = "";
	m_strName = "";
	m_strVersion = "";
	m_strProvider = "";
	m_strCategory = "";
	m_bShowMenu = FALSE;
	m_bShowTask = FALSE;
	m_bShowToolbar = FALSE;
	m_bShowStatusbar = FALSE;
	m_strType = "";
	m_strVciId = "";
	m_strInstanceName = "";
	m_strExecFile = "";
	m_strExecCommand = "";
}

CTool::CTool(CString strPath)
{
	m_nMenuIndex = 0;
	m_strPath = strPath;
	m_strId = "";
	m_strName = "";
	m_strVersion = "";
	m_strProvider = "";
	m_strCategory = "";
	m_bShowMenu = FALSE;
	m_bShowTask = FALSE;
	m_bShowToolbar = FALSE;
	m_bShowStatusbar = FALSE;
	m_strType = "";
	m_strVciId = "";
	m_strInstanceName = "";
	m_strExecFile = "";
	m_strExecCommand = "";
}

CTool::~CTool()
{
}

/////////////////////////////////////////////////////////////////////////////
// 加载工具信息
/////////////////////////////////////////////////////////////////////////////
BOOL CTool::LoadInfo()
{
	CString strXmlFile = m_strPath;
	if(LANGUAGE_PAGE_CHINESE == theApp.m_curLanguage)
	{
		strXmlFile += "\\tool_cn.xml";
		// 如果不存在中文的配置文件,就用缺省的
		if(GetFileAttributes(strXmlFile) == 0xFFFFFFFF)
		{
			strXmlFile = m_strPath + "\\tool.xml";
		}
	}else
	{
		strXmlFile += "\\tool.xml";
	}
	CXmlParser parser;
	if(parser.Open(strXmlFile))
	{
		OUTPUT(COLOR_ERROR, "Load %s fail!\r\n", strXmlFile);
		return FALSE;
	}

	DOMNode* pNode = parser.GetRootNode();
	m_strId = parser.GetNodeAttribute(pNode, "id");
	m_strName = parser.GetNodeAttribute(pNode, "name");
	m_strVersion = parser.GetNodeAttribute(pNode, "version");
	m_strProvider = parser.GetNodeAttribute(pNode, "provider-name");

	// 读取属性
	DOMNode* pSubNode = parser.GetChildNode(pNode, "property");
	m_strCategory = parser.GetNodeText(parser.GetChildNode(pSubNode, "category"));
	m_bShowMenu = (parser.GetNodeText(parser.GetChildNode(pSubNode, "showmenu")) == "true");
	m_bShowTask = (parser.GetNodeText(parser.GetChildNode(pSubNode, "showtask")) == "true");
	m_bShowToolbar = (parser.GetNodeText(parser.GetChildNode(pSubNode, "showtoolbar")) == "true");
	m_bShowStatusbar = (parser.GetNodeText(parser.GetChildNode(pSubNode, "showstatusbar")) == "true");

	// 读取状态栏信息
	pSubNode = parser.GetNode("statusbar\\pane");
	while (pSubNode != NULL)
	{
		CToolStatusPane paneInfo;

		paneInfo.m_nID = atoi(parser.GetNodeAttribute(pSubNode, "id"));
		paneInfo.m_nWidth = atoi(parser.GetNodeAttribute(pSubNode, "width"));
		paneInfo.m_strText = parser.GetNodeAttribute(pSubNode, "text");
		paneInfo.m_strTooltip = parser.GetNodeAttribute(pSubNode, "tooltip");
		paneInfo.m_strIcon = parser.GetNodeAttribute(pSubNode, "icon");
		paneInfo.m_strAction = parser.GetNodeText(pSubNode);
		if(paneInfo.m_strAction == "")
		{
			paneInfo.m_strAction = "tool:" + m_strId;
		}

		m_arStatusPane.Add(paneInfo);

		pSubNode = parser.GetNextNode(pSubNode, "pane");
	}

	// 读取DockingPane信息
	pSubNode = parser.GetNode("dockingpanes\\pane");
	while (pSubNode != NULL)
	{
		CToolDockingPane paneInfo;
		paneInfo.m_bFileDefined = TRUE;	// 配置文件中定义的DockingPane

		paneInfo.m_strName = parser.GetNodeAttribute(pSubNode, "name");
		paneInfo.m_nId = atoi(parser.GetNodeAttribute(pSubNode, "id"));
		CString strDirection = parser.GetNodeAttribute(pSubNode, "direction");
		if(strDirection == "left")
		{
			paneInfo.m_nDirection = xtpPaneDockLeft;
		}else
		if(strDirection == "right")
		{
			paneInfo.m_nDirection = xtpPaneDockRight;
		}else
		if(strDirection == "top")
		{
			paneInfo.m_nDirection = xtpPaneDockTop;
		}else
		if(strDirection == "bottom")
		{
			paneInfo.m_nDirection = xtpPaneDockBottom;
		}else
		{
			paneInfo.m_nDirection = xtpPaneDockLeft;
		}
		paneInfo.m_strIcon = parser.GetNodeAttribute(pSubNode, "icon");
		CString strAttachType = parser.GetNodeAttribute(pSubNode, "type");
		if(strAttachType == "window")
		{
			paneInfo.m_nAttachType = PANE_ATTACH_WND;
		}else
		if(strAttachType == "view")
		{
			paneInfo.m_nAttachType = PANE_ATTACH_VIEW;
		}else
		{
			paneInfo.m_nAttachType = PANE_ATTACH_WND;
		}
		paneInfo.m_bAutoCreate = (parser.GetNodeAttribute(pSubNode, "autocreate") == "true");
		paneInfo.m_bSave = (parser.GetNodeAttribute(pSubNode, "save") == "true");

		m_arDockingPane.Add(paneInfo);

		pSubNode = parser.GetNextNode(pSubNode, "pane");
	}

	// 读取运行信息
	pSubNode = parser.GetChildNode(pNode, "runtime");
	m_strType = parser.GetNodeText(parser.GetChildNode(pSubNode, "type"));
	m_strVciId = parser.GetNodeText(parser.GetChildNode(pSubNode, "vci"));
	m_strInstanceName = parser.GetNodeText(parser.GetChildNode(pSubNode, "instancename"));
	CString strFile = parser.GetNodeText(parser.GetChildNode(pSubNode, "file"));
	if(strFile != "")
	{
		if(strFile.Find(":") != -1)
		{
			m_strExecFile = strFile;
		}else
		{
			m_strExecFile = m_strPath + "\\" + strFile;
		}
	}
	m_strExecCommand = parser.GetNodeText(parser.GetChildNode(pSubNode, "command"));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 显示About对话框
/////////////////////////////////////////////////////////////////////////////
void CTool::ShowAboutBox()
{
}

/////////////////////////////////////////////////////////////////////////////
// 初始化
/////////////////////////////////////////////////////////////////////////////
BOOL CTool::Init()
{
	if(m_bShowStatusbar)
	{
		// 初始化状态行Pane
		for(int i=0; i<m_arStatusPane.GetSize(); i++)
		{
			COM_CMD("", OC_ADDPANE, m_arStatusPane[i].m_nID, m_arStatusPane[i].m_nWidth);
			COM_CMD("", OC_SETPANETEXT, m_arStatusPane[i].m_nID, (LPARAM)(LPCTSTR)(m_arStatusPane[i].m_strText));
			COM_CMD("", OC_SETPANETOOLTIP, m_arStatusPane[i].m_nID, (LPARAM)(LPCTSTR)(m_arStatusPane[i].m_strTooltip));

			CString strIconFile = m_strPath + "\\" + m_arStatusPane[i].m_strIcon;
			COM_CMD("", OC_SETCOMMANDICON, m_arStatusPane[i].m_nID, (LPARAM)(LPCTSTR)(strIconFile));
			COM_CMD("", OC_SETPANEIMAGEITEM, m_arStatusPane[i].m_nID, m_arStatusPane[i].m_nID);

			CExtMenuInfo extMenuInfo;
			extMenuInfo.nId = m_arStatusPane[i].m_nID;
			extMenuInfo.m_strAction = m_arStatusPane[i].m_strAction;
			COM_CMD("", OC_SETPANEBUTTON, m_arStatusPane[i].m_nID, (LPARAM)(&extMenuInfo));
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 加载DockingPane
// nID = -1表示加载所有自动加载的
/////////////////////////////////////////////////////////////////////////////
BOOL CTool::LoadDockingPane(int nID)
{
	CMainFrame* pMainFrame = (CMainFrame*)(AfxGetMainWnd());
	if(nID == -1)
	{
		for(int i=0; i<m_arDockingPane.GetSize(); i++)
		{
			CXTPDockingPane* pPane = pMainFrame->GetPaneManager()->FindPane(m_arDockingPane[i].m_nId);
			if(m_arDockingPane[i].m_bAutoCreate || pPane)
			{
				CString strIconPath = m_arDockingPane[i].m_strIcon;
				if(strIconPath.Find(":") == -1)
				{
					strIconPath = m_strPath+"\\"+m_arDockingPane[i].m_strIcon;
				}
				WORD wIndex = 0;
				HICON hIcon = ::ExtractAssociatedIcon(theApp.m_hInstance,
					strIconPath.GetBuffer(0), &wIndex);
				strIconPath.ReleaseBuffer();
				if(m_arDockingPane[i].m_bAutoCreate)
				{
					pMainFrame->CreateDockingBar(m_arDockingPane[i].m_nId,
						m_arDockingPane[i].m_nDirection, hIcon);
				}else
				if(hIcon)
				{
					pMainFrame->GetPaneManager()->SetIcon(m_arDockingPane[i].m_nId,
						CXTPImageManagerIconHandle(hIcon));
				}
			}
		}
	}else
	{
		for(int i=0; i<m_arDockingPane.GetSize(); i++)
		{
			if(m_arDockingPane[i].m_nId == nID)
			{
				CString strIconPath = m_arDockingPane[i].m_strIcon;
				if(strIconPath.Find(":") == -1)
				{
					strIconPath = m_strPath+"\\"+m_arDockingPane[i].m_strIcon;
				}
				WORD wIndex = 0;
				HICON hIcon = ::ExtractAssociatedIcon(theApp.m_hInstance,
					strIconPath.GetBuffer(0), &wIndex);
				strIconPath.ReleaseBuffer();
				pMainFrame->CreateDockingBar(m_arDockingPane[i].m_nId,
					m_arDockingPane[i].m_nDirection, hIcon);
				return TRUE;
			}
		}
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 执行工具
/////////////////////////////////////////////////////////////////////////////
BOOL CTool::Run()
{
	if(m_strType == "interp")
	{
		IInterp* pInterp = (IInterp*)(theApp.CreateVciObject(m_strVciId, m_strInstanceName));
		if(pInterp == NULL)
		{
			return FALSE;
		}

		if(m_strExecFile != "")
		{
			theApp.ProcessPath(m_strExecFile);
			pInterp->RunScriptFile(m_strExecFile);
		}else
		if(m_strExecCommand != "")
		{
			pInterp->RunScriptCommand(m_strExecCommand);
		}

		theApp.ReleaseObject(pInterp);
	}else
	if(m_strType == "html")
	{
		CString strURL = "";
		if(m_strVciId.Find("org.interp") == 0)
		{
			strURL = m_strVciId;
			strURL.Delete(0, 11);
			strURL += ":";
			if(m_strInstanceName != "")
			{
				strURL += "<<";
				strURL += m_strInstanceName;
				strURL += ">>";
			}
			if(m_strExecFile != "")
			{
				strURL += "<file>";
				strURL += m_strExecFile;
			}else
			{
				strURL += m_strExecCommand;
			}
		}else
		if(m_strExecFile != "")
		{
			strURL = m_strExecFile;
		}else
		{
			strURL = m_strExecCommand;
		}
		COM_CMD(VCIID_HTMLVIEW, OC_VIEWHTMLPAGE, (WPARAM)(LPCTSTR)strURL, 0);
	}else
	if(m_strType == "owm")
	{
		if(m_strExecFile == "")
		{
			// 文件名为空则打开OWM窗口
			CPlugIn* pPlugIn = theApp.GetOWM(m_strVciId);
			if(pPlugIn)
			{
				// 如果插件属性设置了动态加载,有可能当前并未加载,需要先强制加载
				if(pPlugIn->GetIVciOwm() == NULL)
				{
					pPlugIn->LoadPlugIn(TRUE);
				}
				pPlugIn->ShowOwmView(TRUE, TRUE);
			}
		}else
		{
			theApp.OpenDocumentFileWithOwm(m_strExecFile, m_strVciId);
		}
	}else
	if(m_strType == "owm-bar")	// Dockingbar类型
	{
		if(m_arDockingPane.GetSize() > 0)
		{
			LoadDockingPane(m_arDockingPane[0].m_nId);
		}
		// 如果文件字段不为空,则用OWM打开文件
		if(m_strExecFile.GetLength() > 0)
		{
			theApp.OpenDocumentFileWithOwm(m_strExecFile, m_strVciId);
		}
	}

	return TRUE;
}