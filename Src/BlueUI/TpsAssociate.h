#if !defined(AFX_TPSASSOCIATE_H__CB8B21B9_9A62_4D37_A2FA_171EFA15E6EA__INCLUDED_)
#define AFX_TPSASSOCIATE_H__CB8B21B9_9A62_4D37_A2FA_171EFA15E6EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsAssociate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTpsAssociate dialog

class CTpsAssociate : public CDialog
{
// Construction
public:
	CTpsAssociate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsAssociate)
	enum { IDD = IDD_TPS_ASSOCIATE };
	CXTPReportControl m_wndReport;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	virtual void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsAssociate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList m_ilIcon;

	// Generated message map functions
	//{{AFX_MSG(CTpsAssociate)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSASSOCIATE_H__CB8B21B9_9A62_4D37_A2FA_171EFA15E6EA__INCLUDED_)
