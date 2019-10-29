/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbImageToLuminanceImageFilter_h
#define otbImageToLuminanceImageFilter_h

#include <otbImageToRadianceImageFilter.h>

namespace otb
{


/** \class ImageToLuminanceImageFilter
 *  \brief Convert a raw value into a Luminance value
 *
 * \deprecated in OTB 6.2, please use ImageToRadianceImageFilter instead
 *
 * \ingroup OTBOpticalCalibration
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ImageToLuminanceImageFilter : public ImageToRadianceImageFilter<TInputImage, TOutputImage>
{
};


} // namespace otb

#endif
