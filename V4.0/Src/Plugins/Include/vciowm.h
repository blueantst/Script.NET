////////////////////////////////////////////////////////////////////////////
//	File:		VciOwm.h
//	Version:	1.0.0.0
//	Created:	2005-07-08
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	VCI OWM Interface
////////////////////////////////////////////////////////////////////////////
#ifndef __VCIOWM_H_
#define __VCIOWM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define IID_IVciOwm "IVciOwm"

interface IVciOwm : public IUnknown
{
	//{{ 注意：!!!这里是保留的重要函数，不可删除!!!
	// 获取接口的额外信息
	virtual LPVOID __stdcall GetExtraInfo(LPVOID lpRefData) = 0;
	//}} 注意：!!!这里是保留的重要函数，不可删除!!!

	// 获取模块名
	virtual int __stdcall GetOwmName(CString& strOwmName) = 0;
	// 获取模块描述
	virtual int __stdcall GetOwmDesc(CString& strOwmDesc) = 0;
	// 获取模块版本
	virtual int __stdcall GetOwmVersion(int& nVersion) = 0;
	// 获取模块类型
	virtual int __stdcall GetOwmType(UINT& nType) = 0;
	// 获取模块ID
	virtual int __stdcall GetOwmID(UINT& nID) = 0;
	// 设置平台UI接口指针
	virtual int __stdcall SetIPlatUI(LPVOID lpIPlatUI) = 0;
	// 设置文档模板指针
	virtual int __stdcall SetDocTemplate(CMultiDocTemplate* pDocTemplate) = 0;
	// 获取文档模板指针
	virtual int __stdcall GetDocTemplate(CMultiDocTemplate** ppDocTemplate) = 0;
	// 获取指定运行时类指针
	virtual int __stdcall GetRuntimeClass(int nType, CRuntimeClass** ppRuntimeClass) = 0;
	// 显示DockingPane窗口时调用的函数
	virtual CWnd* __stdcall OnShowDockingPane(int nID, CWnd* pParentWnd, LPCTSTR lpszParam) = 0;
	// OWM命令处理
	virtual int __stdcall Command(int nCmd, WPARAM wParam, LPARAM lParam) = 0;
	// 注册菜单
	virtual int __stdcall RegisterClientMenu(CMenu* pMenu, COWMMenuArray& aOWMMenus, int nOWMIdStart, CDocument* pDoc) = 0;
	// 卸载菜单
	virtual int __stdcall UnregisterClientMenu(COWMMenuArray& aOWMMenus) = 0;
	// 注册ToolBar
	virtual int __stdcall RegisterClientToolBar(CWnd* pBar, COWMMenuArray& aOWMMenus, int nOWMIdStart, int nIndex) = 0;
	// 卸载ToolBar
	virtual int __stdcall UnregisterClientToolBar(CWnd* pBar, COWMMenuArray& aOWMMenus, int nOWMIdStart) = 0;
	// 注册OWM属性页
	virtual int __stdcall RegisterOption(COWMOptionArray& aOWMOption) = 0;
	// 注册打印页面设置属性页
	virtual int __stdcall RegisterPrintPageOption(COWMOptionArray& aOWMOption) = 0;
	// 加载文档Session
	virtual int __stdcall LoadDocumentSession(CDocument* pDoc, CStringArray& asSessionLine) = 0;
	// 保存文档Session
	virtual int __stdcall SaveDocumentSession(CDocument* pDoc, CStringArray& asSessionLine) = 0;
	// 备份文档
	virtual int __stdcall BackupDocument(CDocument* pDoc, LPCTSTR lpszBackupFile, int nBackupType) = 0;
	// 获取导入类型
	virtual int __stdcall GetImportTypeList(CStringArray& asImportType) = 0;
	// 获取导出类型
	virtual int __stdcall GetExportTypeList(CStringArray& asExportType) = 0;
	// 导入文档
	virtual int __stdcall ImportDocument(CDocument* pDoc, LPCTSTR lpszImportSource, int nImportType) = 0;
	// 导出文档
	virtual int __stdcall ExportDocument(CDocument* pDoc, LPCTSTR lpszExportDest, int nExportType) = 0;
	// 缩放(pDoc为空表示对所有文档)
	virtual int __stdcall Zoom(float fZoomFactor, CDocument* pDoc) = 0;
	// 获取当前缩放因子(pDoc为空表示对所有文档)
	virtual int __stdcall GetZoomFactor(float& fZoomFactor, CDocument* pDoc) = 0;
	// 获取缩放列表
	virtual int __stdcall GetZoomFactorRange(CStringArray& asZoomFactor, float& fZoomMin, float& fZoomMax) = 0;
};


#endif // __VCIOWM_H_