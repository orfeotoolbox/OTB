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
#include "otbWrapperInputComplexImageParameter.h"
#include "otbWrapperInputImageParameter.h"
#include "otbWrapperInputVectorDataParameter.h"
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperOutputImageParameter.h"
#include "otbWrapperOutputVectorDataParameter.h"
#include "otbWrapperRadiusParameter.h"
#include "otbWrapperStringParameter.h"
#include "otbWrapperListViewParameter.h"


// List value parameter
#include "otbWrapperInputImageListParameter.h"
#include "otbWrapperStringListParameter.h"

#include "otbStreamingImageFileWriter.h"

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

CommandLineLauncher::CommandLineLauncher() : m_Expression(""), m_WatcherList()
{
  m_Application = NULL;
  m_Parser = CommandLineParser::New();
  m_LogOutput = itk::StdStreamLogOutput::New();
  m_LogOutput->SetStream( std::cout );
}

CommandLineLauncher::CommandLineLauncher(const char * exp) : m_Expression(exp)
{
  m_Application = NULL;
  m_Parser = CommandLineParser::New();
}

CommandLineLauncher::~CommandLineLauncher()
{
  this->DeleteWatcherList();
}

void
CommandLineLauncher::DeleteWatcherList()
{
  for( unsigned int i= 0; i<m_WatcherList.size(); i++ )
    {
      delete m_WatcherList[i];
      m_WatcherList[i] = NULL;
    }
  m_WatcherList.clear();
}


bool
CommandLineLauncher::Load( const std::string & exp )
{
  m_Expression = exp;

  return this->Load();
}


bool
CommandLineLauncher::Load()
{
  if (m_Expression == "")
    {
      itkExceptionMacro("No expression specified...");
    }

  if( this->CheckUnicity() == false )
    {
      std::cerr<<"ERROR: At least one key is not unique in the expression..."<<std::endl;
      return false;
    }

  this->LoadPath();
  this->LoadApplication();

  return true;
}

bool
CommandLineLauncher::Execute()
{
  if( this->BeforeExecute() == false )
    {
      return false;
    }
  
  m_Application->Execute();
  return true;
 
}


bool
CommandLineLauncher::ExecuteAndWriteOutput()
{
  if( this->BeforeExecute() == false )
    {
      return false;
    }

  m_Application->ExecuteAndWriteOutput();
  return true;
 
}


bool
CommandLineLauncher::BeforeExecute()
{
    if( m_Application.IsNull() )
    {
      itkExceptionMacro("No application loaded");
    }

    // if help is asked...
    if ( m_Parser->IsAttributExists( "--help", m_Expression ) == true )
      {
        this->DisplayHelp();
        return false;
      }

    // Check the key validity (ie. exist in the application parameters)
    if ( this->CheckKeyValidity() == false )
      {
        std::cerr<<"ERROR: At least one key is not known by the application..."<<std::endl;
        this->DisplayHelp();
        return false;
      }

    try
      {
        if ( this->LoadParameters() != OKPARAM )
          {
            std::cerr<<"ERROR: troubles loading parameter, please check your line argument..."<<std::endl;
            // Force to reload the application, the LoadParameters can change wrong values
            this->LoadApplication();
            this->DisplayHelp();
            
            return false;
          }
      }
    catch(itk::ExceptionObject& err)
      {
        std::cerr<<"ERROR: troubles in parameter setting, please check your line argument..."<<std::endl;
        std::cerr<<err.GetDescription()<<std::endl;
        // Force to reload the application, the LoadParameters can change wrong values
        this->LoadApplication();
        this->DisplayHelp();
        
        return false;
      }

    m_Application->UpdateParameters();

    // Check for the progress report
    bool doProgressReport = true;
    if( m_Parser->IsAttributExists( "--progress", m_Expression ) == true )
      {
        std::vector<std::string> val;
        val = m_Parser->GetAttribut( "--progress", m_Expression );
        if( val.size() != 1)
          {
            std::cerr<<"Invalid progress argument, must be unique value..."<<std::endl;
            return false;
          }
          if( val[0] == "1" || val[0] == "true")
            {
              doProgressReport = true;
            }
          else if( val[0] == "0" || val[0] == "false")
            {
               doProgressReport = false;
            }
          else
            {
              std::cerr<<"Invalid progress argument, must be 0, 1, false or true..."<<std::endl;
              // Force to reload the application, the LoadParameters can change wrong values
              this->LoadApplication();
              this->DisplayHelp();
              return false;
            }
      }
    
    if( doProgressReport == true )
      {
        this->LinkWatchers();
      }

    return true;
}

