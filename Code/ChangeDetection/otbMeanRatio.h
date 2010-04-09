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
#ifndef __otbMeanRatio_h
#define __otbMeanRatio_h

#include "otbBinaryFunctorNeighborhoodImageFilter.h"

namespace otb
{

/** \class Functor::MeanRatio
 *
 * - compute the ratio of the two pixel values
 * - compute the value of the ratio of means
 * - cast the \c double value resulting to the pixel type of the output image
 * - store the casted value into the output image.
 *

 * \ingroup Functor
 */
namespace Functor
{

template<class TInput1, class TInput2, class TOutput>
class MeanRatio
{
public:
  MeanRatio() {}
  virtual ~MeanRatio() {}
  inline TOutput operator ()(const TInput1& itA,
                             const TInput2& itB)
  {

    TOutput meanA = 0.0;
    TOutput meanB = 0.0;

    for (unsigned long pos = 0; pos < itA.Size(); ++pos)
      {

      meanA += static_cast<TOutput>(itA.GetPixel(pos));
      meanB += static_cast<TOutput>(itB.GetPixel(pos));

      }

    meanA /= itA.Size();
    meanB /= itB.Size();

    //std::cout<<"meanA= "<<meanA<<", meanB= "<<meanB<<std::endl;

    TOutput ratio;

    if (meanA == meanB) ratio = 0.;
    else if (meanA > meanB) ratio = static_cast<TOutput>(1.0 - meanB / meanA);
    else ratio = static_cast<TOutput>(1.0 - meanA / meanB);

    return ratio;
  }
};
}
} // end namespace otb

#endif
