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
#include "otbWrapperFilenameParameter.h"
#include "otbWrapperComplexInputImageParameter.h"
#include "otbWrapperInputImageParameter.h"
#include "otbWrapperInputVectorDataParameter.h"
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperOutputImageParameter.h"
#include "otbWrapperOutputVectorDataParameter.h"
#include "otbWrapperRadiusParameter.h"
#include "otbWrapperStringParameter.h"
#include "otbWrapperListViewParameter.h"
#include "otbWrapperAddProcessToWatchEvent.h"

// List value parameter
#include "otbWrapperInputImageListParameter.h"
#include "otbWrapperInputVectorDataListParameter.h"
#include "otbWrapperStringListParameter.h"

#include "otbStreamingImageFileWriter.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

#include "otbWrapperApplicationRegistry.h"
#include "otbWrapperApplication.h"
#include "otbWrapperTypes.h"
#include <itksys/RegularExpression.hxx>
#include <string>
#include <iostream>

namespace otb
{
namespace Wrapper
{

CommandLineLauncher::CommandLineLauncher() :
  m_Expression(""), m_WatcherList(), m_ReportProgress(true)
{
  m_Application = NULL;
  m_Parser = CommandLineParser::New();
  m_LogOutput = itk::StdStreamLogOutput::New();
  m_LogOutput->SetStream(std::cout);

  // Add the callback to be added when a AddProcessToWatch event is invoked
  m_AddProcessCommand = AddProcessCommandType::New();
  m_AddProcessCommand->SetCallbackFunction(this, &CommandLineLauncher::LinkWatchers);
}

CommandLineLauncher::CommandLineLauncher(const char * exp) :
  m_Expression(exp)
{
  m_Application = NULL;
  m_Parser = CommandLineParser::New();
}

CommandLineLauncher::~CommandLineLauncher()
{
  this->DeleteWatcherList();
}

void CommandLineLauncher::DeleteWatcherList()
{
  for (unsigned int i = 0; i < m_WatcherList.size(); i++)
    {
    delete m_WatcherList[i];
    m_WatcherList[i] = NULL;
    }
  m_WatcherList.clear();
}

bool CommandLineLauncher::Load(const std::string & exp)
{
  m_Expression = exp;

  return this->Load();
}

bool CommandLineLauncher::Load()
{
  if (m_Expression == "")
    {
    itkExceptionMacro("No expression specified...");
    }

  if (this->CheckParametersPrefix() == true)
    {
    std::cerr << "ERROR: Parameter keys have to set using \"--\", not \"-\"" << std::endl;
    return true;
    }

  if (this->CheckUnicity() == true)
    {
    std::cerr << "ERROR: At least one key is not unique in the expression..." << std::endl;
    return true;
    }

  if (this->LoadPath() == true)
    {
    if (m_Parser->GetPathsAsString(m_Expression).size() != 0)
      {
      std::cerr << "ERROR: At least one specified path within \"" << m_Parser->GetPathsAsString(m_Expression)
          << "\" is invalid or doesn't exist..." << std::endl;
      }
    else
      {
      std::cerr << "ERROR: Trouble loading path, please check your command line..." << std::endl;
      }
    return true;
    }

  this->LoadApplication();

  return false;
}

bool CommandLineLauncher::Execute()
{
  if (this->BeforeExecute() == true)
    {
    return true;
    }

  int res = m_Application->Execute();
  if (res == 0)
    return false;
  else return true;
}

bool CommandLineLauncher::ExecuteAndWriteOutput()
{
  if (this->BeforeExecute() == true)
    {
    return true;
    }

  int res = m_Application->ExecuteAndWriteOutput();

  if (res == 0)
    return false;
  else return true;
}

bool CommandLineLauncher::BeforeExecute()
{
  if (m_Application.IsNull())
    {
    std::cerr << "ERROR: No loaded application..." << std::endl;
    return true;
    }

  // if help is asked...
  if (m_Parser->IsAttributExists("--help", m_Expression) == true)
    {
    this->DisplayHelp();
    return true;
    }

  // if we want to load test environnement
   if (m_Parser->IsAttributExists("--testenv", m_Expression) == true)
     {
     this->LoadTestEnv();
     }


  // Check the key validity (ie. exist in the application parameters)
  if (this->CheckKeyValidity() == true)
    {
    std::cerr << "ERROR: At least one key is not known by the application..." << std::endl;
    this->DisplayHelp();
    return true;
    }

  try
    {
    if (this->LoadParameters() != OKPARAM)
      {
      std::cerr << "ERROR: Troubles loading parameter, please check your line argument..." << std::endl;
      // Force to reload the application, the LoadParameters can change wrong values
      this->LoadApplication();
      this->DisplayHelp();

      return true;
      }
    }
  catch (itk::ExceptionObject& err)
    {
    std::cerr << "ERROR: Troubles in parameter setting, please check your line argument..." << std::endl;
    std::cerr << err.GetDescription() << std::endl;
    // Force to reload the application, the LoadParameters can change wrong values
    this->LoadApplication();
    this->DisplayHelp();

    return true;
    }

  m_Application->UpdateParameters();

  // Check for the progress report
  if (m_Parser->IsAttributExists("--progress", m_Expression) == true)
    {
    std::vector<std::string> val;
    val = m_Parser->GetAttribut("--progress", m_Expression);
    if (val.size() != 1)
      {
      std::cerr << "ERROR: Invalid progress argument, must be unique value..." << std::endl;
      return true;
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
        return true;
        }
    }

  return false;
}

bool CommandLineLauncher::LoadPath()
{
  std::vector<std::string> pathList;
  // If users has set path...
  if (m_Parser->GetPaths(pathList, m_Expression) == CommandLineParser::OK)
    {
    for (unsigned i = 0; i < pathList.size(); i++)
      {
      ApplicationRegistry::AddApplicationPath(pathList[i]);
      }
    }
  else
    {
    return true;
    }

  return false;
}

void CommandLineLauncher::LoadApplication()
{
  // Look for the module name
  std::string moduleName;
  if (m_Parser->GetModuleName(moduleName, m_Expression) != CommandLineParser::OK)
    {
    std::cerr << "ERROR: LoadApplication, no module found..." << std::endl;
    return;
    }

  // Instantiate the application using the factory
  m_Application = ApplicationRegistry::CreateApplication(moduleName);

  if (m_Application.IsNull())
    {
    std::cerr << "ERROR: Could not find application \"" << moduleName << "\"" << std::endl;
    std::cerr << "ERROR: Module search path: " << itksys::SystemTools::GetEnv("ITK_AUTOLOAD_PATH") << std::endl;

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
      return;
      }
    }

