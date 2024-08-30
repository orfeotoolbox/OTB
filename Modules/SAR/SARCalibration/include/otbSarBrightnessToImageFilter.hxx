/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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
  std::unique_ptr<SARCalib> sarCalibPtr;
  if (imd.Has(MDGeom::SARCalib))
  {
    sarCalibPtr = std::make_unique<SARCalib>(boost::any_cast<SARCalib>(imd[MDGeom::SARCalib]));
  }
  else if ((imd.Bands.size() > 0) && imd.Bands[0].Has(MDGeom::SARCalib))
  {
    sarCalibPtr = std::make_unique<SARCalib>(boost::any_cast<SARCalib>(imd.Bands[0][MDGeom::SARCalib]));
  }
  else
      throw std::runtime_error("otbSarBrightnessToImageFilter was not able to fetch the SARCalib metadata.");

  FunctionPointer function = this->GetFunction();

  function->SetScale(imd[MDNum::CalScale]);

  ParametricFunctionPointer noise = function->GetNoise();
  noise->SetPointSet(sarCalibPtr->radiometricCalibrationNoise);
  noise->SetPolynomalSize(sarCalibPtr->radiometricCalibrationNoisePolynomialDegree);
  noise->EvaluateParametricCoefficient();

  ParametricFunctionPointer antennaPatternNewGain = function->GetAntennaPatternNewGain();
  antennaPatternNewGain->SetPointSet(sarCalibPtr->radiometricCalibrationAntennaPatternNewGain);
  antennaPatternNewGain->SetPolynomalSize(sarCalibPtr->radiometricCalibrationAntennaPatternNewGainPolynomialDegree);
  antennaPatternNewGain->EvaluateParametricCoefficient();

  ParametricFunctionPointer antennaPatternOldGain = function->GetAntennaPatternOldGain();
  antennaPatternOldGain->SetPointSet(sarCalibPtr->radiometricCalibrationAntennaPatternOldGain);
  antennaPatternOldGain->SetPolynomalSize(sarCalibPtr->radiometricCalibrationAntennaPatternOldGainPolynomialDegree);
  antennaPatternOldGain->EvaluateParametricCoefficient();

  ParametricFunctionPointer rangeSpreadLoss = function->GetRangeSpreadLoss();
  rangeSpreadLoss->SetPointSet(sarCalibPtr->radiometricCalibrationRangeSpreadLoss);
  rangeSpreadLoss->SetPolynomalSize(sarCalibPtr->radiometricCalibrationRangeSpreadLossPolynomialDegree);
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
