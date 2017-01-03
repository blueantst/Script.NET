#if !defined(AFX_TPSNEWDOCUMENT_H__D4B70679_83D2_42E4_AFDB_8C3C2FD438FF__INCLUDED_)
#define AFX_TPSNEWDOCUMENT_H__D4B70679_83D2_42E4_AFDB_8C3C2FD438FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsNewDocument.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTpsNewDocument dialog

class CTpsNewDocument : public CDialog
{
// Construction
public:
	CTpsNewDocument(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTpsNewDocument)
	enum { IDD = IDD_TPS_NEWDOC };
	int		m_nEncoding;
	int		m_nFormat;
	//}}AFX_DATA

	virtual void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsNewDocument)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsNewDocument)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSNEWDOCUMENT_H__D4B70679_83D2_42E4_AFDB_8C3C2FD438FF__INCLUDED_)
