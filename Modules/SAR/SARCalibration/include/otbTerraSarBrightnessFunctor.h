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

#ifndef otbTerraSarBrightnessFunctor_h
#define otbTerraSarBrightnessFunctor_h

#include "otbMath.h"

#include <complex>

namespace otb
{
namespace Functor
{
/**
 * \class TerraSarBrightnessFunctor
 *  \brief Compute beta naught coefficient.
 *
 * This functor is intended to be used by the
 * TerrasarBrightnessImageFilter.
 *
 * For more information on Terrasar calibration,
 * please refer to the
 * documentation of this filter.
 *
 * \sa TerraSarBrightnessImageFilter
 * \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBSARCalibration
 */

template <class TInput, class TOutput>
class TerraSarBrightnessFunctor
{
public:
  TerraSarBrightnessFunctor();
  virtual ~TerraSarBrightnessFunctor()
  {
  }

  /** Accessors */
  void SetCalibrationFactor(double val)
  {
    m_CalibrationFactor = val;
  }

  double GetCalibrationFactor()
  {
    return m_CalibrationFactor;
  }

  /** Set results in decibels */
  void SetResultsInDecibels(bool db)
  {
    m_ResultsInDecibels = db;
  }

  /** Get the value of the flag */
  bool GetResultsInDecibels()
  {
    return m_ResultsInDecibels;
  }

  /** Set Default value for null or negative pixel beta */
  void SetDefaultValue(double db)
  {
    m_DefaultValue = db;
  }

  /** Set Default value for null or negative pixel beta */
  bool GetDefaultValue()
  {
    return m_DefaultValue;
  }

  /** We assume that the input pixel is a scalar -> modulus image */
  inline TOutput operator()(const TInput& inPix);
  /** We assume that the input pixel is a complex -> complex image */
  inline std::complex<TOutput> operator()(const std::complex<TInput>& inPix);

private:
  /** Calibration Factor */
  double m_CalibrationFactor;

  /** Shall we compute results in decibels ? */
  bool m_ResultsInDecibels;

  /** Default value for null or negative pixel beta */
  double m_DefaultValue;
};

} // end namespace functor
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTerraSarBrightnessFunctor.hxx"
#endif

#endif
