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
 * Author : Alexandre Parenteau <aubonbeurre@hotmail.com> --- October 2001
 */

/*
 * PythonGlue.impl.h --- wrappers to the Python library
 */

#ifndef PYTHONGLUE_IMPL_H
#define PYTHONGLUE_IMPL_H

#ifndef PYTHONGLUE_EXTERN
#	define PYTHONGLUE_EXTERN extern
#endif

#if !qCvsDebug
#	define DEC_GLUE(f) (*f##_glue)
#	define VAR_GLUE(f) f##_glue

	PYTHONGLUE_EXTERN PyThreadState * DEC_GLUE(PyThreadState_Swap) (PyThreadState *);
	PYTHONGLUE_EXTERN int DEC_GLUE(PyDict_SetItemString) (PyObject *dp, char *key, PyObject *item);
	PYTHONGLUE_EXTERN void  DEC_GLUE(PyErr_Clear) (void);
	PYTHONGLUE_EXTERN PyObject * DEC_GLUE(PyInt_FromLong) (long);
	PYTHONGLUE_EXTERN void DEC_GLUE(PyErr_SetString) (PyObject *, const char *);
	PYTHONGLUE_EXTERN int DEC_GLUE(PyArg_ParseTuple) (PyObject *, char *, ...);
	PYTHONGLUE_EXTERN PyObject * DEC_GLUE(PyTuple_New) (int size);
	PYTHONGLUE_EXTERN PyObject * DEC_GLUE(Py_BuildValue) (char *, ...);
	PYTHONGLUE_EXTERN FILE * DEC_GLUE(PyFile_AsFile) (PyObject *);
	PYTHONGLUE_EXTERN char * DEC_GLUE(PyString_AsString) (PyObject *);
	PYTHONGLUE_EXTERN int DEC_GLUE(PyTuple_Size) (PyObject *);
	PYTHONGLUE_EXTERN int DEC_GLUE(PyList_Size) (PyObject *);
	PYTHONGLUE_EXTERN int DEC_GLUE(PyRun_SimpleString) (char *);
	PYTHONGLUE_EXTERN int DEC_GLUE(PyRun_SimpleFile) (FILE *, char *);
	PYTHONGLUE_EXTERN void DEC_GLUE(Py_SetProgramName) (char *);
	PYTHONGLUE_EXTERN void DEC_GLUE(Py_Initialize) (void);
	PYTHONGLUE_EXTERN void DEC_GLUE(Py_Finalize) (void);
	PYTHONGLUE_EXTERN int DEC_GLUE(Py_IsInitialized) (void);
	PYTHONGLUE_EXTERN PyThreadState * DEC_GLUE(Py_NewInterpreter) (void);
	PYTHONGLUE_EXTERN void DEC_GLUE(Py_EndInterpreter) (PyThreadState *);
	PYTHONGLUE_EXTERN PyObject * DEC_GLUE(PyString_FromString) (const char *);
	PYTHONGLUE_EXTERN PyObject * DEC_GLUE(PyErr_NewException) (char *name, PyObject *base, PyObject *dict);
	PYTHONGLUE_EXTERN PyObject * DEC_GLUE(PyModule_GetDict) (PyObject *);
	PYTHONGLUE_EXTERN PyObject * DEC_GLUE(Py_InitModule4) (char *name, PyMethodDef *methods, char *doc, PyObject *self, int apiver);
	PYTHONGLUE_EXTERN void DEC_GLUE(PyEval_InitThreads) (void);
	PYTHONGLUE_EXTERN PyObject * DEC_GLUE(Py_FindMethod) (PyMethodDef[], PyObject *, char *);
	PYTHONGLUE_EXTERN PyObject * DEC_GLUE(PyList_GetItem) (PyObject *, int);
	PYTHONGLUE_EXTERN int DEC_GLUE(PyType_IsSubtype) (PyTypeObject *, PyTypeObject *);
	PYTHONGLUE_EXTERN PyObject * DEC_GLUE(PyDict_GetItemString) (PyObject *dp, const char *key);
	PYTHONGLUE_EXTERN PyObject * DEC_GLUE(PyImport_ImportModule) (char *name);
	PYTHONGLUE_EXTERN PyObject * DEC_GLUE(_PyObject_New) (PyTypeObject *);
