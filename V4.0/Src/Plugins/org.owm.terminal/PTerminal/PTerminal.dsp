# Microsoft Developer Studio Project File - Name="PTerminal" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=PTerminal - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PTerminal.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PTerminal.mak" CFG="PTerminal - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PTerminal - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PTerminal - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PTerminal - Win32 PDBRelease" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PTerminal - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"../PTerminal.dll"

!ELSEIF  "$(CFG)" == "PTerminal - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ /Zm500 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../PTerminald.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "PTerminal - Win32 PDBRelease"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PTerminal___Win32_PDBRelease"
# PROP BASE Intermediate_Dir "PTerminal___Win32_PDBRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PDBRelease"
# PROP Intermediate_Dir "PDBRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"../PTerminal.dll"
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386

!ENDIF 

# Begin Target

# Name "PTerminal - Win32 Release"
# Name "PTerminal - Win32 Debug"
# Name "PTerminal - Win32 PDBRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CCrystalPopupWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\CPopupListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\CPopupListTips.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgComParam.cpp
# End Source File
# Begin Source File

SOURCE=.\ProtocolRx.cpp
# End Source File
# Begin Source File

SOURCE=.\PTerminal.cpp
# End Source File
# Begin Source File

SOURCE=.\PTerminal.def
# End Source File
# Begin Source File

SOURCE=.\PTerminal.rc
# End Source File
# Begin Source File

SOURCE=.\PTerminalDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\PTerminalView.cpp
# End Source File
# Begin Source File

SOURCE=.\SerialPort.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketDx.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketRx.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketTx.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TelnetClient.cpp
# End Source File
# Begin Source File

SOURCE=.\TpsTerminal.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CCrystalPopupWnd.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrame.h
# End Source File
# Begin Source File

SOURCE=.\ClientSocket.h
# End Source File
# Begin Source File

SOURCE=.\CommLogin.h
# End Source File
# Begin Source File

SOURCE=.\CPopupListBox.h
# End Source File
# Begin Source File

SOURCE=.\CPopupListTips.h
# End Source File
# Begin Source File

SOURCE=.\DlgComParam.h
# End Source File
# Begin Source File

SOURCE=.\IRemoteClient.h
# End Source File
# Begin Source File

SOURCE=.\OptTree.h
# End Source File
# Begin Source File

SOURCE=.\ProtocolRx.h
# End Source File
# Begin Source File

SOURCE=.\PServer.h
# End Source File
# Begin Source File

SOURCE=.\PTerminal.h
# End Source File
# Begin Source File

SOURCE=.\PTerminalDoc.h
# End Source File
# Begin Source File

SOURCE=.\PTerminalView.h
# End Source File
# Begin Source File

SOURCE=.\RCResult.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SerialPort.h
# End Source File
# Begin Source File

SOURCE=.\SocketDx.h
# End Source File
# Begin Source File

SOURCE=.\SocketRx.h
# End Source File
# Begin Source File

SOURCE=.\SocketTx.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Telnet.h
# End Source File
# Begin Source File

SOURCE=.\TelnetClient.h
# End Source File
# Begin Source File

SOURCE=.\Terminal.h
# End Source File
# Begin Source File

SOURCE=.\TermReg.h
# End Source File
# Begin Source File

SOURCE=.\TpsTerminal.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\RES\atestudioowm.bmp
# End Source File
# Begin Source File

SOURCE=".\RES\Hardware - Plug.ico"
# End Source File
# Begin Source File

SOURCE=.\res\idr_owm.ico
# End Source File
# Begin Source File

SOURCE=.\RES\login_title.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\LoginTitle.BMP
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PTerminal.rc2
# End Source File
# Begin Source File

SOURCE=.\res\telnet.ico
# End Source File
# Begin Source File

SOURCE=.\res\toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\workspc.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section PTerminal : {9C7DA1E0-488F-11D5-9673-00C0DF258498}
# 	2:16:ClientSocket.cpp:ClientSocket.cpp
# 	2:20:CLASS: CClientSocket:CClientSocket
# 	2:14:ClientSocket.h:ClientSocket.h
# 	2:19:Application Include:PTerminal.h
# End Section
