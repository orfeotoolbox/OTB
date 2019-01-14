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

#ifndef otbMeanShiftConnectedComponentSegmentationFilter_hxx
#define otbMeanShiftConnectedComponentSegmentationFilter_hxx

#include "otbMeanShiftConnectedComponentSegmentationFilter.h"
#include "itkExtractImageFilter.h"

namespace otb {

template<class TVImage,class TMaskImage, class TLabelImage>
MeanShiftConnectedComponentSegmentationFilter<TVImage,TMaskImage,  TLabelImage>
::MeanShiftConnectedComponentSegmentationFilter()
{
   m_MinimumObjectSize=2;
   m_MeanShiftFilter= MeanShiftFilterType::New();

}

template<class TVImage,class TMaskImage, class TLabelImage>
MeanShiftConnectedComponentSegmentationFilter<TVImage,TMaskImage,  TLabelImage>
::~MeanShiftConnectedComponentSegmentationFilter()
{
}

template<class TVImage,class TMaskImage, class TLabelImage>
void
MeanShiftConnectedComponentSegmentationFilter<TVImage,TMaskImage,  TLabelImage>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
}

template<class TVImage,class TMaskImage, class TLabelImage>
void
MeanShiftConnectedComponentSegmentationFilter<TVImage,TMaskImage,  TLabelImage>
::GenerateData()
{
  // Apply an ExtractImageFilter to avoid problems with filters asking for the LargestPossibleRegion
  typedef itk::ExtractImageFilter<TVImage, TVImage> ExtractImageFilterType;
  typename ExtractImageFilterType::Pointer extract = ExtractImageFilterType::New();
  extract->SetInput(this->GetInput());
  extract->SetExtractionRegion(this->GetOutput()->GetRequestedRegion());
  // WARNING: itk::ExtractImageFilter does not copy the MetadataDictionary

  // meanshift filtering

  //not necessary
  /* if (this->m_MeanShiftFilter.IsNotNull())
   {
   itkExceptionMacro("MeanShiftFilter is null");
   }*/
  this->m_MeanShiftFilter->SetInput(extract->GetOutput());

  //this->m_MeanShiftFilter->SetInput(this->GetInput());


  typename MaskImageType::Pointer mask;
  if (!m_MaskExpression.empty())
    {
    // Compute the mask
    typename MaskMuParserFilterType::Pointer maskFilter;
    maskFilter = MaskMuParserFilterType::New();
    maskFilter->SetInput(this->m_MeanShiftFilter->GetOutput());
    maskFilter->SetExpression(m_MaskExpression);
    maskFilter->Update();
    mask = maskFilter->GetOutput();
    }

  // Perform connected components segmentation
  typename ConnectedComponentFilterType::Pointer connected = ConnectedComponentFilterType::New();
  connected->SetInput(this->m_MeanShiftFilter->GetOutput());

  if (mask.IsNotNull()) connected->SetMaskImage(mask);
  connected->GetFunctor().SetExpression(m_ConnectedComponentExpression);
  connected->Update();

  // Relabel connected component output
  typename RelabelComponentFilterType::Pointer relabel = RelabelComponentFilterType::New();
  relabel->SetInput(connected->GetOutput());
  relabel->SetMinimumObjectSize(m_MinimumObjectSize);
  relabel->Update();

  this->GraftOutput(relabel->GetOutput());

}

} // end namespace otb
#endif