void
CommandLineLauncher::LoadPath()
{
  std::vector<std::string> pathList;
  // If users has set path...
  if( m_Parser->GetPaths( pathList, m_Expression ) == CommandLineParser::OK )
    {
      // Contain paths into a string, separating each path with ":"
      m_Path = std::string("");
      for( unsigned i=0; i<pathList.size(); i++)
        {
          m_Path.append(pathList[i]);
          m_Path.append(":");
        }

      std::string specificEnv("ITK_AUTOLOAD_PATH=");
      specificEnv.append(m_Path);
      // do NOT use putenv() directly, since the string memory must be managed carefully
      itksys::SystemTools::PutEnv(specificEnv.c_str());
      // Reload factories to take into account new path
      itk::ObjectFactoryBase::ReHash();
    }
}


void
CommandLineLauncher::LoadApplication()
{
  // Look for the module name
  std::string moduleName;
  if( m_Parser->GetModuleName( moduleName, m_Expression ) != CommandLineParser::OK )
    {
      std::cerr << "ERROR: LoadApplication, no module found..." <<std::endl;
      return;
    }

  // Instanciate the application usingt the factory
  m_Application = ApplicationRegistry::CreateApplication(moduleName);
  
  if (m_Application.IsNull())
    {
      std::cerr << "ERROR: Could not find application \"" << moduleName <<"\""<< std::endl;
      return;
    }

  // Attach log output to the Application logger
  m_Application->GetLogger()->SetTimeStampFormat(itk::LoggerBase::HUMANREADABLE);
  m_Application->GetLogger()->AddLogOutput(m_LogOutput);
}

CommandLineLauncher::ParamResultType
CommandLineLauncher::LoadParameters()
{
  if( m_Application.IsNull() )
    {
      itkExceptionMacro("No application loaded");
    }
  
  const std::vector<std::string> appKeyList = m_Application->GetParametersKeys( true );

  // Loop over each parameter key declared in the application
  for( unsigned int i=0; i<appKeyList.size(); i++ )
    {
      const std::string paramKey(appKeyList[i]);
      std::vector<std::string> values;
      Parameter::Pointer param =  m_Application->GetParameterByKey( paramKey );
      ParameterType type = m_Application->GetParameterType( paramKey );
      
      const bool paramExists( m_Parser->IsAttributExists( std::string("--").append(paramKey), m_Expression )  );
      const bool hasValue = m_Application->HasValue( paramKey );
      // Check if mandatory parameter are present and have value
      if( param->GetMandatory() == true )
        {
          if( !paramExists )
             {
               // If key doesn't exist and parameter hasn't default value set...
               if( !hasValue )
                 {
                   std::cout<<"MISSINGMANDATORYPARAMETER: "<<paramKey<<"  "<<m_Application->HasValue( paramKey )<<std::endl;
                   return MISSINGMANDATORYPARAMETER;
                 }
             }

          if( paramExists )
            {
              values = m_Parser->GetAttribut( std::string("--").append(paramKey), m_Expression);
              if(  values.size() == 0 && !m_Application->HasValue( paramKey ) )
                {
                  std::cout<<"MISSINGPARAMETERVALUE: "<<paramKey<<std::endl;
                  return MISSINGPARAMETERVALUE;
                }
            }
        }
      // Check if non mandatory parameter have values
      else
        {
          if( paramExists )
            {
              values = m_Parser->GetAttribut( std::string("--").append(paramKey), m_Expression);
              if(  values.size() == 0 )
                {
   std::cout<<"MISSINGPARAMETERVALUE: "<<paramKey<<std::endl;
                  return MISSINGPARAMETERVALUE;
                }
            }
        }

      // If the param is optionnal and hasn't been set : don't do anything
      // If the param is mandatory but a default value exists : don't do anything
      if( paramExists )
        {
          // List values parameter case
          if( type == ParameterType_InputImageList )
            {
              dynamic_cast<InputImageListParameter *>(param.GetPointer())->SetListFromFileName( values );
            }
          else if( type == ParameterType_StringList )
            {
              dynamic_cast<StringListParameter *>(param.GetPointer())->SetValue( values );
            }
          else if( type == ParameterType_OutputImage )
            {
              m_Application->SetParameterString( paramKey, values[0] );
              // Check if pixel type is given
              if( values.size() == 2  )
                {
                  ImagePixelType outPixType = ImagePixelType_float;
                  if( values[1] == "int8" )
                    outPixType = ImagePixelType_int8;
                  else if( values[1] == "uint8" )
                    outPixType = ImagePixelType_uint8;
                  else if( values[1] == "int16" )
                    outPixType = ImagePixelType_int16;
                  else if( values[1] == "uint16" )
                    outPixType = ImagePixelType_uint16;
                  else if( values[1] == "int32" )
                    outPixType = ImagePixelType_int32;
                  else if( values[1] == "uint32" )
                    outPixType = ImagePixelType_uint32;
                  else if( values[1] == "float" )
                    outPixType = ImagePixelType_float;
                  else if( values[1] == "double" )
                    outPixType = ImagePixelType_double;
                  else 
                    {
                      return WRONGPARAMETERVALUE;
                    }
                 dynamic_cast<OutputImageParameter *>(param.GetPointer())->SetPixelType( outPixType );
                }
              else if( values.size() != 1 && values.size() != 2 )
                {
                  std::cout<<"INVALIDNUMBEROFVALUE2: "<<paramKey<<" "<<values.size()<<std::endl;
                  return INVALIDNUMBEROFVALUE;
                }
            }
          else if( type == ParameterType_ListView )
            {
              dynamic_cast<ListViewParameter *>(param.GetPointer())->SetSelectedNames( values );
            }
          else  if( values.size() != 1)
            {
              std::cout<<"INVALIDNUMBEROFVALUE: "<<paramKey<<" "<<values.size()<<std::endl;
              return INVALIDNUMBEROFVALUE;
            }
          
          // Single value parameter
          if( type == ParameterType_Choice || type == ParameterType_Float || type == ParameterType_Int || type == ParameterType_Radius
              || type == ParameterType_Directory || type == ParameterType_String || type == ParameterType_Filename || type == ParameterType_InputComplexImage
              || type == ParameterType_InputImage || type == ParameterType_InputVectorData || type == ParameterType_OutputVectorData )
            {
              m_Application->SetParameterString( paramKey, values[0] );
            }
          else if( type == ParameterType_Empty )
            {
              if( values[0] == "1" || values[0] == "true")
                {
                  dynamic_cast<EmptyParameter *>(param.GetPointer())->SetActive(true);
                }
              else if( values[0] == "0" || values[0] == "false")
                {
                  dynamic_cast<EmptyParameter *>(param.GetPointer())->SetActive(false);
                }
              else
                {
                  std::cout<<"WRONGPARAMETERVALUE: "<<paramKey<<std::endl;
                  return WRONGPARAMETERVALUE;
                }
            }
        }
    }
  
   return OKPARAM;
}


