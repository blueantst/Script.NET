// CTemplateLoader.cpp: implementation of the CTemplateLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CTemplateLoader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CTemplateLoader::CTemplateLoader(CString& strTemplFile)
	: m_strTmplFile(strTemplFile), m_bIsFileLoaded(FALSE)
{
	m_mapNameToIndex.RemoveAll();
	VERIFY(Load());
}

CTemplateLoader::CTemplateLoader()
	: m_strTmplFile(_T("")), m_bIsFileLoaded(FALSE)
{
	m_mapNameToIndex.RemoveAll();
}

CTemplateLoader::~CTemplateLoader()
{
	m_arrTemplateData.RemoveAll();
	m_mapNameToIndex.RemoveAll();
}

BOOL CTemplateLoader::Load(CString& strTmplFilePath)
{
	m_strTmplFile = strTmplFilePath;
	return Load();
}

BOOL CTemplateLoader::Load()
{
	ASSERT(!m_strTmplFile.IsEmpty());
	if (m_strTmplFile.IsEmpty())
	{
		//TODO--display error message
		return FALSE;
	}

	// Verify the existence of the template file here
	CCFileSpec  fs(m_strTmplFile);
	if (!fs.FileExists())
	{
		//TODO--display file does not exits message box and exit
		return FALSE;
	}
	
	m_criSection.Lock();
	m_bIsFileLoaded = FALSE;

	TemplateData td;          // buffer for current template
	
	enum { PS_Tag, PS_Template } eState = PS_Tag; // parsing state
	int nLevel = 1;
	
	//  ensure our current template data is cleared down first, so
	//  that we can refresh things on the fly
	m_arrTemplateData.RemoveAll();
	
	CCReadFile 	rfText;
	CString		strLine;
	
	// When the given file cannot be opened
	if (!rfText.Open(m_strTmplFile)) 
	{
		//TODO--display error message
		TRACE0("BOOL CTemplateLoader::Load()--template file could not be opened.\n");
		return FALSE;
	}

	//  ...and whilst we still got some left...
	while (rfText.GetNextLine(strLine) != 0)
	{
		if (strLine[0] == _T(';'))    // it is a comment...
			continue;                 //...get next line. 

		//  handle it according to parsing state (PS_Tag or PS_Template),
		//  we are either looking for the openning #{ tag or reading
		//  template text for the current template and looking for the
		//  closing #} which terminates a template definition
		if (eState == PS_Tag)
		{
			//  does this line look like an openning #{ tag...
			if (strLine.GetLength() > 2 && strLine.Mid(0, 2) == ITEM_START_DELIM)
			{
				//  ...yep, grab the template name which immediately follows
				//  the #{ tag, and switch to PS_Template state.
				td.strName = strLine.Mid(2);
				td.strText = _T("");                
				eState     = PS_Template;
				
				// remove all starting and end whitespaces
				td.strName.TrimLeft();
				td.strName.TrimRight();
			} 
			//  a pair (or more) of hashes inserts a separator on the popup menu
			else if (strLine.GetLength() > 2 && strLine.Mid(0, 2) == _T("##"))
			{
				td.strName = _T("");
				td.strText = _T("");
			}   
			else if ((strLine.GetLength() > 2) && 
				(strLine.Mid(0, 2) == SUBITEM_START_DELIM))
			{
				// Enter a new level
				nLevel++;
			}            
			else if ((strLine.GetLength() >= 2) && 
				(strLine.Mid(0, 2) == SUBITEM_END_DELIM))
			{
				// Leave the current level
				nLevel--;
			}       
			
			// ignore all other rubbish !
		}
		else 
		{
			//  does this line look like a closing #} tag...
			if (strLine.GetLength() >= 2 && strLine.Mid(0, 2) == ITEM_END_DELIM) 
			{
				//  ...yep, push current template data into vector, and
				//  switch to PS_Tag state
				td.nLevel  = nLevel;
				int nIndex = m_arrTemplateData.Add(td);   // Add to the templates
				m_mapNameToIndex.Set(td.strName, nIndex); // Map the name to index
				eState = PS_Tag;
			}
			else
			{
				//  ...nope, just append the line to our current template definition
				td.strText += strLine;
				td.strText += _T("\r\n");
			}; 
		}; // if (eState == PS_Tag)
	}; // while (GetNextLine(...))

	m_bIsFileLoaded = TRUE;
	m_criSection.Unlock();
	return TRUE;  // Return TRUE for successfully loading the template file
};

