// PTerminalView.cpp : implementation file
//

#include "stdafx.h"
#include "PTerminal.h"
#include "PTerminalView.h"
#include "PTerminalDoc.h"
#include "OptTree.h"
#include "ClientSocket.h"
#include "CommLogin.h"
#include "DlgComParam.h"
#include "ChildFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LOADRCSTRING(str,strID) \
	get_dll_resource();\
	str.LoadString(strID);\
	reset_dll_resource();

void get_dll_resource(void);
void reset_dll_resource(void);

BOOL CCharFormat::operator==(CCharFormat& cf)
{
	return
	dwMask == cf.dwMask
	&& dwEffects == cf.dwEffects
	&& yHeight == cf.yHeight
	&& yOffset == cf.yOffset
	&& crTextColor == cf.crTextColor
	&& bPitchAndFamily == cf.bPitchAndFamily
	&& (lstrcmpA(szFaceName, cf.szFaceName) == 0);
}

/////////////////////////////////////////////////////////////////////////////
// CPTerminalView

IMPLEMENT_DYNCREATE(CPTerminalView, CRichEditView)

CPTerminalView::CPTerminalView()
{
	m_hAccel			= NULL;

	// The auto-completion support
	m_bAutoCompletionMode = FALSE;
	m_nAutoComStartPos   = 0;
	m_pAutoComPopupWnd    = NULL;
	m_bAutoComplete       = TRUE;
	m_strLastMsg		= "";

	m_bHaveDocument		= FALSE;

	m_pMenu = NULL;
	m_bConnectRemote	= FALSE;
	m_bBuf.SetSize(1024);
	bOptionsSent		= FALSE;
	TempCounter			= 0;
	m_sClient			= NULL;
	m_nCurHistory		= 0;
	m_nCurCodeLevel		= 0;
	m_nCurCommaLevel	= 0;
	m_nCurEscape		= 0;
	m_nContinueLine		= 0;
	m_pInterp			= NULL;

	m_pSocketDx			= NULL;
	m_pSocketRx			= NULL;
	m_pSocketTx			= NULL;
	m_hTelnetSocket		= NULL;
	m_hTelnetThread[0]	= NULL;
	m_hTelnetThread[1]	= NULL;
	m_bEnableAnsiColor	= FALSE;

	m_pSerialPort		= NULL;

//	m_bLogin            = FALSE;
	m_pIRemoteClient    = NULL;
//	m_nInterpreterID    = 0;

	int nDefaultShell = AfxGetApp()->GetProfileInt(REG_PTERMINAL, REG_DEFAULTSHELL, SHELL_TCL);
	SetWorkShell(nDefaultShell);
}

CPTerminalView::~CPTerminalView()
{
	DoneTerminal();
}


BEGIN_MESSAGE_MAP(CPTerminalView, CRichEditView)
	//{{AFX_MSG_MAP(CPTerminalView)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_COMMAND(ID_OWM_ABOUT, OnOwmAbout)
	ON_UPDATE_COMMAND_UI(ID_OWM_ABOUT, OnUpdateOwmAbout)
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_TERM_OPENFILE, OnTermOpenfile)
	ON_UPDATE_COMMAND_UI(ID_TERM_OPENFILE, OnUpdateTermOpenfile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_COMMAND(ID_LIST_COMMAND, OnListCommand)
	ON_UPDATE_COMMAND_UI(ID_LIST_COMMAND, OnUpdateListCommand)
	ON_COMMAND(ID_LIST_HISTORYCMD, OnListHistorycmd)
	ON_UPDATE_COMMAND_UI(ID_LIST_HISTORYCMD, OnUpdateListHistorycmd)
	ON_COMMAND(ID_LIST_LOADLIB, OnListLoadlib)
	ON_UPDATE_COMMAND_UI(ID_LIST_LOADLIB, OnUpdateListLoadlib)
	ON_COMMAND(ID_LIST_KNOWNLIB, OnListKnownlib)
	ON_UPDATE_COMMAND_UI(ID_LIST_KNOWNLIB, OnUpdateListKnownlib)
	ON_COMMAND(ID_LIST_PROCS, OnListProcs)
	ON_UPDATE_COMMAND_UI(ID_LIST_PROCS, OnUpdateListProcs)
	ON_COMMAND(ID_LIST_GLOBALS, OnListGlobals)
	ON_UPDATE_COMMAND_UI(ID_LIST_GLOBALS, OnUpdateListGlobals)
	ON_COMMAND(ID_LIST_LOCALVARS, OnListLocalvars)
	ON_UPDATE_COMMAND_UI(ID_LIST_LOCALVARS, OnUpdateListLocalvars)
	ON_COMMAND(ID_LIST_ITCLOBJECT, OnListItclobject)
	ON_UPDATE_COMMAND_UI(ID_LIST_ITCLOBJECT, OnUpdateListItclobject)
	ON_COMMAND(ID_EDIT_CLEAR_ALL, OnEditClearAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_ALL, OnUpdateEditClearAll)
	ON_COMMAND(ID_SERVER_CONNECT, OnServerConnect)
	ON_UPDATE_COMMAND_UI(ID_SERVER_CONNECT, OnUpdateServerConnect)
	ON_COMMAND(ID_SERVER_DISCONNECT, OnServerDisconnect)
	ON_UPDATE_COMMAND_UI(ID_SERVER_DISCONNECT, OnUpdateServerDisconnect)
	ON_COMMAND(ID_SWITCH_TERMINAL, OnSwitchTerminal)
	ON_UPDATE_COMMAND_UI(ID_SWITCH_TERMINAL, OnUpdateSwitchTerminal)
	ON_COMMAND(ID_RESET_INTERP, OnResetInterp)
	ON_UPDATE_COMMAND_UI(ID_RESET_INTERP, OnUpdateResetInterp)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_COMM_RXCHAR, OnSerialRead)
//	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTerminalView drawing

void CPTerminalView::OnDraw(CDC* pDC)
{
	CPTerminalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CPTerminalView diagnostics

#ifdef _DEBUG
void CPTerminalView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CPTerminalView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CPTerminalDoc* CPTerminalView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPTerminalDoc)));
	return (CPTerminalDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTerminalView message handlers
void CPTerminalView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// 弹出菜单
	get_dll_resource();
	CMenu* pMenu = new CMenu();
	switch(m_nWorkShell)
	{
	case SHELL_TCL:
		pMenu->LoadMenu(IDR_TERM_TCL);
		break;
	case SHELL_PYTHON:
		pMenu->LoadMenu(IDR_TERM_TCL);
		break;
	case SHELL_RUBY:
		pMenu->LoadMenu(IDR_TERM_TCL);
		break;
	case SHELL_LUA:
		pMenu->LoadMenu(IDR_TERM_TCL);
		break;
	case SHELL_PERL:
		pMenu->LoadMenu(IDR_TERM_TCL);
		break;
	default:
		pMenu->LoadMenu(IDR_CONTEXT);
	}
	reset_dll_resource();
	CMenu* pmenu = (CMenu*)pMenu->GetSubMenu(0);
	ClientToScreen(&point);

	BOOL bEnable = (GetTextLength() > 0);
	pmenu->EnableMenuItem(ID_EDIT_CUT, bEnable ? MF_ENABLED : MF_GRAYED);
	pmenu->EnableMenuItem(ID_EDIT_COPY, bEnable ? MF_ENABLED : MF_GRAYED);

	//CFrameWnd* pFrame = GetTopLevelFrame();

	//pmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
	//		point.x, point.y, pFrame);
	CXTPCommandBars::TrackPopupMenu(pmenu, 0, point.x, point.y, this);
	delete pMenu;
}

BOOL CPTerminalView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CRichEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}

int CPTerminalView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditView::OnCreate(lpCreateStruct) == -1)
		return -1;

	GetDefaultFont(m_defTextCharFormat, IDS_DEFAULTTEXTFONT);
	GetDefaultFont(m_defCharFormat, IDS_DEFAULTFONT);

	SetDefaultFont(TRUE);
	SetDefaultFont(FALSE);

	// 禁止拖拽文件到控制台的RichEdit控件,否则会异常
	DragAcceptFiles(FALSE);

/*
	CClientDC dc (this);
    int nHeight = -((dc.GetDeviceCaps (LOGPIXELSY) * 10) / 72);

	//	Initialize LOGFONT structure
	LOGFONT m_lf;
	memset(&m_lf, 0, sizeof(m_lf));
	m_lf.lfWeight = FW_NORMAL;
	m_lf.lfCharSet = GB2312_CHARSET;//ANSI_CHARSET;
	m_lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_lf.lfQuality = DEFAULT_QUALITY;
	m_lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy(m_lf.lfFaceName, _T("FixedSys"));

	CFont font;
	font.CreateFontIndirect(&m_lf);

    SetFont(&font, FALSE);

	CHARFORMAT fmt;
	GetRichEditCtrl().GetDefaultCharFormat(fmt);	// 获取缺省字体信息
	SetCharFormat(fmt);
*/

	// 加载加速键资源
	get_dll_resource();
	m_hAccel = ::LoadAccelerators(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR));
	reset_dll_resource();

	// 4. Create the auto-complete list box control	
	// Note the absence of the visibility WS_VISIBLE flag
	m_listPopup.Create(LBS_NOTIFY | WS_CHILD | LBS_STANDARD | WS_VSCROLL
		| LBS_OWNERDRAWFIXED | LBS_HASSTRINGS,
			CRect(0, 0, 150, 200), this, IDC_EDIT_LIST);

	m_tipWindow.SetMargins(CSize(20,25));
	m_tipWindow.SetLineSpace(15);

	return 0;
}

BOOL CPTerminalView::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		if (m_hAccel != NULL)
		{
			if (::TranslateAccelerator(m_hWnd, m_hAccel, pMsg))
				return TRUE;
		}
	}

	return CRichEditView::PreTranslateMessage(pMsg);
}

void CPTerminalView::OnInitialUpdate() 
{
	CRichEditView::OnInitialUpdate();
	
	//注:Docking窗口状态下,此函数会被调用两次,因此必须用SetWorkShell,避免调用两次造成重复创建解释器
	SetWorkShell(m_nWorkShell, TRUE);
	if(m_pDocument == NULL)
	{
		theApp.m_pTerminalView = this;
	}
}

/////////////////////////////////////////////////////////////////////////////
// 设置缺省字体
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::SetDefaultFont(BOOL bText)
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);
	m_bSyncCharFormat = m_bSyncParaFormat = TRUE;
	CHARFORMAT2* pCharFormat = bText ? (CHARFORMAT2*)(&m_defTextCharFormat) : (CHARFORMAT2*)(&m_defCharFormat);
	// set the default character format -- the FALSE makes it the default
	GetRichEditCtrl().SetSel(0,-1);
	GetRichEditCtrl().SetDefaultCharFormat(*pCharFormat);
	GetRichEditCtrl().SetSelectionCharFormat(*pCharFormat);
	SetCharFormat(*pCharFormat);

	//GetRichEditCtrl().SetParaFormat(m_defParaFormat);

	GetRichEditCtrl().SetSel(0,0);
	GetRichEditCtrl().EmptyUndoBuffer();
	GetRichEditCtrl().SetModify(FALSE);
	ASSERT_VALID(this);
}

/////////////////////////////////////////////////////////////////////////////
// 获取缺省字体
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::GetDefaultFont(CCharFormat& cf, UINT nFontNameID)
{	
	USES_CONVERSION;
	CString strDefFont;
	get_dll_resource();
	VERIFY(strDefFont.LoadString(nFontNameID));
	reset_dll_resource();
	ASSERT(cf.cbSize == sizeof(CHARFORMAT));
	cf.dwMask = CFM_BOLD|CFM_ITALIC|CFM_UNDERLINE|CFM_STRIKEOUT|CFM_SIZE|
		CFM_FACE|CFM_COLOR|CFM_OFFSET|CFM_CHARSET;//|CFM_PROTECTED;
	cf.dwEffects = 0;//CFE_AUTOCOLOR;
	cf.yHeight = 200; //10pt
	cf.yOffset =0;
	cf.crTextColor = RGB(0, 0, 0);
	cf.bCharSet = GB2312_CHARSET;
	cf.bPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	ASSERT(strDefFont.GetLength() < LF_FACESIZE);
	lstrcpynA(cf.szFaceName, T2A((LPTSTR) (LPCTSTR) strDefFont), LF_FACESIZE);
}

/////////////////////////////////////////////////////////////////////////////
// 设置终端文字颜色
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::SetTermFontColor(COLORREF color)
{
	CHARFORMAT2 fmt;
	memcpy(&fmt, (CHARFORMAT2*)(&m_defTextCharFormat), sizeof(CHARFORMAT2));
	fmt.dwMask |= CFM_COLOR;
	//fmt.dwEffects = 0;
	fmt.crTextColor = color;
	//GetRichEditCtrl().SetDefaultCharFormat(fmt);
	//GetRichEditCtrl().SetSelectionCharFormat(fmt);
	SetCharFormat(fmt);
}

/////////////////////////////////////////////////////////////////////////////
// 设置终端前景和背景颜色
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::SetTermTextBackColor(COLORREF colorText, COLORREF colorBack)
{
	CHARFORMAT2 fmt;
	memcpy(&fmt, &m_defTextCharFormat, sizeof(CHARFORMAT));
	fmt.dwMask |= (CFM_COLOR|CFM_BACKCOLOR);
	//fmt.dwEffects = 0;
	fmt.crTextColor = colorText;
	fmt.crBackColor = colorBack;
	//GetRichEditCtrl().SetDefaultCharFormat(fmt);
	//GetRichEditCtrl().SetSelectionCharFormat(fmt);
	SetCharFormat(fmt);
}

/////////////////////////////////////////////////////////////////////////////
// 检查最后一个提示符的位置
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::CheckLastPromptPos()
{
	// 缺省提示符位置
	m_nLastPromptPos = 0;
	// 从最后一行开始找
	int nLines = GetRichEditCtrl().GetLineCount();

	// 限制控制台的最大行数为2000
	if(nLines > 2000)
	{	// 如果超过2000行,就删除前面一千行
		int nLineLim = GetRichEditCtrl().LineIndex(1000);
		GetRichEditCtrl().SetSel(0, nLineLim);
		GetRichEditCtrl().ReplaceSel("");
		GetRichEditCtrl().SetSel(-1, -1);
		nLines = GetRichEditCtrl().GetLineCount();
	}

	// 检查提示符位置
	char buf[512];
	int nCurLine = (nLines-1);
	while(nCurLine >= 0)
	{
		memset(buf, 0, 512);
		buf[1] = 2;	// 缓冲区前两个字节表示缓冲区大小
		if(GetRichEditCtrl().GetLine(nCurLine, buf) > 0)
		{
			CString strLine = buf;
			if(strLine.Find(m_strPrompt) == 0)
			{
				m_nLastPromptPos = GetRichEditCtrl().LineIndex(nCurLine);
				m_nLastPromptPos += m_strPrompt.GetLength();
				return;
			}					
		}
		nCurLine--;
	}
}

