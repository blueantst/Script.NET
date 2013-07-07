////////////////////////////////////////////////////////////////////////////
//	File:		TaskDialog.h
//	Version:	1.0.0.0
//	Created:	2007-06-15
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	PlugIn Interface
////////////////////////////////////////////////////////////////////////////
#ifndef __TASKDIALOG_H_
#define __TASKDIALOG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTaskDialogEx : public CXTPTaskDialog
{
public:
	CTaskDialogEx(CWnd* pWndParent)
		: CXTPTaskDialog(pWndParent)
	{

	}

	TTaskDialogInfo*	m_pInfo;// 信息指针

	void SaveAndSetCurPath(CString strURL);
	void RestoreCurPath();
	BOOL RunScript(CString strURL);

	char	m_szOldPath[256];
	DWORD	m_dwPathLen;

protected:
	virtual void OnDialogConstructed();
	virtual void OnTimer(DWORD dwMilliSeconds, BOOL& bReset);
	virtual void OnVerificationClicked(BOOL bChecked);
	virtual void OnButtonClicked(int nButtonID, BOOL& bCloseDialog);
	virtual void OnHyperlinkClicked(LPCTSTR pszURL);
};

#define ID_BUTTON_NEXT 101
#define ID_BUTTON_PREV 100

// 向导页面TaskDialog
class CTaskDialogPage : public CTaskDialogEx
{
public:
	CTaskDialogPage(CWnd* pWndParent = NULL)
		: CTaskDialogEx(pWndParent)
	{
		SetCommonButtons(TDCBF_CANCEL_BUTTON);
			
		m_pPagePrev = 0;
		m_pPageNext = 0;
	}

	CTaskDialogPage& operator = (const CTaskDialogPage& other)
	{
		m_pPagePrev		= other.m_pPagePrev;
		m_pPageNext		= other.m_pPageNext;
		return *this;
	}
		
	virtual void OnDialogConstructed()
	{
		EnableButton(ID_BUTTON_PREV, m_pPagePrev != NULL);
	}
	virtual void OnNavigated()
	{
		OnDialogConstructed();
			
	};
		
	void OnButtonClicked(int nButtonID, BOOL& bCloseDialog)
	{
		switch (nButtonID)
		{
		case ID_BUTTON_PREV: if (m_pPagePrev) NavigatePage(*m_pPagePrev); bCloseDialog = FALSE; break;
		case ID_BUTTON_NEXT: if (m_pPageNext) NavigatePage(*m_pPageNext);  bCloseDialog = FALSE; break;
		}
	}
public:
	CTaskDialogPage* m_pPagePrev;
	CTaskDialogPage* m_pPageNext;
};

typedef CArray<TTaskDialogInfo, TTaskDialogInfo&> CTaskDialogInfoArray;

#endif //__TASKDIALOG_H_
