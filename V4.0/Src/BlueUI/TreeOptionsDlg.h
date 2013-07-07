#if !defined(AFX_TREEOPTIONSDLG_H__3334A321_63D1_11D5_9673_00C0DF258498__INCLUDED_)
#define AFX_TREEOPTIONSDLG_H__3334A321_63D1_11D5_9673_00C0DF258498__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeOptionsDlg.h : header file
//

#include "ReportCtrl\\ReportCtrl.h"
#include "ReportCtrl\\ReportOptionsCtrl.h"
#include "Label.h"

class CTreeOptionsDlg : public CDialog
{
// Construction
public:
	CTreeOptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTreeOptionsDlg)
	enum { IDD = IDD_TREE_OPTIONS };
	CLabel	m_staticMsg;
	CButton	m_btnHelp;
	CButton	m_btnCancel;
	CButton	m_btnOK;
	CReportOptionsCtrl	m_rcCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList m_ilOptions;
	CImageList m_ilMarks;
	CTOptArray m_aTOpts;
	CTOptArray* m_paTOpts;
	CString m_strTitle;
	CString m_strMsg;
	CString m_strHelpFile;

	// Generated message map functions
	//{{AFX_MSG(CTreeOptionsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

/////////////////////////////
// Attributes
public:
	BOOL IsGroup(HTREEITEM hItem);
	BOOL IsCheckBox(HTREEITEM hItem);
	BOOL IsRadioButton(HTREEITEM hItem);
	BOOL IsEditable(HTREEITEM hItem);

	BOOL IsDisabled(HTREEITEM hItem);

	BOOL SetCheckBox(HTREEITEM hItem, BOOL bCheck);
	BOOL GetCheckBox(HTREEITEM hItem, BOOL& bCheck);

	BOOL SetRadioButton(HTREEITEM hParent, INT iIndex);
	BOOL SetRadioButton(HTREEITEM hItem);
	BOOL GetRadioButton(HTREEITEM hParent, INT& iIndex, HTREEITEM& hCheckItem);
	BOOL GetRadioButton(HTREEITEM hItem, BOOL& bCheck);

	BOOL SetGroupEnable(HTREEITEM hParent, BOOL bEnable);
	BOOL GetGroupEnable(HTREEITEM hParent, BOOL& bEnable);

	BOOL SetCheckBoxEnable(HTREEITEM hItem, BOOL bEnable);
	BOOL GetCheckBoxEnable(HTREEITEM hItem, BOOL& bEnable);

	BOOL SetRadioButtonEnable(HTREEITEM hItem, BOOL bEnable);
	BOOL GetRadioButtonEnable(HTREEITEM hItem, BOOL& bEnable);

	CString GetEditText(HTREEITEM hItem);
	void SetEditText(HTREEITEM hItem, LPCTSTR lpszText);

	CString GetComboText(HTREEITEM hItem);
	void SetComboText(HTREEITEM hItem, LPCTSTR lpszText);

	CString GetDateTimeText(HTREEITEM hItem);
	void SetDateTimeText(HTREEITEM hItem, LPCTSTR lpszText);

// Operations
public:
	void ResetAllItem();
	HTREEITEM InsertItem(LPCTSTR lpszItem, INT iImage, HTREEITEM hParent = RVTI_ROOT);
	HTREEITEM InsertGroup(LPCTSTR lpszItem, INT iImage, HTREEITEM hParent = RVTI_ROOT);
	HTREEITEM InsertCheckBox(LPCTSTR lpszItem, HTREEITEM hParent, BOOL bCheck = TRUE);
	HTREEITEM InsertRadioButton(LPCTSTR lpszItem, HTREEITEM hParent, BOOL bCheck = TRUE);
	BOOL AddEditBox(HTREEITEM hItem, LPCTSTR lpszText, CRuntimeClass* lpRuntimeClass = NULL);
	BOOL AddComboBoxEx(HTREEITEM hItem, LPCTSTR lpszText, CString strTextList, CRuntimeClass* lpRuntimeClass = NULL);
	BOOL AddDateTime(HTREEITEM hItem, LPCTSTR lpszText, CString strTextList, CRuntimeClass* lpRuntimeClass = NULL);
	void DeleteItem(HTREEITEM hItem);
	void SelectItem(HTREEITEM hItem);

	LPTOPTITEM GetItemHandle(LPCTSTR lpszItem);
	void SetTOptItem(LPTOPTITEM lpItem);

	HTREEITEM GetItem(int nItem);
	HTREEITEM InsertTOptItem(LPTOPTITEM lpItem);

// 对外接口
	void SetTOptArrayPoint(CTOptArray* paOpts);
	int  AddTOptItem(int iType, LPCTSTR lpszText, LPCTSTR lpszEdit, LPCTSTR lpszList="", BOOL bCheck=FALSE, int iImage=0, int iParent=-1);
	int  AddTOptMsg(LPCTSTR lpszTitle, LPCTSTR lpszMsg);
	int  AddTOptHelp(LPCTSTR lpszHelpFile);
	int  AddTOptItemCluster(int iType, LPCTSTR lpszText, BOOL bCheck, int iImage=0, int iParent=-1);
	int  AddTOptItemEdit(int iType, LPCTSTR lpszText, LPCTSTR lpszEdit, LPCTSTR lpszList="", int iImage=-1, int iParent=-1);

	BOOL GetCheckBox(LPCTSTR lpszItem);
	BOOL GetRadioButton(LPCTSTR lpszItem);
	CString GetEditText(LPCTSTR lpszItem);
	CString GetComboText(LPCTSTR lpszItem);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEOPTIONSDLG_H__3334A321_63D1_11D5_9673_00C0DF258498__INCLUDED_)
