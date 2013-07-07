// OwmSciteView.h : interface of the COwmSciteView class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __OWMSCITEVIEW_H__
#define __OWMSCITEVIEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Parameters.h"
#include "ScintillaEditView.h"
#include "AutoCompletion.h"
#include "CodeSnippet.h"
#include "CTemplateLoader.h"
// For the codesnip-auto-completion support
#include "CPopupListBox.h"
#include "DbSqlite.h"

const int blockSizeSci = 128 * 1024;

enum comment_mode {cm_comment, cm_uncomment, cm_toggle};

// 定义调试信息
#define _WND_DEBUG	1		// 定义调试窗口

///////////////////////////////////////////////////////////////////////
// Export define

// 带格式导出和拷贝相关定义
#define CF_RTF 			TEXT("Rich Text Format")
#define CF_HTML			TEXT("HTML Format")

#define SCINTILLA_FONTNAME_SIZE	MAX_PATH

//size definitions for memory allocation when using clipboard
#define EXPORT_SIZE_HTML_STATIC		(266)			//including default style params
#define EXPORT_SIZE_HTML_STYLE		(175)			//bold color bgcolor
#define EXPORT_SIZE_HTML_SWITCH		(34)			//<span ...></span>
#define EXPORT_SIZE_HTML_HEADER		(105+22+20)		//CF_HTML data

//RTF is
#define EXPORT_SIZE_RTF_STATIC		(21+11+5+12+5+3)	//header + fonttbl + fonttbl end + colortbl + colortbl end + eof
#define EXPORT_SIZE_RTF_STYLE		(11+27+27)			//font decl + color decl + color decl
#define EXPORT_SIZE_RTF_SWITCH		(33)				// '\f127\fs56\cb254\cf255\b0\i0\ul0 '

struct StyleData {
	char fontString[SCINTILLA_FONTNAME_SIZE];
	int fontIndex;
	int size;
	int bold;
	int italic;
	int underlined;
	int fgColor;
	int bgColor;
	int fgClrIndex;
	int bgClrIndex;
};

struct CurrentScintillaData {
	//HWND hScintilla;
	long nrChars;
	int tabSize;
	bool usedStyles[STYLE_MAX];
	StyleData * styles;
	char * dataBuffer;
	int nrUsedStyles;
	int nrStyleSwitches;
	int totalFontStringLength;
};

struct ExportFileData {
	FILE * file;
	CurrentScintillaData * csd;
};

///////////////////////////////////////////////////////////////////////
// Search define
#define DIR_DOWN true
#define DIR_UP false

//#define FIND_REPLACE_STR_MAX 256
#define FINDREPLACE_MAXLENGTH 2048

enum InWhat{ALL_OPEN_DOCS, FILES_IN_DIR, CURRENT_DOC};

struct FoundInfo {
	FoundInfo(int start, int end, const TCHAR *fullPath)
		: _start(start), _end(end), _fullPath(fullPath) {};
	int _start;
	int _end;
	std::generic_string _fullPath;
};

struct TargetRange {
	int targetStart;
	int targetEnd;
};

enum SearchType { FindNormal, FindExtended, FindRegex };
enum ProcessOperation { ProcessFindAll, ProcessReplaceAll, ProcessCountAll, ProcessMarkAll, ProcessMarkAll_2, ProcessMarkAll_IncSearch };

struct FindOption {
	bool _isWholeWord;
	bool _isMatchCase;
	bool _isWrapAround;
	bool _whichDirection;
	bool _isIncremental;
	bool _isInSelection;
	SearchType _searchType;
	FindOption() :_isWholeWord(true), _isMatchCase(true), _searchType(FindNormal),\
		_isWrapAround(true), _whichDirection(DIR_DOWN), _isIncremental(false), _isInSelection(false){};
};

//This class contains generic search functions as static functions for easy access
class Searching {
public:
	static int convertExtendedToString(const TCHAR * query, TCHAR * result, int length);
	static TargetRange t;
	static int buildSearchFlags(FindOption * option) {
		return	(option->_isWholeWord ? SCFIND_WHOLEWORD : 0) |
				(option->_isMatchCase ? SCFIND_MATCHCASE : 0) |
				(option->_searchType == FindRegex ? SCFIND_REGEXP|SCFIND_POSIX : 0);
	};
	static void displaySectionCentered(int posStart, int posEnd, ScintillaEditView * pEditView, bool isDownwards = true);

private:
	static bool readBase(const TCHAR * str, int * value, int base, int size);

};

