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

#ifndef otbPointSetExtractROI_hxx
#define otbPointSetExtractROI_hxx

#include "otbPointSetExtractROI.h"
#include "itkMacro.h"

namespace otb
{

/**
 *
 */
template <class TInputPointSet, class TOutputPointSet>
PointSetExtractROI<TInputPointSet, TOutputPointSet>
::PointSetExtractROI() : m_SizeX(0), m_StartY(0), m_SizeX(0), m_SizeY(0)
{
}

/**
 *
 */
template <class TInputPointSet, class TOutputPointSet>
void
PointSetExtractROI<TInputPointSet, TOutputPointSet>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

}

/**
 * This method causes the filter to generate its output.
 */
template <class TInputPointSet, class TOutputPointSet>
void
PointSetExtractROI<TInputPointSet, TOutputPointSet>
::GenerateData(void)
{
  typedef typename TInputPointSet::PointsContainer  InputPointsContainer;
  typedef typename TInputPointSet::PointsContainerPointer  InputPointsContainerPointer;
  typedef typename TOutputPointSet::PointsContainerPointer OutputPointsContainerPointer;

  typedef typename TInputPointSet::PointDataContainerPointer  InputPointDataContainerPointer;
  typedef typename TOutputPointSet::PointDataContainerPointer OutputPointDataContainerPointer;

  InputPointSetPointer  inputPointSet      =  this->GetInput();
  OutputPointSetPointer outputPointSet     =  this->GetOutput();

  if (!inputPointSet)
    {
    itkExceptionMacro(<< "Missing Input PointSet");
    }

  if (!outputPointSet)
    {
    itkExceptionMacro(<< "Missing Output PointSet");
    }

  outputPointSet->SetBufferedRegion(outputPointSet->GetRequestedRegion());  //TODO update outputRegion

  InputPointsContainerPointer     inPoints  = inputPointSet->GetPoints();
  InputPointDataContainerPointer  inData    = inputPointSet->GetPointData();
  OutputPointsContainerPointer    outPoints = outputPointSet->GetPoints();
  OutputPointDataContainerPointer outData = outputPointSet->GetPointData();

  typename InputPointsContainer::ConstIterator    inputPoint  = inPoints->Begin();

  // Commented cause using iterator on the pointSetData crash
  // Use a direct access to pointSet Data instead to avoid segfault
  //typename InputPointDataContainer::ConstIterator inputData = inData->Begin();
  //if (inData.IsNotNull())
  //  {
  //    inputData = inData->Begin();
  //  }

  while (inputPoint != inPoints->End())
    {
    typename InputPointsContainer::Element point = inputPoint.Value();

    if ((((point[0] >= m_StartX) && (point[0] < m_StartX + m_SizeX))
         || ((point[0] <= m_StartX) && (point[0] > m_StartX + m_SizeX))) //cover the case when size<0
        && (((point[1] >= m_StartY) && (point[1] < m_StartY + m_SizeY))
            || ((point[1] <= m_StartY) && (point[1] > m_StartY + m_SizeY))))
      {
      // Add the point
      outPoints->push_back(point);
      // Get & Add the data
      typename InputPointSetType::PixelType  data;
      inputPointSet->GetPointData(inputPoint.Index(), &data);
      outData->push_back(data/*inputData.Value()*/);
      }

    ++inputPoint;
    //++inputData;
    }
}

} // end namespace otb

#endif
