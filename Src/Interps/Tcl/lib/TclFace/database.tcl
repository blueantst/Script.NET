#################################################################
# File       : database.tcl
# Author     : Blueant Software
# Version    : 1.0
# Date       : 2007-10-21
# Description: Script.NET Tcl Database Face script
#################################################################

package provide DatabaseFace 1.0

package require TclFace

#-------------------------------------------------------------
# TLFDataProvider class define
#-------------------------------------------------------------
if {[itcl::find classes TLFDataProvider] != "TLFDataProvider"} {
global g_ls_dataprovider;
set g_ls_dataprovider {}
class  TLFDataProvider {
	inherit TLFObject
	
	constructor {} {};
	destructor {};
	
	### data member ###
	public variable _name;			# 数据提供者名字
	public variable _type;			# 数据提供者类型
	public variable _version;		# 数据提供者版本
	public variable _connect;		# 数据提供者连接
	public variable _username;		# 用户名
	public variable _pasword;		# 口令
	
	### public methods ###
	public method  Open {{cnstr ""}} {};		# 打开
	public method  Close {} {};		# 关闭
	public method  GetConnectionString {} {};	# 获取连接字符串
	public method  Exec { sql } {};	# 执行
	public method  Transaction { param } {};	# 事物处理
	public method  Pack { } {};	# 整理
	public method  QueryTables {{type TABLE}} {};		# 获取Table列表
	public method  QueryColumn {tablename {detail ""}} {};	# 查询表的列名
	public method  CreateTable {tablename fields} {};	# 创建表
	public method  DropTable {tablename} {};	# 删除表
	public method  ClearTable {tablename} {};	# 清空表
	public method  GetRecords {tablename {startrow ""} {endrow ""} {props {}}} {};	# 获取指定记录
	public method  InsertRecord {tablename rowindex props values} {};	# 添加记录
	public method  DeleteRecords {tablename startrow {endrow ""}} {};	# 删除记录
	public method  SetRecord {tablename rowindex props values} {};	# 设置记录
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
body TLFDataProvider::constructor {} {
	chain;
	set _name "";
	set _type "";
	set _version "";
	set _connect "";
	set _username "";
	set _pasword "";
}

#-------------------------------------------------------------
#  Get Connection String
#-------------------------------------------------------------
body TLFDataProvider::GetConnectionString {} {
	return $_connect
}

#-------------------------------------------------------------
# TLFDataSource class define
#-------------------------------------------------------------
if {[itcl::find classes TLFDataSource] != "TLFDataSource"} {
class  TLFDataSource {
	inherit TLFObject
	
	constructor {} {};
	destructor {};
	
	### data member ###
	protected variable _objDataProvider;# 数据提供者对象
	protected variable _tableName;		# 表名
	protected variable _fields;			# 字段列表
	
	### public methods ###
	public method  SetDataProvider {objProvider};	# 设置数据提供者对象
	public method  SetTable {tablename};			# 设置数据表
	public method  SetFields {fields};				# 设置字段
	public method  GetData {line {field ""}};		# 获取指定行的数据
	public method  SetData {line data {field ""}};	# 设置指定行的数据
}
}

#-------------------------------------------------------------
#	单元构造函数
#-------------------------------------------------------------
body TLFDataSource::constructor {} {
	chain;
	set _pageType "";
	set _svcName "";
}

#-------------------------------------------------------------
#	设置数据提供者对象
#-------------------------------------------------------------
body TLFDataSource::SetDataProvider {objProvider} {
}

#-------------------------------------------------------------
#	设置数据表
#-------------------------------------------------------------
body TLFDataSource::SetTable {tablename} {
}

#-------------------------------------------------------------
#	设置字段
#-------------------------------------------------------------
body TLFDataSource::SetFields {fields} {
}

#-------------------------------------------------------------
#	获取指定行的数据
#-------------------------------------------------------------
body TLFDataSource::GetData {line {field ""}} {
}

#-------------------------------------------------------------
#	设置指定行的数据
#-------------------------------------------------------------
body TLFDataSource::SetData {line data {field ""}} {
}


#-------------------------------------------------------------
# TLFDataProviderADO class define
#-------------------------------------------------------------
if {[itcl::find classes TLFDataProviderADO] != "TLFDataProviderADO"} {
package require tcom
lappend g_ls_dataprovider "TLFDataProviderADO"
class  TLFDataProviderADO {
	inherit TLFDataProvider
	
	constructor {} {};
	destructor {};
	
	### data member ###
	
	# 数据库字段类型定义
	public common DBTYPE_EMPTY		0
	public common DBTYPE_NULL		1
	public common DBTYPE_I2			2
	public common DBTYPE_I4			3
	public common DBTYPE_R4			4
	public common DBTYPE_R8			5
	public common DBTYPE_CY			6
	public common DBTYPE_DATE		7
	public common DBTYPE_BSTR		8
	public common DBTYPE_IDISPATCH	9
	public common DBTYPE_ERROR		10
	public common DBTYPE_BOOL		11
	public common DBTYPE_VARIANT	12
	public common DBTYPE_IUNKNOWN	13
	public common DBTYPE_DECIMAL	14
	public common DBTYPE_UI1		17
	public common DBTYPE_I1			16
	public common DBTYPE_UI2		18
	public common DBTYPE_UI4		19
	public common DBTYPE_I8			20
	public common DBTYPE_UI8		21
	public common DBTYPE_GUID		72
	public common DBTYPE_FILETIME	64
	public common DBTYPE_BYTES		128
	public common DBTYPE_STR		129
	public common DBTYPE_WSTR		130
	public common DBTYPE_NUMERIC	131
	public common DBTYPE_UDT		132
	public common DBTYPE_DBDATE		133
	public common DBTYPE_DBTIME		134
	public common DBTYPE_DBTIMESTAMP 135

	# 数据库字段类型名定义
	public common TYPENAME

	# 内部变量定义
	protected variable m_cn		""	;# Connection对象句柄
	protected variable m_rs		""	;# Recordset对象句柄
	
	# 数据集的游标类型3=adOpenStatic
	protected variable m_CursorType	3
	# 数据集的锁定类型1=adLockReadOnly
	protected variable m_LockType	1
	
	### public methods ###
	public method  Open {{cnstr ""}} {};		# 打开
	public method  Close {} {};					# 关闭
	public method  Exec { sql } {};				# 执行
	public method  Transaction { param } {};	# 事物处理
	public method  Pack { } {};	# 整理
	public method  QueryTables {{type TABLE}} {};		# 获取Table列表
	public method  QueryColumn {tablename {detail ""}} {};	# 查询表的列名
	public method  CreateTable {tablename fields} {};	# 创建表
	public method  DropTable {tablename} {};	# 删除表
	public method  ClearTable {tablename} {};	# 清空表
	public method  GetRecords {tablename {startrow ""} {endrow ""} {props {}}} {};	# 获取指定记录
	public method  InsertRecord {tablename rowindex props values} {};	# 添加记录
	public method  DeleteRecords {tablename startrow {endrow ""}} {};	# 删除记录
	public method  SetRecord {tablename rowindex props values} {};	# 设置记录
}
}

#-------------------------------------------------------------
#	constructor
#-------------------------------------------------------------
body TLFDataProviderADO::constructor {} {
	chain;
	
	set _name "ADO";
	set _version "1.0";
	
	# Create ADO COM Object
	set ret [catch {set m_cn [::tcom::ref createobject "ADODB.Connection"]} msg]
	if {$ret} {
		error "Create ADO connection failed, because: $msg"
	}
	
	set ret2 [catch {set m_rs [::tcom::ref createobject "ADODB.Recordset"]} msg]
	if {$ret} {
		error "Create ADO recordset failed, because: $msg"
	}
}

#-------------------------------------------------------------
#	destructor
#-------------------------------------------------------------
body TLFDataProviderADO::destructor {} {
	Close
	catch {unset m_cn m_rs}
}

#-------------------------------------------------------------
#  Open Database
#-------------------------------------------------------------
body TLFDataProviderADO::Open {{cnstr ""}} {
	# 关闭连接
	Close
	# 建立连接
	if {$cnstr == ""} {
		set ret [catch {set dl [::tcom::ref createobject "Datalinks"]} msg]
		if {$ret} {
			error "Create ADO Datalinks failed, because: $msg"
		}
		
		set ret [catch {
			set conn [$dl PromptNew]
			set cnstr [$conn ConnectionString]
			unset conn
			unset dl
			} msg]
		if {$ret} {
			#error "获取连接字符串失败,原因:$msg!"
			set _connect ""
			return
		}
	}

	set ret [catch {$m_cn Open $cnstr} msg]
	if {$ret} {
		error "$msg\nOpen database connection failed, please check connection string!\n$cnstr"
	}
	
	# 保存连接字符串
	set _connect $cnstr
	
	return
}

#-------------------------------------------------------------
#  Close Database
#-------------------------------------------------------------
body TLFDataProviderADO::Close {} {
	# 关闭连接
	catch {$m_rs Close}
	catch {$m_cn Close}
	return
}

#-------------------------------------------------------------
#  Exec SQL
#-------------------------------------------------------------
body TLFDataProviderADO::Exec { sql } {
	set m_rowcount 0
	
	# 关闭Recordset
	catch {$m_rs Close}
	
	# 执行查询
	set ret [catch {$m_rs Open $sql $m_cn $m_CursorType $m_LockType} msg]
	if {$ret} {
		error "$msg\nExec SQL failed:\n$sql"
	}
	
	# 检查SQL语句是否返回了数据
	catch {set m_rowcount [$m_rs RecordCount]}
	if {$m_rowcount < 1} {
		catch {$m_rs Close}
		return
	}
	
	set flds [$m_rs Fields]
	set m_colcount [$flds Count]
	set m_data {}
	
	# 数据
	catch {
		for {set j 1} {$j <= $m_rowcount} {incr j} {
			set line {}
			for {set i 0} {$i < $m_colcount} {incr i} {
				lappend line [string trimright [$m_rs Collect $i]]
			}
			
			lappend m_data $line
			$m_rs MoveNext
		}
	}
	
	# 关闭Recordset
	catch {$m_rs Close}
	
	# 创建并返回数据列表
	return $m_data
}

#-------------------------------------------------------------
#  Transaction
#-------------------------------------------------------------
body TLFDataProviderADO::Transaction { param } {
}

#-------------------------------------------------------------
#  Pack database
#-------------------------------------------------------------
body TLFDataProviderADO::Pack {} {
}

#-------------------------------------------------------------
#  Query Tables
#  default is query all TABLE, return table name
#  if type is null, then return list of table name and type
#-------------------------------------------------------------
body TLFDataProviderADO::QueryTables {{type TABLE}} {
	# SchemaEnum 20=adSchemaTables
	if {[catch {set srs [$m_cn OpenSchema 20]} msg]} {
		error $msg
	}
	
	set data {}
	while {[$srs EOF] == 0} {
		if {($type != "") && ($type != "-all")} {
			if {[$srs Collect TABLE_TYPE] == $type} {
				lappend data [$srs Collect TABLE_NAME]
			} 
		} else {
			lappend data [list [$srs Collect TABLE_NAME] [$srs Collect TABLE_TYPE]]
		}
		$srs MoveNext
	}
	
	catch {$srs Close}
	
	return $data
}

#-------------------------------------------------------------
#  Query one table's all column information
#  if follow -detail parameter, then return column detail info
#  detail is column's: Name, Type, Max Length, Default, NullAble,
#                      HasDefault 
#-------------------------------------------------------------
body TLFDataProviderADO::QueryColumn {tablename {detail ""}} {
	# SchemaEnum 4=adSchemaColumns
	if {[catch {set srs [$m_cn OpenSchema 4]} msg]} {
		error $msg
	}
	
	# 数据库字段类型名定义
	set TYPENAME($DBTYPE_EMPTY)		"NULL"
	set TYPENAME($DBTYPE_NULL)		"NULL"
	set TYPENAME($DBTYPE_I2)		"SMALLINT"
	set TYPENAME($DBTYPE_I4)		"INT"
	set TYPENAME($DBTYPE_R4)		"FLOAT"
	set TYPENAME($DBTYPE_R8)		"REAL"
	set TYPENAME($DBTYPE_CY)		"INT"
	set TYPENAME($DBTYPE_DATE)		"DATETIME"
	set TYPENAME($DBTYPE_BSTR)		"TEXT"
	set TYPENAME($DBTYPE_IDISPATCH)	"INT"
	set TYPENAME($DBTYPE_ERROR)		"INT"
	set TYPENAME($DBTYPE_BOOL)		"INT"
	set TYPENAME($DBTYPE_VARIANT)	"INT"
	set TYPENAME($DBTYPE_IUNKNOWN)	"INT"
	set TYPENAME($DBTYPE_DECIMAL)	"INT"
	set TYPENAME($DBTYPE_UI1)		"INT"
	set TYPENAME($DBTYPE_I1)		"INT"
	set TYPENAME($DBTYPE_UI2)		"INT"
	set TYPENAME($DBTYPE_UI4)		"INT"
	set TYPENAME($DBTYPE_I8)		"BIGINT"
	set TYPENAME($DBTYPE_UI8)		"INT"
	set TYPENAME($DBTYPE_GUID)		"TEXT"
	set TYPENAME($DBTYPE_FILETIME)	"DATETIME"
	set TYPENAME($DBTYPE_BYTES)		"INT"
	set TYPENAME($DBTYPE_STR)		"TEXT"
	set TYPENAME($DBTYPE_WSTR)		"TEXT"
	set TYPENAME($DBTYPE_NUMERIC)	"INT"
	set TYPENAME($DBTYPE_UDT)		"INT"
	set TYPENAME($DBTYPE_DBDATE)	"DATETIME"
	set TYPENAME($DBTYPE_DBTIME)	"DATETIME"
	set TYPENAME($DBTYPE_DBTIMESTAMP) "TIMESTAMP"
	
	set data {}
	while {[$srs EOF] == 0} {
		if {[$srs Collect TABLE_NAME] == $tablename} {
			if {$detail == "-detail"} {
				set data_type [$srs Collect DATA_TYPE]
				set data_size [$srs Collect CHARACTER_MAXIMUM_LENGTH]
				if {$data_size != ""} {
					set data_size [expr int($data_size)]
				}
				lappend data [list  [$srs Collect COLUMN_NAME] \
									$TYPENAME($data_type) \
									$data_size \
									[$srs Collect COLUMN_DEFAULT] \
									[$srs Collect IS_NULLABLE] \
									[$srs Collect COLUMN_HASDEFAULT] \
									]
			} else {
				lappend data [$srs Collect COLUMN_NAME]
			}
		}
		$srs MoveNext
	}
	
	catch {$srs Close}
	
	return $data
}

#-------------------------------------------------------------
#  Create new table
#  field parameter is a list of field, every field is a list
#  of field name, type, size, default value, not null, auto
#  increment, primary key or index or unique
#-------------------------------------------------------------
body TLFDataProviderADO::CreateTable {tablename fields} {
	set lsTable [QueryTables]
	if {[lsearch $lsTable $tablename] != -1} {
		error "Table $tablename is exist in database."
	}
	
	set sql "CREATE TABLE $tablename\("
	set field_count 0
	foreach field $fields {
		set field_name [lindex $field 0]
		if {$field_name == ""} {
			continue;
		}
		
		set field_type [lindex $field 1]
		set field_size [lindex $field 2]
		set field_default [lindex $field 3]
		if {[regexp {(|)} $field_default] == 1} {
			# 过滤掉有括号的值,这种值表示执行函数,暂不支持
			set field_default ""
		}
		
		set field_notnull ""
		if {[lsearch [lrange $field 4 end] "notnull"] != -1} {
			set field_notnull "notnull"
		}
		
		set field_extend ""
		if {[lsearch [lrange $field 4 end] "AUTO_INCREMENT"] != -1} {
			set field_extend "AUTO_INCREMENT"
		}
		
		set field_key ""
		if {[lsearch [lrange $field 4 end] "primary"] != -1} {
			set field_key primary
		} elseif {[lsearch [lrange $field 4 end] "index"] != -1} {
			set field_key index
		} elseif {[lsearch [lrange $field 4 end] "unique"] != -1} {
			set field_key unique
		}
		
		if {$field_count > 0} {
			set sql "$sql ,"
		}
		set sql "$sql $field_name $field_type"
		if {($field_size != "") && ($field_size != "0")} {
			set sql "$sql\($field_size\)"
		}
		if {$field_notnull != ""} {
			set sql "$sql NOT NULL"
		}
		if {$field_default != ""} {
			if {[lsearch $field_type {"TEXT" "LONGTEXT" "VARCHAR"}] != -1} {
				set sql "$sql DEFAULT '$field_default'"
			} else {
				set sql "$sql DEFAULT $field_default"
			}		
		}
		if {$field_extend == "AUTO_INCREMENT" } {
			set sql "$sql AUTONUMBER"
		}
		switch $field_key {
			primary	{ set sql "$sql PRIMARY KEY" }
			index	{}
			unique	{}
		}
		
		incr field_count
	}
	
	set sql "$sql \)"
	
	Exec $sql
}

#-------------------------------------------------------------
#  Drop table
#-------------------------------------------------------------
body TLFDataProviderADO::DropTable {tablename} {
	set lsTable [QueryTables]
	if {[lsearch $lsTable $tablename] == -1} {
		error "Table $tablename is not exist in database."
	}
	
	set sql "DROP TABLE $tablename"
	Exec $sql
}

#-------------------------------------------------------------
#  Clear table
#-------------------------------------------------------------
body TLFDataProviderADO::ClearTable {tablename} {
	set lsTable [QueryTables]
	if {[lsearch $lsTable $tablename] == -1} {
		error "Table $tablename is not exist in database."
	}
	
	set sql "DELETE FROM $tablename"
	Exec $sql
}

#-------------------------------------------------------------
#  Get records
#-------------------------------------------------------------
body TLFDataProviderADO::GetRecords {tablename {startrow ""} {endrow ""} {props {}}} {
	if {[llength $props] == 0} {
		set lsColumn [QueryColumn $tablename]
	} else {
		set lsColumn $props
	}
	
	set sql "SELECT"
	if {[llength $props] == 0} {
		set sql "$sql *"
	} else {
		set sql "$sql [join $lsColumn ,]"
	}
	set sql "$sql FROM $tablename"
	
	if {$startrow == 0} {
		set startrow ""
	}
	if {$endrow == "end"} {
		set endrow ""
	}
	
	if {($startrow != "") || ($endrow != "")} {
		set sql "$sql WHERE"
		if {$startrow != ""} {
			set sql "$sql $startrow"
		}
		if {($startrow != "") && ($endrow != "")} {
			set sql "$sql AND"
		}
		if {$endrow != ""} {
			set sql "$sql $endrow"
		}
	}
	
	return [Exec $sql]
}

#-------------------------------------------------------------
#  Insert records
#-------------------------------------------------------------
body TLFDataProviderADO::InsertRecord {tablename rowindex props values} {
	if {$rowindex == "unique"} {
		#判断指定属性的唯一性,如果存在就退出
		set sql "SELECT * FROM $tablename WHERE"
		set props_new {}
		set sep " "
		foreach prop $props value $values {
			if {[string first ":unique" $prop] != -1} {
				regsub {:unique} $prop {} prop
				if {[string is double -strict $value]} {
					append sql $sep$prop=$value
				} else {
					append sql $sep$prop='[string map {' ''} $value]'
				}
				set sep " AND "
			}
			lappend props_new $prop
		}
		set props $props_new
		set lsFind [Exec $sql]
		if {[llength $lsFind] != 0} {
			return
		}
	}
	
	set sql "INSERT INTO '[string map {' ''} $tablename]' VALUES"
    set sep (
	set rowid $rowindex
	set ncol [llength $values]
    for {set i 0} {$i<$ncol} {incr i} {
		set val [lindex $values $i]
		if {[string is double -strict $val]} {
			append sql $sep$val
		} else {
			append sql $sep'[string map {' ''} $val]'
		}
		set sep ,
    }
    append sql )
	
	Exec $sql
}

