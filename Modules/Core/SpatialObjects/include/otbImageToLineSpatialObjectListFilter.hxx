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

#ifndef otbImageToLineSpatialObjectListFilter_hxx
#define otbImageToLineSpatialObjectListFilter_hxx

#include "otbImageToLineSpatialObjectListFilter.h"

namespace otb
{

/**
 *
 */
template<class TInputImage>
ImageToLineSpatialObjectListFilter<TInputImage>::ImageToLineSpatialObjectListFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  typename LinesListType::Pointer list = LinesListType::New();
  this->SetOutput(list);

}

template <class TInputImage>
void
ImageToLineSpatialObjectListFilter<TInputImage>
::SetInput(const InputImageType *image)
{
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<InputImageType *>(image));
}

template <class TInputImage>
const typename ImageToLineSpatialObjectListFilter<TInputImage>::InputImageType *
ImageToLineSpatialObjectListFilter<TInputImage>
::GetInput(void)
{
  return static_cast<const InputImageType *>
           (this->itk::ProcessObject::GetInput(0));
}

template <class TInputImage>
void
ImageToLineSpatialObjectListFilter<TInputImage>
::SetOutput(const LinesListType *list)
{
  this->ProcessObjectType::SetNthOutput(0,
                                        const_cast<LinesListType *>(list));
}

template <class TInputImage>
typename ImageToLineSpatialObjectListFilter<TInputImage>::LinesListType *
ImageToLineSpatialObjectListFilter<TInputImage>
::GetOutput(void)
{
  return static_cast<LinesListType *>
           (this->ProcessObjectType::GetOutput(0));
}

/**
 *
 */
template<class TInputImage>
void
ImageToLineSpatialObjectListFilter<TInputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
