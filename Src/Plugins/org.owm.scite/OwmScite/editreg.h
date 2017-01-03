////////////////////////////////////////////////////////////////////////////
//	File:		editcmd.h
//	Version:	1.0.0.0
//
////////////////////////////////////////////////////////////////////////////

#ifndef __CRYSEDITREG_H__INCLUDED_
#define __CRYSEDITREG_H__INCLUDED_

//	Registry keys & values
#define REG_OWMEDIT			_T("owm\\OwmScite")
#define REG_DEFAULTDOC		_T("DefaultDoc")
#define REG_INTERPNAME		_T("InterpName")
#define REG_AUTORESET_INTERP	_T("AutoResetInterp")
#define REG_EDITORSTYLE		_T("EditorStyle")

#define REG_FIND_SUBKEY		_T("owm\\OwmScite\\Find")
#define REG_REPLACE_SUBKEY	_T("owm\\OwmScite\\Replace")
#define REG_FIND_HISTORY	_T("owm\\OwmScite\\FindHistory")
#define REG_MATCH_CASE		_T("MatchCase")
#define REG_WHOLE_WORD		_T("WholeWord")
#define REG_FIND_WHAT		_T("FindWhat")
#define REG_REPLACE_WITH	_T("ReplaceWith")

#define REG_PAGE_SUBKEY		_T("owm\\OwmScite\\PageSetup")
#define REG_MARGIN_LEFT		_T("LeftMargin")
#define REG_MARGIN_RIGHT	_T("RightMargin")
#define REG_MARGIN_TOP		_T("TopMargin")
#define REG_MARGIN_BOTTOM	_T("BottomMargin")

// 编辑器的Python子键
#define REG_PYTHON_SUBKEY		_T("owm\\OwmScite\\Python")
#define REG_PYTHON_TAB2SPACE	_T("Tab2Space")		// TAB转换为空格

// 帮助子健
#define REG_HELP_SUBKEY			_T("owm\\OwmScite\\Help")
#define REG_HELP_WORD_TOOLTIP	_T("EnableWordTooltip")		// 允许显示单词提示信息
#define REG_HELP_VAR_TOOLTIP	_T("EnableVarTooltip")		// 允许在调试状态下显示变量提示信息
#define REG_HELP_WORDHELP_IN_DYNHELP _T("ShowWordHelpInDynamicHelp")	// 在动态帮助窗口中显示单词帮助
#define REG_HELP_SEARCHWORD_ACCURATE _T("SearchWordAccurate")	// 使用精确匹配进行单词帮助搜索
#define REG_HELP_AUTO_COMPLETE	_T("AutoComplete")			// 启用命令的自动完成功能

// 定义字体格式子键
#define REG_FORMAT_SUBKEY		_T("Format")
#define REG_FMFONT				_T("Font")			// 字体名
#define REG_FMSIZE				_T("Size")			// 大小
#define REG_FMCOLORFG			_T("ColorFg")		// 前景色
#define REG_FMCOLORBK			_T("ColorBk")		// 背景色
#define REG_FMBOLD				_T("Bold")			// 是否粗体
#define REG_FMITALIC			_T("Italic")		// 是否斜体
#define REG_FMUNDERLINE			_T("UnderLine")		// 下划线
#define REG_FMSTRIKEOUT			_T("StrikeOut")		// 删除线


#endif
