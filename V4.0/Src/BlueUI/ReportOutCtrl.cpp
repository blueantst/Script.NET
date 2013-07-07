// TreeOutCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "BlueUI.h"
#include "ReportOutCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportOutCtrl

CReportOutCtrl::CReportOutCtrl()
{
	m_pLastRecord = NULL;
	m_strSeparator = _T(",");
	m_bSetColor = FALSE;
}

CReportOutCtrl::~CReportOutCtrl()
{
}


BEGIN_MESSAGE_MAP(CReportOutCtrl, CXTPReportControl)
	//{{AFX_MSG_MAP(CReportOutCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_OEDIT_CLEAR, OnOeditClear)
	ON_COMMAND(ID_OUTCTRL_DELROW, OnOutctrlDelrow)
	ON_COMMAND(ID_OUTCTRL_SCRIPT, OnOutctrlScript)
	ON_UPDATE_COMMAND_UI(ID_OUTCTRL_SCRIPT, OnUpdateOutctrlScript)
	ON_UPDATE_COMMAND_UI(ID_OEDIT_CLEAR, OnUpdateOeditClear)
	ON_UPDATE_COMMAND_UI(ID_OUTCTRL_DELROW, OnUpdateOutctrlDelrow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CReportOutCtrl message handlers

BOOL CReportOutCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	//dwStyle |= (ES_AUTOVSCROLL | ES_MULTILINE| ES_AUTOHSCROLL | ES_READONLY 
	//	| WS_BORDER | WS_TABSTOP | WS_VSCROLL | WS_HSCROLL);
	dwStyle |= (ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_READONLY | WS_TABSTOP | WS_BORDER); 
	if(!CXTPReportControl::Create( dwStyle, rect, pParentWnd, nID))
		return FALSE;

	SetGridStyle(FALSE, (XTPReportGridStyle)xtpReportGridSmallDots);
	SetGridStyle(TRUE, (XTPReportGridStyle)xtpReportGridSmallDots);

	//GetPaintManager()->m_columnStyle = xtpReportColumnOffice2003;
	GetPaintManager()->SetColumnStyle(xtpReportColumnOffice2007);

	m_ilIcons.Create(16, 16, ILC_MASK|ILC_COLOR24, 0, 1);

	CBitmap bmp;
	VERIFY(bmp.LoadBitmap(IDB_REPORT_CTRL));
	m_ilIcons.Add(&bmp, CXTPImageManager::GetBitmapMaskColor(bmp, CPoint(0, 0)));

	SetImageList(&m_ilIcons);

	// 初始化颜色表
	AppendColor(RGB(255,255,225));	// 淡黄
	AppendColor(RGB(0,0,0));		// 黑
	AppendColor(RGB(255,255,255));	// 白
	AppendColor(RGB(255,0,0));		// 红
	AppendColor(RGB(0,255,0));		// 绿
	AppendColor(RGB(0,0,255));		// 蓝
	AppendColor(RGB(128,128,128));	// 灰
	AppendColor(RGB(128,0,0));		// 半红
	AppendColor(RGB(0,128,0));		// 半绿
	AppendColor(RGB(0,0,128));		// 半蓝

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 添加颜色表
/////////////////////////////////////////////////////////////////////////////
void CReportOutCtrl::AppendColor(COLORREF color)
{
	m_aColorTable.Add(color);
}

/////////////////////////////////////////////////////////////////////////////
// 设置颜色表
/////////////////////////////////////////////////////////////////////////////
void CReportOutCtrl::SetColor(int nIndex, COLORREF color)
{
	if(nIndex < m_aColorTable.GetSize())
	{
		m_aColorTable.SetAt(nIndex, color);
	}
}

/////////////////////////////////////////////////////////////////////////////
// 设置颜色表
/////////////////////////////////////////////////////////////////////////////
COLORREF CReportOutCtrl::GetColor(int nIndex)
{
	if(nIndex < m_aColorTable.GetSize())
	{
		return m_aColorTable.GetAt(nIndex);
	}
	return RGB(0,0,0);
}

/////////////////////////////////////////////////////////////////////////////
// 删除所有内容
/////////////////////////////////////////////////////////////////////////////
void CReportOutCtrl::Clear()
{
	// 删除所有树节点对象
	GetRecords()->RemoveAll();
	GetColumns()->Clear();
	m_bSetColor = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 关闭窗口时执行的脚本
/////////////////////////////////////////////////////////////////////////////
void CReportOutCtrl::DoCloseScript() 
{
	// 删除所有树节点对象
	GetRecords()->RemoveAll();
	GetColumns()->Clear();

	if(m_strScriptClose.GetLength() == 0)
		return;

	// 执行窗口关闭前的脚本
	CString strInterpInstance = _T("");

	// 解析解释器名(支持命名方式的解释器)
	if(m_strScriptClose.Find("<<") == 0)
	{
		int nPos = m_strScriptClose.Find(">>");
		if(nPos != -1)
		{
			m_strScriptClose.Delete(0, 2);
			strInterpInstance = m_strScriptClose.Left(nPos-2);
			m_strScriptClose.Delete(0, nPos);
		}
	}

	CString strInterpName = "";	// 解释器名
	if(m_strScriptClose.Find("<") == 0)
	{
		m_strScriptClose.Delete(0, 1);
		int nPos = m_strScriptClose.Find(">");
		if(nPos != -1)
		{
			strInterpName = m_strScriptClose.Left(nPos);
			if(strInterpName.Find("org.interp") != 0)
			{
				strInterpName = "org.interp." + strInterpName;
			}
			m_strScriptClose.Delete(0, nPos+1);
		}		
	}

	// 创建解释器并执行
	if(strInterpName.GetLength() > 0)
	{
		IInterp* pInterp = (IInterp*)(theApp.CreateVciObject(strInterpName, strInterpInstance));
		if(pInterp)
		{
			pInterp->RunScriptCommand(m_strScriptClose);
			theApp.ReleaseObject(pInterp);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 拆分成若干行
/////////////////////////////////////////////////////////////////////////////
void CReportOutCtrl::ParseLines(CString strSet, CStringArray& aLines)
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
// 分析style
/////////////////////////////////////////////////////////////////////////////
XTPReportGridStyle CReportOutCtrl::ParseStyle(CString strStyle)
{
	if(strStyle == _T("NOLINES"))
		return xtpReportGridNoLines;
	else if(strStyle == _T("SMALLDOTS"))
		return xtpReportGridSmallDots;
	else if(strStyle == _T("LARGEDOTS"))
		return xtpReportGridLargeDots;
	else if(strStyle == _T("DASHES"))
		return xtpReportGridDashes;
	else if(strStyle == _T("SOLID"))
		return xtpReportGridSolid;
	return xtpReportGridSolid;
}

/////////////////////////////////////////////////////////////////////////////
// 分析单个行
// 语法规则：
// 1.可以有若干个设置项，用";"隔开
// 2.添加列: ADDCOLUMN name,width,format
// 3.删除列: DELCOLUMN nCol|all
// 4.删除行: DELITEM nItem|all
// 5.设置分割符: SEPARATOR=???
// 6.设置所有行的颜色: SETROWCOLORS cText cTextBk
// 7.设置颜色表的某个颜色: SETCOLORTABLE index color
// 8.修改控件风格: MODIFYSTYLE vertical|horizontal style
// 9.从注册表读取列信息: READPROFILE Section, Entry
// 10.向注册表写入列信息: WRITEPROFILE Section, Entry
// 11.设置窗口关闭时执行的脚本: SETCLOSESCRIPT script
// 12.设置字体: SETFONT font size
/////////////////////////////////////////////////////////////////////////////
void CReportOutCtrl::ParseALine(CString strLine)
{
	CString strSet = strLine;
	strSet.MakeUpper();
	int pos;
	if((pos = strSet.Find("ADDCOLUMN ")) == 0)	// 添加列
	{
		CString strColName;
		CString strFormat = _T("");
		strSet = strLine.Right(strLine.GetLength()-10);
		int nPos = strSet.Find(",");
		strColName = strSet.Left(nPos);
		strSet = strSet.Right(strSet.GetLength()-nPos-1);

		// 解析列宽度
		nPos = strSet.Find(",");
		if(nPos != -1)
		{
			strFormat = strSet.Right(strSet.GetLength()-nPos-1);
			strFormat.MakeUpper();
			strSet = strSet.Left(nPos);
		}
		int nWidth = atoi(strSet);

		// 解析AutoSize
		BOOL bAutoSize = (strFormat.Find("AUTOSIZE") == 0);
		if(bAutoSize)
		{
			strFormat.Delete(0, 8);
			if((strFormat.GetLength() > 0) && (strFormat.Find(",") == 0))
			{
				strFormat.Delete(0, 1);
			}
		}

		// 解析对齐方式和图标
		CString strImage = _T("");
		nPos = strFormat.Find(",");
		if(nPos != -1)
		{
			strImage = strFormat.Right(strFormat.GetLength()-nPos-1);
			strImage.MakeUpper();
			strFormat = strFormat.Left(nPos);
		}

		int nAlignment = DT_LEFT;
		if(strFormat == _T("CENTER"))
		{
			nAlignment = DT_CENTER;
		}else
		if(strFormat == _T("RIGHT"))
		{
			nAlignment = DT_RIGHT;
		}

		int nImage = XTP_REPORT_NOICON;
		if(strImage.GetLength() > 0)
		{
			nImage = atoi(strImage);
		}

		CXTPReportColumn* pColumn = AddColumn(new CXTPReportColumn(GetColumns()->GetCount(), strColName, nWidth, bAutoSize, nImage));
		if(pColumn)
		{
			pColumn->EnableResize(TRUE);
			pColumn->SetHeaderAlignment(nAlignment);
		}
	}else
	if((pos = strSet.Find("DELCOLUMN ")) == 0)	// 删除列
	{
		strSet = strSet.Right(strSet.GetLength()-10);
		if(strSet == _T("ALL"))
		{
			GetColumns()->Clear();
			m_asItemScript.RemoveAll();
		}else
		{
			int nCol = atoi(strSet);
			GetColumns()->Remove(GetColumns()->GetAt(nCol));
			if(GetColumns()->GetCount() == 0)
			{
				m_asItemScript.RemoveAll();
			}
		}
	}else
	if((pos = strSet.Find("DELITEM ")) == 0)	// 删除行
	{
		strSet = strSet.Right(strSet.GetLength()-8);
		if(strSet == _T("ALL"))
		{
			GetRecords()->RemoveAll();
			m_asItemScript.RemoveAll();
		}else
		{
			int nItem = atoi(strSet);
			GetRecords()->RemoveAt(nItem);
			if(GetRecords()->GetCount() == 0)
			{
				m_asItemScript.RemoveAll();
			}
		}
	}else
	if((pos = strSet.Find("SEPARATOR=")) == 0)	// 设置分割符
	{
		m_strSeparator = strLine.Right(strLine.GetLength()-10);
	}else
	if((pos = strSet.Find("SETROWCOLORS ")) == 0)	// 设置所有行颜色
	{
		strSet = strSet.Right(strSet.GetLength()-13);
		CString strBkColor = _T("");
		int nPos = strSet.Find(" ");
		if(nPos != -1)
		{
			strBkColor = strSet.Right(strSet.GetLength()-nPos-1);
			strSet = strSet.Left(nPos);
		}
		COLORREF cText = RGB(0,0,0);
		COLORREF cTextBk = RGB(255,255,255);
		int nPos1;
		if((nPos1 = strSet.Find("RGB(")) == 0)
		{
			strSet = strSet.Right(strSet.GetLength()-pos-4);
			strSet.Delete(strSet.GetLength()-1, 1);
			nPos1 = strSet.Find(",");
			CString strTmp = strSet.Left(nPos1);
			int nr = atoi(strTmp);
			strSet = strSet.Mid(nPos1 + 1, strSet.GetLength() - nPos1 -1);
			nPos1 = strSet.Find(",");
			strTmp = strSet.Left(nPos1);
			int ng = atoi(strTmp);
			strTmp = strSet.Mid(nPos1 + 1, strSet.GetLength() - nPos1 -1);
			int nb = atoi(strTmp);
			cText = RGB(nr, ng, nb);
		}
		strSet = strBkColor;
		if((nPos1 = strSet.Find("RGB(")) == 0)
		{
			strSet = strSet.Right(strSet.GetLength()-pos-4);
			strSet.Delete(strSet.GetLength()-1, 1);
			nPos1 = strSet.Find(",");
			CString strTmp = strSet.Left(nPos1);
			int nr = atoi(strTmp);
			strSet = strSet.Mid(nPos1 + 1, strSet.GetLength() - nPos1 -1);
			nPos1 = strSet.Find(",");
			strTmp = strSet.Left(nPos1);
			int ng = atoi(strTmp);
			strTmp = strSet.Mid(nPos1 + 1, strSet.GetLength() - nPos1 -1);
			int nb = atoi(strTmp);
			cTextBk = RGB(nr, ng, nb);
		}
		m_clText = cText;
		m_clBkGnd = cTextBk;
		m_bSetColor = TRUE;
		/*
		BeginUpdate();
		for(int i=0; i<GetRows()->GetCount(); i++)
		{
			CXTPReportRow* pRow = GetRows()->GetAt(i);
			CXTPReportRecord* pRecord = pRow->GetRecord();
			for(int j=0; j<pRecord->GetItemCount(); j++)
			{
				CXTPReportRecordItem* pItem = pRecord->GetItem(j);
				if(pItem)
				{
					pItem->SetTextColor(cText);
					pItem->SetBackgroundColor(cTextBk);
				}
			}
		}
		EndUpdate();
		*/
	}else
	if((pos = strSet.Find("SETGRIDCOLOR ")) == 0)	// 设置表格线颜色
	{
		strSet = strSet.Right(strSet.GetLength()-13);
		COLORREF cGrid = RGB(0,0,0);
		int nPos1;
		if((nPos1 = strSet.Find("RGB(")) == 0)
		{
			strSet = strSet.Right(strSet.GetLength()-pos-4);
			strSet.Delete(strSet.GetLength()-1, 1);
			nPos1 = strSet.Find(",");
			CString strTmp = strSet.Left(nPos1);
			int nr = atoi(strTmp);
			strSet = strSet.Mid(nPos1 + 1, strSet.GetLength() - nPos1 -1);
			nPos1 = strSet.Find(",");
			strTmp = strSet.Left(nPos1);
			int ng = atoi(strTmp);
			strTmp = strSet.Mid(nPos1 + 1, strSet.GetLength() - nPos1 -1);
			int nb = atoi(strTmp);
			cGrid = RGB(nr, ng, nb);
		}
		SetGridColor(cGrid);
	}else
	if((pos = strSet.Find("SETCOLORTABLE ")) == 0)	// 设置颜色表的某个颜色
	{
		strSet = strSet.Right(strSet.GetLength()-14);
		int nIndex = 0;
		int nPos = strSet.Find(" ");
		if(nPos != -1)
		{
			CString strIndex = strSet.Left(nPos);
			nIndex = atoi(strIndex);
			if(nIndex<0 || nIndex>9)
				return;
			strSet.Delete(0, nPos+1);
		}
		if(strSet.Find("RGB(") == 0)
		{
			strSet = strSet.Right(strSet.GetLength()-pos-4);
			strSet.Delete(strSet.GetLength()-1, 1);
			int nPos1 = strSet.Find(",");
			CString strTmp = strSet.Left(nPos1);
			int nr = atoi(strTmp);
			strSet = strSet.Mid(nPos1 + 1, strSet.GetLength() - nPos1 -1);
			nPos1 = strSet.Find(",");
			strTmp = strSet.Left(nPos1);
			int ng = atoi(strTmp);
			strTmp = strSet.Mid(nPos1 + 1, strSet.GetLength() - nPos1 -1);
			int nb = atoi(strTmp);
			COLORREF cColor = RGB(nr, ng, nb);
			SetColor(nIndex, cColor);
		}
	}else
	if((pos = strSet.Find("MODIFYSTYLE ")) == 0)	// 修改控件风格
	{
		strSet = strSet.Right(strSet.GetLength()-12);
		BOOL bVertical = TRUE;
		if(strSet.Find("VERTICAL ") == 0)
		{
			strSet.Delete(0, 9);
		}else
		if(strSet.Find("HORIZONTAL ") == 0)
		{
			bVertical = FALSE;
			strSet.Delete(0, 11);
		}
		SetGridStyle(bVertical, ParseStyle(strSet));
	}else
	if((pos = strSet.Find("SETTREECOLUMN ")) == 0)	// 设置树的列
	{
		strSet = strSet.Right(strSet.GetLength()-14);
		CXTPReportColumn* pColumn = GetColumns()->Find(atoi(strSet));
		if(pColumn)
		{
			pColumn->SetTreeColumn(TRUE);
		}
	}else
		/*
	if(strSet.Find("READPROFILE ") == 0)	// 从注册表读取列信息
	{
		strLine = strLine.Right(strLine.GetLength()-12);
		int nPos = strLine.Find(",");
		if(nPos != -1)
		{
			CString strSection = strLine.Left(nPos);
			CString strEntry = strLine.Right(strLine.GetLength()-nPos-1);
			GetProfile(strSection, strEntry);
		}
	}else
	if(strSet.Find("WRITEPROFILE ") == 0)	// 向注册表写入列信息
	{
		strLine = strLine.Right(strLine.GetLength()-13);
		int nPos = strLine.Find(",");
		if(nPos != -1)
		{
			CString strSection = strLine.Left(nPos);
			CString strEntry = strLine.Right(strLine.GetLength()-nPos-1);
			WriteProfile(strSection, strEntry);
		}
	}else*/
	if(strSet.Find("SETCLOSESCRIPT ") == 0)	// 设置窗口关闭时执行的脚本
	{
		m_strScriptClose = strLine.Right(strLine.GetLength()-15);
	}
}

//////////////////////////////////////////////////////////////////
// 设置输出格式
//////////////////////////////////////////////////////////////////
void CReportOutCtrl::SetCtrlFormat(CString strSet)
{
	CStringArray aLines;
	// 拆分成若干行
	ParseLines(strSet, aLines);
	for(int i=0; i<aLines.GetSize(); i++)
	{
		CString strLine = aLines[i];
		ParseALine(strLine);	// 分析行
	}
}

//////////////////////////////////////////////////////////////////
// 获取指定项的Record对象指针
// item前加#表示后面跟的是指针
//////////////////////////////////////////////////////////////////
CXTPReportRecord* CReportOutCtrl::GetItemRecord(CString strItem)
{
	if(strItem.Find("#") == 0)
	{
		strItem.Delete(0, 1);
		return (CXTPReportRecord*)(atoi(strItem));
	}else
	{
		int nItem = atoi(strItem);
		if((nItem >= 0) && (nItem < GetRows()->GetCount()))
		{
			CXTPReportRecord* pRecord = GetRows()->GetAt(nItem)->GetRecord();
			return pRecord;
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////
// 输出信息
// 语法规则：
// 1.设置单项文字: <I>SET ITEM<nItem,nSubItem> TEXT XXXX
// 2.设置单项图标: <I>SET ITEM<nItem,nSubItem> IMAGE nImage
// 3.设置单项检查标记: <I>SET ITEM<nItem,nSubItem> CHECK nCheck
// 4.设置文字颜色: <I>SET ITEM<nItem,nSubItem> COLOR colorIndex
// 5.设置文字状态: <I>SET ITEM<nItem,nSubItem> STATE ?BOLD?
// 6.设置行双击事件脚本: <I>SET SCRIPT<nRow> <nIndex>|script
// 7.添加扩展菜单: <I>ADDEXMENU menu script
// 8.清除扩展菜单: <I>CLEAREXMENU
// 9.添加行: ?<hParentItem,image>?XXX,XXX,XXX,XXX
// 10.展开树节点: <T>EXPANDITEM<hItem>
//////////////////////////////////////////////////////////////////
void CReportOutCtrl::Output(CString strOutput)
{
	if(strOutput == "\r\n")
	{
		return;
	}

	if(strOutput.Find("<I>") == 0)	// 单项操作
	{
		strOutput = strOutput.Right(strOutput.GetLength()-3);
		strOutput.TrimLeft();
		CString strSet = strOutput;
		strSet.MakeUpper();
		if(strSet.Find("SET ITEM<") == 0)	// 设置单项内容
		{
			int nPos = strSet.Find("> ");
			if(nPos == -1)
				return;
			strOutput = strOutput.Right(strOutput.GetLength()-nPos-2);
			strSet = strSet.Mid(9, nPos-9);
			nPos = strSet.Find(",");
			if(nPos == -1)
				return;
			CString strItem = strSet.Left(nPos);
			CXTPReportRecord* pRecord = GetItemRecord(strItem);
			if(!pRecord)
				return;

			strSet = strSet.Right(strSet.GetLength()-nPos-1);
			int nSubItem = atoi(strSet);
			if(nSubItem >= GetColumns()->GetVisibleColumnsCount())
				return;

			while(pRecord->GetItemCount() < (nSubItem+1))
			{
				pRecord->AddItem(new CXTPReportRecordItemText(""));
			}

			strSet = strOutput;
			strSet.MakeUpper();
			if(strSet.Find("TEXT ") == 0)	// 设置单项文字
			{
				strOutput = strOutput.Right(strOutput.GetLength()-5);
				CXTPReportRecordItem* pItem = pRecord->GetItem(nSubItem);
				pItem->SetCaption(strOutput);
				if(m_bSetColor)
				{
					pItem->SetTextColor(m_clText);
					pItem->SetBackgroundColor(m_clBkGnd);
				}
			}else
			if(strSet.Find("IMAGE ") == 0)	// 设置单项图标
			{
				strOutput = strOutput.Right(strOutput.GetLength()-6);
				int nImage = atoi(strOutput);
				CXTPReportRecordItem* pItem = pRecord->GetItem(nSubItem);
				if(pItem)
				{
					pItem->SetIconIndex(nImage);
				}
			}else
			if(strSet.Find("CHECK ") == 0)	// 设置单项检查标记
			{
				strOutput = strOutput.Right(strOutput.GetLength()-6);
				int nCheck = atoi(strOutput);
				CXTPReportRecordItem* pItem = pRecord->GetItem(nSubItem);
				if(pItem)
				{
					pItem->HasCheckbox(TRUE);
					pItem->SetChecked(nCheck);
				}
			}else
			if(strSet.Find("COLOR ") == 0)	// 设置文字颜色
			{
				strOutput = strOutput.Right(strOutput.GetLength()-6);
				int nColor = atoi(strOutput);	// 颜色表的索引

				if(nSubItem == -1)	// -1表示设置整行的颜色
				{
					for(int i=0; i<pRecord->GetItemCount(); i++)
					{
						CXTPReportRecordItem* pItem = pRecord->GetItem(nSubItem);
						if(pItem)
						{
							pItem->SetTextColor(GetColor(nColor));
						}
					}
				}else
				{
					CXTPReportRecordItem* pItem = pRecord->GetItem(nSubItem);
					if(pItem)
					{
						pItem->SetTextColor(GetColor(nColor));
					}
				}
			}else
			if(strSet.Find("STATE ") == 0)	// 设置文字状态
			{
				strOutput = strOutput.Right(strOutput.GetLength()-6);
				strOutput.MakeUpper();
				BOOL bBold = FALSE;
				if(strOutput == "BOLD")
					bBold = TRUE;
				if(nSubItem == -1)	// -1表示设置整行的状态
				{
					for(int i=0; i<pRecord->GetItemCount(); i++)
					{
						CXTPReportRecordItem* pItem = pRecord->GetItem(nSubItem);
						if(pItem)
						{
							pItem->SetBold(bBold);
						}
					}
				}else
				{
					CXTPReportRecordItem* pItem = pRecord->GetItem(nSubItem);
					if(pItem)
					{
						pItem->SetBold(bBold);
					}
				}
			}
		}else
		if(strSet.Find("SET SCRIPT<") == 0)	// 设置行的双击事件脚本
		{
			strSet = strOutput.Right(strOutput.GetLength()-11);
			// 获取行号
			int nPos = strSet.Find("> ");
			if(nPos == -1)
				return;
			CString strItem = strSet.Left(nPos);
			strSet = strSet.Right(strSet.GetLength()-nPos-2);
			if((strSet.Find("<")==0) && (strSet.Find("<<")!=0))	// 使用以前定义的某个脚本
			{
				strSet.Delete(0, 1);
				int nPos = strSet.Find(">");
				if(nPos == -1)
					return;
				strSet = strSet.Left(nPos);
				CXTPReportRecord* pRecord = GetItemRecord(strItem);
				if(pRecord)
				{
					CXTPReportRecordItem* pItem = pRecord->GetItem(0);
					if(pItem)
					{
						pItem->SetItemData(atoi(strSet));
					}
				}
				
				return;
			}
			// 添加脚本,设置链接(注:设置的行号连接是从1开始计数的,因为ItemData缺省为0)
			m_asItemScript.Add(strSet);
			CXTPReportRecord* pRecord = GetItemRecord(strItem);
			if(pRecord)
			{
				CXTPReportRecordItem* pItem = pRecord->GetItem(0);
				if(pItem)
				{
					pItem->SetItemData(m_asItemScript.GetSize());
				}
			}
		}/*else
		if(strSet.Find("ADDEXMENU ") == 0)	// 添加扩展菜单
		{
			strOutput = strOutput.Right(strOutput.GetLength()-10);
			int nPos = strOutput.Find(" ");
			if(nPos != -1)
			{
				CString strScript = strOutput.Right(strOutput.GetLength()-nPos-1);
				strOutput = strOutput.Left(nPos);
				m_asExMenu.Add(strOutput);
				m_asExMenuScript.Add(strScript);
			}
		}else
		if(strSet.Find("CLEAREXMENU") == 0)	// 清除扩展菜单
		{
			m_asExMenu.RemoveAll();
			m_asExMenuScript.RemoveAll();
		}*/
	}else
	if(strOutput.Find("<T>") == 0)	// 树操作
	{
		strOutput = strOutput.Right(strOutput.GetLength()-3);
		strOutput.TrimLeft();
		CString strSet = strOutput;
		strSet.MakeUpper();
		if(strSet.Find("EXPANDITEM<") == 0)	// 展开树节点
		{
			int nPos = strSet.Find(">");
			if(nPos == -1)
				return;
			strSet = strSet.Mid(11, nPos-11);
			if(strSet == "ROOT")
			{
				ExpandAll();
			}else
			{
				CXTPReportRecord* pRecord = GetItemRecord(strSet);
				if(pRecord)
				{
					pRecord->SetExpanded(TRUE);
				}
			}
		}
	}else	// 添加整行
	{
		CString strItem = _T("");
		int nSepLen = m_strSeparator.GetLength();
		int nPos;
		int nImage = -1;
		CXTPReportRecord* pParentRecord = NULL;
		// 解析父节点和图标字段
		if(strOutput.Find("<") == 0)
		{
			nPos = strOutput.Find(">");
			if(nPos > 1)
			{
				CString strParam = strOutput.Mid(1, nPos-1);
				strOutput = strOutput.Right(strOutput.GetLength()-nPos-1);
				nPos = strParam.Find(",");
				if(nPos != -1)
				{
					CString strImage = strParam.Right(strParam.GetLength()-nPos-1);
					nImage=  atoi(strImage);
					strParam = strParam.Left(nPos);
				}
				if(strParam != "ROOT")
				{
					pParentRecord = (CXTPReportRecord*)(atoi(strParam));
				}
			}
		}
		// 解析行内容
		nPos = strOutput.Find(m_strSeparator);
		if(nPos != -1)
		{
			strItem = strOutput.Left(nPos);
			strOutput = strOutput.Right(strOutput.GetLength()-nPos-nSepLen);
		}else
		{
			strItem = strOutput;
		}
		m_pLastRecord = new CXTPReportRecord();
		m_pLastRecord->AddItem(new CXTPReportRecordItemText(strItem));
		if(m_bSetColor)
		{
			m_pLastRecord->GetItem(0)->SetTextColor(m_clText);
			m_pLastRecord->GetItem(0)->SetBackgroundColor(m_clBkGnd);
		}
		if(nImage != -1)
		{
			m_pLastRecord->GetItem(0)->SetIconIndex(nImage);
		}
		
		if(nPos != -1)
		{
			int nCount = 1;
			while((nPos = strOutput.Find(m_strSeparator)) != -1)
			{
				strItem = strOutput.Left(nPos);
				strOutput = strOutput.Right(strOutput.GetLength()-nPos-nSepLen);
				if(nCount >= GetColumns()->GetCount())
					break;
				m_pLastRecord->AddItem(new CXTPReportRecordItemText(strItem));
				if(m_bSetColor)
				{
					m_pLastRecord->GetItem(nCount)->SetTextColor(m_clText);
					m_pLastRecord->GetItem(nCount)->SetBackgroundColor(m_clBkGnd);
				}
				nCount++;
			}
			if(nCount < GetColumns()->GetCount())
			{
				m_pLastRecord->AddItem(new CXTPReportRecordItemText(strOutput));
				if(m_bSetColor)
				{
					m_pLastRecord->GetItem(nCount)->SetTextColor(m_clText);
					m_pLastRecord->GetItem(nCount)->SetBackgroundColor(m_clBkGnd);
				}
			}
		}

		if(pParentRecord == NULL)
		{
			AddRecord(m_pLastRecord);
		}else
		{
			pParentRecord->GetChilds()->Add(m_pLastRecord);
		}
	}

	// Populate all data on control
	Populate();
}

/////////////////////////////////////////////////////////////////////////////
// 格式化输出
/////////////////////////////////////////////////////////////////////////////
void CReportOutCtrl::vsWrite(LPCTSTR lpszFormat, va_list argList)
{
	ASSERT(AfxIsValidString(lpszFormat, FALSE));

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

	Output(p_ssData);	// 输出

	delete[] p_ssData;

	va_end(argListSave);
}

//////////////////////////////////////////////////////////////////
// 获取输出窗口信息
// 语法规则:
// 1.获取热点行句柄: HOTITEM
// 2.获取选择的行数: SELITEMCOUNT
// 3.获取选择的行: SELITEM ?nItem?
// 4.查找某个元素的行号: FINDITEM<nSubItem> item
// 5.获取单元内容: <nItem,nSubItem>
// 6.获取最后一次操作的树节点行句柄(Record对象): LASTITEM
// 7.获取某个节点的索引(行号): ITEMINDEX
// 8.获取某个节点的句柄: ITEMHANDLE
// 9.获取某个节点的子节点个数: ITEMCOUNT
// 10.获取某个节点的父节点句柄: ITEMPARENT
// 11.获取列数: COLUMNCOUNT
//////////////////////////////////////////////////////////////////
CString CReportOutCtrl::GetInfo(CString strParam)
{
	CString strRet = _T("");
	CString strSet = strParam;
	strSet.MakeUpper();
	if(strSet.Find("HOTITEM") == 0)	// 获取热点行句柄
	{
		CXTPReportRow* pRow = GetFocusedRow();
		/*int nIndex = -1;
		if(pRow)
		{
			nIndex = pRow->GetIndex();
		}
		strRet.Format("%d", nIndex);*/
		if(pRow)
		{
			CXTPReportRecord* pRecord = pRow->GetRecord();
			strRet.Format("%d", pRecord);
		}
	}else
	if(strSet.Find("SELITEMCOUNT") == 0)	// 获取选择的行数
	{
		int nCount = GetSelectedRows()->GetCount();
		strRet.Format("%d", nCount);
	}else
	if(strSet.Find("<") == 0)
	{
		int nPos = strSet.Find(">");
		if(nPos == -1)
			return _T("");
		strSet = strSet.Mid(1, nPos-1);
		nPos = strSet.Find(",");
		if(nPos == -1)
			return _T("");
		CString strItem = strSet.Left(nPos);
		strSet = strSet.Right(strSet.GetLength()-nPos-1);
		int nSubItem = atoi(strSet);
		CXTPReportRecord* pRecord = GetItemRecord(strItem);
		if(pRecord)
		{
			CXTPReportRecordItem* pItem = pRecord->GetItem(nSubItem);
			if(pItem)
			{
				strRet = pItem->GetCaption(GetColumns()->GetAt(nSubItem));
			}
		}
	}else
	if(strSet.Find("PARENTROW ") == 0)	// 获取某个节点的父节点行号
	{
		strSet = strSet.Right(strSet.GetLength()-10);
		int nItem = atoi(strSet);
		if(nItem>=0 && nItem<GetRows()->GetCount())
		{
			CXTPReportRow* pRow = GetRows()->GetAt(nItem);
			if(pRow && pRow->GetParentRow())
			{
				strRet.Format("%d", pRow->GetParentRow()->GetIndex());
			}
		}
	}else
	if(strSet.Find("CHILDCOUNT ") == 0)	// 获取某个节点的子节点个数
	{
		strRet = "0";
		strSet = strSet.Right(strSet.GetLength()-11);
		CXTPReportRecord* pRecord = (CXTPReportRecord*)(atoi(strSet));
		if(pRecord)
		{
			strRet.Format("%d", pRecord->GetChilds()->GetCount());
		}
	}else
	if(strSet.Find("LASTITEM") == 0)	// 获取最后一次添加的树节点行Record对象
	{
		if(m_pLastRecord)
		{
			strRet.Format("%d", m_pLastRecord);
		}
	}else
	if(strSet.Find("SELITEM") == 0)	// 获取选择的行句柄
	{
		strSet = strSet.Right(strSet.GetLength()-7);
		int nItem = 0;
		if(strSet.GetLength() > 0)
			nItem = atoi(strSet);
		CXTPReportSelectedRows* pSelectedRows = GetSelectedRows();
		if(pSelectedRows && (pSelectedRows->GetCount() > 0))
		{
			CXTPReportRow* pFocusedRow = pSelectedRows->GetAt(nItem);
			if(pFocusedRow)
			{
				strRet.Format("%d", pFocusedRow->GetRecord());
			}
		}
	}else
	if(strSet.Find("ITEMHANDLE ") == 0)	// 获取某个节点行的句柄
	{
		strSet = strSet.Right(strSet.GetLength()-11);
		int nIndex = atoi(strSet);
		CXTPReportRow* pRow = GetRows()->GetAt(nIndex);
		if(pRow)
		{
			CXTPReportRecord* pRecord = pRow->GetRecord();
			strRet.Format("%d", pRecord);
		}
	}else
	if(strSet.Find("ITEMINDEX ") == 0)	// 获取某个节点的索引(行号)
	{
		strSet = strSet.Right(strSet.GetLength()-10);
		CXTPReportRecord* pRecord = (CXTPReportRecord*)(atoi(strSet));
		for(int i=0; i<GetRows()->GetCount(); i++)
		{
			if(GetRows()->GetAt(i)->GetRecord() == pRecord)
			{
				strRet.Format("%d", i);
			}
		}
	}else
	if(strSet.Find("ITEMPARENT ") == 0)	// 获取某个节点的父节点句柄
	{
		strSet = strSet.Right(strSet.GetLength()-11);
		CXTPReportRecord* pRecord = (CXTPReportRecord*)(atoi(strSet));
		if(pRecord)
		{
			CXTPReportRecord* pParentRecord = pRecord->GetRecords()->GetOwnerRecord();
			if(pParentRecord)
			{
				strRet.Format("%d", pParentRecord);
			}else
			{
				strRet = "ROOT";
			}
		}
	}else
	if(strSet.Find("FINDITEM<") == 0)	// 查找某个元素的行号
	{
		int nPos = strSet.Find("> ");
		if(nPos == -1)
			return _T("");
		strSet = strSet.Mid(9, nPos-9);
		int nSubItem = atoi(strSet);
		if(nSubItem<0)
			return _T("");
		strParam = strParam.Right(strParam.GetLength()-nPos-2);
		for(int i=0; i<GetRows()->GetCount(); i++)
		{
			CXTPReportRow* pRow = GetRows()->GetAt(i);
			if(pRow)
			{
				CXTPReportRecord* pRecord = pRow->GetRecord();
				if(pRecord)
				{
					CXTPReportRecordItem* pItem = pRecord->GetItem(nSubItem);
					if(pItem)
					{
						if(pItem->GetCaption(GetColumns()->GetAt(nSubItem)) == strParam)
						{
							strRet.Format("%d", i);
							return strRet;
						}
					}
				}
			}
		}
		return _T("");
	}else
	if(strSet.Find("COLUMNCOUNT") == 0)	// 获取列数
	{
		strRet.Format("%d", GetColumns()->GetCount());
	}

	return strRet;
}

/////////////////////////////////////////////////////////////////////////////
// 添加树窗口内容
/////////////////////////////////////////////////////////////////////////////
void CReportOutCtrl::AddTreeItems(CTTreeItemArray*& pItems)
{
	if(pItems == NULL)
	{
		return;
	}

	// 首先删除所有行
	GetRecords()->RemoveAll();
	m_asItemScript.RemoveAll();

	int nSepLen = m_strSeparator.GetLength();
	CXTPReportRecord* pParentRecord = NULL;

	for(int i=0; i<pItems->GetSize(); i++)
	{
		LPTTREEITEM pItem = &(pItems->GetAt(i));
		if(pItem == NULL)
		{
			continue;
		}

		if(pItem->nParent < 0)
		{
			pParentRecord = NULL;
		}else
		if((pItem->nParent < pItems->GetSize()) && (pItem->nParent < i))
		{
			pParentRecord = (CXTPReportRecord*)((*pItems)[pItem->nParent].hItem);
		}

		// 解析行内容
		CString strItem = "";
		CString strOutput = pItem->strLine;
		int nPos = strOutput.Find(m_strSeparator);
		if(nPos != -1)
		{
			strItem = strOutput.Left(nPos);
			strOutput = strOutput.Right(strOutput.GetLength()-nPos-nSepLen);
		}else
		{
			strItem = strOutput;
		}
		CXTPReportRecord* pItemRow = new CXTPReportRecord();
		pItemRow->AddItem(new CXTPReportRecordItemText(strItem));
		pItem->hItem = pItemRow;
		(*pItems)[i].hItem = pItem->hItem;	// 树结构的hItem存放添加的Record
		if((*pItems)[i].hItem == NULL)
		{
			continue;
		}
		/*if(m_bSetColor)
		{
			pItemRow->GetItem(0)->SetTextColor(m_clText);
			pItemRow->GetItem(0)->SetBackgroundColor(m_clBkGnd);
		}*/
		if(pItem->nImage != -1)
		{
			pItemRow->GetItem(0)->SetIconIndex(pItem->nImage);
		}
		if(pItem->nClickFunc != 0)
		{
			CXTPReportRecordItem* pRecordItem = pItemRow->GetItem(0);
			if(pRecordItem)
			{
				pRecordItem->SetItemData(pItem->nClickFunc);
			}
		}

		if(nPos != -1)
		{
			int nCount = 1;
			while((nPos = strOutput.Find(m_strSeparator)) != -1)
			{
				strItem = strOutput.Left(nPos);
				strOutput = strOutput.Right(strOutput.GetLength()-nPos-nSepLen);
				if(nCount >= GetColumns()->GetCount())
					break;
				pItemRow->AddItem(new CXTPReportRecordItemText(strItem));
				if(m_bSetColor)
				{
					pItemRow->GetItem(nCount)->SetTextColor(m_clText);
					pItemRow->GetItem(nCount)->SetBackgroundColor(m_clBkGnd);
				}
				nCount++;
			}
			if(nCount < GetColumns()->GetCount())
			{
				pItemRow->AddItem(new CXTPReportRecordItemText(strOutput));
				/*if(m_bSetColor)
				{
					m_pLastRecord->GetItem(nCount)->SetTextColor(m_clText);
					m_pLastRecord->GetItem(nCount)->SetBackgroundColor(m_clBkGnd);
				}*/
			}
		}

		if(pParentRecord == NULL)
		{
			AddRecord(pItemRow);
		}else
		{
			pParentRecord->GetChilds()->Add(pItemRow);
			pParentRecord->SetExpanded(TRUE);
		}
	}

	// 把内容显示出来
	Populate();
}

typedef void (CALLBACK_ITEM_CLICK)(LPVOID lpObj, LPCTSTR lpszParams);

//////////////////////////////////////////////////////////////////
// 鼠标双击事件处理
//////////////////////////////////////////////////////////////////
void CReportOutCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{	
	CXTPReportControl::OnLButtonDblClk(nFlags, point);

	CXTPReportRow* pRow = HitTest(point);
	if(pRow == NULL)
	{
		return;
	}

	CXTPReportRecord* pActiveRecord = pRow->GetRecord();
	if(pActiveRecord == NULL)
	{
		return;
	}

	CXTPReportRecordItem* pItem = pActiveRecord->GetItem(0);
	if(pItem == NULL)
	{
		return;
	}

	int nIndex = pItem->GetItemData();
	if((nIndex > 0) && (nIndex <= m_asItemScript.GetSize()))
	{	// 执行双击操作脚本
		CString strScriptCommand = m_asItemScript[nIndex-1];
		CString strInterpInstance = _T("");

		// 解析解释器名(支持命名方式的解释器)
		if(strScriptCommand.Find("<<") == 0)
		{
			int nPos = strScriptCommand.Find(">>");
			if(nPos != -1)
			{
				strScriptCommand.Delete(0, 2);
				strInterpInstance = strScriptCommand.Left(nPos-2);
				strScriptCommand.Delete(0, nPos);
			}
		}

		if(strInterpInstance.Find("cfunction:") == 0)
		{
			// C回调函数
			strInterpInstance.Delete(0, 10);
			CALLBACK_ITEM_CLICK* callback_item_click = (CALLBACK_ITEM_CLICK*)(atoi(strInterpInstance));
			int nPos1 = strScriptCommand.Find(",");
			if(nPos1 != -1)
			{
				LPVOID lpObj = (LPVOID)(atoi(strScriptCommand.Left(nPos1)));
				strScriptCommand.Delete(0, nPos1+1);
				CString strTmp;
				strTmp.Format("%d,", pRow->GetIndex());
				strScriptCommand = strTmp + strScriptCommand;
				(*callback_item_click)(lpObj, (LPCTSTR)strScriptCommand);
			}
		}else
		{
			IInterp* pInterp = (IInterp*)(theApp.CreateVciObject("org.interp.tcl", strInterpInstance));
			if(pInterp)
			{
				pInterp->RunScriptCommand(strScriptCommand);
				//CString strRes = pInterp->GetResult();
				//TRACE(strRes);
				theApp.ReleaseObject(pInterp);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 右键菜单
/////////////////////////////////////////////////////////////////////////////
void CReportOutCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// 安装CoolMenu
	CMenu* pMenu = new CMenu();
	pMenu->LoadMenu(IDR_OUTPUT_GRID);
	CMenu* pmenu = (CMenu*)pMenu->GetSubMenu(0);
	ClientToScreen(&point);
	BOOL bEnable = (GetRecords()->GetCount() > 0);
	pmenu->EnableMenuItem(ID_OEDIT_CLEAR, bEnable ? MF_ENABLED : MF_GRAYED);
	pmenu->EnableMenuItem(ID_OUTCTRL_DELROW, bEnable ? MF_ENABLED : MF_GRAYED);
	pmenu->EnableMenuItem(ID_OFILE_SAVE, bEnable ? MF_ENABLED : MF_GRAYED);
	//pmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
	//		point.x, point.y, this);
	CXTPCommandBars::TrackPopupMenu(pmenu, 0, point.x, point.y, this);
	delete pMenu;
}

/////////////////////////////////////////////////////////////////////////////
// 全部清除
/////////////////////////////////////////////////////////////////////////////
void CReportOutCtrl::OnOeditClear() 
{
	BeginUpdate();
	GetRows()->Clear();
	EndUpdate();
}

void CReportOutCtrl::OnUpdateOeditClear(CCmdUI* pCmdUI) 
{
	BOOL bEnable = (GetRows()->GetCount() > 0);
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 删除行
/////////////////////////////////////////////////////////////////////////////
void CReportOutCtrl::OnOutctrlDelrow() 
{
	CXTPReportRow* pRow = GetFocusedRow();
	int nIndex = -1;
	if(pRow)
	{
		BeginUpdate();
		nIndex = pRow->GetIndex();
		GetRows()->RemoveAt(nIndex);
		EndUpdate();
	}
}

void CReportOutCtrl::OnUpdateOutctrlDelrow(CCmdUI* pCmdUI) 
{
	BOOL bEnable = (GetRows()->GetCount() > 0);
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 查看脚本
/////////////////////////////////////////////////////////////////////////////
void CReportOutCtrl::OnOutctrlScript() 
{
	CXTPReportRow* pRow = GetFocusedRow();
	int nIndex = -1;
	if(pRow)
	{
		CXTPReportRecord* pRecord = pRow->GetRecord();
		if(pRecord)
		{
			CXTPReportRecordItem* pItem = pRecord->GetItem(0);
			if(pItem)
			{
				nIndex = pItem->GetItemData();
			}
		}
	}

	if((nIndex > 0) && (nIndex <= m_asItemScript.GetSize()))
	{
		CString strScriptCommand = m_asItemScript[nIndex-1];
		CString strInterp = _T("");
		// 解析解释器名(支持命名方式的解释器)
		if(strScriptCommand.Find("<<") == 0)
		{
			int nPos = strScriptCommand.Find(">>");
			if(nPos != -1)
			{
				strScriptCommand.Delete(0, 2);
				strInterp = strScriptCommand.Left(nPos-2);
				strScriptCommand.Delete(0, nPos);
			}
		}

		CString strMsg;
		PLAT_LOADSTRING(strInfoOutputScript, IDS_INFO_OUTPUT_SCRIPT); // 解释器名:%s\n脚本:\n%s
		PLAT_LOADSTRING(strInfoOutputScriptTitle, IDS_INFO_OUTPUT_SCRIPT_TITLE); // 单元脚本
		//CString strInfoOutputScript = "解释器名:%s\n脚本:\n%s";
		//CString strInfoOutputScriptTitle = "单元脚本";
		strMsg.Format(strInfoOutputScript, strInterp, strScriptCommand);
		::MessageBox(AfxGetMainWnd()->m_hWnd, strMsg, strInfoOutputScriptTitle, MB_OK|MB_ICONINFORMATION);
/*
		IInterp* pInterp = (IInterp*)(theApp.CreateVciObject("org.interp.tcl", ""));
		if(pInterp)
		{
			CString strCommand;
			strCommand.Format("proc viewscript {} {\n%s\n}", strScriptCommand);
			pInterp->RunScriptCommand(strCommand);
			CString strFile = theApp.GetPlatRootPath() + "\\conf\\menu\\viewscript.tcl";
			pInterp->RunScriptFile(strFile);
			theApp.ReleaseObject(pInterp);
		}
*/
	}
}

void CReportOutCtrl::OnUpdateOutctrlScript(CCmdUI* pCmdUI) 
{
	CXTPReportRow* pRow = GetFocusedRow();
	int nIndex = -1;
	if(pRow)
	{
		CXTPReportRecord* pRecord = pRow->GetRecord();
		if(pRecord)
		{
			CXTPReportRecordItem* pItem = pRecord->GetItem(0);
			if(pItem)
			{
				nIndex = pItem->GetItemData();
			}
		}
	}

	pCmdUI->Enable((nIndex > 0) && (nIndex <= m_asItemScript.GetSize()));
}
