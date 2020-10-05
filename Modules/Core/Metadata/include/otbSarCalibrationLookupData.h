/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef SarCalibrationLookupData_H
#define SarCalibrationLookupData_H 1
#include <string>
#include <itkLightObject.h>
#include <itkNumericTraits.h>
#include <itkObjectFactory.h>

#include "OTBMetadataExport.h"

namespace otb
{

class OTBMetadata_EXPORT SarCalibrationLookupData : public itk::LightObject
{

public:
  /** Standard typedefs */
  typedef SarCalibrationLookupData      Self;
  typedef itk::LightObject              Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Creation through the object factory */
  itkNewMacro(Self);

  /** RTTI */
  itkTypeMacro(SarCalibrationLookupData, itk::LightObject);

  typedef itk::IndexValueType IndexValueType;

  enum
  {
    SIGMA = 0,
    BETA,
    GAMMA,
    DN
  };

  SarCalibrationLookupData() : m_Type(0)
  {
  }

  ~SarCalibrationLookupData() override
  {
  }

  virtual double GetValue(const IndexValueType itkNotUsed(x), const IndexValueType itkNotUsed(y)) const
  {
    return 1.0;
  }

  void SetType(short t)
  {
    m_Type = t;
  }

  itkGetMacro(Type, short);

  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    os << indent << " lookup table type:'" << m_Type << "'" << std::endl;
    Superclass::PrintSelf(os, indent);
  }

private:
  SarCalibrationLookupData(const Self&) = delete;
  void  operator=(const Self&) = delete;
  short m_Type;
};
}
#endif
