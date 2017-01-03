#################################################################
# Script.NET 页面脚本
# 作者:blueant
# 版本:1.0
# 日期:2002-03-05
# 注:此脚本提供演示程序浏览功能
#################################################################

package require TclFace;

#-------------------------------------------------------------
# TLFAppPage class define
#-------------------------------------------------------------
if {[itcl::find classes TLFAppPage] != "TLFAppPage"} {
class  TLFAppPage {
	inherit TLFPage
	
	constructor {} {};
	
	### data member ###
	protected variable _objTextInfo;
	protected variable _tableFile;
	protected variable _curDir;			# 当前路径
	protected variable _cdDir;			# 变换的路径
	
	### public methods ###
	public method  ProcessFormValue {name value};	# 处理Form结果
	public method  OnCreate {};						# 创建页面
	public method  OnInitialUpdate {};				# 页面初始化
	public method  OnEndAction {};					# 动作执行完后的操作
	public method  OnChangeDir {};
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
body TLFAppPage::constructor {} {
	chain;
	SetFormAction;
	set _curDir "";
	set _cdDir "";
	set _objTextInfo "";
}

#-------------------------------------------------------------
# 处理Form结果
#-------------------------------------------------------------
body TLFAppPage::ProcessFormValue {name value} {
	if {[chain $name $value] == true} {
		return true
	}
	switch -glob $name {
		dir			{set _curDir $value}
		cddir		{set _cdDir $value;regsub -all "{|}" $_cdDir {} _cdDir;}
	}
}

#-------------------------------------------------------------
#	创建页面
#-------------------------------------------------------------
body TLFAppPage::OnCreate {} {
	LangStringEC STR_INFO_DIR "Current Directory:" "当前路径："
	set _objTextInfo [AppendText "<b>$STR_INFO_DIR $_curDir</b>"]
	$_objTextInfo SetColor #800000
}

#-------------------------------------------------------------
#	页面初始化
#-------------------------------------------------------------
body TLFAppPage::OnInitialUpdate {} {
}

#-------------------------------------------------------------
#	动作执行完后的操作
#-------------------------------------------------------------
body TLFAppPage::OnEndAction {} {
	
	regsub -all {\\} $_curDir {/} _curDir;
	
	# 关闭连接
	LangStringEC STR_INFO_DIR "Current Directory:" "当前路径："
	$_objTextInfo SetText "<b>$STR_INFO_DIR $_curDir</b>"
	
	# 创建文件表格
	set _tableInfo [CreateObject TLFTable]
	set _tableFile [CreateObject TLFTable]
	LangStringEC STR_INFO "File Name" "文件名"
	$_tableFile AddColumn $STR_INFO
	LangStringEC STR_INFO "Size" "大小"
	$_tableFile AddColumn $STR_INFO
	LangStringEC STR_INFO "Create Time" "创建时间"
	$_tableFile AddColumn $STR_INFO

	# 创建dir和file列表
	set lsItems [list]
	catch {set lsItems [glob -directory "$_curDir/" *]}

	# create back parent directory line
	if {[string index $_curDir end] != ":"} {
		$_tableFile AddLine2 \
							[list "<img border=0 src='image\\back.gif'>.." "" "tcl:<file>PLAT://\\html\\samples.tcl?action=OnChangeDir&dir=$_curDir&cddir=\{..\}"] \
							[list "&nbsp"] \
							[list "&nbsp"]
	}

	foreach item $lsItems {
		set splitpos [string last "/" $item];
		if {$splitpos != -1} {
			incr splitpos;
			set name [string range $item $splitpos end];
		} else {
			set name $item;
		}
		set size [file size $item];
		set mtime [clock format [file mtime $item] -format "%Y-%m-%d %H:%M:%S"];
		
		# 过滤目录名
		if { $name == "." || $name == ".." } {
			continue;
		}
		
		if {[file isdirectory $item]} {
			# create dir line
			$_tableFile AddLine2 \
							[list "<img border=0 src='image\\folder.gif'>$name" "" "tcl:<file>PLAT://\\html\\samples.tcl?action=OnChangeDir&dir=$_curDir&cddir=\{$name\}"] \
							[list "&nbsp"] \
							[list "&nbsp"]
		} else {
			# create file line
			set img "file.gif";
			set dot [string last "." $name];
			if {$dot != -1} {
				incr dot;
				set ext [string range $name $dot end];
				set ext [string tolower $ext];				
				switch $ext {
					"gif"	{set img "filetype/gif.gif"}
					"jpg"	{set img "filetype/jpg.gif"}
					"bmp"	{set img "filetype/bmp.gif"}
					"ico"	-
					"pcx"	-
					"png"	-
					"tiff"	{set img "image.gif"}
					"zip"	-
					"rar"	-
					"arj"	-
					"lzh"	-
					"cab"	{set img "compressed.gif"}
					"htm"	-
					"html"	{set img "filetype/html.gif"}
					"xml"	{set img "filetype/xml.gif"}
					"c"		-
					"cpp"	{set img "filetype/source-c.gif"}
					"h"		-
					"hpp"	{set img "filetype/source-h.gif"}
					"cs"	{set img "filetype/cs.gif"}
					"doc"	{set img "filetype/doc.gif"}
					"ppt"	{set img "filetype/ppt.gif"}
					"xls"	{set img "filetype/xls.gif"}
					"js"	{set img "filetype/js.gif"}
					"tkv"	{set img "filetype/tkview.gif"}
					"tk"		-
					"tcl"	{set img "filetype/tcl.gif"}
					"pyw"		-
					"py"	{set img "filetype/python.gif"}
					"rb"	{set img "filetype/ruby.gif"}
					"pl"	{set img "filetype/perl.gif"}
					"php"	{set img "filetype/php.gif"}
					"lua"	{set img "filetype/lua.gif"}
					"java"	{set img "filetype/java.gif"}
					"uml"	{set img "filetype/uml.gif"}
					"nsh"		-
					"nsi"	{set img "filetype/nsis.gif"}
					"exe"	{set img "this.gif"}
				}
			}
			
			if {$img == "this.gif"} {
				$_tableFile AddLine2 \
								[list "<img border=0 src='image\\$img'>$name" "" "tcl:exec \"$item\" &"] \
								[list "$size"] \
								[list "$mtime"]
			} else {
				$_tableFile AddLine2 \
								[list "<img border=0 src='image\\$img'>$name" "" "pfe:open $item"] \
								[list "$size"] \
								[list "$mtime"]
			}
		}
	}
}

#-------------------------------------------------------------
#	cd dir
#-------------------------------------------------------------
body TLFAppPage::OnChangeDir {} {
	regsub -all {\\} $_curDir {/} _curDir;
	if {$_cdDir == ".."} {
		set splitpos [string last "/" $_curDir];
		if {$splitpos != -1} {
			incr splitpos -1;
			set _curDir [string range $_curDir 0 $splitpos];
		}
	} else {
		set _curDir "$_curDir/$_cdDir";
	}
	
	return true
}

#-------------------------------------------------------------
#	main
#-------------------------------------------------------------
LangString STR_PAGE_TITLE chinese "演示程序"
LangString STR_PAGE_TITLE english "Samples"
set page [TLFCreatePage $STR_PAGE_TITLE TLFAppPage]
#delete object $page;
