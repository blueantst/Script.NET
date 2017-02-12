// OwmSciteView.cpp : implementation of the COwmSciteView class
//

#include "stdafx.h"
#include "OwmScite.h"
#include "OwmSciteDoc.h"
#include "OwmSciteView.h"
#include "Parameters.h"
#include "ScitillaComponent/ScintillaEditView.h"
#include "ScitillaComponent/xmlMatchedTagsHighlighter.h"
#include "Utf8_16.h"
#include "UniConversion.h"
#include "ScitillaComponent/Printer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COwmSciteView

IMPLEMENT_DYNCREATE(COwmSciteView, CView)

BEGIN_MESSAGE_MAP(COwmSciteView, CView)
	//{{AFX_MSG_MAP(COwmSciteView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
	ON_COMMAND(ID_COMMENT_SELECT, OnCommentSelect)
	ON_UPDATE_COMMAND_UI(ID_COMMENT_SELECT, OnUpdateCommentSelect)
	ON_COMMAND(ID_UNCOMMENT_SELECT, OnUncommentSelect)
	ON_UPDATE_COMMAND_UI(ID_UNCOMMENT_SELECT, OnUpdateUncommentSelect)
	ON_COMMAND(ID_COPY_AS_RTF, OnCopyAsRtf)
	ON_UPDATE_COMMAND_UI(ID_COPY_AS_RTF, OnUpdateCopyAsRtf)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFind)
	ON_COMMAND(ID_EDIT_REPEAT, OnEditRepeat)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPEAT, OnUpdateEditRepeat)
	ON_COMMAND(ID_EDIT_REPLACE, OnEditReplace)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, OnUpdateEditReplace)
	ON_COMMAND(ID_EDIT_BOOKMARKS, OnEditBookmarks)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BOOKMARKS, OnUpdateEditBookmarks)
	ON_COMMAND(ID_EDIT_CLEARBOOKMARKS, OnEditClearbookmarks)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEARBOOKMARKS, OnUpdateEditClearbookmarks)
	ON_COMMAND(ID_COMPILE, OnCompile)
	ON_UPDATE_COMMAND_UI(ID_COMPILE, OnUpdateCompile)
	ON_COMMAND(ID_BUILD, OnBuild)
	ON_UPDATE_COMMAND_UI(ID_BUILD, OnUpdateBuild)
	ON_COMMAND(ID_BUILD_AND_RUN, OnBuildAndRun)
	ON_UPDATE_COMMAND_UI(ID_BUILD_AND_RUN, OnUpdateBuildAndRun)
	ON_COMMAND(ID_GO, OnGo)
	ON_UPDATE_COMMAND_UI(ID_GO, OnUpdateGo)
	ON_COMMAND(ID_OWM_RUN, OnOwmRun)
	ON_UPDATE_COMMAND_UI(ID_OWM_RUN, OnUpdateOwmRun)
	ON_COMMAND(ID_SCRIPT_EVAL, OnScriptEval)
	ON_UPDATE_COMMAND_UI(ID_SCRIPT_EVAL, OnUpdateScriptEval)
	ON_COMMAND(ID_OWM_EXECUTE, OnOwmExecute)
	ON_UPDATE_COMMAND_UI(ID_OWM_EXECUTE, OnUpdateOwmExecute)
	ON_COMMAND(ID_STOP_RUN, OnStopRun)
	ON_UPDATE_COMMAND_UI(ID_STOP_RUN, OnUpdateStopRun)
	ON_COMMAND(ID_STEP_INTO, OnStepInto)
	ON_UPDATE_COMMAND_UI(ID_STEP_INTO, OnUpdateStepInto)
	ON_COMMAND(ID_STEP_OVER, OnStepOver)
	ON_UPDATE_COMMAND_UI(ID_STEP_OVER, OnUpdateStepOver)
	ON_COMMAND(ID_STEP_OUT, OnStepOut)
	ON_UPDATE_COMMAND_UI(ID_STEP_OUT, OnUpdateStepOut)
	ON_COMMAND(ID_RUN_TOCURSOR, OnRunTocursor)
	ON_UPDATE_COMMAND_UI(ID_RUN_TOCURSOR, OnUpdateRunTocursor)
	ON_COMMAND(ID_HIDE_LINES, OnHideLines)
	ON_UPDATE_COMMAND_UI(ID_HIDE_LINES, OnUpdateHideLines)
	ON_COMMAND(ID_INSERT_BREAK, OnInsertBreak)
	ON_UPDATE_COMMAND_UI(ID_INSERT_BREAK, OnUpdateInsertBreak)
	ON_COMMAND(ID_CLEAR_BREAKALL, OnClearBreakall)
	ON_UPDATE_COMMAND_UI(ID_CLEAR_BREAKALL, OnUpdateClearBreakall)
	ON_COMMAND(ID_CONDITION_BREAK, OnConditionBreak)
	ON_UPDATE_COMMAND_UI(ID_CONDITION_BREAK, OnUpdateConditionBreak)
	ON_COMMAND(ID_WATCH_BREAKS, OnWatchBreaks)
	ON_UPDATE_COMMAND_UI(ID_WATCH_BREAKS, OnUpdateWatchBreaks)
	ON_COMMAND(ID_WATCH_VARS, OnWatchVars)
	ON_UPDATE_COMMAND_UI(ID_WATCH_VARS, OnUpdateWatchVars)
	ON_COMMAND(ID_WATCH_OBJECT, OnWatchObject)
	ON_UPDATE_COMMAND_UI(ID_WATCH_OBJECT, OnUpdateWatchObject)
	ON_COMMAND(ID_WATCH_PROC, OnWatchProc)
	ON_UPDATE_COMMAND_UI(ID_WATCH_PROC, OnUpdateWatchProc)
	ON_COMMAND(ID_WATCH_CALLSTACK, OnWatchCallstack)
	ON_UPDATE_COMMAND_UI(ID_WATCH_CALLSTACK, OnUpdateWatchCallstack)
	ON_COMMAND(ID_WORD_HELP, OnWordHelp)
	ON_UPDATE_COMMAND_UI(ID_WORD_HELP, OnUpdateWordHelp)
	ON_COMMAND(ID_DOC_PROPERTY, OnDocProperty)
	ON_UPDATE_COMMAND_UI(ID_DOC_PROPERTY, OnUpdateDocProperty)
	ON_COMMAND(ID_OWM_ABOUT, OnOwmAbout)
	ON_UPDATE_COMMAND_UI(ID_OWM_ABOUT, OnUpdateOwmAbout)
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_IN, OnUpdateZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_OUT, OnUpdateZoomOut)
	ON_COMMAND(ID_ZOOM_RESTORE, OnZoomRestore)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_RESTORE, OnUpdateZoomRestore)
	ON_COMMAND(ID_EDIT_AUTOCOMPLETE, OnEditAutocomplete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_AUTOCOMPLETE, OnUpdateEditAutocomplete)
	ON_COMMAND(ID_EDIT_COMPLETE_KEYWORD, OnEditCompleteKeyword)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COMPLETE_KEYWORD, OnUpdateEditCompleteKeyword)
	ON_COMMAND(ID_EDIT_FUNCCALLTIP, OnEditFunccalltip)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FUNCCALLTIP, OnUpdateEditFunccalltip)
	ON_COMMAND(ID_EDIT_BOOKMARK_NEXT, OnEditBookmarkNext)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BOOKMARK_NEXT, OnUpdateEditBookmarkNext)
	ON_COMMAND(ID_EDIT_BOOKMARK_PREV, OnEditBookmarkPrev)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BOOKMARK_PREV, OnUpdateEditBookmarkPrev)
	ON_COMMAND(ID_EDIT_UPPERCASE, OnEditUppercase)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UPPERCASE, OnUpdateEditUppercase)
	ON_COMMAND(ID_EDIT_LOWERCASE, OnEditLowercase)
	ON_UPDATE_COMMAND_UI(ID_EDIT_LOWERCASE, OnUpdateEditLowercase)
	ON_COMMAND(ID_FORMAT_CONV2_ANSI, OnFormatConv2Ansi)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_CONV2_ANSI, OnUpdateFormatConv2Ansi)
	ON_COMMAND(ID_FORMAT_CONV2_AS_UTF_8, OnFormatConv2AsUtf8)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_CONV2_AS_UTF_8, OnUpdateFormatConv2AsUtf8)
	ON_COMMAND(ID_FORMAT_CONV2_UCS_2BE, OnFormatConv2Ucs2be)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_CONV2_UCS_2BE, OnUpdateFormatConv2Ucs2be)
	ON_COMMAND(ID_FORMAT_CONV2_UCS_2LE, OnFormatConv2Ucs2le)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_CONV2_UCS_2LE, OnUpdateFormatConv2Ucs2le)
	ON_COMMAND(ID_FORMAT_CONV2_UTF_8, OnFormatConv2Utf8)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_CONV2_UTF_8, OnUpdateFormatConv2Utf8)
	ON_COMMAND(ID_FORMAT_TOWIN, OnFormatTowin)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_TOWIN, OnUpdateFormatTowin)
	ON_COMMAND(ID_FORMAT_TOMAC, OnFormatTomac)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_TOMAC, OnUpdateFormatTomac)
	ON_COMMAND(ID_FORMAT_TOUNIX, OnFormatTounix)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_TOUNIX, OnUpdateFormatTounix)
	ON_COMMAND(ID_WRAP, OnWrap)
	ON_UPDATE_COMMAND_UI(ID_WRAP, OnUpdateWrap)
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_WATCH_BOOKMARKS, OnWatchBookmarks)
	ON_UPDATE_COMMAND_UI(ID_WATCH_BOOKMARKS, OnUpdateWatchBookmarks)
	ON_COMMAND(ID_EDIT_CODESNIPPET, OnEditCodesnippet)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CODESNIPPET, OnUpdateEditCodesnippet)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	// Standard printing commands
	//ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	//	Status
	ON_UPDATE_COMMAND_UI(ID_EDIT_INDICATOR_READONLY, OnUpdateIndicatorReadOnly)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INDICATOR_CRLF, OnUpdateIndicatorCRLF)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INDICATOR_UNIMODE, OnUpdateIndicatorUniMode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INDICATOR_POSITION, OnUpdateIndicatorPosition)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COwmSciteView construction/destruction

COwmSciteView::COwmSciteView() : _autoComplete(&m_SciEdit), _codeSnippet(&m_SciEdit)
{
	// TODO: add construction code here
	m_hAccel = NULL;
	m_pInterp = NULL;
	m_bSlaveDebug = FALSE;

	_isFileOpening = FALSE;
	_linkTriggered = TRUE;
	_isDocModifing = FALSE;

	m_LangType = (LangType)L_UNKNOWN;

	m_bExtAutoCompletionMode = FALSE;
	m_ptAutoComStartPos   = CPoint(0, 0);
	m_pAutoComPopupWnd    = NULL;

	// 初始化Tooltip对象,并设置为Markup类型
	m_pTooltipContext = new CXTPToolTipContext();
	m_pTooltipContext->SetModuleToolTipContext();
	m_pTooltipContext->SetStyle(xtpToolTipMarkup);
	m_pTooltipContext->ModifyToolTipStyle(XTP_TTS_OFFICEFRAME, XTP_TTS_OFFICE2007FRAME);
	m_pTooltipContext->ShowImage(TRUE);
	//m_pTooltipContext->SetDelayTime(TTDT_INITIAL,   100);//   100ms   
	//m_pTooltipContext->SetDelayTime(TTDT_AUTOPOP,   1000*1);//   10s   
	//m_pTooltipContext->SetDelayTime(TTDT_RESHOW,   100);//   100ms   
	//m_pTooltipContext->SetMaxTipWidth(400);
	m_strLastTooltipWord = "";
	m_strLastTooltipTip = "";
	m_strLastTooltipDesc = "";
	m_strLastTooltipLibrary = "";
	m_strLastTooltipVarName = "";
	m_strLastTooltipVarInfo = "";
}

COwmSciteView::~COwmSciteView()
{
	delete m_pTooltipContext;
}

BOOL COwmSciteView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// COwmSciteView drawing

void COwmSciteView::OnDraw(CDC* pDC)
{
	COwmSciteDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// COwmSciteView printing

void COwmSciteView::OnFilePrintPreview() 
{
	//DoPrintPreview (this);
}

void COwmSciteView::OnFilePrint() 
{
	Printer printer;

	int startPos = int(m_SciEdit.execute(SCI_GETSELECTIONSTART));
	int endPos = int(m_SciEdit.execute(SCI_GETSELECTIONEND));
	
	printer.init(AfxGetStaticModuleState()->m_hCurrentInstanceHandle, m_hWnd, &m_SciEdit, true, startPos, endPos);
	printer.doPrint();
}

BOOL COwmSciteView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return TRUE;
}

void COwmSciteView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COwmSciteView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// COwmSciteView diagnostics

#ifdef _DEBUG
void COwmSciteView::AssertValid() const
{
	CView::AssertValid();
}

void COwmSciteView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COwmSciteDoc* COwmSciteView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COwmSciteDoc)));
	return (COwmSciteDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COwmSciteView message handlers
void COwmSciteView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	get_dll_resource();
	CMenu* pMenu = new CMenu();
	pMenu->LoadMenu(IDR_EDITOR_POPUP);
	reset_dll_resource();
	CMenu* pmenu = (CMenu*)pMenu->GetSubMenu(0);
	theApp.m_pIPlatUI->TrackPopupMenu(pmenu, 0, point.x, point.y, this);
	delete pMenu;
}


//////////////////////////////////////////////////////////////////////////////
// 窗口切换时调用
void COwmSciteView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if (bActivate)
	{
		// 在属性窗口显示图像信息
		GetDocument()->RefreshDocProperty();
	}

	if (bActivate)
	{
		get_dll_resource();
		CMenu* pMenuDebug = new CMenu();
		pMenuDebug->LoadMenu(IDR_OWMEDIT_DEBUG);
		CMenu* pMenuEdit = new CMenu();
		pMenuEdit->LoadMenu(IDR_OWMEDIT_EDIT);
		reset_dll_resource();
		theApp.m_pIPlatUI->AddExtMenu(pMenuDebug);
		theApp.m_pIPlatUI->AddExtMenu(pMenuEdit);
	}else
	{
		get_dll_resource();
		CMenu* pMenuDebug = new CMenu();
		pMenuDebug->LoadMenu(IDR_OWMEDIT_DEBUG);
		CMenu* pMenuEdit = new CMenu();
		pMenuEdit->LoadMenu(IDR_OWMEDIT_EDIT);
		reset_dll_resource();
		theApp.m_pIPlatUI->DelExtMenu(pMenuDebug);
		theApp.m_pIPlatUI->DelExtMenu(pMenuEdit);
	}

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void COwmSciteView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	
	// 重载WM_SETFOCUS,然后把消息传递到scintilla,否则此消息无法传递,导致不能自动激活编辑器焦点
	::SetFocus(m_SciEdit.getHSelf());
}

int COwmSciteView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_SciEdit.init(AfxGetApp()->m_hInstance,m_hWnd);
	m_SciEdit.display();

	NppParameters *pNppParam = NppParameters::getInstance();
	NppGUI & nppGUI = (NppGUI &)pNppParam->getNppGUI();
	const ScintillaViewParams & svp1 = pNppParam->getSVP(SCIV_PRIMARY);

	m_SciEdit.showMargin(ScintillaEditView::_SC_MARGE_LINENUMBER, svp1._lineNumberMarginShow);		// 显示行号
	m_SciEdit.showMargin(ScintillaEditView::_SC_MARGE_SYBOLE, svp1._bookMarkMarginShow);			// 显示符号
	m_SciEdit.showMargin(ScintillaEditView::_SC_MARGE_MODIFMARKER, svp1._docChangeStateMarginShow);	// 显示修改标记
	m_SciEdit.showIndentGuideLine(svp1._indentGuideLineShow);	// 显示边界线
	m_SciEdit.setMakerStyle(FOLDER_STYLE_BOX);			// 代码折叠标志的类型
	m_SciEdit.execute(SCI_SETCARETLINEVISIBLE, svp1._currentLineHilitingShow);	// 当前行高亮模式
	m_SciEdit.execute(SCI_SETCARETLINEVISIBLEALWAYS, true);
	m_SciEdit.wrap(svp1._doWrap);						// 折行显示
	m_SciEdit.execute(SCI_SETEDGECOLUMN, svp1._edgeNbColumn);//0x00000064);	// 边界宽度(默认100)
	m_SciEdit.execute(SCI_SETEDGEMODE, svp1._edgeMode);	// 边界模式(边界线模式=0,背景色模式=1)
	m_SciEdit.showEOL(svp1._eolShow);						// 显示行尾符
	m_SciEdit.showWSAndTab(svp1._whiteSpaceShow);			// 显示空格与制表符
	m_SciEdit.showWrapSymbol(svp1._wrapSymbolShow);			// 显示换行符(折行的行尾)
	m_SciEdit.execute(SCI_SETZOOM, svp1._zoom);				// 缩放比例(默认0)
	// 设置光标宽度
	if (nppGUI._caretWidth < 4)
	{
		m_SciEdit.execute(SCI_SETCARETWIDTH, nppGUI._caretWidth);
	}else
	{
		m_SciEdit.execute(SCI_SETCARETWIDTH, 1);
		m_SciEdit.execute(SCI_SETCARETSTYLE, CARETSTYLE_BLOCK);
	}
	// 设置光标刷新速率
	m_SciEdit.execute(SCI_SETCARETPERIOD, nppGUI._caretBlinkRate);

	// 不使用Scintilla的右键菜单
	m_SciEdit.execute(SCI_USEPOPUP, false);

	//m_SciEdit.execute(SCI_SETHSCROLLBAR, false);
	//m_SciEdit.execute(SCI_SETVSCROLLBAR, false);

	m_SciEdit.performGlobalStyles();

	// 加载加速键资源
	get_dll_resource();
	m_hAccel = ::LoadAccelerators(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_DEFAULT_ACCEL));
	reset_dll_resource();

	// Create the extend auto-complete list box control	
	// Note the absence of the visibility WS_VISIBLE flag
	m_listPopup.Create(LBS_NOTIFY | WS_CHILD | LBS_STANDARD | WS_VSCROLL
		| LBS_OWNERDRAWFIXED | LBS_HASSTRINGS,
			CRect(0, 0, 150, 200), this, IDC_EDIT_LIST);

	return 0;
}

BOOL COwmSciteView::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		if (m_hAccel != NULL)
		{
			if (::TranslateAccelerator(m_hWnd, m_hAccel, pMsg))
				return TRUE;
		}
	}

	if (pMsg->message == WM_MOUSEMOVE)
	{
		m_pTooltipContext->FilterToolTipMessage(this, pMsg);
	}

	return CView::PreTranslateMessage(pMsg);
}

void COwmSciteView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	RECT rc;
	::GetClientRect(m_hWnd, &rc);
	m_SciEdit.reSizeTo(rc);
}

void COwmSciteView::OnDestroy() 
{
	CView::OnDestroy();	
}

BOOL COwmSciteView::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (HIWORD(wParam) == SCEN_SETFOCUS)
    {
		HWND hMain = m_SciEdit.getHSelf();
		HWND hFocus = (HWND)lParam;
		if (hMain == hFocus)
		{
			m_SciEdit.getFocus();
		}
		return TRUE;
    }
	return CView::OnCommand(wParam, lParam);
}

