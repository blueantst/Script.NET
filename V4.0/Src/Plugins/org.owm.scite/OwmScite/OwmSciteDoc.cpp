// OwmSciteDoc.cpp : implementation file
//

#include "stdafx.h"
#include "OwmScite.h"
#include "OwmSciteDoc.h"
#include "OwmSciteView.h"
#include "Utf8_16.h"
#include "UniConversion.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COwmSciteDoc

IMPLEMENT_DYNCREATE(COwmSciteDoc, CDocument)

COwmSciteDoc::COwmSciteDoc()
{
	m_pMenu = NULL;
}

BOOL COwmSciteDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	POSITION pos = GetFirstViewPosition();
	if(pos != NULL)
	{
		// 通知平台取消文档的修改标记
		COwmSciteView* pView = (COwmSciteView*)GetNextView(pos);

		NppParameters *pNppParam = NppParameters::getInstance();
		NppGUI & nppGUI = (NppGUI &)pNppParam->getNppGUI();
		NewDocDefaultSettings & ndds = (NewDocDefaultSettings &)nppGUI.getNewDocDefaultSettings();

		// 设置编码方式和scintilla的codepage
		pView->m_SciEdit.setUnicodeMode(ndds._encoding);
		if (ndds._encoding == uni8Bit) 
		{	//either 0 or CJK codepage
			if (pView->m_SciEdit.isCJK())
			{
				pView->executeThreadSciCommand(SCI_SETCODEPAGE, pView->m_SciEdit.getCodepage());	//you may also want to set charsets here, not yet implemented
			}
			else
			{
				pView->executeThreadSciCommand(SCI_SETCODEPAGE, 0);
			}
		} 
		else
		{	//CP UTF8 for all unicode
			pView->executeThreadSciCommand(SCI_SETCODEPAGE, SC_CP_UTF8);
		}

		// 设置文档格式(WIN/MAC/UNIX)
		pView->m_SciEdit.setFormat(ndds._format);
	}

	return TRUE;
}

COwmSciteDoc::~COwmSciteDoc()
{
}


BEGIN_MESSAGE_MAP(COwmSciteDoc, CDocument)
	//{{AFX_MSG_MAP(COwmSciteDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COwmSciteDoc diagnostics

#ifdef _DEBUG
void COwmSciteDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COwmSciteDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COwmSciteDoc serialization

void COwmSciteDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// COwmSciteDoc commands
void COwmSciteDoc::SetTitle(LPCTSTR lpszTitle) 
{
	
	CDocument::SetTitle(lpszTitle);
}

void COwmSciteDoc::OnCloseDocument() 
{
	CString strFilePath = GetPathName();
	if (!strFilePath.IsEmpty())
	{
		StopWatching();
		RemoveFile(strFilePath);

		// 刷新文件树节点
		CString strCmdLine = "RefreshFile close \"";
		strCmdLine += strFilePath;
		strCmdLine += "\"";
		theApp.ExecFileMngScript(strCmdLine);
	}

	if(m_pMenu)
	{
		delete m_pMenu;
		m_pMenu = NULL;
	}
	
	CDocument::OnCloseDocument();
}

