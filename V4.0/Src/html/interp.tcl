#################################################################
# Script.NET Interp组件管理页面脚本
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
	protected variable _tableManage;
	protected variable _objInterpSelect;
	protected variable _objInputScript;
	protected variable _objScriptResult;
	protected variable _objParam1Type;
	protected variable _objParam2Type;
	protected variable _objResultType;
	protected variable _objInputInterpName;
	protected variable _vciId;		# 组件ID
	protected variable _CloseObjHandle;
	
	### public methods ###
	public method  ProcessFormValue {name value};	# 处理Form结果
	public method  OnCreate {};						# 创建页面
	public method  OnInitialUpdate {};				# 页面初始化
	public method  OnEndAction {};					# 动作执行完后的操作
	public method  OnExecScriptCommand {};			# 执行脚本
	public method  CreateInstance {};				# 创建解释器实例
	public method  CloseInstance {};				# 关闭解释器实例
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
body TLFAppPage::constructor {} {
	chain;
	SetFormAction;
	set _vciId "";
	set _CloseObjHandle "";
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
		closehandle {set _CloseObjHandle $value}
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
	LangStringEC STR_PAGE3 "Manage" "管理"
	set objPage1 [$objTabPane CreateObject TLFTabPage $STR_PAGE1]
	set objPage2 [$objTabPane CreateObject TLFTabPage $STR_PAGE2]
	set objPage3 [$objTabPane CreateObject TLFTabPage $STR_PAGE3]
	
	# Page1 - Interp Information
	LangString STR_INFO chinese "解释器信息："
	LangString STR_INFO english "Interp Information:"
	set objTextInfo [$objPage1 AppendText "<b>${STR_INFO}</b>"]
	
	set _tableInfo [$objPage1 CreateObject TLFTable]
	$_tableInfo SetHaveTitle false
	$_tableInfo AddColumn ""
	$_tableInfo AddColumn ""
	
	# Page2 - 执行脚本命令
	LangString STR_INFO chinese "执行脚本命令："
	LangString STR_INFO english "Execute script command:"
	set objTextInfo [$objPage2 AppendText "<b>${STR_INFO}</b>"]
	
	set _tableCmd [$objPage2 CreateObject TLFTable]
	$_tableCmd SetHaveTitle false
	$_tableCmd AddColumn ""
	$_tableCmd AddColumn ""

	# 选择解释器
	LangString STR_INFO_NAME chinese "解释器"
	LangString STR_INFO_NAME english "Interp"
	$_tableCmd AddLine2 [list $STR_INFO_NAME] [list TLFSelect object]
	set _objInterpSelect [$_tableCmd GetItemObject 0 1]

	# 输入命令
	LangString STR_INFO_NAME chinese "执行脚本"
	LangString STR_INFO_NAME english "Execute Script"
	$_tableCmd AddLine2 [list $STR_INFO_NAME] [list [list TLFInput "" "" 70] object]
	set _objInputScript [$_tableCmd GetItemObject 1 1]
	$_objInputScript SetRowCount 10

	# 执行结果
	LangString STR_INFO_NAME chinese "执行结果"
	LangString STR_INFO_NAME english "Execute Result"
	$_tableCmd AddLine2 [list $STR_INFO_NAME] [list [list TLFInput "" "" 70] object]
	set _objScriptResult [$_tableCmd GetItemObject 2 1]
	$_objScriptResult SetRowCount 10

	# 提交
	LangString STR_INFO_NAME chinese "执行"
	LangString STR_INFO_NAME english "Execute"
	$_tableCmd AddLine2 [list $STR_INFO_NAME] [list [list TLFButton $STR_INFO_NAME] object]
	set objButtonSend [$_tableCmd GetItemObject 3 1]
	$objButtonSend SetStyle "width:58;height:20;padding:1px;border:0;font-size:12px;background-image:url(image/button.gif)"
	$objButtonSend SetAction "OnExecScriptCommand"
	
	# Page3 - 管理解释器实例
	LangString STR_INFO chinese "创建删除解释器实例："
	LangString STR_INFO english "Create and delete interpreter instance:"
	set objTextInfo [$objPage3 AppendText "<b>${STR_INFO}</b>"]
	
	set _tableCreateInterp [$objPage3 CreateObject TLFTable]
	$_tableCreateInterp SetHaveTitle false
	$_tableCreateInterp AddColumn ""
	$_tableCreateInterp AddColumn ""
	LangStringEC STR_INTERP_NAME "Instance name" "实例名"
	LangStringEC STR_CREATE_BUTTON "Create" "创建"
	$_tableCreateInterp AddLine2 [list $STR_INTERP_NAME] [list [list TLFInput "" "" 200] object] [list [list TLFButton $STR_CREATE_BUTTON] object]
	set _objInputInterpName [$_tableCreateInterp GetItemObject 0 1]
	set objButtonCreate [$_tableCreateInterp GetItemObject 0 2]
	$objButtonCreate SetStyle "width:58;height:20;padding:1px;border:0;font-size:12px;background-image:url(image/button.gif)"
	$objButtonCreate SetAction "CreateInstance"

	set _tableManage [$objPage3 CreateObject TLFTable]
	#$_tableManage SetHaveTitle false
	LangStringEC STR_COL1 "Instance name" "实例名"
	$_tableManage AddColumn $STR_COL1
	LangStringEC STR_COL2 "Instance handle" "实例句柄"
	$_tableManage AddColumn $STR_COL2
	LangStringEC STR_COL3 "Delete instance" "删除实例"
	$_tableManage AddColumn $STR_COL3
}

