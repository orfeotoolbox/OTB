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
#include <boost/any.hpp>

namespace otb
{
namespace Wrapper
{
/** \class NumericalParameter
 *  \brief This class represents a numerical parameter
 */
template <class T>
class ITK_EXPORT NumericalParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef NumericalParameter            Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(NumericalParameter, Parameter);

  /** Typedef of the scalar type */
  typedef T ScalarType;

  /** Implement the reset method (replace value by default value) */
  virtual void Reset()
  {
    m_Value = m_DefaultValue;
  }

  /** Set the value */
  void SetValue( ScalarType value)
  {
    // TODO check minimum/maximum
    m_Value = value;

    // Set Active only if the parameter is not automatically set
    if (!GetAutomaticValue())
      {
      SetActive(true);
      }
  }

  void SetValue( const std::string & valueStr )
  {
    ScalarType value = static_cast<ScalarType>(atof(valueStr.c_str()));
    SetValue(value);
  }

  /** Get the value */
  ScalarType GetValue() const
  {
    if (!HasValue())
      {
      itkGenericExceptionMacro(<< "Parameter " << this->GetKey() << " has no value yet.");
      }
    return boost::any_cast<ScalarType>(m_Value);
  }

  bool HasValue() const
  {
    return !m_Value.empty();
  }

  void ClearValue()
  {
    m_Value = boost::any();
  }

  /** Set the default value */
  itkSetMacro(DefaultValue, ScalarType);

  /** Get the default value */
  itkGetMacro(DefaultValue, ScalarType);

  /** Set the minimum value */
  itkSetMacro(MinimumValue, ScalarType);

  /** Get the minimum value */
  itkGetMacro(MinimumValue, ScalarType);

  /** Set the maximum value */
  itkSetMacro(MaximumValue, ScalarType);

  /** Get the maximum value */
  itkGetMacro(MaximumValue, ScalarType);

protected:
  /** Constructor */
  NumericalParameter()
    : m_DefaultValue(itk::NumericTraits<T>::Zero),
      m_MinimumValue(itk::NumericTraits<T>::NonpositiveMin()),
      m_MaximumValue(itk::NumericTraits<T>::max())
  {}

  /** Destructor */
  virtual ~NumericalParameter()
  {}

  /** Value */
  boost::any m_Value;

  /** Default value (when appliable) */
  ScalarType m_DefaultValue;

  /** Minimum value */
  ScalarType m_MinimumValue;

  /** Maximum value */
  ScalarType m_MaximumValue;

private:
  NumericalParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

}; // End class Numerical Parameter

// Helper typedef for float
typedef NumericalParameter<float> FloatParameter;

// Helper typedef for int
typedef NumericalParameter<int> IntParameter;

} // End namespace Wrapper
} // End namespace otb

#endif
