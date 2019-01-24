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

#ifndef otbPointSetToDisplacementFieldGenerator_hxx
#define otbPointSetToDisplacementFieldGenerator_hxx

#include "otbPointSetToDisplacementFieldGenerator.h"
#include "otbMacro.h"

namespace otb
{
/**
 * Constructor
 */
template <class TPointSet, class TDisplacementField>
PointSetToDisplacementFieldGenerator<TPointSet, TDisplacementField>
::PointSetToDisplacementFieldGenerator()
{
  this->SetNumberOfRequiredInputs(1);
  m_MetricThreshold = 0.;
  m_OutputSize.Fill(100);
  m_OutputSpacing.Fill(1.);
  m_OutputOrigin.Fill(0.);
  m_DefaultValue = 0;
//  m_NearestPoints = PointSetType::New();
}
/**
 * Set the pointset containing the disparity.
 * \param pointset The pointset containing the disparity.
 */
template <class TPointSet, class TDisplacementField>
void
PointSetToDisplacementFieldGenerator<TPointSet, TDisplacementField>
::SetPointSet(const TPointSet * pointset)
{
  this->itk::ProcessObject::SetNthInput(0, const_cast<PointSetType *>(pointset));
}
/**
 * Get the pointset containing the disparity.
 * \return The pointset containing the disparity.
 */
template <class TPointSet, class TDisplacementField>
const TPointSet *
PointSetToDisplacementFieldGenerator<TPointSet, TDisplacementField>
::GetPointSet(void)
{
  return static_cast<const PointSetType *>(this->itk::ProcessObject::GetInput(0));
}
/** Generate output information */
template <class TPointSet, class TDisplacementField>
void
PointSetToDisplacementFieldGenerator<TPointSet, TDisplacementField>
::GenerateOutputInformation(void)
{
  DisplacementFieldPointerType               outputPtr = this->GetOutput();
  typename DisplacementFieldType::RegionType largest;
  largest.SetSize(m_OutputSize);
  IndexType index;
  index.Fill(0);
  largest.SetIndex(index);
  outputPtr->SetRegions(largest);
  outputPtr->SetSignedSpacing(m_OutputSpacing);
  outputPtr->SetOrigin(m_OutputOrigin);
  // Force the displacement field to have vector pixel of size 2.
  outputPtr->SetNumberOfComponentsPerPixel(2);
}
/**
 * Generate the n nearest point in point set
 *  \param index The index of the pixel to compute.
 *  \param n The number of nearest point to seek.
 *  \return A vector containing the index of the nearest point from nearest to most far.
 */
template <class TPointSet, class TDisplacementField>
typename PointSetToDisplacementFieldGenerator<TPointSet, TDisplacementField>
::IndexVectorType
PointSetToDisplacementFieldGenerator<TPointSet, TDisplacementField>
::GenerateNearestValidPointsPointSet(IndexType index, unsigned int n)
{
  typedef Functor::DistanceComparisonFunctor ComparisonFunctorType;
  DistanceVectorType distanceVector;
  IndexVectorType    indexVector;
  IndexVectorType    sortVector;
  unsigned int       i =  0;
  unsigned int       j = 0;

  typedef typename PointSetType::PointsContainer::ConstIterator PointSetIteratorType;

  PointSetIteratorType it = this->GetPointSet()->GetPoints()->Begin();
  for (; it != this->GetPointSet()->GetPoints()->End(); ++it)
    {
    PointType p;
    p[0] = it.Value()[0];
    p[1] = it.Value()[1];
    if (std::abs(this->GetPointSet()->GetPointData()->GetElement(j)[0]) >= m_MetricThreshold)
      {

      distanceVector.push_back(EuclideanDistanceMetric(index, p));
      sortVector.push_back(i);
      indexVector.push_back(j);
      ++i;
      }
    ++j;
    }

  ComparisonFunctorType comp;
  comp.SetDistanceVector(distanceVector);
  sort(sortVector.begin(), sortVector.end(), comp);

  // building output vector
  unsigned int    nbElements = (n < indexVector.size() ? n : indexVector.size());
  IndexVectorType output;
  for (i = 0; i < nbElements; ++i)
    {
    output.push_back(indexVector[sortVector[i]]);
    }
  return output;
}

template <class TPointSet, class TDisplacementField>
double
PointSetToDisplacementFieldGenerator<TPointSet, TDisplacementField>
::EuclideanDistanceMetric(IndexType index, PointType p)
{
  PointType pprime;
  // our point are expressed in index and not in physical coordinates
  //this->GetOutput()->TransformIndexToPhysicalPoint(index, pprime);
  return std::sqrt(std::pow(index[0] - p[0], 2) + std::pow(index[1] - p[1], 2));
}
/**
 * PrintSelf Method
 */
template <class TPointSet, class TDisplacementField>
void
PointSetToDisplacementFieldGenerator<TPointSet, TDisplacementField>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
