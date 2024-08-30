/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbLabelMapToLabelImageFilter_hxx
#define otbLabelMapToLabelImageFilter_hxx

#include "otbLabelMapToLabelImageFilter.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "itkImageRegionConstIteratorWithIndex.h"

namespace otb
{

template <class TInputImage, class TOutputImage>
LabelMapToLabelImageFilter<TInputImage, TOutputImage>::LabelMapToLabelImageFilter()
{
}

template <class TInputImage, class TOutputImage>
void LabelMapToLabelImageFilter<TInputImage, TOutputImage>::GenerateInputRequestedRegion()
{
  itk::ImageToImageFilter<TInputImage, TOutputImage>::GenerateInputRequestedRegion();
}

} // end namespace otb
#endif
