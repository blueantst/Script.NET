
#include "stdafx.h"
#include "../OwmScite.h"
#include "CodeSnippet.h"
#include <algorithm>

static bool isInList(generic_string word, const vector<generic_string> & wordArray)
{
	for (size_t i = 0 ; i < wordArray.size() ; i++)
		if (wordArray[i] == word)
			return true;
	return false;
};

CodeSnippet::CodeSnippet(ScintillaEditView * pEditView) : 
		_funcCompletionActive(false), _pEditView(pEditView), _funcCalltip(pEditView), 
		_curLang(L_TXT), _XmlFile(TEXT("")),
		_pXmlKeyword(NULL), _ignoreCase(true), _keyWords(TEXT(""))
{
	//Do not load any language yet
}

bool CodeSnippet::showCodeSnippet() {
	if (!_funcCompletionActive)
		return false;

	int curPos = int(_pEditView->execute(SCI_GETCURRENTPOS));
	int line = _pEditView->getCurrentLineNumber();
	int startLinePos = int(_pEditView->execute(SCI_POSITIONFROMLINE, line ));
	int startWordPos = startLinePos;

	int len = curPos-startLinePos;
	char * lineBuffer = new char[len+1];
	_pEditView->getText(lineBuffer, startLinePos, curPos);

	int offset = len-1;
	int nrChars = 0;
	char c;
	while (offset>=0)
	{
		c = lineBuffer[offset];
		if (isalnum(c) || c == '_') {
			nrChars++;
		} else {
			break;
		}
		offset--;
		
	}
	startWordPos = curPos-nrChars;

	_pEditView->execute(SCI_AUTOCSETSEPARATOR, WPARAM('\n'));
	_pEditView->execute(SCI_AUTOCSETIGNORECASE, _ignoreCase);
	_pEditView->showAutoComletion(curPos - startWordPos, _keyWords.c_str());

	return true;
}

void CodeSnippet::update(int character)
{
	const NppGUI & nppGUI = NppParameters::getInstance()->getNppGUI();
	if (!_funcCompletionActive && nppGUI._autocStatus == nppGUI.autoc_func)
		return;

	if (nppGUI._funcParams || _funcCalltip.isVisible()) {
		if (_funcCalltip.updateCalltip(character)) {	//calltip visible because triggered by autocomplete, set mode
			return;	//only return in case of success, else autocomplete
		}
	}

	if (!character)
		return;

	//If autocomplete already active, let Scintilla handle it
	if (_pEditView->execute(SCI_AUTOCACTIVE) != 0)
		return;

	const int wordSize = 64;
	TCHAR s[wordSize];
	_pEditView->getWordToCurrentPos(s, wordSize);
	showCodeSnippet();
/*	
	if (lstrlen(s) >= int(nppGUI._autocFromLen))
	{
		if (nppGUI._autocStatus == nppGUI.autoc_word)
			showWordComplete(false);
		else if (nppGUI._autocStatus == nppGUI.autoc_func)
			showAutoComplete();
	}*/
}

void CodeSnippet::callTipClick(int direction) {
	if (!_funcCompletionActive)
		return;

	if (direction == 1) {
		_funcCalltip.showPrevOverload();
	} else if (direction == 2) {
		_funcCalltip.showNextOverload();
	}
}

bool CodeSnippet::setLanguage(LangType language) {
	if (_curLang == language)
		return true;
	_curLang = language;

	TCHAR path[MAX_PATH];
	CString strOwmPath = theApp.GetModulePath();
	lstrcpy(path, strOwmPath);
	lstrcat(path, TEXT("\\Snippets\\"));
	lstrcat(path, getSnippetFileName());
	lstrcat(path, TEXT(".xml"));

	_XmlFile = TiXmlDocument(path);
	_funcCompletionActive = _XmlFile.LoadFile();

	TiXmlNode * pAutoNode = NULL;
	if (_funcCompletionActive) {
		_funcCompletionActive = false;	//safety
		TiXmlNode * pNode = _XmlFile.FirstChild(TEXT("CodeSnippets"));
		if (!pNode)
			return false;
		pAutoNode = pNode = pNode->FirstChildElement(TEXT("CodeSnippet"));
		if (!pNode)
			return false;
		pNode = pNode->FirstChildElement(TEXT("Snippet"));
		if (!pNode)
			return false;
		_pXmlKeyword = reinterpret_cast<TiXmlElement *>(pNode);
		if (!_pXmlKeyword)
			return false;
		_funcCompletionActive = true;
	}

	if(_funcCompletionActive) {	//try setting up environment
		//setup defaults
		_ignoreCase = true;
/*		_funcCalltip._start = '(';
		_funcCalltip._stop = ')';
		_funcCalltip._param = ',';
		_funcCalltip._terminal = ';';
		_funcCalltip._ignoreCase = true;

		TiXmlElement * pElem = pAutoNode->FirstChildElement(TEXT("Environment"));
		if (pElem) {	
			const TCHAR * val = 0;
			val = pElem->Attribute(TEXT("ignoreCase"));
			if (val && !lstrcmp(val, TEXT("no"))) {
				_ignoreCase = false;
				_funcCalltip._ignoreCase = false;
			}
			val = pElem->Attribute(TEXT("startFunc"));
			if (val && val[0])
				_funcCalltip._start = val[0];
			val = pElem->Attribute(TEXT("stopFunc"));
			if (val && val[0])
				_funcCalltip._stop = val[0];
			val = pElem->Attribute(TEXT("paramSeparator"));
			if (val && val[0])
				_funcCalltip._param = val[0];
			val = pElem->Attribute(TEXT("terminal"));
			if (val && val[0])
				_funcCalltip._terminal = val[0];
		}
*/
	}

	if (_funcCompletionActive) {
		_funcCalltip.setLanguageXML(_pXmlKeyword);
	} else {
		_funcCalltip.setLanguageXML(NULL);
	}

	_keyWords = TEXT("");
	if (_funcCompletionActive) {	//Cache the keywords
		//Iterate through all keywords
		TiXmlElement *snipNode = _pXmlKeyword;
		const TCHAR * name = NULL;
		for (; snipNode; snipNode = snipNode->NextSiblingElement(TEXT("Snippet")) ) {
			name = snipNode->Attribute(TEXT("name"));
			if (!name)		//malformed node
				continue;
			_keyWords.append(name);
			_keyWords.append(TEXT("\n"));
		}
	}
	return _funcCompletionActive;
}

const TCHAR * CodeSnippet::getSnippetFileName() {
	if (_curLang == L_USER)
	{
		/*Buffer * currentBuf = _pEditView->getCurrentBuffer();
		if (currentBuf->isUserDefineLangExt())
		{
			return currentBuf->getUserDefineLangName();
		}*/
	}

	if (_curLang >= L_EXTERNAL && _curLang < NppParameters::getInstance()->L_END)
		return NppParameters::getInstance()->getELCFromIndex(_curLang - L_EXTERNAL)._name;

	if (_curLang > L_EXTERNAL)
        _curLang = L_TXT;

	return ScintillaEditView::langNames[_curLang].lexerName;
}
