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
 * Author : Alexandre Parenteau <aubonbeurre@hotmail.com> --- September 2001
 */

#ifndef __PYIMPL
#define __PYIMPL

#include "PythonContainer.h"


class CTclInterp;
class PyTclInterp : public PyContainer
{
	PY_DECL_CLASS(TclInterp)
	PY_DECL_METHOD(TclInterp, Execute)
	PY_DECL_METHOD(TclInterp, RunScriptFile)

public:
	PyTclInterp(CString strInterpName);
//	{
//	}

	virtual ~PyTclInterp();
//	{
//	}

	virtual PyObject *GetAttr(char *name);

protected:
	CTclInterp* m_pTclInterp;
	int			m_nInterpID;
};


#endif
