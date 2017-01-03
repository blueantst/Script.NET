# Microsoft Developer Studio Generated NMAKE File, Based on WorkSpace.dsp
!IF "$(CFG)" == ""
CFG=WorkSpace - Win32 Debug
!MESSAGE No configuration specified. Defaulting to WorkSpace - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "WorkSpace - Win32 Release" && "$(CFG)" != "WorkSpace - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WorkSpace.mak" CFG="WorkSpace - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WorkSpace - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "WorkSpace - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "WorkSpace - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\WorkSpace.dll"


CLEAN :
	-@erase "$(INTDIR)\ChildFrame.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TclCmd.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WorkSpace.obj"
	-@erase "$(INTDIR)\WorkSpace.pch"
	-@erase "$(INTDIR)\WorkSpace.res"
	-@erase "$(INTDIR)\WorkSpaceBar.obj"
	-@erase "$(INTDIR)\WorkSpaceDoc.obj"
	-@erase "$(INTDIR)\WorkSpaceView.obj"
	-@erase "$(INTDIR)\WorkTreeCtrl.obj"
	-@erase "$(OUTDIR)\WorkSpace.dll"
	-@erase "$(OUTDIR)\WorkSpace.exp"
	-@erase "$(OUTDIR)\WorkSpace.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "_FORCE" /Fp"$(INTDIR)\WorkSpace.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\WorkSpace.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WorkSpace.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\WorkSpace.pdb" /machine:I386 /def:".\WorkSpace.def" /out:"$(OUTDIR)\WorkSpace.dll" /implib:"$(OUTDIR)\WorkSpace.lib" 
DEF_FILE= \
	".\WorkSpace.def"
LINK32_OBJS= \
	"$(INTDIR)\ChildFrame.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TclCmd.obj" \
	"$(INTDIR)\WorkSpace.obj" \
	"$(INTDIR)\WorkSpaceBar.obj" \
	"$(INTDIR)\WorkSpaceDoc.obj" \
	"$(INTDIR)\WorkSpaceView.obj" \
	"$(INTDIR)\WorkTreeCtrl.obj" \
	"$(INTDIR)\WorkSpace.res"

"$(OUTDIR)\WorkSpace.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "WorkSpace - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\WorkSpace.dll" "$(OUTDIR)\WorkSpace.bsc"


CLEAN :
	-@erase "$(INTDIR)\ChildFrame.obj"
	-@erase "$(INTDIR)\ChildFrame.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TclCmd.obj"
	-@erase "$(INTDIR)\TclCmd.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WorkSpace.obj"
	-@erase "$(INTDIR)\WorkSpace.pch"
	-@erase "$(INTDIR)\WorkSpace.res"
	-@erase "$(INTDIR)\WorkSpace.sbr"
	-@erase "$(INTDIR)\WorkSpaceBar.obj"
	-@erase "$(INTDIR)\WorkSpaceBar.sbr"
	-@erase "$(INTDIR)\WorkSpaceDoc.obj"
	-@erase "$(INTDIR)\WorkSpaceDoc.sbr"
	-@erase "$(INTDIR)\WorkSpaceView.obj"
	-@erase "$(INTDIR)\WorkSpaceView.sbr"
	-@erase "$(INTDIR)\WorkTreeCtrl.obj"
	-@erase "$(INTDIR)\WorkTreeCtrl.sbr"
	-@erase "$(OUTDIR)\WorkSpace.bsc"
	-@erase "$(OUTDIR)\WorkSpace.dll"
	-@erase "$(OUTDIR)\WorkSpace.exp"
	-@erase "$(OUTDIR)\WorkSpace.ilk"
	-@erase "$(OUTDIR)\WorkSpace.lib"
	-@erase "$(OUTDIR)\WorkSpace.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\WorkSpace.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\WorkSpace.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WorkSpace.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ChildFrame.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\TclCmd.sbr" \
	"$(INTDIR)\WorkSpace.sbr" \
	"$(INTDIR)\WorkSpaceBar.sbr" \
	"$(INTDIR)\WorkSpaceDoc.sbr" \
	"$(INTDIR)\WorkSpaceView.sbr" \
	"$(INTDIR)\WorkTreeCtrl.sbr"

