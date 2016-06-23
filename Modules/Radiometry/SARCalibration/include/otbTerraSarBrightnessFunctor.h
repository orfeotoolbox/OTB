/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
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
  virtual ~TerraSarBrightnessFunctor() {}

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
  inline TOutput operator ()(const TInput& inPix);
  /** We assume that the input pixel is a complex -> complex image */
  inline std::complex<TOutput> operator ()(const std::complex<TInput>& inPix);

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
#include "otbTerraSarBrightnessFunctor.txx"
#endif

#endif
