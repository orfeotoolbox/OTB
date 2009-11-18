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
#ifndef __otbMeanDifference_h
#define __otbMeanDifference_h


namespace otb
{

/** \class MeanDifferenceImageFilter
 * \brief Implements neighborhood-wise the computation of mean difference.
 *
 * This filter is parametrized over the types of the two
 * input images and the type of the output image.
 *
 * Numeric conversions (castings) are done by the C++ defaults.
 *
 * The filter will walk over all the pixels in the two input images, and for
 * each one of them it will do the following:
 *
 * - cast the input 1 pixel value to \c double
 * - cast the input 2 pixel value to \c double
 * - compute the difference of the two pixel values
 * - compute the value of the difference of means
 * - cast the \c double value resulting to the pixel type of the output image
 * - store the casted value into the output image.
 *
 * The filter expect all images to have the same dimension
 * (e.g. all 2D, or all 3D, or all ND)
 *
 * \ingroup IntensityImageFilters Multithreaded
 */
namespace Functor
{

template< class TInput1, class TInput2, class TOutput>
class MeanDifference
{
public:
  MeanDifference() {};
  virtual ~MeanDifference() {};
  inline TOutput operator()( const TInput1 & itA,
                             const TInput2 & itB)
  {

    TOutput meanA = 0.0;
    TOutput meanB = 0.0;

    for (unsigned long pos = 0; pos< itA.Size(); ++pos)
    {

      meanA += static_cast<TOutput>(itA.GetPixel(pos));
      meanB += static_cast<TOutput>(itB.GetPixel(pos));


    }
    return static_cast<TOutput>( (meanA-meanB)/itA.Size() );
  }
};
}
}

#endif
