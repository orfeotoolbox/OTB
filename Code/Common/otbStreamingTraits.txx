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
#ifndef __otbStreamingTraits_txx
#define __otbStreamingTraits_txx

#include <boost/cstdint.hpp>

#include "otbStreamingTraits.h"
#include "otbMacro.h"
#include "otbConfigure.h"
#include "otbConfigurationFile.h"

#include "itkInterpolateImageFunction.h"
#include "itkBSplineInterpolateImageFunction.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkNearestNeighborInterpolateImageFunction.h"

namespace otb
{

template <class TImage>
unsigned long StreamingTraitsBase<TImage>
::CalculateNumberOfStreamDivisions(const TImage * image,
                                   RegionType region,
                                   SplitterType * splitter,
                                   StreamingModeType mode,
                                   unsigned long numberOfStreamDivision,
                                   unsigned long bufferMemorySize,
                                   unsigned long bufferNumberOfLinesDivisions)
{
  using boost::uint64_t;
  unsigned long numDivisions(0);

  switch (mode)
    {
    case SET_TILING_WITH_SET_NUMBER_OF_STREAM_DIVISIONS: // Just like SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS
    case SET_NUMBER_OF_STREAM_DIVISIONS:
      {
      numDivisions = numberOfStreamDivision;
      }
      break;
    case SET_BUFFER_MEMORY_SIZE:
      {
      const uint64_t bufferMemorySizeOctet = bufferMemorySize / 8;
      const uint64_t numberColumnsOfRegion = region.GetSize()[0]; // X dimension
      const uint64_t sizeLine = static_cast<uint64_t>(
                                     numberColumnsOfRegion * \
                                     image->GetNumberOfComponentsPerPixel() * \
                                     sizeof(PixelType));

      uint64_t regionSize = static_cast<uint64_t>(region.GetSize()[1]) * sizeLine;

      otbMsgDevMacro(<< "image->GetNumberOfComponentsPerPixel()   = " << image->GetNumberOfComponentsPerPixel());
      otbMsgDevMacro(<< "sizeof(PixelType)                        = " << sizeof(PixelType));
      otbMsgDevMacro(<< "numberColumnsOfRegion                    = " << numberColumnsOfRegion);
      otbMsgDevMacro(<< "sizeLine                                 = " << sizeLine);
      otbMsgDevMacro(<< "regionSize                               = " << regionSize);
      otbMsgDevMacro(<< "BufferMemorySize                         = " << bufferMemorySize);
      otbMsgDevMacro(<< "bufferMemorySizeOctet                    = " << bufferMemorySizeOctet);

      //Active streaming
      if (regionSize > bufferMemorySizeOctet)
        {
        //The regionSize must be at least equal to the sizeLine
        if (regionSize < sizeLine)
          {
          otbMsgDevMacro(<< "Force buffer size.");
          regionSize = sizeLine;
          }
        //Calculate NumberOfStreamDivisions
        numDivisions = static_cast<unsigned long>(
          vcl_ceil(static_cast<double>(regionSize) / static_cast<double>(bufferMemorySizeOctet))
          );
        }
      else
        {
        //No streaming
        numDivisions = 1;
        }

      }
      break;
    case SET_BUFFER_NUMBER_OF_LINES:
      {
      if (bufferNumberOfLinesDivisions < 1)
        {
        itkGenericExceptionMacro(<< "Buffer number of lines division must be greater than 0 !");
        }
      /* Calculate number of split */
      unsigned long numberLinesOfRegion = region.GetSize()[1]; // Y dimension
      if (numberLinesOfRegion > bufferNumberOfLinesDivisions)
        {
        numDivisions =
          static_cast<unsigned long>(vcl_ceil(static_cast<double>(numberLinesOfRegion) /
                                              static_cast<double>(bufferNumberOfLinesDivisions)));
        }
      else
        {
        //Non streaming
        numDivisions = 1;
        }
      }
      break;
    case SET_TILING_WITH_SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS: // Just like SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS
    case SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS:
      {
      typedef otb::ConfigurationFile ConfigurationType;
      ConfigurationType::Pointer conf = ConfigurationType::GetInstance();
      uint64_t streamMaxSizeBufferForStreamingInBytes;
      uint64_t streamImageSizeToActivateStreamingInBytes;
      try
        {
        streamMaxSizeBufferForStreamingInBytes = conf->GetParameter<uint64_t>(
          "OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING");
        streamImageSizeToActivateStreamingInBytes = conf->GetParameter<uint64_t>(
          "OTB_STREAM_IMAGE_SIZE_TO_ACTIVATE_STREAMING");
        }
      catch(...)
        {
        // We should never have to go here if the configuration file is
        // correct and found. In case it is not fallback on the cmake
        // defined constants.
        streamMaxSizeBufferForStreamingInBytes = static_cast<uint64_t>(OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING);
        streamImageSizeToActivateStreamingInBytes = static_cast<uint64_t>(OTB_STREAM_IMAGE_SIZE_TO_ACTIVATE_STREAMING);
        }

      const uint64_t numberColumnsOfRegion = region.GetSize()[0]; // X dimension
      const uint64_t sizeLineInBytes
              = static_cast<uint64_t>( numberColumnsOfRegion
                                        * image->GetNumberOfComponentsPerPixel()
                                        * sizeof(PixelType) );

      const uint64_t regionSizeInBytes = static_cast<uint64_t>(region.GetSize()[1]) * sizeLineInBytes;
      otbMsgDevMacro(<< "streamImageSizeToActivateStreaming in Bytes  = " << streamImageSizeToActivateStreamingInBytes);
      otbMsgDevMacro(<< "streamMaxSizeBufferForStreaming in Bytes     = " << streamMaxSizeBufferForStreamingInBytes);
      otbMsgDevMacro(<< "image->GetNumberOfComponentsPerPixel()       = " << image->GetNumberOfComponentsPerPixel());
      otbMsgDevMacro(<< "sizeof(PixelType)                            = " << sizeof(PixelType));
      otbMsgDevMacro(<< "numberColumnsOfRegion                        = " << numberColumnsOfRegion);
      otbMsgDevMacro(<< "sizeLine in Bytes                            = " << sizeLineInBytes);
      otbMsgDevMacro(<< "regionSizeInBytes                            = " << regionSizeInBytes);

      if ( regionSizeInBytes > streamImageSizeToActivateStreamingInBytes )
        {
        // Activate streaming

        // If no tiling, make sure there is at least one line
        if ( mode != SET_TILING_WITH_SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS
            && mode != SET_TILING_WITH_SET_NUMBER_OF_STREAM_DIVISIONS
            && (streamMaxSizeBufferForStreamingInBytes < sizeLineInBytes))
          {
          otbMsgDevMacro(<< "Force buffer size.");
          streamMaxSizeBufferForStreamingInBytes = sizeLineInBytes;
          }

        otbMsgDevMacro(<< "streamMaxSizeBufferForStreaming in Bytes     = " << streamMaxSizeBufferForStreamingInBytes);

        //Calculate NumberOfStreamDivisions
        numDivisions =
          static_cast<unsigned long>(vcl_ceil(static_cast<double>(regionSizeInBytes) /
                                              static_cast<double>(streamMaxSizeBufferForStreamingInBytes)));
        }
      else
        {
        //Non streaming
        numDivisions = 1;
        }

      }
      break;
    default:
      itkGenericExceptionMacro(<< "Method use to calculate number of stream divisions is not set !");
      break;
    }

  if (numDivisions == 0) numDivisions = 1;
  otbMsgDevMacro(<< " -> Resume : method : " << mode << "\n -> Number of divisions = " << numDivisions);

  // Compute real number of splitter
  unsigned int numDivisionsFromSplitter = splitter->GetNumberOfSplits(region, numDivisions);

  /** In tiling streaming mode, we keep the number of divisions calculed by splitter */
  if ((numDivisionsFromSplitter < numDivisions)
      || (mode == SET_TILING_WITH_SET_NUMBER_OF_STREAM_DIVISIONS)
      || (mode == SET_TILING_WITH_SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS)
      )
    {
    numDivisions = numDivisionsFromSplitter;
    }

  return (numDivisions);
}


template <class TImage>
std::string StreamingTraitsBase<TImage>
::GetMethodUseToCalculateNumberOfStreamDivisions(StreamingModeType mode)
{
  switch (mode)
    {
    case SET_NUMBER_OF_STREAM_DIVISIONS:
      return "CalculationDivisionEnumType::SET_NUMBER_OF_STREAM_DIVISIONS";
      break;
    case SET_BUFFER_MEMORY_SIZE:
      return "CalculationDivisionEnumType::SET_BUFFER_MEMORY_SIZE";
      break;
    case SET_BUFFER_NUMBER_OF_LINES:
      return "CalculationDivisionEnumType::SET_BUFFER_NUMBER_OF_LINES";
      break;
    case SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS:
      return "CalculationDivisionEnumType::SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS";
      break;
    case SET_TILING_WITH_SET_NUMBER_OF_STREAM_DIVISIONS:
      return "CalculationDivisionEnumType::SET_TILING_WITH_SET_NUMBER_OF_STREAM_DIVISIONS";
      break;
    case SET_TILING_WITH_SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS:
      return "CalculationDivisionEnumType::SET_TILING_WITH_SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS";
      break;
    default:
      return "unknown";
      break;
    }

}


template <class TImage>
unsigned int StreamingTraits<TImage>
::CalculateNeededRadiusForInterpolator(const InterpolationType* interpolator)
{
  unsigned int neededRadius = 0;
  std::string  className;

  className = interpolator->GetNameOfClass();

  if (className == "LinearInterpolateImageFunction")
    {
    otbMsgDevMacro(<< "Linear Interpolator");
    neededRadius = 1;
    }
  else if (className == "NearestNeighborInterpolateImageFunction")
    {
    otbMsgDevMacro(<< "Nearest Neighbor Interpolator");
    neededRadius = 1;
    }
  else if (className == "BSplineInterpolateImageFunction")
    {
    otbMsgDevMacro(<< "BSpline Interpolator");
    neededRadius = 2;
    }
  else if (className == "ProlateInterpolateImageFunction")
    {
    otbMsgDevMacro(<< "Prolate Interpolator");
    neededRadius = dynamic_cast<const ProlateInterpolationType *>(interpolator)->GetRadius();
    }
  else if (className == "WindowedSincInterpolateImageGaussianFunction")
    {
    otbMsgDevMacro(<< "Gaussian Windowed Interpolator");
    neededRadius = dynamic_cast<const GaussianInterpolationType *>(interpolator)->GetRadius();
    }
  else if (className == "WindowedSincInterpolateImageHammingFunction")
    {
    otbMsgDevMacro(<< "Hamming Windowed Interpolator");
    neededRadius = dynamic_cast<const HammingInterpolationType *>(interpolator)->GetRadius();
    }
  else if (className == "WindowedSincInterpolateImageCosineFunction")
    {
    otbMsgDevMacro(<< "Cosine Windowed Interpolator");
    neededRadius = dynamic_cast<const CosineInterpolationType *>(interpolator)->GetRadius();
    }
  else if (className == "WindowedSincInterpolateImageWelchFunction")
    {
    otbMsgDevMacro(<< "Welch Windowed Interpolator");
    neededRadius = dynamic_cast<const WelchInterpolationType *>(interpolator)->GetRadius();
    }
  else if (className == "WindowedSincInterpolateImageLanczosFunction")
    {
    otbMsgDevMacro(<< "Lanczos Windowed Interpolator");
    neededRadius = dynamic_cast<const LanczosInterpolationType *>(interpolator)->GetRadius();
    }
  else if (className == "WindowedSincInterpolateImageBlackmanFunction")
    {
    otbMsgDevMacro(<< "Blackman Windowed Interpolator");
    neededRadius = dynamic_cast<const BlackmanInterpolationType *>(interpolator)->GetRadius();
    }
  else if (className == "BCOInterpolateImageFunction")
    {
    otbMsgDevMacro(<< "BCO Interpolator");
    neededRadius = dynamic_cast<const BCOInterpolationType *>(interpolator)->GetRadius();
    }


  /*else if (className == "WindowedSincInterpolateImageFunction")
  {
  itkGenericExceptionMacro(<< "Windowed Sinc Interpolator not supported yet in resample");
  otbMsgDevMacro(<<"Windowed Sinc Interpolator not supported yet in resample");
  //                dynamic_cast<typename itk::WindowedSincInterpolateImageFunction*>(interpolator);
  }
  else
  {
  itkGenericExceptionMacro(<< "Interpolator not recognized, please choose another type !");
  }        */

  return neededRadius;
}

template <typename TPixel, unsigned int VImageDimension>
unsigned int StreamingTraits< otb::VectorImage<TPixel, VImageDimension> >
::CalculateNeededRadiusForInterpolator(const InterpolationType* interpolator)
{
  unsigned int neededRadius = 0;
  std::string  className;

  className = interpolator->GetNameOfClass();

  if (className == "LinearInterpolateImageFunction")
    {
    otbMsgDevMacro(<< "Linear Interpolator");
    neededRadius = 1;
    }
  else if (className == "NearestNeighborInterpolateImageFunction")
    {
    otbMsgDevMacro(<< "Nearest Neighbor Interpolator");
    neededRadius = 1;
    }
  else if (className == "BSplineInterpolateImageFunction")
    {
    otbMsgDevMacro(<< "BSpline Interpolator");
    neededRadius = 2;
    }
  else if (className == "WindowedSincInterpolateImageGaussianFunction")
    {
    otbMsgDevMacro(<< "Gaussian Windowed Interpolator");
    neededRadius = dynamic_cast<const GaussianInterpolationType *>(interpolator)->GetRadius();
    }
  else if (className == "BCOInterpolateImageFunction")
    {
    otbMsgDevMacro(<< "BCO Interpolator");
    neededRadius = dynamic_cast<const BCOInterpolationType *>(interpolator)->GetRadius();
    }

  /*else if (className == "WindowedSincInterpolateImageFunction")
  {
  itkGenericExceptionMacro(<< "Windowed Sinc Interpolator not supported yet in resample");
  otbMsgDevMacro(<<"Windowed Sinc Interpolator not supported yet in resample");
  //                dynamic_cast<typename itk::WindowedSincInterpolateImageFunction*>(interpolator);
  }
  else
  {
  itkGenericExceptionMacro(<< "Interpolator not recognized, please choose another type !");
  }        */

  return neededRadius;
}

} // End namespace otb

#endif
