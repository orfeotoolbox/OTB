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
#ifndef __otbStreamingTraits_h
#define __otbStreamingTraits_h

#include "otbMacro.h"

#include "itkBSplineInterpolateImageFunction.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "otbWindowedSincInterpolateImageGaussianFunction.h"
#include "otbWindowedSincInterpolateImageHammingFunction.h"
#include "otbWindowedSincInterpolateImageCosineFunction.h"
#include "otbWindowedSincInterpolateImageWelchFunction.h"
#include "otbWindowedSincInterpolateImageLanczosFunction.h"
#include "otbWindowedSincInterpolateImageBlackmanFunction.h"
#include "otbBCOInterpolateImageFunction.h"

#include "otbProlateInterpolateImageFunction.h"

namespace otb
{

/** \class StreamingTraits
 *  \brief This class provides internal information for streamable filters
 *
 *  \note
 *  This class is specialized for otb::VectorImage because VectorImage support less interpolator types
 *
 * \sa ImageFileWriter
 * \sa StreamingStatisticsImageFilter
 * \sa StreamingResampleImageFilter
 *
 * \ingroup OTBInterpolation
 */
template <class TImage>
class ITK_EXPORT StreamingTraits
{
public:
  /** Standard class typedefs. */
  typedef StreamingTraits Self;
  typedef TImage          ImageType;

  /** Dimension of input image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      ImageType::ImageDimension);

  // ITK Interpolators
  typedef itk::InterpolateImageFunction<ImageType, double>                InterpolationType;
  typedef itk::BSplineInterpolateImageFunction<ImageType, double>         BSplineInterpolationType;
  typedef itk::LinearInterpolateImageFunction<ImageType, double>          LinearInterpolationType;
  typedef itk::NearestNeighborInterpolateImageFunction<ImageType, double> NearestNeighborInterpolationType;

  // OTB Interpolators (supported for otb::Image)
  typedef WindowedSincInterpolateImageGaussianFunction<ImageType>         GaussianInterpolationType;
  typedef WindowedSincInterpolateImageCosineFunction<ImageType>           CosineInterpolationType;
  typedef WindowedSincInterpolateImageHammingFunction<ImageType>          HammingInterpolationType;
  typedef WindowedSincInterpolateImageWelchFunction<ImageType>            WelchInterpolationType;
  typedef WindowedSincInterpolateImageLanczosFunction<ImageType>          LanczosInterpolationType;
  typedef WindowedSincInterpolateImageBlackmanFunction<ImageType>         BlackmanInterpolationType;
  typedef ProlateInterpolateImageFunction<ImageType>                      ProlateInterpolationType;
  typedef BCOInterpolateImageFunction<ImageType>                          BCOInterpolationType;

  static unsigned int CalculateNeededRadiusForInterpolator(const InterpolationType* interpolator);
};

 /** \class StreamingTraits
  *  \brief This class provides internal information for streamable filters
  *
  *  \note
  *  This class is specialized for otb::VectorImage because VectorImage support less interpolator types
  *
  * \sa ImageFileWriter
  * \sa StreamingStatisticsImageFilter
  * \sa StreamingResampleImageFilter
 *
 * \ingroup OTBInterpolation
  */
template <typename TPixel, unsigned int VImageDimension>
class ITK_EXPORT StreamingTraits< otb::VectorImage<TPixel, VImageDimension> >
{
public:
  /** Standard class typedefs. */
  typedef StreamingTraits                           Self;
  typedef otb::VectorImage<TPixel, VImageDimension> ImageType;

  /** Dimension of input image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      ImageType::ImageDimension);

  // ITK Interpolators
  typedef itk::InterpolateImageFunction<ImageType, double>                InterpolationType;
  typedef itk::LinearInterpolateImageFunction<ImageType, double>          LinearInterpolationType;
  typedef itk::NearestNeighborInterpolateImageFunction<ImageType, double> NearestNeighborInterpolationType;

  // OTB Interpolators (supported for otb::VectorImage)
  typedef WindowedSincInterpolateImageGaussianFunction<ImageType>         GaussianInterpolationType;
  typedef BCOInterpolateImageFunction<ImageType>                          BCOInterpolationType;

  static unsigned int CalculateNeededRadiusForInterpolator(const InterpolationType* interpolator);
};


} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingTraits.txx"
#endif

#endif
