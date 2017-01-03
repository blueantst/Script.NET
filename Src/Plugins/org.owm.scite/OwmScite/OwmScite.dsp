# Microsoft Developer Studio Project File - Name="OwmScite" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=OwmScite - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OwmScite.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OwmScite.mak" CFG="OwmScite - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OwmScite - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "OwmScite - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "OwmScite - Win32 PDBRelease" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OwmScite - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /Od /I "..\scintilla\include" /I "TinyXml" /I "TinyXml\tinyXmlA" /I "MISC\Common" /I "ScitillaComponent" /I "..\..\include" /I "..\..\..\..\..\Xtreme ToolkitPro v12.0.0\Source" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "TIXML_USE_STL" /D "TIXMLA_USE_STL" /Yu"stdafx.h" /FD /Zm200 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 w32SQLite3.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../OwmScite.dll" /libpath:"..\..\..\..\..\Xtreme ToolkitPro v12.0.0\lib\vc60"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "OwmScite - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\scintilla\include" /I "TinyXml" /I "TinyXml\tinyXmlA" /I "MISC\Common" /I "ScitillaComponent" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "TIXML_USE_STL" /D "TIXMLA_USE_STL" /FR /Yu"stdafx.h" /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 w32SQLite3.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../OwmScited.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "OwmScite - Win32 PDBRelease"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "OwmScite___Win32_PDBRelease"
# PROP BASE Intermediate_Dir "OwmScite___Win32_PDBRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PDBRelease"
# PROP Intermediate_Dir "PDBRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /I "..\scintilla\include" /I "TinyXml" /I "TinyXml\tinyXmlA" /I "MISC\Common" /I "ScitillaComponent" /I "..\..\include" /I "..\..\..\..\..\Xtreme ToolkitPro v12.0.0\Source" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "TIXML_USE_STL" /D "TIXMLA_USE_STL" /Yu"stdafx.h" /FD /Zm200 /c
# ADD CPP /nologo /MD /W3 /GX /Zi /Od /I "..\scintilla\include" /I "TinyXml" /I "TinyXml\tinyXmlA" /I "MISC\Common" /I "ScitillaComponent" /I "..\..\include" /I "..\..\..\..\..\Xtreme ToolkitPro v12.0.0\Source" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "TIXML_USE_STL" /D "TIXMLA_USE_STL" /Yu"stdafx.h" /FD /Zm200 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 w32SQLite3.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../OwmScite.dll" /libpath:"..\..\..\..\..\Xtreme ToolkitPro v12.0.0\lib\vc60"
# ADD LINK32 w32SQLite3.lib /nologo /subsystem:windows /dll /debug /machine:I386 /libpath:"..\..\..\..\..\Xtreme ToolkitPro v12.0.0\lib\vc60"

!ENDIF 

# Begin Target

# Name "OwmScite - Win32 Release"
# Name "OwmScite - Win32 Debug"
# Name "OwmScite - Win32 PDBRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ScitillaComponent\AutoCompletion.cpp
# End Source File
# Begin Source File

SOURCE=.\BalanceTree.cpp
# End Source File
# Begin Source File

SOURCE=.\CCrystalPopupWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\ScitillaComponent\CodeSnippet.cpp
# End Source File
# Begin Source File

SOURCE=.\MISC\Common\Common.cpp
# End Source File
# Begin Source File

SOURCE=.\CPopupListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\CPopupListTips.cpp
# End Source File
# Begin Source File

SOURCE=.\CTemplateFormater.cpp
# End Source File
# Begin Source File

SOURCE=.\CTemplateLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\DbSqlite.cpp
# End Source File
# Begin Source File

SOURCE=.\FastMaps.cpp
# End Source File
# Begin Source File

SOURCE=.\FileEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\FileUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\ScitillaComponent\FunctionCallTip.cpp
# End Source File
# Begin Source File

SOURCE=.\Label.cpp
# End Source File
# Begin Source File

SOURCE=.\OwmScite.cpp
# End Source File
# Begin Source File

SOURCE=.\OwmScite.def
# End Source File
# Begin Source File

SOURCE=.\OwmScite.rc
# End Source File
# Begin Source File

SOURCE=.\OwmSciteDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\OwmSciteView.cpp
# End Source File
# Begin Source File

SOURCE=.\Parameters.cpp
# End Source File
# Begin Source File

SOURCE=.\ScitillaComponent\Printer.cpp
# End Source File
# Begin Source File

SOURCE=.\ScitillaComponent\ScintillaEditView.cpp
# End Source File
# Begin Source File

SOURCE=.\WinControls\shortcut\shortcut.cpp
# End Source File
# Begin Source File

SOURCE=.\WinControls\StaticDialog\StaticDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TinyXml\tinystr.cpp
# End Source File
# Begin Source File

SOURCE=.\TinyXml\tinyXmlA\tinystrA.cpp
# End Source File
# Begin Source File

SOURCE=.\TinyXml\tinyxml.cpp
# End Source File
# Begin Source File