void CPTerminalView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_RETURN)	// 处理回车
	{
		if( (SHELL_TCL == m_nWorkShell) || (SHELL_LUA == m_nWorkShell)
			|| (SHELL_RUBY == m_nWorkShell) || (SHELL_PERL == m_nWorkShell) )
		{	// 需要考虑多行的情况，需要对提示符所在行进行判断
			ProcessChar((char)nChar);
			if( (m_nCurCodeLevel > 0) ||
				(m_nCurCommaLevel > 0) ||
				(m_nCurEscape > 0) )
			{	// 多行的命令
				SetTermFontColor(SHELL_COLOR_INPUT);
				CRichEditView::OnChar(nChar, nRepCnt, nFlags);
				m_nCurEscape = 0;
				return;
			}

			// 获取输入字符个数
			int nInputLen = GetRichEditCtrl().GetTextLength() - m_nLastPromptPos;
			if(nInputLen > 1024)	// 控制台限制执行的命令长度最多是1024
			{
				SetTermFontColor(SHELL_COLOR_RESFAIL);
				Message("\r\ntoo many chars in command, this terminal can't exec more than 1024 chars command!");
				Message("\r\nif you want to exec more chars command, please use TCL file!");
				ProcessReturn("");
				return;
			}else
			if(nInputLen >= 0)	// 获取输入字符,送给Doc处理
			{				
				GetRichEditCtrl().SetSel(m_nLastPromptPos, -1);
				SetTermFontColor(SHELL_COLOR_INPUT);
				CString strSel = GetRichEditCtrl().GetSelText();
				GetRichEditCtrl().SetSel(-1, -1);
				ProcessReturn(strSel);
				return;
			}

			// 未找到提示符,则输出空命令
			ProcessReturn("");
		}else
		if(SHELL_PYTHON == m_nWorkShell)
		{	// 需要考虑多行的情况，需要对提示符所在行进行判断
			ProcessChar((char)nChar);
			if(m_nContinueLine > 0)
			{	// 续行符的处理,多行的命令
				SetTermFontColor(SHELL_COLOR_PROMPT);				
				Message(SHELL_EXTPROMPTLINE_PYTHON);
				SetTermFontColor(SHELL_COLOR_INPUT);
				m_nCurEscape = 0;
				m_nCurCodeLevel++;	// 代码层数加1
				for(int i=0; i<m_nCurCodeLevel; i++)
				{
					Message("    ");
				}
				return;
			}

			// 获取输入字符个数
			int nInputLen = GetRichEditCtrl().GetTextLength() - m_nLastPromptPos;
			if(nInputLen > 1024)	// 控制台限制执行的命令长度最多是1024
			{
				SetTermFontColor(SHELL_COLOR_RESFAIL);
				Message("\r\ntoo many chars in command, this terminal can't exec more than 1024 chars command!");
				Message("\r\nif you want to exec more chars command, please use Python file!");
				ProcessReturn("");
				return;
			}else
			if(nInputLen >= 0)	// 获取输入字符,送给Doc处理
			{				
				GetRichEditCtrl().SetSel(m_nLastPromptPos, -1);
				SetTermFontColor(SHELL_COLOR_INPUT);
				CString strSel = GetRichEditCtrl().GetSelText();
				GetRichEditCtrl().SetSel(-1, -1);
				// 判断代码层数,如果为0表示要执行
				int nPos = strSel.ReverseFind('\r');
				if((nPos != -1) && (strSel.Find(SHELL_EXTPROMPTLINE_PYTHON, nPos) == nPos))
				{
					CString strLastLine = strSel.Right(strSel.GetLength()-nPos-strlen(SHELL_EXTPROMPTLINE_PYTHON));
					int nWhiteCount = 0;
					while(strLastLine.Find(' ') == 0)
					{
						strLastLine.Delete(0, 1);
						nWhiteCount++;
					}
					m_nCurCodeLevel = nWhiteCount/4;
					if(m_nCurCodeLevel > 0)
					{
						SetTermFontColor(SHELL_COLOR_PROMPT);
						Message(SHELL_EXTPROMPTLINE_PYTHON);
						SetTermFontColor(SHELL_COLOR_INPUT);
						for(int i=0; i<m_nCurCodeLevel; i++)
						{
							Message("    ");
						}
						return;
					}
				}
				ProcessReturn(strSel);
				return;
			}

			// 未找到提示符,则输出空命令
			ProcessReturn("");
		}else
		if(SHELL_TELNET == m_nWorkShell)
		{
			ProcessReturn("");
		}else
		if(SHELL_COM == m_nWorkShell)
		{
			ProcessReturn("");
		}

		return;
	}else
	if (nChar == 0x0A)
	{
		if(m_bAutoCompletionMode)	// 处理关键字补齐模式
			return;
	}else
	{
		if( (SHELL_TCL == m_nWorkShell) || (SHELL_PERL == m_nWorkShell) )
		{	// 限定输出的字符都送到最后一条命令
			long nStartChar, nEndChar;
			GetRichEditCtrl().GetSel(nStartChar, nEndChar);
			if(nStartChar < m_nLastPromptPos)
			{
				GetRichEditCtrl().SetSel(-1, -1);
			}
			ProcessChar((char)nChar);	// 字符转义处理
			SetTermFontColor(SHELL_COLOR_INPUT);			
			CRichEditView::OnChar(nChar, nRepCnt, nFlags);
		}else
		if(SHELL_PYTHON == m_nWorkShell)
		{	// 限定输出的字符都送到最后一条命令
			long nStartChar, nEndChar;
			GetRichEditCtrl().GetSel(nStartChar, nEndChar);
			if(nStartChar < m_nLastPromptPos)
			{
				GetRichEditCtrl().SetSel(-1, -1);
			}
			ProcessChar((char)nChar);	// 字符转义处理
			SetTermFontColor(SHELL_COLOR_INPUT);			
			CRichEditView::OnChar(nChar, nRepCnt, nFlags);

			if(nChar == '.')	// 输入"."后自动查找成员函数列表
			{
				// 获取最后一行的最后一个单词
				int nLine = GetRichEditCtrl().GetLineCount();
				char buf[512];
				memset(buf, 0, 512);
				buf[1] = 2;	// 缓冲区前两个字节表示缓冲区大小
				if(GetRichEditCtrl().GetLine(nLine-1, buf) > 0)
				{
					CString strLine;
					strLine.Format("%s", buf);
					if(strLine.Find(m_strPrompt) == 0)
					{
						strLine.Delete(0, strlen(m_strPrompt));
					}else
					if(strLine.Find(m_strExtPrompt) == 0)
					{
						strLine.Delete(0, strlen(m_strExtPrompt));
					}
					CString strWord = "";
					int nPos = strLine.GetLength()-2;
					while((nPos>=0) && (strLine.GetAt(nPos)!=' ') && (strLine.GetAt(nPos)!='\t'))
					{
						strWord = strLine.GetAt(nPos) + strWord;
						nPos--;
					}
					strWord.TrimRight();
					strWord.TrimRight('.');
					ProcessDot(strWord);
				}				
			}else
			if(nChar == '(')	// 输入"("后自动用ToolTip显示函数说明
			{
				// 获取最后一行的最后一个单词
				int nLine = GetRichEditCtrl().GetLineCount();
				char buf[512];
				memset(buf, 0, 512);
				buf[1] = 2;	// 缓冲区前两个字节表示缓冲区大小
				if(GetRichEditCtrl().GetLine(nLine-1, buf) > 0)
				{
					CString strLine;
					strLine.Format("%s", buf);
					if(strLine.Find(m_strPrompt) == 0)
					{
						strLine.Delete(0, strlen(m_strPrompt));
					}else
					if(strLine.Find(m_strExtPrompt) == 0)
					{
						strLine.Delete(0, strlen(m_strExtPrompt));
					}
					CString strWord = "";
					int nPos = strLine.GetLength()-4;
					CString str;
					str.Format("%d", nPos);
					while((nPos>=0) && 
						( ((strLine.GetAt(nPos)>='A') && (strLine.GetAt(nPos)<='Z')) ||
						  ((strLine.GetAt(nPos)>='a') && (strLine.GetAt(nPos)<='z')) ||
						  ((strLine.GetAt(nPos)>='0') && (strLine.GetAt(nPos)<='9')) ||
						  (strLine.GetAt(nPos)=='_') || (strLine.GetAt(nPos)=='.') || (strLine.GetAt(nPos)==0) ) )
					{
						strWord = strLine.GetAt(nPos) + strWord;
						nPos--;
					}
					strWord.TrimRight();
					strWord.TrimRight('.');
					ProcessLeftBracket(strWord);
				}				
			}
		}else
		if( (SHELL_LUA == m_nWorkShell) || (SHELL_RUBY == m_nWorkShell) )
		{	// 限定输出的字符都送到最后一条命令
			long nStartChar, nEndChar;
			GetRichEditCtrl().GetSel(nStartChar, nEndChar);
			if(nStartChar < m_nLastPromptPos)
			{
				GetRichEditCtrl().SetSel(-1, -1);
			}
			ProcessChar((char)nChar);	// 字符转义处理
			SetTermFontColor(SHELL_COLOR_INPUT);			
			CRichEditView::OnChar(nChar, nRepCnt, nFlags);

			if((nChar == '.') && (!m_bAutoCompletionMode))	// 输入"."后自动查找成员函数列表
			{
				// 获取最后一行的最后一个单词
				int nLine = GetRichEditCtrl().GetLineCount();
				char buf[512];
				memset(buf, 0, 512);
				buf[1] = 2;	// 缓冲区前两个字节表示缓冲区大小
				if(GetRichEditCtrl().GetLine(nLine-1, buf) > 0)
				{
					CString strLine;
					strLine.Format("%s", buf);
					if(strLine.Find(m_strPrompt) == 0)
					{
						strLine.Delete(0, strlen(m_strPrompt));
					}else
					if(strLine.Find(m_strExtPrompt) == 0)
					{
						strLine.Delete(0, strlen(m_strExtPrompt));
					}
					CString strWord = "";
					int nPos = strLine.GetLength()-2;
					while((nPos>=0) && (strLine.GetAt(nPos)!=' ') && (strLine.GetAt(nPos)!='\t'))
					{
						strWord = strLine.GetAt(nPos) + strWord;
						nPos--;
					}
					strWord.TrimRight();
					strWord.TrimRight('.');
					ProcessDot(strWord);
				}				
			}
		}else
		{
			ProcessChar((char)nChar);
		}

		// If in auto-complete mode, send the current word for incremental search
		// and selection in the popup list box control
		if (m_bAutoCompletionMode)
		{
			CString strWord = _T("");
			GetCurrentWord(strWord);
			//theApp.m_ol.TraceFmt(1, strWord);
			//theApp.m_ol.TraceFmt(1, "\r\n");
			// 如果第一个字符为".",则必须删除,否则无法和列表中的匹配,因为列表中是不包含".的"
			if(strWord.Find(".") == 0)
			{
				strWord.Delete(0, 1);
			}
			m_listPopup.DoIncrementalSearch(strWord);
		}

		return;
	}
	
	CRichEditView::OnChar(nChar, nRepCnt, nFlags);
}

/////////////////////////////////////////////////////////////////////////////
// 处理输入回车
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::ProcessReturn(CString strCmd)
{
	if(SHELL_TCL == m_nWorkShell)
	{
		// 处理提示符
		if(strCmd.Find(m_strPrompt) == 0)
		{
			strCmd.Delete(0, m_strPrompt.GetLength());
		}
		if( (strCmd.GetLength() > 0) && (strCmd[strCmd.GetLength()-1] == '\n') )
			strCmd.Delete(strCmd.GetLength()-1, 1);
		if( (strCmd.GetLength() > 0) && (strCmd[strCmd.GetLength()-1] == '\r') )
			strCmd.Delete(strCmd.GetLength()-1, 1);
		//while(strCmd.Remove('\r') > 0);
		//while(strCmd.Remove('\n') > 0);

		// 命令解释
		if(strCmd.GetLength() > 0)
		{
			// 命令添加到历史记录
			while(m_asCmdHistory.GetSize() > 100)
			{
				m_asCmdHistory.RemoveAt(0);
			}
			m_asCmdHistory.Add(strCmd);

			if(strCmd == "package require Tk")
			{
				m_pInterp->SetInterpParam("usetk", "true");
			}

			m_pInterp->RunScriptCommand(strCmd);
			int nLine = m_pInterp->GetErrorLine();
			CString strRes = m_pInterp->GetResult();
			if(strRes.GetLength() > 0)
			{
				Message("\r\n");
				if(nLine == 0)
					SetTermFontColor(SHELL_COLOR_RESOK);
				else
					SetTermFontColor(SHELL_COLOR_RESFAIL);
				Message(strRes);
			}
		}

		// 换行和显示结果
		SetTermFontColor(SHELL_COLOR_PROMPT);
		CString strLine = "\r\n";
		strLine += m_strPrompt;
		Message(strLine);
		SetTermFontColor(SHELL_COLOR_INPUT);

		// 历史记录指针重置
		m_nCurHistory		= m_asCmdHistory.GetSize();
		// 重置代码和引号层数
		m_nCurCodeLevel		= 0;
		m_nCurCommaLevel	= 0;
		m_nCurEscape		= 0;
		m_nContinueLine		= 0;
		// 提示符位置重新计算
		CheckLastPromptPos();
	}else
	if(SHELL_PYTHON == m_nWorkShell)
	{
		if(m_pInterp == NULL)
		{
			return;
		}

		// 处理提示符
		if(strCmd.Find(m_strPrompt) == 0)
		{
			strCmd.Delete(0, m_strPrompt.GetLength());
		}
		if( (strCmd.GetLength() > 0) && (strCmd[strCmd.GetLength()-1] == '\n') )
			strCmd.Delete(strCmd.GetLength()-1, 1);
		if( (strCmd.GetLength() > 0) && (strCmd[strCmd.GetLength()-1] == '\r') )
			strCmd.Delete(strCmd.GetLength()-1, 1);
		// 删除每一行的扩展提示符
		int nPos;
		while((nPos = strCmd.Find(SHELL_EXTPROMPTLINE_PYTHON)) != -1)
		{
			strCmd.Delete(nPos+2, strlen(SHELL_EXTPROMPT_PYTHON));
		}

		// 命令解释
		if(strCmd.GetLength() > 0)
		{
			// 命令添加到历史记录
			while(m_asCmdHistory.GetSize() > 100)
			{
				m_asCmdHistory.RemoveAt(0);
			}
			m_asCmdHistory.Add(strCmd);

			CString strRes = "";
			if(strCmd.Find("\r\n") != -1)
			{
				// 如果是多行的命令,则写入一个临时文件然后执行文件,临时文件在PTerminal所在目录
				CFile tempFile;
				// 获取临时文件路径
				TCHAR szFullPath[MAX_PATH];
				TCHAR szdrive[_MAX_DRIVE];
				TCHAR szdir[_MAX_DIR];
				::GetModuleFileName(AfxGetStaticModuleState()->m_hCurrentInstanceHandle, szFullPath, MAX_PATH);
				_splitpath(szFullPath, szdrive, szdir, NULL, NULL);

				CString strTempFile;
				strTempFile.Format(_T("%s%s%s"), szdrive, szdir, "tempterm.py");
				// 写入临时文件
				if (tempFile.Open(strTempFile, CFile::modeWrite | CFile::modeCreate, NULL) != 0)
				{
					tempFile.Write(strCmd.GetBuffer(0), strCmd.GetLength());
					tempFile.Close();
					m_pInterp->RunScriptFile(strTempFile);
					strRes = m_pInterp->GetResult();
				}
			}else
			{
				// 单行的命令可以直接用执行命令的方式
				m_pInterp->RunScriptCommand(strCmd);
				strRes = m_pInterp->GetResult();
			}
			if(strRes.GetLength() > 0)
			{
				int nLine = m_pInterp->GetErrorLine();
				Message("\r\n");
				if(nLine == 0)
					SetTermFontColor(SHELL_COLOR_RESOK);
				else
					SetTermFontColor(SHELL_COLOR_RESFAIL);
				Message(strRes);
			}
		}

		// 换行和显示结果
		SetTermFontColor(SHELL_COLOR_PROMPT);
		CString strLine = "\r\n";
		strLine += m_strPrompt;
		Message(strLine);
		SetTermFontColor(SHELL_COLOR_INPUT);

		// 历史记录指针重置
		m_nCurHistory		= m_asCmdHistory.GetSize();
		// 重置代码和引号层数
		m_nCurCodeLevel		= 0;
		m_nCurCommaLevel	= 0;
		m_nCurEscape		= 0;
		m_nContinueLine		= 0;
		// 提示符位置重新计算
		CheckLastPromptPos();
	} else
	if(SHELL_LUA == m_nWorkShell || SHELL_RUBY == m_nWorkShell || SHELL_PERL == m_nWorkShell)
	{
		// 处理提示符
		if(strCmd.Find(m_strPrompt) == 0)
		{
			strCmd.Delete(0, m_strPrompt.GetLength());
		}
		if( (strCmd.GetLength() > 0) && (strCmd[strCmd.GetLength()-1] == '\n') )
			strCmd.Delete(strCmd.GetLength()-1, 1);
		if( (strCmd.GetLength() > 0) && (strCmd[strCmd.GetLength()-1] == '\r') )
			strCmd.Delete(strCmd.GetLength()-1, 1);

		// 命令解释
		if(strCmd.GetLength() > 0)
		{
			// 命令添加到历史记录
			while(m_asCmdHistory.GetSize() > 100)
			{
				m_asCmdHistory.RemoveAt(0);
			}
			m_asCmdHistory.Add(strCmd);

			m_pInterp->RunScriptCommand(strCmd);
			int nLine = m_pInterp->GetErrorLine();
			CString strRes = m_pInterp->GetResult();
			if(strRes.GetLength() > 0)
			{
				Message("\r\n");
				if(nLine == 0)
					SetTermFontColor(SHELL_COLOR_RESOK);
				else
					SetTermFontColor(SHELL_COLOR_RESFAIL);
				Message(strRes);
			}
		}

		// 换行和显示结果
		SetTermFontColor(SHELL_COLOR_PROMPT);
		CString strLine = "\r\n";
		strLine += m_strPrompt;
		Message(strLine);
		SetTermFontColor(SHELL_COLOR_INPUT);

		// 历史记录指针重置
		m_nCurHistory		= m_asCmdHistory.GetSize();
		// 重置代码和引号层数
		m_nCurCodeLevel		= 0;
		m_nCurCommaLevel	= 0;
		m_nCurEscape		= 0;
		m_nContinueLine		= 0;
		// 提示符位置重新计算
		CheckLastPromptPos();
	}else
	if(SHELL_TELNET == m_nWorkShell)
	{
		if(m_pSocketRx && m_pSocketTx)
		{
			m_pSocketTx->m_strSend = "\r\n";
			m_pSocketTx->SetSendEvent();

			DWORD dwMode;
			m_pSocketRx->m_Protocol.GetConsoleMode(NULL, &dwMode);

			// 允许回显
			if(dwMode & ENABLE_ECHO_INPUT)
			{
				SetTermFontColor(SHELL_COLOR_INPUT);
				Message("\r\n");
			}
		}
	} else
	if(SHELL_COM == m_nWorkShell)
	{
		if(m_pSerialPort)
		{
			m_pSerialPort->WriteToPort("\r\n");

			// 是否允许回显
			if(m_bSerialEcho)
			{
				SetTermFontColor(SHELL_COLOR_INPUT);
				Message("\r\n");
			}
		}
	} else
	if(SHELL_RCADMIN == m_nWorkShell)
	{
		// 处理提示符
		if(strCmd.Find(m_strPrompt) == 0)
		{
			strCmd.Delete(0, m_strPrompt.GetLength());
		}
		if( (strCmd.GetLength() > 0) && (strCmd[strCmd.GetLength()-1] == '\n') )
			strCmd.Delete(strCmd.GetLength()-1, 1);
		if( (strCmd.GetLength() > 0) && (strCmd[strCmd.GetLength()-1] == '\r') )
			strCmd.Delete(strCmd.GetLength()-1, 1);
		//while(strCmd.Remove('\r') > 0);
		//while(strCmd.Remove('\n') > 0);

		// 命令解释
		if(strCmd.GetLength() > 0)
		{
			// 命令添加到历史记录
			while(m_asCmdHistory.GetSize() > 100)
			{
				m_asCmdHistory.RemoveAt(0);
			}
			m_asCmdHistory.Add(strCmd);

			if( m_bConnectRemote == FALSE )
			{
				SetTermFontColor(SHELL_COLOR_RESFAIL);
				Message("\r\nNot Connect Server!");
			}
		}

		// 换行和显示结果
		SetTermFontColor(SHELL_COLOR_PROMPT);
		CString strLine = "\r\n";
		strLine += m_strPrompt;
		Message(strLine);
		SetTermFontColor(SHELL_COLOR_INPUT);

		// 历史记录指针重置
		m_nCurHistory		= m_asCmdHistory.GetSize();
		// 重置代码和引号层数
		m_nCurCodeLevel		= 0;
		m_nCurCommaLevel	= 0;
		m_nCurEscape		= 0;
		m_nContinueLine		= 0;
		// 提示符位置重新计算
		CheckLastPromptPos();
	}
}

