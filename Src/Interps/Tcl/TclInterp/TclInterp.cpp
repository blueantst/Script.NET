// TclInterp.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "CTclInterp.h"
#include "TclPcom.h"
#include "TclXml.h"
#include "wait.h"
#include "TclDuiVision.h"
#include "resource.h"
#include "TpsTclInterp.h"
#include "TpsTclDebug.h"
#include "TpsTclCompile.h"
#include "TpsTclBuild.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE TclInterpDLL = { NULL, NULL };

HINSTANCE GetDllInstance()
{
	return TclInterpDLL.hModule;
}

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("TCLINTERP.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(TclInterpDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(TclInterpDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		CloseAllVciObject();

		CloseAllTclXmlFile();

		DeleteAllWaitObject();

		CloseAllDuiObject();

		TRACE0("TCLINTERP.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(TclInterpDLL);
	}
	return 1;   // ok
}

/////////////////////////////////////////////////////////////////////////////
// 资源切换函数
// get_dll_resource   切换到DLL的资源
// reset_dll_resource 恢复以前的资源
/////////////////////////////////////////////////////////////////////////////
static int resource_counter = 0;		// 资源计数
static HINSTANCE save_hInstance = NULL;	// 资源保存

void get_dll_resource(void)
{
   if (resource_counter == 0)
   {
	   save_hInstance = AfxGetResourceHandle();
	   AfxSetResourceHandle(GetDllInstance());
   }

   resource_counter++;
}

void reset_dll_resource(void)
{
   if (resource_counter > 0)
      resource_counter--;

   if (resource_counter == 0)
      AfxSetResourceHandle(save_hInstance);
}

//////////////////////////////////////////////////////////////////////////
// 全局变量
//////////////////////////////////////////////////////////////////////////
const TInterfaceInfo INTERFACE_LIST[] =
{
	INF_IInterp,
};


//////////////////////////////////////////////////////////////////////////
// 本 DLL 输出的函数
//////////////////////////////////////////////////////////////////////////
//
// 查询本 DLL 输出的接口列表，返回接口数目
//
extern "C" __declspec(dllexport)
int GetInterfaceList(const TInterfaceInfo **pInfo)
{
	*pInfo = INTERFACE_LIST;
	return (sizeof(INTERFACE_LIST)/sizeof(INTERFACE_LIST[0]));
}

//
// 注册属性页
//
extern "C" __declspec(dllexport)
int gRegisterPropertyPage(CVciPropertyPageArray& aPropertyPage)
{
	// 在此添加属性页
	DLL_LOADSTRING(strTpsTclInterp, IDS_TPS_TCL_INTERP);	// Tcl解释器
	DLL_LOADSTRING(strTpsAboutTclInterp, IDS_TPS_ABOUT_TCL_INTERP);	// 关于Tcl解释器...
	VCI_REGISTER_PROPERTYPAGE("", strTpsTclInterp, strTpsAboutTclInterp, IDD_TPS_TCLINTERP, CTpsTclInterp, 0)

	// 检查procomp.exe文件,如果没有,表示未安装完整版Tcl解释器,则调试、编译、构建等属性页不用显示
	CString strProCompFile	= GetInterpRootPath();
	strProCompFile	+= "bin\\procomp.exe";
	if(GetFileAttributes(strProCompFile) != 0xFFFFFFFF)
	{
		DLL_LOADSTRING(strTpsTclDebug, IDS_TPS_TCL_DEBUG);	// 调试参数
		DLL_LOADSTRING(strTpsAboutTclDebug, IDS_TPS_ABOUT_TCL_DEBUG);	// 设置Tcl调试参数...
		VCI_REGISTER_PROPERTYPAGE(strTpsTclInterp, strTpsTclDebug, strTpsAboutTclDebug, IDD_TPS_TCL_DEBUG, CTpsTclDebug, 0)
		DLL_LOADSTRING(strTpsTclComplie, IDS_TPS_TCL_COMPILE);	// 编译选项
		DLL_LOADSTRING(strTpsAboutTclCompile, IDS_TPS_ABOUT_TCL_COMPILE);	// 设置Tcl编译选项...
		VCI_REGISTER_PROPERTYPAGE(strTpsTclInterp, strTpsTclComplie, strTpsAboutTclCompile, IDD_TPS_TCL_COMPILE, CTpsTclCompile, 0)
		DLL_LOADSTRING(strTpsTclBuild, IDS_TPS_TCL_BUILD);	// 构建选项
		DLL_LOADSTRING(strTpsAboutTclBuild, IDS_TPS_ABOUT_TCL_BUILD);	// 设置Tcl构建选项...
		VCI_REGISTER_PROPERTYPAGE(strTpsTclInterp, strTpsTclBuild, strTpsAboutTclBuild, IDD_TPS_TCL_BUILD, CTpsTclBuild, 0)
	}

	return aPropertyPage.GetSize();
}

//
// 产生对象并返回对象的接口
//
extern "C" __declspec(dllexport)
LPVOID CreateObject(LPCSTR lpcsInterfaceName, LPVOID* lppVciControl, LPVOID lpInitData)
{
	TRACE("CreateObject(\"%s\")\n", lpcsInterfaceName);

	CVisualComponent *pObject;
	if(strcmp(lpcsInterfaceName, IID_IInterp) == 0)
	{
		pObject = new CTclInterp;
	}
	else
	{
		TRACE("Can't find Object: %s, CreateObject failed!\n", lpcsInterfaceName);
		return NULL;
	}

	// 为设备安装调试信息输出回调函数
	if(lpInitData)
	{
		TInitData_DebugOut *pInitData_DebugOut = (TInitData_DebugOut *)lpInitData;
		pObject->InstallHandler(DEBUG_OUT_EVENT, pInitData_DebugOut->lpfnDebugOut, pInitData_DebugOut->nDeviceID);
	}

	*lppVciControl = pObject->VciControlQueryInterface();

	return pObject->ExternalQueryInterface();
}
