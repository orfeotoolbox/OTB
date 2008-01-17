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
#ifndef _otbStreamingTraits_h
#define _otbStreamingTraits_h

#include "otbMacro.h"
#include "otbConfigure.h"

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
  
/** \class StreamingTraits
 *  \brief This class is a helper class for terminal streaming filter implementation.
 * \sa StreamingImageFileWriter
 * \sa StreamingStatisticsImageFilter
 */  
template <class TImage>
class ITK_EXPORT StreamingTraits
{
public:
  /** Standard class typedefs. */
  typedef StreamingTraits  Self;

  typedef TImage ImageType;
  typedef typename ImageType::Pointer ImagePointerType;
  typedef typename ImageType::RegionType RegionType;
  typedef typename ImageType::InternalPixelType PixelType;
  
  typedef StreamingMode StreamingModeType;

  /** Dimension of input image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      ImageType::ImageDimension);

  typedef itk::ImageRegionSplitter<itkGetStaticConstMacro(ImageDimension)>  SplitterType;
   // ITK Interpolators
  typedef itk::InterpolateImageFunction<TImage,double>                      InterpolationType;
  typedef itk::BSplineInterpolateImageFunction<TImage,double>               BSplineInterpolationType;
  typedef itk::LinearInterpolateImageFunction<TImage,double>                LinearInterpolationType;
  typedef itk::NearestNeighborInterpolateImageFunction<TImage,double>       NearestNeighborInterpolationType;
  // OTB Interpolators
  // Gaussian Interpolators
  typedef WindowedSincInterpolateImageGaussianFunction<ImageType>          GaussianInterpolationType; 
  // Cosine Interpolators
  typedef WindowedSincInterpolateImageCosineFunction<ImageType>            CosineInterpolationType;
  // Hamming Interpolators 
  typedef WindowedSincInterpolateImageHammingFunction<ImageType>           HammingInterpolationType; 
  // Welch
  typedef WindowedSincInterpolateImageWelchFunction<ImageType>             WelchInterpolationType;
  // Lanczos
  typedef WindowedSincInterpolateImageLanczosFunction<ImageType>           LanczosInterpolationType; 
  // Blackman
  typedef WindowedSincInterpolateImageBlackmanFunction<ImageType>          BlackmanInterpolationType;
  // Prolate
  typedef otb::ProlateInterpolateImageFunction<ImageType>                  ProlateInterpolationType;
  

  /**
   * This method computes the number of streaming divisions, based on
   * the streaming mode and the image attributes. The last three parameters are ignored if
   * the mode does not need them.
   * \param image The image to stream, 
   * \param region the region to stream,
   * \param mode  the streaming mode,
   * \param numberOfStreamDivision the number of stream division if streaming mode is SET_NUMBER_OF_STREAM_DIVISIONS,
   * \param bufferMemorySize the buffer memory size in bytes if streaming mode is SET_BUFFER_MEMORY_SIZE,
   * \param bufferNumberOfLinesDivision the buffer number of lines division if the streaming mode is SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS.
   * \return The number of streaming divisions.
   */
  static unsigned long CalculateNumberOfStreamDivisions(const TImage * image,
                                                        RegionType region,
                                                        SplitterType * splitter,
                                                        StreamingModeType mode, 
                                                        unsigned long numberOfStreamDivision,
                                                        unsigned long bufferMemorySize,
                                                        unsigned long bufferNumberOfLinesDivisions);
         
         
  static unsigned int CalculateNeededRadiusForInterpolator(const InterpolationType* interpolator);
  
  static std::string GetMethodUseToCalculateNumberOfStreamDivisions(StreamingModeType mode);
         
};
}// End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingTraits.txx"
#endif


#endif
