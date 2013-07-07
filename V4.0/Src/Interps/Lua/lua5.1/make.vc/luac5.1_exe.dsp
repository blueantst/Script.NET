# Microsoft Developer Studio Project File - Name="luac5.1_exe" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=luac5.1_exe - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "luac5.1_exe.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "luac5.1_exe.mak" CFG="luac5.1_exe - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "luac5.1_exe - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "luac5.1_exe - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "luac5.1_exe - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../bin"
# PROP BASE Intermediate_Dir "../obj/luac5.1_exe/Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../bin"
# PROP Intermediate_Dir "../obj/luac5.1_exe/Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /I "../include" /I "../src" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../include" /I "../src" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 lua5.1.lib /nologo /entry:"mainCRTStartup" /subsystem:console /machine:I386 /out:"../bin/luac5.1.exe" /libpath:"../lib" /libpath:"../lib" setargv.obj
# ADD LINK32 lua5.1.lib /nologo /entry:"mainCRTStartup" /subsystem:console /machine:I386 /out:"../bin/luac5.1.exe" /libpath:"../lib" /libpath:"../lib" setargv.obj

!ELSEIF  "$(CFG)" == "luac5.1_exe - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../bin"
# PROP BASE Intermediate_Dir "../obj/luac5.1_exe/Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../bin"
# PROP Intermediate_Dir "../obj/luac5.1_exe/Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "../include" /I "../src" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "../include" /I "../src" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 lua5.1.lib /nologo /entry:"mainCRTStartup" /subsystem:console /incremental:yes /debug /machine:I386 /out:"../bin/luac5.1.exe" /pdbtype:sept /libpath:"../lib" /libpath:"../lib" setargv.obj
# ADD LINK32 lua5.1.lib /nologo /entry:"mainCRTStartup" /subsystem:console /incremental:yes /debug /machine:I386 /out:"../bin/luac5.1.exe" /pdbtype:sept /libpath:"../lib" /libpath:"../lib" setargv.obj

!ENDIF

# Begin Target

# Name "luac5.1_exe - Win32 Release"
# Name "luac5.1_exe - Win32 Debug"
# Begin Group "src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=../src/luac.c
# End Source File
# Begin Source File

SOURCE=../src/print.c
# End Source File
# Begin Source File

SOURCE=../src/lua.rc
# End Source File
# End Group
# End Target
# End Project
