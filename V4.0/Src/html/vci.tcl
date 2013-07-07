#################################################################
# Script.NET VCI组件管理页面脚本
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
	protected variable _objTextInfoInstance;
	protected variable _tableInstance;
	protected variable _vciId;		# 组件ID
	protected variable _vciVersion;	# 组件版本
	protected variable _CurrentObjHandle;
	protected variable _CloseObjHandle;
	protected variable _CurrentFunction;
	
	### public methods ###
	public method  ProcessFormValue {name value};	# 处理Form结果
	public method  OnInitialUpdate {};				# 页面初始化
	public method  OnEndAction {};					# 动作执行完后的操作
	public method  CreateVciObject {};				# 创建新组件实例
	public method  CloseInstance {};				# 关闭组件实例
	public method  ExecFunction {};					# 执行函数
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
body TLFAppPage::constructor {} {
	chain;
	#SetFormAction;
	set _vciId "";
	set _vciVersion "";
	set _CurrentObjHandle "";
	set _CloseObjHandle "";
	set _CurrentFunction "";
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
		version	{set _vciVersion $value}
		handle	{set _CurrentObjHandle $value}
		closehandle {set _CloseObjHandle $value}
		function {set _CurrentFunction $value}
	}
}

#-------------------------------------------------------------
#	页面初始化
#-------------------------------------------------------------
body TLFAppPage::OnInitialUpdate {} {
	global platform_path
	
	# 如果句柄被删除,则置为空
	if {$_CloseObjHandle == $_CurrentObjHandle} {
		set _CurrentObjHandle ""
	}
	
	# 打开组件,查询接口声明,然后关闭组件(如果有打开的实例,则直接使用)
	set openvci false
	if {[catch {set lsVci [vci_info -listobject $_vciId]}] == 1} {
		set obj [vci_open $_vciId "" $_vciVersion];
		set openvci true
	}
	set lstDef [vci_info -listdef $_vciId];
	set lsFunc [vci_info -listfunc $_vciId];
	set lsVciInfo [vci_info -listinstalled $_vciId $_vciVersion];
	
	# 加载JavaScript cookie,RightEdge类
	AppendJavaScript "Cookie" class
	AppendJavaScript "RightEdge" class
	
	LangString STR_WIN_TITLE chinese "执行函数"
	LangString STR_WIN_TITLE english "Execute Function"
	set winFunction [CreateObject TLFWindow $STR_WIN_TITLE]
	$winFunction SetSize 350 200
	LangString STR_MSG_1 chinese "请输入函数 "
	LangString STR_MSG_1 english "Please input function "
	LangString STR_MSG_2 chinese " 参数："
	LangString STR_MSG_2 english " parameters:"
	LangString STR_MSG_3 chinese " 参数"
	LangString STR_MSG_3 english " Parameters"
	LangString STR_MSG_4 chinese "执行函数"
	LangString STR_MSG_4 english "Execute Function"
	set vciExecFunction "\
		function vciExecFunction(funcIndex, funcName, paramNum) {\
			[$winFunction GetObjId].Content = \"<FORM name='[$winFunction GetFullId]' action='tcl:<form><file>$platform_path/html/vci.tcl' onsubmit='processFormSubmit(this)' method=POST>\" + \
				\"<input type=hidden name='vci' value='$_vciId'>\" + \
				\"<input type=hidden name='version' value='$_vciVersion'>\" + \
				\"<input type=hidden name='handle' value='$_CurrentObjHandle'>\" + \
				\"<input type=hidden name='function' value='\" + funcName + \"'>\" + \
				\"<input type=hidden name='action' value='ExecFunction'>\" + \
				\"<font color=#800000><b>$STR_MSG_1\" + \
				funcName + \"$STR_MSG_2<b></font></br>\"; \
			[$winFunction GetObjId].Content += \"<table width='100%' cellSpacing=1 cellPadding=3 bgColor=#7195c4 border=0>\"; \
			for(var i=0; i<paramNum; i++) \
			{ \
				[$winFunction GetObjId].Content += \"<tr><td class=text_all bgColor=#f0f2f6>$STR_MSG_3\" + i + \"</td><td class=text_all bgColor=#f0f2f6>\"; \
				[$winFunction GetObjId].Content += \"<input type=text size=40 name=param\"+i+\"></td></tr>\"; \
			} \
			[$winFunction GetObjId].Content += \"</table>\"; \
			[$winFunction GetObjId].Content += \"<input type=submit name='button_submit' value='$STR_MSG_4'>\"; \
			[$winFunction GetObjId].Content += \"</FORM>\"; \
			[$winFunction Show] \
			new DronFw.Class.RightEdge(\"[$winFunction GetObjId]\"); \
		}\
		"
	if {$_CurrentObjHandle == ""} {
		set vciExecFunction "\
			function vciExecFunction(funcIndex, funcName, paramNum) {\
			}\
			"
	}
	AppendJavaScript "$vciExecFunction" body
	
	LangString STR_INFO chinese "组件接口 $_vciId\($_vciVersion\) 声明： \[<a href=\"tcl:<file>PLAT:///html/vcimanager.tcl\">组件列表</a>\]"
	LangString STR_INFO english "Component $_vciId\($_vciVersion\) Decalare: \[<a href=\"tcl:<file>PLAT:///html/vcimanager.tcl\">Component List</a>\]"
	set objTextInfo [AppendText "<b>${STR_INFO}</b>"]
	
	# 创建函数列表内容
	set tableFunction [CreateObject TLFTable]
	$tableFunction SetLineColor #000040
	LangStringEC STR_COL1 "Serial No" "序号"
	$tableFunction AddColumn $STR_COL1
	LangStringEC STR_COL2 "Function Declare" "函数声明"
	$tableFunction AddColumn $STR_COL2
	if { [catch {
		#set lsFunc [vci_info -listfunc $_vciId]
		set index 1
		foreach func $lstDef {
			# 第一个函数为保留函数,不显示
			if {$index != 1} {
				#$tableFunction AddLine2 [expr $index-1] [list $func "" "tcl:<file>PLAT:///html/vci.tcl?vci=$_vciId&version=$_vciVersion"]
				# 参数名列表
				set funcname [lindex $lsFunc [expr $index-1]]
				set lsParam [vci_info -listparam $_vciId $funcname]
				$tableFunction AddLine2 [expr $index-1]\
					[list $func "" "javascript:vciExecFunction([expr $index-1], \"$funcname\", [llength $lsParam]);"]
			}
			incr index
		}
	} errinfo ] } {
		puts $errinfo
	}
	# 打开的仪器句柄列表
	set _objTextInfoInstance [AppendText ""]
	
	set _tableInstance [CreateObject TLFTable]
	$_tableInstance SetLineColor #000040
	LangStringEC STR_COL1 "Serial No" "序号"
	$_tableInstance AddColumn $STR_COL1
	LangStringEC STR_COL2 "Instance Handle" "实例句柄"
	$_tableInstance AddColumn $STR_COL2
	LangStringEC STR_COL3 "Instance Name" "实例名"
	$_tableInstance AddColumn $STR_COL3
	LangStringEC STR_COL4 "Operate" "操作"
	$_tableInstance AddColumn $STR_COL4
	
	# 组件注册信息
	LangString STR_INFO chinese "组件注册信息："
	LangString STR_INFO english "Component Information:"
	set objTextInfo [AppendText "<b>${STR_INFO}</b>"]
	
	set tableInfo [CreateObject TLFTable]
	$tableInfo SetHaveTitle false
	$tableInfo AddColumn ""
	$tableInfo AddColumn ""
	
	catch {
		# 组件名
		LangString STR_INFO_NAME chinese "组件名"
		LangString STR_INFO_NAME english "Component Name"
		$tableInfo AddLine2 [list $STR_INFO_NAME] [list $_vciId]
		# 版本
		LangString STR_INFO_NAME chinese "组件版本"
		LangString STR_INFO_NAME english "Component Version"
		$tableInfo AddLine2 [list $STR_INFO_NAME] [list [lindex $lsVciInfo 0]]
		# 作者
		LangString STR_INFO_NAME chinese "作者"
		LangString STR_INFO_NAME english "Author"
		$tableInfo AddLine2 [list $STR_INFO_NAME] [list [lindex $lsVciInfo 2]]
		# 创建日期
		#LangString STR_INFO_NAME chinese "创建日期"
		#LangString STR_INFO_NAME english "Create Date"
		#$tableInfo AddLine2 [list $STR_INFO_NAME] ""
		# 说明
		LangString STR_INFO_NAME chinese "说明"
		LangString STR_INFO_NAME english "Description"
		$tableInfo AddLine2 [list $STR_INFO_NAME] [list [lindex $lsVciInfo 5] "" [lindex $lsVciInfo 5]]
		# 头文件
		LangString STR_INFO_NAME chinese "头文件"
		LangString STR_INFO_NAME english "Header File"
		set headlink [lindex $lsVciInfo 4]
		regsub -all "/" $headlink "\\" headlink;
		$tableInfo AddLine2 [list $STR_INFO_NAME] [list [lindex $lsVciInfo 4] "" "pfe:open $headlink"]
		# 手册
		#LangString STR_INFO_NAME chinese "手册"
		#LangString STR_INFO_NAME english "Manual File"
		#$tableInfo AddLine2 [list $STR_INFO_NAME] [list [lindex $lsVciInfo 5] "" [lindex $lsVciInfo 5]]
	}
	
	if {$openvci == "true"} {
		vci_close $obj;
	}
}

