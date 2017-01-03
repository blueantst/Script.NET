#################################################################
# File       : filemng.tcl
# Author     : Blueant Software
# Version    : 1.0
# Date       : 2001-02-04
# Description: Script.NET file manager script
#################################################################

#-------------------------------------------------------------
# ID关系:
# ID=1	: 根节点 
# ID=2	: 文件节点
# ID=3	: proc/def节点
# ID=4	: itcl类/class节点
# ID=5	: itcl成员函数/class def节点
# ID=6	: itcl成员变量节点
# ID=10	: VC dsp工程
# ID=11	: VC dsp Group
# ID=12	: VC dsp File
#-------------------------------------------------------------

#-------------------------------------------------------------
# 创建文件树页面
#-------------------------------------------------------------
proc _plat_Tcl_CreateFileTree { } {
	global platform_language
	
#	if {[WorkTree treeexist 10006] == 0} {
		# 如果树不存在,就退出
#		return
#	}
	
	# 创建树
	LangStringEC STR_TREE_NAME "File" "文件"
	set tree [WorkTree gettreeid $STR_TREE_NAME]
	if {$tree == -1} {
		set tree [WorkTree addtree 6 $STR_TREE_NAME]
		return
	}
	#WorkTree closetree $STR_TREE_NAME
	if {$tree == -1} {
		return
	}
	
	# 创建根节点
	set hRootItem [WorkTree additem $tree ROOT "WorkSpace files" -image(0) -id(1) -type(1) -bold(true)];
	WorkTree expanditem $tree $hRootItem -TIM_ITEM;

	# 节点双击的脚本,必须是一个TCL过程,且有3个参数,分别为树ID,节点句柄,节点ID
	set dclickscript {
		proc dclick {tree item id} {
			set ItemName [WorkTree getitem name $tree $item -TIM_ITEM];
			set ItemInfo [WorkTree getitem info $tree $item -TIM_ITEM];
			#puts "item=$item,id=$id,ItemName=$ItemName,ItemInfo=$ItemInfo";

			# 根节点则不继续处理
			if {$id == -1 || $id == 1 || $id == 2} {
				return;
			} elseif {$id == 12} {				
				#set res [owm -sendcmd "org.owm.edit" 102 "<string>$ItemInfo" 0];
				set res [owm -sendcmd "" 86 "<string>$ItemInfo" "<string>"];
				return;
			}

			set hItem $item;
			while {[WorkTree getitem type $tree $hItem -TIM_ITEM] != 2 && $hItem != 0} {
				set hItem [WorkTree getitem parent $tree $hItem -TIM_ITEM];
			}
			set filename [WorkTree getitem info $tree $hItem -TIM_ITEM];
			
			# 让指定文件跳转到指定的行
			regsub -all {/} $filename {\\} filename;
			# 激活窗口
			owm -sendcmd "org.owm.edit" 102 "<string>$filename" 0;
			# 查找到指定的行并设置为当前行
			owm -sendcmd "org.owm.edit" 113 "<string>$filename" "<string>$ItemInfo";
		};dclick}

	WorkTree setdclickscript $tree $dclickscript;
}

#-------------------------------------------------------------
# 关闭文件树页面
#-------------------------------------------------------------
proc _plat_Tcl_CloseFileTree { } {
	global platform_language
	
	if {$platform_language == "chinese"} {
		WorkTree closetree "文件";
	} else {
		WorkTree closetree "File";
	}
}