#-------------------------------------------------------------
#  Delete records
#-------------------------------------------------------------
body TLFDataProviderADO::DeleteRecords {tablename startrow {endrow ""}} {
	set sql "DELETE FROM $tablename"
	
	if {$startrow == 0} {
		set startrow ""
	}
	if {$endrow == "end"} {
		set endrow ""
	}
	
	if {($startrow != "") || ($endrow != "")} {
		set sql "$sql WHERE"
		if {$startrow != ""} {
			if {[string match {[0-9]*} $startrow]} {
				set sql "$sql rowid>=$startrow"
			} else {
				set sql "$sql $startrow"
			}
		}
		if {($startrow != "") && ($endrow != "")} {
			set sql "$sql AND"
		}
		if {($endrow != "") && ($endrow != "end")} {
			if {[string match {[0-9]*} $endrow]} {
				set sql "$sql rowid<=$endrow"
			} else {
				set sql "$sql $endrow"
			}
		}
	}
	
	Exec $sql
}

#-------------------------------------------------------------
#  Set record
#-------------------------------------------------------------
body TLFDataProviderADO::SetRecord {tablename rowindex props values} {
	set sql "UPDATE $tablename "
    set sep {SET}
    set rowid $rowindex
	set ncol [llength $values]
    for {set i 0} {$i<$ncol} {incr i} {
		set val [lindex $values $i]
		set name [lindex $props $i]
		append sql "$sep $name="
		if {[string is double -strict $val]} {
			append sql $val
		} else {
			append sql '[string map {' ''} $val]'
		}
		set sep ,
    }
	
    if {[string match {*[<>=]*} $rowid]} {
		append sql " WHERE $rowid"
	} else {
		append sql " WHERE rowid=$rowid"
	}
	
	Exec $sql
}

