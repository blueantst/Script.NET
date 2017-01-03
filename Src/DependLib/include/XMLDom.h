////////////////////////////////////////////////////////////////////////////
//	File:		XMLDom.h
//	Version:	1.0.0.0
//	Created:	2005-07-08
//	Last Modify:2010-11-30
//
//	Author:		blueant
//
//	Xerces Dom Interface Wrap
////////////////////////////////////////////////////////////////////////////
#ifndef __XMLDOM_H_
#define __XMLDOM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define XML_USE_XERCES
#define XML_USE_TINYXML

#ifdef XML_USE_XERCES
// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#if defined(XERCES_NEW_IOSTREAMS)
#include <iostream>
#else
#include <iostream.h>
#endif
#include <list>

XERCES_CPP_NAMESPACE_USE

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/AbstractDOMParser.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMBuilder.hpp>
#include <xercesc/dom/DOMWriter.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMError.hpp>
#include <xercesc/dom/DOMLocator.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>


class attribute
{
public:
    attribute(const std::string & strName, const std::string & strValue);
    attribute();
public:
    std::string m_strName;
    std::string m_strValue;
};

// ---------------------------------------------------------------------------
//  This is a simple class that lets us do easy (though not terribly efficient)
//  trancoding of XMLCh data to local code page for display.
// ---------------------------------------------------------------------------
class StrX
{
public :
	// -----------------------------------------------------------------------
	//  Constructors and Destructor
	// -----------------------------------------------------------------------
	StrX(const XMLCh* const toTranscode)
	{
		// Call the private transcoding method
		fLocalForm = XMLString::transcode(toTranscode);
	}

	~StrX()
	{
		XMLString::release(&fLocalForm);
	}

	// -----------------------------------------------------------------------
	//  Getter methods
	// -----------------------------------------------------------------------
	const char* localForm() const
	{
		return fLocalForm;
	}

private :
	// -----------------------------------------------------------------------
	//  Private data members
	//
	//  fLocalForm
	//      This is the local code page form of the string.
	// -----------------------------------------------------------------------
	char*   fLocalForm;
};

inline XERCES_STD_QUALIFIER ostream& operator<<(XERCES_STD_QUALIFIER ostream& target, const StrX& toDump)
{
	target << toDump.localForm();
	return target;
}

// ---------------------------------------------------------------------------
//  Simple error handler deriviative to install on parser
// ---------------------------------------------------------------------------
class XMLDOMErrorHandler : public DOMErrorHandler
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
	XMLDOMErrorHandler() : fSawErrors(false) {}
    ~XMLDOMErrorHandler() {}

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    bool getSawErrors() const
	{
		return fSawErrors;
	}

    // -----------------------------------------------------------------------
    //  Implementation of the DOM ErrorHandler interface
    // -----------------------------------------------------------------------
    bool handleError(const DOMError& domError)
	{
		fSawErrors = true;
		if (domError.getSeverity() == DOMError::DOM_SEVERITY_WARNING)
			XERCES_STD_QUALIFIER cerr << "\nWarning at file ";
		else if (domError.getSeverity() == DOMError::DOM_SEVERITY_ERROR)
			XERCES_STD_QUALIFIER cerr << "\nError at file ";
		else
			XERCES_STD_QUALIFIER cerr << "\nFatal Error at file ";

		XERCES_STD_QUALIFIER cerr << StrX(domError.getLocation()->getURI())
			<< ", line " << domError.getLocation()->getLineNumber()
			<< ", char " << domError.getLocation()->getColumnNumber()
			<< "\n  Message: " << StrX(domError.getMessage()) << XERCES_STD_QUALIFIER endl;

		return true;
	}

    void resetErrors()
	{
		fSawErrors = false;
	}

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XMLDOMErrorHandler(const XMLDOMErrorHandler&);
    void operator=(const XMLDOMErrorHandler&);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fSawErrors
    //      This is set if we get any errors, and is queryable via a getter
    //      method. Its used by the main code to suppress output if there are
    //      errors.
    // -----------------------------------------------------------------------
    bool    fSawErrors;
};

// XML Node Type
enum XMLNodeType
{
	XMLNODE_ELEMENT			       = 1,
	XMLNODE_ATTRIBUTE              = 2,
	XMLNODE_TEXT                   = 3,
	XMLNODE_CDATA                  = 4,
	XMLNODE_ENTITY_REFERENCE       = 5,
	XMLNODE_ENTITY                 = 6,
	XMLNODE_PROCESSING_INSTRUCTION = 7,
	XMLNODE_COMMENT                = 8,
	XMLNODE_DOCUMENT               = 9,
	XMLNODE_DOCUMENT_TYPE          = 10,
	XMLNODE_DOCUMENT_FRAGMENT      = 11,
	XMLNODE_NOTATION               = 12
};