CString CTemplateLoader::GetTemplateText(int nIndex)
{
	ASSERT(nIndex >= 0 && nIndex < m_arrTemplateData.GetSize());
	ASSERT(m_bIsFileLoaded);
	ASSERT(m_arrTemplateData.GetSize() > 0);

	if (nIndex >= 0 && nIndex < m_arrTemplateData.GetSize())
		return m_arrTemplateData[nIndex].strText;

	return _T("");
}

void CTemplateLoader::SetTemplateText(int nIndex, const CString& strTemplateText)
{
	ASSERT(nIndex >= 0);
	ASSERT(m_bIsFileLoaded);

	if (nIndex >= 0 && nIndex < m_arrTemplateData.GetSize())
		m_arrTemplateData[nIndex].strText = strTemplateText;
}

CString CTemplateLoader::GetTemplateName(int nIndex)
{
	ASSERT(nIndex >= 0 && nIndex < m_arrTemplateData.GetSize());
	ASSERT(m_bIsFileLoaded);
	ASSERT(m_arrTemplateData.GetSize() > 0);

	if (nIndex >= 0 && nIndex < m_arrTemplateData.GetSize())
		return m_arrTemplateData[nIndex].strName;

	return _T("");
}

void CTemplateLoader::SetTemplateName(int nIndex, const CString& strTemplateName)
{
	ASSERT(nIndex >= 0);
	ASSERT(m_bIsFileLoaded);

	if (nIndex >= 0 && nIndex < m_arrTemplateData.GetSize())
		m_arrTemplateData[nIndex].strName = strTemplateName;
}

CString CTemplateLoader::GetFormattedText(int nIndex)
{
	ASSERT(nIndex >= 0 && nIndex < m_arrTemplateData.GetSize());
	ASSERT(m_bIsFileLoaded);
	ASSERT(m_arrTemplateData.GetSize() > 0);

	if (nIndex >= 0 && nIndex < m_arrTemplateData.GetSize())
		return CTemplateFormater::Format(m_arrTemplateData[nIndex].strText);

	return _T("");
}

TemplateData CTemplateLoader::GetTemplate(int nIndex)
{
	ASSERT(nIndex >= 0 && nIndex < m_arrTemplateData.GetSize());
	ASSERT(m_bIsFileLoaded);
	ASSERT(m_arrTemplateData.GetSize() > 0);

	if (nIndex >= 0 && nIndex < m_arrTemplateData.GetSize())
		return m_arrTemplateData.GetAt(nIndex);
	
	TemplateData xNullTemplate;
	xNullTemplate.strName = _T("");
	xNullTemplate.strText = _T("");
	xNullTemplate.nLevel  = 0;

	return xNullTemplate;
}

int CTemplateLoader::SetTemplate(int nIndex, TemplateData& xTemplate)
{
	ASSERT(nIndex >= 0);
	if (!m_bIsFileLoaded)
		m_bIsFileLoaded = TRUE;
	if (nIndex < 0)
		return -1;

	// If within the current item size, then we are mostly likely changing
	// the template item at the requested index...
	if (nIndex < m_arrTemplateData.GetSize())
	{
		// Change the template at the requested index,
		m_arrTemplateData.SetAt(nIndex, xTemplate);
		m_mapNameToIndex.Set(xTemplate.strName, nIndex);  //TODO--check well!
		return nIndex;
	}
	else //...otherwise, we are creating a new template.
	{
		// Add and obtain the index of the newly added item.
		int nNewIndex = m_arrTemplateData.Add(xTemplate);
		// Map the name of the template to the index.
		m_mapNameToIndex.Set(xTemplate.strName, nNewIndex);
		return nNewIndex;
	}
}

int CTemplateLoader::MatchName(LPCTSTR lpszName)
{
	POSITION pos = m_mapNameToIndex.Find(lpszName);
	if (pos != NULL)
	{
		return (int)m_mapNameToIndex.GetData(pos);
	}

	return -1;
}

