/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbTransformPointSetFilter_hxx
#define otbTransformPointSetFilter_hxx

#include "otbTransformPointSetFilter.h"
#include "itkMacro.h"

namespace otb
{

/**
 *
 */
template <class TInputPointSet, class TOutputPointSet, class TTransform>
TransformPointSetFilter<TInputPointSet, TOutputPointSet, TTransform>
::TransformPointSetFilter()
{
  m_Transform = TransformType::New();
}

/**
 *
 */
template <class TInputPointSet, class TOutputPointSet, class TTransform>
void
TransformPointSetFilter<TInputPointSet, TOutputPointSet, TTransform>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  if (m_Transform)
    {
    os << indent << "Transform: " << m_Transform << std::endl;
    }
}

/**
 * This method causes the filter to generate its output.
 */
template <class TInputPointSet, class TOutputPointSet, class TTransform>
void
TransformPointSetFilter<TInputPointSet, TOutputPointSet, TTransform>
::GenerateData(void)
{

  typedef typename TInputPointSet::PointsContainer  InputPointsContainer;
  typedef typename TOutputPointSet::PointsContainer OutputPointsContainer;

  typedef typename TInputPointSet::PointsContainerPointer  InputPointsContainerPointer;
  typedef typename TOutputPointSet::PointsContainerPointer OutputPointsContainerPointer;

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

  outputPointSet->SetBufferedRegion(outputPointSet->GetRequestedRegion());

  InputPointsContainerPointer  inPoints  = inputPointSet->GetPoints();
  OutputPointsContainerPointer outPoints = outputPointSet->GetPoints();

  outPoints->Reserve(inputPointSet->GetNumberOfPoints());
  outPoints->Squeeze();  // in case the previous PointSet had
                         // allocated a larger memory

  typename InputPointsContainer::ConstIterator inputPoint  = inPoints->Begin();
  typename OutputPointsContainer::Iterator     outputPoint = outPoints->Begin();

  while (inputPoint != inPoints->End())
    {
    outputPoint.Value() =
      m_Transform->TransformPoint(inputPoint.Value());

    ++inputPoint;
    ++outputPoint;
    }

  // Create duplicate references to the rest of data on the PointSet

  outputPointSet->SetPointData(inputPointSet->GetPointData());
  //TODO validate that we are copying everything
}

} // end namespace itk

#endif
