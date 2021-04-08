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
#include "otbSARMetadata.h"
#include <boost/any.hpp>

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
  /** Fetch the SARCalib */
  if (!imd.Has(MDGeom::SARCalib))
    throw std::runtime_error("otbSarBrightnessToImageFilter was not able to fetch the SARCalib metadata.");
  auto sarCalib = boost::any_cast<SARCalib>(imd[MDGeom::SARCalib]);

  FunctionPointer function = this->GetFunction();

  function->SetScale(imd[MDNum::CalScale]);

  ParametricFunctionPointer noise = function->GetNoise();
  noise->SetPointSet(sarCalib.radiometricCalibrationNoise);
  noise->SetPolynomalSize(sarCalib.radiometricCalibrationNoisePolynomialDegree);
  noise->EvaluateParametricCoefficient();

  ParametricFunctionPointer antennaPatternNewGain = function->GetAntennaPatternNewGain();
  antennaPatternNewGain->SetPointSet(sarCalib.radiometricCalibrationAntennaPatternNewGain);
  antennaPatternNewGain->SetPolynomalSize(sarCalib.radiometricCalibrationAntennaPatternNewGainPolynomialDegree);
  antennaPatternNewGain->EvaluateParametricCoefficient();

  ParametricFunctionPointer antennaPatternOldGain = function->GetAntennaPatternOldGain();
  antennaPatternOldGain->SetPointSet(sarCalib.radiometricCalibrationAntennaPatternOldGain);
  antennaPatternOldGain->SetPolynomalSize(sarCalib.radiometricCalibrationAntennaPatternOldGainPolynomialDegree);
  antennaPatternOldGain->EvaluateParametricCoefficient();

  ParametricFunctionPointer rangeSpreadLoss = function->GetRangeSpreadLoss();
  rangeSpreadLoss->SetPointSet(sarCalib.radiometricCalibrationRangeSpreadLoss);
  rangeSpreadLoss->SetPolynomalSize(sarCalib.radiometricCalibrationRangeSpreadLossPolynomialDegree);
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