#-------------------------------------------------------------
# TLFDataProviderMetakit class define
#-------------------------------------------------------------
if {[itcl::find classes TLFDataProviderMetakit] != "TLFDataProviderMetakit"} {
package require Mk4tcl
lappend g_ls_dataprovider "TLFDataProviderMetakit"
set g_db_mk_count 0		;# Metakit数据库计数器,用于自动产生标记名
class  TLFDataProviderMetakit {
	inherit TLFDataProvider
	
	constructor {} {};
	destructor {};
	
	### data member ###
	
	# 内部变量定义
	protected variable m_dbtag	""	;# 数据库Tag
	protected variable m_table	""	;# Recordset
	
	### public methods ###
	public method  Open {{cnstr ""}} {};		# 打开
	public method  Close {} {};					# 关闭
	public method  Exec { sql } {};				# 执行
	public method  Transaction { param } {};	# 事物处理
	public method  Pack { } {};	# 整理
	public method  QueryTables {{type TABLE}} {};		# 获取Table列表
	public method  QueryColumn {tablename {detail ""}} {};	# 查询表的列名
	public method  CreateTable {tablename fields} {};	# 创建表
	public method  DropTable {tablename} {};	# 删除表
	public method  ClearTable {tablename} {};	# 清空表
	public method  GetRecords {tablename {startrow ""} {endrow ""} {props {}}} {};	# 获取指定记录
	public method  InsertRecord {tablename rowindex props values} {};	# 添加记录
	public method  DeleteRecords {tablename startrow {endrow ""}} {};	# 删除记录
	public method  SetRecord {tablename rowindex props values} {};	# 设置记录
}
}