"$(OUTDIR)\WorkSpace.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\WorkSpace.pdb" /debug /machine:I386 /def:".\WorkSpace.def" /out:"$(OUTDIR)\WorkSpace.dll" /implib:"$(OUTDIR)\WorkSpace.lib" /pdbtype:sept 
DEF_FILE= \
	".\WorkSpace.def"
LINK32_OBJS= \
	"$(INTDIR)\ChildFrame.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TclCmd.obj" \
	"$(INTDIR)\WorkSpace.obj" \
	"$(INTDIR)\WorkSpaceBar.obj" \
	"$(INTDIR)\WorkSpaceDoc.obj" \
	"$(INTDIR)\WorkSpaceView.obj" \
	"$(INTDIR)\WorkTreeCtrl.obj" \
	"$(INTDIR)\WorkSpace.res"

"$(OUTDIR)\WorkSpace.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("WorkSpace.dep")
!INCLUDE "WorkSpace.dep"
!ELSE 
!MESSAGE Warning: cannot find "WorkSpace.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "WorkSpace - Win32 Release" || "$(CFG)" == "WorkSpace - Win32 Debug"
SOURCE=.\ChildFrame.cpp

!IF  "$(CFG)" == "WorkSpace - Win32 Release"


"$(INTDIR)\ChildFrame.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WorkSpace.pch"


!ELSEIF  "$(CFG)" == "WorkSpace - Win32 Debug"


"$(INTDIR)\ChildFrame.obj"	"$(INTDIR)\ChildFrame.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WorkSpace.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "WorkSpace - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "_FORCE" /Fp"$(INTDIR)\WorkSpace.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\WorkSpace.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "WorkSpace - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\WorkSpace.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\WorkSpace.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TclCmd.cpp

!IF  "$(CFG)" == "WorkSpace - Win32 Release"


"$(INTDIR)\TclCmd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WorkSpace.pch"


!ELSEIF  "$(CFG)" == "WorkSpace - Win32 Debug"


"$(INTDIR)\TclCmd.obj"	"$(INTDIR)\TclCmd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WorkSpace.pch"


!ENDIF 

SOURCE=.\WorkSpace.cpp

!IF  "$(CFG)" == "WorkSpace - Win32 Release"


"$(INTDIR)\WorkSpace.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WorkSpace.pch"


!ELSEIF  "$(CFG)" == "WorkSpace - Win32 Debug"


"$(INTDIR)\WorkSpace.obj"	"$(INTDIR)\WorkSpace.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WorkSpace.pch"


!ENDIF 

SOURCE=.\WorkSpace.rc

"$(INTDIR)\WorkSpace.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\WorkSpaceBar.cpp

!IF  "$(CFG)" == "WorkSpace - Win32 Release"


"$(INTDIR)\WorkSpaceBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WorkSpace.pch"


!ELSEIF  "$(CFG)" == "WorkSpace - Win32 Debug"


"$(INTDIR)\WorkSpaceBar.obj"	"$(INTDIR)\WorkSpaceBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WorkSpace.pch"


!ENDIF 

SOURCE=.\WorkSpaceDoc.cpp

!IF  "$(CFG)" == "WorkSpace - Win32 Release"


"$(INTDIR)\WorkSpaceDoc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WorkSpace.pch"


!ELSEIF  "$(CFG)" == "WorkSpace - Win32 Debug"


"$(INTDIR)\WorkSpaceDoc.obj"	"$(INTDIR)\WorkSpaceDoc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WorkSpace.pch"


!ENDIF 

SOURCE=.\WorkSpaceView.cpp

!IF  "$(CFG)" == "WorkSpace - Win32 Release"


"$(INTDIR)\WorkSpaceView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WorkSpace.pch"


!ELSEIF  "$(CFG)" == "WorkSpace - Win32 Debug"


"$(INTDIR)\WorkSpaceView.obj"	"$(INTDIR)\WorkSpaceView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WorkSpace.pch"


!ENDIF 

SOURCE=.\WorkTreeCtrl.cpp

!IF  "$(CFG)" == "WorkSpace - Win32 Release"


"$(INTDIR)\WorkTreeCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WorkSpace.pch"


!ELSEIF  "$(CFG)" == "WorkSpace - Win32 Debug"


"$(INTDIR)\WorkTreeCtrl.obj"	"$(INTDIR)\WorkTreeCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WorkSpace.pch"


!ENDIF 


!ENDIF 

