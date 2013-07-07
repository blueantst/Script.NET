#################################################################
# File       : Build.tcl
# Author     : Blueant Software
# Version    : 1.0
# Date       : 2007-03-10
# Description: Script.NET Tcl Build script
#################################################################

global interp_path
set tclkit "$interp_path/bin/tclkit-8.4.13.exe"
set tclkit2 "$interp_path/bin/tclkit2.exe"
set tclsdx "$interp_path/bin/sdx.kit"

set f [FileDlg -ext "tcl" -filter "tcl File(*.tcl)|*.tcl|All Files(*.*)|*.*||"];
if { $f != "" } {
	puts "===================================="
	puts "build tcl file: $f"
	if {[catch { puts [exec "$tclkit" "$tclsdx" qwrap "$f"]; } errinfo]} {
		puts $errinfo;
		return
	}
	
	regsub {\.tcl} $f {.kit} f
	puts $f
	if {[catch { puts [exec "$tclkit" "$tclsdx" unwrap "$f"]; } errinfo]} {
		puts $errinfo;
		return
	}
	
	regsub {\.kit} $f {.exe} f
	puts $f
	if {[catch { puts [exec "$tclkit" "$tclsdx" wrap "$f" -runtime "$tclkit2"]; } errinfo]} {
		puts $errinfo;
		return
	}
}
