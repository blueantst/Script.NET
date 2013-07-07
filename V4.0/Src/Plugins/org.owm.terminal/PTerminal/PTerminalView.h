#ifndef __PTERMINALVIEW_H__
#define __PTERMINALVIEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTerminalView.h : header file
//

#include "telnet.h"
#include "SerialPort.h"
#include "IRemoteClient.h"

// For the auto-completion support
#include "CPopupListBox.h"

// 自动补齐的内容
enum{
	AUTOCOMPLETE_COMMAND,		// TCL命令
	AUTOCOMPLETE_PROCEDURE,		// 过程
	AUTOCOMPLETE_VAR,			// 变量
	AUTOCOMPLETE_CLASS,			// 类
	AUTOCOMPLETE_OBJECT,		// 对象
};

struct CCharFormat : public CHARFORMAT
{
	CCharFormat() {cbSize = sizeof(CHARFORMAT);}
	BOOL operator==(CCharFormat& cf);
};

/////////////////////////////////////////////////////////////////////////////
// CPTerminalView view
class CPTerminalDoc;
class CClientSocket;
class CPTerminalView : public CRichEditView
{
	DECLARE_DYNCREATE(CPTerminalView)

// Attributes
public:
	CPTerminalView();
	virtual ~CPTerminalView();

	CPTerminalDoc* GetDocument();

// Attributes
public:
	HACCEL		m_hAccel;

	CCharFormat m_defCharFormat;
	CCharFormat m_defTextCharFormat;

	// Support for auto-completion. This is necessary to keep the caret
	// blinking in the editor window while the autocompletion popup window
	// is displayed. It is used in the UpdateCaret() method of this class only.
	BOOL  m_bAutoCompletionMode;

	CString		m_strLastMsg;	// 记录上次未显示的字符串,用于和本次的字符串组合,形成一个多字节字符串

	CMenu* m_pMenu;

	BOOL		m_bHaveDocument;	// 是否有Document

	int			m_nCurConnType;		// 当前连接类型
	BOOL		m_bConnectRemote;	// 是否已经连接到远程主机
	CByteArray	m_bBuf;				// 缓冲区
	CString		m_strPrompt;		// 提示符
	CString		m_strExtPrompt;		// 扩展提示符
	int			m_nWorkShell;		// 工作外壳
	CString		m_strInterpName;	// 解释器名
	IInterp*	m_pInterp;			// 保存解释器指针
	CStringArray m_asCmdHistory;	// 命令历史
	int			m_nCurHistory;		// 当前索引的历史记录
	int			m_nCurCodeLevel;	// 当前代码层数
	int			m_nCurCommaLevel;	// 当前引号层数
	int			m_nCurEscape;		// 当前转义状态
	int			m_nContinueLine;	// 续行标志
	CString		m_strNormalText;
	CString		m_strLine;
	CStringList	m_ListOptions;
	CString		m_strResp;			// 调用解释器的返回结果
	int			TempCounter;
	CClientSocket* m_sClient;
	BOOL		bOptionsSent;

	// Telnet相关对象
	CSocketDx*	m_pSocketDx;		// Telnet侦听对象
	CSocketRx*	m_pSocketRx;		// Telnet接收对象
	CSocketTx*	m_pSocketTx;		// Telnet发送对象
	SOCKET		m_hTelnetSocket;	// Telent使用的Socket
	HANDLE		m_hTelnetThread[2];	// Telnet发送和接收线程句柄

	BOOL		m_bEnableAnsiColor;	// 是否支持ANSI颜色定义
	COLORREF	m_clrRecv;			// Telnet接收的字符颜色

	// 串口相关对象
	CSerialPort*	m_pSerialPort;	// 串口设备
	BOOL		m_bSerialEcho;		// 串口是否回显

//{{远程控制台
//	BOOL        m_bLogin;          //登录标识
	IRemoteClient* m_pIRemoteClient;//远程控制接口指针
//	int         m_nInterpreterID;
//远程控制台}}

// Operations
public:
	void SetDefaultFont(BOOL bText);
	void GetDefaultFont(CCharFormat& cf, UINT nFontNameID);
	void SetTermFontColor(COLORREF color);
	void SetTermTextBackColor(COLORREF colorText, COLORREF colorBack);
	void CheckLastPromptPos();
	void Message(LPCTSTR lpszMessage);
	void ColorMessage(CString strMsg, COLORREF color = SHELL_COLOR_PROMPT);

	BOOL SendInterpListCommand(CString strCmd, CString strSeparator, CStringArray& asRes);
	BOOL SendInterpCommand(CString strCmd, CString& strRes);
	int  FindStringInArray(CStringArray* pas, CString strFind);

