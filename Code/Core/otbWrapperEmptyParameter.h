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
  typedef itk::LightObject              Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(EmptyParameter, Parameter);

  /** Set any value */
  virtual void SetAnyValue(boost::any v)
  {
    // Perform any cast
    m_Value = boost::any_cast<bool>(v);

    // Call Modified();
    this->Modified();
  }

  /** Return any value */
  virtual boost::any GetAnyValue()
  {
    return boost::any(m_Value);
  }


protected:
  /** Constructor */
  EmptyParameter()
  {}

  /** Destructor */
  ~EmptyParameter()
  {}

private:
  EmptyParameter(const EmptyParameter &); //purposely not implemented
  void operator =(const EmptyParameter&); //purposely not implemented

  bool m_Value;

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
