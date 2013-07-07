#################################################################
# iconreplace.tcl
# Author     : blueant
# Version    : 1.0
# Date       : 2007年5月22日
# Description: Execute File Icon Replace
#################################################################

#-------------------------------------------------------------
#  read file and return its contents.
#-------------------------------------------------------------
proc _readFile {name} {
	set fd [open $name]
	fconfigure $fd -translation binary
	set data [read $fd]
	close $fd
	return $data
}

#-------------------------------------------------------------
#  write data to file
#-------------------------------------------------------------
proc _writeFile {name data} {
	set fd [open $name w]
	fconfigure $fd -translation binary
	puts -nonewline $fd $data
	close $fd
}

#-------------------------------------------------------------
#  Decode icon file
#-------------------------------------------------------------
proc _decICO {data} {
	set result {}
	binary scan $data sss - type count
	for {set pos 6} {[incr count -1] >= 0} {incr pos 16} {
		binary scan $data @${pos}ccccssii w h cc - p bc bir io
		if {$cc == 0} {
			set cc 256
			# 注:颜色大于256之后,cc都为0,此时只能用bc进行区分
			if {$bc > 8} {
				set cc "bc$bc"
			} 
		}
		binary scan $data @${io}a$bir image
		lappend result ${w}*${h}/$cc $image
	}
	return $result
}

#-------------------------------------------------------------
#  Replace icon file
#-------------------------------------------------------------
proc IconReplace {filename newicon fwname} {
	set rtnval {}
	if {[file normalize $filename] == [info nameofexec]} {
		return "File is in use, can't be prefix: [file normalize $filename]"
	}
	set out $filename
	
	# we only look for set an icon if the runtime is called *.exe
	catch { set origicon [_readFile $fwname] }
	
	if {[info exists origicon] && [file exists $newicon]} {
		set header [_readFile $filename]
		set custicon [_readFile $newicon]
		array set newimg [_decICO $custicon]
		set kvlist [_decICO $origicon]
		#puts "==[array names newimg]"
		foreach {k v} $kvlist {
			#puts "--$k, len=[string length $v]"
			if {[info exists newimg($k)]} {
				set len [string length $v]
				set pos [string first $v $header]
				if {$pos < 0} {
					lappend rtnval "icon $k: NOT FOUND"
				} elseif {[string length $newimg($k)] != $len} {
					lappend rtnval "icon $k: NOT SAME SIZE!,len=[string length $newimg($k)]"
				} else {
					binary scan $header a${pos}a${len}a* prefix - suffix
					set header "$prefix$newimg($k)$suffix"
					#puts "++$k, set header"
				}
			} else {
				lappend rtnval "icon $k: NOT FOUND!"
			}
		}
		_writeFile $out $header
	}
	
	return $rtnval	
}

#set filename [FileDlg -?]
#set newicon [FileDlg -?]
#set fwname [FileDlg -?]

#set ret [IconReplace $filename $newicon $fwname]
#puts $ret
