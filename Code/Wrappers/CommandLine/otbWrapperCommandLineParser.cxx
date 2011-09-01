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


  std::string expFromPath = std::string(exp).substr(found+m_PathKey.size(), std::string(exp).size());
  std::string tempModPath = expFromPath;
  if( expFromPath.find("--") != std::string::npos)
    {
      tempModPath = expFromPath.substr( 0, expFromPath.find_first_of("--")-1);
    }
  
  if( tempModPath.size() == 0 )
    {
      return NOMODULEPATH;
    }
  else
    {
      std::vector<itksys::String> spaceSplittedModulePath = itksys::SystemTools::SplitString(tempModPath.substr(1, tempModPath.size()).c_str(), ' ', false);
      for(unsigned int i=0; i<spaceSplittedModulePath.size(); i++)
        {
          if( spaceSplittedModulePath[i] == " ")
            {
              spaceSplittedModulePath.erase(spaceSplittedModulePath.begin()+i);
              i--;
            }
        }
      if( spaceSplittedModulePath.size() == 0 )
        {
          return NOMODULEPATH;
        }
   
      std::string pathlist;
      for( unsigned i=0; i<spaceSplittedModulePath.size(); i++)
        {  
          std::string fullPath = itksys::SystemTools::CollapseFullPath(spaceSplittedModulePath[i].c_str());
          if( !itksys::SystemTools::FileIsDirectory(fullPath.c_str()) )
            {
              return INVALIDMODULEPATH;
            }
          pathlist.append(fullPath);
          pathlist.append(":");
        }

      m_Path = pathlist;
    }

  return OK;
}

void
CommandLineParser::LoadPath()
{
  // Load the path in the environment
  std::string specificEnv("ITK_AUTOLOAD_PATH=");
  specificEnv.append(m_Path);
  std::cout<<specificEnv<<std::endl;
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
              std::cout<<"Multiple at level 1"<<std::endl;
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
          /*
          std::string expFromModule = std::string(exp).substr(found+m_ModuleNameKey.size(), std::string(exp).size());
          std::string tempModName = expFromModule;
          if( expFromModule.find("--") != std::string::npos)
            {
              tempModName = expFromModule.substr( 0, expFromModule.find_first_of("--")-1);
            }
        
          if( tempModName.size() == 0 )
            {
              return NOMODULENAME;
            }
          else
            {
              std::vector<itksys::String> spaceSplittedModuleName = itksys::SystemTools::SplitString(tempModName.substr(1, tempModName.size()).c_str(), ' ', false);
              for(unsigned int i=0; i<spaceSplittedModuleName.size(); i++)
                {
                  if( spaceSplittedModuleName[i] == " ")
                    {
                      spaceSplittedModuleName.erase(spaceSplittedModuleName.begin()+i);
                      i--;
                    }
                }
          */
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


bool 
CommandLineParser::LoadApplication( const std::string & moduleName )
{
  bool res = true;

  m_Application = ApplicationRegistry::CreateApplication(moduleName);

  for(unsigned int i=0; i<ApplicationRegistry::GetAvailableApplications().size(); i++)
    std::cout<<ApplicationRegistry::GetAvailableApplications()[i]<<std::endl;

  std::cout<<moduleName<<std::endl;

  if (m_Application.IsNull())
    {
      std::cout << "Could not find application \"" << moduleName <<"\""<< std::endl;
      
      res = false;
    }

  return res;
}


bool 
CommandLineParser::CheckApplicationArgument( const std::string & exp )
{
  return this->CheckApplicationArgument( exp.c_str() );
}

bool 
CommandLineParser::CheckApplicationArgument( const char * exp )
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
      if( paramGr->GetParameterByIndex(i)->GetMandatory() == true )
        {
          // Check if the attribut is in the expression
          std::size_t found = std::string(exp).find(paramGr->GetParameterByIndex(i)->GetKey());
           if( found == std::string::npos )
             {
               std::cout<<paramGr->GetParameterByIndex(i)->GetKey()<<std::endl;
               return MISSINGMANDATORYATTRIBUT;
             }
        }
    }
         
  // Other 
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
      std::cout<<spaceSplitted[i]<<std::endl;
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

  for(unsigned int i=0; i<spaceSplitted.size(); i++)
    {
      std::cout<<spaceSplitted[i]<<std::endl;
    }


  return spaceSplitted; 
}

}
}

