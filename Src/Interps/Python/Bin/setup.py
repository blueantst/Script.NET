# A setup script showing advanced features.
#
# Note that for the NT service to build correctly, you need at least
# win32all build 161, for the COM samples, you need build 163.
# Requires wxPython, and Tim Golden's WMI module.

# Note: WMI is probably NOT a good example for demonstrating how to
# include a pywin32 typelib wrapper into the exe: wmi uses different
# typelib versions on win2k and winXP.  The resulting exe will only
# run on the same windows version as the one used to build the exe.
# So, the newest version of wmi.py doesn't use any typelib anymore.

from distutils.core import setup
import py2exe
import sys
import glob

# If run without args, build executables, in quiet mode.
if len(sys.argv) == 1:
    sys.argv.append("py2exe")
    sys.argv.append("-q")

class Target:
    def __init__(self, **kw):
        self.__dict__.update(kw)
        # for the versioninfo resources
        self.version = "1.0"
        self.company_name = ""
        self.copyright = "Create by Script.NET"
        self.name = ""

################################################################
# A program using wxPython

# The manifest will be inserted as resource into test_wx.exe.  This
# gives the controls the Windows XP appearance (if run on XP ;-)
#
# Another option would be to store it in a file named
# test_wx.exe.manifest, and copy it with the data_files option into
# the dist-dir.
#
manifest_template = '''
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0">
<assemblyIdentity
    version="5.0.0.0"
    processorArchitecture="x86"
    name="%(prog)s"
    type="win32"
/>
<description>%(prog)s Program</description>
<dependency>
    <dependentAssembly>
        <assemblyIdentity
            type="win32"
            name="Microsoft.Windows.Common-Controls"
            version="6.0.0.0"
            processorArchitecture="X86"
            publicKeyToken="6595b64144ccf1df"
            language="*"
        />
    </dependentAssembly>
</dependency>
</assembly>
'''

RT_MANIFEST = 24

################################################################
setup_wx = Target(
    # used for the versioninfo resource
    description = "A GUI app with wxPython",

    # what to build
    script = "$$SCRIPT_FILE$$",
    other_resources = [(RT_MANIFEST, 1, manifest_template % dict(prog="$$PROGRAM_NAME$$"))],
    icon_resources = [(1, "$$PROGRAM_ICON$$")],
    dest_base = "$$PROGRAM_NAME$$")

################################################################
setup_wx_console = Target(
    # used for the versioninfo resource
    description = "An app with python",

    # what to build
    script = "$$SCRIPT_FILE$$",
    other_resources = [(RT_MANIFEST, 1, manifest_template % dict(prog="$$PROGRAM_NAME$$"))],
    icon_resources = [(1, "$$PROGRAM_ICON$$")],
    dest_base = "$$PROGRAM_NAME$$")

################################################################
# A program using early bound COM, needs the typelibs option below
setup_wmi = Target(
    description = "Early bound COM client app with python",
    script = "$$SCRIPT_FILE$$",
    )

################################################################
# a NT service, modules is required
setup_service = Target(
    # used for the versioninfo resource
    description = "A Windows NT service with python",
    # what to build.  For a service, the module name (not the
    # filename) must be specified!
    modules = ["$$PROGRAM_NAME$$"]
    )

################################################################
# a COM server (exe and dll), modules is required
#
# If you only want a dll or an exe, comment out one of the create_xxx
# lines below.

setup_com_exe = Target(
    description = "Python Interpreter as COM server module",
    # what to build.  For COM servers, the module name (not the
    # filename) must be specified!
    modules = ["win32com.servers.$$PROGRAM_NAME$$"],
    create_dll = False,
    )
################################################################

setup_com_dll = Target(
    description = "Python Interpreter as COM server module",
    # what to build.  For COM servers, the module name (not the
    # filename) must be specified!
    modules = ["win32com.servers.$$PROGRAM_NAME$$"],
    create_exe = False,
    )
################################################################

setup(
    options = {"py2exe": {"compressed": 1,
                          "optimize": 2,
                          "ascii": 1,
                          "bundle_files": 1}},
    zipfile = None,    
    $$SETUP_TYPE$$
    )
