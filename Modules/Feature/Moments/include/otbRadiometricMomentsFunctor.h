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
#ifndef __otbRadiometricMomentsFunctor_h
#define __otbRadiometricMomentsFunctor_h

#include "itkVariableLengthVector.h"
#include "otbMath.h"

namespace otb
{
namespace Functor
{
/** \class RadiometricMomentsFunctor
 *  \brief
 *
 *  \ingroup Functor
 */
template <class TNeighIter, class TPrecision = float>
class RadiometricMomentsFunctor
{
public:

  typedef RadiometricMomentsFunctor                    Self;
  typedef TNeighIter                                   IteratorType;
  typedef typename IteratorType::PixelType             PixelType;
  typedef TPrecision                                   ScalarRealType;
  typedef itk::VariableLengthVector< ScalarRealType  > OutputType;

  RadiometricMomentsFunctor() {}
  ~RadiometricMomentsFunctor() {}

  inline OutputType operator ()(TNeighIter& it) const
  {
    OutputType moments;
    moments.SetSize(4);
    moments.Fill( itk::NumericTraits< ScalarRealType >::Zero );

    ScalarRealType sum1, sum2, sum3, sum4;
    sum1 = itk::NumericTraits< ScalarRealType >::Zero;
    sum2 = itk::NumericTraits< ScalarRealType >::Zero;
    sum3 = itk::NumericTraits< ScalarRealType >::Zero;
    sum4 = itk::NumericTraits< ScalarRealType >::Zero;

    //it.GoToBegin();
    const unsigned int size = it.Size();
    for (unsigned int i = 0; i < size; ++i)
      {
      ScalarRealType value = static_cast<ScalarRealType>(it.GetPixel(i));
      ScalarRealType value2 = value * value;

     // Update cumulants
      sum1 += value;
      sum2 += value2;
      sum3 += value * value2;
      sum4 += value2 * value2;
      }

    // final computations
    // Mean
    moments[0] = sum1 / size;
    // Variance
    moments[1] = (sum2 - (sum1 * moments[0])) / (size - 1);

    ScalarRealType sigma      = vcl_sqrt(moments[1]);
    ScalarRealType mean2      = moments[0] * moments[0];

    const double epsilon = 1E-10;
    if (vcl_abs(moments[1]) > epsilon)
      {
      // Skewness
      moments[2] = ((sum3 - 3.0 * moments[0] * sum2) / size + 2.0 * moments[0] * mean2) / (moments[1] * sigma);
      // Kurtosis
      moments[3] = ((sum4 - 4.0 * moments[0] * sum3 + 6.0 * mean2 * sum2) / size - 3.0 * mean2 * mean2)
              / (moments[1] * moments[1]) - 3.0;
      }

    return moments;
  }

};

}
}

#endif
