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
#ifndef __otbNearestTransformDisplacementFieldGenerator_txx
#define __otbNearestTransformDisplacementFieldGenerator_txx

#include "otbNearestTransformDisplacementFieldGenerator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "otbMacro.h"

namespace otb
{
/** Main computation method */
template <class TPointSet, class TDisplacementField>
void
NearestTransformDisplacementFieldGenerator<TPointSet, TDisplacementField>
::GenerateData(void)
{
  DisplacementFieldPointerType outputPtr = this->GetOutput();
  PixelType defaultValue(2);
  defaultValue.Fill(this->GetDefaultValue());
  outputPtr->Allocate();
  outputPtr->FillBuffer(defaultValue);

  typedef itk::ImageRegionIteratorWithIndex<DisplacementFieldType> IteratorType;
  IteratorType it(outputPtr, outputPtr->GetRequestedRegion());

  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    IndexVectorType indexVector = this->GenerateNearestValidPointsPointSet(it.GetIndex(), 1);
    PixelType pixel(2);
    if (indexVector.size() >= 1)
      {
      ParametersType params(this->GetTransform()->GetNumberOfParameters());
      for (unsigned int i = 0; i < this->GetTransform()->GetNumberOfParameters(); ++i)
        {
        params[i] = this->GetPointSet()->GetPointData()->GetElement(indexVector[0])[i + 3];
        }
      this->GetTransform()->SetParameters(params);
      PointType sourcePoint, targetPoint;

      outputPtr->TransformIndexToPhysicalPoint(it.GetIndex(), sourcePoint);
      targetPoint = this->GetTransform()->TransformPoint(sourcePoint);
      pixel[0] = static_cast<ValueType>(targetPoint[0] - sourcePoint[0]);
      pixel[1] = static_cast<ValueType>(targetPoint[1] - sourcePoint[1]);
      }
    else
      {
      pixel = defaultValue;
      }
    it.Set(pixel);
    }
}
/**
 * PrintSelf Method
 */
template <class TPointSet, class TDisplacementField>
void
NearestTransformDisplacementFieldGenerator<TPointSet, TDisplacementField>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