BOOL COwmSciteView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	SCNotification* notification = reinterpret_cast<SCNotification *>(lParam);
	ScintillaEditView * notifyView = &m_SciEdit;

	switch (notification->nmhdr.code) 
	{
		case SCN_MODIFIED:
		{
			static bool prevWasEdit = false;
			if (notification->modificationType & (SC_MOD_DELETETEXT | SC_MOD_INSERTTEXT))
			{
				prevWasEdit = true;
				_linkTriggered = TRUE;
				_isDocModifing = TRUE;
				::InvalidateRect(notifyView->getHSelf(), NULL, TRUE);
			}

			if (notification->modificationType & SC_MOD_CHANGEFOLD)
			{
				if (prevWasEdit) {
					notifyView->foldChanged(notification->line,
							notification->foldLevelNow, notification->foldLevelPrev);
					prevWasEdit = false;
				}
			}
			else if (!(notification->modificationType & (SC_MOD_DELETETEXT | SC_MOD_INSERTTEXT)))
			{
				prevWasEdit = false;
			}

			if (!_isFileOpening && m_SciEdit.hasMarginShowed(ScintillaEditView::_SC_MARGE_MODIFMARKER))
			{
				bool isProcessed = false;

				int fromLine = m_SciEdit.execute(SCI_LINEFROMPOSITION, notification->position);
				pair<size_t, bool> undolevel = m_SciEdit.getLineUndoState(fromLine);

				if ((notification->modificationType & (SC_MOD_DELETETEXT | SC_MOD_INSERTTEXT)) &&
					(notification->modificationType & SC_PERFORMED_USER))
				{
					//printStr(TEXT("user type"));
					
					m_SciEdit.setLineUndoState(fromLine, undolevel.first+1);

					m_SciEdit.execute(SCI_MARKERADD, fromLine, MARK_LINEMODIFIEDUNSAVED);
					m_SciEdit.execute(undolevel.second?SCI_MARKERADD:SCI_MARKERDELETE, fromLine, MARK_LINEMODIFIEDSAVED);


					if (notification->linesAdded > 0)
					{
						for (int i = 0 ; i < notification->linesAdded ; i++)
						{
							++fromLine;
							m_SciEdit.execute(SCI_MARKERADD, fromLine, MARK_LINEMODIFIEDUNSAVED);
							pair<size_t, bool> modifInfo = m_SciEdit.getLineUndoState(fromLine);
							m_SciEdit.execute(modifInfo.second?SCI_MARKERADD:SCI_MARKERDELETE, fromLine, MARK_LINEMODIFIEDSAVED);
						}
					}
				}

				if ((notification->modificationType & (SC_MOD_DELETETEXT | SC_MOD_INSERTTEXT)) &&
					(notification->modificationType & SC_PERFORMED_REDO) &&
					(notification->modificationType & SC_MULTISTEPUNDOREDO))
				{
					//printStr(TEXT("redo multiple"));
					isProcessed = true;

					m_SciEdit.setLineUndoState(fromLine, undolevel.first+1);

					m_SciEdit.execute(SCI_MARKERADD, fromLine, MARK_LINEMODIFIEDUNSAVED);
					if (notification->linesAdded > 0)
					{
						for (int i = 0 ; i < notification->linesAdded ; i++)
						{
							++fromLine;
							m_SciEdit.execute(SCI_MARKERADD, fromLine, MARK_LINEMODIFIEDUNSAVED);
							pair<size_t, bool> modifInfo = m_SciEdit.getLineUndoState(fromLine);
							m_SciEdit.execute(modifInfo.second?SCI_MARKERADD:SCI_MARKERDELETE, fromLine, MARK_LINEMODIFIEDSAVED);
						}
					}
				}

				if ((notification->modificationType & (SC_MOD_DELETETEXT | SC_MOD_INSERTTEXT)) &&
					(notification->modificationType & SC_PERFORMED_UNDO) &&
					(notification->modificationType & SC_MULTISTEPUNDOREDO))
				{
					//printStr(TEXT("undo multiple"));
					isProcessed = true;

					--undolevel.first;
					if (undolevel.first == 0)
					{
						m_SciEdit.execute(SCI_MARKERDELETE, fromLine, MARK_LINEMODIFIEDUNSAVED);
					}
					else
					{
						m_SciEdit.execute(SCI_MARKERADD, fromLine, MARK_LINEMODIFIEDUNSAVED);
					}
					m_SciEdit.execute(undolevel.second?SCI_MARKERADD:SCI_MARKERDELETE, fromLine, MARK_LINEMODIFIEDSAVED);
					m_SciEdit.setLineUndoState(fromLine, undolevel.first);

					if (notification->linesAdded > 0)
					{
						for (int i = fromLine + 1 ; i < fromLine + notification->linesAdded ; i++)
						{
							pair<size_t, bool> level = m_SciEdit.getLineUndoState(i);
							if (level.first > 0)
								m_SciEdit.execute(SCI_MARKERADD, i, MARK_LINEMODIFIEDUNSAVED);
							m_SciEdit.execute(level.second?SCI_MARKERADD:SCI_MARKERDELETE, fromLine, MARK_LINEMODIFIEDSAVED);
						}
					}
				}

				if ((notification->modificationType & (SC_MOD_DELETETEXT | SC_MOD_INSERTTEXT)) &&
					(notification->modificationType & SC_PERFORMED_REDO) &&
					(notification->modificationType & SC_LASTSTEPINUNDOREDO) && !isProcessed)
				{
					//printStr(TEXT("redo LASTO"));
					m_SciEdit.setLineUndoState(fromLine, undolevel.first+1);

					m_SciEdit.execute(SCI_MARKERADD, fromLine, MARK_LINEMODIFIEDUNSAVED);
					m_SciEdit.execute(undolevel.second?SCI_MARKERADD:SCI_MARKERDELETE, fromLine, MARK_LINEMODIFIEDSAVED);

					if (notification->linesAdded > 0)
					{
						for (int i = 0 ; i < notification->linesAdded ; i++)
						{
							++fromLine;
							m_SciEdit.execute(SCI_MARKERADD, fromLine, MARK_LINEMODIFIEDUNSAVED);
							pair<size_t, bool> modifInfo = m_SciEdit.getLineUndoState(fromLine);
							m_SciEdit.execute(modifInfo.second?SCI_MARKERADD:SCI_MARKERDELETE, fromLine, MARK_LINEMODIFIEDSAVED);
						}
					}
				}

				if ((notification->modificationType & (SC_MOD_DELETETEXT | SC_MOD_INSERTTEXT)) &&
					(notification->modificationType & SC_PERFORMED_UNDO) &&
					(notification->modificationType & SC_LASTSTEPINUNDOREDO) && !isProcessed)
				{
					//printStr(TEXT("undo LASTO"));
					--undolevel.first;
					if (undolevel.first == 0)
					{
						m_SciEdit.execute(SCI_MARKERDELETE, fromLine, MARK_LINEMODIFIEDUNSAVED);
					}
					else
					{
						m_SciEdit.execute(SCI_MARKERADD, fromLine, MARK_LINEMODIFIEDUNSAVED);
					}
					m_SciEdit.execute(undolevel.second?SCI_MARKERADD:SCI_MARKERDELETE, fromLine, MARK_LINEMODIFIEDSAVED);
					m_SciEdit.setLineUndoState(fromLine, undolevel.first);

					if (notification->linesAdded > 0)
					{
						for (int i = fromLine + 1 ; i < fromLine + notification->linesAdded ; i++)
						{
							pair<size_t, bool> level = m_SciEdit.getLineUndoState(i);
							if (level.first > 0)
								m_SciEdit.execute(SCI_MARKERADD, i, MARK_LINEMODIFIEDUNSAVED);
							m_SciEdit.execute(level.second?SCI_MARKERADD:SCI_MARKERDELETE, fromLine, MARK_LINEMODIFIEDSAVED);
						}
					}
				}
			}
			break;
		}

		case SCN_SAVEPOINTREACHED:
		{	
			COwmSciteDoc* pDoc = GetDocument();
			pDoc->SetModifiedFlag(FALSE);
			break;
		}
		case SCN_SAVEPOINTLEFT:
		{
			COwmSciteDoc* pDoc = GetDocument();
			pDoc->SetModifiedFlag(TRUE);
			break;
		}
		
		case  SCN_MODIFYATTEMPTRO :
		{
			// on fout rien
			break;
		}

		case SCN_KEY:
		{
			break;
		}
		
		/*case TCN_TABDROPPEDOUTSIDE:
		case TCN_TABDROPPED:
		{
			break;
		}
		
		case TCN_TABDELETE:
		{
			break;
		}*/
		
		case TCN_SELCHANGE:
		{
			break;
		}
		
		case NM_CLICK :
		{
			break;
		}
		
		case NM_DBLCLK :
		{
			break;
		}
		
		case NM_RCLICK :
		{
			break;
		}
		
		case SCN_MARGINCLICK:
		{
			if (notification->margin == ScintillaEditView::_SC_MARGE_FOLDER)
			{
				m_SciEdit.marginClick(notification->position, notification->modifiers);
			}else
			if ((notification->margin == ScintillaEditView::_SC_MARGE_SYBOLE) && !notification->modifiers)
		    {
				int lineClick = int(m_SciEdit.execute(SCI_LINEFROMPOSITION, notification->position));
				if (!m_SciEdit.markerMarginClick(lineClick))
				{
					//bookmarkToggle(lineClick);
					linemarkToggle(MARK_BREAKPOINT, lineClick);
				}
			}
			break;
		}
		
		case SCN_CHARADDED:
		{
			charAdded(static_cast<TCHAR>(notification->ch));
			AutoCompletion * autoC = &_autoComplete;
			autoC->update(notification->ch);
			break;
		}
		
		case SCN_DOUBLECLICK :
		{
			break;
		}
		
		case SCN_UPDATEUI:
		{
			if (notification->nmhdr.hwndFrom != m_SciEdit.getHSelf())
				break;

			// 自动符号配对匹配并高亮
			braceMatch();

			// XML等文件自动匹配标签
			XmlMatchedTagsHighlighter xmlTagMatchHiliter(&m_SciEdit, m_LangType);
			xmlTagMatchHiliter.tagMatch(TRUE);

			// 智能高亮(例如有多个case语句时可以全部高亮)
			//_smartHighlighter.highlightView(notifyView);

			AutoCompletion * autoC = &_autoComplete;
			autoC->update(0);
			break;
		}
		
		case SCN_SCROLLED:
		{
			break;
		}
		
		case TTN_GETDISPINFO:
		{ 
			break;
		}
		
	    case SCN_ZOOM:
		{
			// 刷新属性窗口信息
			GetDocument()->RefreshDocProperty();
			break;
		}
		
	    case SCN_MACRORECORD:
		{
			break;
		}
		
		case SCN_PAINTED:
		{
			notifyView->updateLineNumberWidth();
			//if (_syncInfo.doSync()) 
			//	doSynScorll(HWND(notification->nmhdr.hwndFrom));

			// if it's searching/replacing, then do nothing
			/*if (_linkTriggered && !nppParam->_isFindReplacing)
			{
				int urlAction = (NppParameters::getInstance())->getNppGUI()._styleURL;
				if ((urlAction == 1) || (urlAction == 2))
					addHotSpot(_isDocModifing);
				_linkTriggered = false;
				_isDocModifing = false;
			}*/
			break;
		}
		
		case SCN_HOTSPOTDOUBLECLICK :
		{
			break;
		}

		case SCN_NEEDSHOWN :
		{
			break;
		}

		case SCN_CALLTIPCLICK:
		{
			AutoCompletion * autoC = &_autoComplete;
			autoC->callTipClick(notification->position);
			break;
		}

		case RBN_HEIGHTCHANGE:
		{
			break;
		}
	
		/*case RBN_CHEVRONPUSHED:
		{
			break;
		}*/
		default :
		{
			break;
		}
	}
	//m_SciEdit.Notify(reinterpret_cast<SCNotification *>(lParam));
	return CView::OnNotify(wParam, lParam, pResult);
}

// 括号匹配
void COwmSciteView::findMatchingBracePos(int & braceAtCaret, int & braceOpposite)
{
	int caretPos = int(m_SciEdit.execute(SCI_GETCURRENTPOS));
	braceAtCaret = -1;
	braceOpposite = -1;
	TCHAR charBefore = '\0';
	//TCHAR styleBefore = '\0';
	int lengthDoc = int(m_SciEdit.execute(SCI_GETLENGTH));

	if ((lengthDoc > 0) && (caretPos > 0)) 
    {
		charBefore = TCHAR(m_SciEdit.execute(SCI_GETCHARAT, caretPos - 1, 0));
	}
	// Priority goes to character before caret
	if (charBefore && generic_strchr(TEXT("[](){}"), charBefore))
    {
		braceAtCaret = caretPos - 1;
	}

	if (lengthDoc > 0  && (braceAtCaret < 0)) 
    {
		// No brace found so check other side
		TCHAR charAfter = TCHAR(m_SciEdit.execute(SCI_GETCHARAT, caretPos, 0));
		if (charAfter && generic_strchr(TEXT("[](){}"), charAfter))
        {
			braceAtCaret = caretPos;
		}
	}
	if (braceAtCaret >= 0) 
		braceOpposite = int(m_SciEdit.execute(SCI_BRACEMATCH, braceAtCaret, 0));
}

void COwmSciteView::braceMatch() 
{
	int braceAtCaret = -1;
	int braceOpposite = -1;
	findMatchingBracePos(braceAtCaret, braceOpposite);

	if ((braceAtCaret != -1) && (braceOpposite == -1))
    {
		m_SciEdit.execute(SCI_BRACEBADLIGHT, braceAtCaret);
		m_SciEdit.execute(SCI_SETHIGHLIGHTGUIDE);
	} 
    else 
    {
		m_SciEdit.execute(SCI_BRACEHIGHLIGHT, braceAtCaret, braceOpposite);

		if (m_SciEdit.isShownIndentGuide())
        {
            int columnAtCaret = int(m_SciEdit.execute(SCI_GETCOLUMN, braceAtCaret));
		    int columnOpposite = int(m_SciEdit.execute(SCI_GETCOLUMN, braceOpposite));
			m_SciEdit.execute(SCI_SETHIGHLIGHTGUIDE, (columnAtCaret < columnOpposite)?columnAtCaret:columnOpposite);
        }
    }
}

// 缩进排版支持
void COwmSciteView::charAdded(TCHAR chAdded)
{
	bool indentMaintain = NppParameters::getInstance()->getNppGUI()._maitainIndent;
	if (indentMaintain)
		MaintainIndentation(chAdded);
}

void COwmSciteView::MaintainIndentation(TCHAR ch)
{
	int eolMode = int(m_SciEdit.execute(SCI_GETEOLMODE));
	int curLine = int(m_SciEdit.getCurrentLineNumber());
	int lastLine = curLine - 1;
	int indentAmount = 0;

	if (((eolMode == SC_EOL_CRLF || eolMode == SC_EOL_LF) && ch == '\n') ||
	        (eolMode == SC_EOL_CR && ch == '\r')) 
	{
		while (lastLine >= 0 && m_SciEdit.getLineLength(lastLine) == 0)
			lastLine--;

		if (lastLine >= 0) {
			indentAmount = m_SciEdit.getLineIndent(lastLine);
		}
		if (indentAmount > 0) {
			m_SciEdit.setLineIndent(curLine, indentAmount);
		}
	}
}

// 行标记相关操作
void COwmSciteView::linemarkAdd(int marktype, int lineno, bool bSingle) const
 {
	if(bSingle)
	{
		// 如果只允许一个此类型的标记,就先删除所有标记
		linemarkClearAll(marktype);
	}
	if (lineno == -1)
		lineno = m_SciEdit.getCurrentLineNumber();
	if (!linemarkPresent(marktype, lineno))
		m_SciEdit.execute(SCI_MARKERADD, lineno, marktype);
}

void COwmSciteView::linemarkDelete(int marktype, int lineno) const
{
	if (lineno == -1)
		lineno = m_SciEdit.getCurrentLineNumber();
	if ( linemarkPresent(marktype, lineno))
		m_SciEdit.execute(SCI_MARKERDELETE, lineno, marktype);
}

bool COwmSciteView::linemarkPresent(int marktype, int lineno) const 
{
	if (lineno == -1)
		lineno = m_SciEdit.getCurrentLineNumber();
	LRESULT state = m_SciEdit.execute(SCI_MARKERGET, lineno);
	return ((state & (1 << marktype)) != 0);
}

void COwmSciteView::linemarkToggle(int marktype, int lineno) const
{
	if (lineno == -1)
	{
		lineno = m_SciEdit.getCurrentLineNumber();
	}
	
	if (linemarkPresent(marktype, lineno))
	{
		linemarkDelete(marktype, lineno);
	}
	else
	{
		linemarkAdd(marktype, lineno);
	}
}

void COwmSciteView::linemarkClearAll(int marktype) const
{
	m_SciEdit.execute(SCI_MARKERDELETEALL, marktype);
}

// 获取所有指定标记的行号
int COwmSciteView::linemarkGetAll(int marktype, CUIntArray& auMarkLine)
{
	int nLineNo = -1;
	while((nLineNo = m_SciEdit.execute(SCI_MARKERNEXT, nLineNo, (1<<marktype))) != -1)
	{
		auMarkLine.Add(nLineNo+1);
		nLineNo++;
	}
	return auMarkLine.GetSize();
}

// 书签相关操作
void COwmSciteView::bookmarkAdd(int lineno) const
 {
	if (lineno == -1)
		lineno = m_SciEdit.getCurrentLineNumber();
	if (!bookmarkPresent(lineno))
		m_SciEdit.execute(SCI_MARKERADD, lineno, MARK_BOOKMARK);
}

void COwmSciteView::bookmarkDelete(int lineno) const
{
	if (lineno == -1)
		lineno = m_SciEdit.getCurrentLineNumber();
	if ( bookmarkPresent(lineno))
		m_SciEdit.execute(SCI_MARKERDELETE, lineno, MARK_BOOKMARK);
}

bool COwmSciteView::bookmarkPresent(int lineno) const 
{
	if (lineno == -1)
		lineno = m_SciEdit.getCurrentLineNumber();
	LRESULT state = m_SciEdit.execute(SCI_MARKERGET, lineno);
	return ((state & (1 << MARK_BOOKMARK)) != 0);
}

void COwmSciteView::bookmarkToggle(int lineno) const
{
	if (lineno == -1)
	{
		lineno = m_SciEdit.getCurrentLineNumber();
	}
	
	if (bookmarkPresent(lineno))
	{
		bookmarkDelete(lineno);
	}
	else
	{
		bookmarkAdd(lineno);
	}
}

void COwmSciteView::bookmarkClearAll() const
{
	m_SciEdit.execute(SCI_MARKERDELETEALL, MARK_BOOKMARK);
}

void COwmSciteView::bookmarkNext(bool forwardScan) 
{
	int lineno = m_SciEdit.getCurrentLineNumber();
	int sci_marker = SCI_MARKERNEXT;
	int lineStart = lineno + 1;	//Scan starting from next line
	int lineRetry = 0;				//If not found, try from the beginning
	if (!forwardScan) 
    {
		lineStart = lineno - 1;		//Scan starting from previous line
		lineRetry = int(m_SciEdit.execute(SCI_GETLINECOUNT));	//If not found, try from the end
		sci_marker = SCI_MARKERPREVIOUS;
	}
	int nextLine = int(m_SciEdit.execute(sci_marker, lineStart, 1 << MARK_BOOKMARK));
	if (nextLine < 0)
		nextLine = int(m_SciEdit.execute(sci_marker, lineRetry, 1 << MARK_BOOKMARK));

	if (nextLine < 0)
		return;

    m_SciEdit.execute(SCI_ENSUREVISIBLEENFORCEPOLICY, nextLine);
	m_SciEdit.execute(SCI_GOTOLINE, nextLine);
}

/////////////////////////////////////////////////////////////////////////////
// 查找并设置当前行
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::SearchLine(CString strLine, int nMarkType)
{
	TextToFind ttf;
	ttf.chrg.cpMin = 0;
	ttf.chrg.cpMax = m_SciEdit.getCurrentDocLen();
	ttf.lpstrText = strLine.GetBuffer(0);
	ttf.chrgText.cpMin = 0;
	ttf.chrgText.cpMax = 0;
	int nFindPos = m_SciEdit.execute(SCI_FINDTEXT, SCFIND_MATCHCASE, (long)(&ttf));
	strLine.ReleaseBuffer();

	if(nFindPos != -1)
	{
		m_SciEdit.execute(SCI_GOTOPOS, nFindPos);
		int currentLine = m_SciEdit.getCurrentLineNumber();
		linemarkAdd(MARK_CURRENTLINE, currentLine, true);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 刷新状态行

// 只读状态
void COwmSciteView::OnUpdateIndicatorReadOnly(CCmdUI* pCmdUI)
{
	BOOL bReadOnly = m_SciEdit.execute(SCI_GETREADONLY);
	if(bReadOnly)
	{
		DLL_LOADSTRING(strReadOnly, IDS_INDICATOR_READONLY);
		pCmdUI->SetText(strReadOnly);//_T("只读"));
	}
	else
		pCmdUI->SetText(_T(""));
	pCmdUI->Enable(TRUE);
}

// 行列显示
void COwmSciteView::OnUpdateIndicatorPosition(CCmdUI* pCmdUI)
{
	CString stat;
	DLL_LOADSTRING(strIndicatorPos, IDS_INDICATOR_POS);	// _T("行:%d, 列:%d")
	stat.Format(strIndicatorPos,
			m_SciEdit.getCurrentLineNumber() + 1,
			m_SciEdit.getCurrentColumnNumber() + 1);
	//m_SciEdit.getSelectedByteNumber()
	pCmdUI->SetText(stat);
}

// 文件类型(Windows/UNIX/MAC)
void COwmSciteView::OnUpdateIndicatorCRLF(CCmdUI* pCmdUI)
{
	formatType crlfMode = m_SciEdit.getFormat();
	pCmdUI->SetText(m_SciEdit.getFormatText(crlfMode));
	pCmdUI->Enable(TRUE);
}

// 文件Unicode模式
void COwmSciteView::OnUpdateIndicatorUniMode(CCmdUI* pCmdUI)
{
	UniMode uniMode = m_SciEdit.getUnicodeMode();
	pCmdUI->SetText(m_SciEdit.getUniModeText(uniMode));
	pCmdUI->Enable(TRUE);
}

static bool isInList(const TCHAR *token, const TCHAR *list)
{
	if ((!token) || (!list))
		return false;
	TCHAR word[64];
	int i = 0;
	int j = 0;
	for (; i <= int(lstrlen(list)) ; i++)
	{
		if ((list[i] == ' ')||(list[i] == '\0'))
		{
			if (j != 0)
			{
				word[j] = '\0';
				j = 0;
				
				if (!generic_stricmp(token, word))
					return true;
			}
		}
		else 
		{
			word[j] = list[i];
			j++;
		}
	}
	return false;
}

LangType COwmSciteView::getLangFromExt(const TCHAR *ext)
{
	NppParameters *pNppParam = NppParameters::getInstance();
	int i = pNppParam->getNbLang();
	i--;
	while (i >= 0)
	{
		Lang *l = pNppParam->getLangFromIndex(i--);
		
		const TCHAR *defList = l->getDefaultExtList();
		const TCHAR *userList = NULL;
		
		LexerStylerArray &lsa = pNppParam->getLStylerArray();
		const TCHAR *lName = l->getLangName();
		LexerStyler *pLS = lsa.getLexerStylerByName(lName);
		
		if (pLS)
			userList = pLS->getLexerUserExt();
		
		std::generic_string list(TEXT(""));
		if (defList)
			list += defList;
		if (userList)
		{
			list += TEXT(" ");
			list += userList;
		}
		if (isInList(ext, list.c_str()))
			return l->getLangID();
	}
	return L_TXT;
}

CString COwmSciteView::getDocIdFromLangType(LangType langType)
{
	CString strDocId = "doc.edit.txt";
	switch(langType)
	{
	case L_PHP:strDocId = "doc.edit.php";break;
	case L_C:strDocId = "doc.edit.c";break;
	case L_CPP:strDocId = "doc.edit.cpp";break;
	case L_CS:strDocId = "doc.edit.cs";break;
	case L_OBJC:strDocId = "doc.edit.objc";break;
	case L_JAVA:strDocId = "doc.edit.java";break;
	case L_RC:strDocId = "doc.edit.rc";break;
	case L_HTML:strDocId = "doc.edit.html";break;
	case L_XML:strDocId = "doc.edit.xml";break;
	case L_MAKEFILE:strDocId = "doc.edit.makefile";break;
	case L_PASCAL:strDocId = "doc.edit.pas";break;
	case L_BATCH:strDocId = "doc.edit.bat";break;
	case L_INI:strDocId = "doc.edit.ini";break;
	case L_NFO:strDocId = "doc.edit.nfo";break;
	case L_USER:strDocId = "doc.edit.user";break;
	case L_ASP:strDocId = "doc.edit.asp";break;
	case L_SQL:strDocId = "doc.edit.sql";break;
	case L_VB:strDocId = "doc.edit.vb";break;
	case L_JS:strDocId = "doc.edit.js";break;
	case L_CSS:strDocId = "doc.edit.css";break;
	case L_PERL:strDocId = "doc.edit.perl";break;
	case L_PYTHON:strDocId = "doc.edit.python";break;
	case L_LUA:strDocId = "doc.edit.lua";break;
	case L_TEX:strDocId = "doc.edit.tex";break;
	case L_FORTRAN:strDocId = "doc.edit.fortran";break;
	case L_BASH:strDocId = "doc.edit.bash";break;
	case L_FLASH:strDocId = "doc.edit.flash";break;
	case L_NSIS:strDocId = "doc.edit.nsis";break;
	case L_TCL:strDocId = "doc.edit.tcl";break;
	case L_LISP:strDocId = "doc.edit.lisp";break;
	case L_SCHEME:strDocId = "doc.edit.scheme";break;
	case L_ASM:strDocId = "doc.edit.asm";break;
	case L_DIFF:strDocId = "doc.edit.diff";break;
	case L_PROPS:strDocId = "doc.edit.props";break;
	case L_PS:strDocId = "doc.edit.ps";break;
	case L_RUBY:strDocId = "doc.edit.ruby";break;
	case L_SMALLTALK:strDocId = "doc.edit.smalltalk";break;
	case L_VHDL:strDocId = "doc.edit.vhdl";break;
	case L_KIX:strDocId = "doc.edit.kix";break;
	case L_AU3:strDocId = "doc.edit.au3";break;
	case L_CAML:strDocId = "doc.edit.caml";break;
	case L_ADA:strDocId = "doc.edit.ada";break;
	case L_VERILOG:strDocId = "doc.edit.verilog";break;
	case L_MATLAB:strDocId = "doc.edit.matlab";break;
	case L_HASKELL:strDocId = "doc.edit.haskell";break;
	case L_INNO:strDocId = "doc.edit.inno";break;
	case L_SEARCHRESULT:strDocId = "doc.edit.rearchresult";break;
	case L_CMAKE:strDocId = "doc.edit.cmake";break;
	case L_YAML:strDocId = "doc.edit.caml";break;
	case L_BSDL:strDocId = "doc.edit.bsdl";break;
	case L_SVF:strDocId = "doc.edit.svf";break;
	}
	return strDocId;
}

int COwmSciteView::executeThreadSciCommand(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	// 线程中无法处理界面操作,因此通过中转消息由主线程进行界面操作
	if(!theApp.m_pIPlatUI)
	{
		return 0;
	}
	CString strFilePath = GetDocument()->GetPathName();
	CVciMessage msg(VMC_SENDOWMCMD, UINT(&strFilePath));
	msg.SetOwmCmd(Msg, wParam, lParam);
	int nRet = 0;
	theApp.m_pIPlatUI->SendCommand(OC_SENDVCICOMMAND, (WPARAM)(LPCTSTR)"owm:org.owm.edit", (LPARAM)(&msg), &nRet);
	return nRet;
}

void COwmSciteView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	COwmSciteDoc* pDoc = GetDocument();
	LangType newLang = L_TXT;

	if(pDoc)
	{
		CString pathName = pDoc->GetPathName();
		OpenFile(pathName);
		//MainFileManager->init(&m_SciEdit);
		//m_SciEdit.doOpen(pDoc->GetPathName(), FALSE);
		
		TCHAR *ext = PathFindExtension(pathName);
		if (*ext == '.')
		{	//extension found
			ext += 1;
			
			NppParameters *pNppParamInst = NppParameters::getInstance();
			// Define User Lang firstly
			const TCHAR *langName = NULL;
			if ((langName = pNppParamInst->getUserDefinedLangNameFromExt(ext)))
			{
				newLang = L_USER;
			}
			else // if it's not user lang, then check if it's supported lang
			{
				newLang = getLangFromExt(ext);
			}	
		}
	}
	
	m_SciEdit.defineDocType(newLang);
	m_LangType = newLang;

	if(pDoc)
	{
		pDoc->SetModifiedFlag(FALSE);
	}

	// 文档类型设置之后才能调用自动完成的设置语言函数
	_autoComplete.setLanguage(m_LangType);

	_codeSnippet.setLanguage(m_LangType);

	// 是否允许CWnd的Tooltip支持
	BOOL bEnableWordTooltip = AfxGetApp()->GetProfileInt(REG_HELP_SUBKEY, REG_HELP_WORD_TOOLTIP, TRUE);
	if(bEnableWordTooltip)
	{
		EnableToolTips();
	}

	// 打开Keyword数据库
	CString strExecuteInterp = theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType));
	if((strExecuteInterp != "") && (theApp.m_pIPlatUI))
	{
		CString strInterpName = theApp.m_pIPlatUI->GetVciName(strExecuteInterp);
		CString strKeywordFile;
		strKeywordFile.Format("%sdoc\\%s\\keyword.s3db", theApp.GetPlatRootPath(), strInterpName);
		if(GetFileAttributes(strKeywordFile) != 0xFFFFFFFF)
		{
			m_sqliteIndex.Open(strKeywordFile);
		}
	}
}

