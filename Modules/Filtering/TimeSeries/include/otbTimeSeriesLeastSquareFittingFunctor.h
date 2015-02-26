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
#ifndef __otbTimeSeriesLSFF_h
#define __otbTimeSeriesLSFF_h

#include "vnl/algo/vnl_matrix_inverse.h"
#include "vnl/vnl_transpose.h"
#include "vnl/vnl_matrix.h"

namespace otb
{
namespace Functor
{
/** \class TimeSeriesLeastSquareFittingFunctor
  * \brief Implements a least squares fitting of a time profile
  *
  * This functor
  *  implements a least squares fitting of a time profile. The time
  *  series as well as the date series are supposed to accept the []
  *  syntax to get the values and the Size() method to get their
  *  length. The fitting is performed using a time function which is
  *  just a weighted sum of basis functions : \f$ f(t) = c_0 *
  *  \phi_0(t) + c_1 * \phi_1(t) + c_1 * \phi_1(t) + \cdots \f$ Using
  *  a matrix notation, this can be written as follows :
  *  \f$ F = \Phi c \f$
  *  \f$ F = ( f(t_i) ) \f$
  *  \f$ c = ( c_j ) \f$
  *  \f$ \Phi_{ij} = ( \phi_j(t_i) ) \f$
  *  If information about the error
  *  of each measure is available, it can be provided using a weight
  *  series \f$\sigma_i\f$ (the higher the error, the higher the \f$ \sigma_i \f$. The problem then becomes:
  *  \f$ b = A c \f$
  *  \f$ b = (\frac{ f(t_i) }{\sigma_i}) \f$
  *  \f$ A_{ij} = \frac{\Phi_{ij}}{\sigma_i} \f$
  *
  *
 *
 * \ingroup OTBSmoothing
  */
template <class TSeriesType, class TTimeFunction, class TDateType = TSeriesType, class TWeightType = TSeriesType>
class TimeSeriesLeastSquareFittingFunctor
{
public:

  typedef typename TTimeFunction::CoefficientsType CoefficientsType;

  /// Constructor
  TimeSeriesLeastSquareFittingFunctor()
  {
    for(unsigned int i=0; i<m_WeightSeries.Size(); ++i)
      m_WeightSeries[i] = 1.0;
  }
  /// Destructor
  virtual ~TimeSeriesLeastSquareFittingFunctor() {}

  inline TSeriesType operator ()(const TSeriesType& series)
  {
    TTimeFunction estFunction = this->EstimateTimeFunction(series);
    TSeriesType outSeries;
    for(unsigned int i = 0; i < m_DoySeries.Size(); ++i)
      outSeries[i] = estFunction.GetValue( m_DoySeries[i] );
    return outSeries;
  }

  inline void SetDates(const TDateType& doy)
  {
    for(unsigned int i = 0; i < doy.Size(); ++i)
      m_DoySeries[i] = doy[i];
  }

  inline void SetWeights(const TWeightType& weights)
  {
    for(unsigned int i=0; i<weights.Size(); ++i)
      m_WeightSeries[i] = weights[i];
  }

  inline CoefficientsType GetCoefficients(const TSeriesType& series) const
  {
    return (this->EstimateTimeFunction(series)).GetCoefficients();
  }

  inline TTimeFunction EstimateTimeFunction(const TSeriesType& series) const
  {
    TTimeFunction estFunction;
    unsigned int nbDates = m_DoySeries.Size();
    unsigned int nbCoefs = estFunction.GetCoefficients().Size();

    // b = A * c
    vnl_matrix<double> A(nbDates, nbCoefs);
    vnl_matrix<double> b(nbDates, 1);

    // fill the matrices

    typename TTimeFunction::CoefficientsType tmpCoefs;
    for(unsigned int j = 0; j < nbCoefs; ++j)
      tmpCoefs[j] = 0.0;

    for(unsigned int i = 0; i < nbDates; ++i)
      {
      b.put(i, 0, series[i] / m_WeightSeries[i]);
      for(unsigned int j = 0; j < nbCoefs; ++j)
        {
        tmpCoefs[j] = 1.0;
        estFunction.SetCoefficients(tmpCoefs);
        A.put(i, j, estFunction.GetValue(m_DoySeries[i]) / m_WeightSeries[i]);
        tmpCoefs[j] = 0.0;
        }
      }
    // solve the problem c = (At * A)^-1*At*b

    vnl_matrix<double> atainv =  vnl_matrix_inverse<double>(vnl_transpose(A) * A);

    vnl_matrix<double> atainvat = atainv * vnl_transpose(A);
    vnl_matrix<double> c = atainvat * b;

    for(unsigned int j = 0; j < nbCoefs; ++j)
      tmpCoefs[j] = c.get(j, 0);
    estFunction.SetCoefficients(tmpCoefs);

    return estFunction;
  }

private:
  ///
  TDateType m_DoySeries;
  TWeightType m_WeightSeries;
};
}
} //namespace otb
#endif
