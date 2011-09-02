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
#include "otbWrapperCommandLineParser.h"

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

  CommandLineParser::CommandLineParser() : m_Path(""), m_ModuleName(""), m_ModuleNameKey("--moduleName"), m_PathKey("--modulePath")
{
  m_Application = NULL;
}

CommandLineParser::~CommandLineParser()
{
}

CommandLineParser::ParseResultType
CommandLineParser::CheckExpression( const std::string & exp )
{
  return this->CheckExpression( exp.c_str() );
}

CommandLineParser::ParseResultType
CommandLineParser::CheckExpression( const char * exp )
{
  // Check if a module name was specified
  if( std::string(exp).size() == 0)
    {
      return EMPTYEXPRESSION;
    }

  
  ParseResultType resModulePath =this->CheckPath( exp );
  if( resModulePath != OK )
    {
      return resModulePath;
    }
  
  this->LoadPath();

  ParseResultType resModuleName = this->CheckModuleName( exp );
  if( resModuleName != OK )
    {
      return resModuleName;
    }

  // Check the module name validity
  if (this->LoadApplication( m_ModuleName ) == false)
    {
      return WRONGMODULENAME;
    }

  return OK;
}

CommandLineParser::ParseResultType
CommandLineParser::CheckPath( const char * exp )
{
  // CASE 2 : set as --moduleName
  // Must contain the string --moduleName
  // Only 1 string before the following --
  // And must contain only alphanumerical character

  std::size_t found = std::string(exp).find(m_PathKey);
  if( found == std::string::npos )
    {
      return NOMODULEPATH;
    }

  std::vector<itksys::String> pathAttribut = GetAttribut(m_PathKey, std::string(exp));
  
  if( pathAttribut.size() == 0 )
    {
      return NOMODULEPATH;
    }
  
  std::string pathlist;
  for( unsigned i=0; i<pathAttribut.size(); i++)
    {  
      std::string fullPath = itksys::SystemTools::CollapseFullPath(pathAttribut[i].c_str());
      if( !itksys::SystemTools::FileIsDirectory(fullPath.c_str()) )
        {
          return INVALIDMODULEPATH;
        }
      pathlist.append(fullPath);
      pathlist.append(":");
    }
  
  m_Path = pathlist;
  
  return OK;
}

void
CommandLineParser::LoadPath()
{
  // Load the path in the environment
  std::string specificEnv("ITK_AUTOLOAD_PATH=");
  specificEnv.append(m_Path);

  // do NOT use putenv() directly, since the string memory must be managed carefully
  itksys::SystemTools::PutEnv(specificEnv.c_str());
  // Reload factories to take into account new path
  itk::ObjectFactoryBase::ReHash();
}

CommandLineParser::ParseResultType
CommandLineParser::CheckModuleName( const char * exp )
{
  std::vector<itksys::String> spaceSplittedExp = itksys::SystemTools::SplitString(exp, ' ', false);
  // if the chain is "  module", SplitString will return: [ ], [module]
  for(unsigned int i=0; i<spaceSplittedExp.size(); i++)
    {
      if( spaceSplittedExp[i] == " ")
        {
          spaceSplittedExp.erase(spaceSplittedExp.begin()+i);
          i--;
        }
    }

  // The SplitString keep the separator in the string.
  // If exists a space a the beginnig of the string, it will be interpreted...
  // We have to delete it
  if( spaceSplittedExp[0][0] == ' ' )
    {
      spaceSplittedExp[0].erase(spaceSplittedExp[0].begin());
    }
  
  itksys::RegularExpression reg;
  reg.compile("([^0-9a-zA-Z])");
  // CASE 1 : direct module name
  // First string mustn't start with --
  // Second string must contains -- (2 modules name not allow)
  // And must contain only alphanumerical character
  if( spaceSplittedExp[0].substr(0,2) != "--" )
    {
      if( spaceSplittedExp.size() > 1 )
        {
          if( spaceSplittedExp[1].substr(0,2) != "--" )
            {
              return MULTIPLEMODULENAME;
            }
        }
      if(reg.find(spaceSplittedExp[0]))
        {
          return INVALIDMODULENAME;
        }
      else
        {
          m_ModuleName = spaceSplittedExp[0];
        }
    }
  // CASE 2 : set as --moduleName
  // Must contain the string --moduleName
  // Only 1 string before the following --
  // And must contain only alphanumerical character
  else
    {
      std::size_t found = std::string(exp).find(m_ModuleNameKey);
      if( found == std::string::npos )
        {
          return NOMODULENAME;
        }
      else
        {
          std::vector<itksys::String> moduleNameAttribut = GetAttribut(m_ModuleNameKey, std::string(exp));
          
          if( moduleNameAttribut.size() == 0 )
            {
              return NOMODULENAME;
            }
          if( moduleNameAttribut.size() > 1 )
            {
              return MULTIPLEMODULENAME;
            }
          if(reg.find(moduleNameAttribut[0]))
            {
              return INVALIDMODULENAME;
            }
          
          m_ModuleName = moduleNameAttribut[0];
        }
    }

  return OK;
}


