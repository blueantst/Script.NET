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
 * dll_loader.cpp --- code to load dynamic code
 */

//#ifdef HAVE_CONFIG_H
//#	include "config.h"
//#endif /* HAVE_CONFIG_H */

#include "stdafx.h"

#	include <windows.h>
#	include <winuser.h>
#	include <stdio.h>

#include "dll_loader.h"

#if _MSC_VER >= 1400
#	pragma warning(disable : 4996)
#endif

static void SetLastLoadedLibrary(const char* name);

void* sCompatProc;

/*!
	Load the shared library
	\param connID Return the connection ID
	\param name_dll	Filename of the library (on Mac the fragment name of the shared library)
	\return true on success, false otherwise
*/
int CompatLoadLibrary(CompatConnectID* connID, const char* name_dll)
{
	int loadFailed = FALSE;
	int confirmLoad = FALSE;

	if( connID == 0L )
		return 0;

	// In case the dll caused problems we will give user a warning and a chance to cancel load
	{
		char lastLibPath[_MAX_PATH] = "";

		CString strLastLib = AfxGetApp()->GetProfileString(REG_PY_LIB_SUBKEY, REG_PY_LASTLIBVAL);
		if( stricmp(name_dll, strLastLib) == 0 )
		{
			confirmLoad = TRUE;
		}
		else
		{
			loadFailed = TRUE;
		}
	}

	if( confirmLoad )
	{
		char message[1024] = "";

		sprintf(message, 
			"DLL library loading deadlock detected!\n"
			"File: %s\n"
			"DLL failed to load properly (stalled) during previous attempt. Please review your settings in the preferences and your PATH environmental variable in the system settings to assure this is the correct file and there are no incompatible duplicates that might be accessed instead.\n"
			"You can reset this warning message by deleting the following registry key: %s\n"
			"\nAre you sure you want to attempt loading the file?",
			name_dll,
			"HKEY_CURRENT_USER" "\\" REG_PY_LIB_SUBKEY "\\" REG_PY_LASTLIBVAL);

		if( MessageBox(NULL, 
			message, 
			"DLL load deadlock", 
			MB_ICONERROR | MB_YESNO | MB_DEFBUTTON2 | MB_SYSTEMMODAL) != IDYES )
		{
			return 0L;
		}
	}

	if( !loadFailed || confirmLoad )
	{
		SetLastLoadedLibrary(name_dll);
	}
	
	*connID = LoadLibrary(name_dll);
	
	if( !loadFailed || confirmLoad )
	{
		SetLastLoadedLibrary("");
	}

	return (*connID) != 0L;
}

/*!
	Load the shared library
	\param connID The connection ID, should not be used anymore after this call 
	\return true on success, false otherwise
*/
int CompatCloseLibrary(CompatConnectID* connID)
{
	if( connID == 0L )
		return 0;

	FreeLibrary(*connID);
	connID = 0L;

	return 1;
}

/*!
	Call a function in the shared library
	\param connID The connection ID
	\param name_func Function name be called
	\return The function address
*/
void* _CompatCallLibrary(CompatConnectID connID, const char* name_func)
{
	return GetProcAddress(connID, name_func);
}

//////////////////////////////////////////////////////////////////////////
// Internal functions

/*!
	Set last loaded library setting
	\param name Name of the last loaded library, empty string to reset
*/
static void SetLastLoadedLibrary(const char* name)
{
	AfxGetApp()->WriteProfileString(REG_PY_LIB_SUBKEY, REG_PY_LASTLIBVAL, name);
}

/*!
	Reset failed library setting
	\note To be called at the exit of application since a controlled exit means correct loading
*/
void ResetFailedLibrary()
{
	SetLastLoadedLibrary("");
}
