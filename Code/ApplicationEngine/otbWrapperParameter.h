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
#ifndef __otbWrapperParameter_h
#define __otbWrapperParameter_h

#include "otbMacro.h"
#include "itkObject.h"
#include "itkObjectFactory.h"

#include "otbWrapperTypes.h"

namespace otb
{
namespace Wrapper
{
enum DefaultValueMode
  {
    /**
     * This parameter has no default behaviour and should be set by
     * the user.
     */
    DefaultValueMode_UNKNOWN,
    /**
     * The default value of this parameter can be estimated from
     * other parameters.
     */
    DefaultValueMode_RELATIVE,

    /**
     * The default value of this parameter is not depending on any
     * other parameter.
     */
    DefaultValueMode_ABSOLUTE
  };


/** \class Parameter
 *  \brief This class represent a parameter for the wrapper framework
 *  This class is a high level class representing a parameter for the
 *  wrapper framework. It should be subclassed to represent different
 *  kinds of parameters
 */
class ITK_ABI_EXPORT Parameter
  : public itk::Object
{
public:
  /** Standard class typedef */
  typedef Parameter                     Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(Parameter, itk::Object);

  /** Set the parameter name */
  itkSetStringMacro(Name);

  /** Get the parameter name */
  itkGetStringMacro(Name);

  /** Set the parameter description */
  itkSetStringMacro(Description);

  /** Get the parameter description */
  itkGetStringMacro(Description);

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

  /** Set the parameter AutomaticValue flag */
  itkSetMacro(AutomaticValue, bool);

  /** Get the parameter AutomaticValue flag */
  itkGetConstMacro(AutomaticValue, bool);

  /** Toogle the parameter AutomaticValue flag */
  itkBooleanMacro(AutomaticValue);

  /** Set the default value mode */
  itkSetEnumMacro(DefaultValueMode, DefaultValueMode);

  /** Get the default value mode */
  itkGetEnumMacro(DefaultValueMode, DefaultValueMode);

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

  virtual bool HasValue() const
  {
    itkExceptionMacro(<<"HasValue() method must be re-implemented by sub-classes.");
  }

  virtual bool HasUserValue() const
  {
    return HasValue() && m_UserValue;
  }

  virtual void SetUserValue(bool isUserValue)
  {
    m_UserValue = isUserValue;
  }

  virtual void ClearValue()
  {
    itkExceptionMacro(<<"ClearValue() method must be re-implemented by sub-classes.");
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

  /** Is the paramter a root or a child of another param */
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

  /** Store the state of the check box relative to this parameter (TO
    * BE MOVED to QtWrapper Model )
    */
  virtual bool IsChecked()
  {
    return m_IsChecked;
  }

  /** Modify the state of the checkbox relative to this paramter */
  virtual void SetChecked(const bool value)
  {
    m_IsChecked = value;
  }

protected:
  /** Constructor */
  Parameter() : m_Name(""),
                m_Description(""),
                m_Key(""),
                m_Mandatory(true),
                m_Active(false),
                m_UserValue(false),
                m_AutomaticValue(false),
                m_DefaultValueMode(DefaultValueMode_UNKNOWN),
                m_UserLevel(UserLevel_Basic),
                m_Role(Role_Input),
                m_Root(this),
                m_IsChecked(false)
  {}

  /** Destructor */
  virtual ~Parameter()
  {}

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

  /** True if the value is set in user mode */
  bool                               m_UserValue;

  /** True if the application change the value of this parameter */
  bool                               m_AutomaticValue;

  /** Default value behaviour */
  DefaultValueMode                   m_DefaultValueMode;

  UserLevel                          m_UserLevel;

  /** Default iotype mode */
  Role                               m_Role;

  /** List of parents Parameters */
  itk::WeakPointer<Parameter>        m_Root;

  /** List of children parameters */
  std::vector<Parameter::Pointer >   m_ChildrenList;

  /** Store the status of the checkbox */
  bool                               m_IsChecked;

private:
  Parameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