// 打开文件
void COwmSciteView::OpenFile(const char *fileName)
{
	//SendEditor(SCI_CANCEL);
	//SendEditor(SCI_SETUNDOCOLLECTION, 0);

	if(strlen(fileName) == 0)
	{
		return;
	}

	const int blockSize = 128 * 1024;	//128 kB
	char data[blockSize];
	FILE *fp = generic_fopen(fileName, TEXT("rb"));
	if (!fp)
		return;

	_isFileOpening = TRUE;

	Utf8_16_Read UnicodeConvertor;	//declare here so we can get information after loading is done

	bool success = true;
	try {
		size_t lenFile = 0;
		size_t lenConvert = 0;	//just in case conversion results in 0, but file not empty
		do {
			lenFile = fread(data, 1, blockSize, fp);
			lenConvert = UnicodeConvertor.convert(data, lenFile);
			executeThreadSciCommand(SCI_APPENDTEXT, lenConvert, (LPARAM)(UnicodeConvertor.getNewBuf()));
		} while (lenFile > 0);
	} catch(...)
	{
		::MessageBox(NULL, "File is too big to be opened by OwmScite", NULL, MB_OK);
		success = false;
	}
	
	fclose(fp);

	// 3 formats : WIN_FORMAT, UNIX_FORMAT and MAC_FORMAT
	if (UnicodeConvertor.getNewBuf()) 
	{
		m_SciEdit.determinateFormat(UnicodeConvertor.getNewBuf());
	}
	else
	{
		m_SciEdit.determinateFormat("");
	}

	// 设置编码方式和scintilla的codepage
	m_SciEdit.setUnicodeMode(UnicodeConvertor.getEncoding());
	if (UnicodeConvertor.getEncoding() == uni8Bit) 
	{	//either 0 or CJK codepage
		if (m_SciEdit.isCJK() && (m_LangType != L_CSS || m_LangType != L_CAML || m_LangType != L_ASM || m_LangType != L_MATLAB))
		{
			executeThreadSciCommand(SCI_SETCODEPAGE, m_SciEdit.getCodepage());	//you may also want to set charsets here, not yet implemented
		}
		else
		{
			executeThreadSciCommand(SCI_SETCODEPAGE, 0);
		}
	} 
	else
	{	//CP UTF8 for all unicode
		executeThreadSciCommand(SCI_SETCODEPAGE, SC_CP_UTF8);
	}

	_isFileOpening = FALSE;
	_linkTriggered = TRUE;
	_isDocModifing = FALSE;

	//SendEditor(SCI_SETUNDOCOLLECTION, 1);

	DWORD dwFileAttributes = ::GetFileAttributes(fileName);
	BOOL bReadOnly = (dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0;
	executeThreadSciCommand(SCI_SETREADONLY, bReadOnly);

	executeThreadSciCommand(EM_EMPTYUNDOBUFFER);
	executeThreadSciCommand(SCI_SETSAVEPOINT);
	executeThreadSciCommand(SCI_GOTOPOS, 0);
	::SetFocus(m_SciEdit.getHSelf());

	//_autoComplete.setLanguage(m_LangType);
}

// 保存当前位置
void COwmSciteView::saveCurrentPos()
{
	//Save data so, that the current topline becomes visible again after restoring.
	int displayedLine = static_cast<int>(executeThreadSciCommand(SCI_GETFIRSTVISIBLELINE));
	int docLine = executeThreadSciCommand(SCI_DOCLINEFROMVISIBLE, displayedLine);		//linenumber of the line displayed in the top
	//int offset = displayedLine - execute(SCI_VISIBLEFROMDOCLINE, docLine);		//use this to calc offset of wrap. If no wrap this should be zero

	Position pos;
	// the correct visible line number
	pos._firstVisibleLine = docLine;
	pos._startPos = static_cast<int>(executeThreadSciCommand(SCI_GETANCHOR));
	pos._endPos = static_cast<int>(executeThreadSciCommand(SCI_GETCURRENTPOS));
	pos._xOffset = static_cast<int>(executeThreadSciCommand(SCI_GETXOFFSET));
	pos._selMode = executeThreadSciCommand(SCI_GETSELECTIONMODE);
	pos._scrollWidth = executeThreadSciCommand(SCI_GETSCROLLWIDTH);

	_position = pos;
}

// 恢复当前位置
void COwmSciteView::restoreCurrentPos()
{
	Position & pos = _position;

	executeThreadSciCommand(SCI_GOTOPOS, 0);	//make sure first line visible by setting caret there, will scroll to top of document

	executeThreadSciCommand(SCI_SETSELECTIONMODE, pos._selMode);	//enable
	executeThreadSciCommand(SCI_SETANCHOR, pos._startPos);
	executeThreadSciCommand(SCI_SETCURRENTPOS, pos._endPos);
	executeThreadSciCommand(SCI_CANCEL);							//disable
	bool bIsWrap = (executeThreadSciCommand(SCI_GETWRAPMODE) == SC_WRAP_WORD);
	if (!bIsWrap) {	//only offset if not wrapping, otherwise the offset isnt needed at all
		executeThreadSciCommand(SCI_SETSCROLLWIDTH, pos._scrollWidth);
		executeThreadSciCommand(SCI_SETXOFFSET, pos._xOffset);
	}
	executeThreadSciCommand(SCI_CHOOSECARETX); // choose current x position

	int lineToShow = executeThreadSciCommand(SCI_VISIBLEFROMDOCLINE, pos._firstVisibleLine);
	executeThreadSciCommand(SCI_LINESCROLL, 0, lineToShow);
}

void COwmSciteView::OnEditUndo()
{
	m_SciEdit.execute(SCI_UNDO);
}

void COwmSciteView::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_SciEdit.execute(SCI_CANUNDO));
}

void COwmSciteView::OnEditRedo() 
{
	m_SciEdit.execute(SCI_REDO);
}

void COwmSciteView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SciEdit.execute(SCI_CANREDO));
}

void COwmSciteView::OnEditCut() 
{
	m_SciEdit.execute(SCI_CUT);
}

void COwmSciteView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SciEdit.execute(SCI_GETSELECTIONSTART) != m_SciEdit.execute(SCI_GETSELECTIONEND));
}

void COwmSciteView::OnEditCopy() 
{
	m_SciEdit.execute(SCI_COPY);
}

void COwmSciteView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SciEdit.execute(SCI_GETSELECTIONSTART) != m_SciEdit.execute(SCI_GETSELECTIONEND));
}

void COwmSciteView::OnEditPaste() 
{
	m_SciEdit.execute(SCI_PASTE);
}

void COwmSciteView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SciEdit.execute(SCI_CANPASTE));
}

void COwmSciteView::OnEditSelectAll() 
{
	m_SciEdit.execute(SCI_SELECTALL);
}

void COwmSciteView::OnUpdateEditSelectAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

static generic_string extractSymbol(TCHAR prefix, const TCHAR *str2extract)
{
	bool found = false;
	TCHAR extracted[128] = TEXT("");

	for (int i = 0, j = 0 ; i < lstrlen(str2extract) ; i++)
	{
		if (found)
		{
			if (!str2extract[i] || str2extract[i] == ' ')
			{
				extracted[j] = '\0';
				return generic_string(extracted);
			}
			extracted[j++] = str2extract[i];

		}
		else
		{
			if (!str2extract[i])
				return TEXT("");

			if (str2extract[i] == prefix)
				found = true;
		}
	}
	return  generic_string(extracted);
}

// 块注释
bool COwmSciteView::doBlockComment(comment_mode currCommentMode)
{
	const TCHAR *commentLineSybol = "";
	generic_string symbol;

	if (m_LangType == L_USER)
	{
		/*
		UserLangContainer * userLangContainer = NppParameters::getInstance()->getULCFromName(buf->getUserDefineLangName());
		if (!userLangContainer)
			return false;

		symbol = extractSymbol('0', userLangContainer->_keywordLists[4]);
		commentLineSybol = symbol.c_str();
		*/
	}
	else
	{
		NppParameters *pNppParam = NppParameters::getInstance();
		Lang *l = pNppParam->getLangFromID(m_LangType);
		if(l)
		{
			commentLineSybol = l->_pCommentLineSymbol;
		}
	}

	if ((!commentLineSybol) || (!commentLineSybol[0]))
		return false;

    generic_string comment(commentLineSybol);
    comment += TEXT(" ");
    
	const int linebufferSize = 1000;
    TCHAR linebuf[linebufferSize];
    size_t comment_length = comment.length();
    size_t selectionStart = m_SciEdit.execute(SCI_GETSELECTIONSTART);
    size_t selectionEnd = m_SciEdit.execute(SCI_GETSELECTIONEND);
    size_t caretPosition = m_SciEdit.execute(SCI_GETCURRENTPOS);
    // checking if caret is located in _beginning_ of selected block
    bool move_caret = caretPosition < selectionEnd;
    int selStartLine = m_SciEdit.execute(SCI_LINEFROMPOSITION, selectionStart);
    int selEndLine = m_SciEdit.execute(SCI_LINEFROMPOSITION, selectionEnd);
    int lines = selEndLine - selStartLine;
    size_t firstSelLineStart = m_SciEdit.execute(SCI_POSITIONFROMLINE, selStartLine);
    // "caret return" is part of the last selected line
    if ((lines > 0) && (selectionEnd == static_cast<size_t>(m_SciEdit.execute(SCI_POSITIONFROMLINE, selEndLine))))
		selEndLine--;
    m_SciEdit.execute(SCI_BEGINUNDOACTION);

    for (int i = selStartLine; i <= selEndLine; i++) 
	{
		int lineStart = m_SciEdit.execute(SCI_POSITIONFROMLINE, i);
        int lineIndent = lineStart;
        int lineEnd = m_SciEdit.execute(SCI_GETLINEENDPOSITION, i);
        if ((lineEnd - lineIndent) >= linebufferSize)        // Avoid buffer size problems
                continue;

        lineIndent = m_SciEdit.execute(SCI_GETLINEINDENTPOSITION, i);
		m_SciEdit.getGenericText(linebuf, lineIndent, lineEnd);
        
        generic_string linebufStr = linebuf;

        // empty lines are not commented
        if (lstrlen(linebuf) < 1)
			continue;
   		if (currCommentMode != cm_comment)
		{
            if (linebufStr.substr(0, comment_length - 1) == comment.substr(0, comment_length - 1))
			{
                int len = (linebufStr.substr(0, comment_length) == comment)?comment_length:comment_length - 1;
					
                m_SciEdit.execute(SCI_SETSEL, lineIndent, lineIndent + len);
				m_SciEdit.replaceSelWith("");
				
				if (i == selStartLine) // is this the first selected line?
				selectionStart -= len;
				selectionEnd -= len; // every iteration
				continue;
			}
		}
		if ((currCommentMode == cm_toggle) || (currCommentMode == cm_comment))
		{
			if (i == selStartLine) // is this the first selected line?
				selectionStart += comment_length;
			selectionEnd += comment_length; // every iteration
			m_SciEdit.insertGenericTextFrom(lineIndent, comment.c_str());
		}
     }
    // after uncommenting selection may promote itself to the lines
    // before the first initially selected line;
    // another problem - if only comment symbol was selected;
    if (selectionStart < firstSelLineStart)
	{
        if (selectionStart >= selectionEnd - (comment_length - 1))
			selectionEnd = firstSelLineStart;
        selectionStart = firstSelLineStart;
    }
    if (move_caret) 
	{
        // moving caret to the beginning of selected block
        m_SciEdit.execute(SCI_GOTOPOS, selectionEnd);
        m_SciEdit.execute(SCI_SETCURRENTPOS, selectionStart);
    }
	else 
	{
        m_SciEdit.execute(SCI_SETSEL, selectionStart, selectionEnd);
    }
    m_SciEdit.execute(SCI_ENDUNDOACTION);
    return true;
}

void COwmSciteView::OnCommentSelect() 
{
	doBlockComment(cm_toggle);
}

void COwmSciteView::OnUpdateCommentSelect(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SciEdit.execute(SCI_GETSELECTIONSTART) != m_SciEdit.execute(SCI_GETSELECTIONEND));
}

void COwmSciteView::OnUncommentSelect() 
{
	doBlockComment(cm_uncomment);
}

void COwmSciteView::OnUpdateUncommentSelect(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SciEdit.execute(SCI_GETSELECTIONSTART) != m_SciEdit.execute(SCI_GETSELECTIONEND));
}

/////////////////////////////////////////////////////////////////////////////
// 行隐藏
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnHideLines() 
{
	m_SciEdit.hideLines();
}

void COwmSciteView::OnUpdateHideLines(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 带格式拷贝操作

void COwmSciteView::initScintillaData(CurrentScintillaData * csd)
{
	csd->styles = new StyleData[STYLE_MAX];
	csd->dataBuffer = 0;
}

void COwmSciteView::fillScintillaData(CurrentScintillaData * csd, int start, int end)
{
	bool doColourise = true;

	if (csd->dataBuffer)
		delete [] csd->dataBuffer;

	if (end == 0 && start == 0) {
		int selStart = 0, selEnd = 0;
		selStart = m_SciEdit.execute(SCI_GETSELECTIONSTART, 0, 0);
		selEnd = m_SciEdit.execute(SCI_GETSELECTIONEND, 0, 0);
		if (selStart != selEnd) {
			start = selStart;
			end = selEnd;
			doColourise = false;	//do not colourise on selection, scintilla should have done this by now. Colourise on selection and the state of the lexer doesnt always match
		} else {
			end = -1;
		}
	}

	if (end == -1) {
		end = m_SciEdit.execute(SCI_GETTEXTLENGTH, 0, 0);
	}

	int len = end - start;
	int tabSize = m_SciEdit.execute(SCI_GETTABWIDTH, 0, 0);

	//csd->hScintilla = hScintilla;
	csd->nrChars = len;
	csd->tabSize = tabSize;

	csd->dataBuffer = new char[csd->nrChars * 2 + 2];
	TextRange tr;
	tr.lpstrText = csd->dataBuffer;
	tr.chrg.cpMin = start;
	tr.chrg.cpMax = end;

	if (doColourise)
		m_SciEdit.execute(SCI_COLOURISE, start, (LPARAM)end);	//colourise doc so stylers are set
	m_SciEdit.execute(SCI_GETSTYLEDTEXT, 0, (LPARAM)&tr);
	
	csd->nrStyleSwitches = 0, csd->nrUsedStyles = 1;	//Default always
	csd->totalFontStringLength = 0;
	int prevStyle = -1, currentStyle;

	for(int i = 0; i < STYLE_MAX; i++) {
		csd->usedStyles[i] = false;
	}

	csd->usedStyles[STYLE_DEFAULT] = true;
	m_SciEdit.execute(SCI_STYLEGETFONT, STYLE_DEFAULT, (LPARAM) (csd->styles[STYLE_DEFAULT].fontString));
	csd->totalFontStringLength += (int)strlen((csd->styles[STYLE_DEFAULT].fontString));
	csd->styles[STYLE_DEFAULT].size =		m_SciEdit.execute(SCI_STYLEGETSIZE,		STYLE_DEFAULT, 0);
	csd->styles[STYLE_DEFAULT].bold =		m_SciEdit.execute(SCI_STYLEGETBOLD,		STYLE_DEFAULT, 0);
	csd->styles[STYLE_DEFAULT].italic =		m_SciEdit.execute(SCI_STYLEGETITALIC,	STYLE_DEFAULT, 0);
	csd->styles[STYLE_DEFAULT].underlined =	m_SciEdit.execute(SCI_STYLEGETUNDERLINE, STYLE_DEFAULT, 0);
	csd->styles[STYLE_DEFAULT].fgColor =	m_SciEdit.execute(SCI_STYLEGETFORE,		STYLE_DEFAULT, 0);
	csd->styles[STYLE_DEFAULT].bgColor =	m_SciEdit.execute(SCI_STYLEGETBACK,		STYLE_DEFAULT, 0);

	for(int i = 0; i < len - 1; i++) {
		currentStyle = csd->dataBuffer[i*2+1];
		if (currentStyle != prevStyle) {
			prevStyle = currentStyle;
			csd->nrStyleSwitches++;
		}
		if (csd->usedStyles[currentStyle] == false) {
			csd->nrUsedStyles++;
			m_SciEdit.execute(SCI_STYLEGETFONT, currentStyle, (LPARAM) (csd->styles[currentStyle].fontString));
			csd->totalFontStringLength += (int)strlen((csd->styles[currentStyle].fontString));
			csd->styles[currentStyle].size =		m_SciEdit.execute(SCI_STYLEGETSIZE,		currentStyle, 0);
			csd->styles[currentStyle].bold =		m_SciEdit.execute(SCI_STYLEGETBOLD,		currentStyle, 0);
			csd->styles[currentStyle].italic =		m_SciEdit.execute(SCI_STYLEGETITALIC,	currentStyle, 0);
			csd->styles[currentStyle].underlined =	m_SciEdit.execute(SCI_STYLEGETUNDERLINE, currentStyle, 0);
			csd->styles[currentStyle].fgColor =		m_SciEdit.execute(SCI_STYLEGETFORE,		currentStyle, 0);
			csd->styles[currentStyle].bgColor =		m_SciEdit.execute(SCI_STYLEGETBACK,		currentStyle, 0);
			csd->usedStyles[currentStyle] = true;
		}
	}
}

void COwmSciteView::exportRTF(ExportFileData * efd)
{
	//estimate buffer size needed
	char * buffer = efd->csd->dataBuffer;
	int totalBytesNeeded = 1;	//zero terminator
	
	totalBytesNeeded += EXPORT_SIZE_RTF_STATIC + EXPORT_SIZE_RTF_STYLE * efd->csd->nrUsedStyles + efd->csd->totalFontStringLength + EXPORT_SIZE_RTF_SWITCH * efd->csd->nrStyleSwitches;

	for(int i = 0; i < efd->csd->nrChars; i++) {
		switch(buffer[(i*2)]) {
			case '{':
				totalBytesNeeded += 2;	// '\{'
				break;
			case '}':
				totalBytesNeeded += 2;	// '\}'
				break;
			case '\\':
				totalBytesNeeded += 2;	// '\\'
				break;
			case '\t':
				totalBytesNeeded += efd->csd->tabSize;
				break;
			case '\r':
				if (buffer[(i*2)+2] == '\n')
					break;
			case '\n':
				totalBytesNeeded += 6;	// '\par\r\n'
				break;
			default:
				totalBytesNeeded += 1; //	'char'
				break;
		}
	}

	int currentBufferOffset = 0;
	HGLOBAL hRTFBuffer = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, totalBytesNeeded);
	char * clipbuffer = (char *)::GlobalLock(hRTFBuffer);
	clipbuffer[0] = 0;

	currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "{\\rtf1\\ansi\\deff0\r\n\r\n");
	currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "{\\fonttbl\r\n");

	StyleData * currentStyle;

	int currentFontIndex = 0;
	for(int i = 0; i < STYLE_MAX; i++) {
		if (efd->csd->usedStyles[i] == true) {
			currentStyle = (efd->csd->styles)+i;
			currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "{\\f%03d %s;}\r\n", currentFontIndex, currentStyle->fontString);
			currentStyle->fontIndex = currentFontIndex;
			currentFontIndex++;
		}
	}


	currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "}\r\n\r\n");	//fonttbl
	currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "{\\colortbl\r\n");

	int currentColorIndex = 0;
	for(int i = 0; i < STYLE_MAX; i++) {
		if (efd->csd->usedStyles[i] == true) {
			currentStyle = (efd->csd->styles)+i;

			currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\\red%03d\\green%03d\\blue%03d;\r\n", (currentStyle->fgColor>>0)&0xFF, (currentStyle->fgColor>>8)&0xFF, (currentStyle->fgColor>>16)&0xFF);
			currentStyle->fgClrIndex = currentColorIndex;
			currentColorIndex++;

			currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\\red%03d\\green%03d\\blue%03d;\r\n", (currentStyle->bgColor>>0)&0xFF, (currentStyle->bgColor>>8)&0xFF, (currentStyle->bgColor>>16)&0xFF);
			currentStyle->bgClrIndex = currentColorIndex;
			currentColorIndex++;
		}
	}

	currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "}\r\n\r\n");	//colortbl

