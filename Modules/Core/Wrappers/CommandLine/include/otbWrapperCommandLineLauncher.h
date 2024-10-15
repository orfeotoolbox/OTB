/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperCommandLineLauncher_h
#define otbWrapperCommandLineLauncher_h


#include "otbWrapperApplication.h"
#include "otbWrapperParameter.h"
#include "itksys/SystemTools.hxx"
#include "otbWrapperCommandLineParser.h"

#include "itkStdStreamLogOutput.h"

#include "otbStandardOneLineFilterWatcher.h"

#include "itkCommand.h"

#include <vector>
#include <string>

namespace otb
{
namespace Wrapper
{

/** \class CommandLineLauncher
 *  \brief This class check the validity of a command line application.
 *
 * To be valid, the expression must be as follow:
 * ModuleName (ModulePath) --attribut1_Key attribut1_Value --attribut2_Key
 * attribut2_Value
 * After the attribute key, if the user give several values (expression
 * without \"--\" separated by space), it will automatically be
 * interpreted as a list.
 *
 * \ingroup OTBCommandLine
 */

class ITK_ABI_EXPORT CommandLineLauncher : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef CommandLineLauncher           Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(CommandLineLauncher, itk::Object);

  /** Parse result enum */
  typedef CommandLineParser::ParseResultType ParseResultType;
  typedef enum { OKPARAM, MISSINGMANDATORYPARAMETER, MISSINGPARAMETERVALUE, WRONGPARAMETERVALUE, INVALIDNUMBEROFVALUE, DEFAULT } ParamResultType;

  /** Filter watcher list type */
  typedef std::vector<StandardOneLineFilterWatcher<>*> WatcherListType;

  /** Command Member */
  typedef itk::MemberCommand<Self> AddProcessCommandType;

  /** Load the application in several steps :
   * - Load the paths
   * - Load the application using the ApplicationRegistry
   */
  bool Load();

  /** same as Load method but set the expression before. */
  bool Load(const std::vector<std::string>& vexp);

  /** Launch the process, using the Execute application method
   * The method will check if the user asked for help (looking at
   * --help key) before loading parameter and launching process.
   **/
  bool Execute();

  /** Launch the process, using the ExecuteAndWriteOutput application
   * method
   * The method will check if the user asked for help (looking at
   * --help key) before loading parameter and launching process.
   */
  bool ExecuteAndWriteOutput();

  /** Gather the code process to do before Execute and
   * ExecuteAndWriteOutput
   * Returns false if a problem occurs, true otherwise.
   */
  bool BeforeExecute();

  /** Create and display the help of the application */
  void DisplayHelp(bool longHelp = false);

  /** Performs specific action for testing environment */
  void LoadTestEnv();

protected:
  /** Constructor */
  CommandLineLauncher();

  /** Destructor */
  ~CommandLineLauncher() override;

  /** Load the executable path. It looks for the key --modulePath,
    * extract and interpret as path the following strings.
    */
  bool LoadPath();

  /** Load the application calling the CreateApplication method of the
    * ApplicationRegistry classes.
    * Pay attention, the executable paths have to be loaded or set in
    * the environment before calling the function.
    */
  bool LoadApplication();

  /** Parse the user expression, extract the key and the associated
    * string and set it as value of each corresponding application
    *  parameter.
    */
  CommandLineLauncher::ParamResultType LoadParameters();

  /** Create and display the help of the application */
  std::string DisplayParameterHelp(const Parameter::Pointer& param, const std::string paramKey, bool longHelp = false);

  /** Check if each key is unique in the expression. */
  bool CheckUnicity();

  /** Check if option contains at least one "-". */
  bool CheckParametersPrefix();

  /** Check if each key is valid for the application. */
  bool CheckKeyValidity(std::string& key);

  /** Display the output parameter (Role == RoleOutput) */
  void DisplayOutputParameters();

  /** Load the watchers for internal progress and writing progress report. */
  void LinkWatchers(itk::Object* caller, const itk::EventObject& event);

  /** Clear watcher list, deleting its pointers. */
  void DeleteWatcherList();

  /** Returns the width of the longest key (in number of chars) */
  unsigned int GetMaxKeySize() const;

private:
  /** \return false if paramKey is a missing mandatory parameter */
  bool CheckMissingMandatoryParameter(const std::string& paramKey) const;

  /** Prints a warning to std::cerr if paramKey is an unused parameter */
  void CheckUnusedParameter(const std::string& paramKey) const;

  /** \return false if paramKey is an OutputFilename parameter
  pointing to a path that does not exist */
  bool CheckOutputPathsValidity(const std::string& paramKey) const;

  CommandLineLauncher(const CommandLineLauncher&) = delete;
  void operator=(const CommandLineLauncher&) = delete;

  /**
   * Actually launch the process and write outputs, without catching exceptions.
   */
  bool ExecuteAndWriteOutputNoCatch();


  std::string m_Path;

  Application::Pointer m_Application;
  // std::string                       m_Expression;
  std::vector<std::string>   m_VExpression;
  CommandLineParser::Pointer m_Parser;

  WatcherListType m_WatcherList;

  itk::StdStreamLogOutput::Pointer m_LogOutput;

  AddProcessCommandType::Pointer m_AddProcessCommand;
  bool                           m_ReportProgress;

}; // end class

} // end namespace Wrapper
} // end namespace otb

#endif // otbWrapperCommandLineLauncher_h_
