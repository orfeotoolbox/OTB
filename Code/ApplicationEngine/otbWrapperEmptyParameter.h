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
#ifndef __otbWrapperEmptyParameter_h
#define __otbWrapperEmptyParameter_h

#include "otbWrapperParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class EmptyParameter
 *  \brief This class represent an empty parameter for the wrapper framework (boolean value)
 */
class EmptyParameter
  : public Parameter
{
public:
  /** Standard class typedef */
  typedef EmptyParameter                Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(EmptyParameter, Parameter);

  bool HasValue() const
  {
    return m_Value;
  }
  
  virtual void ClearValue()
  {
    m_Value = false;
  }

  /** Set the value */
  void SetValue( bool value)
  {
    // TODO check minimum/maximum
    m_Value = value;
    SetActive(true);
  }

  /** 
    * Implement the reset method (replace value by default value = false)
    */
  virtual void Reset()
  {
    m_Value = false;
  }

protected:
  /** Constructor */
  EmptyParameter():m_Value(false)
  {}

  /** Destructor */
  virtual ~EmptyParameter()
  {}

private:
  EmptyParameter(const EmptyParameter &); //purposely not implemented
  void operator =(const EmptyParameter&); //purposely not implemented

  /** Value */
  bool         m_Value;

}; // End class EmptyParameter

} // End namespace Wrapper
} // End namespace otb

#endif
