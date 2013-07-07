#########################################################################
# Script.NET TCL调试器支持脚本											#
# 作者:Blueant															#
# 版本:2.2																#
# 日期:2009-06-30														#
#########################################################################
# 注:1.此脚本为Script.NET TCL调试器使用,如果需要改变调试器的表现		#
#    可以修改此脚本,但必须保证所有函数的名字不变						#
#    2.为了便于区分,此脚本内所有自定义变量必须以_plat作为前缀,		#
#    对这种前缀的变量此处显示的时候会过滤掉							#
#########################################################################

#-------------------------------------------------------------
# 重载的source命令
#-------------------------------------------------------------
proc source_debug { args } {
	# 如果参数不是一个就调用缺省的source命令
	if {[llength $args] != 1} {
		source_std $args
	}
	
	# 调用平台扩展的source命令(用于支持多文件调试)
	plat_debug_source [lindex $args 0]
}

#-------------------------------------------------------------
# 查看变量列表脚本
#-------------------------------------------------------------
set _plat_Tcl_WatchVariables {
	
	LangStringEC _plat_win_var "Variable" "变量"
	
	set _plat_nOT [output -GetInfo "$_plat_win_var" window-id];

	# 清除所有项
	output -format -$_plat_nOT "DelItem ALL";

	# 获取当前解释器名
	set _plat_interpName [uplevel #0 {set interpName}];

	# 设置变量双击的脚本(普通节点)
	set _plat_script_item {
		LangStringEC _plat_win_var "Variable" "变量"
		LangStringEC _plat_str_watchvar "Watch Variable" "查看变量"
		LangStringEC _plat_str_varname "Variable Name" "变量名"
		LangStringEC _plat_str_varvalue "Variable Value" "变量值"
		
		set _plat_hItem [output -GetInfo "$_plat_win_var" HotItem];
		set _plat_varname [output -GetInfo "$_plat_win_var" <#$_plat_hItem,0>];
		set _plat_hParentItem [output -GetInfo "$_plat_win_var" "ItemParent $_plat_hItem"];
		if { $_plat_hParentItem == "ROOT" } {
			set _plat_varvalue [set $_plat_varname];
		} else {
			set _plat_parentname [output -GetInfo "$_plat_win_var" <#$_plat_hParentItem,0>];
			set _plat_item1 "";
			set _plat_varvalue "";
			foreach {_plat_item1 _plat_varvalue} [array get $_plat_parentname $_plat_varname] {}
			unset _plat_item1 _plat_parentname;
		}
		MessageBox -MBOK -MB_ICONINFORMATION -title "$_plat_str_watchvar" \
					"$_plat_str_varname:$_plat_varname\n$_plat_str_varvalue:$_plat_varvalue";
		unset _plat_hItem _plat_varname _plat_varvalue;
		unset _plat_win_var _plat_str_watchvar _plat_str_varname _plat_str_varvalue;
	}
	output -$_plat_nOT "<I>Set Script<0> <<$_plat_interpName>>$_plat_script_item";
	
	# 设置变量双击的脚本(数组节点),双击时才创建子节点
	set _plat_script_item {
		LangStringEC _plat_win_var "Variable" "变量"
		
		set _plat_hItem [output -GetInfo "$_plat_win_var" SelItem];
		set _plat_varname [output -GetInfo "$_plat_win_var" <#$_plat_hItem,0>];
		set _plat_item_child_count [output -GetInfo "$_plat_win_var" "ChildCount $_plat_hItem"];
		if {$_plat_item_child_count == 0} {
			# 添加数组的元素
			set _plat_item1 "";
			set _plat_item_var "";
			foreach {_plat_item1 _plat_item_var} [array get $_plat_varname] {
				output "$_plat_win_var" "<$_plat_hItem,7>$_plat_item1,$_plat_item_var";
				set _plat_lasthItem [output -GetInfo "$_plat_win_var" LastItem];
				output "$_plat_win_var" "<I>Set Script<#$_plat_lasthItem> <1>";
				unset _plat_lasthItem;
			}
			output "$_plat_win_var" "<T>ExpandItem<#$_plat_hItem>";
			unset _plat_item1 _plat_item_var;
		}
		unset _plat_hItem _plat_varname _plat_item_child_count _plat_win_var;
	}
	output -$_plat_nOT "<I>Set Script<0> <<$_plat_interpName>>$_plat_script_item";

	unset _plat_interpName _plat_script_item;

	# 添加局部变量
	set _plat_varcount 0;
	set _plat_var "";
	
	foreach _plat_var [info local] {
		if { [info exist $_plat_var] && [string first "_plat_" $_plat_var]==-1 } {
			if { [array exists $_plat_var] } {
				output -$_plat_nOT "<ROOT,4>$_plat_var,{...}" -direct;
				output -$_plat_nOT "<I>Set Item<$_plat_varcount,1> COLOR 5";
				output -$_plat_nOT "<I>Set Script<$_plat_varcount> <2>";
				incr _plat_varcount;
			} else {
				output -$_plat_nOT "<ROOT,5>$_plat_var,[set $_plat_var]" -direct;
				output -$_plat_nOT "<I>Set Script<$_plat_varcount> <1>";
				incr _plat_varcount;
			}
		}
	}

	# 添加全局变量
	foreach _plat_var [info globals] {
		if { [info exist $_plat_var] && [string first "_plat_" $_plat_var]==-1 } {
			if { [array exists $_plat_var] } {
				output -$_plat_nOT "<ROOT,4>$_plat_var,{...}" -direct;
				output -$_plat_nOT "<I>Set Item<$_plat_varcount,1> COLOR 5";
				output -$_plat_nOT "<I>Set Script<$_plat_varcount> <2>";
				incr _plat_varcount;
			} else {
				output -$_plat_nOT "<ROOT,5>$_plat_var,[set $_plat_var]" -direct;
				output -$_plat_nOT "<I>Set Script<$_plat_varcount> <1>";
				incr _plat_varcount;
			}
		}
	}

	# 销毁临时变量
	unset _plat_nOT _plat_var _plat_varcount _plat_win_var;
}

#-------------------------------------------------------------
# 查看对象列表脚本
#-------------------------------------------------------------
set _plat_Tcl_WatchObjects {
	LangStringEC _plat_win_object "Object" "对象"
	
	set _plat_nOT [output -GetInfo "$_plat_win_object" window-id];

	# 清除所有项
	output -format -$_plat_nOT "DelItem ALL";

	# 获取当前解释器名
	set _plat_interpName [uplevel #0 {set interpName}];
		
	# 设置变量双击的脚本(对象节点)
	set _plat_script_item {
		LangStringEC _plat_win_object "Object" "对象"
		
		set _plat_hItem [output -GetInfo "$_plat_win_object" SelItem];
		set _plat_varname [output -GetInfo "$_plat_win_object" <#$_plat_hItem,0>];
		set _plat_item_child_count [output -GetInfo "$_plat_win_object" "ChildCount $_plat_hItem"];
		if { $_plat_item_child_count == 0 } {
			if [catch { [$_plat_varname] }] {
				global errorInfo;
				# 使用正则表达式匹配对象的所有方法并创建子节点
				# 如果不在全局作用域,则_plat_varname前面有::作用域标志,下面的分析会失败
				set _plat_reg "\\n *$_plat_varname (\[^\\n\]*)\\n";
				set _plat_ls [regexp -all -inline $_plat_reg $errorInfo];
				set _plat_temp "";
				set _plat_method "";
				foreach {_plat_temp _plat_method} $_plat_ls {
					set _plat_spacepos [string first " " $_plat_method];
					set _plat_param "";
					if { $_plat_spacepos != -1 } {
						incr _plat_spacepos;
						set _plat_param [string range $_plat_method $_plat_spacepos end];
						incr _plat_spacepos -1;
						set _plat_method [string range $_plat_method 0 $_plat_spacepos];
					}
					output "$_plat_win_object" "<$_plat_hItem,7>$_plat_method,$_plat_param";
					set _plat_lasthItem [output -GetInfo "$_plat_win_object" LastItem];
					output "$_plat_win_object" "<I>Set Script<#$_plat_lasthItem> <3>";
					unset _plat_lasthItem;
				}
				output "$_plat_win_object" "<T>ExpandItem<#$_plat_hItem>";
				unset _plat_reg _plat_ls _plat_temp _plat_method;
			}
		}
		unset _plat_hItem _plat_varname _plat_item_child_count _plat_win_object;
	}
	output -$_plat_nOT "<I>Set Script<0> <<$_plat_interpName>>$_plat_script_item";
	
	# 设置对象双击的脚本(对象节点),双击时展开对象列表
	set _plat_script_item {
		LangStringEC _plat_win_object "Object" "对象"
		
		set _plat_hItem [output -GetInfo "$_plat_win_object" SelItem];
		set _plat_classname [output -GetInfo "$_plat_win_object" <#$_plat_hItem,0>];
		set _plat_item_child_count [output -GetInfo "$_plat_win_object" "ChildCount $_plat_hItem"];
		if {$_plat_item_child_count == 0} {
			# 添加iTcl对象
			foreach _plat_object [itcl::find objects -class $_plat_classname] {
				output "$_plat_win_object" "<$_plat_hItem,28>$_plat_object,{...}";
				set _plat_lasthItem [output -GetInfo "$_plat_win_object" LastItem];
				output "$_plat_win_object" "<I>Set Item<#$_plat_lasthItem,1> COLOR 5";
				output "$_plat_win_object" "<I>Set Script<#$_plat_lasthItem> <1>";
				unset _plat_lasthItem;
			}
		}
		output "$_plat_win_object" "<T>ExpandItem<$_plat_hItem>";
		unset _plat_hItem _plat_classname _plat_object _plat_win_object _plat_item_child_count;
	}
	output -$_plat_nOT "<I>Set Script<0> <<$_plat_interpName>>$_plat_script_item";
	
	# 设置变量双击的脚本(对象方法节点),双击时执行方法
	set _plat_script_item {
		LangStringEC _plat_win_object "Object" "对象"
		
		set _plat_hItem [output -GetInfo "$_plat_win_object" SelItem];
		set _plat_method [output -GetInfo "$_plat_win_object" <#$_plat_hItem,0>];
		set _plat_param [output -GetInfo "$_plat_win_object" <#$_plat_hItem,1>];
		if { [string length $_plat_param] == 0 } {
			set _plat_hParentItem [output -GetInfo "$_plat_win_object" "ItemParent $_plat_hItem"];
			set _plat_parentname [output -GetInfo "$_plat_win_object" <#$_plat_hParentItem,0>];
			eval "$_plat_parentname $_plat_method";
			unset _plat_hParentItem _plat_parentname;
		}		
		unset _plat_hItem _plat_method _plat_param _plat_win_object;
	}
	output -$_plat_nOT "<I>Set Script<0> <<$_plat_interpName>>$_plat_script_item";

	unset _plat_interpName _plat_script_item;

	# 添加iTcl类
	set _plat_varcount 0;
	set _plat_class "";
	foreach _plat_class [itcl::find classes] {
		if { [llength [itcl::find objects -class $_plat_class]] > 0 } {
			output -$_plat_nOT "<ROOT,44>$_plat_class,{class}";
		} else {
			output -$_plat_nOT "<ROOT,30>$_plat_class,{class-no object}";
		}
		set _plat_lasthItem [output -GetInfo -$_plat_nOT LastItem];
		output -$_plat_nOT "<I>Set Item<$_plat_varcount,1> COLOR 5";
		output -$_plat_nOT "<I>Set Script<$_plat_varcount> <2>";
		incr _plat_varcount;
		set _plat_object "";
		# 添加iTcl对象
		# foreach _plat_object [itcl::find objects -class $_plat_class] {
			# output -$_plat_nOT "<$_plat_lasthItem,45>$_plat_object,{...}";
			# output -$_plat_nOT "<I>Set Item<$_plat_varcount,1> COLOR 5";
			# output -$_plat_nOT "<I>Set Script<$_plat_varcount> <1>";
			# incr _plat_varcount;
		# }
		
		unset _plat_lasthItem _plat_index _plat_object;		
	}

	# 销毁临时变量
	unset _plat_nOT _plat_class _plat_varcount _plat_win_object;
}

#-------------------------------------------------------------
# 查看过程列表脚本
#-------------------------------------------------------------
set _plat_Tcl_WatchProcedures {
	LangStringEC _plat_win_proc "Function" "函数"
	
	set _plat_nOT [output -GetInfo "$_plat_win_proc" window-id];

	# 清除所有项
	output -format -$_plat_nOT "DelItem ALL";

	# 获取当前解释器名
	set _plat_interpName [uplevel #0 {set interpName}];

	# 设置过程双击的脚本,双击时创建参数子节点
	set _plat_script_item {
		LangStringEC _plat_win_proc "Function" "函数"
		
		set _plat_hItem [output -GetInfo "$_plat_win_proc" SelItem];
		set _plat_proc [output -GetInfo "$_plat_win_proc" <#$_plat_hItem,0>];
		set _plat_itemcount [output -GetInfo "$_plat_win_proc" "ItemCount $_plat_hItem"];
		if { $_plat_itemcount == 0 } {
			# 添加参数子节点
			set _plat_param "";
			foreach {_plat_param} [info args $_plat_proc] {
				output "$_plat_win_proc" "<$_plat_hItem,7>$_plat_param";
			}
			output "$_plat_win_proc" "<T>ExpandItem<#$_plat_hItem>";
			unset _plat_param;
		}
		unset _plat_proc _plat_hItem _plat_itemcount;
	}
	output -$_plat_nOT "<I>Set Script<0> <<$_plat_interpName>>$_plat_script_item";

	unset _plat_interpName _plat_script_item;

	# 添加过程
	set _plat_proccount 0;
	set _plat_proc "";
	foreach _plat_proc [info procs] {
		output -$_plat_nOT "<ROOT,29>$_plat_proc";
		output -$_plat_nOT "<I>Set Script<$_plat_proccount> <1>";
		incr _plat_proccount;
	}
	
	# 销毁临时变量
	unset _plat_nOT _plat_proc _plat_proccount _plat_win_proc;
}

#-------------------------------------------------------------
# 查看堆栈脚本
#-------------------------------------------------------------
set _plat_Tcl_WatchCallStack {
	LangStringEC _plat_win_stack "Stack" "堆栈"
	LangStringEC _plat_str_globalspace "Global Space" "全局空间"
	
	set _plat_nOT [output -GetInfo "$_plat_win_stack" window-id];

	# 清除所有项
	output -format -$_plat_nOT "DelItem ALL";

	# 创建堆栈树
	set _plat_level [info level];
	for {set _plat_i $_plat_level} {$_plat_i >= 1} {incr _plat_i -1} {
		set _plat_stack [info level $_plat_i];
		set _plat_spacepos [string first " " $_plat_stack];
		set _plat_param "";
		if { $_plat_spacepos != -1 } {
			incr _plat_spacepos;
			set _plat_param [string range $_plat_stack $_plat_spacepos end];
			incr _plat_spacepos -1;
			set _plat_stack [string range $_plat_stack 0 $_plat_spacepos];
		}
		output -$_plat_nOT "<ROOT,27>$_plat_stack,$_plat_param";
		
		unset _plat_stack _plat_spacepos _plat_param;
	}
	output -$_plat_nOT "<ROOT,27>$_plat_str_globalspace,{null}";
	
	# 销毁临时变量
	unset _plat_nOT _plat_level _plat_i _plat_win_stack _plat_str_globalspace;
}
