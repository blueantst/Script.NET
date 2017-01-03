#################################################################
# File       : extendtools.tcl									#
# Author     : Blueant Software									#
# Version    : 1.0												#
# Date       : 2007-06-10										#
# Description: Script.NET extend tools page script				#
#################################################################

package require TclFace
package require Mk4tcl

#-------------------------------------------------------------
# TLFAppPage class define
#-------------------------------------------------------------
class  TLFAppPage {
	inherit TLFPage
	
	constructor {} {};
	
	### data member ###
	protected variable _lsSelectTool;	# 选择的工具
	protected variable _CurrentToolIndex;# 当前选择项
	protected variable _objInputToolName;
	protected variable _objInputToolDesc;
	protected variable _objInputToolLink;
	protected variable _objInputToolVersion;
	protected variable _objInputToolProvider;
	protected variable _objSelectToolLanguage;
	protected variable _winModify;
	
	### public methods ###
	public method  ProcessFormValue {name value};	# 处理Form结果
	public method  OnCreate {};
	public method  OnEndAction {};					# 动作执行完后的操作
	public method  OnAddTool {};
	public method  OnRemoveTool {};
	public method  OnModifyTool {};
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
body TLFAppPage::constructor {} {
	chain;
	#SetEnableSerial true;
	SetFormAction;
	set _lsSelectTool {};
	set _CurrentToolIndex -1;
}

#-------------------------------------------------------------
# 处理Form结果
#-------------------------------------------------------------
body TLFAppPage::ProcessFormValue {name value} {
	if {[chain $name $value] == true} {
		return true
	}
	switch -glob $name {
		"#*"	{lappend _lsSelectTool [string range $name 1 end]}
		"index"	{set _CurrentToolIndex $value}
	}
}

#-------------------------------------------------------------
#	创建页面
#-------------------------------------------------------------
body TLFAppPage::OnCreate {} {
	
	# 添加工具窗口
	LangStringEC STR_WINTITLE_ADDTOOL "Add Tool" "添加工具"
	set win [CreateObject TLFWindow $STR_WINTITLE_ADDTOOL]
	$win SetFormAction;
	$win SetWindowPos 100 80
	#$win SetSize 300 200
	
	set objT [$win CreateObject TLFTable]
	$objT SetHaveTitle false
	$objT AddColumn ""
	$objT AddColumn ""
	LangStringEC STR_TOOL_NAME "Name" "名称"
	$objT AddLine2 [list $STR_TOOL_NAME] [list [list TLFInput "" "" 220] object]
	set _objInputToolName [$objT GetItemObject 0 1]
	LangStringEC STR_TOOL_DESC "Description" "描述"
	$objT AddLine2 [list $STR_TOOL_DESC] [list [list TLFInput "" "" 220] object]
	set _objInputToolDesc [$objT GetItemObject 1 1]
	LangStringEC STR_TOOL_LINK "Action Link" "动作链接"
	$objT AddLine2 [list $STR_TOOL_LINK] [list [list TLFInput "" "" 220] object]
	set _objInputToolLink [$objT GetItemObject 2 1]
	LangStringEC STR_TOOL_VERSION "Version" "版本"
	$objT AddLine2 [list $STR_TOOL_VERSION] [list [list TLFInput "" "" 220] object]
	set _objInputToolVersion [$objT GetItemObject 3 1]
	LangStringEC STR_TOOL_PROVIDER "Provider" "开发商"
	$objT AddLine2 [list $STR_TOOL_PROVIDER] [list [list TLFInput "" "" 220] object]
	set _objInputToolProvider [$objT GetItemObject 4 1]
	LangStringEC STR_TOOL_LANGUAGE "Language" "语言"
	$objT AddLine2 [list $STR_TOOL_LANGUAGE] [list [list TLFSelect 220] object]
	set _objSelectToolLanguage [$objT GetItemObject 5 1]
	LangStringEC STR_TOOL_LANGUAGE_ENGLISH "English" "英语"
	LangStringEC STR_TOOL_LANGUAGE_CHINESE "Chinese" "中文"
	LangStringEC STR_TOOL_LANGUAGE_ALL "All Language" "所有语言"
	$_objSelectToolLanguage AddOption $STR_TOOL_LANGUAGE_ENGLISH "english"
	$_objSelectToolLanguage AddOption $STR_TOOL_LANGUAGE_CHINESE "chinese"
	$_objSelectToolLanguage AddOption $STR_TOOL_LANGUAGE_ALL "all"
	#$_objSelectToolLanguage SetStyleType ss

	# 提交
	LangStringEC STR_INFO_NAME "Submit" "提交"
	$objT AddLine2 [list $STR_INFO_NAME] [list [list TLFButton $STR_INFO_NAME] object]
	set objButtonSend [$objT GetItemObject 6 1]
	$objButtonSend SetStyle "width:58;height:20;padding:1px;border:0;font-size:12px;background-image:url(image/button.gif)"
	$objButtonSend SetAction "OnAddTool"
	$objT SetOneColumnStyle 0 #ffffd0

	# 修改工具窗口
	LangStringEC STR_WINTITLE_MODIFYTOOL "Modify Tool Information" "修改工具信息"
	set _winModify [CreateObject TLFWindow $STR_WINTITLE_MODIFYTOOL]
	$_winModify SetFormAction;
	$_winModify SetWindowPos 100 80
	#$_winModify SetSize 300 200
	
	set objT [$_winModify CreateObject TLFTable]
	$objT SetHaveTitle false
	$objT AddColumn ""
	$objT AddColumn ""
	LangStringEC STR_TOOL_NAME "Name" "名称"
	$objT AddLine2 [list $STR_TOOL_NAME] [list [list TLFInput "" "" 220] object]
	set _objInputToolName [$objT GetItemObject 0 1]
	LangStringEC STR_TOOL_DESC "Description" "描述"
	$objT AddLine2 [list $STR_TOOL_DESC] [list [list TLFInput "" "" 220] object]
	set _objInputToolDesc [$objT GetItemObject 1 1]
	LangStringEC STR_TOOL_LINK "Action Link" "动作链接"
	$objT AddLine2 [list $STR_TOOL_LINK] [list [list TLFInput "" "" 220] object]
	set _objInputToolLink [$objT GetItemObject 2 1]
	LangStringEC STR_TOOL_VERSION "Version" "版本"
	$objT AddLine2 [list $STR_TOOL_VERSION] [list [list TLFInput "" "" 220] object]
	set _objInputToolVersion [$objT GetItemObject 3 1]
	LangStringEC STR_TOOL_PROVIDER "Provider" "开发商"
	$objT AddLine2 [list $STR_TOOL_PROVIDER] [list [list TLFInput "" "" 220] object]
	set _objInputToolProvider [$objT GetItemObject 4 1]
	LangStringEC STR_TOOL_LANGUAGE "Language" "语言"
	$objT AddLine2 [list $STR_TOOL_LANGUAGE] [list [list TLFSelect 220] object]
	set _objSelectToolLanguage [$objT GetItemObject 5 1]
	LangStringEC STR_TOOL_LANGUAGE_ENGLISH "English" "英语"
	LangStringEC STR_TOOL_LANGUAGE_CHINESE "Chinese" "中文"
	LangStringEC STR_TOOL_LANGUAGE_ALL "All Language" "所有语言"
	$_objSelectToolLanguage AddOption $STR_TOOL_LANGUAGE_ENGLISH "english"
	$_objSelectToolLanguage AddOption $STR_TOOL_LANGUAGE_CHINESE "chinese"
	$_objSelectToolLanguage AddOption $STR_TOOL_LANGUAGE_ALL "all"

	# 提交
	LangStringEC STR_INFO_NAME "Submit" "提交"
	$objT AddLine2 [list $STR_INFO_NAME] [list [list TLFButton $STR_INFO_NAME] object]
	set objButtonSend [$objT GetItemObject 6 1]
	$objButtonSend SetStyle "width:58;height:20;padding:1px;border:0;font-size:12px;background-image:url(image/button.gif)"
	$objButtonSend SetAction "OnModifyTool"
	$objT SetOneColumnStyle 0 #ffffd0

	# 添加和删除按钮	
	set _objTextButton [AppendText ""]
	LangStringEC STR_BUTTON_ADDTOOL "Add Tool" "添加工具"
	LangStringEC STR_BUTTON_REMOVETOOL "Remove Tool" "删除工具"
	set objAddTool [$_objTextButton CreateObject TLFButton $STR_BUTTON_ADDTOOL]
	$objAddTool SetAction "OnAddTool"
	$objAddTool SetSubmitAction "Show()" "$win"
	set objRemoveTool [$_objTextButton CreateObject TLFButton $STR_BUTTON_REMOVETOOL]
	$objRemoveTool SetAction "OnRemoveTool"
}

#-------------------------------------------------------------
#	动作执行完后的操作
#-------------------------------------------------------------
body TLFAppPage::OnEndAction {} {
	global platform_path platform_language

	# 创建扩展工具管理界面
	set mkfile "$platform_path/tools/tools.mk"
	
	# 工具列表
	set tableTools [CreateObject TLFTable]
	$tableTools AddColumn "" "" "" "" 16
	LangStringEC STR_COL1 "Name" "工具名"
	$tableTools AddColumn $STR_COL1
	LangStringEC STR_COL2 "Description" "工具描述"
	$tableTools AddColumn $STR_COL2
	LangStringEC STR_COL3 "Version" "版本"
	$tableTools AddColumn $STR_COL3
	LangStringEC STR_COL4 "Provider" "开发商"
	$tableTools AddColumn $STR_COL4
	LangStringEC STR_COL5 "Modify" "修改"
	#$tableTools AddColumn $STR_COL5

	catch {
		mk::file open db $mkfile -readonly
		set vw [mk::view layout db.tools]
		
		set index 0
		set showindex 0
		mk::loop c db.tools {
			set lsRecord [mk::get $c name link desc version provider language]
			#puts $lsRecord
			set itemLanguage [lindex $lsRecord 5]
			if {($itemLanguage == $platform_language) || ($itemLanguage == "all")} {
				set itemCheckBox "<input type='checkbox' name='#$index' value='ON'>"
				set itemDesc [lindex $lsRecord 2]
				set itemVersion [lindex $lsRecord 3]
				set itemProvider [lindex $lsRecord 4]
				$tableTools AddLine2 [list $itemCheckBox] \
								[list [lindex $lsRecord 0] "" [lindex $lsRecord 1]] \
								[list $itemDesc] \
								[list $itemVersion] \
								[list $itemProvider]
								#[list [list TLFButton $STR_COL5] object]
								#[list [list TLFLink "action=OnModifyTool&index=$index" $STR_COL5] object]
				
				set objModifyButton [$tableTools GetItemObject $showindex 5]
				$objModifyButton SetAction "OnModifyTool&index=$index"
				$objModifyButton SetSubmitAction "Show($index)" "$_winModify"
				
				incr showindex
			}
			incr index
		}
		
		mk::file close db
	}
}

#-------------------------------------------------------------
#	添加工具
#-------------------------------------------------------------
body TLFAppPage::OnAddTool {} {
	global platform_path

	set szToolName [$_objInputToolName GetInputText]
	set szToolDesc [$_objInputToolDesc GetInputText]
	set szToolLink [$_objInputToolLink GetInputText]
	regsub {PLAT:\\\\} $szToolLink {PLAT://} szToolLink
	set szToolVersion [$_objInputToolVersion GetInputText]
	set szToolProvider [$_objInputToolProvider GetInputText]
	set szToolLanguage [$_objSelectToolLanguage GetSelectValue]
	
	if {($szToolName == "") || ($szToolLink == "")} {
		return
	} 
	
	set mkfile "$platform_path/tools/tools.mk"
	
	catch {
		mk::file open db $mkfile
		mk::view layout db.tools {name link desc version provider language}
		mk::row append db.tools name $szToolName link $szToolLink desc $szToolDesc version $szToolVersion provider $szToolProvider language $szToolLanguage
		mk::file close db
	}
}

#-------------------------------------------------------------
#	删除工具
#-------------------------------------------------------------
body TLFAppPage::OnRemoveTool {} {
	global platform_path

	set mkfile "$platform_path/tools/tools.mk"
	# 选择项降序排序,从最后开始删除
	set _lsSelectTool [lsort -integer -decreasing $_lsSelectTool]
	
	catch {
		mk::file open db $mkfile
		foreach index $_lsSelectTool {
			mk::row delete db.tools!$index
		}
		mk::file close db
	}
}

#-------------------------------------------------------------
#	修改工具
#-------------------------------------------------------------
body TLFAppPage::OnModifyTool {} {
	global platform_path

	puts $_CurrentToolIndex
}

#-------------------------------------------------------------
#	main
#-------------------------------------------------------------
LangString STR_PAGE_TITLE chinese "扩展工具"
LangString STR_PAGE_TITLE english "Extend Tools"
set page [TLFCreatePage $STR_PAGE_TITLE TLFAppPage]

delete object $page;
