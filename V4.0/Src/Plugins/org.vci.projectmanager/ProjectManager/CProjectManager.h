#if !defined(__VCI_INTERFACE_CProjectManager_H_INCLUDED__)
#define __VCI_INTERFACE_CProjectManager_H_INCLUDED__


#include "vcicomm.h"
#include "IProjectManager.h"

#define INF_IProjectManager \
{\
	"IProjectManager",\
	"1.0.0.0",\
	THIS_REVISION_TYPE,\
	"{6F20ED82-A398-44CF-A45B-1E1906DA2C6A}",\
	"Project manager",\
	"blueant",\
	__DATE__\
}
const TInterfaceInfo INTERFACE_INFO_IProjectManager = INF_IProjectManager;

class CProjectManager : public CVisualComponent
{
public:
	CProjectManager();
	~CProjectManager();

//////////////////////////////////////////////////////////////////////////
// 提供给接口的功能函数
//////////////////////////////////////////////////////////////////////////
public:
	//{{VCI_IMPLEMENT_DEFINE_BEGIN
	// 启动新建工程向导
	virtual int __stdcall NewProjectWizard(CString& strPrjVci, CString& strPrjFile);
	// 启动新建文件向导
	virtual int __stdcall NewFileWizard();
	//}}VCI_IMPLEMENT_DEFINE_END

	virtual int __stdcall InstallHandler(DWORD nEventID, LPVOID lpHandler, DWORD dwRefData);
	virtual int __stdcall UninstallHandler(DWORD nEventID, LPVOID lpHandler);
	virtual int __stdcall ProcessMessage(CVciMessage* pIn, CVciMessage* ppOut);

//////////////////////////////////////////////////////////////////////////
// 要输出的功能接口
//////////////////////////////////////////////////////////////////////////
protected:
	// 接口定义
	BEGIN_INTERFACE_PART(VciControl, IVciControl)
		CONTROL_INTERFACE_DEFINE
	END_INTERFACE_PART(VciControl)
	EXPORT_CONTROL_INTERFACE_PART(VciControl)

	BEGIN_INTERFACE_PART(ProjectManager, IProjectManager)
		//{{ 注意：!!!这里是保留的重要函数，不可删除!!!
		// 获取接口的额外信息
		virtual LPVOID __stdcall GetExtraInfo(LPVOID lpRefData);
		//}} 注意：!!!这里是保留的重要函数，不可删除!!!
	//{{VCI_INTERFACE_PART_BEGIN
	// 启动新建工程向导
	virtual int __stdcall NewProjectWizard(CString& strPrjVci, CString& strPrjFile);
	// 启动新建文件向导
	virtual int __stdcall NewFileWizard();
	//}}VCI_INTERFACE_PART_END
	END_INTERFACE_PART(ProjectManager)
	EXPORT_INTERFACE_PART(ProjectManager)
};


#endif // !defined(__VCI_INTERFACE_CProjectManager_H_INCLUDED__)
