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
// List value parameter
#include "otbWrapperOutputImageListParameter.h"
#include "otbWrapperInputImageListParameter.h"
#include "otbWrapperStringListParameter.h"
//#include "otbWrapperParameterGroup.h"


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

CommandLineLauncher::CommandLineLauncher() : m_Expression("")
{
  m_Application = NULL;
  m_Parser = CommandLineParser::New();
}

CommandLineLauncher::CommandLineLauncher(const char * exp) : m_Expression(exp)
{
  m_Application = NULL;
  m_Parser = CommandLineParser::New();
}

CommandLineLauncher::~CommandLineLauncher()
{
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

  if ( this->LoadPath() == false )
    {
      std::cout<<"Invalid paths..."<<std::endl;
      std::cout<<"Please check values : "<<m_Parser->GetAttributAsString( "--modulePath", m_Expression )<<"."<<std::endl;
      return false;
    }
  this->LoadApplication();

  return true;
}

void
CommandLineLauncher::Execute()
{
  if( m_Application.IsNull() )
    {
      itkExceptionMacro("No application loaded");  
    }
  
  // if help is asked...
  if ( m_Parser->IsAttributExists( "--help", m_Expression ) == true )
    {
      this->DisplayHelp();
    }
  else
    {
      if ( this->LoadParameters() != OKPARAM )
        {
          this->LoadApplication();
          this->DisplayHelp();
          
          return;
        }
      m_Application->Execute();
    }
} 

 void
CommandLineLauncher::ExecuteAndWriteOutput()
{
  if( m_Application.IsNull() )
    {
      itkExceptionMacro("No application loaded");  
    }

  // if help is asked...
  if ( m_Parser->IsAttributExists( "--help", m_Expression ) == true )
    {
      this->DisplayHelp();
    }
  else
    {
      if ( this->LoadParameters() != OKPARAM )
        {
          this->LoadApplication();
          this->DisplayHelp();

          return;
        }

      m_Application->ExecuteAndWriteOutput();
    }
}

bool
CommandLineLauncher::LoadPath()
{
  std::vector<std::string> pathList;
  // look for the paths
  if( m_Parser->GetPaths( pathList, m_Expression) != CommandLineParser::OK )
    {
      return false;
    }
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

  return true;
}


void
CommandLineLauncher::LoadApplication()
{
  // Look for the module name
  std::string moduleName;
  if( m_Parser->GetModuleName( moduleName, m_Expression ) != CommandLineParser::OK )
    {
      std::cout << "LoadApplication, no module found..." <<std::endl;     
      return;
    }

  m_Application = ApplicationRegistry::CreateApplication(moduleName);
  
  if (m_Application.IsNull())
    {
      std::cout << "Could not find application \"" << moduleName <<"\""<< std::endl;
      return;
    }
}

CommandLineLauncher::ParamResultType
CommandLineLauncher::LoadParameters()
{
  if( m_Application.IsNull() )
    {
      itkExceptionMacro("No application loaded");
    }
  
  // Mandatory case
  ParameterGroup::Pointer paramGr = m_Application->GetParameterList();
  const unsigned int nbOfParam = paramGr->GetNumberOfParameters();
  
  for( unsigned int i=0; i<nbOfParam; i++ )
    {
      std::cout<<paramGr->GetParameterByIndex(i)->GetKey()<<std::endl;
      std::vector<std::string> values;
      Parameter::Pointer param =  paramGr->GetParameterByIndex(i);
      // Check if mandatory parameter are present and have value
      if( param->GetMandatory() == true )
        {
          if( !m_Parser->IsAttributExists( std::string("--").append(param->GetKey()), m_Expression ) )
             {
               std::cout<<"No param for (tu fais quoi la??)... "<<param->GetKey()<<std::endl;
               return MISSINGMANDATORYPARAMETER;
             }
           values = m_Parser->GetAttribut( std::string("--").append(param->GetKey()), m_Expression);
           if(  values.size() == 0 )
             {
               std::cout<<"No param for... "<<param->GetKey()<<std::endl;
               return MISSINGPARAMETERVALUE;
             }
        }
      // Check if non mandatory parameter have values
      else
        {
          if( m_Parser->IsAttributExists( std::string("--").append(param->GetKey()), m_Expression ) )
            {
              values = m_Parser->GetAttribut( std::string("--").append(param->GetKey()), m_Expression);
              if(  values.size() == 0 )
                {
                  std::cout<<"No param for... "<<param->GetKey()<<std::endl;
                  return MISSINGPARAMETERVALUE;
                }
            }
        }

      // Cast into std::vecto<std::string>
      std::vector<std::string> svValues;
      for( unsigned int j=0; j<values.size(); j++)
        {
          svValues.push_back(values[j]);
        }
      
      ParameterType type = m_Application->GetParameterType( param->GetKey());
      // List values parameter case
      if( type == ParameterType_InputImageList )
        {
          dynamic_cast<InputImageListParameter *>(param.GetPointer())->SetListFromFileName( svValues );
        }
      else if( type == ParameterType_OutputImageList )
        {
          dynamic_cast<OutputImageListParameter *>(param.GetPointer())->SetFileNameList( svValues );
        }
      else if( type == ParameterType_StringList )
        {
          dynamic_cast<StringListParameter *>(param.GetPointer())->SetValue( svValues );
        }
      else  if( svValues.size() != 1)
        {
          return INVALIDNUMBEROFVALUE;
        }

      // Single value parameter
   
      if( type == ParameterType_Choice || type == ParameterType_Float || type == ParameterType_Int || type == ParameterType_Radius 
          || type == ParameterType_Directory || type == ParameterType_String || type == ParameterType_Filename || type == ParameterType_InputComplexImage 
          || type == ParameterType_InputImage || type == ParameterType_InputVectorData || type == ParameterType_OutputImage || type == ParameterType_OutputVectorData )
        {
          m_Application->SetParameterString( param->GetKey(), svValues[0] );
        }
      else if( type == ParameterType_Empty )
        {
          if( svValues[0] == "1" || svValues[0] == "true")
            {
              dynamic_cast<EmptyParameter *>(param.GetPointer())->SetActive(true);
            }
          else if( svValues[0] == "0" || svValues[0] == "false")
            {
              dynamic_cast<EmptyParameter *>(param.GetPointer())->SetActive(false);
            }
          else
            {
              return WRONGPARAMETERVALUE;
            }
        }
    }
  return OKPARAM;
}