/////////////////////////////////////////////////////////////////////////////
// COwmSciteView view
class COwmSciteDoc;
class COwmSciteView : public CView
{
protected: // create from serialization only
	COwmSciteView();
	DECLARE_DYNCREATE(COwmSciteView)

// Attributes
public:
	COwmSciteDoc* GetDocument();

	HACCEL m_hAccel;
	ScintillaEditView m_SciEdit;
	//SmartHighlighter _smartHighlighter;
	AutoCompletion _autoComplete;
	CodeSnippet _codeSnippet;
	LangType m_LangType;
	Position _position;

	//Accelerator _accelerator;
	//ScintillaAccelerator _scintaccelerator;

	BOOL _isFileOpening;	// 文件正在打开过程中
	BOOL _linkTriggered;	// 触发器,用于判断缓冲区是否可用
	BOOL _isDocModifing;	// 文档是否修改

	// Support for auto-completion. This is necessary to keep the caret
	// blinking in the editor window while the autocompletion popup window
	// is displayed. It is used in the UpdateCaret() method of this class only.
	// 扩展的自动完成模式
	BOOL  m_bExtAutoCompletionMode;

    CPopupListBox           m_listPopup;
	CCrystalPopupWnd*       m_pAutoComPopupWnd;
	CPoint                  m_ptAutoComStartPos;
	CTemplateLoader         m_CodeSnipTemplateLoader;

	CDbSQLite				m_sqliteIndex;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COwmSciteView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COwmSciteView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	LangType getLangType() {return m_LangType;}
	LangType getLangFromExt(const TCHAR *ext);
	CString getDocIdFromLangType(LangType langType);
	int  executeThreadSciCommand(UINT Msg, WPARAM wParam=0, LPARAM lParam=0);

	void findMatchingBracePos(int & braceAtCaret, int & braceOpposite);
	void braceMatch();

	void charAdded(TCHAR chAdded);
	void MaintainIndentation(TCHAR ch);

	void linemarkAdd(int marktype, int lineno, bool bSingle = false) const;
	void linemarkDelete(int marktype, int lineno) const;
	bool linemarkPresent(int marktype, int lineno) const;
	void linemarkToggle(int marktype, int lineno) const;
	void linemarkClearAll(int marktype) const;
	int  linemarkGetAll(int marktype, CUIntArray& auMarkLine);

	void bookmarkAdd(int lineno) const;
	void bookmarkDelete(int lineno) const;
	bool bookmarkPresent(int lineno) const; 
	void bookmarkToggle(int lineno) const;
	void bookmarkClearAll() const;
	void bookmarkNext(bool forwardScan);
	void ViewBookmarksWindow();

	void ViewBreaksWindow();

	void SearchLine(CString strLine, int nMarkType);

	int processAll(ProcessOperation op, const TCHAR *txt2find, const TCHAR *txt2replace, bool isEntire = false, const TCHAR *fileName = NULL, FindOption *opt = NULL);
	int processRange(ProcessOperation op, const TCHAR *txt2find, const TCHAR *txt2replace, int startRange, int endRange, const TCHAR *fileName = NULL, FindOption *opt = NULL);
	generic_string getTextFromGrep(int nCmdId) const;

	void OpenFile(const char *fileName);

	void EncodingConvert(UniMode umto);

	void saveCurrentPos();
	void restoreCurrentPos();

	bool doBlockComment(comment_mode currCommentMode);

	void initScintillaData(CurrentScintillaData * csd);
	void fillScintillaData(CurrentScintillaData * csd, int start, int end);
	void exportRTF(ExportFileData * efd);
	void exportHTML(ExportFileData * efd);
	void exportHtmlFile(CString strExportFile = "");
	void exportRtfFile(CString strExportFile = "");

	void SetDocIcon(CString strIconType);
	void EvalScript(int nRunMode);
	void EvalScriptThread(int nRunMode);
	int  GetRunState();
	void SetSlaveDebugMode(int nSlaveMode);
	void DoCompile();
	void DoBuildAndRun(BOOL bRun);