class CXmlParser : public CObject
{
public:
	CXmlParser()
	{
		m_strEncoding = "";
		m_nParserType = 0;
		m_impl = NULL;
		m_pDomParser	= NULL;
		m_pDomDoc = NULL;
		bool recognizeNEL = false;

		// Initialize the XML4C system
		try
		{
			XMLPlatformUtils::Initialize();

			if (recognizeNEL)
			{
				XMLPlatformUtils::recognizeNEL(recognizeNEL);
			}
		}

		catch (const XMLException& toCatch)
		{
			XERCES_STD_QUALIFIER cerr << "Error during initialization! :\n"
				<< StrX(toCatch.getMessage()) << XERCES_STD_QUALIFIER endl;
			return;
		}

		Init();
	}
	~CXmlParser()
	{
		Close();

		//
		//  Delete the parser itself.  Must be done prior to calling Terminate, below.
		//
		if(m_pDomParser)
		{
			m_pDomParser->release();
		}

		// And call the termination method
		XMLPlatformUtils::Terminate();
	}

//////////////////////////////////////////////////////////////////////////
// 提供给接口的功能函数
//////////////////////////////////////////////////////////////////////////
public:
	// 初始化XML解析器
	int Init()
	{
		AbstractDOMParser::ValSchemes valScheme = AbstractDOMParser::Val_Auto;
		bool                       doNamespaces       = false;
		bool                       doSchema           = false;
		bool                       schemaFullChecking = false;		

		// Instantiate the DOM parser.
		static const XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };
		DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(gLS);
		m_impl = impl;
		m_pDomParser = ((DOMImplementationLS*)impl)->createDOMBuilder(DOMImplementationLS::MODE_SYNCHRONOUS, 0);

		m_pDomParser->setFeature(XMLUni::fgDOMNamespaces, doNamespaces);
		m_pDomParser->setFeature(XMLUni::fgXercesSchema, doSchema);
		m_pDomParser->setFeature(XMLUni::fgXercesSchemaFullChecking, schemaFullChecking);

		if (valScheme == AbstractDOMParser::Val_Auto)
		{
			m_pDomParser->setFeature(XMLUni::fgDOMValidateIfSchema, true);
		}
		else if (valScheme == AbstractDOMParser::Val_Never)
		{
			m_pDomParser->setFeature(XMLUni::fgDOMValidation, false);
		}
		else if (valScheme == AbstractDOMParser::Val_Always)
		{
			m_pDomParser->setFeature(XMLUni::fgDOMValidation, true);
		}

		// enable datatype normalization - default is off
		m_pDomParser->setFeature(XMLUni::fgDOMDatatypeNormalization, true);

		// And create our error handler and install it
		m_pDomParser->setErrorHandler(&m_DomErrorHandler);

