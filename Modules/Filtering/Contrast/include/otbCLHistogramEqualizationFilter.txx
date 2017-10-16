/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbCLHistogramEqualizationFilter_txx
#define otbCLHistogramEqualizationFilter_txx

#include "otbCLHistogramEqualizationFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"

#include <limits>

namespace otb
{
template < class TInputImage , class TOutputImage >
CLHistogramEqualizationFilter < TInputImage , TOutputImage >
::CLHistogramEqualizationFilter()
{
  
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage , class TOutputImage >
void CLHistogramEqualizationFilter < TInputImage , TOutputImage >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

  
}  // End namespace otb

#endif
