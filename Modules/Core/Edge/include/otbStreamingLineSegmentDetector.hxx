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

#ifndef otbStreamingLineSegmentDetector_hxx
#define otbStreamingLineSegmentDetector_hxx

#include "otbStreamingLineSegmentDetector.h"

#include "otbVectorDataTransformFilter.h"
#include "itkAffineTransform.h"

namespace otb
{

template <class TInputImage>
PersistentStreamingLineSegmentDetector<TInputImage>::PersistentStreamingLineSegmentDetector()
{
}

template <class TInputImage>
PersistentStreamingLineSegmentDetector<TInputImage>::~PersistentStreamingLineSegmentDetector()
{
}

template <class TInputImage>
void PersistentStreamingLineSegmentDetector<TInputImage>::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();

  if (this->GetInput())
  {
    InputImagePointerType input = const_cast<InputImageType*>(this->GetInput());

    typename InputImageType::RegionType region = this->GetOutput()->GetRequestedRegion();

    region.PadByRadius(1);
    region.Crop(input->GetLargestPossibleRegion());

    input->SetRequestedRegion(region);
  }
}

template <class TInputImage>
typename PersistentStreamingLineSegmentDetector<TInputImage>::OutputVectorDataPointerType PersistentStreamingLineSegmentDetector<TInputImage>::ProcessTile()
{
  // Apply an ExtractImageFilter to avoid problems with filters asking for the LargestPossibleRegion
  typename ExtractImageFilterType::Pointer extract = ExtractImageFilterType::New();
  extract->SetInput(this->GetInput());
  extract->SetExtractionRegion(this->GetInput()->GetBufferedRegion());
  extract->Update();

  // WARNING: itk::ExtractImageFilter does not copy the MetadataDictionary
  // but LSD filter need the projection ref if available
  extract->GetOutput()->SetMetaDataDictionary(this->GetInput()->GetMetaDataDictionary());

  typename LSDType::Pointer lsd = LSDType::New();
  lsd->SetInput(extract->GetOutput());
  lsd->UpdateOutputInformation();
  lsd->Update();

  // return the LSD VectorData in image physical coordinates
  return lsd->GetOutput();
}


} // end namespace otb
#endif