		return 0;
	}

	// 打开文件
	int Open(LPCTSTR lpcsFile, LPCTSTR lpszEncoding = "")
	{
		bool                       errorOccurred = false;

		//
		//  Get the starting time and kick off the parse of the indicated
		//  file. Catch any exceptions that might propogate out of it.
		//
		unsigned long duration;

		//reset error count first
		m_DomErrorHandler.resetErrors();

		try
		{
		
			// reset document pool
			m_pDomParser->resetDocumentPool();

			const unsigned long startMillis = XMLPlatformUtils::getCurrentMillis();
			m_pDomDoc = m_pDomParser->parseURI(lpcsFile);
			if(m_pDomDoc == NULL)
			{
				errorOccurred = true;
				return errorOccurred;
			}
			const unsigned long endMillis = XMLPlatformUtils::getCurrentMillis();
			duration = endMillis - startMillis;

			// Get and set encoding
			m_strEncoding = lpszEncoding;
			if(m_strEncoding != "")
			{
				m_pDomDoc->setEncoding(XMLString::transcode(m_strEncoding));
				//m_pDomDoc->setAcutalEncoding(XMLString::transcode(m_strEncoding));
			}else
			{
				const XMLCh* xsz = m_pDomDoc->getEncoding();
				if(xsz != NULL)
				{
					m_pDomDoc->setEncoding(xsz);
					//m_pDomDoc->setAcutalEncoding(xsz);
				}
			}
		}

		catch (const XMLException& toCatch)
		{
			XERCES_STD_QUALIFIER cerr << "\nError during parsing: '" << lpcsFile << "'\n"
				 << "Exception message is:  \n"
				 << StrX(toCatch.getMessage()) << "\n" << XERCES_STD_QUALIFIER endl;
			errorOccurred = true;
			return errorOccurred;
		}
		catch (const DOMException& toCatch)
		{
			const unsigned int maxChars = 2047;
			XMLCh errText[maxChars + 1];

			XERCES_STD_QUALIFIER cerr << "\nDOM Error during parsing: '" << lpcsFile << "'\n"
				 << "DOMException code is:  " << toCatch.code << XERCES_STD_QUALIFIER endl;

			if (DOMImplementation::loadDOMExceptionMsg(toCatch.code, errText, maxChars))
				 XERCES_STD_QUALIFIER cerr << "Message is: " << StrX(errText) << XERCES_STD_QUALIFIER endl;

			errorOccurred = true;
			return errorOccurred;
		}
		catch (...)
		{
			XERCES_STD_QUALIFIER cerr << "\nUnexpected exception during parsing: '" << lpcsFile << "'\n";
			errorOccurred = true;
			return errorOccurred;
		}

		//
		//  Extract the DOM tree, get the list of all the elements and report the
		//  length as the count of elements.
		//
		if (m_DomErrorHandler.getSawErrors())
		{
			XERCES_STD_QUALIFIER cout << "\nErrors occurred, no output available\n" << XERCES_STD_QUALIFIER endl;
			errorOccurred = true;
		}

		return errorOccurred;
	}

    // 设置编码方式
	int SetEncoding(LPCTSTR lpszEncoding)
	{
		bool                       errorOccurred = false;

		try
		{
			// Get and set encoding
			m_strEncoding = lpszEncoding;
			if(m_pDomDoc != NULL)
			{
				if(m_strEncoding != "")
				{
					m_pDomDoc->setEncoding(XMLString::transcode(m_strEncoding));
				}else
				{
					const XMLCh* xsz = m_pDomDoc->getEncoding();
					if(xsz != NULL)
					{
						m_pDomDoc->setEncoding(xsz);
					}
				}
			}
		}

		catch (const XMLException& toCatch)
		{
			XERCES_STD_QUALIFIER cerr << "\nError during set encoding\n"
				<< "Exception message is:  \n"
				<< StrX(toCatch.getMessage()) << "\n" << XERCES_STD_QUALIFIER endl;
			errorOccurred = true;
			return errorOccurred;
		}
		catch (const DOMException& toCatch)
		{
			const unsigned int maxChars = 2047;
			XMLCh errText[maxChars + 1];

			XERCES_STD_QUALIFIER cerr << "\nDOM Error during set encoding\n"
				<< "DOMException code is:  " << toCatch.code << XERCES_STD_QUALIFIER endl;

			if (DOMImplementation::loadDOMExceptionMsg(toCatch.code, errText, maxChars))
				XERCES_STD_QUALIFIER cerr << "Message is: " << StrX(errText) << XERCES_STD_QUALIFIER endl;

			errorOccurred = true;
			return errorOccurred;
		}
		catch (...)
		{
			XERCES_STD_QUALIFIER cerr << "\nUnexpected exception during set encoding\n";
			errorOccurred = true;
			return errorOccurred;
		}

		return errorOccurred;
	}

	// 关闭文件
	int Close()
	{
		return 0;
	}

	// 保存文件
	int Save(LPCTSTR lpcsFile)
	{
		bool                       errorOccurred = false;

		if(!m_impl)
		{
			return true;
		}

		//reset error count first
		m_DomErrorHandler.resetErrors();

		try
		{
			// construct the DOMWriter
			DOMWriter* domWriter = ((DOMImplementationLS*)m_impl)->createDOMWriter();

			// set encoding
			if(m_strEncoding != "")
			{
				domWriter->setEncoding(XMLString::transcode(m_strEncoding));
			}else
			{
				const XMLCh* xsz = m_pDomDoc->getEncoding();
				if(xsz != NULL)
				{
					domWriter->setEncoding(xsz);
				}
			}

			// optionally, set some DOMWriter features
			// set feature if the serializer supports the feature/mode 
			//if (domWriter->canSetFeature(XMLUni::fgDOMWRTSplitCdataSections, false)) 
			//	domWriter->setFeature(XMLUni::fgDOMWRTSplitCdataSections, false); 

			//if (domWriter->canSetFeature(XMLUni::fgDOMWRTDiscardDefaultContent, false)) 
			//	domWriter->setFeature(XMLUni::fgDOMWRTDiscardDefaultContent, false);

			// set the format-pretty-print feature
			if (domWriter->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
				domWriter->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);

			// set the byte-order-mark feature      
			if (domWriter->canSetFeature(XMLUni::fgDOMWRTBOM, true))
				domWriter->setFeature(XMLUni::fgDOMWRTBOM, true);

			m_pDomDoc->normalizeDocument() ;

			XMLFormatTarget* outfile = new LocalFileFormatTarget( lpcsFile ) ;
			domWriter->writeNode(outfile, *GetRootNode());

			// release the memory
			delete outfile;
			domWriter->release();
		}

		catch (const XMLException& toCatch)
		{
			XERCES_STD_QUALIFIER cerr << "\nError during parsing: '" << lpcsFile << "'\n"
				<< "Exception message is:  \n"
				<< StrX(toCatch.getMessage()) << "\n" << XERCES_STD_QUALIFIER endl;
			errorOccurred = true;
			return errorOccurred;
		}
		catch (const DOMException& toCatch)
		{
			const unsigned int maxChars = 2047;
			XMLCh errText[maxChars + 1];

			XERCES_STD_QUALIFIER cerr << "\nDOM Error during parsing: '" << lpcsFile << "'\n"
				<< "DOMException code is:  " << toCatch.code << XERCES_STD_QUALIFIER endl;

			if (DOMImplementation::loadDOMExceptionMsg(toCatch.code, errText, maxChars))
				XERCES_STD_QUALIFIER cerr << "Message is: " << StrX(errText) << XERCES_STD_QUALIFIER endl;

			errorOccurred = true;
			return errorOccurred;
		}
		catch (...)
		{
			XERCES_STD_QUALIFIER cerr << "\nUnexpected exception during parsing: '" << lpcsFile << "'\n";
			errorOccurred = true;
			return errorOccurred;
		}

		//
		//  Extract the DOM tree, get the list of all the elements and report the
		//  length as the count of elements.
		//
		if (m_DomErrorHandler.getSawErrors())
		{
			XERCES_STD_QUALIFIER cout << "\nErrors occurred, no output available\n" << XERCES_STD_QUALIFIER endl;
			errorOccurred = true;
		}

		return errorOccurred;
	}

	// 将内存中的XML解析成DOM树
	int ParseXMLStream(LPCTSTR lpXMLStream)
	{
		try
		{
			if (NULL == lpXMLStream)
			{
				return -1;
			}

			if (!m_pDomParser)
			{
				return -1;
			}

			if (m_pDomDoc)
			{
				m_pDomDoc->release();
			}

			////////////////////////////////////////////////////////
			MemBufInputSource* memBufIS = new MemBufInputSource((const XMLByte*)(lpXMLStream), \
				strlen(lpXMLStream), "1", false);
			if (!memBufIS)
			{
				return -1;
			}

			Wrapper4InputSource *pp = new Wrapper4InputSource(memBufIS, false);
			if (!pp)
			{
				delete memBufIS;
				return -1;
			}

			////////////////////////////////////////////////////////
			m_pDomParser->resetDocumentPool();
			m_pDomDoc = m_pDomParser->parse(*pp);

			delete memBufIS;
			delete pp;

			return 0;
		} catch (const XMLException &) {
			return -1;
		} catch (DOMException &) {
			return -1;
		} catch (...) {
			return -1;
		}
	}

	CString GetXMLString(const XMLCh *ch)
	{
		CString strName;
		char *name = XMLString::transcode(ch);
		strName = name;
		XMLString::release(&name);
		return strName;
	}

	// 获取根节点
	DOMNode* GetRootNode()
	{
		if(m_pDomDoc)
		{
			return (DOMNode*)(m_pDomDoc->getDocumentElement());
		}

		return NULL;
	}

	// 获取子节点
	DOMNode* GetChildNode(DOMNode* pNode, int nType = DOMNode::ELEMENT_NODE)
	{
		if(pNode)
		{
			DOMNode *child;
			for (child = pNode->getFirstChild(); child != 0; child=child->getNextSibling())
			{
				if(child != NULL && child->getNodeType() == nType)//DOMNode::ELEMENT_NODE)
				{
					return child;
				}
			}
		}

		return NULL;
	}

	// 获取下一个节点
	DOMNode* GetNextNode(DOMNode* pNode, int nType = DOMNode::ELEMENT_NODE)
	{
		if(pNode)
		{
			while ((pNode = pNode->getNextSibling()) != NULL)
			{
				if(pNode->getNodeType() == nType)
				{
					return pNode;
				}
			}
		}

		return NULL;
	}

	// 获取子节点(指定名字)
	DOMNode* GetChildNode(DOMNode* pNode, CString strName)
	{
		if(pNode)
		{
			DOMNode *child;
			for (child = pNode->getFirstChild(); child != 0; child=child->getNextSibling())
			{
				if(child != NULL && child->getNodeType() == DOMNode::ELEMENT_NODE)
				{
					CString str = GetXMLString(child->getNodeName());
					if(GetXMLString(child->getNodeName()) == strName)
					{
						return child;
					}
				}
			}
		}

		return NULL;
	}

	// 获取下一个节点(指定名字)
	DOMNode* GetNextNode(DOMNode* pNode, CString strName)
	{
		if(pNode)
		{
			while ((pNode = pNode->getNextSibling()) != NULL)
			{
				if(pNode->getNodeType() == DOMNode::ELEMENT_NODE)
				{
					if(GetXMLString(pNode->getNodeName()) == strName)
					{
						return pNode;
					}
				}
			}
		}

		return NULL;
	}

	// 获取节点名
	CString GetNodeName(DOMNode* pNode)
	{
		if(pNode)
		{
			return GetXMLString(pNode->getNodeName());
		}

		return "";
	}

	// 获取节点值
	CString GetNodeText(DOMNode* pNode, int nType = DOMNode::TEXT_NODE)
	{
		if(!pNode)
			return "";

		int nNodeType = pNode->getNodeType();
		if(DOMNode::ELEMENT_NODE == nNodeType)
		{
			pNode = GetChildNode(pNode, nType);//DOMNode::TEXT_NODE);
		}
		if(pNode && ((pNode->getNodeType() == DOMNode::TEXT_NODE) || (pNode->getNodeType() == DOMNode::CDATA_SECTION_NODE)))
			//if(pNode && (pNode->getNodeType() == DOMNode::TEXT_NODE))
		{
			return GetXMLString(pNode->getNodeValue());
		}

		return "";
	}

	// 设置节点值
	void SetNodeText(DOMNode* pNode, CString strNodeText, int nType = DOMNode::TEXT_NODE)
	{
		if(!pNode)
			return;

		DOMNode* pNodeTemp = NULL;
		if(DOMNode::ELEMENT_NODE == pNode->getNodeType())
		{
			pNodeTemp = GetChildNode(pNode, nType);//DOMNode::TEXT_NODE);
			if(pNodeTemp != NULL)
			{
				pNode = pNodeTemp;
			}else
			{
				XMLCh* ch = XMLString::transcode(strNodeText.GetBuffer(0));
				if(nType == DOMNode::TEXT_NODE)
				{
					DOMText* pNodeText = m_pDomDoc->createTextNode(ch);
					pNode->appendChild((DOMNode*)pNodeText);
				}else
				if(nType == DOMNode::CDATA_SECTION_NODE)
				{
					DOMCDATASection* pNodeCData = m_pDomDoc->createCDATASection(ch);
					pNode->appendChild((DOMNode*)pNodeCData);
				}
				XMLString::release(&ch);
				strNodeText.ReleaseBuffer();
				return;
			}
		}
		//if(pNode && (pNode->getNodeType() == DOMNode::TEXT_NODE))
		if(pNode && ((pNode->getNodeType() == DOMNode::TEXT_NODE) || (pNode->getNodeType() == DOMNode::CDATA_SECTION_NODE)))
		{
			XMLCh* ch = XMLString::transcode(strNodeText.GetBuffer(0));
			pNode->setNodeValue(ch);
			XMLString::release(&ch);
			strNodeText.ReleaseBuffer();
		}
	}

	// 获取节点属性
	CString GetNodeAttribute(DOMNode* pNode, CString strAttrName)
	{
		if(pNode && pNode->hasAttributes())
		{
			// get all the attributes of the node
			DOMNamedNodeMap *pAttributes = pNode->getAttributes();
			int nSize = pAttributes->getLength();
			for(int i=0;i<nSize;++i)
			{
				DOMAttr *pAttributeNode = (DOMAttr*) pAttributes->item(i);
				// get attribute name
				CString strName = GetXMLString(pAttributeNode->getName());
				if(strName == strAttrName)
				{
					return GetXMLString(pAttributeNode->getValue());
				}
			}
		}

		return "";
	}
	
	// 设置节点属性
	void SetNodeAttribute(DOMNode* pNode, CString strAttrName, CString strAttrValue)
	{
		if(pNode && pNode->hasAttributes())
		{
			// get all the attributes of the node
			DOMNamedNodeMap *pAttributes = pNode->getAttributes();
			int nSize = pAttributes->getLength();
			for(int i=0;i<nSize;++i)
			{
				DOMAttr *pAttributeNode = (DOMAttr*) pAttributes->item(i);
				// get attribute name
				CString strName = GetXMLString(pAttributeNode->getName());
				if(strName == strAttrName)
				{
					XMLCh* ch = XMLString::transcode(strAttrValue.GetBuffer(0));
					pAttributeNode->setValue(ch);
					XMLString::release(&ch);
					strAttrValue.ReleaseBuffer();
					return;
				}
			}
		}

		if(pNode)
		{
			// 不存在属性,就创建
			XMLCh* chName = XMLString::transcode(strAttrName.GetBuffer(0));
			XMLCh* chValue = XMLString::transcode(strAttrValue.GetBuffer(0));
			((DOMElement*)pNode)->setAttribute(chName, chValue);
			XMLString::release(&chName);
			XMLString::release(&chValue);
			strAttrName.ReleaseBuffer();
			strAttrValue.ReleaseBuffer();
		}
	}

	// 获取节点属性名列表
	int GetNodeAttributeNames(DOMNode* pNode, CStringArray& arAttrName)
	{
		arAttrName.RemoveAll();
		if(pNode->hasAttributes())
		{
			// get all the attributes of the node
			DOMNamedNodeMap *pAttributes = pNode->getAttributes();
			int nSize = pAttributes->getLength();
			for(int i=0;i<nSize;++i)
			{
				DOMAttr *pAttributeNode = (DOMAttr*) pAttributes->item(i);
				// get attribute name
				CString strName = GetXMLString(pAttributeNode->getName());
				arAttrName.Add(strName);
			}
		}

		return arAttrName.GetSize();
	}

	// 解析\分隔的串(xx\xx\xx)
	void SplitItem(CString strSrc, CStringArray& asItem)
	{
		strSrc.TrimLeft();
		strSrc.TrimRight();
		int nPos;
		while((nPos = strSrc.Find("\\")) != -1)
		{
			CString strTmp = strSrc.Left(nPos);
			strTmp.TrimRight();
			if(strTmp.GetLength() > 0)
				asItem.Add(strTmp);
			strSrc.Delete(0, nPos+1);
			strSrc.TrimLeft();
		}
		if(strSrc.GetLength() > 0)
			asItem.Add(strSrc);
	}

	// 获取子节点(根据路径)
	DOMNode* GetNode(CString strNodePath)
	{
		CStringArray arPath;
		SplitItem(strNodePath, arPath);

		DOMNode* pNode = GetRootNode();
		for(int i=0; i<arPath.GetSize(); i++)
		{
			pNode = GetChildNode(pNode, arPath[i]);
			if(pNode == NULL)
			{
				break;
			}
		}

		return pNode;
	}

	// 获取节点值
	CString GetNodeText(CString strNodePath, int nType = DOMNode::TEXT_NODE)
	{
		DOMNode* pNode = GetNode(strNodePath);
		return GetNodeText(pNode, nType);
	}

	// 设置节点值
	void SetNodeText(CString strNodePath, CString strNodeText, int nType = DOMNode::TEXT_NODE)
	{
		DOMNode* pNode = GetNode(strNodePath);
		SetNodeText(pNode, strNodeText, nType);
	}

	// 获取节点属性
	CString GetNodeAttribute(CString strNodePath, CString strAttrName)
	{
		DOMNode* pNode = GetNode(strNodePath);
		return GetNodeAttribute(pNode, strAttrName);
	}

	// 设置节点属性
	void SetNodeAttribute(CString strNodePath, CString strAttrName, CString strAttrValue)
	{
		DOMNode* pNode = GetNode(strNodePath);
		SetNodeAttribute(pNode, strAttrName, strAttrValue);
	}

	// 创建节点
	DOMNode* CreateChildNode(DOMNode* pNode, CString strNodeName)
	{
		if(!pNode)
			return NULL;

		int nType = pNode->getNodeType();
		if(DOMNode::ELEMENT_NODE == nType)
		{
			XMLCh* ch = XMLString::transcode(strNodeName.GetBuffer(0));
			DOMElement*  prodElem = m_pDomDoc->createElement(ch);
			pNode->appendChild((DOMNode*)prodElem);
			XMLString::release(&ch);
			strNodeName.ReleaseBuffer();
			return (DOMNode*)prodElem;
		}

		return NULL;
	}

	// 删除节点
	BOOL RemoveChildNode(DOMNode* pNode, DOMNode* pChildNode)
	{
		if(!pNode)
			return FALSE;

		int nType = pNode->getNodeType();
		if(DOMNode::ELEMENT_NODE == nType)
		{
			if(pNode->removeChild(pChildNode) != NULL)
			{
				return TRUE;
			}
		}

		return FALSE;
	}

	// 创建DOM根节点
	DOMNode* CreateDomRoot(const LPCTSTR lpName)
	{
		try
		{
			if (NULL == lpName)
			{
				return NULL;
			}
		
			if (!m_impl)
			{
				return NULL;
			}
		
			if (m_pDomDoc)
			{
				m_pDomDoc->release();
			}
		
			XMLCh* ch = XMLString::transcode(lpName);
			m_pDomDoc = m_impl->createDocument(0, ch, 0);

			if(m_strEncoding != "")
			{
				SetEncoding(m_strEncoding);
			}

			return m_pDomDoc->getDocumentElement();//*(m_listElement.begin());
		} catch (...) {
			return NULL;
		}
	}

	// 添加DOM节点
	int AddDomNode(const LPCTSTR lpName, const LPCTSTR lpValue,DOMNode* pParent)
	{
		try
		{
			if (!pParent)
			{
				return -1;
			}

			if (!m_pDomDoc)
			{
				return -1;
			}

			XMLCh* chName = XMLString::transcode(lpName);
			XMLCh* chValue = XMLString::transcode(lpValue);

			DOMElement *newElement  = m_pDomDoc->createElement(chName);
			DOMText *elementContent = m_pDomDoc->createTextNode(chValue);
			if (newElement && elementContent)
			{
				newElement->appendChild(elementContent);

				pParent->appendChild(newElement);

				return 0;
			}
			else
			{
				return -1;
			}
		} catch (...) {
			return -1;
		}
	}

	// 添加节点属性
	int AddAttribute(DOMNode* pParent,  std::list<attribute> lsAttribute)
	{
		if (!pParent)
		{
			return -1;
		}

		DOMElement* pp = (DOMElement*)pParent;
		std::list <attribute>::iterator iter = lsAttribute.begin();
		std::list <attribute>::iterator enditer = lsAttribute.end();
		for (; iter != enditer; ++iter)
		{
			XMLCh* arName = XMLString::transcode((*iter).m_strName.c_str());
			XMLCh* arValue = XMLString::transcode((*iter).m_strValue.c_str());
			pp->setAttribute(arName, arValue);
		}

		return 0;
	}

	// DOM节点内容写入字符串
	CString  WriteToStr()
	{
		try
		{
			if (m_pDomDoc && m_impl)
			{
				XMLFormatTarget *myFormTarget = new MemBufFormatTarget();
				DOMWriter *theSerializer = ((DOMImplementationLS*)m_impl)->createDOMWriter();
				theSerializer->writeNode(myFormTarget, *m_pDomDoc);
				int nLen  = (int)((MemBufFormatTarget*)myFormTarget)->getLen() + 1;
				char *buf = new char[(unsigned int)nLen];
				memset(buf, 0, (unsigned int)nLen);
				memcpy(buf, ((MemBufFormatTarget*)myFormTarget)->getRawBuffer(), (unsigned int)(nLen - 1));
				theSerializer->release();
				delete myFormTarget;
				std::string strResult = buf;
				delete[] buf;
				return strResult.c_str();
			}
			else
			{
				return "";
			}
		} catch (...) {
			return "";
		}
	}

	// DOM节点内容写入文件
	int   WriteToFile(const LPCTSTR lpFileName)
	{
		try
		{
			bool bSucc = false;
			if (!m_pDomDoc && !m_pDomParser)
			{
				return -1;
			}

			////////////////////////////////////////////////////////////
			XMLFormatTarget *myFormTarget = new LocalFileFormatTarget(lpFileName);
			if (!myFormTarget)
			{
				return -1;
			}

			////////////////////////////////////////////////////////////
			if (m_impl)
			{
				DOMWriter *theSerializer = ((DOMImplementationLS*)m_impl)->createDOMWriter();
				if (theSerializer && m_pDomDoc)
				{
					bSucc = theSerializer->writeNode(myFormTarget, *m_pDomDoc);
					theSerializer->release();
				}

				delete myFormTarget;
				if (bSucc)
				{
					return 0;
				}

				return -1;
			}

			delete myFormTarget;
			return -1;
		} catch (const XMLException &) {
			return -1;
		} catch (DOMException &) {
			return -1;
		} catch (...) {
			return -1;
		}
	}

