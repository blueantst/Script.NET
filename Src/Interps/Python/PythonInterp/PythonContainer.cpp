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

#include "stdafx.h"

#include "PythonContainer.h"

#include <vector>

PyObject *gPlatErr = NULL;

PyRegistry *PyRegistry::m_all = NULL;

PyRegistry::PyRegistry(const char *name, size_t size, destructor d, getattrfunc g,
	PyRegistry_InitValues v, PyCFunction c, const char *doc) : m_v(v), m_c(c), m_doc(doc)
{
	m_next = m_all;
	m_all = this;

	PyTypeObject deftype =
	{
		PyObject_HEAD_INIT(0)
		0,
		(char *)name,
		size,
		0, \
		d,						     /*tp_dealloc*/
		0,                              /*tp_print*/
		g,						      /*tp_getattr*/
		0,                              /*tp_setattr*/
		0,                              /*tp_compare*/
		0,                              /*tp_repr*/
		0,                              /*tp_as_number*/
		0,                              /*tp_as_sequence*/
		0,                              /*tp_as_mapping*/
	};

	m_type = deftype;
	m_type.ob_type = GetPyType_Type();
}

PyRegistry::~PyRegistry()
{
}

void PyRegistry::RegisterAllObjects(std::vector<PyMethodDef> & methods)
{
	PyRegistry *tmp = m_all;
	PyMethodDef def;
	while(tmp != 0L)
	{
		def.ml_name = tmp->m_type.tp_name;
		def.ml_meth = tmp->m_c;
		def.ml_flags = METH_VARARGS;
		def.ml_doc = (char *)tmp->m_doc;

		methods.push_back(def);

		tmp = tmp->m_next;
	}

	memset(&def, 0, sizeof(def));
	methods.push_back(def);
}

void PyRegistry::CallInitValues(PyObject *d)
{
	PyRegistry *tmp = m_all;
	while(tmp != 0L)
	{
		tmp->m_v(d);
		tmp = tmp->m_next;
	}
}

/* Convenience routine to export an integer value.
   For simplicity, errors (which are unlikely anyway) are ignored. */
void PyContainer::RegisterValue(PyObject *d, char *name, int value)
{
	PyObject *v = PyInt_FromLong((long) value);
	if (v == NULL) {
		/* Don't bother reporting this error */
		PyErr_Clear();
	}
	else {
		PyDict_SetItemString(d, name, v);
		Py_DECREF(v);
	}
}

void PyHandlePlatError(CException* e)
{
	char tmp[256];
	if(e->GetErrorMessage(tmp, sizeof(tmp)))
		PyErr_SetString(gPlatErr, tmp);
	else
		PyErr_SetString(gPlatErr, "Unknown error");
}

void PyHandlePlatError(const char *msg)
{
	PyErr_SetString(gPlatErr, msg);
}

