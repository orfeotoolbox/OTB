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
#ifndef __otbTimeSeries_h
#define __otbTimeSeries_h

#include "itkFixedArray.h"

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


private:
  CoefficientsType m_Coefficients;
};
}
#endif
