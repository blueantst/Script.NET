# Microsoft Developer Studio Project File - Name="TclInterp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TclInterp - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TclInterp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TclInterp.mak" CFG="TclInterp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TclInterp - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TclInterp - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TclInterp - Win32 PDBRelease" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TclInterp - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /I "../include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 xerces-c_2.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../Bin/TclInterp.dll" /libpath:"../Lib"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "TclInterp - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /Zm500 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 xerces-c_2D.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../Bin/TclInterp_d.dll" /pdbtype:sept /libpath:"../Lib"

!ELSEIF  "$(CFG)" == "TclInterp - Win32 PDBRelease"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "TclInterp___Win32_PDBRelease"
# PROP BASE Intermediate_Dir "TclInterp___Win32_PDBRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PDBRelease"
# PROP Intermediate_Dir "PDBRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /I "../include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /I "../include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 xerces-c_2.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../Bin/TclInterp.dll" /libpath:"../Lib"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 xerces-c_2.lib /nologo /subsystem:windows /dll /debug /machine:I386 /libpath:"../Lib"

!ENDIF 

# Begin Target

# Name "TclInterp - Win32 Release"
# Name "TclInterp - Win32 Debug"
# Name "TclInterp - Win32 PDBRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CTclInterp.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TclExCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\TclInterp.cpp
# End Source File
# Begin Source File

SOURCE=.\TclInterp.rc
# End Source File
# Begin Source File

SOURCE=.\TclPcom.cpp
# End Source File
# Begin Source File

SOURCE=.\TclWorkTree.cpp
# End Source File
# Begin Source File

SOURCE=.\TclXml.cpp
# End Source File
# Begin Source File

SOURCE=.\TpsTclBuild.cpp
# End Source File
# Begin Source File

SOURCE=.\TpsTclCompile.cpp
# End Source File
# Begin Source File

SOURCE=.\TpsTclDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\TpsTclInterp.cpp
# End Source File
# Begin Source File

SOURCE=.\Wait.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLDom.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CTclInterp.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TclExCmd.h
# End Source File
# Begin Source File

SOURCE=.\TclPcom.h
# End Source File
# Begin Source File

SOURCE=.\TclXml.h
# End Source File
# Begin Source File

SOURCE=.\TpsTclBuild.h
# End Source File
# Begin Source File

SOURCE=.\TpsTclCompile.h
# End Source File
# Begin Source File

SOURCE=.\TpsTclDebug.h
# End Source File
# Begin Source File

SOURCE=.\TpsTclInterp.h
# End Source File
# Begin Source File

SOURCE=.\Wait.h
# End Source File
# Begin Source File

SOURCE=.\XMLDom.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\141.ico
# End Source File
# Begin Source File

SOURCE=.\res\256.ico
# End Source File
# Begin Source File

SOURCE=.\res\275.ico
# End Source File
# Begin Source File

SOURCE=.\res\flash2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\link_intern.ico
# End Source File
# Begin Source File

SOURCE=.\res\optionmarks.bmp
# End Source File
# Begin Source File

SOURCE=.\res\options.bmp
# End Source File
# Begin Source File

SOURCE=.\tcl.ico
# End Source File
# Begin Source File

SOURCE=.\res\TclInterp.rc2
# End Source File
# End Group
# End Target
# End Project