private:
	int					m_nParserType;	// 解析器类型
	DOMImplementation*	m_impl;			// XML实现
	DOMBuilder*			m_pDomParser;	// DOM解析器
	XMLDOMErrorHandler	m_DomErrorHandler;	// DOM错误处理器
	XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *m_pDomDoc;	// DOM文档

	CString				m_strEncoding;	// 编码
};

// 自动链接Xerces库
#ifdef _DEBUG
#pragma comment(lib, "xerces-c_2D.lib")
#else
#pragma comment(lib, "xerces-c_2.lib")
#endif
#pragma message("Automatically linking with xerces library")

#endif	// XML_USE_XERCES

#ifdef XML_USE_TINYXML

#ifdef TIXML_USE_STL
#include <iostream>
#include <sstream>
using namespace std;
#endif

#include "tinystr.h"
#include "tinyxml.h"

#define DOMNode	TiXmlNode

// XML Node Type
enum XMLNodeType
{
	XMLNODE_DOCUMENT,
	XMLNODE_ELEMENT,
	XMLNODE_COMMENT,
	XMLNODE_UNKNOWN,
	XMLNODE_TEXT,
	XMLNODE_DECLARATION,
	XMLNODE_TYPECOUNT,
	XMLNODE_CDATA			// 此类型非TinyXML的类型,是为了设置CDATA类型节点增加的
};

