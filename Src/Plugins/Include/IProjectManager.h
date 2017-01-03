#if !defined(__VCI_INTERFACE_IProjectManager_H_INCLUDED__)
#define __VCI_INTERFACE_IProjectManager_H_INCLUDED__



#define IID_IProjectManager "IProjectManager"

interface IProjectManager : public IUnknown
{
	//{{ 注意：!!!这里是保留的重要函数，不可删除!!!
	// 获取接口的额外信息
	virtual LPVOID __stdcall GetExtraInfo(LPVOID lpRefData) = 0;
	//}} 注意：!!!这里是保留的重要函数，不可删除!!!

	//{{VCI_INTERFACE_BEGIN
	// 启动新建工程向导
	virtual int __stdcall NewProjectWizard(CString& strPrjVci, CString& strPrjFile) = 0;
	// 启动新建文件向导
	virtual int __stdcall NewFileWizard() = 0;
	//}}VCI_INTERFACE_END
};


#endif // !defined(__VCI_INTERFACE_IProjectManager_H_INCLUDED__)
