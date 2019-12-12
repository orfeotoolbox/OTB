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

#ifndef otbWrapperParameter_h
#define otbWrapperParameter_h


#include "OTBApplicationEngineExport.h"
#include "otbMacro.h"
#include "otbWrapperTypes.h"


#include <itkObjectFactory.h>


#include <string>


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
class OTBApplicationEngine_EXPORT Parameter : public itk::Object
{
public:
  /** Standard class typedef */
  typedef Parameter                     Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** RTTI support */
  itkTypeMacro(Parameter, itk::Object);

  /** Set/get the parameter name */
  virtual void        SetName(const std::string&);
  virtual const char* GetName() const;

  /** Set/get the parameter description */
  virtual void               SetDescription(const std::string&);
  virtual const std::string& GetDescription() const;

  /** Set/get the parameter key */
  virtual void        SetKey(const std::string&);
  virtual const char* GetKey() const;

  /** Set the parameter Active flag */
  virtual void SetActive(bool flag);
  bool GetActive(bool recurseParents = false) const;

  /** Set the parameter Mandatory flag */
  virtual void SetMandatory(bool flag);
  virtual bool GetMandatory() const;
  virtual void MandatoryOn();
  virtual void MandatoryOff();

  /** Set the parameter AutomaticValue flag (which is the opposite of UserValue)*/
  virtual void SetAutomaticValue(bool flag);

  /** Get the parameter AutomaticValue flag */
  virtual bool GetAutomaticValue() const;

  /** Toogle ON the parameter AutomaticValue flag */
  void AutomaticValueOn();

  /** Toogle OFF the parameter AutomaticValue flag */
  void AutomaticValueOff();

  /** Set the user access level */
  virtual void SetUserLevel(const UserLevel level);

  /** Get the user access level */
  virtual UserLevel GetUserLevel() const;

  /** Set the parameter io type*/
  virtual void SetRole(const Role role);

  /** Get the user access level */
  virtual Role GetRole() const;

  /** Reset to the the default value. Default implementation does
   * nothing
   */
  virtual void Reset();

  virtual bool HasValue() const = 0;

  virtual bool HasUserValue() const;

  virtual void SetUserValue(bool isUserValue);

  virtual void ClearValue();

  /** Set/Get the root of the current parameter (direct parent) */
  virtual void SetRoot(const Parameter::Pointer root);

  virtual const Parameter::Pointer GetRoot() const;

  /** Is the parameter a root or a child of another param */
  virtual bool IsRoot() const;

  /** Add a child of this parameter when the param is a Group or a
    * choice
    */
  virtual void AddChild(Parameter::Pointer child);


  /** Get the children pointer list : not const cause we need to
    * alterate the m_Active status and the m_IsCheckbox
    */
  virtual std::vector<Parameter::Pointer> GetChildrenList();

  /** Get the dynamic type as declared in WrapperTypes.h */
  virtual ParameterType GetType() const = 0;

  /** Error raising function to indicate a type conversion error */
  [[noreturn]] void TypeError(const std::string& target_type) const;

  /** Parameter conversion functions. They are used by WrapperApplication
   * to provide functions like SetParameterInt, GetParameterString, etc.
   */
  virtual int                      ToInt() const;
  virtual float                    ToFloat() const;
  virtual double                   ToDouble() const;
  virtual std::string              ToString() const;
  virtual std::vector<std::string> ToStringList() const;

  virtual void FromInt(int);
  virtual void FromFloat(float);
  virtual void FromDouble(double);
  virtual void FromString(const std::string&);
  virtual void FromStringList(const std::vector<std::string>&);

protected:
  /** Constructor */
  Parameter();

  /** Name of the parameter */
  std::string m_Name;

  /** Description of the parameter */
  std::string m_Description;

  /** Key of the parameter */
  std::string m_Key;

  /** True if the parameter is mandatory */
  bool m_Mandatory;

  /** True if activated (a mandatory parameter is always active) */
  bool m_Active;

  /** True if the value is set in user mode (otherwise, it is an automatic value)*/
  bool m_UserValue;

  UserLevel m_UserLevel;

  /** Default iotype mode */
  Role m_Role;

  /** List of parents Parameters */
  itk::WeakPointer<Parameter> m_Root;

  /** List of children parameters */
  std::vector<Parameter::Pointer> m_ChildrenList;

private:
  Parameter(const Parameter&) = delete;
  void operator=(const Parameter&) = delete;

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