void
CommandLineLauncher::LinkWatchers()
{
  this->DeleteWatcherList();
  // Link internall filters watcher
  for( unsigned int i=0; i<m_Application->GetInternalProcessList().size(); i++ )
    {
      StandardOneLineFilterWatcher * watch = new StandardOneLineFilterWatcher(m_Application->GetInternalProcessList()[i], m_Application->GetInternalProcessListName()[i]);
      m_WatcherList.push_back( watch );
    }
  
  // Link output image writers watchers
  std::vector<std::string> paramList = m_Application->GetParametersKeys(true);
  std::vector<std::string>::const_iterator it = paramList.begin();
  for (; it != paramList.end(); ++it)
    {
      if (m_Application->GetParameterType(*it) == ParameterType_OutputImage)
        {
          Parameter* param = m_Application->GetParameterByKey(*it);
          OutputImageParameter* outputParam = dynamic_cast<OutputImageParameter*> (param);
          itk::OStringStream oss;
          oss << "Writing " << param->GetName() << "...";
          
          StandardOneLineFilterWatcher * watch = new StandardOneLineFilterWatcher(outputParam->GetWriter(), oss.str());
          m_WatcherList.push_back(watch);
        }
    }
}

void
CommandLineLauncher::DisplayHelp()
{
  std::cerr<<std::endl;
  std::cerr<<"====================== HELP CONTEXT ======================"<<std::endl;
  std::cerr<<"NAME: "<<m_Application->GetName()<<std::endl;
  std::cerr<<"DESCRIPTION: "<<m_Application->GetDescription()<<std::endl;
  std::cerr<<"PARAMETERS: "<<std::endl;

  std::cerr<<"====== Mandatory parameters: ======"<<std::endl;


  const std::vector<std::string> appKeyList = m_Application->GetParametersKeys( true );
  const unsigned int nbOfParam = appKeyList.size();

  // Mandatory parameters
  for( unsigned int i=0; i<nbOfParam; i++ )
    {
      Parameter::Pointer param =  m_Application->GetParameterByKey( appKeyList[i] );
      // Check if mandatory parameter are present and have value
      if( param->GetMandatory() == true )
        {
          std::cerr<< this->DisplayParameterHelp( param, appKeyList[i] );
        }
    }
 
  // Optional parameters
  std::cerr<<"====== Optional parameters: ======"<<std::endl;
  //// Module path parameter
  std::cerr<<m_Parser->GetModulePathKey()<<" (Executables paths)"<<std::endl;
  std::cerr<<"\t   Description: Paths to the executable library."<<std::endl;
  std::cerr<<"\t          Type: List of string (list of path)"<<std::endl;
  if( !m_Parser->IsAttributExists( m_Parser->GetModulePathKey(), m_Expression ) )
    {
      const std::string envVal = itksys::SystemTools::GetEnv("ITK_AUTOLOAD_PATH");
      if( envVal.size() != 0)
        std::cerr<< "\t       Status: ENVIRONEMENT PATH : "<<envVal<<std::endl;
      else
        std::cerr<< "\t       Status: NO VALUE "<<std::endl;
    }
  else if( m_Path == "")
    {
        std::cerr<< "\t       Status: NO VALUE "<<std::endl;
    }
  else
    std::cerr<< "\t       Status: USER VALUE: "<<m_Path<<std::endl;

  //// progress report parameter
  std::cerr<<"--progress (Report progress)"<<std::endl;
  std::cerr<<"\t   Description: Do report progress."<<std::endl;
  std::cerr<<"\t          Type: Boolean"<<std::endl;
  std::cerr<<"\t Default value: 1"<< std::endl;
  if( !m_Parser->IsAttributExists( "--progress", m_Expression ) )
    std::cerr<<"\t        Status: DEFAULT VALUE"<<std::endl;
  else if( m_Parser->GetAttribut( "--progress", m_Expression ).size() == 0 )
    std::cerr<< "\t       Status: none"<<m_Path<<std::endl;
  else
    std::cerr<< "\t       Status: USER VALUE: "<<m_Parser->GetAttribut( "--progress", m_Expression )[0]<<std::endl;

  for( unsigned int i=0; i<nbOfParam; i++ )
    {
      Parameter::Pointer param =  m_Application->GetParameterByKey(  appKeyList[i] );
      // Check if mandatory parameter are present and have value
      if( param->GetMandatory() != true )
        {
          std::cerr << this->DisplayParameterHelp( param, appKeyList[i] );
        }
 

    }

}

