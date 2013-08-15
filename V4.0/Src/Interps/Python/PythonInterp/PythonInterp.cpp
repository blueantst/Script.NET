// PythonInterp.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "CPythonInterp.h"
#include "TpsPythonInterp.h"
#include "TpsPythonCompile.h"
#include "TpsPythonBuild.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE PythonInterpDLL = { NULL, NULL };

HINSTANCE GetDllInstance()
{
	return PythonInterpDLL.hModule;
}

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("PYTHONINTERP.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(PythonInterpDLL, hInstance))
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

		new CDynLinkLibrary(PythonInterpDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("PYTHONINTERP.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(PythonInterpDLL);
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
	DLL_LOADSTRING(strTpsPythonInterp, IDS_TPS_PYTHON_INTERP);	// Python解释器
	DLL_LOADSTRING(strTpsAboutPythonInterp, IDS_TPS_ABOUT_PYTHON_INTERP);	// 关于Python解释器...
	VCI_REGISTER_PROPERTYPAGE("", strTpsPythonInterp, strTpsAboutPythonInterp, IDD_TPS_PYTHONINTERP, CTpsPythonInterp, 0)

	DLL_LOADSTRING(strTpsPythonComplie, IDS_TPS_PYTHON_COMPILE);	// 编译选项
	DLL_LOADSTRING(strTpsAboutPythonCompile, IDS_TPS_ABOUT_PYTHON_COMPILE);	// 设置Python编译选项...
	VCI_REGISTER_PROPERTYPAGE(strTpsPythonInterp, strTpsPythonComplie, strTpsAboutPythonCompile, IDD_TPS_PYTHON_COMPILE, CTpsPythonCompile, 0);

	DLL_LOADSTRING(strTpsPythonBuild, IDS_TPS_PYTHON_BUILD);	// 构建选项
	DLL_LOADSTRING(strTpsAboutPythonBuild, IDS_TPS_ABOUT_PYTHON_BUILD);	// 设置Python构建选项...
	VCI_REGISTER_PROPERTYPAGE(strTpsPythonInterp, strTpsPythonBuild, strTpsAboutPythonBuild, IDD_TPS_PYTHON_BUILD, CTpsPythonBuild, 0)

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
		pObject = new CPythonInterp;
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