	CString GetCurrentWord();
	void SearchWordHelp(CString strSearchWord = "");
	BOOL GetWordTipInApiTable(CString strWord, CString& strTip, CString& strDesc, CString& strLibrary);

	// Extend Auto-completion support
	void SetExtAutoCompletionMode(BOOL bMode) { m_bExtAutoCompletionMode = bMode; }
	BOOL GetExtAutoCompletionMode(void) const { return m_bExtAutoCompletionMode; }

	void DoAutoCompletePopupWnd(INSERTTYPE xType);
	BOOL InsertTemplateText(int nIndex);
	BOOL LoadCodeSnippetTemplates();

	IInterp* m_pInterp;	// 保存解释器指针
	BOOL m_bSlaveDebug;	// 是否处于从调试模式

	CXTPToolTipContext* m_pTooltipContext;	// Tooltip对象
	CString	m_strLastTooltipWord;			// 上一次Tooltip单词
	CString	m_strLastTooltipTip;			// 上一次单词Tooltip信息
	CString	m_strLastTooltipDesc;			// 上一次单词Tooltip描述信息
	CString	m_strLastTooltipLibrary;		// 上一次单词Tooltip库信息
	CString	m_strLastTooltipVarName;		// 上一次变量Tooltip名
	CString	m_strLastTooltipVarInfo;		// 上一次变量Tooltip信息

// Generated message map functions
protected:
	//{{AFX_MSG(COwmSciteView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditSelectAll();
	afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
	afx_msg void OnCommentSelect();
	afx_msg void OnUpdateCommentSelect(CCmdUI* pCmdUI);
	afx_msg void OnUncommentSelect();
	afx_msg void OnUpdateUncommentSelect(CCmdUI* pCmdUI);
	afx_msg void OnCopyAsRtf();
	afx_msg void OnUpdateCopyAsRtf(CCmdUI* pCmdUI);
	afx_msg void OnEditFind();
	afx_msg void OnUpdateEditFind(CCmdUI* pCmdUI);
	afx_msg void OnEditRepeat();
	afx_msg void OnUpdateEditRepeat(CCmdUI* pCmdUI);
	afx_msg void OnEditReplace();
	afx_msg void OnUpdateEditReplace(CCmdUI* pCmdUI);
	afx_msg void OnEditBookmarks();
	afx_msg void OnUpdateEditBookmarks(CCmdUI* pCmdUI);
	afx_msg void OnEditClearbookmarks();
	afx_msg void OnUpdateEditClearbookmarks(CCmdUI* pCmdUI);
	afx_msg void OnCompile();
	afx_msg void OnUpdateCompile(CCmdUI* pCmdUI);
	afx_msg void OnBuild();
	afx_msg void OnUpdateBuild(CCmdUI* pCmdUI);
	afx_msg void OnBuildAndRun();
	afx_msg void OnUpdateBuildAndRun(CCmdUI* pCmdUI);
	afx_msg void OnGo();
	afx_msg void OnUpdateGo(CCmdUI* pCmdUI);
	afx_msg void OnOwmRun();
	afx_msg void OnUpdateOwmRun(CCmdUI* pCmdUI);
	afx_msg void OnScriptEval();
	afx_msg void OnUpdateScriptEval(CCmdUI* pCmdUI);
	afx_msg void OnOwmExecute();
	afx_msg void OnUpdateOwmExecute(CCmdUI* pCmdUI);
	afx_msg void OnStopRun();
	afx_msg void OnUpdateStopRun(CCmdUI* pCmdUI);
	afx_msg void OnStepInto();
	afx_msg void OnUpdateStepInto(CCmdUI* pCmdUI);
	afx_msg void OnStepOver();
	afx_msg void OnUpdateStepOver(CCmdUI* pCmdUI);
	afx_msg void OnStepOut();
	afx_msg void OnUpdateStepOut(CCmdUI* pCmdUI);
	afx_msg void OnRunTocursor();
	afx_msg void OnUpdateRunTocursor(CCmdUI* pCmdUI);
	afx_msg void OnHideLines();
	afx_msg void OnUpdateHideLines(CCmdUI* pCmdUI);
	afx_msg void OnInsertBreak();
	afx_msg void OnUpdateInsertBreak(CCmdUI* pCmdUI);
	afx_msg void OnClearBreakall();
	afx_msg void OnUpdateClearBreakall(CCmdUI* pCmdUI);
	afx_msg void OnConditionBreak();
	afx_msg void OnUpdateConditionBreak(CCmdUI* pCmdUI);
	afx_msg void OnWatchBreaks();
	afx_msg void OnUpdateWatchBreaks(CCmdUI* pCmdUI);
	afx_msg void OnWatchVars();
	afx_msg void OnUpdateWatchVars(CCmdUI* pCmdUI);
	afx_msg void OnWatchObject();
	afx_msg void OnUpdateWatchObject(CCmdUI* pCmdUI);
	afx_msg void OnWatchProc();
	afx_msg void OnUpdateWatchProc(CCmdUI* pCmdUI);
	afx_msg void OnWatchCallstack();
	afx_msg void OnUpdateWatchCallstack(CCmdUI* pCmdUI);
	afx_msg void OnWordHelp();
	afx_msg void OnUpdateWordHelp(CCmdUI* pCmdUI);
	afx_msg void OnDocProperty();
	afx_msg void OnUpdateDocProperty(CCmdUI* pCmdUI);
	afx_msg void OnOwmAbout();
	afx_msg void OnUpdateOwmAbout(CCmdUI* pCmdUI);
	afx_msg void OnZoomIn();
	afx_msg void OnUpdateZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnZoomOut();
	afx_msg void OnUpdateZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnZoomRestore();
	afx_msg void OnUpdateZoomRestore(CCmdUI* pCmdUI);
	afx_msg void OnEditAutocomplete();
	afx_msg void OnUpdateEditAutocomplete(CCmdUI* pCmdUI);
	afx_msg void OnEditCompleteKeyword();
	afx_msg void OnUpdateEditCompleteKeyword(CCmdUI* pCmdUI);
	afx_msg void OnEditFunccalltip();
	afx_msg void OnUpdateEditFunccalltip(CCmdUI* pCmdUI);
	afx_msg void OnEditBookmarkNext();
	afx_msg void OnUpdateEditBookmarkNext(CCmdUI* pCmdUI);
	afx_msg void OnEditBookmarkPrev();
	afx_msg void OnUpdateEditBookmarkPrev(CCmdUI* pCmdUI);
	afx_msg void OnEditUppercase();
	afx_msg void OnUpdateEditUppercase(CCmdUI* pCmdUI);
	afx_msg void OnEditLowercase();
	afx_msg void OnUpdateEditLowercase(CCmdUI* pCmdUI);
	afx_msg void OnFormatConv2Ansi();
	afx_msg void OnUpdateFormatConv2Ansi(CCmdUI* pCmdUI);
	afx_msg void OnFormatConv2AsUtf8();
	afx_msg void OnUpdateFormatConv2AsUtf8(CCmdUI* pCmdUI);
	afx_msg void OnFormatConv2Ucs2be();
	afx_msg void OnUpdateFormatConv2Ucs2be(CCmdUI* pCmdUI);
	afx_msg void OnFormatConv2Ucs2le();
	afx_msg void OnUpdateFormatConv2Ucs2le(CCmdUI* pCmdUI);
	afx_msg void OnFormatConv2Utf8();
	afx_msg void OnUpdateFormatConv2Utf8(CCmdUI* pCmdUI);
	afx_msg void OnFormatTowin();
	afx_msg void OnUpdateFormatTowin(CCmdUI* pCmdUI);
	afx_msg void OnFormatTomac();
	afx_msg void OnUpdateFormatTomac(CCmdUI* pCmdUI);
	afx_msg void OnFormatTounix();
	afx_msg void OnUpdateFormatTounix(CCmdUI* pCmdUI);
	afx_msg void OnWrap();
	afx_msg void OnUpdateWrap(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnWatchBookmarks();
	afx_msg void OnUpdateWatchBookmarks(CCmdUI* pCmdUI);
	afx_msg void OnEditCodesnippet();
	afx_msg void OnUpdateEditCodesnippet(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFilePrint();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnUpdateIndicatorReadOnly(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIndicatorCRLF(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIndicatorUniMode(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIndicatorPosition(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

	INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
};

#ifndef _DEBUG  // debug version in OwmSciteView.cpp
inline COwmSciteDoc* COwmSciteView::GetDocument()
   { return (COwmSciteDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__OWMSCITEVIEW_H__)
