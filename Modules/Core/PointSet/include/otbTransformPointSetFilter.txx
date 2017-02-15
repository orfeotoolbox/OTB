/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbTransformPointSetFilter_txx
#define otbTransformPointSetFilter_txx

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
