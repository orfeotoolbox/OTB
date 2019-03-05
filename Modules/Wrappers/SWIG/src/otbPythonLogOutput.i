/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#if SWIGPYTHON

%module(directors="1") cb
%{
   #include "otbPythonLogOutput.h"
   #include "otbLogOutputCallback.h"
%}

%feature("director") LogOutputCallback;

%include "otbLogOutputCallback.h"

class itkLogOutput : public itkObject
{
protected:
  itkLogOutput();
  ~itkLogOutput();
};

/** This Callback class can be called from C++ to print, flush and to determine
 * if stdout is redirected */
%pythoncode
  {
  class PythonLogOutputCallback(LogOutputCallback):
      def __init__(self):
          super(PythonLogOutputCallback, self).__init__()
      def Call(self, content):
          sys.stdout.write(content)
      def Flush(self):
          sys.stdout.flush()
      def Isatty(self):
          return sys.stdout.isatty()
  }

class PythonLogOutput : public itkLogOutput
{
public:
  static PythonLogOutput_Pointer New();
  virtual void Delete();
  void SetCallback(otb::LogOutputCallback* callback);
  virtual void Write(std::string const & content);

protected:
  PythonLogOutput();
};

DECLARE_REF_COUNT_CLASS( PythonLogOutput )

class Logger
{
public:
  virtual void AddLogOutput(itkLogOutput *output);
  static Logger * Instance();
  void ResetOutputs();
protected:
  Logger();
  virtual ~Logger();

};

class ProgressReporterManager: public itkObject
{
public:
  /** Default constructor */
  
  static ProgressReporterManager_Pointer New();
  virtual void Delete();
  void SetLogOutputCallback(otb::LogOutputCallback* callback);
  itkCommand* GetAddProcessCommand();
  
protected:
  ProgressReporterManager();
};

DECLARE_REF_COUNT_CLASS( ProgressReporterManager )

/** Create the required objects for logging. Logger.Instance() is reset in 
 * order to replace the itkStdStreamLogOutput by a PythonLogOutput */
%pythoncode {
  _libraryLogOutput = PythonLogOutput_New()
  _libraryLogCallback = PythonLogOutputCallback()
  _libraryProgressReportManager = ProgressReporterManager_New()
  
  Logger.Instance().ResetOutputs()
  _libraryLogOutput.SetCallback(_libraryLogCallback)
  Logger.Instance().AddLogOutput(_libraryLogOutput.GetPointer())
  
  _libraryProgressReportManager.SetLogOutputCallback(_libraryLogCallback)

}

#endif
