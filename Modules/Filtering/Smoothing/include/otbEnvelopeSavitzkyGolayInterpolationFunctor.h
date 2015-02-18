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
#ifndef __otbEnvelopeSavitzkyGolayInterpolationFunctor_h
#define __otbEnvelopeSavitzkyGolayInterpolationFunctor_h

#include "otbSavitzkyGolayInterpolationFunctor.h"


namespace otb
{
namespace Functor
{
/** \class otbEnvelopeSavitzkyGolayInterpolationFunctor
 *
 *  \brief: This class implements a Savitzky-Golay interpolation
 *  fitting the upper or lower envelope of the series being interpolated.
 *
 * \sa otbSavitzkyGolayInterpolationFunctor
 */
template <unsigned int Radius, class TSeries, class TDates, class TWeight = TSeries>
class EnvelopeSavitzkyGolayInterpolationFunctor
{
public:

  typedef typename TSeries::ValueType ValueType;
  typedef typename TDates::ValueType DateType;
  typedef typename TWeight::ValueType WeightType;
  static const unsigned int Degree = 2;
  typedef double CoefficientPrecisionType;
  typedef otb::PolynomialTimeSeries< Degree, CoefficientPrecisionType > TSFunctionType;
  static const unsigned int nbDates = TSeries::Dimension;

  typedef otb::Functor::SavitzkyGolayInterpolationFunctor<Radius, TSeries, TDates, TWeight> SGFunctorType;

  /// Constructor
  EnvelopeSavitzkyGolayInterpolationFunctor() : m_Iterations(2), m_UpperEnvelope(true), m_DecreaseFactor(0.5)
  {
  }
  /// Destructor
  virtual ~EnvelopeSavitzkyGolayInterpolationFunctor() {}

  inline void SetWeights(const TWeight weights)
  {
    for(unsigned int i=0; i<m_WeightSeries.Size(); ++i)
      m_WeightSeries[i] = weights[i];
    m_SGFunctor.SetWeights( weights );
  }

  inline void SetDates(const TDates doy)
  {
    m_SGFunctor.SetDates( doy );
  }

  inline void SetUpperEnvelope()
  {
    m_UpperEnvelope = true;
  }

  inline void SetLowerEnvelope()
  {
    m_UpperEnvelope = false;
  }

  inline void SetDecreaseFactor(double df)
  {
    if(df>0.0 && df <1.0)
      m_DecreaseFactor = df;
  }

  inline void SetIterations(unsigned int its)
  {
    m_Iterations = its;
  }

  inline TSeries operator ()(const TSeries& series)
  {
    TSeries outSeries = m_SGFunctor(series);

    for(unsigned int i=0; i<m_Iterations; ++i)
      {
      for(unsigned int j=0; j<nbDates; ++j)
        {
        if( m_UpperEnvelope && outSeries[j]<series[j] )
          m_WeightSeries[j] = m_WeightSeries[j]*m_DecreaseFactor;
        if( !m_UpperEnvelope && outSeries[j]>series[j] )
          m_WeightSeries[j] = m_WeightSeries[j]*m_DecreaseFactor;
        }

      m_SGFunctor.SetWeights(m_WeightSeries);
      outSeries = m_SGFunctor(series);
      }

    return outSeries;
  }

private:

  TWeight m_WeightSeries;
  SGFunctorType m_SGFunctor;
  unsigned int m_Iterations;
  bool m_UpperEnvelope;
  double m_DecreaseFactor;

};
}
} //namespace otb
#endif
