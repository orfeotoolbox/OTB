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
#include "boost/any.hpp"
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
class Parameter
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
  itkTypeMacro(Parameter,itk::Object);

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

  /** Set the parameter mandatory flag */
  itkSetMacro(Active,bool);

  /** Get the parameter mandatory flag */
  itkGetConstMacro(Active,bool);

  /** Set the parameter mandatory flag */
  itkSetMacro(Mandatory,bool);

  /** Get the parameter mandatory flag */
  itkGetConstMacro(Mandatory,bool);

  /** Toogle the parameter mandatory flag */
  itkBooleanMacro(Mandatory);

  /** Set the default value mode */
  itkSetEnumMacro(DefaultValueMode,DefaultValueMode);

  /** Get the default value mode */
  itkGetEnumMacro(DefaultValueMode,DefaultValueMode);

  /** Set the user access level */
  itkSetEnumMacro(UserLevel,UserLevel);

  /** Get the user access level */
  itkGetEnumMacro(UserLevel,UserLevel);

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

protected:
  /** Constructor */
  Parameter() : m_Name(""),
                m_Description(""),
                m_Key(""),
                m_Mandatory(false),
                m_Active(false),
                m_UserValue(false),
                m_DefaultValueMode(DefaultValueMode_UNKNOWN),
                m_UserLevel(UserLevel_Basic)
  {}

  /** Destructor */
  virtual ~Parameter()
  {}

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

  /** True if the value is set in user mode */
  bool m_UserValue;

  /** Default value behaviour */
  DefaultValueMode m_DefaultValueMode;

  UserLevel m_UserLevel;

private:
  Parameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
