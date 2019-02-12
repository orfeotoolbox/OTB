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

#ifndef otbLikelihoodPathListFilter_hxx
#define otbLikelihoodPathListFilter_hxx

#include "otbLikelihoodPathListFilter.h"
#include "otbPolyLineImageConstIterator.h"
#include "itkMetaDataObject.h"

namespace otb
{
/**
 * Constructor
 */
template <class TPath, class TImage>
LikelihoodPathListFilter<TPath, TImage>
::LikelihoodPathListFilter()
{
  m_Key = "Value";
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredInputs(2);
}
template <class TPath, class TImage>
void
LikelihoodPathListFilter<TPath, TImage>
::SetInputImage(const ImageType * image)
{
  this->itk::ProcessObject::SetNthInput(1, const_cast<ImageType *>(image));
}
template <class TPath, class TImage>
const typename LikelihoodPathListFilter<TPath, TImage>
::ImageType *
LikelihoodPathListFilter<TPath, TImage>
::GetInputImage(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return nullptr;
    }
  return static_cast<const ImageType *>(this->itk::ProcessObject::GetInput(1));
}

template <class TPath, class TImage>
void
LikelihoodPathListFilter<TPath, TImage>
::GenerateData()
{
  // I/O wiring
  ImageConstPointerType inputImagePtr = this->GetInputImage();
  const PathListType *  inputPtr  = this->GetInput();
  PathListType *        outputPtr = this->GetOutput();

  typedef otb::PolyLineImageConstIterator<ImageType, PathType> ImageIteratorType;

  for (IteratorType it = inputPtr->Begin(); it != inputPtr->End(); ++it)
    {
    PathPointerType path = it.Get();
    ImageIteratorType imageIt(inputImagePtr, path);
    double cumulatedValues = 0.0;
    double nbPoints = 0.0;
    for (imageIt.GoToBegin(); !imageIt.IsAtEnd(); ++imageIt, ++nbPoints)
      {
      cumulatedValues += static_cast<double>(imageIt.Get());
      }
    itk::MetaDataDictionary& dict = path->GetMetaDataDictionary();
    itk::EncapsulateMetaData<double>(dict, m_Key, cumulatedValues / nbPoints);
    outputPtr->PushBack(path);
    }
}
/**
 * PrintSelf Method
 */
template <class TPath, class TImage>
void
LikelihoodPathListFilter<TPath, TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // End namespace otb
#endif
