/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbSarBrightnessToImageFilter_hxx
#define otbSarBrightnessToImageFilter_hxx

#include "otbSarBrightnessToImageFilter.h"

#include "otbSarImageMetadataInterfaceFactory.h"

namespace otb
{

/**
 * Constructor
 */
template<class TInputImage, class TOutputImage>
SarBrightnessToImageFilter<TInputImage, TOutputImage>
::SarBrightnessToImageFilter()
{
}

template<class TInputImage, class TOutputImage>
void
SarBrightnessToImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  // will SetInputImage on the function
  Superclass::BeforeThreadedGenerateData();

  SarImageMetadataInterface::Pointer imageMetadataInterface = SarImageMetadataInterfaceFactory::CreateIMI(
      this->GetInput()->GetMetaDataDictionary());

  FunctionPointer function = this->GetFunction();

  function->SetScale(imageMetadataInterface->GetRadiometricCalibrationScale());

  ParametricFunctionPointer   noise = function->GetNoise();
  noise->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationNoise());
  noise->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationNoisePolynomialDegree());
  noise->EvaluateParametricCoefficient();

  ParametricFunctionPointer   antennaPatternNewGain = function->GetAntennaPatternNewGain();
  antennaPatternNewGain->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationAntennaPatternNewGain());
  antennaPatternNewGain->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree());
  antennaPatternNewGain->EvaluateParametricCoefficient();

  ParametricFunctionPointer   antennaPatternOldGain = function->GetAntennaPatternOldGain();
  antennaPatternOldGain->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationAntennaPatternOldGain());
  antennaPatternOldGain->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree());
  antennaPatternOldGain->EvaluateParametricCoefficient();

  ParametricFunctionPointer   rangeSpreadLoss = function->GetRangeSpreadLoss();
  rangeSpreadLoss->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationRangeSpreadLoss());
  rangeSpreadLoss->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationRangeSpreadLossPolynomialDegree());
  rangeSpreadLoss->EvaluateParametricCoefficient();

#if 0
  this->Print(std::cout, id);
#endif
}

template<class TInputImage, class TOutputImage>
void
SarBrightnessToImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
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
