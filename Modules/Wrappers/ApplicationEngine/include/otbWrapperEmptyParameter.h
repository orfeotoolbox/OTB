/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperEmptyParameter_h
#define otbWrapperEmptyParameter_h

#include "otbWrapperParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class EmptyParameter
 *  \brief This class represent an empty parameter for the wrapper framework (boolean value)
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT EmptyParameter
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
  bool HasValue() const ITK_OVERRIDE
  {
    return false;
  }

  bool HasUserValue() const ITK_OVERRIDE
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
  ~EmptyParameter() ITK_OVERRIDE
  {}

private:
  EmptyParameter(const EmptyParameter &); //purposely not implemented
  void operator =(const EmptyParameter&); //purposely not implemented

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