class CXmlParser : public CObject
{
public:
	CXmlParser()
	{
		m_xmlEncoding = TIXML_ENCODING_UNKNOWN;
	}

	~CXmlParser()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	// 提供给接口的功能函数
	//////////////////////////////////////////////////////////////////////////
public:
	// 初始化XML解析器
	int Init()
	{
		return 0;
	}

	// 打开文件
	int Open(LPCTSTR lpcsFile, LPCTSTR lpszEncoding = "")
	{
		m_strFileName = lpcsFile;
		SetEncoding(lpszEncoding);
		return m_xmlDom.LoadFile(lpcsFile, m_xmlEncoding) ? 0 : 1;
	}

	// 设置编码方式
	int SetEncoding(LPCTSTR lpszEncoding)
	{
		CString strEncoding = lpszEncoding;
		if(strEncoding == _T("utf-8"))
		{
			m_xmlEncoding = TIXML_ENCODING_UTF8;
		}else
		if(strEncoding == _T("legacy"))
		{
			m_xmlEncoding = TIXML_ENCODING_LEGACY;
		}else
		{
			m_xmlEncoding = TIXML_ENCODING_UNKNOWN;
		}

		return 0;
	}

	// 关闭文件
	int Close()
	{
		m_xmlDom.Clear();
		return 0;
	}

