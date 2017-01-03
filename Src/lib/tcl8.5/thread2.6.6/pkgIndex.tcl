# -*- tcl -*-
# Tcl package index file, version 1.1
#
if {[package vsatisfies [package provide Tcl] 8.4]} {

    package ifneeded Thread 2.6.6 [list load [file join $dir thread266.dll]]

    if {[llength [info commands apply]]} {
	# We can use a lambda (anon function).

	package ifneeded Ttrace 2.6.6 [list ::apply {{dir} {
	    if {[info exists ::env(TCL_THREAD_LIBRARY)] &&
		[file readable $::env(TCL_THREAD_LIBRARY)/ttrace.tcl]} {
		source $::env(TCL_THREAD_LIBRARY)/ttrace.tcl
	    } elseif {[file readable [file join $dir .. lib ttrace.tcl]]} {
		source [file join $dir .. lib ttrace.tcl]
	    } elseif {[file readable [file join $dir ttrace.tcl]]} {
		source [file join $dir ttrace.tcl]
	    }
	    if {[llength [info commands ttrace::update]]} {
		ttrace::update
	    }
	}} $dir]
    } else {
	# No anon functions available, go with the necessary evil of a
	# named procedure, but use package specific prefix and no
	# hardwired data changing between package versions.

	package ifneeded Ttrace 2.6.6 [list thread_source $dir]

	proc thread_source {dir} {
	    if {[info exists ::env(TCL_THREAD_LIBRARY)] &&
		[file readable $::env(TCL_THREAD_LIBRARY)/ttrace.tcl]} {
		source $::env(TCL_THREAD_LIBRARY)/ttrace.tcl
	    } elseif {[file readable [file join $dir .. lib ttrace.tcl]]} {
		source [file join $dir .. lib ttrace.tcl]
	    } elseif {[file readable [file join $dir ttrace.tcl]]} {
		source [file join $dir ttrace.tcl]
	    }
	    if {[llength [info commands ttrace::update]]} {
		ttrace::update
	    }
	    rename thread_source {}
	}
    }
}
