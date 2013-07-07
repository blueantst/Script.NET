// commdef.h : include comm define
//

#if !defined(__COMMDEF_H_)
#define __COMMDEF_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


enum LangType {L_TXT, L_PHP , L_C, L_CPP, L_CS, L_OBJC, L_JAVA, L_RC,\
			   L_HTML, L_XML, L_MAKEFILE, L_PASCAL, L_BATCH, L_INI, L_NFO, L_USER,\
			   L_ASP, L_SQL, L_VB, L_JS, L_CSS, L_PERL, L_PYTHON, L_LUA,\
			   L_TEX, L_FORTRAN, L_BASH, L_FLASH, L_NSIS, L_TCL, L_LISP, L_SCHEME,\
			   L_ASM, L_DIFF, L_PROPS, L_PS, L_RUBY, L_SMALLTALK, L_VHDL, L_KIX, L_AU3,\
			   L_CAML, L_ADA, L_VERILOG, L_MATLAB, L_HASKELL, L_INNO, L_SEARCHRESULT,\
			   L_CMAKE, L_YAML, L_BSDL, L_SVF,\
			   // The end of enumated language type, so it should be always at the end
			   L_EXTERNAL};

#define L_UNKNOWN	1000

#endif // __COMMDEF_H_
