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


#include "otbImageMetadataCorrectionParameters.h"
#include "otbSpectralSensitivityReader.h"

namespace otb
{


/***********************      ImageMetadataCorrectionParameters **************************/
/**
 * Constructor
 */

ImageMetadataCorrectionParameters::ImageMetadataCorrectionParameters()
{
  m_SolarZenithalAngle           = 361.;
  m_SolarAzimutalAngle           = 361.;
  m_ViewingZenithalAngle         = 361.;
  m_ViewingAzimutalAngle         = 361.;
  m_Month                        = 0;
  m_Day                          = 0;
  m_Year                         = 0;
  m_FilterFunctionValuesFileName = "";

  m_WavelengthSpectralBand = InternalWavelengthSpectralBandVectorType::New();
  m_WavelengthSpectralBand->Clear();
}


/** Get data from filter function file*/
void ImageMetadataCorrectionParameters::LoadFilterFunctionValue(const std::string& filename)
{
  m_WavelengthSpectralBand->Clear();
  SpectralSensitivityReader::Pointer spectralSensitivityReader = SpectralSensitivityReader::New();
  spectralSensitivityReader->SetFileName(filename);
  spectralSensitivityReader->Update();
  m_WavelengthSpectralBand = spectralSensitivityReader->GetOutput();
}

/**PrintSelf method */
void ImageMetadataCorrectionParameters::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << "Solar zenithal angle  : " << m_SolarZenithalAngle << std::endl;
  os << "Solar azimutal angle  : " << m_SolarAzimutalAngle << std::endl;
  os << "Viewing zenithal angle: " << m_ViewingZenithalAngle << std::endl;
  os << "Viewing azimutal angle: " << m_ViewingAzimutalAngle << std::endl;
  os << "Month                 : " << m_Month << std::endl;
  os << "Day                   : " << m_Day << std::endl;
  os << "Year                  : " << m_Year << std::endl;

  // Function values print :
  os << "Filter function values: " << std::endl;
  for (unsigned int i = 0; i < m_WavelengthSpectralBand->Size(); ++i)
  {
    os << indent << "Channel " << i + 1 << " : " << std::endl;
    os << indent << m_WavelengthSpectralBand->GetNthElement(i) << std::endl;
  }
}
} // end namespace otb
