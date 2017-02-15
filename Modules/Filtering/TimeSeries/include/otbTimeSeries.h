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
#ifndef otbTimeSeries_h
#define otbTimeSeries_h

#include "itkFixedArray.h"
#include <vcl_cmath.h>

namespace otb
{
template <unsigned int TDegree=2, class TCoefPrecision = double>
class PolynomialTimeSeries
{
public:

  typedef TCoefPrecision CoefficientsPrecisionType;
  typedef itk::FixedArray< CoefficientsPrecisionType, TDegree+1 > CoefficientsType;


  /// Constructor
  PolynomialTimeSeries()
  {
  }
  /// Destructor
  virtual ~PolynomialTimeSeries() {}

  inline void SetCoefficients(CoefficientsType& coeffs)
  {
    for(unsigned int i=0; i<=TDegree; ++i)
      m_Coefficients[i] = coeffs[i];
  }

  inline TCoefPrecision GetCoefficient(unsigned int co) const
  {
    return m_Coefficients[co];
  }

  inline CoefficientsType GetCoefficients() const
  {
    return m_Coefficients;
  }

  inline CoefficientsPrecisionType GetValue( CoefficientsPrecisionType val) const
  {
    CoefficientsPrecisionType tmpVal = 0;
    for( unsigned int i=0; i<=TDegree; ++i)
      tmpVal += m_Coefficients[i]*vcl_pow(val, static_cast<CoefficientsPrecisionType>(i));
    return tmpVal;
  }

private:
  CoefficientsType m_Coefficients;
};
}
#endif
