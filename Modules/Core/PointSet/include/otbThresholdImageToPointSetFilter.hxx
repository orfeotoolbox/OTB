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


#ifndef otbThresholdImageToPointSetFilter_hxx
#define otbThresholdImageToPointSetFilter_hxx

#include "otbThresholdImageToPointSetFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"

namespace otb
{

template <class TInputImage, class TOutputPointSet>
ThresholdImageToPointSetFilter<TInputImage, TOutputPointSet>
::ThresholdImageToPointSetFilter()
{
  m_LowerThreshold = itk::NumericTraits<InputPixelType>::NonpositiveMin();
  m_UpperThreshold = itk::NumericTraits<InputPixelType>::max();
}

template <class TInputImage, class TOutputPointSet>
void
ThresholdImageToPointSetFilter<TInputImage, TOutputPointSet>
::ThreadedGenerateData(const InputImageRegionType& inputRegionForThread, itk::ThreadIdType threadId)
{
  this->m_PointsContainerPerThread[threadId] = PointsContainerType::New();
  this->m_PointDataContainerPerThread[threadId] = PointDataContainerType::New();
  InputImageConstPointer inputPtr = this->GetInput();

  // Define the iterators
  itk::ImageRegionConstIterator<TInputImage>  inputIt(inputPtr, inputRegionForThread);

  itk::ProgressReporter progress(this, threadId, inputRegionForThread.GetNumberOfPixels());

  typename OutputPointSetType::PointType position;
  inputIt.GoToBegin();

  unsigned long currentIndex = 0;

  while (!inputIt.IsAtEnd())
    {
    const InputPixelType value = inputIt.Get();
    if ((value >= m_LowerThreshold) && (value <= m_UpperThreshold))
      {
      //FIXME: non valid for image with dim > 2
      const IndexType index = inputIt.GetIndex();
      position[0] = index[0];
      position[1] = index[1];
      this->m_PointsContainerPerThread[threadId]->InsertElement(currentIndex,position);
      this->m_PointDataContainerPerThread[threadId]->InsertElement(currentIndex,static_cast<typename PointDataContainerType::Element>(value));
      ++currentIndex;
      }
    ++inputIt;
    progress.CompletedPixel();  // potential exception thrown here
    }
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutputPointSet>
void
ThresholdImageToPointSetFilter<TInputImage, TOutputPointSet>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "LowerThreshold : " << m_LowerThreshold << std::endl;
  os << indent << "UpperThreshold : " << m_UpperThreshold << std::endl;
}

} // end namespace otb

#endif
