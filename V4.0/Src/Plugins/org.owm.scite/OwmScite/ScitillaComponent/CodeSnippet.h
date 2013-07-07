
#ifndef CODESNIPPET_H
#define CODESNIPPET_H

#include "ScintillaEditView.h"
#include "FunctionCallTip.h"
#include "tinyxml.h"

class CodeSnippet {
public:
	CodeSnippet(ScintillaEditView * pEditView);
	bool setLanguage(LangType language);

	//CodeSnippet from the list
	bool showCodeSnippet();

	void update(int character);
	void callTipClick(int direction);

private:
	bool _funcCompletionActive;
	ScintillaEditView * _pEditView;
	LangType _curLang;
	TiXmlDocument _XmlFile;
	TiXmlElement * _pXmlKeyword;

	bool _ignoreCase;

	std::generic_string _keyWords;

	FunctionCallTip _funcCalltip;
	const TCHAR * getSnippetFileName();
};

#endif //CODESNIPPET_H
