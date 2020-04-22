/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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


#include "otbWaveletOperator.h"

int otbWaveletOperator(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  const otb::Wavelet::WaveletDirection directionID = otb::Wavelet::FORWARD;
  const otb::Wavelet::Wavelet          wvltID      = otb::Wavelet::HAAR;

  const int      Dimension = 2;
  typedef double PixelType;

  typedef otb::WaveletOperator<wvltID, directionID, PixelType, Dimension> WaveletOperator;
  typedef WaveletOperator::LowPassOperator  LowPassOperatorType;
  typedef WaveletOperator::HighPassOperator HighPassOperatorType;

  LowPassOperatorType h_filter;
  h_filter.SetDirection(0);
  h_filter.SetUpSampleFactor(2);
  h_filter.CreateDirectional();

  HighPassOperatorType g_filter;
  g_filter.SetDirection(0);
  g_filter.SetUpSampleFactor(2);
  g_filter.CreateDirectional();

  return EXIT_SUCCESS;
}
