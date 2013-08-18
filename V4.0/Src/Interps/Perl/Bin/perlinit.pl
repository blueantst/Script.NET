####################################################
#     
#     Perl Initial Interpreter
#         Core Script File
#
####################################################
# 
#    >>> Be careful modifying this file ! <<<
#
####################################################
#         include all needed libs here


####################################################

package ReverseSTDOUT;
use strict;
sub TIEHANDLE {
        my $class = shift;
        bless [], $class;
}
sub PRINT {
        my $self = shift;
        push @$self, join ' ', @_;
		main::platform_print('stdout', @$self);
		pop @$self;
}
sub READLINE {
        my $self = shift;
        pop @$self;
}
## end of class ReverseSTDOUT

package ReverseSTDERR;
use strict;
sub TIEHANDLE {
        my $class = shift;
        bless [], $class;
}
sub PRINT {
        my $self = shift;
        push @$self, join ' ', @_;
		main::platform_print('stderr',@$self);
		pop @$self;
}
sub READLINE {
        my $self = shift;
        pop @$self;
}
## end of class ReverseSTDERR

#package main;

BEGIN
{
	# so you can put all .pm file under lib dir or site/lib dir
	#push @INC, 'lib', 'site/lib';

	use strict;
	use warnings 'all';
	#use Symbol qw(delete_package);
	
	# Magic here!, tie wrap STDOUT & STDERR into object class ReverseSTDOUT & ReverseSTDERR
	tie *STDOUT, "ReverseSTDOUT";
	tie *STDERR, "ReverseSTDERR";
}

END
{
	untie *STDOUT;
	untie *STDERR;
}

1;
