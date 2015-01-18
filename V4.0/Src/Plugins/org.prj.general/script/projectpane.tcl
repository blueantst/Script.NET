#################################################################
# projectpane.tcl
# Author     : blueant
# Version    : 4.0
# Date       : 2015-01-18
# Description: General project pane
#################################################################

package require Itcl

catch {namespace import itcl::*}

namespace eval prjdef {
# 页面类型
set PGITYPE_FILE		0
set PGITYPE_TEST		1
set PGITYPE_CUSTOM		2
set PGITYPE_TASKPANEL	3

set TREE_PROJECT_ID		10008					;# Project页面的ID

# 图片索引
set ILINK_PROJECT		0						;# 根节点
}

#-------------------------------------------------------------
# ID关系:
# ID=1	: 根节点(工程)
# ID=2	: 脚本文件
# ID=3	: 文件节点
#-------------------------------------------------------------

#-------------------------------------------------------------
# 创建工程树页面
#-------------------------------------------------------------
proc ProjectCreateProjectTree { } {
	global platform_language
	
	if {[WorkTree treeexist $prjdef::TREE_PROJECT_ID] == 0} {
		# 如果树不存在,就退出
		return
	}
	
	# 创建树
	LangStringEC STR_TREE_NAME "Project" "工程"
	set tree [WorkTree gettreeid $STR_TREE_NAME]
	if {$tree == -1} {
		return
	}

	# 创建根节点
	set hRootItem [WorkTree searchitem $tree "###PROJECT_ROOT###"]
	if {$hRootItem == 0} {
		set hRootItem [WorkTree additem $tree ROOT $STR_TREE_NAME -image(0) -id(1) -type(1) -bold(true) -info(###PROJECT_ROOT###)];

		# 节点双击的脚本,必须是一个TCL过程,且有3个参数,分别为树ID,节点句柄,节点ID
		set dclickscript {
			proc dclick {tree item id} {
				set ItemName [WorkTree getitem name $tree $item -TIM_ITEM];
				set ItemInfo [WorkTree getitem info $tree $item -TIM_ITEM];
				#puts "item=$item,id=$id,ItemName=$ItemName,ItemInfo=$ItemInfo";
				
				if {$id != 3} {
					return
				}
				
				set filename [WorkTree getitem info $tree $item -TIM_ITEM]
				
				# 让指定文件跳转到指定的行
				regsub -all {/} $filename {\\} filename
				# 激活窗口
				owm -sendcmd "org.owm.edit" 102 "<string>$filename" 0
				
			};dclick}
	
		WorkTree setdclickscript $tree $dclickscript;
	} else {
		#set vciprj [vci_exec plat GetObjectByInstanceName "###project###"]
		#set tclvciprj [vci_info -gettclvciobj "org.prj.general" $vciprj]
		#set project_name ""
		#vci_exec $tclvciprj GetActiveProject project_name
		#set project_name [pproject name]
		if {[pproject isopen] == 0} {
			# 工程已关闭
			WorkTree delitem $tree $hRootItem -TIM_ITEM -TIM_CHILDONLY
			return
		}
	}
	
	# 添加脚本文件节点
	set hItemScripts [WorkTree searchitem $tree "###SCRIPTFILE_ROOT###"]
	if {$hItemScripts != 0} {
		WorkTree delitem $tree $hRootItem -TIM_ITEM -TIM_CHILDONLY
	}
	set hItemScripts [WorkTree additem $tree $hRootItem "Script Files" -id(2) -image(4) -type(2) -info(###SCRIPTFILE_ROOT###)]
	
	if {$hItemScripts != 0} {
		set project_path [pproject path]
		set lsFile [glob -path "$project_path\\" "*.{tcl,py,pl,lua,rb}"]
		
		# 将所有脚本文件加入到文件节点下面
		foreach filename $lsFile {
			set name [file tail $filename]
			WorkTree additem $tree $hItemScripts $name -id(3) -image(5) -type(3) -info($filename)
		}
	}
	
	WorkTree expanditem $tree $hRootItem -TIM_ITEM
	WorkTree expanditem $tree $hItemScripts -TIM_ITEM
}

#-------------------------------------------------------------
# Main
#-------------------------------------------------------------
ProjectCreateProjectTree
