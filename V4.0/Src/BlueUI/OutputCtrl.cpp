// OutputCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "BlueUI.h"
#include "OutputCtrl.h"
#include "TreeOutCtrl.h"
#include "ReportOutCtrl.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputCtrl

COutputCtrl::COutputCtrl()
{
	dwMaxLine	= 10000;
	m_Color		= RGB(0, 0, 0);
	m_Effect	= 0;
	m_Height	= 200;
	strcpy(m_FaceName, "\0");
	m_pParentBar= NULL;
}

COutputCtrl::~COutputCtrl()
{
}

IMPLEMENT_DYNAMIC(COutputCtrl, CRichEditCtrl)

BEGIN_MESSAGE_MAP(COutputCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(COutputCtrl)
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_OEDIT_COPY, OnOeditCopy)
	ON_COMMAND(ID_OEDIT_CLEAR, OnOeditClear)
	ON_COMMAND(ID_OFILE_SAVE, OnOfileSave)
	ON_COMMAND(ID_OEDIT_SELECTALL, OnOeditSelectall)
	ON_UPDATE_COMMAND_UI(ID_OEDIT_COPY, OnUpdateOeditCopy)
	ON_UPDATE_COMMAND_UI(ID_OEDIT_CLEAR, OnUpdateOeditClear)
	ON_UPDATE_COMMAND_UI(ID_OFILE_SAVE, OnUpdateOfileSave)
	ON_UPDATE_COMMAND_UI(ID_OEDIT_SELECTALL, OnUpdateOeditSelectall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputCtrl message handlers
BOOL COutputCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	dwStyle |= (ES_AUTOVSCROLL | ES_MULTILINE| ES_AUTOHSCROLL | ES_READONLY 
		| WS_BORDER | WS_TABSTOP | WS_VSCROLL | WS_HSCROLL);
	if(!CRichEditCtrl::Create( dwStyle, rect, pParentWnd, nID))
		return FALSE;

	GetDefaultCharFormat(m_fmtCur);	// 获取缺省字体信息

	ResetFormat();					// 初始化字体

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 清除输出内容
/////////////////////////////////////////////////////////////////////////////
void COutputCtrl::Clear()
{
	int len = GetWindowTextLength();
	SetSel(0,len);
	ReplaceSel("");
}

/////////////////////////////////////////////////////////////////////////////
// 清除指定行
/////////////////////////////////////////////////////////////////////////////
void COutputCtrl::ClearLineFrom(INT dlineFrom, INT dlineTo)
{
	int Num1 = LineIndex(dlineFrom);
	int Num2 = LineIndex(dlineTo+1);
	SetSel(Num1,Num2);
	ReplaceSel("");
}

/////////////////////////////////////////////////////////////////////////////
// 设置最大行数限制
/////////////////////////////////////////////////////////////////////////////
void COutputCtrl::SetMaxLine(DWORD dwLine)
{
	dwMaxLine = dwLine;
	if( dwMaxLine < 100)
		dwMaxLine = 100;
	ArrangeLine();
}

/////////////////////////////////////////////////////////////////////////////
// 清除多余的行
/////////////////////////////////////////////////////////////////////////////
void COutputCtrl::ArrangeLine()
{
	INT TotalLine;
	TotalLine = GetLineCount();
	if( TotalLine > (INT)dwMaxLine ) {

		TotalLine -= dwMaxLine;
		ClearLineFrom( 0 , TotalLine-1 );
	}
}

/////////////////////////////////////////////////////////////////////////////
// 输出
/////////////////////////////////////////////////////////////////////////////
void COutputCtrl::vsWrite(LPCTSTR lpszFormat, va_list argList)
{
	ASSERT(AfxIsValidString(lpszFormat, FALSE));

	if(!::IsWindow(m_hWnd))
	{
		return;
	}

	va_list argListSave = argList;
	// make a guess at the maximum length of the resulting string
	int nMaxLen = 0;
	for (LPCTSTR lpsz = lpszFormat; *lpsz != '\0'; lpsz = _tcsinc(lpsz)) {

		// handle '%' character, but watch out for '%%'
		if (*lpsz != '%' || *(lpsz = _tcsinc(lpsz)) == '%') {

			nMaxLen += _tclen(lpsz);
			continue;
		}

		int nItemLen = 0;
		// handle '%' character with format
		int nWidth = 0;
		for (; *lpsz != '\0'; lpsz = _tcsinc(lpsz)) {

			// check for valid flags
			if (*lpsz == '#')
				nMaxLen += 2;   // for '0x'
			else if (*lpsz == '*')
				nWidth = va_arg(argList, int);
			else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' ||
				*lpsz == ' ')
				;
			else // hit non-flag character
				break;
		}
		// get width and skip it
		if (nWidth == 0) {

			// width indicated by
			nWidth = _ttoi(lpsz);
			for (; *lpsz != '\0' && _istdigit(*lpsz); lpsz = _tcsinc(lpsz))
				;
		}
		ASSERT(nWidth >= 0);

		int nPrecision = 0;
		if (*lpsz == '.') {

			// skip past '.' separator (width.precision)
			lpsz = _tcsinc(lpsz);

			// get precision and skip it
			if (*lpsz == '*') {

				nPrecision = va_arg(argList, int);
				lpsz = _tcsinc(lpsz);
			}
			else {

				nPrecision = _ttoi(lpsz);
				for (; *lpsz != '\0' && _istdigit(*lpsz); lpsz = _tcsinc(lpsz))
					;
			}
			ASSERT(nPrecision >= 0);
		}

		// should be on type modifier or specifier
		int nModifier = 0;
		switch (*lpsz) {

		// modifiers that affect size
			case 'h':
				nModifier = FORCE_ANSI;
				lpsz = _tcsinc(lpsz);
				break;
			case 'l':
				nModifier = FORCE_UNICODE;
				lpsz = _tcsinc(lpsz);
				break;

			// modifiers that do not affect size
			case 'F':
			case 'N':
			case 'L':
				lpsz = _tcsinc(lpsz);
				break;
		}

		// now should be on specifier
		switch (*lpsz | nModifier) {

			// single characters
			case 'c':
			case 'C':
				nItemLen = 2;
				va_arg(argList, TCHAR_ARG);
				break;
			case 'c'|FORCE_ANSI:
			case 'C'|FORCE_ANSI:
				nItemLen = 2;
				va_arg(argList, CHAR_ARG);
				break;
			case 'c'|FORCE_UNICODE:
			case 'C'|FORCE_UNICODE:
				nItemLen = 2;
				va_arg(argList, WCHAR_ARG);
				break;

			// strings
			case 's': {

				LPCTSTR pstrNextArg = va_arg(argList, LPCTSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6;  // "(null)"
				else {

				   nItemLen = lstrlen(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
				break;
			}

			case 'S': {

	#ifndef _UNICODE
				LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6;  // "(null)"
				else {

				   nItemLen = wcslen(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
	#else
				LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else {

				   nItemLen = lstrlenA(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
	#endif
				break;
			}

			case 's'|FORCE_ANSI:
			case 'S'|FORCE_ANSI:
			{

				LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else
				{

				   nItemLen = lstrlenA(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
				break;
			}

	#ifndef _MAC
			case 's'|FORCE_UNICODE:
			case 'S'|FORCE_UNICODE: {

				LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else
				{

				   nItemLen = wcslen(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
				break;
			}
	#endif
		}

			// adjust nItemLen for strings
		if (nItemLen != 0) {

			nItemLen = max(nItemLen, nWidth);
			if (nPrecision != 0)
				nItemLen = min(nItemLen, nPrecision);
		}
		else{

			switch (*lpsz) {

				// integers
				case 'd':
				case 'i':
				case 'u':
				case 'x':
				case 'X':
				case 'o':
					va_arg(argList, int);
					nItemLen = 32;
					nItemLen = max(nItemLen, nWidth+nPrecision);
					break;

				case 'e':
				case 'f':
				case 'g':
				case 'G':
					va_arg(argList, DOUBLE_ARG);
					nItemLen = 128;
					nItemLen = max(nItemLen, nWidth+nPrecision);
					break;

				case 'p':
					va_arg(argList, void*);
					nItemLen = 32;
					nItemLen = max(nItemLen, nWidth+nPrecision);
					break;

				// no output
				case 'n':
					va_arg(argList, int*);
					break;

				default:
					//ASSERT(FALSE);  // unknown formatting option
					nMaxLen += 2;
			}
		}

		// adjust nMaxLen for output nItemLen
		nMaxLen += nItemLen;	// add for blueant
	}

	char  *p_ssData;
	p_ssData = new char[nMaxLen+1];
	VERIFY(_vstprintf(p_ssData, lpszFormat, argListSave) <= nMaxLen+1);

	SetSel(-1,-1);			// 选择到最后
	SetCurFormat();			// 设置字体
	ReplaceSel(p_ssData);	// 对选择部分进行替换

	delete[] p_ssData;

	UpdateWindow();

	va_end(argListSave);
	
	ArrangeLine();			// 删除多余的行

	// 自动滚行
	CDC* pDC =  GetWindowDC( );
	TEXTMETRIC tm;
	pDC->GetTextMetrics( &tm );
	RECT  sRect;
	GetRect( &sRect );
	ReleaseDC( pDC );
	int nWndLns = (sRect.bottom - sRect.top) / tm.tmAscent;
	int nCurLine= GetFirstVisibleLine( );
	long lScrLns = GetLineCount( ) - nCurLine;
	if(lScrLns>nWndLns)
		LineScroll( lScrLns-nWndLns );  //added by wpr to scroll to the newest line
}

/////////////////////////////////////////////////////////////////////////////
// 直接输出,无格式化
/////////////////////////////////////////////////////////////////////////////
void COutputCtrl::WriteDirect(LPCTSTR lpszOutput)
{
	SetSel(-1,-1);			// 选择到最后
	SetCurFormat();			// 设置字体
	ReplaceSel(lpszOutput);	// 对选择部分进行替换

	UpdateWindow();
	
	ArrangeLine();			// 删除多余的行

	// 自动滚行
	CDC* pDC =  GetWindowDC( );
	TEXTMETRIC tm;
	pDC->GetTextMetrics( &tm );
	RECT  sRect;
	GetRect( &sRect );
	ReleaseDC( pDC );
	int nWndLns = (sRect.bottom - sRect.top) / tm.tmAscent;
	int nCurLine= GetFirstVisibleLine( );
	long lScrLns = GetLineCount( ) - nCurLine;
	if(lScrLns>nWndLns)
		LineScroll( lScrLns-nWndLns );  //added by wpr to scroll to the newest line
}

/*
/////////////////////////////////////////////////////////////////////////////
// 修改一行的颜色
/////////////////////////////////////////////////////////////////////////////
void COutputCtrl::SetLineFormat(int nFmt)
{
	long nCharNumber  = 0, nLineIndex = 0;

	for (int i = 0; i < nLineNO - 1; i++)
	{
		//得到每行的索引号
		nLineIndex = pEditCtrl->LineIndex(i); 

		//累计字符数
		nCharNumber = 2 + nCharNumber + pEditCtrl->LineLength(nLineIndex);
	}

	//定位光标到指定行
	SetSel(nCharNumber, nCharNumber);

	//计算由开始到指定行行首的字符数
	long nStartChar = LineIndex(-1);;

	//计算由开始到指定行行尾的字符数
	long nEndChar = nStartChar + LineLength(-1);

	HideSelection(TRUE, FALSE);
	//根据由开始到指定行行首行尾的字符数选定该行
	SetSel(nStartChar,nEndChar);

	//设置选定行的字符为红色
	CHARFORMAT cf;
	GetSelectionCharFormat(cf);

	cf.crTextColor = RGB(255,0,0);
	cf.dwMask = CFM_COLOR;
	SetSelectionCharFormat(cf);

	SetCursorLineNO(nOldCursorLine);
	HideSelection(FALSE, FALSE);
}
*/
/////////////////////////////////////////////////////////////////////////////
// 设置颜色类型
/////////////////////////////////////////////////////////////////////////////
void COutputCtrl::SetFormat(int nFmt)
{
	m_fmtCur.dwMask |= CFM_COLOR;
	m_fmtCur.dwEffects &= ~CFE_AUTOCOLOR ;
	switch(nFmt)
	{
	case 0:m_fmtCur.crTextColor = RGB( 0 , 0, 0 );break;
	case 1:m_fmtCur.crTextColor = RGB( 0 , 0, 255 );break;
	case 2:m_fmtCur.crTextColor = RGB( 255 , 0, 0 );break;
	case 3:m_fmtCur.crTextColor = RGB( 0 , 255, 0 );break;
	case 4:m_fmtCur.crTextColor = RGB( 128 , 128, 128 );break;
	default:m_fmtCur.crTextColor = RGB( 0 , 0, 0 );
	}
}

/////////////////////////////////////////////////////////////////////////////
// 设置文字颜色
/////////////////////////////////////////////////////////////////////////////
void COutputCtrl::SetTextColor(COLORREF color)
{
	m_fmtCur.dwMask |= CFM_COLOR;
	m_fmtCur.dwEffects &= ~CFE_AUTOCOLOR ;
	m_fmtCur.crTextColor = color;
}

/////////////////////////////////////////////////////////////////////////////
// 设置字体为当前字体
/////////////////////////////////////////////////////////////////////////////
void COutputCtrl::SetCurFormat()
{
	SetSelectionCharFormat(m_fmtCur);
}

/////////////////////////////////////////////////////////////////////////////
// 初始化字体(设置为缺省值)
/////////////////////////////////////////////////////////////////////////////
void COutputCtrl::ResetFormat()
{
	CString strSubKey;
	strSubKey.Format("%s\\%s", REG_FORMAT_SUBKEY, REG_FMT_OUTPUT);
	// 字体名
	CString strFont = AfxGetApp()->GetProfileString(strSubKey, REG_FMFONT, "FixedSys");
	// 字体大小
	int nSize = AfxGetApp()->GetProfileInt(strSubKey, REG_FMSIZE, 16);
	// 前景色
	COLORREF colorFg = AfxGetApp()->GetProfileInt(strSubKey, REG_FMCOLORFG, RGB(0,0,0));
	// 背景色
	COLORREF colorBk = AfxGetApp()->GetProfileInt(strSubKey, REG_FMCOLORBK, RGB(255,255,255));
	// 是否粗体
	BOOL bBold = AfxGetApp()->GetProfileInt(strSubKey, REG_FMBOLD, FALSE);
	// 是否斜体
	BOOL bItalic = AfxGetApp()->GetProfileInt(strSubKey, REG_FMITALIC, FALSE);
	// 是否下划线
	BOOL bUnderLine = AfxGetApp()->GetProfileInt(strSubKey, REG_FMUNDERLINE, FALSE);
	// 是否删除线
	BOOL bStrikeOut = AfxGetApp()->GetProfileInt(strSubKey, REG_FMSTRIKEOUT, FALSE);
	// 设置字体配置串
	CString strFmt;
	strFmt.Format("FONT=%s;HEIGHT=%d;COLOR=(%d,%d,%d);", strFont, nSize*12,
				GetRValue(colorFg), GetGValue(colorFg), GetBValue(colorFg));
	CString strEffect = "EFFECT=";
	if(bBold)	strEffect += "BOLD,";
	if(bItalic)	strEffect += "ITALIC,";
	if(bUnderLine)	strEffect += "UNDERLINE,";
	if(bStrikeOut)	strEffect += "STRIKEOUT";
	strFmt += strEffect;
	// 设置字体到当前字体对象中
	SetCtrlFormat(strFmt);
}

/////////////////////////////////////////////////////////////////////////////
// 按照配置串设置字体信息到当前字体对象中
/////////////////////////////////////////////////////////////////////////////
void COutputCtrl::SetCtrlFormat(LPCTSTR lpszSet)
{
	CString strSet = lpszSet;
	CStringArray aLines;
	// 拆分成若干行
	ParseLines(strSet, aLines);
	for(int i=0; i<aLines.GetSize(); i++)
	{
		CString strLine = aLines[i];
		strLine.MakeUpper();
		ParseALine(strLine);	// 分析行
	}

	// 设置到当前字体对象中
	m_fmtCur.dwMask |= CFM_CHARSET;	// char set
	m_fmtCur.dwMask |= CFM_COLOR;
	m_fmtCur.dwMask |= CFM_SIZE;
	m_fmtCur.dwMask |= CFM_FACE;
	m_fmtCur.dwMask |= CFM_BOLD;
	m_fmtCur.dwMask |= CFM_ITALIC;
	m_fmtCur.dwMask |= CFM_STRIKEOUT;
	m_fmtCur.dwMask |= CFM_UNDERLINE;
	m_fmtCur.dwEffects = 0;
	m_fmtCur.dwEffects &= ~CFE_AUTOCOLOR;
	m_fmtCur.dwEffects |= m_Effect;
	m_fmtCur.crTextColor = m_Color;
	m_fmtCur.yHeight = m_Height;
	if(m_FaceName[0] != 0)
	{
		strcpy(m_fmtCur.szFaceName, m_FaceName);
	}
	m_fmtCur.bCharSet = GB2312_CHARSET;	// char set
}

/////////////////////////////////////////////////////////////////////////////
// 拆分成若干行
/////////////////////////////////////////////////////////////////////////////
void COutputCtrl::ParseLines(CString strSet, CStringArray& aLines)
{
	int pos;
	while((pos = strSet.Find(";")) >= 0)
	{
		aLines.Add(strSet.Left(pos));
		int len = strSet.GetLength() - pos -1;
		strSet = strSet.Right(len);
	}
	if(strSet.GetLength() > 0)
	{
		aLines.Add(strSet);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 分析单个行
// 语法规则：
// 1.可以有若干个设置项，用";"隔开
// 2.设置颜色的语法是: COLOR=R,G,B
// 3.设置高度的语法是: HEIGHT=高度
// 4.设置效果的语法规则是: EFFECT=效果1,效果2,...
//                         效果可以是: BOLD       - 粗体
//                                     ITALIC     - 斜体
//                                     STRIKEOUT  - 删除线
//                                     UNDERLINE  - 下划线
//                                     NOEFFECT   - 没有效果
// 5.设置字体的语法规则是: FONT=字体名
/////////////////////////////////////////////////////////////////////////////
void COutputCtrl::ParseALine(CString strLine)
{
	CString strSet, strTmp;
	int pos;
	if((pos = strLine.Find("COLOR=")) == 0)
	{
		strSet = strLine.Right(strLine.GetLength()-pos-6);
		int nPos = strSet.Find(",");
		strTmp = strSet.Left(nPos);
		int nr = atoi(strTmp);
		strSet = strSet.Mid(nPos + 1, strSet.GetLength() - nPos -1);
		nPos = strSet.Find(",");
		strTmp = strSet.Left(nPos);
		int ng = atoi(strTmp);
		strTmp = strSet.Mid(nPos + 1, strSet.GetLength() - nPos -1);
		int nb = atoi(strTmp);
		m_Color = RGB(nr, ng, nb);
		return;
	}

	if((pos = strLine.Find("HEIGHT=")) == 0)
	{
		strSet = strLine.Right(strLine.GetLength()-pos-7);
		m_Height = atoi(strSet);
		return;
	}

	if((pos = strLine.Find("EFFECT=")) == 0)
	{
		strSet = strLine.Right(strLine.GetLength()-pos-7);

		int nPos;
		while((nPos = strSet.Find(",")) >= 0)
		{
			strTmp = strSet.Left(nPos);
			if(strcmp(strTmp, "BOLD") == 0)
			{
				m_Effect |= CFE_BOLD;
			}else
			if(strcmp(strTmp, "ITALIC") == 0)
			{
				m_Effect |= CFE_ITALIC;
			}else
			if(strcmp(strTmp, "STRIKEOUT") == 0)
			{
				m_Effect |= CFE_STRIKEOUT;
			}else
			if(strcmp(strTmp, "UNDERLINE") == 0)
			{
				m_Effect |= CFE_UNDERLINE;
			}else
			if(strcmp(strTmp, "NOEFFECT") == 0)
			{
				m_Effect = 0;
			}
			strSet = strSet.Mid(nPos + 1, strSet.GetLength() - nPos -1);
		}
		if(strSet.GetLength() > 0)
		{
			strTmp = strSet;
			if(strcmp(strTmp, "BOLD") == 0)
			{
				m_Effect |= CFE_BOLD;
			}else
			if(strcmp(strTmp, "ITALIC") == 0)
			{
				m_Effect |= CFE_ITALIC;
			}else
			if(strcmp(strTmp, "STRIKEOUT") == 0)
			{
				m_Effect |= CFE_STRIKEOUT;
			}else
			if(strcmp(strTmp, "UNDERLINE") == 0)
			{
				m_Effect |= CFE_UNDERLINE;
			}else
			if(strcmp(strTmp, "NOEFFECT") == 0)
			{
				m_Effect = 0;
			}
		}
		return;
	}

	if((pos = strLine.Find("FONT=")) == 0)
	{
		strSet = strLine.Right(strLine.GetLength()-pos-5);
		if(strcmp(strSet, "DEFAULT") == 0)
		{
			CHARFORMAT fmt;
			GetDefaultCharFormat(fmt);
			strcpy(m_FaceName, fmt.szFaceName);
		}else
			strcpy(m_FaceName, strSet);
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////
// 指定颜色输出
/////////////////////////////////////////////////////////////////////////////
void COutputCtrl::OutputFormat(int nColor, LPCTSTR format, ...) 
{
	SetFormat(nColor);
	va_list apList;
	va_start( apList, format );     /* Initialize variable arguments. */
	vsWrite( format, apList);
	va_end( apList ) ;
}

/////////////////////////////////////////////////////////////////////////////
// 输出
/////////////////////////////////////////////////////////////////////////////
void COutputCtrl::Output(LPCTSTR format, ...) 
{
	va_list apList;
	va_start( apList, format );     /* Initialize variable arguments. */
	vsWrite( format, apList);
	va_end( apList ) ;
}

/////////////////////////////////////////////////////////////////////////////
// 右键菜单
/////////////////////////////////////////////////////////////////////////////
void COutputCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// 安装CoolMenu
	CMenu* pMenu = new CMenu();
	pMenu->LoadMenu(IDR_OUTPUT);
	CMenu* pmenu = (CMenu*)pMenu->GetSubMenu(0);
	ClientToScreen(&point);
	BOOL bEnable = (GetTextLength() > 0);
	pmenu->EnableMenuItem(ID_OEDIT_CLEAR, bEnable ? MF_ENABLED : MF_GRAYED);
	pmenu->EnableMenuItem(ID_OEDIT_SELECTALL, bEnable ? MF_ENABLED : MF_GRAYED);
	pmenu->EnableMenuItem(ID_OFILE_SAVE, bEnable ? MF_ENABLED : MF_GRAYED);
	//pmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
	//		point.x, point.y, this);
	CXTPCommandBars::TrackPopupMenu(pmenu, 0, point.x, point.y, this);
	delete pMenu;
}

/////////////////////////////////////////////////////////////////////////////
// RichEdit 流操作回调函数
/////////////////////////////////////////////////////////////////////////////
DWORD CALLBACK COutputCtrl::EditStreamInCallback(DWORD dwCookie, 
    LPBYTE pbBuff, LONG cb, LONG FAR *pcb)
{
	CFile* pFile = (CFile*) dwCookie;

	*pcb = pFile->Read(pbBuff, cb);

	return 0;
}

DWORD CALLBACK COutputCtrl::EditStreamOutCallback(DWORD dwCookie, 
    LPBYTE pbBuff, LONG cb, LONG FAR *pcb)
{
	CFile* pFile = (CFile*) dwCookie;

	pFile->Write(pbBuff, cb);
	*pcb = cb;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 保存成文件
/////////////////////////////////////////////////////////////////////////////
void COutputCtrl::SaveToFile(CString strFile, int nFormat) 
{
	EDITSTREAM es;
	CFile rtfFile(TEXT(strFile), CFile::modeCreate|CFile::modeWrite);
	es.dwCookie = (DWORD) &rtfFile;
	es.pfnCallback = EditStreamOutCallback;
	StreamOut(nFormat, es);	// SF_TEXT
	rtfFile.Close();
}

/////////////////////////////////////////////////////////////////////////////
// 处理按键操作
/////////////////////////////////////////////////////////////////////////////
void COutputCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// 解决在输出窗口上按ESC异常的问题,将ESC屏蔽掉
	if(VK_ESCAPE == nChar)
	{
		return;
	}

	if((GetKeyState(VK_CONTROL) < 0) && GetAsyncKeyState('C'))
	{
		Copy();
	}
	
	CRichEditCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

//////////////////////////////////////////////////////////////////
// 拷贝选择部分
//////////////////////////////////////////////////////////////////
void COutputCtrl::OnOeditCopy() 
{
	Copy();
}

void COutputCtrl::OnUpdateOeditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

//////////////////////////////////////////////////////////////////
// 清除输出窗口
//////////////////////////////////////////////////////////////////
void COutputCtrl::OnOeditClear() 
{
	SetSel(0, -1);
	Clear();
}

void COutputCtrl::OnUpdateOeditClear(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetTextLength() > 0);
}

//////////////////////////////////////////////////////////////////
// 输出窗口内容全选
//////////////////////////////////////////////////////////////////
void COutputCtrl::OnOeditSelectall() 
{
	int nLen = GetWindowTextLength();
	SetSel(0, nLen);
}

void COutputCtrl::OnUpdateOeditSelectall(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetTextLength() > 0);
}

//////////////////////////////////////////////////////////////////
// 保存输出窗口内容
//////////////////////////////////////////////////////////////////
void COutputCtrl::OnOfileSave() 
{
	CString	strFilePath = "";

	CString strFileFilterOutput = _T("文本文件(*.txt)|*.txt|RTF文档(*.rtf)|*.rtf||");
	CFileDialog dialog(FALSE, "*.txt", "", OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, strFileFilterOutput);

	if(IDOK == dialog.DoModal())
	{
		strFilePath = dialog.GetPathName();

		// 根据扩展名决定保存为何种类型的文档
		CString strExt = dialog.GetFileExt();
		strExt.MakeLower();
		if(strExt == "txt")	// 普通文本
			SaveToFile(strFilePath, SF_TEXT);
		else				// RTF文档
			SaveToFile(strFilePath, SF_RTF);
	}
}

void COutputCtrl::OnUpdateOfileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetTextLength() > 0);
}

//////////////////////////////////////////////////////////////////
// 清除输出内容
//////////////////////////////////////////////////////////////////
void OUTPUT_CLEAR(int nID)
{
	CMainFrame* pMainFrm = (CMainFrame*)(theApp.m_pMainWnd);
	if(!pMainFrm)
	{
		return;
	}

	COutputWndCtrl* pOutputCtrl = pMainFrm->GetOutputCtrl(nID+IDR_PANE_OUTPUT);
	if(pOutputCtrl == NULL)
	{
		return;
	}
	
	if(pOutputCtrl->nCtrlType == OUTPUT_RICH)
	{
		COutputCtrl* pOutCtrl = (COutputCtrl*)(pOutputCtrl->pWnd);
		if(pOutCtrl && pOutCtrl->m_hWnd)
		{
			pOutCtrl->SetSel(0, -1);
			pOutCtrl->Clear();
		}
	}else
	if(pOutputCtrl->nCtrlType == OUTPUT_GRID)
	{
		CReportOutCtrl* pOutCtrl = (CReportOutCtrl*)(pOutputCtrl->pWnd);
		if(pOutCtrl && pOutCtrl->m_hWnd)
		{
			pOutCtrl->Clear();
		}
	}else
	if(pOutputCtrl->nCtrlType == OUTPUT_TREE)
	{
		CTreeOutCtrl* pOutCtrl = (CTreeOutCtrl*)(pOutputCtrl->pWnd);
		if(pOutCtrl && pOutCtrl->m_hWnd)
		{
			pOutCtrl->DeleteAllItems();
		}
	}
}

//////////////////////////////////////////////////////////////////
// 激活某个窗口
//////////////////////////////////////////////////////////////////
int OUTPUT_ACTIVE_ID(int nID)
{
	CMainFrame* pMainFrm = (CMainFrame*)(theApp.m_pMainWnd);
	if(!pMainFrm)
	{
		return FALSE;
	}
	
	COutputWndCtrl* pOutputCtrl = pMainFrm->GetOutputCtrl(nID+IDR_PANE_OUTPUT);
	if(pOutputCtrl == NULL)
	{
		return FALSE;
	}

	pMainFrm->GetPaneManager()->ShowPane(nID+IDR_PANE_OUTPUT);

	return TRUE;
}

//////////////////////////////////////////////////////////////////
// 设置某个窗口输出格式和颜色
//////////////////////////////////////////////////////////////////
int OUTPUT_SET_ID(int nID, CString strFormat)
{
	CMainFrame* pMainFrm = (CMainFrame*)(theApp.m_pMainWnd);
	if(!pMainFrm)
	{
		return FALSE;
	}
	
	COutputWndCtrl* pOutputCtrl = pMainFrm->GetOutputCtrl(nID+IDR_PANE_OUTPUT);
	if(pOutputCtrl == NULL)
	{
		return FALSE;
	}

	if(pOutputCtrl->nCtrlType == OUTPUT_RICH)
	{
		COutputCtrl* pOutCtrl = (COutputCtrl*)(pOutputCtrl->pWnd);
		if(pOutCtrl && pOutCtrl->m_hWnd)
		{
			pOutCtrl->SetCtrlFormat(strFormat);
		}
	}else
	if(pOutputCtrl->nCtrlType == OUTPUT_GRID)
	{
		CReportOutCtrl* pOutCtrl = (CReportOutCtrl*)(pOutputCtrl->pWnd);
		if(pOutCtrl && pOutCtrl->m_hWnd)
		{
			pOutCtrl->SetCtrlFormat(strFormat);
		}
	}else
	if(pOutputCtrl->nCtrlType == OUTPUT_TREE)
	{
		CTreeOutCtrl* pOutCtrl = (CTreeOutCtrl*)(pOutputCtrl->pWnd);
		if(pOutCtrl && pOutCtrl->m_hWnd)
		{
			pOutCtrl->SetCtrlFormat(strFormat);
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////
// 获取某个窗口输出信息
//////////////////////////////////////////////////////////////////
CString OUTPUT_GET_ID(int nID, CString strParam)
{
	CMainFrame* pMainFrm = (CMainFrame*)(theApp.m_pMainWnd);
	if(!pMainFrm)
	{
		return "";
	}
	
	COutputWndCtrl* pOutputCtrl = pMainFrm->GetOutputCtrl(nID+IDR_PANE_OUTPUT);
	if(pOutputCtrl == NULL)
	{
		return "";
	}

	if(pOutputCtrl->nCtrlType == OUTPUT_RICH)
	{
		COutputCtrl* pOutCtrl = (COutputCtrl*)(pOutputCtrl->pWnd);
		if(pOutCtrl && pOutCtrl->m_hWnd)
		{
			return "";
		}
	}else
	if(pOutputCtrl->nCtrlType == OUTPUT_GRID)
	{
		CReportOutCtrl* pOutCtrl = (CReportOutCtrl*)(pOutputCtrl->pWnd);
		if(pOutCtrl && pOutCtrl->m_hWnd)
		{
			return pOutCtrl->GetInfo(strParam);
		}
	}else
	if(pOutputCtrl->nCtrlType == OUTPUT_TREE)
	{
		CTreeOutCtrl* pOutCtrl = (CTreeOutCtrl*)(pOutputCtrl->pWnd);
		if(pOutCtrl && pOutCtrl->m_hWnd)
		{
			return pOutCtrl->GetInfo(strParam);
		}
	}

	return "";
}

//////////////////////////////////////////////////////////////////
// 输出树窗口内容
//////////////////////////////////////////////////////////////////
int OUTPUT_TREEITEMS_ID(int nID, LPVOID pItems)
{
	CMainFrame* pMainFrm = (CMainFrame*)(theApp.m_pMainWnd);
	if(!pMainFrm)
	{
		return FALSE;
	}
	
	COutputWndCtrl* pOutputCtrl = pMainFrm->GetOutputCtrl(nID+IDR_PANE_OUTPUT);
	if(pOutputCtrl == NULL)
	{
		return FALSE;
	}

	if(pOutputCtrl->nCtrlType == OUTPUT_GRID)
	{
		CReportOutCtrl* pOutCtrl = (CReportOutCtrl*)(pOutputCtrl->pWnd);
		if(pOutCtrl && pOutCtrl->m_hWnd)
		{
			pOutCtrl->AddTreeItems((CTTreeItemArray*&)pItems);
		}
	}else
	if(pOutputCtrl->nCtrlType == OUTPUT_TREE)
	{
		CTreeOutCtrl* pOutCtrl = (CTreeOutCtrl*)(pOutputCtrl->pWnd);
		if(pOutCtrl && pOutCtrl->m_hWnd)
		{
			pOutCtrl->AddTreeItems((CTTreeItemArray*&)pItems);
		}
	}else
	{
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////
// 向Trace窗口输出信息,可以改变格式和颜色
//////////////////////////////////////////////////////////////////
int OUTPUT_COLOR(COLORREF color, LPCTSTR format, ...)
{
	CMainFrame* pMainFrm = (CMainFrame*)(theApp.m_pMainWnd);
	if(!pMainFrm)
	{
		return FALSE;
	}
	
	COutputWndCtrl* pOutputCtrl = pMainFrm->GetOutputCtrl(IDR_PANE_OUTPUT);
	if(pOutputCtrl == NULL)
	{
		return FALSE;
	}

	COutputCtrl* pOutCtrl = (COutputCtrl*)(pOutputCtrl->pWnd);
	if(pOutCtrl && pOutCtrl->m_hWnd)
	{
		pOutCtrl->SetTextColor(color);
		va_list apList;
		va_start( apList, format );     /* Initialize variable arguments. */
		pOutCtrl->vsWrite( format, apList);
		va_end( apList ) ;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////
// 向某个窗口输出信息,可以改变格式和颜色
//////////////////////////////////////////////////////////////////
int OUTPUT_COLOR_ID(int nID, COLORREF color, LPCTSTR format, ...)
{
	CMainFrame* pMainFrm = (CMainFrame*)(theApp.m_pMainWnd);
	if(!pMainFrm)
	{
		return FALSE;
	}
	
	COutputWndCtrl* pOutputCtrl = pMainFrm->GetOutputCtrl(nID+IDR_PANE_OUTPUT);
	if(pOutputCtrl == NULL)
	{
		return FALSE;
	}

	COutputCtrl* pOutCtrl = (COutputCtrl*)(pOutputCtrl->pWnd);
	if(pOutCtrl && pOutCtrl->m_hWnd)
	{
		pOutCtrl->SetTextColor(color);
		va_list apList;
		va_start( apList, format );     /* Initialize variable arguments. */
		pOutCtrl->vsWrite( format, apList);
		va_end( apList ) ;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////
// 向指定窗口输出信息,可以改变颜色
//////////////////////////////////////////////////////////////////
int OutputText(int nID, CString strText, COLORREF color)
{
	return OUTPUT_COLOR_ID(nID, color, strText);
}

//////////////////////////////////////////////////////////////////
// 向Trace窗口输出信息,可以改变格式
//////////////////////////////////////////////////////////////////
int OUTPUT(int nFmt, LPCTSTR format, ...)
{
	CMainFrame* pMainFrm = (CMainFrame*)(theApp.m_pMainWnd);
	if(!pMainFrm)
	{
		return FALSE;
	}
	
	COutputWndCtrl* pOutputCtrl = pMainFrm->GetOutputCtrl(IDR_PANE_OUTPUT);
	if(pOutputCtrl == NULL)
	{
		return FALSE;
	}

	COutputCtrl* pOutCtrl = (COutputCtrl*)(pOutputCtrl->pWnd);
	if(pOutCtrl && pOutCtrl->m_hWnd)
	{
		pOutCtrl->SetFormat(nFmt);
		va_list apList;
		va_start( apList, format );     /* Initialize variable arguments. */
		pOutCtrl->vsWrite( format, apList);
		va_end( apList ) ;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////
// 向某个窗口输出信息
//////////////////////////////////////////////////////////////////
int OUTPUT_ID(int nID, LPCTSTR format, ...)
{
	CMainFrame* pMainFrm = (CMainFrame*)(theApp.m_pMainWnd);
	if(!pMainFrm)
	{
		return FALSE;
	}
	
	COutputWndCtrl* pOutputCtrl = pMainFrm->GetOutputCtrl(nID+IDR_PANE_OUTPUT);
	if(pOutputCtrl == NULL)
	{
		return FALSE;
	}

	if(pOutputCtrl->nCtrlType == OUTPUT_RICH)
	{
		COutputCtrl* pOutCtrl = (COutputCtrl*)(pOutputCtrl->pWnd);
		if(pOutCtrl && pOutCtrl->m_hWnd)
		{
			va_list apList;
			va_start( apList, format );     /* Initialize variable arguments. */
			pOutCtrl->vsWrite( format, apList);
			va_end( apList ) ;
		}
	}else
	if(pOutputCtrl->nCtrlType == OUTPUT_GRID)
	{
		CReportOutCtrl* pOutCtrl = (CReportOutCtrl*)(pOutputCtrl->pWnd);
		if(pOutCtrl && pOutCtrl->m_hWnd)
		{
			va_list apList;
			va_start( apList, format );     /* Initialize variable arguments. */
			pOutCtrl->vsWrite( format, apList);
			va_end( apList ) ;
		}
	}else
	if(pOutputCtrl->nCtrlType == OUTPUT_TREE)
	{
		CTreeOutCtrl* pOutCtrl = (CTreeOutCtrl*)(pOutputCtrl->pWnd);
		if(pOutCtrl && pOutCtrl->m_hWnd)
		{
			va_list apList;
			va_start( apList, format );     /* Initialize variable arguments. */
			pOutCtrl->vsWrite( format, apList);
			va_end( apList ) ;
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////
// 向某个窗口输出信息(不用解析转义符)
//////////////////////////////////////////////////////////////////
int OUTPUT_ID_DIRECT(int nID, LPCTSTR strOut)
{
	CMainFrame* pMainFrm = (CMainFrame*)(theApp.m_pMainWnd);
	if(!pMainFrm)
	{
		return FALSE;
	}
	
	COutputWndCtrl* pOutputCtrl = pMainFrm->GetOutputCtrl(nID+IDR_PANE_OUTPUT);
	if(pOutputCtrl == NULL)
	{
		return FALSE;
	}

	if(pOutputCtrl->nCtrlType == OUTPUT_RICH)
	{
		COutputCtrl* pOutCtrl = (COutputCtrl*)(pOutputCtrl->pWnd);
		if(pOutCtrl && pOutCtrl->m_hWnd)
		{
			pOutCtrl->WriteDirect(strOut);
		}
	}else
	if(pOutputCtrl->nCtrlType == OUTPUT_GRID)
	{
		CReportOutCtrl* pOutCtrl = (CReportOutCtrl*)(pOutputCtrl->pWnd);
		if(pOutCtrl && pOutCtrl->m_hWnd)
		{
			pOutCtrl->Output(strOut);
		}
	}else
	if(pOutputCtrl->nCtrlType == OUTPUT_TREE)
	{
		CTreeOutCtrl* pOutCtrl = (CTreeOutCtrl*)(pOutputCtrl->pWnd);
		if(pOutCtrl && pOutCtrl->m_hWnd)
		{
			pOutCtrl->Output(strOut);
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////
// 向Trace窗口输出信息,和校验字符串比较，不一致的位显示为红色
//////////////////////////////////////////////////////////////////
BOOL OUTPUT_VERIFY(CString strActual, CString strVerify)
{
	CMainFrame* pMainFrm = (CMainFrame*)(theApp.m_pMainWnd);
	if(!pMainFrm)
	{
		return FALSE;
	}
	COutputWndCtrl* pOutputCtrl = pMainFrm->GetOutputCtrl(IDR_PANE_OUTPUT);
	if(pOutputCtrl == NULL)
	{
		return FALSE;
	}

	COutputCtrl* pOutCtrl = (COutputCtrl*)(pOutputCtrl->pWnd);
	if(!pOutCtrl || !pOutCtrl->m_hWnd)
	{
		return FALSE;
	}

	BOOL bValid = TRUE;
	int nLenVerify = strVerify.GetLength();
	CString strTemp;
	for(int i=0; i<strActual.GetLength(); i++)
	{
		strTemp = strActual[i];
		if((i < nLenVerify) && (strVerify[i] != 'X') && (strVerify[i] != 'x') && (strActual[i] != strVerify[i]))
		{
			pOutCtrl->SetFormat(COLOR_ERROR);
			bValid = FALSE;
		} else
		{
			pOutCtrl->SetFormat(COLOR_NORMAL);
		}
		pOutCtrl->WriteDirect(strTemp);
	}

	return bValid;
}

//////////////////////////////////////////////////////////////////
// 不输出信息,只是比较,和校验字符串比较
//////////////////////////////////////////////////////////////////
BOOL STRING_VERIFY(CString strActual, CString strVerify)
{
	BOOL bValid = TRUE;
	int nLenVerify = strVerify.GetLength();
	CString strTemp;
	for(int i=0; i<strActual.GetLength(); i++)
	{
		strTemp = strActual[i];
		if((i < nLenVerify) && (strVerify[i] != 'X') && (strVerify[i] != 'x') && (strActual[i] != strVerify[i]))
		{
			bValid = FALSE;
		}
	}

	return bValid;
}

//////////////////////////////////////////////////////////////////
// 将字符串补空格到指定长度
//////////////////////////////////////////////////////////////////
void StringFillSpace(CString& strLine, int nLen)
{
	int nOldLen = strLine.GetLength();
	for(int i=nOldLen; i<nLen; i++)
	{
		strLine += _T(" ");
	}
}

//////////////////////////////////////////////////////////////////
// 反转字符串，倒序输出
//////////////////////////////////////////////////////////////////
CString ReverseString(CString strInput)
{
	CString strOutput = "";
	int nLen = strInput.GetLength();
	for(int i=nLen-1; i>=0; i--)
	{
		strOutput += strInput[i];
	}
	return strOutput;
}
