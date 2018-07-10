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

#ifndef otbWrapperParameter_h
#define otbWrapperParameter_h

#include "otbMacro.h"
#include "itkObjectFactory.h"

#include "otbWrapperTypes.h"
#include "OTBApplicationEngineExport.h"

namespace otb
{
namespace Wrapper
{

/** \class Parameter
 *  \brief This class represent a parameter for the wrapper framework
 *  This class is a high level class representing a parameter for the
 *  wrapper framework. It should be subclassed to represent different
 *  kinds of parameters
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT Parameter
  : public itk::Object
{
public:
  /** Standard class typedef */
  typedef Parameter                     Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** RTTI support */
  itkTypeMacro(Parameter, itk::Object);

  /** Set the parameter name */
  itkSetStringMacro(Name);

  /** Get the parameter name */
  itkGetStringMacro(Name);

  /** Set the parameter description */
  itkSetStringMacro(Description);

  /** Get the parameter description */
  itkGetConstReferenceMacro( Description, std::string );

  /** Set the parameter key */
  itkSetStringMacro(Key);

  /** Get the parameter key */
  itkGetStringMacro(Key);

  /** Set the parameter Active flag */
  itkSetMacro(Active, bool);

  /** Get the parameter Active flag */
  bool GetActive(bool recurseParents = false) const
  {
    bool result = m_Active;
    if (recurseParents && !IsRoot())
      {
      result = result && GetRoot()->GetActive(recurseParents);
      }
    return result;
  }

  /** Set the parameter mandatory flag */
  itkSetMacro(Mandatory, bool);

  /** Get the parameter mandatory flag */
  itkGetConstMacro(Mandatory, bool);

  /** Toogle the parameter mandatory flag */
  itkBooleanMacro(Mandatory);

  /** Set the parameter AutomaticValue flag (which is the opposite of UserValue)*/
  virtual void SetAutomaticValue(bool flag)
    {
    this->SetUserValue(!flag);
    }

  /** Get the parameter AutomaticValue flag */
  virtual bool GetAutomaticValue() const
    {
    return !m_UserValue;
    }

  /** Toogle ON the parameter AutomaticValue flag */
  void AutomaticValueOn()
    {
    this->SetAutomaticValue(true);
    }

  /** Toogle OFF the parameter AutomaticValue flag */
  void AutomaticValueOff()
    {
    this->SetAutomaticValue(false);
    }

  /** Set the user access level */
  itkSetEnumMacro(UserLevel, UserLevel);

  /** Get the user access level */
  itkGetEnumMacro(UserLevel, UserLevel);

  /** Set the parameter io type*/
  itkSetEnumMacro(Role, Role);

  /** Get the user access level */
  itkGetEnumMacro(Role, Role);

  /** Reset to the the default value. Default implementation does
   * nothing
   */
  virtual void Reset()
  {
  }

  virtual bool HasValue() const = 0;

  virtual bool HasUserValue() const
  {
    return this->HasValue() && m_UserValue;
  }

  virtual void SetUserValue(bool isUserValue)
  {
    m_UserValue = isUserValue;
  }

  virtual void ClearValue()
  {
    SetActive( false );
    Modified();
  }

  /** Set/Get the root of the current parameter (direct parent) */
  virtual void SetRoot(const Parameter::Pointer  root)
  {
    m_Root = root.GetPointer();
  }

  virtual const Parameter::Pointer GetRoot() const
  {
    return m_Root.GetPointer();
  }

  /** Is the parameter a root or a child of another param */
  virtual bool IsRoot() const
  {
    return (this == m_Root.GetPointer());
  }

  /** Add a child of this parameter when the param is a Group or a
    * choice
    */
  virtual void AddChild(Parameter::Pointer child)
  {
    m_ChildrenList.push_back(child);
  }

  /** Get the children pointer list : not const cause we need to
    * alterate the m_Active status and the m_IsCheckbox
    */
  virtual std::vector<Parameter::Pointer > GetChildrenList()
  {
    return m_ChildrenList;
  }

protected:
  /** Constructor */
  Parameter() :
    m_Name( "" ),
    m_Description( "" ),
    m_Key( "" ),
    m_Mandatory( true ),
    m_Active( false ),
    m_UserValue( false ),
    m_UserLevel( UserLevel_Basic ),
    m_Role( Role_Input ),
    m_Root( this )
  {}

  /** Destructor */
  ~Parameter() override {}

  /** Name of the parameter */
  std::string                        m_Name;

  /** Description of the parameter */
  std::string                        m_Description;

  /** Key of the parameter */
  std::string                        m_Key;

  /** True if the parameter is mandatory */
  bool                               m_Mandatory;

  /** True if activated (a mandatory parameter is always active) */
  bool                               m_Active;

  /** True if the value is set in user mode (otherwise, it is an automatic value)*/
  bool                               m_UserValue;

  UserLevel                          m_UserLevel;

  /** Default iotype mode */
  Role                               m_Role;

  /** List of parents Parameters */
  itk::WeakPointer<Parameter>        m_Root;

  /** List of children parameters */
  std::vector<Parameter::Pointer >   m_ChildrenList;

private:
  Parameter(const Parameter &) = delete;
  void operator =(const Parameter&) = delete;

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
