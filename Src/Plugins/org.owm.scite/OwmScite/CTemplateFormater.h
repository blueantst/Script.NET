// CTemplateFormater.h: interface for the CTemplateFormater class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTEMPLATEFORMATER_H__6D962357_5F8F_11D4_86B6_E45694D14953__INCLUDED_)
#define AFX_CTEMPLATEFORMATER_H__6D962357_5F8F_11D4_86B6_E45694D14953__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileUtils.h"

#define MAX_SPEC_SIZE     4    // Maximum size of a specifier
#define MAX_KW_SIZE       20   // Maximum size of a keyword

#define ARRAY_SIZE(Array) (sizeof(Array) / sizeof((Array)[0]))


// The following define the delimiters, keywords, and
// format specifiers supported by the template framework.

// Main item
#define ITEM_START_DELIM         _T("#{")
#define ITEM_END_DELIM           _T("}#")
							     
// Sub items				     
#define SUBITEM_START_DELIM      _T("#[")
#define SUBITEM_END_DELIM        _T("]#")
							     
// Formatting specifiers	     
#define FORMAT_BEGIN             _T("#<")
#define FORMAT_END               _T(">#")
							     
// Keyword specifiers		     
#define KEYWORD_BEGIN            _T("#%")
#define KEYWORD_END              _T("%#")
							     
// Replacable parameters	     
#define PARAM_BEGIN				 _T("#@")
#define PARAM_END				 _T("@#")
							     
// Cursor positioning		     
#define CURSOR_POS				 _T("^$")


/////////////////////////////////////////////////////////////////////////////
//  struct TemplateData/CTemplateDataArray - storage for our code templates

typedef struct tagTemplateData
{
    CString   strName;
    CString   strText;
	int       nLevel;
}TemplateData;

typedef CArray <TemplateData, TemplateData&> CTemplateDataArray;

// Format specifier functions accept the string to format as input
// and return the modified string as output.

typedef CString(*SpecifierProc)(const CString&);
typedef struct tagSpecData
{
	TCHAR szSpecifier[MAX_SPEC_SIZE];    // The specifier,
	SpecifierProc Proc;                  // The specifier processor function
}SpecData;

// Keyword functions return the string to use in place of
// the keyword.

typedef CString(*KWProc)(void);
typedef struct tagKWData
{
	TCHAR  szKeyword[MAX_KW_SIZE];    // The keyword
	KWProc Proc;                      // The keyword processor function
}KWData;

/////////////////////////////////////////////////////////////////////////////
// CTemplateFormater class

class CTemplateFormater  
{
public:
	CTemplateFormater(const CString& strSource);
	CTemplateFormater();
	virtual ~CTemplateFormater();
	
	// operations
	CString Format(void);
	void SetText(const CString& strSource)
	{
		m_strSource = strSource;
	}
	
	CString GetText() const
	{
		return m_strSource;
	}
	
	static CString Format(const CString& strSource);
	
protected:
	//   Replacable parameter functions
	void GetReplacebleParams(CStringArray& ParamTable, 
		const CString& szSource);
	CString GetParam(const CString& szPrompt);
	
	// Format specifier processing functions
	static CString FormatLower(const CString& strSource);
	static CString FormatUpper(const CString& strSource);
	static CString FormatWidth(const CString& strSource);
	
	// Keyword processing functions
	static CString GetDate(void);
	static CString GetExtOnly(void);
	static CString GetFile(void);		 // TODO
	static CString GetFileNameOnly(void);
	static CString GetPath(void);		 // TODO
	static CString GetProject(void);	 // TODO
	static CString GetTime(void);
	static CString GetTime24(void);
	static CString GetLongDate(void);
	static CString GetUserName(void);	//TODO
	
	// Other functions
	CString ExtractText(const CString, const CString, const int, int&);
	BOOL    GetNextInstance(const CString, const CString, int&);
	CString ReplaceSpecifier(const CString, int, int, const CString);
	
private:
	// Array of format specifiers, be sure to use uppercase and leave
	// off the delimiters.  Make sure the delimiter has a space so
	// it is easier to read.
	static const SpecData s_FormatSpecTable[];
	
	// Array of keywords, be sure to use uppercase and leave off
	// the delimiters
	static const KWData s_KeywordTable[];
	
	CString m_strSource;
};

#endif // !defined(AFX_CTEMPLATEFORMATER_H__6D962357_5F8F_11D4_86B6_E45694D14953__INCLUDED_)
