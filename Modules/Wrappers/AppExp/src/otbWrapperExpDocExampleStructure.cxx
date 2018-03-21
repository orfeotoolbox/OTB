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

#include "otbWrapperExpDocExampleStructure.h"
#include "otbStringToHTML.h"

namespace otb
{
namespace WrapperExp
{

DocExampleStructure::DocExampleStructure() : m_ParameterList(), m_ApplicationName(""), m_ExampleCommentList()
{
  m_ExampleCommentList.push_back("");
}

void
DocExampleStructure::AddParameter( const std::string key, const std::string value, unsigned int exId)
{
  if( m_ParameterList.size() < exId+1 )
    {
    m_ParameterList.resize(exId+1);
    }

  m_ParameterList[exId].emplace_back(std::make_pair(key, value));
}

void
DocExampleStructure::SetExampleComment( const std::string & comm, unsigned int i )
{
  if( m_ExampleCommentList.size() < i )
    itkGenericExceptionMacro( "Index "<<i<<" out of range. (max index: "<<m_ExampleCommentList.size()-1<<")." );

  this->m_ExampleCommentList[i] = comm;
}


unsigned int
DocExampleStructure::AddExample( const std::string & comm )
{
  m_ExampleCommentList.push_back( comm );

  m_ParameterList.push_back(ParametersVectorType());

  return m_ExampleCommentList.size()-1;
}


std::string
DocExampleStructure::GenerateCLExample( unsigned int exId )
{
  std::string res("None");
  if( m_ApplicationName.empty() || m_ParameterList.size() == 0 )
    {
    return res;
    }
  else if ( m_ParameterList.at(exId).empty() )
    return res;

  std::ostringstream oss;
  oss << "otbcli_" << m_ApplicationName << " ";

  for ( const auto & param : m_ParameterList[exId] )
    {
    if( param.second != "" )
      {
      oss<< "-" << param.first << " " << param.second <<" ";
      }
    }

  res = oss.str();

  // Suppress last added space
  res.erase( res.size()-1, 1);

  return res;
}

std::string
DocExampleStructure::GenerateCLExample()
{
  std::ostringstream oss;
  for( unsigned int exId=0; exId<m_ExampleCommentList.size(); exId++)
    {
    if( !m_ExampleCommentList[exId].empty() )
      oss << m_ExampleCommentList[exId]<< std::endl;
    oss << this->GenerateCLExample( exId );
    oss << std::endl;
    }

  std::string res = oss.str();
  return res;
}


std::string
DocExampleStructure::GenerateHtmlExample( unsigned int exId )
{
  if( m_ApplicationName.empty() || m_ParameterList.size() == 0 )
    {
    return "";
    }

  std::ostringstream oss;
  oss << "<ul>";

  for (ParametersVectorType::const_iterator it = m_ParameterList.at(exId).begin();
       it != m_ParameterList.at(exId).end(); ++it)
    {
    if( it->second != "" )
      {
      oss << "<li>";
      oss << "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">";
      oss << it->first << ": " << otb::StringToHTML(it->second);
      oss << "</p>";
      oss << "</li>";
      }
    }
  oss << "</ul>";

  std::string res = oss.str();

  return res;

}


std::string
DocExampleStructure::GenerateHtmlExample()
{
  std::ostringstream oss;
  unsigned int nbOfExamples = m_ExampleCommentList.size();
  if( nbOfExamples>1 )
    oss << "<ul>";

  for( unsigned int exId=0; exId<nbOfExamples; exId++)
    {
    if( nbOfExamples>1 )
      oss << "<li>";
    if( !m_ExampleCommentList[exId].empty() )
      oss << otb::StringToHTML(m_ExampleCommentList[exId]);
    oss << this->GenerateHtmlExample( exId );
    if( nbOfExamples>1 )
      oss << "</li>";
    }
  if( nbOfExamples>1 )
    oss << "</ul>";

  std::string res = oss.str();

  return res;
}

} // End namespace WrapperExp
} // End namespace otb
