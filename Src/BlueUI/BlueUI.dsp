# Microsoft Developer Studio Project File - Name="BlueUI" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=BlueUI - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BlueUI.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BlueUI.mak" CFG="BlueUI - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BlueUI - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "BlueUI - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "BlueUI - Win32 PDBRelease" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BlueUI - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 xerces-c_2.lib /nologo /subsystem:windows /machine:I386 /out:"../Bin/BlueUI.exe"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "BlueUI - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 xerces-c_2D.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../Bind/BlueUId.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "BlueUI - Win32 PDBRelease"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BlueUI___Win32_PDBRelease"
# PROP BASE Intermediate_Dir "BlueUI___Win32_PDBRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PDBRelease"
# PROP Intermediate_Dir "PDBRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 xerces-c_2.lib /nologo /subsystem:windows /machine:I386 /out:"../Bin/BlueUI.exe"
# ADD LINK32 xerces-c_2.lib /nologo /subsystem:windows /debug /machine:I386

!ENDIF 

# Begin Target

# Name "BlueUI - Win32 Release"
# Name "BlueUI - Win32 Debug"
# Name "BlueUI - Win32 PDBRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BlueUI.cpp
# End Source File
# Begin Source File

SOURCE=.\BlueUI.rc
# End Source File
# Begin Source File

SOURCE=.\BlueUIDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\BlueUIView.cpp
# End Source File
# Begin Source File

SOURCE=.\cgfiltyp.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ComboBoxSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\DIB.CPP
# End Source File
# Begin Source File

SOURCE=.\FileVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashWin.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl\FlatHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\InetTools.cpp
# End Source File
# Begin Source File

SOURCE=.\Label.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\mdump.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\PanePropertyGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\PlatOwmCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\PlatUI.cpp
# End Source File
# Begin Source File

SOURCE=.\PlugIn.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Project.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl\ReportCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl\ReportOptionsCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportOutCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinProgress.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TaskDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TclLoginDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Timer.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools.cpp
# End Source File
# Begin Source File

SOURCE=.\TpsAssociate.cpp
# End Source File
# Begin Source File

SOURCE=.\TpsFont.cpp
# End Source File
# Begin Source File

SOURCE=.\TpsInterpDirect.cpp
# End Source File
# Begin Source File

SOURCE=.\TpsLanguage.cpp
# End Source File
# Begin Source File

SOURCE=.\TpsUIStyle.cpp
# End Source File
# Begin Source File

SOURCE=.\TpsVci.cpp
# End Source File
# Begin Source File

SOURCE=.\TpsWorkspace.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeOptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeOutCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TreePropertySheet.cpp
# End Source File
# Begin Source File

SOURCE=.\TreePropertySheetAnimateWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLDom.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BlueUI.h
# End Source File
# Begin Source File

SOURCE=.\BlueUIDoc.h
# End Source File
# Begin Source File

SOURCE=.\BlueUIView.h
# End Source File
# Begin Source File

SOURCE=.\cgfiltyp.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ComboBoxSearch.h
# End Source File
# Begin Source File

SOURCE=.\DIB.H
# End Source File
# Begin Source File

SOURCE=.\DockPaneWnd.h
# End Source File
# Begin Source File

SOURCE=.\FileVersion.h
# End Source File
# Begin Source File

SOURCE=.\FlashWin.h
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl\FlatHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\InetTools.h
# End Source File
# Begin Source File

SOURCE=.\Label.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\mdump.h
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\OptionTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\OutputCtrl.h
# End Source File
# Begin Source File

SOURCE=.\PanePropertyGrid.h
# End Source File
# Begin Source File

SOURCE=.\plat.h
# End Source File
# Begin Source File

SOURCE=.\PlatReg.h
# End Source File
# Begin Source File

SOURCE=.\PlatUI.h
# End Source File
# Begin Source File

SOURCE=.\PlugIn.h
# End Source File
# Begin Source File

SOURCE=.\ProgDlg.h
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl\ReportCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl\ReportOptionsCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ReportOutCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SkinProgress.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TaskDialog.h
# End Source File
# Begin Source File

SOURCE=.\TclLoginDlg.h
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Timer.h
# End Source File
# Begin Source File

SOURCE=.\Tools.h
# End Source File
# Begin Source File

SOURCE=.\TpsAssociate.h
# End Source File
# Begin Source File

SOURCE=.\TpsFont.h
# End Source File
# Begin Source File

SOURCE=.\TpsInterpDirect.h
# End Source File
# Begin Source File

SOURCE=.\TpsLanguage.h
# End Source File
# Begin Source File

SOURCE=.\TpsUIStyle.h
# End Source File
# Begin Source File

SOURCE=.\TpsVci.h
# End Source File
# Begin Source File

SOURCE=.\TpsWorkspace.h
# End Source File
# Begin Source File

SOURCE=.\TreeOptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\TreeOutCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TreePropertySheet.h
# End Source File
# Begin Source File

SOURCE=.\TreePropertySheetAnimateWidget.h
# End Source File
# Begin Source File

SOURCE=.\XMLDom.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\1.ico
# End Source File
# Begin Source File

SOURCE=.\res\155.ico
# End Source File
# Begin Source File

SOURCE=.\res\16.ico
# End Source File
# Begin Source File

SOURCE=.\res\256.ico
# End Source File
# Begin Source File

SOURCE=.\res\6.ico
# End Source File
# Begin Source File

SOURCE=.\res\63.ico
# End Source File
# Begin Source File

SOURCE=.\res\80.ico
# End Source File
# Begin Source File

SOURCE=.\res\abouttitle.bmp
# End Source File
# Begin Source File

SOURCE=.\res\abouttitle_scriptdev.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BlueUI.ico
# End Source File
# Begin Source File

SOURCE=.\res\BlueUI.rc2
# End Source File
# Begin Source File

SOURCE=.\res\BlueUIDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\CloseMSN.bmp
# End Source File
# Begin Source File

SOURCE=.\res\devicon.ico
# End Source File
# Begin Source File

SOURCE=.\res\flash2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_inf.ico
# End Source File
# Begin Source File

SOURCE=.\res\img_reportctrl.bmp
# End Source File
# Begin Source File

SOURCE=.\res\login_title.bmp
# End Source File
# Begin Source File

SOURCE=.\res\optionmarks.bmp
# End Source File
# Begin Source File

SOURCE=.\res\options.bmp
# End Source File
# Begin Source File

SOURCE=.\res\output.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PaneProperties.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
