#if !defined(AFX_OPTIONTREECTRL_H__0A6563AD_3666_47E3_BFB8_D1C3F2C90294__INCLUDED_)
#define AFX_OPTIONTREECTRL_H__0A6563AD_3666_47E3_BFB8_D1C3F2C90294__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionTreeCtrl.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// COptionTreeCtrl window

class COptionTreeCtrl : public CTreeCtrl
{
// Construction
public:
	COptionTreeCtrl();

// Attributes
public:
	CImageList	m_ImageList;

// Operations
public:
	void InitTree();
	HTREEITEM AddTreeItem(CString strText, HTREEITEM hParent, int nType, int nSelect=-1);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COptionTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(COptionTreeCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONTREECTRL_H__0A6563AD_3666_47E3_BFB8_D1C3F2C90294__INCLUDED_)