	// 保存文件
	int Save(LPCTSTR lpcsFile)
	{
		CString strFileName = lpcsFile;
		if(strFileName = _T(""))
		{
			strFileName = m_strFileName;
		}
		return m_xmlDom.SaveFile(strFileName) ? 0 : 1;
	}

	// 将内存中的XML解析成DOM树
	int ParseXMLStream(LPCTSTR lpXMLStream)
	{
		m_xmlDom.Parse(lpXMLStream, 0, m_xmlEncoding);
		if (m_xmlDom.Error())
		{
			return -1;
		}
		return 0;
	}

	// 获取根节点
	DOMNode* GetRootNode()
	{
		return m_xmlDom.RootElement();
	}

	// 获取子节点
	DOMNode* GetChildNode(DOMNode* pNode, int nType = XMLNODE_ELEMENT)
	{
		if(pNode)
		{
			TiXmlNode *child;
			for (child = pNode->FirstChild(); child != 0; child=child->NextSibling())
			{
				if(child != NULL && child->Type() == nType)
				{
					return child;
				}
			}
		}

		return NULL;
	}

	// 获取下一个节点
	DOMNode* GetNextNode(DOMNode* pNode, int nType = XMLNODE_ELEMENT)
	{
		if(pNode)
		{
			while ((pNode = pNode->NextSibling()) != NULL)
			{
				if(pNode->Type() == nType)
				{
					return pNode;
				}
			}
		}

		return NULL;
	}

