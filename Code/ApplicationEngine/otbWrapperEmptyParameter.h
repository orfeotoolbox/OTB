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
class ITK_ABI_EXPORT EmptyParameter
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

  /** HasValue */
  bool HasValue() const
  {
    return false;
  }

  bool HasUserValue() const
  {
    return this->m_UserValue;
  }

protected:
  /** Constructor */
  EmptyParameter()
  {
  // It does not make sense for an empty parameter to be mandatory
  this->MandatoryOff();
  }

  /** Destructor */
  virtual ~EmptyParameter()
  {}

private:
  EmptyParameter(const EmptyParameter &); //purposely not implemented
  void operator =(const EmptyParameter&); //purposely not implemented

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
