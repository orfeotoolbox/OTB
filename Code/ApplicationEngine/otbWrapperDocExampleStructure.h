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

#include "itkObject.h"
#include "itkObjectFactory.h"

#include <string>
#include <iostream>
#include <vector>
#include "itkMacro.h"
#include "otbConfigure.h"
#include "itkFixedArray.h"


namespace otb
{
namespace Wrapper
{

/** \class DocExampleStructure
 *  \brief This class is a structure that gathered the necessary
 *  element to generate an example (for CommandLine, python, Java
 *  ...).
 * User has to set the application name, the binary path and a list of
 *  key/value couple.
 */
class DocExampleStructure :
  public itk::Object
{
public:
  /** Standard class typedefs */
  typedef DocExampleStructure           Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standards macros */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(DocExampleStructure, itk::Object);

  typedef itk::FixedArray<std::string, 3> ThreeStringType;
  typedef std::vector<ThreeStringType> ParameterListType;

  /** Parameter list accessors. */
  /** Parameter list accessors : adding key and name */
  void AddParameter( const std::string key, const std::string name )
  {
    ThreeStringType mParam;
    mParam[0] = key;
    mParam[1] = name;
    mParam[2] = "";
    m_ParameterList.push_back( mParam );
  }
  
 void SetParameterValue( const std::string key, const std::string value )
  {
    bool found = false;
    unsigned int i=0;
    while ( i<m_ParameterList.size() && !found )
      {
      if( this->GetParameterKey(i) == key )
        {
        m_ParameterList[i][2] = value;
        found = true;
        }
      i++;
      }

    if( !found)
      itkGenericExceptionMacro( "No parameter with key "<<key<<" found." );
  }

 /** Get the parameter list. */
  ParameterListType GetParameterList()
  {
    return m_ParameterList;
  }

  /** Get a specific parameter couple.*/
  ThreeStringType GetParameter( unsigned int i )
  {
    if( m_ParameterList.size() <= i )
      itkGenericExceptionMacro( "Index "<<i<<" out of range. (max index: "<<m_ParameterList.size()<<")." );

    return m_ParameterList[i];
  }

  /** Get a specific parameter couple key.*/
  std::string GetParameterKey( unsigned int i )
  {
    return this->GetParameter(i)[0];
  }

 /** Get a specific parameter couple key.*/
  std::string GetParameterName( unsigned int i )
  {
    return this->GetParameter(i)[1];
  }

  /** Get a specific parameter couple value as string.*/
  std::string GetParameterValue( unsigned int i )
  {
    return  this->GetParameter(i)[2];
  }
  /** Set Application name. */
  void SetApplicationName( const std::string name )
  {
    m_ApplicationName = name;
  }
  /** Get Application name. */
  std::string GetApplicationName()
  {
    return m_ApplicationName;
  }

  /** Generation of the documentation for CommandLine. */
  std::string GenerateCLExample()
  {
    if( m_ApplicationName.empty() || m_ParameterList.size() == 0 )
      {
      return "";
      }
    
    itk::OStringStream oss;
    oss << "otbcli_" << m_ApplicationName << " ";   

    for (unsigned int i=0; i< m_ParameterList.size(); i++)
      {
      if( this->GetParameterValue(i) != "" )
        {
        oss<< "-" << this->GetParameterKey(i) << " " << this->GetParameterValue(i) <<" ";
        }
      }


    std::string res = oss.str();

    // Supress last added space
    res.erase( res.size()-1, 1);

    return res;
    
  }

  /** Generation of teh documentation for Qt. */
  std::string GenerateHtmlExample()
  {
    if( m_ApplicationName.empty() || m_ParameterList.size() == 0 )
      {
      return "";
      }

    itk::OStringStream oss;
    oss << "<ul>";
    for (unsigned int i=0; i< m_ParameterList.size(); i++)
      {
      if( this->GetParameterValue(i) != "" )
        {
        oss << "<li>";
        oss << "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">";
        oss << this->GetParameterName(i) << ": ";
        oss << this->GetParameterValue(i);
        
        oss << "</p>";
        oss << "</li>";
        }
      }
      oss << "</ul>";

    std::string res = oss.str();

    return res;
    
  }


protected:
  /** Constructor */
  DocExampleStructure(){}

  /** Destructor */
  virtual ~DocExampleStructure()
  {}

private:
  DocExampleStructure(const DocExampleStructure &); //purposely not implemented
  void operator =(const DocExampleStructure&); //purposely not implemented

  /** List of the application parameters. List of key/name/value couples. */
  ParameterListType m_ParameterList;
  /** List of the application parameter names. List of key/name couples. */
  ParameterListType m_ParameterNameList; // ???
  /** application name */
  std::string m_ApplicationName;


}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
