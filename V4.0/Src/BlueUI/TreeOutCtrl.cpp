// TreeOutCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "BlueUI.h"
#include "TreeOutCtrl.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeOutCtrl

CTreeOutCtrl::CTreeOutCtrl()
{
	m_hLastItem = NULL;
	m_strSeparator = _T(",");
	m_pParentBar = NULL;
}

CTreeOutCtrl::~CTreeOutCtrl()
{
	// 删除所有树节点对象
	DeleteAllItems();
}

BEGIN_MESSAGE_MAP(CTreeOutCtrl, CReportCtrl)
	//{{AFX_MSG_MAP(CTreeOutCtrl)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_OEDIT_CLEAR, OnOeditClear)
	ON_COMMAND(ID_OUTCTRL_DELROW, OnOutctrlDelrow)
	ON_COMMAND(ID_OUTCTRL_SCRIPT, OnOutctrlScript)
	ON_UPDATE_COMMAND_UI(ID_OUTCTRL_SCRIPT, OnUpdateOutctrlScript)
	ON_UPDATE_COMMAND_UI(ID_OEDIT_CLEAR, OnUpdateOeditClear)
	ON_UPDATE_COMMAND_UI(ID_OUTCTRL_DELROW, OnUpdateOutctrlDelrow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTreeOutCtrl message handlers

BOOL CTreeOutCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= RVS_TREEVIEW;	// 设置ReportCtrl控件为树类型
	
	return CReportCtrl::PreCreateWindow(cs);
}

int CTreeOutCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CReportCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyle(0, RVS_SHOWHGRID|RVS_SHOWVGRID|RVS_EXPANDSUBITEMS);
	//ModifyProperty(RVP_SEPARATOR, 0x01);

	// 初始化颜色表
	InsertColor(0, RGB(255,255,225));	// 淡黄
	InsertColor(1, RGB(0,0,0));			// 黑
	InsertColor(2, RGB(255,255,255));	// 白
	InsertColor(3, RGB(255,0,0));		// 红
	InsertColor(4, RGB(0,255,0));		// 绿
	InsertColor(5, RGB(0,0,255));		// 蓝
	InsertColor(6, RGB(128,128,128));	// 灰
	InsertColor(7, RGB(128,0,0));		// 半红
	InsertColor(8, RGB(0,128,0));		// 半绿
	InsertColor(9, RGB(0,0,128));		// 半蓝

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// 初始化
/////////////////////////////////////////////////////////////////////////////
void CTreeOutCtrl::Init()
{
	// Create the image list.
	m_ilTree.Create(16, 16, ILC_MASK|ILC_COLOR24, 0, 1);

	CBitmap bmp;
	VERIFY(bmp.LoadBitmap(IDB_REPORT_CTRL));
	m_ilTree.Add(&bmp, CXTPImageManager::GetBitmapMaskColor(bmp, CPoint(0, 0)));

	SetImageList(&m_ilTree);
}