std::string
CommandLineLauncher::DisplayParameterHelp( const Parameter::Pointer & param, const std::string paramKey )
{
  // Display the type the type
  ParameterType type = m_Application->GetParameterType( paramKey );
  if( type == ParameterType_Group )
    {
      return "";
    }

  
  itk::OStringStream oss;
  oss<<"--"<<paramKey<<" ("<<param->GetName()<<")"<< std::endl;

   // Display parameter description
  if( std::string(param->GetDescription()).size() != 0 )
    {
      oss<<"\t   Description: "<<param->GetDescription()<< std::endl;
    }
  else
    {
      oss<<"\t   Description: none"<< std::endl;
    }
 
 // Display the type the parameter
  if( type == ParameterType_Radius )
    {
      oss<<"\t          Type: Int"<<std::endl;
    }
  else if( type == ParameterType_Empty )
    {
      oss<<"\t          Type: Boolean"<<std::endl;
    }
  else if( type == ParameterType_Int )
    {
      oss<<"\t          Type: Int"<<std::endl;
    }
  else if( type == ParameterType_Float )
    {
      oss<<"\t          Type: Float"<<std::endl;
    }
  else if( type == ParameterType_Filename )
    {
      oss<<"\t          Type: String (file name)"<<std::endl;
    }
  else if( type == ParameterType_Directory )
    {
      oss<<"\t          Type: String (Directory path)"<<std::endl;
    }
  else if( type == ParameterType_InputImage || type == ParameterType_InputComplexImage )
    {
      oss<<"\t          Type: String (input image file name)"<<std::endl;
    }
  else if( type == ParameterType_InputVectorData )
    {
      oss<<"\t          Type: String (input vector data file name)"<<std::endl;
    }
  else if( type == ParameterType_OutputImage )
    {
      oss<<"\t          Type: String (output image file name and optionally its pixel type)"<<std::endl;
      oss<<"\t                Possible pixel type: int8, uint8, int16, uint16, int32, uint32, float or double"<<std::endl;
    }
  else if( type == ParameterType_OutputVectorData )
    {
      oss<<"\t          Type: String (output vector data file name)"<<std::endl;
    }
  else if( type == ParameterType_String )
    {
      oss<<"\t          Type: String"<<std::endl;
    }
  else if( type == ParameterType_Choice )
    {
      oss<<"\t          Type: String: ";
      
      std::vector<std::string> keys  = m_Application->GetChoiceKeys(paramKey);
      std::vector<std::string> names = m_Application->GetChoiceNames(paramKey);
      for( unsigned int j=0; j<keys.size(); j++ )
        {
          oss << keys[j] << " for "<<names[j];
          if( j<= keys.size()-1 )
            {
              oss << ", ";
            }
        }
      oss<<std::endl;
    }
  else if( type == ParameterType_ListView )
    {
      // We don't have access to the possible values...
      oss<<"\t          Type: List of String" << std::endl;
    }
  else
    {
      oss<<"\t          Type: Type not handle yet"<<std::endl;
    }

 // Display parameter values
  if( m_Application->HasValue( paramKey ) )
    {
      // In the case choice, don't show the enum type.
      if( type == ParameterType_Choice )
        {
          oss << "\t Default value: "<< m_Application->GetChoiceKeys(paramKey)[m_Application->GetParameterInt( paramKey )]<< std::endl;
        }
      else if( type == ParameterType_OutputImage )
        {
          oss << "\t Default value: filename: "<< m_Application->GetParameterAsString( paramKey )<< std::endl;
          oss << "\t                pixel type: float"<< std::endl;
        }
      else
        {
          oss << "\t Default value: "<<m_Application->GetParameterAsString( paramKey )<< std::endl;
        }
    }
  else
    {
      if( type == ParameterType_OutputImage ) 
        {
          oss << "\t Default value: filename: none"<< std::endl;
          oss << "\t                pixel type: float"<< std::endl;
        }
      else
        oss << "\t Default value: none"<<std::endl;
    }

  if( !m_Parser->IsAttributExists( std::string("--").append(paramKey), m_Expression) )
    {
      if ( !m_Application->HasValue( paramKey ) )
        oss << "\t        Status: MISSING"<< std::endl;
      else
        oss << "\t        Status: DEFAULT VALUE"<< std::endl;
    }
  else if( m_Parser->GetAttribut( std::string("--").append(paramKey), m_Expression).size() == 0 )
    {
      oss << "\t        Status: NO VALUE ASSOCIATED"<< std::endl;
    }
  else
    {
      oss << "\t        Status: USER VALUE (";
      oss << m_Parser->GetAttributAsString( std::string("--").append(paramKey), m_Expression ) <<")";
      oss << std::endl;
    }

  return oss.str();
}


