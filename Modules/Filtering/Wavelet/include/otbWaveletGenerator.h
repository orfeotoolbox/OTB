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


#ifndef otbWaveletGenerator_h
#define otbWaveletGenerator_h

#include <vector>
#include "itkLightObject.h"
#include "itkObjectFactory.h"
#include "OTBWaveletExport.h"

namespace otb
{

namespace Wavelet
{
enum Wavelet {
  HAAR = 0,
  DAUBECHIES4 = 1, DB4 = 1,
  DAUBECHIES6 = 2, DB6 = 2,
  DAUBECHIES8 = 3, DB8 = 3,
  DAUBECHIES12 = 4, DB12 = 4,
  DAUBECHIES20 = 5, DB20 = 5,
  SPLINE_BIORTHOGONAL_2_4, // 6
  SPLINE_BIORTHOGONAL_4_4, // 7
  SYMLET8, // 8
  TotalNumberOfDefinedMotherWavelets
  };
}

/** \class WaveletGenerator
 * \brief Wavelet coefficient definition
 *
 * The wavelet coefficient definitions mainly come from
 * "Ten Lecture on Wavelets", of Ingrid Daubechies,
 * Society for Industrial and Applied Mathematics, 1992.
 *
 * The class is templated with the wavelet ID from the
 * Wavelet::Wavelet type. The members throw an
 * exception if the template specialization is not defined
 * according to the wavelet ID.
 *
 * \ingroup OTBWavelet
 */
template <Wavelet::Wavelet TMotherWaveletOperator>
class OTBWavelet_EXPORT WaveletGenerator
  : public itk::LightObject
{
public:
  /** Standard typedefs */
  typedef WaveletGenerator              Self;
  typedef itk::LightObject              Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(WaveletGenerator, LightObject);

  /** Typedef similar to itk::NeighborhoodOperator */
  typedef std::vector<double> CoefficientVector;

  /**
   * GetWaveletName
   * By default (without template specification) it does nothing usable
   */
  const char * GetWaveletName() const;

  /**
   * GetLowPassCoefficientVector
   * By default (without template specification) it does nothing usable
   */
  void GetLowPassCoefficientVector(CoefficientVector& coeff) const;

  /**
   * GetHighPassCoefficientVector
   * By default (without template specification) it does nothing usable
   */
  void GetHighPassCoefficientVector(CoefficientVector& coeff) const;

protected:
  WaveletGenerator() {}
  ~WaveletGenerator() override {}

private:
  WaveletGenerator(const Self &);  // not implemented
  void operator =(const Self&); // not implemented
}; // end of class

} // end of namespace otb

#endif