#-------------------------------------------------------------
# 刷新文件节点(Tcl文件)
#-------------------------------------------------------------
proc _plat_Tcl_RefreshTclFile {filename} {
	global platform_language
	
	# 获取文件树ID
	if {$platform_language == "chinese"} {
		set tree [WorkTree gettreeid "文件"];
	} else {
		set tree [WorkTree gettreeid "File"];
	}

	set hItem [WorkTree searchitem $tree $filename];

	if {$hItem == 0} {
		set name [file tail $filename];
		set hItem [WorkTree additem $tree 1 "$name" -TIM_ID -id(2) -image(18) -type(2) -info($filename)];
		WorkTree expanditem $tree 1;
	} else {
		WorkTree delitem $tree $hItem -TIM_ITEM -TIM_CHILDONLY;
	}

	# 打开文件进行函数和类分析
	set fi [open $filename r];
	set buf [read $fi];

	# 解析和生成iTcl类和成员函数节点
	set ls [regexp -all -inline -lineanchor {^[ \t]*(?:::itcl::)*class[ \t]+(\w+)[ \t]+\{[ \t]*$} $buf];
	foreach {iline iclass} $ls {
		set hItem1 [WorkTree additem $tree $hItem "$iclass" -id(4) -image(4) -type(4) -info($iline)];
		# 解析itcl类的成员函数
		set exp "^\[ \t\]*(?:::itcl::)*body\[ \t\]+$iclass\::\(\[\\w:\]+\)\[ \t\]+\{.*\}\[ \t\]+\{\[ \t\]*\$";
		set lsMethod [regexp -all -inline -line $exp $buf];
		foreach {mline mname} $lsMethod {
			WorkTree additem $tree $hItem1 "$mname" -id(5) -image(5) -type(5) -info($mline);
		}
	}
	
#	set ls [regexp -all -inline -lineanchor {^[ \t]*::itcl::class[ \t]+(\w+)[ \t]+\{[ \t]*$} $buf];
#	foreach {iline iclass} $ls {
#		set hItem1 [WorkTree additem $tree $hItem "$iclass" -id(4) -image(4) -type(4) -info($iline)];
#		# 解析itcl类的成员函数
#		set exp "^\[ \t\]*::itcl::body\[ \t\]+$iclass\::\(\[\\w:\]+\)\[ \t\]+\{.*\}\[ \t\]+\{\[ \t\]*\$";
#		set lsMethod [regexp -all -inline -line $exp $buf];
#		foreach {mline mname} $lsMethod {
#			WorkTree additem $tree $hItem1 "$mname" -id(5) -image(5) -type(5) -info($mline);
#		}
#	}

	# 解析和生成函数节点
	set ls [regexp -all -inline -line {^[ \t]*proc[ \t]+([\w:]+)[ \t]+\{.*\}[ \t]+\{[ \t]*$} $buf];
	foreach {pline pname} $ls {
		WorkTree additem $tree $hItem "$pname" -id(3) -image(7) -type(3) -info($pline);
	}

	close $fi;
}

#-------------------------------------------------------------
# 刷新文件节点(Python文件)
#-------------------------------------------------------------
proc _plat_Tcl_RefreshPythonFile {filename} {
	global platform_language
	
	# 获取文件树ID
	if {$platform_language == "chinese"} {
		set tree [WorkTree gettreeid "文件"];
	} else {
		set tree [WorkTree gettreeid "File"];
	}

	set hItem [WorkTree searchitem $tree $filename];

	if {$hItem == 0} {
		set name [file tail $filename];
		set hItem [WorkTree additem $tree 1 "$name" -TIM_ID -id(2) -image(19) -type(2) -info($filename)];
		WorkTree expanditem $tree 1;
	} else {
		WorkTree delitem $tree $hItem -TIM_ITEM -TIM_CHILDONLY;
	}

	# 打开文件进行函数和类分析
	set fi [open $filename r];
	set level 0;
	set hPItem $hItem;	# 父节点
	set hLItem $hItem;	# 上一次操作的节点
	while {[gets $fi line] >= 0} {
		if {[regexp {(\s*)(def\s+(\w+)\(.*|class\s+(\w+)(\(.*\))?\s*:)} $line all space ldef defname classname classderive] > 0} {
			# 分析缩进层次
			regsub -all {\t} $space {    } space;
			set newlevel [expr [string length $space] / 4];
			
			if {$newlevel > $level} {
				# 创建子节点
				set hPItem $hLItem;
			} elseif {$newlevel < $level} {
				# 遍历到上一级节点
				while {$newlevel != $level && $hPItem != $hItem} {
					set hPItem [WorkTree getitem parent $tree $hPItem -TIM_ITEM];
					incr level -1;
				}
			}
			
			# 生成节点
			if {$defname != ""} {
				if {$newlevel > 0} {
					set id 5;
				} else {
					set id 3;
				}
				set hItem1 [WorkTree additem $tree $hPItem "$defname" -id($id) -image(5) -type($id) -info($line)];
			} else {
				set hItem1 [WorkTree additem $tree $hPItem "$classname" -id(4) -image(4) -type(4) -info($line)];
			}
			
			set hLItem $hItem1;			
			set level $newlevel;
		}
	}

	close $fi;
}

#-------------------------------------------------------------
# 刷新文件节点(Perl文件)
#-------------------------------------------------------------
proc _plat_Tcl_RefreshPerlFile {filename} {
	global platform_language
	
	# 获取文件树ID
	if {$platform_language == "chinese"} {
		set tree [WorkTree gettreeid "文件"];
	} else {
		set tree [WorkTree gettreeid "File"];
	}

	set hItem [WorkTree searchitem $tree $filename];

	if {$hItem == 0} {
		set name [file tail $filename];
		set hItem [WorkTree additem $tree 1 "$name" -TIM_ID -id(2) -image(21) -type(2) -info($filename)];
		WorkTree expanditem $tree 1;
	} else {
		WorkTree delitem $tree $hItem -TIM_ITEM -TIM_CHILDONLY;
	}

	# 打开文件进行函数和类分析
	set fi [open $filename r];
	set buf [read $fi];

	# 解析和生成函数节点
	set ls [regexp -all -inline -line {^[ \t]*sub[ \t]+([\w:]+)[ \t]?.*$} $buf];
	foreach {pline pname} $ls {
		WorkTree additem $tree $hItem "$pname" -id(3) -image(7) -type(3) -info($pline);
	}

	close $fi;
}

#-------------------------------------------------------------
# 刷新文件节点(Ruby文件)
#-------------------------------------------------------------
proc _plat_Tcl_RefreshRubyFile {filename} {
	global platform_language
	
	# 获取文件树ID
	if {$platform_language == "chinese"} {
		set tree [WorkTree gettreeid "文件"];
	} else {
		set tree [WorkTree gettreeid "File"];
	}

	set hItem [WorkTree searchitem $tree $filename];

	if {$hItem == 0} {
		set name [file tail $filename];
		set hItem [WorkTree additem $tree 1 "$name" -TIM_ID -id(2) -image(22) -type(2) -info($filename)];
		WorkTree expanditem $tree 1;
	} else {
		WorkTree delitem $tree $hItem -TIM_ITEM -TIM_CHILDONLY;
	}

	# 打开文件进行函数和类分析
	set fi [open $filename r];
	set level 0;
	set hPItem $hItem;	# 父节点
	set hLItem $hItem;	# 上一次操作的节点
	while {[gets $fi line] >= 0} {
		if {[regexp {^(\s*)(def\s+(\w+).*|class\s+(\w+).*)} $line all space ldef defname classname] > 0} {
			# 分析缩进层次
			regsub -all {\t} $space {    } space;
			set newlevel [expr [string length $space] / 4];
			
			if {$newlevel > $level} {
				# 创建子节点
				set hPItem $hLItem;
			} elseif {$newlevel < $level} {
				# 遍历到上一级节点
				while {$newlevel != $level && $hPItem != $hItem} {
					set hPItem [WorkTree getitem parent $tree $hPItem -TIM_ITEM];
					incr level -1;
				}
			}
			
			# 生成节点
			if {$defname != ""} {
				if {$newlevel > 0} {
					set id 5;
				} else {
					set id 3;
				}
				set hItem1 [WorkTree additem $tree $hPItem "$defname" -id($id) -image(5) -type($id) -info($line)];
			} else {
				set hItem1 [WorkTree additem $tree $hPItem "$classname" -id(4) -image(4) -type(4) -info($line)];
			}
			
			set hLItem $hItem1;			
			set level $newlevel;
		}
	}

	close $fi;
}

#-------------------------------------------------------------
# 刷新文件节点(Lua文件)
#-------------------------------------------------------------
proc _plat_Tcl_RefreshLuaFile {filename} {
	global platform_language
	
	# 获取文件树ID
	if {$platform_language == "chinese"} {
		set tree [WorkTree gettreeid "文件"];
	} else {
		set tree [WorkTree gettreeid "File"];
	}

	set hItem [WorkTree searchitem $tree $filename];

	if {$hItem == 0} {
		set name [file tail $filename];
		set hItem [WorkTree additem $tree 1 "$name" -TIM_ID -id(2) -image(20) -type(2) -info($filename)];
		WorkTree expanditem $tree 1;
	} else {
		WorkTree delitem $tree $hItem -TIM_ITEM -TIM_CHILDONLY;
	}

	# 打开文件进行函数和类分析
	set fi [open $filename r];
	set buf [read $fi];

	# 解析和生成函数节点
	set ls [regexp -all -inline -line {^[ \t]*function[ \t]+([\w:\.]+)\s*\(.*?\).*$} $buf];
	foreach {pline pname} $ls {
		WorkTree additem $tree $hItem "$pname" -id(3) -image(7) -type(3) -info($pline);
	}

	close $fi;
}

#-------------------------------------------------------------
# 刷新文件节点(Java文件)
#-------------------------------------------------------------
proc _plat_Tcl_RefreshJavaFile {filename} {
	global platform_language
	
	# 获取文件树ID
	if {$platform_language == "chinese"} {
		set tree [WorkTree gettreeid "文件"];
	} else {
		set tree [WorkTree gettreeid "File"];
	}

	set hItem [WorkTree searchitem $tree $filename];

	if {$hItem == 0} {
		set name [file tail $filename];
		set hItem [WorkTree additem $tree 1 "$name" -TIM_ID -id(2) -image(17) -type(2) -info($filename)];
		WorkTree expanditem $tree 1;
	} else {
		WorkTree delitem $tree $hItem -TIM_ITEM -TIM_CHILDONLY;
	}

	# 打开文件进行函数和类分析
	set fi [open $filename r];
	set buf [read $fi];

	# 解析和生成iTcl类和成员函数节点
	set ls [regexp -all -inline -lineanchor {^[ \t]*public[ \t]+class[ \t]+(\w+)[ \t]+.*$} $buf];
	foreach {iline iclass} $ls {
		set hItem1 [WorkTree additem $tree $hItem "$iclass" -id(4) -image(4) -type(4) -info($iline)];
		# 解析类的成员函数
		#set exp "^\[ \t\]*::itcl::body\[ \t\]+$iclass\::\(\[\\w:\]+\)\[ \t\]+\{.*\}\[ \t\]+\{\[ \t\]*\$";
		#set lsMethod [regexp -all -inline -line $exp $buf];
		#foreach {mline mname} $lsMethod {
		#	WorkTree additem $tree $hItem1 "$mname" -id(5) -image(5) -type(5) -info($mline);
		#}
	}

	close $fi;
}

#-------------------------------------------------------------
# 刷新文件节点(JavaScript文件)
#-------------------------------------------------------------
proc _plat_Tcl_RefreshJavaScriptFile {filename} {
	global platform_language
	
	# 获取文件树ID
	if {$platform_language == "chinese"} {
		set tree [WorkTree gettreeid "文件"];
	} else {
		set tree [WorkTree gettreeid "File"];
	}

	set hItem [WorkTree searchitem $tree $filename];

	if {$hItem == 0} {
		set name [file tail $filename];
		set hItem [WorkTree additem $tree 1 "$name" -TIM_ID -id(2) -image(23) -type(2) -info($filename)];
		WorkTree expanditem $tree 1;
	} else {
		WorkTree delitem $tree $hItem -TIM_ITEM -TIM_CHILDONLY;
	}

	# 打开文件进行函数和类分析
	set fi [open $filename r];
	set buf [read $fi];

	# 解析和生成函数节点
	set ls [regexp -all -inline -line {^[ \t]*function[ \t]+([\w:]+)\s*\(.*?\).*$} $buf];
	foreach {pline pname} $ls {
		WorkTree additem $tree $hItem "$pname" -id(3) -image(7) -type(3) -info($pline);
	}

	close $fi;
}

#-------------------------------------------------------------
# 刷新文件节点(NSIS文件)
#-------------------------------------------------------------
proc _plat_Tcl_RefreshNsisFile {filename} {
	global platform_language
	
	# 获取文件树ID
	if {$platform_language == "chinese"} {
		set tree [WorkTree gettreeid "文件"];
	} else {
		set tree [WorkTree gettreeid "File"];
	}

	set hItem [WorkTree searchitem $tree $filename];

	if {$hItem == 0} {
		set name [file tail $filename];
		set hItem [WorkTree additem $tree 1 "$name" -TIM_ID -id(2) -image(42) -type(2) -info($filename)];
		WorkTree expanditem $tree 1;
	} else {
		WorkTree delitem $tree $hItem -TIM_ITEM -TIM_CHILDONLY;
	}

	# 打开文件进行函数和类分析
	set fi [open $filename r];
	set buf [read $fi];

	# 解析和生成函数节点
	set ls [regexp -all -inline -line {^[ \t]*Function[ \t]+"*([\w:.]+)"*[ \t]*.*?$} $buf];
	foreach {pline pname} $ls {
		WorkTree additem $tree $hItem "$pname" -id(3) -image(29) -type(3) -info($pline);
	}
	
	# 解析和生成macro节点
	set ls [regexp -all -inline -line {^[ \t]*!macro[ \t]+([\w:]+)[ \t]+.*?$} $buf];
	foreach {pline pname} $ls {
		WorkTree additem $tree $hItem "$pname" -id(4) -image(7) -type(4) -info($pline);
	}

	# 解析和生成区段组节点
	set ls [regexp -all -inline -line {^[ \t]*SectionGroup[ \t]+(?:/e[ \t]+)*("(.*?)"|[^ \t]+)[ \t]*.*?$} $buf];
	foreach {pline pname1 pname2} $ls {
		if {$pname2 != ""} {
			set pname $pname2
		} else {
			set pname $pname1
		}
		WorkTree additem $tree $hItem "$pname" -id(5) -image(44) -type(5) -info($pline);
	}

	# 解析和生成区段节点
	set ls [regexp -all -inline -line {^[ \t]*Section[ \t]+(?:/o[ \t]+)*("(.*?)"|[^ \t]+)[ \t]*.*?$} $buf];
	foreach {pline pname1 pname2} $ls {
		if {$pname2 != ""} {
			set pname $pname2
		} else {
			set pname $pname1
		}
		
		set pname [string trimleft $pname]
		if {[string first ";" $pname] == 0} {
			set pname "{Hide Section}"
		} elseif {$pname == {""}} {
			set pname "{Hide Section}"
		}
		WorkTree additem $tree $hItem "$pname" -id(5) -image(28) -type(5) -info($pline);
	}

	close $fi;
}

#-------------------------------------------------------------
# 刷新文件节点(.H文件)
#-------------------------------------------------------------
proc _plat_Tcl_RefreshHFile {filename} {
	global platform_language
	
	# 获取文件树ID
	if {$platform_language == "chinese"} {
		set tree [WorkTree gettreeid "文件"];
	} else {
		set tree [WorkTree gettreeid "File"];
	}

	set hItem [WorkTree searchitem $tree $filename];

	if {$hItem == 0} {
		set name [file tail $filename];
		set hItem [WorkTree additem $tree 1 "$name" -TIM_ID -id(2) -image(17) -type(2) -info($filename)];
		WorkTree expanditem $tree 1;
	} else {
		WorkTree delitem $tree $hItem -TIM_ITEM -TIM_CHILDONLY;
	}

	# 打开文件进行函数和类分析
	set fi [open $filename r];
	set buf [read $fi];

	# 解析和生成类和成员函数节点
	set ls [regexp -all -inline -lineanchor {[ \t]*class[ \t]+(\w+)[ \t]*.*?;} $buf];
	foreach {iline iclass} $ls {
		set hItem1 [WorkTree additem $tree $hItem "$iclass" -id(4) -image(4) -type(4) -info($iline)];
		# 解析类的成员函数
		set exp ".*?[ \t]+(\w+)[ \t]*\(.+?\)[ \t]*;";
		set lsMethod [regexp -all -inline -line $exp $buf];
		foreach {mline mname} $lsMethod {
			WorkTree additem $tree $hItem1 "$mname" -id(5) -image(5) -type(5) -info($mline);
		}
	}

	close $fi;
}

#-------------------------------------------------------------
# 刷新文件节点
#-------------------------------------------------------------
proc _plat_Tcl_RefreshFile {opr filename} {
	global platform_language
	
	# 获取文件树ID
	if {$platform_language == "chinese"} {
		set tree [WorkTree gettreeid "文件"];
	} else {
		set tree [WorkTree gettreeid "File"];
	}

	set hItem [WorkTree searchitem $tree $filename];

	# 关闭文件操作
	if {$opr == "close"} {
		if {$hItem != 0 && $filename != ""} {
			WorkTree delitem $tree $hItem -TIM_ITEM;
		}
		return;
	}
	
	# 获取文件后缀
	set ext [string tolower [file extension $filename]];
	
	# TCL文件的处理
	if {$ext == ".tcl" || $ext == ".tk" || $ext == ".itcl"} {
		_plat_Tcl_RefreshTclFile $filename;
	} elseif {$ext == ".py" || $ext == ".pyw"} {
		_plat_Tcl_RefreshPythonFile $filename;
	} elseif {$ext == ".pl" || $ext == ".pm"} {
		_plat_Tcl_RefreshPerlFile $filename;
	} elseif {$ext == ".rb" || $ext == ".rbw"} {
		_plat_Tcl_RefreshRubyFile $filename;
	} elseif {$ext == ".lua"} {
		_plat_Tcl_RefreshLuaFile $filename;
	} elseif {$ext == ".java"} {
		_plat_Tcl_RefreshJavaFile $filename;
	} elseif {$ext == ".js"} {
		_plat_Tcl_RefreshJavaScriptFile $filename;
	} elseif {$ext == ".nsh" || $ext == ".nsi"} {
		_plat_Tcl_RefreshNsisFile $filename;
	} elseif {$ext == ".h" || $ext == ".hpp"} {
		#_plat_Tcl_RefreshHFile $filename;
	}
}

#-------------------------------------------------------------
# 解析VC工程
#-------------------------------------------------------------
proc _plat_Tcl_ParseVCDsp {opr filename} {
	global platform_language
	
	# 检查文件后缀是否为dsp工程文件
	set ext [string tolower [file extension $filename]];
	if {$ext != ".dsp"} {
		return;
	}
	set filedir [file dirname $filename];

	# 获取文件树ID
	if {$platform_language == "chinese"} {
		set tree [WorkTree gettreeid "文件"];
	} else {
		set tree [WorkTree gettreeid "File"];
	}
	if {$tree == -1} {
		_plat_Tcl_CreateFileTree;
		if {$platform_language == "chinese"} {
			set tree [WorkTree gettreeid "文件"];
		} else {
			set tree [WorkTree gettreeid "File"];
		}
	}

	set hItem [WorkTree searchitem $tree $filename];

	# 关闭文件操作
	if {$opr == "close"} {
		if {$hItem != 0} {
			WorkTree delitem $tree $hItem -TIM_ITEM;
		}
		return;
	}

	# 打开文件
	set fi [open $filename r];
	
	if {$hItem == 0} {
		set name [file tail $filename];
		set hItem [WorkTree additem $tree 1 "$name" -TIM_ID -id(10) -image(1) -type(10) -info($filename)];
		WorkTree expanditem $tree 1;
	} else {
		WorkTree delitem $tree $hItem -TIM_ITEM -TIM_CHILDONLY;
	}

	# 显示进度条
	#set nPaneID [StatusBar -createpane -width 150];
	#StatusBar -progress -Attach $nPaneID "" 100;
	pwait 1;
	
	# 函数和类分析
	set buf [read $fi];
	close $fi;
	#StatusBar -progress -Pos $nPaneID 10;

	# 解析Target段
	regexp {# Begin Target(.*)# End Target} $buf all buf;
	#StatusBar -progress -Pos $nPaneID 20;
	
	# 解析Group
	set lsGroup [regexp -all -inline {# Begin Group \"(.*?)\"(.*?)# End Group} $buf];
	#StatusBar -progress -Pos $nPaneID 50;
	set nGroupCount [llength $lsGroup];
	set i 1;
	foreach {all groupname group} $lsGroup {
		set hItem1 [WorkTree additem $tree $hItem "$groupname" -id(11) -image(2) -type(11) -info($groupname)];
		set lsFile [regexp -all -inline -line {^SOURCE=\"?(.*?)\"?$} $group];
		foreach {allfile sfile} $lsFile {
			set filepath [file join $filedir $sfile];			
			set filetail [file tail $filepath];
			set ext [string tolower [file extension $filepath]];
			set fileimage 23
			switch $ext {
				".cpp" -
				".cxx" -
				".c" {set fileimage 16}
				".hpp" -
				".h" {set fileimage 17}
				".ico" -
				".bmp" {set fileimage 11}
			}
			WorkTree additem $tree $hItem1 $filetail -id(12) -image($fileimage) -type(12) -info($filepath);			
		}
		#StatusBar -progress -Pos $nPaneID [expr 50+$i*50/$nGroupCount];
		incr i;
	}
	
	# 隐藏进度条
	#StatusBar -progress -Hide $nPaneID;
	#StatusBar -delpane $nPaneID;
}

#-------------------------------------------------------------
# 主程序
#-------------------------------------------------------------
if {$argc > 0} {
	set opr [lindex $argv 0];
	if {$opr == "CreateFileTree"} {
		_plat_Tcl_CreateFileTree;
	} elseif {$opr == "CloseFileTree"} {
		_plat_Tcl_CloseFileTree;
	} elseif {$opr == "RefreshFile"} {
		if {$argc > 1} {
			_plat_Tcl_RefreshFile [lindex $argv 1] [lindex $argv 2];
		}
	}
}
