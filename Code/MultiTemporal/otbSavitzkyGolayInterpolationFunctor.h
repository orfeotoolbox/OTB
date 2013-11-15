/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbSavitzkyGolayInterpolationFunctor_h
#define __otbSavitzkyGolayInterpolationFunctor_h

#include "otbTimeSeries.h"
#include "otbTimeSeriesLeastSquareFittingFunctor.h"


namespace otb
{
namespace Functor
{

/** \class otbSavitzkyGolayInterpolationFunctor
 *
 *  \brief: This functor implements a local polynomial regression (of
 *  degree k) on a series of values (of at least k+1 points which are
 *  treated as being equally spaced in the series) to determine the
 *  smoothed value for each point.
 *
 *  In this implementation, the interpolation is performed by least
 *  squares fitting. The size of the moving window for the
 *  interpolation can be set using the radius variable. If needed,
 *  weights for each value of the series to be used by the least
 *  squares estimation can be set (the higher the weight, the lower the
 *  confidence in the value).
 *
 *  Savitzky, A.; Golay, M.J.E. (1964). "Smoothing and Differentiation of
 *  Data by Simplified Least Squares Procedures". Analytical Chemistry 36
 *  (8): 1627-1639. doi:10.1021/ac60214a047
 *
 * \sa otbTimeSeriesLeastSquareFittingFunctor
 *
 */
template <unsigned int Radius, class TSeries, class TDates, class TWeight = TSeries, unsigned int Degree=2>
class SavitzkyGolayInterpolationFunctor
{
public:

  typedef typename TSeries::ValueType ValueType;
  typedef typename TDates::ValueType DateType;
  typedef typename TWeight::ValueType WeightType;
  typedef double CoefficientPrecisionType;
  typedef otb::PolynomialTimeSeries< Degree, CoefficientPrecisionType > TSFunctionType;
  static const unsigned int nbDates = TSeries::Dimension;
  static const unsigned int InterpolatedLength = 2*Radius+1;

  typedef itk::FixedArray< ValueType, InterpolatedLength> InterpolatedSeriesType;
  typedef itk::FixedArray< DateType, InterpolatedLength> InterpolatedDatesType;
  typedef itk::FixedArray< WeightType, InterpolatedLength> InterpolatedWeightType;

  typedef otb::Functor::TimeSeriesLeastSquareFittingFunctor<InterpolatedSeriesType,
            TSFunctionType, InterpolatedDatesType, InterpolatedWeightType> TLSFunctorType;

  /// Constructor
  SavitzkyGolayInterpolationFunctor()
  {
  }
  /// Destructor
  virtual ~SavitzkyGolayInterpolationFunctor() {}

  inline void SetWeights(const TWeight weights)
  {
    for(unsigned int i = 0; i < m_WeightSeries.Size(); ++i)
      m_WeightSeries[i] = weights[i];
  }

  inline void SetDates(const TDates doy)
  {
    for(unsigned int i = 0; i < m_DoySeries.Size(); ++i)
      m_DoySeries[i] = doy[i];
  }

  inline TSeries operator ()(const TSeries& series) const
  {
    TSeries outSeries;

    unsigned int firstSample = Radius;
    unsigned int lastSample = nbDates - Radius - 1;

    for(unsigned int i = 0; i<firstSample; ++i)
      outSeries[i] = series[i];
    for(unsigned int i = lastSample+1; i<nbDates; ++i)
      outSeries[i] = series[i];

    for(unsigned int i = firstSample; i <= lastSample; ++i)
      {
      InterpolatedSeriesType tmpInSeries;
      InterpolatedDatesType tmpDates;
      InterpolatedWeightType tmpWeights;

      for(unsigned int j = 0; j <= 2*Radius; ++j)
        {
        tmpInSeries[j] = series[i+j-Radius];
        tmpDates[j] = m_DoySeries[i+j-Radius];
        tmpWeights[j] = m_WeightSeries[i+j-Radius];
        }

      TLSFunctorType f;
      f.SetDates( tmpDates );
      f.SetWeights( tmpWeights);
      InterpolatedSeriesType tmpOutSeries = f(tmpInSeries);
      outSeries[i] = tmpOutSeries[Radius];
      }

    return outSeries;
  }

private:

  TWeight m_WeightSeries;
  TDates m_DoySeries;

};
}
} //namespace otb
#endif