  // Attach log output to the Application logger
  m_Application->GetLogger()->SetTimeStampFormat(itk::LoggerBase::HUMANREADABLE);
  m_Application->GetLogger()->AddLogOutput(m_LogOutput);

  // Add an observer to the AddedProcess event
  m_Application->AddObserver(AddProcessToWatchEvent(), m_AddProcessCommand.GetPointer());
}

CommandLineLauncher::ParamResultType CommandLineLauncher::LoadParameters()
{
  if (m_Application.IsNull())
    {
    itkExceptionMacro("No application loaded");
    }

  const std::vector<std::string> appKeyList = m_Application->GetParametersKeys(true);

  // Loop over each parameter key declared in the application
  for (unsigned int i = 0; i < appKeyList.size(); i++)
    {
    const std::string paramKey(appKeyList[i]);
    std::vector<std::string> values;
    Parameter::Pointer param = m_Application->GetParameterByKey(paramKey);
    ParameterType type = m_Application->GetParameterType(paramKey);

    const bool paramExists(m_Parser->IsAttributExists(std::string("--").append(paramKey), m_Expression));
    const bool hasValue = m_Application->HasValue(paramKey);
    // Check if mandatory parameter are present and have value
    if (param->GetMandatory() == true && param->GetRoot()->GetActive())
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

      if (paramExists)
        {
        values = m_Parser->GetAttribut(std::string("--").append(paramKey), m_Expression);
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
      if (paramExists)
        {
        values = m_Parser->GetAttribut(std::string("--").append(paramKey), m_Expression);
        if (values.size() == 0)
          {
          std::cerr << "ERROR: Missing mandatory parameter: " << paramKey << std::endl;
          return MISSINGPARAMETERVALUE;
          }
        }
      }

    // If the param is optional and hasn't been set : don't do anything
    // If the param is mandatory but a default value exists : don't do anything
    if (paramExists)
      {
      // List values parameter case
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
          if (type == ParameterType_StringList)
            {
            dynamic_cast<StringListParameter *> (param.GetPointer())->SetValue(values);
            }
          else
            if (type == ParameterType_OutputImage)
              {
              m_Application->SetParameterString(paramKey, values[0]);
              // Check if pixel type is given
              if (values.size() == 2)
                {
                ImagePixelType outPixType = ImagePixelType_float;
                if (values[1] == "int8")
                  outPixType = ImagePixelType_int8;
                else
                  if (values[1] == "uint8")
                    outPixType = ImagePixelType_uint8;
                  else
                    if (values[1] == "int16")
                      outPixType = ImagePixelType_int16;
                    else
                      if (values[1] == "uint16")
                        outPixType = ImagePixelType_uint16;
                      else
                        if (values[1] == "int32")
                          outPixType = ImagePixelType_int32;
                        else
                          if (values[1] == "uint32")
                            outPixType = ImagePixelType_uint32;
                          else
                            if (values[1] == "float")
                              outPixType = ImagePixelType_float;
                            else
                              if (values[1] == "double")
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
                  std::cerr << "ERROR: Invalid number of value for: \"" << paramKey << "\", invalid number of values "
                      << values.size() << std::endl;
                  return INVALIDNUMBEROFVALUE;
                  }
              }
            else
              if (type == ParameterType_ListView)
                {
                dynamic_cast<ListViewParameter *> (param.GetPointer())->SetSelectedNames(values);
                }
              else
                if (values.size() != 1)
                  {
                  std::cerr << "ERROR: Invalid number of value for: \"" << paramKey << "\", must have 1 value, not  "
                      << values.size() << std::endl;
                  // Try to find a "-" instead of "--"...
                  itk::OStringStream oss;
                  for (unsigned int i = 0; i < values.size(); i++)
                    {
                    if (values[i][0] == '-')
                      {
                      oss << std::string(values[i]).substr(1, std::string(values[i]).size() - 1) << ", ";
                      }
                    }
                  if (oss.str().size() > 0) std::cerr << "ERROR: If values \""
                      << oss.str().substr(0, oss.str().size() - 2)
                      << "\" is/are keys, it should be prefix by \"--\"..." << std::endl;

                  return INVALIDNUMBEROFVALUE;
                  }

      // Single value parameter
      if (type == ParameterType_Choice || type == ParameterType_Float || type == ParameterType_Int || type
          == ParameterType_Radius || type == ParameterType_Directory || type == ParameterType_String || type
          == ParameterType_Filename || type == ParameterType_ComplexInputImage || type == ParameterType_InputImage ||
          type == ParameterType_InputVectorData || type == ParameterType_InputVectorDataList || type
          == ParameterType_OutputVectorData)
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
      }
    // Update the flag UserValue
    param->SetUserValue(true);
    }

  return OKPARAM;
}

void CommandLineLauncher::LinkWatchers(itk::Object * caller, const itk::EventObject & event)
{
  // Report the progress only if asked
  if (m_ReportProgress)
    {

    if (typeid(otb::Wrapper::AddProcessToWatchEvent) == typeid( event ))
      {
      const AddProcessToWatchEvent* eventToWacth = dynamic_cast<const AddProcessToWatchEvent*> (&event);

      StandardOneLineFilterWatcher * watch = new StandardOneLineFilterWatcher(eventToWacth->GetProcess(),
                                                                              eventToWacth->GetProcessDescription());
      m_WatcherList.push_back(watch);
      }
    }
}

void CommandLineLauncher::DisplayHelp()
{
  std::cerr << std::endl;
  std::cerr << "====================== HELP CONTEXT ======================" << std::endl;
  std::cerr << "NAME: " << m_Application->GetName() << std::endl;
  std::cerr << "DESCRIPTION: " << m_Application->GetDescription() << std::endl;
  std::cerr << "PARAMETERS: " << std::endl;

  std::cerr << "====== Mandatory parameters: ======" << std::endl;

  const std::vector<std::string> appKeyList = m_Application->GetParametersKeys(true);
  const unsigned int nbOfParam = appKeyList.size();

  // Mandatory parameters
  for (unsigned int i = 0; i < nbOfParam; i++)
    {
    Parameter::Pointer param = m_Application->GetParameterByKey(appKeyList[i]);
    // Check if mandatory parameter are present and have value
    if (param->GetMandatory() == true)
      {
      std::cerr << this->DisplayParameterHelp(param, appKeyList[i]);
      }
    }

  // Optional parameters
  std::cerr << "====== Optional parameters: ======" << std::endl;
  //// Module path parameter
  std::cerr << m_Parser->GetPathsAsString(m_Expression) << " (Executables paths)" << std::endl;
  std::cerr << "\t   Description: Paths to the executable library." << std::endl;
  std::cerr << "\t          Type: List of string (list of path)" << std::endl;
  if (m_Parser->GetPathsAsString(m_Expression).size() != 0)
    {
    const std::string envVal = itksys::SystemTools::GetEnv("ITK_AUTOLOAD_PATH");
    if (envVal.size() != 0)
      std::cerr << "\t       Status: ENVIRONEMENT PATH : " << envVal << std::endl;
    else std::cerr << "\t       Status: NO VALUE " << std::endl;
    }
  else
    if (m_Path == "")
      {
      std::cerr << "\t       Status: NO VALUE " << std::endl;
      }
    else std::cerr << "\t       Status: USER VALUE: " << m_Path << std::endl;

  //// progress report parameter
  std::cerr << "--progress (Report progress)" << std::endl;
  std::cerr << "\t   Description: Do report progress." << std::endl;
  std::cerr << "\t          Type: Boolean" << std::endl;
  std::cerr << "\t Default value: 1" << std::endl;
  if (!m_Parser->IsAttributExists("--progress", m_Expression))
    std::cerr << "\t        Status: DEFAULT VALUE" << std::endl;
  else
    if (m_Parser->GetAttribut("--progress", m_Expression).size() == 0)
      std::cerr << "\t       Status: none" << m_Path << std::endl;
    else std::cerr << "\t       Status: USER VALUE: " << m_Parser->GetAttribut("--progress", m_Expression)[0]
        << std::endl;

  for (unsigned int i = 0; i < nbOfParam; i++)
    {
    Parameter::Pointer param = m_Application->GetParameterByKey(appKeyList[i]);
    // Check if mandatory parameter are present and have value
    if (param->GetMandatory() != true)
      {
      std::cerr << this->DisplayParameterHelp(param, appKeyList[i]);
      }
    }
}


void CommandLineLauncher::LoadTestEnv()
{
  //Set seed for rand and itk mersenne twister
  srand(1);
  itk::Statistics::MersenneTwisterRandomVariateGenerator::GetInstance()->SetSeed(1234);
}


std::string CommandLineLauncher::DisplayParameterHelp(const Parameter::Pointer & param, const std::string paramKey)
{
  // Display the type the type
  ParameterType type = m_Application->GetParameterType(paramKey);
  if (type == ParameterType_Group)
    {
    return "";
    }

  std::ostringstream oss;
  oss << "--" << paramKey << " (" << param->GetName() << ")" << std::endl;

  // Display parameter description
  if (std::string(param->GetDescription()).size() != 0)
    {
    oss << "\t   Description: " << param->GetDescription() << std::endl;
    }
  else
    {
    oss << "\t   Description: none" << std::endl;
    }

  // Display the type the parameter
  if (type == ParameterType_Radius)
    {
    oss << "\t          Type: Int" << std::endl;
    }
  else
    if (type == ParameterType_Empty)
      {
      oss << "\t          Type: Boolean" << std::endl;
      }
    else
      if (type == ParameterType_Int)
        {
        oss << "\t          Type: Int" << std::endl;
        }
      else
        if (type == ParameterType_Float)
          {
          oss << "\t          Type: Float" << std::endl;
          }
        else
          if (type == ParameterType_Filename)
            {
            oss << "\t          Type: String (file name)" << std::endl;
            }
          else
            if (type == ParameterType_Directory)
              {
              oss << "\t          Type: String (Directory path)" << std::endl;
              }
            else
              if (type == ParameterType_InputImage || type == ParameterType_ComplexInputImage)
                {
                oss << "\t          Type: String (input image file name)" << std::endl;
                }
              else
                if (type == ParameterType_InputVectorData)
                  {
                  oss << "\t          Type: String (input vector data file name)" << std::endl;
                  }
                else
                  if (type == ParameterType_OutputImage)
                    {
                    oss << "\t          Type: String (output image file name and optionally its pixel type)"
                        << std::endl;
                            oss
                            << "\t                Possible pixel type: int8, uint8, int16, uint16, int32, uint32, flooat or double"
                        << std::endl;
                    }
                  else
                    if (type == ParameterType_OutputVectorData)
                      {
                      oss << "\t          Type: String (output vector data file name)" << std::endl;
                      }
                    else
                      if (type == ParameterType_String)
                        {
                        oss << "\t          Type: String" << std::endl;
                        }
                      else
                        if (type == ParameterType_Choice)
                          {
                          oss << "\t          Type: String: ";

                          std::vector<std::string> keys = m_Application->GetChoiceKeys(paramKey);
                          std::vector<std::string> names = m_Application->GetChoiceNames(paramKey);
                          for (unsigned int j = 0; j < keys.size(); j++)
                            {
                            oss << keys[j] << " for " << names[j];
                            if (j <= keys.size() - 1)
                              {
                              oss << ", ";
                              }
                            }
                          oss << std::endl;
                          }
                        else
                          if (type == ParameterType_ListView)
                            {
                            // We don't have access to the possible values...
                            oss << "\t          Type: List of String" << std::endl;
                            }
                          else
                            {
                            oss << "\t          Type: Type not handle yet" << std::endl;
                            }

  // Display parameter values
  if (m_Application->HasValue(paramKey))
    {
    // In the case choice, don't show the enum type.
    if (type == ParameterType_Choice)
      {
      oss << "\t Default value: " << m_Application->GetChoiceKeys(paramKey)[m_Application->GetParameterInt(paramKey)]
          << std::endl;
      }
    else
      if (type == ParameterType_OutputImage)
        {
        oss << "\t Default value: filename: " << m_Application->GetParameterAsString(paramKey) << std::endl;
        oss << "\t                pixel type: ";
        ImagePixelType outPixType = m_Application->GetParameterOutputImagePixelType(paramKey);
        if (outPixType == ImagePixelType_int8)
          oss << "int8";
        else
          if (outPixType == ImagePixelType_uint8)
            oss << "uint8";
          else
            if (outPixType == ImagePixelType_int16)
              oss << "int16";
            else
              if (outPixType == ImagePixelType_uint16)
                oss << "uint16";
              else
                if (outPixType == ImagePixelType_int32)
                  oss << "int32";
                else
                  if (outPixType == ImagePixelType_uint32)
                    oss << "uint32";
                  else
                    if (outPixType == ImagePixelType_float)
                      oss << "float";
                    else
                      if (outPixType == ImagePixelType_double)
                        oss << "double";
                      else
                        {
                        itkExceptionMacro("Unknown output pixel type.");
                        }
        }
      else
        {
        oss << "\t Default value: " << m_Application->GetParameterAsString(paramKey) << std::endl;
        }
    }
  else
    {
    if (type == ParameterType_OutputImage)
      {
      oss << "\t Default value: filename: none" << std::endl;
      oss << "\t                pixel type: ";
      ImagePixelType outPixType = m_Application->GetParameterOutputImagePixelType(paramKey);
      if (outPixType == ImagePixelType_int8)
        oss << "int8" << std::endl;
      else
        if (outPixType == ImagePixelType_uint8)
          oss << "uint8" << std::endl;
        else
          if (outPixType == ImagePixelType_int16)
            oss << "int16" << std::endl;
          else
            if (outPixType == ImagePixelType_uint16)
              oss << "uint16" << std::endl;
            else
              if (outPixType == ImagePixelType_int32)
                oss << "int32" << std::endl;
              else
                if (outPixType == ImagePixelType_uint32)
                  oss << "uint32" << std::endl;
                else
                  if (outPixType == ImagePixelType_float)
                    oss << "float" << std::endl;
                  else
                    if (outPixType == ImagePixelType_double)
                      oss << "double" << std::endl;
                    else
                      {
                      itkExceptionMacro("Unknown output pixel type.");
                      }
      }
    else oss << "\t Default value: none" << std::endl;
    }

  if (!m_Parser->IsAttributExists(std::string("--").append(paramKey), m_Expression))
    {
    if (!m_Application->HasValue(paramKey))
      oss << "\t        Status: MISSING" << std::endl;
    else oss << "\t        Status: DEFAULT VALUE" << std::endl;
    }
  else
    if (m_Parser->GetAttribut(std::string("--").append(paramKey), m_Expression).size() == 0)
      {
      oss << "\t        Status: NO VALUE ASSOCIATED" << std::endl;
      }
    else
      {
      if (type == ParameterType_OutputImage)
        {
        std::vector<std::string> values = m_Parser->GetAttribut(std::string("--").append(paramKey), m_Expression);
        oss << "\t        Status: filename : USER VALUE (" << values[0] << ")" << std::endl;
        if (values.size() == 2)
          oss << "\t        Status: pixel type : USER VALUE (" << values[1] << ")" << std::endl;
        else oss << "\t        Status: pixel type : DEFAULT VALUE" << std::endl;
        }
      else
        {
        oss << "\t        Status: USER VALUE (";
        oss << m_Parser->GetAttributAsString(std::string("--").append(paramKey), m_Expression) << ")";
        oss << std::endl;
        }
      }

  return oss.str();
}

bool CommandLineLauncher::CheckUnicity()
{
  bool res = false;
  // Extract expression keys
  std::vector<std::string> keyList = m_Parser->GetKeyList(m_Expression);

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
        res = true;
        break;
        }
      }
    if (res == true) break;
    }

  return res;
}

