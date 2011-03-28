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
#ifndef __otbWrapperStringParameter_h
#define __otbWrapperStringParameter_h

#include "otbWrapperParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class StringParameter
 *  \brief This class represent a string parameter for the wrapper framework
 */
class StringParameter
  : public Parameter
{
public:
  /** Standard class typedef */
  typedef StringParameter               Self;
  typedef itk::LightObject              Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(StringParameter, Parameter);


protected:
  /** Constructor */
  StringParameter()
  {}

  /** Destructor */
  ~StringParameter()
  {}

private:
  StringParameter(const StringParameter &); //purposely not implemented
  void operator =(const StringParameter&); //purposely not implemented

  std::string m_Value;

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
