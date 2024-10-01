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

#ifndef otbGeodesicMorphologyIterativeDecompositionImageFilter_hxx
#define otbGeodesicMorphologyIterativeDecompositionImageFilter_hxx

#include "otbGeodesicMorphologyIterativeDecompositionImageFilter.h"

#include "itkProgressAccumulator.h"
#include "otbMacro.h"

namespace otb
{
/**
 * Constructor
 */
template <class TImage, class TStructuringElement>
GeodesicMorphologyIterativeDecompositionImageFilter<TImage, TStructuringElement>::GeodesicMorphologyIterativeDecompositionImageFilter()
{
  // Create a process accumulator for tracking the progress of minipipeline
  m_Progress = itk::ProgressAccumulator::New();
  m_Progress->SetMiniPipelineFilter(this);

  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(3);
  m_NumberOfIterations                = 2;
  m_InitialValue                      = 1;
  m_Step                              = 1;
  OutputImageListPointerType convList = OutputImageListType::New();
  this->SetNthOutput(0, convList.GetPointer());
  OutputImageListPointerType concList = OutputImageListType::New();
  this->SetNthOutput(1, concList.GetPointer());
  OutputImageListPointerType outputList = OutputImageListType::New();
  this->SetNthOutput(2, outputList.GetPointer());
}

template <class TImage, class TStructuringElement>
typename GeodesicMorphologyIterativeDecompositionImageFilter<TImage, TStructuringElement>::OutputImageListType*
GeodesicMorphologyIterativeDecompositionImageFilter<TImage, TStructuringElement>::GetOutput(void)
{
  return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(2));
}

template <class TImage, class TStructuringElement>
typename GeodesicMorphologyIterativeDecompositionImageFilter<TImage, TStructuringElement>::OutputImageListType*
GeodesicMorphologyIterativeDecompositionImageFilter<TImage, TStructuringElement>::GetConvexOutput(void)
{
  return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(0));
}

template <class TImage, class TStructuringElement>
typename GeodesicMorphologyIterativeDecompositionImageFilter<TImage, TStructuringElement>::OutputImageListType*
GeodesicMorphologyIterativeDecompositionImageFilter<TImage, TStructuringElement>::GetConcaveOutput(void)
{
  return dynamic_cast<OutputImageListType*>(this->itk::ProcessObject::GetOutput(1));
}

template <class TImage, class TStructuringElement>
void GeodesicMorphologyIterativeDecompositionImageFilter<TImage, TStructuringElement>::GenerateOutputInformation(void)
{
  // Retrieving input/output pointers
  InputImagePointerType      inputPtr      = this->GetInput();
  OutputImageListPointerType outputPtr     = this->GetOutput();
  OutputImageListPointerType convOutputPtr = this->GetConvexOutput();
  OutputImageListPointerType concOutputPtr = this->GetConcaveOutput();
  if (outputPtr)
  {
    if (outputPtr->Size() != m_NumberOfIterations)
    {
      // in this case, clear the list
      outputPtr->Clear();
      for (unsigned int i = 0; i < m_NumberOfIterations; ++i)
      {
        // Create the output image
        outputPtr->PushBack(OutputImageType::New());
      }
    }
    // For each output image
    typename OutputImageListType::Iterator outputListIt = outputPtr->Begin();
    while (outputListIt != outputPtr->End())
    {
      // Set the image information
      outputListIt.Get()->CopyInformation(this->GetInput());
      outputListIt.Get()->SetRequestedRegion(this->GetInput()->GetLargestPossibleRegion());
      ++outputListIt;
    }
  }
  if (convOutputPtr)
  {
    if (convOutputPtr->Size() != m_NumberOfIterations)
    {
      // in this case, clear the list
      convOutputPtr->Clear();
      for (unsigned int i = 0; i < m_NumberOfIterations; ++i)
      {
        // Create the output image
        convOutputPtr->PushBack(OutputImageType::New());
      }
    }
    // For each output image
    typename OutputImageListType::Iterator outputListIt = convOutputPtr->Begin();
    while (outputListIt != convOutputPtr->End())
    {
      // Set the image information
      outputListIt.Get()->CopyInformation(this->GetInput());
      outputListIt.Get()->SetRequestedRegion(this->GetInput()->GetLargestPossibleRegion());
      ++outputListIt;
    }
  }
  if (concOutputPtr)
  {
    if (concOutputPtr->Size() != m_NumberOfIterations)
    {
      // in this case, clear the list
      concOutputPtr->Clear();
      for (unsigned int i = 0; i < m_NumberOfIterations; ++i)
      {
        // Create the output image
        concOutputPtr->PushBack(OutputImageType::New());
      }
    }
    // For each output image
    typename OutputImageListType::Iterator outputListIt = concOutputPtr->Begin();
    while (outputListIt != concOutputPtr->End())
    {
      // Set the image information
      outputListIt.Get()->CopyInformation(this->GetInput());
      outputListIt.Get()->SetRequestedRegion(this->GetInput()->GetLargestPossibleRegion());
      ++outputListIt;
    }
  }
}

template <class TImage, class TStructuringElement>
void GeodesicMorphologyIterativeDecompositionImageFilter<TImage, TStructuringElement>::GenerateInputRequestedRegion(void)
{
  // Retrieving input/output pointers
  InputImagePointerType      inputPtr  = this->GetInput();
  OutputImageListPointerType outputPtr = this->GetOutput();

  // For each output image
  typename OutputImageListType::Iterator outputListIt = outputPtr->Begin();
  inputPtr->SetRequestedRegion(outputListIt.Get()->GetRequestedRegion());
}

/**
 * Main computation method
 */
template <class TImage, class TStructuringElement>
void GeodesicMorphologyIterativeDecompositionImageFilter<TImage, TStructuringElement>::GenerateData(void)
{
  // Input image pointer
  InputImagePointerType      current       = this->GetInput();
  OutputImageListPointerType outputPtr     = this->GetOutput();
  OutputImageListPointerType convOutputPtr = this->GetConvexOutput();
  OutputImageListPointerType concOutputPtr = this->GetConcaveOutput();

  unsigned int i = 0;

  DecompositionFilterPointerType filter;

  while (i < m_NumberOfIterations)
  {
    filter = DecompositionFilterType::New();

    // Register Internal Filter for progress
    m_Progress->RegisterInternalFilter(filter, 1. / m_NumberOfIterations);

    typename StructuringElementType::RadiusType radius;
    radius.Fill(m_InitialValue + i * m_Step);
    filter->SetRadius(radius);
    filter->SetInput(current);
    filter->GetOutput()->UpdateOutputInformation();
    filter->GetOutput()->SetRequestedRegion(outputPtr->GetNthElement(i)->GetRequestedRegion());
    filter->GetOutput()->PropagateRequestedRegion();
    filter->GetOutput()->UpdateOutputData();

    outputPtr->SetNthElement(i, filter->GetOutput());
    concOutputPtr->SetNthElement(i, filter->GetConcaveMap());
    convOutputPtr->SetNthElement(i, filter->GetConvexMap());

    current = filter->GetOutput();

    ++i;
  }
}
/**
 * PrintSelf method
 */
template <class TImage, class TStructuringElement>
void GeodesicMorphologyIterativeDecompositionImageFilter<TImage, TStructuringElement>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "NumberOfIterations: " << m_NumberOfIterations << std::endl;
  os << indent << "InitialValue: " << m_InitialValue << std::endl;
  os << indent << "Step: " << m_Step << std::endl;
}
} // End namespace otb
#endif