#-------------------------------------------------------------
#	constructor
#-------------------------------------------------------------
body TLFDataProviderMetakit::constructor {} {
	chain;
	
	set _name "Metakit";
	set _version "1.0";
}

#-------------------------------------------------------------
#	destructor
#-------------------------------------------------------------
body TLFDataProviderMetakit::destructor {} {
	Close
}

#-------------------------------------------------------------
#  Open Database
#-------------------------------------------------------------
body TLFDataProviderMetakit::Open {{cnstr ""}} {
	global g_db_mk_count
	
	# 关闭连接
	Close
	# 建立连接
	if {$cnstr == ""} {
		set ret [catch {set cnstr [FileDlg -ext "mk" -filter "Metakit files(*.mk)|*.mk|All files(*.*)|*.*||"]} msg]
		if {$ret} {
			error "Open database file failed, because: $msg"
		}
	}

	set m_dbtag db_mk_$g_db_mk_count
	incr g_db_mk_count
	
	set ret [catch {mk::file open $m_dbtag $cnstr} msg]
	if {$ret} {
		error "$msg\nOpen database failed, because: $msg"
	}
	
	set _connect $cnstr
	
	return
}

#-------------------------------------------------------------
#  Close Database
#-------------------------------------------------------------
body TLFDataProviderMetakit::Close {} {
	catch {
		mk::file commit $m_dbtag
		mk::file close $m_dbtag
	}
	return
}

