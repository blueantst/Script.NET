# Microsoft Developer Studio Generated NMAKE File, Based on PTerminal.dsp
!IF "$(CFG)" == ""
CFG=PTerminal - Win32 Debug
!MESSAGE No configuration specified. Defaulting to PTerminal - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "PTerminal - Win32 Release" && "$(CFG)" != "PTerminal - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PTerminal.mak" CFG="PTerminal - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PTerminal - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PTerminal - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PTerminal - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\PTerminal.dll"


CLEAN :
	-@erase "$(INTDIR)\CCrystalPopupWnd.obj"
	-@erase "$(INTDIR)\ChildFrame.obj"
	-@erase "$(INTDIR)\ClientSocket.obj"
	-@erase "$(INTDIR)\CPopupListBox.obj"
	-@erase "$(INTDIR)\CPopupListTips.obj"
	-@erase "$(INTDIR)\PTerminal.obj"
	-@erase "$(INTDIR)\PTerminal.pch"
	-@erase "$(INTDIR)\PTerminal.res"
	-@erase "$(INTDIR)\PTerminalDoc.obj"
	-@erase "$(INTDIR)\PTerminalView.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TelnetClient.obj"
	-@erase "$(INTDIR)\TpsTerminal.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\PTerminal.dll"
	-@erase "$(OUTDIR)\PTerminal.exp"
	-@erase "$(OUTDIR)\PTerminal.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\PTerminal.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\PTerminal.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PTerminal.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\PTerminal.pdb" /machine:I386 /def:".\PTerminal.def" /out:"$(OUTDIR)\PTerminal.dll" /implib:"$(OUTDIR)\PTerminal.lib" 
DEF_FILE= \
	".\PTerminal.def"
LINK32_OBJS= \
	"$(INTDIR)\CCrystalPopupWnd.obj" \
	"$(INTDIR)\ChildFrame.obj" \
	"$(INTDIR)\ClientSocket.obj" \
	"$(INTDIR)\CPopupListBox.obj" \
	"$(INTDIR)\CPopupListTips.obj" \
	"$(INTDIR)\PTerminal.obj" \
	"$(INTDIR)\PTerminalDoc.obj" \
	"$(INTDIR)\PTerminalView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TelnetClient.obj" \
	"$(INTDIR)\TpsTerminal.obj" \
	"$(INTDIR)\PTerminal.res"

"$(OUTDIR)\PTerminal.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "PTerminal - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\PTerminal.dll" "$(OUTDIR)\PTerminal.bsc"


CLEAN :
	-@erase "$(INTDIR)\CCrystalPopupWnd.obj"
	-@erase "$(INTDIR)\CCrystalPopupWnd.sbr"
	-@erase "$(INTDIR)\ChildFrame.obj"
	-@erase "$(INTDIR)\ChildFrame.sbr"
	-@erase "$(INTDIR)\ClientSocket.obj"
	-@erase "$(INTDIR)\ClientSocket.sbr"
	-@erase "$(INTDIR)\CPopupListBox.obj"
	-@erase "$(INTDIR)\CPopupListBox.sbr"
	-@erase "$(INTDIR)\CPopupListTips.obj"
	-@erase "$(INTDIR)\CPopupListTips.sbr"
	-@erase "$(INTDIR)\PTerminal.obj"
	-@erase "$(INTDIR)\PTerminal.pch"
	-@erase "$(INTDIR)\PTerminal.res"
	-@erase "$(INTDIR)\PTerminal.sbr"
	-@erase "$(INTDIR)\PTerminalDoc.obj"
	-@erase "$(INTDIR)\PTerminalDoc.sbr"
	-@erase "$(INTDIR)\PTerminalView.obj"
	-@erase "$(INTDIR)\PTerminalView.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TelnetClient.obj"
	-@erase "$(INTDIR)\TelnetClient.sbr"
	-@erase "$(INTDIR)\TpsTerminal.obj"
	-@erase "$(INTDIR)\TpsTerminal.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\PTerminal.bsc"
	-@erase "$(OUTDIR)\PTerminal.dll"
	-@erase "$(OUTDIR)\PTerminal.exp"
	-@erase "$(OUTDIR)\PTerminal.ilk"
	-@erase "$(OUTDIR)\PTerminal.lib"
	-@erase "$(OUTDIR)\PTerminal.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\PTerminal.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\PTerminal.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PTerminal.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\CCrystalPopupWnd.sbr" \
	"$(INTDIR)\ChildFrame.sbr" \
	"$(INTDIR)\ClientSocket.sbr" \
	"$(INTDIR)\CPopupListBox.sbr" \
	"$(INTDIR)\CPopupListTips.sbr" \
	"$(INTDIR)\PTerminal.sbr" \
	"$(INTDIR)\PTerminalDoc.sbr" \
	"$(INTDIR)\PTerminalView.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\TelnetClient.sbr" \
	"$(INTDIR)\TpsTerminal.sbr"

"$(OUTDIR)\PTerminal.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\PTerminal.pdb" /debug /machine:I386 /def:".\PTerminal.def" /out:"$(OUTDIR)\PTerminal.dll" /implib:"$(OUTDIR)\PTerminal.lib" /pdbtype:sept 
DEF_FILE= \
	".\PTerminal.def"
