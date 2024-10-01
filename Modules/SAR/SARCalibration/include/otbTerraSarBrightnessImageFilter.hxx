/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbTerraSarBrightnessImageFilter_hxx
#define otbTerraSarBrightnessImageFilter_hxx

#include "otbTerraSarBrightnessImageFilter.h"
#include "otbMetaDataKey.h"

namespace otb
{

template <class TInputImage, class TOutputImage>
void TerraSarBrightnessImageFilter<TInputImage, TOutputImage>::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();

  /** Retrieve the ImageMetadata */
  auto imd = this->GetInput()->GetImageMetadata();

  // If the user doesn't set it AND the metadata is available, set calFactor using image metadata
  if (this->GetCalibrationFactor() == itk::NumericTraits<double>::Zero)
  {
    if (imd.Has(MDNum::CalFactor))
    {
      this->SetCalibrationFactor(imd[MDNum::CalFactor]);
    }
    else
    {
      itkExceptionMacro(<< "Invalid input image. Only TerraSar images are supported");
    }
  }
}
}

#endif
