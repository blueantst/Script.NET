////////////////////////////////////////////////////////////////////////////
//	File:		ILicense.h
//	Version:	2.0.0.0
//	Created:	2000-07-20
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	license interface define
////////////////////////////////////////////////////////////////////////////
#if !defined(__VCI_INTERFACE_ILicense_H_INCLUDED__)
#define __VCI_INTERFACE_ILicense_H_INCLUDED__


#define IID_ILicense "ILicense"

namespace License {
// License返回值定义
enum ETJResult
{
    trOk             = 0,                       //执行成功
	trLicenseFail	 = 100,						//License校验失败
	trLicenseExpire		,						//License过期
	trLicenseDriver		,						//没有相应驱动的License授权
	trLicenseUpdateExpire,						//License过升级期限,不能使用当前版本,但可以使用老版本
	
	trUnknownErr    	,	                    //未知错误
    //请在前面添加其他的返回码
    trLast                                     //最后的返回值
};

// License功能项动作定义
enum{
	FUNC_ACTION_LOAD	= 0,	// 组件加载
	FUNC_ACTION_CHECKLICENSE,	// 检查License有效性
	FUNC_ACTION_RUNFILE,		// 执行文件
	FUNC_ACTION_RUNCMD,			// 执行命令
	FUNC_ACTION_DEBUG,			// 调试
	FUNC_ACTION_COMPILE,		// 编译
	FUNC_ACTION_BUILD,			// 构建
	FUNC_ACTION_TEST,			// 测试
	FUNC_ACTION_REFACTOR,		// 重构
	FUNC_ACTION_UIDESIGN,		// 界面可视化设计
	FUNC_ACTION_MODEL,			// 建模
	FUNC_ACTION_WEBDEV,			// WEB开发
};


// License类型定义
enum{
	LICENSE_NORMAL,				// 正式License
	LICENSE_TRIAL,				// 试用License
	LICENSE_ERROR,				// 错误License
	LICENSE_FREE,				// Free License
};
};

interface ILicense : public IUnknown
{
	//{{ 注意：!!!这里是保留的重要函数，不可删除!!!
	// 获取接口的额外信息
	virtual LPVOID __stdcall GetExtraInfo(LPVOID lpRefData) = 0;
	//}} 注意：!!!这里是保留的重要函数，不可删除!!!

	//{{VCI_INTERFACE_BEGIN
	// 应用程序关于对话框
	virtual int __stdcall ApplicationAbout() = 0;
	// 组件的关于对话框
	virtual int __stdcall ComponentAbout(LPCTSTR lpszVciId) = 0;
	// 校验License组件
	virtual int __stdcall VerifyLicenseComponent(int nCmd, LPCTSTR lpszParamIn, CString& strParamOut) = 0;
	// 插件的License校验
	virtual int __stdcall VerifyPluginLicense(LPCTSTR lpszVciId, int nAction) = 0;
	// 校验产品License
	virtual int __stdcall VerifyProductLicense() = 0;
	// 校验功能License
	virtual int __stdcall VerifyFunctionLicense(int nFunction, int nValue) = 0;
	// 获取产品序列号
	virtual int __stdcall GetProductNo(CString& strProductNo) = 0;
	// 获取升级请求字符串
	virtual int __stdcall GetUpgradeRequestString(CString& strRequest) = 0;
	// 升级License文件
	virtual int __stdcall UpgradeLicenseFile(LPCTSTR lpszUpgradeFile) = 0;
	// 获取硬件唯一序列号
	virtual int __stdcall GetESN(CString& strESN) = 0;
	// 生成License ID
	virtual int __stdcall GenerateLicenseID(LPCTSTR lpszESN, LPCTSTR lpszMaskCode, CString& strLicenseID) = 0;
	// 导入License ID
	virtual int __stdcall ImportLicenseID(LPCTSTR lpszLicenseID) = 0;
	// 校验License ID
	virtual int __stdcall VerifyLicenseID(LPCTSTR lpszFunction) = 0;
	// 获取License ID
	virtual int __stdcall GetLicenseID(CString& strLicenseID) = 0;
	// 获取插件的License信息
	virtual int __stdcall GetPluginLicenseInfo(LPCTSTR lpszVciId, int& nLicenseType, CTime& tUpdateLimit, int& nUserCount, CTime& tTrialTime) = 0;
	// 获取授权用户信息
	virtual int __stdcall GetCustomerInfo(CString& strCustomerInfo) = 0;
	// 获取订单信息
	virtual int __stdcall GetOrderInfo(CString& strOrderInfo) = 0;
	//}}VCI_INTERFACE_END
};


#endif // !defined(__VCI_INTERFACE_ILicense_H_INCLUDED__)
