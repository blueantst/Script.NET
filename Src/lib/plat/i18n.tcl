#################################################################
# Script.NET 国际化支持脚本
# 作者:Blueant
# 版本:1.0
# 日期:2006-06-10
#################################################################

#----------------------------------------------------------------
# 设置语言字符串
# 用法: 分别用不同语言调用此函数,最终将会把当前语言对应的字符串
#		设置到变量strName中
#----------------------------------------------------------------
proc LangString { strName lang strValue } {
	upvar $strName _strName
	global platform_language
	if {$lang == $platform_language} {
		set _strName $strValue
	}
}

#----------------------------------------------------------------
# 设置语言字符串(同时设置英文和中文)
#----------------------------------------------------------------
proc LangStringEC { strName strValueEnglish strValueChinese } {
	upvar $strName _strName
	global platform_language
	switch $platform_language {
		english {set _strName $strValueEnglish}
		chinese {set _strName $strValueChinese}
	}
}

#----------------------------------------------------------------
# 根据当前语言,返回相应的文件名
# 参数: dir			- 文件路径
#		fileEnglish	- 英文文件名
#		fileChinese	- 中文文件名
#----------------------------------------------------------------
proc GetLangFile { dir fileEnglish fileChinese } {
	global platform_language
	switch $platform_language {
		english {set fileName "$dir/$fileEnglish"}
		chinese {set fileName "$dir/$fileChinese"}
	}
	if {[file exist $fileName]} {
		return $fileName
	} else {
		if {[file exist "$dir/$fileEnglish"]} {
			return "$dir/$fileEnglish"
		} elseif {[file exist "$dir/$fileChinese"]} {
			return "$dir/$fileChinese"
		}
	}
	return ""
}