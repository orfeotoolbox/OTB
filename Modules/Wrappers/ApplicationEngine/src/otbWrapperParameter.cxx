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


#include "otbWrapperParameter.h"


namespace otb
{
namespace Wrapper
{

// TODO: Constructor/destructor
Parameter::Parameter()
  : m_Name(), m_Description(), m_Key(), m_Mandatory(true), m_Active(false), m_UserValue(false), m_UserLevel(UserLevel_Basic), m_Role(Role_Input), m_Root(this)
{
}

/** Set/get the parameter name */
void Parameter::SetName(const std::string& name)
{
  m_Name = name;
  this->Modified();
}

const char* Parameter::GetName() const
{
  return this->m_Name.c_str();
}

/** Set/get the parameter description */
void Parameter::SetDescription(const std::string& description)
{
  m_Description = description;
  this->Modified();
}

const std::string& Parameter::GetDescription() const
{
  return m_Description;
}

/** Set/get the parameter key */
void Parameter::SetKey(const std::string& key)
{
  m_Key = key;
  this->Modified();
}
const char* Parameter::GetKey() const
{
  return this->m_Key.c_str();
}

/** Set the parameter Mandatory flag */
void Parameter::SetMandatory(bool flag)
{
  m_Mandatory = flag;
  this->Modified();
}

bool Parameter::GetMandatory() const
{
  return m_Mandatory;
}

void Parameter::MandatoryOn()
{
  SetMandatory(true);
}

void Parameter::MandatoryOff()
{
  SetMandatory(false);
}

/** Set the user access level */
void Parameter::SetUserLevel(const UserLevel level)
{
  m_UserLevel = level;
  this->Modified();
}

/** Parameter::Get the user access level */
UserLevel Parameter::GetUserLevel() const
{
  return m_UserLevel;
}

/** Set the parameter io type*/
void Parameter::SetRole(const Role role)
{
  m_Role = role;
  this->Modified();
}

/** Parameter::Get the user access level */
Role Parameter::GetRole() const
{
  return m_Role;
}

void Parameter::SetActive(bool flag)
{
  m_Active = flag;
  this->Modified();
}

bool Parameter::GetActive(bool recurseParents) const
{
  bool result = m_Active;
  if (recurseParents && !IsRoot())
  {
    result = result && GetRoot()->GetActive(recurseParents);
  }
  return result;
}


void Parameter::SetAutomaticValue(bool flag)
{
  this->SetUserValue(!flag);
}


bool Parameter::GetAutomaticValue() const
{
  return !m_UserValue;
}
/** Toogle ON the parameter AutomaticValue flag */
void Parameter::AutomaticValueOn()
{
  this->SetAutomaticValue(true);
}

/** Toogle OFF the parameter AutomaticValue flag */
void Parameter::AutomaticValueOff()
{
  this->SetAutomaticValue(false);
}

void Parameter::Reset()
{
}


bool Parameter::HasUserValue() const
{
  return this->HasValue() && m_UserValue;
}

void Parameter::SetUserValue(bool isUserValue)
{
  m_UserValue = isUserValue;
}

void Parameter::ClearValue()
{
  SetActive(false);
  Modified();
}


/** Set/Get the root of the current parameter (direct parent) */
void Parameter::SetRoot(const Parameter::Pointer root)
{
  m_Root = root.GetPointer();
}

const Parameter::Pointer Parameter::GetRoot() const
{
  return m_Root.GetPointer();
}

/** Is the parameter a root or a child of another param */
bool Parameter::IsRoot() const
{
  return (this == m_Root.GetPointer());
}

/** Add a child of this parameter when the param is a Group or a
    * choice
    */
void Parameter::AddChild(Parameter::Pointer child)
{
  m_ChildrenList.push_back(child);
}

/** Get the children pointer list : not const cause we need to
    * alterate the m_Active status and the m_IsCheckbox
    */
std::vector<Parameter::Pointer> Parameter::GetChildrenList()
{
  return m_ChildrenList;
}

void Parameter::TypeError(const std::string& target_type) const
{
  itkGenericExceptionMacro(<< "Cannot convert parameter '" << this->GetKey() << "'"
                           << " of type " << ParameterTypeToString(this->GetType()) << " to/from type " << target_type);
}

// Base implementation of To*/From* indicate that the parameter cannot be converted
// Derived classes override them if they can provide the conversion

int Parameter::ToInt() const
{
  TypeError("int");
}

double Parameter::ToFloat() const
{
  TypeError("double");
}

std::string Parameter::ToString() const
{
  TypeError("std::string");
}

std::vector<std::string> Parameter::ToStringList() const
{
  TypeError("std::vector<std::string>");
}

void Parameter::FromInt(int)
{
  TypeError("int");
}

void Parameter::FromFloat(double)
{
  TypeError("double");
}

void Parameter::FromString(const std::string&)
{
  TypeError("std::string");
}

void Parameter::FromStringList(const std::vector<std::string>&)
{
  TypeError("std::vector<std::string>");
}
}
}
