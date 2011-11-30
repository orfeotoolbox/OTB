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

/** \class ParamStructure
 *  \brief This class is a structure that gathered information on a parameter.
 *  It stores the parameter jey, name and a list of possible
 *  values. Each value is an element of a specific example.
 *  ...).
 */
class ParamStructure
{
public :
  ParamStructure() : m_Key(""), m_Name(""), m_Values()
  {
    m_Values.push_back("");
  };
  virtual ~ParamStructure(){};

  std::string m_Key;
  std::string m_Name;
  std::vector< std::string > m_Values;

  ParamStructure(const ParamStructure & param)
  {
    this->m_Key = param.m_Key;
    this->m_Name = param.m_Name;
    this->m_Values =  param.m_Values;
  };

 void operator =(const ParamStructure & param)
  {
    this->m_Key = param.m_Key;
    this->m_Name = param.m_Name;
    this->m_Values =  param.m_Values;
  }
};

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

  typedef std::vector<ParamStructure> ParameterListType;

  /** Parameter list accessors. */
  /** Parameter list accessors : adding key and name */
  void AddParameter( const std::string key, const std::string name );

  /** et a value to a parameter. Only parmeters with value will be
  * used in the example. */
  void SetParameterValue( const std::string key, const std::string value, unsigned int exId=0 );

  /** Get the parameter list. */
  ParameterListType GetParameterList();

  /** Get a specific parameter couple.*/
  ParamStructure GetParameter( unsigned int i );

  /** Get a specific parameter couple key.*/
  std::string GetParameterKey( unsigned int i );

  /** Get a specific parameter couple key.*/
  std::string GetParameterName( unsigned int i );

  /** Get a specific parameter couple value as string.*/
  std::string GetParameterValue( unsigned int i, unsigned int exId=0 );

  /** Set Application name. */
  void SetApplicationName( const std::string name );

  /** Get Application name. */
  std::string GetApplicationName();

  /** Get the example comment list */
  std::vector<std::string> GetExampleCommentList();

  /** Get one example comment */
  std::string GetExampleComment( unsigned int i);

  /** Set one example comment */
  void SetExampleComment( const std::string & comm, unsigned int i);

  /** Add an example using comment. Retrun the index of the new example.*/
  unsigned int  AddExample( const std::string & comm = "");

  /** Generation of the documentation for CommandLine for one specific
  * example. */
  std::string GenerateCLExample( unsigned int exId );

 /** Generation of the documentation for CommandLine. */
  std::string GenerateCLExample();

  /** Generation of teh documentation for Qt for one specific
  * example. */
  std::string GenerateHtmlExample( unsigned int exId );

  /** Generation of teh documentation for Qt. */
  std::string GenerateHtmlExample();

protected:
  /** Constructor */
  DocExampleStructure();
  /** Destructor */
  virtual ~DocExampleStructure();

private:
  DocExampleStructure(const DocExampleStructure &); //purposely not implemented
  void operator =(const DocExampleStructure&); //purposely not implemented

  /** List of the application parameters. List of key/name/value couples. */
  ParameterListType m_ParameterList;
  /** Application name */
  std::string m_ApplicationName;
  /** Example comments */
  std::vector<std::string> m_ExampleCommentList;
  /** Stores the number of example. */
  unsigned int m_NbOfExamples;

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
