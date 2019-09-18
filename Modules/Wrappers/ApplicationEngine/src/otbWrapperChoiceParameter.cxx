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

#include "otbWrapperChoiceParameter.h"

namespace otb
{
namespace Wrapper
{

ChoiceParameter::ChoiceParameter() : m_CurrentChoice(0)
{
}

ChoiceParameter::~ChoiceParameter()
{
}

void ChoiceParameter::AddChoice(std::string choicekey, std::string choiceName)
{
  Choice choice;
  choice.m_Key                 = choicekey;
  choice.m_Name                = choiceName;
  choice.m_AssociatedParameter = ParameterGroup::New();
  choice.m_AssociatedParameter->SetName(choiceName);
  choice.m_AssociatedParameter->SetRoot(this);
  choice.m_AssociatedParameter->SetKey(choicekey);
  choice.m_AssociatedParameter->SetMandatory(false);
  m_ChoiceList.push_back(choice);

  // check if the new choice matches the m_CurrentChoice : if so the group should be active.
  if (m_CurrentChoice == (m_ChoiceList.size() - 1))
  {
    m_ChoiceList[m_CurrentChoice].m_AssociatedParameter->SetActive(true);
  }

  // Add the associated parameter as a child
  // in order to not have a gap in the children hierarchy
  this->AddChild(choice.m_AssociatedParameter.GetPointer());
}

std::string ChoiceParameter::GetChoiceKey(int i) const
{
  return m_ChoiceList[i].m_Key;
}


std::vector<std::string> ChoiceParameter::GetChoiceKeys()
{
  std::vector<std::string> ret;
  ChoiceList::iterator     it = m_ChoiceList.begin();

  for (it = m_ChoiceList.begin(); it != m_ChoiceList.end(); ++it)
  {
    ret.push_back(it->m_Key);
  }

  return ret;
}


std::string ChoiceParameter::GetChoiceName(int i)
{
  return m_ChoiceList[i].m_Name;
}


std::vector<std::string> ChoiceParameter::GetChoiceNames()
{
  std::vector<std::string> ret;
  ChoiceList::iterator     it = m_ChoiceList.begin();

  for (it = m_ChoiceList.begin(); it != m_ChoiceList.end(); ++it)
  {
    ret.push_back(it->m_Name);
  }
  return ret;
}


ParameterGroup::Pointer ChoiceParameter::GetChoiceParameterGroupByIndex(int i)
{
  return m_ChoiceList[i].m_AssociatedParameter;
}

ParameterGroup::Pointer ChoiceParameter::GetChoiceParameterGroupByKey(std::string choiceKey)
{
  ChoiceList::iterator it = m_ChoiceList.begin();

  for (it = m_ChoiceList.begin(); it != m_ChoiceList.end(); ++it)
  {
    if (it->m_Key == choiceKey)
    {
      return it->m_AssociatedParameter;
    }
  }

  itkExceptionMacro(<< "Cannot find choice key: '" << choiceKey << "'");
}

unsigned int ChoiceParameter::GetNbChoices(void)
{
  return m_ChoiceList.size();
}

void ChoiceParameter::SetValue(unsigned int v)
{
  m_CurrentChoice = v;
  SetActive(true);
  // update the active flag in sub parameters
  for (unsigned int i = 0; i < m_ChoiceList.size(); i++)
  {
    if (m_ChoiceList[i].m_AssociatedParameter)
    {
      if (i == m_CurrentChoice)
      {
        if (!m_ChoiceList[i].m_AssociatedParameter->GetActive())
        {
          m_ChoiceList[i].m_AssociatedParameter->SetActive(true);
        }
      }
      else
      {
        if (m_ChoiceList[i].m_AssociatedParameter->GetActive())
        {
          m_ChoiceList[i].m_AssociatedParameter->SetActive(false);
        }
      }
    }
  }

  // Call Modified();
  this->Modified();
}

void ChoiceParameter::SetValue(std::string choiceKey)
{
  ChoiceList::iterator it = m_ChoiceList.begin();

  unsigned int i = 0;
  for (it = m_ChoiceList.begin(); it != m_ChoiceList.end(); ++it)
  {
    if (it->m_Key == choiceKey)
    {
      SetValue(i);
      return;
    }
    ++i;
  }

  itkExceptionMacro(<< "Invalid parameter value '" << choiceKey << "'"
                    << " given for parameter '-" << this->GetKey() << "'");
}

unsigned int ChoiceParameter::GetValue() const
{
  return m_CurrentChoice;
}

/** Return any value */
std::vector<std::string> ChoiceParameter::GetParametersKeys()
{
  std::vector<std::string> parameters;

  ChoiceList::iterator cit = m_ChoiceList.begin();

  for (cit = m_ChoiceList.begin(); cit != m_ChoiceList.end(); ++cit)
  {
    if (cit->m_AssociatedParameter)
    {
      std::vector<std::string> subparams = cit->m_AssociatedParameter->GetParametersKeys();
      for (std::vector<std::string>::const_iterator it = subparams.begin(); it != subparams.end(); ++it)
      {
        parameters.push_back(cit->m_Key + "." + *it);
      }
    }
  }
  return parameters;
}

bool ChoiceParameter::HasValue() const
{
  return !m_ChoiceList.empty();
}

void ChoiceParameter::ClearValue()
{
  // Same as constructor init value
  // Note that this may be invalid if HasValue() == false
  m_CurrentChoice = 0;
}

ParameterType ChoiceParameter::GetType() const
{
  return ParameterType_Choice;
}

int ChoiceParameter::ToInt() const
{
  return this->GetValue();
}

void ChoiceParameter::FromInt(int value)
{
  SetValue(value);
}

std::string ChoiceParameter::ToString() const
{
  if (HasValue())
  {
    std::string  choiceKey    = GetChoiceKey(GetValue());
    const size_t lastPointPos = choiceKey.find_last_of('.');
    if (lastPointPos != std::string::npos)
    {
      return choiceKey.substr(lastPointPos);
    }
    else
    {
      return choiceKey;
    }
  }
  else
  {
    return "";
  }
}

void ChoiceParameter::FromString(const std::string& value)
{
  SetValue(value);
}
}
}