bool CommandLineLauncher::CheckParametersPrefix()
{
  bool res = false;
  // Extract Expression elements
  std::vector<itksys::String> spaceSplittedExp = itksys::SystemTools::SplitString(m_Expression.c_str(), ' ', false);
  // if the chain is "  module", SplitString will return: [ ], [module]
  for (unsigned int i = 0; i < spaceSplittedExp.size(); i++)
    {
    if (spaceSplittedExp[i] == " ")
      {
      spaceSplittedExp.erase(spaceSplittedExp.begin() + i);
      i--;
      }
    }

  // If the expression contains parameters
  if (spaceSplittedExp.size() > 2)
    {
    // Check if the chain "--" appears at least one time when "-" is present
    if (m_Expression.find("--") == std::string::npos && m_Expression.find("-") != std::string::npos)
      {
      res = true;
      }
    }

  return res;
}

bool CommandLineLauncher::CheckKeyValidity()
{
  bool res = false;
  // Extract expression keys
  std::vector<std::string> expKeyList = m_Parser->GetKeyList(m_Expression);

  // Extract application keys
  std::vector<std::string> appKeyList = m_Application->GetParametersKeys(true);
  appKeyList.push_back("help");
  appKeyList.push_back("progress");
  appKeyList.push_back("testenv");
  
  // Check if each key in the expression exists in the application
  for (unsigned int i = 0; i < expKeyList.size(); i++)
    {
    const std::string refKey = expKeyList[i];
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
      res = true;
      break;
      }
    }

  return res;
}

}
}
