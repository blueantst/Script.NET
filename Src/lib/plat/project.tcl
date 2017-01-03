#################################################################
# File       : project.tcl
# Author     : Blueant Software
# Version    : 1.0
# Date       : 2001-02-04
# Description: Script.NET project operater script
#################################################################

#-------------------------------------------------------------
# 创建状态栏Pane
#-------------------------------------------------------------
proc CreateStatusPane { hSnp } {
	# 创建状态栏Pane
	set node [xml::GetNode $hSnp ptr1 "StatusBar" "Pane"];
	while {$node != "null"} {
		set Name [xml::GetNode $hSnp text $node];
		set ID [xml::GetNode $hSnp attr $node "ID"];
		set Width [xml::GetNode $hSnp attr $node "Width"];
		set ImageGroup [xml::GetNode $hSnp attr $node "ImageGroup"];
		set Image [xml::GetNode $hSnp attr $node "Image"];		
		set nID [StatusBar -createpane -width $Width -id $ID];
		StatusBar -settext $nID $Name;
		StatusBar -seticon $nID $ImageGroup $Image;
		set node [xml::GetNode $hSnp next $node];		
	}
}

#-------------------------------------------------------------
# 创建工程树
#-------------------------------------------------------------
proc CreateProjectTree { hSnp prjfile } {

	# 获取工程文件基路径
	set prjpath [file dirname $prjfile];
		
	# 创建树
	WorkTree closetree "工程";
	set tree [WorkTree gettreeid "工程"];	
	if {$tree == -1} {
		set tree [WorkTree addtree PGI_CLASS "工程"];
	}
	
	set PrjName [xml::GetNode $hSnp text "Project" ""];
	set hRootItem [WorkTree additem $tree ROOT "工程\[$PrjName\]" -image(1) -id(1) -type(1) -bold(true)];

	set node [xml::GetNode $hSnp ptr1 "Files" "Folder"];
	while {$node != "null"} {
		set Name [xml::GetNode $hSnp attr $node "Name"];
		set Image [xml::GetNode $hSnp attr $node "Image"];
		set hFolderItem [WorkTree additem $tree $hRootItem $Name -image($Image) -id(2) -type(2)];

		set node1 [xml::GetNode $hSnp child $node ptr2];
		while {$node1 != "null"} {
			set Name [xml::GetNode $hSnp text $node1];
			set Type [xml::GetNode $hSnp attr $node1 "Type"];
			set Image [xml::GetNode $hSnp attr $node1 "Image"];
			set sfilename "$prjpath/$Name";
			if {$Type == "tcl" || $Type == "python"} {
				set _Name [xml::GetNode $hSnp attr $node1 "Name"];
				set hFileItem [WorkTree additem $tree $hFolderItem $_Name -image($Image) -id(4) -type(1) -info($sfilename)];
			} else {
				set hFileItem [WorkTree additem $tree $hFolderItem $Name -image($Image) -id(3) -type(3) -info($sfilename)];
			}
			
			set node1 [xml::GetNode $hSnp next $node1];		
		}
		
		set node [xml::GetNode $hSnp next $node];
	}

	WorkTree expanditem $tree $hRootItem -TIM_ITEM;
	
	# 节点双击的脚本,必须是一个TCL过程,且有3个参数,分别为树ID,节点句柄,节点ID
	set dclickscript {
		proc dclick {tree item id} {
			set ItemName [WorkTree getitem name $tree $item -TIM_ITEM];
			set ItemInfo [WorkTree getitem info $tree $item -TIM_ITEM];
			#puts "item=$item,id=$id,ItemName=$ItemName,ItemInfo=$ItemInfo";

			# 根节点则不继续处理
			if {$id == 1 || $id == 2} {
				# 根节点和文件夹节点
				return;
			} elseif {$id == 3} {
				# 文件节点
				set res [owm -sendcmd 100 102 "<string>$ItemInfo" 0];
				if {$res == 0} {
					# 如果返回0说明OwmEdit窗口未打开,需要先打开一个窗口
					owm -open Frame 100 -active -onlyone;
					# 在OwmEdit窗口的第一个位置打开文件
					owm -sendcmd 100 103 "<string>$ItemInfo" 0;
				} else {
					owm -open Frame 100 -active -onlyone;
				}
				return;
			} elseif {$id == 4} {
				# 执行节点
				source $ItemInfo;
			}
		};dclick}

	WorkTree setdclickscript $tree $dclickscript;
}

#-------------------------------------------------------------
# 关闭工程树
#-------------------------------------------------------------
proc CloseProjectTree {} {

	WorkTree closetree "工程";
}

#-------------------------------------------------------------
# 打开工程
#-------------------------------------------------------------
proc OpenProject { prjfile } {

	set hSnp [xml::open file $prjfile];
	if {$hSnp == 0} {
		return;
	}
	
	CreateStatusPane $hSnp;
	CreateProjectTree $hSnp $prjfile;
	
	xml::close $hSnp;

}

#-------------------------------------------------------------
# 关闭工程
#-------------------------------------------------------------
proc CloseProject { prjfile } {

	CloseProjectTree;

}

#-------------------------------------------------------------
# 主程序
#-------------------------------------------------------------
if {$argc > 0} {
	set opr [lindex $argv 0];
	if {$opr == "open"} {
		if {$argc > 1} {
			OpenProject [lindex $argv 1];
		}
	} elseif {$opr == "close"} {
		if {$argc > 1} {
			CloseProject [lindex $argv 1];
		}
	}
}
