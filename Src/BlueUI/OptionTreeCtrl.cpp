// OptionTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "BlueUI.h"
#include "OptionTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionTreeCtrl

COptionTreeCtrl::COptionTreeCtrl()
{
}

COptionTreeCtrl::~COptionTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(COptionTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(COptionTreeCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// ³õÊ¼»¯Ê÷
/////////////////////////////////////////////////////////////////////////////
void COptionTreeCtrl::InitTree()
{
	m_ImageList.Create(16, 16, ILC_MASK|ILC_COLOR24, 0, 1);
	CBitmap bmp;
	VERIFY(bmp.LoadBitmap(IDB_OPTIONTREE));
	m_ImageList.Add(&bmp, CXTPImageManager::GetBitmapMaskColor(bmp, CPoint(0, 0)));

	SetImageList( &m_ImageList, TVSIL_NORMAL );
}

HTREEITEM COptionTreeCtrl::AddTreeItem(CString strText, HTREEITEM hParent, int nType, int nSelect)
{
	HTREEITEM hItem;       // return value
	TV_ITEM tvI;           // item structure
	TV_INSERTSTRUCT tvIns; // item insert structure

	DWORD dwStyle = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	
	tvI.mask			= dwStyle;
	tvI.pszText			= strText.GetBuffer(1);
	tvI.cchTextMax		= strText.GetLength();
	tvI.iImage			= nType;
	if(nSelect == -1)
		tvI.iSelectedImage	= nType;
	else
		tvI.iSelectedImage	= nSelect;

	// allow one more level down
	tvI.cChildren		= 1;
	
	// do the insert
	tvIns.item			= tvI;
	tvIns.hInsertAfter	= TVI_LAST;
	tvIns.hParent		= hParent;
	hItem				= InsertItem(&tvIns);

	return( hItem ); // return (add error trap here)
}
/////////////////////////////////////////////////////////////////////////////
// COptionTreeCtrl message handlers