//-------Dump text to RTF
	char * tabBuffer = new char[efd->csd->tabSize+1];
	tabBuffer[efd->csd->tabSize] = 0;
	for(int i = 0; i < efd->csd->tabSize; i++)
		tabBuffer[i] = ' ';

	int nrCharsSinceLinebreak = -1, nrTabCharsToSkip = 0;
	int lastStyle = -1;
	int prevStyle = STYLE_DEFAULT;
	char currentChar;
	StyleData * styles = efd->csd->styles;

	for(int i = 0; i < efd->csd->nrChars; i++) {

		//print new span object if style changes
		if (buffer[i*2+1] != lastStyle) {
			if (lastStyle != -1)
				prevStyle = lastStyle;
			lastStyle = buffer[i*2+1];
			currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\\f%d\\fs%d\\cb%d\\cf%d", styles[lastStyle].fontIndex, styles[lastStyle].size * 2, styles[lastStyle].bgClrIndex, styles[lastStyle].fgClrIndex);
			//if (styles[lastStyle].bold != styles[prevStyle].bold) {
				if (styles[lastStyle].bold) {
					currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\\b");
				} else {
					currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\\b0");
				}
			//}
			//if (styles[lastStyle].italic != styles[prevStyle].italic) {
				if (styles[lastStyle].underlined) {
					currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\\i");
				} else {
					currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\\i0");
				}
			//}
			//if (styles[lastStyle].underlined != styles[prevStyle].underlined) {
				if (styles[lastStyle].underlined) {
					currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\\ul");
				} else {
					currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\\ul0");
				}
			//}
			currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, " ");
		}

		//print character, parse special ones
		currentChar = buffer[(i*2)];
		nrCharsSinceLinebreak++;
		switch(currentChar) {
			case '{':
				currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\\{");
				break;
			case '}':
				currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\\}");
				break;
			case '\\':
				currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\\\\");
				break;
			case '\t':
				nrTabCharsToSkip = nrCharsSinceLinebreak%(efd->csd->tabSize);
				currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, tabBuffer + (nrTabCharsToSkip));
				nrCharsSinceLinebreak += efd->csd->tabSize - nrTabCharsToSkip - 1;
				break;
			case '\r':
				if (buffer[(i*2)+2] == '\n')
					break;
			case '\n':
				currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\\par\r\n");
				nrCharsSinceLinebreak = -1;
				break;
			default:
				if (currentChar < 20)	//ignore control characters
					break;
				currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "%c", currentChar);
				break;
		}
	}

	delete [] tabBuffer;

	currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "}\r\n");	//rtf/ansi

	if (efd->file != NULL) {
		fwrite(clipbuffer, 1, currentBufferOffset, efd->file);
		::GlobalUnlock(hRTFBuffer);
		::GlobalFree(hRTFBuffer);
	} else {
		BOOL result = ::OpenClipboard(AfxGetMainWnd()->GetSafeHwnd());
		if (result == FALSE) {
			TRACE("Unable to open clipboard");
		}
		result = ::EmptyClipboard();
		if (result == FALSE) {
			TRACE("Unable to empty clipboard");
		}

		::GlobalUnlock(hRTFBuffer);
		::SetClipboardData(::RegisterClipboardFormat(CF_RTF), hRTFBuffer);

		result = ::CloseClipboard();
		if (result == FALSE) {
			TRACE("Unable to close clipboard");
		}
	}

}

void COwmSciteView::exportHTML(ExportFileData * efd)
{
	//estimate buffer size needed
	char * buffer = efd->csd->dataBuffer;
	int totalBytesNeeded = 1;	//zero terminator
	bool addHeader = (efd->file == NULL);	//true if putting data on clipboard
	
	totalBytesNeeded += EXPORT_SIZE_HTML_STATIC + EXPORT_SIZE_HTML_STYLE * (efd->csd->nrUsedStyles-1) + efd->csd->totalFontStringLength + EXPORT_SIZE_HTML_SWITCH * efd->csd->nrStyleSwitches;
	if (addHeader)
		totalBytesNeeded += EXPORT_SIZE_HTML_HEADER;
	int startHTML = EXPORT_SIZE_HTML_HEADER, endHTML = 0, startFragment = 0, endFragment = 0;

	int i;
	for(i = 0; i < efd->csd->nrChars; i++) {
		switch(buffer[(i*2)]) {
			case ' ':
				totalBytesNeeded += 6;	// '\{'
				break;
			case '<':
				totalBytesNeeded += 4;	// '\}'
				break;
			case '>':
				totalBytesNeeded += 4;	// '\\'
				break;
			case '\t':
				totalBytesNeeded += efd->csd->tabSize * 6;
				break;
			case '\r':
				if (buffer[(i*2)+2] == '\n')
					break;
			case '\n':
				totalBytesNeeded += 7;	// '\par\r\n'
				break;
			default:
				totalBytesNeeded += 1; //	'char'
				break;
		}
	}

	int currentBufferOffset = 0;
	HGLOBAL hHTMLBuffer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, totalBytesNeeded);
	char * clipbuffer = (char *)GlobalLock(hHTMLBuffer);
	clipbuffer[0] = 0;

	//add CF_HTML header if needed, return later to fill in the blanks
	if (addHeader) {
		currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "Version:0.9\r\nStartHTML:0000000105\r\nEndHTML:0000000201\r\nStartFragment:0000000156\r\nEndFragment:0000000165");
	}
	//end CF_HTML header

	//begin building context

	currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "<html>\r\n<head>\r\n<title>Exported from Notepad++</title>\r\n<style type=\"text/css\">\r\n");

	StyleData * currentStyle, * defaultStyle;
	defaultStyle = (efd->csd->styles)+STYLE_DEFAULT;

	currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "span {\n", i);
	currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\tfont-family: '%s';\r\n", defaultStyle->fontString);
	currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\tfont-size: %0dpt;\r\n", defaultStyle->size);
	if (defaultStyle->bold)		currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\tfont-weight: bold;\r\n");
	if (defaultStyle->italic)		currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\tfont-style: italic;\r\n");
	currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\tcolor: #%02X%02X%02X;\r\n", (defaultStyle->fgColor>>0)&0xFF, (defaultStyle->fgColor>>8)&0xFF, (defaultStyle->fgColor>>16)&0xFF);
	currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "}\r\n");

	for(int i = 0; i < STYLE_MAX; i++) {
		if (i == STYLE_DEFAULT)
			continue;

		currentStyle = (efd->csd->styles)+i;
		if (efd->csd->usedStyles[i] == true) {
			currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, ".SpanClass%d {\r\n", i);
			if (strcmpi(currentStyle->fontString, defaultStyle->fontString))
				currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\tfont-family: '%s';\r\n", currentStyle->fontString);
			if (currentStyle->size != defaultStyle->size)
				currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\tfont-size: %0dpt;\r\n", currentStyle->size);
			if (currentStyle->bold != defaultStyle->bold) {
				if (currentStyle->bold)
					currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\tfont-weight: bold;\r\n");
				else
					currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\tfont-weight: normal;\r\n");
			}
			if (currentStyle->italic != defaultStyle->italic) {
				if (currentStyle->italic)
					currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\tfont-style: italic;\r\n");
				else
					currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\tfont-style: normal;\r\n");
			}
			if (currentStyle->underlined)
				currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\ttext-decoration: underline;\r\n");
			if (currentStyle->fgColor != defaultStyle->fgColor)
				currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\tcolor: #%02X%02X%02X;\r\n", (currentStyle->fgColor>>0)&0xFF, (currentStyle->fgColor>>8)&0xFF, (currentStyle->fgColor>>16)&0xFF);
			if (currentStyle->bgColor != defaultStyle->bgColor)
				currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\tbackground: #%02X%02X%02X;\r\n", (currentStyle->bgColor>>0)&0xFF, (currentStyle->bgColor>>8)&0xFF, (currentStyle->bgColor>>16)&0xFF);
			currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "}\r\n");
		}
	}

	currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "</style>\r\n</head>\r\n");
	currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "<body bgcolor=\"#%02X%02X%02X\">\r\n", (defaultStyle->bgColor>>0)&0xFF, (defaultStyle->bgColor>>8)&0xFF, (defaultStyle->bgColor>>16)&0xFF);
	currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "<span>\r\n");

	//end building context

	//add StartFragment if doing CF_HTML
	currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "<!--StartFragment-->\r\n");
	startFragment = currentBufferOffset;
	//end StartFragment

//-------Dump text to HTML
	char * tabBuffer = new char[efd->csd->tabSize * 6 + 1];
	tabBuffer[0] = 0;
	for(int i = 0; i < efd->csd->tabSize; i++) {
		strcat(tabBuffer, "&nbsp;");
	}

	int nrCharsSinceLinebreak = -1, nrTabCharsToSkip = 0;
	int lastStyle = -1;
	char currentChar;
	bool openSpan = false;

	for(int i = 0; i < efd->csd->nrChars; i++) {
		//print new span object if style changes
		if (buffer[i*2+1] != lastStyle) {
			if (openSpan) {
				currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "</span>");
			}
			lastStyle = buffer[i*2+1];
			currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "<span class=\"SpanClass%d\">", lastStyle);
			openSpan = true;
		}

		//print character, parse special ones
		currentChar = buffer[(i*2)];
		nrCharsSinceLinebreak++;
		switch(currentChar) {
			case '\r':
				if (buffer[(i*2)+2] == '\n')
					break;
			case '\n':
				currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "<br/>\r\n");
				nrCharsSinceLinebreak = -1;
				break;
			case '<':
				currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "&lt;");
				break;
			case '>':
				currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "&gt;");
				break;
			case ' ':
				currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "&nbsp;");
				break;
			case '\t':
				nrTabCharsToSkip = nrCharsSinceLinebreak%(efd->csd->tabSize);
				currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "%s", tabBuffer + (nrTabCharsToSkip * 6));
				nrCharsSinceLinebreak += efd->csd->tabSize - nrTabCharsToSkip - 1;
				break;
			default:
				if (currentChar < 20)	//ignore control characters
					break;
				currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "%c", currentChar);
				break;
		}
	}

	if (openSpan) {
		currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "</span>");
	}

	delete [] tabBuffer;

	//add EndFragment if doing CF_HTML
	endFragment = currentBufferOffset;
	currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "<!--EndFragment-->\r\n");
	//end EndFragment

	//add closing context
	currentBufferOffset += sprintf(clipbuffer+currentBufferOffset, "\r\n</span>\r\n</body>\r\n</html>\r\n");
	endHTML = currentBufferOffset;

	//if doing CF_HTML, fill in header data
	if (addHeader) {
		char number[11];
		sprintf(number, "%.10d", startHTML);
		memcpy(clipbuffer + 23, number, 10);
		sprintf(number, "%.10d", endHTML);
		memcpy(clipbuffer + 43, number, 10);
		sprintf(number, "%.10d", startFragment);
		memcpy(clipbuffer + 69, number, 10);
		sprintf(number, "%.10d", endFragment);
		memcpy(clipbuffer + 93, number, 10);
	}
	//end header


	if (efd->file != NULL) {
		fwrite(clipbuffer, 1, currentBufferOffset, efd->file);
		::GlobalUnlock(hHTMLBuffer);
		::GlobalFree(hHTMLBuffer);
	} else {
		BOOL result = ::OpenClipboard(AfxGetMainWnd()->GetSafeHwnd());
		if (result == FALSE) {
			TRACE("Unable to open clipboard");
		}
		result = ::EmptyClipboard();
		if (result == FALSE) {
			TRACE("Unable to empty clipboard");
		}

		::GlobalUnlock(hHTMLBuffer);
		::SetClipboardData(::RegisterClipboardFormat(CF_HTML), hHTMLBuffer);

		result = ::CloseClipboard();
		if (result == FALSE) {
			TRACE("Unable to close clipboard");
		}
	}
}

// RTF方式复制
void COwmSciteView::OnCopyAsRtf() 
{
	ExportFileData efd;
	CurrentScintillaData csd;
	initScintillaData(&csd);
	fillScintillaData(&csd, 0, 0);
	efd.csd = &csd;	
	efd.file = NULL;
	exportRTF(&efd);
}

void COwmSciteView::OnUpdateCopyAsRtf(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SciEdit.execute(SCI_GETSELECTIONSTART) != m_SciEdit.execute(SCI_GETSELECTIONEND));
}

// 导出Html文件
void COwmSciteView::exportHtmlFile(CString strExportFile) 
{
	CString strFilePath = strExportFile;
	if(strFilePath == "")
	{
		CFileDialog dlg(FALSE, "html", NULL,
			OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT,
			"html file(*.html)|*.html|All Files (*.*)|*.*||", this);

		if (dlg.DoModal() != IDOK)
		{
			return;
		}
		strFilePath = dlg.GetPathName();
	}

	ExportFileData efd;
	CurrentScintillaData csd;
	initScintillaData(&csd);
	fillScintillaData(&csd, 0, 0);
	efd.csd = &csd;	
	FILE * output = fopen(strFilePath, "wb");
	if (!output) {
		return;
	}
	efd.file = output;
	exportHTML(&efd);
	fclose(efd.file);
}

// 导出RTF文件
void COwmSciteView::exportRtfFile(CString strExportFile) 
{
	CString strFilePath = strExportFile;
	if(strFilePath == "")
	{
		CFileDialog dlg(FALSE, "rtf", NULL,
			OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT,
			"rich text file(*.rtf)|*.rtf|All Files (*.*)|*.*||", this);

		if (dlg.DoModal() != IDOK)
		{
			return;
		}
		strFilePath = dlg.GetPathName();
	}

	ExportFileData efd;
	CurrentScintillaData csd;
	initScintillaData(&csd);
	fillScintillaData(&csd, 0, 0);
	efd.csd = &csd;	
	FILE * output = fopen(strFilePath, "wb");
	if (!output) {
		return;
	}
	efd.file = output;
	exportRTF(&efd);
	fclose(efd.file);
}

//////////////////////////////////////////////////////////////////////////

int Searching::convertExtendedToString(const TCHAR * query, TCHAR * result, int length) {	//query may equal to result, since it always gets smaller
	int i = 0, j = 0;
	int charLeft = length;
	bool isGood = true;
	TCHAR current;
	while(i < length) {	//because the backslash escape quences always reduce the size of the generic_string, no overflow checks have to be made for target, assuming parameters are correct
		current = query[i];
		charLeft--;
		if (current == '\\' && charLeft) {	//possible escape sequence
			i++;
			charLeft--;
			current = query[i];
			switch(current) {
				case 'r':
					result[j] = '\r';
					break;
				case 'n':
					result[j] = '\n';
					break;
				case '0':
					result[j] = '\0';
					break;
				case 't':
					result[j] = '\t';
					break;
				case '\\':
					result[j] = '\\';
					break;
				case 'b':
				case 'd':
				case 'o':
				case 'x': {
					int size = 0, base = 0;
					if (current == 'b') {			//11111111
						size = 8, base = 2;
					} else if (current == 'o') {	//377
						size = 3, base = 8;
					} else if (current == 'd') {	//255
						size = 3, base = 10;
					} else if (current == 'x') {	//0xFF
						size = 2, base = 16;
					}
					if (charLeft >= size) {
						int res = 0;
						if (Searching::readBase(query+(i+1), &res, base, size)) {
							result[j] = (TCHAR)res;
							i+=size;
							break;
						}
					}
					//not enough chars to make parameter, use default method as fallback
					}
				default: {	//unknown sequence, treat as regular text
					result[j] = '\\';
					j++;
					result[j] = current;
					isGood = false;
					break;
				}
			}
		} else {
			result[j] = query[i];
		}
		i++;
		j++;
	}
	result[j] = 0;
	return j;
}

bool Searching::readBase(const TCHAR * str, int * value, int base, int size) {
	int i = 0, temp = 0;
	*value = 0;
	TCHAR max = '0' + base - 1;
	TCHAR current;
	while(i < size) {
		current = str[i];
		if (current >= '0' && current <= max) {
			temp *= base;
			temp += (current - '0');
		} else {
			return false;
		}
		i++;
	}
	*value = temp;
	return true;
}

void Searching::displaySectionCentered(int posStart, int posEnd, ScintillaEditView * pEditView, bool isDownwards) 
{
	// to make sure the found result is visible
	//When searching up, the beginning of the (possible multiline) result is important, when scrolling down the end
	int testPos = (isDownwards)?posEnd:posStart;
	pEditView->execute(SCI_SETCURRENTPOS, testPos);
	int currentlineNumberDoc = (int)pEditView->execute(SCI_LINEFROMPOSITION, testPos);
	int currentlineNumberVis = (int)pEditView->execute(SCI_VISIBLEFROMDOCLINE, currentlineNumberDoc);
	pEditView->execute(SCI_ENSUREVISIBLE, currentlineNumberDoc);	// make sure target line is unfolded

	int firstVisibleLineVis =	(int)pEditView->execute(SCI_GETFIRSTVISIBLELINE);
	int linesVisible =			(int)pEditView->execute(SCI_LINESONSCREEN) - 1;	//-1 for the scrollbar
	int lastVisibleLineVis =	(int)linesVisible + firstVisibleLineVis;
	
	//if out of view vertically, scroll line into (center of) view
	int linesToScroll = 0;
	if (currentlineNumberVis < firstVisibleLineVis)
	{
		linesToScroll = currentlineNumberVis - firstVisibleLineVis;
		//use center
		linesToScroll -= linesVisible/2;		
	}
	else if (currentlineNumberVis > lastVisibleLineVis)
	{
		linesToScroll = currentlineNumberVis - lastVisibleLineVis;
		//use center
		linesToScroll += linesVisible/2;
	}
	pEditView->scroll(0, linesToScroll);

	//Make sure the caret is visible, scroll horizontally (this will also fix wrapping problems)
	pEditView->execute(SCI_GOTOPOS, posStart);
	pEditView->execute(SCI_GOTOPOS, posEnd);

	pEditView->execute(SCI_SETANCHOR, posStart);	
}

int COwmSciteView::processAll(ProcessOperation op, const TCHAR *txt2find, const TCHAR *txt2replace, bool isEntire, const TCHAR *fileName, FindOption *opt)
{
	FindOption _options;
	_options._isWholeWord = (m_SciEdit.execute(SCI_GETSELECTIONSTART) != m_SciEdit.execute(SCI_GETSELECTIONEND));
	_options._isMatchCase = false;
	_options._searchType = FindNormal;
	_options._isWrapAround = true;
	// Set Direction : Down by default
	_options._whichDirection = DIR_DOWN;

	FindOption *pOptions = opt?opt:&_options;

	CharacterRange cr = m_SciEdit.getSelection();
	int docLength = int(m_SciEdit.execute(SCI_GETLENGTH));

	// Default : 
	//        direction : down
	//        begin at : 0
	//        end at : end of doc
	int startPosition = 0;
	int endPosition = docLength;

	bool direction = pOptions->_whichDirection;

	//first try limiting scope by direction
	if (direction == DIR_UP)	
	{
		startPosition = 0;
		endPosition = cr.cpMax;
	}
	else
	{
		startPosition = cr.cpMin;
		endPosition = docLength;
	}

	//then adjust scope if the full document needs to be changed
	if (pOptions->_isWrapAround || isEntire || (op == ProcessCountAll))	//entire document needs to be scanned
	{		
		startPosition = 0;
		endPosition = docLength;
	}
	
	//then readjust scope if the selection override is active and allowed
	if ((pOptions->_isInSelection) && ((op == ProcessMarkAll) || ((op == ProcessReplaceAll) && (!isEntire))))	//if selection limiter and either mark all or replace all w/o entire document override
	{
		startPosition = cr.cpMin;
		endPosition = cr.cpMax;
	}

	return processRange(op, txt2find, txt2replace, startPosition, endPosition, fileName, pOptions);
}