#	if PY_MAJOR_VERSION == 2 && PY_MINOR_VERSION < 3
	PYTHONGLUE_EXTERN void DEC_GLUE(_PyObject_Del) (PyObject *);
#	else
	PYTHONGLUE_EXTERN void DEC_GLUE(PyObject_Free) (void *);
#	endif
	PYTHONGLUE_EXTERN void DEC_GLUE(PySys_SetPath) (char *);
	PYTHONGLUE_EXTERN char * DEC_GLUE(Py_GetPath) (void);

	//////////////////////////////////////////
	PYTHONGLUE_EXTERN PyObject * DEC_GLUE(PyObject_GetAttrString) (PyObject *, char *);
	PYTHONGLUE_EXTERN int DEC_GLUE(PyObject_SetAttrString) (PyObject *, char *, PyObject *);
	PYTHONGLUE_EXTERN int DEC_GLUE(PyObject_HasAttrString) (PyObject *, char *);
	//////////////////////////////////////////

	PYTHONGLUE_EXTERN PyTypeObject * VAR_GLUE(PyType_Type);
	PYTHONGLUE_EXTERN PyTypeObject * VAR_GLUE(PyFile_Type);
	PYTHONGLUE_EXTERN PyTypeObject * VAR_GLUE(PyList_Type);
	PYTHONGLUE_EXTERN PyTypeObject * VAR_GLUE(PyString_Type);
	PYTHONGLUE_EXTERN PyTypeObject * VAR_GLUE(PyTuple_Type);
	PYTHONGLUE_EXTERN PyTypeObject * VAR_GLUE(PyInt_Type);
	PYTHONGLUE_EXTERN PyObject * VAR_GLUE(_Py_NoneStruct);
	PYTHONGLUE_EXTERN PyObject * VAR_GLUE(_Py_ZeroStruct);
	PYTHONGLUE_EXTERN PyObject * VAR_GLUE(_Py_TrueStruct);

#	define GLUE_WRAP(f) f##_glue

#	define PyThreadState_Swap GLUE_WRAP(PyThreadState_Swap)
#	define PyDict_SetItemString GLUE_WRAP(PyDict_SetItemString)
#	define PyErr_Clear GLUE_WRAP(PyErr_Clear)
#	define PyInt_FromLong GLUE_WRAP(PyInt_FromLong)
#	define PyErr_SetString GLUE_WRAP(PyErr_SetString)
#	define PyArg_ParseTuple GLUE_WRAP(PyArg_ParseTuple)
#	define PyTuple_New GLUE_WRAP(PyTuple_New)
#	define Py_BuildValue GLUE_WRAP(Py_BuildValue)
#	define PyFile_AsFile GLUE_WRAP(PyFile_AsFile)
#	define PyString_AsString GLUE_WRAP(PyString_AsString)
#	define PyTuple_Size GLUE_WRAP(PyTuple_Size)
#	define PyList_Size GLUE_WRAP(PyList_Size)
#	define PyRun_SimpleString GLUE_WRAP(PyRun_SimpleString)
#	define PyRun_SimpleFile GLUE_WRAP(PyRun_SimpleFile)
#	define Py_SetProgramName GLUE_WRAP(Py_SetProgramName)
#	define Py_Initialize GLUE_WRAP(Py_Initialize)
#	define Py_Finalize GLUE_WRAP(Py_Finalize)
#	define Py_IsInitialized GLUE_WRAP(Py_IsInitialized)
#	define Py_NewInterpreter GLUE_WRAP(Py_NewInterpreter)
#	define Py_EndInterpreter GLUE_WRAP(Py_EndInterpreter)
#	define PyString_FromString GLUE_WRAP(PyString_FromString)
#	define PyErr_NewException GLUE_WRAP(PyErr_NewException)
#	define PyModule_GetDict GLUE_WRAP(PyModule_GetDict)
#	define Py_InitModule4 GLUE_WRAP(Py_InitModule4)
#	define PyEval_InitThreads GLUE_WRAP(PyEval_InitThreads)
#	define Py_FindMethod GLUE_WRAP(Py_FindMethod)
#	define PyList_GetItem GLUE_WRAP(PyList_GetItem)
#	define PyType_IsSubtype GLUE_WRAP(PyType_IsSubtype)
#	define PyDict_GetItemString GLUE_WRAP(PyDict_GetItemString)
#	define PyImport_ImportModule GLUE_WRAP(PyImport_ImportModule)
#	define _PyObject_New GLUE_WRAP(_PyObject_New)
#	if PY_MAJOR_VERSION == 2 && PY_MINOR_VERSION < 3
#		define _PyObject_Del GLUE_WRAP(_PyObject_Del)
#	else
#		define PyObject_Free GLUE_WRAP(PyObject_Free)
#	endif
#	define PySys_SetPath GLUE_WRAP(PySys_SetPath)
#	define Py_GetPath GLUE_WRAP(Py_GetPath)
	/////////////////////add by lcmao
