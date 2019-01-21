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

#include "otbWrapperCommandLineLauncher.h"

// Single value parameter
#include "otbWrapperChoiceParameter.h"
#include "otbWrapperDirectoryParameter.h"
#include "otbWrapperInputFilenameParameter.h"
#include "otbWrapperOutputFilenameParameter.h"
#include "otbWrapperInputImageParameter.h"
#include "otbWrapperInputVectorDataParameter.h"
#include "otbWrapperOutputImageParameter.h"
#include "otbWrapperOutputVectorDataParameter.h"
#include "otbWrapperRadiusParameter.h"
#include "otbWrapperListViewParameter.h"
#include "otbWrapperRAMParameter.h"
#include "otbWrapperOutputProcessXMLParameter.h"
#include "otbWrapperAddProcessToWatchEvent.h"

// List value parameter
#include "otbWrapperInputImageListParameter.h"
#include "otbWrapperInputVectorDataListParameter.h"
#include "otbWrapperInputFilenameListParameter.h"
#include "otbWrapperStringListParameter.h"


#include "otbWrapperApplicationRegistry.h"
#include "otbWrapperTypes.h"
#include <itksys/RegularExpression.hxx>
#include <string>
#include <iostream>

using std::string;

namespace otb
{
namespace Wrapper
{

CommandLineLauncher::CommandLineLauncher() :
  /*m_Expression(""),*/m_VExpression(), m_WatcherList(), m_ReportProgress(true)
{
  m_Application = nullptr;
  m_Parser = CommandLineParser::New();
  m_LogOutput = itk::StdStreamLogOutput::New();
  m_LogOutput->SetStream(std::cout);

  // Add the callback to be added when a AddProcessToWatch event is invoked
  m_AddProcessCommand = AddProcessCommandType::New();
  m_AddProcessCommand->SetCallbackFunction(this, &CommandLineLauncher::LinkWatchers);
}

CommandLineLauncher::~CommandLineLauncher()
{
  this->DeleteWatcherList();
  m_Application = nullptr;
  ApplicationRegistry::CleanRegistry();
}

void CommandLineLauncher::DeleteWatcherList()
{
  for (unsigned int i = 0; i < m_WatcherList.size(); i++)
    {
    delete m_WatcherList[i];
    m_WatcherList[i] = nullptr;
    }
  m_WatcherList.clear();
}


bool CommandLineLauncher::Load(const std::vector<std::string> &vexp)
{
  m_VExpression = vexp;
  return this->Load();
}


bool CommandLineLauncher::Load()
{
  if (this->CheckParametersPrefix() == false)
    {
    std::cerr << "ERROR: Parameters are set using \"-\", not \"--\"." << std::endl;
    return false;
    }

  if (this->CheckUnicity() == false)
    {
    std::cerr << "ERROR: At least one parameter is not unique in the expression." << std::endl;
    return false;
    }

  if (this->LoadPath() == false)
    {
    if (m_Parser->GetPathsAsString(m_VExpression).size() != 0)
      {
      std::cerr << "ERROR: At least one specified path within \"" << m_Parser->GetPathsAsString(m_VExpression)
                << "\" is invalid or doesn't exist." << std::endl;
      return false;
      }
    }

  return this->LoadApplication();
}

bool CommandLineLauncher::Execute()
{
  if (this->BeforeExecute() == false)
    {
    return false;
    }

  if( m_Application->Execute() == 0 )
    {
    this->DisplayOutputParameters();
    return true;
    }
  else
    return false;
}

bool CommandLineLauncher::ExecuteAndWriteOutputNoCatch()
{
   if (this->BeforeExecute() == false)
      {
      return false;
      }
    if( m_Application->ExecuteAndWriteOutput() == 0 )
      {
      this->DisplayOutputParameters();
      }
    else
      {
      return false;
      }
    return true;
}

bool CommandLineLauncher::ExecuteAndWriteOutput()
{
  // If testenv is used, do not catch exceptions
  if (m_Parser->IsAttributExists("-testenv", m_VExpression) == true)
    {
    return ExecuteAndWriteOutputNoCatch();
    }
  else
    {
    try
      {
      return ExecuteAndWriteOutputNoCatch();
      }
    catch(otb::ApplicationException& err)
      {
      // These are thrown with otbAppLogFATAL, a macro which logs a user
      // friendly error message before throwing. So log exception details only
      // in debug.
      m_Application->GetLogger()->Debug("Caught otb::ApplicationException during application execution:\n");
      m_Application->GetLogger()->Debug(string(err.what()) + "\n");
      return false;
      }
    catch(otb::ImageFileReaderException& err)
      {
      m_Application->GetLogger()->Debug("Caught otb::ImageFileReaderException during application execution:\n");
      m_Application->GetLogger()->Debug(string(err.what()) + "\n");
      m_Application->GetLogger()->Fatal(string("Cannot open image ") + err.m_Filename + string(". ") + err.GetDescription() + string("\n"));
      return false;
      }
    catch(itk::ExceptionObject& err)
      {
      m_Application->GetLogger()->Debug("Caught itk::ExceptionObject during application execution:\n");
      m_Application->GetLogger()->Debug(string(err.what()) + "\n");
      m_Application->GetLogger()->Fatal(string(err.GetDescription()) + "\n");
      return false;
      }
    catch(std::exception& err)
      {
      m_Application->GetLogger()->Fatal(std::string("Caught std::exception during application execution: ") + err.what() + "\n");
      return false;
      }
    catch(...)
      {
      m_Application->GetLogger()->Fatal("Caught unknown exception during application execution.\n");
      return false;
      }
    }
}

bool CommandLineLauncher::BeforeExecute()
{
  if (m_Application.IsNull())
    {
    std::cerr << "ERROR: No loaded application." << std::endl;
    return false;
    }

  // Check if there's keys in the expression if the application takes
  // at least 1 mandatory parameter
  const std::vector<std::string> appKeyList = m_Application->GetParametersKeys(true);
  std::vector<std::string> keyList = m_Parser->GetKeyList( m_VExpression );

  if( appKeyList.size()!=0 && keyList.size()==0 )
    {
    std::cerr << "ERROR: Waiting for at least one parameter." << std::endl;
    std::cerr << std::endl;
    this->DisplayHelp();
    return false;
    }

  // if help is asked...
  if (m_Parser->IsAttributExists("-help", m_VExpression) == true)
    {
    std::vector<std::string> val;
    val = m_Parser->GetAttribut("-help", m_VExpression);

    if(val.empty())
      {      
      this->DisplayHelp(true);
      }
    else
      {
     
      
      for(std::vector<std::string>::const_iterator it = val.begin(); it!=val.end();++it)
        {
        Parameter::Pointer param = m_Application->GetParameterByKey(*it);
        if (param->GetRole() != Role_Output)
          {
          std::cerr << this->DisplayParameterHelp(param, *it,true)<<std::endl;
          }
        }
      }
    return false;
    }

  //display OTB version
  if (m_Parser->IsAttributExists("-version", m_VExpression) == true)
    {
    std::cerr << "This is the "<<m_Application->GetName() << " application, version " << OTB_VERSION_STRING <<std::endl;
    return false;
    }

  // if we want to load test environnement
  if (m_Parser->IsAttributExists("-testenv", m_VExpression) == true)
    {
    this->LoadTestEnv();
    }

  // Check the key validity (ie. exist in the application parameters)
  std::string unknownKey;
  if (this->CheckKeyValidity(unknownKey) == false)
    {
    std::cerr << "ERROR: Parameter -" << unknownKey <<" does not exist in the application." << std::endl;
    return false;
    }

  ParamResultType result = this->LoadParameters();

  if (result == MISSINGMANDATORYPARAMETER)
    {
    std::cerr << std::endl;
    this->DisplayHelp();
    return false;
    }
  else if (result != OKPARAM)
    {
    return false;
    }

  return true;
}

bool CommandLineLauncher::LoadPath()
{
  std::vector<std::string> pathList;
  // If users has set path...
  //if (m_Parser->GetPaths(pathList, m_Expression) == CommandLineParser::OK)
  if (m_Parser->GetPaths(pathList, m_VExpression) == CommandLineParser::OK)
    {
    for (unsigned i = 0; i < pathList.size(); i++)
      {
      ApplicationRegistry::AddApplicationPath(pathList[i]);
      }
    }
  else
    {
    return false;
    }

  return true;
}

bool CommandLineLauncher::LoadApplication()
{
  // Look for the module name
  std::string moduleName;
  if (m_Parser->GetModuleName(moduleName, m_VExpression) != CommandLineParser::OK)
    {
    std::cerr << "ERROR: Invalid module name: " << m_VExpression[0] << "." << std::endl;
    return false;
    }

  // Instantiate the application using the factory
  m_Application = ApplicationRegistry::CreateApplication(moduleName);

  if (m_Application.IsNull())
    {
    std::cerr << "ERROR: Could not find application \"" << moduleName << "\"" << std::endl;
    std::string modulePath = ApplicationRegistry::GetApplicationPath();
    std::cerr << "ERROR: Module search path: " << (modulePath.empty() ? "none (check OTB_APPLICATION_PATH)" : modulePath) << std::endl;

    std::vector<std::string> list = ApplicationRegistry::GetAvailableApplications();
    if (list.size() == 0)
      {
      std::cerr << "ERROR: Available modules: none." << std::endl;
      }
    else
      {
      std::cerr << "ERROR: Available modules:" << std::endl;
      for (std::vector<std::string>::const_iterator it = list.begin(); it != list.end(); ++it)
        {
        std::cerr << "\t" << *it << std::endl;
        }
      }
    return false;
    }
  else
    {
    // Attach log output to the Application logger
    m_Application->GetLogger()->AddLogOutput(m_LogOutput);

    // Add an observer to the AddedProcess event
    m_Application->AddObserver(AddProcessToWatchEvent(), m_AddProcessCommand.GetPointer());

    return true;
    }
}

CommandLineLauncher::ParamResultType CommandLineLauncher::LoadParameters()
{
  if (m_Application.IsNull())
    {
    itkExceptionMacro("No application loaded");
    }

  /* Check for inxml parameter. If exists Update all Parameters from xml and
   * check for user defined parameters for overriding those from XML
   */
  const char *inXMLKey =  "inxml";
  const char *attrib   = "-inxml";
  const bool paramInXMLExists(m_Parser->IsAttributExists(attrib, m_VExpression));
  if(paramInXMLExists)
    {
    std::vector<std::string> inXMLValues;
    inXMLValues = m_Parser->GetAttribut(attrib, m_VExpression);
    m_Application->SetParameterString(inXMLKey, inXMLValues[0]);
    m_Application->UpdateParameters();
    }

  // Check for the progress report parameter
  if (m_Parser->IsAttributExists("-progress", m_VExpression) == true)
    {
    std::vector<std::string> val = m_Parser->GetAttribut("-progress", m_VExpression);
    if (val.size() == 1 && (val[0] == "1" || val[0] == "true"))
      {
      m_ReportProgress = true;
      }
    else if (val.size() == 1 && (val[0] == "0" || val[0] == "false"))
      {
      m_ReportProgress = false;
      }
    else
      {
      std::cerr << "ERROR: Invalid value for parameter -progress. It must be 0, 1, false or true." << std::endl;
      return WRONGPARAMETERVALUE;
      }
    }

  const std::vector<std::string> appKeyList = m_Application->GetParametersKeys(true);
  // Loop over each parameter key declared in the application
  // FIRST PASS : set parameter values
  for (unsigned int i = 0; i < appKeyList.size(); i++)
    {
    const std::string paramKey(appKeyList[i]);
    std::vector<std::string> values;

    ParameterType type = m_Application->GetParameterType(paramKey);

    const bool paramExists(m_Parser->IsAttributExists(std::string("-").append(paramKey), m_VExpression));

    // if param is a Group, don't do anything, ParamGroup don't have values
    if (type != ParameterType_Group)
      {
      // Get the attribute relative to this key as vector
      values = m_Parser->GetAttribut(std::string("-").append(paramKey), m_VExpression);

      // If the param does not exists in the cli, don't try to set a
      // value on it, an exception will be thrown later in this function
      if (paramExists)
        {
        // Check if there is a value associated to the attribute
        if ( values.empty() )
          {
          std::cerr << "ERROR: No value associated to parameter -" << paramKey << "." << std::endl;
          return INVALIDNUMBEROFVALUE;
          }

        if (type == ParameterType_InputVectorDataList ||
            type == ParameterType_InputImageList ||
            type == ParameterType_InputFilenameList ||
            type == ParameterType_StringList ||
            type == ParameterType_ListView)
          {
          // Multiple values parameters
          m_Application->SetParameterStringList(paramKey, values);
          }
        else if (type == ParameterType_Choice ||
                 type == ParameterType_Float ||
                 type == ParameterType_Int ||
                 type == ParameterType_Radius ||
                 type == ParameterType_Directory ||
                 type == ParameterType_String ||
                 type == ParameterType_InputFilename ||
                 type == ParameterType_OutputFilename ||
                 type == ParameterType_ComplexInputImage ||
                 type == ParameterType_InputImage ||
                 type == ParameterType_OutputImage ||
                 type == ParameterType_ComplexOutputImage ||
                 type == ParameterType_InputVectorData ||
                 type == ParameterType_OutputVectorData ||
                 type == ParameterType_RAM ||
                 type == ParameterType_OutputProcessXML ||
                 type == ParameterType_Bool) // || type == ParameterType_InputProcessXML)
          {
          // Single value parameter
          m_Application->SetParameterString(paramKey, values[0]);

          if (type == ParameterType_OutputImage)
            {
            // Check if pixel type is given
            if (values.size() == 2)
              {
              ImagePixelType pixType = ImagePixelType_float;
              if ( !OutputImageParameter::ConvertStringToPixelType(values[1],pixType) )
                {
                std::cerr << "ERROR: Invalid output type for parameter -" <<
                  paramKey << ": " << values[1] << "." << std::endl;
                return WRONGPARAMETERVALUE;
                }
              m_Application->SetParameterOutputImagePixelType(paramKey, pixType);
              }
            else if (values.size() > 2)
              {
              std::cerr << "ERROR: Too many values for parameter -" <<
                paramKey << " (expected 2 or 1, got " << values.size() << ")."
                        << std::endl;
              return INVALIDNUMBEROFVALUE;
              }
            }
          else if (type == ParameterType_ComplexOutputImage)
            {
            // Check if pixel type is given
            if (values.size() == 2)
              {
              ComplexImagePixelType cpixType = ComplexImagePixelType_float;
              if ( !ComplexOutputImageParameter::ConvertStringToPixelType(values[1],cpixType) )
                {
                std::cerr << "ERROR: Invalid output type for parameter -" <<
                  paramKey << ": " << values[1] << "." << std::endl;
                return WRONGPARAMETERVALUE;
                }
              m_Application->SetParameterComplexOutputImagePixelType(paramKey, cpixType);
              }
            else if (values.size() > 2)
              {
              std::cerr << "ERROR: Too many values for parameter: -" << paramKey
                        << " (expected 2 or 1, got " << values.size() << ")." <<std::endl;
              return INVALIDNUMBEROFVALUE;
              }
            }
          }
        // Call the DoUpdateParameter to update dependent params
        m_Application->UpdateParameters();
        }
      }
    }

  // SECOND PASS : checks
  for (const auto & paramKey : appKeyList)
    {
    // Check for missing mandatory parameters
    if(!CheckMissingMandatoryParameter(paramKey))
      return MISSINGMANDATORYPARAMETER;

    // Check and warn unused parameters
    CheckUnusedParameter(paramKey);

    // Check output paths are valid
    if(!CheckOutputPathsValidity(paramKey))
      return WRONGPARAMETERVALUE;
    }

  return OKPARAM;
}

bool CommandLineLauncher::CheckOutputPathsValidity(const std::string & paramKey) const
{
  ParameterType type = m_Application->GetParameterType(paramKey);
  if (m_Application->HasValue(paramKey) &&
      type == ParameterType_OutputFilename)
    {
    std::string filename = m_Application->GetParameterString(paramKey);
    itksys::String path = itksys::SystemTools::GetFilenamePath(filename);
    if (path!="" && !itksys::SystemTools::FileIsDirectory(path))
      {
      std::cerr <<"ERROR: Directory doesn't exist : "<< path << std::endl;
      return false;
        }
      }
  return true;
}

bool CommandLineLauncher::CheckMissingMandatoryParameter(const std::string & paramKey) const
{
    if (m_Application->IsParameterMissing(paramKey))
      {
      std::cerr << "ERROR: Missing mandatory parameter -" << paramKey << "." << std::endl;
      return false;
      }
    return true;
}

void CommandLineLauncher::CheckUnusedParameter(const std::string & paramKey) const
{
  // Check for ignored parameters
    if(m_Application->HasUserValue(paramKey))
      {

      // Find the position of the next dot
      unsigned int start = 0;
      auto end = paramKey.find_first_of('.',start);

      // Until we reach en of key
      while(end != std::string::npos)
        {
        // Extract key until the current dot
        const std::string & key = paramKey.substr(0,end);

        // Find the corresponding parameter type
        ParameterType type = m_Application->GetParameterType(key);

        // In any case update the position of current and next dot
        start = end+1;
        end = paramKey.find_first_of('.',start);

        // If the current parameter (paramKey) has a choice in it
        if(type == ParameterType_Choice)
          {
          const std::string & value = m_Application->GetParameterString(key);

          // Check that this choice is active
          if(paramKey.find(value) == std::string::npos)
            {
            std::cerr<<"WARNING: Parameter -"<<paramKey<<" will be ignored because -"<<key<<" is "<<value<<"."<<std::endl;
            
            break;
            }
          }
        }
      }
}

void CommandLineLauncher::LinkWatchers(itk::Object * itkNotUsed(caller), const itk::EventObject & event)
{
  // Report the progress only if asked
  if (m_ReportProgress)
    {

    if (typeid(otb::Wrapper::AddProcessToWatchEvent) == typeid( event ))
      {
      const AddProcessToWatchEvent* eventToWatch = dynamic_cast<const AddProcessToWatchEvent*> (&event);

      StandardOneLineFilterWatcher * watch = new StandardOneLineFilterWatcher(eventToWatch->GetProcess(),
                                                                              eventToWatch->GetProcessDescription());
      m_WatcherList.push_back(watch);
      }
    }
}

void CommandLineLauncher::DisplayHelp(bool longHelp)
{
  std::cerr<<std::endl;
  std::cerr << "This is the "<<m_Application->GetDocName() << " ("<<m_Application->GetName()<<") application, version " << OTB_VERSION_STRING <<std::endl<<std::endl;

  std::cerr << m_Application->GetDescription() << std::endl;

  if(longHelp)
    { 
    std::cerr<<"Tags: ";

    std::vector<std::string> tags = m_Application->GetDocTags();
    
    for(std::vector<std::string>::const_iterator it = tags.begin(); it!=tags.end();++it)
      {
      std::cerr<<*it<<" ";
      }
    std::cerr<<std::endl;
  
    std::cerr<<std::endl;
    std::cerr<<m_Application->GetDocLongDescription() << std::endl;
    std::cerr<<std::endl;
    }
  else
    {
    std::string link = m_Application->GetDocLink();
    if (!link.empty())
      {
      std::cerr << "Complete documentation: " << link << " or -help" <<std::endl;
      std::cerr<<std::endl;
      }
    }

  std::cerr << "Parameters: " << std::endl;

  const std::vector<std::string> appKeyList = m_Application->GetParametersKeys(true);
  const unsigned int nbOfParam = appKeyList.size();

  unsigned int maxKeySize = GetMaxKeySize();

  //// progress report parameter
  std::string bigKey = "progress";
  for(unsigned int i=0; i<maxKeySize-std::string("progress").size(); i++)
    bigKey.append(" ");

  for (unsigned int i = 0; i < nbOfParam; i++)
    {
    Parameter::Pointer param = m_Application->GetParameterByKey(appKeyList[i]);
    if (param->GetRole() != Role_Output)
      {
      std::cerr << this->DisplayParameterHelp(param, appKeyList[i]);
      }
    }

  std::cerr << "        -"<<bigKey<<" <boolean>        Report progress " << std::endl;
  bigKey = "help";
  for(unsigned int i=0; i<maxKeySize-std::string("help").size(); i++)
    bigKey.append(" ");
  std::cerr << "        -"<<bigKey<<" <string list>    Display long help (empty list), or help for given parameters keys" << std::endl;

  std::cerr<<std::endl;
  //std::string cl(m_Application->GetCLExample());


  std::cerr<<"Use -help param1 [... paramN] to see detailed documentation of those parameters."<<std::endl;
  std::cerr<<std::endl;
  
  std::cerr << "Examples: " << std::endl;
  std::cerr << m_Application->GetCLExample() << std::endl;

  if(longHelp)
    {
    std::cerr<<"Authors: "<<std::endl<<m_Application->GetDocAuthors() << std::endl;

    std::cerr<<std::endl;
    std::cerr<<"Limitations: "<<std::endl<<m_Application->GetDocLimitations() << std::endl;

    std::cerr<<std::endl;
    std::cerr<<"See also: "<<std::endl<<m_Application->GetDocSeeAlso() << std::endl;
    std::cerr<<std::endl;

    }  

}

void CommandLineLauncher::LoadTestEnv()
{
  //Set seed for rand and itk mersenne twister
  //srand(1);
  // itk::Statistics::MersenneTwisterRandomVariateGenerator::GetInstance()->SetSeed(121212);
}


std::string CommandLineLauncher::DisplayParameterHelp(const Parameter::Pointer & param, const std::string paramKey, bool longHelp)
{
  // Display the type the type
  ParameterType type = m_Application->GetParameterType(paramKey);

  std::string bigKey = paramKey;
  
  unsigned int maxKeySize = GetMaxKeySize();

  bool singleSelectionForListView = false;

  if(type == ParameterType_ListView)
    {
    ListViewParameter * tmp = static_cast<ListViewParameter *>(param.GetPointer());
    singleSelectionForListView = tmp->GetSingleSelection();
    }

  std::ostringstream oss;

  if( m_Application->IsParameterMissing(paramKey) )
    {
    oss << "MISSING ";
    }
  else
    {
    oss << "        ";
    }
  
  for(unsigned int i=0; i<maxKeySize-paramKey.size(); i++)
    bigKey.append(" ");

  oss<< "-" << bigKey << " ";

  // Display the type the parameter
  if (type == ParameterType_Radius || type == ParameterType_Int || type == ParameterType_RAM)
    {
    oss << "<int32>         ";
    }
  else if (type == ParameterType_Bool)
    {
    oss << "<boolean>       ";
    }
  else if (type == ParameterType_Float)
    {
    oss << "<float>         ";
    }
  else if (type == ParameterType_InputFilename || type == ParameterType_OutputFilename ||type == ParameterType_Directory || type == ParameterType_InputImage || type == ParameterType_OutputProcessXML || type == ParameterType_InputProcessXML ||
           type == ParameterType_ComplexInputImage || type == ParameterType_InputVectorData || type == ParameterType_OutputVectorData ||
           type == ParameterType_String || type == ParameterType_Choice || (type == ParameterType_ListView && singleSelectionForListView))
    {
    oss << "<string>        ";
    }
  else if (type == ParameterType_OutputImage || type == ParameterType_ComplexOutputImage)
    {
    oss << "<string> [pixel]";
    }
  else if (type == ParameterType_Choice || (type == ParameterType_ListView && !singleSelectionForListView) || type == ParameterType_InputImageList ||
           type == ParameterType_InputVectorDataList || type == ParameterType_InputFilenameList ||
           type == ParameterType_StringList )
    {
    oss << "<string list>   ";
    }
  else if(type == ParameterType_Group)
    {
    oss<< "<group>         ";
    }
  else
    itkExceptionMacro("Not handled parameter type.");


  oss<< " " << m_Application->GetParameterName(paramKey) << " ";

  if (type == ParameterType_OutputImage)
    {
    OutputImageParameter* paramDown = dynamic_cast<OutputImageParameter*>(param.GetPointer());
    std::string defPixType("float");
    if (paramDown)
      {
      defPixType = OutputImageParameter::ConvertPixelTypeToString(paramDown->GetDefaultPixelType());
      }
    oss << " [pixel=uint8/uint16/int16/uint32/int32/float/double/cint16/cint32/cfloat/cdouble]";
    oss << " (default value is " << defPixType <<")";
    }

  if (type == ParameterType_ComplexOutputImage)
    {
    ComplexOutputImageParameter* paramDown = dynamic_cast<ComplexOutputImageParameter*>(param.GetPointer());
    std::string defPixType("cfloat");
    if (paramDown)
      {
      defPixType = ComplexOutputImageParameter::ConvertPixelTypeToString(paramDown->GetDefaultComplexPixelType());
      }
    oss << " [pixel=cfloat/cdouble]";
    oss << " (default value is "<< defPixType <<")";
    }


  if (type == ParameterType_Choice)
    {
    ChoiceParameter* paramDown = dynamic_cast<ChoiceParameter*>(param.GetPointer());
    if (paramDown)
      {
      std::vector<std::string> keys = paramDown->GetChoiceKeys();
      std::vector<std::string> names = paramDown->GetChoiceNames();

      oss << "[";
      for(unsigned int i=0; i<keys.size(); i++)
        {
        oss<<keys[i];
        if( i != keys.size()-1 )
          oss << "/";
        }

      oss << "]";
      }
    }

  if(type != ParameterType_Group)
    {

    if(m_Application->IsMandatory(paramKey))
      {
      oss<<" (mandatory";
      }
    else
      {
      oss<<" (optional";

      if(m_Application->IsParameterEnabled(paramKey))
        {
        oss<<", on by default";
        }
      else
        {
        oss<<", off by default";
        }
      }
    
    if(m_Application->HasValue(paramKey))
      {
      if ( m_Application->GetParameterAsString(paramKey).empty() )
        oss<<", no default value";
      else
        {
        oss<<", default value is "
           <<m_Application->GetParameterAsString(paramKey);
        }
      }
    oss<<")";
    }
  oss << std::endl;

  if(longHelp)
    {
    oss << "        ";
    for(unsigned int i=0; i<maxKeySize;++i)
      oss<<" ";
    oss<<"                   ";
    oss<<m_Application->GetParameterDescription(paramKey)<<std::endl;


    if (type == ParameterType_Choice)
      {
      std::vector<std::string> keys = dynamic_cast<ChoiceParameter*>(param.GetPointer())->GetChoiceKeys();
      std::vector<std::string> names = dynamic_cast<ChoiceParameter*>(param.GetPointer())->GetChoiceNames();
      
      auto keyIt = keys.begin();
      auto nameIt = names.begin();
      
      for( ; keyIt!=keys.end()&&nameIt!=names.end();++keyIt,++nameIt)
        {
        oss << "        ";
        for(unsigned int i=0; i<maxKeySize;++i)
          oss<<" ";
        oss<<"                   ";
        oss<<"- "<<*nameIt<<" ("<<*keyIt<<"): "<<m_Application->GetParameterDescription(paramKey+"."+(*keyIt))<<std::endl;
        }
      }
    }
  return oss.str();
}

bool CommandLineLauncher::CheckUnicity()
{
  bool res = true;
  // Extract expression keys
  //std::vector<std::string> keyList = m_Parser->GetKeyList(m_Expression);
  std::vector<std::string> keyList = m_Parser->GetKeyList(m_VExpression);

  // Check Unicity
  for (unsigned int i = 0; i < keyList.size(); i++)
    {
    std::vector<std::string> listTmp = keyList;
    const std::string keyRef = keyList[i];
    listTmp.erase(listTmp.begin() + i);
    for (unsigned int j = 0; j < listTmp.size(); j++)
      {
      if (keyRef == listTmp[j])
        {
        res = false;
        break;
        }
      }
    if (!res)
      break;
    }

  return res;
}

bool CommandLineLauncher::CheckParametersPrefix()
{
  // Check if the chain " --" appears in the args, could be a common mistake
  for (std::vector<std::string>::iterator it = m_VExpression.begin(); it != m_VExpression.end(); ++it)
    {
    if (it->compare(0, 2, "--") == 0 )
      {
      return false;
      }
    }
  return true;
}

bool CommandLineLauncher::CheckKeyValidity(std::string& refKey)
{
  bool res = true;
  // Extract expression keys
  std::vector<std::string> expKeyList = m_Parser->GetKeyList(m_VExpression);

  // Extract application keys
  std::vector<std::string> appKeyList = m_Application->GetParametersKeys(true);
  appKeyList.push_back("help");
  appKeyList.push_back("progress");
  appKeyList.push_back("testenv");
  appKeyList.push_back("version");

  // Check if each key in the expression exists in the application
  for (unsigned int i = 0; i < expKeyList.size(); i++)
    {
    refKey = expKeyList[i];
    bool keyExist = false;
    for (unsigned int j = 0; j < appKeyList.size(); j++)
      {
      if (refKey == appKeyList[j])
        {
        keyExist = true;
        break;
        }
      }
    if (keyExist == false)
      {
      res = false;
      break;
      }
    }

  return res;
}

void CommandLineLauncher::DisplayOutputParameters()
{
  std::vector< std::pair<std::string, std::string> > paramList;
  paramList = m_Application->GetOutputParametersSumUp();
  if( paramList.size() == 0 )
    return;

  std::ostringstream oss;
  for( unsigned int i=0; i<paramList.size(); i++)
    {
    oss << paramList[i].first;
    oss << ": ";
    oss << paramList[i].second;
    oss << std::endl;
    }


  if ( m_Parser->IsAttributExists("-testenv", m_VExpression) )
    {
    std::vector<std::string> val = m_Parser->GetAttribut("-testenv", m_VExpression);
    if( val.size() == 1 )
      {
      std::ofstream ofs(val[0]);
      if (!ofs.is_open())
        {
        fprintf(stderr, "Error, can't open file");
        itkExceptionMacro( << "Error, can't open file "<<val[0]<<".");
        }
      ofs << oss.str();
      ofs.close();
      }
    }

  std::cout << "Output parameters value:" << std::endl;
  std::cout << oss.str() << std::endl;
}

unsigned int CommandLineLauncher::GetMaxKeySize() const
{
  const std::vector<std::string> appKeyList = m_Application->GetParametersKeys(true);
  const unsigned int nbOfParam = appKeyList.size();

  unsigned int maxKeySize = std::string("progress").size();
  
  for (unsigned int i = 0; i < nbOfParam; i++)
    {
    if (m_Application->GetParameterRole(appKeyList[i]) != Role_Output)
      {
      if( maxKeySize < appKeyList[i].size() )
        maxKeySize = appKeyList[i].size();
      }
    }
  
  return maxKeySize;
}

}
}
