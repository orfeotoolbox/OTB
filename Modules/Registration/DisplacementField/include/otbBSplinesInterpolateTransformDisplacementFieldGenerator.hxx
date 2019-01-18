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

#ifndef otbBSplinesInterpolateTransformDisplacementFieldGenerator_hxx
#define otbBSplinesInterpolateTransformDisplacementFieldGenerator_hxx

#include "otbBSplinesInterpolateTransformDisplacementFieldGenerator.h"
#include "itkLandmarkDisplacementFieldSource.h"
#include "otbObjectList.h"
#include "otbMacro.h"

namespace otb
{
template <class TPointSet, class TDisplacementField>
BSplinesInterpolateTransformDisplacementFieldGenerator<TPointSet, TDisplacementField>
::BSplinesInterpolateTransformDisplacementFieldGenerator()
{
  m_SplineOrder = 3;
  m_NumberOfControlPoints = 4;
  m_NumberOfLevels = 6;
}

template <class TPointSet, class TDisplacementField>
void
BSplinesInterpolateTransformDisplacementFieldGenerator<TPointSet, TDisplacementField>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  this->GetOutput()->SetNumberOfComponentsPerPixel(2);
}

/** Main computation method */
template <class TPointSet, class TDisplacementField>
void
BSplinesInterpolateTransformDisplacementFieldGenerator<TPointSet, TDisplacementField>
::GenerateData(void)
{
  DisplacementFieldPointerType outputPtr = this->GetOutput();

  // Default displacement field
  PixelType defaultValue(2);
  defaultValue.Fill(this->GetDefaultValue());
  outputPtr->Allocate();
  outputPtr->FillBuffer(defaultValue);

  typedef otb::ObjectList<BSplinerType>                         SPlineInterpolatorListType;
  typedef typename PointSetType::PointsContainer::ConstIterator PointSetIteratorType;

  // Initialization of the splines interpolator
  typename SPlineInterpolatorListType::Pointer splineIntList = SPlineInterpolatorListType::New();

  for (unsigned int paramIndex = 0; paramIndex < this->GetTransform()->GetNumberOfParameters(); ++paramIndex)
    {
    // Create a new interpolator
    splineIntList->PushBack(BSplinerType::New());
    // Create a new point set
    typename InternalPointSetType::Pointer tmpPointSet = InternalPointSetType::New();
    tmpPointSet->Initialize();
    unsigned int pointDataCounter = 0;
    for (PointSetIteratorType it = this->GetPointSet()->GetPoints()->Begin();
         it != this->GetPointSet()->GetPoints()->End();
         ++it)
      {
      if (std::abs(this->GetPointSet()->GetPointData()->GetElement(pointDataCounter)[0]) >= this->GetMetricThreshold())
        {
        typename InternalPointSetType::PixelType V(0.0);

        if (!IsAngular(paramIndex))
          {
          V[0] = this->GetPointSet()->GetPointData()->GetElement(pointDataCounter)[paramIndex + 3];
          V[1] = this->GetPointSet()->GetPointData()->GetElement(pointDataCounter)[paramIndex + 3];
          }
        else
          {
          V[0] =
            static_cast<ValueType>(std::cos(this->GetPointSet()->GetPointData()->GetElement(pointDataCounter)[paramIndex
                                                                                                             + 3]));
          V[1] =
            static_cast<ValueType>(std::sin(this->GetPointSet()->GetPointData()->GetElement(pointDataCounter)[paramIndex
                                                                                                             + 3]));
          }
        unsigned long nbPoints = tmpPointSet->GetNumberOfPoints();
        tmpPointSet->SetPoint(nbPoints, it.Value());
        tmpPointSet->SetPointData(nbPoints, V);
        }
      ++pointDataCounter;
      }

    SPlineInterpolateFilterPointerType splineFilter = SPlineInterpolateFilterType::New();

    // Set the interpolator parameters
    splineFilter->SetInput(tmpPointSet);
    splineFilter->SetSplineOrder(m_SplineOrder);
    typename SPlineInterpolateFilterType::ArrayType ncps;
    ncps.Fill(m_NumberOfControlPoints);
    splineFilter->SetNumberOfControlPoints(ncps);
    splineFilter->SetNumberOfLevels(m_NumberOfLevels);
    // splineFilter->SetGenerateOutputImage(false);

    // Define the parametric domain.
    splineFilter->SetOrigin(this->GetOutput()->GetOrigin());
    splineFilter->SetSpacing(this->GetOutput()->GetSpacing());
    splineFilter->SetSize(this->GetOutput()->GetLargestPossibleRegion().GetSize());
    splineFilter->Update();

    // Image function
    splineIntList->Back()->SetSplineOrder( splineFilter->GetSplineOrder() );
    splineIntList->Back()->SetSize( splineFilter->GetSize() );
    splineIntList->Back()->SetSpacing( splineFilter->GetSpacing() );
    splineIntList->Back()->SetOrigin( splineFilter->GetOrigin() );
    splineIntList->Back()->SetInputImage( static_cast<const InternalImageType*>(splineFilter->GetPhiLattice().GetPointer()) );
    }

  // Interpolation
  typedef itk::ImageRegionIteratorWithIndex<DisplacementFieldType> IteratorType;
  IteratorType outIt(outputPtr, outputPtr->GetRequestedRegion());

  // main loop
  for (outIt.GoToBegin(); !outIt.IsAtEnd(); ++outIt)
    {
    // Index of the pixel
    IndexType index = outIt.GetIndex();
    // parameters vector
    ParametersType params(this->GetTransform()->GetNumberOfParameters());
    // we iterate over the number of parameters of the transform
    for (unsigned int paramIndex = 0; paramIndex < this->GetTransform()->GetNumberOfParameters(); ++paramIndex)
      {
      typename InternalPointSetType::PointType p;
      this->GetOutput()->TransformIndexToPhysicalPoint(index, p);
      typename InternalPointSetType::PixelType V(0.0);
      // For each parameter we use the associated spline interpolator
      if (!IsAngular(paramIndex))
        {
        V = splineIntList->GetNthElement(paramIndex)->EvaluateAtParametricPoint(p);
        params[paramIndex] = V[0];
        }
      else
        {
        V = splineIntList->GetNthElement(paramIndex)->EvaluateAtParametricPoint(p);
        params[paramIndex] = std::atan2(V[1], V[0]);
        }
      // We then compute the target point using the transform
      this->GetTransform()->SetParameters(params);
      PointType sourcePoint, targetPoint;
      outputPtr->TransformIndexToPhysicalPoint(index, sourcePoint);
      targetPoint = this->GetTransform()->TransformPoint(sourcePoint);

      // And the displacement
      PixelType pixel;
      pixel.SetSize(2);
      pixel[0] = static_cast<ValueType>(targetPoint[0] - sourcePoint[0]);
      pixel[1] = static_cast<ValueType>(targetPoint[1] - sourcePoint[1]);
      outIt.Set(pixel);
      }
    }
}
template <class TPointSet, class TDisplacementField>
void
BSplinesInterpolateTransformDisplacementFieldGenerator<TPointSet, TDisplacementField>
::AddAngularParameter(unsigned int index)
{
  m_AngularParameters.push_back(index);
}
template <class TPointSet, class TDisplacementField>
void
BSplinesInterpolateTransformDisplacementFieldGenerator<TPointSet, TDisplacementField>
::RemoveAngularParameter(unsigned int index)
{
  m_AngularParameters.erase(index);
}
template <class TPointSet, class TDisplacementField>
bool
BSplinesInterpolateTransformDisplacementFieldGenerator<TPointSet, TDisplacementField>
::IsAngular(unsigned int index)
{
  bool resp = std::binary_search(m_AngularParameters.begin(), m_AngularParameters.end(), index);
  return resp;
}
/**
 * PrintSelf Method
 */
template <class TPointSet, class TDisplacementField>
void
BSplinesInterpolateTransformDisplacementFieldGenerator<TPointSet, TDisplacementField>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