#-------------------------------------------------------------
#  Exec SQL
#-------------------------------------------------------------
body TLFDataProviderMetakit::Exec { sql } {
	set m_rowcount 0
	
	if {[string first "select " $sql] == 0} {
		set sql [string range $sql 7 end]
	}
	set sql [string trimleft $sql]
	
	if {[regexp {(.*?)\s+from\s+(\S+)\s*.*$} $sql all fields tablename]} {
		# 标准SQL模式
		set m_table $tablename
		
		set mkcmd "mk::select $m_dbtag.$m_table"
	} else {
		# Metakit select模式
		set pos1 [string first " " $sql]
		if {$pos1 != -1} {
			set m_table [string range $sql 0 [expr $pos1 - 1]]
			set m_rowcount [mk::view size $m_dbtag.$m_table]
		}
		
		set mkcmd "mk::select $m_dbtag.$sql"
	}
	
	# 执行查询
	set m_data {}
	foreach i [eval $mkcmd] {
		set line {}
		foreach {name value} [mk::get $m_dbtag.${m_table}!$i] {
			lappend line $value
		}
		
		lappend m_data $line
    }
	
	# 创建并返回数据列表
	return $m_data
}

#-------------------------------------------------------------
#  Transaction
#-------------------------------------------------------------
body TLFDataProviderMetakit::Transaction { param } {
}

#-------------------------------------------------------------
#  Pack database
#-------------------------------------------------------------
body TLFDataProviderMetakit::Pack {} {
}

#-------------------------------------------------------------
#  Query Tables
#-------------------------------------------------------------
body TLFDataProviderMetakit::QueryTables {{type TABLE}} {
	set lsTable [mk::view info $m_dbtag]
	
	set data {}
	foreach table $lsTable {
		if {[string first ":V" $table] != -1} {
			regsub {:V} $table {} table
			lappend data $table
		}
	}
	
	return $data
}

#-------------------------------------------------------------
#  Query one table's all column information
#-------------------------------------------------------------
body TLFDataProviderMetakit::QueryColumn {tablename {detail ""}} {
	set lsColumn [mk::view info $m_dbtag.$tablename]
	#puts "==[mk::view layout $m_dbtag.$tablename]=="
	set data {}
	foreach column $lsColumn {
		if {$detail == "-detail"} {
			lappend data [list $column TEXT 0 {} 0]
		} else {
			lappend data $column
		}
	}
	
	return $data
}

