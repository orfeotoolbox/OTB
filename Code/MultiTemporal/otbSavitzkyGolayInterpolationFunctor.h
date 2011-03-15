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
template <unsigned int Radius, class TSeries, class TDates, class TWeight = TSeries>
class SavitzkyGolayInterpolationFunctor
{
public:

  typedef typename TSeries::ValueType ValueType;
  typedef typename TDates::ValueType DateType;
  typedef typename TWeight::ValueType WeightType;
  static const unsigned int Degree = 2;
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
    for(unsigned int i=0; i<m_WeightSeries.Size(); ++i)
      m_WeightSeries[i] = weights[i];
  }

  inline void SetDates(const TDates doy)
  {
    for(unsigned int i=0; i<m_DoySeries.Size(); ++i)
      m_DoySeries[i] = doy[i];
  }


  inline TSeries operator ()(const TSeries& series)
  {
    TSeries outSeries;

    unsigned int firstSample = Radius;
    unsigned int lastSample = nbDates-Radius-1;

    for(unsigned int i=0; i<firstSample; ++i)
      outSeries[i] = series[i];
    for(unsigned int i=lastSample+1; i<nbDates; ++i)
      outSeries[i] = series[i];

    for(unsigned int i=firstSample; i<=lastSample; ++i)
      {

      InterpolatedSeriesType tmpInSeries;
      InterpolatedDatesType tmpDates;
      InterpolatedWeightType tmpWeights;

      for(int j=-Radius; j<=Radius; ++j)
        {
        tmpInSeries[j+Radius] = series[i+j];
        tmpDates[j+Radius] = m_DoySeries[i+j];
        tmpWeights[j+Radius] = m_WeightSeries[i+j];
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
}
#endif