LINK32_OBJS= \
	"$(INTDIR)\CCrystalPopupWnd.obj" \
	"$(INTDIR)\ChildFrame.obj" \
	"$(INTDIR)\ClientSocket.obj" \
	"$(INTDIR)\CPopupListBox.obj" \
	"$(INTDIR)\CPopupListTips.obj" \
	"$(INTDIR)\PTerminal.obj" \
	"$(INTDIR)\PTerminalDoc.obj" \
	"$(INTDIR)\PTerminalView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TelnetClient.obj" \
	"$(INTDIR)\TpsTerminal.obj" \
	"$(INTDIR)\PTerminal.res"

"$(OUTDIR)\PTerminal.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("PTerminal.dep")
!INCLUDE "PTerminal.dep"
!ELSE 
!MESSAGE Warning: cannot find "PTerminal.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "PTerminal - Win32 Release" || "$(CFG)" == "PTerminal - Win32 Debug"
SOURCE=.\CCrystalPopupWnd.cpp

!IF  "$(CFG)" == "PTerminal - Win32 Release"


"$(INTDIR)\CCrystalPopupWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PTerminal.pch"


!ELSEIF  "$(CFG)" == "PTerminal - Win32 Debug"


"$(INTDIR)\CCrystalPopupWnd.obj"	"$(INTDIR)\CCrystalPopupWnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PTerminal.pch"


!ENDIF 

SOURCE=.\ChildFrame.cpp

!IF  "$(CFG)" == "PTerminal - Win32 Release"


"$(INTDIR)\ChildFrame.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PTerminal.pch"


!ELSEIF  "$(CFG)" == "PTerminal - Win32 Debug"


"$(INTDIR)\ChildFrame.obj"	"$(INTDIR)\ChildFrame.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PTerminal.pch"


!ENDIF 

SOURCE=.\ClientSocket.cpp

!IF  "$(CFG)" == "PTerminal - Win32 Release"


"$(INTDIR)\ClientSocket.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PTerminal.pch"


!ELSEIF  "$(CFG)" == "PTerminal - Win32 Debug"


"$(INTDIR)\ClientSocket.obj"	"$(INTDIR)\ClientSocket.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PTerminal.pch"


!ENDIF 

SOURCE=.\CPopupListBox.cpp

!IF  "$(CFG)" == "PTerminal - Win32 Release"


"$(INTDIR)\CPopupListBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PTerminal.pch"


!ELSEIF  "$(CFG)" == "PTerminal - Win32 Debug"


"$(INTDIR)\CPopupListBox.obj"	"$(INTDIR)\CPopupListBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PTerminal.pch"


!ENDIF 

SOURCE=.\CPopupListTips.cpp

!IF  "$(CFG)" == "PTerminal - Win32 Release"


"$(INTDIR)\CPopupListTips.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PTerminal.pch"


!ELSEIF  "$(CFG)" == "PTerminal - Win32 Debug"


"$(INTDIR)\CPopupListTips.obj"	"$(INTDIR)\CPopupListTips.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PTerminal.pch"


!ENDIF 

SOURCE=.\PTerminal.cpp

!IF  "$(CFG)" == "PTerminal - Win32 Release"


"$(INTDIR)\PTerminal.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PTerminal.pch"


!ELSEIF  "$(CFG)" == "PTerminal - Win32 Debug"


"$(INTDIR)\PTerminal.obj"	"$(INTDIR)\PTerminal.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PTerminal.pch"


!ENDIF 

SOURCE=.\PTerminal.rc

"$(INTDIR)\PTerminal.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\PTerminalDoc.cpp

!IF  "$(CFG)" == "PTerminal - Win32 Release"


"$(INTDIR)\PTerminalDoc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PTerminal.pch"


!ELSEIF  "$(CFG)" == "PTerminal - Win32 Debug"


"$(INTDIR)\PTerminalDoc.obj"	"$(INTDIR)\PTerminalDoc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PTerminal.pch"


!ENDIF 

SOURCE=.\PTerminalView.cpp

!IF  "$(CFG)" == "PTerminal - Win32 Release"


"$(INTDIR)\PTerminalView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PTerminal.pch"


!ELSEIF  "$(CFG)" == "PTerminal - Win32 Debug"


"$(INTDIR)\PTerminalView.obj"	"$(INTDIR)\PTerminalView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PTerminal.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "PTerminal - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)\PTerminal.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\PTerminal.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "PTerminal - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\PTerminal.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\PTerminal.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TelnetClient.cpp

!IF  "$(CFG)" == "PTerminal - Win32 Release"


"$(INTDIR)\TelnetClient.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PTerminal.pch"


!ELSEIF  "$(CFG)" == "PTerminal - Win32 Debug"


"$(INTDIR)\TelnetClient.obj"	"$(INTDIR)\TelnetClient.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PTerminal.pch"


!ENDIF 

SOURCE=.\TpsTerminal.cpp

!IF  "$(CFG)" == "PTerminal - Win32 Release"


"$(INTDIR)\TpsTerminal.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PTerminal.pch"


!ELSEIF  "$(CFG)" == "PTerminal - Win32 Debug"


"$(INTDIR)\TpsTerminal.obj"	"$(INTDIR)\TpsTerminal.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PTerminal.pch"


!ENDIF 


!ENDIF 