// 刷新属性树
void COwmSciteDoc::RefreshDocProperty()
{
	if(!theApp.m_pIPlatUI || !theApp.m_pIPlatUI->EnableUpdatePropertyGridContent())
	{
		return;
	}

	// 开始属性页更新
	theApp.m_pIPlatUI->BeginUpdatePropertyGridContent();

	CXTPPropertyGrid* pPropertyGrid = (CXTPPropertyGrid*)(theApp.m_pIPlatUI->GetPropertyGridWnd());
	if(!pPropertyGrid->m_hWnd)
	{
		return;
	}

	get_dll_resource();
	CXTPPropertyGridItem* pCategoryDoc = pPropertyGrid->AddCategory(ID_PROP_DOCUMENT);
	CXTPPropertyGridItem* pItem = NULL;
	CString strTmp;

	CString strFilePath = GetPathName();
	if(strFilePath != "")
	{
		pItem = pCategoryDoc->AddChildItem(
			new CXTPPropertyGridItem(IDS_DOCPROP_FILEPATH,//_T("文件路径"),
				GetPathName()));
		pItem->SetDescription(GetPathName());
		pItem->SetReadOnly(TRUE);
	}

	COwmSciteView* pView = NULL;
	POSITION pos = GetFirstViewPosition();
	if(pos != NULL)
	{
		pView = (COwmSciteView*)(GetNextView(pos));
	}
	if(pView)
	{
		if(L_UNKNOWN == pView->m_LangType)
		{
			// View还没有初始化完成,语言类型未知
			reset_dll_resource();
			theApp.m_pIPlatUI->EndUpdatePropertyGridContent();
			return;
		}

		const TCHAR * shortName = ScintillaEditView::langNames[pView->m_LangType].shortName;
		const TCHAR * longName = ScintillaEditView::langNames[pView->m_LangType].longName;
		pItem = pCategoryDoc->AddChildItem(
			new CXTPPropertyGridItem(IDS_DOCPROP_FILETYPE, shortName)); //_T("文件类型")
		pItem->SetDescription(longName);
		pItem->SetReadOnly(TRUE);
	}

	if((strFilePath != "") && (pView != NULL))
	{
		// 文件大小
		strTmp.Format(IDS_DOCPROP_FILEBYTE, pView->m_SciEdit.getCurrentDocLen());	//"%d 字节"
		pItem = pCategoryDoc->AddChildItem(
			new CXTPPropertyGridItem(IDS_DOCPROP_FILESIZE, strTmp));
		pItem->SetDescription(strTmp);
		pItem->SetReadOnly(TRUE);

		// 文件行数
		strTmp.Format("%d", pView->m_SciEdit.execute(SCI_GETLINECOUNT));
		pItem = pCategoryDoc->AddChildItem(
			new CXTPPropertyGridItem(IDS_DOCPROP_FILELINECOUNT, strTmp));
		pItem->SetDescription(strTmp);
		pItem->SetReadOnly(TRUE);

		// 文件编码
		strTmp = pView->m_SciEdit.getUniModeText(pView->m_SciEdit.getUnicodeMode());
		pItem = pCategoryDoc->AddChildItem(
			new CXTPPropertyGridItem(IDS_DOCPROP_FILEUNIMODE, strTmp));
		pItem->SetDescription(strTmp);
		pItem->SetReadOnly(TRUE);

		// 文件类型
		strTmp = pView->m_SciEdit.getFormatText(pView->m_SciEdit.getFormat());
		pItem = pCategoryDoc->AddChildItem(
			new CXTPPropertyGridItem(IDS_DOCPROP_FILEFORMAT, strTmp));
		pItem->SetDescription(strTmp);
		pItem->SetReadOnly(TRUE);

		// 缩放级别
		pItem = pCategoryDoc->AddChildItem(
			new CXTPPropertyGridItemNumber(IDS_DOCPROP_ZOOM, pView->m_SciEdit.execute(SCI_GETZOOM)));
		pItem->SetReadOnly(TRUE);

	}

	pCategoryDoc->Expand();
	reset_dll_resource();

	// 添加快捷链接
	DLL_LOADSTRING(strFileSave, IDS_VERB_FILESAVE);	// 保存文件
	pPropertyGrid->GetVerbs()->Add(strFileSave, ID_FILE_SAVE);
	DLL_LOADSTRING(strFind, IDS_VERB_FIND);	// 查找
	pPropertyGrid->GetVerbs()->Add(strFind, ID_EDIT_FIND);

	// 结束属性页更新
	theApp.m_pIPlatUI->EndUpdatePropertyGridContent();
}

BOOL COwmSciteDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	NppParameters *pNppParam = NppParameters::getInstance();
	NppGUI & nppGUI = (NppGUI &)pNppParam->getNppGUI();
	if(nppGUI._fileAutoDetection == cdEnabled)
	{
		AddFile(lpszPathName);
		StartWatching();
	}

	// 刷新文件树节点
	CString strCmdLine = "RefreshFile open \"";
	strCmdLine += lpszPathName;
	strCmdLine += "\"";
	theApp.ExecFileMngScript(strCmdLine);

	return TRUE;
}

