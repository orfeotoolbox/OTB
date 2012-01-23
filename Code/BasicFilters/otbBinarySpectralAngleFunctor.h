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
#ifndef __otbBinarySpectralAngleFunctor_h
#define __otbBinarySpectralAngleFunctor_h

#include "otbMath.h"

namespace otb
{
/** \class BinarySpectralAngleFunctor
 *  \brief This functor computes the spectral angle between two pixels.
 *
 *  It can be used as a functor in a BinaryFunctorImageFilter to
 *  compute the pixel-by-pixel spectral angles.
 */
namespace Functor
{
template<class TInput1, class TInput2, class TOutputValue>
class BinarySpectralAngleFunctor
{
public:
  BinarySpectralAngleFunctor()
  {
  }

  virtual ~BinarySpectralAngleFunctor() {}

  // Binary operator
  inline TOutputValue operator ()(const TInput1& a, const TInput2& b) const
  {
    const double Epsilon = 1E-10;
    double dist = 0.0;
    double scalarProd = 0.0;
    double norma = 0.0;
    double normb = 0.0;
    double sqrtNormProd = 0.0;
    for (unsigned int i = 0; i < std::min(a.Size(), b.Size()); ++i)
      {
      scalarProd += a[i] * b[i];
      norma += a[i] * a[i];
      normb += b[i] * b[i];
      }
    sqrtNormProd = vcl_sqrt(norma * normb);
    if ( vcl_abs(sqrtNormProd) < Epsilon || scalarProd / sqrtNormProd > 1 )
      {
      dist = 0.0;
      }
    else
      {
      dist = vcl_acos(scalarProd / sqrtNormProd);
      }

    return static_cast<TOutputValue>(dist);
  }
};

} // end namespace Functor
} // end namespace otb

#endif
