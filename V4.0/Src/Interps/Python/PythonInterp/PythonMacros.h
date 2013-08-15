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

#ifndef __PY_MACROS
#define __PY_MACROS

// it is important with Python to not throw, instead we need
// to intercept each exception and turn it into a Python exception
extern void PyHandlePlatError(CException* e);
extern void PyHandlePlatError(const char *msg);

/*
 * PY_DECL_CLASS : declate a python wrapper class
 * PY_IMPL_CLASS : implement python wrapper class
 */

#define _COMMON_PY_DECL_CLASS(c) \
public: \
	typedef struct \
	{ \
		PyObject_HEAD \
		Py##c *m_object; \
	} obj_t; \
	static const char *m_doc; \
	static void dealloc(obj_t *self); \
	static PyObject *getattr(obj_t *self, char *name); \
	static PyAutoObject<obj_t> New(void); \
		static PyMethodDef m_methods[]; \
	static PyRegistry m_registry; \
	static inline PyTypeObject *GetType(void) { return &m_registry.m_type; } \
	static PyObject *Create(PyObject *selfptr, PyObject *args); \
	static void InitValues(PyObject *d);

#define PY_DECL_CLASS_INHERITED(c, a) \
	_COMMON_PY_DECL_CLASS(c) \
	static PyObject *FindMethod(PyObject *self, char *name) \
	{ \
		PyObject *res = Py_FindMethod(m_methods, self, name); \
		if(res != NULL) \
			return res; \
		PyErr_Clear(); \
		return Py##a::FindMethod(self, name); \
	}

#define PY_DECL_CLASS(c) \
	_COMMON_PY_DECL_CLASS(c) \
	static PyObject *FindMethod(PyObject *self, char *name) \
	{ \
		return Py_FindMethod(m_methods, self, name); \
	}
	

#define PY_IMPL_CLASS(c, adoc) \
	PyRegistry Py##c::m_registry("C"#c, sizeof(Py##c::obj_t), (destructor)Py##c::dealloc, (getattrfunc)Py##c::getattr, \
		Py##c::InitValues, (PyCFunction)Py##c::Create, Py##c::m_doc); \
	PyAutoObject<Py##c::obj_t> Py##c::New(void) \
	{ \
		Py##c::obj_t *self = PyObject_New(Py##c::obj_t, Py##c::GetType()); \
		self->m_object = NULL; \
		PyAutoObject<Py##c::obj_t> res(self); \
		Py_DECREF(self); \
		return res; \
	} \
	const char *Py##c::m_doc = adoc; \
 \
	void Py##c::dealloc(obj_t *self) \
	{ \
		delete self->m_object; \
		PyObject_Del(self); \
	} \
 \
	PyObject *Py##c::getattr(obj_t *self, char *name) \
	{ \
		if(self->m_object) \
		{ \
			TRY \
			{ \
				PyObject *res = self->m_object->GetAttr(name); \
				if(res) \
					return res; \
			} \
			CATCH_ALL(e) \
			{ \
				PyHandlePlatError(e); \
				return NULL; \
			} \
			END_CATCH_ALL \
		} \
		return Py##c::FindMethod((PyObject *)self, name); \
	}

#define PY_DECL_METHOD(c, m) \
protected: \
	static PyObject *_##m(obj_t *self, PyObject *args) \
	{ \
		TRY \
		{ \
			if(self->m_object) \
				return self->m_object->##m(args); \
		} \
		CATCH_ALL(e) \
		{ \
			PyHandlePlatError(e); \
			return NULL; \
		} \
		END_CATCH_ALL \
		return NULL; \
	} \
public: \
	PyObject *m(PyObject *args);

#define PY_BEGIN_IMPL_METHOD(c) \
	PyMethodDef Py##c::m_methods[] = \
	{

#define PY_IMPL_METHOD(c, m, adoc) \
	{#m, (binaryfunc)Py##c::_##m, METH_VARARGS, (char *)adoc},

#define PY_END_IMPL_METHOD \
		{NULL, NULL} \
	};

#endif