int COwmSciteView::processRange(ProcessOperation op, const TCHAR *txt2find, const TCHAR *txt2replace, int startRange, int endRange, const TCHAR *fileName, FindOption *opt)
{
	int nbProcessed = 0;
	
	if (!txt2find)
		return nbProcessed;

	if ((op == ProcessReplaceAll) && m_SciEdit.execute(SCI_GETREADONLY))
		return nbProcessed;

	if ((op == ProcessReplaceAll) && (!txt2replace))
		return nbProcessed;

	if (startRange == endRange)
		return nbProcessed;

	if (!fileName)
		fileName = TEXT("");

	FindOption *pOptions = opt?opt:NULL;//&_options;
	//bool isUnicode = (m_SciEdit.execute(SCI_GETCODEPAGE) == SC_CP_UTF8);

	int stringSizeFind = 0;
	int stringSizeReplace = 0;

	TCHAR *pTextFind = NULL;//new TCHAR[stringSizeFind + 1];
	stringSizeFind = lstrlen(txt2find);
	pTextFind = new TCHAR[stringSizeFind + 10];
	lstrcpy(pTextFind, txt2find);

	if (!pTextFind[0]) 
	{
		delete [] pTextFind;
		return nbProcessed;
	}

	TCHAR *pTextReplace = NULL;
	if (op == ProcessReplaceAll)
	{
		stringSizeReplace = lstrlen(txt2replace);
		pTextReplace = new TCHAR[stringSizeReplace + 1];
		lstrcpy(pTextReplace, txt2replace);
	}	

	if (pOptions->_searchType == FindExtended) {
		stringSizeFind = Searching::convertExtendedToString(pTextFind, pTextFind, stringSizeFind);
		if (op == ProcessReplaceAll)
			stringSizeReplace = Searching::convertExtendedToString(pTextReplace, pTextReplace, stringSizeReplace);
	}

	bool isRegExp = pOptions->_searchType == FindRegex;
	int flags = Searching::buildSearchFlags(pOptions);

	if (op == ProcessMarkAll)	//if marking, check if purging is needed
	{
		//if (_doPurge) {
			//if (_doMarkLine)
				m_SciEdit.execute(SCI_MARKERDELETEALL, MARK_BOOKMARK);

			//if (_doStyleFoundToken) --高亮查询结果
			//	m_SciEdit.clearIndicator(SCE_UNIVERSAL_FOUND_STYLE);
		//}
	}

	// 清空查找结果
	theApp.m_pIPlatUI->SendOwmCmd("org.owm.grep", OC_GREP_CLEARRESULT, 0, 0, NULL);

	int targetStart = 0;
	int targetEnd = 0;

	//Initial range for searching
	m_SciEdit.execute(SCI_SETSEARCHFLAGS, flags);
	targetStart = m_SciEdit.searchInTarget(pTextFind, startRange, endRange);
	
	if ((targetStart != -1) && (op == ProcessFindAll))	//add new filetitle if this file results in hits
	{
		//_pFinder->addFileNameTitle(fileName);
	}
	while (targetStart != -1)
	{
		//int posFindBefore = posFind;
		targetStart = int(m_SciEdit.execute(SCI_GETTARGETSTART));
		targetEnd = int(m_SciEdit.execute(SCI_GETTARGETEND));
		if (targetEnd > endRange) {	//we found a result but outside our range, therefore do not process it
			break;
		}
		int foundTextLen = targetEnd - targetStart;
		int replaceDelta = 0;

		// Search resulted in empty token, possible with RE
		if (!foundTextLen) {
			delete [] pTextFind;
			delete [] pTextReplace;
			return -1;
		}
		
		switch (op)
		{
			case ProcessFindAll: 
			{
				int lineNumber = m_SciEdit.execute(SCI_LINEFROMPOSITION, targetStart);
				int lend = m_SciEdit.execute(SCI_GETLINEENDPOSITION, lineNumber);
				int lstart = m_SciEdit.execute(SCI_POSITIONFROMLINE, lineNumber);
				int nbChar = lend - lstart;

				// use the static buffer
				TCHAR lineBuf[1024];

				if (nbChar > 1024 - 3)
					lend = lstart + 1020;

				int start_mark = targetStart - lstart;
				int end_mark = targetEnd - lstart;

				m_SciEdit.getGenericText(lineBuf, lstart, lend, &start_mark, &end_mark);
				generic_string line;
#ifdef UNICODE
				line = lineBuf;
#else
				UINT cp = m_SciEdit.execute(SCI_GETCODEPAGE);
				/*if (cp != SC_CP_UTF8)
				{
					WcharMbcsConvertor *wmc = WcharMbcsConvertor::getInstance();
					const wchar_t *pTextW = wmc->char2wchar(lineBuf, ::GetACP());
					const char *pTextA = wmc->wchar2char(pTextW, SC_CP_UTF8);
					line = pTextA;
				}
				else
					line = lineBuf;*/
				if (cp == SC_CP_UTF8)
				{
					WcharMbcsConvertor *wmc = WcharMbcsConvertor::getInstance();
					const wchar_t *pTextW = wmc->char2wchar(lineBuf, SC_CP_UTF8);
					const char *pTextA = wmc->wchar2char(pTextW, ::GetACP());
					line = pTextA;
				}
				else
					line = lineBuf;
#endif
				line += TEXT("\r\n");
				SearchResultMarking srm;
				srm._start = start_mark;
				srm._end = end_mark;
				//_pFinder->add(FoundInfo(targetStart, targetEnd,  fileName), srm, line.c_str(), lineNumber + 1);
				SearchResultInfo resultInfo(GetDocument()->GetPathName(), lineNumber + 1, line.c_str());
				theApp.m_pIPlatUI->SendOwmCmd("org.owm.grep", OC_GREP_ADDRESULTLINE, 0, (LPARAM)(&resultInfo), NULL);
				

				break; 
			}

			case ProcessReplaceAll: 
			{
				int replacedLength;
				if (isRegExp)
					replacedLength = m_SciEdit.replaceTargetRegExMode(pTextReplace);
				else
					replacedLength = m_SciEdit.replaceTarget(pTextReplace);

				replaceDelta = replacedLength - foundTextLen;
				break; 
			}

			case ProcessMarkAll: 
			{
				/*if (_doStyleFoundToken)
				{
					m_SciEdit.execute(SCI_SETINDICATORCURRENT, SCE_UNIVERSAL_FOUND_STYLE);
					m_SciEdit.execute(SCI_INDICATORFILLRANGE,  targetStart, foundTextLen);
				}*/

				//if (_doMarkLine)
				{
					int lineNumber = m_SciEdit.execute(SCI_LINEFROMPOSITION, targetStart);
					int state = m_SciEdit.execute(SCI_MARKERGET, lineNumber);

					if (!(state & (1 << MARK_BOOKMARK)))
						m_SciEdit.execute(SCI_MARKERADD, lineNumber, MARK_BOOKMARK);
				}
				break; 
			}

			case ProcessMarkAll_2:
			{
				m_SciEdit.execute(SCI_SETINDICATORCURRENT,  SCE_UNIVERSAL_FOUND_STYLE_2);
				m_SciEdit.execute(SCI_INDICATORFILLRANGE,  targetStart, foundTextLen);
				break;
			}

			case ProcessMarkAll_IncSearch:
			{
				m_SciEdit.execute(SCI_SETINDICATORCURRENT,  SCE_UNIVERSAL_FOUND_STYLE_INC);
				m_SciEdit.execute(SCI_INDICATORFILLRANGE,  targetStart, foundTextLen);
				break;
			}

			case ProcessCountAll: 
			{
				//Nothing to do
				break;
			}

			default:
			{
				delete [] pTextFind;
				delete [] pTextReplace;
				return nbProcessed;
			}
			
		}	

		startRange = targetStart + foundTextLen + replaceDelta;		//search from result onwards
		endRange += replaceDelta;									//adjust end of range in case of replace

		nbProcessed++;
		
		//::SendMessageA(_hParent, WM_SETTEXT, 0, (LPARAM)pTextFind);
		targetStart = m_SciEdit.searchInTarget(pTextFind, startRange, endRange);
	}
	delete [] pTextFind;
	delete [] pTextReplace;

	//if (nbProcessed > 0 && op == ProcessFindAll) 
	//	_pFinder->addFileHitCount(nbProcessed);

	return nbProcessed;
}

// 获取查找、替换字符串
generic_string COwmSciteView::getTextFromGrep(int nCmdId) const
{
	bool isUnicode = (m_SciEdit.execute(SCI_GETCODEPAGE) == SC_CP_UTF8);

	LPCTSTR lpszText = NULL;
	theApp.m_pIPlatUI->SendOwmCmd("org.owm.grep", nCmdId, 0, 0, &lpszText);
	if(lpszText == NULL)
	{
		return generic_string("");
	}

	TCHAR str[FINDREPLACE_MAXLENGTH];
	if (isUnicode)
	{
		wchar_t wchars[FINDREPLACE_MAXLENGTH];
		char achars[FINDREPLACE_MAXLENGTH];
		lstrcpy(achars, lpszText);
		::MultiByteToWideChar(CP_ACP, 0, achars, -1, wchars, FINDREPLACE_MAXLENGTH - 1);
		::WideCharToMultiByte(CP_UTF8, 0, wchars, -1, str, FINDREPLACE_MAXLENGTH - 1, NULL, NULL);
	}
	else
	{
		lstrcpy(str, lpszText);
	}
	return generic_string(str);
}

// 查找
void COwmSciteView::OnEditFind() 
{
	if(theApp.m_pIPlatUI == NULL)
	{
		return;
	}
	theApp.m_pIPlatUI->CreateDockingPane("org.owm.grep", 10002);

	CString text;
	int nSelCount = m_SciEdit.getSelectedTextCount();
	m_SciEdit.getGenericSelectedText(text.GetBuffer(nSelCount+1), nSelCount+1);
	text.ReleaseBuffer();

	if(nSelCount > 0)
	{
		generic_string seltext;
#ifdef UNICODE
		seltext = text;
#else
		UINT cp = m_SciEdit.execute(SCI_GETCODEPAGE);
		if (cp == SC_CP_UTF8)
		{
			WcharMbcsConvertor *wmc = WcharMbcsConvertor::getInstance();
			const wchar_t *pTextW = wmc->char2wchar(text, SC_CP_UTF8);
			const char *pTextA = wmc->wchar2char(pTextW, ::GetACP());
			seltext = pTextA;
		}
		else
			seltext = text;
#endif
		theApp.m_pIPlatUI->SendOwmCmd("org.owm.grep", OC_GREP_SETTEXTFIND, (WPARAM)(seltext.c_str()), 0, NULL);
		theApp.m_pIPlatUI->SendOwmCmd("org.owm.grep", OC_GREP_SETLOCATION, GREP_LOC_CURRENTDOC, 0, NULL);
	}
}