SOURCE=.\TinyXml\tinyXmlA\tinyxmlA.cpp
# End Source File
# Begin Source File

SOURCE=.\TinyXml\tinyxmlerror.cpp
# End Source File
# Begin Source File

SOURCE=.\TinyXml\tinyXmlA\tinyxmlerrorA.cpp
# End Source File
# Begin Source File

SOURCE=.\TinyXml\tinyxmlparser.cpp
# End Source File
# Begin Source File

SOURCE=.\TinyXml\tinyXmlA\tinyxmlparserA.cpp
# End Source File
# Begin Source File

SOURCE=.\TpsGeneral.cpp
# End Source File
# Begin Source File

SOURCE=.\TpsNewDocument.cpp
# End Source File
# Begin Source File

SOURCE=.\TpsPrintPage.cpp
# End Source File
# Begin Source File

SOURCE=.\TpsStyle.cpp
# End Source File
# Begin Source File

SOURCE=.\UniConversion.cpp
# End Source File
# Begin Source File

SOURCE=.\Utf8_16.cpp
# End Source File
# Begin Source File

SOURCE=.\ScitillaComponent\xmlMatchedTagsHighlighter.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ScitillaComponent\AutoCompletion.h
# End Source File
# Begin Source File

SOURCE=.\BalanceTree.h
# End Source File
# Begin Source File

SOURCE=.\CCrystalPopupWnd.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrame.h
# End Source File
# Begin Source File

SOURCE=.\ScitillaComponent\CodeSnippet.h
# End Source File
# Begin Source File

SOURCE=.\commdef.h
# End Source File
# Begin Source File

SOURCE=.\MISC\Common\Common.h
# End Source File
# Begin Source File

SOURCE=.\CPopupListBox.h
# End Source File
# Begin Source File

SOURCE=.\CPopupListTips.h
# End Source File
# Begin Source File

SOURCE=.\CTemplateFormater.h
# End Source File
# Begin Source File

SOURCE=.\CTemplateLoader.h
# End Source File
# Begin Source File

SOURCE=.\DbSqlite.h
# End Source File
# Begin Source File

SOURCE=.\editcmd.h
# End Source File
# Begin Source File

SOURCE=.\editreg.h
# End Source File
# Begin Source File

SOURCE=.\FastMaps.h
# End Source File
# Begin Source File

SOURCE=.\FileEvent.h
# End Source File
# Begin Source File

SOURCE=.\FileUtils.h
# End Source File
# Begin Source File

SOURCE=.\ScitillaComponent\FunctionCallTip.h
# End Source File
# Begin Source File

SOURCE=.\keys.h
# End Source File
# Begin Source File

SOURCE=.\Label.h
# End Source File
# Begin Source File

SOURCE=.\OwmScite.h
# End Source File
# Begin Source File

SOURCE=.\OwmSciteDoc.h
# End Source File
# Begin Source File

SOURCE=.\OwmSciteView.h
# End Source File
# Begin Source File

SOURCE=.\Parameters.h
# End Source File
# Begin Source File

SOURCE=.\ScitillaComponent\Printer.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ScitillaComponent\ScintillaEditView.h
# End Source File
# Begin Source File

SOURCE=.\WinControls\shortcut\shortcut.h
# End Source File
# Begin Source File

SOURCE=.\WinControls\StaticDialog\StaticDialog.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TinyXml\tinystr.h
# End Source File
# Begin Source File

SOURCE=.\TinyXml\tinyXmlA\tinystrA.h
# End Source File
# Begin Source File

SOURCE=.\TinyXml\tinyxml.h
# End Source File
# Begin Source File

SOURCE=.\TinyXml\tinyXmlA\tinyxmlA.h
# End Source File
# Begin Source File

SOURCE=.\TpsGeneral.h
# End Source File
# Begin Source File

SOURCE=.\TpsNewDocument.h
# End Source File
# Begin Source File

SOURCE=.\TpsPrintPage.h
# End Source File
# Begin Source File

SOURCE=.\TpsStyle.h
# End Source File
# Begin Source File

SOURCE=.\UniConversion.h
# End Source File
# Begin Source File

SOURCE=.\Utf8_16.h
# End Source File
# Begin Source File

SOURCE=.\ScitillaComponent\Window.h
# End Source File
# Begin Source File

SOURCE=.\ScitillaComponent\xmlMatchedTagsHighlighter.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\155.ico
# End Source File
# Begin Source File

SOURCE=.\res\39.ico
# End Source File
# Begin Source File

SOURCE=.\res\47.ico
# End Source File
# Begin Source File

SOURCE=.\res\atestudioowm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BlueUIDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\develop.ico
# End Source File
# Begin Source File

SOURCE=.\res\Help.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_owm.ico
# End Source File
# Begin Source File

SOURCE=.\res\owm1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\OwmScite.rc2
# End Source File
# Begin Source File

SOURCE=.\res\simpletext.ico
# End Source File
# Begin Source File

SOURCE=.\res\toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\workspc.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
