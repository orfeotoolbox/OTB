/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperRadiusParameter_h
#define otbWrapperRadiusParameter_h

#include "otbWrapperNumericalParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class StringParameter
 *  \brief This class represent a radius parameter for the wrapper framework
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT RadiusParameter
  : public IntParameter
{
public:
  /** Standard class typedef */
  typedef RadiusParameter               Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(RadiusParameter, Parameter);

  bool HasValue() const override
  {
    return true;
  }

protected:
  /** Constructor */
  RadiusParameter()
  {
    this->SetName("Radius");
    this->SetKey("r");
    this->SetDescription("Radius in pixels");
  }

  /** Destructor */
  ~RadiusParameter() override
  {}

private:
  RadiusParameter(const RadiusParameter &) = delete;
  void operator =(const RadiusParameter&) = delete;

};

} // End namespace Wrapper
} // End namespace otb

#endif
