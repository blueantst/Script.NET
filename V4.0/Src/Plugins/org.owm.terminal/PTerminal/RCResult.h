//----------------------------------------------------------------------------
//Name            : RCResult.h
//Function        : RemoteControl函数返回值声明文件
//Date            : 2002-08-10
//Author          : 
//Modification    :
//----------------------------------------------------------------------------

#ifndef RC_RESULT_H
#define RC_RESULT_H

#define      ENGLISH_STRINGTABLE             0
#define      CHINESE_STRINGTABLE             1

//RemoteControl函数返回值
enum ERCResult
{
    trOk             = 0,                       //执行成功
    trUnconnected       ,                       //网络未连接
    trUnlogin           ,                       //未登录
    trNetFail           ,	                    //网络不通
    trUserNotFound      ,	                    //未找到用户
    trPassWordErr       ,	                    //口令不对
	trRemoteDeviceOpenErr,                      //远程设备打开失败
	trReceiveTimeOut    ,                       //接收数据超时

    //请在前面添加其他的返回码
    trLast                                     //最后的返回值
};

const TCHAR* GetResultString(ERCResult eResult, UINT iLanguage = ENGLISH_STRINGTABLE);


#endif // end of RC_RESULT_H