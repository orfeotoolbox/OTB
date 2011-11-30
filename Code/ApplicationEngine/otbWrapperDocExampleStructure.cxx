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
#ifndef __otbWrapperDocExampleStructure_cxx
#define __otbWrapperDocExampleStructure_cxx

#include "otbWrapperDocExampleStructure.h"


namespace otb
{
namespace Wrapper
{

DocExampleStructure::DocExampleStructure() : m_NbOfExamples(1), m_ExampleCommentList()
{
  m_ExampleCommentList.push_back("");
}

DocExampleStructure::~DocExampleStructure()
{
}

void
DocExampleStructure::AddParameter( const std::string key, const std::string name )
{
  ParamStructure mParam;
  mParam.m_Key = key;
  mParam.m_Name = name;
  mParam.m_Values = std::vector< std::string >(m_NbOfExamples, "");
  m_ParameterList.push_back( mParam );
}
  
void 
DocExampleStructure::SetParameterValue( const std::string key, const std::string value, unsigned int exId )
{
  bool found = false;
  unsigned int i=0;
  while ( i<m_ParameterList.size() && !found )
    {
    if( this->GetParameterKey(i) == key )
      {
      ParamStructure mParam = m_ParameterList[i];
      if( m_ParameterList[i].m_Values.size() < exId )
        {
        itkGenericExceptionMacro( "Only "<<m_ParameterList[i].m_Values.size()<<" values can be stored ("<<exId<<" asked).");
        }

      m_ParameterList[i].m_Values[exId] = value;
      
      found = true;
      }
    i++;
    }

  if( !found)
    itkGenericExceptionMacro( "No parameter with key \""<<key<<"\" found." );
}

DocExampleStructure::ParameterListType 
DocExampleStructure::GetParameterList()
{
  return m_ParameterList;
}

/** Get a specific parameter couple.*/
ParamStructure
DocExampleStructure::GetParameter( unsigned int i )
{
  if( m_ParameterList.size() < i )
    itkGenericExceptionMacro( "Index "<<i<<" out of range. (max index: "<<m_ParameterList.size()-1<<")." );
  
  return m_ParameterList[i];
}



std::string 
DocExampleStructure::GetParameterKey( unsigned int i )
{
  return this->GetParameter(i).m_Key;
}

std::string 
DocExampleStructure::GetParameterName( unsigned int i )
{
  return this->GetParameter(i).m_Name;
}

std::string 
DocExampleStructure::GetParameterValue( unsigned int i, unsigned int exId )
{
  return this->GetParameter(i).m_Values[exId];
}

void 
DocExampleStructure::SetApplicationName( const std::string name )
{
  m_ApplicationName = name;
}

std::string 
DocExampleStructure::GetApplicationName()
{
  return m_ApplicationName;
}

std::vector<std::string>  
DocExampleStructure::GetExampleCommentList()
{
  return m_ExampleCommentList;
}

std::string
DocExampleStructure::GetExampleComment( unsigned int i)
{
  if( m_ExampleCommentList.size() < i )
    itkGenericExceptionMacro( "Index "<<i<<" out of range. (max index: "<<m_ExampleCommentList.size()-1<<")." );
  
  return m_ExampleCommentList[i];
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
  m_NbOfExamples++;
  // Add a field for the values
  for( unsigned int i=0; i<m_ParameterList.size(); i++)
    m_ParameterList[i].m_Values.push_back("");

  return m_ExampleCommentList.size()-1;
}


std::string 
DocExampleStructure::GenerateCLExample( unsigned int exId )
{
  if( m_ApplicationName.empty() || m_ParameterList.size() == 0 )
    {
    return "";
    }
  
  itk::OStringStream oss;
  oss << "otbcli_" << m_ApplicationName << " ";
  
  for (unsigned int i=0; i< m_ParameterList.size(); i++)
    {
    if( this->GetParameterValue(i, exId) != "" )
      {
      oss<< "-" << this->GetParameterKey(i) << " " << this->GetParameterValue(i, exId) <<" ";
      }
    }
  
  std::string res = oss.str();
  
  // Supress last added space
  res.erase( res.size()-1, 1);
  
  return res;
}

std::string 
DocExampleStructure::GenerateCLExample()
{
  itk::OStringStream oss;
  for( unsigned int exId=0; exId<m_NbOfExamples; exId++)
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
  
  itk::OStringStream oss;
  oss << "<ul>";
  for (unsigned int i=0; i< m_ParameterList.size(); i++)
    {
    if( this->GetParameterValue(i, exId) != "" )
      {
      oss << "<li>";
      oss << "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">";
      oss << this->GetParameterName(i) << ": " << this->GetParameterValue(i, exId);
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
  itk::OStringStream oss;
  if( m_NbOfExamples>1 )
    oss << "<ul>";
  
  for( unsigned int exId=0; exId<m_NbOfExamples; exId++)
    {
    if( m_NbOfExamples>1 )
      oss << "<li>";
    if( !m_ExampleCommentList[exId].empty() )
      oss << m_ExampleCommentList[exId];
    oss << this->GenerateHtmlExample( exId );
    if( m_NbOfExamples>1 )
      oss << "</li>";
    }
  if( m_NbOfExamples>1 )
    oss << "</ul>";
  
  std::string res = oss.str();
  
  return res;
}

} // End namespace Wrapper
} // End namespace otb

#endif
