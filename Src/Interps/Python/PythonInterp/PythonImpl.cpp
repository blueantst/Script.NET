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

#include "PythonImpl.h"
#include "CTclInterp.h"


PY_IMPL_CLASS(TclInterp, "Class for warp tcl interp")

PY_BEGIN_IMPL_METHOD(TclInterp)
	PY_IMPL_METHOD(TclInterp, Execute, "Execute tcl command")
	PY_IMPL_METHOD(TclInterp, RunScriptFile, "Run tcl script file")
PY_END_IMPL_METHOD

// constructor
PyTclInterp::PyTclInterp(CString strInterpName)
{
	m_nInterpID = OpenTclInterp(strInterpName);
	m_pTclInterp = GetTclInterp(m_nInterpID);
}

PyTclInterp::~PyTclInterp()
{
	CloseTclInterp(m_nInterpID);
}

PyObject *PyTclInterp::Create(PyObject *selfptr, PyObject *args)
{
	char *str;	// 传入的解释器名

	if (!PyArg_ParseTuple(args, "s:CTclInterp", &str))
		return NULL;

	PyAutoObject<PyTclInterp::obj_t> self = PyTclInterp::New();
	if (!self.IsValid())
		return NULL;

	TRY
	{
		self->m_object = new PyTclInterp(str);
	}
	CATCH_ALL(e)
	{
		PyHandlePlatError(e);
		return NULL;
	}
	END_CATCH_ALL

	return self.CopyReturn();
}

// 初始化字典内容
void PyTclInterp::InitValues(PyObject *d)
{
}

// 获取属性
PyObject *PyTclInterp::GetAttr(char *name)
{
	if(strcmp(name, "ObjectPtr") == 0)		// 获取PyTclInterp对象指针
		return PyInt_FromLong((long)this);
	else if(strcmp(name, "result") == 0)	// 获取脚本执行结果
		return PyString_FromString(m_pTclInterp->GetResult());
	else if(strcmp(name, "errorline") == 0)	// 获取错误行号
		return PyInt_FromLong(m_pTclInterp->GetErrorLine());
	else
		//edit by lcmao do not find  GetTclVar change to GetVar
		//return PyString_FromString(m_pTclInterp->GetTclVar(name));
		return PyString_FromString(m_pTclInterp->GetVar(name));

	return NULL;
}

// 执行Tcl脚本命令
PyObject *PyTclInterp::Execute(PyObject *args)
{
	PyTclInterp* pPyTclInterp = NULL;
	char *str;
	if(PyArg_ParseTuple(args, "is", &pPyTclInterp, &str))
	{
		if(m_pTclInterp)
		{
			return Py_BuildValue("i", m_pTclInterp->RunScriptCommand(str));
		}
		return Void();
	}

	return NULL;
}

// 执行Tcl脚本文件
PyObject *PyTclInterp::RunScriptFile(PyObject *args)
{
	PyTclInterp* pPyTclInterp = NULL;
	char *str;
	if(PyArg_ParseTuple(args, "is", &pPyTclInterp, &str))
	{
		if(m_pTclInterp)
		{
			return Py_BuildValue("i", m_pTclInterp->RunScriptFile(str));
		}
		return Void();
	}

	return NULL;
}

