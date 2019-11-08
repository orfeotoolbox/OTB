/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbStreamingTraits_hxx
#define otbStreamingTraits_hxx

#include "otbStreamingTraits.h"

namespace otb
{
template <class TImage>
unsigned int StreamingTraits<TImage>::CalculateNeededRadiusForInterpolator(const InterpolationType* interpolator)
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
    neededRadius = dynamic_cast<const ProlateInterpolationType*>(interpolator)->GetRadius();
  }
  else if (className == "WindowedSincInterpolateImageGaussianFunction")
  {
    otbMsgDevMacro(<< "Gaussian Windowed Interpolator");
    neededRadius = dynamic_cast<const GaussianInterpolationType*>(interpolator)->GetRadius();
  }
  else if (className == "WindowedSincInterpolateImageHammingFunction")
  {
    otbMsgDevMacro(<< "Hamming Windowed Interpolator");
    neededRadius = dynamic_cast<const HammingInterpolationType*>(interpolator)->GetRadius();
  }
  else if (className == "WindowedSincInterpolateImageCosineFunction")
  {
    otbMsgDevMacro(<< "Cosine Windowed Interpolator");
    neededRadius = dynamic_cast<const CosineInterpolationType*>(interpolator)->GetRadius();
  }
  else if (className == "WindowedSincInterpolateImageWelchFunction")
  {
    otbMsgDevMacro(<< "Welch Windowed Interpolator");
    neededRadius = dynamic_cast<const WelchInterpolationType*>(interpolator)->GetRadius();
  }
  else if (className == "WindowedSincInterpolateImageLanczosFunction")
  {
    otbMsgDevMacro(<< "Lanczos Windowed Interpolator");
    neededRadius = dynamic_cast<const LanczosInterpolationType*>(interpolator)->GetRadius();
  }
  else if (className == "WindowedSincInterpolateImageBlackmanFunction")
  {
    otbMsgDevMacro(<< "Blackman Windowed Interpolator");
    neededRadius = dynamic_cast<const BlackmanInterpolationType*>(interpolator)->GetRadius();
  }
  else if (className == "BCOInterpolateImageFunction")
  {
    otbMsgDevMacro(<< "BCO Interpolator");
    neededRadius = dynamic_cast<const BCOInterpolationType*>(interpolator)->GetRadius();
  }
  return neededRadius;
}

template <typename TPixel, unsigned int VImageDimension>
unsigned int StreamingTraits<otb::VectorImage<TPixel, VImageDimension>>::CalculateNeededRadiusForInterpolator(const InterpolationType* interpolator)
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
    neededRadius = dynamic_cast<const GaussianInterpolationType*>(interpolator)->GetRadius();
  }
  else if (className == "BCOInterpolateImageFunction")
  {
    otbMsgDevMacro(<< "BCO Interpolator");
    neededRadius = dynamic_cast<const BCOInterpolationType*>(interpolator)->GetRadius();
  }

  return neededRadius;
}

} // End namespace otb

#endif
