#################################################################
# File       : xmlwdom.tcl
# Author     : Blueant Software
# Version    : 1.0
# Date       : 2004-09-20
# Description: Script.NET xml dom warp script
#################################################################

package require Itcl;
package require dom;

#-------------------------------------------------------------
# TXmlDom class define
#-------------------------------------------------------------
if {[itcl::find classes TXmlDom] != "TXmlDom"} {
::itcl::class  TXmlDom {
	constructor { rootname {fname "" } } {};
	destructor {};
	
	### data member ###
	private variable _filename;		#文件名
	private variable _xmlstream;	#xml流
	private variable _rootname;		#根名字
	private variable _doc;			#xml文档
	
	### public methods ###
	public method  Load { {fname ""} };	#打开文件
	public method  Save { {fname ""} };	#建立文件关联
	public method  GetNode { nodeName {nodeParaent ""} }; # 获取下层的某个节点
	public method  NewNode { nodeParaent nodeStream };	#创建一个子节点,并用指定的串替代
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
::itcl::body TXmlDom::constructor { rootname {fname "" } } {
	set _filename $fname
	set _xmlstream ""
	set _rootname $rootname
	Load
}

#-------------------------------------------------------------
#	单元析构函数
#-------------------------------------------------------------
::itcl::body TXmlDom::destructor {} {
}

#-------------------------------------------------------------
#	加载文件到内部流
#	参数:
#	fname	: 文件名,为空表示用保存的文件名
#-------------------------------------------------------------
::itcl::body TXmlDom::Load { {fname ""} } {
	if {$fname != ""} {
		set _filename $fname
	}
	
	set _xmlstream ""
	catch {
	set fxml [open $_filename r]
	set _xmlstream [read -nonewline $fxml]
	close $fxml
	
	set _doc [::dom::tcl::DOMImplementation parse $_xmlstream]
	} inf
	puts $inf
}

#-------------------------------------------------------------
#	保存文件
#	参数:
#	fname	: 文件名,为空表示用保存的文件名
#-------------------------------------------------------------
::itcl::body TXmlDom::Save { {fname ""} } {
	set _fname $fname
	if {$fname != ""} {
		set _fname $_filename
	}
	
	# XML文档序列化到缓冲区
	set _xmlstream [::dom::DOMImplementation serialize $_doc -newline $_rootname]
	
	catch {
	set fxml [open $_fname w+]
	puts -nonewline $fxml $_xmlstream
	close $fxml
	}
}

#-------------------------------------------------------------
#	获取下层的某个节点
#	参数:	nodeName	- 节点名
#			nodeParent	- 上层节点句柄
#-------------------------------------------------------------
::itcl::body TXmlDom::GetNode { nodeName {nodeParaent ""} } {
	if {$nodeParaent == ""} {
		return [::dom::document getElementsByTagName $_doc $nodeName];
	} else {
		return [::dom::document getElementsByTagName $nodeParaent $nodeName];
	}
}

#-------------------------------------------------------------
#	创建一个子节点,并用指定的串替代
#	参数:	nodeParent	- 上层节点句柄
#			nodeStream	- 节点的流
#-------------------------------------------------------------
::itcl::body TXmlDom::NewNode { nodeParaent nodeStream } {
	if {$nodeParaent == ""} {
		set newNode [::dom::document createElement $_doc "NEWNODE"];
	} else {
		set newNode [::dom::document createElement $nodeParaent "NEWNODE"];
	}
	
	# XML文档序列化到缓冲区
	set _xmlstream [::dom::DOMImplementation serialize $_doc -newline $_rootname]
	
	regsub {<NEWNODE/>} $_xmlstream $nodeStream _xmlstream
	
	regsub "<!DOCTYPE $_rootname\[^>\]*>" $_xmlstream {} _xmlstream
	#regsub -all -line {^$} $_xmlstream {} _xmlstream
	set lsLine [regexp -all -line -inline {^.*?$} $_xmlstream]
	set xmlstream ""
	foreach line $lsLine {
		set line [string trimright $line]
		if {$line != ""} {
			if {$xmlstream != ""} {
				set xmlstream "$xmlstream\n$line"
			} else {
				set xmlstream "$line"
			}
		}
	}	
	
	# 保存流
	catch {
	set fxml [open $_filename w+]
	puts -nonewline $fxml $xmlstream
	#puts $xmlstream
	close $fxml
	}
}
