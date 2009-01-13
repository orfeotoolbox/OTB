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

#include "otbStreamingTraits.h"
#include "otbMacro.h"
#include "otbConfigure.h"

#include "itkInterpolateImageFunction.h"
#include "itkBSplineInterpolateImageFunction.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkNearestNeighborInterpolateImageFunction.h"

namespace otb
{


template <class TImage>
unsigned long StreamingTraits<TImage>::CalculateNumberOfStreamDivisions(const TImage * image,
                                                        RegionType region,
                                                        SplitterType * splitter,
                                                        StreamingModeType mode,
                                                        unsigned long numberOfStreamDivision,
                                                        unsigned long bufferMemorySize,
                                                        unsigned long bufferNumberOfLinesDivisions)
{
        unsigned long numDivisions(0);

        switch(mode)
        {
                case SET_TILING_WITH_SET_NUMBER_OF_STREAM_DIVISIONS : // Just like SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS
                case SET_NUMBER_OF_STREAM_DIVISIONS :
                {
                        numDivisions = numberOfStreamDivision;
                }
                break;
                case SET_BUFFER_MEMORY_SIZE :
                {
                        const unsigned long bufferMemorySizeOctet = bufferMemorySize/8;
                        unsigned long numberColumnsOfRegion = region.GetSize()[0]; // X dimension
                        const unsigned long sizeLine = numberColumnsOfRegion * \
                            image->GetNumberOfComponentsPerPixel() * \
                            sizeof(PixelType);
                        unsigned long regionSize = region.GetSize()[1] * sizeLine;
                        otbMsgDevMacro(<<"image->GetNumberOfComponentsPerPixel()   = "<<image->GetNumberOfComponentsPerPixel());
                        otbMsgDevMacro(<<"sizeof(PixelType)                        = "<<sizeof(PixelType));
                        otbMsgDevMacro(<<"numberColumnsOfRegion                    = "<<numberColumnsOfRegion);
                        otbMsgDevMacro(<<"sizeLine                                 = "<<sizeLine);
                        otbMsgDevMacro(<<"regionSize                               = "<<regionSize);
                        otbMsgDevMacro(<<"BufferMemorySize                         = "<<bufferMemorySize);
                        otbMsgDevMacro(<<"bufferMemorySizeOctet                    = "<<bufferMemorySizeOctet);

                        //Active streaming
                        if( regionSize > bufferMemorySizeOctet )
                        {
                                //The regionSize must be at list equal to the sizeLine
                                if( regionSize < sizeLine )
                                {
                                        otbMsgDevMacro(<<"Force buffer size.");
                                        regionSize = sizeLine;
                                }
                                //Calculate NumberOfStreamDivisions
                                numDivisions = static_cast<unsigned long>(vcl_ceil(static_cast<double>(regionSize)/static_cast<double>(bufferMemorySizeOctet)));
                        }
                        else
                        {
                                //Non streaming
                                numDivisions = 1;
                        }

                }
                break;
                case SET_BUFFER_NUMBER_OF_LINES :
                {
                        if( bufferNumberOfLinesDivisions < 1 )
                        {
                                itkGenericExceptionMacro(<<"Buffer number of lines division must be greater than 0 !");
                        }
                        /* Calculate number of split */
                        unsigned long numberLinesOfRegion = region.GetSize()[1]; // Y dimension
                        if( numberLinesOfRegion > bufferNumberOfLinesDivisions )
                        {
                                numDivisions = static_cast<unsigned long>(vcl_ceil(static_cast<double>(numberLinesOfRegion)/static_cast<double>(bufferNumberOfLinesDivisions)));
                        }
                        else
                        {
                                //Non streaming
                                numDivisions = 1;
                        }
                }
                break;
                case SET_TILING_WITH_SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS  : // Just like SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS
                case SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS :
                {
                        const unsigned long streamMaxSizeBufferForStreamingBytes = OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING;
                        const unsigned long streamImageSizeToActivateStreamingBytes = OTB_STREAM_IMAGE_SIZE_TO_ACTIVATE_STREAMING;
                        //Convert in octet unit
                        unsigned long streamMaxSizeBufferForStreaming = streamMaxSizeBufferForStreamingBytes/8;
                        const unsigned long streamImageSizeToActivateStreaming = streamImageSizeToActivateStreamingBytes/8;

                                        unsigned long numberColumnsOfRegion = region.GetSize()[0]; // X dimension
                        const unsigned long sizeLine = numberColumnsOfRegion * \
                                                                                              image->GetNumberOfComponentsPerPixel() * \
                                                                                              sizeof(PixelType);
                        const unsigned long regionSize = region.GetSize()[1] * sizeLine;
                        otbMsgDevMacro(<<"streamImageSizeToActivateStreaming in Bytes  = "<<streamImageSizeToActivateStreamingBytes);
                        otbMsgDevMacro(<<"streamMaxSizeBufferForStreaming in Bytes     = "<<streamMaxSizeBufferForStreamingBytes);
                        otbMsgDevMacro(<<"streamImageSizeToActivateStreaming           = "<<streamImageSizeToActivateStreaming);
                        otbMsgDevMacro(<<"streamMaxSizeBufferForStreaming              = "<<streamMaxSizeBufferForStreaming);
                        otbMsgDevMacro(<<"image->GetNumberOfComponentsPerPixel()   = "<<image->GetNumberOfComponentsPerPixel());
                        otbMsgDevMacro(<<"sizeof(PixelType)                 = "<<sizeof(PixelType));
                        otbMsgDevMacro(<<"numberColumnsOfRegion                        = "<<numberColumnsOfRegion);
                        otbMsgDevMacro(<<"sizeLine                                     = "<<sizeLine);
                        otbMsgDevMacro(<<"regionSize                                   = "<<regionSize);

                        //Active streaming
                        if( regionSize > streamImageSizeToActivateStreaming )
                        {
                                //On s'assure que la taille du bandeau fait au moins une ligne de l'image si pas TILING
                                if( (mode != SET_TILING_WITH_SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS) && (streamMaxSizeBufferForStreaming < sizeLine) )
                                {
                                        otbMsgDevMacro(<<"Force buffer size.");
                                        streamMaxSizeBufferForStreaming = sizeLine;
                                }
                                otbMsgDevMacro(<<"Buffer size : "<<streamMaxSizeBufferForStreaming);
                                //Calculate NumberOfStreamDivisions
                                numDivisions = static_cast<unsigned long>(vcl_ceil(static_cast<double>(regionSize)/static_cast<double>(streamMaxSizeBufferForStreaming)));
                        }
                        else
                        {
                                //Non streaming
                                numDivisions = 1;
                        }

                }
                break;
                default :
                        itkGenericExceptionMacro(<<"Method use to calculate number of stream divisions is not set !");
                break;
        }
        if( numDivisions == 0) numDivisions = 1;
        otbMsgDevMacro(<<" -> Resume : method : "<<mode<<"\n -> Number of divisions = "<<numDivisions);

        // Compute real number of splitter
        unsigned int numDivisionsFromSplitter = splitter->GetNumberOfSplits(region, numDivisions);

        /** In tiling streaming mode, we keep the number of divisions calculed by splitter */
        if ((numDivisionsFromSplitter < numDivisions)||(mode==SET_TILING_WITH_SET_NUMBER_OF_STREAM_DIVISIONS)||(mode==SET_TILING_WITH_SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS))
        {
                numDivisions = numDivisionsFromSplitter;
        }

        return(numDivisions);
}


template <class TImage>
unsigned int StreamingTraits<TImage>::CalculateNeededRadiusForInterpolator(const InterpolationType* interpolator)
{
        unsigned int neededRadius = 0;
        std::string className;

        className = interpolator->GetNameOfClass();

        if (className == "LinearInterpolateImageFunction")
    {
      otbMsgDevMacro(<<"Linear Interpolator");
      neededRadius = 1;
    }
        else if (className == "NearestNeighborInterpolateImageFunction")
    {
      otbMsgDevMacro(<<"Nearest Neighbor Interpolator");
      neededRadius = 1;
    }
        else if (className == "BSplineInterpolateImageFunction")
    {
      otbMsgDevMacro(<<"Nearest Neighbor Interpolator");
      neededRadius = 2;
    }
         else if (className == "ProlateInterpolateImageFunction")
    {
      otbMsgDevMacro(<<"Prolate Interpolator");
      neededRadius = dynamic_cast<const ProlateInterpolationType *>(interpolator)->GetRadius();
    }
  else if (className == "WindowedSincInterpolateImageGaussianFunction")
    {
      otbMsgDevMacro(<<"Gaussian Windowed Interpolator");
      neededRadius = dynamic_cast<const GaussianInterpolationType *>(interpolator)->GetRadius();
    }
  else if (className == "WindowedSincInterpolateImageHammingFunction")
    {
      otbMsgDevMacro(<<"Hamming Windowed Interpolator");
      neededRadius = dynamic_cast<const HammingInterpolationType *>(interpolator)->GetRadius();
    }
  else if (className == "WindowedSincInterpolateImageCosineFunction")
    {
      otbMsgDevMacro(<<"Cosine Windowed Interpolator");
      neededRadius = dynamic_cast<const CosineInterpolationType *>(interpolator)->GetRadius();
    }
  else if (className == "WindowedSincInterpolateImageWelchFunction")
    {
      otbMsgDevMacro(<<"Welch Windowed Interpolator");
      neededRadius = dynamic_cast<const WelchInterpolationType *>(interpolator)->GetRadius();
    }
  else if (className == "WindowedSincInterpolateImageLanczosFunction")
    {
      otbMsgDevMacro(<<"Lanczos Windowed Interpolator");
      neededRadius = dynamic_cast<const LanczosInterpolationType *>(interpolator)->GetRadius();
    }
  else if (className == "WindowedSincInterpolateImageBlackmanFunction")
    {
      otbMsgDevMacro(<<"Blackman Windowed Interpolator");
      neededRadius = dynamic_cast<const BlackmanInterpolationType *>(interpolator)->GetRadius();
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


template <class TImage>
std::string StreamingTraits<TImage>::GetMethodUseToCalculateNumberOfStreamDivisions( StreamingModeType mode )
{
  switch(mode)
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


}// End namespace otb

#endif