BOOL COwmSciteDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	CString strOldPathName = GetPathName();
	if(strOldPathName != lpszPathName)
	{
		// 文件名已经修改,刷新文件树节点,删除原先的节点
		CString strCmdLine = "RefreshFile close \"";
		strCmdLine += strOldPathName;
		strCmdLine += "\"";
		theApp.ExecFileMngScript(strCmdLine);
	}

	// 关闭文件监视
	StopWatching();
	RemoveFile(strOldPathName);

	POSITION pos = GetFirstViewPosition();
	if(pos != NULL)
	{
		// 通知平台取消文档的修改标记
		COwmSciteView* pView = (COwmSciteView*)GetNextView(pos);
		
		UniMode mode = pView->m_SciEdit.getUnicodeMode();
		if (mode == uniCookie)
			mode = uni8Bit;	//set the mode to ANSI to prevent converter from adding BOM and performing conversions, Scintilla's data can be copied directly

		Utf8_16_Write UnicodeConvertor;
		UnicodeConvertor.setEncoding(mode);

		FILE *fp = UnicodeConvertor.fopen(lpszPathName, TEXT("wb"));
		if (fp)
		{
			char data[blockSize + 1];
			int lengthDoc = pView->m_SciEdit.getCurrentDocLen();
			for (int i = 0; i < lengthDoc; i += blockSize)
			{
				int grabSize = lengthDoc - i;
				if (grabSize > blockSize) 
					grabSize = blockSize;
				
				pView->m_SciEdit.getText(data, i, i + grabSize);
				UnicodeConvertor.fwrite(data, grabSize);
			}
			UnicodeConvertor.fclose();

			pView->m_SciEdit.execute(SCI_SETSAVEPOINT);
			pView->m_SciEdit.markSavedLines();	// 设置修改行的已保存标记

			// 判断并设置语言类型
			LangType newLang = L_TXT;
			TCHAR *ext = PathFindExtension(lpszPathName);
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
					newLang = pView->getLangFromExt(ext);
				}	
			}
			pView->m_SciEdit.defineDocType(newLang);
			pView->m_LangType = newLang;
			pView->m_SciEdit.styleChange();

			// 文档类型设置之后才能调用自动完成的设置语言函数
			pView->_autoComplete.setLanguage(newLang);

			pView->_codeSnippet.setLanguage(newLang);

			NppParameters *pNppParam = NppParameters::getInstance();
			NppGUI & nppGUI = (NppGUI &)pNppParam->getNppGUI();
			if(nppGUI._fileAutoDetection == cdEnabled)
			{
				// 启动文件监视
				AddFile(lpszPathName);
				StartWatching();
			}

			// 刷新文件树节点
			CString strCmdLine = "RefreshFile save \"";
			strCmdLine += lpszPathName;
			strCmdLine += "\"";
			theApp.ExecFileMngScript(strCmdLine);

			return TRUE;
		}
		else 
		{
			char msg[MAX_PATH + 100];
			strcpy(msg, "Could not save file \"");
			strcat(msg, lpszPathName);
			strcat(msg, "\".");
			MessageBox(NULL, msg, NULL, MB_OK);
			return FALSE;
		}
	}
	return FALSE;
}

void COwmSciteDoc::SetModifiedFlag(BOOL bModified)
{
	if(bModified != IsModified())
	{
		CDocument::SetModifiedFlag(bModified);
		
		//if(IsModified())
		{
			// 通知平台增加文档的修改标记
			POSITION pos = GetFirstViewPosition();
			if(pos != NULL)
			{
				CView* pView = GetNextView(pos);
				//CWnd* pFrame = pView->GetParent();
				CWnd* pFrame = pView;
				if(pFrame && theApp.m_pIPlatUI)
				{
					if(IsModified())
					{
						theApp.m_pIPlatUI->SetDocumentTitle("", 1, ::GetParent(pFrame->GetSafeHwnd()));
					}
					else
					{
						theApp.m_pIPlatUI->SetDocumentTitle("", 0, ::GetParent(pFrame->GetSafeHwnd()));
					}
				}
			}
		}
	}
}

//#define IDS_FILE_CHANGED   _T("文件 %s 被改变，是否重新加载？")
//#define IDS_FILE_DELETED   _T("文件 %s 被删除！")
void COwmSciteDoc::OnFileEvent(WPARAM wEvent, LPCTSTR pszPathName)
{
	MessageBeep(MB_ICONEXCLAMATION);

	if (wEvent & FE_MODIFIED)
	{
		BOOL bReload = FALSE;
		if (!bReload)
		{
			CString sMsg;
			get_dll_resource();
			sMsg.Format(IDS_FILE_CHANGED, pszPathName);
			reset_dll_resource();
			//bReload = AfxMessageBox(sMsg, MB_YESNO | MB_ICONQUESTION) == IDYES;
			bReload = MessageBox(NULL, sMsg, NULL, MB_YESNO | MB_ICONQUESTION) == IDYES;
		}
		if (bReload)
		{
			POSITION pos = GetFirstViewPosition();
			ASSERT(pos);
			COwmSciteView* pView;
			do
			{
				pView = static_cast<COwmSciteView*>(GetNextView(pos));
				//pView->PushCursor();
			}
			while (pos);

			if(pView)
			{
				pView->saveCurrentPos();
				pView->executeThreadSciCommand(SCI_CLEARALL);
				pView->OpenFile(pszPathName);
				pView->restoreCurrentPos();
			}
		}
	}
	else if (wEvent & FE_DELETED)
	{
		CString sMsg;
		get_dll_resource();
		sMsg.Format(IDS_FILE_DELETED, pszPathName);
		reset_dll_resource();
		//AfxMessageBox(sMsg, MB_OK | MB_ICONINFORMATION);
		MessageBox(NULL, sMsg, NULL, MB_OK | MB_ICONINFORMATION);
	}
}
