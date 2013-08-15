"""
MyPyShellWindow
"""

import sys, string, keyword, _platform
from code import InteractiveInterpreter

# new style numbers
_stdout_style = 15
_stderr_style = 16
_trace_style = 17

#----------------------------------------------------------------------

class MyPyShellWindow(InteractiveInterpreter):
	def __init__(self, locals=None, banner=None):
		InteractiveInterpreter.__init__(self, locals)

		sys.stdout = FauxFile(self, _stdout_style)
		sys.stderr = FauxFile(self, _stderr_style)

		# copyright/banner message
		if banner is None:
			pass
		#	self.write("Python %s on %s\n" % #%s\n(%s)\n" %
		#			   (sys.version, sys.platform,
		#				#sys.copyright, self.__class__.__name__
		#				))
		else:
			self.write("%s\n" % banner)

	# used for writing to stdout, etc.
	def _write(self, text, style=_stdout_style):
		_platform.write(_platform._outputwnd, text, style, self)

	write = _write

	def writeTrace(self, text):
		self._write(text, _trace_style)


	#----------------------------------------------
	# overloaded methods from InteractiveInterpreter
	def runsource(self, source):
		#stdout, stderr = sys.stdout, sys.stderr
		#sys.stdout = FauxFile(self, _stdout_style)
		#sys.stderr = FauxFile(self, _stderr_style)

		more = InteractiveInterpreter.runsource(self, source)

		#sys.stdout, sys.stderr = stdout, stderr
		return more

	def showsyntaxerror(self, filename=None):
		self.write = self.writeTrace
		InteractiveInterpreter.showsyntaxerror(self, filename)
		self.write = self._write

	def showtraceback(self):
		self.write = self.writeTrace
		InteractiveInterpreter.showtraceback(self)
		self.write = self._write

#----------------------------------------------------------------------

class FauxFile:
	def __init__(self, psw, style):
		self.psw = psw
		self.style = style

	def write(self, text):
		self.psw.write(text, self.style)

	def writelines(self, lst):
		map(self.write, lst)

	def flush(self):
		pass


if __name__ == '__main__':
	shell = MyPyShellWindow()