	// 获取子节点(指定名字)
	DOMNode* GetChildNode(DOMNode* pNode, CString strName)
	{
		if(pNode)
		{
			TiXmlNode *child;
			for (child = pNode->FirstChild(); child != 0; child=child->NextSibling())
			{
				if(child != NULL && child->Type() == XMLNODE_ELEMENT)
				{
					CString str = child->Value();
					if(str == strName)
					{
						return child;
					}
				}
			}
		}

		return NULL;
	}

	// 获取下一个节点(指定名字)
	DOMNode* GetNextNode(DOMNode* pNode, CString strName)
	{
		if(pNode)
		{
			while ((pNode = pNode->NextSibling()) != NULL)
			{
				if((pNode->Type() == XMLNODE_ELEMENT) && (pNode->Value() == strName))
				{
					return pNode;
				}
			}
		}

		return NULL;
	}

	// 获取节点名
	CString GetNodeName(DOMNode* pNode)
	{
		if(pNode)
		{
			return pNode->Value();
		}

		return "";
	}

	// 获取节点值
	CString GetNodeText(DOMNode* pNode, int nType = XMLNODE_TEXT)
	{
		if(!pNode)
			return "";

		if(XMLNODE_CDATA == nType)
		{
			nType = XMLNODE_TEXT;
		}

		int nNodeType = pNode->Type();
		if(XMLNODE_ELEMENT == nNodeType)
		{
			pNode = GetChildNode(pNode, nType);
		}
		if(pNode && (pNode->Type() == XMLNODE_TEXT))	// CDATA也是TEXT类型
		{
			return pNode->ToText()->Value();
		}

		return "";
	}

	// 设置节点值
	void SetNodeText(DOMNode* pNode, CString strNodeText, int nType = XMLNODE_TEXT)
	{
		if(!pNode)
			return;

		DOMNode* pNodeTemp = pNode;
		if(XMLNODE_ELEMENT == pNode->Type())
		{
			pNodeTemp = GetChildNode(pNode, nType);
			if(pNodeTemp == NULL)
			{
				// 如果没有内容节点就创建一个Text节点
				TiXmlText nodeChild(_T(""));
				pNodeTemp = pNode->InsertEndChild(nodeChild);
			}
		}
		if(!pNodeTemp)
			return;

		TiXmlText* pNodeText = pNodeTemp->ToText();
		if(pNodeText)
		{
			if(nType == XMLNODE_CDATA)
			{
				pNodeText->SetCDATA(true);
			}
			pNodeText->SetValue(strNodeText);
		}
	}

