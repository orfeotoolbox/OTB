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
#ifndef __otbWrapperDocExampleStructure_h
#define __otbWrapperDocExampleStructure_h

#include <string>
#include <iostream>
#include <vector>
#include "itkMacro.h"
#include "otbConfigure.h"

namespace otb
{
namespace Wrapper
{

/** \class DocExampleStructure
 *  \brief This class represent a string parameter for the wrapper framework
 */
class DocExampleStructure
{
public:
  typedef std::pair<std::string, std::string> PairType;
  typedef std::vector<PairType> ParameterListType;


  void AddParameter( const std::string key, const std::string value )
  {
    PairType myPair;
    myPair.first = key;
    myPair.second = value;
    this->AddParameter( myPair );
  }
  
  void AddParameter( const PairType myPair )
  {
    m_ParameterList.push_back( myPair );
  }

  PairType GetParameter( unsigned int i )
  {
    if( m_ParameterList.size() <= i )
      itkGenericExceptionMacro( "Index "<<i<<" out of range. (max index: "<<m_ParameterList.size()<<")." );

    return m_ParameterList[i];
  }

  ParameterListType GetParameterList()
  {
    return m_ParameterList;
  }

  std::string GetParameterKey( unsigned int i )
  {
    return this->GetParameter(i).first;
  }

  int GetParameterValueAsInt( unsigned int i )
  {
    return atoi(this->GetParameter(i).second.c_str());
  }

 float GetParameterValueAsFloat( unsigned int i )
  {
    return  atoi(this->GetParameter(i).second.c_str());
  }

  std::string GetParameterValueAsString( unsigned int i )
  {
    return  this->GetParameter(i).second;
  }

  void SetApplicationName( const std::string name )
  {
    m_ApplicationName = name;
  }

  std::string GetApplicationName()
  {
    return m_ApplicationName;
  }

  void SetBinPath( const std::vector<std::string> bPath )
  {
    m_BinPath = bPath;
  }
  
  void AddBinPath( std::string myPath )
  {
    m_BinPath.push_back( myPath );
  }

  std::vector<std::string> GetBinPath()
  {
    return m_BinPath;
  }

  std::string GenerateCLExample()
  {
    if( m_ApplicationName.empty() || m_BinPath.size() == 0 ||  m_ParameterList.size() == 0 )
      {
      itkGenericExceptionMacro( "Missing attributs to generate the example." );
      }
    
    itk::OStringStream oss;
    oss << OTB_CONFIG << "/bin/otbApplicationLauncherCommandLine ";
    
    for (unsigned int i=0; i< m_BinPath.size(); i++)
      {
      oss << m_BinPath[i]<< " ";
      }

    for (unsigned int i=0; i< m_ParameterList.size(); i++)
      {
      oss << "-" << m_ParameterList[i].first << " " << m_ParameterList[i].second <<" ";
      }


    std::string res = oss.str();

    // Supress last added space
    res.erase( res.size()-1, 1);

    return res;
    
  }

  /** Constructor */
  DocExampleStructure()
  {}

  /** Destructor */
  virtual ~DocExampleStructure()
  {}

protected:


private:
  DocExampleStructure(const DocExampleStructure &); //purposely not implemented
  void operator =(const DocExampleStructure&); //purposely not implemented

  ParameterListType m_ParameterList;
  std::string m_ApplicationName;
  std::vector<std::string> m_BinPath;


}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
