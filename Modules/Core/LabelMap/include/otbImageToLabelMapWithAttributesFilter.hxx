/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbImageToLabelMapWithAttributesFilter_hxx
#define otbImageToLabelMapWithAttributesFilter_hxx

#include "otbImageToLabelMapWithAttributesFilter.h"
#include "itkProcessObject.h"

namespace otb
{
template <class TInputImage, class TLabeledImage, class TOutputLabel, class TObjectLabel>
ImageToLabelMapWithAttributesFilter<TInputImage, TLabeledImage, TOutputLabel, TObjectLabel>::ImageToLabelMapWithAttributesFilter()
{
  m_Output = LabelMapType::New();
  this->itk::ProcessObject::SetNumberOfRequiredInputs(2);
}

template <class TInputImage, class TLabeledImage, class TOutputLabel, class TObjectLabel>
void ImageToLabelMapWithAttributesFilter<TInputImage, TLabeledImage, TOutputLabel, TObjectLabel>::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // We need all the input.
  typename InputImageType::Pointer input = const_cast<InputImageType*>(this->GetInput());

  if (!input)
  {
    return;
  }

  input->SetRequestedRegion(input->GetLargestPossibleRegion());
}

template <class TInputImage, class TLabeledImage, class TOutputLabel, class TObjectLabel>
void ImageToLabelMapWithAttributesFilter<TInputImage, TLabeledImage, TOutputLabel, TObjectLabel>::SetInput(const InputImageType* image)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0, const_cast<InputImageType*>(image));
}

template <class TInputImage, class TLabeledImage, class TOutputLabel, class TObjectLabel>
void ImageToLabelMapWithAttributesFilter<TInputImage, TLabeledImage, TOutputLabel, TObjectLabel>::SetLabeledImage(const LabeledImageType* image)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(1, const_cast<LabeledImageType*>(image));
}

template <class TInputImage, class TLabeledImage, class TOutputLabel, class TObjectLabel>
const typename ImageToLabelMapWithAttributesFilter<TInputImage, TLabeledImage, TOutputLabel, TObjectLabel>::InputImageType*
ImageToLabelMapWithAttributesFilter<TInputImage, TLabeledImage, TOutputLabel, TObjectLabel>::GetInput()
{
  if (this->GetNumberOfInputs() < 2)
  {
    return nullptr;
  }

  return static_cast<const InputImageType*>(this->itk::ProcessObject::GetInput(0));
}

template <class TInputImage, class TLabeledImage, class TOutputLabel, class TObjectLabel>
const typename ImageToLabelMapWithAttributesFilter<TInputImage, TLabeledImage, TOutputLabel, TObjectLabel>::LabeledImageType*
ImageToLabelMapWithAttributesFilter<TInputImage, TLabeledImage, TOutputLabel, TObjectLabel>::GetLabeledImage()
{
  if (this->GetNumberOfInputs() < 2)
  {
    return nullptr;
  }

  return static_cast<const LabeledImageType*>(this->itk::ProcessObject::GetInput(1));
}


template <class TInputImage, class TLabeledImage, class TOutputLabel, class TObjectLabel>
typename ImageToLabelMapWithAttributesFilter<TInputImage, TLabeledImage, TOutputLabel, TObjectLabel>::LabelMapType*
ImageToLabelMapWithAttributesFilter<TInputImage, TLabeledImage, TOutputLabel, TObjectLabel>::GetOutput()
{
  return dynamic_cast<LabelMapType*>(this->Superclass::GetOutput(0));
}


template <class TInputImage, class TLabeledImage, class TOutputLabel, class TObjectLabel>
void ImageToLabelMapWithAttributesFilter<TInputImage, TLabeledImage, TOutputLabel, TObjectLabel>::GenerateData()
{
  typename InputImageType::Pointer   inputImage  = const_cast<InputImageType*>(this->GetInput());
  typename LabeledImageType::Pointer labeldImage = const_cast<LabeledImageType*>(this->GetLabeledImage());

  // Convert to label map
  typename LabelMapFilterType::Pointer lfilter = LabelMapFilterType::New();
  lfilter->SetBackgroundValue(itk::NumericTraits<LabelType>::max());
  lfilter->SetInput(labeldImage);

  // Compute shape attributes
  typename ShapeLabelMapFilterType::Pointer shapeLabelMapFilter = ShapeLabelMapFilterType::New();
  shapeLabelMapFilter->SetInput(lfilter->GetOutput());

  // Compute radiometric attributes
  typename BandStatisticsLabelMapFilterType::Pointer bandStatsLabelMapFilter = BandStatisticsLabelMapFilterType::New();
  bandStatsLabelMapFilter->SetInput(shapeLabelMapFilter->GetOutput());
  bandStatsLabelMapFilter->SetFeatureImage(inputImage);

  // Get the label map
  bandStatsLabelMapFilter->GetOutput()->SetAdjacencyMap(lfilter->GetOutput()->GetAdjacencyMap());
  bandStatsLabelMapFilter->GraftOutput(this->GetOutput());

  // execute the mini-pipeline
  bandStatsLabelMapFilter->Update();

  // graft the mini-pipeline output back onto this filter's output.
  this->GraftOutput(bandStatsLabelMapFilter->GetOutput());
}
}

#endif