	// Auto-completion support
	void SetAutoCompletionMode(BOOL bMode) { m_bAutoCompletionMode = bMode; }
	BOOL GetAutoCompletionMode(void) const { return m_bAutoCompletionMode; }

	// Auto-completion support, useful functions for other operations too
	void GetCurrentWord(CString& strWord, BOOL bSelectIt = FALSE);
	BOOL SetCurrentWord(const CString& strWord, BOOL bSelectIt = FALSE);
	BOOL InsertCurrentWord(const CString& strWord, BOOL bSelectIt = FALSE);

	void DoAutoCompletePopupWnd(INSERTTYPE xType);
	void DoEditCompleteKeyword(int nListType, CStringArray* paKeyword = NULL);
	void FillKeywordList(int nListType);
	void FillKeywordListCommand();
	void FillKeywordListProcedure();
	void FillKeywordListVar();
	void FillKeywordListClass();
	void FillKeywordListObject();

	void ShowTipWindow(CString strTipTitle, CString strTipText);

	LRESULT OnSerialRead(WPARAM, LPARAM);

	void ProcessReturn(CString strCmd);
	void ProcessChar(CString strChar);
	void ProcessDot(CString strWord);
	void ProcessLeftBracket(CString strWord);

	void ProcessMessage(CClientSocket *cSocket);
	BOOL GetLine(const CByteArray &bytes, int nBytes, int &ndx);
	void ProcessOptions();
	void RespondToOptions();
	void ArrangeReply(CString strOption);
	void DispatchMessage(CString strText);
	BOOL ConnectToHost(CString m_strHost, UINT m_nPort);
	void EraseScreen(int nPosX, int nPosY, int nSizeX, int nSizeY);
	void EraseLine(int nPosX, int nPosY, int nLen);
	void SetTextAttribute(BYTE bySa);

	BOOL InitSerialPort(UINT nPort, UINT nBaud, char cParity, UINT nDataBits, UINT nStopBits);

	BOOL InitTerminal();
	void DoneTerminal();
	void SetWorkShell(int nShell, BOOL bSwitch = FALSE);
	void SetPrompt(CString strPrompt) { m_strPrompt = strPrompt; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTerminalView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	virtual HRESULT QueryAcceptData( LPDATAOBJECT lpdataobj,
									 CLIPFORMAT* lpcfFormat,
									 DWORD dwReco,
									 BOOL bReally,
									 HGLOBAL hMetaFile
								   );
protected:
	int m_nLastPromptPos;

	// For the auto-completion support
    CPopupListBox           m_listPopup;
	CCrystalPopupWnd*       m_pAutoComPopupWnd;
	int						m_nAutoComStartPos;
	BOOL					m_bAutoComplete;
	CXTTipWindow			m_tipWindow;

	// Generated message map functions
protected:
	//{{AFX_MSG(CPTerminalView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnOwmAbout();
	afx_msg void OnUpdateOwmAbout(CCmdUI* pCmdUI);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTermOpenfile();
	afx_msg void OnUpdateTermOpenfile(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditClear();
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnListCommand();
	afx_msg void OnUpdateListCommand(CCmdUI* pCmdUI);
	afx_msg void OnListHistorycmd();
	afx_msg void OnUpdateListHistorycmd(CCmdUI* pCmdUI);
	afx_msg void OnListLoadlib();
	afx_msg void OnUpdateListLoadlib(CCmdUI* pCmdUI);
	afx_msg void OnListKnownlib();
	afx_msg void OnUpdateListKnownlib(CCmdUI* pCmdUI);
	afx_msg void OnListProcs();
	afx_msg void OnUpdateListProcs(CCmdUI* pCmdUI);
	afx_msg void OnListGlobals();
	afx_msg void OnUpdateListGlobals(CCmdUI* pCmdUI);
	afx_msg void OnListLocalvars();
	afx_msg void OnUpdateListLocalvars(CCmdUI* pCmdUI);
	afx_msg void OnListItclobject();
	afx_msg void OnUpdateListItclobject(CCmdUI* pCmdUI);
	afx_msg void OnEditClearAll();
	afx_msg void OnUpdateEditClearAll(CCmdUI* pCmdUI);
	afx_msg void OnServerConnect();
	afx_msg void OnUpdateServerConnect(CCmdUI* pCmdUI);
	afx_msg void OnServerDisconnect();
	afx_msg void OnUpdateServerDisconnect(CCmdUI* pCmdUI);
	afx_msg void OnSwitchTerminal();
	afx_msg void OnUpdateSwitchTerminal(CCmdUI* pCmdUI);
	afx_msg void OnResetInterp();
	afx_msg void OnUpdateResetInterp(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PTerminalView.cpp
inline CPTerminalDoc* CPTerminalView::GetDocument()
   { return (CPTerminalDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__PTERMINALVIEW_H__)
