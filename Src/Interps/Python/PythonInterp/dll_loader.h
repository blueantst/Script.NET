/*
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 1, or (at your option)
** any later version.

** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.

** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/*
 * Author : Alexandre Parenteau <aubonbeurre@hotmail.com> --- December 1997
 */

/*
 * dll_loader.h --- code to load dynamic code
 */

#ifndef DLLLOADER_H
#define DLLLOADER_H

typedef HINSTANCE CompatConnectID;

#ifdef __cplusplus
extern "C" {
#endif

extern void* sCompatProc;
	
int CompatLoadLibrary(CompatConnectID* connID, const char* name_dll);
int CompatCloseLibrary(CompatConnectID* connID);
void* _CompatCallLibrary(CompatConnectID connID, const char* name_func);

#ifdef WIN32
void ResetFailedLibrary();
#endif

/*!
	Call a function
	\param ID Connection ID
	\param res Result
	\param idf macDescInfo (0 if not mac)
	\param nf Function name
	\param tf Function prototype
	\param a1 Argument
*/
#define CompatCallLibrary1(ID, res, idf, nf, tf, a1) \
		( \
			sCompatProc = _CompatCallLibrary(ID, nf), \
			res = sCompatProc != NULL ? \
				((tf)sCompatProc)(a1) : res, \
			sCompatProc != NULL \
		)

/*!
	Call a function
	\param ID Connection ID
	\param res Result
	\param idf macDescInfo (0 if not mac)
	\param nf Function name
	\param tf Function prototype
	\param a1 Argument
	\param a2 Argument
*/
#define CompatCallLibrary2(ID, res, idf, nf, tf, a1, a2) \
		( \
			sCompatProc = _CompatCallLibrary(ID, nf), \
			res = sCompatProc != NULL ? \
				((tf)sCompatProc)(a1, a2) : res, \
			sCompatProc != NULL \
		)

/*!
	Call a function
	\param ID Connection ID
	\param res Result
	\param idf macDescInfo (0 if not mac)
	\param nf Function name
	\param tf Function prototype
	\param a1 Argument
	\param a2 Argument
	\param a3 Argument
*/
#define CompatCallLibrary3(ID, res, idf, nf, tf, a1, a2, a3) \
		( \
			sCompatProc = _CompatCallLibrary(ID, nf), \
			res = sCompatProc != NULL ? \
				((tf)sCompatProc)(a1, a2, a3) : res, \
			sCompatProc != NULL \
		)

/*!
	Used with CompatCallLibrary when you want to give a function as an argument 
	\param funcInfo	For mac preCarbon the desc. of the function arguments, if not 0
	\param func Function
*/
#define CompatFuncArg(funcInfo, func) func

#ifdef __cplusplus
}
#endif

#endif /* DLLLOADER_H */
