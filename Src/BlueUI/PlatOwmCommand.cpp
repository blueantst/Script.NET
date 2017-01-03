////////////////////////////////////////////////////////////////////////////
//	File:		PlatOwmCommand.cpp
//	Version:	1.0.0.0
//	Created:	2005-08-18
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	Plat Owm Command Processer implement
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlueUI.h"
#include "PlugIn.h"
#include "MainFrm.h"
#include "resource.h"
#include "ProgDlg.h"
#include "TreeOptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 进度对话框
static CProgressDlg* g_ProgressDlg = NULL;

/////////////////////////////////////////////////////////////////////////////
// 平台命令处理函数
/////////////////////////////////////////////////////////////////////////////
int CBlueUIApp::CommandPlat(int nCmd, WPARAM wParam, LPARAM lParam)
{
	switch(nCmd)
	{
	case OC_QUERYOWMID:	// 查询OWM
		{
			return FALSE;
		}

	case OC_RUNSCRIPT:	// 运行TCL脚本
		{
		}

	case OC_OPENVIEW:	// 打开OWM窗口
		{
			if(wParam == NULL)
				return FALSE;

			CPlugIn* pOWM = theApp.GetDynamicOWM((LPCTSTR)wParam);
			if(pOWM)
			{
				//pOWM->NewOwmFrame();
				BOOL bActive = lParam;
				pOWM->ShowOwmView(TRUE, bActive);
				return TRUE;
			}

			return FALSE;
		}

	case OC_CLOSEVIEW:	// 关闭OWM窗口
		{
			if(wParam == NULL)
				return FALSE;

			CPlugIn* pOWM = theApp.GetDynamicOWM((LPCTSTR)wParam);
			if(pOWM)
			{
				pOWM->CloseOwmFrame();
				return TRUE;
			}

			return FALSE;
		}

	case OC_OPENBAR:	// 打开OWM ControlBar
		{
			if(wParam == NULL)
				return FALSE;

			CPlugIn* pOWM = theApp.GetDynamicOWM((LPCTSTR)wParam);
			if(pOWM)
			{
				if(lParam == 1)
					pOWM->ShowOwmControlBar(TRUE);
				else
					pOWM->NewOwmControlBar();
				return TRUE;
			}

			return FALSE;
		}

	case OC_CLOSEBAR:	// 关闭OWM ControlBar
		{
			if(wParam == NULL)
				return FALSE;

			CPlugIn* pOWM = theApp.GetDynamicOWM((LPCTSTR)wParam);
			if(pOWM)
			{
				if(lParam == 1)
					pOWM->ShowOwmControlBar(FALSE);
				else
					pOWM->CloseOwmControlBar();
				return TRUE;
			}

			return FALSE;
		}

	case OC_GETTCLEXKEYWORD:	// 获取TCL扩展命令列表指针
		{
			return NULL;
		}

	case OC_DYNCLOSE:	// 某个扩展模块的动态卸载请求
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			return pMainFrame->PostMessage(WM_PLAT_DYNCLOSE, wParam, lParam);
		}

	case OC_CREATEOUTPUT:	// 创建输出窗口
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

			// 解释窗口名、窗口类型、挂靠的窗口
			CString strName = (LPCTSTR)wParam;
			int nPos = strName.Find("##");
			int nType = OUTPUT_RICH;
			int nAttachPane = 0;
			int nPaneID = 0;
			if(nPos != -1)
			{
				CString strType = strName;
				strType.Delete(0, nPos+2);
				if(strType.Find("GRID") == 0)
				{
					nType = OUTPUT_GRID;
					strType.Delete(0, 4);
				}else
				if(strType.Find("TREE") == 0)
				{
					nType = OUTPUT_TREE;
					strType.Delete(0, 4);
				}
				strName = strName.Left(nPos);
				int nPos1 = strType.Find("##");
				if(nPos1 == 0)
				{
					// 表示要停靠在指定窗口右边,##后面为停靠的窗口名
					strType.Delete(0, 2);
					COutputWndCtrl* pOutputCtrl = pMainFrame->GetOutputCtrl(strType);
					if(pOutputCtrl)
					{
						nAttachPane = pOutputCtrl->nID;
					}else
					{
						nAttachPane = -1;
					}
				}else
				{
					nPos1 = strType.Find("**");
					if(nPos1 == 0)
					{
						// 表示指定Pane ID
						strType.Delete(0, 2);
						nPaneID = atoi(strType);
					}
				}
			}

			int nID = *((int*)lParam);
			if(nID == 0)
			{
				nID = nPaneID;
			}

			if(nID == 0)
			{	// =0表示自动分配ID
				COutputWndCtrl* pOutputWndCtrl = pMainFrame->CreateOutputWnd(strName, nType, -1, nAttachPane);
				*((int*)lParam) = pOutputWndCtrl->nID - IDR_PANE_OUTPUT;	// 第2个参数用于返回ID
			}else
			{	// !=0表示使用指定ID
				COutputWndCtrl* pOutputWndCtrl = pMainFrame->CreateOutputWnd(strName, nType, nID);
			}
			return nID;
		}

	case OC_CLOSEOUTPUT:	// 关闭输出窗口
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			//pMainFrame->GetPaneManager()->ClosePane((int)wParam + IDR_PANE_OUTPUT);
			pMainFrame->GetPaneManager()->DestroyPane((int)wParam + IDR_PANE_OUTPUT);
			pMainFrame->DeleteOutputCtrl((int)wParam + IDR_PANE_OUTPUT);
			return TRUE;
		}

	case OC_OUTPUTSET:	// 设置输出窗口
		{
			return OUTPUT_SET_ID((int)wParam, (LPCTSTR)lParam);
		}

	case OC_OUTPUTGET:	//  获取输出窗口信息
		{
			if((int)wParam == -1)	// wParam=-1表示根据窗口名获取窗口ID,lParam=窗口名
			{
				CString strTmp = "-1";
				CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
				COutputWndCtrl* pOutputCtrl = pMainFrame->GetOutputCtrl((LPCTSTR)lParam);
				if(pOutputCtrl)
				{
					strTmp.Format("%d", pOutputCtrl->nID - IDR_PANE_OUTPUT);
				}
				CString* pStr = new CString(strTmp);
				return (int)pStr;
			}else
			if((int)wParam == -2)	// wParam=-2表示判断窗口是否存在,lParam=窗口ID
			{
				CString strTmp = "-1";
				CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
				COutputWndCtrl* pOutputCtrl = pMainFrame->GetOutputCtrl((int)lParam);
				if(pOutputCtrl)
				{
					strTmp.Format("%d", pOutputCtrl->nID - IDR_PANE_OUTPUT);
				}
				CString* pStr = new CString(strTmp);
				return (int)pStr;
			}
			CString* pStr = new CString(OUTPUT_GET_ID((int)wParam, (LPCTSTR)lParam));
			return (int)pStr;
			//return (int)(LPCTSTR)OUTPUT_GET_ID((int)wParam, (LPCTSTR)lParam);
		}

	case OC_OUTPUT:	// 向输出窗口输出
		{
			//return OutputText((int)wParam, (LPCTSTR)lParam, RGB(0,0,0));
			return OUTPUT_ID_DIRECT((int)wParam, (LPCTSTR)lParam);
		}

	case OC_ACTIVEOUTPUT:	// 激活窗口
		{
			return OUTPUT_ACTIVE_ID((int)wParam);
		}

	case OC_CLEAROUTPUT:	// 清除窗口内容
		{
			OUTPUT_CLEAR((int)wParam);
			return TRUE;
		}

	case OC_TREEOUTPUT:	// 输出树窗口内容
		{
			return OUTPUT_TREEITEMS_ID((int)wParam, (LPVOID)lParam);
		}

	case OC_ADDPANE:	// 添加Pane
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->AddPane((UINT)wParam);
			pMainFrame->SetPaneWidth((UINT)wParam, (int)lParam);
			return TRUE;
		}

	case OC_REMOVEPANE:	// 删除Pane
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->ClosePaneItem((UINT)wParam);
			return TRUE;
		}

	case OC_SETPANETEXT:	// 设置Pane文字
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->SetPaneText((UINT)wParam, (LPCTSTR)lParam);
			return TRUE;
		}

	case OC_SETPANECONTROL:	// 设置Pane控件
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->SetPaneControl((UINT)wParam, (CWnd*)lParam);
			return TRUE;
		}

	case OC_SETPANEIMAGE:	// 设置Pane图标组
		{
			//CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			//pMainFrame->SetPaneImage((UINT)wParam, lParam);
			return TRUE;
		}

	case OC_SETPANEIMAGEITEM:	// 设置Pane图标项
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->SetPaneImageItem((UINT)wParam, lParam);
			return TRUE;
		}

	case OC_SETPANEWIDTH:	// 设置Pane宽度
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->SetPaneWidth((UINT)wParam, lParam);
			return TRUE;
		}

	case OC_ATTACHPANEPROGRESS:	// 建立Pane和进度条的关联
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			UINT nPaneID = LOWORD(wParam);
			UINT nMaxValue = HIWORD(wParam);
			pMainFrame->AttachPaneProgress(nPaneID, (LPCTSTR)lParam, nMaxValue);
			return TRUE;
		}

	case OC_SHOWPANEPROGRESS:	// 显示/隐藏Pane进度条
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->ShowPaneProgress((UINT)wParam, lParam);
			return TRUE;
		}

	case OC_SETPANEPROGRESSPOS:	// 设置Pane进度条进度
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->SetPaneProgressPos((UINT)wParam, (int)lParam);
			return TRUE;
		}

	case OC_SETPANEBUTTON:	// 设置Pane按钮
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->SetPaneButton((UINT)wParam, (CExtMenuInfo*)lParam);
			return TRUE;
		}

	case OC_SETPANETOOLTIP:	// 设置Pane提示信息
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->SetPaneTooltip((UINT)wParam, (LPCTSTR)lParam);
			return TRUE;
		}

	case OC_OPENPROJECT:	// 打开工程
		{
			if(wParam == 0)
			{
				theApp.OnProjectOpen();
			}else
			{
				theApp.OpenProjectFile(NULL, (LPCTSTR)wParam);
			}
			return TRUE;
		}

	case OC_CLOSEPROJECT:	// 关闭工程
		{
			theApp.OnProjectClose();
			return TRUE;
		}

	case OC_EXECOPTION:	// 执行属性页操作
		{
			if((wParam == 1) && (theApp.IsProjectOpen()))	// 工程属性页
			{
				theApp.OnProjectProperty();
			}
			return TRUE;
		}

	case OC_EXECTREEOPTION:	// 执行通用树状属性对话框
		{
			CTreeOptionsDlg option;
			option.SetTOptArrayPoint((CTOptArray*)lParam);
			return option.DoModal();
		}

	case OC_OPENDOCUMENTFILE:	// 打开文件
		{
			return (int)(theApp.OpenDocumentFileWithOwm((LPCTSTR)wParam, (LPCTSTR)lParam));
		}

	case OC_SETDOCICON:	// 设置文档图标
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->SetDocumentIcon((LPCTSTR)wParam, (HWND)lParam);
			return TRUE;
		}

	case OC_SETDOCTITLE:	// 设置文档标题
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->SetDocumentTitle((LPCTSTR)wParam, 0, (HWND)lParam);
			return TRUE;
		}

	case OC_MODIFYDOCTITLE:	// 修改文档标题
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->SetDocumentTitle("", wParam, (HWND)lParam);
			return TRUE;
		}

	case OC_SETCOMMANDICON:	// 设置某个命令ID的图标
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			return pMainFrame->ImageManagerSetIcon(wParam, (LPCTSTR)lParam);
		}

	case OC_SETCOMMANDICONS:	// 设置多个命令ID的图标
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			return pMainFrame->ImageManagerSetIcons((CUIntArray*)wParam, (LPCTSTR)lParam);
		}

	case OC_SETOWMICONS:	// 设置某个OWM的图标
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			if(pMainFrame)
			{
				CPlugIn* pOwmEdit = theApp.FindPlugIn((LPCTSTR)wParam);
				if(pOwmEdit)
				{
					CString strImagePath = pOwmEdit->m_strPath;
					strImagePath += "\\image";
					return pMainFrame->LoadImageStyleIcons(strImagePath);
				}
			}
			return FALSE;
		}

	case OC_REFRESHDEBUGWIN:	// 刷新调试窗口
		{
			IInterp* pInterp = (IInterp*)wParam;
			if(pInterp)
			{
				CString strRes;
				pInterp->RefreshDebugWindow("all", FALSE, strRes);
			}
			return TRUE;
		}

	case OC_CHECK_ACTIVEOWM:	// 判断指定OWM窗口是否为当前激活窗口
		{
			return theApp.CheckActiveOWM((LPCTSTR)wParam, (LPCTSTR)lParam);
		}

	case OC_SHOWABOUTBOX:	// 显示指定模块的About对话框
		{
			CString strPluginId = (LPCTSTR)wParam;
			theApp.ShowAboutBox(strPluginId);
			return TRUE;
		}

	case OC_SENDVCICOMMAND:	// 发送VCI的命令(ProcessMessage函数)
		{
			if((wParam == 0) || (lParam == 0))
			{
				return FALSE;
			}
			CString strVciInstance = (LPCTSTR)wParam;
			CVciMessage* pMsg = (CVciMessage*)lParam;
			if(strVciInstance.Find("owm:") == 0)
			{
				// 发送OWM命令
				CString strOwmId = strVciInstance;
				strOwmId.Delete(0, 4);
				CPlugIn* pPlugIn = theApp.GetOWM(strOwmId);
				if(pPlugIn == NULL)
				{
					return FALSE;
				}
				IVciOwm* pIVciOwm = pPlugIn->GetIVciOwm();
				if(pIVciOwm == NULL)
				{
					// 如果OWM组件尚未加载,则先加载
					pPlugIn->LoadOwmPlugIn();
					pIVciOwm = pPlugIn->GetIVciOwm();
				}
				IVciControl* pIVciControl = (IVciControl*)(pPlugIn->GetObjectControlPtrByInstancePtr(pIVciOwm));
				if(pIVciControl != NULL)
				{
					return pIVciControl->ProcessMessage(pMsg, NULL);
				}
			}else
			{
				// 发送到指定实例名的VCI组件对象
				LPVOID pVciObj = theApp.GetObjectByInstanceName(strVciInstance);
				IVciControl* pIVciControl = (IVciControl*)(theApp.GetObjectControlPtrByInstancePtr(pVciObj));
				if(pIVciControl != NULL)
				{
					return pIVciControl->ProcessMessage(pMsg, NULL);
				}
			}
			return FALSE;
		}

	case OC_PATHSELECT_DIALOG:	// 路径选择对话框
		{
			CXTBrowseDialog dlg(AfxGetMainWnd());
			// 设置标题
			CString strTitle = (LPCTSTR)wParam;
			dlg.SetTitle(strTitle.GetBuffer(0));
			// 设置初始路径
			if(lParam != NULL)
			{
				CString strPath = (LPCTSTR)lParam;
				dlg.SetSelPath(strPath.GetBuffer(0));
			}

			if (dlg.DoModal() == IDOK)
			{
				CString strPath = dlg.GetSelPath();
				CString* pStr = new CString(strPath);
				return (int)pStr;
			}

			return NULL;
		}

	case OC_LOGIN_DIALOG:	// 通用登录对话框
		{
			return theApp.CommLoginDialog((void*)wParam);
		}

	// 通用进度对话框操作
	case OC_PROGDLG_CREATE:	// 创建通用进度对话框
		{
			if(g_ProgressDlg == NULL)
			{
				CString strCaption = "";
				CString strStatus = "";
				if(wParam)
				{
					strCaption = (LPCTSTR)wParam;
					int nPos = strCaption.Find('\n');
					if(nPos != -1)
					{
						strStatus = strCaption.Right(strCaption.GetLength()-nPos-1);
						strCaption = strCaption.Left(nPos);
					}
				}
				g_ProgressDlg = new CProgressDlg(strCaption);
				g_ProgressDlg->Create();
				g_ProgressDlg->SetStatus(strStatus);
				if(lParam != 0)
				{
					g_ProgressDlg->SetRange(0, lParam);
				}
				//int nResult = g_ProgressDlg->LoopDialog();
				//delete g_ProgressDlg;
				//g_ProgressDlg = NULL;
				// 1=正常结束,0=用户cancel
				//return nResult;
				return 0;
			}
			// 已经有正在运行的对话框
			return -1;
		}

	case OC_PROGDLG_SETPOS:	// 设置进度
		{
			if(g_ProgressDlg != NULL)
			{
				g_ProgressDlg->SetPos(wParam);
			}

			MSG msg;
			if(::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{															
				TranslateMessage(&msg);										
				DispatchMessage(&msg);
			}

			return 0;
		}

	case OC_PROGDLG_SETSTATUS:	// 设置状态信息
		{
			if(g_ProgressDlg != NULL)
			{
				g_ProgressDlg->SetStatus(LPCTSTR(wParam));
			}

			MSG msg;
			if(::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{															
				TranslateMessage(&msg);										
				DispatchMessage(&msg);
			}

			return 0;
		}

	case OC_PROGDLG_END:	// 结束进度对话框
		{
			if(g_ProgressDlg != NULL)
			{
				g_ProgressDlg->EndDialog();
				delete g_ProgressDlg;
				g_ProgressDlg = NULL;
			}
			return 0;
		}

	case OC_PROGDLG_CANCEL:	// cancel进度对话框
		{
			if(g_ProgressDlg != NULL)
			{
				// 执行cancel脚本
				CString strScript = g_ProgressDlg->GetCancelScript();
				if(strScript != "")
				{
					CString strInterpInstance = _T("");

					// 解析解释器名(支持命名方式的解释器)
					if(strScript.Find("<<") == 0)
					{
						int nPos = strScript.Find(">>");
						if(nPos != -1)
						{
							strScript.Delete(0, 2);
							strInterpInstance = strScript.Left(nPos-2);
							strScript.Delete(0, nPos);
						}
					}

					CString strInterpName = "";	// 解释器名
					if(strScript.Find("<") == 0)
					{
						strScript.Delete(0, 1);
						int nPos = strScript.Find(">");
						if(nPos != -1)
						{
							strInterpName = strScript.Left(nPos);
							if(strInterpName.Find("org.interp") != 0)
							{
								strInterpName = "org.interp." + strInterpName;
							}
							strScript.Delete(0, nPos+1);
						}		
					}

					BOOL bCancel = TRUE;
					// 创建解释器并执行
					if(strInterpName.GetLength() > 0)
					{
						IInterp* pInterp = (IInterp*)(theApp.CreateVciObject(strInterpName, strInterpInstance));
						if(pInterp)
						{
							pInterp->RunScriptCommand(strScript);
							if(pInterp->GetResult() == "0")
								bCancel = FALSE;
							theApp.ReleaseObject(pInterp);
						}
					}

					if(!bCancel)
						return -1;
				}

				// 结束对话框
				g_ProgressDlg->EndDialog();
				delete g_ProgressDlg;
				g_ProgressDlg = NULL;
			}
			return 0;
		}

	case OC_PROGDLG_CANCELSCRIPT:	// 设置cancel脚本
		{
			if(g_ProgressDlg != NULL)
			{
				g_ProgressDlg->SetCancelScript((LPCTSTR)wParam);
			}
			return 0;
		}

	case OC_TASK_DIALOG:	// 通用任务对话框
		{
			if(wParam == 0)	// 执行TaskDialog
			{
				return theApp.DoTaskDialog((void*)lParam);
			}else
			if(wParam == 1)	// 刷新TaskDialog
			{
				return theApp.RefreshTaskDialog((void*)lParam);
			}else
			if(wParam == 2)	// 从XML加载并执行TaskDialog
			{
				return theApp.DoXmlTaskDialog((void*)lParam);
			}else
			if(wParam == 3)	// 删除所有向导页面
			{
				theApp.ClearTaskDialogPage();
			}else
			if(wParam == 4)	// 添加向导页面
			{
				return theApp.AddTaskDialogPage((void*)lParam);
			}else
			if(wParam == 5)	// 执行向导
			{
				return theApp.DoTaskDialogSheets();
			}
			return 0;
		}

	case OC_POPUP_CONTROL:	// 显示Popup窗口
		{
			theApp.ShowPopupControl((TPopupControlInfo*)lParam);
			return 0;
		}

	default:
		break;
	}

	return 0;
}