std::vector<itksys::String> 
CommandLineParser::GetAttribut( const std::string & key, const std::string & exp )
{
  std::size_t found = std::string(exp).find(key);
  if( found == std::string::npos )
    {
      itkExceptionMacro("No key \""<<key<<"\" found in \""<<exp<<"\".");
    }
  
  std::string expFromKey = std::string(exp).substr(found+key.size(), std::string(exp).size());
  std::string tempModKey = expFromKey;
  // remove other key in the string if there's any
  if( expFromKey.find("--") != std::string::npos)
    {
      tempModKey = expFromKey.substr( 0, expFromKey.find("--")-1);
    }

  std::vector<itksys::String> spaceSplitted = itksys::SystemTools::SplitString(tempModKey.substr(1, tempModKey.size()).c_str(), ' ', false);
  // Remove " " string element
  for(unsigned int i=0; i<spaceSplitted.size(); i++)
    {
      if( spaceSplitted[i] == " ")
        {
          spaceSplitted.erase(spaceSplitted.begin()+i);
          i--;
        }
    }

  // Removespace at the begining of the string
  for(unsigned int i=0; i<spaceSplitted.size(); i++)
    {
      while(  spaceSplitted[i][0] == ' ' )
        {
          spaceSplitted[i] = spaceSplitted[i].substr(1, spaceSplitted[i].size());
        }
    }

   return spaceSplitted; 
}


bool 
CommandLineParser::LoadApplication( const std::string & moduleName )
{
  bool res = true;

  m_Application = ApplicationRegistry::CreateApplication(moduleName);

  if (m_Application.IsNull())
    {
      std::cout << "Could not find application \"" << moduleName <<"\""<< std::endl;
      
      res = false;
    }

  return res;
}


CommandLineParser::ParseResultType 
CommandLineParser::ParseApplicationArgument( const std::string & exp )
{
  return this->ParseApplicationArgument( exp.c_str() );
}

CommandLineParser::ParseResultType 
CommandLineParser::ParseApplicationArgument( const char * exp )
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
      std::vector<itksys::String> values;
      Parameter::Pointer param =  paramGr->GetParameterByIndex(i);
      // Check if mandatory parameter are present and have value
      if( param->GetMandatory() == true )
        {
          std::size_t found = std::string(exp).find(param->GetKey());
           if( found == std::string::npos )
             {
               std::cout<<param->GetKey()<<std::endl;
               return MISSINGMANDATORYPARAMETER;
             }
           values = this->GetAttribut( std::string("--").append(param->GetKey()), std::string(exp));
           if(  values.size() == 0 )
             {
               std::cout<<"No param for... "<<param->GetKey()<<std::endl;
               return MISSINGPARAMETERVALUE;
             }
        }
      // Check if non mandatory parameter have values
      else
        {
          std::size_t found = std::string(exp).find(param->GetKey());
          if( found != std::string::npos )
            {
              values = this->GetAttribut( std::string("--").append(param->GetKey()), std::string(exp));
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
         
  // Other 
  return OK;
}



template <class TParameterType>
void
CommandLineParser::SetValueToParameter(Parameter * param, const std::string & val )
{
  if( !this->CanCreateParameter<TParameterType>( param ) )
    {
      itkExceptionMacro("Impossible cast to add value to the parameter "<<param->GetKey()<<".");
    }
  
  dynamic_cast<TParameterType *>(param)->SetValue( val );
}

template <class TParameterType>
void
CommandLineParser::SetValueToParameter(Parameter * param, const std::vector<std::string> & val )
{
  if( !this->CanCreateParameter<TParameterType>( param ) )
    {
      itkExceptionMacro("Impossible cast to add value to the parameter "<<param->GetKey()<<".");
    }
  
  dynamic_cast<TParameterType *>(param)->SetValue( val );
}

template <class TParameterType>
void
CommandLineParser::SetFileNameToParameter(Parameter * param, const std::string & val )
{
  if( !this->CanCreateParameter<TParameterType>( param ) )
    {
      itkExceptionMacro("Impossible cast to add value to the parameter "<<param->GetKey()<<".");
    }
  
  dynamic_cast<TParameterType *>(param)->SetFileName( val );
}

template <class TParameterType>
void
CommandLineParser::SetFileNameToParameter(Parameter * param, const std::vector<std::string> & val )
{
  if( !this->CanCreateParameter<TParameterType>( param ) )
    {
      itkExceptionMacro("Impossible cast to add value to the parameter "<<param->GetKey()<<".");
    }
  
  dynamic_cast<TParameterType *>(param)->SetFileNameList( val );
}

template <class TParameterType>
void
CommandLineParser::SetFromFileNameToParameter(Parameter * param, const std::string & val )
{
  if( !this->CanCreateParameter<TParameterType>( param ) )
    {
      itkExceptionMacro("Impossible cast to add value to the parameter "<<param->GetKey()<<".");
    }
  
  dynamic_cast<TParameterType *>(param)->SetFromFileName( val );
}

template <class TParameterType>
void
CommandLineParser::SetFromFileNameToParameter(Parameter * param, const std::vector<std::string> & val )
{
  if( !this->CanCreateParameter<TParameterType>( param ) )
    {
      itkExceptionMacro("Impossible cast to add value to the parameter "<<param->GetKey()<<".");
    }
  
  dynamic_cast<TParameterType *>(param)->SetListFromFileName( val );
}

template <class TParameterType>
bool
CommandLineParser::CanCreateParameter( Parameter * param )
{
  return dynamic_cast<TParameterType *>(param) != 0;
}

}
}