/////////////////////////////////////////////////////////////////////////////
// 处理普通输入字符
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::ProcessChar(CString strChar)
{	
	if( SHELL_TCL == m_nWorkShell )
	{
		// 处理代码层数等特殊字符
		if(strChar == '\\')
		{
			if(m_nCurEscape > 0)
				m_nCurEscape--;
			else
				m_nCurEscape++;
		}else
		if(strChar == '{')
		{
			m_nCurCodeLevel++;
		}else
		if(strChar == '}')
		{
			if(m_nCurCodeLevel > 0)
				m_nCurCodeLevel--;
		}else
		if(strChar == '\"')
		{
			if(m_nCurCommaLevel > 0)
				m_nCurCommaLevel--;
			else
				m_nCurCommaLevel++;
		}else
		if((strChar != ' ') && (strChar != '\t') && (strChar != '\r'))
		{
			m_nCurEscape = 0;
		}
	}else
	if(SHELL_PYTHON == m_nWorkShell)
	{
		// 处理代码层数等特殊字符
		if(strChar == '\\')
		{
			if(m_nCurEscape > 0)
				m_nCurEscape--;
			else
				m_nCurEscape++;
			m_nContinueLine = 0;
		}else
		if((strChar == '\"') || (strChar == '\''))
		{
			if(m_nCurCommaLevel > 0)
				m_nCurCommaLevel--;
			else
				m_nCurCommaLevel++;
			m_nContinueLine = 0;
		}else
		if((strChar == ":") && (m_nCurCommaLevel == 0))
		{
			// 续行标志处理
			if(m_nContinueLine == 0)
				m_nContinueLine = 1;
		}else
		if((strChar != ' ') && (strChar != '\t') && (strChar != '\r'))
		{
			m_nCurEscape = 0;
			m_nContinueLine = 0;
		}
	}else
	if( SHELL_LUA == m_nWorkShell )
	{
		// 处理代码层数等特殊字符
		if(strChar == '\\')
		{
			if(m_nCurEscape > 0)
				m_nCurEscape--;
			else
				m_nCurEscape++;
		}else
		if(strChar == '\"')
		{
			if(m_nCurCommaLevel > 0)
				m_nCurCommaLevel--;
			else
				m_nCurCommaLevel++;
		}else
		if((strChar != ' ') && (strChar != '\t') && (strChar != '\r'))
		{
			m_nCurEscape = 0;
		}
	}else
	if(SHELL_RUBY == m_nWorkShell)
	{
		// 处理代码层数等特殊字符
		if(strChar == '\\')
		{
			if(m_nCurEscape > 0)
				m_nCurEscape--;
			else
				m_nCurEscape++;
		}else
		if((strChar == '\"') || (strChar == '\''))
		{
			if(m_nCurCommaLevel > 0)
				m_nCurCommaLevel--;
			else
				m_nCurCommaLevel++;
		}else
		if((strChar != ' ') && (strChar != '\t') && (strChar != '\r'))
		{
			m_nCurEscape = 0;
		}
	}else
	if( SHELL_PERL == m_nWorkShell )
	{
		// 处理代码层数等特殊字符
		if(strChar == '\\')
		{
			if(m_nCurEscape > 0)
				m_nCurEscape--;
			else
				m_nCurEscape++;
		}else
		if(strChar == '{')
		{
			m_nCurCodeLevel++;
		}else
		if(strChar == '}')
		{
			if(m_nCurCodeLevel > 0)
				m_nCurCodeLevel--;
		}else
		if((strChar == '\"') || (strChar == '\''))
		{
			if(m_nCurCommaLevel > 0)
				m_nCurCommaLevel--;
			else
				m_nCurCommaLevel++;
		}else
		if((strChar != ' ') && (strChar != '\t') && (strChar != '\r'))
		{
			m_nCurEscape = 0;
		}
	}else
	if( SHELL_TELNET == m_nWorkShell )
	{
		if(m_pSocketRx && m_pSocketTx)
		{
			m_pSocketTx->m_strSend = strChar;
			m_pSocketTx->SetSendEvent();

			DWORD dwMode;
			m_pSocketRx->m_Protocol.GetConsoleMode(NULL, &dwMode);

			// 允许回显
			if(dwMode & ENABLE_ECHO_INPUT)
			{
				SetTermFontColor(SHELL_COLOR_INPUT);
				Message(strChar);
			}
		}
	}else
	if( SHELL_COM == m_nWorkShell )
	{
		if(m_pSerialPort)
		{
			m_pSerialPort->WriteToPort(strChar.GetBuffer(0));
			strChar.ReleaseBuffer();

			// 是否允许回显
			if(m_bSerialEcho)
			{
				SetTermFontColor(SHELL_COLOR_INPUT);
				Message(strChar);
			}
		}
	}else
	if( SHELL_RCADMIN == m_nWorkShell )
	{
		// 处理代码层数等特殊字符
		if(strChar == '\\')
		{
			if(m_nCurEscape > 0)
				m_nCurEscape--;
			else
				m_nCurEscape++;
		}else
		if(strChar == '{')
		{
			m_nCurCodeLevel++;
		}else
		if(strChar == '}')
		{
			if(m_nCurCodeLevel > 0)
				m_nCurCodeLevel--;
		}else
		if(strChar == '\"')
		{
			if(m_nCurCommaLevel > 0)
				m_nCurCommaLevel--;
			else
				m_nCurCommaLevel++;
		}else
		if((strChar != ' ') && (strChar != '\t') && (strChar != '\r'))
		{
			m_nCurEscape = 0;
		}
	}
	else
	{
		SetTermFontColor(SHELL_COLOR_INPUT);
		Message(strChar);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 处理'.'字符(表示列对象方法)
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::ProcessDot(CString strWord)
{
	if(m_pInterp == NULL)
	{
		return;
	}

	if(SHELL_PYTHON == m_nWorkShell)
	{
		CString strCmd;
		strCmd.Format("_platform.write(0,'%%s' %% dir(%s),18,0)", strWord);
		m_pInterp->RunScriptCommand(strCmd);
		CString strRes = m_pInterp->GetResult();
		if(strRes.GetLength() > 0)
		{
			int nLine = m_pInterp->GetErrorLine();
			if(nLine == 0)
			{
				// 返回的列表形式['xx','xx']
				CStringArray asFunc;
				strRes.Remove('[');
				strRes.Remove(']');
				strRes.Remove('\'');
				strRes.Remove(',');
				strRes.TrimLeft();
				strRes.TrimRight();
				int nPos = 0;
				while((nPos = strRes.Find(' ')) != -1)
				{						
					asFunc.Add(strRes.Left(nPos));
					strRes.Delete(0, nPos);
					strRes.TrimLeft();
				}
				if(strRes.GetLength() > 0)
				{
					asFunc.Add(strRes);
				}

				if(asFunc.GetSize() > 0)
				{
					DoEditCompleteKeyword(AUTOCOMPLETE_PROCEDURE, &asFunc);
				}
			}
		}
	}else
	if(SHELL_LUA == m_nWorkShell)
	{
		if(strWord == "")
		{
			// 如果没有单词,就列出所有全局变量名
			strWord = "_G";
		}
		CString strCmd;
		strCmd.Format("for k,v in pairs(%s) do put_result(k,\" \") end", strWord);
		CStringArray asMethod;
		if(!SendInterpListCommand(strCmd, " ", asMethod))
			return;
		if(asMethod.GetSize() > 0)
		{
			DoEditCompleteKeyword(AUTOCOMPLETE_PROCEDURE, &asMethod);
		}
	}else
	if(SHELL_RUBY == m_nWorkShell)
	{
		CStringArray asMethod;
		if(!SendInterpListCommand(_T("methods"), " ", asMethod))
			return;
		if(asMethod.GetSize() > 0)
		{
			DoEditCompleteKeyword(AUTOCOMPLETE_PROCEDURE, &asMethod);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 处理'('字符,获取函数说明
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::ProcessLeftBracket(CString strWord)
{
	if(m_pInterp == NULL)
	{
		return;
	}

	if(SHELL_PYTHON == m_nWorkShell)
	{
		CString strCmd;
		strCmd.Format("_platform.write(0,%s.__doc__,18,0)", strWord);
		m_pInterp->RunScriptCommand(strCmd);
		CString strRes = m_pInterp->GetResult();
		if(strRes.GetLength() > 0)
		{
			int nLine = m_pInterp->GetErrorLine();
			if(nLine == 0)
			{
				CString strFuncDoc = strRes;					
				if(strFuncDoc.GetLength() > 0)
				{
					ShowTipWindow(strWord, strFuncDoc);
				}
			}
		}
	}
}

void CPTerminalView::ShowTipWindow(CString strTipTitle, CString strTipText)
{
	if (::IsWindow(m_tipWindow.m_hWnd))
	{
		m_tipWindow.DestroyWindow();
	}
	
    CString sTemp(strTipText);
    sTemp.Replace(_T("\\n"), _T("\n"));
	m_tipWindow.SetTipText(strTipTitle, sTemp);

	CPoint point = GetCaretPos();  
	ClientToScreen(&point);
	point.Offset(0, 20);

//	m_tipWindow.ShowTipWindow(
//		point, this, TWS_XT_ALPHASHADOW | TWS_XT_DROPSHADOW, 5000);
	m_tipWindow.ShowTipWindow(
		point, TWS_XT_ALPHASHADOW | TWS_XT_DROPSHADOW, 5000);

}

void CPTerminalView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_F10 && GetKeyState(VK_SHIFT) < 0)	// 右键菜单的热键
	{
		long nStart, nEnd;
		GetRichEditCtrl().GetSel(nStart, nEnd);
		CPoint pt = GetRichEditCtrl().GetCharPos(nEnd);
		OnRButtonUp(0, pt);
	}else
	if(nChar == VK_RETURN && GetKeyState(VK_CONTROL) < 0)	// 自动补齐命令
	{
		DoEditCompleteKeyword(AUTOCOMPLETE_COMMAND);
		return;
	}else
	if(nChar == VK_F7 && GetKeyState(VK_CONTROL) < 0)	// 自动补齐过程
	{
		DoEditCompleteKeyword(AUTOCOMPLETE_PROCEDURE);
		return;
	}else
	if(nChar == VK_F8 && GetKeyState(VK_CONTROL) < 0)	// 自动补齐变量
	{
		DoEditCompleteKeyword(AUTOCOMPLETE_VAR);
		return;
	}else
	if(nChar == VK_F9 && GetKeyState(VK_CONTROL) < 0)	// 自动补齐类
	{
		DoEditCompleteKeyword(AUTOCOMPLETE_CLASS);
		return;
	}else
	if (nChar == VK_UP)	// 处理上箭头,表示向上查看历史
	{
		long nStartChar, nEndChar;
		GetRichEditCtrl().GetSel(nStartChar, nEndChar);
		if((nStartChar >= m_nLastPromptPos) && (nStartChar == nEndChar))
		{
			if( (m_asCmdHistory.GetSize() > 0) && 
				(m_nCurHistory > 0) )
			{
				m_nCurHistory--;
				GetRichEditCtrl().SetSel(m_nLastPromptPos, -1);
				SetTermFontColor(SHELL_COLOR_INPUT);
				GetRichEditCtrl().ReplaceSel(m_asCmdHistory[m_nCurHistory]);				
				GetRichEditCtrl().SetSel(-1, -1);
				// 重置代码和引号层数
				m_nCurCodeLevel	= 0;
				m_nCurCommaLevel	= 0;
				m_nCurEscape		= 0;
				return;
			}
		}
		return;
	}else
	if (nChar == VK_DOWN)	// 处理下箭头,表示向下查看历史
	{
		long nStartChar, nEndChar;
		GetRichEditCtrl().GetSel(nStartChar, nEndChar);
		if((nStartChar >= m_nLastPromptPos) && (nStartChar == nEndChar))
		{
			if(m_nCurHistory < m_asCmdHistory.GetSize())
			{
				m_nCurHistory++;				
				GetRichEditCtrl().SetSel(m_nLastPromptPos, -1);
				SetTermFontColor(SHELL_COLOR_INPUT);
				if(m_nCurHistory < m_asCmdHistory.GetSize())
				{					
					GetRichEditCtrl().ReplaceSel(m_asCmdHistory[m_nCurHistory]);
				}else
				{
					GetRichEditCtrl().ReplaceSel(_T(""));
				}
				GetRichEditCtrl().SetSel(-1, -1);				
				// 重置代码和引号层数
				m_nCurCodeLevel	= 0;
				m_nCurCommaLevel	= 0;
				m_nCurEscape		= 0;
				return;
			}
		}
	}else
	if (nChar == VK_LEFT)	// 处理左箭头
	{
		long nStartChar, nEndChar;
		GetRichEditCtrl().GetSel(nStartChar, nEndChar);

		////////////////////////////////////////////
		if (m_bAutoCompletionMode)
		{
			ASSERT(m_pAutoComPopupWnd != NULL);
			if (m_nAutoComStartPos == nStartChar && m_pAutoComPopupWnd != NULL)
			{
				m_pAutoComPopupWnd->EndPopup(TRUE, TRUE);
				return;
			}
		}
		/////////////////////////////////////////////

		if(nStartChar < m_nLastPromptPos)
		{
			GetRichEditCtrl().SetSel(m_nLastPromptPos, m_nLastPromptPos);
			return;
		}else
		if(nStartChar == m_nLastPromptPos)
		{			
			return;
		}
	}else
	if (nChar == VK_RIGHT)	// 处理右箭头
	{
		//////////////////////////////////////////////
		if (m_bAutoCompletionMode)
		{
			ASSERT(m_pAutoComPopupWnd != NULL);
			if (/*(m_nAutoComStartPos == ptCurPos) ||*/
				 m_pAutoComPopupWnd != NULL)
			{
				m_pAutoComPopupWnd->EndPopup(TRUE, TRUE);
				return;
			}
		}
		//////////////////////////////////////////////

		long nStartChar, nEndChar;
		GetRichEditCtrl().GetSel(nStartChar, nEndChar);
		if(nEndChar < m_nLastPromptPos)
		{
			GetRichEditCtrl().SetSel(m_nLastPromptPos, m_nLastPromptPos);
			return;
		}
	}
	if (nChar == VK_BACK)	// 处理BackSpace
	{
		long nStartChar, nEndChar;
		GetRichEditCtrl().GetSel(nStartChar, nEndChar);
		if(nStartChar <= m_nLastPromptPos)
		{
			return;
		}
		GetRichEditCtrl().SetSel(nStartChar-1, nStartChar);
		CString strSel = GetRichEditCtrl().GetSelText();
		GetRichEditCtrl().SetSel(nStartChar, nEndChar);
		// 处理几种转义和代码层数控制符
		if((strSel == '\"') || (strSel == '\\'))
		{
			ProcessChar((char)nChar);
		}else
		if(strSel == '{')
		{
			ProcessChar("}");
		}else
		if(strSel == '}')
		{
			ProcessChar("{");
		}
	}
	if (nChar == VK_DELETE)	// 处理Del
	{
		long nStartChar, nEndChar;
		GetRichEditCtrl().GetSel(nStartChar, nEndChar);
		if(nStartChar < m_nLastPromptPos)
		{
			return;
		}
		if(nStartChar < (GetRichEditCtrl().GetTextLength()-1))
		{
			GetRichEditCtrl().SetSel(nStartChar, nStartChar+1);
			CString strSel = GetRichEditCtrl().GetSelText();
			GetRichEditCtrl().SetSel(nStartChar, nEndChar);
			// 处理几种转义和代码层数控制符
			if((strSel == '\"') || (strSel == '\\'))
			{
				ProcessChar((char)nChar);
			}else
			if(strSel == '{')
			{
				ProcessChar("}");
			}else
			if(strSel == '}')
			{
				ProcessChar("{");
			}
		}
	}else
	if (nChar == VK_HOME)	// 处理HOME键
	{
		if(GetKeyState(VK_SHIFT) < 0)	// 处理Shift+HOME键
		{
			int nLineIndex = GetRichEditCtrl().LineIndex();
			if(nLineIndex < m_nLastPromptPos)
			{
				long nStartChar, nEndChar;
				GetRichEditCtrl().GetSel(nStartChar, nEndChar);
				GetRichEditCtrl().SetSel(m_nLastPromptPos, nEndChar);
				return;
			}
		}else	// 处理HOME
		{
			int nLineIndex = GetRichEditCtrl().LineIndex();
			if(nLineIndex < m_nLastPromptPos)
			{
				GetRichEditCtrl().SetSel(m_nLastPromptPos, m_nLastPromptPos);
				return;
			}
		}
	}

	CRichEditView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CPTerminalView::GetCurrentWord(CString& strWord, BOOL bSelectIt/* = FALSE*/)
{
	strWord = "";

	long nStartChar, nEndChar;
	GetRichEditCtrl().GetSel(nStartChar, nEndChar);

	int nStartPos = nStartChar;
	int nEndPos = nStartChar;
	if(nStartChar < m_nLastPromptPos)
		nStartPos = m_nLastPromptPos;
	if(nEndPos >= GetRichEditCtrl().GetTextLength())
		nEndPos = GetRichEditCtrl().GetTextLength();

	while(TRUE)
	{
		nStartPos--;
		if(nStartPos < m_nLastPromptPos)
			nStartPos = m_nLastPromptPos;
		GetRichEditCtrl().SetSel(nStartPos, nEndPos);
		strWord = GetRichEditCtrl().GetSelText();
		if(nStartPos == m_nLastPromptPos)
			break;
		if((strWord.GetLength() > 0) && ((strWord[0] == ' ') || (strWord[0] == '.')))
			break;
	}

	while(TRUE)
	{
		nEndPos++;
		if(nEndPos >= GetRichEditCtrl().GetTextLength())
			nEndPos = GetRichEditCtrl().GetTextLength();
		GetRichEditCtrl().SetSel(nStartPos, nEndPos);
		strWord = GetRichEditCtrl().GetSelText();
		if(nEndPos == GetRichEditCtrl().GetTextLength())
			break;
		if((strWord.GetLength() > 0) && (strWord[strWord.GetLength()-1] == ' '))
			break;
	}
	GetRichEditCtrl().SetSel(nStartChar, nEndChar);
	if(strWord.GetLength() > 0)
	{
		strWord.TrimLeft();
		strWord.TrimRight();
	}
}

BOOL CPTerminalView::SetCurrentWord(const CString& strWord, BOOL bSelectIt/* = FALSE*/)
{
	CString strWordGet = "";

	long nStartChar, nEndChar;
	GetRichEditCtrl().GetSel(nStartChar, nEndChar);

	int nStartPos = nStartChar;
	int nEndPos = nStartChar;
	if(nStartChar < m_nLastPromptPos)
		nStartPos = m_nLastPromptPos;
	if(nEndPos >= GetRichEditCtrl().GetTextLength())
		nEndPos = GetRichEditCtrl().GetTextLength();

	while(TRUE)
	{
		nStartPos--;
		if(nStartPos < m_nLastPromptPos)
			nStartPos = m_nLastPromptPos;
		GetRichEditCtrl().SetSel(nStartPos, nEndPos);
		strWordGet = GetRichEditCtrl().GetSelText();
		if(nStartPos == m_nLastPromptPos)
			break;
		if((strWordGet.GetLength() > 0) && ((strWordGet[0] == ' ') || (strWordGet[0] == '.')))
			break;
	}

	while(TRUE)
	{
		nEndPos++;
		if(nEndPos >= GetRichEditCtrl().GetTextLength())
			nEndPos = GetRichEditCtrl().GetTextLength();
		GetRichEditCtrl().SetSel(nStartPos, nEndPos);
		strWordGet = GetRichEditCtrl().GetSelText();
		if(nEndPos == GetRichEditCtrl().GetTextLength())
			break;
		if((strWordGet.GetLength() > 0) && (strWordGet[strWordGet.GetLength()-1] == ' '))
			break;
	}

	if(strWordGet.GetLength() > 0)
	{
		if((strWordGet[0] == ' ') && (nStartPos < nStartChar))
		{
			nStartPos++;
			strWordGet.TrimLeft();
		}
	}
	if(strWordGet.GetLength() > 0)
	{
		if((strWordGet[strWordGet.GetLength()-1] == ' ') && (nEndPos > nEndChar))
		{
			nEndPos--;
			strWordGet.TrimRight();
		}
	}

	GetRichEditCtrl().SetSel(nStartPos, nEndPos);
	GetRichEditCtrl().ReplaceSel(strWord);
	int nPos = nStartPos + strWord.GetLength();
	GetRichEditCtrl().SetSel(nPos, nPos);

	return TRUE;
}

BOOL CPTerminalView::InsertCurrentWord(const CString& strWord, BOOL bSelectIt/* = FALSE*/)
{
	CString strWordGet = "";

	long nStartChar, nEndChar;
	GetRichEditCtrl().GetSel(nStartChar, nEndChar);

	int nStartPos = nStartChar;
	int nEndPos = nStartChar;
	if(nStartChar < m_nLastPromptPos)
		nStartPos = m_nLastPromptPos;
	if(nEndPos >= GetRichEditCtrl().GetTextLength())
		nEndPos = GetRichEditCtrl().GetTextLength();

	while(TRUE)
	{
		nStartPos--;
		if(nStartPos < m_nLastPromptPos)
			nStartPos = m_nLastPromptPos;
		GetRichEditCtrl().SetSel(nStartPos, nEndPos);
		strWordGet = GetRichEditCtrl().GetSelText();
		if(nStartPos == m_nLastPromptPos)
			break;
		if((strWordGet.GetLength() > 0) && (strWordGet[0] == ' '))
			break;
		if((strWordGet.GetLength() > 0) && (strWordGet[0] == '.'))
		{
			nStartPos++;
			break;
		}
	}

	while(TRUE)
	{
		nEndPos++;
		if(nEndPos >= GetRichEditCtrl().GetTextLength())
			nEndPos = GetRichEditCtrl().GetTextLength();
		GetRichEditCtrl().SetSel(nStartPos, nEndPos);
		strWordGet = GetRichEditCtrl().GetSelText();
		if(nEndPos == GetRichEditCtrl().GetTextLength())
			break;
		if((strWordGet.GetLength() > 0) && (strWordGet[strWordGet.GetLength()-1] == ' '))
			break;
	}

	if(strWordGet.GetLength() > 0)
	{
		if((strWordGet[0] == ' ') && (nStartPos < nStartChar))
		{
			nStartPos++;
			strWordGet.TrimLeft();
		}
	}
	if(strWordGet.GetLength() > 0)
	{
		if((strWordGet[strWordGet.GetLength()-1] == ' ') && (nEndPos > nEndChar))
		{
			nEndPos--;
			strWordGet.TrimRight();
		}
	}

	GetRichEditCtrl().SetSel(nStartPos, nEndPos);
	GetRichEditCtrl().ReplaceSel(strWord);
	int nPos = nStartPos + strWord.GetLength();
	GetRichEditCtrl().SetSel(nPos, nPos);

	return TRUE;
}

void CPTerminalView::DoAutoCompletePopupWnd(INSERTTYPE xType)
{
	// 1. Make sure the popup list box is created.
	if (m_listPopup.GetSafeHwnd() == NULL)	  
	{
		//TODO--cool error handler here!
		return;
	}
	
	// 2. Find out the location of the caret (input point) and intialize
	//    all the necessary screen parameters.
	CPoint point        = GetCaretPos();  
	CPoint ptCursor     = point;
	ClientToScreen(&point);

	long nStartChar, nEndChar;
	GetRichEditCtrl().GetSel(nStartChar, nEndChar);
	m_nAutoComStartPos = nStartChar; //Save the current position
	
	// 3. Calculate the dimensions of the list box window, it will determine
	//    the dimension of the popup window.
	CWindowRect rcCaller(m_listPopup);
	rcCaller.left = point.x;
	rcCaller.top  = point.y + 16;
	
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
		m_nAutoComStartPos = 0;
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
	pPopup->SetTextHeight(16);	  // Text height, based on the font size
	pPopup->SetSendViewKeys(TRUE);			  // Let the view receive keyboard
	// messages while popup displayed and the user types further.
	VERIFY(pPopup->Display(&m_listPopup, this, 
		rcCaller, (CCrystalPopupWnd::CURSORALIGN)nOrientation, 
		&m_listPopup.m_xPopupCtrlEvent, FALSE));
}

/////////////////////////////////////////////////////////////////////////////
// 关键字拼写
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::DoEditCompleteKeyword(int nListType, CStringArray* paKeyword)
{
	// 1. Make sure the popup list box is created.
	if (m_listPopup.GetSafeHwnd() == NULL)	  
	{	
		return;
	}

//	if(m_listPopup.m_pPopWnd)
//	{
//		m_listPopup.m_pPopWnd->EndPopup(TRUE);
//	}
	
	// 2. Initialize the popup list box, reset it!
	m_listPopup.ResetContent();
	
	// 3.
	//...for each keyword map, loop through adding each item
	// to the popup list.
	if(paKeyword == NULL)
	{
		FillKeywordList(nListType);
	}else
	{
		int nImage;
		switch(nListType)
		{
		case AUTOCOMPLETE_COMMAND: nImage=ACI_KEYWORD; break;
		case AUTOCOMPLETE_PROCEDURE: nImage=ACI_CLASS_FUNC; break;
		case AUTOCOMPLETE_VAR: nImage=ACI_CLASS_VAR; break;
		case AUTOCOMPLETE_CLASS: nImage=ACI_CLASS; break;
		case AUTOCOMPLETE_OBJECT: nImage=ACI_CLASS; break;
		}
		// 添加列表
		for(int i=0; i<paKeyword->GetSize(); i++)
		{
			m_listPopup.AddString(		// Add to the list box
						(*paKeyword)[i], 
						nImage);	//...it is a keyword type
		}
	}
	
	// 4. Resize to fix the content, displaying at most 10 items
	m_listPopup.AutoSizeWindow();

	// 5. Get the current word being type, if any
	CString strWord = _T("");
	GetCurrentWord(strWord);
									   
	// 6. Select a matching of the current word in the list box
	int nIndex = m_listPopup.SelectString(-1, strWord);
	if (nIndex == LB_ERR || strWord.IsEmpty())
		m_listPopup.SetCurSel(-1);

	// 7. Finally, display the popup window with a complete word flag
	if(paKeyword == NULL)
	{
		DoAutoCompletePopupWnd(PLIT_COMPLETEWORD);
	}else
	{
		DoAutoCompletePopupWnd(PLIT_INSERTWORD);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 填充关键字列表
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::FillKeywordList(int nListType)
{
	// 目前只支持Tcl模式下的关键字列表
	if(SHELL_TCL != m_nWorkShell)
	{
		return;
	}

	switch(nListType)
	{
	case AUTOCOMPLETE_COMMAND:FillKeywordListCommand(); break;
	case AUTOCOMPLETE_PROCEDURE:FillKeywordListProcedure(); break;
	case AUTOCOMPLETE_VAR:FillKeywordListVar(); break;
	case AUTOCOMPLETE_CLASS:FillKeywordListClass(); break;
	case AUTOCOMPLETE_OBJECT:FillKeywordListObject(); break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// 填充关键字列表(命令)
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::FillKeywordListCommand()
{
	CStringArray asCommands;	// 命令数组
	if(SendInterpListCommand(_T("info commands"), " ", asCommands))
	{
		// 平台扩展命令列表
		CStringArray asKeyWordEx;
		m_pInterp->GetKeyWordList("platform", asKeyWordEx);

		// 添加内核命令
		for(int i=0; i<asCommands.GetSize(); i++)
		{
			if(FindStringInArray(&asKeyWordEx, asCommands[i]) >= 0)
				continue;
			m_listPopup.AddString(		// Add to the list box
						asCommands[i], 
						ACI_KEYWORD);	//...it is a keyword type
		}

		// 添加平台扩展命令
		for(int i=0; i<asKeyWordEx.GetSize(); i++)
		{
			m_listPopup.AddString(		// Add to the list box
						asKeyWordEx[i], 
						ACI_KEYWORDEX);	//...it is a keyword type
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 填充关键字列表(过程)
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::FillKeywordListProcedure()
{
	CStringArray asProcs;	// 过程数组
	if(SendInterpListCommand(_T("info procs"), " ", asProcs))
	{
		for(int i=0; i<asProcs.GetSize(); i++)
		{
			m_listPopup.AddString(		// Add to the list box
						asProcs[i], 
						ACI_CLASS_FUNC);	//...it is a keyword type
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 填充关键字列表(变量,包括对象)
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::FillKeywordListVar()
{
	CStringArray asLocals;	// 全局变量数组
	if(SendInterpListCommand(_T("info local"), " ", asLocals))
	{
		for(int i=0; i<asLocals.GetSize(); i++)
		{
			m_listPopup.AddString(		// Add to the list box
						asLocals[i], 
						ACI_CLASS_PRIVVAR);	//...it is a keyword type
		}
	}

	CStringArray asGlobals;	// 全局变量数组
	if(SendInterpListCommand(_T("info globals"), " ", asGlobals))
	{
		for(int i=0; i<asGlobals.GetSize(); i++)
		{
			m_listPopup.AddString(		// Add to the list box
						asGlobals[i], 
						ACI_CLASS_VAR);	//...it is a keyword type
		}
	}

	CStringArray asObjects;	// 对象数组
	if(SendInterpListCommand(_T("itcl::find objects"), " ", asObjects))
	{
		for(int i=0; i<asObjects.GetSize(); i++)
		{
			m_listPopup.AddString(		// Add to the list box
						asObjects[i], 
						ACI_CLASS);	//...it is a keyword type
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 填充关键字列表(类)
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::FillKeywordListClass()
{
	CStringArray asClass;	// 类数组
	if(SendInterpListCommand(_T("itcl::find classes"), " ", asClass))
	{
		for(int i=0; i<asClass.GetSize(); i++)
		{
			m_listPopup.AddString(		// Add to the list box
						asClass[i], 
						ACI_CLASS);	//...it is a keyword type
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 填充关键字列表(对象)
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::FillKeywordListObject()
{
	CStringArray asObjects;	// 对象数组
	if(SendInterpListCommand(_T("itcl::find objects"), " ", asObjects))
	{
		for(int i=0; i<asObjects.GetSize(); i++)
		{
			m_listPopup.AddString(		// Add to the list box
						asObjects[i], 
						ACI_CLASS);	//...it is a keyword type
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 输出
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::Message(LPCTSTR lpszMessage)
{
	CString strTemp = lpszMessage;
	if(strTemp.GetLength() == 1 && (BYTE)(strTemp[0]) >= 0x80)
	{
		// 字符大于可显示字符的处理
		if(m_strLastMsg.GetLength() > 0)
		{
			// 如果有未显示完的字符串,则先拼接
			strTemp = m_strLastMsg + strTemp;
			m_strLastMsg = "";
		}else
		{
			// 否则记录字符串,不显示,退出
			m_strLastMsg = strTemp;
			return;
		}
	}else
	{
		// 如果有未显示完的不可显示字符,则转换为"."拼接在前面
		if(m_strLastMsg.GetLength() > 0)
		{
			strTemp = "." + strTemp;
			m_strLastMsg = "";
		}
	}
	int len = GetWindowTextLength();
	GetRichEditCtrl().SetSel(len,len);
	GetRichEditCtrl().ReplaceSel(strTemp);
}

void CPTerminalView::ColorMessage(CString strMsg, COLORREF color)
{
	SetTermFontColor(m_clrRecv);
	Message(strMsg);
}

/////////////////////////////////////////////////////////////////////////////
// 关于
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::OnOwmAbout() 
{
	if(theApp.m_pIPlatUI)
	{
		theApp.m_pIPlatUI->SendOwmCmd("", OC_SHOWABOUTBOX, (WPARAM)(LPCTSTR)"org.owm.terminal", 0, NULL);
	}
}

void CPTerminalView::OnUpdateOwmAbout(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 加载脚本或库文件
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::OnTermOpenfile() 
{
	CString strTemp;
	CString strDefExt;
	if(SHELL_TCL == m_nWorkShell)
	{
		LOADRCSTRING( strTemp, IDS_TCL_FILE_FILTER )
		strDefExt = _T(".tcl");
	}else
	if(SHELL_PYTHON == m_nWorkShell)
	{
		LOADRCSTRING( strTemp, IDS_PYTHON_FILE_FILTER )
		strDefExt = _T(".py");
	}else
	if(SHELL_LUA == m_nWorkShell)
	{
		LOADRCSTRING( strTemp, IDS_LUA_FILE_FILTER )
		strDefExt = _T(".lua");
	}else
	if(SHELL_RUBY == m_nWorkShell)
	{
		LOADRCSTRING( strTemp, IDS_RUBY_FILE_FILTER )
		strDefExt = _T(".rb");
	}else
	if(SHELL_PERL == m_nWorkShell)
	{
		LOADRCSTRING( strTemp, IDS_PERL_FILE_FILTER )
		strDefExt = _T(".pl");
	}
	CFileDialog fd(TRUE, strDefExt, NULL, OFN_HIDEREADONLY, strTemp);

	if (fd.DoModal() == IDCANCEL)
		return;

	CString strFileName = fd.GetPathName();
	CString strExt = fd.GetFileExt();
	strExt.MakeUpper();

	CString strCmd;
	if(strExt == _T("TCL") || strExt == _T("TBC"))
	{
		strFileName.Replace('\\', '/');	// 路径替换,因为TCL只支持UNIX风格路径

		// 如果是pkgIndex.tcl,则预处理脚本,创建dir变量
		CString strFile = fd.GetFileName();
		strFile.MakeUpper();
		if(strFile == "PKGINDEX.TCL")
		{
			CString strPreCmd = "set dir \"";
			strPreCmd += strFileName;
			strPreCmd.Delete(strPreCmd.GetLength()-13, 13);
			strPreCmd += "\"";
			//TRACE("%s\n", strPreCmd);
			m_pInterp->RunScriptCommand(strPreCmd);
		}
		
		// 调用source加载脚本
		strCmd = _T("source ");
		strCmd += strFileName;
		SetTermFontColor(SHELL_COLOR_INPUT);
		Message(strCmd);
		ProcessReturn(strCmd);

		{	// 删除dir变量
			CString strPreCmd = "unset dir";
			m_pInterp->RunScriptCommand(strPreCmd);
		}
	}else
	if(strExt == _T("DLL"))
	{
		strFileName.Replace('\\', '/');	// 路径替换,因为TCL只支持UNIX风格路径
		// 调用load加载库(有些问题,因为需要指明package名)
		strCmd = _T("load ");
		strCmd += strFileName;
		SetTermFontColor(SHELL_COLOR_INPUT);
		Message(strCmd);
		ProcessReturn(strCmd);
	}else
	if(strExt == _T("PY") || strExt == _T("PYW") || strExt == _T("PYC"))	// 执行Python脚本
	{
		m_pInterp->RunScriptFile(strFileName);
		CString strRes = m_pInterp->GetResult();
		if(strRes != "")
		{
			int nResult = m_pInterp->GetErrorLine();
			SetTermFontColor(SHELL_COLOR_INPUT);
			Message("exec ");
			Message(strFileName);
			Message("\r\n");
			if(nResult == -1)
			{
				SetTermFontColor(SHELL_COLOR_RESFAIL);
			}else
			{
				SetTermFontColor(SHELL_COLOR_RESOK);
			}			
			Message(strRes);
			ProcessReturn("");
		}
	}else
	if(strExt == _T("LUA") || strExt == _T("LUAC"))	// 执行Lua脚本
	{
		m_pInterp->RunScriptFile(strFileName);
		CString strRes = m_pInterp->GetResult();
		if(strRes != "")
		{
			int nResult = m_pInterp->GetErrorLine();
			SetTermFontColor(SHELL_COLOR_INPUT);
			Message("exec ");
			Message(strFileName);
			Message("\r\n");
			if(nResult == -1)
			{
				SetTermFontColor(SHELL_COLOR_RESFAIL);
			}else
			{
				SetTermFontColor(SHELL_COLOR_RESOK);
			}			
			Message(strRes);
			ProcessReturn("");
		}
	}else
	if(strExt == _T("RB"))	// 执行Ruby脚本
	{
		m_pInterp->RunScriptFile(strFileName);
		CString strRes = m_pInterp->GetResult();
		if(strRes != "")
		{
			int nResult = m_pInterp->GetErrorLine();
			SetTermFontColor(SHELL_COLOR_INPUT);
			Message("exec ");
			Message(strFileName);
			Message("\r\n");
			if(nResult == -1)
			{
				SetTermFontColor(SHELL_COLOR_RESFAIL);
			}else
			{
				SetTermFontColor(SHELL_COLOR_RESOK);
			}			
			Message(strRes);
			ProcessReturn("");
		}
	}else
	if(strExt == _T("PL") || strExt == _T("PM"))	// 执行Perl脚本
	{
		m_pInterp->RunScriptFile(strFileName);
		CString strRes = m_pInterp->GetResult();
		if(strRes != "")
		{
			int nResult = m_pInterp->GetErrorLine();
			SetTermFontColor(SHELL_COLOR_INPUT);
			Message("exec ");
			Message(strFileName);
			Message("\r\n");
			if(nResult == -1)
			{
				SetTermFontColor(SHELL_COLOR_RESFAIL);
			}else
			{
				SetTermFontColor(SHELL_COLOR_RESOK);
			}			
			Message(strRes);
			ProcessReturn("");
		}
	}else
	{
	}
}

void CPTerminalView::OnUpdateTermOpenfile(CCmdUI* pCmdUI) 
{
	BOOL bEnable = ((SHELL_TCL == m_nWorkShell) || (SHELL_PYTHON == m_nWorkShell)
					 || (SHELL_LUA == m_nWorkShell) || (SHELL_RUBY == m_nWorkShell)
					 || (SHELL_PERL == m_nWorkShell));
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 重载剪切操作
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::OnEditCut() 
{
	GetRichEditCtrl().Cut();
}

void CPTerminalView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	long nStartChar, nEndChar;
	GetRichEditCtrl().GetSel(nStartChar, nEndChar);
	if(nStartChar < m_nLastPromptPos)
	{
		pCmdUI->Enable(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 重载粘贴操作
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::OnEditPaste() 
{
	//只能粘贴文本
	GetRichEditCtrl().PasteSpecial(CF_TEXT);	
//	GetRichEditCtrl().Paste();
}

void CPTerminalView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	long nStartChar, nEndChar;
	GetRichEditCtrl().GetSel(nStartChar, nEndChar);
	if(nStartChar < m_nLastPromptPos)
	{
		pCmdUI->Enable(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 重载清除操作
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::OnEditClear() 
{
	GetRichEditCtrl().Clear();
}

void CPTerminalView::OnUpdateEditClear(CCmdUI* pCmdUI) 
{
	long nStartChar, nEndChar;
	GetRichEditCtrl().GetSel(nStartChar, nEndChar);
	if(nStartChar < m_nLastPromptPos)
	{
		pCmdUI->Enable(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 清空
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::OnEditClearAll() 
{
	int nEndPos = GetRichEditCtrl().GetTextLength();
	GetRichEditCtrl().SetSel(0, nEndPos);
	GetRichEditCtrl().ReplaceSel("");
	GetRichEditCtrl().SetSel(-1, -1);
}

void CPTerminalView::OnUpdateEditClearAll(CCmdUI* pCmdUI) 
{
	int nEndPos = GetRichEditCtrl().GetTextLength();
	pCmdUI->Enable(nEndPos > 0);
}

/////////////////////////////////////////////////////////////////////////////
// 执行一条解释器命令,并将结果分割到字符串数组中
/////////////////////////////////////////////////////////////////////////////
BOOL CPTerminalView::SendInterpListCommand(CString strCmd, CString strSeparator, CStringArray& asRes) 
{
	if(m_pInterp == NULL)
	{
		return FALSE;
	}
	
	// 调用解释器执行命令
	m_pInterp->RunScriptCommand(strCmd);
	CString strRes = m_pInterp->GetResult();
	if(strRes.GetLength() > 0)
	{	// 获取执行结果
		int nResult = m_pInterp->GetErrorLine();
		if(nResult == 0)	// 结果为0表示没有错误
		{
			int nPos = 0;
			// 分割列表,元素之间以指定的分割符分开
			while((nPos = strRes.Find(strSeparator)) > 0)
			{
				asRes.Add(strRes.Left(nPos));
				strRes.Delete(0, nPos+1);
			}
			if(strRes.GetLength() > 0)
			{
				asRes.Add(strRes);	// 把命令添加到结果列表中
			}
		}else
		{	// 有错误,则提示错误信息,并退出
			asRes.Add(strRes);	// 把错误信息添加到结果列表中
			CString strErr;
			strErr.Format(_T("Error:%s"), strRes);
			AfxMessageBox(strErr, MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 执行一条解释器命令,并将结果返回
/////////////////////////////////////////////////////////////////////////////
BOOL CPTerminalView::SendInterpCommand(CString strCmd, CString& strRes) 
{
	if(m_pInterp == NULL)
	{
		return FALSE;
	}

	// 调用解释器执行命令
	m_pInterp->RunScriptCommand(strCmd);
	strRes = m_pInterp->GetResult();
	if(strRes.GetLength() > 0)
	{	// 获取执行结果
		int nResult = m_pInterp->GetErrorLine();
		if(nResult != 0)	// 结果不是0表示有错误
		{
			return FALSE;
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 在一个字符数组中查询是否存在特定字符串
/////////////////////////////////////////////////////////////////////////////
int CPTerminalView::FindStringInArray(CStringArray* pas, CString strFind) 
{
	if(!pas)
	{
		return -1;
	}

	for(int i=0; i<pas->GetSize(); i++)
	{	// 如果此命令属于平台扩展命令则不列在此分支中
		if((*pas)[i] == strFind)
			return i;
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// 可用命令列表
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::OnListCommand() 
{
	CStringArray asCommands;	// 命令数组

	if(SHELL_TCL == m_nWorkShell)
	{
		if(!SendInterpListCommand(_T("lsort -dictionary [info commands]"), " ", asCommands))
			return;
	}else
	if(SHELL_RUBY == m_nWorkShell)
	{
		if(!SendInterpListCommand(_T("methods"), " ", asCommands))
			return;
	}

	CStringArray asTclKeyWordEx;
	if(SHELL_TCL == m_nWorkShell)
	{
		m_pInterp->GetKeyWordList("platform", asTclKeyWordEx);
	}

	// 更新属性
	if(!theApp.m_pIPlatUI || !theApp.m_pIPlatUI->EnableUpdatePropertyGridContent())
	{
		return;
	}

	// 开始属性页更新
	theApp.m_pIPlatUI->BeginUpdatePropertyGridContent();

	CXTPPropertyGrid* pPropertyGrid = (CXTPPropertyGrid*)(theApp.m_pIPlatUI->GetPropertyGridWnd());
	if(pPropertyGrid->m_hWnd)
	{
		get_dll_resource();
		CXTPPropertyGridItem* pCategoryTmp = pPropertyGrid->AddCategory(IDS_TCLCMD_KERNEL_LIB);
		CXTPPropertyGridItem* pItem = NULL;
		CString strTmp;

		// 基本命令
		for(int i=0; i<asCommands.GetSize(); i++)
		{
			if(FindStringInArray(&asTclKeyWordEx, asCommands[i]) >= 0)
				continue;
			pItem = pCategoryTmp->AddChildItem(
				new CXTPPropertyGridItem(asCommands[i],	""));
			//pItem->SetReadOnly(TRUE);
		}

		pCategoryTmp = pPropertyGrid->AddCategory(IDS_TCLCMD_EXPAND);

		// 添加平台扩展命令
		for(int i=0; i<asTclKeyWordEx.GetSize(); i++)
		{
			pItem = pCategoryTmp->AddChildItem(
				new CXTPPropertyGridItem(asTclKeyWordEx[i],	""));
			//pItem->SetReadOnly(TRUE);
		}

		pCategoryTmp->Expand();
		reset_dll_resource();
	}

	// 结束属性页更新
	theApp.m_pIPlatUI->EndUpdatePropertyGridContent();

	BOOL bEnableListDlg = AfxGetApp()->GetProfileInt(REG_PTERMINAL, REG_ENABLE_LISTDLG, TRUE);
	if(bEnableListDlg)
	{
		CTOptArray aopt;	// 创建属性树元素数组
		TOPTITEM item;		// 属性树元素
		item.hItem		= NULL;
		item.iType		= TOPT_GROUP;
		CString strTemp;
		LOADRCSTRING( strTemp, IDS_TCLCMD_USE_TOTAL)
		item.lpszText.Format( strTemp, asCommands.GetSize());
		item.lpszEdit	= "";
		item.lpszList	= "";
		item.bCheck		= FALSE;
		item.iImage		= 0;
		item.iParent	= -1;
		aopt.Add(item);		// 添加根节点

		// 添加内核命令组
		item.iType		= TOPT_GROUP;
		item.iImage		= TOPTI_GROUP;
		LOADRCSTRING( strTemp, IDS_TCLCMD_KERNEL_LIB)
		item.lpszText	= strTemp;
		item.lpszEdit	= "";
		item.iParent	= 0;
		aopt.Add(item);
		int nSubRootIndex1 = aopt.GetSize()-1;

		// 添加内核和库命令
		int nCmdCount = 0;
		for(int i=0; i<asCommands.GetSize(); i++)
		{
			if(FindStringInArray(&asTclKeyWordEx, asCommands[i]) >= 0)
				continue;
			item.iType		= TOPT_EDIT;
			item.iImage		= TOPTI_INFO;
			item.lpszText	= asCommands[i];
			item.lpszEdit	= "";
			item.iParent	= nSubRootIndex1;
			aopt.Add(item);
			nCmdCount++;
		}

		LOADRCSTRING( strTemp, IDS_TCLCMD_KERNEL_LIB_TOTAL)
		aopt[nSubRootIndex1].lpszText.Format( strTemp, nCmdCount);

		// 添加平台扩展命令组
		item.iType		= TOPT_GROUP;
		item.iImage		= TOPTI_GROUP;
		LOADRCSTRING( strTemp, IDS_TCLCMD_EXPAND)
		item.lpszText.Format( strTemp, asTclKeyWordEx.GetSize());
		item.lpszEdit	= "";
		item.iParent	= 0;
		aopt.Add(item);
		int nSubRootIndex2 = aopt.GetSize()-1;

		// 添加平台扩展命令
		for(int i=0; i<asTclKeyWordEx.GetSize(); i++)
		{
			item.iType		= TOPT_EDIT;
			item.iImage		= TOPTI_INFO;
			item.lpszText	= asTclKeyWordEx[i];
			item.lpszEdit	= "";
			item.iParent	= nSubRootIndex2;
			aopt.Add(item);
		}

		// 添加标题和提示信息
		item.iType		= TOPT_MSG;
		LOADRCSTRING( strTemp, IDS_TCLCMD_LIST)
		item.lpszText	= strTemp;
		LOADRCSTRING( strTemp, IDS_TCLCMD_USE_INFO)
		item.lpszEdit.Format( strTemp, asCommands.GetSize());;
		item.iParent	= -1;
		aopt.Add(item);

		// 执行属性树
		if(theApp.m_ol.CommandPlat(OC_EXECTREEOPTION, 0, (LPARAM)(&aopt)) == IDOK)
		{
			int nSelect = (int)(aopt[0].hItem);	// 被选中的元素索引,放在元素0的hItem中
			if( (nSelect > 0) && nSelect <= (aopt.GetSize()-1) &&
				(nSelect != nSubRootIndex1) && (nSelect != nSubRootIndex2) )
			{
				SetTermFontColor(SHELL_COLOR_INPUT);
				CString strSelCmd;
				if(aopt[nSelect].lpszEdit.GetLength() > 0)	// 有输入参数表示是完整的命令,则执行
				{
					strSelCmd.Format("%s %s", aopt[nSelect].lpszText, aopt[nSelect].lpszEdit);
					Message(strSelCmd);	// 输出命令
					ProcessReturn(strSelCmd);	// 执行命令
				}else
				{	// 否则只显示,不执行
					Message(aopt[nSelect].lpszText);	// 输出命令
				}				
			}
		}
	}
}

void CPTerminalView::OnUpdateListCommand(CCmdUI* pCmdUI) 
{
	BOOL bEnable = ( (SHELL_TCL == m_nWorkShell) || (SHELL_RUBY == m_nWorkShell) || 
					((SHELL_RCADMIN == m_nWorkShell)&& m_bConnectRemote));
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 历史命令列表
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::OnListHistorycmd() 
{
	CTOptArray aopt;	// 创建属性树元素数组
	TOPTITEM item;		// 属性树元素
	item.hItem		= NULL;
	item.iType		= TOPT_GROUP;
	CString strTemp;
	LOADRCSTRING( strTemp, IDS_TCLCMD_HISTORY_TOTAL)
	item.lpszText.Format(strTemp, m_asCmdHistory.GetSize());
	item.lpszEdit	= "";
	item.lpszList	= "";
	item.bCheck		= FALSE;
	item.iImage		= 0;
	item.iParent	= -1;
	aopt.Add(item);		// 添加根节点

	// 添加所有命令
	for(int i=0; i<m_asCmdHistory.GetSize(); i++)
	{
		item.iType		= TOPT_ITEM;
		item.iImage		= TOPTI_INFO;
		item.lpszText	= (m_asCmdHistory)[i];
		item.iParent	= 0;
		aopt.Add(item);
	}

	// 添加标题和提示信息
	item.iType		= TOPT_MSG;
	LOADRCSTRING( strTemp, IDS_TCLCMD_HISTORY_LIST)
	item.lpszText	= strTemp;
	LOADRCSTRING( strTemp, IDS_TCLCMD_HISTORY_INFO)
	item.lpszEdit.Format( strTemp, m_asCmdHistory.GetSize());;
	item.iParent	= -1;
	aopt.Add(item);

	// 执行属性树
	if(theApp.m_ol.CommandPlat(OC_EXECTREEOPTION, 0, (LPARAM)(&aopt)) == IDOK)
	{
		int nSelect = (int)(aopt[0].hItem);	// 被选中的元素索引,放在元素0的hItem中
		if(nSelect > 0 && nSelect <= m_asCmdHistory.GetSize())
		{
			SetTermFontColor(SHELL_COLOR_INPUT);
			Message(aopt[nSelect].lpszText);	// 输出命令
			ProcessReturn(aopt[nSelect].lpszText);	// 执行命令					
		}
	}
}

void CPTerminalView::OnUpdateListHistorycmd(CCmdUI* pCmdUI) 
{
	BOOL bEnable =  ((SHELL_TCL == m_nWorkShell) || (SHELL_PYTHON == m_nWorkShell)
					 || (SHELL_LUA == m_nWorkShell) || (SHELL_RUBY == m_nWorkShell)
					 || (SHELL_PERL == m_nWorkShell)
					 || (SHELL_RCADMIN == m_nWorkShell));
	bEnable &= (m_asCmdHistory.GetSize() > 0);
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 已加载库列表
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::OnListLoadlib() 
{
	CStringArray asLibs;	// 命令数组
	if(!SendInterpListCommand(_T("info loaded"), " ", asLibs))
		return;

	// 更新属性
	if(!theApp.m_pIPlatUI || !theApp.m_pIPlatUI->EnableUpdatePropertyGridContent())
	{
		return;
	}

	// 开始属性页更新
	theApp.m_pIPlatUI->BeginUpdatePropertyGridContent();

	CXTPPropertyGrid* pPropertyGrid = (CXTPPropertyGrid*)(theApp.m_pIPlatUI->GetPropertyGridWnd());
	if(pPropertyGrid->m_hWnd)
	{
		get_dll_resource();
		CXTPPropertyGridItem* pCategoryTmp = pPropertyGrid->AddCategory(IDS_TCLDLL_LIST);
		CXTPPropertyGridItem* pItem = NULL;
		CString strTmp;

		// 基本命令
		for(int i=0; i<asLibs.GetSize(); i++)
		{
			pItem = pCategoryTmp->AddChildItem(
				new CXTPPropertyGridItem(asLibs[i],	""));
		}

		pCategoryTmp->Expand();
		reset_dll_resource();
	}

	// 结束属性页更新
	theApp.m_pIPlatUI->EndUpdatePropertyGridContent();

	BOOL bEnableListDlg = AfxGetApp()->GetProfileInt(REG_PTERMINAL, REG_ENABLE_LISTDLG, TRUE);
	if(bEnableListDlg)
	{
		CTOptArray aopt;	// 创建属性树元素数组
		TOPTITEM item;		// 属性树元素
		item.hItem		= NULL;
		item.iType		= TOPT_GROUP;
		CString strTemp;
		LOADRCSTRING( strTemp, IDS_TCLDLL_TOTAL)
		item.lpszText.Format( strTemp, asLibs.GetSize());
		item.lpszEdit	= "";
		item.lpszList	= "";
		item.bCheck		= FALSE;
		item.iImage		= 0;
		item.iParent	= -1;
		aopt.Add(item);		// 添加根节点

		// 添加所有命令
		for(int i=0; i<asLibs.GetSize(); i++)
		{
			item.iType		= TOPT_ITEM;
			item.iImage		= TOPTI_INFO;
			item.lpszText	= asLibs[i];
			item.lpszEdit	= "";
			item.iParent	= 0;
			aopt.Add(item);
		}

		// 添加标题和提示信息
		item.iType		= TOPT_MSG;
		LOADRCSTRING( strTemp, IDS_TCLDLL_LIST)
		item.lpszText	= strTemp;
		LOADRCSTRING( strTemp, IDS_TCLDLL_INFO)
		item.lpszEdit.Format(strTemp, asLibs.GetSize());;
		item.iParent	= -1;
		aopt.Add(item);

		// 执行属性树
		if(theApp.m_ol.CommandPlat(OC_EXECTREEOPTION, 0, (LPARAM)(&aopt)) == IDOK)
		{
		}
	}
}

void CPTerminalView::OnUpdateListLoadlib(CCmdUI* pCmdUI) 
{
	BOOL bEnable = ( (SHELL_TCL == m_nWorkShell) || ((SHELL_RCADMIN == m_nWorkShell)&& m_bConnectRemote));
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 可加载库列表
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::OnListKnownlib() 
{

}

void CPTerminalView::OnUpdateListKnownlib(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// 过程列表
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::OnListProcs() 
{
	CStringArray asProcs;	// 过程数组
	if(!SendInterpListCommand(_T("lsort -dictionary [info procs]"), " ", asProcs))
		return;

	// 更新属性
	if(!theApp.m_pIPlatUI || !theApp.m_pIPlatUI->EnableUpdatePropertyGridContent())
	{
		return;
	}

	// 开始属性页更新
	theApp.m_pIPlatUI->BeginUpdatePropertyGridContent();

	CXTPPropertyGrid* pPropertyGrid = (CXTPPropertyGrid*)(theApp.m_pIPlatUI->GetPropertyGridWnd());
	if(pPropertyGrid->m_hWnd)
	{
		get_dll_resource();
		CXTPPropertyGridItem* pCategoryTmp = pPropertyGrid->AddCategory(IDS_TCLPROC_LIST);
		CXTPPropertyGridItem* pItem = NULL;
		CString strTmp;

		// 基本命令
		for(int i=0; i<asProcs.GetSize(); i++)
		{
			pItem = pCategoryTmp->AddChildItem(
				new CXTPPropertyGridItem(asProcs[i], ""));
		}

		pCategoryTmp->Expand();
		reset_dll_resource();
	}

	// 结束属性页更新
	theApp.m_pIPlatUI->EndUpdatePropertyGridContent();

	BOOL bEnableListDlg = AfxGetApp()->GetProfileInt(REG_PTERMINAL, REG_ENABLE_LISTDLG, TRUE);
	if(bEnableListDlg)
	{
		CTOptArray aopt;	// 创建属性树元素数组
		TOPTITEM item;		// 属性树元素
		item.hItem		= NULL;
		item.iType		= TOPT_GROUP;
		CString strTemp;
		LOADRCSTRING( strTemp, IDS_TCLPROC_TOTAL )
		item.lpszText.Format( strTemp, asProcs.GetSize());
		item.lpszEdit	= "";
		item.lpszList	= "";
		item.bCheck		= FALSE;
		item.iImage		= 0;
		item.iParent	= -1;
		aopt.Add(item);		// 添加根节点

		// 添加所有过程
		for(int i=0; i<asProcs.GetSize(); i++)
		{
			item.iType		= TOPT_EDIT;
			item.iImage		= TOPTI_INFO;
			item.lpszText	= asProcs[i];
			item.lpszEdit	= "";
			item.iParent	= 0;
			aopt.Add(item);
		}

		// 添加标题和提示信息
		item.iType		= TOPT_MSG;
		LOADRCSTRING( strTemp, IDS_TCLPROC_LIST )
		item.lpszText	= strTemp;
		LOADRCSTRING( strTemp, IDS_TCLPROC_INFO )
		item.lpszEdit.Format( strTemp, asProcs.GetSize());;
		item.iParent	= -1;
		aopt.Add(item);

		// 执行属性树
		if(theApp.m_ol.CommandPlat(OC_EXECTREEOPTION, 0, (LPARAM)(&aopt)) == IDOK)
		{
			int nSelect = (int)(aopt[0].hItem);	// 被选中的元素索引,放在元素0的hItem中
			if(nSelect > 0 && nSelect <= asProcs.GetSize())
			{
				SetTermFontColor(SHELL_COLOR_INPUT);
				CString strSelCmd;
				if(aopt[nSelect].lpszEdit.GetLength() > 0)	// 有输入参数表示是完整的命令,则执行
				{
					strSelCmd.Format("%s %s", aopt[nSelect].lpszText, aopt[nSelect].lpszEdit);
					Message(strSelCmd);	// 输出命令
					ProcessReturn(strSelCmd);	// 执行命令
				}else
				{	// 否则只显示,不执行
					Message(aopt[nSelect].lpszText);	// 输出命令
				}				
			}
		}
	}
}

void CPTerminalView::OnUpdateListProcs(CCmdUI* pCmdUI) 
{
	BOOL bEnable = ( (SHELL_TCL == m_nWorkShell) || ((SHELL_RCADMIN == m_nWorkShell)&& m_bConnectRemote));
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 全局变量列表
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::OnListGlobals() 
{
	CStringArray asGlobals;	// 全局变量数组(经过排序)

	if((SHELL_TCL == m_nWorkShell)/* || ((SHELL_RCADMIN == m_nWorkShell) && IsTclInterp(m_nInterpID))*/)
	{
		if(!SendInterpListCommand(_T("lsort -dictionary [info globals]"), " ", asGlobals))
			return;
	}else
	if((SHELL_PYTHON == m_nWorkShell)/* || ((SHELL_RCADMIN == m_nWorkShell) && !IsTclInterp(m_nInterpID))*/)
	{
		if(!SendInterpListCommand(_T("for _platform.listglobals in globals().keys(): \
										_platform.write(0, '%s ' % _platform.listglobals, 18, 0)"), " ", asGlobals))
			return;
	}else
	if(SHELL_LUA == m_nWorkShell)
	{
		//if(!SendInterpListCommand(_T("for k,v in pairs(_G) do put_result(k,\" \",v) end"), " ", asGlobals))
		if(!SendInterpListCommand(_T("for k,v in pairs(_G) do put_result(k,\" \") end"), " ", asGlobals))
			return;
	}else
	if(SHELL_RUBY == m_nWorkShell)
	{
		if(!SendInterpListCommand(_T("global_variables"), " ", asGlobals))
			return;
	}else
	if(SHELL_PERL == m_nWorkShell)
	{
		if(!SendInterpListCommand(_T("foreach $__platform_symname (sort keys \%main::) { local *sym = $main::{$__platform_symname}; main::platform_print('toresult', \"\\$$__platform_symname \") if defined $sym; main::platform_print('toresult', \"\\@$__platform_symname \") if @sym; main::platform_print('toresult', \"\\\%$__platform_symname \") if \%sym;}"), " ", asGlobals))
			return;
	}

	// 更新属性
	if(!theApp.m_pIPlatUI || !theApp.m_pIPlatUI->EnableUpdatePropertyGridContent())
	{
		return;
	}

	// 开始属性页更新
	theApp.m_pIPlatUI->BeginUpdatePropertyGridContent();

	CXTPPropertyGrid* pPropertyGrid = (CXTPPropertyGrid*)(theApp.m_pIPlatUI->GetPropertyGridWnd());
	if(pPropertyGrid->m_hWnd)
	{
		get_dll_resource();
		CXTPPropertyGridItem* pCategoryTmp = pPropertyGrid->AddCategory(IDS_GLOBEVAR_LIST);
		CXTPPropertyGridItem* pItem = NULL;
		CString strTmp;

		// 添加所有全局变量
		if(SHELL_TCL == m_nWorkShell)
		{
			for(int i=0; i<asGlobals.GetSize(); i++)
			{
				CString strVar;	// 变量
				CString strCmd;	// 命令
				strCmd.Format(_T("set %s"), asGlobals[i]);
				BOOL bRes = SendInterpCommand(strCmd, strVar);
				if(bRes)	// 是普通变量
				{
					pItem = pCategoryTmp->AddChildItem(
						new CXTPPropertyGridItem(asGlobals[i], strVar));
					pItem->SetDescription(strVar);
				}else
				if(strVar.Find(_T("variable is array")) > 0)	// 是一个数组变量
				{
					strCmd.Format(_T("array names %s"), asGlobals[i]);	// 列数组元素名
					CStringArray asArrayNames;
					bRes = SendInterpListCommand(strCmd, " ", asArrayNames);	// 发送命令
					if(bRes && asArrayNames.GetSize() > 0)	// 成功,且元素个数大于0
					{
						pItem = pCategoryTmp->AddChildItem(
							new CXTPPropertyGridItem(asGlobals[i], "[...]"));

						for(int j=0; j<asArrayNames.GetSize(); j++)
						{	// 添加数组的每个元素
							strCmd.Format(_T("set %s(%s)"), asGlobals[i], asArrayNames[j]);	// 获取数组元素值
							SendInterpCommand(strCmd, strVar);

							CXTPPropertyGridItem* pSubItem = pItem->AddChildItem(
								new CXTPPropertyGridItem(asArrayNames[j], strVar));
							pSubItem->SetDescription(strVar);
						}

						continue;
					}else
					if(!bRes)	// 失败
					{
						pItem = pCategoryTmp->AddChildItem(
							new CXTPPropertyGridItem(asGlobals[i], asArrayNames[0]));
						pItem->SetDescription(asArrayNames[0]);
					}else	// 成功,但数组为空
					{
						pItem = pCategoryTmp->AddChildItem(
							new CXTPPropertyGridItem(asGlobals[i], "[Null Array]"));
						pItem->SetDescription("[Null Array]");
					}
				}
			}
		}

		pCategoryTmp->Expand();
		reset_dll_resource();
	}

	// 结束属性页更新
	theApp.m_pIPlatUI->EndUpdatePropertyGridContent();

	BOOL bEnableListDlg = AfxGetApp()->GetProfileInt(REG_PTERMINAL, REG_ENABLE_LISTDLG, TRUE);
	if(bEnableListDlg)
	{
		CTOptArray aopt;	// 创建属性树元素数组
		TOPTITEM item;		// 属性树元素
		item.hItem		= NULL;
		item.iType		= TOPT_GROUP;
		CString strTemp;
		LOADRCSTRING( strTemp, IDS_GLOBEVAR_TOTAL)
		item.lpszText.Format( strTemp, asGlobals.GetSize());
		item.lpszEdit	= "";
		item.lpszList	= "";
		item.bCheck		= FALSE;
		item.iImage		= TOPTI_FOLDER;
		item.iParent	= -1;
		aopt.Add(item);		// 添加根节点

		// 添加所有全局变量
		if(SHELL_TCL == m_nWorkShell)
		// || ((SHELL_RCADMIN == pDoc->m_nWorkShell) && IsTclInterp(pDoc->m_nInterpID))
		{
			for(int i=0; i<asGlobals.GetSize(); i++)
			{
				item.iType		= TOPT_EDIT;
				item.iImage		= TOPTI_INFO;
				item.lpszText	= asGlobals[i];
				item.iParent	= 0;

				CString strVar;	// 变量
				CString strCmd;	// 命令
				strCmd.Format(_T("set %s"), asGlobals[i]);
				BOOL bRes = SendInterpCommand(strCmd, strVar);
				if(bRes)	// 是普通变量
				{
					item.lpszEdit	= strVar;
				}else
				if(strVar.Find(_T("variable is array")) > 0)	// 是一个数组变量
				{
					strCmd.Format(_T("array names %s"), asGlobals[i]);	// 列数组元素名
					CStringArray asArrayNames;
					bRes = SendInterpListCommand(strCmd, " ", asArrayNames);	// 发送命令
					if(bRes && asArrayNames.GetSize() > 0)	// 成功,且元素个数大于0
					{
						item.iType		= TOPT_GROUP;
						item.iImage		= TOPTI_GROUP;
						aopt.Add(item);	// 添加数组根节点

						int nArrRootIndex = aopt.GetSize()-1;
						for(int j=0; j<asArrayNames.GetSize(); j++)
						{	// 添加数组的每个元素
							TOPTITEM item1;		// 属性树元素
							item1.hItem		= NULL;
							item1.iType		= TOPT_EDIT;
							item1.iImage	= TOPTI_INFO;
							item1.lpszText	= asArrayNames[j];
							item1.lpszList	= "";
							item1.bCheck	= FALSE;
							item1.iParent	= nArrRootIndex;

							strCmd.Format(_T("set %s(%s)"), asGlobals[i], asArrayNames[j]);	// 获取数组元素值
							SendInterpCommand(strCmd, strVar);
							item1.lpszEdit	= strVar;
							
							aopt.Add(item1);	// 添加数组元素节点
						}

						continue;
					}else
					if(!bRes)	// 失败
					{
						item.iImage		= TOPTI_INFO;
						item.lpszEdit	= asArrayNames[0];
					}else	// 成功,但数组为空
					{
						item.iImage		= TOPTI_GROUP;
						item.lpszEdit	= "[Null Array]";
					}
				}
				aopt.Add(item);
			}
		}else
		if(SHELL_PYTHON == m_nWorkShell)
		{
			CStringArray asGlobalValues;	// 全局变量值数组
			if(!SendInterpListCommand(_T("for _platform.listglobals in globals().values(): \
											_platform.write(0, '%s%c' % (_platform.listglobals, 0x01), 18, 0)"), (char)0x01, asGlobalValues))
				return;
			for(int i=0; i<asGlobals.GetSize(); i++)
			{
				item.iType		= TOPT_EDIT;
				item.iImage		= TOPTI_INFO;
				item.lpszText	= asGlobals[i];
				item.iParent	= 0;
				if(asGlobalValues.GetSize() > i)
					item.lpszEdit	= asGlobalValues[i];
				else
					item.lpszEdit	= "";
				aopt.Add(item);
			}
		}else
		if(SHELL_LUA == m_nWorkShell)
		{
			for(int i=0; i<asGlobals.GetSize(); i++)
			{
				item.iType		= TOPT_EDIT;
				item.iImage		= TOPTI_INFO;
				item.lpszText	= asGlobals[i];
				item.iParent	= 0;

				CString strVar;	// 变量
				CString strCmd;	// 命令
				strCmd.Format(_T("put_result(%s)"), asGlobals[i]);
				BOOL bRes = SendInterpCommand(strCmd, strVar);
				if(bRes)	// 是普通变量
				{
					item.lpszEdit	= strVar;
				}
				aopt.Add(item);
			}
		}else
		if(SHELL_RUBY == m_nWorkShell)
		{
			for(int i=0; i<asGlobals.GetSize(); i++)
			{
				item.iType		= TOPT_EDIT;
				item.iImage		= TOPTI_INFO;
				item.lpszText	= asGlobals[i];
				item.iParent	= 0;

				CString strVar;	// 变量
				CString strCmd;	// 命令
				strCmd.Format(_T("%s"), asGlobals[i]);
				BOOL bRes = SendInterpCommand(strCmd, strVar);
				if(bRes)	// 是普通变量
				{
					item.lpszEdit	= strVar;
				}
				aopt.Add(item);
			}
		}else
		if(SHELL_PERL == m_nWorkShell)
		{
			for(int i=0; i<asGlobals.GetSize(); i++)
			{
				// 去掉%main变量
				if(asGlobals[i] == "%main::")
				{
					continue;
				}
				// 以$_<开头的变量不用显示
				if(asGlobals[i].Find("$_<") == 0)
				{
					continue;
				}
				// 去掉平台内部变量
				if(asGlobals[i].Find("$__platform_") == 0)
				{
					continue;
				}
				// 去掉几个包含特殊字符的变量名
				if((asGlobals[i].Find("E_TRIE_MAXBUF") != -1) || (asGlobals[i].Find("ARNING_BITS") != -1))
				{
					continue;
				}

				item.iType		= TOPT_EDIT;
				item.iImage		= TOPTI_INFO;
				item.lpszText	= asGlobals[i];
				item.iParent	= 0;

				CString strVar;	// 变量
				CString strCmd;	// 命令
				strCmd.Format(_T("main::platform_print('toresult', %s);"), asGlobals[i]);
				BOOL bRes = SendInterpCommand(strCmd, strVar);
				if(bRes)	// 是普通变量
				{
					item.lpszEdit	= strVar;
				}
				aopt.Add(item);
			}
		}

		// 添加标题和提示信息
		item.iType		= TOPT_MSG;
		LOADRCSTRING( strTemp, IDS_GLOBEVAR_LIST)
		item.lpszText	= strTemp;
		LOADRCSTRING( strTemp, IDS_GLOBEVAR_INFO )
		item.lpszEdit.Format( strTemp, asGlobals.GetSize());
		item.iParent	= -1;
		aopt.Add(item);

		// 执行属性树
		if(theApp.m_ol.CommandPlat(OC_EXECTREEOPTION, 0, (LPARAM)(&aopt)) == IDOK)
		{
		}
	}
}

void CPTerminalView::OnUpdateListGlobals(CCmdUI* pCmdUI) 
{
	BOOL bEnable = ( (SHELL_TCL == m_nWorkShell) || (SHELL_PYTHON == m_nWorkShell) 
				|| (SHELL_LUA == m_nWorkShell) || (SHELL_RUBY == m_nWorkShell)
				|| (SHELL_PERL == m_nWorkShell)
				|| ((SHELL_RCADMIN == m_nWorkShell)&& m_bConnectRemote));
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 局部变量列表
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::OnListLocalvars() 
{
	CStringArray asLocals;	// 局部变量数组

	if((SHELL_TCL == m_nWorkShell)/* || ((SHELL_RCADMIN == m_nWorkShell) && IsTclInterp(m_nInterpID))*/)
	{
		if(!SendInterpListCommand(_T("lsort -dictionary [info local]"), " ", asLocals))
			return;
	}else
	if((SHELL_PYTHON == m_nWorkShell)/* || ((SHELL_RCADMIN == m_nWorkShell) && !IsTclInterp(m_nInterpID))*/)
	{
		if(!SendInterpListCommand(_T("for _platform.listlocals in locals().keys(): \
										_platform.write(0, '%s ' % _platform.listlocals, 18, 0)"), " ", asLocals))
			return;
	}else
	if(SHELL_LUA == m_nWorkShell)
	{
		//if(!SendInterpListCommand(_T("local platform_temp_index = 1;while true do local name, value = debug.getlocal(1, platform_temp_index);if not name then break end;put_result(name, \" \");platform_temp_index = platform_temp_index + 1;end"), " ", asLocals))
		//	return;
	}else
	if(SHELL_RUBY == m_nWorkShell)
	{
		if(!SendInterpListCommand(_T("local_variables"), " ", asLocals))
			return;
	}

	// 开始属性页更新
	theApp.m_pIPlatUI->BeginUpdatePropertyGridContent();

	CXTPPropertyGrid* pPropertyGrid = (CXTPPropertyGrid*)(theApp.m_pIPlatUI->GetPropertyGridWnd());
	if(pPropertyGrid->m_hWnd)
	{
		get_dll_resource();
		CXTPPropertyGridItem* pCategoryTmp = pPropertyGrid->AddCategory(IDS_LOCAL_LIST);
		CXTPPropertyGridItem* pItem = NULL;
		CString strTmp;

		// 添加所有全局变量
		if(SHELL_TCL == m_nWorkShell)
		{
			for(int i=0; i<asLocals.GetSize(); i++)
			{
				CString strVar;	// 变量
				CString strCmd;	// 命令
				strCmd.Format(_T("set %s"), asLocals[i]);
				BOOL bRes = SendInterpCommand(strCmd, strVar);
				if(bRes)	// 是普通变量
				{
					pItem = pCategoryTmp->AddChildItem(
						new CXTPPropertyGridItem(asLocals[i], strVar));
					pItem->SetDescription(strVar);
				}else
				if(strVar.Find(_T("variable is array")) > 0)	// 是一个数组变量
				{
					strCmd.Format(_T("array names %s"), asLocals[i]);	// 列数组元素名
					CStringArray asArrayNames;
					bRes = SendInterpListCommand(strCmd, " ", asArrayNames);	// 发送命令
					if(bRes && asArrayNames.GetSize() > 0)	// 成功,且元素个数大于0
					{
						pItem = pCategoryTmp->AddChildItem(
							new CXTPPropertyGridItem(asLocals[i], "[...]"));

						for(int j=0; j<asArrayNames.GetSize(); j++)
						{	// 添加数组的每个元素
							strCmd.Format(_T("set %s(%s)"), asLocals[i], asArrayNames[j]);	// 获取数组元素值
							SendInterpCommand(strCmd, strVar);

							CXTPPropertyGridItem* pSubItem = pItem->AddChildItem(
								new CXTPPropertyGridItem(asArrayNames[j], strVar));
							pSubItem->SetDescription(strVar);
						}

						continue;
					}else
					if(!bRes)	// 失败
					{
						pItem = pCategoryTmp->AddChildItem(
							new CXTPPropertyGridItem(asLocals[i], asArrayNames[0]));
						pItem->SetDescription(asArrayNames[0]);
					}else	// 成功,但数组为空
					{
						pItem = pCategoryTmp->AddChildItem(
							new CXTPPropertyGridItem(asLocals[i], "[Null Array]"));
						pItem->SetDescription("[Null Array]");
					}
				}
			}
		}

		pCategoryTmp->Expand();
		reset_dll_resource();
	}

	// 结束属性页更新
	theApp.m_pIPlatUI->EndUpdatePropertyGridContent();

	BOOL bEnableListDlg = AfxGetApp()->GetProfileInt(REG_PTERMINAL, REG_ENABLE_LISTDLG, TRUE);
	if(bEnableListDlg)
	{
		CTOptArray aopt;	// 创建属性树元素数组
		TOPTITEM item;		// 属性树元素
		item.hItem		= NULL;
		item.iType		= TOPT_GROUP;
		CString strTemp;
		LOADRCSTRING( strTemp, IDS_LOCAL_TOTAL)
		item.lpszText.Format(strTemp, asLocals.GetSize());
		item.lpszEdit	= "";
		item.lpszList	= "";
		item.bCheck		= FALSE;
		item.iImage		= TOPTI_FOLDER;
		item.iParent	= -1;
		aopt.Add(item);		// 添加根节点

		// 添加所有局部变量
		if(SHELL_TCL == m_nWorkShell)
		// || ((SHELL_RCADMIN == pDoc->m_nWorkShell) && IsTclInterp(pDoc->m_nInterpID))
		{
			for(int i=0; i<asLocals.GetSize(); i++)
			{
				item.iType		= TOPT_EDIT;
				item.iImage		= TOPTI_INFO;
				item.lpszText	= asLocals[i];
				item.iParent	= 0;

				CString strVar;	// 变量
				CString strCmd;	// 命令
				strCmd.Format(_T("set %s"), asLocals[i]);
				BOOL bRes = SendInterpCommand(strCmd, strVar);
				if(bRes)	// 是普通变量
				{
					item.lpszEdit	= strVar;
				}else
				if(strVar.Find(_T("variable is array")) > 0)	// 是一个数组变量
				{
					strCmd.Format(_T("array names %s"), asLocals[i]);	// 列数组元素名
					CStringArray asArrayNames;
					bRes = SendInterpListCommand(strCmd, " ", asArrayNames);	// 发送命令
					if(bRes && asArrayNames.GetSize() > 0)	// 成功,且元素个数大于0
					{
						item.iType		= TOPT_GROUP;
						item.iImage		= TOPTI_GROUP;
						aopt.Add(item);	// 添加数组根节点

						int nArrRootIndex = aopt.GetSize()-1;
						for(int j=0; j<asArrayNames.GetSize(); j++)
						{	// 添加数组的每个元素
							TOPTITEM item1;		// 属性树元素
							item1.hItem		= NULL;
							item1.iType		= TOPT_EDIT;
							item1.iImage	= TOPTI_INFO;
							item1.lpszText	= asArrayNames[j];
							item1.lpszList	= "";
							item1.bCheck	= FALSE;
							item1.iParent	= nArrRootIndex;

							strCmd.Format(_T("set %s(%s)"), asLocals[i], asArrayNames[j]);	// 获取数组元素值
							SendInterpCommand(strCmd, strVar);
							item1.lpszEdit	= strVar;
							
							aopt.Add(item1);	// 添加数组元素节点
						}

						continue;
					}else
					if(!bRes)	// 失败
					{
						item.iImage		= TOPTI_INFO;
						item.lpszEdit	= asArrayNames[0];
					}else	// 成功,但数组为空
					{
						item.iImage		= TOPTI_GROUP;
						item.lpszEdit	= "[Null Array]";
					}
				}
				aopt.Add(item);
			}

			// 获取函数名
			CString strVar;
			BOOL bRes = SendInterpCommand(_T("info level"), strVar);
			if(bRes && (strVar != "0"))
			{
				strVar = _T("info level ") + strVar;
				CStringArray asArgvs;
				bRes = SendInterpListCommand(strVar, " ", asArgvs);
				if(bRes && (asArgvs.GetSize()>0))	// 是普通变量
				{
					LOADRCSTRING( strTemp, IDS_LOCAL_IN_PROC)
					item.lpszEdit.Format(strTemp, asArgvs[0], asLocals.GetSize());
				}
			}else
			{
				LOADRCSTRING( strTemp, IDS_LOCAL_OUT_PROC)
				item.lpszEdit.Format(strTemp, asLocals.GetSize());
			}
		}else
		if(SHELL_PYTHON == m_nWorkShell)
		{
			CStringArray asLocalValues;	// 全局变量值数组
			if(!SendInterpListCommand(_T("for _platform.listlocals in locals().values(): \
											_platform.write(0, '%s%c' % (_platform.listlocals, 0x01), 18, 0)"), (char)0x01, asLocalValues))
				return;
			for(int i=0; i<asLocals.GetSize(); i++)
			{
				item.iType		= TOPT_EDIT;
				item.iImage		= TOPTI_INFO;
				item.lpszText	= asLocals[i];
				item.iParent	= 0;
				if(asLocalValues.GetSize() > i)
					item.lpszEdit	= asLocalValues[i];
				else
					item.lpszEdit	= "";
				aopt.Add(item);
			}
		}else
		if(SHELL_RUBY == m_nWorkShell)
		{
			for(int i=0; i<asLocals.GetSize(); i++)
			{
				item.iType		= TOPT_EDIT;
				item.iImage		= TOPTI_INFO;
				item.lpszText	= asLocals[i];
				item.iParent	= 0;
				CString strVar;	// 变量
				CString strCmd;	// 命令
				strCmd.Format(_T("%s"), asLocals[i]);
				BOOL bRes = SendInterpCommand(strCmd, strVar);
				if(bRes)	// 是普通变量
				{
					item.lpszEdit	= strVar;
				}
				aopt.Add(item);
			}
		}

		// 添加标题和提示信息
		item.iType		= TOPT_MSG;
		LOADRCSTRING( strTemp, IDS_LOCAL_LIST)
		item.lpszText	= strTemp;
		item.iParent	= -1;
		aopt.Add(item);

		// 执行属性树
		if(theApp.m_ol.CommandPlat(OC_EXECTREEOPTION, 0, (LPARAM)(&aopt)) == IDOK)
		{
		}
	}
}

void CPTerminalView::OnUpdateListLocalvars(CCmdUI* pCmdUI) 
{
	BOOL bEnable = ( (SHELL_TCL == m_nWorkShell) || (SHELL_PYTHON == m_nWorkShell) ||
					/*(SHELL_LUA == m_nWorkShell) ||*/ (SHELL_RUBY == m_nWorkShell) ||
					((SHELL_RCADMIN == m_nWorkShell)&& m_bConnectRemote));
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// iTcl类和对象列表
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::OnListItclobject() 
{
	CStringArray asClass;	// 类数组
	if(!SendInterpListCommand(_T("itcl::find classes"), " ", asClass))
		return;

	// 开始属性页更新
	theApp.m_pIPlatUI->BeginUpdatePropertyGridContent();

	CXTPPropertyGrid* pPropertyGrid = (CXTPPropertyGrid*)(theApp.m_pIPlatUI->GetPropertyGridWnd());
	if(pPropertyGrid->m_hWnd)
	{
		get_dll_resource();
		CXTPPropertyGridItem* pCategoryTmp = pPropertyGrid->AddCategory(IDS_ITCL_LIST);
		CXTPPropertyGridItem* pItem = NULL;
		CString strTmp;

		// 添加所有全局变量
		for(int i=0; i<asClass.GetSize(); i++)
		{
			pItem = pCategoryTmp->AddChildItem(
				new CXTPPropertyGridItem(asClass[i], "[...]"));

			CString strCmd;	// 命令
			CStringArray asObjects;	// 对象数组
			strCmd.Format(_T("itcl::find objects -class %s"), asClass[i]);
			BOOL bRes = SendInterpListCommand(strCmd, " ", asObjects);
			if(bRes)	// 是普通变量
			{
				if(bRes && asObjects.GetSize() > 0)	// 成功,且元素个数大于0
				{
					for(int j=0; j<asObjects.GetSize(); j++)
					{	// 添加数组的每个元素
						CXTPPropertyGridItem* pSubItem = pItem->AddChildItem(
							new CXTPPropertyGridItem(asObjects[j], ""));
					}
					continue;
				}
			}
		}

		pCategoryTmp->Expand();
		reset_dll_resource();
	}

	// 结束属性页更新
	theApp.m_pIPlatUI->EndUpdatePropertyGridContent();

	BOOL bEnableListDlg = AfxGetApp()->GetProfileInt(REG_PTERMINAL, REG_ENABLE_LISTDLG, TRUE);
	if(bEnableListDlg)
	{
		CTOptArray aopt;	// 创建属性树元素数组
		TOPTITEM item;		// 属性树元素
		item.hItem		= NULL;
		item.iType		= TOPT_GROUP;
		CString strTemp;
		LOADRCSTRING( strTemp, IDS_ITCL_TOTAL)
		item.lpszText.Format(strTemp, asClass.GetSize());
		item.lpszEdit	= "";
		item.lpszList	= "";
		item.bCheck		= FALSE;
		item.iImage		= TOPTI_FOLDER;
		item.iParent	= -1;
		aopt.Add(item);		// 添加根节点

		// 添加所有类
		for(int i=0; i<asClass.GetSize(); i++)
		{
			item.iType		= TOPT_ITEM;
			item.iImage		= TOPTI_GROUP;
			item.lpszText	= asClass[i];
			item.iParent	= 0;

			CString strCmd;	// 命令
			CStringArray asObjects;	// 对象数组
			strCmd.Format(_T("itcl::find objects -class %s"), asClass[i]);
			BOOL bRes = SendInterpListCommand(strCmd, " ", asObjects);
			if(bRes)	// 是普通变量
			{
				if(bRes && asObjects.GetSize() > 0)	// 成功,且元素个数大于0
				{
					item.iType		= TOPT_GROUP;
					
					aopt.Add(item);	// 添加数组根节点

					int nArrRootIndex = aopt.GetSize()-1;
					for(int j=0; j<asObjects.GetSize(); j++)
					{	// 添加数组的每个元素
						TOPTITEM item1;		// 属性树元素
						item1.hItem		= NULL;
						item1.iType		= TOPT_ITEM;
						item1.iImage	= TOPTI_INFO;
						item1.lpszText	= asObjects[j];
						item1.lpszList	= "";
						item1.bCheck	= FALSE;
						item1.iParent	= nArrRootIndex;
						item1.lpszEdit	= "";
						
						aopt.Add(item1);	// 添加数组元素节点
					}

					continue;
				}
			}
			aopt.Add(item);
		}

		// 添加标题和提示信息
		item.iType		= TOPT_MSG;
		LOADRCSTRING( strTemp, IDS_ITCL_LIST)
		item.lpszText	= strTemp;
		LOADRCSTRING( strTemp, IDS_ITCL_INFO)
		item.lpszEdit.Format(strTemp, asClass.GetSize());
		item.iParent	= -1;
		aopt.Add(item);

		// 执行属性树
		if(theApp.m_ol.CommandPlat(OC_EXECTREEOPTION, 0, (LPARAM)(&aopt)) == IDOK)
		{
		}
	}
}

void CPTerminalView::OnUpdateListItclobject(CCmdUI* pCmdUI) 
{
	BOOL bEnable = ( (SHELL_TCL == m_nWorkShell) || ((SHELL_RCADMIN == m_nWorkShell)&& m_bConnectRemote));
	pCmdUI->Enable(bEnable);
}

//////////////////////////////////////////////////////////////////
// 串口接收处理函数
//////////////////////////////////////////////////////////////////
LRESULT CPTerminalView::OnSerialRead(WPARAM ch, LPARAM pSock) 
{
	//CClientSocket* pSocket = (CClientSocket*)pSock;
	//if(pSocket)
	//	pSocket->SerialDataToSocket((unsigned char)ch);

	CString strTmp = (char)ch;
	SetTermFontColor(SHELL_COLOR_RESOK);
	if(ch == 13)
		Message("\r\n");
	else if(ch != 10)
		Message(strTmp);
	return 0;
}

void CPTerminalView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if(!m_bHaveDocument)
	{
		return;
	}

	if (bActivate)
	{
		// 在属性窗口显示文件信息
		if(pActivateView != pDeactiveView)
		{
			GetDocument()->RefreshDocProperty();
		}
	}

	if (bActivate)
	{
		get_dll_resource();
		CMenu* pMenuTerm = new CMenu();
		pMenuTerm->LoadMenu(IDR_TERM_TCL);
		reset_dll_resource();
		theApp.m_pIPlatUI->AddExtMenu(pMenuTerm);
	}else
	{
		get_dll_resource();
		CMenu* pMenuTerm = new CMenu();
		pMenuTerm->LoadMenu(IDR_TERM_TCL);
		reset_dll_resource();
		theApp.m_pIPlatUI->DelExtMenu(pMenuTerm);
	}

	CRichEditView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/////////////////////////////////////////////////////////////////////////////
// Telnet

/////////////////////////////////////////////////////////////////////////////
// 处理Telnet信息
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::ProcessMessage(CClientSocket *cSocket)
{
	int nBytes = cSocket->Receive(m_bBuf.GetData(),m_bBuf.GetSize() );
	if(nBytes != SOCKET_ERROR)
	{
		int ndx = 0;
		while(GetLine(m_bBuf, nBytes, ndx) != TRUE);

		ProcessOptions();
		Message(m_strNormalText);
	}
	m_strLine.Empty();
	m_strResp.Empty();

	TempCounter++;
}

BOOL CPTerminalView::GetLine(const CByteArray &bytes, int nBytes, int &ndx)
{
	BOOL bLine = FALSE;
	while ( bLine == FALSE && ndx < nBytes )
	{
		unsigned char ch = (char)(bytes.GetAt( ndx ));
		
		switch( ch )
		{
		case '\r': // ignore
			m_strLine += "\r\n"; //"CR";
			break;
		case '\n': // end-of-line
//			m_strLine += '\n'; //"LF";
//			bLine = TRUE;
			break;
		default:   // other....
			m_strLine += ch;
			break;
		} 

		++ndx;

		if (ndx == nBytes)
		{
			bLine = TRUE;
		}
	}
	return bLine;
}

/////////////////////////////////////////////////////////////////////////////
// 处理Telnet选项协议
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::ProcessOptions()
{
	CString m_strTemp;
	CString m_strOption;
	unsigned char ch;
	int ndx;
	int ldx;
	BOOL bScanDone = FALSE;

	m_strTemp = m_strLine;

	while(!m_strTemp.IsEmpty() && bScanDone != TRUE)
	{
		ndx = m_strTemp.Find(IAC);
		if(ndx != -1)
		{
			m_strNormalText += m_strTemp.Left(ndx);
			ch = m_strTemp.GetAt(ndx + 1);
			switch(ch)
			{
			case DO:
			case DONT:
			case WILL:
			case WONT:
				m_strOption		= m_strTemp.Mid(ndx, 3);
				m_strTemp		= m_strTemp.Mid(ndx + 3);
				m_strNormalText	= m_strTemp.Left(ndx);
				m_ListOptions.AddTail(m_strOption);
				break;
			case IAC:
				m_strNormalText	= m_strTemp.Left(ndx);
				m_strTemp		= m_strTemp.Mid(ndx + 1);
				break;
			case SB:
				m_strNormalText = m_strTemp.Left(ndx);
				ldx = m_strTemp.Find(SE);
				m_strOption		= m_strTemp.Mid(ndx, ldx);
				m_ListOptions.AddTail(m_strOption);
				m_strTemp		= m_strTemp.Mid(ldx);
				//AfxMessageBox(m_strOption,MB_OK);
				break;
			}
		}

		else
		{
			m_strNormalText = m_strTemp;
			bScanDone = TRUE;
		}
	} 
	
	RespondToOptions();
}

void CPTerminalView::RespondToOptions()
{
	CString strOption;
	
	while(!m_ListOptions.IsEmpty())
	{
		strOption = m_ListOptions.RemoveHead();

		ArrangeReply(strOption);
	}

	DispatchMessage(m_strResp);
	m_strResp.Empty();
}

void CPTerminalView::ArrangeReply(CString strOption)
{
	unsigned char Verb;
	unsigned char Option;
	unsigned char Modifier;
	unsigned char ch;
	BOOL bDefined = FALSE;

	Verb = strOption.GetAt(1);
	Option = strOption.GetAt(2);

	switch(Option)
	{
	case 1:	// Echo
	case 3: // Suppress Go-Ahead
		bDefined = TRUE;
		break;
	}

	m_strResp += IAC;

	if(bDefined == TRUE)
	{
		switch(Verb)
		{
		case DO:
			ch = WILL;
			m_strResp += ch;
			m_strResp += Option;
			break;
		case DONT:
			ch = WONT;
			m_strResp += ch;
			m_strResp += Option;
			break;
		case WILL:
			ch = DO;
			m_strResp += ch;
			m_strResp += Option;
			break;
		case WONT:
			ch = DONT;
			m_strResp += ch;
			m_strResp += Option;
			break;
		case SB:
			Modifier = strOption.GetAt(3);
			if(Modifier == SEND)
			{
				ch = SB;
				m_strResp += ch;
				m_strResp += Option;
				m_strResp += IS;
				m_strResp += IAC;
				m_strResp += SE;
			}
			break;
		}
	}

	else
	{
		switch(Verb)
		{
		case DO:
			ch = WONT;
			m_strResp += ch;
			m_strResp += Option;
			break;
		case DONT:
			ch = WONT;
			m_strResp += ch;
			m_strResp += Option;
			break;
		case WILL:
			ch = DONT;
			m_strResp += ch;
			m_strResp += Option;
			break;
		case WONT:
			ch = DONT;
			m_strResp += ch;
			m_strResp += Option;
			break;
		}
	}
}

void CPTerminalView::DispatchMessage(CString strText)
{
	if(m_sClient)
		m_sClient->Send(strText, strText.GetLength());
}

/////////////////////////////////////////////////////////////////////////////
// 连接Telnet服务器
/////////////////////////////////////////////////////////////////////////////
BOOL CPTerminalView::ConnectToHost(CString m_strHost, UINT m_nPort)
{
	BOOL bOK;
	
	m_sClient = new CClientSocket(this);
	if(m_sClient != NULL)
	{
		bOK = m_sClient->Create();
		if(bOK == TRUE)
		{
			m_sClient->AsyncSelect(FD_READ | FD_WRITE | FD_CLOSE | FD_CONNECT | FD_OOB);
			bOK = m_sClient->Connect(m_strHost, m_nPort);
		}
		else
		{
			delete m_sClient;
		}
	}
	else
	{
		bOK = FALSE;
//		AfxMessageBox("Could not create new socket",MB_OK);
	}
	m_bConnectRemote = bOK;
	return bOK;
}

/////////////////////////////////////////////////////////////////////////////
// Telnet:清除屏幕
// 参数:光标位置,屏幕大小
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::EraseScreen(int nPosX, int nPosY, int nSizeX, int nSizeY)
{
	SendMessage(WM_COMMAND, ID_EDIT_CLEAR_ALL, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Telnet:清除行
// 参数:起始位置,宽度
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::EraseLine(int nPosX, int nPosY, int nLen)
{
//	SendMessage(WM_COMMAND, ID_EDIT_CLEAR_ALL, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Telnet:设置文字颜色
// 参数:颜色
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::SetTextAttribute(BYTE bySa)
{
	if(!m_bEnableAnsiColor)
	{
		return;
	}

	TRACE("COLOR:%d\n", bySa);
	COLORREF color = SHELL_COLOR_RESOK;
/*	int cR = (bySa&0x01) ? 255 : 0;
	if(cR == 0)
		cR = (bySa&0x08) ? 128 : 0;
	int cG = (bySa&0x02) ? 255 : 0;
	if(cG == 0)
		cG = (bySa&0x10) ? 128 : 0;
	int cB = (bySa&0x04) ? 255 : 0;
	if(cB == 0)
		cB = (bySa&0x20) ? 128 : 0;
	color = RGB(cR, cG, cB);
*/
	switch(bySa)
	{
	case 0:color = RGB(0,0,0);break;
	case 1:color = RGB(255,0,0);break;
	case 2:color = RGB(0,255,0);break;
	case 3:color = RGB(255,0,255);break;
	case 4:color = RGB(0,0,255);break;
	case 10:color = RGB(0,255,255);break;
	case 12:color = RGB(255,255,0);break;
	case 120:color = RGB(128,128,0);break;
	case 122:color = RGB(255,0,128);break;
	}
	m_clrRecv = color;	
	SetTermFontColor(color);
	//SetTermTextBackColor(color, RGB(128,0,192));
}

/////////////////////////////////////////////////////////////////////////////
// Serial Port

/////////////////////////////////////////////////////////////////////
// 串口回调函数
/////////////////////////////////////////////////////////////////////
void SerialHandleReturn(void* pSocket, int nCmd, WPARAM wParam, LPARAM lParam)
{
	if(nCmd == WM_COMM_RXCHAR)		//接收返回字符串
	{
		//CClientSocket* pClientSocket = (CClientSocket*)pSocket;
		unsigned char RXBuf = (unsigned char)wParam;
		//pClientSocket->SerialDataToSocket(RXBuf);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 初始化串口
/////////////////////////////////////////////////////////////////////////////
BOOL CPTerminalView::InitSerialPort(UINT nPort, UINT nBaud, char cParity, UINT nDataBits, UINT nStopBits)
{
	// 创建串口对象
	m_pSerialPort = new CSerialPort();
	
	if(!m_pSerialPort->InitPort(SerialHandleReturn, nPort,
			nBaud, cParity,	nDataBits, nStopBits))
	{	// 串口初始化失败
		TRACE("Serial Port %d Init Fail...\n", nPort);
		delete m_pSerialPort;
		m_pSerialPort = NULL;
		return FALSE;
	}

	if(!m_pSerialPort->StartMonitoring())
	{	// 启动串口线程失败
		TRACE("Serial Port %d StartMonitor Fail...\n", nPort);
		delete m_pSerialPort;
		m_pSerialPort = NULL;
		return FALSE;
	}

	// 设置串口父窗口
	m_pSerialPort->SetOwner(this);

	if(m_bHaveDocument)
	{
		CString strConnInfo;
		strConnInfo.Format("COM%d,%d,%d,%d,%c", nPort, nBaud, nDataBits, nStopBits, cParity);
		CChildFrame* pFrame = (CChildFrame*)(GetParent());
		pFrame->SetConnInfo(strConnInfo);
	}

	return TRUE;
}

HANDLE stdin1;
HANDLE stdout1;
HANDLE stderr1;
/////////////////////////////////////////////////////////////////////////////
// 连接主机
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::OnServerConnect() 
{
	switch(m_nWorkShell)
	{
	case SHELL_TELNET:
		{
			TCommLoginInfo LoginInfo;
			CString strTemp;
			LOADRCSTRING( strTemp, IDS_TERM_TELNET );
			strTemp += "---";
			LoginInfo.strTitle		= strTemp;
			LOADRCSTRING( strTemp, IDS_STR_LOGIN);
			LoginInfo.strTitle += strTemp;
			LOADRCSTRING( strTemp, IDS_TELNET_LOGIN_INFO);
			LoginInfo.strInfo		= strTemp;
			LOADRCSTRING( strTemp, IDS_TELNET_ADDRESS_LABEL);
			LoginInfo.strLAddress	= strTemp;
			LOADRCSTRING( strTemp, IDS_TELNET_PORT_LABEL);
			LoginInfo.strLUserName	= strTemp;
			LoginInfo.strUserName	= "23";
			LoginInfo.strLPassword	= " ";
			LOADRCSTRING( strTemp, IDS_TELNET_DOMAIN_LABEL);
			LoginInfo.strLDomain	= strTemp;
			CString  strDomain;
			LOADRCSTRING( strDomain, IDS_DOMAIN_TELNET)
			LoginInfo.bUseAddress	= TRUE;
			LoginInfo.bUseUserName	= TRUE;
			LoginInfo.bUsePassword	= FALSE;
			LoginInfo.bUseDomain	= TRUE;
			LoginInfo.strDomain		= strDomain;
			LoginInfo.strDomainList	= strDomain;
			LoginInfo.bCheck		= TRUE;
			CString  strEnableAnsiColor;	// 是否允许ANSI颜色定义
			LOADRCSTRING( strEnableAnsiColor, IDS_TELNET_ENABLE_ANSI_COLOR)
			LoginInfo.strCheck		= strEnableAnsiColor;
			if(theApp.m_ol.CommandPlat(OC_LOGIN_DIALOG, (WPARAM)(&LoginInfo), 0) == 1)
			{
				if(LoginInfo.strDomain == strDomain)
				{

					stdin1 = GetStdHandle(STD_INPUT_HANDLE);
					stdout1 = GetStdHandle(STD_OUTPUT_HANDLE);
					stderr1 = GetStdHandle(STD_ERROR_HANDLE);

					int nPort = atoi(LoginInfo.strUserName);
					m_pSocketDx = new CSocketDx((LPTSTR)(LPCTSTR)(LoginInfo.strAddress), nPort);
					m_hTelnetSocket = m_pSocketDx->TelnetConnect();
					if ( m_hTelnetSocket == NULL ) 
					{
						delete m_pSocketDx;
						m_pSocketDx = NULL;
						TRACE("\nTelnet Unable To Connect\n");
						CString strTemp;
						LOADRCSTRING( strTemp, IDS_LOGIN_TELNET_FAIL)
						AfxMessageBox(strTemp,MB_OK);
						return;
					}

					m_pSocketRx = new CSocketRx(m_hTelnetSocket,m_hTelnetThread[0]); 
					m_pSocketTx = new CSocketTx(m_hTelnetSocket,m_hTelnetThread[1]);
					m_pSocketRx->m_Protocol.m_pTerminalView = this;
					m_pSocketRx->m_Protocol.SetConsoleMode(stdin1,ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT);

					if(m_bHaveDocument)
					{
						((CChildFrame*)(GetParent()))->SetConnInfo(LoginInfo.strAddress);											
					}

					m_bEnableAnsiColor = LoginInfo.bCheck;
				}
			}
		}
		break;

	case SHELL_COM:
		{
			get_dll_resource();
			CDlgComParam ComParam;
			BOOL bOK = (ComParam.DoModal() == IDOK);
			reset_dll_resource();
			if(bOK)
			{
				CString strPort = ComParam.m_strComPort;
				strPort.Delete(0, 3);
				int nPort = atoi(strPort);
				char cParity = 'N';
				switch(ComParam.m_nParity)
				{
				case 0: cParity = 'P';break;
				case 1: cParity = 'O';break;
				case 2: cParity = 'N';break;
				}
				InitSerialPort(nPort, atoi(ComParam.m_strBaud), cParity,
							ComParam.m_nDataBits+7,	ComParam.m_nStopBits+1);
				m_bSerialEcho = ComParam.m_bEcho;
			}
		}
		break;

	case SHELL_RCADMIN:
		{
			TCommLoginInfo LoginInfo;
			CString strTemp;
			LOADRCSTRING( strTemp, IDS_TERM_REMOTE );
			strTemp += "---";
			LoginInfo.strTitle = strTemp;
			LOADRCSTRING( strTemp, IDS_STR_LOGIN)
			LoginInfo.strTitle += strTemp;
			CString  strDomain;
			LOADRCSTRING( strDomain, IDS_INTERPRETER_NAME)
			LoginInfo.bUseAddress	= TRUE;
			LoginInfo.bUseUserName	= TRUE;
			LoginInfo.bUsePassword	= TRUE;
			LoginInfo.bUseDomain	= TRUE;
			LOADRCSTRING( LoginInfo.strLDomain, IDS_RC_INTERPRETER)

			LoginInfo.strAddress = AfxGetApp()->GetProfileString(REG_PTERMINAL, REG_RCADMIN_IP, "");	
			LoginInfo.strUserName = AfxGetApp()->GetProfileString(REG_PTERMINAL, REG_RCADMIN_USER, "");	
			LoginInfo.strDomain = AfxGetApp()->GetProfileString(REG_PTERMINAL, REG_RCADMIN_TCLINTERPNAME, strDomain);	
			LoginInfo.strDomainList	= LoginInfo.strDomain;			
			/*
			if(theApp.m_ol.CommandPlat(OC_LOGIN_DIALOG, (WPARAM)(&LoginInfo), 0) == 1)
			{				
				//登录远程
				if( m_pIRemoteClient == NULL )
				{
					m_bConnectRemote = FALSE;
					break;
				}

				ERCResult eRet = m_pIRemoteClient->ConnectAndLogin(LoginInfo.strAddress, LoginInfo.strUserName, LoginInfo.strPassword);
				if( eRet != trOk )
				{
					CString strTemp;
					LOADRCSTRING( strTemp, IDS_LOGIN_RCADMIN_FAIL)
					AfxMessageBox(strTemp,MB_OK);
					m_bConnectRemote = FALSE;
				}
				else
				{
					//创建远程解释器
					eRet = m_pIRemoteClient->ProcessOpenInterpreter(LoginInfo.strDomain, m_nInterpID);
					if( eRet != trOk )
					{
						m_bConnectRemote = FALSE;
					}

					//保存登录对话框信息
					AfxGetApp()->WriteProfileString(REG_PTERMINAL, REG_RCADMIN_IP, LoginInfo.strAddress);	
					AfxGetApp()->WriteProfileString(REG_PTERMINAL, REG_RCADMIN_USER, LoginInfo.strUserName);	
					AfxGetApp()->WriteProfileString(REG_PTERMINAL, REG_RCADMIN_TCLINTERPNAME, LoginInfo.strDomain);	

					//修改状态栏信息
					CString strMsg;
					LOADRCSTRING(strTemp, IDS_TCL_RCINTERP)		// 远程TCL解释器名:%s[%d]-[%s]
					strMsg.Format(strTemp, LoginInfo.strDomain, m_nInterpID, LoginInfo.strAddress);
					if(m_bHaveDocument)
					{
						((CChildFrame*)(GetParent()))->SetConnInfo(strMsg);											
					}
					m_bConnectRemote = TRUE;
				}
			}*/
		}
		break;
	}
}

void CPTerminalView::OnUpdateServerConnect(CCmdUI* pCmdUI) 
{
	switch(m_nWorkShell)
	{
		case SHELL_TELNET:
			pCmdUI->Enable(!m_hTelnetSocket);
			break;
		case SHELL_COM:
			pCmdUI->Enable(!m_pSerialPort);
			break;
		case SHELL_RCADMIN:
			pCmdUI->Enable(!m_bConnectRemote);
			break;
		default:
			pCmdUI->Enable(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 关闭连接
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::OnServerDisconnect() 
{
	switch(m_nWorkShell)
	{
	case SHELL_TELNET:
		{
			m_pSocketRx->m_nExit = 1;
			m_pSocketTx->m_nExit = 1;
			m_pSocketTx->SetSendEvent();
			WaitForMultipleObjects(2, m_hTelnetThread, FALSE, INFINITE);
			delete m_pSocketDx;
			delete m_pSocketRx;
			delete m_pSocketTx;
			m_pSocketDx = NULL;
			m_pSocketRx = NULL;
			m_pSocketTx = NULL;
			m_hTelnetSocket = NULL;
			m_hTelnetThread[0] = NULL;
			m_hTelnetThread[1] = NULL;
			if(m_bHaveDocument)
			{
				((CChildFrame*)(GetParent()))->SetConnInfo("");											
			}
		}
		break;

	case SHELL_COM:
		{
			if(m_pSerialPort != NULL)
			{
				m_pSerialPort->EndMonitoring();
				delete m_pSerialPort;
				m_pSerialPort = NULL;
				if(m_bHaveDocument)
				{
					((CChildFrame*)(GetParent()))->SetConnInfo("");											
				}
			}
		}
		break;

	case SHELL_RCADMIN:
		{
			if( m_pIRemoteClient != NULL )
			{
			}
		}
		break;
	}
}

void CPTerminalView::OnUpdateServerDisconnect(CCmdUI* pCmdUI) 
{
	switch(m_nWorkShell)
	{
		case SHELL_TELNET:
			pCmdUI->Enable(m_hTelnetSocket);
			break;
		case SHELL_COM:
			pCmdUI->Enable(m_pSerialPort != NULL);
			break;
		case SHELL_RCADMIN:
			pCmdUI->Enable(m_bConnectRemote);
			break;
		default:
			pCmdUI->Enable(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 初始化控制台
/////////////////////////////////////////////////////////////////////////////
BOOL CPTerminalView::InitTerminal()
{
	SetWindowText(NULL);

	// 创建解释器
	if(SHELL_TCL == m_nWorkShell)
	{
		m_strInterpName = AfxGetApp()->GetProfileString(REG_PTERMINAL, REG_INTERPNAME, INTERP_NAME);
		m_pInterp = (IInterp*)(theApp.m_pIPlatUI->CreateVciObject("org.interp.tcl", m_strInterpName));
		if(m_pInterp)
		{
			// 先将iTcl库加载进来
			m_pInterp->RunScriptCommand("package require Itcl");

		}
	}else
	if(SHELL_PYTHON == m_nWorkShell)
	{
		m_strInterpName = AfxGetApp()->GetProfileString(REG_PTERMINAL, REG_INTERPNAME, INTERP_NAME);
		m_pInterp = (IInterp*)(theApp.m_pIPlatUI->CreateVciObject("org.interp.python", m_strInterpName));
	}else
	if(SHELL_RUBY == m_nWorkShell)
	{
		m_strInterpName = AfxGetApp()->GetProfileString(REG_PTERMINAL, REG_INTERPNAME, INTERP_NAME);
		m_pInterp = (IInterp*)(theApp.m_pIPlatUI->CreateVciObject("org.interp.ruby", m_strInterpName));
	}else
	if(SHELL_LUA == m_nWorkShell)
	{
		m_strInterpName = AfxGetApp()->GetProfileString(REG_PTERMINAL, REG_INTERPNAME, INTERP_NAME);
		m_pInterp = (IInterp*)(theApp.m_pIPlatUI->CreateVciObject("org.interp.lua", m_strInterpName));
	}else
	if(SHELL_PERL == m_nWorkShell)
	{
		m_strInterpName = AfxGetApp()->GetProfileString(REG_PTERMINAL, REG_INTERPNAME, INTERP_NAME);
		m_pInterp = (IInterp*)(theApp.m_pIPlatUI->CreateVciObject("org.interp.perl", m_strInterpName));
	}else
	if(SHELL_RCADMIN == m_nWorkShell)
	{
		typedef LPVOID (*CreateObject)(LPCSTR lpcsInterfaceName, LPVOID lpInitData);
		CreateObject fnCreateObject;

//#ifdef  _DEBUG
		//获取平台路径
		TCHAR szFullPath[MAX_PATH];
		TCHAR szdrive[_MAX_DRIVE];
		TCHAR szdir[_MAX_DIR];
		::GetModuleFileName(theApp.m_hInstance, szFullPath, MAX_PATH);
		_splitpath(szFullPath, szdrive, szdir, NULL, NULL);
		CString strDllName;
		strDllName.Format("%s%sRmClient.dll", szdrive, szdir);		
//#endif
		HMODULE hDll = AfxLoadLibrary(strDllName);
		if( hDll != NULL )
		{
			fnCreateObject = (CreateObject)GetProcAddress(hDll, "CreateObject");
			m_pIRemoteClient = (IRemoteClient*)fnCreateObject("IRemoteClient", NULL);
		}
		else
		{
			AfxMessageBox( "Load RemoteClient.dll failed", MB_OK|MB_ICONERROR );
			return FALSE;
		}
	}

	// 初始化提示符
	SetTermFontColor(SHELL_COLOR_PROMPT);
	Message(m_strPrompt);
	SetTermFontColor(SHELL_COLOR_INPUT);
	// 设置接收字符颜色
	m_clrRecv = SHELL_COLOR_RESOK;
	// 初始化提示符位置
	CheckLastPromptPos();

	// 清空记录未显示字符的变量
	m_strLastMsg = "";

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 结束化控制台
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::DoneTerminal() 
{
	if(m_pMenu)
	{
		delete m_pMenu;
		m_pMenu = NULL;
	}
	
	// 释放解释器
	if(m_nWorkShell == SHELL_TCL || m_nWorkShell == SHELL_PYTHON ||
		m_nWorkShell == SHELL_RUBY  || m_nWorkShell == SHELL_LUA ||
		m_nWorkShell == SHELL_PERL)
	{
		theApp.m_pIPlatUI->ReleaseVciObject(m_pInterp, FALSE);
		m_pInterp = NULL;
	}

	// (远程控制台)释放接口指针
	if(m_nWorkShell == SHELL_RCADMIN)
	{
		if( m_pIRemoteClient != NULL )
			m_pIRemoteClient->Release();
	}
	
	// (TELNET)关闭Telnet
	if(m_nWorkShell == SHELL_TELNET)
	{
		if(m_hTelnetSocket)
		{
			m_pSocketRx->m_nExit = 1;
			m_pSocketTx->m_nExit = 1;
			m_pSocketTx->SetSendEvent();
			WaitForMultipleObjects(2, m_hTelnetThread, FALSE, INFINITE);
			delete m_pSocketDx;
			delete m_pSocketRx;
			delete m_pSocketTx;
			m_pSocketDx = NULL;
			m_pSocketRx = NULL;
			m_pSocketTx = NULL;
			m_hTelnetSocket = NULL;
			m_hTelnetThread[0] = NULL;
			m_hTelnetThread[1] = NULL;
		}
	}

	// (串口)关闭串口
	if(m_nWorkShell == SHELL_COM)
	{
		if(m_pSerialPort != NULL)
		{
			m_pSerialPort->EndMonitoring();
			delete m_pSerialPort;
			m_pSerialPort = NULL;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// 设置当前工作外壳,bSwitch表示是否是切换控制台的命令
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::SetWorkShell(int nShell, BOOL bSwitch)
{
	if(bSwitch)
	{
		DoneTerminal();
	}

	m_nWorkShell	= nShell;
	switch(m_nWorkShell)
	{
	case SHELL_TCL:
		m_strPrompt = SHELL_PROMPT_TCL;
		m_strExtPrompt = _T("");
		break;
	case SHELL_PYTHON:
		m_strPrompt = SHELL_PROMPT_PYTHON;
		m_strExtPrompt = SHELL_EXTPROMPT_PYTHON;
		break;
	case SHELL_RUBY:
		m_strPrompt = SHELL_PROMPT_RUBY;
		m_strExtPrompt = _T("");
		break;
	case SHELL_LUA:
		m_strPrompt = SHELL_PROMPT_LUA;
		m_strExtPrompt = _T("");
		break;
	case SHELL_PERL:
		m_strPrompt = SHELL_PROMPT_PERL;
		m_strExtPrompt = _T("");
		break;
	case SHELL_TELNET:
		m_strPrompt = SHELL_PROMPT_TELNET;
		m_strExtPrompt = _T("");
		break;
	case SHELL_COM:
		m_strPrompt = SHELL_PROMPT_COM;
		m_strExtPrompt = _T("");
		break;
	}
	// 清空命令历史
	m_asCmdHistory.RemoveAll();

	if(bSwitch)
	{
		InitTerminal();
	}
}

/////////////////////////////////////////////////////////////////////////////
// 切换控制台类型
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::OnSwitchTerminal() 
{
	CTOptArray aopt;	// 创建属性树元素数组
	TOPTITEM item;		// 属性树元素
	item.hItem		= NULL;
	item.iType		= TOPT_GROUP;
	CString strTemp;
	LOADRCSTRING( strTemp, IDS_SWITCH_SHELL)
	item.lpszText	= strTemp;
	item.lpszEdit	= "";
	item.lpszList	= "";
	item.bCheck		= FALSE;
	item.iImage		= 0;
	item.iParent	= -1;
	aopt.Add(item);		// 添加根节点

	// 添加Shell选项节点
	item.iType		= TOPT_RADIOBUTTON;
	item.iImage		= 0;
	item.iParent	= 0;
	LOADRCSTRING( strTemp, IDS_TERM_TCL)
	item.lpszText	= strTemp;
	item.bCheck		= (m_nWorkShell == SHELL_TCL);
	aopt.Add(item);
	LOADRCSTRING( strTemp, IDS_TERM_PYTHON)
	item.lpszText	= strTemp;
	item.bCheck		= (m_nWorkShell == SHELL_PYTHON);
	aopt.Add(item);
	LOADRCSTRING( strTemp, IDS_TERM_LUA)
	item.lpszText	= strTemp;
	item.bCheck		= (m_nWorkShell == SHELL_LUA);
	aopt.Add(item);
	LOADRCSTRING( strTemp, IDS_TERM_RUBY)
	item.lpszText	= strTemp;
	item.bCheck		= (m_nWorkShell == SHELL_RUBY);
	aopt.Add(item);
	LOADRCSTRING( strTemp, IDS_TERM_PERL)
	item.lpszText	= strTemp;
	item.bCheck		= (m_nWorkShell == SHELL_PERL);
	aopt.Add(item);
	LOADRCSTRING( strTemp, IDS_TERM_TELNET)
	item.lpszText	= strTemp;
	item.bCheck		= (m_nWorkShell == SHELL_TELNET);
	aopt.Add(item);
	LOADRCSTRING( strTemp, IDS_TERM_COM)
	item.lpszText	= strTemp;
	item.bCheck		= (m_nWorkShell == SHELL_COM);
	aopt.Add(item);

	// 添加标题和提示信息
	item.iType		= TOPT_MSG;
	LOADRCSTRING( strTemp, IDS_SWITCH_SHELL)
	item.lpszText	= strTemp;
	LOADRCSTRING( strTemp, IDS_SWITCH_SHELL)
	item.lpszEdit	= strTemp;
	item.iParent	= -1;
	aopt.Add(item);

	// 执行属性树
	if(theApp.m_ol.CommandPlat(OC_EXECTREEOPTION, 0, (LPARAM)(&aopt)) == IDOK)
	{
		int nSelect = (int)(aopt[0].hItem);	// 被选中的元素索引,放在元素0的hItem中

		if((nSelect > 0) && ((nSelect-1) != m_nWorkShell))
		{
			if(m_bHaveDocument)
			{
				theApp.m_ol.CommandOWM(OC_TERM_SWITCHSHELL, nSelect-1, 0);
			}else
			{
				SetWorkShell(nSelect-1, TRUE);
			}
			AfxGetApp()->WriteProfileInt(REG_PTERMINAL, REG_DEFAULTSHELL, nSelect-1);
		}
	}
}

void CPTerminalView::OnUpdateSwitchTerminal(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// 重置解释器(避免和编辑器设置的解释器相同的情况下存在信息残留)
/////////////////////////////////////////////////////////////////////////////
void CPTerminalView::OnResetInterp() 
{
	SetWorkShell(m_nWorkShell, TRUE);
}

void CPTerminalView::OnUpdateResetInterp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

// 重载的拖拽是否允许函数,必须和DragAcceptFiles(FALSE);配合使用才能起到效果
HRESULT CPTerminalView::QueryAcceptData(LPDATAOBJECT lpdataobj,
	CLIPFORMAT* lpcfFormat, DWORD dwReco, BOOL bReally,
	HGLOBAL hMetaPict)
{
	if (bReally && *lpcfFormat == 0 && (m_nPasteType == 0))
	{
		COleDataObject dataobj;
		dataobj.Attach(lpdataobj, FALSE);
		if (!dataobj.IsDataAvailable(CF_TEXT)) // native avail, let richedit do as it wants
		{
			return S_FALSE;
			/*if (dataobj.IsDataAvailable(cfEmbeddedObject))
			{
				if (PasteNative(lpdataobj))
					return S_FALSE;
			}*/
		}
	}
	return CRichEditView::QueryAcceptData(lpdataobj, lpcfFormat, dwReco, bReally,
		hMetaPict);
}

