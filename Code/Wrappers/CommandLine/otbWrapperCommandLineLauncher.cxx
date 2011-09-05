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

void
CommandLineLauncher::Load( const std::string & exp )
{
  m_Expression = exp;

  this->Load();
}


void
CommandLineLauncher::Load()
{
  if (m_Expression == "")
    {
      itkExceptionMacro("No expression specified...");
    }

  this->LoadPath();
  this->LoadApplication();
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
      this->LoadParameters();
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
      this->LoadParameters();
      m_Application->ExecuteAndWriteOutput();
    }
}

void
CommandLineLauncher::LoadPath()
{
  std::vector<std::string> pathList;
  // look for the paths
  if( m_Parser->GetPaths( pathList, m_Expression) != CommandLineParser::OK )
    {
      return;
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
               std::cout<<param->GetKey()<<std::endl;
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
}


void
CommandLineLauncher::DisplayHelp()
{
  std::cerr<<"====================== HELP CONTEXT ======================"<<std::endl;
  std::cerr<<"Name: "<<m_Application->GetName()<<std::endl;
  std::cerr<<"Description: "<<m_Application->GetDescription()<<std::endl;
  std::cerr<<"Parameters: "<<std::endl;
  ParameterGroup::Pointer paramGr = m_Application->GetParameterList();
  const unsigned int nbOfParam = paramGr->GetNumberOfParameters();
  
  bool addMandaTag = true;
  // Mandatory parameters
  for( unsigned int i=0; i<nbOfParam; i++ )
    {
      Parameter::Pointer param =  paramGr->GetParameterByIndex(i);
      // Check if mandatory parameter are present and have value
      if( param->GetMandatory() == true )
        {
          itk::OStringStream oss;
          oss<<"--"<<param->GetKey()<<" ("<<param->GetName()<<")"<< std::endl;
          if( std::string(param->GetDescription()).size() != 0 )
            {
              oss<<"\t Description: "<<param->GetDescription()<<")."<<std::endl;
            }
          //oss << "\t Default value: "<<param->GetDefaultValue()<< std::endl;
          if( !m_Parser->IsAttributExists( std::string("--").append(param->GetKey()), m_Expression) )
            {
              oss << "\t Status: MISSING."<< std::endl;
            }
          else if( m_Parser->GetAttribut( std::string("--").append(param->GetKey()), m_Expression).size() == 0 )
            {
              oss << "\t Status: NO VALUE ASSOCIATED."<< std::endl;
            }
          else
            {
              oss << "\t Status: USER VALUE: ";
              std::vector<std::string> values = m_Parser->GetAttribut( std::string("--").append(param->GetKey()), m_Expression);
              for( unsigned int i=0; i<values.size(); i++)
                {
                  if( i<values.size() )
                    {
                      oss<<values[i]<<" ";
                    }
                  else
                    {
                      oss<<values[i];
                    }
                }
              oss << std::endl;
            }

          if( addMandaTag == true )
            {
              std::cerr<<"=== Mandatory parameters: "<<std::endl;
              addMandaTag = false;
            }
          std::cerr<< oss.str();
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
          itk::OStringStream oss;
          oss<<"--"<<param->GetKey()<<" ("<<param->GetName()<<")"<< std::endl;
          if( std::string(param->GetDescription()).size() != 0 )
            {
              oss<<"\t Description: "<<param->GetDescription()<<")."<<std::endl;
            }
          //oss << "\t Default value: "<<param->GetDefaultValue()<< std::endl;
          if( !m_Parser->IsAttributExists( std::string("--").append(param->GetKey()), m_Expression) )
            {
              oss << "\t Status: NOT USED."<< std::endl;
            }
          else if( m_Parser->GetAttribut( std::string("--").append(param->GetKey()), m_Expression).size() == 0 )
            {
              oss << "\t Status: NO VALUE ASSOCIATED."<< std::endl;
            }
          else
            {
              oss << "\t Status: USER VALUE: ";
              std::vector<std::string> values = m_Parser->GetAttribut( std::string("--").append(param->GetKey()), m_Expression);
              for( unsigned int i=0; i<values.size(); i++)
                {
                  if( i<values.size() )
                    {
                      oss<<values[i]<<" ";
                    }
                  else
                    {
                      oss<<values[i];
                    }
                }
              oss << std::endl;
            }

          if( addOptionTag == true )
            {
              std::cerr<<"=== Optional parameters: "<<std::endl;
              addOptionTag = false;
            }
          std::cerr<< oss.str() << std::endl;
        }
    }

  std::cerr<<"Detected user paths: "<<m_Path<<std::endl;

}

/*
template <class TParameterType>
void
CommandLineLauncher::SetValueToParameter(Parameter * param, const std::string & val )
{
  if( !this->CanCreateParameter<TParameterType>( param ) )
    {
      itkExceptionMacro("Impossible cast to add value to the parameter "<<param->GetKey()<<".");
    }
  
  dynamic_cast<TParameterType *>(param)->SetValue( val );
}

template <class TParameterType>
void
CommandLineLauncher::SetValueToParameter(Parameter * param, const std::vector<std::string> & val )
{
  if( !this->CanCreateParameter<TParameterType>( param ) )
    {
      itkExceptionMacro("Impossible cast to add value to the parameter "<<param->GetKey()<<".");
    }
  
  dynamic_cast<TParameterType *>(param)->SetValue( val );
}

template <class TParameterType>
void
CommandLineLauncher::SetFileNameToParameter(Parameter * param, const std::string & val )
{
  if( !this->CanCreateParameter<TParameterType>( param ) )
    {
      itkExceptionMacro("Impossible cast to add value to the parameter "<<param->GetKey()<<".");
    }
  
  dynamic_cast<TParameterType *>(param)->SetFileName( val );
}

template <class TParameterType>
void
CommandLineLauncher::SetFileNameToParameter(Parameter * param, const std::vector<std::string> & val )
{
  if( !this->CanCreateParameter<TParameterType>( param ) )
    {
      itkExceptionMacro("Impossible cast to add value to the parameter "<<param->GetKey()<<".");
    }
  
  dynamic_cast<TParameterType *>(param)->SetFileNameList( val );
}

template <class TParameterType>
void
CommandLineLauncher::SetFromFileNameToParameter(Parameter * param, const std::string & val )
{
  if( !this->CanCreateParameter<TParameterType>( param ) )
    {
      itkExceptionMacro("Impossible cast to add value to the parameter "<<param->GetKey()<<".");
    }
  
  dynamic_cast<TParameterType *>(param)->SetFromFileName( val );
}

template <class TParameterType>
void
CommandLineLauncher::SetFromFileNameToParameter(Parameter * param, const std::vector<std::string> & val )
{
  if( !this->CanCreateParameter<TParameterType>( param ) )
    {
      itkExceptionMacro("Impossible cast to add value to the parameter "<<param->GetKey()<<".");
    }
  
  dynamic_cast<TParameterType *>(param)->SetListFromFileName( val );
}

template <class TParameterType>
bool
CommandLineLauncher::CanCreateParameter( Parameter * param )
{
  return dynamic_cast<TParameterType *>(param) != 0;
}
*/
}
}

