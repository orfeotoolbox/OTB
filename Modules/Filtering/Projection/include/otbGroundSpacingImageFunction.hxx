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


#ifndef otbGroundSpacingImageFunction_hxx
#define otbGroundSpacingImageFunction_hxx

#include "otbMath.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbGroundSpacingImageFunction.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
GroundSpacingImageFunction<TInputImage, TCoordRep>
::GroundSpacingImageFunction()
{
  m_R = 6371000;
  m_Deg2radCoef = CONST_PI / 180;
}

/**
 *
 */
template <class TInputImage, class TCoordRep>
void
GroundSpacingImageFunction<TInputImage, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

/**
 *
 */
template <class TInputImage, class TCoordRep>
typename GroundSpacingImageFunction<TInputImage, TCoordRep>
::FloatType
GroundSpacingImageFunction<TInputImage, TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  FloatType var;

  if (!this->GetInputImage())
    {
    var.Fill(itk::NumericTraits<ValueType>::min());
    return var;
    }

  PointType point = this->GetPixelLocation(index);

  IndexType indexSrcX, indexSrcY;
  indexSrcX[0] =
    static_cast<IndexValueType>(std::fabs(static_cast<ValueType>(this->GetInputImage()->GetLargestPossibleRegion().
                                                                GetSize()[0] -
                                                                index[0])));                                                                 // x position
  indexSrcX[1] = index[1];   // y position

  indexSrcY[0] = index[0];   // x position
  indexSrcY[1] =
    static_cast<IndexValueType>(std::fabs(static_cast<ValueType>(this->GetInputImage()->GetLargestPossibleRegion().
                                                                GetSize()[1] -
                                                                index[1])));

  PointType pointSrcX = this->GetPixelLocation(indexSrcX);
  PointType pointSrcY = this->GetPixelLocation(indexSrcY);

  ValueType dLatX = (std::fabs(pointSrcX[1] - point[1])) * m_Deg2radCoef;
  ValueType dLonX = (std::fabs(pointSrcX[0] - point[0])) * m_Deg2radCoef;

  const ValueType One = itk::NumericTraits<ValueType>::One;
  const ValueType Two = One + One;

  ValueType aX = std::sin(dLatX / Two) * std::sin(dLatX / Two) + std::cos(point[1] * m_Deg2radCoef) * std::cos(
    pointSrcX[1] * m_Deg2radCoef) * std::sin(dLonX / Two) * std::sin(dLonX / Two);
  ValueType cX = Two * std::atan2(std::sqrt(aX), std::sqrt(One - aX));
  ValueType dX = m_R * cX;

  ValueType dLatY = (std::fabs(pointSrcY[1] - point[1])) * m_Deg2radCoef;
  ValueType dLonY = (std::fabs(pointSrcY[0] - point[0])) * m_Deg2radCoef;

  ValueType aY = std::sin(dLatY / Two) * std::sin(dLatY / Two) + std::cos(point[1] * m_Deg2radCoef) * std::cos(
    pointSrcY[1] * m_Deg2radCoef) * std::sin(dLonY / Two) * std::sin(dLonY / Two);
  ValueType cY = Two * std::atan2(std::sqrt(aY), std::sqrt(One - aY));
  ValueType dY = m_R * cY;

  //FloatType var;
  var[0] = dX / (std::fabs(static_cast<ValueType>(indexSrcX[0] - index[0])));
  var[1] = dY / (std::fabs(static_cast<ValueType>(indexSrcY[1] - index[1])));

  return var;
}

template <class TInputImage, class TCoordRep>
typename GroundSpacingImageFunction<TInputImage, TCoordRep>
::PointType
GroundSpacingImageFunction<TInputImage, TCoordRep>
::GetPixelLocation(const IndexType& index) const
{
  PointType inputPoint;
  inputPoint[0] = index[0];
  inputPoint[1] = index[1];

  if (!this->GetInputImage())
    {
    itkExceptionMacro(<< "No input image!");
    }

  TransformType::Pointer transform = TransformType::New();
  const itk::MetaDataDictionary& inputDict = this->GetInputImage()->GetMetaDataDictionary();
  transform->SetInputDictionary(inputDict);
  transform->SetInputOrigin(this->GetInputImage()->GetOrigin());
  transform->SetInputSpacing(this->GetInputImage()->GetSignedSpacing());

  transform->InstantiateTransform();
  return transform->TransformPoint(inputPoint);
}

} // end namespace otb

#endif
