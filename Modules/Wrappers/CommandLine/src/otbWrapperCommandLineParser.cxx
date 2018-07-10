/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include "otbWrapperCommandLineParser.h"


#include "otbWrapperApplicationRegistry.h"

#include <itksys/SystemTools.hxx>
#include <itksys/RegularExpression.hxx>
#include <string>
#include <iostream>

namespace otb
{
namespace Wrapper
{

CommandLineParser::CommandLineParser()
{
}

CommandLineParser::~CommandLineParser()
{
}

std::string
CommandLineParser::GetPathsAsString(const std::vector<std::string> & vexp )
{
  std::string res;
  std::vector<std::string> pathList;

  if (this->GetPaths(pathList,vexp) == OK)
    {
    std::vector<std::string>::iterator it;
    for (it=pathList.begin(); it!=pathList.end(); ++it)
      {
      if (!res.empty())
        {
        res.append(" ");
        }
      res.append(*it);
      }
    }
  return res;
}

std::string
CommandLineParser::GetPathsAsString( const std::string & exp )
{
  std::string res;
  // The first element must be the module path, non " -" allowed.
  // The module path list element are the strings between the first
  // element and the next key (ie. " -" string).
  std::string tempModPath = exp;

  if( exp.find(" -") != std::string::npos)
    {
    tempModPath = exp.substr( 0, exp.find(" -"));
    }

  // Get everything after the module path
  if( tempModPath.find(" ") != std::string::npos)
    {
    res = tempModPath.substr( tempModPath.find(" ")+1, tempModPath.size());
    }

  // Suppress possible multi space at the beginning of the string
  while (res.size()>0 && res[0]==' ')
    {
    res.erase(0, 1);
    }

  return res;
}

CommandLineParser::ParseResultType
CommandLineParser::GetPaths( std::vector<std::string> & paths, const std::vector<std::string> & exp )
{
  if (exp.empty())
    {
    return NOMODULEPATH;
    }

  // The first element must be the module name, non " -" allowed.
  // The module path list elements are the strings between the first
  // element and the first key (ie. string which begins with "-").
  std::vector<std::string>::const_iterator it = exp.begin();
  ++it; // first element is module name

  std::string tmpPath;

  while (it != exp.end())
    {
    tmpPath = *it;
    // If this is the first key : exit loop
    if (tmpPath[0]=='-')
      {
      break;
      }
    // Add path to result
    std::string fullPath = itksys::SystemTools::CollapseFullPath(tmpPath);
    if( !itksys::SystemTools::FileIsDirectory(fullPath) )
      {
      std::cerr<<"Invalid module path: "<<fullPath<<std::endl;
      return INVALIDMODULEPATH;
      }
    paths.push_back(fullPath);
    ++it;
    }

  if (paths.empty())
    {
    return NOMODULEPATH;
    }

  return OK;
}

CommandLineParser::ParseResultType
CommandLineParser::GetPaths( std::vector<std::string> & paths, const std::string & exp )
{
  std::string pathsList = this->GetPathsAsString( exp );

  if( pathsList.size() == 0 )
    {
    return NOMODULEPATH;
    }

  std::string tempModPath = pathsList;
  // remove other key in the string if there's any
  if( pathsList.find(" -") != std::string::npos)
    {
    tempModPath = pathsList.substr( 0, pathsList.find(" -"));
    }

  if( tempModPath.size() > 0 )
    {
    std::vector<itksys::String> pathAttribut = itksys::SystemTools::SplitString(tempModPath.substr(0, tempModPath.size()), ' ', false);


    // Remove " " string element
    for(unsigned int i=0; i<pathAttribut.size(); i++)
      {
      // Suppress possible multi space at the beginning of the string
      while (pathAttribut[i].size()>1 && pathAttribut[i][0]==' ')
      {
      pathAttribut[i].erase(0, 1);
      }

      std::string fullPath = itksys::SystemTools::CollapseFullPath(pathAttribut[i]);

      if( !itksys::SystemTools::FileIsDirectory(fullPath) )
        {
        std::cerr<<"Invalid module path: "<<fullPath<<std::endl;
        return INVALIDMODULEPATH;
        }
      paths.push_back(fullPath);
      }
    }
  else
    {
    return NOMODULEPATH;
    }


  return OK;
}

CommandLineParser::ParseResultType
CommandLineParser::GetModuleName( std::string & modName, const std::vector<std::string> & exp )
{
  if (exp.empty())
    {
    return NOMODULENAME;
    }

  itksys::RegularExpression reg;
  reg.compile("([^0-9a-zA-Z])");
  // The first element must be the module path, non " -" allowed.
  if( exp[0][0] == '-')
    {
    return NOMODULENAME;
    }
  
  // It must contain only alphanumerical character
  if(reg.find(exp[0]))
    {
    return INVALIDMODULENAME;
    }
  else
    {
    modName = exp[0];
    }
      
  return OK;
}

CommandLineParser::ParseResultType
CommandLineParser::GetModuleName( std::string & modName, const std::string & exp )
{
  std::vector<itksys::String> spaceSplitExp = itksys::SystemTools::SplitString(exp, ' ', false);
  // if the chain is "  module", SplitString will return: [ ], [module]
  for(unsigned int i=0; i<spaceSplitExp.size(); i++)
    {
    if( spaceSplitExp[i] == " ")
      {
      spaceSplitExp.erase(spaceSplitExp.begin()+i);
      i--;
      }
    }

  // The SplitString keep the separator in the string.
  // If exists a space a the beginning of the string, it will be interpreted...
  // We have to delete it
  if( spaceSplitExp[0][0] == ' ' )
    {
    spaceSplitExp[0].erase(spaceSplitExp[0].begin());
    }

  itksys::RegularExpression reg;
  reg.compile("([^0-9a-zA-Z])");
  // The first element must be the module path, non " -" allowed.
  if( spaceSplitExp[0].substr(0, 2) == " -" || spaceSplitExp.size() == 0 )
    {
    return NOMODULENAME;
    }

  // It must contain only alphanumerical character
  if(reg.find(spaceSplitExp[0]))
    {
    return INVALIDMODULENAME;
    }
  else
    {
    modName = spaceSplitExp[0];
    }

  return OK;
}

std::vector<std::string>
CommandLineParser::GetAttribut( const std::string & key, const std::vector<std::string> & exp )
{
  std::vector<std::string> res;
  if (!this->IsAttributExists(key,exp))
    return res;

  bool foundKey=false;
  std::vector<std::string>::const_iterator it = exp.begin();
  while(it!=exp.end() )
    {
    if (foundKey)
      {
      if (it->find("-") == 0)
        {
        std::string tmpKey = it->substr(1,std::string::npos);
        if (this->IsAValidKey(tmpKey))
          {
          break;
          }
        else
          {
          res.push_back(*it);
          }
        }
      else
        res.push_back(*it);
      }
    else
      {
      if (it->compare(key) == 0)
        {
        foundKey=true;
        }
      }
    ++it;
    }
  return res;
}

std::vector<std::string>
CommandLineParser::GetAttribut( const std::string & key, const std::string & exp )
{
  std::vector<std::string> res;

  std::string keySpaced = key;
  keySpaced.append(" ");
  std::size_t found = std::string(exp).find(keySpaced);
  if( found == std::string::npos )
    {
    return res;
    }
  std::string expFromKey = std::string(exp).substr(found+key.size()+1, std::string(exp).size());

  if( expFromKey.size() == 0 )
    {
    return res;
    }
  std::string tempModKey = expFromKey;
  // remove other key in the string if there's any
  bool stop = false;
  std::size_t curPos = expFromKey.find(" -");
  std::size_t curPosOld = 0;

  while ( !stop && curPos != std::string::npos )
    {
    std::string tmp = expFromKey.substr(curPosOld, curPos);

    if( tmp.find(" ") != std::string::npos)
      {
      tmp = tmp.substr(0, tmp.find(" "));
      }

    if( this->IsAValidKey(tmp) )
      {
      tempModKey = expFromKey.substr(0, curPos);
      stop = true;
      }
    else
      {
      if( curPosOld>1 )
        {
        if( expFromKey[curPos] != '-' )
          {
          tempModKey = expFromKey.substr(0, curPos);
          stop = true;
          }
        }
      else
        {
        tempModKey = expFromKey.substr(0, curPos);
        stop = true;
        }
      }

    curPosOld = curPos;
    curPos = expFromKey.find(" -", curPos);
    if(curPos!=std::string::npos)
      curPos += 2;
    }

  // Only if the key has values associated
  if( tempModKey.size() > 0 )
    {
    std::vector<itksys::String> spaceSplit = itksys::SystemTools::SplitString(tempModKey.substr(0, tempModKey.size()), ' ', false);

    // Remove " " string element
    for(unsigned int i=0; i<spaceSplit.size(); i++)
      {
      if( spaceSplit[i] == " ")
        {
        spaceSplit.erase(spaceSplit.begin()+i);
        i--;
        }
      }

    // Remove space at the beginning of the string and cast into std::vector<std::string>
    for(unsigned int i=0; i<spaceSplit.size(); i++)
      {
      while( spaceSplit[i].size()>0  && spaceSplit[i][0] == ' ' )
        {
        spaceSplit[i] = spaceSplit[i].substr(1, spaceSplit[i].size());
        }

      res.push_back(spaceSplit[i]);
      }
    }
  return res;
}

std::string
CommandLineParser::GetAttributAsString( const std::string & key, const std::vector<std::string> & exp )
{
  std::string res("");
  std::vector<std::string> values = this->GetAttribut( key, exp );

  if( values.size() == 0 )
    {
    return "";
    }
  else if( values.size() == 1 && values[0] == " " )
    {
    return "";
    }

  for( unsigned int i=0; i<values.size(); i++)
    {
    if( i<values.size()-1 )
      {
      res.append(values[i]);
      res.append(" ");
      }
    else
      {
      res.append(values[i]);
      }
    }
  return res;
}

std::string
CommandLineParser::GetAttributAsString( const std::string & key, const std::string & exp )
{
  std::string res("");
  std::vector<std::string> values = this->GetAttribut( key, exp );

  if( values.size() == 0 )
    {
    return "";
    }
  else if( values.size() == 1 && values[0] == " " )
    {
    return "";
    }

  for( unsigned int i=0; i<values.size(); i++)
    {
    if( i<values.size()-1 )
      {
      res.append(values[i]);
      res.append(" ");
      }
    else
      {
      res.append(values[i]);
      }
    }
  return res;
}


bool
CommandLineParser::IsAttributExists( const std::string key, const std::string & exp  )
{
  std::string keySpaced = key;
  std::string expSpaced = exp;
  // Add space to avoid troubles with key which starts by another one : -out and -outmax for example
  keySpaced.append(" ");
  expSpaced.append(" ");
  std::size_t found = expSpaced.find(keySpaced);
  return (found != std::string::npos);
}

bool
CommandLineParser::IsAttributExists( const std::string key, const std::vector<std::string> & exp  )
{
  for (std::vector<std::string>::const_iterator it = exp.begin(); it != exp.end(); ++it)
    {
    if (it->compare(key) == 0)
      return true;
    }
  return false;
}

std::vector<std::string>
CommandLineParser::GetKeyList( const std::vector<std::string> & exp  )
{
  std::vector<std::string> keyList;
  for (std::vector<std::string>::const_iterator it = exp.begin(); it != exp.end(); ++it)
    {
    if (it->find("-") == 0)
      {
      // Remove first character ('-')
      std::string key = it->substr(1,std::string::npos);
      if (this->IsAValidKey(key))
        {
        keyList.push_back(key);
        }
      }
    }

  return keyList;
}

std::vector<std::string>
CommandLineParser::GetKeyList( const std::string & exp  )
{
  std::vector<std::string> keyList;
  std::string cutExp(exp);
  std::size_t found = exp.find(" -");

  while( found != std::string::npos )
    {
    // Suppress everything before the key
    cutExp = cutExp.substr(found+2, exp.size());
    // Search the end of the key (a space)
    std::size_t foundSpace = cutExp.find(" ");
    std::string tmpKey = cutExp;
    if( foundSpace != std::string::npos )
      {
      tmpKey = cutExp.substr(0, foundSpace);
      }

    if( this->IsAValidKey(tmpKey) )
      {
      keyList.push_back( tmpKey );
      }


    // Search the next key (ie. " -")
    found = cutExp.find(" -");
    }

  return keyList;
}


bool
CommandLineParser::IsAValidKey( const std::string & foundKey )
{
  bool res = false;
  std::string tmp = foundKey;
  // make sure the tested key ends with a dot.
  // the starting dash should be already removed
  tmp.append(".");

  // To be a key, the string must be a series of groups separated by dots so that :
  // - each group has at least one character
  // - each group contains only alphanumeric characters (and lowercase)
  // - there is at least one group
  // The following regular expression says just that
  itksys::RegularExpression reg;
  reg.compile("^([a-z0-9]+\\.)+$");
  if( reg.find(tmp) )
    {
    res = true;
    }

  // a second test is performed to detect negative numbers (even in
  // scientific notation). Any key that matches the search pattern for a
  // number is considered not valid.
  itksys::RegularExpression regNum;
  regNum.compile("^([0-9]+(\\.[0-9]*)?([eE][+-]?[0-9]+)?)$");
  if( regNum.find(foundKey) )
    {
    res = false;
    }
  
  return res;
}


}
}


