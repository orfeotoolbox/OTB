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
CommandLineLauncher::Launch()
{
  if (m_Expression == "")
    {
      itkExceptionMacro("No expression specified...");
    }

  this->LoadPath();
  this->LoadApplication();
  this->LoadParameters();
}

void
CommandLineLauncher::Execute()
{
  if( m_Application.IsNull() )
    {
      itkExceptionMacro("No application loaded");  
    }
  m_Application->Execute();
} 

 void
CommandLineLauncher::ExecuteAndWriteOutput()
{
  if( m_Application.IsNull() )
    {
      itkExceptionMacro("No application loaded");  
    }
  m_Application->ExecuteAndWriteOutput();
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
  std::string pathAttribut;
  for( unsigned i=0; i<pathList.size(); i++)
    {  
      pathAttribut.append(pathList[i]);
      pathAttribut.append(":");
    }

  std::string specificEnv("ITK_AUTOLOAD_PATH=");
  specificEnv.append(pathAttribut);
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
  if( m_Parser->GetModuleName( moduleName, m_Expression) != CommandLineParser::OK )
    {
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
          std::size_t found = m_Expression.find(param->GetKey());
           if( found == std::string::npos )
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
          std::size_t found = m_Expression.find(param->GetKey());
          if( found != std::string::npos )
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
      
      // List values parameter case
      if( this->CanCreateParameter<InputImageListParameter>(param) == true )
        {
          this->SetFromFileNameToParameter<InputImageListParameter>( param, svValues );
        }
      else if( this->CanCreateParameter<OutputImageListParameter>(param) == true )
        {
          this->SetFileNameToParameter<OutputImageListParameter>( param, svValues);
        }
      else if( this->CanCreateParameter<StringListParameter>(param) == true )
        {
          this->SetValueToParameter<StringListParameter>( param, svValues );
        }
      else  if( svValues.size() != 1)
        {
          return INVALIDNUMBEROFVALUE;
        }

      // Single value parameter
      if( this->CanCreateParameter<ChoiceParameter>(param) == true )
        {
          this->SetValueToParameter<ChoiceParameter>( param, svValues[0] );
        }
      else if( this->CanCreateParameter<FloatParameter>(param) == true )
        {
          this->SetValueToParameter<FloatParameter>( param, svValues[0] );
        }
      else if( this->CanCreateParameter<IntParameter>(param) == true )
        {
          this->SetValueToParameter<IntParameter>( param, svValues[0] );
        }
      else if( this->CanCreateParameter<RadiusParameter>(param) == true )
        {
          this->SetValueToParameter<RadiusParameter>( param, svValues[0] );
        }
      else if( this->CanCreateParameter<DirectoryParameter>(param) == true )
        {
          this->SetValueToParameter<DirectoryParameter>(param, svValues[0]);
        }
      else if( this->CanCreateParameter<StringParameter>(param) == true )
        {
          this->SetValueToParameter<StringParameter>( param, svValues[0] );
        }
      else if( this->CanCreateParameter<FilenameParameter>(param) == true )
        {
          this->SetValueToParameter<FilenameParameter>(param, svValues[0]);
        }
      else if( this->CanCreateParameter<InputComplexImageParameter>(param) == true )
        {
          this->SetFromFileNameToParameter<InputComplexImageParameter>(param, svValues[0]);
        }
      else if( this->CanCreateParameter<InputImageParameter>(param) == true )
        {
          this->SetFromFileNameToParameter<InputImageParameter>(param, svValues[0]);
        }
      else if( this->CanCreateParameter<InputVectorDataParameter>(param) == true )
        {
          this->SetFromFileNameToParameter<InputVectorDataParameter>(param, svValues[0]);
        }
      else if( this->CanCreateParameter<OutputImageParameter>(param) == true )
        {
          this->SetFileNameToParameter<OutputImageParameter>(param, svValues[0]);
        }
      else if( this->CanCreateParameter<OutputVectorDataParameter>(param) == true )
        {
          this->SetFileNameToParameter<OutputVectorDataParameter>(param, svValues[0]);
        }
      else if( this->CanCreateParameter<EmptyParameter>(param) == true )
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

}
}

