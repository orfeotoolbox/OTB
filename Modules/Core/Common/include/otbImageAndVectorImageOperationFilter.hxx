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

#ifndef otbImageAndVectorImageOperationFilter_hxx
#define otbImageAndVectorImageOperationFilter_hxx

#include "otbImageAndVectorImageOperationFilter.h"
#include "itkObjectFactory.h"

namespace otb
{

template <class TInputImage, class TVectorInputImage, class TOutputImage>
ImageAndVectorImageOperationFilter<TInputImage, TVectorInputImage, TOutputImage>::ImageAndVectorImageOperationFilter()
{
  this->UseAddition();
}

template <class TInputImage, class TVectorInputImage, class TOutputImage>
ImageAndVectorImageOperationFilter<TInputImage, TVectorInputImage, TOutputImage>::~ImageAndVectorImageOperationFilter()
{
}

template <class TInputImage, class TVectorInputImage, class TOutputImage>
void ImageAndVectorImageOperationFilter<TInputImage, TVectorInputImage, TOutputImage>::SetInput(const InputImageType* input)
{
  this->SetNthInput(0, const_cast<InputImageType*>(input));
}

template <class TInputImage, class TVectorInputImage, class TOutputImage>
void ImageAndVectorImageOperationFilter<TInputImage, TVectorInputImage, TOutputImage>::SetVectorInput(const VectorInputImageType* input)
{
  this->SetNthInput(1, const_cast<VectorInputImageType*>(input));
}

template <class TInputImage, class TVectorInputImage, class TOutputImage>
const typename ImageAndVectorImageOperationFilter<TInputImage, TVectorInputImage, TOutputImage>::InputImageType*
ImageAndVectorImageOperationFilter<TInputImage, TVectorInputImage, TOutputImage>::GetInput()
{
  if (this->GetNumberOfInputs() < 1)
    return 0;

  return dynamic_cast<const InputImageType*>(this->GetInput(0));
}

template <class TInputImage, class TVectorInputImage, class TOutputImage>
const typename ImageAndVectorImageOperationFilter<TInputImage, TVectorInputImage, TOutputImage>::VectorInputImageType*
ImageAndVectorImageOperationFilter<TInputImage, TVectorInputImage, TOutputImage>::GetVectorInput()
{
  if (this->GetNumberOfInputs() < 2)
    return nullptr;

  return dynamic_cast<const VectorInputImageType*>(this->itk::ProcessObject::GetInput(1));
}
/**
 *
 */
template <class TInputImage, class TVectorInputImage, class TOutputImage>
void ImageAndVectorImageOperationFilter<TInputImage, TVectorInputImage, TOutputImage>::GenerateOutputInformation()
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();
  this->GetOutput()->SetNumberOfComponentsPerPixel(this->GetVectorInput()->GetNumberOfComponentsPerPixel());
}

} // end namespace otb

#endif
