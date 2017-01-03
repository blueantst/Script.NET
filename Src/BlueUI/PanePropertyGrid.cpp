// PanePropertyGrid.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlueUI.h"
#include "PanePropertyGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPanePropertyGrid

CPanePropertyGrid::CPanePropertyGrid()
{
}

CPanePropertyGrid::~CPanePropertyGrid()
{
}

BEGIN_MESSAGE_MAP(CPanePropertyGrid, CWnd)
	//{{AFX_MSG_MAP(CPanePropertyGrid)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_PANEPROPERIES_CATEGORIZED, OnPaneproperiesCategorized)
	ON_UPDATE_COMMAND_UI(ID_PANEPROPERIES_CATEGORIZED, OnUpdatePaneproperiesCategorized)
	ON_COMMAND(ID_PANEPROPERIES_ALPHABETIC, OnPaneproperiesAlphabetic)
	ON_UPDATE_COMMAND_UI(ID_PANEPROPERIES_ALPHABETIC, OnUpdatePaneproperiesAlphabetic)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanePropertyGrid message handlers

int CPanePropertyGrid::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndPropertyGrid.Create(CRect(0, 0, 0, 0), this, 1000);

	m_wndPropertyGrid.SetOwner(GetTopLevelFrame());

	COLORREF clrMask = XTPImageManager()->SetMaskColor(RGB(0, 255, 0));
	VERIFY(m_wndToolBar.CreateToolBar(WS_TABSTOP|WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS, this));
	VERIFY(m_wndToolBar.LoadToolBar(IDR_PANE_PROPERTIES));
	XTPImageManager()->SetMaskColor(clrMask);

	// Enable Office 2007 Theme
	m_wndPropertyGrid.SetTheme(xtpGridThemeOffice2007);

	return 0;
}

void CPanePropertyGrid::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	int nTop = 0;

	if (m_wndToolBar.GetSafeHwnd())
	{
		CSize sz = m_wndToolBar.CalcDockingLayout(cx, /*LM_HIDEWRAP|*/ LM_HORZDOCK|LM_HORZ | LM_COMMIT);

		m_wndToolBar.MoveWindow(0, nTop, cx, sz.cy);
		m_wndToolBar.Invalidate(FALSE);
		nTop += sz.cy;
	}
	if (m_wndPropertyGrid.GetSafeHwnd())
	{
		m_wndPropertyGrid.MoveWindow(0, nTop, cx, cy - nTop);
		m_wndPropertyGrid.Invalidate(FALSE);
	}
}

void CPanePropertyGrid::OnPaneproperiesCategorized()
{
	m_wndPropertyGrid.SetPropertySort(xtpGridSortCategorized);
}

void CPanePropertyGrid::OnUpdatePaneproperiesCategorized(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropertyGrid.GetPropertySort() == xtpGridSortCategorized);
}

void CPanePropertyGrid::OnPaneproperiesAlphabetic()
{
	m_wndPropertyGrid.SetPropertySort(xtpGridSortAlphabetical);
}

void CPanePropertyGrid::OnUpdatePaneproperiesAlphabetic(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropertyGrid.GetPropertySort() == xtpGridSortAlphabetical);
}

void CPanePropertyGrid::OnSysColorChange()
{
	CWnd::OnSysColorChange();

	XTPPaintManager()->RefreshMetrics();
}

