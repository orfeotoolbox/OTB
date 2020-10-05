/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#ifndef otbRGBAPixelConverter_hxx
#define otbRGBAPixelConverter_hxx

#include "otbRGBAPixelConverter.h"

namespace otb
{

// Base
template <class TInternalInputPixelType, class TOutputPixelType>
typename RGBAPixelConverter<TInternalInputPixelType, TOutputPixelType>::OutputPixelType
RGBAPixelConverter<TInternalInputPixelType, TOutputPixelType>::Convert(InputPixelType input)
{
  double result;

  result = ((2125.0 * static_cast<double>(input[0]) + 7154.0 * static_cast<double>(input[1]) + 0721.0 * static_cast<double>(input[2])) /
            10000.0); // alpha not supported yet

  return (static_cast<OutputPixelType>(result));
}

// RGBA to RGBA
template <class TInternalInputPixelType, class TInternalOutputPixelType>
typename RGBAPixelConverter<TInternalInputPixelType, typename itk::RGBAPixel<TInternalOutputPixelType>>::OutputPixelType
RGBAPixelConverter<TInternalInputPixelType, typename itk::RGBAPixel<TInternalOutputPixelType>>::Convert(InputPixelType input)
{
  OutputPixelType result;

  result[0] = static_cast<TInternalOutputPixelType>(input[0]);
  result[1] = static_cast<TInternalOutputPixelType>(input[1]);
  result[2] = static_cast<TInternalOutputPixelType>(input[2]);
  result[3] = static_cast<TInternalOutputPixelType>(input[3]);

  return result;
}

// RGBA to RGB
template <class TInternalInputPixelType, class TInternalOutputPixelType>
typename RGBAPixelConverter<TInternalInputPixelType, typename itk::RGBPixel<TInternalOutputPixelType>>::OutputPixelType
RGBAPixelConverter<TInternalInputPixelType, typename itk::RGBPixel<TInternalOutputPixelType>>::Convert(InputPixelType input)
{
  OutputPixelType result;

  result[0] = static_cast<TInternalOutputPixelType>(input[0]);
  result[1] = static_cast<TInternalOutputPixelType>(input[1]);
  result[2] = static_cast<TInternalOutputPixelType>(input[2]);

  return result;
}


} // end namespace otb


#endif
