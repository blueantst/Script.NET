#include "StdAfx.h"

class PyObject;
#ifdef _DEBUG
extern "C"
{
	long _Py_RefTotal = 0;
	void _Py_Dealloc(PyObject *) {};
	void _Py_NegativeRefcount(const char *fname, int lineno, PyObject *op){};
}
#endif