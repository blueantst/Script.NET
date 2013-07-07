////////////////////////////////////////////////////////////////////////////
//	File:		Project.cpp
//	Version:	1.0.0.0
//	Created:	2008-10-20
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	Project operate
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlueUI.h"
#include "MainFrm.h"
#include "PlugIn.h"
#include "TreePropertySheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////////////////////////////////////////////////////////////
// Project operate

// 判断是否有打开的工程
BOOL CBlueUIApp::IsProjectOpen() 
{
	if(m_pIProject == NULL)
	{
		return FALSE;
	}

	CString strActiveProject;
	if(m_pIProject->GetActiveProject(strActiveProject) != trpOk)
	{
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// 保存最近打开的工程信息到注册表
//////////////////////////////////////////////////////////////////////
void CBlueUIApp::SaveRecentProject(CString strProjectFile)
{
	CStringArray asRecentProject;
	for(int i=0; i<MAX_RECENT_PROJECT; i++)
	{
		CString strPrjName;
		strPrjName.Format("%s%d", REG_RECENT_PROJECT, i+1);
		CString strPrjPath = AfxGetApp()->GetProfileString(REG_RECENTPRJ_SUBKEY, strPrjName, "");
		if(strPrjPath != "")
		{
			asRecentProject.Add(strPrjPath);
		}
	}

	BOOL bFind = FALSE;
	int nCount = asRecentProject.GetSize();
	for(int i=0; i<nCount; i++)
	{
		if(strProjectFile.CompareNoCase(asRecentProject[i]) == 0)
		{
			asRecentProject.RemoveAt(i);
			asRecentProject.InsertAt(0, strProjectFile);
			bFind = TRUE;
			break;
		}
	}

	if(!bFind)
	{
		asRecentProject.InsertAt(0, strProjectFile);
	}

	nCount = asRecentProject.GetSize();
	for(int i=0; i<MAX_RECENT_PROJECT; i++)
	{
		CString strPrjName;
		strPrjName.Format("%s%d", REG_RECENT_PROJECT, i+1);
		if(i < nCount)
		{
			AfxGetApp()->WriteProfileString(REG_RECENTPRJ_SUBKEY, strPrjName, asRecentProject[i]);
		}else
		{
			AfxGetApp()->WriteProfileString(REG_RECENTPRJ_SUBKEY, strPrjName, "");
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 执行工程脚本操作
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::ExecProjectScript(CString strMenuAction) 
{
	// TODO: Add your command handler code here
	CString strProjectPlugInId = theApp.GetPlugInIdByInstancePtr(m_pIProject);
	if(strProjectPlugInId == "")
	{
		return;
	}

	CString strPlugInPath = GetPlatRootPath() + "PrjPlugins\\" + strProjectPlugInId;
	CString strXmlFile = strPlugInPath;
	if(LANGUAGE_PAGE_CHINESE == m_curLanguage)
	{
		strXmlFile += "\\plugin_cn.xml";
		// 如果不存在中文的配置文件,就用缺省的
		if(GetFileAttributes(strXmlFile) == 0xFFFFFFFF)
		{
			strXmlFile = strPlugInPath + "\\plugin.xml";
		}
	}else
	{
		strXmlFile += "\\plugin.xml";
	}

	CXmlParser parser;
	if(parser.Open(strXmlFile))
	{
		OUTPUT(COLOR_ERROR, "Load %s fail!\r\n", strXmlFile);
		return;
	}

	CString strXmlNodeName = _T("menu-action\\") + strMenuAction;
	CString strInterp = parser.GetNodeText(strXmlNodeName + "\\Interp");
	CString strInterpInstance = parser.GetNodeAttribute(strXmlNodeName + "\\Interp" , "Name");
	CString strScriptFile = parser.GetNodeText(strXmlNodeName + "\\script");
	ProcessPath(strScriptFile, strProjectPlugInId);
	if(strInterp == VCIID_HTMLVIEW)
	{
		COM_CMD(VCIID_HTMLVIEW, OC_VIEWHTMLPAGE, (WPARAM)(LPCTSTR)strScriptFile, 0);
		return;
	}

	//ProcessPath(strScriptFile);
	IInterp* pInterp = (IInterp*)CreateVciObject(strInterp, strInterpInstance);
	if(pInterp)
	{
		if(!pInterp->RunScriptFile(strScriptFile))
		{
			AfxMessageBox(pInterp->GetResult());
		}
		ReleaseObject(pInterp);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 打开工程
/////////////////////////////////////////////////////////////////////////////
BOOL CBlueUIApp::OpenProjectFile(CPlugIn* pPlugIn, CString strProjectFile) 
{
	if(IsProjectOpen())
	{
		return FALSE;
	}

	if(pPlugIn == NULL)
	{
		// 如果插件指针为空,则根据后缀查找插件
		CString strFileName = strProjectFile;
		strFileName.MakeLower();
		int nPos = strFileName.ReverseFind('.');
		if(nPos != -1)
		{
			CString strFileExt = strFileName;
			strFileExt.Delete(0, nPos+1);
			for(int i=0; i<m_aPlugIns.GetSize(); i++)
			{
				if(m_aPlugIns[i].m_strType != "project")
					continue;

				for(int j=0; j<m_aPlugIns[i].m_arDocExtend.GetSize(); j++)
				{
					for(int k=0; k<m_aPlugIns[i].m_arDocExtend[j].m_arDocExt.GetSize(); k++)
					{
						if(strFileExt == m_aPlugIns[i].m_arDocExtend[j].m_arDocExt[k])
						{
							pPlugIn = &(m_aPlugIns[i]);
						}
					}
				}
			}
		}

		if(pPlugIn == NULL)
		{
			// 未找到能打开工程的插件
			return FALSE;
		}
	}

	// 注意要释放
	IProject* pIProject = pPlugIn->CreateProjectObject("###project");
	if(pIProject)
	{
		PLAT_LOADSTRING(strInfoOpenProject, IDS_INFO_OPEN_PROJECT); // 打开工程 %s\r\n
		OUTPUT(COLOR_NORMAL, strInfoOpenProject, strProjectFile);

		if(pIProject->OpenProject(strProjectFile) == trpOk)
		{
			m_pIProject = pIProject;
			SaveRecentProject(strProjectFile);
			ExecProjectScript("project\\open");
			return TRUE;
		}
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 新建工程
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnProjectNew() 
{
	// TODO: Add your command handler code here
	PLAT_LOADSTRING(strInfoNewProject, IDS_INFO_NEW_PROJECT); // 新建工程\r\n
	OUTPUT(COLOR_NORMAL, strInfoNewProject);

	if(ExecMenuScript("project\\newproject") == "success")
	{
		m_pIProject = (IProject*)(theApp.GetObjectByInstanceName("###project"));
		ExecProjectScript("project\\new");
	}
}

void CBlueUIApp::OnUpdateProjectNew(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!IsProjectOpen());
}

/////////////////////////////////////////////////////////////////////////////
// 打开工程
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnProjectOpen() 
{
	// TODO: Add your command handler code here
	CString strFilter;
	CString strTemp;

	CString strDefaultExt = GetProfileString(REG_PROJECT_SUBKEY, REG_PROJECT_DEFFILEEXT, "");
	strDefaultExt.MakeLower();
	BOOL bFoundDefaultExt = FALSE;

	// 挨个工程插件查找扩展点,看哪个工程插件可以打开此文档
	for(int i=0; i<m_aPlugIns.GetSize(); i++)
	{
		if(m_aPlugIns[i].m_strType != "project")
			continue;

		for(int j=0; j<m_aPlugIns[i].m_arDocExtend.GetSize(); j++)
		{
			BOOL bDefaultExt = FALSE;
			CString strTemp1;
			for(int k=0; k<m_aPlugIns[i].m_arDocExtend[j].m_arDocExt.GetSize(); k++)
			{
				strTemp1 += "*.";
				strTemp1 += m_aPlugIns[i].m_arDocExtend[j].m_arDocExt[k];
				if(k != (m_aPlugIns[i].m_arDocExtend[j].m_arDocExt.GetSize()-1))
				{
					strTemp1 += ";";
				}

				CString strDocExt = m_aPlugIns[i].m_arDocExtend[j].m_arDocExt[k];
				strDocExt.MakeLower();
				if(strDefaultExt == strDocExt)
				{
					// 是上次保存的后缀名
					bDefaultExt = TRUE;
				}
			}

			strTemp.Format("%s (%s)|%s|", m_aPlugIns[i].m_arDocExtend[j].m_strDocName, strTemp1, strTemp1);
			if(bDefaultExt && !bFoundDefaultExt)
			{
				strFilter = strTemp + strFilter;
				bFoundDefaultExt = TRUE;
			}else
			{				
				strFilter += strTemp;
			}
		}
	}
	strFilter += "All files(*.*)|*.*||";

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,	strFilter);
	if (dlg.DoModal() == IDOK)
	{
		CString strExt = dlg.GetFileExt();
		WriteProfileString(REG_PROJECT_SUBKEY, REG_PROJECT_DEFFILEEXT, strExt);
		for(int i=0; i<m_aPlugIns.GetSize(); i++)
		{
			if(m_aPlugIns[i].m_strType != "project")
				continue;

			OpenProjectFile(&(m_aPlugIns[i]), dlg.GetPathName());
		}
	}
}

void CBlueUIApp::OnUpdateProjectOpen(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!IsProjectOpen());
}

/////////////////////////////////////////////////////////////////////////////
// 保存工程
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnProjectSave() 
{
	// TODO: Add your command handler code here
	PLAT_LOADSTRING(strInfoSaveProject, IDS_INFO_SAVE_PROJECT); // 保存工程\r\n
	OUTPUT(COLOR_NORMAL, strInfoSaveProject);
	m_pIProject->SaveProject();
	ExecProjectScript("project\\save");
}

void CBlueUIApp::OnUpdateProjectSave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(IsProjectOpen());	
}

/////////////////////////////////////////////////////////////////////////////
// 执行工程
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnProjectRun() 
{
	// TODO: Add your command handler code here
	PLAT_LOADSTRING(strInfoRunProject, IDS_INFO_RUN_PROJECT); // 运行工程\r\n
	OUTPUT(COLOR_NORMAL, strInfoRunProject);
	m_pIProject->RunProject();
	ExecProjectScript("project\\run");
}

void CBlueUIApp::OnUpdateProjectRun(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(IsProjectOpen());
}

/////////////////////////////////////////////////////////////////////////////
// 关闭工程
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnProjectClose() 
{
	// TODO: Add your command handler code here
	if(!IsProjectOpen())
	{
		return;
	}

	PLAT_LOADSTRING(strInfoCloseProject, IDS_INFO_CLOSE_PROJECT); // 关闭工程\r\n
	OUTPUT(COLOR_NORMAL, strInfoCloseProject);
	if(m_pIProject->CloseProject(FALSE) != trpOk)
	{
		return;
	}
	ExecProjectScript("project\\close");

	// 释放工程对象
	for(int i=0; i<theApp.m_aPlugIns.GetSize(); i++)
	{
		if(m_aPlugIns[i].ReleaseProjectObject(m_pIProject, TRUE))
		{
			m_pIProject = NULL;
			return;
		}
	}
}

void CBlueUIApp::OnUpdateProjectClose(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(IsProjectOpen());
}

/////////////////////////////////////////////////////////////////////////////
// 编译工程
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnProjectBuild() 
{
	// TODO: Add your command handler code here
	PLAT_LOADSTRING(strInfoBuildProject, IDS_INFO_BUILD_PROJECT); // 编译工程\r\n
	OUTPUT(COLOR_NORMAL, strInfoBuildProject);
	m_pIProject->BuildProject();
	ExecProjectScript("project\\build");
}

void CBlueUIApp::OnUpdateProjectBuild(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(IsProjectOpen());
}

/////////////////////////////////////////////////////////////////////////////
// 重新编译工程
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnProjectRebuild() 
{
	// TODO: Add your command handler code here
	PLAT_LOADSTRING(strInfoRebuildProject, IDS_INFO_REBUILD_PROJECT); // 重新编译工程\r\n
	OUTPUT(COLOR_NORMAL, strInfoRebuildProject);
	m_pIProject->RebuildProject();
	ExecProjectScript("project\\rebuild");
}

void CBlueUIApp::OnUpdateProjectRebuild(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(IsProjectOpen());
}

/////////////////////////////////////////////////////////////////////////////
// 清除工程编译信息
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnProjectClean() 
{
	// TODO: Add your command handler code here
	PLAT_LOADSTRING(strInfoCleanProject, IDS_INFO_CLEAN_PROJECT); // 清除工程编译信息\r\n
	OUTPUT(COLOR_NORMAL, strInfoCleanProject);
	m_pIProject->CleanProject();
	ExecProjectScript("project\\clean");
}

void CBlueUIApp::OnUpdateProjectClean(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(IsProjectOpen());
}

/////////////////////////////////////////////////////////////////////////////
// 工程信息
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnProjectInfo() 
{
	// TODO: Add your command handler code here
	m_pIProject->ProjectInformation();
	ExecProjectScript("project\\information");
}

void CBlueUIApp::OnUpdateProjectInfo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(IsProjectOpen());
}

/////////////////////////////////////////////////////////////////////////////
// 工程设置
/////////////////////////////////////////////////////////////////////////////
void CBlueUIApp::OnProjectProperty() 
{
	// TODO: Add your command handler code here
	CVciPropertyPageArray aPropertyPage;
	//ExecProjectScript("project\\property");

	CTreePropertySheet tps(150);
	PLAT_LOADSTRING(strTitle, IDS_TITLE_TREEPROPSHEET_PROJECTCFG); // 工程设置
	tps.m_strWinTitle = strTitle;

	// 从License组件中获取平台关于对话框
	CDialog* pLicenseTpsPlatform = NULL;
	CString strLicensePlugin = theApp.m_xmlPlat.GetNodeText("application\\LicensePlugin");
	CPlugIn* pLicensePlugIn = theApp.FindPlugIn(strLicensePlugin);
	if(pLicensePlugIn)
	{
		if(!(pLicensePlugIn->HasLoad()))
		{
			pLicensePlugIn->LoadPlugIn(TRUE);
		}
		CVciPropertyPageArray aLicensePropertyPage;
		pLicensePlugIn->RegisterPropertyPage(aLicensePropertyPage);
		if(aLicensePropertyPage.GetSize() > 0)
		{
			pLicenseTpsPlatform = (CDialog*)(aLicensePropertyPage[0].pPage);
		}
	}

	// 工程设置
	if(pLicenseTpsPlatform)
	{
		tps.AddPage(strTitle,tps_item_branch,-1,"",pLicenseTpsPlatform,OPT_PLATFORM,TRUE);
	}else
	{
		tps.AddPage(strTitle,tps_item_branch,IDD_TPS_PLATFORM,"",NULL,OPT_PLATFORM);
	}

	m_pIProject->ProjectPropertyPage(aPropertyPage);

	int count = aPropertyPage.GetSize();
	for(int i=0; i<count; i++)
	{
		CString strNode = aPropertyPage[i].strNode;
		CString strDesc = aPropertyPage[i].strDesc;
		CDialog* pDialog = (CDialog*)(aPropertyPage[i].pPage);
		int nType;
		if(count == 1)
			nType = tps_item_node;
		else
		{
			nType = tps_item_node;
			//if(i == 0)
			//	nType = tps_item_branch;
			//if(i+1 == count)
			//	nType = tps_item_endbranch;
		}
		tps.AddPage(strNode,nType,-1,strDesc,pDialog,OPT_NODE,TRUE);
	}

	tps.SetLastPageType(tps_item_endbranch);

	tps.SetFancy(FALSE);

	// 执行属性设置对话框
	tps.DoModal();

	// 释放资源,防止因下面的操作而引起析构函数出错
	tps.RemoveResource();

}

void CBlueUIApp::OnUpdateProjectProperty(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(IsProjectOpen());
}
