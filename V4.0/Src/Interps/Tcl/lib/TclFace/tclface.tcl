#################################################################
# File       : TclFace.tcl
# Author     : Blueant
# Version    : 1.0.20101022
# Date       : 2005-10-31
# Description: Tcl Face script library
#################################################################

package provide TclFace 1.0

package require Itcl

catch {namespace import itcl::*}

set g_tlf_obj_count 0		;# TLF对象名计数器,用于自动产生对象名
set g_tlf_mode		tlf		;# TclFace模式是(tlf or tsf)

# 如果platform_path不存在,则设置,并设置为TSF模式
if {[info exist platform_path] == 0} {
	catch {
		set platform_path "[ns_info home]/modules"
		set g_tlf_mode tsf
	}
}

if {[info exist platform_path_style] == 0} {
	set platform_path_style "style"
}

if {[info exist platform_ui_style] == 0} {
	set platform_ui_style default
}

if {[info exist platform_language] == 0} {
	set platform_language english
}

#-------------------------------------------------------------
# set tclface mode to tlf or tsf
#-------------------------------------------------------------
proc set_tlf_mode {mode} {
	global g_tlf_mode
	set g_tlf_mode $mode
}

#-------------------------------------------------------------
# TLF Template class define
#-------------------------------------------------------------
if {[itcl::find classes TLFTemplate] != "TLFTemplate"} {
::itcl::class  TLFTemplate {
	constructor {{style "default"}} {};
	destructor {};
	
	### data member ###
	private variable _templateStyle;#模板风格
	private variable _templatePath;	#模板路径
	private variable _template;		#模板数组
	private variable _templateVars;	#模板变量数组
	private variable _templateConsts;	#模板常量数组(如果同名常量有多个,则每个数组元素是一个列表)
	
	### public methods ###
	public method  LoadTemplateFile {filename};	# 加载模板文件
	public method  GetTemplateList {{type ""} {field ""}};	# 获取模板块列表
	public method  GetTemplateBlock {type field {name default}};	# 获取指定模板块
	public method  GetTemplateBlockByName {fullname};	# 获取指定模板块
	public method  GetTemplateVars {type field {name default}};	# 获取指定模板块变量列表
	public method  GetTemplateVarsByName {fullname};	# 获取指定模板块变量列表
	public method  GetTemplateConstValue {name};	# 获取指定模板常量值
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFTemplate::constructor {{style "default"}} {
	global platform_path
	# 设置模板路径
	set _templatePath "$platform_path/template/html/$style";
	# 如果模板路径不存在,就用默认路径
	if {[file exist $_templatePath] == 0} {
		set _templatePath "$platform_path/template/html/default";
	}
	catch {LoadTemplateFile "default.tft"}
}

#-------------------------------------------------------------
#	单元析构函数
#-------------------------------------------------------------
::itcl::body TLFTemplate::destructor {} {
}

#-------------------------------------------------------------
#	加载模板文件
#	参数:
#	filename	: 模板文件
#-------------------------------------------------------------
::itcl::body TLFTemplate::LoadTemplateFile {filename} {
	# 模板文件为XML格式
	if {[string first "/" $filename] == -1 && [string first "\\" $filename] == -1} {
		set xmlfilename "$_templatePath/$filename"
	} else {
		set xmlfilename $filename
	}
	set xmlTemplate [xml_open file $xmlfilename]
	
	set nodeBlocks [xml_get $xmlTemplate rootNode]	
	set lsNodeBlock [xml_get $xmlTemplate childNodes $nodeBlocks]
	foreach node $lsNodeBlock {
		set nodeType [xml_get $xmlTemplate nodeName $node]
		
		if {$nodeType == "consts"} {
			set lsNodeConst [xml_get $xmlTemplate childNodes $node]
			foreach nodeConst $lsNodeConst {
				set constName [xml_get $xmlTemplate nodeAttr -node $nodeConst name]
				set constValue [xml_get $xmlTemplate nodeAttr -node $nodeConst value]
				# const数组的每个元素都是一个列表,也就是每个常量都可以有多个值
				lappend _templateConsts($constName) [list $constValue]
			}
			continue
		}
		
		set nodeField [xml_get $xmlTemplate nodeAttr -node $node Field]
		set nodeName [xml_get $xmlTemplate nodeAttr -node $node Name]
		set nodeContent [xml_get $xmlTemplate childNode $node Content]
		set _template($nodeType.$nodeField.$nodeName) [xml_get $xmlTemplate nodeText -node $nodeContent -cdata]
		set _templateVars($nodeType.$nodeField.$nodeName) {}
		set nodeVars [xml_get $xmlTemplate childNode $node Vars]
		set lsNodeVar [xml_get $xmlTemplate childNodes $nodeVars]
		foreach nodeVar $lsNodeVar {
			set varName [xml_get $xmlTemplate nodeAttr -node $nodeVar name]
			set varValue [xml_get $xmlTemplate nodeAttr -node $nodeVar value]
			lappend _templateVars($nodeType.$nodeField.$nodeName) [list $varName $varValue]
		}
	}
	
	xml_close $xmlTemplate
}

#-------------------------------------------------------------
#	获取模板块列表
#	参数:
#	type	: 块类型
#	field	: 块字段
#-------------------------------------------------------------
::itcl::body TLFTemplate::GetTemplateList {{type ""} {field ""}} {
	if {$field != ""} {
		set tempPre "$type.$field."
	} elseif {$type != ""} {
		set tempPre "$type."
	} else {
		return [array names _template]
	}
	
	set lsTemplate {}
	foreach templateName [array names _template] {
		if {[string first $tempPre $templateName] != -1} {
			lappend lsTemplate $templateName
		}
	}
	return $lsTemplate
}

#-------------------------------------------------------------
#	获取指定模板块
#	参数:
#	type	: 块类型
#	field	: 块字段
#	name	: 块名字
#-------------------------------------------------------------
::itcl::body TLFTemplate::GetTemplateBlock {type field {name default}} {
	return $_template($type.$field.$name);
}

#-------------------------------------------------------------
#	获取指定模板块
#	参数:
#	fullname: 块名字
#-------------------------------------------------------------
::itcl::body TLFTemplate::GetTemplateBlockByName {fullname} {
	return $_template($fullname);
}

#-------------------------------------------------------------
#	获取指定模板块变量列表
#	参数:
#	type	: 块类型
#	field	: 块字段
#	name	: 块名字
#-------------------------------------------------------------
::itcl::body TLFTemplate::GetTemplateVars {type field {name default}} {
	return $_templateVars($type.$field.$name);
}

#-------------------------------------------------------------
#	获取指定模板块变量列表
#	参数:
#	fullname: 块名字
#-------------------------------------------------------------
::itcl::body TLFTemplate::GetTemplateVarsByName {fullname} {
	return $_templateVars($fullname);
}

#-------------------------------------------------------------
#	获取指定模板常量值
#	参数:
#	name	: 常量名字
#-------------------------------------------------------------
::itcl::body TLFTemplate::GetTemplateConstValue {name} {
	return $_templateConsts($name);
}

# 创建缺省的模板管理对象
if {[itcl::find objects g_TemplateManager -class TLFTemplate] != "g_TemplateManager"} {
	global platform_ui_style;
	TLFTemplate g_TemplateManager $platform_ui_style; #"default"
}

#-------------------------------------------------------------
# TLF Base Object class define
#-------------------------------------------------------------
if {[itcl::find classes TLFObject] != "TLFObject"} {
::itcl::class  TLFObject {
	constructor {{id ""}  {parent ""}} {};
	destructor {};
	
	### data member ###
	protected variable _obj_id;			#对象ID
	protected variable _parent;			#父对象
	protected variable _contentHtml;	#对象内容
	protected variable _contentTag;		#对象内容对应的标记名,如果为空则为全部
	protected variable _lsObj;			#附加对象列表
	protected variable _enableDrawObjs;	#是否画附加对象
	protected variable _form_action;	#Form的动作
	protected variable _form_post_type;	#Form的提交类型
	protected variable _action;			#动作
	protected variable _actionObj;		#动作作用于哪个对象上(空表示根对象)
	protected variable _submitAction;	#提交动作(JavaScript动作)
	protected variable _submitActionObj;#提交动作作用于哪个对象上(空表示无)
	protected variable _varList;		#存储所有序列化变量的数组
	protected variable _enableSerial;	#是否允许对象序列化
	protected variable _enableDraw;		#是否允许显示此对象
	protected variable _disabled;		#是否禁用(变灰)
	protected variable _jsproperty;		#JavaScript属性字段
	protected variable _styleType;		#对象风格(对应模板库中的风格)
	protected variable _addBr;			#是否需要在对象后面加一个换行符
	protected variable _addTrTd;		#是否需要在对象前后加TRTD
	protected variable _addTrTdAlign;	#在对象前后加TRTD的对齐方式
	
	### public methods ###
	public method  Reset {};			# 复位对象
	public method  SetObjId {id};		# 设置Id
	public method  GetObjId {};			# 获取Id
	public method  GetClassName {};		# 获取类名
	public method  GetFullId {};		# 获取完整Id
	public method  SetParent {parent};	# 设置父对象
	public method  GetParent {};		# 获取父对象
	public method  SetContentTag {tag};	# 设置对象内容对应的标记名
	public method  SetStyleType {style};# # 设置对象风格(对应模板库中的风格)
	public method  SetDrawObjs {enable};# 是否画附加对象
	public method  SetEnableSerial {enable};	# 设置序列化允许标志
	public method  GetEnableSerial {};	# 获取序列化允许标志
	public method  SetEnableDraw {enable};	# 设置对象显示允许标志
	public method  GetEnableDraw {};	# 获取对象显示允许标志
	public method  SetDisable {disable};	# 设置是否紧用标志
	public method  SetJavaScriptProperty {property};	# 设置JavaScript属性字段
	public method  AddBr {};			# 在对象后面加一个换行符
	public method  GetBr {};			# 获取是否加换行符标记
	public method  AddTrTd {{enable true} {align ""}};	# 在对象前后加TRTD
	public method  GetTrTd {};			# 获取是否加TRTD标记
	public method  GetTrTdAlign {};		# 获取加TRTD标记的对齐方式
	public method  SerializeVar {dir varName};	# 序列化对象的一个变量
	public method  SerializeVars {dir};	# 序列化对象的所有变量
	public method  Serialize {dir xmlfile xmlnode};	# 对象序列化
	public method  ShowInformation {};	# 显示对象信息
	public method  SetFormAction {{action ""} {post_type "POST"}};	# 设置Form动作
	public method  ProcessFormValue {name value};	# 处理Form结果
	public method  SetAction {action {actionObj ""}};	# 设置动作
	public method  GetAction {};		# 获取动作
	public method  SetSubmitAction {action {actionObj ""}};# 设置提交动作
	public method  OnCreate {};			# 创建
	public method  OnAfterCreate {};	# 创建之后
	public method  OnInitialUpdate {};	# 对象初始化
	public method  OnEndAction {};		# 动作执行完后的操作
	public method  Draw {};				# 输出对象内容
	public method  AppendText {text};	# 添加文字内容
	public method  AppendTargetTag {tagTr replaceTr {cleartag "-cleartag"}};	# 将一个标记段存储到目标缓冲区中
	public method  ReplaceStringTag {str tagTr replaceTr {cleartag "-cleartag"}};# 替换指定字符串中的替换标记
	public method  GetVarValue {lsVars varName};	# 获取变量值
	public method  GetVarValueDefault {varValue lsVars varName};	# (如果指定值为空则查缺省值)
	public method  AppendObject {obj};	# 添加TLF对象
	public method  CreateObject {args};	# 创建添加TLF对象,返回对象名
	public method  DeleteObject {obj};	# 删除某个对象
	public method  SetTagObject {tagTr {obj ""} {cleartag "-cleartag"}};	# 插入TLF对象,替换模板中的一个标记
	public method  CreateTagObject {args};	# 创建并插入TLF对象,替换模板中的一个标记,返回对象名
	public method  AppendJavaScript {jscript {type "file"}};	# 添加JavaScript
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFObject::constructor {{id ""} {parent ""}} {
	set _obj_id $id;
	set _contentHtml "";
	set _contentTag "";
	set _lsObj {};
	set _enableDrawObjs true;
	set _parent $parent;
	set _form_action "";
	set _form_post_type "POST";
	set _action "";
	set _actionObj "";
	set _submitAction "";
	set _submitActionObj "";
	set _enableSerial true;
	set _enableDraw true;
	set _disabled "";
	set _jsproperty "";
	set _styleType "default";
	set _addBr false;
	set _addTrTd false;
	set _addTrTdAlign "";
	array set _varList {};
}

#-------------------------------------------------------------
#	单元析构函数
#-------------------------------------------------------------
::itcl::body TLFObject::destructor {} {
	foreach obj $_lsObj {
		::itcl::delete object $obj;
	}
}

#-------------------------------------------------------------
# 复位对象
#-------------------------------------------------------------
::itcl::body TLFObject::Reset {} {
	set _contentHtml "";
	foreach obj $_lsObj {
		::itcl::delete object $obj;
	}
	set _lsObj {};
	array unset _varList;
	array set _varList {};
	set _action "";
	set _actionObj "";
}

#-------------------------------------------------------------
# 设置ID
#-------------------------------------------------------------
::itcl::body TLFObject::SetObjId {id} {
	set _obj_id $id;
}

#-------------------------------------------------------------
# 获取ID
#-------------------------------------------------------------
::itcl::body TLFObject::GetObjId {} {
	return $_obj_id;
}

#-------------------------------------------------------------
# 获取类名
#-------------------------------------------------------------
::itcl::body TLFObject::GetClassName {} {
	set classname [$this info class];
	if {[string first "::" $classname] == 0} {
		set classname [string range $classname 2 end];
	}
	return $classname;
}

#-------------------------------------------------------------
# 获取完整的Id,从顶层对象开始遍历
#-------------------------------------------------------------
::itcl::body TLFObject::GetFullId {} {
	if {$_parent != ""} {
		return "[$_parent GetFullId].$_obj_id";
	} else {
		return $_obj_id;
	}
}

#-------------------------------------------------------------
# 设置父对象
#-------------------------------------------------------------
::itcl::body TLFObject::SetParent {parent} {
	set _parent $parent;
}

#-------------------------------------------------------------
# 获取父对象
#-------------------------------------------------------------
::itcl::body TLFObject::GetParent {} {
	return $_parent;
}

#-------------------------------------------------------------
# 设置对象内容对应的标记名
#-------------------------------------------------------------
::itcl::body TLFObject::SetContentTag {tag} {
	set _contentTag $tag;
}

#-------------------------------------------------------------
# 设置对象风格(对应模板库中的风格)
#-------------------------------------------------------------
::itcl::body TLFObject::SetStyleType {style} {
	set _styleType $style;
}

#-------------------------------------------------------------
# 设置是否画附加对象
#-------------------------------------------------------------
::itcl::body TLFObject::SetDrawObjs {enable} {
	set _enableDrawObjs $enable;
}

#-------------------------------------------------------------
# 设置序列化允许标志
#-------------------------------------------------------------
::itcl::body TLFObject::SetEnableSerial {enable} {
	set _enableSerial $enable;
}

#-------------------------------------------------------------
# 获取序列化允许标志
#-------------------------------------------------------------
::itcl::body TLFObject::GetEnableSerial {} {
	return $_enableSerial;
}

#-------------------------------------------------------------
# 设置对象显示允许标志
#-------------------------------------------------------------
::itcl::body TLFObject::SetEnableDraw {enable} {
	set _enableDraw $enable;
}

#-------------------------------------------------------------
# 获取对象显示允许标志
#-------------------------------------------------------------
::itcl::body TLFObject::GetEnableDraw {} {
	return $_enableDraw;
}

#-------------------------------------------------------------
# 设置是否紧用标志
#-------------------------------------------------------------
::itcl::body TLFObject::SetDisable {disable} {
	if {($disable == "true") || ($disable == "disable")} {
		set _disabled "disabled";
	} else {
		set _disabled "";
	}
}

#-------------------------------------------------------------
# 设置JavaScript属性字段
#-------------------------------------------------------------
::itcl::body TLFObject::SetJavaScriptProperty {property} {
	set _jsproperty $property;
}

#-------------------------------------------------------------
# 在对象后面加一个换行符
#-------------------------------------------------------------
::itcl::body TLFObject::AddBr {} {
	set _addBr true;
}

#-------------------------------------------------------------
# 获取是否加换行符标记
#-------------------------------------------------------------
::itcl::body TLFObject::GetBr {} {
	return $_addBr;
}

#-------------------------------------------------------------
# 在对象前后加<TR><TD>
#-------------------------------------------------------------
::itcl::body TLFObject::AddTrTd {{enable true} {align ""}} {
	set _addTrTd $enable;
	set _addTrTdAlign $align;
}

#-------------------------------------------------------------
# 获取是否加<TR><TD>标记
#-------------------------------------------------------------
::itcl::body TLFObject::GetTrTd {} {
	return $_addTrTd;
}

#-------------------------------------------------------------
# 获取加TRTD标记的对齐方式
#-------------------------------------------------------------
::itcl::body TLFObject::GetTrTdAlign {} {
	return $_addTrTdAlign;
}

#-------------------------------------------------------------
# 序列化对象的一个变量
#-------------------------------------------------------------
::itcl::body TLFObject::SerializeVar {dir varName} {
	upvar $varName _local_varName
	if {$dir == "get"} {
		set lsTemp [array get _varList $varName]
		# 只有在变量尚未赋值的情况下才从XML文件中序列化
		if {([llength $lsTemp] != 0) && ($_local_varName == "")} {
			set _local_varName [lindex $lsTemp 1]
		}
	} elseif {$dir == "set"} {
		set _varList($varName) $_local_varName;
	}
}

#-------------------------------------------------------------
# 序列化对象的所有变量
#-------------------------------------------------------------
::itcl::body TLFObject::SerializeVars {dir} {
}

#-------------------------------------------------------------
# 对象序列化
#-------------------------------------------------------------
::itcl::body TLFObject::Serialize {dir xmlfile xmlnode} {
	if {$dir == "get"} {
		#puts "--$_obj_id,$dir,$xmlfile,$xmlnode"
		# 获取
		set className [xml_get $xmlfile nodeAttr -node $xmlnode Class]
		# 判断类名是否正确
		if {$className == [$this GetClassName]} {
			set nodeVarList [xml_get $xmlfile childNode $xmlnode VarList]
			if {$nodeVarList != 0} {
				set lsNodeVar [xml_get $xmlfile childNodes $nodeVarList]
				foreach nodeVar $lsNodeVar {
					set nodeName [xml_get $xmlfile nodeAttr -node $nodeVar Name]
					set nodeValue [xml_get $xmlfile nodeText -node $nodeVar]
					#puts "==$nodeName,$nodeValue"
					if {$nodeName != ""} {
						set _varList($nodeName) $nodeValue
					}
				}
			}
		}
		# 序列化所有需要序列化的变量到变量列表中
		SerializeVars $dir
	} elseif {$dir == "set"} {
		# 设置
		# 序列化所有需要序列化的变量到变量列表中
		SerializeVars $dir
		set className [xml_get $xmlfile nodeAttr -node $xmlnode Class]
		# 判断类名是否正确
		if {$className != [$this GetClassName]} {
			# 类型不正确就修改类名
			xml_set $xmlfile nodeAttr -node $xmlnode Class [$this GetClassName];
			set className [xml_get $xmlfile nodeAttr -node $xmlnode Class]
		}
		if {$className == [$this GetClassName]} {
			set nodeVarList [xml_get $xmlfile childNode $xmlnode VarList]
			
			if {($nodeVarList == 0) && ([array size _varList] > 0)} {
				# 如果VarList节点不存在,且_VarList数组变量不为空,则创建此节点
				set nodeVarList [xml_create $xmlfile node -node $xmlnode VarList]
			}
			
			if {$nodeVarList != 0} {
				set lsNodeVar [xml_get $xmlfile childNodes $nodeVarList]
				# 生成当前XML变量列表,记录所有变量名和XML节点
				set lsCurNode {}
				foreach nodeVar $lsNodeVar {
					set nodeName [xml_get $xmlfile nodeAttr -node $nodeVar Name]
					if {$nodeName != ""} {
						lappend lsCurNode [list $nodeName $nodeVar]
					}
				}
				# 将所有对象变量序列化到XML中,如果节点不存在就创建
				foreach nodeName [array names _varList] {
					set pos [lsearch $lsCurNode [list $nodeName *]]
					if {$pos != -1} {
						# 如果在XML变量列表中找到了变量,则直接设置此节点的值
						set nodeVar [lindex [lindex $lsCurNode $pos] 1]
						xml_set $xmlfile nodeText -node $nodeVar $_varList($nodeName)
						continue
					} else {
						# 否则就创建新的XML节点
						set nodeVar [xml_create $xmlfile node -node $nodeVarList Var]
						if {$nodeVar != 0} {
							xml_set $xmlfile nodeAttr -node $nodeVar Name $nodeName
							xml_set $xmlfile nodeText -node $nodeVar $_varList($nodeName)
						}
					}
				}
				# 删除xml序列化文件中多余的变量子节点(无法区分哪些是多余的变量,此功能暂不实现)
#				foreach nodeAtXml $lsCurNode {
#					puts $nodeAtXml
#					puts [array names _varList]
#					set nodeName [lindex $nodeAtXml 0]
#					if {[lsearch [array names _varList] [lindex $nodeAtXml 0]] == -1} {
#						puts "--[lindex $nodeAtXml 0]"
#					}
#				}
			}
		}
	}
	
	# 子对象序列化
	set lsObjId {}
	foreach obj $_lsObj {
		set childNode [xml_get $xmlfile childNode $xmlnode [$obj GetObjId]];
		if {($childNode == 0) && ([$obj GetEnableSerial] == "true")} {
			# 如果子对象允许序列化但在xml文件中不存在,则创建xml节点
			set childNode [xml_create $xmlfile node -node $xmlnode [$obj GetObjId]];
			if {$childNode != 0} {
				xml_set $xmlfile nodeAttr -node $childNode Class [$obj GetClassName];
			}
		}
		if {$childNode != 0} {
			$obj Serialize $dir $xmlfile $childNode;
		}
		lappend lsObjId [$obj GetObjId];
	}
	# 删除xml序列化文件中多余的子节点
	set lsNodeChildObj [xml_get $xmlfile childNodes $xmlnode]
	foreach nodeChildObj $lsNodeChildObj {
		set nodeObjId [xml_get $xmlfile nodeName $nodeChildObj]
		if {($nodeObjId != "VarList") && ([lsearch $lsObjId $nodeObjId] == -1)} {
			xml_remove $xmlfile node -node $xmlnode $nodeChildObj
		}
	}
}

#-------------------------------------------------------------
# 显示对象信息
#-------------------------------------------------------------
::itcl::body TLFObject::ShowInformation {} {
	puts "###information $_obj_id: classname=[GetClassName], parent=[GetParent], fullid=[GetFullId]"
	# 子对象信息
	foreach obj $_lsObj {
		$obj ShowInformation
	}
}

#-------------------------------------------------------------
# 设置Form动作
#-------------------------------------------------------------
::itcl::body TLFObject::SetFormAction {{action ""} {post_type "POST"}} {
	global interpName g_tlf_mode
	
	if {$action == ""} {
		if {$g_tlf_mode == "tsf"} {
			set _form_action "[ns_conn url]"
		} else {
			set _form_action "tcl:<<$interpName>><form><file>[info script]";
		}
	} else {
		set _form_action $action;
	}
	set _form_post_type $post_type;
}

#-------------------------------------------------------------
# 处理Form结果
#-------------------------------------------------------------
::itcl::body TLFObject::ProcessFormValue {name value} {
	#puts "[GetObjId],$name,$value"
	if {[string first "[GetObjId]." $name] == 0} {
		# 是此对象的子对象
		set name [string range $name [string length "[GetObjId]."] end]
		
		# 对于坐标类型,只考虑x的情况,否则会重复调用两次
		if {$name == "x"} {
			# 如果是.x或.y,则表示是图象button类的对象,不需要继续向下分析,按照下面正好匹配的方法处理
			if {$_action != ""} {
				if {$_actionObj != ""} {
					# 执行指定动作对象的动作
					$_actionObj $_action $value
				} else {
					# 动作对象为空表示作用于根对象,找到最顶层对象,设置对象的_action
					set objParent $this
					while {[$objParent GetParent] != ""} {
						set objParent [$objParent GetParent]
					}
					if {$objParent != ""} {
						$objParent SetAction $_action
					}
				}
			}
			return true
		}
		
		#puts "==$name,$_lsObj"
		foreach obj $_lsObj {
			if {[string first "." $name] == -1} {
				# 如果name中没有.,则name和objid必须相同
				if {[$obj GetObjId] == $name} {
					$obj ProcessFormValue $name $value
					return true
				}
			} else {
				# 用objid+.来比较,否则如果objid正好和name的前面相同但不完整,
				# 就会判断错误,例如obj_tlf1和obj_tlf10的判断结果
				if {[string first "[$obj GetObjId]." $name] == 0} {
					$obj ProcessFormValue $name $value
					return true
				}
			}
		}
	} elseif {[GetObjId] == $name} {
		# 如果正好匹配,则检查_action变量,如果非空,则设置最顶层对象的action
		if {$_action != ""} {
			if {$_actionObj != ""} {
				# 执行指定动作对象的动作
				$_actionObj $_action $value
			} else {
				# 动作对象为空表示作用于根对象,找到最顶层对象,设置对象的_action
				set objParent $this
				while {[$objParent GetParent] != ""} {
					set objParent [$objParent GetParent]
				}
				if {$objParent != ""} {
					$objParent SetAction $_action
				}
			}
			return true
		}
	}
}

#-------------------------------------------------------------
# 设置动作
#-------------------------------------------------------------
::itcl::body TLFObject::SetAction {action {actionObj ""}} {
	set _action $action
	set _actionObj $actionObj
}

#-------------------------------------------------------------
# 获取动作
#-------------------------------------------------------------
::itcl::body TLFObject::GetAction {} {
	return $_action
}

#-------------------------------------------------------------
# 设置提交动作
#-------------------------------------------------------------
::itcl::body TLFObject::SetSubmitAction {action {actionObj ""}} {
	set _submitAction $action
	set _submitActionObj $actionObj
}

#-------------------------------------------------------------
# 创建
#-------------------------------------------------------------
::itcl::body TLFObject::OnCreate {} {
}

#-------------------------------------------------------------
# 创建之后
#-------------------------------------------------------------
::itcl::body TLFObject::OnAfterCreate {} {
}

#-------------------------------------------------------------
# 对象初始化
#-------------------------------------------------------------
::itcl::body TLFObject::OnInitialUpdate {} {
}

#-------------------------------------------------------------
# 动作执行完后的操作
#-------------------------------------------------------------
::itcl::body TLFObject::OnEndAction {} {
}

#-------------------------------------------------------------
# 输出对象内容
#-------------------------------------------------------------
::itcl::body TLFObject::Draw {} {
	# 返回保存的内容
	set _temp_contentHtml "";
	if {$_contentTag == ""} {
		set _temp_contentHtml $_contentHtml;
	}
	
	# 生成所有子对象的内容
	if {$_form_action != ""} {
		set _temp_contentHtml "<FORM name='[GetFullId]' action='$_form_action' onsubmit='processFormSubmit(this)' method=$_form_post_type>\n$_temp_contentHtml";
	}
	
	if {$_enableDrawObjs == true} {
		foreach obj $_lsObj {
			if {[$obj GetEnableDraw] == true} {
				set objContent [$obj Draw];
				# 是否加<TR><TD>
				if {[$obj GetTrTd] == "true"} {
					set align [$obj GetTrTdAlign]
					if {$align != ""} {
						set align "align=$align"
					}
					set _temp_contentHtml "$_temp_contentHtml\n<tr $align><td $align>$objContent</td></tr>";
				} else {
					set _temp_contentHtml "$_temp_contentHtml\n$objContent";
				}
				# 是否在对象后面添加一个换行符
				if {[$obj GetBr] == "true"} {
					set _temp_contentHtml "$_temp_contentHtml<br>";
				}
			}
		}
	}
	
	if {$_form_action != ""} {
		set _temp_contentHtml "$_temp_contentHtml\n</FORM>";
	}
	
	if {$_jsproperty != ""} {
		ReplaceStringTag _temp_contentHtml "JSPROP" $_jsproperty;
	} 
	
	if {$_contentTag != ""} {
		# 如果设置了内容对应的标记,则先将所有子对象内容替换到标记中
		AppendTargetTag $_contentTag $_temp_contentHtml;
	} else {
		# 否则直接将将所有子对象内容添加到内容的后面
		set _contentHtml $_temp_contentHtml;
	}
	
	return $_contentHtml;
}

#-------------------------------------------------------------
#	将一个标记段存储到目标缓冲区中
#	参数:
#	tagTr	: 在模板库文件和模板页面中的Tr标记名
#	cleartag: 是否清除替换元素标记(-cleartag or -append)
#-------------------------------------------------------------
::itcl::body TLFObject::AppendTargetTag {tagTr replaceTr {cleartag "-cleartag"}} {
	# 存储到目标缓冲区
	set index [string first "%$tagTr%" $_contentHtml];
	while { $index != -1 } {
		if { $cleartag == "-cleartag" } {
			set tagLen [string length $tagTr];
			set _contentHtml [string replace $_contentHtml $index [expr $index+$tagLen+1] $replaceTr];
			set index [string first "%$tagTr%" $_contentHtml];
		} else {
			set _contentHtml [string replace $_contentHtml $index $index "$replaceTr%"];
			set index -1;
		}
	}
}

#-------------------------------------------------------------
#	替换指定字符串中的标记
#	参数:
#	buffer	: 目标缓冲区
#	tagTr	: 在模板库文件和模板页面中的Tr标记名
#	cleartag: 是否清除替换元素标记(-cleartag or -append)
#-------------------------------------------------------------
::itcl::body TLFObject::ReplaceStringTag {str tagTr replaceTr {cleartag "-cleartag"}} {
	upvar $str _str
	# 存储到目标缓冲区
	set index [string first "%$tagTr%" $_str];
	while { $index != -1 } {
		if { $cleartag == "-cleartag" } {
			set tagLen [string length $tagTr];
			set _str [string replace $_str $index [expr $index+$tagLen+1] $replaceTr];
			set index [string first "%$tagTr%" $_str];
		} else {
			set _str [string replace $_str $index $index "$replaceTr%"];
			set index -1;
		}
	}
}

#-------------------------------------------------------------
#	获取变量值
#	参数:
#	lsVars: 变量列表
#	varName: 变量名
#-------------------------------------------------------------
::itcl::body TLFObject::GetVarValue {lsVars varName} {
	foreach var $lsVars {
		if {[lindex $var 0] == $varName} {
			return [lindex $var 1]
		}
	}
	return ""
}

#-------------------------------------------------------------
#	获取变量值(如果指定值为空则查缺省值)
#	参数:
#	lsVars: 变量列表
#	varName: 变量名
#-------------------------------------------------------------
::itcl::body TLFObject::GetVarValueDefault {varValue lsVars varName} {
	if {$varValue != ""} {
		return $varValue
	}
	foreach var $lsVars {
		if {[lindex $var 0] == $varName} {
			return [lindex $var 1]
		}
	}
	return ""
}

#-------------------------------------------------------------
# 添加文字
#-------------------------------------------------------------
::itcl::body TLFObject::AppendText {text} {
	return [CreateObject ::TLFText $text]
}

#-------------------------------------------------------------
# 添加TLF对象
#-------------------------------------------------------------
::itcl::body TLFObject::AppendObject {obj} {
	lappend _lsObj $obj;
	# 设置对象的父对象和对象Id
	$obj SetParent $_obj_id;
	$obj SetObjId $obj
}

#-------------------------------------------------------------
# 创建添加TLF对象,返回对象名
#-------------------------------------------------------------
::itcl::body TLFObject::CreateObject {args} {
	global g_tlf_obj_count
	# 修改对象名产生的方法,使用自定义的全局计数器来产生对象名
	#set lsTmp [list [lindex $args 0] "obj_#auto"]
	set lsTmp [list [lindex $args 0] "obj_tlf_$g_tlf_obj_count"]
	incr g_tlf_obj_count
	
	if {[llength $args] > 1} {
		foreach arg [lrange $args 1 end] {
			lappend lsTmp $arg
		}
	}

	set objName [eval $lsTmp]
	#puts $objName
	
	AppendObject $objName

	return "TLFObject::$objName"
}

#-------------------------------------------------------------
# 删除某个对象
#-------------------------------------------------------------
::itcl::body TLFObject::DeleteObject {obj} {
	set index 0
	foreach objItem $_lsObj {
		if {$obj == "TLFObject::$objItem"} {
			::itcl::delete object $objItem;
			set _lsObj [lreplace $_lsObj $index $index];
			return
		}
		incr index
	}
}

#-------------------------------------------------------------
# 插入TLF对象,替换模板中的一个标记
# 参数分别为:tagTr cleartag args
#-------------------------------------------------------------
::itcl::body TLFObject::SetTagObject {tagTr {obj ""} {cleartag "-cleartag"}} {
	if {$obj != ""} {
		#AppendObject $obj
		# 设置对象的父对象和对象Id
		$obj SetParent $_obj_id;
		$obj SetObjId $obj
		AppendTargetTag $tagTr [$obj Draw] $cleartag
	} else {
		AppendTargetTag $tagTr ""
	}
}

#-------------------------------------------------------------
# 添加JavaScript
#-------------------------------------------------------------
::itcl::body TLFObject::AppendJavaScript {jscript {type "file"}} {
	if {$_parent != ""} {
		return [$_parent AppendJavaScript $jscript $type];
	}
}

#-------------------------------------------------------------
# 创建并插入TLF对象,替换模板中的一个标记,返回对象名
# 参数分别为:tagTr cleartag args
#-------------------------------------------------------------
::itcl::body TLFObject::CreateTagObject {args} {
	if {[llength $args] < 3} {
		return
	}
	
	set objname [CreateObject [lrange $args 2 end]]
	AppendTargetTag $tagTr [$objname Draw] $cleartag
	return $objname
}

#-------------------------------------------------------------
# TLF Variable class define
#-------------------------------------------------------------
if {[itcl::find classes TLFVariable] != "TLFVariable"} {
::itcl::class  TLFVariable {
	inherit TLFObject
	
	constructor {{name ""} {value ""}} {};
	
	### data member ###
	protected variable _name;			# 变量名
	protected variable _value;			# 变量值
	
	### public methods ###
	public method  SetVar {name value};	# 设置变量
	public method  Draw {};				# 输出对象内容
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFVariable::constructor {{name ""} {value ""}} {
	chain;
	set _name $name;
	set _value $value;
}

#-------------------------------------------------------------
# 设置变量
#-------------------------------------------------------------
::itcl::body TLFVariable::SetVar {name value} {
	set _name $name;
	set _value $value;
}

#-------------------------------------------------------------
# 输出对象内容
#-------------------------------------------------------------
::itcl::body TLFVariable::Draw {} {
	
	set _contentHtml "<INPUT type=hidden name=$_name value=$_value>";
	return $_contentHtml;
}

#-------------------------------------------------------------
# TLF Row class define
#-------------------------------------------------------------
if {[itcl::find classes TLFRow] != "TLFRow"} {
::itcl::class  TLFRow {
	inherit TLFObject
	
	constructor {} {};
	
	### data member ###
	protected variable _color;			# 颜色
	
	### public methods ###
	public method  SetColor {color};	# 设置颜色
	public method  Draw {};				# 输出对象内容
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFRow::constructor {} {
	chain;
	set _color "";
}

#-------------------------------------------------------------
# 设置颜色
#-------------------------------------------------------------
::itcl::body TLFRow::SetColor {color} {
	set _color $color;
}

#-------------------------------------------------------------
# 输出对象内容
#-------------------------------------------------------------
::itcl::body TLFRow::Draw {} {
	set font1 "";
	set font2 "";
	if {$_color != ""} {
		set font1 "<font color=$_color>";
		set font2 "</font>";
	}
	set _contentHtml "<TR $_disabled><TD>$font1[chain]$font2</TD></TR>";
	return $_contentHtml;
}

#-------------------------------------------------------------
# TLF Text class define
#-------------------------------------------------------------
if {[itcl::find classes TLFText] != "TLFText"} {
::itcl::class  TLFText {
	inherit TLFObject
	
	constructor {text {borderColor ""} {bgColor ""}} {};
	
	### data member ###
	protected variable _text;			# 文本
	protected variable _color;			# 颜色
	protected variable _lsText;			# 文本列表
	protected variable _borderColor;	# 边框颜色
	protected variable _bgColor;		# 背景颜色
	
	### public methods ###
	public method  SetText {text};		# 设置文本
	public method  AddText {text};		# 添加文本到文本列表
	public method  SetColor {color};	# 设置颜色
	public method  SetBorder {borderColor {bgColor ""}};# 设置边框
	public method  Draw {};				# 输出对象内容
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFText::constructor {text {borderColor ""} {bgColor ""}} {
	chain;
	set _color "";
	set _text $text;
	set _lsText {};
	set _borderColor $borderColor;
	set _bgColor $bgColor;
}

#-------------------------------------------------------------
# 设置文本
#-------------------------------------------------------------
::itcl::body TLFText::SetText {text} {
	set _text $text;
}

#-------------------------------------------------------------
# 添加文本
#-------------------------------------------------------------
::itcl::body TLFText::AddText {text} {
	lappend _lsText $text;
}

#-------------------------------------------------------------
# 设置颜色
#-------------------------------------------------------------
::itcl::body TLFText::SetColor {color} {
	set _color $color;
}

#-------------------------------------------------------------
# 设置边框
#-------------------------------------------------------------
::itcl::body TLFText::SetBorder {borderColor {bgColor ""}} {
	set _borderColor $borderColor;
	set _bgColor $bgColor;
}

#-------------------------------------------------------------
# 输出对象内容
#-------------------------------------------------------------
::itcl::body TLFText::Draw {} {
	
	# 读取参数
	set varsText [g_TemplateManager GetTemplateVars Text Text];
	
	if {$_borderColor != ""} {
		# 带边框类型模板和参数
		set blockBorderText [g_TemplateManager GetTemplateBlock Text Border];
		set varsBorderText [g_TemplateManager GetTemplateVars Text Border];
	}
	
	if {$_color == ""} {
		set _color [GetVarValue $varsText color];
	}
	
	set font1 "";
	set font2 "";
	if {$_color != ""} {
		set font1 "<font color=$_color>";
		set font2 "</font>";
	}
	
	if {$_borderColor == ""} {
		# 普通类型
		set objParent [GetParent]
		if {($objParent != "") && ([::itcl::is object TLFObject::$objParent]) && ([TLFObject::$objParent isa TLFTable])} {
			# 父对象存在,且父对象是TLFTable或派生类
			set _contentHtml "$font1$_text$font2[chain]";
		} else {
			set _contentHtml "<TR $_disabled><TD>$font1$_text$font2[chain]</TD></TR>";
		}
	} else {
		# 带边框类型
		if {$_borderColor == "default"} {
			set _borderColor [GetVarValue $varsBorderText borderColor];
		}
		if {$_bgColor == ""} {
			set _bgColor [GetVarValue $varsBorderText bgColor];
		}
		set tmpContent $blockBorderText;
		ReplaceStringTag tmpContent "DISABLE" $_disabled;
		ReplaceStringTag tmpContent "BORDERCOLOR" $_borderColor;
		ReplaceStringTag tmpContent "BGCOLOR" $_bgColor;
		if {[llength $_lsText] == 0} {
			ReplaceStringTag tmpContent "CONTENT" "<TR><TD>$font1$_text$font2[chain]</TD></TR>";
		} else {
			set tmpRow "<TD>$font1$_text$font2</TD>";
			foreach text $_lsText {
				set tmpRow "$tmpRow<TD>$font1$text$font2</TD>";
			}
			ReplaceStringTag tmpContent "CONTENT" "<TR>$tmpRow</TR><TR><TD>[chain]</TD></TR>";
		}
		set _contentHtml "<TR><TD>$tmpContent</TD></TR>";
	}
	
	return $_contentHtml;#[chain];
}

#-------------------------------------------------------------
# TLF SyntaxText class define
#-------------------------------------------------------------
if {[itcl::find classes TLFSyntaxText] != "TLFSyntaxText"} {
::itcl::class  TLFSyntaxText {
	inherit TLFText
	
	constructor {text {borderColor ""} {bgColor ""}} {chain $text $borderColor $bgColor} {};
	
	### data member ###
	protected variable _SyntaxText;		# 语言内容
	protected variable _SyntaxLanguage;	# 语言类型
	protected variable _option;			# 选项
	
	### public methods ###
	public method  SetSyntaxLanguage {type};	# 设置语言类型
	public method  SetOption {option};			# 设置选项
	public method  SetText {text};				# 设置文本
	public method  LoadFile {filename};			# 加载一个文件的内容
	public method  Draw {};						# 输出对象内容
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFSyntaxText::constructor {text {borderColor ""} {bgColor ""}} {
	set _SyntaxText $text;
	set _SyntaxLanguage "";
	set _option "";
}

#-------------------------------------------------------------
# 设置语言类型
#-------------------------------------------------------------
::itcl::body TLFSyntaxText::SetOption {option} {
	set _option "$_option:$option";
}

#-------------------------------------------------------------
# 设置选项
#-------------------------------------------------------------
::itcl::body TLFSyntaxText::SetSyntaxLanguage {type} {
	set _SyntaxLanguage $type;
	AppendJavaScript "js/file/syntax/SyntaxHighlighter.css" css;
	AppendJavaScript "js/file/syntax/shCore.js";	
	AppendJavaScript "js/file/syntax/shBrush${_SyntaxLanguage}.js";
	AppendJavaScript "window.onload = function () {dp.SyntaxHighlighter.HighlightAll('_TLFSyntaxText_Code');}" body;
}

#-------------------------------------------------------------
# 设置内容
#-------------------------------------------------------------
::itcl::body TLFSyntaxText::SetText {text} {
	set _SyntaxText $text;
}

#-------------------------------------------------------------
# 加载一个文件的内容
#-------------------------------------------------------------
::itcl::body TLFSyntaxText::LoadFile {filename} {
	catch {
		set f [open $filename r];
		set _SyntaxText [read $f];
		close $f;
	}
}

#-------------------------------------------------------------
# 输出对象内容
#-------------------------------------------------------------
::itcl::body TLFSyntaxText::Draw {} {
	set _text "<pre name='_TLFSyntaxText_Code' class='${_SyntaxLanguage}${_option}'>\n$_SyntaxText\n</pre>";
	#set _text "<textarea name='_TLFSyntaxText_Code' class='${_SyntaxLanguage}${_option}'>\n$_SyntaxText\n</textarea>";
	return [chain];
}

#-------------------------------------------------------------
# TLF Link class define
#-------------------------------------------------------------
if {[itcl::find classes TLFLink] != "TLFLink"} {
::itcl::class  TLFLink {
	inherit TLFObject
	
	constructor {{link ""} {text ""} {title ""}} {};
	
	### data member ###
	protected variable _text;			# 文本
	protected variable _link;			# 链接
	protected variable _title;			# 链接提示
	protected variable _color;			# 颜色
	
	### public methods ###
	public method  SetText {text};		# 设置文本
	public method  SetLink {link};		# 设置链接
	public method  SetTitle {title};	# 设置链接提示
	public method  SetColor {color};	# 设置颜色
	public method  Draw {};				# 输出对象内容
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFLink::constructor {{link ""} {text ""} {title ""}} {
	chain;
	set _color "";
	set _text $text;
	set _link $link;
	set _title $title;
}

#-------------------------------------------------------------
# 设置文本
#-------------------------------------------------------------
::itcl::body TLFLink::SetText {text} {
	set _text $text;
}

#-------------------------------------------------------------
# 设置链接
#-------------------------------------------------------------
::itcl::body TLFLink::SetLink {link} {
	set _link $link;
}

#-------------------------------------------------------------
# 设置链接提示
#-------------------------------------------------------------
::itcl::body TLFLink::SetTitle {title} {
	set _title $title;
}

#-------------------------------------------------------------
# 设置颜色
#-------------------------------------------------------------
::itcl::body TLFLink::SetColor {color} {
	set _color $color;
}

#-------------------------------------------------------------
# 输出对象内容
#-------------------------------------------------------------
::itcl::body TLFLink::Draw {} {
	global interpName g_tlf_mode;
	
	set font1 "";
	set font2 "";
	if {$_color != ""} {
		set font1 "<font color=$_color>";
		set font2 "</font>";
	}
	set linkTemp "";
	if {$_link != ""} {
		if {$g_tlf_mode == "tsf"} {
			set linkTemp "[ns_conn url]?$_link"
		} else {
			set linkTemp "tcl:<<$interpName>><file>[info script]?$_link";
		}
	}
	set _contentHtml "$font1<a href=\"$linkTemp\" title=\"$_title\" $_disabled>$_text</a>$font2[chain]";
	return $_contentHtml;#[chain];
}

#-------------------------------------------------------------
# TLF RadioButton class define
#-------------------------------------------------------------
if {[itcl::find classes TLFRadioButton] != "TLFRadioButton"} {
::itcl::class  TLFRadioButton {
	inherit TLFObject
	
	constructor {{name ""} {text ""} {check ""}} {};
	
	### data member ###
	protected variable _name;			# 变量名
	protected variable _text;			# 文字
	protected variable _checked;		# 是否选中
	
	### public methods ###
	public method  SerializeVars {dir};	# 序列化对象的所有变量
	public method  ProcessFormValue {name value};	# 处理Form结果
	public method  Draw {};				# 输出对象内容
	public method  SetCheck {check};	# 设置检查标志
	public method  GetCheck {};			# 获取检查标志
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFRadioButton::constructor {{name ""} {text ""} {check ""}} {
	chain;
	set _name $name;
	set _text $text;
	set _checked $check;
}

#-------------------------------------------------------------
# 对象序列化
#-------------------------------------------------------------
::itcl::body TLFRadioButton::SerializeVars {dir} {
	chain $dir;
	SerializeVar $dir _checked;
}

#-------------------------------------------------------------
# 处理Form结果
#-------------------------------------------------------------
::itcl::body TLFRadioButton::ProcessFormValue {name value} {
	if {[chain $name $value] == "true"} {
		return true
	}
	set _checked $value
}

#-------------------------------------------------------------
# 输出对象内容
#-------------------------------------------------------------
::itcl::body TLFRadioButton::Draw {} {
	global g_TemplateManager;
	
	# 获取模板数据
	set blockRadioButton [g_TemplateManager GetTemplateBlock RadioButton RadioButton];
	
	set _contentHtml $blockRadioButton;
	
	if {($_name == "") && ($_parent != "")} {
		AppendTargetTag "NAME" $_parent;
	} else {
		AppendTargetTag "NAME" $_name;
	}
	AppendTargetTag "ID" [GetFullId];
	AppendTargetTag "TEXT" $_text;
	
	set check "";
	switch $_checked {
	true -
	1 -
	on {set check "checked"}
	}
	AppendTargetTag "CHECKED" $check;
	
	AppendTargetTag "DISABLED" $_disabled;

	return [chain];
}

#-------------------------------------------------------------
# 设置检查标志
#-------------------------------------------------------------
::itcl::body TLFRadioButton::SetCheck {check} {
	set _checked $check;
}

#-------------------------------------------------------------
# 获取检查标志
#-------------------------------------------------------------
::itcl::body TLFRadioButton::GetCheck {} {
	return $_checked;
}

#-------------------------------------------------------------
# TLF CheckBox class define
#-------------------------------------------------------------
if {[itcl::find classes TLFCheckBox] != "TLFCheckBox"} {
::itcl::class  TLFCheckBox {
	inherit TLFObject

	constructor {{text ""} {check ""}} {};
	
	### data member ###
	protected variable _text;			# 文字
	protected variable _checked;		# 是否选中
	
	### public methods ###
	public method  SerializeVars {dir};	# 序列化对象的所有变量
	public method  ProcessFormValue {name value};	# 处理Form结果
	public method  Draw {};				# 输出对象内容
	public method  SetCheck {check};	# 设置检查标志
	public method  GetCheck {};			# 获取检查标志
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFCheckBox::constructor {{text ""} {check ""}} {
	chain;
	set _text $text;
	set _checked $check;
}

#-------------------------------------------------------------
# 对象序列化
#-------------------------------------------------------------
::itcl::body TLFCheckBox::SerializeVars {dir} {
	chain $dir;
	SerializeVar $dir _checked;
}

#-------------------------------------------------------------
# 处理Form结果
#-------------------------------------------------------------
::itcl::body TLFCheckBox::ProcessFormValue {name value} {
	if {[chain $name $value] == "true"} {
		return true
	}
	set _checked $value
}

#-------------------------------------------------------------
# 输出对象内容
#-------------------------------------------------------------
::itcl::body TLFCheckBox::Draw {} {
	global g_TemplateManager;
	
	# 获取模板数据
	set blockCheckBox [g_TemplateManager GetTemplateBlock CheckBox CheckBox];
	
	set _contentHtml $blockCheckBox;
	AppendTargetTag "NAME" [GetFullId];
	AppendTargetTag "TEXT" $_text;
	
	set check "";
	switch $_checked {
	true -
	1 -
	on {set check "checked"}
	}
	AppendTargetTag "CHECKED" $check;
	
	AppendTargetTag "DISABLED" $_disabled;

	return [chain];
}

#-------------------------------------------------------------
# 设置检查标志
#-------------------------------------------------------------
::itcl::body TLFCheckBox::SetCheck {check} {
	set _checked $check;
}

#-------------------------------------------------------------
# 获取检查标志
#-------------------------------------------------------------
::itcl::body TLFCheckBox::GetCheck {} {
	return $_checked;
}

#-------------------------------------------------------------
# TLF Select class define
#-------------------------------------------------------------
if {[itcl::find classes TLFSelect] != "TLFSelect"} {
::itcl::class  TLFSelect {
	inherit TLFObject
	
	constructor {{width ""}} {};
	
	### data member ###
	protected variable _width;			# 宽度
	protected variable _multiple;		# 多行模式
	protected variable _count;			# 选项计数器
	protected variable _lsOption;		# 选项列表
	protected variable _curSelect;		# 当前选择的
	
	### public methods ###
	public method  SerializeVars {dir};	# 序列化对象的所有变量
	public method  ProcessFormValue {name value};	# 处理Form结果
	public method  SetWidth {width};	# 设置宽度
	public method  SetMultiple {multiple};# 设置多行模式
	public method  Draw {};				# 输出对象内容
	public method  AddOption {text {value ""}};		# 添加行
	public method  SetSelectValue {curSelect};	# 设置当前选择项
	public method  GetSelectValue {};	# 获取当前选择项的值
	public method  GetSelectIndex {};	# 获取当前选择项的索引
	public method  GetSelectText {};	# 获取当前选择项的文字
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFSelect::constructor {{width ""}} {
	chain;
	set _width $width;
	set _multiple "";
	set _count 0;
	set _lsOption {};
	set _curSelect "";
}

#-------------------------------------------------------------
# 对象序列化
#-------------------------------------------------------------
::itcl::body TLFSelect::SerializeVars {dir} {
	chain $dir;
	SerializeVar $dir _curSelect;
}

#-------------------------------------------------------------
# 处理Form结果
#-------------------------------------------------------------
::itcl::body TLFSelect::ProcessFormValue {name value} {
	if {[chain $name $value] == "true"} {
		return true
	}
	set _curSelect $value
}

#-------------------------------------------------------------
# 设置宽度
#-------------------------------------------------------------
::itcl::body TLFSelect::SetWidth {width} {
	set _width $width;
}

#-------------------------------------------------------------
# 设置多行模式
#-------------------------------------------------------------
::itcl::body TLFSelect::SetMultiple {multiple} {
	if {($multiple == "true") || ($multiple == "multiple")} {
		set _multiple "multiple";
	} else {
		set _multiple "";
	}
}

#-------------------------------------------------------------
# 输出对象内容
#-------------------------------------------------------------
::itcl::body TLFSelect::Draw {} {
	global g_TemplateManager;
	
	# 获取模板数据
	set blockSelect [g_TemplateManager GetTemplateBlock Select Select $_styleType];
	set blockOption [g_TemplateManager GetTemplateBlock Select Option];
	
	set _contentHtml $blockSelect;
	
	AppendTargetTag "NAME" [GetFullId];
	AppendTargetTag "DISABLED" $_disabled;
	if {$_multiple != ""} {
		# 多行模式(仅对普通风格的有效)
		AppendTargetTag "MULTIPLE" "$_multiple";
	} else {
		AppendTargetTag "MULTIPLE" "size=1";
	}
	
	set text_length 0;
	foreach option $_lsOption {
		set tmpOption $blockOption;
		set text [lindex $option 0];
		set value [lindex $option 1];
		if {[string length $text] > $text_length} {
			set text_length [string length $text]
		}
		ReplaceStringTag tmpOption "TEXT" $text
		ReplaceStringTag tmpOption "VALUE" $value
		set selected ""
		if {$_curSelect == $value} {
			set selected "selected"
		}
		ReplaceStringTag tmpOption "SELECTED" $selected
		AppendTargetTag "OPTIONS" $tmpOption -append
	}
	AppendTargetTag "OPTIONS" ""
	
	if {$_styleType == "ss"} {
		AppendJavaScript "js/file/select.js" file;
		if {$_width == ""} {
			set _width [expr $text_length*8+15];
		}
		AppendTargetTag "WIDTH" $_width;
		regsub -all {\n} $_contentHtml {} _contentHtml
	}
	
	return [chain];
}

#-------------------------------------------------------------
# 输出对象内容
#-------------------------------------------------------------
::itcl::body TLFSelect::AddOption {text {value ""}} {
	if {$value == ""} {
		set value $_count;
	}
	lappend _lsOption [list $text $value];
	incr _count;
}

#-------------------------------------------------------------
# 设置当前选择项
#-------------------------------------------------------------
::itcl::body TLFSelect::SetSelectValue {curSelect} {
	set _curSelect $curSelect;
}

#-------------------------------------------------------------
# 获取当前选择项的值
#-------------------------------------------------------------
::itcl::body TLFSelect::GetSelectValue {} {
	return $_curSelect;
}

#-------------------------------------------------------------
# 获取当前选择项的索引
#-------------------------------------------------------------
::itcl::body TLFSelect::GetSelectIndex {} {
	set count 0;
	foreach option $_lsOption {
		if {[lindex $option 1] == $_curSelect} {
			return $count;
		}
		incr count;
	}
	return -1;
}

#-------------------------------------------------------------
# 获取当前选择项的文字
#-------------------------------------------------------------
::itcl::body TLFSelect::GetSelectText {} {
	set count 0;
	foreach option $_lsOption {
		if {[lindex $option 1] == $_curSelect} {
			return [lindex $option 0];
		}
		incr count;
	}
	return "";
}

#-------------------------------------------------------------
# TLF Input class define
#-------------------------------------------------------------
if {[itcl::find classes TLFInput] != "TLFInput"} {
::itcl::class  TLFInput {
	inherit TLFObject
	
	constructor {{title ""} {text ""} {size ""} {bold ""}} {};
	
	### data member ###
	protected variable _title;			# 标题
	protected variable _text;			# 文字
	protected variable _size;			# 长度
	protected variable _bold;			# 粗体
	protected variable _rowcount;		# 行数
	protected variable _type;			# 类型
	
	### public methods ###
	public method  SerializeVars {dir};	# 序列化对象的所有变量
	public method  SetType {type};		# 设置类型
	public method  SetSize {size};		# 设置宽度
	public method  SetRowCount {rowcount};	# 设置行数
	public method  SetText {text};		# 设置文本
	public method  GetInputText {};		# 获取文字
	public method  ProcessFormValue {name value};	# 处理Form结果
	public method  Draw {};				# 输出对象内容
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFInput::constructor {{title ""} {text ""} {size ""} {bold ""}} {
	chain;
	set _title $title;
	set _text $text;
	set _size $size;
	set _bold $bold;
	set _rowcount 1;
	set _type "text";
}

#-------------------------------------------------------------
# 对象序列化
#-------------------------------------------------------------
::itcl::body TLFInput::SerializeVars {dir} {
	chain $dir;
	SerializeVar $dir _text;
}

#-------------------------------------------------------------
# 设置类型
#-------------------------------------------------------------
::itcl::body TLFInput::SetType {type} {
	set _type $type;
}

#-------------------------------------------------------------
# 设置宽度
#-------------------------------------------------------------
::itcl::body TLFInput::SetSize {size} {
	set _size $size;
}

#-------------------------------------------------------------
# 设置行数
#-------------------------------------------------------------
::itcl::body TLFInput::SetRowCount {rowcount} {
	set _rowcount $rowcount;
}

#-------------------------------------------------------------
# 设置文字
#-------------------------------------------------------------
::itcl::body TLFInput::SetText {text} {
	set _text $text;
}

#-------------------------------------------------------------
# 获取文字
#-------------------------------------------------------------
::itcl::body TLFInput::GetInputText {} {
	return $_text;
}

#-------------------------------------------------------------
# 处理Form结果
#-------------------------------------------------------------
::itcl::body TLFInput::ProcessFormValue {name value} {
	if {[chain $name $value] == "true"} {
		return true
	}
	set _text $value
}

#-------------------------------------------------------------
# 输出对象内容
#-------------------------------------------------------------
::itcl::body TLFInput::Draw {} {
	set bold1 "";
	set bold2 "";
	if {$_bold == "bold"} {
		set bold1 "<b>";
		set bold2 "</b>";
	}
	if {$_rowcount == 1} {
		set size "";
		if {$_size != ""} {
			set size "style=\"WIDTH: ${_size}px\""
		}
		set _contentHtml "$bold1$_title$bold2<INPUT class=normal type=$_type $size name=\"[GetFullId]\" value=\"$_text\" $_disabled>";
	} else {
		set _contentHtml "$bold1$_title$bold2<textarea class=normal cols=$_size rows=$_rowcount name=\"[GetFullId]\" $_disabled>$_text</textarea>";
	}
	return [chain];
}

#-------------------------------------------------------------
# TLF PathFileInput class define
#-------------------------------------------------------------
if {[itcl::find classes TLFPathFileInput] != "TLFPathFileInput"} {
::itcl::class  TLFPathFileInput {
	inherit TLFInput
	
	constructor {{title ""} {text ""} {size ""} {bold ""}} {chain $title $text $size $bold} {};
	
	### data member ###
	protected variable _set_by_button;	# 值是否已经被按钮设置过
	protected variable _type;			# 类型
	protected variable _filter;			# 文件对话框过滤字符串
	protected variable _saveto;			# 文件对话框类型(打开还是保存)
	protected variable _dlgtitle;		# 对话框标题
	
	### public methods ###
	public method  SetType {type};		# 设置类型(path or file)
	public method  SetFileDlg {filter {saveto ""}};	# 设置文件对话框参数
	public method  SetDlgTitle {dlgtitle};	# 设置对话框标题
	public method  ProcessFormValue {name value};	# 处理Form结果
	public method  OnButtonClick {value};	# 按钮点击的处理函数
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFPathFileInput::constructor {{title ""} {text ""} {size ""} {bold ""}} {
	set _type "file";
	set _filter "All Files(*.*)|*.*||";
	set _saveto "";
	set _dlgtitle "";
	set _set_by_button false;
}

#-------------------------------------------------------------
# 设置类型(path or file)
#-------------------------------------------------------------
::itcl::body TLFPathFileInput::SetType {type} {
	set _type $type;
	set objButton [CreateObject TLFButton];
	$objButton SetImage "image/folder.gif";
	$objButton SetParent $this;
	$objButton SetAction OnButtonClick $this
}

#-------------------------------------------------------------
# 设置文件对话框参数
#-------------------------------------------------------------
::itcl::body TLFPathFileInput::SetFileDlg {filter {saveto ""}} {
	set _filter $filter;
	set _saveto $saveto;
}

#-------------------------------------------------------------
# 设置对话框标题
#-------------------------------------------------------------
::itcl::body TLFPathFileInput::SetDlgTitle {dlgtitle} {
	set _dlgtitle $dlgtitle;
}

#-------------------------------------------------------------
# 处理Form结果
#-------------------------------------------------------------
::itcl::body TLFPathFileInput::ProcessFormValue {name value} {
	if {[TLFObject::ProcessFormValue $name $value] == "true"} {
		return true
	}
	if {$_set_by_button != "true"} {
		set _text $value
	}
}

#-------------------------------------------------------------
# 按钮点击的处理函数
#-------------------------------------------------------------
::itcl::body TLFPathFileInput::OnButtonClick {value} {
	if {$_type == "file"} {
		# 文件对话框
		if {$_saveto != ""} {
			set filepath [FileDlg -filter $_filter $_saveto]
		} else {
			set filepath [FileDlg -filter $_filter]
		}
		if {$filepath != ""} {
			set _text $filepath
			set _set_by_button true
		}
	} elseif {$_type == "path"} {
		# 路径选择对话框
		set filepath [PathSelect $_dlgtitle $_text]
		if {$filepath != ""} {
			set _text $filepath
			set _set_by_button true
		}
	}
}

#-------------------------------------------------------------
# TLF DateTimeInput class define
#-------------------------------------------------------------
if {[itcl::find classes TLFDateTimeInput] != "TLFDateTimeInput"} {
::itcl::class  TLFDateTimeInput {
	inherit TLFInput
	
	constructor {{title ""} {text ""} {size "100"} {bold ""}} {chain $title $text $size $bold} {};
	
	### data member ###
	protected variable _set_by_button;	# 值是否已经被按钮设置过
	protected variable _type;			# 类型
	protected variable _objButton;		# 按钮
	protected variable _lsSetup;		# 设置信息列表
	protected variable _flat;			# 是否flat类型日历
	
	### public methods ###
	public method  SetProperty {name value};		# 设置Setup属性
	public method  ProcessFormValue {name value};	# 处理Form结果
	public method  Draw {};				# 输出对象内容
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFDateTimeInput::constructor {{title ""} {text ""} {size "100"} {bold ""}} {
	set _type "date";
	set _set_by_button false;
	set _objButton "";
	set _lsSetup {{ifFormat "%Y-%m-%d"}};
	set _flat false;
}

#-------------------------------------------------------------
# 设置Setup属性
#-------------------------------------------------------------
::itcl::body TLFDateTimeInput::SetProperty {name value} {
	if {$name == "flat"} {
		# 是否平板类型
		set _flat $value
		return
	}
	
	set index 0
	foreach setup $_lsSetup {
		if {[lindex $setup 0] == $name} {
			lset _lsSetup $index 1 $value
			return
		}
		incr index
	}
	lappend _lsSetup [list $name $value]
}

#-------------------------------------------------------------
# 处理Form结果
#-------------------------------------------------------------
::itcl::body TLFDateTimeInput::ProcessFormValue {name value} {
	if {[TLFObject::ProcessFormValue $name $value] == "true"} {
		return true
	}
	if {$_set_by_button != "true"} {
		set _text $value
	}
}

#-------------------------------------------------------------
# 输出对象内容
#-------------------------------------------------------------
::itcl::body TLFDateTimeInput::Draw {} {
	global g_TemplateManager;
	
	if {($_flat == "false") && ($_objButton == "")} {
		set _objButton [CreateObject TLFButton];
		$_objButton SetImage "image/calendar.gif";#"image/iconCalendar.gif";
		$_objButton SetParent $this;
	}
	
	if {$_flat == "true"} {
		# 平板类型
		set _contentHtml "<div style='float: right; margin-left: 1em; margin-bottom: 1em;' id='[GetFullId]'></div>";
	} else {
		chain;
	}

	# 引用的JavaScript和CSS文件
	AppendJavaScript "js/file/jscalendar/skins/aqua/theme.css" css;
	AppendJavaScript "js/file/jscalendar/calendar.js" file;
	AppendJavaScript "js/file/jscalendar/calendar-setup.js" file;
	LangStringEC STR_JSCALENDAR_LANGFILE "calendar-en.js" "calendar-zh.js";
	AppendJavaScript "js/file/jscalendar/lang/$STR_JSCALENDAR_LANGFILE" file;
	
	# 获取模板数据
	set blockCalendar [g_TemplateManager GetTemplateBlock Calendar Calendar];
	set tmpJBody $blockCalendar;
	
	if {$_flat == "true"} {
		ReplaceStringTag tmpJBody "CALENDAR_SETUP" "flatCallback : \"\"," -append;
	}
	
	if {$_objButton != ""} {
		ReplaceStringTag tmpJBody "CALENDAR_SETUP" "button : \"[$_objButton GetFullId]\"," -append;
	}
	foreach setup $_lsSetup {
		ReplaceStringTag tmpJBody "CALENDAR_SETUP" "[lindex $setup 0] : \"[lindex $setup 1]\"," -append;
	}
	if {$_flat == "true"} {
		ReplaceStringTag tmpJBody "CALENDAR_SETUP" "flat : \"[GetFullId]\"" -append;
	} else {
		ReplaceStringTag tmpJBody "CALENDAR_SETUP" "inputField : \"[GetFullId]\"" -append;
	}
	ReplaceStringTag tmpJBody "CALENDAR_SETUP" "";
	
	AppendJavaScript $tmpJBody body;
	
	return $_contentHtml;#[chain];
}

#-------------------------------------------------------------
# TLF Button class define
#-------------------------------------------------------------
if {[itcl::find classes TLFButton] != "TLFButton"} {
::itcl::class  TLFButton {
	inherit TLFObject
	
	constructor {{text ""} {bold ""}} {};
	
	### data member ###
	protected variable _text;			# 文字
	protected variable _bold;			# 粗体
	protected variable _image;			# 图片
	protected variable _style;			# 风格
	
	### public methods ###
	public method  SetText {text};		# 设置文字
	public method  SetBold {bold};		# 设置粗体
	public method  SetImage {image};	# 设置图片
	public method  SetStyle {style};	# 设置风格
	public method  Draw {};				# 输出对象内容
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFButton::constructor {{text ""} {bold ""}} {
	chain;
	set _text $text;
	set _bold $bold;
	set _image "";
	set _style "";
}

#-------------------------------------------------------------
# 设置文字
#-------------------------------------------------------------
::itcl::body TLFButton::SetText {text} {
	set _text $text;
}

#-------------------------------------------------------------
# 设置按钮粗体
#-------------------------------------------------------------
::itcl::body TLFButton::SetBold {bold} {
	set _bold $bold;
}

#-------------------------------------------------------------
# 设置图片
#-------------------------------------------------------------
::itcl::body TLFButton::SetImage {image} {
	set _image $image;
}

#-------------------------------------------------------------
# 设置风格
#-------------------------------------------------------------
::itcl::body TLFButton::SetStyle {style} {
	set _style $style;
}

#-------------------------------------------------------------
# 输出对象内容
#-------------------------------------------------------------
::itcl::body TLFButton::Draw {} {
	global g_TemplateManager;
	
	# 获取模板数据
	if {$_image != ""} {
		set blockButton [g_TemplateManager GetTemplateBlock Button Image];
		set _contentHtml $blockButton;
		AppendTargetTag "NAME" [GetFullId];
		AppendTargetTag "VALUE" $_text;
		AppendTargetTag "IMAGE" $_image;
	} else {
		set blockButton [g_TemplateManager GetTemplateBlock Button Button];
		set _contentHtml $blockButton;
		AppendTargetTag "NAME" [GetFullId];
		AppendTargetTag "VALUE" $_text;
		if {$_style == ""} {
			AppendTargetTag "STYLE" "";
		} else {
			AppendTargetTag "STYLE" "style='$_style'";
		}
	}
	
	AppendTargetTag "DISABLED" $_disabled;
	
	if {($_submitActionObj != "") && ($_submitAction != "")} {
		AppendTargetTag "ONSUBMIT" "[$_submitActionObj GetObjId].$_submitAction;return false;";
	}
	AppendTargetTag "ONSUBMIT" ""
	
	return [chain];
}

#-------------------------------------------------------------
# TLF table class define
#-------------------------------------------------------------
if {[itcl::find classes TLFTable] != "TLFTable"} {
::itcl::class  TLFTable {
	inherit TLFObject
	
	constructor {} {};
	
	### data member ###
	private variable _lsTitle;			# 标题列表
	private variable _lsLine;			# 行列表
	private variable _haveTitle;		# 是否有标题行
	private variable _gridColor;		# 表格线颜色
	private variable _titleColor;		# 标题文字颜色
	private variable _titleBgColor;		# 标题背景颜色
	private variable _lineColor;		# 正文文字颜色
	private variable _lineBgColor;		# 正文背景颜色
	private variable _item_style;		# TD项中的style属性
	
	### public methods ###
	public method  Draw {};				# 输出对象内容
	public method  SetHaveTitle {enable};# 设置是否有标题行
	public method  SetItemStyle {style};# 设置TD项中的style属性
	public method  SetGridColor {color};# 设置表格线颜色
	public method  SetTitleColor {color};# 设置标题行文字颜色
	public method  SetTitleBgColor {color};# 设置标题行背景颜色
	public method  SetLineColor {color};# 设置表格行文字颜色
	public method  SetLineBgColor {color};# 设置表格行背景颜色
	public method  SetOneLineStyle {lineno colorBg {colorText ""} {bold ""}};	# 设置表格一行风格
	public method  SetOneColumnStyle {columnno colorBg {colorText ""} {bold ""}};	# 设置表格一列风格
	public method  AddColumn {title {color ""} {bold ""} {bgColor ""} {width ""}};	# 添加列
	public method  AddLine {args};		# 添加行
	public method  AddLine2 {args};		# 添加行
	public method  DeleteLine {lineno};	# 删除行
	public method  GetLineCount {};		# 获取行数
	public method  SetItemText {lineno colno text {link ""} {tip ""} {color ""} {bgColor ""} {bold ""}}; #设置某一单元格文字内容(可选链接、提示、颜色、背景色、粗体)
	public method  GetItemText {lineno colno};			# 获取某一单元格文字
	public method  SetItemColor {lineno colno color {bgColor ""}};	# 设置某一单元格颜色
	public method  AppendItemText {lineno colno text};	# 在某一单元格中添加文字内容(添加到后面)
	public method  AppendItemObject {lineno colno args};# 在某一单元格中添加对象(添加到后面)
	public method  GetItemObject {lineno colno};	# 获取对象名
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFTable::constructor {} {
	chain;
	set _addTrTd true;
	set _lsTitle {};
	set _lsLine {};
	set _haveTitle true;
	set _gridColor "";
	set _titleColor "";
	set _titleBgColor "";
	set _lineColor "";
	set _lineBgColor "";
	set _item_style "";
	SetDrawObjs false;	# 禁止画附加对象
}

#-------------------------------------------------------------
# 输出对象内容
#-------------------------------------------------------------
::itcl::body TLFTable::Draw {} {
	global g_TemplateManager;
	
	# 获取模板数据
	set blockFrame [g_TemplateManager GetTemplateBlock Table Frame];
	set blockTitle [g_TemplateManager GetTemplateBlock Table Title];
	set blockTitleItem [g_TemplateManager GetTemplateBlock Table TitleItem];
	set varsTitle [g_TemplateManager GetTemplateVars Table TitleItem];
	set varsItem [g_TemplateManager GetTemplateVars Table Item];
	set blockLine [g_TemplateManager GetTemplateBlock Table Line];
	set blockItem [g_TemplateManager GetTemplateBlock Table Item];
	set _contentHtml $blockFrame
	
	AppendTargetTag "DISABLED" $_disabled;
	
	if {$_gridColor == ""} {
		set varsFrame [g_TemplateManager GetTemplateVars Table Frame];
		set _gridColor [GetVarValue $varsFrame bgColor]
	}
	AppendTargetTag "GRIDCOLOR" $_gridColor
	
	if {$_haveTitle == "true"} {
		# 画标题
		set tmpTitle $blockTitle
		foreach title $_lsTitle {
			set tmp $blockTitleItem
			
			set title_text [lindex $title 0]
			set title_color [lindex $title 1]
			set title_bold [lindex $title 2]
			set title_bgColor [lindex $title 3]
			set title_width [lindex $title 4]
			
			if {$title_color == ""} {
				set title_color [GetVarValueDefault $_titleColor $varsTitle color]
			}
			
			set color1 ""
			set color2 ""
			if {$title_color != ""} {
				set color1 "<font color='$title_color'>"
				set color2 "</font>"
			}
			
			if {$title_bold == ""} {
				set title_bold [GetVarValue $varsTitle bold]
			}
	
			set bold1 ""
			set bold2 ""
			if {$title_bold == "bold"} {
				set bold1 "<b>"
				set bold2 "</b>"
			}
			
			if {$title_bgColor == ""} {
				set title_bgColor [GetVarValueDefault $_titleBgColor $varsTitle bgColor]
			}
			
			if {$title_width != ""} {
				set title_width "width='$title_width'"
			}

			ReplaceStringTag tmp "WIDTH" $title_width
			ReplaceStringTag tmp "BGCOLOR" $title_bgColor
			ReplaceStringTag tmp "TEXT" "$color1$bold1$title_text$bold2$color2"
			ReplaceStringTag tmpTitle "TITLE" $tmp -append
		}
		ReplaceStringTag tmpTitle "TITLE" ""
		AppendTargetTag "TITLE" $tmpTitle -append
	}
	AppendTargetTag "TITLE" ""
	
	set line_color $_lineColor
	set line_bgColor [GetVarValueDefault $_lineBgColor $varsItem bgColor]
	
	foreach line $_lsLine {
		set tmpLine $blockLine
		set columnIndex 0
		foreach item $line {
			set tmpItem $blockItem
			
			set item_color $line_color
			set item_bgcolor $line_bgColor
			if {[lindex $item 5] != ""} {
				set item_bgcolor [lindex $item 5]
			}
			
			set item_text [lindex $item 0]
			set item_type [lindex $item 1]
			
			set item_width [lindex [lindex $_lsTitle $columnIndex] 4]
			if {$item_width != ""} {
				set item_width "width='$item_width'"
			}
			
			if {$item_type == "object"} {
				ReplaceStringTag tmpItem "WIDTH" $item_width
				ReplaceStringTag tmpItem "BGCOLOR" $item_bgcolor
				ReplaceStringTag tmpItem "STYLE" $_item_style
				foreach obj $item_text {
					# 如果是TLF对象,则直接调用对象的Draw函数获取结果
					ReplaceStringTag tmpItem "TEXT" [$obj Draw] -append
				}
				ReplaceStringTag tmpItem "TEXT" ""
				ReplaceStringTag tmpLine "ITEM" $tmpItem -append
				continue
			}

			set item_link [lindex $item 2]
			set item_tip [lindex $item 3]
			if {$item_link != ""} {
				set item_color ""
			}
			if {[lindex $item 4] != ""} {
				set item_color [lindex $item 4]
			}
			set item_bold [lindex $item 6]
			
			set link1 ""
			set link2 ""
			if {$item_link != ""} {
				set link1 "<a title='$item_tip' href='$item_link'>"
				set link2 "</a>"
			}
			
			set color1 ""
			set color2 ""
			if {$item_color != ""} {
				set color1 "<font color='$item_color'>"
				set color2 "</font>"
			}
			
			set bold1 ""
			set bold2 ""
			if {$item_bold == "bold"} {
				set bold1 "<b>"
				set bold2 "</b>"
			}
			
			ReplaceStringTag tmpItem "WIDTH" $item_width
			ReplaceStringTag tmpItem "BGCOLOR" $item_bgcolor
			ReplaceStringTag tmpItem "STYLE" $_item_style
			ReplaceStringTag tmpItem "TEXT" "$link1$color1$bold1$item_text$bold2$color2$link2"
			ReplaceStringTag tmpLine "ITEM" $tmpItem -append
			
			incr columnIndex
		}
		ReplaceStringTag tmpLine "ITEM" ""
		AppendTargetTag "LINE" $tmpLine -append
	}
	AppendTargetTag "LINE" ""
	
	return [chain];
}

#-------------------------------------------------------------
# 设置是否有标题行
#-------------------------------------------------------------
::itcl::body TLFTable::SetHaveTitle {enable} {
	set _haveTitle $enable;
}

#-------------------------------------------------------------
# 设置TD项中的style属性
#-------------------------------------------------------------
::itcl::body TLFTable::SetItemStyle {style} {
	set _item_style "style='$style'";
}

#-------------------------------------------------------------
# 设置表格线颜色
#-------------------------------------------------------------
::itcl::body TLFTable::SetGridColor {color} {
	set _gridColor $color;
}

#-------------------------------------------------------------
# 设置标题行文字颜色
#-------------------------------------------------------------
::itcl::body TLFTable::SetTitleColor {color} {
	set _titleColor $color;
}

#-------------------------------------------------------------
# 设置标题行背景颜色
#-------------------------------------------------------------
::itcl::body TLFTable::SetTitleBgColor {color} {
	set _titleBgColor $color;
}

#-------------------------------------------------------------
# 设置表格行文字颜色
#-------------------------------------------------------------
::itcl::body TLFTable::SetLineColor {color} {
	set _lineColor $color;
}

#-------------------------------------------------------------
# 设置表格行背景颜色
#-------------------------------------------------------------
::itcl::body TLFTable::SetLineBgColor {color} {
	set _lineBgColor $color;
}

#-------------------------------------------------------------
# 添加列
#-------------------------------------------------------------
::itcl::body TLFTable::AddColumn {title {color ""} {bold ""} {bgColor ""} {width ""}} {
	lappend _lsTitle [list $title $color $bold $bgColor $width];
}

#-------------------------------------------------------------
# 添加行
#-------------------------------------------------------------
::itcl::body TLFTable::AddLine {args} {
	set line {}
	foreach item_text $args {
		# 分别为文字、类型、链接、提示、颜色、背景色、粗体
		# 类型为空表示普通类型,object表示TLF对象
		set item [list $item_text "" "" "" "" "" ""]
		lappend line $item
	}
	lappend _lsLine $line;
	return [expr [llength $_lsLine] -1];
}

#-------------------------------------------------------------
# 添加行(支持链接、提示)
#-------------------------------------------------------------
::itcl::body TLFTable::AddLine2 {args} {
	lappend _lsLine $args;
	set count 0
	foreach item $args {
		if {[lindex $item 1] == "object"} {
			set lsTmp [list CreateObject]
			foreach arg [lindex $item 0] {
				lappend lsTmp $arg
			}
			set objTmp [eval $lsTmp];
			lset _lsLine end $count 0 $objTmp;
		} elseif {[lindex $item 1] == "objname"} {
			# 设置对象的父对象和对象Id
			[lindex $item 0] SetParent $_obj_id;
			[lindex $item 0] SetObjId [lindex $item 0];
		}
		incr count;
	}
	return [expr [llength $_lsLine] -1];
}

#-------------------------------------------------------------
# 删除行
#-------------------------------------------------------------
::itcl::body TLFTable::DeleteLine {lineno} {
	if {$lineno == "all"} {
		# 删除所有
		set _lsLine {}
	} else {
		# 删除指定行
		set lsTmp {}
		set index 0
		foreach line $_lsLine {
			if {$index != $lineno} {
				lappend lsTmp $line
			}
			incr index
		}
		set _lsLine $lsTmp
	}
}

#-------------------------------------------------------------
# 获取行数
#-------------------------------------------------------------
::itcl::body TLFTable::GetLineCount {} {
	return [llength $_lsLine];
}

#-------------------------------------------------------------
# 设置某一单元格文字内容(可选链接、提示、颜色、背景色、粗体)
#-------------------------------------------------------------
::itcl::body TLFTable::SetItemText {lineno colno text {link ""} {tip ""} {color ""} {bgColor ""} {bold ""}} {
	if {$lineno >= [llength $_lsLine]} {
		return
	}
	
	# 如果列不足,则补足
	set line [lindex $_lsLine $lineno]
	if {[llength $line] <= $colno} {
		while {[llength $line] <= $colno} {
			 lappend line ""
		}
		lset _lsLine $lineno $line
	}
	
	set item [lindex [lindex $_lsLine $lineno] $colno]
	if {([lindex $item 1] == "object") || ([lindex $item 1] == "objname")} {
		return
	}
	while {[llength $item] < 7} {
		lappend item ""
	}
	
	lset item 0 "$text"
	if {$link != ""} {
		lset item 2 "$link"
	}
	if {$tip != ""} {
		lset item 3 "$tip"
	}
	if {$color != ""} {
		lset item 4 "$color"
	}
	if {$bgColor != ""} {
		lset item 5 "$bgColor"
	}
	if {$bold != ""} {
		lset item 6 "$bold"
	}
	lset _lsLine $lineno $colno $item
}

#-------------------------------------------------------------
# 获取某一单元格文字
#-------------------------------------------------------------
::itcl::body TLFTable::GetItemText {lineno colno} {
	if {$lineno >= [llength $_lsLine]} {
		return ""
	}
	
	# 如果列不足,则补足
	if {[llength [lindex $_lsLine $lineno]] <= $colno} {
		return ""
	}
	
	set item [lindex [lindex $_lsLine $lineno] $colno]
	if {([lindex $item 1] == "object") || ([lindex $item 1] == "objname")} {
		# 返回对象名
		return [lindex $item 0]
	} else {
		# 返回文字内容
		return [lindex $item 0]
	}
}

#-------------------------------------------------------------
# 设置某一单元格颜色
#-------------------------------------------------------------
::itcl::body TLFTable::SetItemColor {lineno colno color {bgColor ""}} {
	if {$lineno >= [llength $_lsLine]} {
		return
	}
	
	# 如果列不足,则退出
	if {[llength [lindex $_lsLine $lineno]] <= $colno} {
		return
	}

	set item [lindex [lindex $_lsLine $lineno] $colno]
	while {[llength $item] < 7} {
		lappend item ""
	}
	if {$color != ""} {
		lset item 4 $color
	}
	if {$bgColor != ""} {
		lset item 5 $bgColor
	}
	lset _lsLine $lineno $colno $item
}

#-------------------------------------------------------------
# 在某一单元格中添加文字内容(添加到后面)
#-------------------------------------------------------------
::itcl::body TLFTable::AppendItemText {lineno colno text} {
	if {$lineno >= [llength $_lsLine]} {
		return
	}
	
	# 如果列不足,则补足
	set line [lindex $_lsLine $lineno]
	if {[llength $line] <= $colno} {
		while {[llength $line] <= $colno} {
			 lappend line ""
		}
		lset _lsLine $lineno $line
	}
	
	set item [lindex [lindex $_lsLine $lineno] $colno]
	if {([lindex $item 1] == "object") || ([lindex $item 1] == "objname")} {
		return
	}
	
	lset _lsLine $lineno $colno 0 "[lindex $item 0]$text"
}

#-------------------------------------------------------------
# 在某一单元格中添加对象(添加到后面)
#-------------------------------------------------------------
::itcl::body TLFTable::AppendItemObject {lineno colno args} {
	if {$lineno >= [llength $_lsLine]} {
		return
	}
	
	# 如果列不足,则补足
	set line [lindex $_lsLine $lineno]
	if {[llength $line] <= $colno} {
		while {[llength $line] <= $colno} {
			 lappend line ""
		}
		lset _lsLine $lineno $line
	}
	
	set item [lindex [lindex $_lsLine $lineno] $colno]
	if {([lindex $item 1] != "object") && ([lindex $item 0] != "")} {
		return
	}
	
	# 组装命令行并创建对象
	set lsTmp [list CreateObject]
	foreach arg $args {
		lappend lsTmp $arg
	}
	
	set objTmp [eval $lsTmp]
	# 生成临时对象名列表
	set lsObjTmp [lindex $item 0]
	lappend lsObjTmp $objTmp
	if {[lindex $item 1] != "object"} {
		# 如果此单元格尚未添加任何内容,则生成完整的内容
		lset _lsLine $lineno $colno [list $lsObjTmp "object"]
	} else {
		# 否则只是增加对象名列表
		lset _lsLine $lineno $colno 0 $lsObjTmp
	}
	
	return $objTmp
}

#-------------------------------------------------------------
# 设置一行风格
#-------------------------------------------------------------
::itcl::body TLFTable::SetOneLineStyle {lineno colorBg {colorText ""} {bold ""}} {
	if {$lineno >= [llength $_lsLine]} {
		return
	}
	set count 0
	foreach item [lindex $_lsLine $lineno] {
		while {[llength $item] < 7} {
			lappend item ""
		}
		if {[llength $item] > 6} {
			if {$colorBg != ""} {
				lset item 5 $colorBg
			}
			if {$colorText != ""} {
				lset item 4 $colorText
			}
			if {$bold != ""} {
				lset item 6 $bold
			}
		}
		lset _lsLine $lineno $count $item
		incr count
	}	
}

#-------------------------------------------------------------
# 设置一列风格
#-------------------------------------------------------------
::itcl::body TLFTable::SetOneColumnStyle {columnno colorBg {colorText ""} {bold ""}} {
	set lineno 0
	foreach line $_lsLine {
		# 定位到列
		if {[llength $line] < $columnno} {
			while {[llength $line] < $columnno} {
				lappend line [list ""]
			}
			lset _lsLine $lineno $line
		}
		set item [lindex $line $columnno]
		# 设置风格
		while {[llength $item] < 7} {
			lappend item ""
		}
		if {[llength $item] > 6} {
			if {$colorBg != ""} {
				lset item 5 $colorBg
			}
			if {$colorText != ""} {
				lset item 4 $colorText
			}
			if {$bold != ""} {
				lset item 6 $bold
			}
		}
		lset _lsLine $lineno $columnno $item
		incr lineno
	}	
}

#-------------------------------------------------------------
# 获取对象名
#-------------------------------------------------------------
::itcl::body TLFTable::GetItemObject {lineno colno} {
	if {$lineno >= [llength $_lsLine]} {
		return
	}
	set item [lindex [lindex $_lsLine $lineno] $colno]
	if {[lindex $item 1] != "object"} {
		return
	}
	
	return [lindex $item 0]
}

#-------------------------------------------------------------
# TLF toggle menu class define
#-------------------------------------------------------------
if {[itcl::find classes TLFToggleMenu] != "TLFToggleMenu"} {
::itcl::class  TLFToggleMenu {
	inherit TLFObject
	
	constructor {} {};
	
	### data member ###
	protected variable _lsMenu;			# 菜单列表
	
	### public methods ###
	public method  Draw {};				# 输出对象内容
	public method  ClearMenu {};		# 清除所有菜单项
	public method  AddMenu {id text image display submenu};	# 添加菜单项
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFToggleMenu::constructor {} {
	chain;
	set _lsMenu {};
}

#-------------------------------------------------------------
# 输出对象内容
#-------------------------------------------------------------
::itcl::body TLFToggleMenu::Draw {} {
	global g_TemplateManager;
	
	# 获取模板数据
	set blockMainMenu [g_TemplateManager GetTemplateBlock ToggleMenu MainMenu];
	set blockSubMenu [g_TemplateManager GetTemplateBlock ToggleMenu SubMenu];
	set _contentHtml ""
	set _id_array_script "<SCRIPT type=text/javascript>var array_toggle_menu_id = new Array("
	
	set count 0
	foreach menu $_lsMenu {
		set tmp $blockMainMenu
		
		set menu_id [lindex $menu 0]
		set menu_text [lindex $menu 1]
		set menu_image [lindex $menu 2]
		set menu_display [lindex $menu 3]
		set lsSubMenu [lindex $menu 4]

		ReplaceStringTag tmp "ID_MAIN_MENU" $menu_id
		ReplaceStringTag tmp "STR_MAIN_MENU" $menu_text
		ReplaceStringTag tmp "IMG_MAIN_MENU" $menu_image
		ReplaceStringTag tmp "SUB_MENU_DISPLAY" $menu_display
		
		foreach submenu $lsSubMenu {
			set tmpSubMenu $blockSubMenu
			ReplaceStringTag tmpSubMenu "STR_SUB_MENU" [lindex $submenu 0]
			ReplaceStringTag tmpSubMenu "SUB_MENU_LINK" [lindex $submenu 1]
			
			ReplaceStringTag tmp "BLOCK_SUB_MENU" $tmpSubMenu -append
		}
		
		ReplaceStringTag tmp "BLOCK_SUB_MENU" ""
		
		set _contentHtml "$_contentHtml\n$tmp"
		
		incr count
		set _id_array_script "$_id_array_script \"$menu_id\""
		if {$count != [llength $_lsMenu]} {
			set _id_array_script "$_id_array_script,"
		}
	}
	
	set _id_array_script "$_id_array_script );</SCRIPT>"
	
	set _contentHtml "$_id_array_script\n$_contentHtml"
	
	return [chain];
}

#-------------------------------------------------------------
# 清除所有菜单项
#-------------------------------------------------------------
::itcl::body TLFToggleMenu::ClearMenu {} {
	set _lsMenu {}
}

#-------------------------------------------------------------
# 添加菜单项
#-------------------------------------------------------------
::itcl::body TLFToggleMenu::AddMenu {id text image display submenu} {
	lappend _lsMenu [list $id $text $image $display $submenu]
}

#-------------------------------------------------------------
# TLF FieldSet class define
#-------------------------------------------------------------
if {[itcl::find classes TLFFieldSet] != "TLFFieldSet"} {
::itcl::class  TLFFieldSet {
	inherit TLFObject
	
	constructor {{title ""}} {};
	
	### data member ###
	protected variable _title;			# 窗口标题
	
	### public methods ###
	public method  Draw {};				# 输出对象内容
	public method  SetTitle {title};	# 设置标题
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFFieldSet::constructor {{title ""}} {
	chain;
	set _title $title;
}

#-------------------------------------------------------------
# 输出对象内容
#-------------------------------------------------------------
::itcl::body TLFFieldSet::Draw {} {
	global g_TemplateManager;
	
	chain;
	
	# 获取模板数据
	set blockFieldSet [g_TemplateManager GetTemplateBlock FieldSet FieldSet];
	
	set tempContentHtml $blockFieldSet;
	
	ReplaceStringTag tempContentHtml "LEGEND" $_title;
	ReplaceStringTag tempContentHtml "DISABLED" $_disabled;
	ReplaceStringTag tempContentHtml "CONTENT" $_contentHtml;
	set _contentHtml $tempContentHtml;

	return $_contentHtml;
}

#-------------------------------------------------------------
# 设置标题
#-------------------------------------------------------------
::itcl::body TLFFieldSet::SetTitle {title} {
	set _title $title;
}

#-------------------------------------------------------------
# TLF TabPage class define
#-------------------------------------------------------------
if {[itcl::find classes TLFTabPage] != "TLFTabPage"} {
::itcl::class  TLFTabPage {
	inherit TLFObject

	constructor {{title ""}} {};
	
	### data member ###
	protected variable _title;			# 页面标题
	
	### public methods ###
	public method  Draw {};				# 输出对象内容
	public method  SetTitle {title};	# 设置页面标题
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFTabPage::constructor {{title ""}} {
	chain;
	set _title $title;
}

#-------------------------------------------------------------
# 输出对象内容
#-------------------------------------------------------------
::itcl::body TLFTabPage::Draw {} {
	global g_TemplateManager;
	
	chain;
	
	# 获取模板数据
	set blockTabPage [g_TemplateManager GetTemplateBlock TabPage TabPage];
	
	set tempContentHtml $blockTabPage;
	
	ReplaceStringTag tempContentHtml "ID" [GetFullId];
	ReplaceStringTag tempContentHtml "TPVAR" [GetParent];
	ReplaceStringTag tempContentHtml "TITLE" $_title;
	ReplaceStringTag tempContentHtml "CONTENT" $_contentHtml;
	set _contentHtml $tempContentHtml;

	return $_contentHtml;
}

#-------------------------------------------------------------
# 设置页面标题
#-------------------------------------------------------------
::itcl::body TLFTabPage::SetTitle {title} {
	set _title $title;
}

#-------------------------------------------------------------
# TLF TabPane class define
#-------------------------------------------------------------
if {[itcl::find classes TLFTabPane] != "TLFTabPane"} {
::itcl::class  TLFTabPane {
	inherit TLFObject

	constructor {{style ""}} {};
	
	### data member ###
	protected variable _style;			# 风格
	
	### public methods ###
	public method  Draw {};				# 输出对象内容
	public method  SetStyle {style};	# 设置风格
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFTabPane::constructor {{style ""}} {
	chain;
	set _style $style;
}

#-------------------------------------------------------------
# 输出对象内容
#-------------------------------------------------------------
::itcl::body TLFTabPane::Draw {} {
	global g_TemplateManager;
	
	chain;
	
	AppendJavaScript "js/file/tabpane/tabpane.js" file
	AppendJavaScript "js/file/tabpane/css/tab.webfx.css" css
	
	# 获取模板数据
	set blockTabPane [g_TemplateManager GetTemplateBlock TabPane TabPane];
	
	set tempContentHtml $blockTabPane;
	
	ReplaceStringTag tempContentHtml "ID" [GetFullId];
	ReplaceStringTag tempContentHtml "TPVAR" [GetObjId];
	
	# 添加页面对象
	ReplaceStringTag tempContentHtml "PAGES" $_contentHtml;
	set _contentHtml $tempContentHtml;

	return $_contentHtml;
}

#-------------------------------------------------------------
# 设置风格
#-------------------------------------------------------------
::itcl::body TLFTabPane::SetStyle {style} {
	set _style $style;
}

#-------------------------------------------------------------
# TLF frame class define
#-------------------------------------------------------------
if {[itcl::find classes TLFFrame] != "TLFFrame"} {
::itcl::class  TLFFrame {
	inherit TLFObject
}
}

#-------------------------------------------------------------
# TLF Window class define
#-------------------------------------------------------------
if {[itcl::find classes TLFWindow] != "TLFWindow"} {
::itcl::class  TLFWindow {
	inherit TLFObject
	
	constructor {{title ""} {type ""}} {};
	
	### data member ###
	protected variable _type;			# 窗口类型
	protected variable _title;			# 窗口标题
	protected variable _width;			# 窗口宽度
	protected variable _height;			# 窗口高度
	protected variable _left;			# 窗口坐标x
	protected variable _top;			# 窗口坐标y
	protected variable _icon;			# 窗口图标
	
	### public methods ###
	public method  SetSize {width height};# 设置窗口大小
	public method  SetWindowPos {left top};# 设置窗口坐标
	public method  SetWindowIcon {ico};	# 设置窗口图标
	public method  Draw {};				# 输出对象内容
	public method  Show {};				# 显示窗口
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFWindow::constructor {{title ""} {type ""}} {
	chain;
	set _title $title;
	set _type $type;
	set _width "";
	set _height "";
	set _left "";
	set _top "";
	set _icon "";
}

#-------------------------------------------------------------
# 设置窗口大小
#-------------------------------------------------------------
::itcl::body TLFWindow::SetSize {width height} {
	set _width $width;
	set _height $height;
}

#-------------------------------------------------------------
# 设置窗口坐标
#-------------------------------------------------------------
::itcl::body TLFWindow::SetWindowPos {left top} {
	set _left $left;
	set _top $top;
}

#-------------------------------------------------------------
# 设置窗口图标
#-------------------------------------------------------------
::itcl::body TLFWindow::SetWindowIcon {ico} {
	set _icon $ico;
}

#-------------------------------------------------------------
# 输出对象内容
#-------------------------------------------------------------
::itcl::body TLFWindow::Draw {} {
	global g_TemplateManager;
	
	chain;

	if {$_type == "popwin"} {
		AppendJavaScript "PopWindow" class
	} else {
		AppendJavaScript "Window" class
	}
	
	# 获取模板数据
	set blockWindow [g_TemplateManager GetTemplateBlock Window Window];
	set tmpJBody $blockWindow;
	
	if {$_type == "popwin"} {
		ReplaceStringTag tmpJBody "WINCLASS" "PopWindow";
	} else {
		ReplaceStringTag tmpJBody "WINCLASS" "Window";
	}
	ReplaceStringTag tmpJBody "WINNAME" $_obj_id;
	ReplaceStringTag tmpJBody "WININIT" "$_obj_id.SetDivId(\"$_obj_id\");" -append;
	
	if {$_title != ""} {
		ReplaceStringTag tmpJBody "WININIT" "$_obj_id.Title = \"$_title\";" -append;
	}
	if {$_width != ""} {
		ReplaceStringTag tmpJBody "WININIT" "$_obj_id.Width = $_width;" -append;
	}
	if {$_height != ""} {
		ReplaceStringTag tmpJBody "WININIT" "$_obj_id.Height = $_height;" -append;
	}
	if {$_left != ""} {
		ReplaceStringTag tmpJBody "WININIT" "$_obj_id.Left = $_left;" -append;
	}
	if {$_top != ""} {
		ReplaceStringTag tmpJBody "WININIT" "$_obj_id.Top = $_top;" -append;
	}
	if {$_icon != ""} {
		ReplaceStringTag tmpJBody "WININIT" "$_obj_id.Ico = $_icon;" -append;
	}
	if {$_contentHtml != ""} {
		regsub -all {\n} $_contentHtml {} _contentHtml;
		regsub -all {\\} $_contentHtml {\\\\} _contentHtml;
		regsub -all {"} $_contentHtml {\"} _contentHtml;
		ReplaceStringTag tmpJBody "WININIT" "$_obj_id.Content = \"<table width='100%'>$_contentHtml</table>\";" -append;
	}
	ReplaceStringTag tmpJBody "WININIT" "";
	
	AppendJavaScript $tmpJBody body;
	
	return "";#[chain];
}

#-------------------------------------------------------------
# 显示窗口
#-------------------------------------------------------------
::itcl::body TLFWindow::Show {} {
	if {$_type == "popwin"} {
		return "$_obj_id.Show(\"$_obj_id\");"
	} else {
		return "$_obj_id.Show();"
	}
}

#-------------------------------------------------------------
# TLF Dialog class define
#-------------------------------------------------------------
if {[itcl::find classes TLFDialog] != "TLFDialog"} {
::itcl::class  TLFDialog {
	inherit TLFObject

	constructor {{title ""} {width ""} {height ""}} {};
	
	### data member ###
	protected variable _title;			# 标题
	protected variable _width;			# 宽度
	protected variable _height;			# 高度
	protected variable _icon;			# 图标
	
	### public methods ###
	public method  SetSize {width height};# 设置窗口大小
	public method  SetWindowIcon {ico};	# 设置窗口图标
	public method  Draw {};				# 输出对象内容
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFDialog::constructor {{title ""} {width ""} {height ""}} {
	chain;
	set _title $title;
	set _width $width;
	set _height $height;
	set _icon "";
}

#-------------------------------------------------------------
# 设置窗口大小
#-------------------------------------------------------------
::itcl::body TLFDialog::SetSize {width height} {
	set _width $width;
	set _height $height;
}

#-------------------------------------------------------------
# 设置窗口图标
#-------------------------------------------------------------
::itcl::body TLFDialog::SetWindowIcon {ico} {
	set _icon $ico;
}

#-------------------------------------------------------------
# 输出对象内容
#-------------------------------------------------------------
::itcl::body TLFDialog::Draw {} {
	global g_TemplateManager;
	
	set temp_contentHtml [chain];
	
	# 获取模板数据
	set blockDialog [g_TemplateManager GetTemplateBlock Dialog Dialog];
	set _contentHtml $blockDialog;
	
	AppendTargetTag "DISABLED" $_disabled;
	if {$_width != ""} {
		AppendTargetTag "WIDTH" "width='$_width'";
	} else {
		AppendTargetTag "WIDTH" $_width;
	}
	if {$_height != ""} {
		AppendTargetTag "HEIGHT" "height='$_height'";
	} else {
		AppendTargetTag "HEIGHT" $_height;
	}
	AppendTargetTag "TITLE" $_title;
	
	AppendTargetTag "CONTENT" $temp_contentHtml;
	
	return $_contentHtml;
}

#-------------------------------------------------------------
# TLF Portlet class define
#-------------------------------------------------------------
if {[itcl::find classes TLFPortlet] != "TLFPortlet"} {
::itcl::class  TLFPortlet {
	inherit TLFObject
}
}

#-------------------------------------------------------------
# TLFPage class define
#-------------------------------------------------------------
if {[itcl::find classes TLFPage] != "TLFPage"} {
::itcl::class  TLFPage {
	inherit TLFObject
	
	constructor {} {};
	
	### data member ###
	protected variable _templateStyle;#模板风格
	protected variable _targetHtml;	#目标页面
	protected variable _xmlSerializeFile;	#序列化用的XML文件
	protected variable _targetVarList;#存储目标页面中所有变量
	protected variable _stylePath;	#当前风格路径
	protected variable _htmlTitle;	#页面标题
	protected variable _actionArgs;	#输入参数数组
	protected variable _javaScriptFile;		#JavaScript数组(文件)
	protected variable _javaScriptCss;		#JavaScript数组(CSS引用)
	protected variable _javaScriptClass;	#JavaScript数组(类)
	protected variable _javaScriptPrototype;#JavaScript数组(属性扩展)
	protected variable _javaScriptBody;		#JavaScript数组(代码)
	
	### public methods ###
	public method  Init {targetHtml templatePath templateHtml {style "default"}};
	public method  Reset {};						#复位对象
	public method  GetStylePath {};					#获取当前风格路径
	public method  SetTitle {title};				#设置页面标题
	public method  SetMenuDisable {disable};		#设置页面是否允许右键菜单
	public method  MakeTargetHtml {};				#创建目标页面
	public method  LoadTargetHtmlVar {};			#从目标页面中读取所有变量信息
	public method  LoadFromXmlFile {};				#从XML文件中读取所有序列化信息
	public method  SaveToXmlFile {};				#写入序列化信息到XML文件
	public method  SaveTargetVar {varName varValue};#将指定变量信息写入目标缓冲区
	public method  GetTargetVar {varName};			#获取目标页面中指定变量的值
	public method  ParseActionArgs {strAction};		#解析输入参数,并执行动作函数
	public method  ProcessFormValues {};			# 处理Form结果
	public method  AppendJavaScript {jscript {type "file"}};#添加JavaScript
	public method  CreateJavaScriptDefine {};		# 生成JavaScript引用
	public method  OnPaint {};						# Paint处理函数
	public method  OnAfterPaint {};					# 页面已经显示之后调用的处理函数(仅针对解释器驻留模式)
	public method  Redirect {{url ""}};				# 页面重定向
	public method  InitialMessageFilter {};			# 初始化页面消息过滤列表
	public method  ProcessMessage {msg};			# 页面消息处理
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFPage::constructor {} {
	chain;
	SetEnableSerial false;
	set _xmlSerializeFile "";
	array set _targetVarList {};
	array set _actionArgs {};
	set _javaScriptFile {};		# 初始化JavaScript文件列表
	set _javaScriptCss {};		# 初始化JavaScript CSS文件列表
	set _javaScriptClass {};	# 初始化JavaScript类列表
	set _javaScriptPrototype {};# 初始化JavaScript对象扩展方法列表
	set _javaScriptBody {};		# 初始化JavaScript代码列表
	set _contentTag "TABLE_ALL";
}

#-------------------------------------------------------------
#	初始化
#-------------------------------------------------------------
::itcl::body TLFPage::Init {targetHtml templatePath templateHtml {style "default"}} {
	global g_tlf_mode platform_path_style platform_ui_style
	set _targetHtml $targetHtml;
	# 如果模板路径不存在,就用默认路径
	if {[file exist "$templatePath/$style"] == 0} {
		set style "default";
	}
	# 读取模板库和模板页面
	set fi [open "$templatePath/$style/$templateHtml" r];
	set _contentHtml [read $fi];
	close $fi;
	if {$g_tlf_mode == "tsf"} {
		set _stylePath "$platform_path_style/$platform_ui_style";
	} else {
		set _stylePath "$templatePath/$style";
	}
}

#-------------------------------------------------------------
# 复位对象
#-------------------------------------------------------------
::itcl::body TLFPage::Reset {} {
	chain;
	array unset _targetVarList;
	array unset _actionArgs;
	array set _targetVarList {};
	array set _actionArgs {};
	set _javaScriptFile {};		# 初始化JavaScript文件列表
	set _javaScriptCss {};		# 初始化JavaScript CSS文件列表
	set _javaScriptClass {};	# 初始化JavaScript类列表
	set _javaScriptPrototype {};# 初始化JavaScript对象扩展方法列表
	set _javaScriptBody {};		# 初始化JavaScript代码列表
}

#-------------------------------------------------------------
#	获取当前风格路径
#-------------------------------------------------------------
::itcl::body TLFPage::GetStylePath {} {
	return $_stylePath
}

#-------------------------------------------------------------
#	设置页面标题
#-------------------------------------------------------------
::itcl::body TLFPage::SetTitle {title} {
	set _htmlTitle $title
	AppendTargetTag "HTML_TITLE" $title
}

#-------------------------------------------------------------
#	设置页面是否允许右键菜单
#-------------------------------------------------------------
::itcl::body TLFPage::SetMenuDisable {disable} {
	if {$disable == "disablemenu"} {
		set menu_disable_script "<SCRIPT LANGUAGE=javascript>document.oncontextmenu = function(){return false;};</SCRIPT>"
		AppendTargetTag "MENU_DISABLE_SCRIPT" $menu_disable_script
	}	
}

#-------------------------------------------------------------
#	创建目标页面
#-------------------------------------------------------------
::itcl::body TLFPage::MakeTargetHtml {} {
	global g_tlf_mode
	
	# 设置页面语言
	LangString charset english "UTF-8"
	LangString charset chinese "GB2312"
	AppendTargetTag "PAGE_CHARSET" $charset
	# 菜单禁止脚本
	AppendTargetTag "MENU_DISABLE_SCRIPT" ""
	# 替换当前风格路径
	AppendTargetTag "STYLE_PATH" $_stylePath
	
	Draw;
	
	# 生成JavaScript的所有引用
	CreateJavaScriptDefine
	
	if {$g_tlf_mode == "tsf"} {
		return $_contentHtml;
	} else {	
		set fi [open $_targetHtml w+];
		puts $fi $_contentHtml;
		close $fi;
	}
}

#-------------------------------------------------------------
#	从目标页面中读取所有变量信息
#	变量定义形式<!-- @VarName=Value@ -->
#-------------------------------------------------------------
::itcl::body TLFPage::LoadTargetHtmlVar {} {
	global g_tlf_mode
	if {$g_tlf_mode == "tsf"} {
		return
	}
	
	catch {
		set fi [open $_targetHtml r];
		set targetHtml [read $fi];
		close $fi;
		# 解析所有变量并存储到对象的一个数组中
		set ls [regexp -all -inline -line "^<!-- @(\\\w+)=(.*?)@ -->$" $targetHtml];
		set index 0;
		set name "";
		foreach item $ls {
			if {$index%3 == 1} {
				set name $item;
			} elseif {$index%3 == 2} {
				set _targetVarList($name) $item;
			}		
			incr index;
		}
	}
}

#-------------------------------------------------------------
#	从XML文件中读取所有序列化信息
#-------------------------------------------------------------
::itcl::body TLFPage::LoadFromXmlFile {} {
	if {[GetEnableSerial] == "false"} {
		return
	}
	
	if { [ catch {
		if {$_xmlSerializeFile == ""} {
			set scriptfile [info script]
			regsub -nocase {\.tcl$} $scriptfile ".xml" _xmlSerializeFile
		}

		if {($_xmlSerializeFile != "") && [file exists $_xmlSerializeFile]} {
			set xml [xml_open file $_xmlSerializeFile]
			set nodePage [xml_get $xml node $_obj_id]
			Serialize get $xml $nodePage
			xml_close $xml
		}
	} errInfo ]} {
		puts stderr $errInfo
	}
}

#-------------------------------------------------------------
#	写入序列化信息到XML文件
#-------------------------------------------------------------
::itcl::body TLFPage::SaveToXmlFile {} {
	global platform_path
	
	if {[GetEnableSerial] == "false"} {
		return
	}
	
	if { [ catch {
		if {($_xmlSerializeFile != "") && ([file exists $_xmlSerializeFile] == 0)} {
			# 如果XML文件不存在就从模板路径拷贝一个模板过来
			file copy "$platform_path/Template/Html/serial.xml" $_xmlSerializeFile
		}
		
		if {($_xmlSerializeFile != "") && [file exists $_xmlSerializeFile]} {
			set xml [xml_open file $_xmlSerializeFile]
			set nodePage [xml_get $xml node $_obj_id]
			Serialize set $xml $nodePage
			xml_write $xml file $_xmlSerializeFile
			xml_close $xml			
		}
	} errInfo ]} {
		puts stderr $errInfo
	}
}

#-------------------------------------------------------------
#	将指定变量信息写入目标缓冲区
#-------------------------------------------------------------
::itcl::body TLFPage::SaveTargetVar {varName varValue} {
	# 存储到目标缓冲区
	set index [string first "<!-- @$varName=" $_contentHtml];
	if { $index != -1 } {
		set len [string length "<!-- @$varName="];
		incr index $len;
		set index2 [string first "@ -->" $_contentHtml [expr $index]];
		if { $index2 != -1 } {
			set _contentHtml [string replace $_contentHtml $index $index2 "$varValue@"];
		}
	}
}

#-------------------------------------------------------------
#	获取目标页面中指定变量的值
#-------------------------------------------------------------
::itcl::body TLFPage::GetTargetVar {varName} {
	set value "";
	catch {
		set a $_targetVarList($varName);
		set value $_targetVarList($varName);
	}
	return $value;
}

#-------------------------------------------------------------
#	解析输入参数,并执行动作函数
#-------------------------------------------------------------
::itcl::body TLFPage::ParseActionArgs {strAction} {
	set strAction "$strAction&"
	regsub -all {&nbsp;} $strAction " " strAction
	regsub -all {\r\n} $strAction "\n" strAction
	#puts $strAction
	set lsAction [regexp -all -inline {([\w\(\)\.\-# ]+)=([^&]*)&} $strAction]
	foreach {all name value} $lsAction {
		if {$name == "action"} {
			set _action $value
		} else {
			set _actionArgs($name) $value
			#puts [array names _actionArgs]
		}
	}
	# 返回动作函数名
	return $_action
}

#-------------------------------------------------------------
#	处理Form结果
#-------------------------------------------------------------
::itcl::body TLFPage::ProcessFormValues {} {
	#puts "++[array names _actionArgs]"
	# 处理Form参数
	foreach name [array names _actionArgs] {
		ProcessFormValue $name $_actionArgs($name)
		#puts "$name=$_actionArgs($name)"
	}
}

#-------------------------------------------------------------
# 添加JavaScript
#-------------------------------------------------------------
::itcl::body TLFPage::AppendJavaScript {jscript {type "file"}} {
	# 如果没有添加过,才添加到JavaScript列表中
	switch $type {
		file {
			if {[lsearch $_javaScriptFile $jscript] == -1} {
				lappend _javaScriptFile $jscript
			}
		}
		css {
			if {[lsearch $_javaScriptCss $jscript] == -1} {
				lappend _javaScriptCss $jscript
			}
		}
		class {
			if {[lsearch $_javaScriptClass $jscript] == -1} {
				lappend _javaScriptClass $jscript
			}
		}
		prototype {
			if {[lsearch $_javaScriptPrototype $jscript] == -1} {
				lappend _javaScriptPrototype $jscript
			}
		}
		body {
			if {[lsearch $_javaScriptBody $jscript] == -1} {
				lappend _javaScriptBody $jscript
			}
		}
	}
}

#-------------------------------------------------------------
# 生成JavaScript引用
#-------------------------------------------------------------
::itcl::body TLFPage::CreateJavaScriptDefine {} {
	# 添加文件
	foreach jfile $_javaScriptCss {
		AppendTargetTag "EXT_JSCRIPT_FILE" "<link rel=\"stylesheet\" type=\"text/css\" href=\"$jfile\" />" -append
	}
	foreach jfile $_javaScriptFile {
		AppendTargetTag "EXT_JSCRIPT_FILE" "<SCRIPT src=\"$jfile\" type=text/javascript></SCRIPT>" -append
	}
	AppendTargetTag "EXT_JSCRIPT_FILE" ""
	
	# 添加class
	AppendTargetTag "LOAD_JSCRIPT_CLASS" [join $_javaScriptClass ";"]
	
	# 添加prototype
	AppendTargetTag "LOAD_JSCRIPT_PROTOTYPE" [join $_javaScriptPrototype ";"]
	
	# 添加执行体
	foreach jbody $_javaScriptBody {
		AppendTargetTag "JSCRIPT_BODY" $jbody -append
	}
	AppendTargetTag "JSCRIPT_BODY" ""
}

#-------------------------------------------------------------
#	Paint
#-------------------------------------------------------------
::itcl::body TLFPage::OnPaint {} {
}

#-------------------------------------------------------------
#	After Paint(only exec at interp trs mode)
#-------------------------------------------------------------
::itcl::body TLFPage::OnAfterPaint {} {
}

#-------------------------------------------------------------
#	Page Redirect
#-------------------------------------------------------------
::itcl::body TLFPage::Redirect {{url ""}} {
	if {$url == ""} {
		set url $_form_action
	}
	# 使用javascript脚本设置页面的location.href属性来实现页面重定向
	set jsRelocation "window.location.href='$url'"
	regsub -all {\\} $jsRelocation "/" jsRelocation
	AppendJavaScript $jsRelocation body
}

#-------------------------------------------------------------
# 初始化页面消息过滤列表
#-------------------------------------------------------------
::itcl::body TLFPage::InitialMessageFilter {} {
}

#-------------------------------------------------------------
# 页面消息处理
#-------------------------------------------------------------
::itcl::body TLFPage::ProcessMessage {msg} {
}


#-------------------------------------------------------------
# TLFPageCreater class define
#-------------------------------------------------------------
if {[itcl::find classes TLFPageCreater] != "TLFPageCreater"} {
::itcl::class  TLFPageCreater {
	
	constructor { {title ""} } {};
	
	### data member ###
	protected variable _objname;	#页面对象
	protected variable _title;		#页面标题
	protected variable _pageClass;	#页面类名
	protected variable _template;	#页面模版文件
	protected variable _disableMenu;#禁用右键菜单
	protected variable _outFile;	#生成文件名
	protected variable _footInfo;	#页面底部信息
	protected variable _startpage;	#起始页链接
	
	### public methods ###
	public method  GetPageObj {};				#获取页面对象
	public method  SetPageClass { pageclass };	#设置页面类
	public method  SetTemplate { template };	#设置页面模版
	public method  SetMenuDisable { disable };	#设置右键菜单是否允许
	public method  SetOutFile { outfile };		#设置输出文件
	public method  SetFootInfo { footinfo };	#设置底部信息
	public method  SetStartPage { startpage };	#设置起始页链接
	public method  CreatePage {{objname "obj_#auto"}};	#创建页面
	public method  MakeHtml {};		#生成Html页面
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TLFPageCreater::constructor { {title ""} } {
	global g_tlf_obj_count
	set g_tlf_obj_count 0
	set _objname ""
	set _title $title
	set _pageClass "TLFPage"
	set _template "normal.tph"
	set _disableMenu ""
	set _outFile "index.htm"
	set _startpage ""
	# 设置版权
	#set copyright [vci_exec plat GetPlatCopyRight]
	set copyright [platform_info copyright]
	set _footInfo $copyright
}

#-------------------------------------------------------------
# 获取页面对象
#-------------------------------------------------------------
::itcl::body TLFPageCreater::GetPageObj {} {
	return $_objname
}

#-------------------------------------------------------------
# 设置页面类
#-------------------------------------------------------------
::itcl::body TLFPageCreater::SetPageClass { pageclass } {
	set _pageClass $pageclass
}

#-------------------------------------------------------------
# 设置页面模版
#-------------------------------------------------------------
::itcl::body TLFPageCreater::SetTemplate { template } {
	set _template $template
}

#-------------------------------------------------------------
# 设置右键菜单是否允许
#-------------------------------------------------------------
::itcl::body TLFPageCreater::SetMenuDisable { disable } {
	if {$disable == "true"} {
		set _disableMenu "disablemenu"
	} else {
		set _disableMenu ""
	}
}

#-------------------------------------------------------------
# 设置输出文件
#-------------------------------------------------------------
::itcl::body TLFPageCreater::SetOutFile { outfile } {
	set _outFile $outfile
}

#-------------------------------------------------------------
# 设置底部信息
#-------------------------------------------------------------
::itcl::body TLFPageCreater::SetFootInfo { footinfo } {
	set _footInfo $footinfo
}

#-------------------------------------------------------------
# 设置起始页链接
#-------------------------------------------------------------
::itcl::body TLFPageCreater::SetStartPage { startpage } {
	set _startpage $startpage
}

#-------------------------------------------------------------
# 创建页面
#-------------------------------------------------------------
::itcl::body TLFPageCreater::CreatePage {{objname "obj_#auto"}} {
	global platform_path platform_ui_style _htmlNewURL g_TemplateManager
	global argc argv platform_argv
	global g_tlf_obj_count
	
	# 创建页面对象
	set _htmlCurrentFile "$platform_path/html/$_outFile"
	if {$_objname == ""} {
		#set _objname [$_pageClass $objname]
		set _objname [$_pageClass "obj_tLFAppPage$g_tlf_obj_count"]
		incr g_tlf_obj_count
	} else {
		# 复位页面对象
		$_objname Reset
	}
	
	$_objname Init "$_htmlCurrentFile" "$platform_path/template/html" \
						$_template $platform_ui_style;# "class"
	$_objname SetObjId $_objname
	
	# 初始化页面消息过滤列表
	$objname InitialMessageFilter
	
	# 设置css和jscript
	set stylePath [$_objname GetStylePath]
	set lsCssFile [g_TemplateManager GetTemplateConstValue CSS_FILE]
	if {[llength $lsCssFile] == 1} {
		$_objname AppendTargetTag "CSS_FILE" "$stylePath/[lindex $lsCssFile 0]"
	} else {
		foreach cssFile $lsCssFile {
			set css "<LINK href=\"$stylePath/$cssFile\" type=text/css rel=stylesheet>"
			$_objname AppendTargetTag "CSS_FILES" $css -append
		}
		$_objname AppendTargetTag "CSS_FILES" ""
	}
	set lsJScriptFile [g_TemplateManager GetTemplateConstValue INCLUDE_JSCRIPT]
	if {[llength $lsJScriptFile] == 1} {
		$_objname AppendTargetTag "INCLUDE_JSCRIPT" "$stylePath/[lindex $lsJScriptFile 0]"
	} else {
		foreach jscriptFile $lsJScriptFile {
			set jscript "<SCRIPT src=\"$stylePath/$jscriptFile\" type=text/javascript></SCRIPT>"
			$_objname AppendTargetTag "INCLUDE_JSCRIPTS" $jscript -append
		}
		$_objname AppendTargetTag "INCLUDE_JSCRIPTS" ""
	}
		
	$_objname AppendTargetTag "COPYRIGHT" $_footInfo
	
	# 设置标题
	if {$_title != ""} {
		$_objname SetTitle $_title
	}
	
	# 设置StartPage
	if {$_startpage == ""} {
		set startpage [platform_info startpage]
	} else {
		set startpage $_startpage
	}
	$_objname AppendTargetTag "STARTPAGE" $startpage
	
	# 设置禁止右键菜单标记
	$_objname SetMenuDisable $_disableMenu

	# 设置页面位置
	set _htmlNewURL "$platform_path/html/$_outFile";
	# 创建
	$_objname OnCreate;
	# 加载XML配置文件
	$_objname LoadFromXmlFile;
	# 创建之后
	$_objname OnAfterCreate;
	if {$argc >= 1} {
		set actionArgs $platform_argv
	} else {
		set actionArgs "action=OnPaint"
	}
	#puts $actionArgs
	set actionResult true
	set action [$_objname ParseActionArgs $actionArgs]
	if {$action == ""} {
		set action "OnPaint"
	}
	$_objname ProcessFormValues
	if {[$_objname GetAction] != ""} {
		set action [$_objname GetAction]
	}
	#puts "---$action"
	# 页面初始化
	$_objname OnInitialUpdate;

	# 执行相应动作的函数
	if {[catch {
			set actionResult [$_objname $action]
		} errInfo]} {
		puts stderr $errInfo
	}
	
	# 动作执行完后的操作
	$_objname OnEndAction;

	# 如果执行成功,就保存XML配置文件
	if {$actionResult != "false"} {
		$_objname SaveToXmlFile
	}

	return $actionResult
}

#-------------------------------------------------------------
# 生成Html页面
#-------------------------------------------------------------
::itcl::body TLFPageCreater::MakeHtml {} {
	set actionResult [CreatePage]
	# 生成Html文件
	if {$actionResult != "false"} {
		$_objname MakeTargetHtml
	}
}


#-------------------------------------------------------------
#	创建一个TLF页面
#-------------------------------------------------------------
proc TLFCreatePage {{title ""} {page_class "TLFPage"} {template_file "normal.tph"} {disable_menu ""} {startpage ""} {makehtml "true"} {outfile "index.htm"} {footinfo ""}} {
	global platform_path platform_ui_style _htmlNewURL g_TemplateManager
	global argc argv platform_argv
	global g_tlf_obj_count g_tlf_mode
	
	# 复位TLF对象名计数器
	set g_tlf_obj_count 0
	
	if {$page_class == ""} {
		set page_class "TLFPage"
	}
	
	if {$template_file == ""} {
		set template_file "normal.tph"
	}
	
	if {$outfile == ""} {
		set outfile "index.htm"
	}
	
	# 创建页面对象
	set _htmlCurrentFile "$platform_path/html/$outfile"
	
	# 如果已经创建过页面对象,就使用已经创建的对象
	set lsObjPage [itcl::find objects -class $page_class]
	if {[llength $lsObjPage] > 0} {
		set objname [lindex $lsObjPage 0]
		# 复位页面对象
		$objname Reset
	} else {
		#set objname [$page_class "obj_#auto"]
		#set objname [$page_class "obj_tlf_$g_tlf_obj_count"]
		set objname [$page_class "obj_tLFAppPage$g_tlf_obj_count"]
		incr g_tlf_obj_count
	}

	$objname Init "$_htmlCurrentFile" "$platform_path/template/html" \
						$template_file $platform_ui_style;# "class"
	$objname SetObjId $objname
	
	# 初始化页面消息过滤列表
	$objname InitialMessageFilter
	
	# 设置css和jscript
	set stylePath [$objname GetStylePath]
	set lsCssFile [g_TemplateManager GetTemplateConstValue CSS_FILE]
	if {[llength $lsCssFile] == 1} {
		$objname AppendTargetTag "CSS_FILE" "$stylePath/[lindex $lsCssFile 0]"
	} else {
		foreach cssFile $lsCssFile {
			set css "<LINK href=\"$stylePath/$cssFile\" type=text/css rel=stylesheet>"
			$objname AppendTargetTag "CSS_FILES" $css -append
		}
		$objname AppendTargetTag "CSS_FILES" ""
	}
	set lsJScriptFile [g_TemplateManager GetTemplateConstValue INCLUDE_JSCRIPT]
	if {[llength $lsJScriptFile] == 1} {
		$objname AppendTargetTag "INCLUDE_JSCRIPT" "$stylePath/[lindex $lsJScriptFile 0]"
	} else {
		foreach jscriptFile $lsJScriptFile {
			set jscript "<SCRIPT src=\"$stylePath/$jscriptFile\" type=text/javascript></SCRIPT>"
			$objname AppendTargetTag "INCLUDE_JSCRIPTS" $jscript -append
		}
		$objname AppendTargetTag "INCLUDE_JSCRIPTS" ""
	}
	
	# 设置版权
	#set copyright [vci_exec plat GetPlatCopyRight]
	set copyright [platform_info copyright]

	if {$footinfo != ""} {
		set copyright $footinfo
	}
		
	$objname AppendTargetTag "COPYRIGHT" $copyright
	
	# 设置标题
	if {$title != ""} {
		$objname SetTitle $title
	}
	
	# 设置StartPage
	if {$startpage == ""} {
		set startpage [platform_info startpage]
	}
	$objname AppendTargetTag "STARTPAGE" $startpage
	
	# 设置禁止右键菜单标记
	$objname SetMenuDisable $disable_menu

	# 设置页面位置
	set _htmlNewURL "$platform_path/html/$outfile";
	# 创建
	$objname OnCreate;
	# 加载XML配置文件
	$objname LoadFromXmlFile;
	# 创建之后
	$objname OnAfterCreate;
	if {$argc >= 1} {
		#set actionArgs [lindex $argv 0]
		set actionArgs $platform_argv
	} else {
		set actionArgs "action=OnPaint"
	}
	#puts $actionArgs
	
	set actionResult true
	set action [$objname ParseActionArgs $actionArgs]
	if {$action == ""} {
		set action "OnPaint"
	}
	$objname ProcessFormValues
	if {[$objname GetAction] != ""} {
		set action [$objname GetAction]
	}
	#puts "---$action"
	
	# 页面初始化
	$objname OnInitialUpdate;

	# 执行相应动作的函数
	if {[catch {
			set actionResult [$objname $action]
		} errInfo]} {
		puts stderr $errInfo
	}
	
	# 动作执行完后的操作
	$objname OnEndAction;

	# 如果执行成功,就保存XML配置文件
	if {$actionResult != "false"} {
		$objname SaveToXmlFile
	}

	# 生成Html文件
	if {($makehtml == "true") && ($actionResult != "false") && ($g_tlf_mode == "tlf")} {
		$objname MakeTargetHtml
	}

	return $objname;
}

#-------------------------------------------------------------
#	显示Html页面
#-------------------------------------------------------------
proc ShowHtmlPage { {url ""} } {
	global platform_path
	
	if {$url == ""} {
		set url "$platform_path/html/index.htm"
	}
	
	owm -sendcmd "org.owm.htmlview" 84 "<string>$url" 0
}