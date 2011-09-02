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

  CommandLineParser::CommandLineParser() : m_ModuleNameKey("--moduleName"), m_PathKey("--modulePath")
{
}

CommandLineParser::~CommandLineParser()
{
}

CommandLineParser::ParseResultType
CommandLineParser::GetPaths( std::vector<std::string> paths, const std::string & exp )
{
  std::size_t found = std::string(exp).find(m_PathKey);
  if( found == std::string::npos )
    {
      return NOMODULEPATH;
    }

  std::vector<std::string> pathAttribut = GetAttribut(m_PathKey, std::string(exp));
  
  if( pathAttribut.size() == 0 )
    {
      return NOMODULEPATH;
    }
  
  for( unsigned i=0; i<pathAttribut.size(); i++)
    {  
      std::string fullPath = itksys::SystemTools::CollapseFullPath(pathAttribut[i].c_str());
      if( !itksys::SystemTools::FileIsDirectory(fullPath.c_str()) )
        {
          return INVALIDMODULEPATH;
        }
      paths.push_back(fullPath);
    }
  
    
  return OK;
}

CommandLineParser::ParseResultType
CommandLineParser::GetModuleName( std::string modName, const std::string & exp )
{
  std::vector<itksys::String> spaceSplittedExp = itksys::SystemTools::SplitString(exp.c_str(), ' ', false);
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
          modName = spaceSplittedExp[0];
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
          std::vector<std::string> moduleNameAttribut = GetAttribut(m_ModuleNameKey, std::string(exp));
          
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
          
          modName = moduleNameAttribut[0];
        }
    }

  return OK;
}



std::vector<std::string> 
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

  // Remove space at the begining of the string and cast into std::vector<std::string>
  std::vector<std::string> res;
  for(unsigned int i=0; i<spaceSplitted.size(); i++)
    {
      while( spaceSplitted[i].size()>0  && spaceSplitted[i][0] == ' ' )
        {
          spaceSplitted[i] = spaceSplitted[i].substr(1, spaceSplitted[i].size());
        }
      res.push_back(spaceSplitted[i]);
    }

   return res; 
}

}
}

