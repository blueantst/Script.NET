#################################################################
# File       : remoterun.tcl
# Author     : Blueant Software
# Version    : 1.0
# Date       : 2001-02-04
# Description: Script.NET remote run protocal script
#################################################################

package require ftp;

#-------------------------------------------------------------
# Main Program
#-------------------------------------------------------------

# 赋变量初值
set username "";
set password "";
set address "";
set filename "";
set bLogin "false";

# 解析命令行参数
if {$argc > 0} {
	set argv1 [lindex $argv 0];
	if {[string first "@" $argv1] == -1} {
		regexp {([\w\.]*)[\\/]?(.*)} $argv1 all address filename;
	} else {
		regexp {(\w*)@*([\w\.]*)[\\/]?(.*)} $argv1 all username address filename;
	}
	#puts "username=$username,address=$address,filename=$filename";
}

# 缺省的文件名为index.ral
if {$filename == ""} {
	set filename "index.ral";
}

# 设置缺省用户名为匿名用户
if {$username == ""} {
	set username "Anonymous";
}

# 如果是匿名用户则测试此地址是否可以打开
if {$username == "Anonymous" && $address != ""} {
	if { [ catch {
		set handle [ftp::Open $address $username $password];
	} errorInfo] } {
		puts $errorInfo;
	} else {
		set bLogin "true";
	}
}

# 如果未能登录则输入登录参数
if {$bLogin == "false"} {
	set res [tclLogin -title "登录RemoteRun服务器" \
			-address address $address \
			-username username $username \
			-password password $password \
			-domain filename $filename \
			-check rememberpwd "记忆口令" false \
			-laddress "服 务 器：" \
			-ldomain  "下载文件："];
	if {$res != 1} {
		# 用户cancel此次连接
		return;
	} else {
		# 登录服务器
		set handle [ftp::Open $address $username $password];
	}
}

# 缺省的文件名为index.ral
if {$filename == ""} {
	set filename "index.ral";
}

# 解析本地文件名
regsub -all {\\} $filename {/} filename;
#set path "";
set lfilename $filename;
# 去掉路径,只保留文件名
set index [string last "/" $lfilename];
if {$index != -1} {
	set lfilename [string range $lfilename [expr $index+1] end];
	#set path [string range $filename 0 [expr $index-1]];
}
set localFile "$platform_path/option/temp/$lfilename";

# 下载文件
#ftp::Cd $handle $path;
if { [catch { ftp::Get $handle $filename $localFile; } errInfo ] } {
	MessageBox -MB_OK -MB_ICONERROR -title "下载错误" "从服务器下载文件 $filename 失败,原因为:\r\n$errInfo!";
	return "";
}

#-------------------------------------------------------------
# 下载ral列表过程中的cancel处理
#-------------------------------------------------------------
set bRalCancel 0;
proc ralCancel {} {
	global bRalCancel;
	set bRalCancel 1;
}

# 处理文件名
regsub -all "/" $localFile "\\" localFile;
set ext [string range $localFile end-3 end];

if {[string tolower $ext] == ".ral"} {
	catch {global bRalCancel};
	
	# ral文件为远程地址清单文件
	set fral [open $localFile r];
	set nLineCount 0;
	while {[gets $fral line] >= 0} {
		incr nLineCount;
	}
	if {$nLineCount == 0} { set nLineCount 1; }
	seek $fral 0;

	#set nPaneID [StatusBar -createpane -width 150];
	#StatusBar -progress -Attach $nPaneID "进度:" $nLineCount;
	ProgressDlg create "下载-$lfilename" "文件:" $nLineCount "ralCancel";
	set nLineIndex 0;

	while {$bRalCancel == 0 && [gets $fral line] >= 0} {
		if {[string length $line] > 0 && [string first "#" $line] != 0} {
			# 过滤掉注释行和空行
			
			# 检查文件下载权限
			if {[regexp {\[(\d)\]} $line all rights]} {
				set userLogin [user -current];
				regexp {\((\d)\)} $userLogin all userRights;
				if {[expr $userRights > $rights]} {
					#StatusBar -progress -Pos $nPaneID $nLineIndex;
					ProgressDlg setpos $nLineIndex;
					incr nLineIndex;
					continue;
				}
				regsub {\[(\d)\]} $line {} line;
			}			

			#StatusBar -settext 0 $line;
			ProgressDlg setstatus $line;

			set bRetFile 0;
			set bFile 1;
			if {[string first "::" $line] == 0} {
				# ::开头,表示后面是最终返回的文件名
				eval "set localFile [string range $line 2 end]";
				continue;
			} elseif {[string first ":" $line] == 0} {
				# :开头,表示后面下载的文件是最终执行的
				set bRetFile 1;
				set line [string range $line 1 end];
			} elseif {[string first "%" $line] == 0} {
				# %开头,表示需要创建一个目录
				set line [string range $line 1 end];
				eval "set sdir $line";
				catch {
				file mkdir $sdir;
				}
				set bFile 0;
			}
			if {($bFile == 1) && ([string first ">" $line] > 0)} {
				# 如果为'filesrc > filedesc'的形式,表示将filesrc下载为本地filedesc
				set index [string first ">" $line];
				set filesrc [string range $line 0 [expr $index-1]];
				set filedesc [string range $line [expr $index+1] end];
				set filesrc [string trim $filesrc];
				set filedesc [string trim $filedesc];
				# filedesc支持本地tcl变量的替换
				eval "set filedesc $filedesc";
				catch {
				ftp::Get $handle $filesrc $filedesc;
				}
			} elseif {$bFile == 1} {
				# 直接的文件名,表示下载到本地的temp目录
				set lfilename $line;
				# 去掉路径,只保留文件名
				set index [string last "/" $lfilename];
				if {$index != -1} {
					set lfilename [string range $lfilename [expr $index+1] end];
				}
				set filedesc "$platform_path/option/temp/$lfilename";
				catch {
				ftp::Get $handle $line $filedesc;
				}
			}
			if {$bRetFile == 1} {
				set localFile $filedesc;
				if {[string first ".tcl" $localFile] > 0} {
					set localFile "tcl:$localFile";
				} elseif {[string first ".py" $localFile] > 0} {
					set localFile "python:$localFile";
				}
			}
		}

		#StatusBar -progress -Pos $nPaneID $nLineIndex;
		ProgressDlg setpos $nLineIndex;
		incr nLineIndex;
	}

	#StatusBar -progress -Hide $nPaneID;
	#StatusBar -delpane $nPaneID;
	ProgressDlg end;

	close $fral;
}

ftp::Close $handle;

# 用户cancel则直接退出
if {$bRalCancel == 1} {
	return;
}

# 返回下载的本地文件名
if {[string tolower $ext] == ".tcl"} {
	result "tcl:$localFile";
} elseif {[string tolower $ext] == ".py"} {
	result "python:$localFile";
} else {
	result "$localFile";
}