#-------------------------------------------------------------
#	动作执行完后的操作
#-------------------------------------------------------------
body TLFAppPage::OnEndAction {} {
	# 刷新句柄列表总体信息
	if { [ catch {
		set lsHandle [vci_info -listobject $_vciId];
	} ] } {
		set lsHandle {};
	}
	set HandleCount [llength $lsHandle];
	
	LangString STR_INFO chinese "组件 $_vciId\($_vciVersion\) 已经打开的实例句柄为 $HandleCount。 <a href=\"tcl:<file>PLAT:///html/vci.tcl?action=CreateVciObject&vci=$_vciId&version=$_vciVersion&handle=$_CurrentObjHandle\">\[创建新组件实例\]</a>"
	LangString STR_INFO english "Component $_vciId\($_vciVersion\) have opened $HandleCount instances. <a href=\"tcl:<file>PLAT:///html/vci.tcl?action=CreateVciObject&vci=$_vciId&version=$_vciVersion&handle=$_CurrentObjHandle\">\[Create new component instance\]</a>"
	$_objTextInfoInstance SetText "<b>${STR_INFO}</b>"

	# 刷新组件实例表
	catch {
		set index 1
		foreach handle $lsHandle {
			#if {($_CurrentObjHandle == "") && ($index == 1)} {
				# 如果当前实例为空,则把第一个实例设置为当前实例
			#	set _CurrentObjHandle $handle
			#} 
			
			if {$handle == $_CurrentObjHandle} {
				# 设置为当前操作句柄
				set serial [list "$index<img border=\"0\" src=\"image/point.gif\">"]
			} else {
				set serial [list "$index"]
			}
			
			set vcihandle [vci_info -getvcihandle $handle]
			set instanceName [vci_exec plat GetInstanceNameByObject $_vciId $vcihandle]
			
			$_tableInstance AddLine2 $serial \
				[list $handle "" "tcl:<file>PLAT:///html/vci.tcl?handle=$handle&vci=$_vciId&version=$_vciVersion"] \
				[list $instanceName] \
				[list "close" "" "tcl:<file>PLAT:///html/vci.tcl?action=CloseInstance&closehandle=$handle&vci=$_vciId&version=$_vciVersion&handle=$_CurrentObjHandle"]
			
			incr index
		}
	}
}