void
CommandLineLauncher::DisplayHelp()
{
  std::cerr<<std::endl;
  std::cerr<<"====================== HELP CONTEXT ======================"<<std::endl;
  std::cerr<<"NAME: "<<m_Application->GetName()<<std::endl;
  std::cerr<<"DESCRIPTION: "<<m_Application->GetDescription()<<std::endl;
  std::cerr<<"PARAMETERS: "<<std::endl;

  ParameterGroup::Pointer paramGr = m_Application->GetParameterList();
  const unsigned int nbOfParam = paramGr->GetNumberOfParameters();


  std::cerr<<"=== Mandatory parameters: "<<std::endl;
  std::cerr<<"--modPath (Executables paths)"<<std::endl;
  std::cerr<<"\t   Description: Paths to the executable library."<<std::endl;
  if( !m_Parser->IsAttributExists( "--modPath", m_Expression ) )
    std::cerr<<"\t        Status: ENVIRONEMENT PATH"<<std::endl;
  else if( m_Path == "") 
    std::cerr<< "\t       Status: NO VALUE ASSOCIATED "<<m_Path<<std::endl;
  else
    std::cerr<< "\t       Status: USER VALUE: "<<m_Path<<std::endl;

  // Mandatory parameters
  for( unsigned int i=0; i<nbOfParam; i++ )
    {
      Parameter::Pointer param =  paramGr->GetParameterByIndex(i);
      // Check if mandatory parameter are present and have value
      if( param->GetMandatory() == true )
        {
          std::cerr<< this->DisplayParameterHelp( param );
        }
    }
 
  bool addOptionTag = true;
  // Optional parameters
  for( unsigned int i=0; i<nbOfParam; i++ )
    {
      Parameter::Pointer param =  paramGr->GetParameterByIndex(i);
      // Check if mandatory parameter are present and have value
      if( param->GetMandatory() != true )
        {
          if( addOptionTag == true )
            {
              std::cerr<<"=== Optional parameters: "<<std::endl;
              addOptionTag = false;
            }

          std::cerr << this->DisplayParameterHelp( param );
        }
 

    }

}

std::string
CommandLineLauncher::DisplayParameterHelp( const Parameter::Pointer & param )
{
  itk::OStringStream oss;
  oss<<"--"<<param->GetKey()<<" ("<<param->GetName()<<")"<< std::endl;
  
  if( std::string(param->GetDescription()).size() != 0 )
    {
      oss<<"\t   Description: "<<param->GetDescription()<<std::endl;
    }
  
  if( m_Application->HasValue( param->GetKey() ) )
    oss << "\t Default value: "<<m_Application->GetParameterAsString( param->GetKey() )<< std::endl;
  else
    oss << "\t Default value: none"<<std::endl;
  
  if( !m_Parser->IsAttributExists( std::string("--").append(param->GetKey()), m_Expression) )
    {
      if ( !m_Application->HasValue( param->GetKey() ) )
        oss << "\t        Status: MISSING"<< std::endl;
      else
        oss << "\t        Status: DEFAULT VALUE"<< std::endl;
    }
  else if( m_Parser->GetAttribut( std::string("--").append(param->GetKey()), m_Expression).size() == 0 )
    {
      oss << "\t        Status: NO VALUE ASSOCIATED"<< std::endl;
    }
  else
    {
      oss << "\t        Status: USER VALUE (";
      oss << m_Parser->GetAttributAsString( std::string("--").append(param->GetKey()), m_Expression ) <<")";
      oss << std::endl;
    }

  return oss.str();
}

}
}