void COwmSciteView::OnUpdateEditFind(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

// 重复查找
void COwmSciteView::OnEditRepeat() 
{
	
}

void COwmSciteView::OnUpdateEditRepeat(CCmdUI* pCmdUI) 
{
	
}

// 替换
void COwmSciteView::OnEditReplace() 
{
	if(theApp.m_pIPlatUI == NULL)
	{
		return;
	}
	theApp.m_pIPlatUI->CreateDockingPane("org.owm.grep", 10002);

	CString text;
	int nSelCount = m_SciEdit.getSelectedTextCount();
	m_SciEdit.getGenericSelectedText(text.GetBuffer(nSelCount+1), nSelCount+1);
	text.ReleaseBuffer();

	if(nSelCount > 0)
	{
		generic_string seltext;
#ifdef UNICODE
		seltext = text;
#else
		UINT cp = m_SciEdit.execute(SCI_GETCODEPAGE);
		if (cp == SC_CP_UTF8)
		{
			WcharMbcsConvertor *wmc = WcharMbcsConvertor::getInstance();
			const wchar_t *pTextW = wmc->char2wchar(text, SC_CP_UTF8);
			const char *pTextA = wmc->wchar2char(pTextW, ::GetACP());
			seltext = pTextA;
		}
		else
			seltext = text;
#endif
		theApp.m_pIPlatUI->SendOwmCmd("org.owm.grep", OC_GREP_SETTEXTFIND, (WPARAM)(seltext.c_str()), 0, NULL);
		theApp.m_pIPlatUI->SendOwmCmd("org.owm.grep", OC_GREP_SETLOCATION, GREP_LOC_CURRENTDOC, 0, NULL);
	}
}

void COwmSciteView::OnUpdateEditReplace(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

///////////////////////////////////////////////////////////////////////

// 刷新书签列表窗口
void COwmSciteView::ViewBookmarksWindow() 
{
	if(theApp.m_pIPlatUI == NULL)
	{
		return;
	}

	// 创建解释器(调用平台的bookmark.tcl脚本来创建书签窗口)
	IInterp* pInterp = (IInterp*)(theApp.m_pIPlatUI->CreateVciObject("org.interp.tcl", ""));

	// 获取平台Bookmark脚本并加载
	CString strCreateScript = theApp.GetPlatLibPath();
	strCreateScript += _T("plat\\bookmark.tcl");
	strCreateScript.Replace("\\", "/");
	CString strPathName = GetDocument()->GetPathName();
	strPathName.Replace("\\", "/");
	CString strCmd;
	strCmd.Format("source %s;\
					_plat_Tcl_CreateBookmarks \"%s\";",
					strCreateScript, strPathName);
	pInterp->RunScriptCommand(strCmd);
	int nLine = pInterp->GetErrorLine();
	CString strRes = pInterp->GetResult();
	// 释放解释器
	theApp.m_pIPlatUI->ReleaseVciObject(pInterp, FALSE);
	int nOT = 0;
	if(nLine != 0)	// 行号为0表示没有错误
	{
		AfxMessageBox(strRes);
		return;
	}
	nOT = atoi(strRes);	// 返回窗口ID

	// 添加所有书签到书签列表窗口
	CUIntArray auBookmarks;
	linemarkGetAll(MARK_BOOKMARK, auBookmarks);
	for(int i=0; i<auBookmarks.GetSize(); i++)
	{
		CString strLine;
		int nLineLength = m_SciEdit.getLineLength(auBookmarks[i]-1);
		TCHAR* pszLine = new TCHAR[nLineLength+3];	// 长度+3,因为行结束符有可能占用两个位置
		memset(pszLine, 0, nLineLength+3);
		m_SciEdit.getLine(auBookmarks[i]-1, pszLine, nLineLength);
		strLine = pszLine;
		delete [] pszLine;
		
		CString strTmp;
		strTmp.Format("<ROOT,25>%d,%d", i+1, auBookmarks[i]);
		theApp.m_pIPlatUI->Output(nOT, strTmp);
		strTmp.Format("<I>Set Item<%d,2> TEXT %s", i, strLine);
		theApp.m_pIPlatUI->Output(nOT, strTmp);
		strTmp.Format("<I>Set Script<%d> <1>", i);
		theApp.m_pIPlatUI->Output(nOT, strTmp);
	}
}

// 刷新书签列表
void COwmSciteView::OnWatchBookmarks() 
{
	if(theApp.m_pIPlatUI)
	{
		theApp.m_pIPlatUI->CreateDockingPane("org.owm.edit", VIEW_ID_BOOKMARK);
	}
	ViewBookmarksWindow();
}

void COwmSciteView::OnUpdateWatchBookmarks(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

// 添加/取消书签
void COwmSciteView::OnEditBookmarks() 
{
	bookmarkToggle(-1);
}

void COwmSciteView::OnUpdateEditBookmarks(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

// 清除所有书签
void COwmSciteView::OnEditClearbookmarks() 
{
	bookmarkClearAll();
}

void COwmSciteView::OnUpdateEditClearbookmarks(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

// 下一个书签
void COwmSciteView::OnEditBookmarkNext() 
{
	bookmarkNext(true);
}

void COwmSciteView::OnUpdateEditBookmarkNext(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

// 上一个书签
void COwmSciteView::OnEditBookmarkPrev() 
{
	bookmarkNext(false);
}

void COwmSciteView::OnUpdateEditBookmarkPrev(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////////////

// 设置文档图标
void COwmSciteView::SetDocIcon(CString strIconType)
{
	if(theApp.m_pIPlatUI)
	{
		//theApp.m_pIPlatUI->SetVciDocIcon("org.owm.edit",
		//	GetDocument()->m_strDocId, strIconType, ::GetParent(GetSafeHwnd()));

		if(strIconType == "debug")	// 调试模式
		{
			theApp.m_pIPlatUI->SetDocumentTitle("", 2, ::GetParent(GetSafeHwnd()));
		}else
		if(strIconType == "slavedebug")	// 从调试模式
		{
			theApp.m_pIPlatUI->SetDocumentTitle("", 3, ::GetParent(GetSafeHwnd()));
		}else
		{
			// 正常模式
			theApp.m_pIPlatUI->SetDocumentTitle("", 0, ::GetParent(GetSafeHwnd()));
		}
	}
}

// 脚本调试回调函数
int ScriptDebugCallback(int nEvent, LPCTSTR lpszScriptFile, int nLine)
{
	COwmSciteView* pView = GetSciteEditView(lpszScriptFile);
	if(!pView)
	{
		return FALSE;
	}

	if(nEvent == IDB_USER_SETCURRENTLINE)
	{
		// 判断nLine为0则清除当前行标记然后退出
		if(nLine == 0)
		{
			pView->executeThreadSciCommand(SCI_MARKERDELETEALL, MARK_RUNCURRENT);
			return TRUE;
		}

		// 激活文件(调用OwmEdit的打开文件命令,如果文件存在,就执行激活操作)
		if(theApp.m_pIPlatUI)
		{
			theApp.m_pIPlatUI->SendOwmCmd("org.owm.edit", OC_OPENFILE, (WPARAM)lpszScriptFile, 0, NULL);
		}

		// 设置当前行
		nLine--;
		pView->executeThreadSciCommand(SCI_GOTOLINE, nLine);	// 跳转到当前行
		pView->executeThreadSciCommand(SCI_MARKERADD, nLine, MARK_RUNCURRENT);	// 设置当前行标记
		// 更新调试窗口
		CString strRes;
		if(pView->m_pInterp)
		{
			pView->m_pInterp->RefreshDebugWindow("all", FALSE, strRes);
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 执行脚本
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::EvalScript(int nRunMode) 
{
	if(!theApp.m_pIPlatUI)
	{
		DLL_LOADSTRING(strTmp, IDS_PROMPT_PLATUI_ERROR);	// 获取平台接口失败,请使用更高版本的平台!
		AfxMessageBox(strTmp);
		return;
	}

	if(NULL == GetDocument())
	{
		return;
	}

	CString strTmp;

	// 如果是从调试模式,则初始化解释器ID
	if(m_bSlaveDebug)
	{
		// 获取解释器名,和主解释器指针
		CString strInterpName = AfxGetApp()->GetProfileString(REG_OWMEDIT, REG_INTERPNAME, INTERP_NAME);
		m_pInterp = (IInterp*)(theApp.m_pIPlatUI->GetObjectByInstanceName(strInterpName));

		if(m_pInterp == NULL)
		{
			DLL_LOADSTRING(strTmp, IDS_PROMPT_NOTFOUND_INTERP);	// 平台未安装脚本执行模块,请先安装相应的脚本执行器!
			AfxMessageBox(strTmp);
		}

		return;
	}

	// 调试模式,需要判断是否有其他调试任务,并设置标志
	if(SCRIPT_RUN_NORMAL != nRunMode)
	{
		if(theApp.m_bScriptDebuging)
		{
			DLL_LOADSTRING(strTmp, IDS_PROMPT_SCRIPT_CANTSTOP);	// 已经有脚本正在调试运行，请等待完成或终止任务！
			AfxMessageBox(strTmp, MB_ICONINFORMATION);
			return;
		}
		theApp.m_bScriptDebuging = TRUE;
	}

	CString strPathName = GetDocument()->GetPathName();

	// 激活调试窗口,输出调试信息
	theApp.m_pIPlatUI->OutputClear(_WND_DEBUG);
	theApp.m_pIPlatUI->ActiveOutput(_WND_DEBUG);
	theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=0,0,0;FONT=FixedSys;EFFECT=NOEFFECT");
	CString strExecuteInterp = theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType));
	strTmp.Format("--------------------OwmEdit : Debug %s Script--------------------\r\n", strExecuteInterp);
	theApp.m_pIPlatUI->Output(_WND_DEBUG, strTmp);

	// if modified or new document, save modified before run script
	if(GetDocument()->IsModified() || (GetDocument()->GetPathName()==""))
	{
		SendMessage(WM_COMMAND, ID_FILE_SAVE);
		strPathName = GetDocument()->GetPathName();
		if(strPathName == "")
		{
			theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=255,0,0");
			theApp.m_pIPlatUI->Output(_WND_DEBUG, "Error: empty file name to eval!\r\n");
			// 调试模式,恢复标志
			if(SCRIPT_RUN_NORMAL != nRunMode)
			{
				theApp.m_bScriptDebuging = FALSE;
			}
			return;
		}
		strTmp.Format("Save File: %s\r\n", strPathName);
		theApp.m_pIPlatUI->Output(_WND_DEBUG, strTmp);
	}

	// 清除所有覆盖率分析行
	executeThreadSciCommand(SCI_MARKERDELETEALL, MARK_PROFILELINE);
	// 清除错误行标记
	executeThreadSciCommand(SCI_MARKERDELETEALL, MARK_ERRORLINK);
	// 清除当前行标记
	executeThreadSciCommand(SCI_MARKERDELETEALL, MARK_CURRENTLINE);

	// 获取解释器名,创建解释器
	CString strInterpName = AfxGetApp()->GetProfileString(REG_OWMEDIT, REG_INTERPNAME, INTERP_NAME);
	strTmp.Format("Create %s Interpret: %s\r\n", strExecuteInterp, strInterpName);
	theApp.m_pIPlatUI->Output(_WND_DEBUG, strTmp);

	// 运行脚本前是否重置控制台的同名解释器,避免第二次运行时候因为解释器未释放导致运行结果有偏差
	BOOL bAutoResetInterp = AfxGetApp()->GetProfileInt(REG_OWMEDIT, REG_AUTORESET_INTERP, TRUE);
	if(bAutoResetInterp)
	{
		theApp.m_pIPlatUI->SendOwmCmd("org.owm.terminal", OC_TERM_DOCK_SWITCHSHELL, (WPARAM)(LPCTSTR)strExecuteInterp, (LPARAM)(LPCTSTR)strInterpName, NULL);
	}

	m_pInterp = (IInterp*)(theApp.m_pIPlatUI->CreateVciObject(strExecuteInterp, strInterpName));

	if(m_pInterp == NULL)
	{		
		theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=255,0,0");
		theApp.m_pIPlatUI->Output(_WND_DEBUG, "Error: not install script interp to eval!\r\n");
		DLL_LOADSTRING(strTmp, IDS_PROMPT_NOTFOUND_INTERP);	// 平台未安装脚本执行模块,请先安装相应的脚本执行器!
		AfxMessageBox(strTmp);
		// 调试模式,恢复标志
		if(SCRIPT_RUN_NORMAL != nRunMode)
		{
			theApp.m_bScriptDebuging = FALSE;
		}
		return;
	}

	// 设置文档标签为调试态
	SetDocIcon("debug");

	// 设置为只读模式
	executeThreadSciCommand(SCI_SETREADONLY, TRUE);

	// 安装调试回调函数
	IVciControl* pVciControl = (IVciControl*)(theApp.m_pIPlatUI->GetObjectControlPtrByInstancePtr(m_pInterp));
	if(pVciControl)
	{
		pVciControl->InstallHandler(CALLBACK_EVENT_INTERP_DEBUG, ScriptDebugCallback, NULL);
	}

	// 记录当前路径,并设置当前路径(add at 2004-09-07)
	char szOldPath[256];
	memset(szOldPath, 0, 256);
	DWORD dwLen = GetCurrentDirectory(255, szOldPath);
	CString strPath = strPathName;
	strPath.Replace("/", "\\");
	int nPos = strPath.ReverseFind('\\');
	if(nPos >= 0)
	{
		strPath = strPath.Left(nPos);
	}
	SetCurrentDirectory(strPath);

	// 处于调试模式,则准备调试器
	if(SCRIPT_RUN_NORMAL != nRunMode)
	{
		theApp.m_pIPlatUI->Output(_WND_DEBUG, "Pretreatment interp debug environment...\r\n");

		// 切换到调试环境的窗口布局
		//...

		CString strRes;
		if(!m_pInterp->InitialDebugMode(strRes))
		{
			theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=255,0,0");
			strTmp.Format("Initial interp debug environment error:\r\n%s\r\n", strRes);
			theApp.m_pIPlatUI->Output(_WND_DEBUG, strTmp);
			theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=0,0,0");
		}
	}

	CString strModeName = (SCRIPT_RUN_NORMAL==nRunMode) ? "run" : "debug";
	strTmp.Format("Start eval script %s at %s mode...\r\n", strPathName, strModeName);
	theApp.m_pIPlatUI->Output(_WND_DEBUG, strTmp);

	// 设置运行模式
	m_pInterp->SetRunMode(nRunMode);

	// 获取编辑器断点列表,并设置到解释器中
	CUIntArray auBreakList;
	int nLineNo = -1;
	while((nLineNo = executeThreadSciCommand(SCI_MARKERNEXT, nLineNo, (1 << MARK_BREAKPOINT))) != -1)
	{
		auBreakList.Add(nLineNo+1);
		nLineNo++;
	}
	m_pInterp->SetBreakList(strPathName, auBreakList);

	// 保存puts/print命令的输出窗口,并暂时设置为调试窗口
	int nOT = AfxGetApp()->GetProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_OUTPUT_ID, 2);
	if(SCRIPT_RUN_NORMAL != nRunMode)
	{
		AfxGetApp()->WriteProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_OUTPUT_ID, _WND_DEBUG);
	}

	m_pInterp->SetScriptFile(strPathName);
	if(m_pInterp->GetInterpParam("istkview") == "1")
	{
		theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=255,0,0");
		theApp.m_pIPlatUI->Output(_WND_DEBUG, "Script is a TkView file, please execute with TkView module!\r\n");
		theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=0,0,0");
	}else
	{
		m_pInterp->RunScriptFile(strPathName);
		int nLine = m_pInterp->GetErrorLine();

		if(nLine != 0)	// 行号为0表示没有错误
		{
			if(nLine > 0)
			{
				nLine--;	// 从0开始计算的行号
				// if run error, set execution flag of the error line
				/*
				DWORD dwFlags = LocateTextBuffer()->GetLineFlags(nLine);
				DWORD dwMask = LF_EXECUTION;
				LocateTextBuffer()->SetLineFlag(nLine, dwMask, (dwFlags & dwMask) == 0);
				LocateTextBuffer()->EnsureVisible(CPoint(0, nLine));
				*/
				executeThreadSciCommand(SCI_GOTOLINE, nLine);
				executeThreadSciCommand(SCI_MARKERDELETEALL, MARK_ERRORLINK);
				executeThreadSciCommand(SCI_MARKERADD, nLine, MARK_ERRORLINK);
				if(L_TCL == m_LangType)
				{
					theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=255,0,0");
					strTmp.Format("\r\nScript eval error at line[%d]\r\n", nLine+1);
					theApp.m_pIPlatUI->Output(_WND_DEBUG, strTmp);
					theApp.m_pIPlatUI->Output(_WND_DEBUG, "Result is: ");
					CString strCmd;
					strCmd.Format(_T("regsub -all -line {^td \\*\\d+;} $errorInfo {} errorInfo; puts -%d $errorInfo"), _WND_DEBUG);
					m_pInterp->RunScriptCommand(strCmd);
					theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=0,0,0");
				}else
				{
					theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=255,0,0");
					strTmp.Format("\r\nScript eval error at line[%d]\r\n", nLine+1);
					theApp.m_pIPlatUI->Output(_WND_DEBUG, strTmp);
					theApp.m_pIPlatUI->Output(_WND_DEBUG, "Result is: ");
					theApp.m_pIPlatUI->Output(_WND_DEBUG, m_pInterp->GetResult());
					theApp.m_pIPlatUI->Output(_WND_DEBUG, "\r\n");
					theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=0,0,0");
				}
			}
		}else
		{
			theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=128,0,255");
			theApp.m_pIPlatUI->Output(_WND_DEBUG, "Script eval over and return OK.\r\n");
			CString strRes = m_pInterp->GetResult();
			if(strRes.GetLength()> 0)
			{
				theApp.m_pIPlatUI->Output(_WND_DEBUG, "Result is: ");
				theApp.m_pIPlatUI->Output(_WND_DEBUG, m_pInterp->GetResult());
				theApp.m_pIPlatUI->Output(_WND_DEBUG, "\r\n");
			}
			theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=0,0,0");
		}

		// 处于调试模式,执行覆盖率分析
		if(SCRIPT_RUN_NORMAL != nRunMode)
		{
			// 判断覆盖率分析功能是否打开,输出覆盖率分析结果
			if(m_pInterp->GetEnableProfileAnalyse())
			{
				// 获取解释器的覆盖率Map
				int nTDLineCount = m_pInterp->GetAvailableLineCount();
				CMap<int,int,int,int>* pProfileMap = (CMap<int,int,int,int>*)(m_pInterp->GetProfileMap());
				if(pProfileMap)
				{
					int nProfileLines = 0;
					int nLine=0;
					int nProfile;
					POSITION pos = pProfileMap->GetStartPosition();
					while (pos != NULL)
					{
						pProfileMap->GetNextAssoc( pos, nLine, nProfile );
						if(nProfile && (nLine > 0))
						{
							//SetProfileLine(nLine-1);
							nProfileLines++;
						}
					}

					if(nTDLineCount > 0)
					{
						// 输出效率信息
						theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=256,0,128");
						theApp.m_pIPlatUI->Output(_WND_DEBUG, "Script profile analyse:\r\n");
						theApp.m_pIPlatUI->Output(_WND_DEBUG, "--------------------------\r\n");
						strTmp.Format("Totle tag lines  : %d\r\n", nTDLineCount);
						theApp.m_pIPlatUI->Output(_WND_DEBUG, strTmp);
						strTmp.Format("Cover with lines : %d\r\n", nProfileLines);
						theApp.m_pIPlatUI->Output(_WND_DEBUG, strTmp);
						strTmp.Format("Cover percent    : %d%%\r\n", nProfileLines*100/nTDLineCount);
						theApp.m_pIPlatUI->Output(_WND_DEBUG, strTmp);
						theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=0,0,0");
					}
				}
			}
		}
	}

	// 执行结束,恢复当前路径的设置(add at 2004-09-07)
	if(dwLen > 0)
	{
		CString strOldPath = szOldPath;
		SetCurrentDirectory(strOldPath);
	}

	if(SCRIPT_RUN_NORMAL != nRunMode)
	{
		// 恢复puts命令的输出窗口设置
		AfxGetApp()->WriteProfileInt(REG_DIRECT_SUBKEY, REG_DIRECT_STDOUT_OUTPUT_ID, nOT);
	}

	// 卸载调试回调函数
	if(pVciControl)
	{
		pVciControl->UninstallHandler(CALLBACK_EVENT_INTERP_DEBUG, ScriptDebugCallback);
	}

	// 加一些延时,否则结束解释器太快容易导致解释器未完全释放线程就退出了,会引发异常
	Sleep(100);

	// 释放解释器
	theApp.m_pIPlatUI->ReleaseVciObject(m_pInterp, FALSE);
	m_pInterp = NULL;

	// 恢复为读写模式
	executeThreadSciCommand(SCI_SETREADONLY, FALSE);

	// 恢复文档标签为正常态
	SetDocIcon("normal");

	// 调试模式,恢复标志
	if(SCRIPT_RUN_NORMAL != nRunMode)
	{
		theApp.m_bScriptDebuging = FALSE;
	}

	// 切换回原始窗口布局
	//...
}

// 线程参数
struct _S_THREAD_SCRIPT{
	COwmSciteView*	pSciteView;
	int				nRunMode;
};

//////////////////////////////////////////////////////////////////
// 脚本执行线程
//////////////////////////////////////////////////////////////////
ULONG _stdcall DoEvalScriptThread(LPVOID pParam)
{
	// 设置线程中的语言类型
	SetLocateInThread();

	_S_THREAD_SCRIPT* p = (_S_THREAD_SCRIPT*)pParam;
	p->pSciteView->EvalScript(p->nRunMode);
	delete p;

	AfxEndThread(200);
	return 200;
}

/////////////////////////////////////////////////////////////////////////////
// 线程方式执行脚本文件
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::EvalScriptThread(int nRunMode)
{
	if(!theApp.m_pIPlatUI)
	{
		DLL_LOADSTRING(strTmp, IDS_PROMPT_PLATUI_ERROR);	// 获取平台接口失败,请使用更高版本的平台!
		AfxMessageBox(strTmp);
		return;
	}

	_S_THREAD_SCRIPT* pThreadParam = new _S_THREAD_SCRIPT;
	pThreadParam->pSciteView = this;
	pThreadParam->nRunMode = nRunMode;

	HANDLE	hThread;
	DWORD dwThreadID;
	hThread = ::CreateThread(NULL
							  ,0
							  ,DoEvalScriptThread
							  ,(LPVOID)(pThreadParam)
							  ,0
							  ,&dwThreadID
							  );
}

/////////////////////////////////////////////////////////////////////////////
// 获取运行状态
/////////////////////////////////////////////////////////////////////////////
int COwmSciteView::GetRunState()
{
	try{
	// 增加异常捕获

	if(m_pInterp)
	{
		return m_pInterp->GetRunState();
	}

	}catch(...)
	{
	}

	return SCRIPT_STATE_IDLE;
}

/////////////////////////////////////////////////////////////////////////////
// 设置从调试模式
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::SetSlaveDebugMode(int nSlaveMode)
{
	// 设置从调试模式状态
	m_bSlaveDebug = (nSlaveMode == 1);
	if(nSlaveMode == 1)	// 从调试模式
	{
		// 如果设置为从调试模式,则需要先初始化,获取解释器ID
		EvalScriptThread(SCRIPT_RUN_DEBUG_STEP);
		SetDocIcon("slavedebug");
	}else
	if(nSlaveMode == 0)	// 主调试模式
	{
		// 如果设置为主调试模式,只需要更新文档图标
		SetDocIcon("debug");
	}else	// -1,正常模式
	{
		// 如果设置为正常模式,需要把解释器置空
		SetDocIcon("");
		m_pInterp = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// 编译脚本
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::DoCompile()
{
	if(!theApp.m_pIPlatUI)
	{
		DLL_LOADSTRING(strTmp, IDS_PROMPT_PLATUI_ERROR);	// 获取平台接口失败,请使用更高版本的平台!
		AfxMessageBox(strTmp);
		return;
	}

	CString strPathName = GetDocument()->GetPathName();

	// 激活调试窗口,输出信息
	theApp.m_pIPlatUI->OutputClear(_WND_DEBUG);
	theApp.m_pIPlatUI->ActiveOutput(_WND_DEBUG);
	theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=0,0,0;FONT=FixedSys;EFFECT=NOEFFECT");
	CString strExecuteInterp = theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType));
	CString strTmp;
	strTmp.Format("--------------------OwmEdit : Compile %s Script--------------------\r\n", strExecuteInterp);
	theApp.m_pIPlatUI->Output(_WND_DEBUG, strTmp);

	// if modified or new document, save modified before compile script
	if(GetDocument()->IsModified() || (GetDocument()->GetPathName()==""))
	{
		SendMessage(WM_COMMAND, ID_FILE_SAVE);
		strPathName = GetDocument()->GetPathName();
		if(strPathName == "")
		{
			theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=255,0,0");
			theApp.m_pIPlatUI->Output(_WND_DEBUG, "Error: empty file name to compile!\r\n");
			return;
		}
		strTmp.Format("Save File: %s\r\n", strPathName);
		theApp.m_pIPlatUI->Output(_WND_DEBUG, strTmp);
	}

	// 获取解释器名,创建解释器
	CString strInterpName = AfxGetApp()->GetProfileString(REG_OWMEDIT, REG_INTERPNAME, INTERP_NAME);
	strTmp.Format("Create %s Interpret: %s\r\n", strExecuteInterp, strInterpName);
	theApp.m_pIPlatUI->Output(_WND_DEBUG, strTmp);

	m_pInterp = (IInterp*)(theApp.m_pIPlatUI->CreateVciObject(strExecuteInterp, strInterpName));

	if(m_pInterp == NULL)
	{		
		theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=255,0,0");
		theApp.m_pIPlatUI->Output(_WND_DEBUG, "Error: not install script interp to compile!\r\n");
		DLL_LOADSTRING(strTmp, IDS_PROMPT_NOTFOUND_INTERP);	// 平台未安装脚本执行模块,请先安装相应的脚本执行器!
		AfxMessageBox(strTmp);
		return;
	}

	// 设置为只读模式
	executeThreadSciCommand(SCI_SETREADONLY, TRUE);

	// 设置运行模式
	m_pInterp->SetRunMode(SCRIPT_RUN_CANSTOP);

	strTmp.Format("Start compile script %s ...\r\n", strPathName);
	theApp.m_pIPlatUI->Output(_WND_DEBUG, strTmp);

	// 编译
	m_pInterp->SetScriptFile(strPathName);
	CString strDestFile = "";
	BOOL bSucc = m_pInterp->Compile("", strDestFile, "");
	if(bSucc)
	{
		theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=128,0,255");
		theApp.m_pIPlatUI->Output(_WND_DEBUG, "Compile script finished!\r\n");
		strTmp.Format("Output compiled file : %s\r\n", strDestFile);
		theApp.m_pIPlatUI->Output(_WND_DEBUG, strTmp);
		theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=0,0,0");
	}else
	{
		theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=255,0,0");
		theApp.m_pIPlatUI->Output(_WND_DEBUG, "\r\nCompile script failed!\r\n");
	}

	// 加一些延时,否则结束解释器太快容易导致解释器未完全释放线程就退出了,会引发异常
	Sleep(100);

	// 释放解释器
	theApp.m_pIPlatUI->ReleaseVciObject(m_pInterp, FALSE);
	m_pInterp = NULL;

	// 恢复为读写模式
	executeThreadSciCommand(SCI_SETREADONLY, FALSE);
}

// 线程参数
struct _S_THREAD_COMPILE{
	COwmSciteView*	pSciteView;
};

//////////////////////////////////////////////////////////////////
// 编译脚本线程
//////////////////////////////////////////////////////////////////
ULONG _stdcall DoCompileThread(LPVOID pParam)
{
	// 设置线程中的语言类型
	SetLocateInThread();

	_S_THREAD_COMPILE* p = (_S_THREAD_COMPILE*)pParam;
	p->pSciteView->DoCompile();
	delete p;

	AfxEndThread(200);
	return 200;
}

// 编译
void COwmSciteView::OnCompile() 
{
	_S_THREAD_COMPILE* pThreadParam = new _S_THREAD_COMPILE;
	pThreadParam->pSciteView = this;

	HANDLE	hThread;
	DWORD dwThreadID;
	hThread = ::CreateThread(NULL
							  ,0
							  ,DoCompileThread
							  ,(LPVOID)(pThreadParam)
							  ,0
							  ,&dwThreadID
							  );
}

void COwmSciteView::OnUpdateCompile(CCmdUI* pCmdUI) 
{
	BOOL bEnable = FALSE;
	if(theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType)) != "")
	{
		int nRunState = GetRunState();
		bEnable = ((SCRIPT_STATE_IDLE == nRunState) ||
					(SCRIPT_STATE_END == nRunState) ||
					(SCRIPT_STATE_STOP == nRunState));
	}
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 执行外部进程
/////////////////////////////////////////////////////////////////////////////
BOOL ExecProcess(CString strExePath, CString strParam, BOOL bWaitEnd)
{
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
 
	// Set up members of the PROCESS_INFORMATION structure. 
	ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );

	// Set up members of the STARTUPINFO structure. 
	ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
	siStartInfo.cb = sizeof(STARTUPINFO); 
	//siStartInfo.wShowWindow=SW_HIDE ;
	//siStartInfo.dwFlags=STARTF_USESHOWWINDOW;

	// 组合命令行
	CString strCmd;
	strCmd.Format("%s %s", strExePath, strParam);
	char cscmd[255];
	strcpy(cscmd, strCmd);
	
	// 创建报表进程
	BOOL bRet=CreateProcess(NULL,
      cscmd,		  // command line 
      NULL,          // process security attributes 
      NULL,          // primary thread security attributes 
      TRUE,          // handles are inherited 
      0,             // creation flags 
      NULL,          // use parent's environment 
      NULL,          // use parent's current directory 
      &siStartInfo,  // STARTUPINFO pointer 
      &piProcInfo);  // receives PROCESS_INFORMATION
	
	if(bWaitEnd)
	{
		// 等待进程结束
		WaitForSingleObject(piProcInfo.hProcess, 10000);
	}
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// 构建可执行文件以及运行
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::DoBuildAndRun(BOOL bRun) 
{
	if(!theApp.m_pIPlatUI)
	{
		DLL_LOADSTRING(strTmp, IDS_PROMPT_PLATUI_ERROR);	// 获取平台接口失败,请使用更高版本的平台!
		AfxMessageBox(strTmp);
		return;
	}

	CString strPathName = GetDocument()->GetPathName();

	// 激活调试窗口,输出信息
	theApp.m_pIPlatUI->OutputClear(_WND_DEBUG);
	theApp.m_pIPlatUI->ActiveOutput(_WND_DEBUG);
	theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=0,0,0;FONT=FixedSys;EFFECT=NOEFFECT");
	CString strExecuteInterp = theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType));
	CString strTmp;
	strTmp.Format("--------------------OwmEdit : Build %s Script--------------------\r\n", strExecuteInterp);
	theApp.m_pIPlatUI->Output(_WND_DEBUG, strTmp);

	// if modified or new document, save modified before compile script
	if(GetDocument()->IsModified() || (GetDocument()->GetPathName()==""))
	{
		SendMessage(WM_COMMAND, ID_FILE_SAVE);
		strPathName = GetDocument()->GetPathName();
		if(strPathName == "")
		{
			theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=255,0,0");
			theApp.m_pIPlatUI->Output(_WND_DEBUG, "Error: empty file name to build!\r\n");
			return;
		}
		strTmp.Format("Save File: %s\r\n", strPathName);
		theApp.m_pIPlatUI->Output(_WND_DEBUG, strTmp);
	}

	// 获取解释器名,创建解释器
	CString strInterpName = AfxGetApp()->GetProfileString(REG_OWMEDIT, REG_INTERPNAME, INTERP_NAME);
	strTmp.Format("Create %s Interpret: %s\r\n", strExecuteInterp, strInterpName);
	theApp.m_pIPlatUI->Output(_WND_DEBUG, strTmp);

	m_pInterp = (IInterp*)(theApp.m_pIPlatUI->CreateVciObject(strExecuteInterp, strInterpName));

	if(m_pInterp == NULL)
	{		
		theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=255,0,0");
		theApp.m_pIPlatUI->Output(_WND_DEBUG, "Error: not install script interp to build!\r\n");
		DLL_LOADSTRING(strTmp, IDS_PROMPT_NOTFOUND_INTERP);	// 平台未安装脚本执行模块,请先安装相应的脚本执行器!
		AfxMessageBox(strTmp);
		return;
	}

	// 设置为只读模式
	executeThreadSciCommand(SCI_SETREADONLY, TRUE);

	// 设置运行模式
	m_pInterp->SetRunMode(SCRIPT_RUN_CANSTOP);

	strTmp.Format("Start build script %s ...\r\n", strPathName);
	theApp.m_pIPlatUI->Output(_WND_DEBUG, strTmp);

	// 构建
	m_pInterp->SetScriptFile(strPathName);
	CString strDestFile = "";
	BOOL bSucc = m_pInterp->Build("", strDestFile, "");

	if(bSucc)
	{
		theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=128,0,255");
		theApp.m_pIPlatUI->Output(_WND_DEBUG, "Build script finished!\r\n");
		strTmp.Format("Output execute file : %s\r\n", strDestFile);
		theApp.m_pIPlatUI->Output(_WND_DEBUG, strTmp);
		theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=0,0,0");

		if(bRun)
		{
			if(GetFileAttributes(strDestFile) != 0xFFFFFFFF)
			{
				// 记录当前路径,并设置当前路径
				char szOldPath[256];
				memset(szOldPath, 0, 256);
				DWORD dwLen = GetCurrentDirectory(255, szOldPath);
				CString strPath = strDestFile;
				strPath.Replace("/", "\\");
				int nPos = strPath.ReverseFind('\\');
				if(nPos >= 0)
				{
					strPath = strPath.Left(nPos);
				}
				SetCurrentDirectory(strPath);

				ExecProcess(strDestFile, "", FALSE);

				// 执行结束,恢复当前路径的设置
				if(dwLen > 0)
				{
					CString strOldPath = szOldPath;
					SetCurrentDirectory(strOldPath);
				}
			}else
			{
				theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=255,0,0");
				theApp.m_pIPlatUI->Output(_WND_DEBUG, "Output file is not exist!\r\n"); 
			}
		}
	}else
	{
		theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=255,0,0");
		theApp.m_pIPlatUI->Output(_WND_DEBUG, "\r\nBuild script failed!\r\n");
	}

	// 加一些延时,否则结束解释器太快容易导致解释器未完全释放线程就退出了,会引发异常
	Sleep(100);

	// 释放解释器
	theApp.m_pIPlatUI->ReleaseVciObject(m_pInterp, FALSE);
	m_pInterp = NULL;

	// 恢复为读写模式
	executeThreadSciCommand(SCI_SETREADONLY, FALSE);
}

// 线程参数
struct _S_THREAD_BUILD{
	COwmSciteView*	pSciteView;
	BOOL			bRun;
};

//////////////////////////////////////////////////////////////////
// 构建可执行文件线程
//////////////////////////////////////////////////////////////////
ULONG _stdcall DoBuildThread(LPVOID pParam)
{
	// 设置线程中的语言类型
	SetLocateInThread();

	_S_THREAD_BUILD* p = (_S_THREAD_BUILD*)pParam;
	p->pSciteView->DoBuildAndRun(p->bRun);
	delete p;

	AfxEndThread(200);
	return 200;
}

/////////////////////////////////////////////////////////////////////////////
// 构建可执行文件(线程方式执行)
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnBuild() 
{
	_S_THREAD_BUILD* pThreadParam = new _S_THREAD_BUILD;
	pThreadParam->pSciteView = this;
	pThreadParam->bRun = FALSE;

	HANDLE	hThread;
	DWORD dwThreadID;
	hThread = ::CreateThread(NULL
							  ,0
							  ,DoBuildThread
							  ,(LPVOID)(pThreadParam)
							  ,0
							  ,&dwThreadID
							  );
}

void COwmSciteView::OnUpdateBuild(CCmdUI* pCmdUI) 
{
	BOOL bEnable = FALSE;
	if(theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType)) != "")
	{
		int nRunState = GetRunState();
		bEnable = ((SCRIPT_STATE_IDLE == nRunState) ||
					(SCRIPT_STATE_END == nRunState) ||
					(SCRIPT_STATE_STOP == nRunState));
	}
	pCmdUI->Enable(bEnable);
}

// 构建并运行
void COwmSciteView::OnBuildAndRun() 
{
	_S_THREAD_BUILD* pThreadParam = new _S_THREAD_BUILD;
	pThreadParam->pSciteView = this;
	pThreadParam->bRun = TRUE;

	HANDLE	hThread;
	DWORD dwThreadID;
	hThread = ::CreateThread(NULL
							  ,0
							  ,DoBuildThread
							  ,(LPVOID)(pThreadParam)
							  ,0
							  ,&dwThreadID
							  );
}

void COwmSciteView::OnUpdateBuildAndRun(CCmdUI* pCmdUI) 
{
	BOOL bEnable = FALSE;
	if(theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType)) != "")
	{
		int nRunState = GetRunState();
		bEnable = ((SCRIPT_STATE_IDLE == nRunState) ||
					(SCRIPT_STATE_END == nRunState) ||
					(SCRIPT_STATE_STOP == nRunState));
	}
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 运行脚本
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnOwmRun() 
{
	if(L_HTML == m_LangType)
	{
		// 显示页面
		CString strPathName = GetDocument()->GetPathName();
		theApp.m_pIPlatUI->SendOwmCmd("org.owm.htmlview", OC_VIEWHTMLPAGE, (WPARAM)(LPCTSTR)strPathName, 0, NULL);
	}else
	{
		// 执行脚本
		EvalScriptThread(SCRIPT_RUN_NORMAL);
	}
}

void COwmSciteView::OnUpdateOwmRun(CCmdUI* pCmdUI) 
{
	BOOL bEnable = FALSE;
	if(theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType)) != "")
	{
		int nRunState = GetRunState();
		bEnable = ((SCRIPT_STATE_IDLE == nRunState) ||
					(SCRIPT_STATE_END == nRunState) ||
					(SCRIPT_STATE_STOP == nRunState));
	}
	if(L_HTML == m_LangType)
	{
		bEnable = TRUE;
	}
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 执行脚本(用文档指定的扩展模块打开)
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnOwmExecute() 
{
	// if modified or new document, save modified before run script
	if(GetDocument()->IsModified() || (GetDocument()->GetPathName()==""))
	{
		SendMessage(WM_COMMAND, ID_FILE_SAVE);
		CString strPathName = GetDocument()->GetPathName();
		if(strPathName == "")
		{
			theApp.m_pIPlatUI->OutputSet(_WND_DEBUG, "COLOR=255,0,0");
			theApp.m_pIPlatUI->Output(_WND_DEBUG, "Error: empty file name to execute!\r\n");
			return;
		}
		CString strTmp;
		strTmp.Format("Save File: %s\r\n", strPathName);
		theApp.m_pIPlatUI->Output(_WND_DEBUG, strTmp);
	}

	CStringArray arExecuteModule;
	int nCount = theApp.m_pIPlatUI->GetVciDocExecuteModule("org.owm.edit", getDocIdFromLangType(m_LangType), "owm", arExecuteModule);
	theApp.m_pIPlatUI->OpenDocumentFile(GetDocument()->GetPathName(), arExecuteModule[0]);
}

