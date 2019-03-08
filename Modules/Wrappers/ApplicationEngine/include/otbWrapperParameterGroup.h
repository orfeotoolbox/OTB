/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperParameterGroup_h
#define otbWrapperParameterGroup_h

#include "itkObject.h"
#include "otbWrapperParameter.h"
#include <vector>
#include <string>

namespace otb
{
namespace Wrapper
{

/**
 * \class Group
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT ParameterGroup
  : public Parameter
{
public:
  typedef ParameterGroup                       Self;
  typedef Parameter                            Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(ParameterList, Parameter);

  void AddParameter(Parameter::Pointer p);

  /** Method to substitute a parameter in a group. 
   *  The function returns true on success, false on failure */
  bool ReplaceParameter(std::string &key, Parameter::Pointer p);

  /** Add a new choice value to an existing choice parameter */
  void AddChoice(std::string paramKey, std::string paramName);

  /** Remove choices made in ListViewParameter widget*/
  void ClearChoices(std::string paramKey);

  /** Get the choices made in a ListView Parameter widget*/
  std::vector<int> GetSelectedItems(std::string paramKey);

  /** Add a new parameter to the parameter group
   * the parent key of paramKey can be the path to a parameter group
   * or the path to a choice value */
  void AddParameter(ParameterType type, std::string paramKey, std::string paramName);

  Parameter::Pointer GetParameterByIndex(unsigned int i, bool follow=true);

  Parameter::Pointer GetParameterByKey(std::string name, bool follow=true);

  /** rashad: Add xml parameters eg: -inxml -outxml */
  void AddInXMLParameter();

  void AddOutXMLParameter();

  void Clear()
  {
    m_ParameterList.clear();
  }

  /** Get the parameter type as string from its ParameterType enum
   * For example if type of parameter is ParameterType_InputImage this
   * function return the string InputImage */
  std::string GetParameterTypeAsString(ParameterType paramType);

  /* Get the parameter type from its string version of ParameterType enum */
  ParameterType GetParameterTypeFromString(const std::string& paramType);

  unsigned int GetNumberOfParameters();

  std::vector<std::string> GetParametersKeys(bool recursive = true);

  // Always has value
  bool HasValue() const override
  {
    return true;
  }

  /** Resolve potential proxy parameters by following their targets until
   *  a non-proxy parameter. It will detect cycles and report an error */
  static Parameter* ResolveParameter(Parameter *param);

  virtual ParameterType GetType() const override
  {
    return ParameterType_Group;
  }

protected:
  ParameterGroup();
  ~ParameterGroup() override;

  typedef std::vector<Parameter::Pointer> ParameterListType;
  ParameterListType m_ParameterList;

private:
  ParameterGroup(const ParameterGroup &) = delete;
  void operator =(const ParameterGroup&) = delete;

};

}
}

#endif
