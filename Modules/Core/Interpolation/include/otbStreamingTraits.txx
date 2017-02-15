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
#ifndef otbStreamingTraits_txx
#define otbStreamingTraits_txx

#include "otbStreamingTraits.h"

namespace otb
{
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

  return neededRadius;
}

} // End namespace otb

#endif
