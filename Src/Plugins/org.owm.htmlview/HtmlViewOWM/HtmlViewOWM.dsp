# Microsoft Developer Studio Project File - Name="HtmlViewOWM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=HtmlViewOWM - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HtmlViewOWM.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HtmlViewOWM.mak" CFG="HtmlViewOWM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HtmlViewOWM - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "HtmlViewOWM - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "HtmlViewOWM - Win32 PDBRelease" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HtmlViewOWM - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"../HtmlViewOWM.dll"

!ELSEIF  "$(CFG)" == "HtmlViewOWM - Win32 Debug"

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
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../HtmlViewOWMd.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "HtmlViewOWM - Win32 PDBRelease"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "HtmlViewOWM___Win32_PDBRelease"
# PROP BASE Intermediate_Dir "HtmlViewOWM___Win32_PDBRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PDBRelease"
# PROP Intermediate_Dir "PDBRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"../HtmlViewOWM.dll"
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386

!ENDIF 

# Begin Target

# Name "HtmlViewOWM - Win32 Release"
# Name "HtmlViewOWM - Win32 Debug"
# Name "HtmlViewOWM - Win32 PDBRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ChildFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\ComboBoxWord.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlFavorites.cpp
# End Source File
# Begin Source File

SOURCE=.\CustSite.cpp
# End Source File
# Begin Source File

SOURCE=.\CWebBrowser2.cpp
# End Source File
# Begin Source File

SOURCE=.\DynamicHelpBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ExplorerBar.cpp
# End Source File
# Begin Source File

SOURCE=.\HtmlViewOWM.cpp
# End Source File
# Begin Source File

SOURCE=.\HtmlViewOWM.def
# End Source File
# Begin Source File

SOURCE=.\HtmlViewOWM.rc
# End Source File
# Begin Source File

SOURCE=.\InetTools.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StylerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\StylerView.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ChildFrame.h
# End Source File
# Begin Source File

SOURCE=.\ComboBoxWord.h
# End Source File
# Begin Source File

SOURCE=.\ControlFavorites.h
# End Source File
# Begin Source File

SOURCE=.\CWebBrowser2.h
# End Source File
# Begin Source File

SOURCE=.\DynamicHelpBar.h
# End Source File
# Begin Source File

SOURCE=.\ExplorerBar.h
# End Source File
# Begin Source File

SOURCE=.\HtmlViewOWM.h
# End Source File
# Begin Source File

SOURCE=.\InetTools.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StylerDoc.h
# End Source File
# Begin Source File

SOURCE=.\StylerView.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\disabled_bar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dynamic_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\explorer_bar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HtmlViewOWM.rc2
# End Source File
# Begin Source File

SOURCE=.\res\idr_owm.ico
# End Source File
# Begin Source File

SOURCE=.\res\MenuBar_a.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Tabs.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
