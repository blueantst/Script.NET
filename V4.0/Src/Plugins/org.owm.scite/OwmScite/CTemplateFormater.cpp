// CTemplateFormater.cpp: implementation of the CTemplateFormater class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OwmScite.h"
#include "CTemplateFormater.h"
//#include "InputDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Global buffer
TCHAR szBuffer[100];

const SpecData CTemplateFormater::s_FormatSpecTable[] =
{
	{_T("U "),   CTemplateFormater::FormatUpper},
	{_T("L "),   CTemplateFormater::FormatLower},
	{_T("W "),   CTemplateFormater::FormatWidth}
};

const KWData CTemplateFormater::s_KeywordTable[] =
{
	{_T("TIME"),            CTemplateFormater::GetTime},
	{_T("DATE"),            CTemplateFormater::GetDate},
	{_T("PROJECT"),         CTemplateFormater::GetProject},
	{_T("FILE"),            CTemplateFormater::GetFile},
	{_T("PATH"),            CTemplateFormater::GetPath},
	{_T("FILENAME_ONLY"),   CTemplateFormater::GetFileNameOnly},
	{_T("EXT_ONLY"),        CTemplateFormater::GetExtOnly},
	{_T("TIME24"),          CTemplateFormater::GetTime24},
	{_T("LONGDATE"),        CTemplateFormater::GetLongDate},
	{_T("USERNAME"),        CTemplateFormater::GetUserName}
};


////////////////////////////////////////////
//  Keyword functions
////////////////////////////////////////////

// Returns the current date in user locale default format.
CString CTemplateFormater::GetDate(void)
{
	GetDateFormat(LOCALE_USER_DEFAULT, 0, NULL, NULL,
		szBuffer, sizeof(szBuffer));
	return szBuffer;
};

// Returns only the file extension of the currently opened file.
CString CTemplateFormater::GetExtOnly(void)
{
	// Get the filename, including extension
	CString szExt = GetFile();
	int iPos      = szExt.ReverseFind(_T('.'));
	if (iPos != -1)
		szExt = szExt.Mid(iPos + 1);
	
	return szExt;
};

// Returns the name of the currently opened file, including extension (if saved).
CString CTemplateFormater::GetFile(void)
{
	// Use the generic view class to make this class as portable as possible
	CView* pView = NULL;

	CMDIFrameWnd* pFrame = reinterpret_cast<CMDIFrameWnd*>(AfxGetApp()->m_pMainWnd);
	
	// Get the active MDI child window.
	CMDIChildWnd* pChild = 
		reinterpret_cast<CMDIChildWnd*>(pFrame->GetActiveFrame());		
	// or CMDIChildWnd *pChild = pFrame->MDIGetActive();
	
	// Get the active view attached to the active MDI child window.
	pView = reinterpret_cast<CView*>(pChild->GetActiveView());		

	ASSERT(pView != NULL);
	// If at this point the view could not be obtained, the leave
	if (pView != NULL)
	{
		CDocument* pDoc = pView->GetDocument();
		ASSERT(pDoc != NULL);
		if (pDoc != NULL)
		{
			CString strPathName = pDoc->GetPathName(); 

			if (!strPathName.IsEmpty())
			{
				CCFileSpec fs(strPathName);	//NOTE: This part is not portable!
				return fs.GetFileName();
			}
			else
			{
				return pDoc->GetTitle();
			}
		}
	}

	return _T("");
};

// Returns only the filename, no extension, of the currently
// opened file.
CString CTemplateFormater::GetFileNameOnly(void)
{
	// Get the filename, including extension
	CString szFile = GetFile();
	int iPos       = szFile.ReverseFind(_T('.'));
	if (iPos != -1)
		szFile = szFile.Mid(0, iPos);
	
	return szFile;
};

// Returns the path to the currently opened file.
CString CTemplateFormater::GetPath(void)
{
	//TODO--complete this!
	return _T("");
};

// Returns the current project name.
CString CTemplateFormater::GetProject(void)
{
	//TODO--complete this!
	return _T("");
};

// Returns the current time in user locale default format.
CString CTemplateFormater::GetTime(void)
{
	// Get the current time, in user locale format
	GetTimeFormat(LOCALE_USER_DEFAULT, 0, NULL, NULL, szBuffer, sizeof(szBuffer));
	return szBuffer;
};

// Returns the current time in user locale default format (24 hour format).
CString CTemplateFormater::GetTime24(void)
{
	GetTimeFormat(LOCALE_USER_DEFAULT, TIME_FORCE24HOURFORMAT, NULL, NULL,
		szBuffer, sizeof(szBuffer));
	return szBuffer;
};

