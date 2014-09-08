// NewPrjPage.cpp : implementation file
//

#include "stdafx.h"
#include "NewPrjPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewPrjPage property page

//IMPLEMENT_DYNCREATE(CNewPrjPage, CPropertyPage)

CNewPrjPage::CNewPrjPage(CWnd* pParent) : CDialog(CNewPrjPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewPrjPage)
	m_strPrjName = _T("");
	m_strPrjDesc = _T("");
	m_strAuthor = _T("");
	m_strPrjPath = _T("");
	//}}AFX_DATA_INIT
	m_nPrjType = 100;
}

CNewPrjPage::~CNewPrjPage()
{
}

void CNewPrjPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewPrjPage)
	DDX_Control(pDX, IDC_LIST_PRJTYPE, m_listPrjType);
	DDX_Text(pDX, IDC_EDIT_PRJNAME, m_strPrjName);
	DDX_Text(pDX, IDC_EDIT_PRJDESC, m_strPrjDesc);
	DDX_Text(pDX, IDC_EDIT_AUTHOR, m_strAuthor);
	DDX_Text(pDX, IDC_EDIT_PRJPATH, m_strPrjPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewPrjPage, CDialog)
	//{{AFX_MSG_MAP(CNewPrjPage)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTON_PRJPATH, OnButtonPrjpath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewPrjPage message handlers