/////////////////////////////////////////////////////////////////////////////
// 关闭窗口时执行的脚本
/////////////////////////////////////////////////////////////////////////////
void CTreeOutCtrl::DoCloseScript() 
{
	// 删除所有树节点对象
	DeleteAllItems();

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
void CTreeOutCtrl::ParseLines(CString strSet, CStringArray& aLines)
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
int CTreeOutCtrl::ParseStyle(CString strStyle)
{
	if(strStyle == _T("SINGLESELECT"))
		return RVS_SINGLESELECT;
	else if(strStyle == _T("SHOWSELALWAYS"))
		return RVS_SHOWSELALWAYS;
	else if(strStyle == _T("SHOWCOLORALWAYS"))
		return RVS_SHOWCOLORALWAYS;
	else if(strStyle == _T("SHOWCOLORALTERNATE"))	// 变换的颜色,注意需要设置Colors数组
		return RVS_SHOWCOLORALTERNATE;
	else if(strStyle == _T("SHOWHGRID"))
		return RVS_SHOWHGRID;
	else if(strStyle == _T("SHOWVGRID"))
		return RVS_SHOWVGRID;
	else if(strStyle == _T("NOHEADER"))
		return RVS_NOHEADER;
	else if(strStyle == _T("NOSORT"))
		return RVS_NOSORT;
	else if(strStyle == _T("ALLOWCOLUMNREMOVAL"))
		return RVS_ALLOWCOLUMNREMOVAL;
	else if(strStyle == _T("SHOWHGRIDEX"))	// 显示延伸的表格线
		return RVS_SHOWHGRIDEX;
	else if(strStyle == _T("FOCUSSUBITEMS"))
		return RVS_FOCUSSUBITEMS;
	else if(strStyle == _T("EXPANDSUBITEMS"))
		return RVS_EXPANDSUBITEMS;
	return 0;
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
// 8.修改控件风格: MODIFYSTYLE ADD|REMOVE style
// 9.从注册表读取列信息: READPROFILE Section, Entry
// 10.向注册表写入列信息: WRITEPROFILE Section, Entry
// 11.设置窗口关闭时执行的脚本: SETCLOSESCRIPT script
// 12.设置字体: SETFONT font size
/////////////////////////////////////////////////////////////////////////////
void CTreeOutCtrl::ParseALine(CString strLine)
{
	CString strSet = strLine;
	strSet.MakeUpper();
	int pos;
	if((pos = strSet.Find("ADDCOLUMN ")) == 0)	// 添加列
	{
		CString strColName;
		CString strFormat = _T("LEFT");
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

		RVSUBITEM rvs;
		rvs.nFormat = RVCF_TEXT|RVCF_EX_TOOLTIP|RVCF_EX_PERSISTENT;
		if(strFormat == _T("CENTER"))
		{
			rvs.nFormat |= RVCF_CENTER;
		}else
		if(strFormat == _T("RIGHT"))
		{
			rvs.nFormat |= RVCF_RIGHT;
		}
		rvs.lpszText = strColName.GetBuffer(strColName.GetLength()+1);
		rvs.iWidth = nWidth;
		int nSubItem = GetActiveSubItemCount();
		DefineSubItem(nSubItem, &rvs, TRUE);
		ActivateSubItem(nSubItem, nSubItem);
		strColName.ReleaseBuffer();
	}else
	if((pos = strSet.Find("DELCOLUMN ")) == 0)	// 删除列
	{
		strSet = strSet.Right(strSet.GetLength()-10);
		if(strSet == _T("ALL"))
		{
			UndefineAllSubItems();
			m_asItemScript.RemoveAll();
		}else
		{
			int nCol = atoi(strSet);
			UndefineSubItem(nCol);
			if(GetActiveSubItemCount() == 0)
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
			DeleteAllItems();
			m_asItemScript.RemoveAll();
		}else
		{
			int nItem = atoi(strSet);
			HTREEITEM hItem = GetItemHandle(nItem);
			DeleteItem(hItem);
			if(GetItemCount() == 0)
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
		SetRowColors(cText, cTextBk);
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
		BOOL bAdd = TRUE;
		if(strSet.Find("REMOVE(") == 0)
		{
			bAdd = FALSE;
			strSet.Delete(0, 7);
		}else
		{
			strSet.Delete(0, 4);
		}
		int nPos = strSet.Find(")");
		if(nPos != -1)
		{
			strSet = strSet.Left(nPos);
		}
		int nStyle = 0;
		while((nPos=strSet.Find("|")) != -1)
		{
			CString strStyle = strSet.Left(nPos);
			nStyle |= ParseStyle(strStyle);
			strSet.Delete(0, nPos+1);
		}
		nStyle |= ParseStyle(strSet);
		ModifyStyle(bAdd ? 0 : nStyle, bAdd ? nStyle : 0);
	}else
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
	}else
	if(strSet.Find("SETCLOSESCRIPT ") == 0)	// 设置窗口关闭时执行的脚本
	{
		m_strScriptClose = strLine.Right(strLine.GetLength()-15);
	}else
	if(strSet.Find("SETFONT ") == 0)	// 设置字体
	{
		strLine = strLine.Right(strLine.GetLength()-8);
		CString strFont = strLine;
		int nSize = 0;
		int nPos = strLine.Find(",");
		if(nPos != -1)
		{
			strFont = strLine.Left(nPos);
			CString strSize = strLine.Right(strLine.GetLength()-nPos-1);
			nSize = atoi(strSize);
		}
		SetSelfFont(strFont, nSize);
	}
}

//////////////////////////////////////////////////////////////////
// 设置输出格式
//////////////////////////////////////////////////////////////////
void CTreeOutCtrl::SetCtrlFormat(CString strSet)
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
void CTreeOutCtrl::Output(CString strOutput)
{
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
			int nItem = atoi(strItem);
			if(nItem<0 || nItem>=GetItemCount())
				return;
			strSet = strSet.Right(strSet.GetLength()-nPos-1);
			int nSubItem = atoi(strSet);
			if(nSubItem >= GetActiveSubItemCount())
				return;
			strSet = strOutput;
			strSet.MakeUpper();
			if(strSet.Find("TEXT ") == 0)	// 设置单项文字
			{
				strOutput = strOutput.Right(strOutput.GetLength()-5);
				SetItemText(nItem, nSubItem, strOutput);
			}else
			if(strSet.Find("IMAGE ") == 0)	// 设置单项图标
			{
				strOutput = strOutput.Right(strOutput.GetLength()-6);
				int nImage = atoi(strOutput);
				SetItemImage(nItem, nSubItem, nImage);
			}else
			if(strSet.Find("CHECK ") == 0)	// 设置单项检查标记
			{
				strOutput = strOutput.Right(strOutput.GetLength()-6);
				int nCheck = atoi(strOutput);
				SetItemCheck(nItem, nSubItem, nCheck);
			}else
			if(strSet.Find("COLOR ") == 0)	// 设置文字颜色
			{
				strOutput = strOutput.Right(strOutput.GetLength()-6);
				RVITEM rvi;
				rvi.nMask = RVIM_TEXTCOLOR;
				rvi.iItem = nItem;
				rvi.iSubItem = nSubItem;
				rvi.iTextColor = atoi(strOutput);	// 颜色表的索引
				if(nSubItem == -1)	// -1表示设置整行的颜色
				{
					for(int i=0; i<GetActiveSubItemCount(); i++)
					{
						rvi.iSubItem = i;
						SetItem(&rvi);
					}
				}else
				{
					SetItem(&rvi);
				}
			}else
			if(strSet.Find("STATE ") == 0)	// 设置文字状态
			{
				strOutput = strOutput.Right(strOutput.GetLength()-6);
				strOutput.MakeUpper();
				RVITEM rvi;
				rvi.nMask = RVIM_STATE;
				rvi.iItem = nItem;
				rvi.iSubItem = nSubItem;
				rvi.nState = 0;
				if(strOutput == "BOLD")
					rvi.nState = RVIS_BOLD;
				if(nSubItem == -1)	// -1表示设置整行的颜色
				{
					for(int i=0; i<GetActiveSubItemCount(); i++)
					{
						rvi.iSubItem = i;
						SetItem(&rvi);
					}
				}else
				{
					SetItem(&rvi);
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
				int nItem = atoi(strItem);
				if(strItem.Find("#") == 0)	// #开头表示是句柄,否则是索引
				{
					nItem = GetItemIndex((HTREEITEM)(atoi(strItem)));
				}
				if(nItem < GetItemCount())
					SetItemData(nItem, atoi(strSet));
				return;
			}
			// 添加脚本,设置链接(注:设置的行号连接是从1开始计数的,因为ItemData缺省为0)
			m_asItemScript.Add(strSet);
			int nItem = atoi(strItem);
			if(strItem.Find("#") == 0)	// #开头表示是句柄,否则是索引
			{
				nItem = GetItemIndex((HTREEITEM)(atoi(strItem)));
			}
			if(nItem>=0 && nItem<GetItemCount())
			{
				SetItemData(nItem, m_asItemScript.GetSize());
			}
		}else
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
		}
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
			HTREEITEM hItem = (HTREEITEM)(atoi(strSet));
			if(strSet == "ROOT")
				hItem = RVTI_ROOT;
			if(hItem == NULL)
				return;
			Expand(hItem, RVE_EXPAND);
		}
	}else	// 添加整行
	{
		CString strItem = _T("");
		int nSepLen = m_strSeparator.GetLength();
		int nPos;
		int nImage = -1;
		HTREEITEM hParentItem = RVTI_ROOT;
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
				hParentItem = (HTREEITEM)(atoi(strParam));
				if(strParam == "ROOT")
					hParentItem = RVTI_ROOT;
				if(hParentItem == NULL)
					return;				
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
		m_hLastItem = InsertItem(strItem, nImage, -1, -1, hParentItem, RVTI_LAST);//, 0);
		int nIndex = GetItemIndex(m_hLastItem);
		if(nPos != -1)
		{
			int nCount = 1;
			while((nPos = strOutput.Find(m_strSeparator)) != -1)
			{
				strItem = strOutput.Left(nPos);
				strOutput = strOutput.Right(strOutput.GetLength()-nPos-nSepLen);
				if(nCount >= GetActiveSubItemCount())
					return;
				SetItemText(nIndex, nCount++, strItem);
			}
			if(nCount >= GetActiveSubItemCount())
				return;
			SetItemText(nIndex, nCount++, strOutput);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 格式化输出
/////////////////////////////////////////////////////////////////////////////
void CTreeOutCtrl::vsWrite(LPCTSTR lpszFormat, va_list argList)
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
// 1.获取热点行号: HOTITEM
// 2.获取选择的行数: SELITEMCOUNT
// 3.获取选择的行: SELITEM ?nItem?
// 4.查找某个元素的行号: FINDITEM<nSubItem> item
// 5.获取单元内容: <nItem,nSubItem>
// 6.获取最后一次操作的树节点句柄: LASTITEM
// 7.获取某个节点的索引(行号): ITEMINDEX
// 8.获取某个节点的句柄: ITEMHANDLE
// 9.获取某个节点的子节点个数: ITEMCOUNT or CHILDCOUNT
// 10.获取某个节点的父节点句柄: ITEMPARENT
// 11.获取列数: COLUMNCOUNT
//////////////////////////////////////////////////////////////////
CString CTreeOutCtrl::GetInfo(CString strParam)
{
	CString strRet = _T("");
	CString strSet = strParam;
	strSet.MakeUpper();
	if(strSet.Find("HOTITEM") == 0)	// 获取热点行号
	{
		strRet.Format("%d", GetFirstSelectedItem());
	}else
	if(strSet.Find("SELITEMCOUNT") == 0)	// 获取选择的行号
	{
		int nCount = 0;
		int nItem = RVI_INVALID;
		while((nItem=GetNextSelectedItem(nItem)) != RVI_INVALID)
		{
			nCount++;
		}
		strRet.Format("%d", nCount);
	}else
	if(strSet.Find("SELITEM") == 0)	// 获取选择的行号
	{
		strSet = strSet.Right(strSet.GetLength()-7);
		int nItem = -1;
		if(strSet.GetLength() > 0)
			nItem = atoi(strSet);
		if(nItem == -1)
			strRet.Format("%d", GetFirstSelectedItem());
		else
			strRet.Format("%d", GetNextSelectedItem(nItem));
	}else
	if(strSet.Find("FINDITEM<") == 0)	// 查找某个元素的行号
	{
		int nPos = strSet.Find("> ");
		if(nPos == -1)
			return _T("-1");
		strSet = strSet.Mid(9, nPos-9);
		int nSubItem = atoi(strSet);
		if(nSubItem<0)
			return _T("-1");
		strParam = strParam.Right(strParam.GetLength()-nPos-2);
		for(int i=0; i<GetItemCount(); i++)
		{
			if(GetItemText(i, nSubItem) == strParam)
			{
				strRet.Format("%d", i);
				return strRet;
			}
		}
		return _T("-1");
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
		int nItem = atoi(strItem);
		strSet = strSet.Right(strSet.GetLength()-nPos-1);
		int nSubItem = atoi(strSet);
		if(nItem>=0 && nItem<GetItemCount())
			strRet = GetItemText(nItem, nSubItem);
	}else
	if(strSet.Find("LASTITEM") == 0)	// 获取最后一次添加的树节点句柄
	{
		strRet.Format("%d", m_hLastItem);
	}else
	if(strSet.Find("ITEMINDEX ") == 0)	// 获取某个节点的索引(行号)
	{
		strSet = strSet.Right(strSet.GetLength()-10);
		HTREEITEM hItem = (HTREEITEM)(atoi(strSet));
		if(strSet == "ROOT")
			hItem = RVTI_ROOT;
		if(hItem != NULL)
		{
			strRet.Format("%d", GetItemIndex(hItem));
		}
	}else
	if(strSet.Find("ITEMHANDLE ") == 0)	// 获取某个节点的句柄
	{
		strSet = strSet.Right(strSet.GetLength()-11);
		int nItem = atoi(strSet);
		strRet.Format("%d", GetItemHandle(nItem));
	}else
	if(strSet.Find("ITEMCOUNT ") == 0 || strSet.Find("CHILDCOUNT ") == 0)	// 获取某个节点的子节点个数
	{
		strSet = strSet.Right(strSet.GetLength()-10);
		HTREEITEM hItem = (HTREEITEM)(atoi(strSet));
		if(strSet == "ROOT")
			hItem = RVTI_ROOT;
		if(hItem != NULL)
		{
			strRet.Format("%d", GetItemCount(hItem));
		}
	}else
	if(strSet.Find("ITEMPARENT ") == 0)	// 获取某个节点的父节点句柄
	{
		strSet = strSet.Right(strSet.GetLength()-11);
		HTREEITEM hItem = (HTREEITEM)(atoi(strSet));
		HTREEITEM hParentItem = (HTREEITEM)(GetNextItem(hItem, RVGN_PARENT));
		if(hParentItem == NULL)
			strRet = "ROOT";
		else
			strRet.Format("%d", hParentItem);
	}else
	if(strSet.Find("COLUMNCOUNT") == 0)	// 获取列数
	{
		strRet.Format("%d", GetActiveSubItemCount());
	}

	return strRet;
}

/////////////////////////////////////////////////////////////////////////////
// 添加树窗口内容
/////////////////////////////////////////////////////////////////////////////
void CTreeOutCtrl::AddTreeItems(CTTreeItemArray*& pItems)
{
	if(pItems == NULL)
	{
		return;
	}

	// 首先删除所有行
	DeleteAllItems();
	m_asItemScript.RemoveAll();

	int nSepLen = m_strSeparator.GetLength();
	HTREEITEM hParentItem = RVTI_ROOT;

	for(int i=0; i<pItems->GetSize(); i++)
	{
		LPTTREEITEM pItem = &(pItems->GetAt(i));
		if(pItem == NULL)
		{
			continue;
		}

		if(pItem->nParent < 0)
		{
			hParentItem = RVTI_ROOT;
		}else
		if(pItem->nParent < pItems->GetSize())
		{
			LPTTREEITEM pParentItem = &(pItems->GetAt(pItem->nParent));
			hParentItem = (HTREEITEM)(pParentItem->hItem);
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
		pItem->hItem = InsertItem(strItem, pItem->nImage, -1, -1, hParentItem, RVTI_LAST);
		(*pItems)[i].hItem = pItem->hItem;
		if((*pItems)[i].hItem == NULL)
		{
			continue;
		}

		int nIndex = GetItemIndex((HTREEITEM)(pItem->hItem));
		if(nPos != -1)
		{
			int nCount = 1;
			while((nPos = strOutput.Find(m_strSeparator)) != -1)
			{
				strItem = strOutput.Left(nPos);
				strOutput = strOutput.Right(strOutput.GetLength()-nPos-nSepLen);
				if(nCount >= GetActiveSubItemCount())
					return;
				SetItemText(nIndex, nCount++, strItem);
			}
			if(nCount >= GetActiveSubItemCount())
				return;
			SetItemText(nIndex, nCount++, strOutput);
		}

		if(pItem->nClickFunc != 0)
		{
			SetItemData(nIndex, pItem->nClickFunc);
		}

		if(pItem->nExpand == 1)
		{
			// 展开节点
			Expand((HTREEITEM)((*pItems)[i].hItem), RVE_EXPAND);
		}
	}

	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// 右键菜单
/////////////////////////////////////////////////////////////////////////////
void CTreeOutCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CReportCtrl::OnRButtonDown(nFlags, point);

	// 安装CoolMenu
	CMenu* pMenu = new CMenu();
	pMenu->LoadMenu(IDR_OUTPUT_GRID);
	CMenu* pmenu = (CMenu*)pMenu->GetSubMenu(0);
	ClientToScreen(&point);
	BOOL bEnable = (GetItemCount() > 0);
	pmenu->EnableMenuItem(ID_OEDIT_CLEAR, bEnable ? MF_ENABLED : MF_GRAYED);
	pmenu->EnableMenuItem(ID_OUTCTRL_DELROW, bEnable ? MF_ENABLED : MF_GRAYED);
	pmenu->EnableMenuItem(ID_OFILE_SAVE, bEnable ? MF_ENABLED : MF_GRAYED);
	CXTPCommandBars::TrackPopupMenu(pmenu, 0, point.x, point.y, this);
	delete pMenu;
}

typedef void (CALLBACK_ITEM_CLICK)(LPVOID lpObj, LPCTSTR lpszParams);

/////////////////////////////////////////////////////////////////////////////
// 双击操作
/////////////////////////////////////////////////////////////////////////////
void CTreeOutCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CReportCtrl::OnLButtonDblClk(nFlags, point);

	int nIndex = -1;
	if(GetFirstSelectedItem() != RVI_INVALID)
	{
		nIndex = GetItemData(GetFirstSelectedItem());
	}
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
				strTmp.Format("%d,", GetFirstSelectedItem());
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
				theApp.ReleaseObject(pInterp);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// 全部清除
/////////////////////////////////////////////////////////////////////////////
void CTreeOutCtrl::OnOeditClear() 
{
	DeleteAllItems();
}

void CTreeOutCtrl::OnUpdateOeditClear(CCmdUI* pCmdUI) 
{
	BOOL bEnable = (GetItemCount() > 0);
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 删除行
/////////////////////////////////////////////////////////////////////////////
void CTreeOutCtrl::OnOutctrlDelrow() 
{
	int nItem = GetFirstSelectedItem();
	if(nItem != RVI_INVALID)
	{
		HTREEITEM hItem = GetItemHandle(nItem);
		DeleteItem(hItem);
	}
}

void CTreeOutCtrl::OnUpdateOutctrlDelrow(CCmdUI* pCmdUI) 
{
	BOOL bEnable = (GetFirstSelectedItem() != RVI_INVALID);
	pCmdUI->Enable(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// 查看脚本
/////////////////////////////////////////////////////////////////////////////
void CTreeOutCtrl::OnOutctrlScript() 
{
	int nIndex = -1;
	if(GetFirstSelectedItem() != RVI_INVALID)
	{
		nIndex = GetItemData(GetFirstSelectedItem());
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
		TPopupControlInfo info;
		info.strCaption = strInfoOutputScriptTitle;
		info.strText = strMsg;
		info.strIcon = "info";
		info.nWidth = 600;
		info.nAddHight = 60;
		info.nShowDelay = 0;
		info.asButtonText.Add("OK");
		COM_CMD("", OC_POPUP_CONTROL, 0, (LPARAM)(&info));*/
	}
}

void CTreeOutCtrl::OnUpdateOutctrlScript(CCmdUI* pCmdUI) 
{
	int nIndex = -1;
	if(GetFirstSelectedItem() != RVI_INVALID)
	{
		nIndex = GetItemData(GetFirstSelectedItem());
	}

	pCmdUI->Enable((nIndex > 0) && (nIndex <= m_asItemScript.GetSize()));
}
