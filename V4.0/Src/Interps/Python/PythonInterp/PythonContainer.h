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

#ifndef __PY_CONTAINER
#define __PY_CONTAINER

#if !qCvsDebug
	// force to not import Python
#	define USE_DL_EXPORT
#endif

#include <Python.h>
#include <exception>
#include <vector>
#include "PythonMacros.h"
#include "PythonGlue.impl.h"

typedef void (*PyRegistry_InitValues)(PyObject *d);

/*
 * Each C++ for Python has a registry which help us registring
 * the object to Python
 */

class PyRegistry
{
public:
	PyRegistry(const char *name, size_t size, destructor d, getattrfunc g,
		PyRegistry_InitValues v, PyCFunction c, const char *doc);
	~PyRegistry();

	PyTypeObject m_type;

	static void RegisterAllObjects(std::vector<PyMethodDef> & methods);
		// initialize each wrapper class and return a method table
		// for the module

	static void CallInitValues(PyObject *dict);
		// give a chance for each container to make some registrations

protected:
	PyRegistry *m_next;
	PyRegistry_InitValues m_v;
	PyCFunction m_c;
	const char *m_doc;

	static PyRegistry *m_all;
};

/*
 * Base for a C++ container wrapper
 */

class PyContainer
{
public:
	PyContainer() {}
	virtual ~PyContainer() {}

	virtual PyObject *GetAttr(char *name) { return NULL; } 
		// overide to implement attributes

	static PyObject *True() { Py_INCREF(GetPyTrue()); return GetPyTrue(); }
	static PyObject *False() { Py_INCREF(GetPyFalse()); return GetPyFalse(); }
	static PyObject *Void() { Py_INCREF(GetPyNone()); return GetPyNone(); }
		// goodies for returning void or boolean to python

	static void RegisterValue(PyObject *d, char *name, int value);
		// register a single constant value in the module dictionary
};

// smart pointer for python PyObject
template <class T>
class PyAutoObject
{
public:
	inline PyAutoObject() : m_o(NULL)
	{
	}

	inline PyAutoObject(T *o) : m_o(o)
	{
		Ref();
	}

	inline PyAutoObject(PyObject *o) : m_o((T *)o)
	{
		Ref();
	}

	inline PyAutoObject(PyAutoObject & obj) : m_o((T *)obj.CopyReturn())
	{
	}

	inline operator T*() const	{ return m_o; }

	inline ~PyAutoObject()
	{
		UnRef();
	}

	inline PyAutoObject & operator=(T *o)
	{
		UnRef();
		m_o = o;
		Ref();
		return *this;
	}

	inline PyAutoObject & operator=(PyObject *o)
	{
		UnRef();
		m_o = (T *)o;
		Ref();
		return *this;
	}

	inline PyAutoObject & operator=(PyAutoObject & obj)
	{
		UnRef();
		m_o = (T *)obj.CopyReturn();
		return *this;
	}

	inline PyObject *CopyReturn(void)
	{
		Ref();
		return (PyObject *)m_o;
	}

	inline bool IsValid(void)
	{
		return m_o != 0L;
	}

	inline T *operator->(void)
	{
		return m_o;
	}

protected:
	inline void Ref(void)
	{
		if(m_o)
			Py_INCREF(m_o);
	}

	inline void UnRef(void)
	{
		if(m_o)
		{
			Py_DECREF(m_o);
			m_o = NULL;
		}
	}

	T *m_o;
};

extern PyObject *gPlatErr;


#endif