void COwmSciteView::OnUpdateOwmExecute(CCmdUI* pCmdUI) 
{
	BOOL bEnable = FALSE;
	if(theApp.m_pIPlatUI)
	{
		CStringArray arExecuteModule;
		int nCount = theApp.m_pIPlatUI->GetVciDocExecuteModule("org.owm.edit", getDocIdFromLangType(m_LangType), "owm", arExecuteModule);
		bEnable = (nCount > 0);
	}

	int nRunState = GetRunState();
	bEnable &= ((SCRIPT_STATE_IDLE == nRunState) ||
				(SCRIPT_STATE_END == nRunState) ||
				(SCRIPT_STATE_STOP == nRunState));

	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 评估脚本(只对一段脚本进行评估)
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnScriptEval() 
{
	CString text;
	int nSelCount = m_SciEdit.getSelectedTextCount();
	m_SciEdit.getGenericSelectedText(text.GetBuffer(nSelCount+1), nSelCount+1);
	text.ReleaseBuffer();

	// 获取解释器名
	CString strInterpName = AfxGetApp()->GetProfileString(REG_OWMEDIT, REG_INTERPNAME, INTERP_NAME);
	CString strExecuteInterp = theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType));

	// 创建解释器
	m_pInterp = (IInterp*)(theApp.m_pIPlatUI->CreateVciObject(strExecuteInterp, strInterpName));
	if(m_pInterp == NULL)
	{
		DLL_LOADSTRING(strTmp, IDS_PROMPT_NOTFOUND_INTERP);	// 平台未安装脚本执行模块,请先安装相应的脚本执行器!
		AfxMessageBox(strTmp);
		return;
	}

	// 调用解释器执行命令
	m_pInterp->RunScriptCommand(text);
	// 获取错误行
	int nLine = m_pInterp->GetErrorLine();
	CString strRes = m_pInterp->GetResult();
	if(strRes != "")
	{
		if(nLine != 0)	// 行号为0表示没有错误
		{
			AfxMessageBox(strRes, MB_ICONINFORMATION);
		}else
		{
			AfxMessageBox(strRes, MB_ICONERROR);
		}
	}

	// 释放解释器
	theApp.m_pIPlatUI->ReleaseVciObject(m_pInterp, FALSE);
	m_pInterp = NULL;
}

void COwmSciteView::OnUpdateScriptEval(CCmdUI* pCmdUI) 
{
	BOOL bEnable = FALSE;
	if(theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType)) != "")
	{
		int nRunState = GetRunState();
		bEnable = ((SCRIPT_STATE_IDLE == nRunState) ||
					(SCRIPT_STATE_END == nRunState) ||
					(SCRIPT_STATE_STOP == nRunState));
	}
	bEnable &= (m_SciEdit.getSelectedTextCount() != 0);
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 中止脚本
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnStopRun() 
{
	m_pInterp->SetDebugEvent(IDB_STOP, 0, "");
}

void COwmSciteView::OnUpdateStopRun(CCmdUI* pCmdUI) 
{
	BOOL bEnable = (theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType)) != "");
	int nRunState = GetRunState();
	bEnable &= ((SCRIPT_STATE_RUN == nRunState) ||
				(SCRIPT_STATE_BREAK == nRunState) ||
				(SCRIPT_STATE_STEP == nRunState));
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 调试执行脚本
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnGo() 
{
	int nRunState = GetRunState();
	if( (SCRIPT_STATE_IDLE == nRunState) ||
		(SCRIPT_STATE_END == nRunState) ||
		(SCRIPT_STATE_STOP == nRunState) )
	{	// 空闲状态,则重新启动
		EvalScriptThread(SCRIPT_RUN_DEBUG_GO);
	}else
	if( (SCRIPT_STATE_BREAK == nRunState) ||
		(SCRIPT_STATE_STEP == nRunState) )
	{	// 单步或断点状态,则运行到下一个断点
		m_pInterp->SetDebugEvent(IDB_NEXT, 0, "");
	}
	m_strLastTooltipVarName = "";
}

void COwmSciteView::OnUpdateGo(CCmdUI* pCmdUI) 
{
	BOOL bEnable = (theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType)) != "");
	int nRunState = GetRunState();
	bEnable &= (SCRIPT_STATE_RUN != nRunState);
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 单步进入过程
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnStepInto() 
{
	int nRunState = GetRunState();
	if( (SCRIPT_STATE_IDLE == nRunState) ||
		(SCRIPT_STATE_END == nRunState) ||
		(SCRIPT_STATE_STOP == nRunState) )
	{	// 空闲状态,则重新启动
		EvalScriptThread(SCRIPT_RUN_DEBUG_STEP);
	}else
	if( (SCRIPT_STATE_BREAK == nRunState) ||
		(SCRIPT_STATE_STEP == nRunState) )
	{	// 单步或断点状态,则运行到下一个断点
		m_pInterp->SetDebugEvent(IDB_STEPINTO, 0, "");
	}
	m_strLastTooltipVarName = "";
}

void COwmSciteView::OnUpdateStepInto(CCmdUI* pCmdUI) 
{
	BOOL bEnable = (theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType)) != "");
	int nRunState = GetRunState();
	bEnable &= (SCRIPT_STATE_RUN != nRunState);
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 单步
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnStepOver() 
{
	int nRunState = GetRunState();
	if( (SCRIPT_STATE_IDLE == nRunState) ||
		(SCRIPT_STATE_END == nRunState) ||
		(SCRIPT_STATE_STOP == nRunState) )
	{	// 空闲状态,则重新启动
		EvalScriptThread(SCRIPT_RUN_DEBUG_STEP);
	}else
	if( (SCRIPT_STATE_BREAK == nRunState) ||
		(SCRIPT_STATE_STEP == nRunState) )
	{	// 单步或断点状态,则运行到下一个断点
		m_pInterp->SetDebugEvent(IDB_STEP, 0, "");
	}
	m_strLastTooltipVarName = "";
}

void COwmSciteView::OnUpdateStepOver(CCmdUI* pCmdUI) 
{
	BOOL bEnable = (theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType)) != "");
	int nRunState = GetRunState();
	bEnable &= (SCRIPT_STATE_RUN != nRunState);
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 单步退出过程
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnStepOut() 
{
	int nRunState = GetRunState();
	if( (SCRIPT_STATE_IDLE == nRunState) ||
		(SCRIPT_STATE_END == nRunState) ||
		(SCRIPT_STATE_STOP == nRunState) )
	{	// 空闲状态,则重新启动
		EvalScriptThread(SCRIPT_RUN_DEBUG_STEP);
	}else
	if( (SCRIPT_STATE_BREAK == nRunState) ||
		(SCRIPT_STATE_STEP == nRunState) )
	{	// 单步或断点状态,则运行到下一个断点
		m_pInterp->SetDebugEvent(IDB_SETPOUT, 0, "");
	}
	m_strLastTooltipVarName = "";
}

void COwmSciteView::OnUpdateStepOut(CCmdUI* pCmdUI) 
{
	BOOL bEnable = (theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType)) != "");
	int nRunState = GetRunState();
	bEnable &= (SCRIPT_STATE_RUN != nRunState);
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 执行到指定行
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnRunTocursor() 
{
	int nRunState = GetRunState();
	if( (SCRIPT_STATE_BREAK == nRunState) ||
		(SCRIPT_STATE_STEP == nRunState) )
	{	// 单步或断点状态,则运行到指定行
		m_pInterp->SetDebugEvent(IDB_GOTO, m_SciEdit.getCurrentLineNumber()+1, "");
	}
	m_strLastTooltipVarName = "";
}

void COwmSciteView::OnUpdateRunTocursor(CCmdUI* pCmdUI) 
{
	BOOL bEnable = (theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType)) != "");
	int nRunState = GetRunState();
	bEnable &= (SCRIPT_STATE_RUN != nRunState);
	pCmdUI->Enable(bEnable);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// 断点操作

/////////////////////////////////////////////////////////////////////////////
// 设置/清除断点
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnInsertBreak() 
{
	linemarkToggle(MARK_BREAKPOINT, -1);

	// 获取编辑器断点列表,并设置到解释器中
	if(m_pInterp)
	{
		CUIntArray auBreakList;
		linemarkGetAll(MARK_BREAKPOINT, auBreakList);
		m_pInterp->SetBreakList(GetDocument()->GetPathName(), auBreakList);
	}
}

void COwmSciteView::OnUpdateInsertBreak(CCmdUI* pCmdUI) 
{
	BOOL bEnable = (theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType)) != "");
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 清除所有断点
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnClearBreakall() 
{
	//ClearAllBreak();
	linemarkClearAll(MARK_BREAKPOINT);

	// 设置到解释器中
	if(m_pInterp)
	{
		CUIntArray auBreakList;
		//GetAllBreakPoint(&auBreakList);
		linemarkGetAll(MARK_BREAKPOINT, auBreakList);
		m_pInterp->SetBreakList(GetDocument()->GetPathName(), auBreakList);
	}
}

void COwmSciteView::OnUpdateClearBreakall(CCmdUI* pCmdUI) 
{
	BOOL bEnable = (theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType)) != "");
	pCmdUI->Enable(bEnable);
}

void COwmSciteView::OnConditionBreak() 
{
	
}

void COwmSciteView::OnUpdateConditionBreak(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}

void COwmSciteView::ViewBreaksWindow() 
{
	// 创建解释器(调用平台的watchbreak.tcl脚本来创建断点窗口)
	IInterp* pInterp = (IInterp*)(theApp.m_pIPlatUI->CreateVciObject("org.interp.tcl", ""));

	// 获取平台WatchBreaks脚本并加载
	CString strCreateScript = theApp.GetPlatLibPath();
	strCreateScript += _T("plat\\watchbreak.tcl");
	strCreateScript.Replace("\\", "/");
	CString strPathName = GetDocument()->GetPathName();
	strPathName.Replace("\\", "/");
	CString strCmd;
	strCmd.Format("source %s;\
					_plat_Tcl_CreateWatchBreaks \"%s\";",
					strCreateScript, strPathName);
	pInterp->RunScriptCommand(strCmd);
	int nLine = pInterp->GetErrorLine();
	CString strRes = pInterp->GetResult();
	// 释放解释器
	theApp.m_pIPlatUI->ReleaseVciObject(pInterp, FALSE);
	int nOT = 0;
	if(nLine != 0)	// 行号为0表示没有错误
	{
		AfxMessageBox(strRes);
		return;
	}
	nOT = atoi(strRes);	// 返回窗口ID

	// 添加所有断点到断点列表窗口
	CUIntArray auBreaks;
	linemarkGetAll(MARK_BREAKPOINT, auBreaks);
	for(int i=0; i<auBreaks.GetSize(); i++)
	{
		CString strLine;
		int nLineLength = m_SciEdit.getLineLength(auBreaks[i]-1);
		TCHAR* pszLine = new TCHAR[nLineLength+3];	// 长度+3,因为行结束符有可能占用两个位置
		memset(pszLine, 0, nLineLength+3);
		m_SciEdit.getLine(auBreaks[i]-1, pszLine, nLineLength);
		strLine = pszLine;
		delete [] pszLine;
		
		CString strTmp;
		strTmp.Format("<ROOT,14>%d,%d", i+1, auBreaks[i]);
		theApp.m_pIPlatUI->Output(nOT, strTmp);
		strTmp.Format("<I>Set Item<%d,2> TEXT %s", i, strLine);
		theApp.m_pIPlatUI->Output(nOT, strTmp);
		strTmp.Format("<I>Set Script<%d> <1>", i);
		theApp.m_pIPlatUI->Output(nOT, strTmp);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 查看断点列表
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnWatchBreaks() 
{
	if(theApp.m_pIPlatUI)
	{
		theApp.m_pIPlatUI->CreateDockingPane("org.owm.edit", VIEW_ID_BREAKPOINT);
	}
	ViewBreaksWindow();
}

void COwmSciteView::OnUpdateWatchBreaks(CCmdUI* pCmdUI) 
{
	BOOL bEnable = (theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType)) != "");
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 查看变量
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnWatchVars() 
{
	CString strRes;
	m_pInterp->RefreshDebugWindow("var", TRUE, strRes);
}

void COwmSciteView::OnUpdateWatchVars(CCmdUI* pCmdUI) 
{
	BOOL bEnable = (theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType)) != "");
	int nRunState = GetRunState();
	bEnable &= ((SCRIPT_STATE_BREAK == nRunState) || (SCRIPT_STATE_STEP == nRunState));
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 查看对象
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnWatchObject() 
{
	CString strRes;
	m_pInterp->RefreshDebugWindow("object", TRUE, strRes);
}

void COwmSciteView::OnUpdateWatchObject(CCmdUI* pCmdUI) 
{
	BOOL bEnable = (theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType)) != "");
	int nRunState = GetRunState();
	bEnable &= ((SCRIPT_STATE_BREAK == nRunState) || (SCRIPT_STATE_STEP == nRunState));
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 查看过程
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnWatchProc() 
{
	CString strRes;
	m_pInterp->RefreshDebugWindow("proc", TRUE, strRes);
}

void COwmSciteView::OnUpdateWatchProc(CCmdUI* pCmdUI) 
{
	BOOL bEnable = (theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType)) != "");
	int nRunState = GetRunState();
	bEnable &= ((SCRIPT_STATE_BREAK == nRunState) || (SCRIPT_STATE_STEP == nRunState));
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 查看堆栈
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnWatchCallstack() 
{
	CString strRes;
	m_pInterp->RefreshDebugWindow("stack", TRUE, strRes);
}

void COwmSciteView::OnUpdateWatchCallstack(CCmdUI* pCmdUI) 
{
	BOOL bEnable = (theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType)) != "");
	int nRunState = GetRunState();
	bEnable &= ((SCRIPT_STATE_BREAK == nRunState) || (SCRIPT_STATE_STEP == nRunState));
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 单词帮助
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnWordHelp() 
{
	SearchWordHelp();
}

void COwmSciteView::OnUpdateWordHelp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

CString COwmSciteView::GetCurrentWord()
{
	CString strWord = _T("");
	TCHAR curWord[256];
	memset(curWord, 0, 256);
	m_SciEdit.getWordToCurrentPos(curWord, 255);
	strWord = curWord;

	int curPos = int(m_SciEdit.execute(SCI_GETCURRENTPOS));
	while(true)
	{
		char buf[2];
		buf[0] = buf[1] = 0;
		m_SciEdit.getText(buf, curPos, curPos+1);
		/*
		if((buf[0] == 0) || (buf[0] == ' ') || (buf[0] == '(') || (buf[0] == '.')
			|| (buf[0] == '\t') || (buf[0] == '\r') || (buf[0] == '\n'))
		{
			break;
		}else
		{
			strWord += buf[0];
		}*/
		// 遇到非字符、数字、下划线就退出
		if( (buf[0] == '_') || ((buf[0]>='0') && (buf[0]<='9')) || 
			((buf[0]>='A') && (buf[0]<='Z')) || ((buf[0]>='a') && (buf[0]<='z')) )
		{
			strWord += buf[0];
		}else
		{
			break;
		}
		curPos++;
	}

	return strWord;
}

/////////////////////////////////////////////////////////////////////////////
// F1帮助,搜索并显示当前单词的帮助
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::SearchWordHelp(CString strSearchWord)
{
	// 1. Get the current word being type, if any
	CString strWord = _T("");
	if(strSearchWord != "")
	{
		strWord = strSearchWord;
	}else
	{
		strWord = GetCurrentWord();
	}
	strWord.TrimRight();

	/*if(strWord == "")
	{
		return;
	}*/

	// 2. Search word help
	CString strExecuteInterp = theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType));

	// 在动态帮助窗口中显示单词帮助
	BOOL bShowWordHelpInDynamicHelp = AfxGetApp()->GetProfileInt(REG_HELP_SUBKEY, REG_HELP_WORDHELP_IN_DYNHELP, TRUE);
	if(bShowWordHelpInDynamicHelp)
	{
		theApp.m_pIPlatUI->CreateDockingPane("org.owm.htmlview", VIEW_ID_DYNHELP);
	}

	// 是否精确查找
	BOOL bSearchWordAccurate = AfxGetApp()->GetProfileInt(REG_HELP_SUBKEY, REG_HELP_SEARCHWORD_ACCURATE, TRUE);

	// 如果没有打开索引数据库,则退出
	if(!(m_sqliteIndex.IsOpen()))
	{
		return;
	}

	CString strSql;
	strSql.Format("SELECT * FROM keyword WHERE word %s",
			bSearchWordAccurate ? "= '" + strWord + "'" : "LIKE '%" + strWord + "%'");
	CSqlStatement* stmt = m_sqliteIndex.Statement(strSql);
	if (stmt != NULL)
	{
		int nRowCount = 0;
		while (stmt->NextRow())
		{
			nRowCount++;
		}
		stmt->RestartSelect();

		CString strExecuteInterp = theApp.GetExecuteInterp(getDocIdFromLangType(m_LangType));
		CString strInterpName = theApp.m_pIPlatUI->GetVciName(strExecuteInterp);
		CString strURL;
		strURL.Format("%sdoc\\%s\\", theApp.GetPlatRootPath(), strInterpName);

		CString strDefaultPage;	// 默认页面
		if(LANGUAGE_CHINESE == theApp.m_nLanguage)
		{
			strDefaultPage.Format("%sdoc\\dynhelp_cn.htm", theApp.GetPlatRootPath());
		}else
		{
			strDefaultPage.Format("%sdoc\\dynhelp.htm", theApp.GetPlatRootPath());
		}

		if(nRowCount > 1)	// 大于1个查找到的单词,则选择
		{
			CTOptArray aopt;	// 创建属性树元素数组
			TOPTITEM item;		// 属性树元素
			item.hItem		= NULL;
			item.iType		= TOPT_GROUP;
			DLL_LOADSTRING( strGroup, IDS_SELECT_WORD_GROUP)
			item.lpszText	= strGroup;
			item.lpszEdit	= "";
			item.lpszList	= "";
			item.bCheck		= FALSE;
			item.iImage		= 0;
			item.iParent	= -1;
			aopt.Add(item);		// 添加根节点

			item.iType		= TOPT_RADIOBUTTON;
			item.iImage		= 0;
			item.iParent	= 0;

			CString strTmp;
			while (stmt->NextRow())
			{
				CString strName = stmt->ValueString(0);
				CString strLink = stmt->ValueString(1);
				strTmp.Format("%s[%s]", strName, strLink);
				item.lpszText = strTmp;
				aopt.Add(item);
			}

			// 添加标题和提示信息
			item.iType		= TOPT_MSG;
			DLL_LOADSTRING( strTitle, IDS_SELECT_WORD_TITLE)
			item.lpszText	= strTitle;
			DLL_LOADSTRING( strTipInfo, IDS_SELECT_WORD_TIPINFO)
			strTmp.Format(strTipInfo, strWord);
			item.lpszEdit	= strTmp;
			item.iParent	= -1;
			aopt.Add(item);

			// 执行属性树
			if(theApp.m_ol.CommandPlat(OC_EXECTREEOPTION, 0, (LPARAM)(&aopt)) == IDOK)
			{
				int nSelect = (int)(aopt[0].hItem);	// 被选中的元素索引,放在元素0的hItem中
				if(nSelect > 0)
				{
					stmt->RestartSelect();
					int nIndex = 1;
					while (stmt->NextRow())
					{
						if(nIndex == nSelect)
						{
							CString strLink = stmt->ValueString(1);
							strURL += strLink;
							break;
						}
						nIndex++;
					}
				}else
				{
					strURL = strDefaultPage;
				}
			}else
			{
				strURL = strDefaultPage;
			}
		}else
		if(nRowCount == 1)	// 只有一个查找到的单词,直接显示
		{
			stmt->NextRow();
			CString strLink = stmt->ValueString(1);
			strURL += strLink;
		}else
		{
			if(bShowWordHelpInDynamicHelp)
			{
				// 显示在动态帮助窗口中
				theApp.m_pIPlatUI->SendOwmCmd("org.owm.htmlview", OC_REFRESH_DOCKINGHTML, VIEW_ID_DYNHELP, (LPARAM)(LPCTSTR)strDefaultPage, NULL);
			}else
			{
				// 显示在HtmlView中
				theApp.m_pIPlatUI->SendOwmCmd("org.owm.htmlview", OC_VIEWHTMLPAGE, (WPARAM)(LPCTSTR)strDefaultPage, 0, NULL);
			}
			delete stmt;
			return;
		}

		if(bShowWordHelpInDynamicHelp)
		{
			// 显示在动态帮助窗口中
			theApp.m_pIPlatUI->SendOwmCmd("org.owm.htmlview", OC_REFRESH_DOCKINGHTML, VIEW_ID_DYNHELP, (LPARAM)(LPCTSTR)strURL, NULL);
		}else
		{
			// 显示在HtmlView中
			theApp.m_pIPlatUI->SendOwmCmd("org.owm.htmlview", OC_VIEWHTMLPAGE, (WPARAM)(LPCTSTR)strURL, 0, NULL);
		}

		delete stmt;
	}

}

/////////////////////////////////////////////////////////////////////////////
// 文件属性
/////////////////////////////////////////////////////////////////////////////
#define IDR_PANE_PROPERTIES 4001
void COwmSciteView::OnDocProperty() 
{
	if(theApp.m_pIPlatUI)
	{
		theApp.m_pIPlatUI->ActiveDockingPane(IDR_PANE_PROPERTIES);
	}
}

void COwmSciteView::OnUpdateDocProperty(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.m_pIPlatUI != NULL);
}

/////////////////////////////////////////////////////////////////////////////
// 放大
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnZoomIn() 
{
	m_SciEdit.execute(SCI_ZOOMIN);
	// 刷新属性窗口信息
	GetDocument()->RefreshDocProperty();
}

