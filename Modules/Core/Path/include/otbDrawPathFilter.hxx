/*
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


#ifndef otbDrawPathFilter_hxx
#define otbDrawPathFilter_hxx

#include "otbDrawPathFilter.h"
#include "otbDrawPathListFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TInputPath, class TOutputImage>
DrawPathFilter<TInputImage, TInputPath, TOutputImage>::DrawPathFilter()
  :
  m_Value(static_cast<OutputImagePixelType>(255.0))
{}

template <class TInputImage, class TInputPath, class TOutputImage>
inline void DrawPathFilter<TInputImage, TInputPath, TOutputImage>::SetInputPath(const TInputPath* path)
{
  this->SetPathInput(path);
}

template <class TInputImage, class TInputPath, class TOutputImage>
const TInputPath* DrawPathFilter<TInputImage, TInputPath, TOutputImage>::GetInputPath(void)
{
  return this->GetPathInput();
}

/**
 * Main computation method.
 */
template <class TInputImage, class TInputPath, class TOutputImage>
void DrawPathFilter<TInputImage, TInputPath, TOutputImage>::GenerateData(void)
{
  typedef otb::DrawPathListFilter<TInputImage, TInputPath, TOutputImage> DrawListFilterType;
  typedef typename DrawListFilterType::InputPathListType PathListType;
  typename PathListType::Pointer                         list = PathListType::New();
  InputPathPointer                                       path = const_cast<TInputPath*>(this->GetPathInput());
  list->PushBack(path);

  typename DrawListFilterType::Pointer drawer = DrawListFilterType::New();
  drawer->SetInput(this->GetImageInput());
  drawer->SetInputPath(list);
  drawer->SetPathValue(m_Value);

  drawer->GraftOutput(this->GetOutput());
  drawer->Update();
  this->GraftOutput(drawer->GetOutput());
}
/**
 * Printself Method
 */
template <class TInputImage, class TInputPath, class TOutputImage>
void DrawPathFilter<TInputImage, TInputPath, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Path Value: " << m_Value << std::endl;
}
} // end namespace otb

#endif