#-------------------------------------------------------------
#	页面初始化
#-------------------------------------------------------------
body TLFAppPage::OnInitialUpdate {} {
	global platform_path
	
	set lsVciInfo [vci_info -listinstalled $_vciId];
	
	# 组件注册信息
	LangString STR_INFO chinese "解释器 $_vciId： \[<a href=\"tcl:<file>PLAT:///html/interpmanager.tcl\">解释器列表</a>\]"
	LangString STR_INFO english "Interp $_vciId: \[<a href=\"tcl:<file>PLAT:///html/interpmanager.tcl\">Interp List</a>\]"
	$_objTextInfo SetText "<b>${STR_INFO}</b>"

	catch {
		# 组件名
		LangString STR_INFO_NAME chinese "解释器名"
		LangString STR_INFO_NAME english "Interp Name"
		$_tableInfo AddLine2 [list $STR_INFO_NAME] [list $_vciId]
		# 版本
		LangString STR_INFO_NAME chinese "解释器版本"
		LangString STR_INFO_NAME english "Interp Version"
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
			set interpName [vci_exec plat GetVciName $_vciId]
			set interpPath [vci_exec plat GetVciPath $_vciId]
			LangStringEC PLUGIN_FILE "plugin.xml" "plugin_cn.xml"
			#set pluginfile "$platform_path/interps/$interpName/$PLUGIN_FILE"
			set pluginfile "$interpPath/$PLUGIN_FILE"
			if {[file exist $pluginfile] == 0} {
				#set pluginfile "$platform_path/interps/$interpName/plugin.xml"
				set pluginfile "$interpPath/plugin.xml"
			}
			if {[file exist $pluginfile] == 1} {
				set xml [xml_open file $pluginfile]
				set nodeProp [xml_get $xml node "property\\interp-type"]
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
			set interpName [vci_exec plat GetVciName $_vciId]
			set interpPath [vci_exec plat GetVciPath $_vciId]
			LangStringEC PLUGIN_FILE "plugin.xml" "plugin_cn.xml"
			#set pluginfile "$platform_path/interps/$interpName/$PLUGIN_FILE"
			set pluginfile "$interpPath/$PLUGIN_FILE"
			if {[file exist $pluginfile] == 0} {
				#set pluginfile "$platform_path/interps/$interpName/plugin.xml"
				set pluginfile "$interpPath/plugin.xml"
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
	
	# Table Command
	catch {
		# 获取解释器实例列表
		set lsInstance [list]
		set instanceNumber [vci_exec plat GetVciObjects $_vciId lsInstance]
		foreach instance $lsInstance {
			set instanceName [vci_exec plat GetInstanceNameByObject $_vciId $instance]
			$_objInterpSelect AddOption "${instance}($instanceName)"
		}
		
		$_tableCmd SetOneColumnStyle 0 #ffffd0
	}

	CreateObject TLFVariable "vci" $_vciId
}

#-------------------------------------------------------------
#	动作执行完后的操作
#-------------------------------------------------------------
body TLFAppPage::OnEndAction {} {
	# Table Manage
	catch {
		# 获取解释器实例列表
		set lsVciObject {}
		catch { set lsVciObject [vci_info -listobject $_vciId] }
		set lsInstance [list]
		set instanceNumber [vci_exec plat GetVciObjects $_vciId lsInstance]
		foreach instance $lsInstance {
			set instanceName [vci_exec plat GetInstanceNameByObject $_vciId $instance]
			set interpTclVciObj [vci_info -gettclvciobj $_vciId $instance]
			if {[lsearch $lsVciObject $interpTclVciObj] != -1} {
				$_tableManage AddLine2 [list $instanceName] [list $instance] \
				[list "close" "" "tcl:<file>PLAT:///html/interp.tcl?action=CloseInstance&closehandle=$instance&vci=$_vciId"]
			} else {
				$_tableManage AddLine2 [list $instanceName] [list $instance] ""
			}
		}
		
		$_tableManage SetOneColumnStyle 0 #ffffd0
	}
}

#-------------------------------------------------------------
#	执行解释器命令
#-------------------------------------------------------------
body TLFAppPage::OnExecScriptCommand {} {
	set interpInstance [$_objInterpSelect GetSelectText]
	regsub {\(.*\)} $interpInstance {} interpInstance
	#puts $interpInstance
	set inputScript [$_objInputScript GetInputText]
	#set scriptResult [$_objScriptResult GetInputText]
	
	set interpTclVciObj [vci_info -gettclvciobj $_vciId $interpInstance]
	vci_exec $interpTclVciObj RunScriptCommand $inputScript
	set scriptResult [vci_exec $interpTclVciObj GetResult]
	$_objScriptResult SetText $scriptResult

	puts "###############################################";
	puts "Execute command:";
	puts "$inputScript";
	puts "Result:";
	puts "$scriptResult";
	puts "###############################################";
}

#-------------------------------------------------------------
#	创建组件实例
#-------------------------------------------------------------
body TLFAppPage::CreateInstance {} {
	set objName [$_objInputInterpName GetInputText]
	vci_open $_vciId $objName
}

#-------------------------------------------------------------
#	关闭组件实例
#-------------------------------------------------------------
body TLFAppPage::CloseInstance {} {
	set interpTclVciObj [vci_info -gettclvciobj $_vciId $_CloseObjHandle]
	vci_close $interpTclVciObj
}

#-------------------------------------------------------------
#	main
#-------------------------------------------------------------
LangString STR_PAGE_TITLE chinese "解释器信息"
LangString STR_PAGE_TITLE english "Interp Information"
set page [TLFCreatePage $STR_PAGE_TITLE TLFAppPage]
delete object $page;
