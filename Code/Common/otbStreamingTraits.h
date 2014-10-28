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

#include "itkInterpolateImageFunction.h"
#include "itkBSplineInterpolateImageFunction.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkImageRegionSplitter.h"
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

/**
 * this enum defines the different streaming mode available in OTB.
   */
typedef enum
{
  SET_NUMBER_OF_STREAM_DIVISIONS = 1,
  SET_BUFFER_MEMORY_SIZE = 2,
  SET_BUFFER_NUMBER_OF_LINES = 3,
  SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS = 4,
  SET_TILING_WITH_SET_NUMBER_OF_STREAM_DIVISIONS = 5,
  SET_TILING_WITH_SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS = 6
} StreamingMode;

/** \class StreamingTraitsBase
 *  \brief This class is the base class for StreamingTraits
 *
 *  It provides common definitions to all StreamingTraits specializations
 *
 * \sa ImageFileWriter
 * \sa StreamingStatisticsImageFilter
 * \sa StreamingResampleImageFilter
 */
template<class TImage>
class ITK_EXPORT StreamingTraitsBase
{
public:
  /** Standard class typedefs. */
  typedef StreamingTraitsBase Self;

  typedef TImage                                 ImageType;
  typedef typename ImageType::Pointer            ImagePointerType;
  typedef typename ImageType::RegionType         RegionType;
  typedef typename ImageType::InternalPixelType  PixelType;

  typedef StreamingMode StreamingModeType;

  /** Dimension of input image. */
  itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

  typedef itk::ImageRegionSplitter<itkGetStaticConstMacro(ImageDimension)> SplitterType;

  /**
   * This method computes the number of streaming divisions, based on
   * the streaming mode and the image attributes. The last three parameters are ignored if
   * the mode does not need them.
   * \param image The image to stream,
   * \param region the region to stream,
   * \param splitter the splitter used for the division,
   * \param mode  the streaming mode,
   * \param numberOfStreamDivision the number of stream division if streaming mode is SET_NUMBER_OF_STREAM_DIVISIONS,
   * \param bufferMemorySize the buffer memory size in bytes if streaming mode is SET_BUFFER_MEMORY_SIZE,
   * \param bufferNumberOfLinesDivisions the buffer number of lines division if the streaming mode is SET_BUFFER_NUMBER_OF_LINES.
   * \return The number of streaming divisions.
   */
  static unsigned long CalculateNumberOfStreamDivisions(const TImage * image,
                                                        RegionType region,
                                                        SplitterType * splitter,
                                                        StreamingModeType mode,
                                                        unsigned long numberOfStreamDivision,
                                                        unsigned long bufferMemorySize,
                                                        unsigned long bufferNumberOfLinesDivisions);

  static std::string GetMethodUseToCalculateNumberOfStreamDivisions(StreamingModeType mode);

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
 */
template <class TImage>
class ITK_EXPORT StreamingTraits
 : public StreamingTraitsBase<TImage>
{
public:
  /** Standard class typedefs. */
  typedef StreamingTraits             Self;
  typedef StreamingTraitsBase<TImage> Superclass;

  typedef typename Superclass::ImageType         ImageType;
  typedef typename Superclass::ImagePointerType  ImagePointerType;
  typedef typename Superclass::RegionType        RegionType;
  typedef typename Superclass::PixelType         PixelType;
  typedef typename Superclass::StreamingModeType StreamingModeType;

  /** Dimension of input image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      ImageType::ImageDimension);

  typedef typename Superclass::SplitterType     SplitterType;

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
  */
template <typename TPixel, unsigned int VImageDimension>
class ITK_EXPORT StreamingTraits< otb::VectorImage<TPixel, VImageDimension> >
 : public StreamingTraitsBase<otb::VectorImage<TPixel, VImageDimension> >
{
public:
  /** Standard class typedefs. */
  typedef StreamingTraits                          Self;
  typedef StreamingTraitsBase
      < otb::VectorImage<TPixel, VImageDimension> > Superclass;

  typedef typename Superclass::ImageType         ImageType;
  typedef typename Superclass::ImagePointerType  ImagePointerType;
  typedef typename Superclass::RegionType        RegionType;
  typedef typename Superclass::PixelType         PixelType;
  typedef typename Superclass::StreamingModeType StreamingModeType;

  /** Dimension of input image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      ImageType::ImageDimension);

  typedef typename Superclass::SplitterType     SplitterType;

  // ITK Interpolators
  typedef itk::InterpolateImageFunction<ImageType, double>                InterpolationType;
  //typedef itk::BSplineInterpolateImageFunction<ImageType, double>         BSplineInterpolationType;
  typedef itk::LinearInterpolateImageFunction<ImageType, double>          LinearInterpolationType;
  typedef itk::NearestNeighborInterpolateImageFunction<ImageType, double> NearestNeighborInterpolationType;

  // OTB Interpolators (supported for otb::VectorImage)
  typedef WindowedSincInterpolateImageGaussianFunction<ImageType>         GaussianInterpolationType;
  typedef BCOInterpolateImageFunction<ImageType>                          BCOInterpolationType;

  //typedef WindowedSincInterpolateImageCosineFunction<ImageType>        CosineInterpolationType;
  //typedef WindowedSincInterpolateImageHammingFunction<ImageType>       HammingInterpolationType;
  //typedef WindowedSincInterpolateImageWelchFunction<ImageType>         WelchInterpolationType;
  //typedef WindowedSincInterpolateImageLanczosFunction<ImageType>       LanczosInterpolationType;
  //typedef WindowedSincInterpolateImageBlackmanFunction<ImageType>      BlackmanInterpolationType;
  //typedef ProlateInterpolateImageFunction<ImageType>                   ProlateInterpolationType;

  static unsigned int CalculateNeededRadiusForInterpolator(const InterpolationType* interpolator);
};


} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingTraits.txx"
#endif

#endif
