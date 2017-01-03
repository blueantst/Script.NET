// CTemplateLoader.h: interface for the CTemplateLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTEMPLATELOADER_H__5ADF8586_60EE_11D4_86BF_9AE3ABC25C53__INCLUDED_)
#define AFX_CTEMPLATELOADER_H__5ADF8586_60EE_11D4_86BF_9AE3ABC25C53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>
#include "FastMaps.h"
#include "CTemplateFormater.h"

class CTemplateLoader  
{
public:
	CTemplateLoader(CString& strTmplFile);
	CTemplateLoader();
	virtual ~CTemplateLoader();

    virtual  BOOL Load();
    virtual  BOOL Load(CString& strTmplFilePath);

	BOOL     IsFileLoaded() const;
	CString  GetTemplateFilePath() const;
	void     SetTemplateFilePath(const CString& strFilePath);

	CString  GetTemplateText(int nIndex);
    void     SetTemplateText(int nIndex, const CString& strTemplateText);
	
	CString  GetTemplateName(int nIndex);
    void     SetTemplateName(int nIndex, const CString& strTemplateName);
	
	CString  GetFormattedText(int nIndex);
	
	TemplateData  GetTemplate(int nIndex);
    int           SetTemplate(int nIndex, TemplateData& xTemplate);
	
	int GetSize() const;

	int MatchName(LPCTSTR lpszName);
    
protected:
    CTemplateDataArray   m_arrTemplateData;
	CCStringToULong      m_mapNameToIndex;
	CString              m_strTmplFile;
	CCriticalSection     m_criSection;
	BOOL                 m_bIsFileLoaded;
};

//////////////////////////////////////////////////////////////////////
// CTemplateLoader inline methods

inline BOOL CTemplateLoader::IsFileLoaded()	const
{
	return m_bIsFileLoaded;
}

inline int CTemplateLoader::GetSize() const
{
	return m_arrTemplateData.GetSize();
}

inline CString CTemplateLoader::GetTemplateFilePath() const
{
	return m_strTmplFile;
}

inline void CTemplateLoader::SetTemplateFilePath(const CString& strFilePath)
{
	m_strTmplFile = strFilePath;
}

#endif // !defined(AFX_CTEMPLATELOADER_H__5ADF8586_60EE_11D4_86BF_9AE3ABC25C53__INCLUDED_)
