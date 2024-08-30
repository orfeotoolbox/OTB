/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperChoiceParameter_h
#define otbWrapperChoiceParameter_h

#include "otbWrapperParameterGroup.h"
#include <string>

namespace otb
{
namespace Wrapper
{

/** \class ChoiceParameter
 *  \brief This class represent a multiple choice parameter for the wrapper framework
 *
 *  Represents an enumerated parameter.
 *  The different values of the enumeration can be associated with a parameter
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT ChoiceParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef ChoiceParameter               Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(ChoiceParameter, Parameter);

  /** Add a value to the choice */
  void AddChoice(std::string choicekey, std::string choiceName);

  /** Get the key of a specific choice value */
  std::string GetChoiceKey(int i) const;

  /** Get the list of the different choice keys */
  std::vector<std::string> GetChoiceKeys();

  /** Get the long name of a specific choice value */
  std::string GetChoiceName(int i);

  /** Get the list of the different choice keys */
  std::vector<std::string> GetChoiceNames();

  /** Get the ParameterGroup associated to a choice value */
  ParameterGroup::Pointer GetChoiceParameterGroupByIndex(int i);

  /** Get the ParameterGroup associated to a choice value */
  ParameterGroup::Pointer GetChoiceParameterGroupByKey(std::string choiceKey);

  /** Get all parameters that are child of this choice parameter */
  std::vector<std::string> GetParametersKeys();

  /** Get the number of available choice */
  unsigned int GetNbChoices(void);

  /** Set choice value */
  virtual void SetValue(unsigned int v);

  /** Set choice value by its key */
  virtual void SetValue(std::string choiceKey);

  /** Return any value */
  virtual unsigned int GetValue() const;

  bool HasValue() const override;
  void ClearValue() override;

  ParameterType GetType() const override;

  int  ToInt() const override;
  void FromInt(int value) override;

  std::string ToString() const override;
  void FromString(const std::string& value) override;

protected:
  /** Constructor */
  ChoiceParameter();

  /** Destructor */
  ~ChoiceParameter() override;

  struct Choice
  {
    Choice()
    {
    }

    std::string             m_Key;
    std::string             m_Name;
    ParameterGroup::Pointer m_AssociatedParameter;
  };

  typedef std::vector<Choice> ChoiceList;
  ChoiceList                  m_ChoiceList;

  unsigned int m_CurrentChoice;

private:
  ChoiceParameter(const ChoiceParameter&) = delete;
  void operator=(const ChoiceParameter&) = delete;

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
