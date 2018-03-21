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

#ifndef otbWrapperExpDocExampleStructure_h
#define otbWrapperExpDocExampleStructure_h

#include "itkObject.h"
#include "itkObjectFactory.h"

#include <string>
#include <iostream>
#include <vector>
#include "otbConfigure.h"
#include "itkFixedArray.h"
#include "OTBApplicationEngineExport.h"

namespace otb
{
namespace WrapperExp
{

/** \class DocExampleStructure
 *  \brief This class is a structure that gathered the necessary
 *  element to generate an example (for CommandLine, python, Java
 *  ...).
 * User has to set the application name, the binary path and a list of
 *  key/value couple.
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT DocExampleStructure :
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

  typedef std::pair<std::string, std::string> ParameterType;
  typedef std::vector<ParameterType> ParametersVectorType;
  typedef std::vector<ParametersVectorType> ParametersVectorOfVectorType;

  /** Get the number of examples */
  itkGetMacro(NbOfExamples, unsigned int);

  /** Parameter list accessors. */
  /** Parameter list accessors : adding key and name */
  void AddParameter( const std::string key, const std::string name, unsigned int exId = 0);

  /** Get the parameter list. */
  const ParametersVectorOfVectorType & GetParameterList() const 
    {
    return m_ParameterList;
    }

  /** Get number of parameters */
  unsigned int GetNumberOfParameters(unsigned int exId = 0) const
    {
    if ( exId >= m_ParameterList.size() )
      //log
    return m_ParameterList[exId].size();
    }

  /** Get a specific parameter couple key.*/
  const std::string & GetParameterKey( unsigned int i, unsigned int exId = 0 ) const
    {
    if ( exId >= m_ParameterList.size() )
      //log
    if ( i >= m_ParameterList[exId].size() )
      //log
    return m_ParameterList[exId][i].first;
    }

  /** Get a specific parameter couple value as string.*/
  const std::string & GetParameterValue( unsigned int i, unsigned int exId=0 ) const
    {
    if ( exId >= m_ParameterList.size() )
      //log
    if ( i >= m_ParameterList[exId].size() )
      //log
    return m_ParameterList[exId][i].second;
    }

  /** Set Application name. */
  void SetApplicationName( const std::string & name )
    {
    m_ApplicationName = name;
    }

  /** Get Application name. */
  const std::string & GetApplicationName() const
    {
    return m_ApplicationName;
    }

  /** Get the example comment list */
  const std::vector<std::string> & GetExampleCommentList() const
    {
    return m_ExampleCommentList;
    }

  /** Get one example comment */
  const std::string & GetExampleComment( unsigned int i) const
    {
    return m_ExampleCommentList.at(i);
    }

  /** Set one example comment */
  void SetExampleComment( const std::string & comm, unsigned int i);

  /** Add an example using comment. Return the index of the new example.*/
  unsigned int  AddExample( const std::string & comm = "");

  /** Generation of the documentation for CommandLine for one specific
  * example. */
  std::string GenerateCLExample( unsigned int exId );

 /** Generation of the documentation for CommandLine. */
  std::string GenerateCLExample();

  /** Generation of the documentation for Qt for one specific
  * example. */
  std::string GenerateHtmlExample( unsigned int exId );

  /** Generation of the documentation for Qt. */
  std::string GenerateHtmlExample();

protected:
  /** Constructor */
  DocExampleStructure();
  /** Destructor */
  ~DocExampleStructure() override = default ;

private:
  DocExampleStructure(const DocExampleStructure &); //purposely not implemented
  void operator =(const DocExampleStructure&); //purposely not implemented

  /** List of the application parameters. List of key/name/value couples. */
  ParametersVectorOfVectorType m_ParameterList;

  /** Application name */
  std::string m_ApplicationName;

  /** Example comments */
  std::vector<std::string> m_ExampleCommentList;

  /** Stores the number of example. */
  unsigned int m_NbOfExamples;

}; // End class Parameter

} // End namespace WrapperExp
} // End namespace otb

#endif
