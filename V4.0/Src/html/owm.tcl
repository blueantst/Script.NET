#################################################################
# Script.NET OWM组件管理页面脚本
# 作者:blueant
# 版本:2.0
# 日期:2006-06-25
#################################################################

package require TclFace

#-------------------------------------------------------------
# TLFAppPage class define
#-------------------------------------------------------------
class  TLFAppPage {
	inherit TLFPage
	
	constructor {} {};
	
	### data member ###
	protected variable _objTextInfo;
	protected variable _tableInfo;
	protected variable _tableCmd;
	protected variable _objInputCmdCode;
	protected variable _objInputParam1;
	protected variable _objInputParam2;
	protected variable _objParam1Type;
	protected variable _objParam2Type;
	protected variable _objResultType;
	protected variable _vciId;		# 组件ID
	
	### public methods ###
	public method  ProcessFormValue {name value};	# 处理Form结果
	public method  OnCreate {};						# 创建页面
	public method  OnInitialUpdate {};				# 页面初始化
	public method  OnEndAction {};					# 动作执行完后的操作
	public method  OnOwmSend {};					# 发送OWM命令
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
body TLFAppPage::constructor {} {
	chain;
	SetFormAction;
	set _vciId "";
}

#-------------------------------------------------------------
# 处理Form结果
#-------------------------------------------------------------
body TLFAppPage::ProcessFormValue {name value} {
	if {[chain $name $value] == true} {
		return true
	}
	switch $name {
		vci		{set _vciId $value}
	}
}

#-------------------------------------------------------------
#	创建页面
#-------------------------------------------------------------
body TLFAppPage::OnCreate {} {
	global platform_path
	
	# 组件注册信息
	set _objTextInfo [AppendText ""]
	
	# TabPane页面初始化
	set objTabPane [CreateObject TLFTabPane]
	$objTabPane AddTrTd
	LangStringEC STR_PAGE1 "Information" "信息"
	LangStringEC STR_PAGE2 "Command" "命令"
	set objPage1 [$objTabPane CreateObject TLFTabPage $STR_PAGE1]
	set objPage2 [$objTabPane CreateObject TLFTabPage $STR_PAGE2]
	
	# Page1 - OWM Information
	LangString STR_INFO chinese "界面插件信息："
	LangString STR_INFO english "UI Plugin Information:"
	set objTextInfo [$objPage1 AppendText "<b>${STR_INFO}</b>"]
	
	set _tableInfo [$objPage1 CreateObject TLFTable]
	$_tableInfo SetHaveTitle false
	$_tableInfo AddColumn ""
	$_tableInfo AddColumn ""
	
	# Page2 - 发送OWM消息
	LangString STR_INFO chinese "给此界面插件发送命令："
	LangString STR_INFO english "Send command to this UI plugin:"
	set objTextInfo [$objPage2 AppendText "<b>${STR_INFO}</b>"]
	
	set _tableCmd [$objPage2 CreateObject TLFTable]
	$_tableCmd SetHaveTitle false
	$_tableCmd AddColumn ""
	$_tableCmd AddColumn ""

	# 命令码
	LangString STR_INFO_NAME chinese "命令码"
	LangString STR_INFO_NAME english "Command Code"
	$_tableCmd AddLine2 [list $STR_INFO_NAME] [list [list TLFInput "" "" 120] object]
	set _objInputCmdCode [$_tableCmd GetItemObject 0 1]

	# 参数1
	LangString STR_INFO_NAME chinese "参数1"
	LangString STR_INFO_NAME english "Param1"
	$_tableCmd AddLine2 [list $STR_INFO_NAME] [list [list TLFInput "" "" 240] object]
	set _objInputParam1 [$_tableCmd GetItemObject 1 1]
	set _objParam1Type [$_tableCmd AppendItemObject 1 1 TLFSelect]
	$_objParam1Type AddOption "int"
	$_objParam1Type AddOption "string"
	$_objParam1Type AddOption "int array"
	$_objParam1Type AddOption "int reference"
	#$_objParam1Type SetStyleType ss

	# 参数2
	LangString STR_INFO_NAME chinese "参数2"
	LangString STR_INFO_NAME english "Param2"
	$_tableCmd AddLine2 [list $STR_INFO_NAME] [list [list TLFInput "" "" 240] object]
	set _objInputParam2 [$_tableCmd GetItemObject 2 1]
	set _objParam2Type [$_tableCmd AppendItemObject 2 1 TLFSelect]
	$_objParam2Type AddOption "int"
	$_objParam2Type AddOption "string"
	$_objParam2Type AddOption "int array"
	$_objParam2Type AddOption "int reference"
	#$_objParam2Type SetStyleType ss

	# 返回值类型
	LangString STR_INFO_NAME chinese "返回值类型"
	LangString STR_INFO_NAME english "Result Type"
	$_tableCmd AddLine2 [list $STR_INFO_NAME] [list TLFSelect object]
	set _objResultType [$_tableCmd GetItemObject 3 1]
	$_objResultType AddOption "int"
	$_objResultType AddOption "string"

	# 提交
	LangString STR_INFO_NAME chinese "提交"
	LangString STR_INFO_NAME english "Submit"
	$_tableCmd AddLine2 [list $STR_INFO_NAME] [list [list TLFButton $STR_INFO_NAME] object]
	set objButtonSend [$_tableCmd GetItemObject 4 1]
	$objButtonSend SetStyle "width:58;height:20;padding:1px;border:0;font-size:12px;background-image:url(image/button.gif)"
	$objButtonSend SetAction "OnOwmSend"

}

#-------------------------------------------------------------
#	页面初始化
#-------------------------------------------------------------
body TLFAppPage::OnInitialUpdate {} {
	global platform_path
	
	set lsVciInfo [vci_info -listinstalled $_vciId];
	
	# 组件注册信息
	LangString STR_INFO chinese "界面插件 $_vciId： \[<a href=\"tcl:<file>PLAT:///html/owmmanager.tcl\">插件列表</a>\]"
	LangString STR_INFO english "UI Plugin $_vciId: \[<a href=\"tcl:<file>PLAT:///html/owmmanager.tcl\">Plugin List</a>\]"
	$_objTextInfo SetText "<b>${STR_INFO}</b>"

	catch {
		# 组件名
		LangString STR_INFO_NAME chinese "插件名"
		LangString STR_INFO_NAME english "Plugin Name"
		$_tableInfo AddLine2 [list $STR_INFO_NAME] [list $_vciId]
		# 版本
		LangString STR_INFO_NAME chinese "插件版本"
		LangString STR_INFO_NAME english "Plugin Version"
		$_tableInfo AddLine2 [list $STR_INFO_NAME] [list [lindex $lsVciInfo 0]]
		# 作者
		LangString STR_INFO_NAME chinese "作者"
		LangString STR_INFO_NAME english "Author"
		$_tableInfo AddLine2 [list $STR_INFO_NAME] [list [lindex $lsVciInfo 2]]
		# 创建日期
		#LangString STR_INFO_NAME chinese "创建日期"
		#LangString STR_INFO_NAME english "Create Date"
		#$_tableInfo AddLine2 [list $STR_INFO_NAME] ""
		# 说明
		LangString STR_INFO_NAME chinese "说明"
		LangString STR_INFO_NAME english "Description"
		$_tableInfo AddLine2 [list $STR_INFO_NAME] [list [lindex $lsVciInfo 5] "" [lindex $lsVciInfo 5]]

		# 是否启用
		#LangString STR_INFO_NAME chinese "是否启用"
		#LangString STR_INFO_NAME english "Enable"
		#$_tableInfo AddLine2 [list $STR_INFO_NAME] [list [lindex $lsVciInfo 5] "" ""]

		# 属性
		LangString STR_INFO_NAME chinese "属性"
		LangString STR_INFO_NAME english "Property"
		$_tableInfo AddLine2 [list $STR_INFO_NAME] [list TLFTable object]

		set objTableProp [$_tableInfo GetItemObject 4 1]
		LangStringEC STR_TAB_TITLE "Name" "属性名"
		$objTableProp AddColumn $STR_TAB_TITLE
		LangStringEC STR_TAB_TITLE "Value" "属性值"
		$objTableProp AddColumn $STR_TAB_TITLE
		
		set lsProperty {}
		catch {
			LangStringEC PLUGIN_FILE "plugin.xml" "plugin_cn.xml"
			set pluginfile "$platform_path/plugins/$_vciId/$PLUGIN_FILE"
			if {[file exist $pluginfile] == 0} {
				set pluginfile "$platform_path/plugins/$_vciId/plugin.xml"
			}
			if {[file exist $pluginfile] == 1} {
				set xml [xml_open file $pluginfile]
				set nodeProp [xml_get $xml node "property\\owm-type"]
				set lsAttrName [xml_get $xml nodeAttrNames $nodeProp]
				foreach attrName $lsAttrName {
					set attrValue [xml_get $xml nodeAttr -node $nodeProp $attrName]
					$objTableProp AddLine2 [list $attrName] [list $attrValue]
				}
				xml_close $xml
			}
		}

		# 支持的文档
		LangString STR_INFO_NAME chinese "支持的文档"
		LangString STR_INFO_NAME english "Support Document"
		$_tableInfo AddLine2 [list $STR_INFO_NAME] [list TLFTable object]
		
		set objTableDoc [$_tableInfo GetItemObject 5 1]
		LangStringEC STR_TAB_TITLE "Name" "文档名"
		$objTableDoc AddColumn $STR_TAB_TITLE
		LangStringEC STR_TAB_TITLE "Doc Extend" "文档后缀"
		$objTableDoc AddColumn $STR_TAB_TITLE
		
		set lsDoc {}
		catch {
			LangStringEC PLUGIN_FILE "plugin.xml" "plugin_cn.xml"
			set pluginfile "$platform_path/plugins/$_vciId/$PLUGIN_FILE"
			if {[file exist $pluginfile] == 0} {
				set pluginfile "$platform_path/plugins/$_vciId/plugin.xml"
			}
			if {[file exist $pluginfile] == 1} {
				set xml [xml_open file $pluginfile]
				set nodeDocExt [xml_get $xml node "doc-extend"]
				set lsNodeDoc [xml_get $xml childNodes $nodeDocExt "doc"]
				foreach nodeDoc $lsNodeDoc {
					set attrValue [xml_get $xml nodeAttr -node $nodeDoc name]
					set nodeExts [xml_get $xml childNode $nodeDoc "exts"]
					set lsNodeExt [xml_get $xml childNodes $nodeExts "ext"]
					set lsDocExt {}
					foreach nodeExt $lsNodeExt {
						lappend lsDocExt [xml_get $xml nodeText -node $nodeExt]
					}
					$objTableDoc AddLine2 [list $attrValue] [list [join $lsDocExt]]
				}
				xml_close $xml
			}
		}
		
		$_tableInfo SetOneColumnStyle 0 #ffffd0
	}
	
	$_tableCmd SetOneColumnStyle 0 #ffffd0

	CreateObject TLFVariable "vci" $_vciId
}

#-------------------------------------------------------------
#	动作执行完后的操作
#-------------------------------------------------------------
body TLFAppPage::OnEndAction {} {
}

#-------------------------------------------------------------
#	发送OWM命令
#-------------------------------------------------------------
body TLFAppPage::OnOwmSend {} {
	set cmdcode [$_objInputCmdCode GetInputText]
	set param1 [$_objInputParam1 GetInputText]
	set type1 [$_objParam1Type GetSelectValue]
	set param2 [$_objInputParam2 GetInputText]
	set type2 [$_objParam2Type GetSelectValue]
	set restype [$_objResultType GetSelectValue]

	puts "###############################################";
	puts "Send OWM command to $_vciId, code = $cmdcode";
	puts "Param1 = $param1";
	puts "Param2 = $param2";
	
	set p1 $param1
	switch $type1 {
	1 {set p1 "<string>$param1"}
	2 {set param1 [split $param1 " "];set p1 "<uintarray>param1"}
	3 {set p1 "<&int>param1"}
	}
	
	set p2 $param2
	switch $type2 {
	1 {set p2 "<string>$param2"}
	2 {set param2 [split $param2 " "];set p2 "<uintarray>param2"}
	3 {set p2 "<&int>param2"}
	}
	
	set rt ""
	if {$restype == 1} {
		set rt "<string>"
	}
	
	set ret [owm -sendcmd $_vciId $cmdcode $p1 $p2 $rt];
	puts "Result is:$ret";
	
	if {($type1 == 2) || ($type1 == 3)} {
		puts "param1 = $param1"
	}
	if {($type2 == 2) || ($type2 == 3)} {
		puts "param2 = $param2"
	}
	
	puts "###############################################";
}

#-------------------------------------------------------------
#	main
#-------------------------------------------------------------
LangString STR_PAGE_TITLE chinese "界面插件信息"
LangString STR_PAGE_TITLE english "UI Plugin Information"
set page [TLFCreatePage $STR_PAGE_TITLE TLFAppPage]
delete object $page;