#-------------------------------------------------------------
#	创建新组件实例
#-------------------------------------------------------------
body TLFAppPage::CreateVciObject {} {
	vci_open $_vciId "" $_vciVersion
}

#-------------------------------------------------------------
#	关闭组件实例
#-------------------------------------------------------------
body TLFAppPage::CloseInstance {} {
	vci_close $_CloseObjHandle
}

#-------------------------------------------------------------
#	执行一个组件函数
#-------------------------------------------------------------
body TLFAppPage::ExecFunction {} {
	# 获取函数信息
	set lsFunc [vci_info -listfunc $_vciId];
	set lsDef [vci_info -listdef $_vciId];
	set funcindex 0
	# 判断是否有重名的函数
	set funccount 0;	# 重名函数个数
	set index 0;		# 计数器
	set funcnindex 0;	# 此函数在重名函数列表中的位置
	foreach fname $lsFunc {
		if {$fname == $_CurrentFunction} {
			incr funccount;
			if {$index == $funcindex} {
				set funcnindex $funccount;
			}
		}
		incr index;
	}
	set funcnname $_CurrentFunction;
	if {$funccount > 1} {
		set funcnname "$_CurrentFunction\($funcnindex\)";
	}
	# 参数名列表
	set lsParam [vci_info -listparam $_vciId $_CurrentFunction];

	# 如果没有参数,则直接执行
	if {[llength $lsParam] == 0} {
		puts "###############################################";
		puts "Exec $_vciId $_CurrentFunction:";
		set ret [vci_exec $_CurrentObjHandle $funcnname];
		puts "Result is:$ret";
		puts "###############################################";
		return;
	}

	# 输入参数然后执行
	set index 0;
	foreach param $lsParam {
		# 参数详细信息
		set lsParamInfo [vci_info -listparam $_vciId $funcnname $param];
		# 缺省值
		set defaultValue [lindex $lsParamInfo 2];
		set pvalue($param) $defaultValue;
		if {$_actionArgs(param$index) != ""} {
			set pvalue($param) $_actionArgs(param$index)
		}
		incr index;
	}
	puts "###############################################";
	puts "Exec $_vciId $funcnname:";
	
	set execcmd "vci_exec $_CurrentObjHandle $funcnname";
	foreach param $lsParam {
		# 参数详细信息
		set lsParamInfo [vci_info -listparam $_vciId $funcnname $param];
		# 调用方式
		set ParamMode [lindex $lsParamInfo 1];
		if {$ParamMode==1 || $ParamMode==2} {
			# 指针或引用类型需要传递一个变量名
			set execcmd "$execcmd pvalue($param)";
			puts "$param=$pvalue($param); type=$ParamMode";
		} else {
			# 直接调用,只要传递变量值就可以了
			set execcmd "$execcmd $pvalue($param)";
			puts "$param=$pvalue($param)";
		}			
	}

	puts "$execcmd";
	# 执行函数调用
	set ret [eval $execcmd];
	puts "Result is:$ret";

	# 输出指针或引用的返回值		
	foreach param $lsParam {
		# 参数详细信息
		set lsParamInfo [vci_info -listparam $_vciId $funcnname $param];
		# 调用方式
		set ParamMode [lindex $lsParamInfo 1];
		if {$ParamMode==1 || $ParamMode==2} {
			#set pvar($param) $pvalue($param);
			puts "$param=$pvalue($param)";
		}
	}

	puts "###############################################";
}

#-------------------------------------------------------------
#	main
#-------------------------------------------------------------
LangString STR_PAGE_TITLE chinese "组件信息"
LangString STR_PAGE_TITLE english "Component Information"
set page [TLFCreatePage $STR_PAGE_TITLE TLFAppPage]
delete object $page;