#-------------------------------------------------------------
#  Create new table
#-------------------------------------------------------------
body TLFDataProviderMetakit::CreateTable {tablename fields} {
	set mkcmd "mk::view layout $m_dbtag.$tablename {"
	#puts $fields
	foreach field $fields {
		set field_name [lindex $field 0]
		if {$field_name == ""} {
			continue;
		}
		
		set field_type [string toupper [lindex $field 1]]
		if {($field_type == "") || ([string first "TEXT" $field_type] == 0) || ([string first "VARCHAR" $field_type] == 0)} {
			set field_type "S"
		} elseif {[string first "INT" $field_type] == 0} {
			set field_type "I"
		} else {
			set field_type "S"
		}
		
		set mkcmd "$mkcmd $field_name:$field_type"
	}
	
	set mkcmd "$mkcmd }"
	
	eval $mkcmd
}

#-------------------------------------------------------------
#  Drop table
#-------------------------------------------------------------
body TLFDataProviderMetakit::DropTable {tablename} {
	mk::view delete $m_dbtag.$tablename
}

#-------------------------------------------------------------
#  Clear table
#-------------------------------------------------------------
body TLFDataProviderMetakit::ClearTable {tablename} {
	mk::view size $m_dbtag.$tablename 0
}

#-------------------------------------------------------------
#  Get records
#-------------------------------------------------------------
body TLFDataProviderMetakit::GetRecords {tablename {startrow ""} {endrow ""} {props {}}} {
	if {[llength $props] == 0} {
		set lsColumn [mk::view info $m_dbtag.$tablename]
	} else {
		set lsColumn $props
	}
	
	set rowcount 1
	if {$endrow == "end"} {
		set rowcount [mk::view size $m_dbtag.$tablename]
	} elseif {$endrow != ""} {
		set rowcount [expr $endrow - $startrow + 1]
	}
	
	set mkcmd "mk::select $m_dbtag.$tablename -first $startrow -count $rowcount"
	
	# 执行查询
	set m_data {}
	foreach i [eval $mkcmd] {
		lappend m_data [eval "mk::get $m_dbtag.$tablename!$i $lsColumn"]
    }
	
	# 创建并返回数据列表
	return $m_data
}

#-------------------------------------------------------------
#  Insert record
#-------------------------------------------------------------
body TLFDataProviderMetakit::InsertRecord {tablename rowindex props values} {
	set cursor [mk::row create]
	if {($rowindex == "end") || ($rowindex == "")} {
		set rowindex [mk::view size $m_dbtag.$tablename]
		mk::row append $m_dbtag.$tablename
	} elseif {$rowindex == "unique"} {
		#判断指定属性的唯一性,如果存在就退出
		set sql "mk::select $m_dbtag.$tablename -glob"
		set props_new {}
		foreach prop $props value $values {
			if {[string first ":unique" $prop] != -1} {
				regsub {:unique} $prop {} prop
				if {[string is double -strict $value]} {
					set sql "$sql $prop $value"
				} else {
					set sql "$sql $prop \"[string map {\" \"\"} $value]\""
				}
			}
			lappend props_new $prop
		}
		set props $props_new
		set lsFind [eval $sql]
		if {[llength $lsFind] != 0} {
			return
		}
		
		set rowindex [mk::view size $m_dbtag.$tablename]
		mk::row append $m_dbtag.$tablename
	} else {
		mk::row insert $m_dbtag.$tablename!rowindex 1
	}
	
	SetRecord $tablename $rowindex $props $values
}

#-------------------------------------------------------------
#  Delete records
#-------------------------------------------------------------
body TLFDataProviderMetakit::DeleteRecords {tablename startrow {endrow ""}} {
	set rowcount 1
	if {$endrow == "end"} {
		set rowcount [mk::view size $m_dbtag.$tablename]
	} elseif {$endrow != ""} {
		set rowcount [expr $endrow - $startrow + 1]
	}
	
	mk::row delete $m_dbtag.$tablename!$startrow $rowcount
}

#-------------------------------------------------------------
#  Set record
#-------------------------------------------------------------
body TLFDataProviderMetakit::SetRecord {tablename rowindex props values} {
	if {[llength $props] == 0} {
		set lsColumn [mk::view info $m_dbtag.$tablename]
	} else {
		set lsColumn $props
	}
	
	foreach prop $lsColumn value $values {
		#puts "--$tablename,$rowindex,$prop,$value."
		mk::set $m_dbtag.$tablename!$rowindex $prop $value
	}
}

#-------------------------------------------------------------
# TLFDataProviderSqlite class define
#-------------------------------------------------------------
if {[itcl::find classes TLFDataProviderSqlite] != "TLFDataProviderSqlite"} {
package require sqlite3
lappend g_ls_dataprovider "TLFDataProviderSqlite"
set g_db_sqlite_count 0		;# Sqlite数据库计数器,用于自动产生标记名
class  TLFDataProviderSqlite {
	inherit TLFDataProvider
	
	constructor {} {};
	destructor {};
	
	### data member ###
	
	# 内部变量定义
	protected variable m_dbtag	""	;# 数据库Tag
	protected variable m_table	""	;# Recordset
	
	### public methods ###
	public method  Open {{cnstr ""}} {};		# 打开
	public method  Close {} {};					# 关闭
	public method  Exec { sql } {};				# 执行
	public method  Transaction { param } {};	# 事物处理
	public method  Pack { } {};	# 整理
	public method  QueryTables {{type TABLE}} {};		# 获取Table列表
	public method  QueryColumn {tablename {detail ""}} {};	# 查询表的列名
	public method  CreateTable {tablename fields} {};	# 创建表
	public method  DropTable {tablename} {};	# 删除表
	public method  ClearTable {tablename} {};	# 清空表
	public method  GetRecords {tablename {startrow ""} {endrow ""} {props {}}} {};	# 获取指定记录
	public method  InsertRecord {tablename rowindex props values} {};	# 添加记录
	public method  DeleteRecords {tablename startrow {endrow ""}} {};	# 删除记录
	public method  SetRecord {tablename rowindex props values} {};	# 设置记录
}
}

