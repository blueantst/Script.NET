#################################################################
# project.tcl
# Author     : blueant
# Version    : 4.0
# Date       : 2015-01-18
# Description: general project creater
#################################################################

package require Itcl

catch {namespace import itcl::*}

#-------------------------------------------------------------
# replace keywords of file
#-------------------------------------------------------------
proc ReplaceFileKey { fname lsKey } {
	
	if {[file exist $fname] == 0} {
		return
	}
	
	catch {
	set f [open $fname r]
	set filestream [read -nonewline $f]
	close $f
	}

	catch {
	foreach {key replace} $lsKey {
		regsub -all $key $filestream $replace filestream
	}
	}
	
	catch {
	set f [open $fname w+]
	puts -nonewline $f $filestream		
	close $f
	}
}

#-------------------------------------------------------------
# TProject class define
#-------------------------------------------------------------
if {[itcl::find classes TProject] != "TProject"} {
::itcl::class  TProject {
	constructor {} {};
	destructor {};
	
	### data member ###
	protected variable _project_name;	# 工程名
	
	### public methods ###
	public method  New {prj_name prj_path prj_type prj_desc author prj_template};	# 新建工程
	public method  AfterNew {prj_name prj_path prj_type prj_desc author prj_template};	# 新建工程之后
}
}

#-------------------------------------------------------------
# 单元构造函数
#-------------------------------------------------------------
::itcl::body TProject::constructor {} {
}

#-------------------------------------------------------------
# 单元析构函数
#-------------------------------------------------------------
::itcl::body TProject::destructor {} {
}

#-------------------------------------------------------------
# 新建工程
# Returns    : project file
#-------------------------------------------------------------
::itcl::body TProject::New {prj_name prj_path prj_type prj_desc author prj_template} {
	# 创建工程目录
	#file mkdir $prj_path

	set lsFile [glob -path "$prj_template\\" "*.*"]
	
	# 从模板目录拷贝文件
	foreach sFile $lsFile {
		file copy $sFile $prj_path
	}
	
	# 返回工程文件名
	return "$prj_path\\$prj_name.snp"
}

#-------------------------------------------------------------
# 新建工程之后
# Returns    :
#-------------------------------------------------------------
::itcl::body TProject::AfterNew {prj_name prj_path prj_type prj_desc author prj_template} {
	
	# 替换文件中的关键字
	set lsKey [list "%PRJ_NAME%"]
	lappend lsKey "$prj_name"
	lappend lsKey "%PRJ_TYPE%"
	lappend lsKey "$prj_type"
	lappend lsKey "%PRJ_DESC%"
	lappend lsKey "$prj_desc"
	lappend lsKey "%AUTHOR%"
	lappend lsKey "$author"
	lappend lsKey "%DATE%"
	lappend lsKey [clock format [clock seconds] -format "%Y-%m-%d"]

	ReplaceFileKey "$prj_path\\globalproperty.tcl" $lsKey
	ReplaceFileKey "$prj_path\\test.tcl" $lsKey
	ReplaceFileKey "$prj_path\\test.py" $lsKey
}
