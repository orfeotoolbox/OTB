/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWaveletOperator_h
#define otbWaveletOperator_h

#include "itkMacro.h"
#include "itkNeighborhoodOperator.h"

// This include is needed here to define Wavelet::Wavelet...
#include "otbWaveletGenerator.h"

#include "otbWaveletLowPassOperator.h"
#include "otbWaveletHighPassOperator.h"

namespace otb
{

/**
 * \class WaveletOperator
 *
 * \brief A Generic NeighborhoodOperator wavelets filter set defined for templation
 *
 * This class is the general class to be used for template definition of any
 * Wavelet decomposition. It contains only typedef that give access to specific
 * LowPass and HighPass filters in a dyadic decomposition frame.
 *
 * Note: It is not an operator as it stands
 *
 * \sa WaveletOperatorBase
 * \sa WaveletLowPassOperator
 * \sa WaveletHighPassOperator
 *
 * \ingroup OTBWavelet
 */
template <Wavelet::Wavelet TMotherWaveletOperator, Wavelet::WaveletDirection TDirectionOfTransformation, class TPixel, unsigned int VDimension,
          class TAllocator = itk::NeighborhoodAllocator<TPixel>>
class ITK_EXPORT WaveletOperator
{
public:
  /** Standard typedefs */
  typedef WaveletOperator Self;

  /** Typedefs redirections */
  typedef WaveletLowPassOperator<TMotherWaveletOperator, TDirectionOfTransformation, TPixel, VDimension, TAllocator>  LowPassOperator;
  typedef WaveletHighPassOperator<TMotherWaveletOperator, TDirectionOfTransformation, TPixel, VDimension, TAllocator> HighPassOperator;

private:
  /**
   * This class may not be instantiated
   * Intentionally not implemented
   */
  WaveletOperator();
  WaveletOperator(const Self&);
  ~WaveletOperator();
};

} // end of namespace otb

#endif