#-------------------------------------------------------------
#	constructor
#-------------------------------------------------------------
body TLFDataProviderSqlite::constructor {} {
	chain;
	
	set _name "Sqlite";
	set _version "1.0";
}

#-------------------------------------------------------------
#	destructor
#-------------------------------------------------------------
body TLFDataProviderSqlite::destructor {} {
	Close
}

#-------------------------------------------------------------
#  Open Database
#-------------------------------------------------------------
body TLFDataProviderSqlite::Open {{cnstr ""}} {
	global g_db_sqlite_count
	
	# 关闭连接
	Close
	# 建立连接
	if {$cnstr == ""} {
		set ret [catch {set cnstr [FileDlg -ext "s3db" -filter "Sqlite3 files(*.s3db)|*.s3db|Sqlite files(*.db)|*.db|All files(*.*)|*.*||"]} msg]
		if {$ret} {
			error "Open database file failed, because: $msg"
		}
	}

	set m_dbtag db_sqlite_$g_db_sqlite_count
	incr g_db_sqlite_count
	
	set ret [catch {sqlite3 $m_dbtag $cnstr} msg]
	if {$ret} {
		error "$msg\nOpen database failed, because: $msg"
	}
	
	set _connect $cnstr
	
	return
}

#-------------------------------------------------------------
#  Close Database
#-------------------------------------------------------------
body TLFDataProviderSqlite::Close {} {
	catch {$m_dbtag close}
	return
}

#-------------------------------------------------------------
#  Exec SQL
#-------------------------------------------------------------
body TLFDataProviderSqlite::Exec { sql } {
	set m_rowcount 0
	set m_data {}
	
	# 执行查询
	set ret [catch { $m_dbtag eval $sql values {
		#parray values
		set row {}
		foreach column $values(*) {
			lappend row $values($column)
		}
		lappend m_data $row
	} } msg]
	if {$ret} {
		error "$msg\nExec SQL failed:\n$sql"
	}
	
	set m_rowcount [llength $m_data]
		
	# 创建并返回数据列表
	return $m_data
}

#-------------------------------------------------------------
#  Transaction
#-------------------------------------------------------------
body TLFDataProviderSqlite::Transaction { param } {
	catch { $m_dbtag eval $param transaction }
}

#-------------------------------------------------------------
#  Pack database
#-------------------------------------------------------------
body TLFDataProviderSqlite::Pack {} {
	catch { $m_dbtag eval VACUUM }
}

#-------------------------------------------------------------
#  Query Tables
#-------------------------------------------------------------
body TLFDataProviderSqlite::QueryTables {{type TABLE}} {
	set lsTable {}
	
	set type [string tolower $type]
	set sql "SELECT * FROM sqlite_master WHERE type='$type'"
	set data [Exec $sql]
	
	foreach table $data {
		lappend lsTable [lindex $table 1]
	}
	
	return $lsTable
}

#-------------------------------------------------------------
#  Query one table's all column information
#-------------------------------------------------------------
body TLFDataProviderSqlite::QueryColumn {tablename {detail ""}} {
	set lsColumn {}
	
	set sql "PRAGMA table_info($tablename)"
	set lsColumn [Exec $sql]
	
	set data {}
	foreach column $lsColumn {
		if {$detail == "-detail"} {
			lappend data [lrange $column 1 end]
		} else {
			lappend data [lindex $column 1]
		}
	}
	
	return $data
}

#-------------------------------------------------------------
#  Create new table
#-------------------------------------------------------------
body TLFDataProviderSqlite::CreateTable {tablename fields} {
	set sql "CREATE TABLE $tablename\("
	set field_count 0
	foreach field $fields {
		set field_name [lindex $field 0]
		if {$field_name == ""} {
			continue;
		}
		
		set field_type [string toupper [lindex $field 1]]
		set field_size [lindex $field 2]
		set field_default [lindex $field 3]
		if {[regexp {(|)} $field_default] == 1} {
			# 过滤掉有括号的值,这种值表示执行函数,暂不支持
			set field_default ""
		}
		
		set field_notnull ""
		if {[lsearch [lrange $field 4 end] "notnull"] != -1} {
			set field_notnull "notnull"
		}
		
		set field_extend ""
		if {[lsearch [lrange $field 4 end] "AUTO_INCREMENT"] != -1} {
			set field_extend "AUTO_INCREMENT"
		}
		
		set field_key ""
		if {[lsearch [lrange $field 4 end] "primary"] != -1} {
			set field_key primary
		} elseif {[lsearch [lrange $field 4 end] "index"] != -1} {
			set field_key index
		} elseif {[lsearch [lrange $field 4 end] "unique"] != -1} {
			set field_key unique
		}
		
		if {$field_count > 0} {
			set sql "$sql ,"
		}
		set sql "$sql $field_name $field_type"
		if {($field_size != "") && ($field_size != "0")} {
			set sql "$sql\($field_size\)"
		}
		if {$field_notnull != ""} {
			set sql "$sql NOT NULL"
		}
		if {$field_default != ""} {
			if {[lsearch $field_type {"TEXT" "LONGTEXT" "VARCHAR"}] != -1} {
				set sql "$sql DEFAULT '$field_default'"
			} else {
				set sql "$sql DEFAULT $field_default"
			}		
		}
		if {$field_extend == "AUTO_INCREMENT" } {
			set sql "$sql AUTONUMBER"
		}
		switch $field_key {
			primary	{ set sql "$sql PRIMARY KEY" }
			index	{}
			unique	{}
		}
		
		incr field_count
	}
	
	set sql "$sql \)"
	
	Exec $sql
}