// Returns the current time in user default locale long format
CString CTemplateFormater::GetLongDate(void)
{
	GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, NULL, NULL,
		szBuffer, sizeof(szBuffer));
	return szBuffer;
}

// Returns the current user or environment name
CString CTemplateFormater::GetUserName(void)
{
	lstrcpy(szBuffer, _T("")); //TODO--?

//    OSVERSIONINFO osinfo ;
//
//		ZeroMemory(&osinfo, sizeof(OSVERSIONINFO));
//		osinfo.dwOSVersionInfoSize = sizeof(osinfo); // Get platform information
//		GetVersionEx(&osinfo);
//		
//		// **TODO**: Personalize the software for the user
//		if (osinfo.dwPlatformId == VER_PLATFORM_WIN32_NT)	// if Win NT...
//		{
//			GetRegKey(HKEY_LOCAL_MACHINE, 
//				_T("Software\\Microsoft\\Windows NT\\CurrentVersion"), 
//				_T("RegisteredOwner"), szOwner);	
//			GetRegKey(HKEY_LOCAL_MACHINE, 
//				_T("Software\\Microsoft\\Windows NT\\CurrentVersion"), 
//				_T("RegisteredOrganization"), szOrg);
//		}
//		else												// if Win 95/98
//		{
//			GetRegKey(HKEY_LOCAL_MACHINE, 
//				_T("Software\\Microsoft\\Windows\\CurrentVersion"), 
//				_T("RegisteredOwner"), szOwner);	
//			GetRegKey(HKEY_LOCAL_MACHINE, 
//				_T("Software\\Microsoft\\Windows\\CurrentVersion"), 
//				_T("RegisteredOrganization"), szOrg);
//		}

	// 通过OWM命令从平台获取登录用户名
	CString strUserName = "";
	if(theApp.m_ol.CommandPlat(OC_QUERYUSERINFO, 3, (LPARAM)(&strUserName)))
	{
		lstrcpy(szBuffer, strUserName);
	}

	return szBuffer;
}


////////////////////////////////////////////
//   Format specifier functions
////////////////////////////////////////////

// Lowercases the specified string.
CString CTemplateFormater::FormatLower(const CString& szSource)
{
	CString strReturn(szSource);
	strReturn.MakeLower();
	return strReturn;
};

// Uppercases the specified string.
CString CTemplateFormater::FormatUpper(const CString& szSource)
{
	CString strReturn(szSource);
	strReturn.MakeUpper();
	return strReturn;
};

// Forces the string to be the specified width.  It will
// pad the string with spaces to ensure a fit, or will
// truncate all characters after the specified width
// has been reached.
CString CTemplateFormater::FormatWidth(const CString& szSource)
{
	int iPos, iWidth;
	CString szNew;
	TCHAR* szBuffer = NULL;
	
	// The string should be of the form < Text>, extract
	// the number
	iPos = szSource.Find(_T(' '));
	if (iPos == -1)
		return (_T(""));
	
	// Convert the width to a number
	iWidth = _ttoi(szSource.Mid(0, iPos));
	szNew  = szSource.Mid(iPos + 1);
	
	// Create a string array with spaces
	szBuffer = new TCHAR[iWidth + 1];
	memset(szBuffer, _T(' '), iWidth);
	szBuffer[iWidth] = _T('\0');
	
	// Get the shorter of either the width or the actual
	// string length
	iWidth = min(iWidth, szNew.GetLength());
	
	// Copy the appropriate characters
	memcpy(szBuffer, szNew, iWidth);
	szNew = szBuffer;
	delete[] szBuffer;
	
	return szNew;
};

CTemplateFormater::CTemplateFormater(const CString& strSource) 
	: m_strSource(strSource)
{

}

CTemplateFormater::CTemplateFormater() 
	: m_strSource(_T(""))
{

}

CTemplateFormater::~CTemplateFormater()
{

}

