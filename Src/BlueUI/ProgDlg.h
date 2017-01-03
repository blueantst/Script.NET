// ProgDlg.h : header file
// CG: This file was added by the Progress Dialog component

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog

#ifndef __PROGDLG_H__
#define __PROGDLG_H__

#include "TextProgressCtrl.h"

class CProgressDlg : public CDialog
{
// Construction / Destruction
public:
    CProgressDlg(CString strCaption = "");   // standard constructor
    ~CProgressDlg();

    BOOL Create(CWnd *pParent=NULL);

    // Checking for Cancel button
    BOOL CheckCancelButton();
    // Progress Dialog manipulation
    void SetStatus(LPCTSTR lpszMessage);
    void SetRange(int nLower,int nUpper);
    int  SetStep(int nStep);
    int  SetPos(int nPos);
    int  OffsetPos(int nPos);
    int  StepIt();
	void EndDialog();
	int  LoopDialog();

	void SetCancelScript(CString strScript) { m_strCancelScript = strScript; };
	CString GetCancelScript() { return m_strCancelScript; }
        
// Dialog Data
    //{{AFX_DATA(CProgressDlg)
    enum { IDD = CG_IDD_PROGRESS };
    CTextProgressCtrl	m_Progress;
	CXTButton	m_btnCancel;
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CProgressDlg)
    public:
    virtual BOOL DestroyWindow();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
	CString m_strCaption;
    int m_nLower;
    int m_nUpper;
    int m_nStep;
    
    BOOL m_bCancel;
	BOOL m_bEnd;
    BOOL m_bParentDisabled;

	CString m_strCancelScript;

    void ReEnableParent();

    virtual void OnCancel();
    virtual void OnOK() {}; 
    void PumpMessages();

    // Generated message map functions
    //{{AFX_MSG(CProgressDlg)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif // __PROGDLG_H__
