/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbTimeSeries_h
#define otbTimeSeries_h

#include "itkFixedArray.h"
#include <cmath>

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
      tmpVal += m_Coefficients[i]*std::pow(val, static_cast<CoefficientsPrecisionType>(i));
    return tmpVal;
  }

private:
  CoefficientsType m_Coefficients;
};
}
#endif
