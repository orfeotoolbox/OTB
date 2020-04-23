/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "itkPyCommand.h"

namespace itk
{

PyCommand::PyCommand()
{
  this->obj = nullptr;
}

PyCommand::~PyCommand()
{
  if (this->obj)
  {
    Py_DECREF(this->obj);
  }
  this->obj = nullptr;
}

void PyCommand::SetCommandCallable(PyObject* theObj)
{
  if (theObj != this->obj)
  {
    if (this->obj)
    {
      // get rid of our reference
      Py_DECREF(this->obj);
    }

    // store the new object
    this->obj = theObj;

    if (this->obj)
    {
      // take out reference (so that the calling code doesn't
      // have to keep a binding to the callable around)
      Py_INCREF(this->obj);
    }
  }
}

PyObject* PyCommand::GetCommandCallable()
{
  return this->obj;
}

void PyCommand::Execute(Object*, const EventObject&)
{
  this->PyExecute();
}


void PyCommand::Execute(const Object*, const EventObject&)
{
  this->PyExecute();
}

void PyCommand::PyExecute()
{
  // make sure that the CommandCallable is in fact callable
  if (!PyCallable_Check(this->obj))
  {
    // we throw a standard ITK exception: this makes it possible for
    // our standard CableSwig exception handling logic to take this
    // through to the invoking Python process
    itkExceptionMacro(<< "CommandCallable is not a callable Python object, "
                      << "or it has not been set.");
  }
  else
  {
    PyObject* result;

    result = PyEval_CallObject(this->obj, (PyObject*)nullptr);

    if (result)
    {
      Py_DECREF(result);
    }
    else
    {
      // there was a Python error.  Clear the error by printing to stdout
      PyErr_Print();
      // make sure the invoking Python code knows there was a problem
      // by raising an exception
      itkExceptionMacro(<< "There was an error executing the "
                        << "CommandCallable.");
    }
  }
}


} // namespace itk
