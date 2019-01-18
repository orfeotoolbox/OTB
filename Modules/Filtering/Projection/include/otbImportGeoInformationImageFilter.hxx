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

#ifndef otbImportGeoInformationImageFilter_hxx
#define otbImportGeoInformationImageFilter_hxx

#include "otbImportGeoInformationImageFilter.h"
#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"

namespace otb
{
/**
 * Constructor
 */
template <class TImage, class TSourceImage>
ImportGeoInformationImageFilter<TImage, TSourceImage>
::ImportGeoInformationImageFilter()
{
  this->InPlaceOn();
  this->SetNumberOfRequiredInputs(2);
}

template <class TImage, class TSourceImage>
void
ImportGeoInformationImageFilter<TImage, TSourceImage>
::SetSource(const TSourceImage * source)
{
  this->SetNthInput(1, const_cast<TSourceImage *>(source));
}
template <class TImage, class TSourceImage>
const TSourceImage *
ImportGeoInformationImageFilter<TImage, TSourceImage>
::GetSource()
{
  return static_cast<const TSourceImage *>(this->itk::ProcessObject::GetInput(1));
}

template <class TImage, class TSourceImage>
void
ImportGeoInformationImageFilter<TImage, TSourceImage>
::GenerateInputRequestedRegion(void)
{
  Superclass::GenerateInputRequestedRegion();

  typename SourceImageType::RegionType region;
  typename SourceImageType::SizeType   size;
  typename SourceImageType::IndexType  index;

  size.Fill(0);
  index.Fill(0);
  region.SetSize(size);
  region.SetIndex(index);

  SourceImageType * sourcePtr = const_cast<SourceImageType *>(this->GetSource());
  sourcePtr->SetRequestedRegion(region);
}

template <class TImage, class TSourceImage>
void
ImportGeoInformationImageFilter<TImage, TSourceImage>
::GenerateOutputInformation(void)
{
  Superclass::GenerateOutputInformation();
  // Get output and source pointer
  ImagePointerType  outputPtr = this->GetOutput();
  SourceImageType * sourcePtr = const_cast<SourceImageType *>(this->GetSource());
  // Import metdata
  outputPtr->CopyInformation(sourcePtr);

  outputPtr->SetLargestPossibleRegion(this->GetInput()->GetLargestPossibleRegion());

  outputPtr->SetNumberOfComponentsPerPixel(this->GetInput()->GetNumberOfComponentsPerPixel());
}

// template <class TImage, class TSourceImage>
// void
//  ImportGeoInformationImageFilter<TImage, TSourceImage>
// ::GenerateData(void)
// {
//   // Get output and source pointer
//   ImagePointerType outputPtr = this->GetOutput();
//   SourceImageType * sourcePtr =const_cast<SourceImageType *>(this->GetSource());
//   // Import metdata
//   outputPtr->CopyInformation(sourcePtr);
//   // Don't forget to copy also the origin and spacing, not handled by the CopyInformation method.
//   outputPtr->SetOrigin(sourcePtr->GetOrigin());
//   outputPtr->SetSignedSpacing(sourcePtr->GetSignedSpacing());
// }

/**
 * PrintSelf Method
 */
template <class TImage, class TSourceImage>
void
ImportGeoInformationImageFilter<TImage, TSourceImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
