    namespace export mount unmount archive

    interp alias {} [namespace current]::write {} puts -nonewline

    proc AppendDirToArchive {dir toChan toFile} {
	array set index {}
	foreach link [glob -nocomplain -tails -types l -directory $dir *] {
	    set index($link) [list L [file readlink [file join $dir $link]]]
	}

	foreach file [glob -nocomplain -tails -types {r f} -directory $dir *] {
	    set path [file join $dir $file]
	    if {[file type $path] eq "link"} {
		continue
	    }
	    set normFile [file normalize $path]
	    if {[string equal $toFile $normFile]} {
		# Do not recursively archive the archive...
		continue
	    }
	    set start [tell $toChan]
	    set fromChan [open $path r]
	    fconfigure $fromChan -translation binary
	    fcopy $fromChan $toChan
	    close $fromChan
	    set end [tell $toChan]
	    set index($file) [list F [expr {wide($end) - wide($start)}] $start]
	}

	foreach sub [glob -nocomplain -tails -types {r d} -directory $dir *] {
	    set path [file join $dir $sub]
	    if {[file type $path] eq "link"} {
		continue
	    }
	    set start [AppendDirToArchive $path $toChan $toFile]
	    set end [tell $toChan]
	    set index($sub) [list D [expr {wide($end) - wide($start)}] $start]
	}

	# Record offset for start of index
	set start [tell $toChan]
	# fixup the offsets relative to the index start
	foreach {key value} [array get index] {
	    if {[llength $value] < 3} continue
	    set index($key) [lreplace $value 2 2 \
		    [expr {wide($start) - wide([lindex $value 2])}]]
	}
	fconfigure $toChan -encoding utf-8
	write $toChan [array get index]
	fconfigure $toChan -encoding binary
	return $start
    }

    proc archive {directory archive} {
	if {0 != [catch {
	    set f [open $archive a+]
	    fconfigure $f -translation binary
	    seek $f 0 end
	} result]} {
	    return -code error $result
	}
	write $f \u001A
	set start [AppendDirToArchive $directory $f [file normalize $archive]]
	set end [tell $f]
	write $f \u001Atrofs01
	write $f [binary format I [expr {wide($end) - wide($start)}]]
	close $f
    }
