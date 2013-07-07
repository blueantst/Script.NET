package require Tk

set version 3.0

###############################################################################################
#
# VisualREGEXP -- A graphical front-end to wirte/debug regular expression
# (c) 2000-2002 Laurent Riesterer
#
# VisualREGEXP Home Page: http://laurent.riesterer.free.fr/regexp
#
#----------------------------------------------------------------------------------------------
#
# Usage: tkregexp <sampleFile>
#
#----------------------------------------------------------------------------------------------
#
# This program is free software; you can redistribute it and/or modify  
# it under the terms of the GNU General Public License as published by  
# the Free Software Foundation; either version 2 of the License, or  
# (at your option) any later version.  
#  
# This program is distributed in the hope that it will be useful,  
# but WITHOUT ANY WARRANTY; without even the implied warranty of  
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  
# GNU General Public License for more details.  
#  
# You should have received a copy of the GNU General Public License  
# along with this program; if not, write to the Free Software  
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA  
#
###############################################################################################



#----------------------------------------------------------------------------------------------
# SOME CUSTOMIZATION CAN BE DONE BY MODIFYING VARIABLES BELOW
#----------------------------------------------------------------------------------------------

# main font used to display the text
if {$tcl_platform(platform) == "windows"} {
	set font_regexp     {Courier 10}
	set font_replace    {Courier 10}
	set font_sample     {Courier 10}
} else {
	set font_regexp		9x15
	set font_replace	9x15
	set font_sample		9x15
}
# the font used in the popup menu (use ---- to get a separator, else format is {font size ?bold?}
set fonts 				{{Courier 8} {Courier 9} {Courier 10} {Courier 11} {Courier 12}
						 ----
					     {Arial 8} {Arial 9} {Arial 10} {Arial 11} {Arial 12}
					     ----
					     8x13 8x13bold 9x15 9x15bold 10x20}