BOOL CNewPrjPage::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//get_dll_resource();
	// Initialize the header control
	m_banner.Attach(this);
	m_banner.SetPosFlag(KCSB_ATTACH_TOP);
	m_banner.SetColBkg(RGB(255,255,255));
	m_banner.SetColBkg2(RGB(190,255,190));
	m_banner.SetColTxtTitle(RGB(0,0,0x80));
	m_banner.SetColTxtCaption(RGB(0x80,0,0));
	PLAT_LOADSTRING(strTitle, IDS_NEW_TITLE)
	m_banner.SetTitle(strTitle);
	CString strSubTitle;
	strSubTitle.LoadString(IDS_SUB_TITLE_1);
	m_banner.SetCaption(strSubTitle);//_T("Create new file !"));
	m_banner.SetIcon(::LoadIcon(GetDllInstance(), MAKEINTRESOURCE(IDI_PROJECT)));
	reset_dll_resource();
	
	CFileFind finder;

	CString strWildcard = GetPlatRootPath();
	strWildcard += _T("Plugins\\org.prj.*") ;

	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// skip . and .. files; otherwise, we'd
		// recur infinitely!

		if (finder.IsDots())
			continue;

		// if not a directory, skip
		if (!finder.IsDirectory())
			continue;

		// if it's a directory, and found plusin.xml, then load it
		CString strPath = finder.GetFilePath();
		CString strVciName = finder.GetFileName();
		if( (GetFileAttributes(strPath+"\\new.xml") == 0xFFFFFFFF) &&
			(GetFileAttributes(strPath+"\\new_cn.xml") == 0xFFFFFFFF))
			continue;

		CXmlParser parser;
		CString strNewFileXmlFile = GetPlatRootPath();
		// new.xml or new_cn.xml
		DLL_LOADSTRING(strXmlFile, IDS_NEW_XML_FILE)
		if(GetFileAttributes(strPath + "\\" + strXmlFile) == 0xFFFFFFFF)
		{
			strNewFileXmlFile = strPath+"\\new.xml";
		}else
		{
			strNewFileXmlFile = strPath + "\\" + strXmlFile;
		}
		parser.Open(strNewFileXmlFile);
		DOMNode* pNode = parser.GetRootNode();
		pNode = parser.GetChildNode(pNode, "NewPrj");
		if(pNode)
		{
			pNode = parser.GetChildNode(pNode, "Project");
		}
		while (pNode != NULL)
		{
			CString strName = parser.GetNodeAttribute(pNode, "Name");
			CString strIcon = parser.GetNodeAttribute(pNode, "Icon");
			CString strTemplate = parser.GetNodeAttribute(pNode, "Template");
			CString strType = parser.GetNodeAttribute(pNode, "Type");
			CString strExt = parser.GetNodeAttribute(pNode, "Ext");
			CString strInterp = parser.GetNodeAttribute(pNode, "Interp");
			CString strScript = parser.GetNodeAttribute(pNode, "Script");

			m_listPrjType.AddString(strName);
			m_auIcon.Add(atoi(strIcon));
			ProcessPath(strIcon, strPath);
			if(GetFileAttributes(strIcon) != 0xFFFFFFFF)
			{
				m_asIcon.Add(strIcon);
			}else
			{
				m_asIcon.Add("");
			}
			ProcessPath(strTemplate, strPath);
			m_asTemplate.Add(strTemplate);
			m_asType.Add(strType);
			m_asPrjVci.Add(strVciName);
			m_asExt.Add(strExt);
			m_asInterp.Add(strInterp);
			ProcessPath(strScript, strPath);
			m_asScript.Add(strScript);

			pNode = parser.GetNextNode(pNode, "Project");		
		}
		parser.Close();
	}

	finder.Close();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewPrjPage::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS) 
{
	if (nIDCtl == IDC_LIST_PRJTYPE)
	{
		if (lpDIS->itemID == LB_ERR)
			return;
		
		CDC* pDC     = CDC::FromHandle(lpDIS->hDC);
		CRect rcItem = lpDIS->rcItem;
		int iSaveDC  = pDC->SaveDC();
		
		CSize sizeIcon;
		sizeIcon.cx = ::GetSystemMetrics(SM_CXSMICON);
		sizeIcon.cy = ::GetSystemMetrics(SM_CYSMICON);

		COLORREF crText = ::GetSysColor(COLOR_WINDOWTEXT);
		COLORREF crBack = ::GetSysColor(COLOR_WINDOW);

		if (lpDIS->itemState & ODS_SELECTED)
		{ 
			crBack = ::GetSysColor(COLOR_HIGHLIGHT); 
			crText = ::GetSysColor(COLOR_HIGHLIGHTTEXT); 
		}

		CRect rcLabel = rcItem;
		rcLabel.left += sizeIcon.cx + 3;

		if (lpDIS->itemAction & (ODA_DRAWENTIRE | ODA_SELECT))
			pDC->FillSolidRect(rcLabel, crBack); 
		
		if (lpDIS->itemAction & ODA_FOCUS)
			pDC->DrawFocusRect(&rcLabel);
	
		CString strBuffer;
		m_listPrjType.GetText(lpDIS->itemID, strBuffer);
		
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(crText);

		rcItem.left += sizeIcon.cx + 4;
		pDC->DrawText(strBuffer, -1, &rcItem,
			DT_LEFT|DT_VCENTER|DT_NOPREFIX| DT_SINGLELINE);
		rcItem.left = lpDIS->rcItem.left;

		// Try to locate the icon for the MDI window by calling WM_GETICON
		// first, this will give us the correct icon if the user has called
		// SetIcon(...) for the child frame.
		//HICON hIcon = (HICON)::SendMessage((HWND)lpDIS->itemData, WM_GETICON, FALSE, 0);

		CString strIconFile = m_asIcon[lpDIS->itemID];
		WORD wIndex = 0;
		HICON hIcon = ::ExtractAssociatedIcon(AfxGetApp()->m_hInstance,
					strIconFile.GetBuffer(0), &wIndex);
		strIconFile.ReleaseBuffer();
		
		// If the icon returned is NULL, then try using GCL_HICONSM to get the
		// document icon for the child frame
		if (hIcon == NULL) {
			hIcon = (HICON)::GetClassLong((HWND)lpDIS->itemData, GCL_HICONSM);
		}
		
		// If no icon was found, then use the default windows logo icon.
		if (hIcon == NULL) {
			hIcon = AfxGetApp()->LoadStandardIcon(IDI_WINLOGO);
		}

		rcItem.left += 2;
		::DrawIconEx(pDC->m_hDC, rcItem.left, rcItem.top, hIcon,
			sizeIcon.cx, sizeIcon.cy, 0, NULL, DI_NORMAL);

		pDC->RestoreDC(iSaveDC);
		return;
	}
	
	CDialog::OnDrawItem(nIDCtl, lpDIS);
}