	// 获取节点属性
	CString GetNodeAttribute(DOMNode* pNode, CString strAttrName)
	{
		if(pNode && (XMLNODE_ELEMENT == pNode->Type()))
		{
			return pNode->ToElement()->Attribute(strAttrName);
		}

		return "";
	}

	// 设置节点属性
	void SetNodeAttribute(DOMNode* pNode, CString strAttrName, CString strAttrValue)
	{
		if(pNode && (XMLNODE_ELEMENT == pNode->Type()))
		{
			pNode->ToElement()->SetAttribute(strAttrName, strAttrValue);
		}
	}

	// 获取节点属性名列表
	int GetNodeAttributeNames(DOMNode* pNode, CStringArray& arAttrName)
	{
		arAttrName.RemoveAll();
		if(pNode && (XMLNODE_ELEMENT == pNode->Type()))
		{
			TiXmlAttribute* pAttribute;
			for (pAttribute = pNode->ToElement()->FirstAttribute(); pAttribute != 0; pAttribute=pAttribute->Next())
			{
				arAttrName.Add(pAttribute->Name());
			}
		}

		return arAttrName.GetSize();
	}

	// 解析\分隔的串(xx\xx\xx)
	void SplitItem(CString strSrc, CStringArray& asItem)
	{
		strSrc.TrimLeft();
		strSrc.TrimRight();
		int nPos;
		while((nPos = strSrc.Find("\\")) != -1)
		{
			CString strTmp = strSrc.Left(nPos);
			strTmp.TrimRight();
			if(strTmp.GetLength() > 0)
				asItem.Add(strTmp);
			strSrc.Delete(0, nPos+1);
			strSrc.TrimLeft();
		}
		if(strSrc.GetLength() > 0)
			asItem.Add(strSrc);
	}

	// 获取子节点(根据路径)
	DOMNode* GetNode(CString strNodePath)
	{
		CStringArray arPath;
		SplitItem(strNodePath, arPath);

		DOMNode* pNode = GetRootNode();
		for(int i=0; i<arPath.GetSize(); i++)
		{
			pNode = GetChildNode(pNode, arPath[i]);
			if(pNode == NULL)
			{
				break;
			}
		}

		return pNode;
	}

	// 获取节点值
	CString GetNodeText(CString strNodePath, int nType = XMLNODE_TEXT)
	{
		DOMNode* pNode = GetNode(strNodePath);
		return GetNodeText(pNode, nType);
	}

	// 设置节点值
	void SetNodeText(CString strNodePath, CString strNodeText, int nType = XMLNODE_TEXT)
	{
		DOMNode* pNode = GetNode(strNodePath);
		SetNodeText(pNode, strNodeText, nType);
	}

	// 获取节点属性
	CString GetNodeAttribute(CString strNodePath, CString strAttrName)
	{
		DOMNode* pNode = GetNode(strNodePath);
		return GetNodeAttribute(pNode, strAttrName);
	}

	// 设置节点属性
	void SetNodeAttribute(CString strNodePath, CString strAttrName, CString strAttrValue)
	{
		DOMNode* pNode = GetNode(strNodePath);
		SetNodeAttribute(pNode, strAttrName, strAttrValue);
	}

	// 创建节点
	DOMNode* CreateChildNode(DOMNode* pNode, CString strNodeName)
	{
		if(!pNode)
			return NULL;

		if(XMLNODE_ELEMENT == pNode->Type())
		{
			TiXmlElement nodeChild(strNodeName);
			return pNode->InsertEndChild(nodeChild);
		}

		return NULL;
	}

	// 删除节点
	BOOL RemoveChildNode(DOMNode* pNode, DOMNode* pChildNode)
	{
		if(!pNode)
			return FALSE;

		return pNode->RemoveChild(pChildNode);
	}

	// 创建DOM根节点
	DOMNode* CreateDomRoot(const LPCTSTR lpName)
	{
		TiXmlElement nodeChild(lpName);
		return m_xmlDom.InsertEndChild(nodeChild);
	}

	// 添加DOM节点
	int AddDomNode(const LPCTSTR lpName, const LPCTSTR lpValue,DOMNode* pParent)
	{
		DOMNode* pNode = CreateChildNode(pParent, lpName);
		if(pNode == NULL)
			return -1;
		SetNodeText(pNode, lpValue);
		return 0;
	}

	// DOM节点内容写入字符串
	CString  WriteToStr()
	{
		TiXmlPrinter printer;
		m_xmlDom.Accept(&printer);
		return printer.CStr();
	}

	// DOM节点内容写入文件
	int   WriteToFile(const LPCTSTR lpFileName)
	{
		return m_xmlDom.SaveFile(lpFileName) ? 0 : -1;
	}

private:
	TiXmlDocument	m_xmlDom;
	TiXmlEncoding	m_xmlEncoding;	// 编码
	CString			m_strFileName;
};

// 自动链接TinyXML库
#ifdef _DEBUG
#pragma comment(linker,"/nodefaultlib:LIBCMTd.lib")
#pragma comment(lib, "tinyxmld.lib")
#else
#pragma comment(linker,"/nodefaultlib:LIBCMT.lib")
#pragma comment(lib, "tinyxml.lib")
#endif
#pragma message("Automatically linking with TinyXML library")

#endif	// XML_USE_TINYXML


#endif // !defined(__XMLDOM_H_)