
# @@ Meta Begin
# Package Itcl 3.4
# Meta activestatetags ActiveTcl Public
# Meta as::author      {Michael McLennan}
# Meta as::build::date 2010-09-10
# Meta as::origin      http://sourceforge.net/projects/incrTcl
# Meta category        Object oriented programming
# Meta description     {[incr Tcl]} is an O-O system for Tcl. The name is a
# Meta description     play on C++, and {[incr Tcl]} provides a similar
# Meta description     object model, including mulitple inheritance, and
# Meta description     public and private classes and variables.
# Meta license         BSD
# Meta platform        win32-ix86
# Meta require         {Tcl 8.4}
# Meta subject         OO Objects Classes Inheritance Methods
# Meta summary         Object-oriented package
# @@ Meta End


if {![package vsatisfies [package provide Tcl] 8.4]} return

package ifneeded Itcl 3.4 [string map [list @ $dir] {
        # ACTIVESTATE TEAPOT-PKG BEGIN REQUIREMENTS

        package require Tcl 8.4

        # ACTIVESTATE TEAPOT-PKG END REQUIREMENTS

            set ::env(ITCL_LIBRARY) {@}
            load [file join {@} itcl34.dll] Itcl

        # ACTIVESTATE TEAPOT-PKG BEGIN DECLARE

        package provide Itcl 3.4

        # ACTIVESTATE TEAPOT-PKG END DECLARE
    }]
