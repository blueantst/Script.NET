# Microsoft Developer Studio Project File - Name="PythonInterp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=PythonInterp - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PythonInterp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PythonInterp.mak" CFG="PythonInterp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PythonInterp - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PythonInterp - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PythonInterp - Win32 PDBRelease" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PythonInterp - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /ZI /Od /I "..\include" /I "..\..\..\Plugins\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../Bin/PythonInterp.dll" /libpath:"../Libs"

!ELSEIF  "$(CFG)" == "PythonInterp - Win32 Debug"

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
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\..\..\Plugins\Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../Bin/PythonInterp_d.dll" /pdbtype:sept /libpath:"../Libs"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PythonInterp - Win32 PDBRelease"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PythonInterp___Win32_PDBRelease"
# PROP BASE Intermediate_Dir "PythonInterp___Win32_PDBRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PDBRelease"
# PROP Intermediate_Dir "PDBRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /ZI /Od /I "..\include" /I "..\..\..\Plugins\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /FR /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\include" /I "..\..\..\Plugins\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /pdb:"../Bin/PythonInterp.pdb" /debug /machine:I386 /out:"../Bin/PythonInterp.dll" /libpath:"../Libs"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /libpath:"../Libs"

!ENDIF 

# Begin Target

# Name "PythonInterp - Win32 Release"
# Name "PythonInterp - Win32 Debug"
# Name "PythonInterp - Win32 PDBRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CPythonInterp.cpp
# End Source File
# Begin Source File

SOURCE=.\dll_loader.cpp
# End Source File
# Begin Source File

SOURCE=.\extern.cpp
# End Source File
# Begin Source File

SOURCE=.\PythonContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\PythonInterp.cpp
# End Source File
# Begin Source File

SOURCE=.\PythonInterp.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\TpsPythonBuild.cpp
# End Source File
# Begin Source File

SOURCE=.\TpsPythonCompile.cpp
# End Source File
# Begin Source File

SOURCE=.\TpsPythonInterp.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CPythonInterp.h
# End Source File
# Begin Source File

SOURCE=.\dll_loader.h
# End Source File
# Begin Source File

SOURCE=.\PythonContainer.h
# End Source File
# Begin Source File

SOURCE=.\PythonGlue.impl.h
# End Source File
# Begin Source File

SOURCE=.\PythonMacros.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TpsPythonBuild.h
# End Source File
# Begin Source File

SOURCE=.\TpsPythonCompile.h
# End Source File
# Begin Source File

SOURCE=.\TpsPythonInterp.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\141.ico
# End Source File
# Begin Source File

SOURCE=.\res\link_intern.ico
# End Source File
# Begin Source File

SOURCE=.\python.ico
# End Source File
# Begin Source File

SOURCE=.\res\PythonInterp.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\mfcshell.py
# End Source File
# End Target
# End Project