void COwmSciteView::OnUpdateZoomIn(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 缩小
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnZoomOut() 
{
	m_SciEdit.execute(SCI_ZOOMOUT);
	// 刷新属性窗口信息
	GetDocument()->RefreshDocProperty();
}

void COwmSciteView::OnUpdateZoomOut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 撤销缩放
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnZoomRestore() 
{
	m_SciEdit.execute(SCI_SETZOOM, 0);
	// 刷新属性窗口信息
	GetDocument()->RefreshDocProperty();
}

void COwmSciteView::OnUpdateZoomRestore(CCmdUI* pCmdUI) 
{
	BOOL bEnable = m_SciEdit.execute(SCI_GETZOOM) != 0;
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 关于编辑器
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnOwmAbout() 
{
	if(theApp.m_pIPlatUI)
	{
		theApp.m_pIPlatUI->SendOwmCmd("", OC_SHOWABOUTBOX, (WPARAM)(LPCTSTR)"org.owm.edit", 0, NULL);
	}
}

void COwmSciteView::OnUpdateOwmAbout(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 函数自动完成showAutoComp()
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnEditAutocomplete() 
{
	//bool isFromPrimary = _pEditView == &m_SciEdit;
	//AutoCompletion * autoC = isFromPrimary?&_autoComplete;
	AutoCompletion * autoC = &_autoComplete;
	autoC->showAutoComplete();
}

void COwmSciteView::OnUpdateEditAutocomplete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 单词自动完成autoCompFromCurrentFile(bool autoInsert)
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnEditCompleteKeyword() 
{
	AutoCompletion * autoC = &_autoComplete;
	autoC->showWordComplete(true);
}

void COwmSciteView::OnUpdateEditCompleteKeyword(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 函数参数提示showFunctionComp()
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnEditFunccalltip() 
{
	AutoCompletion * autoC = &_autoComplete;
	autoC->showFunctionComplete();
}

void COwmSciteView::OnUpdateEditFunccalltip(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 扩展的自动完成窗口
void COwmSciteView::DoAutoCompletePopupWnd(INSERTTYPE xType)
{
	// If the buffer is null, or the file is read-only/protected then abort the insert
	if (m_SciEdit.execute(SCI_GETREADONLY))
		return;
	
	// 1. Make sure the popup list box is created.
	if (m_listPopup.GetSafeHwnd() == NULL)	  
	{
		//TODO--cool error handler here!
		TRACE0("CCrystalView::OnCompleteWord()--m_listPopup not created.\n");
		return;
	}
	
	// 2. Find out the location of the caret (input point) and intialize
	//    all the necessary screen parameters.
	CPoint point        = CPoint(m_SciEdit.getCurrentPointX(), m_SciEdit.getCurrentPointY());  
	m_ptAutoComStartPos = point; //Save the current position
	CPoint ptCursor     = point;
	ClientToScreen(&point);
	
	// 3. Calculate the dimensions of the list box window, it will determine
	//    the dimension of the popup window.
	CWindowRect rcCaller(m_listPopup);
	rcCaller.left = point.x;
	rcCaller.top  = point.y + m_SciEdit.getTextHeight();
	
	// 4. Create the popup window object, the constructor is protected therefore
	//    we cannot use the C++ "new" keyword here rather the MFC CreateObject.
	CCrystalPopupWnd* pPopup = reinterpret_cast<CCrystalPopupWnd*>
				(RUNTIME_CLASS(CCrystalPopupWnd)->CreateObject());
	
	ASSERT(pPopup != NULL);
	if (pPopup == NULL) // For this rear case, which may be due to lack of memory
	{
		//TODO--display popup type error message
		// Free all allocated resources and do not proceed.
		m_listPopup.ResetContent();
		m_ptAutoComStartPos = CPoint(0, 0);
		return;
	}
	m_pAutoComPopupWnd = m_listPopup.m_pPopWnd = pPopup;
	
	// 5. Divide the view into four equal parts to determine the orientation
	//    of the popup window, then select the orientaion based on the current
	//    entering point in the view window.
	CRect rcClient, rcWhere;
	GetClientRect(rcClient);
	int cxHalfClient = rcClient.Width() / 2;
	int cyHalfClient = rcClient.Height() / 2;
	int nOrientation = CCrystalPopupWnd::CA_TOPLEFT;  // The default alignment
	
	for (int i = 0; i < 4; i++)
	{
		rcWhere = CRect((i % 2) * cxHalfClient,
			(i > 1) * cyHalfClient,
			(i % 2) * cxHalfClient + cxHalfClient,
			(i > 1) * cyHalfClient + cyHalfClient);
		
		if (rcWhere.PtInRect(ptCursor))
		{
			nOrientation = i + 1;
			break;
		}
	}
	
	// 6. Finally, initialize other popup parameters and display it. The popup
	//     window itself is created in the called to Display().
	pPopup->SetInsertType(xType);
	pPopup->SetTextHeight(m_SciEdit.getTextHeight());	  // Text height, based on the font size
	pPopup->SetSendViewKeys(TRUE);			  // Let the view receive keyboard
	// messages while popup displayed and the user types further.
	VERIFY(pPopup->Display(&m_listPopup, this, 
		rcCaller, (CCrystalPopupWnd::CURSORALIGN)nOrientation, 
		&m_listPopup.m_xPopupCtrlEvent, FALSE));
}

BOOL COwmSciteView::InsertTemplateText(int nIndex)
{
	// If the buffer is null, or the file is read-only/protected then abort the insert
	if (m_SciEdit.execute(SCI_GETREADONLY))
		return FALSE;

	// If the autocomplete handler is created, it also means the text is
	// already loaded-since it is created through the loading process.
	if (m_CodeSnipTemplateLoader.IsFileLoaded())
	{
		int caretPos = m_SciEdit.execute(SCI_GETCURRENTPOS);
		m_SciEdit.execute(SCI_BEGINUNDOACTION);
		m_SciEdit.insertGenericTextFrom(caretPos, m_CodeSnipTemplateLoader.GetFormattedText(nIndex));
		m_SciEdit.execute(SCI_GOTOPOS, caretPos+m_CodeSnipTemplateLoader.GetFormattedText(nIndex).GetLength());
		m_SciEdit.execute(SCI_ENDUNDOACTION);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 加载代码片段模板
/////////////////////////////////////////////////////////////////////////////
BOOL COwmSciteView::LoadCodeSnippetTemplates()
{
	// is loaded template file
	if (m_CodeSnipTemplateLoader.IsFileLoaded())
	{
		return TRUE;
	}

	if (m_LangType >= L_EXTERNAL)
	{
		return FALSE;
	}

	CString strFileName = "";
	CString strOwmPath = theApp.GetModulePath();
	strFileName += strOwmPath;
	strFileName += "\\Snippets\\";
	strFileName += ScintillaEditView::langNames[m_LangType].lexerName;
	// 中文模板名字后面加_cn
	if(LANGUAGE_CHINESE == theApp.m_nLanguage)
	{
		strFileName += "_cn";
	}
	strFileName += ".snip";
	if(GetFileAttributes(strFileName) == 0xFFFFFFFF)
	{
		AfxMessageBox(_T("The snippet file specified does not exists."));
		return FALSE;
	}

	return m_CodeSnipTemplateLoader.Load(strFileName);
}

/////////////////////////////////////////////////////////////////////////////
// 代码片段
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnEditCodesnippet() 
{
	//CodeSnippet* codeSnip = &_codeSnippet;
	//codeSnip->showCodeSnippet();
	// 加载代码片段文件
	LoadCodeSnippetTemplates();

	// 1. Initialize the popup list box, reset it!
	m_listPopup.ResetContent();	
	for (int i = 0; i < m_CodeSnipTemplateLoader.GetSize(); i++)
	{
		m_listPopup.AddString(m_CodeSnipTemplateLoader.GetTemplateName(i), 
			ACI_CLASS);
	}

	// 2. Resize to fix the content, displaying at most 10 items
	m_listPopup.AutoSizeWindow();
	
	// 3. Get the current word being type, if any
	CString strWord = _T("");
	//GetCurrentWord(strWord);
	//strWord.TrimRight();
	
	// 4. Select a matching of the current word in the list box
	int nIndex = m_listPopup.SelectString(-1, strWord);
	if (nIndex == LB_ERR || strWord.IsEmpty())
		m_listPopup.SetCurSel(-1);
	
	// 5. Finally, display the popup window with a language template flag
	DoAutoCompletePopupWnd(PLIT_TEMPLATE);
}

void COwmSciteView::OnUpdateEditCodesnippet(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 格式转换

/////////////////////////////////////////////////////////////////////////////
// 转换为大写
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnEditUppercase() 
{
	m_SciEdit.convertSelectedTextToUpperCase();
}

void COwmSciteView::OnUpdateEditUppercase(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SciEdit.execute(SCI_GETSELECTIONSTART) != m_SciEdit.execute(SCI_GETSELECTIONEND));
}

/////////////////////////////////////////////////////////////////////////////
// 转换为小写
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnEditLowercase() 
{
	m_SciEdit.convertSelectedTextToLowerCase();
}

void COwmSciteView::OnUpdateEditLowercase(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SciEdit.execute(SCI_GETSELECTIONSTART) != m_SciEdit.execute(SCI_GETSELECTIONEND));
}

// 编码类型转换
void COwmSciteView::EncodingConvert(UniMode umto)
{
	UniMode um = m_SciEdit.getUnicodeMode();

	switch(umto)
	{
		case uni8Bit:
		{
			if (um == uni8Bit)
				return;
			break;
		}
		case uniCookie:
		{
			if (um == uniCookie)
				return;

			if (um != uni8Bit)
			{
				m_SciEdit.execute(SCI_EMPTYUNDOBUFFER);
				return;
			}

			break;
		}
		case uniUTF8:
		{
			if (um == uniUTF8)
				return;

			if (um != uni8Bit)
			{
				m_SciEdit.execute(SCI_EMPTYUNDOBUFFER);
				return;
			}
			break;
		}

		case uni16BE:
		{
			if (um == uni16BE)
				return;

			if (um != uni8Bit)
			{
				m_SciEdit.execute(SCI_EMPTYUNDOBUFFER);
				return;
			}
			break;
		}

		case uni16LE:
		{
			if (um == uni16LE)
				return;
			if (um != uni8Bit)
			{
				m_SciEdit.execute(SCI_EMPTYUNDOBUFFER);
				return;
			}
			break;
		}
	}

	// Save the current clipboard content
	::OpenClipboard(AfxGetMainWnd()->GetSafeHwnd());
	HANDLE clipboardData = ::GetClipboardData(CF_TEXT);
	int len = ::GlobalSize(clipboardData);
	LPVOID clipboardDataPtr = ::GlobalLock(clipboardData);

	HANDLE allocClipboardData = ::GlobalAlloc(GMEM_MOVEABLE, len);
	LPVOID clipboardData2 = ::GlobalLock(allocClipboardData);

	::memcpy(clipboardData2, clipboardDataPtr, len);
	::GlobalUnlock(clipboardData);	
	::GlobalUnlock(allocClipboardData);	
	::CloseClipboard();

	saveCurrentPos();

	// Cut all text
	int docLen = m_SciEdit.getCurrentDocLen();
	m_SciEdit.execute(SCI_COPYRANGE, 0, docLen);
	m_SciEdit.execute(SCI_CLEARALL);

	//修改scite的编码类型
	m_SciEdit.setUnicodeMode(umto);
	if (umto == uni8Bit) 
	{	//either 0 or CJK codepage
		if (m_SciEdit.isCJK())
		{
			executeThreadSciCommand(SCI_SETCODEPAGE, m_SciEdit.getCodepage());	//you may also want to set charsets here, not yet implemented
		}
		else
		{
			executeThreadSciCommand(SCI_SETCODEPAGE, 0);
		}
	} 
	else
	{	//CP UTF8 for all unicode
		executeThreadSciCommand(SCI_SETCODEPAGE, SC_CP_UTF8);
	}

	// Paste the texte, restore buffer status
	m_SciEdit.execute(SCI_PASTE);
	restoreCurrentPos();

	// Restore the previous clipboard data
	::OpenClipboard(AfxGetMainWnd()->GetSafeHwnd());
	::EmptyClipboard(); 
	::SetClipboardData(CF_TEXT, clipboardData2);
	::CloseClipboard();

	//Do not free anything, EmptyClipboard does that
	m_SciEdit.execute(SCI_EMPTYUNDOBUFFER);
}

/////////////////////////////////////////////////////////////////////////////
// 转换为ANSI
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnFormatConv2Ansi() 
{
	EncodingConvert(uni8Bit);
}

void COwmSciteView::OnUpdateFormatConv2Ansi(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SciEdit.getUnicodeMode() != uni8Bit);
}

/////////////////////////////////////////////////////////////////////////////
// 转换为UTF-8 no BOM
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnFormatConv2AsUtf8() 
{
	EncodingConvert(uniCookie);
}

void COwmSciteView::OnUpdateFormatConv2AsUtf8(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SciEdit.getUnicodeMode() != uniCookie);
}

/////////////////////////////////////////////////////////////////////////////
// 转换为UTF-8
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnFormatConv2Utf8() 
{
	EncodingConvert(uniUTF8);
}

void COwmSciteView::OnUpdateFormatConv2Utf8(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SciEdit.getUnicodeMode() != uniUTF8);
}

/////////////////////////////////////////////////////////////////////////////
// 转换为UCS-2 BE
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnFormatConv2Ucs2be() 
{
	EncodingConvert(uni16BE);
}

void COwmSciteView::OnUpdateFormatConv2Ucs2be(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SciEdit.getUnicodeMode() != uni16BE);
}

/////////////////////////////////////////////////////////////////////////////
// 转换为UCS-2 LE
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnFormatConv2Ucs2le() 
{
	EncodingConvert(uni16LE);
}

void COwmSciteView::OnUpdateFormatConv2Ucs2le(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SciEdit.getUnicodeMode() != uni16LE);
}

/////////////////////////////////////////////////////////////////////////////
// 转换为Windows
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnFormatTowin() 
{
	m_SciEdit.setFormat(WIN_FORMAT);
	m_SciEdit.execute(SCI_CONVERTEOLS, SC_EOL_CRLF);
}

void COwmSciteView::OnUpdateFormatTowin(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SciEdit.getFormat() != WIN_FORMAT);
}

/////////////////////////////////////////////////////////////////////////////
// 转换为Mac
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnFormatTomac() 
{
	m_SciEdit.setFormat(MAC_FORMAT);
	m_SciEdit.execute(SCI_CONVERTEOLS, SC_EOL_CR);
}

void COwmSciteView::OnUpdateFormatTomac(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SciEdit.getFormat() != MAC_FORMAT);
}

/////////////////////////////////////////////////////////////////////////////
// 转换为UNIX
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnFormatTounix() 
{
	m_SciEdit.setFormat(UNIX_FORMAT);
	m_SciEdit.execute(SCI_CONVERTEOLS, SC_EOL_LF);
}

void COwmSciteView::OnUpdateFormatTounix(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SciEdit.getFormat() != UNIX_FORMAT);
}

/////////////////////////////////////////////////////////////////////////////
// 自动换行
/////////////////////////////////////////////////////////////////////////////
void COwmSciteView::OnWrap() 
{
	m_SciEdit.wrap(!m_SciEdit.isWrap());
}

void COwmSciteView::OnUpdateWrap(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_SciEdit.isWrap());
}

// 转换UTF-8字符串为ASCII字符串
CString ConvertUTF8StringToAscii(CString strUTF8)
{
	WcharMbcsConvertor *wmc = WcharMbcsConvertor::getInstance();
	const wchar_t *pTextW = wmc->char2wchar(strUTF8.GetBuffer(0), SC_CP_UTF8);
	const char *pTextA = wmc->wchar2char(pTextW, ::GetACP());
	CString strAscii = pTextA;
	strUTF8.ReleaseBuffer();
	return strAscii;
}

/////////////////////////////////////////////////////////////////////////////
// 在API表中查找指定单词的Tip信息
/////////////////////////////////////////////////////////////////////////////
BOOL COwmSciteView::GetWordTipInApiTable(CString strWord, CString& strTip, CString& strDesc, CString& strLibrary)
{
	if(!(m_sqliteIndex.IsOpen()))
	{
		return FALSE;
	}

	strTip = "";
	strDesc = "";
	strLibrary = "";
	CString strSql;
	strSql.Format("SELECT * FROM api WHERE word='%s'", strWord);
	CSqlStatement* stmt = m_sqliteIndex.Statement(strSql);
	if (stmt != NULL)
	{
		if (stmt->NextRow())
		{
			if(LANGUAGE_CHINESE == theApp.m_nLanguage)
			{
				strTip = ConvertUTF8StringToAscii(stmt->ValueString(5));	// tip_zh
				strDesc = ConvertUTF8StringToAscii(stmt->ValueString(7));	// desc_zh
				if(strTip == "(null)") strTip = "";
				if(strDesc == "(null)") strDesc = "";
			}
			// 如果是英文模式或者获取不到中文内容,则获取英文内容
			if(strTip == "")
			{
				strTip = stmt->ValueString(4);	// tip_en
			}
			if(strDesc == "")
			{
				strDesc = stmt->ValueString(6);	// desc_en
			}
			strLibrary = stmt->ValueString(8);	// library
			if(strTip == "(null)") strTip = "";
			if(strDesc == "(null)") strDesc = "";
			if(strLibrary == "(null)") strLibrary = "";

			delete stmt;
			return TRUE;
		}

		delete stmt;
	}
	return FALSE;
}

//CMutex	g_MutexToolHitTest;		// 防止OnToolHitTest函数重入的互斥量
/////////////////////////////////////////////////////////////////////////////
// Tooltip消息处理
/////////////////////////////////////////////////////////////////////////////
INT_PTR COwmSciteView::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

//	CSingleLock SingleLock(&g_MutexToolHitTest);
//	SingleLock.Lock();

	// check child windows first by calling CControlBar
	INT_PTR nHit = CWnd::OnToolHitTest(point, pTI);
	// 是否允许CWnd的Tooltip支持
	BOOL bEnableWordTooltip = AfxGetApp()->GetProfileInt(REG_HELP_SUBKEY, REG_HELP_WORD_TOOLTIP, TRUE);
	if(!bEnableWordTooltip)
	{
		//SingleLock.Unlock();
		return nHit;
	}
	//if (nHit != -1)
	//	return nHit;
	
	// 注:SCI_POSITIONFROMPOINT和SCI_POSITIONFROMPOINTCLOSE用法非常相似,但区别是如果当前位置没有字符或者超出了文档范围,
	// SCI_POSITIONFROMPOINTCLOSE会返回-1,SCI_POSITIONFROMPOINT则不会,因此此处必须用SCI_POSITIONFROMPOINTCLOSE,
	// 否则会导致光标移动到文档最后一行后面时候,pos值超出范围,导致后续的调用产生异常
	//int pos = m_SciEdit.execute(SCI_POSITIONFROMPOINT, point.x, point.y);
	int pos = m_SciEdit.execute(SCI_POSITIONFROMPOINTCLOSE, point.x, point.y);
	if(pos == -1)
	{
		//SingleLock.Unlock();
		return nHit;
	}

	CString strExChar = "";	// 扩展单词支持的字符
	if(m_LangType == L_TCL)
	{
		strExChar = "-";
	}else
	if(m_LangType == L_PERL)
	{
		strExChar = "$@%";
	}
	CString strWord = m_SciEdit.getPositionWord(pos, strExChar);

	if(strWord == "")
	{
		//SingleLock.Unlock();
		return nHit;
	}

	int nTipType = 0;	// 0=API信息,1=变量信息

	// 首先从索引库的API表中获取是否有此单词的tip信息
	CString strTip;
	CString strDesc;
	CString strLibrary;
	if(m_strLastTooltipWord == strWord)
	{
		// 如果和上一次的Tip单词相同,则不用获取了,直接用上一次的信息就可以
		strTip = m_strLastTooltipTip;
		strDesc = m_strLastTooltipDesc;
		strLibrary = m_strLastTooltipLibrary;
	}else
	{
		if(((COwmSciteView*)this)->GetWordTipInApiTable(strWord, strTip, strDesc, strLibrary))
		{
			if(strTip == "")	// 如果没有tip信息,用desc信息也可以
			{
				strTip = strDesc;
				strDesc = "";
			}
			((COwmSciteView*)this)->m_strLastTooltipWord = strWord;
			((COwmSciteView*)this)->m_strLastTooltipTip = strTip;
			((COwmSciteView*)this)->m_strLastTooltipDesc = strDesc;
			((COwmSciteView*)this)->m_strLastTooltipLibrary = strLibrary;
		}
	}

	// 如果没有tip信息就判断如果处于调试模式，就按照此单词是变量名来获取变量值信息
	if((strTip == "") && m_pInterp)
	{
		BOOL bEnableVarTooltip = AfxGetApp()->GetProfileInt(REG_HELP_SUBKEY, REG_HELP_VAR_TOOLTIP, TRUE);
		if(bEnableVarTooltip)
		{
			int nRunState = ((COwmSciteView*)this)->GetRunState();
			if((SCRIPT_STATE_BREAK == nRunState) || (SCRIPT_STATE_STEP == nRunState))
			{
				if(m_strLastTooltipVarName == strWord)
				{
					// 如果和上一次的Tip单词相同,则不用获取了,直接用上一次的信息就可以
					strTip = m_strLastTooltipVarInfo;
				}else
				{
					strTip = m_pInterp->GetVar(strWord);
					((COwmSciteView*)this)->m_strLastTooltipVarName = strWord;
					((COwmSciteView*)this)->m_strLastTooltipVarInfo = strTip;
				}
				nTipType = 1;
			}
		}
	}

	if(strTip == "")
	{
		//SingleLock.Unlock();
		return nHit;
	}

	// 字符替换
	strTip.Replace("<", "&lt;");
	strTip.Replace(">", "&gt;");
	strTip.Replace("\\r\\n", "<LineBreak/>");
	strTip.Replace("\\n", "<LineBreak/>");
	strTip.Replace("\r\n", "<LineBreak/>");
	strTip.Replace("\n", "<LineBreak/>");

	strDesc.Replace("<", "&lt;");
	strDesc.Replace(">", "&gt;");
	strDesc.Replace("\\r\\n", "<LineBreak/>");
	strDesc.Replace("\\n", "<LineBreak/>");
	strDesc.Replace("\r\n", "<LineBreak/>");
	strDesc.Replace("\n", "<LineBreak/>");

	CString strTipBox;
	if(nTipType == 1)
	{
		// 显示变量信息
		if(strTip.GetLength() > 2000)
		{
			strTip = strTip.Left(2000);
			strTip += "  <Span Foreground='#9e1111'>...more...</Span>";
		}
		strTipBox.Format(
				_T("<StackPanel Margin='1' MaxWidth='400'>")
					_T("<TextBlock Padding='1, 3, 1, 3' FontWeight='Bold'>%s</TextBlock>")
					_T("<TextBlock Padding='9, 6, 30, 7' TextWrapping='Wrap'>%s</TextBlock>")
				_T("</StackPanel>"),
			strWord, strTip);
	}else
	{
		// 显示API信息
		strTipBox.Format(
				_T("<StackPanel Margin='1' MaxWidth='400'>")
					_T("<TextBlock Padding='1, 3, 1, 3' FontWeight='Bold'>%s <Span Foreground='#11119e'>%s</Span></TextBlock>")
					_T("<TextBlock Padding='9, 6, 30, 7' TextWrapping='Wrap'>%s</TextBlock>")
					_T("<Border Height='1' Background='#9ebbdd' />")
					_T("<Border Height='1' Background='White' />")
					_T("<StackPanel Margin='1, 7, 0, 3' Orientation='Horizontal'>")
						_T("<Image Source='3066'/><TextBlock Margin='7, 0, 0, 0' VerticalAlignment='Center' FontWeight='Bold'>Press F1 for more help.</TextBlock>")
					_T("</StackPanel>")
				_T("</StackPanel>"),
				strWord, (strDesc != "") ? "- "+strDesc : "", strTip);
	}

	//CString strTitle, strDescription;
	//get_dll_resource();
	CXTPToolTipContext::FillInToolInfo(pTI, m_hWnd, CRect(0,0,0,0),
			nHit, strTipBox);//, strTitle, strDescription, XTPImageManager());
	//reset_dll_resource();

	nHit = (INT_PTR)m_pTooltipContext;

	//SingleLock.Unlock();

	return nHit;
}
