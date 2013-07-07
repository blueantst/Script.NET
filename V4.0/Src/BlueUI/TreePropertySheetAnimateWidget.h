// TreePropertySheetAnimateWidget.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTreePropertySheetAnimateWidget window

class CTreePropertySheetAnimateWidget : public CTreePropertySheetInterface, public CWnd
{
// Construction
public:
	CTreePropertySheetAnimateWidget(CWnd *pParentWnd,COLORREF clrLeft1,COLORREF clrLeft2,COLORREF clrRight1,COLORREF clrRight2,int nSteps);

// Attributes
public:
	COLORREF		m_clrLeft1;
	COLORREF		m_clrLeft2;
	COLORREF		m_clrRight1;
	COLORREF		m_clrRight2;
	int				m_nSteps;
	int				m_nStep;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreePropertySheetAnimateWidget)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTreePropertySheetAnimateWidget();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTreePropertySheetAnimateWidget)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