# the colors for the different matching groups
set colors              {#ff0000 #0000ff darkgreen violetred #ff9000 #537db9 #e4c500     firebrick darkgoldenrod hotpink}
set bgcolors            {#ffe6e6 #e6e6ff #e6ffe6   #efd5e1   #fef3e5 #d6dce5 lightyellow white    white        white}
# use background color in sample by default ? (1 use, 0 do not use)
set background			1
# background color to visualize the non-reporting group (?:...)
set color_noreport      #fffdc4
# background color to visualize the lookhead group (?=...) and (?!...)
set color_lookahead     wheat
# show/hide help about control characters in regexp
set show_help			0
# show/hide history windows on startup
set history				0
# mode to use on startup (select/concat = raw, select/insert new lines = nl, replace = replace)
set mode				nl
# database of some regexp to appear in the "Insert regexp" menu
set regexp_db {
	"URL"			{(?:^|")(http|ftp|mailto):(?://)?(\w+(?:[\.:@]\w+)*?)(?:/|@)([^"\?]*?)(?:\?([^\?"]*?))?(?:$|")}
	"IP numbers" 	{[12]?[0-9]?[0-9](\.[12]?[0-9]?[0-9]){3}}
	"HTML tags"		{<[^<>]+>}
	"HTML tag content" {<(\w+)[^>]*?>(.*?)</\1>}
	"vars and arrays (PHP)" {\$[^0-9 ]{1}[a-zA-Z0-9_]*((?:\[[a-zA-Z0-9_'\"]+\])*)}
	"dd/mm/yyyy"	{(0[1-9]|[12][0-9]|3[01])(/|-)(0[1-9]|1[12])(/|-)[12][0-9]{3}}
	"mm/dd/yyyy"	{(0[1-9]|1[12])(/|-)(0[1-9]|[12][0-9]|3[01])(/|-)[12][0-9]{3}}
	"hh:mm"			{([01][0-9]|2[0-3]):[0-5][0-9]}
	"user@domain.net" {[A-Za-z0-9_.-]+@([A-Za-z0-9_]+\.)+[A-Za-z]{2,4}}
}

# "
#----------------------------------------------------------------------------------------------
# DO NOT MODIFY BELOW THIS POINT
#----------------------------------------------------------------------------------------------

namespace eval regexp {} {
	set data(v:undo:index) 0
	set data(v:undo:sample) ""
	set data(v:dir) "."
	set data(v:file) "untitled.txt"

	set data(v:dumpToConsole)		0;
	set data(v:dumpToClipboard)	0;
}

#----------------------------------------------------------------------------------------------
#	Main GUI
#----------------------------------------------------------------------------------------------

proc regexp::gui {} {
variable data
global colors bgcolors color_noreport color_lookahead show_help regexp_db history
global tcl_platform

	wm withdraw .;

	panedwindow	.top	-orient vertical -showhandle 1;

		# frame for regexp
		#
		labelframe .top.regexp -text "pattern" -borderwidth 2 -relief groove;
			# text for regexp entry
			#
			frame .top.regexp.pattern;

			set data(w:regexp)	[text .top.regexp.pattern.text \
				-wrap						char \
				-undo						1 \
				-background				white \
				-font						$::font_regexp \
				-selectbackground		lightblue \
				-selectborderwidth	0 \
				-width					80 \
				-height					5 \
				-borderwidth			1 \
				-yscrollcommand		[list .top.regexp.pattern.sy set] \
				-xscrollcommand		[list .top.regexp.pattern.sx set] \
			];
			scrollbar	.top.regexp.pattern.sy \
				-command			".top.regexp.pattern.text yview" \
				-orient			vertical \
				-borderwidth	1;
			scrollbar	.top.regexp.pattern.sx \
				-command			".top.regexp.pattern.text xview" \
				-orient			horizontal \
				-borderwidth	1;
			grid .top.regexp.pattern.text	.top.regexp.pattern.sy	-sticky news
			grid .top.regexp.pattern.sx	x								-sticky news
			grid columnconfigure	.top.regexp.pattern 0 -weight 1;
			grid rowconfigure		.top.regexp.pattern 0 -weight 1;

			# options
			#
			set sep	0;

			frame .top.regexp.options;

			foreach \
				option		{nocase all - line lineanchor linestop - inline} \
				label			{nocase all - line "lineanchor (k)" "linestop (m)" - inline} \
				underline	{0 0 - 0 12 10 - 0} \
			{
				if {$option != "-"} {
					checkbutton	.top.regexp.options.$option \
						-text				$label  \
						-borderwidth	1  \
						-underline		$underline \
						-variable		regexp::data(v:$option) \
						-offvalue		"" \
						-onvalue		"-$option";

					set data(v:$option)	" ";

					pack .top.regexp.options.$option -side left;
				} else {
					pack [frame .top.regexp.options.[incr sep] -width 40] -fill x -side left -expand 1;
				}
			}

			if {$tcl_platform(platform) == "windows"} {
				set sfont	{Courier 8};
				set sbfont	{Courier 8 bold};
			} else {
				set sfont	6x13;
				set sbfont	6x13bold;
			}

			set data(w:help)	[text .top.regexp.help \
				-font				$sfont \
				-borderwidth	0 \
				-height			9 \
				-wrap				none \
				-background		[.top.regexp cget -background] \
			];

			.top.regexp.help insert 1.0 "\n\n\n\n\n\n\n\n";
			.top.regexp.help insert 1.0 {\a  alert              \n     newline     \0    char 0       \d [[:digit:]]    \A beginning of the string };
			.top.regexp.help insert 2.0 {\b  backspace          \r     carriage    \xyz  octal code   \D [^[:digit:]]   \Z end of string };
			.top.regexp.help insert 3.0 {\B  synomyn for \      \t     tab                            \s [[:space:]]    \m beginning of a word};
			.top.regexp.help insert 4.0 {\cX same as X & 0x1F   \uwxyz unicode     \x    backref      \S [^[:space:]]   \M end of a word};
			.top.regexp.help insert 5.0 {\e  ESC                \v     vert tab                       \w [[:alnum:]_]   \y beginning or end of a word};
			.top.regexp.help insert 6.0 {\f  form feed          \xhhh  hexa code                      \W [^[:alnum:]_]  \Y not beginning or end of a word};
			.top.regexp.help insert 7.0 {----------------------------------------------------------------------------------------------------------------};
			.top.regexp.help insert 8.0 {    ungreedy:          ?? single optional *? zero-many       +? at least one   {n,m}? ungreedy quantifiers};
			.top.regexp.help insert 9.0 {(?:) ghost group       (?=) lookahead     (?!) neg. lookahead};

			.top.regexp.help tag configure bold -font $sbfont;

			foreach line {1 2 3 4 5 6} {
				foreach {min max} {0 2 23 25 42 44 61 63 79 82} {
					.top.regexp.help tag add bold $line.$min $line.$max;
				}
			}

			.top.regexp.help tag remove bold 2.43 2.44 4.43 4.44;

			# buttons & selection of match
			#
			frame	.top.regexp.buttons;
				button	.top.regexp.buttons.go \
					-text				"Go" \
					-underline		0 \
					-command			{
						# puts regexp
						#
						if {($regexp::data(v:dumpToConsole) == 1) ||
							 ($regexp::data(v:dumpToClipboard) == 1)} {
							regexp::dump;
						}

						regexp::go
					} \
					-borderwidth	1 \
					-width			8;
				button	.top.regexp.buttons.clear \
					-text				"Clear (z)" \
					-underline		7 \
					-command			[list regexp::clear] \
					-borderwidth	1 \
					-width			8;
			pack .top.regexp.buttons.go		-side left -padx {0 5} -pady 5;
			pack .top.regexp.buttons.clear	-side left -padx 5 -pady 5;

			# selection - buttons for match level
			#
				label	.top.regexp.buttons.sep;
				label	.top.regexp.buttons.l -text "Select:";
			pack	.top.regexp.buttons.sep -side left -fill x -expand true;
			pack	.top.regexp.buttons.l -side left -padx 5 -pady 5;

			set i	0;

			foreach c $colors t {match 1 2 3 4 5 6 7 8 9} {
				button	.top.regexp.buttons.$i \
					-text				$t \
					-foreground		$c \
					-borderwidth	1 \
					-padx				0 \
					-width			6 \
					-command			[list regexp::select $i];

				pack .top.regexp.buttons.$i -side left -fill y -pady 5;

				incr i;
			}

			# text for replace
			#
			set data(w:allreplace)	[frame .top.regexp.replace];
				frame	.top.regexp.replace.result;
					set data(w:replace) [text	.top.regexp.replace.result.text \
						-wrap						char \
						-undo						1 \
						-background				white \
						-font						$::font_replace \
						-selectbackground		lightblue \
						-selectborderwidth	0 \
						-width					1 \
						-height					2 \
						-borderwidth			1 \
						-yscrollcommand		[list .top.regexp.replace.result.sy set] \
						-xscrollcommand		[list .top.regexp.replace.result.sx set] \
					];
				scrollbar	.top.regexp.replace.result.sy \
					-command			".top.regexp.replace.result.text yview" \
					-orient			vertical \
					-borderwidth	1;
				scrollbar	.top.regexp.replace.result.sx \
					-command			".top.regexp.replace.result.text xview" \
					-orient			horizontal \
					-borderwidth	1;
				grid .top.regexp.replace.result.text	.top.regexp.replace.result.sy	-sticky news
				grid .top.regexp.replace.result.sx		x										-sticky news
				grid columnconfigure	.top.regexp.replace.result 0 -weight 1;
				grid rowconfigure		.top.regexp.replace.result 0 -weight 1;

				button	.top.regexp.replace.replace \
					-text				"Replace" \
					-underline		0 \
					-borderwidth	1 \
					-width			9 \
					-command			[list regexp::replace];

				label .top.regexp.replace.numberOfReplacements \
					-textvariable regexp::data(v:nbreplace) \
					-width			12 \
					-anchor			center \
					-relief			sunken \
					-borderwidth	2;

				set data(v:nbreplace)	"? replaced";

				pack .top.regexp.replace.result						-side left -fill both -expand true -pady 5 -padx 5
				pack .top.regexp.replace.replace						-side left -pady 5
				pack .top.regexp.replace.numberOfReplacements	-side right -fill x -pady 5 -padx 5

			# layout
			#
			pack .top.regexp.pattern	-side top -anchor w -padx 5 -pady {5 0} -expand 1 -fill both
			pack .top.regexp.options	-side top -anchor w -padx 5 -pady {0 5} -expand 0 -fill x
			pack .top.regexp.buttons	-side top -anchor w -padx 5             -expand 0 -fill x

	update;

	.top add .top.regexp -minsize [winfo reqheight .top.regexp] -sticky nesw;

		# frame for sample
		#
		labelframe .top.sample -text "sample" -borderwidth 2 -relief groove;
			frame	.top.sample.sample;
				# text for sample highlighting
				#
				set data(w:sample)	[text	.top.sample.sample.text  \
					-background				white \
					-undo						1 \
					-font						$::font_sample \
					-borderwidth			1 \
					-width					80 \
					-height					10 \
					-selectbackground		lightblue \
					-selectborderwidth	0 \
					-yscrollcommand		[list .top.sample.sample.sy set] \
					-xscrollcommand		[list .top.sample.sample.sx set] \
				];
				scrollbar	.top.sample.sample.sy \
					-command			[list .top.sample.sample.text yview] \
					-orient			vertical \
					-borderwidth	1;
				scrollbar	.top.sample.sample.sx \
					-command			[list .top.sample.sample.text xview] \
					-orient			horizontal \
					-borderwidth	1;
			grid .top.sample.sample.text	.top.sample.sample.sy	-sticky news
			grid .top.sample.sample.sx	x									-sticky news
			grid columnconfigure	.top.sample.sample 0 -weight 1;
			grid rowconfigure		.top.sample.sample 0 -weight 1;

			# set tags for colors & special
			#
			set data(v:levels)	{e0 e1 e2 e3 e4 e5 e6 e7 e8 e9};

			foreach level $data(v:levels) color $colors {
				$data(w:regexp) tag configure $level -foreground $color;
				$data(w:history) tag configure $level -foreground $color;
				$data(w:sample) tag configure $level -foreground $color;
			}

			$data(w:regexp) tag configure lookahead -background $color_lookahead;
			$data(w:regexp) tag configure noreport -background $color_noreport;
			$data(w:history) tag configure lookahead -background $color_lookahead;
			$data(w:history) tag configure noreport -background $color_noreport;

			# options
			#
			frame	.top.sample.matches;
				# button for navigation
				#
				button	.top.sample.matches.first \
					-text				"First" \
					-borderwidth	1 \
					-pady				2 \
					-width			8 \
					-command			[list regexp::sample:move -2];
				button	.top.sample.matches.previous \
					-text				"Previous" \
					-borderwidth	1 \
					-pady				2 \
					-width			8 \
					-command			[list regexp::sample:move -1];
				button	.top.sample.matches.next \
					-text				"Next" \
					-borderwidth	1 \
					-pady				2 \
					-width			8 \
					-command			[list regexp::sample:move +1];
				button	.top.sample.matches.last \
					-text				"Last" \
					-borderwidth	1 \
					-pady				2 \
					-width			8 \
					-command			[list regexp::sample:move +2];

				set data(v:mainPositions)	[list];
				set data(v:positions)		[list];
				set data(v:mainPosition)	0;
				set data(v:position)			0;

				# check, if to move to sub matches too
				#
				checkbutton	.top.sample.matches.subMatches \
					-text				"goto sub matches" \
					-borderwidth	1  \
					-underline		6 \
					-variable		regexp::data(v:subPositions) \
					-command			[list regexp::sample:subPositions];

				set data(v:subPositions)	0;

				# info for the count of matches and the current match
				#
				label	.top.sample.matches.numberOfMatches \
					-textvariable	regexp::data(v:nbmatches) \
					-anchor			center \
					-relief			sunken \
					-borderwidth	2;

				set regexp::data(v:nbmatches) "0 / 0 matches";

				# layout
				#
				pack .top.sample.matches.first -side left -fill none -expand 0 -padx {0 5};
				pack \
					.top.sample.matches.previous \
					.top.sample.matches.next \
					.top.sample.matches.last \
					.top.sample.matches.subMatches \
					-side left -fill none -expand 0 -padx 5;
				pack .top.sample.matches.numberOfMatches	-side right -fill x -expand 1 -padx {5 0};
			
			# layout
			#
			pack .top.sample.sample		-side top    -fill both -expand 1 -padx 5 -pady 5;
			pack .top.sample.matches	-side bottom -fill x    -expand 0 -padx 5 -pady 5;
		
	update;

	.top add .top.sample -minsize [winfo reqheight .top.sample] -sticky nesw;

	update;

	# main layout
	#
	pack .top -side top -fill both -expand 1 -padx 5 -pady 5;

	update;

	wm title . "Visual REGEXP $::version"
	wm minsize	. [winfo reqwidth .] [expr {[winfo reqheight .]+19}];
	wm geometry . [winfo reqwidth .]x[expr {[winfo reqheight .]+19}];

	wm deiconify .;
	grab .
	focus -force $data(w:regexp);

	# main menu
	. configure -menu .menubar
	set m [menu .menubar -tearoff 0 -borderwidth 1 -activeborderwidth 1]
	  # file
	  $m add cascade -menu $m.file -label "File" -underline 0
	  set mm [menu $m.file -tearoff 0 -borderwidth 1 -activeborderwidth 1]
		$mm add command -label "Load regexp ..." -command "regexp::regexp:load"
		$mm add command -label "Load sample ..." -command "regexp::sample:load" -accelerator "Alt-O"
		$mm add separator
		$mm add command -label "Save sample (auto) ..." -command "regexp::sample:save auto" -accelerator "Alt-S"
		$mm add command -label "Save sample Unix (lf) ..." -command "regexp::sample:save lf"
		$mm add command -label "Save sample Windows (crlf) ..." -command "regexp::sample:save crlf"
		$mm add command -label "Save sample Mac (cr) ..." -command "regexp::sample:save cr"
		$mm add separator
		$mm add command -label "Quit" -underline 0 -command "exit" -accelerator "Alt-Q"
	  # edit
	  $m add cascade -menu $m.edit -label "Edit" -underline 0
	  set mm [menu $m.edit -tearoff 0 -borderwidth 1 -activeborderwidth 1]
		$mm add command -label "Copy regexp to clipboard" -command "regexp::dump clipboard" -accelerator "Alt-C"
	  # view
	  $m add cascade -menu $m.view -label "View" -underline 0
	  set mm [menu $m.view -tearoff 0 -borderwidth 1 -activeborderwidth 1]
		set regexp::data(v:background) $::background
		regexp::sample:background
		$mm add checkbutton -label "Show background for matches" -command "regexp::sample:background" \
				-variable regexp::data(v:background)
		$mm add checkbutton -label "Show regexp help" -command "regexp::regexp:help:toggle" \
				-variable regexp::data(v:help)
		set regexp::data(v:help) $show_help
		$mm add checkbutton -label "Wrap lines in sample" -variable regexp::data(v:wrap) \
						-command "$data(w:sample) configure -wrap \$regexp::data(v:wrap)" \
						-offvalue "none" -onvalue "char"
		set regexp::data(v:history) $history
		$mm add checkbutton -label "History of Regexp" -variable regexp::data(v:history) \
						-command "if {\$regexp::data(v:history)} {wm deiconify .history} else {wm iconify .history}"
	  # select mode
	  $m add cascade -menu $m.select -label "Select/Replace mode" -underline 5
	  set mm [menu $m.select -tearoff 0 -borderwidth 1 -activeborderwidth 1]
		$mm add radiobutton -label "select / concat raw matches" \
				-variable regexp::data(v:mode) -value "raw" -command regexp::replace:toggle
		$mm add radiobutton -label "select / insert new line between matches" \
				-variable regexp::data(v:mode) -value "nl" -command regexp::replace:toggle
		$mm add radiobutton -label "replace matches" \
				-variable regexp::data(v:mode) -value "replace" -command regexp::replace:toggle
	  # insert well know regexp
	  $m add cascade -menu $m.insert -label "Insert regexp" -underline 11
	  set mm [menu $m.insert -tearoff 0 -borderwidth 1 -activeborderwidth 1]
		$mm add command -label "Make regexp ..." -command "regexp::make-regexp"
		$mm add separator
		$mm add command -label "Load patterns ..." -command "regexp::pattern:load"
		$mm add separator
		foreach {n e} $regexp_db {
			$mm add command -label "$n" -command "regexp::regexp:insert [list $e]"
		}
		set data(w:menu) $mm
	  # help
	  $m add cascade -menu $m.help -label "Help" -underline 0
	  set mm [menu $m.help -tearoff 0 -borderwidth 1 -activeborderwidth 1]
		$mm add checkbutton -label "dump regexp to console" -underline 0 -variable regexp::data(v:dumpToConsole);
		$mm add checkbutton -label "dump regexp to clipboard" -underline 0 -variable regexp::data(v:dumpToClipboard);
		$mm add separator
		$mm add command -label "tcl console" -underline 0 -command "console show";
		$mm add separator
		$mm add command -label "Help" -underline 0 -command "regexp::help"


	# key binding
	bind all <Alt-q> "exit"
	bind all <Alt-g> "regexp::go"
	bind $data(w:regexp) <Return> "regexp::go; break"
	bind all <Alt-c> "regexp::dump clipboard"
	bind all <Alt-r> "regexp::replace"
	bind all <Alt-o> "regexp::sample:load"
	bind all <Alt-s> "regexp::sample:save auto"

	bind all <Alt-a> [list .top.regexp.options.all toggle];
	bind all <Alt-n> [list .top.regexp.options.nocase toggle];
	bind all <Alt-l> [list .top.regexp.options.line toggle];
	bind all <Alt-k> [list .top.regexp.options.lineanchor toggle];
	bind all <Alt-m> [list .top.regexp.options.linestop toggle];
	bind all <Alt-i> [list .top.regexp.options.inline toggle];
	bind all <Alt-u> [list .top.sample.matches.subMatches toggle];
	bind all <Alt-z> [list regexp::clear];


	bind $data(w:sample) <Control-Tab> "$data(w:sample) insert insert {\t}; break;"

	# special for regexp Ctrl+letter = \<letter>
	#
	bind $data(w:regexp) <Control-V> "event generate $data(w:regexp) <Shift-Insert>;"
	bind $data(w:regexp) <Control-C> "event generate $data(w:regexp) <Control-Insert>;"
	bind $data(w:regexp) <Control-X> "event generate $data(w:regexp) <Shift-Delete>;"

	bind $data(w:regexp) <Control-Tab> "$data(w:regexp) insert insert {\t}; break;"
	bind $data(w:regexp) <Control-Return> "$data(w:regexp) insert insert {\n}; break;"

	foreach key {a b B e f n r t v u x 0 d D s S w W A Z m M y Y} {
		bind $data(w:regexp) <Control-$key> "$data(w:regexp) insert insert {\\$key}; break;"
	}
	foreach key {a b B e f n r t v u x 0} {
		bind $data(w:replace) <Control-$key> "$data(w:replace) insert insert {\\$key}; break;"
	}

	bind Text <Control-v> {}

	# font selection popup
	foreach w {regexp replace sample} {
		set m [menu .fonts_$w -tearoff 0]
		foreach f $::fonts {
			if { $f == "----"} {
				$m add separator
			} else {
				$m add command -label $f -command [list $data(w:$w) configure -font [list $f]];
			}
		}
		bind $data(w:$w) <3> "tk_popup $m %X %Y"
	}

	# some init
	# martin lemburg @ gmx.net - 2006-03-02
	#
	foreach {option flag} {nocase 1 all 1 line 1 lineanchor 0 linestop 0 inline 0} {
		if {$flag == 1} {
			set value	-$option;
		} else {
			set value	"";
		}

		set data(v:$option) $value;
	}
	#
	# martin lemburg @ gmx.net - 2006-03-02

	set data(v:wrap) "char"
	set regexp::data(v:mode) $::mode
	replace:toggle		;# set bindings
	regexp:help:toggle
}

proc regexp::pattern:load {{file ""}} {
variable data

	# get filename
	if {$file == ""} {
		set types [list [list "All" *]]
		set file [tk_getOpenFile -filetypes $types -parent .]
	    if {$file == ""} {
			return
		}
	}
	# do it
	set in [open $file "r"]
	$data(w:menu) delete [expr 4+[llength $::regexp_db]/2] end
	while {![eof $in]} {
		set name [gets $in]
		while {$name == ""} {
			set name [gets $in]
		}
		set pattern [gets $in]
		while {$pattern == ""} {
			set pattern [gets $in]
		}
		$data(w:menu) add command -label $name -command "regexp::regexp:insert [list $pattern]"
	}
	close $in
}


#----------------------------------------------------------------------------------------------
#	Main toplevel commands
#----------------------------------------------------------------------------------------------

proc regexp::go {} {
variable data

	set exp [$data(w:regexp) get 1.0 end-1char]
	# check if regexp is OK
	if {[catch { regexp -- $exp dummy } errMsg]} {
		tk_messageBox -type ok -icon error -message "Malformed regexp: $errMsg"
		return
	}
	regexp::regexp:colorize
	regexp::sample:colorize
	regexp::history:add
}

proc regexp::clear {} {
variable data

	regexp::history:add
	$data(w:regexp) delete 1.0 end
	regexp::go
}

proc regexp::dump {{destinations {}}} {
variable data

	# update display
	go
	# built list of options
	set dump "regexp"
	foreach option {nocase all   line lineanchor linestop   inline} {
		if {$data(v:$option) != ""} {
			append dump " $data(v:$option)"
		}
	}
	# build expression
	set exp [$data(w:regexp) get 1.0 end-1char]
	append dump " -- {$exp} string"
	# add variables if needed
	if {$data(v:inline) == ""} {
		append dump " match"
		for {set i 1} {$i < $data(v:nblevels)} {incr i} {
			append dump " v$i"
		}
	}

	# put the dump explicitely into the clipboard
	#
	if {([lsearch -exact $destinations clipboard] != -1) ||
		 (([llength $destinations] == 0) &&
		  ($data(v:dumpToClipboard) == 1))} {
		clipboard clear;
		clipboard append $dump;
	}

	if {([lsearch -exact $destinations console] != -1) ||
		 (([llength $destinations] == 0) &&
		  ($data(v:dumpToConsole) == 1))} {
		puts "$dump";
	}
}

proc regexp::select {level} {
variable data

	# update
	go
	if {[llength $data(v:result)] == 0} {
		bell
		return
	}

	# puts regexp
	
	if {($data(v:dumpToConsole) == 1) ||
		 ($data(v:dumpToClipboard) == 1)} {
		dump;
	}

	# extract matching parts in sample
	set i 0
	set newsample ""
	foreach match $data(v:result) {
		if {($i % $data(v:nblevels)) == $level} {
			set text [$data(w:sample) get \
							[$data(w:sample) index "1.0+[lindex $match 0]chars"] \
							[$data(w:sample) index "1.0+[expr [lindex $match 1]+1]chars"]]
			append newsample $text
			if {$data(v:mode) == "nl"} {
				append newsample "\n"
			}
		}
		incr i
	}
	$data(w:sample) delete 1.0 end
	$data(w:sample) insert 1.0 $newsample
	# update with regexp
	go
}

proc regexp::help {} {
global tcl_platform

	toplevel .help
	wm title .help "Help"
	# logo
	label .help.l -image logo
	pack .help.l -side top -padx 10 -pady 10
	# help text
	#
	frame .help.text;

	if {$tcl_platform(platform) == "windows"} {
		text .help.text.t -borderwidth 2 -relief groove -font {Courier 10} -yscrollcommand [list .help.text.sy set];
	} else {
		text .help.text.t -borderwidth 2 -relief groove -yscrollcommand [list .help.text.sy set];
	}

	scrollbar	.help.text.sy \
		-command			".help.text.t yview" \
		-orient			vertical \
		-borderwidth	1;

	pack .help.text.t		-side left -fill both -expand 1;
	pack .help.text.sy	-side left -fill y    -expand 0;

	pack .help.text -side top -fill both -expand 1 -padx 20

	.help.text.t tag configure bold -font "[.help.text.t cget -font] bold"
	.help.text.t insert 1.0 "Version:" bold " $::version

" normal "Usage:" bold " tkregexp <sampleFile>

" normal "Key bindings:" bold " Alt-q               exit
              Alt-a               toggle 'all' flag
              Alt-n               toggle 'nocase' flag
              Alt-l               toggle 'line' flag
              Alt-k               toggle 'lineanchor' flag
              Alt-m               toggle 'linestop' flag
              Alt-i               toggle 'inline' flag
              Alt-g               do the highlighting
              Return (in regexp)  do the highlighting

" normal "To clipboard:" bold " Put the 'regexp' command with its arguments to the clipboard

" normal "Tips:" bold " 1) To set the sample, either put a filename on the command line,
         or just copy & paste it in the sample text window.
      2) You can change the default colors or windows size by editing the
         first lines of the program file.
      3) When using the replace function, using Control-Z restore the value
         of the sample before the replacement : you try, retry, reretry, ...

" normal "Send your bug reports, suggestions or any feedback to:" bold "

	mailto:laurent.riesterer@free.fr
	http://laurent.riesterer.free.fr/regexp
" normal
	.help.text.t configure -state disabled;

	# ok button
	button .help.ok -text "Ok" -width 10 -default active -command "destroy .help"
	pack .help.ok -side bottom -pady 10
}

proc regexp::regexp:help:toggle {} {
variable data

	if {$data(v:help) == 0} {
		pack forget $data(w:help)
	} else {
		pack $data(w:help) -before [winfo parent $data(w:regexp)] -fill x -padx 5

		update;

		.top paneconfigure .top.regexp -minsize [winfo reqheight .top.regexp];

		update;
	}
}

#----------------------------------------------------------------------------------------------
#	Undo/redo (quick and dirty UNDO/REDO support)
#----------------------------------------------------------------------------------------------

proc regexp::undo:sample {} {
variable data

	# display result
	$data(w:sample) delete 1.0 end
	$data(w:sample) insert 1.0 $data(v:undo:sample)
	# colorize
	go
}

proc regexp::unredo:regexp {dir} {
variable data

	set index [expr ($data(v:undo:index)+$dir) % 100]
	if {![info exists data(v:undo:r$index)]} {
		return
	}
	set data(v:undo:index) $index

	set t $data(w:regexp)
	$t delete 1.0 end
	$t insert 1.0 [lindex $data(v:undo:r$index) 1]
	$t mark set insert [lindex $data(v:undo:r$index) 0]
}

proc regexp::undo:regexp:compute {w k a} {
variable data

	if {[string match -nocase "*control*" $k]
			|| [string match -nocase "*shift*" $k]
			|| [string match -nocase "*alt*" $k]} {
		return
	}

	set data(v:undo:r$data(v:undo:index)) [list [$w index insert] [$w get 1.0 end-1char]]
	set data(v:undo:index) [expr ($data(v:undo:index)+1) % 100]
}

#----------------------------------------------------------------------------------------------
#	Replace
#----------------------------------------------------------------------------------------------

proc regexp::replace {} {
variable data

	set exp [$data(w:regexp) get 1.0 end-1char]
	set subst [$data(w:replace) get 1.0 end-1char]
	if {$exp == ""} {
		set regexp::data(v:nbreplace) "empty regexp"
		return
	}

	# get sample & store it for undo
	set sample [$data(w:sample) get 1.0 end]
	set data(v:undo:sample) $sample
	set result [eval regsub $data(v:all) \
						$data(v:line) $data(v:lineanchor) $data(v:linestop) \
						$data(v:nocase) -- \
						[list $exp] [list $sample] [list [subst -nocommands -novariables $subst]] sample]
	set regexp::data(v:nbreplace) "$result replaced"
	# display result
	$data(w:sample) delete 1.0 end
	$data(w:sample) insert 1.0 $sample
}

proc regexp::replace:toggle {} {
variable data

	if {$regexp::data(v:mode) == "replace"} {
		bind $data(w:regexp) <Tab> "focus $data(w:replace); break;"
		bind $data(w:regexp) <Shift-Tab> "focus $data(w:sample); break;"
		catch { bind $data(w:regexp) <ISO_Left_Tab> "focus $data(w:sample); break;" }

		bind $data(w:replace) <Tab> "focus $data(w:sample); break;"
		bind $data(w:replace) <Shift-Tab> "focus $data(w:regexp); break;"
		catch { bind $data(w:replace) <ISO_Left_Tab> "focus $data(w:regexp); break;" }

		bind $data(w:sample) <Tab> "focus $data(w:regexp); break;"
		bind $data(w:sample) <Shift-Tab> "focus $data(w:replace); break;"
		catch { bind $data(w:sample) <ISO_Left_Tab> "focus $data(w:replace); break;" }

		pack $data(w:allreplace) -side top -fill both;
	} else {
		bind $data(w:regexp) <Tab> "focus $data(w:sample); break;"
		catch { bind $data(w:regexp) <ISO_Left_Tab> "focus $data(w:sample); break;" }

		bind $data(w:sample) <Tab> "focus $data(w:regexp); break;"
		catch { bind $data(w:sample) <ISO_Left_Tab> "focus $data(w:regexp); break;" }

		pack forget $data(w:allreplace)
	}

	update;

	.top paneconfigure .top.regexp -minsize [winfo reqheight .top.regexp];

	update;
}

#----------------------------------------------------------------------------------------------
#	Manage REGEXP
#----------------------------------------------------------------------------------------------

proc regexp::regexp:set {text} {
variable data

	$data(w:regexp) delete 1.0 end
	$data(w:regexp) insert 1.0 $text
}

proc regexp::regexp:colorize {} {
variable data

	set exp [$data(w:regexp) get 1.0 end-1char]
	set max [string length $exp]
	set stack {}
	# list format : min max min max ...
	set indices [list "report" 0 [string length $exp]]
	# search the groups in the regexp
	set data(v:nblevels) 1
	for {set i 0} {$i < $max} {incr i} {
		set c [string index $exp $i]
		if {$c == "\\"} {
			incr i
			continue
		} elseif {$c == "("} {
			set c [string index $exp [expr $i+1]]
			set what [string index $exp [expr $i+2]]
			# test for escape with (?...)
			if {$c == "?"} {
				if {$what != ":"} {
					lappend indices "lookahead"
				} else {
					lappend indices "noreport"
				}
			} else {
				lappend indices "report"
				incr data(v:nblevels)
			}
			lappend indices $i
			set stack "[llength $indices] $stack"
			lappend indices 0

		} elseif {$c == ")"} {
			set idx [lindex $stack 0]
			if {$idx == ""} {
				continue
			}
			set stack [lrange $stack 1 end]
			set indices [lreplace $indices $idx $idx $i]
		}
	}

	# remove old colors
	foreach level $data(v:levels) {
		$data(w:regexp) tag remove $level 1.0 end
	}
	$data(w:regexp) tag remove "lookahead" 1.0 end
	$data(w:regexp) tag remove "noreport" 1.0 end
	# colorize the regexp
	set i 0
	foreach {type min max} $indices {
		if {$type != "report"} {
			continue
		}
		$data(w:regexp) tag add [lindex $data(v:levels) $i] \
				[$data(w:regexp) index "1.0+${min}chars"] \
				[$data(w:regexp) index "1.0+[expr $max+1]chars"]
		incr i
	}
	# apply special item
	foreach {type min max} $indices {
		if {$type == "report"} {
			continue
		}
		$data(w:regexp) tag add $type \
				[$data(w:regexp) index "1.0+${min}chars"] \
				[$data(w:regexp) index "1.0+[expr $max+1]chars"]
	}
}

#----------------------------------------------------------------------------------------------

proc regexp::regexp:load {} {
variable data

	# get filename
	set types [list [list "All" *]]
	set file [tk_getOpenFile -filetypes $types -parent .]
	if {$file == ""} {
		return
	}
	# do it
	set in [open $file "r"]
	regexp:set [read $in [file size $file]]
	close $in
}

#----------------------------------------------------------------------------------------------

proc regexp::regexp:insert {what} {
variable data

	set w $data(w:regexp)
	# prepare undo/redo
	set data(v:undo:r$data(v:undo:index)) [list [$w index insert] [$w get 1.0 end-1char]]
	set data(v:undo:index) [expr ($data(v:undo:index)+1) % 100]
	# do it
	$w insert insert $what
	# prepare undo/redo
	set data(v:undo:r$data(v:undo:index)) [list [$w index insert] [$w get 1.0 end-1char]]
}

#----------------------------------------------------------------------------------------------
# History window to memorize already typed regexp

proc regexp::history:init {} {
variable data
global font

	set w [toplevel .history]
	wm title $w "Visual REGEXP $::version -- REGEXP History"
	wm geometry $w 640x480
	wm protocol $w WM_DELETE_WINDOW "set regexp::data(v:history) 0; wm withdraw $w"

	# text zone	
	set tf [frame $w.t]
	pack $tf -side top -expand true -fill both
	set t [text $tf.t -xscrollcommand "$tf.x set" -yscrollcommand "$tf.y set" \
					-background white -font $::font_regexp -width 5 -height 1 \
					-selectbackground lightblue -selectborderwidth 0]
	set data(w:history) $t
	$t tag configure spacing -font {Helvetica 6}
	set tx [scrollbar $tf.x -borderwidth 1 -orient horizontal -command "$t xview"]
	set ty [scrollbar $tf.y -borderwidth 1 -orient vertical -command "$t yview"]
	bindtags $t "$t all"
	grid $t  $ty -sticky news
	grid $tx x   -sticky news
	grid columnconfigure $tf {0} -weight 1
	grid columnconfigure $tf {1} -weight 0
	grid rowconfigure $tf {0} -weight 1
	grid rowconfigure $tf {1} -weight 0

	# buttons
	set bf [frame $w.f]
	pack $bf -side bottom -padx 5 -pady 5

	set b1 [button $bf.1 -borderwidth 1 -text "Hide" -command "wm withdraw $w; set ::regexp::data(v:history) 0"]
	set b2 [button $bf.2 -borderwidth 1 -text "Save ..." -command "regexp::history:save"]
	pack $b2 $b1 -side left -anchor c

	wm withdraw $w
}

set last ""
set counter 0

proc regexp::history:add {} {
variable data

	if {$::inReplay} {
		# avoid to put the same expression again when replaying it
		set ::inReplay 0
		return
	}

	set exp [$data(w:regexp) get 1.0 end-1char]
	if {$exp != "" && $exp != $::last} {
		# memorize position
		set start [$data(w:history) index insert]
		# add text
		$data(w:history) insert end "$exp\n"
		set end [$data(w:history) index insert]
		$data(w:history) insert end "\n" {spacing}
		set ::last $exp
		$data(w:history) yview moveto 1.0
		# do the binding
		set tag "t$::counter"
		incr ::counter
	    $data(w:history) tag bind $tag <Any-Enter> "$data(w:history) tag configure $tag -background lightblue"
	    $data(w:history) tag bind $tag <Any-Leave> "$data(w:history) tag configure $tag -background {}"
		$data(w:history) tag bind $tag <1> "regexp::history:replay [list $exp]"
		$data(w:history) tag add $tag $start $end

		# colorize the expression in history
		scan $start "%d.%d" sl sc
		incr sl -1
		foreach tag {e0 e1 e2 e3 e4 e5 e6 e7 e8 e9 lookahead noreport} {
			foreach {start end} [$data(w:regexp) tag ranges $tag] {
				set start [$data(w:history) index "$start + $sc chars + $sl lines"]
				set end [$data(w:history) index "$end + $sc chars + $sl lines"]
				$data(w:history) tag add $tag $start $end
			}
		}
	}
}

set inReplay 0

proc regexp::history:replay {text} {
variable data

	set ::inReplay 1
	regexp:set $text
	go
}

proc regexp::history:save {} {
variable data

	set file [tk_getSaveFile -defaultextension .txt]
	if {$file != ""} {
		set out [open $file "w"]
		puts -nonewline $out [$data(w:history) get 1.0 end]
		close $out
	}
}


#----------------------------------------------------------------------------------------------
#	Manage SAMPLE
#----------------------------------------------------------------------------------------------

proc regexp::sample:set {text} {
variable data

	$data(w:sample) delete 1.0 end
	$data(w:sample) insert 1.0 $text
	set data(v:undo:sample) $text
}

proc regexp::sample:colorize {} {
variable data

	# remove old tags
	foreach level $data(v:levels) {
		$data(w:sample) tag remove $level 1.0 end
	}
	set data(v:position)			0;
	set data(v:mainPosition)	0;
	set data(v:positions)		[list];
	set data(v:mainPositions)	[list];

	array unset data v:mainPositions.*;

	# set new tags
	#
	set exp [$data(w:regexp) get 1.0 end-1char];

	if {$exp == ""} {
		set data(v:result) {}
		return
	}
	
	set result [eval \
		regexp -inline -indices \
			$data(v:all) \
			$data(v:line) $data(v:lineanchor) $data(v:linestop) \
			$data(v:nocase) \
			-- \
			[list $exp] [list [$data(w:sample) get 1.0 end]] \
	];

	set data(v:result) $result

	set i						0;
	set matchFirst			-1;
	set matchLast			-1;
	set mainMatchesCount	0;
	set allMatchesCount	0;

	foreach match $result {
		foreach {first last} $match {break;};

		if {($matchFirst == -1) || ($first > $matchLast)} {
			set matchFirst	$first;
			set matchLast	$last;
			set newMatch	1;

			incr mainMatchesCount;
		} else {
			set newMatch	0;
		}

		if {$first != -1} {
			set start [$data(w:sample) index "1.0+[lindex $match 0]chars"];

			$data(w:sample) tag add \
				e[expr $i % $data(v:nblevels)] \
				$start [$data(w:sample) index "1.0+[expr [lindex $match 1]+1]chars"];


			if {$newMatch == 1} {
				set mainStart	$start;

				lappend data(v:mainPositions) $start;
			} else {
				lappend data(v:mainPositions.$mainStart)	$start
			}

			lappend data(v:positions) $start;

			if {$i == 0} {
				$data(w:sample) see $start
			}

			incr i;
			incr allMatchesCount;
		}
	}

	set data(v:mainMatchesCount)	$mainMatchesCount;
	set data(v:allMatchesCount)	$allMatchesCount;

	# set nb of matches
	#
	if {$data(v:nblevels)} {
		set nb 0
		foreach item $result {
			if {[lindex $item 0] <= [lindex $item 1]} {
				incr nb
			}
		}

		if {$data(v:subPositions) == 0} {
			set count	$mainMatchesCount;
		} else {
			set count	$allMatchesCount;
		}

		set data(v:nbmatches)		"0 / $count matches"
	} else {
		set data(v:nbmatches)		"? / ? matches"
	}

	sample:move -2;
}

proc regexp::sample:background {} {
variable data

	foreach level $data(v:levels) color $::colors bgcolor $::bgcolors {
		if {$data(v:background)} {
			$data(w:sample) tag configure $level -foreground $color -background $bgcolor
		} else {
			$data(w:sample) tag configure $level -foreground $color -background {}
		}
	}
}

proc regexp::sample:subPositions {} {
	variable data;

	set position	$data(v:position);
	set mainPosition	$data(v:mainPosition);

	sample:colorize;

	set data(v:position)			$position;
	set data(v:mainPosition)	$mainPosition;

	if {$data(v:subPositions) == 1} {
		set data(v:position)	[lsearch -exact \
			$data(v:positions) \
			[lindex $data(v:mainPositions) $data(v:mainPosition)] \
		];
	} else {
		set idx	0;

		foreach position $data(v:mainPositions) {
			if {[lsearch -exact $data(v:mainPositions.$position) [lindex $data(v:positions) $data(v:position)]] != -1} {
				set data(v:mainPosition)	$idx;
				break;
			}

			incr idx;
		}
	}

	sample:move 0;
}

proc regexp::sample:move {amount} {
variable data

	if {[llength $data(v:positions)] == 0} {
		set data(v:nbmatches) "0 / 0 matches"
		return;
	}

	if {$amount == -2} {
		if {$data(v:subPositions) == 1} {
			set data(v:position)	0;
		} else {
			set data(v:mainPosition)	0;
		}
	} elseif {$amount == +2} {
		if {$data(v:subPositions) == 1} {
			set data(v:position)			[expr {[llength $data(v:positions)]-1}];
		} else {
			set data(v:mainPosition)	[expr {[llength $data(v:mainPositions)]-1}];
		}
	} elseif {$amount == -1} {
		if {$data(v:subPositions) == 1} {
			if {$data(v:position) > 0} {
				incr data(v:position) -1
			}
		} else {
			if {$data(v:mainPosition) > 0} {
				incr data(v:mainPosition) -1
			}
		}
	} elseif {$amount == +1} {
		if {$data(v:subPositions) == 1} {
			if {$data(v:position) < [llength $data(v:positions)]-1} {
				incr data(v:position) +1
			}
		} else {
			if {$data(v:mainPosition) < [llength $data(v:mainPositions)]-1} {
				incr data(v:mainPosition) +1
			}
		}
	}

	if {$data(v:subPositions) == 1} {
		set where	[lindex $data(v:positions) $data(v:position)];
	} else {
		set where	[lindex $data(v:mainPositions) $data(v:mainPosition)];
	}

	if {$where != ""} {
		if {$data(v:subPositions) == 1} {
			set number	$data(v:position);
			set count	$data(v:allMatchesCount);
		} else {
			set number	$data(v:mainPosition);
			set count	$data(v:mainMatchesCount);
		}

		set data(v:nbmatches) "[expr {$number + 1}] / $count matches"

		$data(w:sample) see $where
		$data(w:sample) mark set insert $where

		focus $data(w:sample)
	}
}

#----------------------------------------------------------------------------------------------

proc regexp::sample:load {} {
variable data

	# get filename
	set types [list [list "All" *]]
	set file [tk_getOpenFile -initialdir $data(v:dir) -filetypes $types -parent .]
    if {$file == ""} {
		return
	}
	# memorize location
	set data(v:dir) [file dirname $file]
	set data(v:file) [file tail $file]
	# do it
	set in [open $file "r"]
	sample:set [read $in [file size $file]]
	close $in
}

proc regexp::sample:save {mode} {
variable data

	# get filename
	set types [list [list "All" *]]
	set file [tk_getSaveFile -initialdir $data(v:dir) -initialfile $data(v:file) \
							 -filetypes $types -parent .]
    if {$file == ""} {
		return
	}
	# memorize location
	set data(v:dir) [file dirname $file]
	set data(v:file) [file tail $file]
	# do it
	set out [open $file "w"]
	fconfigure $out -translation $mode
	puts $out [$data(w:sample) get 1.0 end]
	close $out
}


#----------------------------------------------------------------------------------------------
#	Main toplevel commands
#----------------------------------------------------------------------------------------------

proc regexp::make-regexp {} {
variable data

	# new dialog
	catch { destroy .mkregexp }
	set w [toplevel .mkregexp]
	wm title $w "Make regexp"
	wm geometry $w 640x480
	# widgets
	set f [frame $w.top]
		# area to input words
		label $f.l1 -text "Words list:"
		set list [text	$f.list \
			-wrap						char \
			-background				white \
			-font						$::font_regexp \
			-undo						1 \
			-selectbackground		lightblue \
			-selectborderwidth	0 \
			-width					1 \
			-height					10 \
			-borderwidth			1 \
			-yscrollcommand		[list $f.sy1 set] \
		];
		scrollbar $f.sy1 -command "$list yview" -orient vertical -bd 1
		# button to compute the regexp
		set doit [button $f.doit -text "Compute" -width 15 -bd 1 -command "regexp::make-regexp:compute"]
		# display result
		label $f.l2 -text "Regexp:"
		set output [text	$f.output \
			-wrap						char \
			-undo						1 \
			-background				white \
			-font						$::font_regexp \
			-selectbackground		lightblue \
			-selectborderwidth	0 \
			-width					1 \
			-height					4 \
			-borderwidth			1 \
			-yscrollcommand		[list $f.sy2 set] \
		];
		bindtags $output "$output all"
		scrollbar $f.sy2 -command "$output yview" -orient vertical -bd 1
		# layout
		grid $f.l1	$list		$f.sy1		-sticky news
		grid $doit	-			-			-sticky ns -pady 2
		grid $f.l2	$output	$f.sy2		-sticky news
		grid columnconfigure $f {1} -weight 1
		grid rowconfigure $f {0 2} -weight 1
		# init
		set data(w:make:list) $list
		set data(w:make:output) $output
	# button OK / CANCEL
	set ff [frame $w.bottom]
		set ok [button $ff.ok -text "Insert into regexp" -width 20 -bd 1 -command "regexp::make-regexp:ok $w"]
		set cancel [button $ff.cancel -text "Cancel" -width 20 -bd 1 -command "destroy $w"]
		pack $ok $cancel -side left -fill both -padx 10 -pady 10
	# layout
	pack $f -side top -expand true -fill both
	pack $ff -side bottom -anchor c
}

proc regexp::make-regexp:compute {} {
variable data

	set words [$data(w:make:list) get 1.0 end-1c]
	$data(w:make:output) delete 1.0 end
	$data(w:make:output) insert 1.0 [make-regexp::make-regexp $words]
}

proc regexp::make-regexp:ok {w} {
variable data

	set words [$data(w:make:list) get 1.0 end-1c]

	$data(w:regexp) insert insert "([make-regexp::make-regexp $words])"
	destroy $w
}


#==============================================================================================
#	Main entry point
#==============================================================================================

# try to get customization from 'visual_regexp.ini'
puts "[file exists visual_regexp.ini]"
set filename [file dirname [info nameofexecutable]]/visual_regexp.ini
if {[file exists $filename]} {
	source $filename
} elseif {[file exists visual_regexp.ini]} {
	source visual_regexp.ini
}

# try to auto user patterns
set filename [file dirname [info nameofexecutable]]/regexp.txt
if {[file exists $filename]} {
	regexp::pattern:load $filename
} elseif {[file exists regexp.txt]} {
	regexp::pattern:load regexp.txt
}

# buld the GUI
regexp::history:init
regexp::gui
regexp::go

if {$argc > 1} {
	puts "Usage: $argv0 <sampleFile>"
} elseif {$argc == 1} {
	set filename [lindex $argv 0]
	set file [open $filename]
	set data [read $file [file size $filename]]
	close $file

	# memorize location
	set regexp::data(v:dir) [file dirname $filename]
	set regexp::data(v:file) [file tail $filename]

	regexp::sample:set $data
	unset data
}


#----------------------------------------------------------------------------------------------

image create photo logo -data {R0lGODlhLAFxAMYAAAICAhcXFzw8WFtbb4+Njq2ssioqNMfGxkJCSgYCtcYtJrjOuEpGVs3Y0FJOYr1JNb53Yt/g4BsXq+i9yspGHOjQ08adlebm5rm57unH0NwjGPY4EsJaIgYCwqOjp2hmet6+wurq6uTYvvcuE77Ovh4altZLE9C0uioqjrljT76OevtCDt7Hx7q5u8LC7tzZ2PsbFnJyhvsqEspWGvxaCgYC0+jh3cLSwh4akujY2Ll8Z5qZm8rK6tDQ6PxODNHQ0f///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////yH5BAEKAEAALAAAAAAsAXAAAAf+gBEvBy2FBz8XiYqLixGEhT8vjJOUjD+PLZGVm5yCl4WghpEREZymlBE/hqWnio8HrbGngoUvrLKEP7eym4+SnS0HpaqEB8bHFyHKF57GycrQ0RcNhAWQsSEvl6O8lJ+Qv92TIY6hoIcvts/im9mfwrLlBYjs4uTF4uXq9YwRwfSbUmUKwYxUMVu7Uh2DF60hwXKgAJoiV5DfIlWgGoSzmA2UtQMaSVk8RW1VvIgjeQ06sDGWvxYiU3bUxYmcqgbP7mVKVCrVJWOacjpcBjEYw6HQeApCiFSaIJFJFYqK0LRqw0QYjeoqtayr1a/RbAyCZAPsQ1AvzKqtOo3mWlr+wtZWFUTiV9Wxkogay/twoUaCrSKQOAev1U+QLVEdtmuzUAGQuyxSO3QLsOWUi2w+kkgSVORZ2j6DxsmrceFJpERTUvb01E8bi14Ewwn4ZwPYOSf6PGcrGgsLDzQIF67gQYqPuppeSBfqRdm+r5JbtTBCgwIIz5M+TJ35qlyrWDcLVR5C1WOqase24AuWGY9gadUuxzdUYNyvzIxxRWpwYDKeGq3T2n8ETiRWA6HQdIEKCsDg4IMQOqiACjyolog2oTRAVTJFsWThMzrIIKIMDwA2zn8mEpgiZvkZQlorIbzy4SQI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}




#==============================================================================================
#	Make Regexp
#==============================================================================================
namespace eval make-regexp {
}
#	Takes a list of words, returns a list "prefix <recurse>   prefix <recurse>  ..."
#	after grouping by first common letter.
proc make-regexp::prefix {words} {
	# init
	set result {}
	lappend words ""		;# to force last completion
	# group by first letter
	set prefix [string range [lindex $words 0] 0 0]
	set subwords [list [string range [lindex $words 0] 1 end]]
	foreach word [lrange $words 1 end] {
		set char [string range $word 0 0]
		if {$char == $prefix} {
			lappend subwords [string range $word 1 end]
		} else {
			# compute prefixes recursively
			set recurse [prefix $subwords]
			if {[llength $recurse] == 2} {
				# only one prefix, so concat with previous prefix
				append prefix [lindex $recurse 0]
				set recurse [lindex $recurse 1]
			}
			append result " [verify [list $prefix $recurse]]"
			set prefix $char
			set subwords [list [string range $word 1 end]]
		}
	}
	# return
	set result
}
#	Verification of regexp.
#	After searching common suffixes, some patterns grouped by parenthesis or conditional exps
#	may be broken. We need to fix them.
proc make-regexp::verify {exp} {
	set orphans [isOrphans $exp]
	set result {}
	foreach {prefix recurse} $exp {
		if {![isBalanced $prefix]} {
			if {[llength $recurse]} {
				foreach {pp rr} $recurse {
					lappend result "$prefix$pp" $rr
				}
				if {![isBalanced $prefix] && $orphans} {
					set result [verify $result]
				}
			} else {
				lappend result "$prefix" ""
			}
		} else {
			lappend result $prefix $recurse
		}
	}
	# return result after fixing
	set result
}
#	Check for orphan grouping ('|' lost in lower level)
proc make-regexp::isOrphans {exp} {
	set orphan 0
	foreach {prefix recurse} $exp {
		if {[string index $prefix 0] == "|"} {
			set orphan 1
			break
		}
		if {[isOrphans $recurse]} {
			set orphan 1
			break
		}
	}
	set orphan
}
#==============================================================================================
#	Check if parenthesis in 'str' after balanced.
proc make-regexp::isBalanced {str} {
	# if start with '?' skip it
	if {[string index $str 0] == "?"} {
		return 0
	}
	# must start with a ')'
	if {[string index $str 0] != ")"} {
		return 1
	}
	# try to balanced each ')' with an appropriate '('
	set depth 0
	foreach c [split $str {}] {
		if {$c == "("} {
			incr depth -1
		} elseif {$c == ")"} {
			incr depth +1
		}
	}
	return [expr $depth == 0]
}
#	Check if 'str' contains a first level grouping
proc make-regexp::firstLevelGroup {str} {
	set depth 0
	foreach c [split $str {}] {
		if {$c == "("} {
			incr depth -1
		} elseif {$c == ")"} {
			incr depth +1
		} elseif {$depth == 0 && $c == "|"} {
			return 1
		}
	}
	return 0
}
#==============================================================================================
#	After having found common prefixes, try to find common suffixes in expression
proc make-regexp::suffix {list} {
	# end of recursion if empty list
	if {[llength $list] == 0} {
		return ""
	}
	set newlist {}
	foreach {prefix recurse} $list {
		set result [suffix $recurse]
		lappend newlist $prefix [lindex $result 0]
	}
	# compute longest common suffixes
	set words {}
	foreach {prefix tail} $newlist {
		if {[firstLevelGroup $tail]} {
			set tail "($tail)"
		}
		lappend words [reverse $prefix$tail]
	}
	set words [lsort -unique $words]
	set reverse [prefix $words]
	# compute regexp from precomputed reverse list
	set regexp [build "" $reverse]
	# returns computed regexp
	set regexp
}
proc make-regexp::build {mainstem reverse} {
	# flag to indicate need for '?' (optional group)
	set addQuestionMark 0
	set regexp ""
	foreach {prefix recurse} $reverse {
		set stem "[reverse $prefix]$mainstem"
		if {[llength $recurse]} {
			set fromlower [build $stem $recurse]
		} else {
			set fromlower ""
		}
		# build regexp
		if {$prefix == ""} {
			set addQuestionMark 1
		} else {
			if {[string length $fromlower] > 1 && [string index $fromlower end] != "?"} {
				set fromlower "($fromlower)"
			}
			append regexp "$fromlower[reverse $prefix]|"
		}
	}
	# remove last trailing '|'
	set regexp "[string range $regexp 0 end-1]"
	# add '?' if needed
	if {$addQuestionMark} {
		if {[string length $regexp] == 1} {
			set regexp "$regexp?"
		} else {
			set regexp "($regexp)?"
		}
	}
	# result
	set regexp
}
#----------------------------------------------------------------------------------------------
#	Last pass for grouping '(x|y|z|...)' into char range '[xyz...]'
proc make-regexp::optimize:charset {regexp} {
	set optimized ""
	set memory ""
	set ok 1
	set charset ""
	# examine char one by one
	set len [string length $regexp]
	for {set i 0} {$i < $len} {incr i} {
		set char [string index $regexp $i]
		append memory $char
		if {$char =="("} {
			# start of group
			if {$ok} {
				append optimized [string range $memory 0 end-1]
			}
			incr i
			set result [optimize:charset [string range $regexp $i end]]
			append optimized "[lindex $result 2][lindex $result 0][lindex $result 3]"
			set memory ""
			set ok 0
			incr i [expr [lindex $result 1]]
			continue
		} elseif {$char ==")"} {
			# end of group
			if {$ok} {
				set optimized "\[$charset\]"
				return [list $optimized $i "" ""]
			} else {
				return [list $optimized $i "(" ")"]
			}
		}
		if {$ok} {
			if {$i & 1} {
				if {$char != "|"} {
					set ok 0
					append optimized $memory
				}
			} else {
				append charset $char
			}
		} else {
			append optimized $char
		}
	}
	# return result
	list $optimized $i "(" ")"
}
#==============================================================================================
#	Compute string in reverse order
proc make-regexp::reverse {string} {
	set result ""
	for {set i [expr [string length $string]-1]} {$i >= 0} {incr i -1} {
		append result [string index $string $i]
	}
	set result
}
#==============================================================================================
proc make-regexp::make-regexp {words} {
	set words [lsort -unique $words]
	# escape special chars used to form regexp
	regsub -all -- {\|} $words "\x01" words
	regsub -all -- {\(} $words "\x02" words
	regsub -all -- {\)} $words "\x03" words
	regsub -all -- {\?} $words "\x04" words
	regsub -all -- {\[} $words "\x07" words
	regsub -all -- {\]} $words "\x08" words
	# do it
	set list [prefix $words]
	set regexp [suffix $list]
	# returns regexp
  	set regexp [lindex [optimize:charset $regexp] 0]
	# un-escape special chars used to form regexp
	regsub -all -- "\x01" $regexp "\\|" regexp
	regsub -all -- "\x02" $regexp "\\(" regexp
	regsub -all -- "\x03" $regexp "\\)" regexp
	regsub -all -- "\x04" $regexp "\\?" regexp
	regsub -all -- "\x07" $regexp "\\\[" regexp
	regsub -all -- "\x08" $regexp "\\\]" regexp
	regsub -all -- "\\*" $regexp "\\*" regexp
	regsub -all -- "\\+" $regexp "\\+" regexp
	regsub -all -- "\\\$" $regexp "\$" regexp
	regsub -all -- "\\\^" $regexp "\\\^" regexp
	# returns result
	set regexp
}
#==============================================================================================