#-------------------------------------------------------------
#  Drop table
#-------------------------------------------------------------
body TLFDataProviderSqlite::DropTable {tablename} {
	set sql "DROP TABLE $tablename"
	Exec $sql
}

#-------------------------------------------------------------
#  Clear table
#-------------------------------------------------------------
body TLFDataProviderSqlite::ClearTable {tablename} {
	set sql "DELETE FROM $tablename"
	Exec $sql
}

#-------------------------------------------------------------
#  Get records
#-------------------------------------------------------------
body TLFDataProviderSqlite::GetRecords {tablename {startrow ""} {endrow ""} {props {}}} {
	if {[llength $props] == 0} {
		set lsColumn [QueryColumn $tablename]
	} else {
		set lsColumn $props
	}
	
	set sql "SELECT"
	if {[llength $props] == 0} {
		set sql "$sql *"
	} else {
		set sql "$sql [join $lsColumn ,]"
	}
	set sql "$sql FROM $tablename"
	
	if {$startrow == 0} {
		set startrow ""
	}
	if {$endrow == "end"} {
		set endrow ""
	}
	
	if {($startrow != "") || ($endrow != "")} {
		set sql "$sql WHERE"
		if {$startrow != ""} {
			if {[string match {[0-9]*} $startrow]} {
				set sql "$sql rowid>=$startrow"
			} else {
				set sql "$sql $startrow"
			}
		}
		if {($startrow != "") && ($endrow != "")} {
			set sql "$sql AND"
		}
		if {($endrow != "") && ($endrow != "end")} {
			if {[string match {[0-9]*} $endrow]} {
				set sql "$sql rowid<=$endrow"
			} else {
				set sql "$sql $endrow"
			}
		}
	}
	
	return [Exec $sql]
}

#-------------------------------------------------------------
#  Insert record
#-------------------------------------------------------------
body TLFDataProviderSqlite::InsertRecord {tablename rowindex props values} {
	if {$rowindex == "unique"} {
		#判断指定属性的唯一性,如果存在就退出
		set sql "SELECT * FROM $tablename WHERE"
		set props_new {}
		set sep " "
		foreach prop $props value $values {
			if {[string first ":unique" $prop] != -1} {
				regsub {:unique} $prop {} prop
				if {[string is double -strict $value]} {
					append sql $sep$prop=$value
				} else {
					append sql $sep$prop='[string map {' ''} $value]'
				}
				set sep " AND "
			}
			lappend props_new $prop
		}
		set props $props_new
		set lsFind [Exec $sql]
		if {[llength $lsFind] != 0} {
			return
		}
	}

	set sql "INSERT INTO '[string map {' ''} $tablename]' VALUES"
    set sep (
	set rowid $rowindex
	set ncol [llength $values]
    for {set i 0} {$i<$ncol} {incr i} {
		set val [lindex $values $i]
		if {[string is double -strict $val]} {
			append sql $sep$val
		} else {
			append sql $sep'[string map {' ''} $val]'
		}
		set sep ,
    }
    append sql )
	
	Exec $sql
}

#-------------------------------------------------------------
#  Delete records
#-------------------------------------------------------------
body TLFDataProviderSqlite::DeleteRecords {tablename startrow {endrow ""}} {
	set sql "DELETE FROM $tablename"
	
	if {$startrow == 0} {
		set startrow ""
	}
	if {$endrow == "end"} {
		set endrow ""
	}
	
	if {($startrow != "") || ($endrow != "")} {
		set sql "$sql WHERE"
		if {$startrow != ""} {
			if {[string match {[0-9]*} $startrow]} {
				set sql "$sql rowid>=$startrow"
			} else {
				set sql "$sql $startrow"
			}
		}
		if {($startrow != "") && ($endrow != "")} {
			set sql "$sql AND"
		}
		if {($endrow != "") && ($endrow != "end")} {
			if {[string match {[0-9]*} $endrow]} {
				set sql "$sql rowid<=$endrow"
			} else {
				set sql "$sql $endrow"
			}
		}
	}
	
	Exec $sql
}

#-------------------------------------------------------------
#  Set record
#-------------------------------------------------------------
body TLFDataProviderSqlite::SetRecord {tablename rowindex props values} {
	set sql "UPDATE $tablename "
    set sep {SET}
    set rowid $rowindex
	set ncol [llength $values]
    for {set i 0} {$i<$ncol} {incr i} {
		set val [lindex $values $i]
		set name [lindex $props $i]
		append sql "$sep $name="
		if {[string is double -strict $val]} {
			append sql $val
		} else {
			append sql '[string map {' ''} $val]'
		}
		set sep ,
    }
	
	if {[string match {*[<>=]*} $rowid]} {
		append sql " WHERE $rowid"
	} else {
		append sql " WHERE rowid=$rowid"
	}
	
	Exec $sql
}
