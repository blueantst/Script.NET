# Microsoft Developer Studio Project File - Name="bin2c5.1_exe" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=bin2c5.1_exe - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "bin2c5.1_exe.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "bin2c5.1_exe.mak" CFG="bin2c5.1_exe - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "bin2c5.1_exe - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "bin2c5.1_exe - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "bin2c5.1_exe - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../bin"
# PROP BASE Intermediate_Dir "../obj/bin2c5.1_exe/Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../bin"
# PROP Intermediate_Dir "../obj/bin2c5.1_exe/Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /entry:"mainCRTStartup" /subsystem:console /machine:I386 /out:"../bin/bin2c5.1.exe" /libpath:"../lib"
# ADD LINK32 /nologo /entry:"mainCRTStartup" /subsystem:console /machine:I386 /out:"../bin/bin2c5.1.exe" /libpath:"../lib"

!ELSEIF  "$(CFG)" == "bin2c5.1_exe - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../bin"
# PROP BASE Intermediate_Dir "../obj/bin2c5.1_exe/Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../bin"
# PROP Intermediate_Dir "../obj/bin2c5.1_exe/Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /entry:"mainCRTStartup" /subsystem:console /incremental:yes /debug /machine:I386 /out:"../bin/bin2c5.1.exe" /pdbtype:sept /libpath:"../lib"
# ADD LINK32 /nologo /entry:"mainCRTStartup" /subsystem:console /incremental:yes /debug /machine:I386 /out:"../bin/bin2c5.1.exe" /pdbtype:sept /libpath:"../lib"

!ENDIF

# Begin Target

# Name "bin2c5.1_exe - Win32 Release"
# Name "bin2c5.1_exe - Win32 Debug"
# Begin Group "etc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=../etc/bin2c.c
# End Source File
# End Group
# Begin Group "src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=../src/lua.rc
# End Source File
# End Group
# End Target
# End Project