/////////////////////////////////////////////////////////////////////////////
// 选择工程路径
/////////////////////////////////////////////////////////////////////////////
void CNewPrjPage::OnButtonPrjpath() 
{
	UpdateData(TRUE);

	char szPath[MAX_PATH];     //存放选择的目录路径 
    ZeroMemory(szPath, sizeof(szPath));
	strcpy(szPath, m_strPrjPath.GetBuffer(0));	// 工程路径
	m_strPrjPath.ReleaseBuffer();

	DLL_LOADSTRING(strTitle, IDS_TITLE_SELECT_PRJPATH)

    BROWSEINFO bi;
    bi.hwndOwner = m_hWnd;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = szPath;
    bi.lpszTitle = strTitle;
    bi.ulFlags = 0;
    bi.lpfn = NULL;
    bi.lParam = 0;
    bi.iImage = 0;
    //弹出选择目录对话框
    LPITEMIDLIST lp = SHBrowseForFolder(&bi);

    if(lp && SHGetPathFromIDList(lp, szPath))
	{
		m_strPrjPath = szPath;
	}

	UpdateData(FALSE);
}

void CNewPrjPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	if(m_strPrjName.GetLength() == 0)
	{
		// 必须输入工程名!
		DLL_LOADSTRING(strMsg, IDS_MUST_INPUT_PROJECTNAME)
		AfxMessageBox(strMsg);
		return;
	}

	if(m_strPrjPath.GetLength() == 0)
	{
		// 必须输入工程路径!
		DLL_LOADSTRING(strMsg, IDS_MUST_INPUT_PROJECTPATH)
		AfxMessageBox(strMsg);
		return;
	}

	// 判断工程路径是否存在(创建的工程位置是：工程路径+工程名)
	if(GetFileAttributes(m_strPrjPath + "\\" + m_strPrjName) != 0xFFFFFFFF)
	{
		// 工程目录已经存在，不能创建此工程！
		DLL_LOADSTRING(strMsg, IDS_PROJECTPATH_CANTEXIST)
		AfxMessageBox(strMsg);
		return;
	}

	m_nPrjType = m_listPrjType.GetCurSel();
	if(m_nPrjType == LB_ERR)
	{
		// 必须选择一种工程类型!
		DLL_LOADSTRING(strMsg, IDS_MUST_SELECT_PROJECTTYPE)
		AfxMessageBox(strMsg);
		return;
	}

	m_strTemplate = m_asTemplate[m_nPrjType];

	// 判断工程模版是否存在
	if(GetFileAttributes(m_strTemplate) == 0xFFFFFFFF)
	{
		// 工程模板不存在，无法创建工程！
		DLL_LOADSTRING(strMsg, IDS_PROJECTTEMPLATE_NOTEXIST)
		AfxMessageBox(strMsg);
		return;
	}

	//ProcessPath(m_strTemplate);
	m_strPrjType = m_asType[m_nPrjType];
	m_strPrjVci = m_asPrjVci[m_nPrjType];
	m_strPrjExt = m_asExt[m_nPrjType];
	m_strInterp = m_asInterp[m_nPrjType];
	m_strScript = m_asScript[m_nPrjType];
	//ProcessPath(m_strScript);
/*
	theApp.m_strPrjName = m_strPrjName;
	theApp.m_strPrjPath = m_strPrjPath;
	theApp.m_strPrjType = m_strPrjType;
	theApp.m_strPrjTemplate = m_strTemplate;
	theApp.m_strPrjDesc = m_strPrjDesc;
	theApp.m_strAuthor = m_strAuthor;

	theApp.m_strInterp = m_strInterp;
	theApp.m_strScript = m_strScript;
*/
	// 调用对应工程组件的创建工程接口

	// 调用工程组件的打开工程接口
	
	CDialog::OnOK();
}
