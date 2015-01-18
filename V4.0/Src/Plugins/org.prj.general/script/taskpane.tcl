#################################################################
# taskpane.tcl
# Author     : blueant
# Version    : 4.0
# Date       : 2015-01-18
# Description: General task pane
#################################################################

package require Itcl

catch {namespace import itcl::*}

namespace eval prjdef {
# 页面类型
set PGITYPE_FILE		0
set PGITYPE_TEST		1
set PGITYPE_CUSTOM		2
set PGITYPE_TASKPANEL	3

set TASK_PANEL_ID		10009					;# TaskPanel页面的ID

# 定义TaskPanel的主题类型
set xtpTaskPanelThemeOffice2000 0				;# Office 2000 theme
set xtpTaskPanelThemeOffice2003 1				;# Office 2003 theme
set xtpTaskPanelThemeNativeWinXP 2				;# Win XP theme
set xtpTaskPanelThemeOffice2000Plain 3			;# Office 2000 classic theme
set xtpTaskPanelThemeOfficeXPPlain 4			;# Office XP classic theme
set xtpTaskPanelThemeOffice2003Plain 5			;# Office 2003 classic theme
set xtpTaskPanelThemeNativeWinXPPlain 6			;# Win XP classic theme
set xtpTaskPanelThemeToolbox 7					;# VS.NET style Toolbox theme.
set xtpTaskPanelThemeToolboxWhidbey 8			;# Whidbey VS 2005 style Toolbox theme.
set xtpTaskPanelThemeListView 9					;# Standard List View theme.
set xtpTaskPanelThemeListViewOfficeXP 10		;# Office XP List View theme.
set xtpTaskPanelThemeListViewOffice2003 11		;# Office 2003 List View theme.
set xtpTaskPanelThemeShortcutBarOffice2003 12	;# ShortcutBar Office 2003 theme.
set xtpTaskPanelThemeResource 13				;# ShortcutBar Office 2007/2010 theme.
set xtpTaskPanelThemeVisualStudio2010 14		;# VS 2010 style Toolbox theme.

# TaskPanel中的item的类型
set TASKITEM_GROUP		0
set TASKITEM_TEXT		1
set TASKITEM_LINK		2
set TASKITEM_MASHUP		3

# 图片索引
}

#-------------------------------------------------------------
# 创建工作任务页面
#-------------------------------------------------------------
proc ProjectCreateTaskPanel { } {
	global platform_language platform_path
	
	if {[WorkTree treeexist $prjdef::TASK_PANEL_ID] == 0} {
		# 如果树不存在,就退出
		return
	}
	
	# 创建树
	LangStringEC STR_TREE_NAME "Task" "任务"
	set tree [WorkTree gettreeid $STR_TREE_NAME]
	if {$tree == -1} {
		return
	}
	
	# 设置页面类型为TaskPanel
	WorkTree settreetype $tree $prjdef::PGITYPE_TASKPANEL
	WorkTree settasktheme $tree $prjdef::xtpTaskPanelThemeNativeWinXP
	# 先删除所有项
	WorkTree deltaskitem $tree -id(0)
	
	# 判断工程是否关闭,如果已关闭就删除Task后退出
	set projectisopen [pproject isopen]
	if {$projectisopen == 0} {
		# 工程已关闭
		WorkTree delitem $tree 0 -TIM_ITEM -TIM_CHILDONLY
		return
	}
	
	set prj_vci_path [file dirname [info script]]
	
	# 添加图片,addtaskitem时候的图片索引是按插入顺序计算的
	WorkTree settaskicon $tree "$prj_vci_path/../image/general.ico"
	WorkTree settaskicon $tree "$prj_vci_path/../image/test.ico"
	WorkTree settaskicon $tree "$prj_vci_path/../image/develop.ico"
	WorkTree settaskicon $tree "$prj_vci_path/../image/img_filetree.bmp"
	
	# 添加任务菜单
	LangStringEC STR_TASK_GROUP_MANAGER "Manager" "管理"
	set nGroupID [WorkTree addtaskitem $tree $STR_TASK_GROUP_MANAGER "" -type($prjdef::TASKITEM_GROUP) -image(0)]; #-special]
	LangStringEC STR_TASK_ITEM_VCIMANAGER "Component Manager" "VCI组件管理"
	set command "html:tcl:<file>PLAT:///html/vcimanager.tcl"
	WorkTree addtaskitem $tree $STR_TASK_ITEM_VCIMANAGER $command -type($prjdef::TASKITEM_LINK) -parent($nGroupID) -image(33)
	LangStringEC STR_TASK_ITEM_OWMMANAGER "UI Plugin Manager" "界面组件管理"
	set command "html:tcl:<file>PLAT:///html/owmmanager.tcl"
	WorkTree addtaskitem $tree $STR_TASK_ITEM_OWMMANAGER $command -type($prjdef::TASKITEM_LINK) -parent($nGroupID) -image(33)
	LangStringEC STR_TASK_ITEM_INTERPMANAGER "Interpert Manager" "解释器管理"
	set command "html:tcl:<file>PLAT:///html/interpmanager.tcl"
	WorkTree addtaskitem $tree $STR_TASK_ITEM_INTERPMANAGER $command -type($prjdef::TASKITEM_LINK) -parent($nGroupID) -image(33)
	
	LangStringEC STR_TASK_GROUP_DOC "Document" "文档"
	set nGroupID [WorkTree addtaskitem $tree $STR_TASK_GROUP_DOC "" -type($prjdef::TASKITEM_GROUP) -image(1)]
	LangStringEC STR_TASK_ITEM_TCLDOC "Tcl Document" "Tcl文档"
	set command "html:$platform_path/doc/tcl/index_cn.html"
	WorkTree addtaskitem $tree $STR_TASK_ITEM_TCLDOC $command -type($prjdef::TASKITEM_LINK) -parent($nGroupID) -image(31)
	LangStringEC STR_TASK_ITEM_PYTHONDOC "Python Document" "Python文档"
	set command "html:$platform_path/doc/python/index_cn.html"
	WorkTree addtaskitem $tree $STR_TASK_ITEM_PYTHONDOC $command -type($prjdef::TASKITEM_LINK) -parent($nGroupID) -image(31)
	
	LangStringEC STR_TASK_GROUP_SAMPLES "Samples" "演示"
	set nGroupID [WorkTree addtaskitem $tree $STR_TASK_GROUP_SAMPLES "" -type($prjdef::TASKITEM_GROUP) -image(2)]
	LangStringEC STR_TASK_ITEM_TCLSAMPLE "Tcl Samples" "Tcl演示程序"
	set command "html:tcl:<file>PLAT://\\html\\\\samples.tcl?dir=$platform_path/samples/tcl"
	WorkTree addtaskitem $tree $STR_TASK_ITEM_TCLSAMPLE $command -type($prjdef::TASKITEM_LINK) -parent($nGroupID) -image(42)
	LangStringEC STR_TASK_ITEM_PYTHONSAMPLE "Python Samples" "Python演示程序"
	set command "html:tcl:<file>PLAT://\\html\\\\samples.tcl?dir=$platform_path/samples/python"
	WorkTree addtaskitem $tree $STR_TASK_ITEM_PYTHONSAMPLE $command -type($prjdef::TASKITEM_LINK) -parent($nGroupID) -image(42)
}

#-------------------------------------------------------------
# Main
#-------------------------------------------------------------
ProjectCreateTaskPanel
