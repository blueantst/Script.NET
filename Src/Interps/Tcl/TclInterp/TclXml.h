////////////////////////////////////////////////////////////////////////////
//	File:		TclXml.h
//	Version:	1.0.0.0
//	Created:	2003-04-20
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	Interface define of TclXml.
////////////////////////////////////////////////////////////////////////////
#ifndef __TCLXML_H_
#define __TCLXML_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

void CloseAllTclXmlFile();

int TclXml_Init(Tcl_Interp *interp);


#endif	// __TCLXML_H_