CString CTemplateFormater::Format()
{
	ASSERT(!m_strSource.IsEmpty());
	CString szTmpl = CString(m_strSource);
	int iIndex, iTableSize;
	CString szKey, szText;
	int iStartPos, iNextPos;
	
	// Translate the keyword table...
	iTableSize = ARRAY_SIZE(s_KeywordTable);
	iIndex     = 0;
	// Iterate through the table
	while (iIndex < iTableSize)
	{
		// What is the keyword?
		szKey = KEYWORD_BEGIN;
		szKey += s_KeywordTable[iIndex].szKeyword;
		szKey += KEYWORD_END;
		
		// Substitute the correct value for the keyword
		szTmpl.Replace(szKey, s_KeywordTable[iIndex].Proc());
		
		// Next
		iIndex++;
	};
	
	// Translate replaceble parameters
	CStringArray ParamTable;
	
	GetReplacebleParams(ParamTable, szTmpl);
	
	iTableSize = ParamTable.GetSize();
	iIndex = 0;
	
	while (iIndex < iTableSize)
	{
		// What is the keyword?
		szKey  = PARAM_BEGIN;
		szKey += ParamTable[iIndex];
		szKey += PARAM_END;
		
		// Substitute the correct value for the keyword
		szTmpl.Replace(szKey, GetParam(ParamTable[iIndex]));
		
		// Next
		iIndex++;
	}
	
	// Translate the format specifiers
	iTableSize = ARRAY_SIZE(s_FormatSpecTable);
	iIndex     = 0;
	// Iterate through the table
	while (iIndex < iTableSize)
	{
		// What is the specifier?
		szKey = FORMAT_BEGIN;
		szKey += s_FormatSpecTable[iIndex].szSpecifier;
		iStartPos = 0;
		
		// Replace each occurrence of the specifier with
		// the new value
		while (GetNextInstance(szTmpl, szKey, iStartPos))
		{
			// Extract the enclosed text
			szText = ExtractText(szTmpl, szKey, iStartPos, iNextPos);
			
			// Replace the specifier and enclosed text with the
			// updated version
			szTmpl = ReplaceSpecifier(szTmpl, iStartPos, iNextPos,
				s_FormatSpecTable[iIndex].Proc(szText));
			
			iStartPos = iNextPos;
		};
		
		// Next
		iIndex++;
	};
	
	return (szTmpl);
}

CString CTemplateFormater::Format(const CString& strSource)
{
	CTemplateFormater xFormat(strSource);
	return xFormat.Format();
}

void CTemplateFormater::GetReplacebleParams(CStringArray& ParamTable, 
											const CString& szSource)
{
	CString szTemp = szSource;
	int iStartPos, iEndPos;
	
	do
	{
		iStartPos = szTemp.Find(PARAM_BEGIN);
		if (iStartPos == -1)
			break;
		
		iStartPos += lstrlen(PARAM_BEGIN);
		
		iEndPos = szTemp.Find(PARAM_END);
		if (iEndPos == -1)
			break;
		
		CString sz = szTemp.Mid(iStartPos, iEndPos - iStartPos);
		
		BOOL bFound = FALSE;
		
		for (int i = 0; i < ParamTable.GetSize(); i++)
		{
			if (sz == ParamTable.GetAt(i))
			{
				bFound = TRUE;
				break;
			}
		}
		
		if (!bFound)
			ParamTable.Add(sz);	 // Add to the string list
		
		szTemp = szTemp.Mid(iEndPos + 1);
	} while (TRUE);
}

CString CTemplateFormater::GetParam(const CString& szPrompt)
{

/*	get_dll_resource();
	CInputDialog dlg;
	
	dlg.m_strPromptText = szPrompt;
	
	if (dlg.DoModal() == IDOK)
		return dlg.m_strInputText;
*/	
//	return _T("/*" + szPrompt + "*/");	//TODO--this is C/C++ specific comment
	return szPrompt;
}

// Extracts the text enclosed in the format specifier delimiters.
CString CTemplateFormater::ExtractText(const CString szSrc, const CString szSpec,
                      const int iStartPos, int& iEndPos)
{
	// Find the first ending format specifier
	iEndPos = szSrc.Find(FORMAT_END, iStartPos);
	if (iEndPos == -1)
		return (_T(""));
	
	// Remove the delimiters from the string
	int iPos = iStartPos + szSpec.GetLength();
	CString szText = szSrc.Mid(iPos, iEndPos - iPos);
	
	return szText;
};

// Returns the next instance of the given specifier starting at
// the given position.
BOOL CTemplateFormater::GetNextInstance(const CString szSrc, const CString szFormat,
                       int& iPos)
{
	// Find the next occurrence of the specifier
	iPos = szSrc.Find(szFormat, iPos);
	if (iPos == -1)
		return FALSE;
	
	return TRUE;
};

// Replace the text enclosed by the start/end position with
// the specified text.
CString CTemplateFormater::ReplaceSpecifier(const CString szSrc, int iStartPos,
                           int iEndPos, const CString szText)
{
	// Replace the string delimited by start and end with
	// the specified text.
	CString szTmpl(szSrc);
	int iLen = lstrlen(FORMAT_END);
	CString strReplace = szTmpl.Mid(iStartPos, (iEndPos - iStartPos + iLen));
	szTmpl.Replace(strReplace, szText);
	return szTmpl;
};
