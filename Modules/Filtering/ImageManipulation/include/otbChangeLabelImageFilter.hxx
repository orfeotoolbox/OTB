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

#ifndef otbChangeLabelImageFilter_hxx
#define otbChangeLabelImageFilter_hxx

#include "otbChangeLabelImageFilter.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
ChangeLabelImageFilter<TInputImage, TOutputImage>::ChangeLabelImageFilter() : m_NumberOfComponentsPerPixel(0)
{
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void ChangeLabelImageFilter<TInputImage, TOutputImage>::SetChange(const InputPixelType& original, const OutputPixelType& result)
{
  OutputPixelType current = this->GetFunctor().GetChange(original);
  if (current != result)
  {
    this->GetFunctor().SetChange(original, result);
    this->Modified();
  }
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void ChangeLabelImageFilter<TInputImage, TOutputImage>::SetChangeMap(const ChangeMapType& changeMap)
{
  // If the whole map is being set then we assume that a real change is made
  this->GetFunctor().SetChangeMap(changeMap);
  this->Modified();
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void ChangeLabelImageFilter<TInputImage, TOutputImage>::ClearChangeMap()
{
  // If the whole map is being set then we assume that a real change is made
  this->GetFunctor().ClearChangeMap();
  this->Modified();
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void ChangeLabelImageFilter<TInputImage, TOutputImage>::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  this->GetOutput()->SetNumberOfComponentsPerPixel(m_NumberOfComponentsPerPixel);
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void ChangeLabelImageFilter<TInputImage, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Number of components per pixel: " << m_NumberOfComponentsPerPixel;
}

} // end namespace

#endif
