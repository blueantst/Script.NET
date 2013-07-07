// SearchOptions.h : header file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// 1998-2005 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEARCHOPTIONS_H__07FBB19A_B649_45CC_A248_0EE0D548E383__INCLUDED_)
#define AFX_SEARCHOPTIONS_H__07FBB19A_B649_45CC_A248_0EE0D548E383__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CSearchOptionsView form view

class CSearchOptions
{
public:
	CSearchOptions();

public:
	void DoPropExchange(CXTPPropExchange* pPX);

public:
	BOOL    m_bIncludeSubFolders;
	BOOL    m_bRegularExpressions;
	CString m_strFind;
	CString m_strReplace;
	CString m_strPath;
	CString m_strFileTypes;
	BOOL    m_bMatchCase;
	BOOL    m_bMatchWholeWords;
	
	CStringList m_lstFindHistory;
	CStringList m_lstReplaceHistory;
	CStringList m_lstPathHistory;
	CStringList m_lstFileTypesHistory;

	IRegExp* pRegExp;
};


class CSearchBrowseButton : public CXTBrowseButton
{
public:
	virtual void OnClicked();
	void Initialize(CComboBox* pParentWnd, DWORD dwBStyle, UINT nMenu=0, int nSubMenuIndx=0);

	CComboBox* m_pComboBox;
};

class CSearchThread;
class CSearchOptionsView : public CXTResizeFormView
{
protected:
	CSearchOptionsView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSearchOptionsView)

// Form Data
public:
	//{{AFX_DATA(CSearchOptionsView)
	enum { IDD = IDD_DIALOG_OPTIONS };
	CComboBox   m_cmbReplace;
	CComboBox   m_cmbPath;
	CComboBox   m_cmbFind;
	CComboBox   m_cmbFileTypes;
	CSearchBrowseButton m_btnReplaceExpand;
	CSearchBrowseButton m_btnFindExpand;
	CSearchBrowseButton m_btnPathExpand;
	CXTResizeGroupBox   m_wndGroupFindOptions;
	//}}AFX_DATA
	BOOL m_bInitialized;
// Attributes
public:
	void UpdateOptions();
	void SetSearchMode(BOOL bSearchMode);
	BOOL CheckReplaceEnable();

	struct SEARCH_RESULT
	{
		int nMatchingLines;
		int nMatchingFiles;
		int nTotalFiles;
	} m_searchResult;

// Operations
public:
	BOOL m_bSearchMode;
	CSearchThread* m_pSearchThread;

	void OnFindAll();
	void OnCancelFind();
	void OnReplaceAll();
	void OnFindDone();

	CSearchThread* GetSearchThread() {return m_pSearchThread;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSearchOptionsView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSearchOptionsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CSearchOptionsView)
	afx_msg void OnButtonFindAll();
	afx_msg void OnButtonReplaceAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCHOPTIONS_H__07FBB19A_B649_45CC_A248_0EE0D548E383__INCLUDED_)
