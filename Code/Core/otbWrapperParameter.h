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
    UNKNOWN,
    /**
     * The default value of this parameter can be estimated from
     * other parameters.
     */
    RELATIVE,

    /** 
     * Tge default value of this parameter is not depending of any
     * other parameter.
     */
    ABSOLUTE
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
  typedef itk::LightObject              Superclass;
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
  itkSetMacro(Mandatory,bool);

  /** Get the parameter mandatory flag */
  itkGetMacro(Mandatory,bool);

  /** Toogle the parameter mandatory flag */
  itkBooleanMacro(Mandatory);

  /** Set the default value mode */
  itkSetEnumMacro(DefaultValueMode,DefaultValueMode);

  /** Get the default value mode */
  itkGetEnumMacro(DefaultValueMode,DefaultValueMode);

  /** Reset to the the default value. Default implementation does
   * nothing 
   */
  virtual void Reset() {}

protected:
  /** Constructor */
  Parameter() : m_Name(""),
                m_Description(""),
                m_Key(""),
                m_Mandatory(true),
                m_DefaultValueMode(UNKNOWN)
  {}

  /** Destructor */
  virtual ~Parameter()
  {}

private:
  Parameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

  /** Name of the parameter */
  std::string m_Name;

  /** Description of the parameter */
  std::string m_Description;

  /** Key of the parameter */
  std::string m_Key;

  /** True if the parameter is mandatory */
  bool m_Mandatory;

  /** Default value behaviour */
  DefaultValueMode m_DefaultValueMode;

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
