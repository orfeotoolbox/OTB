/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbSarBrightnessToImageFilter_hxx
#define otbSarBrightnessToImageFilter_hxx

#include "otbSarBrightnessToImageFilter.h"

#include "otbSarImageMetadataInterfaceFactory.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
SarBrightnessToImageFilter<TInputImage, TOutputImage>::SarBrightnessToImageFilter()
{
}

template <class TInputImage, class TOutputImage>
void SarBrightnessToImageFilter<TInputImage, TOutputImage>::BeforeThreadedGenerateData()
{
  // will SetInputImage on the function
  Superclass::BeforeThreadedGenerateData();

  /** Retrieve the ImageMetadata */
  auto imd = this->GetInput()->GetImageMetadata();
  /** Fetch the SARModel */
  auto sarParam = boost::any_cast<SARParam>(imd[MDGeom::SAR]);

  FunctionPointer function = this->GetFunction();

  function->SetScale(imd[MDNum::CalScale]);

  ParametricFunctionPointer noise = function->GetNoise();
  noise->SetPointSet(sarParam.radiometricCalibrationNoise);
  noise->SetPolynomalSize({imd[MDNum::NoisePolyDegX], imd[MDNum::NoisePolyDegY]});
  noise->EvaluateParametricCoefficient();

  ParametricFunctionPointer antennaPatternNewGain = function->GetAntennaPatternNewGain();
  antennaPatternNewGain->SetPointSet(sarParam.radiometricCalibrationAntennaPatternNewGain);
  antennaPatternNewGain->SetPolynomalSize({imd[MDNum::AntennaPatternNewGainPolyDegX], imd[MDNum::AntennaPatternNewGainPolyDegY]});
  antennaPatternNewGain->EvaluateParametricCoefficient();

  ParametricFunctionPointer antennaPatternOldGain = function->GetAntennaPatternOldGain();
  antennaPatternOldGain->SetPointSet(sarParam.radiometricCalibrationAntennaPatternOldGain);
  antennaPatternOldGain->SetPolynomalSize({imd[MDNum::AntennaPatternOldGainPolyDegX], imd[MDNum::AntennaPatternOldGainPolyDegY]});
  antennaPatternOldGain->EvaluateParametricCoefficient();

  ParametricFunctionPointer rangeSpreadLoss = function->GetRangeSpreadLoss();
  rangeSpreadLoss->SetPointSet(sarParam.radiometricCalibrationRangeSpreadLoss);
  rangeSpreadLoss->SetPolynomalSize({imd[MDNum::RangeSpreadLossPolyDegX], imd[MDNum::RangeSpreadLossPolyDegY]});
  rangeSpreadLoss->EvaluateParametricCoefficient();

#if 0
  this->Print(std::cout, id);
#endif
}

template <class TInputImage, class TOutputImage>
void SarBrightnessToImageFilter<TInputImage, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  if (this->GetFunction())
  {
    os << indent << "Function:\n";
    this->GetFunction()->Print(os, indent.GetNextIndent());
  }
}


} // end namespace otb

#endif
