#!/usr/bin/env python

import socket
import sys
import threading
from types import *
import pdb as _pdb

# get sting format value of variable
def stringify(thing):
    try:
        typ = type(thing)
        if hasattr(thing, '__name__'):
            return thing.__name__
            #if hasattr(typ, '__name__'):
            #    return thing.__name__ + ' (' + typ.__name__ + ')'
            #else:
            #    return thing.__name__ + ' (' + str(typ) + ')'
        else:
            return repr(thing)
    except:
        return "Looking at object failed - %s: %s" % (sys.exc_info()[0], sys.exc_info()[1])

def ismultitem(thing):
    typ = type(thing)
    return 0
#    if (typ == ListType || typ == TupleType || typ == DictType):
#        return 1
#    elif (typ == ClassType || typ == ModuleType || typ == InstanceType):
#        return 2
#    else:
#        return 0


class RmXdb(_pdb.Pdb):
    #def __init__(self):
    #    pdb.Pdb.__init__(self)
    #    self.reset()
    #    self.forget()
    #    self.curframe = None
    # sys exit
    def do_sysexit(self, arg):
        print "[RMXDB::SYSEXIT]"
        sys.exit(1)

    def user_call(self, frame, args):
        return

    def user_line(self, frame):
        """This function is called when we stop or break at this line."""
        if frame.f_lineno == 0:
            return

        name = frame.f_code.co_name
        if not name: name = ''
        print >>self.stdout, '[RMDBG][LINE]f=%s,n=%s,l=%d' % (frame.f_code.co_filename, name, frame.f_lineno)

        if self._wait_for_mainpyfile:
            if (self.mainpyfile != self.canonic(frame.f_code.co_filename)
                or frame.f_lineno<= 0):
                return
            self._wait_for_mainpyfile = 0
        if self.bp_commands(frame):
            self.interaction(frame, None)

    def user_return(self, frame, retval):
        return
        #_platform.pydbg(_platform.interpid, 2, frame.f_code.co_filename, frame.f_code.co_name, frame.f_lineno, '')

    def do_jump(self, arg):
        if self.curindex + 1 != len(self.stack):
            print >>self.stdout, "*** You can only jump within the bottom frame"
            return
        try:
            arg = int(arg)
        except ValueError:
            print >>self.stdout, "*** The 'jump' command requires a line number."
        else:
            try:
                # Do the jump, fix up our copy of the stack, and display the
                # new position
                self.curframe.f_lineno = arg
                self.stack[self.curindex] = self.stack[self.curindex][0], arg
                #self.print_stack_entry(self.stack[self.curindex])
                print >>self.stdout, '[RMDBG][LINE]f=%s,n=%s,l=%d' % (self.curframe.f_code.co_filename, self.curframe.f_code.co_name, self.curframe.f_lineno)
            except ValueError, e:
                print >>self.stdout, '*** Jump failed:', e

    def do_print_vars(self, arg):
        frame = self.curframe;
		# local vars
        for key in frame.f_locals:
            if not (key[:2] == key[-2:] == '__'):
                print >>self.stdout, '[RMDBG][VAR][LOCAL]n=%s,v=%s,t=%s' % (key, stringify(frame.f_locals[key]), type(frame.f_locals[key]).__name__)
        # global vars
        row = 0
        for key in frame.f_globals:
            if not (key[:2] == key[-2:] == '__'):
                print >>self.stdout, '[RMDBG][VAR][GLOBAL]n=%s,v=%s,t=%s' % (key, stringify(frame.f_globals[key]), type(frame.f_globals[key]).__name__)

    def do_print_stack(self, arg):
        try:
            for f, lineno in self.stack:
                print >>self.stdout, '[RMDBG][STACK]n=%s,l=%d,f=%s' % (f.f_code.co_name, lineno, f.f_code.co_filename)
        except KeyboardInterrupt:
            pass


def server(addr = 'localhost', port = 17664):
    
    def __output(conn):
        while True:
            try:
                data = conn.recv(1024)
                if not data:break
                sys.stdout.write(data)
                sys.stdout.flush()
            except socket.error:
                print "Connection close."
                break
            except:
                break

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    sock.bind((addr, port))
    sock.listen(1)

    print "Remote Pdb listening at %s:%d\n" % (addr, port)

    try:
        conn, addr = sock.accept()
    except KeyboardInterrupt:
        sys.exit(0)

    print "Connect from %s\n" % str(addr)

    threading.Thread(target = __output, args = (conn,)).start()
    
    try:
        while 1:
            i = sys.stdin.readline()
            conn.sendall(i)
    except socket.error:
        print "Connection close."
    finally:
        conn.close()

def pdb(addr = 'localhost', port = 17664):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((addr, port))
    #return _pdb.Pdb(stdin = sock.makefile('r+', 0), stdout = sock.makefile('w+', 0))
    return RmXdb(stdin = sock.makefile('r+', 0), stdout = sock.makefile('w+', 0))
