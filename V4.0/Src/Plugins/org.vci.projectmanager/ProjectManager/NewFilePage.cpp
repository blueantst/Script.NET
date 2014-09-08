// NewFilePage.cpp : implementation file
//

#include "stdafx.h"
#include "NewFilePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewFilePage property page

//IMPLEMENT_DYNCREATE(CNewFilePage, CPropertyPage)

CNewFilePage::CNewFilePage(CWnd* pParent) : CDialog(CNewFilePage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewFilePage)
	m_bAddToProject = FALSE;
	m_strFileName = _T("");
	m_strFilePath = _T("");
	m_strAuthor = _T("");
	m_strFileDesc = _T("");
	//}}AFX_DATA_INIT
	m_nFileType = 100;
}

CNewFilePage::~CNewFilePage()
{
}

void CNewFilePage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewFilePage)
	DDX_Control(pDX, IDC_LIST_FILETYPE, m_listFileType);
	DDX_Check(pDX, IDC_CHECK_ADDTOPROJECT, m_bAddToProject);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_strFileName);
	DDX_Text(pDX, IDC_EDIT_FILEPATH, m_strFilePath);
	DDX_Text(pDX, IDC_EDIT_FILE_AUTHOR, m_strAuthor);
	DDX_Text(pDX, IDC_EDIT_FILEDESC, m_strFileDesc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewFilePage, CDialog)
	//{{AFX_MSG_MAP(CNewFilePage)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTON_FILEPATH, OnButtonFilepath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewFilePage message handlers

BOOL CNewFilePage::OnInitDialog() 
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
	strSubTitle.LoadString(IDS_SUB_TITLE_2);
	m_banner.SetCaption(strSubTitle);//_T("Create new file !"));
	m_banner.SetIcon(::LoadIcon(GetDllInstance(), MAKEINTRESOURCE(IDI_PROJECT)));
	reset_dll_resource();

	CFileFind finder;

	CString strWildcard = GetPlatRootPath();
	strWildcard += _T("Plugins\\org.owm.*") ;

	// 搜索插件目录下所有org.owm.*目录
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
		pNode = parser.GetChildNode(pNode, "NewFile");
		if(pNode)
		{
			pNode = parser.GetChildNode(pNode, "File");
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

			m_listFileType.AddString(strName);
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
			m_asExt.Add(strExt);
			m_asInterp.Add(strInterp);
			ProcessPath(strScript, strPath);
			m_asScript.Add(strScript);

			pNode = parser.GetNextNode(pNode, "File");		
		}
		parser.Close();
	}

	finder.Close();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewFilePage::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS) 
{
	if (nIDCtl == IDC_LIST_FILETYPE)
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
		m_listFileType.GetText(lpDIS->itemID, strBuffer);
		
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
// 选择文件路径
/////////////////////////////////////////////////////////////////////////////
void CNewFilePage::OnButtonFilepath() 
{
	UpdateData(TRUE);

	char szPath[MAX_PATH];     //存放选择的目录路径 
    ZeroMemory(szPath, sizeof(szPath));
	strcpy(szPath, m_strFilePath.GetBuffer(0));	// 文件路径
	m_strFilePath.ReleaseBuffer();

	DLL_LOADSTRING(strTitle, IDS_TITLE_SELECT_FILEPATH)

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
		m_strFilePath = szPath;
	}

	UpdateData(FALSE);
}

void CNewFilePage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	if(m_strFileName.GetLength() == 0)
	{
		// 必须输入文件名!
		DLL_LOADSTRING(strMsg, IDS_MUST_INPUT_FILENAME)
		AfxMessageBox(strMsg);
		return;
	}

	m_nFileType = m_listFileType.GetCurSel();
	if(m_nFileType == LB_ERR)
	{
		// 必须选择一种文件类型!
		DLL_LOADSTRING(strMsg, IDS_MUST_SELECT_FILETYPE)
		AfxMessageBox(strMsg);
		return;
	}

	CString strTmp = m_strFileName;
	strTmp.MakeLower();
	CString strExt = "." + m_asExt[m_nFileType];
	if(strTmp.Find(strExt) == -1)
	{
		m_strFileName += strExt;
	}
/*
	// 判断文件是否存在
	if(GetFileAttributes(m_strPrjPath + "\\" + m_strPrjName + "\\" + m_strFileName) != 0xFFFFFFFF)
	{
		// 文件已经存在，不能创建此文件！
		DLL_LOADSTRING(strMsg, IDS_FILE_CANTEXIST)
		AfxMessageBox(strMsg);
		return;
	}
*/
	m_strTemplate = m_asTemplate[m_nFileType];

	// 判断文件模版是否存在
	if(GetFileAttributes(m_strTemplate) == 0xFFFFFFFF)
	{
		// 文件模板不存在，无法创建文件！
		DLL_LOADSTRING(strMsg, IDS_FILETEMPLATE_NOTEXIST)
		AfxMessageBox(strMsg);
		return;
	}

/*
	theApp.m_strPrjName = m_strFileName;
	m_strTemplate = m_asTemplate[m_nFileType];
	ProcessPath(m_strTemplate);
	m_strInterp = m_asInterp[m_nFileType];
	m_strScript = m_asScript[m_nFileType];
	ProcessPath(m_strScript);

	theApp.m_strPrjPath = m_strFilePath;

	theApp.m_strPrjTemplate = m_strTemplate;
	theApp.m_strPrjType = m_asType[m_nFileType];

	theApp.m_strPrjDesc = m_strFileDesc;
	theApp.m_strAuthor = m_strAuthor;

	theApp.m_strInterp = m_strInterp;
	theApp.m_strScript = m_strScript;
*/
	
	CDialog::OnOK();
}
