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
#ifndef __otbWrapperNumericalParameter_h
#define __otbWrapperNumericalParameter_h

#include "otbWrapperParameter.h"
#include "itkNumericTraits.h"

namespace otb
{
namespace Wrapper
{
/** \class NumericalParameter
 *  \brief This class represents a numerical parameter
 */ 
template <typename T> NumericalParameter
: public Parameter
{
public:
  /** Standard class typedef */
  typedef NumericalParameter            Self;
  typedef itk::LightObject              Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(NumericalParameter,otb::Wrapper::Parameter);

  /** Typedef of the scalar type */
  typedef T ScalarType;

  /** Implement the reset method (replace value by default value) */
  virtual void Reset()
  {
    m_Value = m_DefaultValue;
  }

  /** Set any value */
  virtual SetAnyValue(boost::any v)
  {
    // Perform any cast
    m_Value=boost::any_cast<T>(v);

    // Call Modified();
    this->Modified();
  }
  
  /** Return any value */
  virtual boost::any GetAnyValue()
  {
    return boost::any(m_Value);
  }

  /** Set the value */
  itkSetMacro(Value,ScalarType);

  /** Get the value */
  itkGetMacro(Value,ScalarType);

  /** Set the default value */
  itkSetMacro(DefaultValue,ScalarType);

  /** Get the default value */
  itkGetMacro(DefaultValue,ScalarType);

  /** Set the minimum value */
  itkSetMacro(MinimumValue,ScalarType);

  /** Get the minimum value */
  itkGetMacro(MinimumValue,ScalarType);

  /** Set the maximum value */
  itkSetMacro(MaximumValue,ScalarType);

  /** Get the maximum value */
  itkGetMacro(MaximumValue,ScalarType);

protected:
  /** Constructor */
  NumericalParameter() 
    : m_Value(itk::NumericTraits<T>::Zero()),
    m_DefaultValue(itk::NumericTraits<T>::Zero()),
    m_MinimumValue(itk::NumericTraits<T>::min()),
    m_MaximumValue(itk::NumericTraits<T>::max())
  {}

  /** Destructor */
  virtual ~NumericalParameter()
  {}

private:
  NumericalParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented
  
  /** Value */
  ScalarType m_Value;

  /** Default value (when appliable) */
  ScalarType m_DefaultValue;

  /** Minimum value */
  ScalarType m_MinimumValue;

  /** Maximum value */
  ScalarType m_MaximumValue;

}; // End class Numerical Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
