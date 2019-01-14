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

#ifndef otbNearestTransformDisplacementFieldGenerator_hxx
#define otbNearestTransformDisplacementFieldGenerator_hxx

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