bool
CommandLineLauncher::CheckUnicity()
{
  bool res = true;
  // Extract expression keys
  std::vector<std::string> keyList = m_Parser->GetKeyList(m_Expression);

  // Check Unicity
  for( unsigned int i=0; i<keyList.size(); i++ )
    {
      std::vector<std::string> listTmp = keyList;
      const std::string keyRef = keyList[i];
      listTmp.erase(listTmp.begin()+i);
      for( unsigned int j=0; j<listTmp.size(); j++ )
        {
          if( keyRef == listTmp[j] )
            {
              res = false;
              break;
            }
        }
      if (res == false )
        break;
    }

  return res;
}

bool
CommandLineLauncher::CheckKeyValidity()
{
  bool res = true;
  // Extract expression keys
  std::vector<std::string> expKeyList = m_Parser->GetKeyList(m_Expression);
  
  // Extract application keys
  std::vector<std::string> appKeyList = m_Application->GetParametersKeys( true );
  appKeyList.push_back( std::string(m_Parser->GetModulePathKey()).substr(2, std::string(m_Parser->GetModulePathKey()).size()) );
  appKeyList.push_back( std::string(m_Parser->GetModuleNameKey()).substr(2, std::string(m_Parser->GetModuleNameKey()).size()) );
  appKeyList.push_back( "help" );
  appKeyList.push_back( "progress" );

  // Check if each key in the expression exists in the application
  for( unsigned int i=0; i<expKeyList.size(); i++ )
    {
      const std::string refKey = expKeyList[i];
      bool keyExist = false;
      for( unsigned int j=0; j<appKeyList.size(); j++ )
        {
          if( refKey == appKeyList[j] )
            {
              keyExist = true;
              break;
            }
        }
      if( keyExist == false )
        {
          res = false;
          break;
        }
    }

  return res;
}

}
}

