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

#ifndef otbMorphologicalPyramidSegmentationFilter_hxx
#define otbMorphologicalPyramidSegmentationFilter_hxx

#include "otbMorphologicalPyramidSegmentationFilter.h"

#include "otbMorphologicalPyramidMRToMSConverter.h"
#include "otbMorphologicalPyramidSegmenter.h"
#include "otbMacro.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
MorphologicalPyramidSegmentationFilter<TInputImage, TOutputImage>
::MorphologicalPyramidSegmentationFilter()
{
  this->SetNumberOfRequiredInputs(3);
  m_MinimumObjectSize = 10;
  m_SeedsQuantile = 0.9;
  m_ConnectedThresholdQuantile = 0.9;
}
/**
 * Destructor
 */
template <class TInputImage, class TOutputImage>
MorphologicalPyramidSegmentationFilter<TInputImage, TOutputImage>
::~MorphologicalPyramidSegmentationFilter() {}

/**
 * Set the reference image.
 * \param image The reference image which was decomposed by the pyramid.
 */
template <class TInputImage, class TOutputImage>
void
MorphologicalPyramidSegmentationFilter<TInputImage, TOutputImage>
::SetReferenceImage(InputImageType * image)
{
  this->SetNthInput(0, const_cast<InputImageType *>(image));
}
/**
 * Set the brighter details from the pyramid. The brighter details are the supFilter or
 * infFilter details from the pyramid, depending on the morphological filter used.
 * \param imageList The brighter details extracted from the filtering operation.
 */
template <class TInputImage, class TOutputImage>
void
MorphologicalPyramidSegmentationFilter<TInputImage, TOutputImage>
::SetBrighterDetails(InputImageListType * imageList)
{
  this->SetNthInput(1, const_cast<InputImageListType *>(imageList));
}
/**
 * Set the darker details from the pyramid. The darker details are the supFilter or
 * infFilter details from the pyramid, depending on the morphological filter used.
 * \param imageList The darker details extracted from the filtering operation.
 */
template <class TInputImage, class TOutputImage>
void
MorphologicalPyramidSegmentationFilter<TInputImage, TOutputImage>
::SetDarkerDetails(InputImageListType * imageList)
{
  this->SetNthInput(2, const_cast<InputImageListType *>(imageList));
}
/**
 * Get the reference image
 * \return The reference image.
 */
template <class TInputImage, class TOutputImage>
typename MorphologicalPyramidSegmentationFilter<TInputImage, TOutputImage>
::InputImageType*
MorphologicalPyramidSegmentationFilter<TInputImage, TOutputImage>
::GetReferenceImage(void)
{
  return dynamic_cast<InputImageType *>(this->itk::ProcessObject::GetInput(0));
}
/**
 * Get the brighter details from the pyramid. The brighter details are the supFilter or
 * infFilter details from the pyramid, depending on the morphological filter used.
 * \param imageList The brighter details extracted from the filtering operation.
 */
template <class TInputImage, class TOutputImage>
typename MorphologicalPyramidSegmentationFilter<TInputImage, TOutputImage>
::InputImageListType*
MorphologicalPyramidSegmentationFilter<TInputImage, TOutputImage>
::GetBrighterDetails(void)
{
  return dynamic_cast<InputImageListType *>(this->itk::ProcessObject::GetInput(1));
}
/**
 * Get the darker details from the pyramid. The darker details are the supFilter or
 * infFilter details from the pyramid, depending on the morphological filter used.
 * \return The darker details extracted from the resampling operation.
 */
template <class TInputImage, class TOutputImage>
typename MorphologicalPyramidSegmentationFilter<TInputImage, TOutputImage>
::InputImageListType*
MorphologicalPyramidSegmentationFilter<TInputImage, TOutputImage>
::GetDarkerDetails(void)
{
  return dynamic_cast<InputImageListType *>(this->itk::ProcessObject::GetInput(2));
}

/**
 * Get a vector containing the number of segmented objects for each image.
 * \return The vector.
 */
template <class TInputImage, class TOutputImage>
typename MorphologicalPyramidSegmentationFilter<TInputImage, TOutputImage>
::NumberOfObjectsVectorType
MorphologicalPyramidSegmentationFilter<TInputImage, TOutputImage>
::GetNumberOfObjectsVector(void)
{
  return m_NumberOfObjectsVector;
}
/**
 * Main computation method
 */
template <class TInputImage, class TOutputImage>
void
MorphologicalPyramidSegmentationFilter<TInputImage, TOutputImage>
::GenerateData(void)
{
  // Reference image pointer
  InputImagePointerType referenceImage = this->GetReferenceImage();
  otbMsgDevMacro("SegmentationFilter: reference image size: " << referenceImage->GetLargestPossibleRegion().GetSize());

  // Output images list pointer
  OutputImageListPointerType outputList = this->GetOutput();

  // Multi resolution to multi scale filter typedef
  typedef otb::MorphologicalPyramid::MRToMSConverter<InputImageType, InputImageType> MRToMSConverterType;

  // Segmentation filter typedef
  typedef otb::MorphologicalPyramid::Segmenter<InputImageType, OutputImageType> SegmenterType;

  // MR To MS conversion
  typename MRToMSConverterType::Pointer mrtoms = MRToMSConverterType::New();
  mrtoms->SetSupFilter(this->GetBrighterDetails());
  mrtoms->SetInfFilter(this->GetDarkerDetails());
  mrtoms->Update();

  // Full resolution Input images lists pointers
  InputImageListPointerType brighter = mrtoms->GetSupFilterFullResolution();
  InputImageListPointerType darker = mrtoms->GetInfFilterFullResolution();

  // Segmentation filter definition
  typename InputImageListType::Iterator it;
  // Segment the supFilter details
  for (it = brighter->Begin(); it != brighter->End(); ++it)
    {
    typename SegmenterType::Pointer segmenter = SegmenterType::New();
    segmenter->SetMinimumObjectSize(m_MinimumObjectSize);
    segmenter->SetSeedsQuantile(m_SeedsQuantile);
    segmenter->SetConnectedThresholdQuantile(m_ConnectedThresholdQuantile);
    segmenter->SetOriginalImage(referenceImage);
    segmenter->SetDetailsImage(it.Get());
    segmenter->Update();
    m_NumberOfObjectsVector.push_back(segmenter->GetNumberOfObjects());
    outputList->PushBack(segmenter->GetOutput());
    }

  // Segment the infFilter details
  for (it = darker->Begin(); it != darker->End(); ++it)
    {
    typename SegmenterType::Pointer segmenter = SegmenterType::New();
    segmenter->SetMinimumObjectSize(m_MinimumObjectSize);
    segmenter->SetSeedsQuantile(m_SeedsQuantile);
    segmenter->SetConnectedThresholdQuantile(m_ConnectedThresholdQuantile);
    segmenter->SetOriginalImage(referenceImage);
    segmenter->SetSegmentDarkDetailsBool(true);

    segmenter->SetDetailsImage(it.Get());
    segmenter->Update();
    m_NumberOfObjectsVector.push_back(segmenter->GetNumberOfObjects());
    outputList->PushBack(segmenter->GetOutput());
    }
}
/**
 * PrintSelf method
 */
template <class TInputImage, class TOutputImage>
void
MorphologicalPyramidSegmentationFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb

#endif
