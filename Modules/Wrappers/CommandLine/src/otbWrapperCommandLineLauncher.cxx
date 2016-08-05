/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
#include "otbWrapperCommandLineLauncher.h"

// Single value parameter
#include "otbWrapperChoiceParameter.h"
#include "otbWrapperDirectoryParameter.h"
#include "otbWrapperEmptyParameter.h"
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

namespace otb
{
namespace Wrapper
{

CommandLineLauncher::CommandLineLauncher() :
  /*m_Expression(""),*/m_VExpression(), m_WatcherList(), m_ReportProgress(true), m_MaxKeySize(0)
{
  m_Application = ITK_NULLPTR;
  m_Parser = CommandLineParser::New();
  m_LogOutput = itk::StdStreamLogOutput::New();
  m_LogOutput->SetStream(std::cout);

  // Add the callback to be added when a AddProcessToWatch event is invoked
  m_AddProcessCommand = AddProcessCommandType::New();
  m_AddProcessCommand->SetCallbackFunction(this, &CommandLineLauncher::LinkWatchers);
}

CommandLineLauncher::CommandLineLauncher(const char *) /*:
  m_Expression(exp)*/
{
  m_Application = ITK_NULLPTR;
  m_Parser = CommandLineParser::New();
}

CommandLineLauncher::~CommandLineLauncher()
{
  this->DeleteWatcherList();
  m_Application = ITK_NULLPTR;
  ApplicationRegistry::CleanRegistry();
}

void CommandLineLauncher::DeleteWatcherList()
{
  for (unsigned int i = 0; i < m_WatcherList.size(); i++)
    {
    delete m_WatcherList[i];
    m_WatcherList[i] = ITK_NULLPTR;
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
  if (m_VExpression.empty())
    {
    itkExceptionMacro("No expression specified...");
    }

  if (this->CheckParametersPrefix() == false)
    {
    std::cerr << "ERROR: Parameter keys have to set using \"-\", not \"--\"" << std::endl;
    return false;
    }

  if (this->CheckUnicity() == false)
    {
    std::cerr << "ERROR: At least one key is not unique in the expression..." << std::endl;
    return false;
    }

  if (this->LoadPath() == false)
    {
    if (m_Parser->GetPathsAsString(m_VExpression).size() != 0)
      {
      std::cerr << "ERROR: At least one specified path within \"" << m_Parser->GetPathsAsString(m_VExpression)
                << "\" is invalid or doesn't exist..." << std::endl;
      return false;
      }
    }

  this->LoadApplication();

  return true;
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

bool CommandLineLauncher::ExecuteAndWriteOutput()
{
  try
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
      return false;
    }
  catch(std::exception& err)
    {
    std::ostringstream message;
    message << "The following error occurred during application execution : " << err.what() << std::endl;
    m_Application->GetLogger()->Write( itk::LoggerBase::FATAL, message.str() );
    return false;
    }
  catch(...)
    {
    m_Application->GetLogger()->Write( itk::LoggerBase::FATAL, "An unknown exception has been raised during application execution" );
    return false;
    }

  return true;
}

bool CommandLineLauncher::BeforeExecute()
{
  if (m_Application.IsNull())
    {
    std::cerr << "ERROR: No loaded application..." << std::endl;
    return false;
    }

  // Check if there's keys in the expression if the application takes
  // at least 1 mandatory parameter
  const std::vector<std::string> appKeyList = m_Application->GetParametersKeys(true);
  std::vector<std::string> keyList = m_Parser->GetKeyList( m_VExpression );

  if( appKeyList.size()!=0 && keyList.size()==0 )
    {
    std::cerr << "ERROR: Waiting for at least one parameter..." << std::endl;
    this->DisplayHelp();
    return false;
    }

  // if help is asked...
  if (m_Parser->IsAttributExists("-help", m_VExpression) == true)
    {
    this->DisplayHelp();
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
    std::cerr << "ERROR: option -"<<unknownKey<<" does not exist in the application." << std::endl;
    this->DisplayHelp();
    return false;
    }
  try
    {
    if (this->LoadParameters() != OKPARAM)
      {
      std::cerr << "ERROR: Troubles loading parameter, please check your line argument..." << std::endl;
      // Force to reload the application, the LoadParameters can change wrong values
      this->LoadApplication();
      m_Application->Init();
      this->DisplayHelp();

      return false;
      }
    }
  catch (itk::ExceptionObject& err)
    {
    std::cerr << "ERROR: Troubles in parameter setting, please check your line argument..." << std::endl;
    std::cerr << err.GetDescription() << std::endl;
    // Force to reload the application, the LoadParameters can change wrong values
    this->LoadApplication();
    m_Application->Init();
    this->DisplayHelp();

    return false;
    }

  // Check for the progress report
  if (m_Parser->IsAttributExists("-progress", m_VExpression) == true)
    {
    std::vector<std::string> val;
    val = m_Parser->GetAttribut("-progress", m_VExpression);
    if (val.size() != 1)
      {
      std::cerr << "ERROR: Invalid progress argument, must be unique value..." << std::endl;
      return false;
      }
    if (val[0] == "1" || val[0] == "true")
      {
      m_ReportProgress = true;
      }
    else
      if (val[0] == "0" || val[0] == "false")
        {
        m_ReportProgress = false;
        }
      else
        {
        std::cerr << "ERROR: Invalid progress argument, must be 0, 1, false or true..." << std::endl;
        // Force to reload the application, the LoadParameters can change wrong values
        this->LoadApplication();
        this->DisplayHelp();

        return false;
        }
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

void CommandLineLauncher::LoadApplication()
{
  // Look for the module name
  std::string moduleName;
  //if (m_Parser->GetModuleName(moduleName, m_Expression) != CommandLineParser::OK)
  if (m_Parser->GetModuleName(moduleName, m_VExpression) != CommandLineParser::OK)
    {
    std::cerr << "ERROR: LoadApplication, no module found..." << std::endl;
    return;
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
      std::cerr << "ERROR: Available modules : none." << std::endl;
      }
    else
      {
      std::cerr << "ERROR: Available modules :" << std::endl;
      for (std::vector<std::string>::const_iterator it = list.begin(); it != list.end(); ++it)
        {
        std::cerr << "\t" << *it << std::endl;
        }
      }
    }
  else
    {
    // Attach log output to the Application logger
    m_Application->GetLogger()->SetTimeStampFormat(itk::LoggerBase::HUMANREADABLE);
    m_Application->GetLogger()->AddLogOutput(m_LogOutput);

    // Add an observer to the AddedProcess event
    m_Application->AddObserver(AddProcessToWatchEvent(), m_AddProcessCommand.GetPointer());

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

  const std::vector<std::string> appKeyList = m_Application->GetParametersKeys(true);
  // Loop over each parameter key declared in the application
  // FIRST PASS : set parameter values
  for (unsigned int i = 0; i < appKeyList.size(); i++)
    {
    const std::string paramKey(appKeyList[i]);
    std::vector<std::string> values;

    Parameter::Pointer param = m_Application->GetParameterByKey(paramKey);
    ParameterType type = m_Application->GetParameterType(paramKey);

    const bool paramExists(m_Parser->IsAttributExists(std::string("-").append(paramKey), m_VExpression));

    // if param is a Group, dont do anything, ParamGroup dont have values
    if (type != ParameterType_Group)
      {
      // Get the attribute relative to this key as vector
      values = m_Parser->GetAttribut(std::string("-").append(paramKey), m_VExpression);

      // If the param does not exists in the cli, dont try to set a
      // value on it, an exception will be thrown later in this function
      if (paramExists)
        {
        // Check if there is a value associated to the attribute
        if ( values.empty() )
          {
          std::cerr << "ERROR: No value associated to the parameter : \"" << paramKey
                    << "\", invalid number of values " << values.size() << std::endl;
          return INVALIDNUMBEROFVALUE;
          }

        // Ensure that the parameter is enabled
        m_Application->EnableParameter(paramKey);

        if (type == ParameterType_InputVectorDataList)
          {
          dynamic_cast<InputVectorDataListParameter *> (param.GetPointer())->SetListFromFileName(values);
          }
        else
          if (type == ParameterType_InputImageList)
            {
            dynamic_cast<InputImageListParameter *> (param.GetPointer())->SetListFromFileName(values);
            }
          else
            if (type == ParameterType_InputFilenameList)
              {
              dynamic_cast<InputFilenameListParameter *> (param.GetPointer())->SetListFromFileName(values);
              }
            else
              if (type == ParameterType_StringList)
                {
                dynamic_cast<StringListParameter *> (param.GetPointer())->SetValue(values);
                }
              else
                if (type == ParameterType_String)
                  {
                  dynamic_cast<StringParameter *> (param.GetPointer())->SetValue(
                    m_Parser->GetAttributAsString(std::string("-").append(paramKey), m_VExpression) );
                  }
                else
                  if (type == ParameterType_OutputImage)
                    {
                    m_Application->SetParameterString(paramKey, values[0]);
                    // Check if pixel type is given
                    if (values.size() == 2)
                      {
                      ImagePixelType outPixType = ImagePixelType_float;
                      if (values[1] == "uint8")
                        outPixType = ImagePixelType_uint8;
                      else if (values[1] == "int16")
                        outPixType = ImagePixelType_int16;
                      else if (values[1] == "uint16")
                        outPixType = ImagePixelType_uint16;
                      else if (values[1] == "int32")
                        outPixType = ImagePixelType_int32;
                      else if (values[1] == "uint32")
                        outPixType = ImagePixelType_uint32;
                      else if (values[1] == "float")
                        outPixType = ImagePixelType_float;
                      else if (values[1] == "double")
                        outPixType = ImagePixelType_double;
                      else
                        {
                        return WRONGPARAMETERVALUE;
                        }
                      dynamic_cast<OutputImageParameter *> (param.GetPointer())->SetPixelType(outPixType);
                      }
                    else
                      if (values.size() != 1 && values.size() != 2)
                        {
                        std::cerr << "ERROR: Invalid number of value for: \"" << paramKey
                                  << "\", invalid number of values " << values.size() << std::endl;
                        return INVALIDNUMBEROFVALUE;
                        }
                    }
                  else if (type == ParameterType_ComplexOutputImage)
                    {
                    m_Application->SetParameterString(paramKey, values[0]);
                    // Check if pixel type is given
                    if (values.size() == 2)
                      {
                      ComplexImagePixelType outPixType = ComplexImagePixelType_float;
                      if (values[1] == "cfloat")
                        outPixType = ComplexImagePixelType_float;
                      else if (values[1] == "cdouble")
                        outPixType = ComplexImagePixelType_double;
                      else
                        {
                        return WRONGPARAMETERVALUE;
                        }
                      dynamic_cast<ComplexOutputImageParameter *> (param.GetPointer())->SetComplexPixelType(outPixType);
                      }
                    else
                      if (values.size() != 1 && values.size() != 2)
                        {
                        std::cerr << "ERROR: Invalid number of value for: \"" << paramKey
                                  << "\", invalid number of values " << values.size() << std::endl;
                        return INVALIDNUMBEROFVALUE;
                        }
                    }
                  else
                    if (type == ParameterType_ListView)
                      {
                      dynamic_cast<ListViewParameter *> (param.GetPointer())->SetSelectedNames(values);
                      }
                    else
                      if(values.size() != 1)
                        {
                        // Handle space in filename. Only for input
                        // files or directories
                        if (type == ParameterType_Directory         || type == ParameterType_InputFilename ||
                            type == ParameterType_ComplexInputImage ||
                            type == ParameterType_InputImage ||
                            type == ParameterType_InputVectorData   || type == ParameterType_OutputVectorData )
                          {
                          for(unsigned int j=1; j<values.size(); j++)
                            {
                            values[0].append(" ");
                            values[0].append(values[j]);
                            }
                          }
                        else if (!param->GetAutomaticValue())
                          {
                          std::cerr << "ERROR: Invalid number of value for: \"" << paramKey << "\", must have 1 value, not  "
                                    << values.size() << std::endl;
                          return INVALIDNUMBEROFVALUE;
                          }
                        }
        // Single value parameter
        if (type == ParameterType_Choice || type == ParameterType_Float || type == ParameterType_Int ||
            type == ParameterType_Radius || type == ParameterType_Directory || type == ParameterType_InputFilename ||
            type == ParameterType_OutputFilename ||
            type == ParameterType_ComplexInputImage || type == ParameterType_InputImage ||
            type == ParameterType_ComplexOutputImage ||
            type == ParameterType_InputVectorData ||
            type == ParameterType_OutputVectorData || type == ParameterType_RAM ||
            type == ParameterType_OutputProcessXML) // || type == ParameterType_InputProcessXML)
          {
          m_Application->SetParameterString(paramKey, values[0]);
          }
        else
          if (type == ParameterType_Empty)
            {
            if (values[0] == "1" || values[0] == "true")
              {
              dynamic_cast<EmptyParameter *> (param.GetPointer())->SetActive(true);
              }
            else
              if (values[0] == "0" || values[0] == "false")
                {
                dynamic_cast<EmptyParameter *> (param.GetPointer())->SetActive(false);
                }
             else
              {
              std::cerr << "ERROR: Wrong parameter value: " << paramKey << std::endl;
              return WRONGPARAMETERVALUE;
              }
            }
        // Update the flag UserValue
        param->SetUserValue(true);
        // Call the DoUpdateParameter to update dependant params
        m_Application->UpdateParameters();
        }
      }
    }

  // SECOND PASS : check mandatory parameters
  for (unsigned int i = 0; i < appKeyList.size(); i++)
    {
    const std::string paramKey(appKeyList[i]);
    Parameter::Pointer param = m_Application->GetParameterByKey(paramKey);
    ParameterType type = m_Application->GetParameterType(paramKey);
    const bool paramExists(m_Parser->IsAttributExists(std::string("-").append(paramKey), m_VExpression));
    std::vector<std::string> values;

    // When a parameter is mandatory :
    // it must be set if :
    //  - The param is root
    //  - The param is not root and belonging to a Mandatory Group
    //    wich is activated
    bool mustBeSet = false;
    const bool hasValue = m_Application->HasValue(paramKey);

    //skip if mandatory parameters are missing because we have it already in XML
    if(!paramInXMLExists)
      {
      if( param->GetMandatory() == true && param->GetRole() != Role_Output && type != ParameterType_Group)
        {
        // check if the parameter is linked to a root parameter with a chain of active parameters
        if( param->IsRoot() || param->GetRoot()->IsRoot())
          {
          // the parameter is a root or inside a group at root level
          mustBeSet = true;
          }
        else
          {
          Parameter* currentParam = param->GetRoot();
          while (!currentParam->IsRoot())
            {
            if (!currentParam->GetActive())
              {
              // the missing parameter is not on an active branch : we can ignore it
              break;
              }
            currentParam = currentParam->GetRoot();

            if (currentParam->IsRoot())
              {
              // the missing parameter is on an active branch : we need it
              mustBeSet = true;
              }
            }
          }
        }
      }

    if( mustBeSet )
      {
      if (!paramExists)
        {
        // If key doesn't exist and parameter hasn't default value set...
        if (!hasValue)
          {
          std::cerr << "ERROR: Missing mandatory parameter: " << paramKey << "  " << m_Application->HasValue(paramKey)
                    << std::endl;
          return MISSINGMANDATORYPARAMETER;
          }
        }
      else
        {
        values = m_Parser->GetAttribut(std::string("-").append(paramKey), m_VExpression);
        if (values.size() == 0 && !m_Application->HasValue(paramKey))
          {
          std::cerr << "ERROR: Missing mandatory parameter: " << paramKey << std::endl;
          return MISSINGPARAMETERVALUE;
          }
        }
      }
    // Check if non mandatory parameter have values
    else
      {
      if( paramExists )
        {
        values = m_Parser->GetAttribut(std::string("-").append(paramKey), m_VExpression);
        if (values.size() == 0)
          {
          std::cerr << "ERROR: Missing non-mandatory parameter: " << paramKey << std::endl;
          return MISSINGPARAMETERVALUE;
          }
        }
      }

    // Check output paths validity
    if (hasValue)
      {
      if (type == ParameterType_OutputFilename)
        {
        std::string filename = m_Application->GetParameterString(paramKey);
        itksys::String path = itksys::SystemTools::GetFilenamePath(filename);
        if (path!="" && !itksys::SystemTools::FileIsDirectory(path.c_str()))
          {
          std::cerr <<"ERROR: Directory doesn't exist : "<< path.c_str() << std::endl;
          return WRONGPARAMETERVALUE;
          }
        }
      }
    }

  return OKPARAM;
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

void CommandLineLauncher::DisplayHelp()
{
  std::cerr << "This is the "<<m_Application->GetName() << " application, version " << OTB_VERSION_STRING <<std::endl;

  std::cerr << m_Application->GetDescription() << std::endl;
  std::cerr<<std::endl;
  std::string link = "http://www.orfeo-toolbox.org/Applications/";
  link.append(m_Application->GetName());
  link.append(".html");

  std::cerr << "Complete documentation: " << link << std::endl;
  std::cerr<<std::endl;
  std::cerr << "Parameters: " << std::endl;

  const std::vector<std::string> appKeyList = m_Application->GetParametersKeys(true);
  const unsigned int nbOfParam = appKeyList.size();

  m_MaxKeySize = std::string("progress").size();
  for (unsigned int i = 0; i < nbOfParam; i++)
    {
    if (m_Application->GetParameterRole(appKeyList[i]) != Role_Output)
      {
      if( m_MaxKeySize < appKeyList[i].size() )
        m_MaxKeySize = appKeyList[i].size();
      }
    }

  //// progress report parameter
  std::string bigKey = "progress";
  for(unsigned int i=0; i<m_MaxKeySize-std::string("progress").size(); i++)
    bigKey.append(" ");

  std::cerr << "        -"<<bigKey<<" <boolean>        Report progress " << std::endl;

  for (unsigned int i = 0; i < nbOfParam; i++)
    {
      Parameter::Pointer param = m_Application->GetParameterByKey(appKeyList[i]);
      if (param->GetRole() != Role_Output)
        {
        std::cerr << this->DisplayParameterHelp(param, appKeyList[i]);
        }
    }

  std::cerr<<std::endl;
  //std::string cl(m_Application->GetCLExample());

  std::cerr << "Examples: " << std::endl;
  std::cerr << m_Application->GetCLExample() << std::endl;

}


void CommandLineLauncher::LoadTestEnv()
{
  //Set seed for rand and itk mersenne twister
  //srand(1);
 // itk::Statistics::MersenneTwisterRandomVariateGenerator::GetInstance()->SetSeed(121212);
}


std::string CommandLineLauncher::DisplayParameterHelp(const Parameter::Pointer & param, const std::string paramKey)
{
  // Display the type the type
  ParameterType type = m_Application->GetParameterType(paramKey);

  // Discard Group parameters (they don't need a value)
  if (type == ParameterType_Group)
    {
    return "";
    }

  std::ostringstream oss;

  // When a parameter is mandatory :
  // it must be set if :
  //  - The param is root
  //  - The param is not root and belonging to a Mandatory Group
  //    wich is activated
  bool isMissing = false;
  if (!m_Parser->IsAttributExists(std::string("-").append(paramKey), m_VExpression))
    {
    if (!m_Application->HasValue(paramKey))
      {
      if( param->GetMandatory() && param->GetRole() != Role_Output )
        {
        if( param->IsRoot() || param->GetRoot()->IsRoot())
          {
          // the parameter is a root or inside a group at root level
          isMissing = true;
          }
        else
          {
          Parameter* currentParam = param->GetRoot();
          while (!currentParam->IsRoot())
            {
            if (!currentParam->GetActive())
              {
              // the missing parameter is not on an active branch : we can ignore it
              break;
              }
            currentParam = currentParam->GetRoot();

            if (currentParam->IsRoot())
              {
              // the missing parameter is on an active branch : we need it
              isMissing = true;
              }
            }
          }
        }
      }
    }

  if( isMissing )
    {
    oss << "MISSING ";
    }
  else
    {
    oss << "        ";
    }

  std::string bigKey = paramKey;
  for(unsigned int i=0; i<m_MaxKeySize-paramKey.size(); i++)
    bigKey.append(" ");

  oss<< "-" << bigKey << " ";

  // Display the type the parameter
  if (type == ParameterType_Radius || type == ParameterType_Int || type == ParameterType_RAM)
    {
    oss << "<int32>         ";
    }
  else if (type == ParameterType_Empty )
    {
    oss << "<boolean>       ";
    }
  else if (type == ParameterType_Float)
    {
    oss << "<float>         ";
    }
  else if (type == ParameterType_InputFilename || type == ParameterType_OutputFilename ||type == ParameterType_Directory || type == ParameterType_InputImage || type == ParameterType_OutputProcessXML || type == ParameterType_InputProcessXML ||
           type == ParameterType_ComplexInputImage || type == ParameterType_InputVectorData || type == ParameterType_OutputVectorData ||
           type == ParameterType_String || type == ParameterType_Choice )
    {
    oss << "<string>        ";
    }
  else if (type == ParameterType_OutputImage || type == ParameterType_ComplexOutputImage)
    {
    oss << "<string> [pixel]";
    }
  else if (type == ParameterType_Choice || type == ParameterType_ListView || type == ParameterType_InputImageList ||
           type == ParameterType_InputVectorDataList || type == ParameterType_InputFilenameList ||
           type == ParameterType_StringList )
    {
    oss << "<string list>   ";
    }
  else
    itkExceptionMacro("Not handled parameter type.");


  oss<< " " << param->GetName() << " ";

  if (type == ParameterType_OutputImage)
    {
    OutputImageParameter* paramDown = dynamic_cast<OutputImageParameter*>(param.GetPointer());
    std::string defPixType("float");
    if (paramDown)
      {
      defPixType = OutputImageParameter::ConvertPixelTypeToString(paramDown->GetDefaultPixelType());
      }
    oss << " [pixel=uint8/uint16/int16/uint32/int32/float/double]";
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
    std::vector<std::string> keys = dynamic_cast<ChoiceParameter*>(param.GetPointer())->GetChoiceKeys();
    std::vector<std::string> names = dynamic_cast<ChoiceParameter*>(param.GetPointer())->GetChoiceNames();

    oss << "[";
    for(unsigned int i=0; i<keys.size(); i++)
      {
      oss<<keys[i];
      if( i != keys.size()-1 )
        oss << "/";
      }

    oss << "]";
    }

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
    oss<<", default value is "<<m_Application->GetParameterAsString(paramKey);
    }
  oss<<")";

  oss << std::endl;
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
      std::ofstream ofs(val[0].c_str());
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

}
}