#	define Py_SetPythonHome GLUE_WRAP(Py_SetPythonHome)
#	define PyEval_AcquireLock GLUE_WRAP(PyEval_AcquireLock)
#	define PyEval_ReleaseLock GLUE_WRAP(PyEval_ReleaseLock)
#	define PySys_SetArgv GLUE_WRAP(PySys_SetArgv)
#	define PyFile_FromFile GLUE_WRAP(PyFile_FromFile)
#	define PySys_SetObject GLUE_WRAP(PySys_SetObject)
#	define PyEval_SetProfile GLUE_WRAP(PyEval_SetProfile)
#	define PyEval_SetTrace GLUE_WRAP(PyEval_SetTrace)
#	define PyLong_FromLong GLUE_WRAP(PyLong_FromLong)
#	define PyLong_AsLong GLUE_WRAP(PyLong_AsLong)
#	define Py_DecRef GLUE_WRAP(Py_DecRef)
#	define PyObject_GetAttr GLUE_WRAP(PyObject_GetAttr)
#	define PyExc_SystemExit VAR_GLUE(PyExc_SystemExit)
#	define PyObject_Str GLUE_WRAP(PyObject_Str)
#	define PyDict_Next GLUE_WRAP(PyDict_Next)
	////////////////////end add

	//////////////////////////////////////////
#	define PyObject_GetAttrString GLUE_WRAP(PyObject_GetAttrString)
#	define PyObject_SetAttrString GLUE_WRAP(PyObject_SetAttrString)
#	define PyObject_HasAttrString GLUE_WRAP(PyObject_HasAttrString)
	//////////////////////////////////////////

	extern PyTypeObject *GetPyType_Type();
	extern PyTypeObject *GetPyFile_Type();
	extern PyTypeObject *GetPyString_Type();
	extern PyTypeObject *GetPyTuple_Type();
	extern PyTypeObject *GetPyList_Type();
	extern PyTypeObject *GetPyInt_Type();
	extern PyObject *GetPyNone();
	extern PyObject *GetPyTrue();
	extern PyObject *GetPyFalse();

#	undef PyString_Check
#	define PyString_Check(op) ((op)->ob_type == GetPyString_Type())
#	undef PyTuple_Check
#	define PyTuple_Check(op) ((op)->ob_type == GetPyTuple_Type())
#	undef PyList_Check
#	define PyList_Check(op) ((op)->ob_type == GetPyList_Type())
#	undef PyInt_Check
#	define PyInt_Check(op) ((op)->ob_type == GetPyInt_Type())

#else /* (!WIN32 || qCvsDebug) */
	inline PyTypeObject *GetPyType_Type() { return &PyType_Type; }
	inline PyTypeObject *GetPyFile_Type() { return &PyFile_Type; }
	inline PyTypeObject *GetPyString_Type() { return &PyString_Type; }
	inline PyTypeObject *GetPyTuple_Type() { return &PyTuple_Type; }
	inline PyTypeObject *GetPyList_Type() { return &PyList_Type; }
	inline PyTypeObject *GetPyInt_Type() { return &PyInt_Type; }
	inline PyObject *GetPyNone() { return Py_None; }
	inline PyObject *GetPyTrue() { return Py_True; }
	inline PyObject *GetPyFalse() { return Py_False; }
#endif /* !WIN32 || qCvsDebug */

#endif /* PYTHONGLUE_IMPL_H */
