/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbNCCRegistrationFunction_hxx
#define otbNCCRegistrationFunction_hxx

#include "vnl/vnl_math.h"
#include "itkNeighborhoodIterator.h"

#include "otbMacro.h"
#include "otbNCCRegistrationFunction.h"

namespace otb
{

/*
 * Default constructor
 */
template <class TFixedImage, class TMovingImage, class TDisplacementField>
NCCRegistrationFunction<TFixedImage, TMovingImage, TDisplacementField>
::NCCRegistrationFunction()
{

  RadiusType   r;
  unsigned int j;
  for (j = 0; j < ImageDimension; ++j)
    {
    r[j] = 1;
    }
  this->SetRadius(r);
  m_MetricTotal = 0.0;

  m_TimeStep = 1.0;
  m_DenominatorThreshold = 1e-9;
  m_IntensityDifferenceThreshold = 0.001;
  this->SetMovingImage(nullptr);
  this->SetFixedImage(nullptr);
  m_FixedImageSpacing.Fill(1.0);
  m_FixedImageOrigin.Fill(0.0);
  m_FixedImageGradientCalculator = GradientCalculatorType::New();

  typename DefaultInterpolatorType::Pointer interp =
    DefaultInterpolatorType::New();

  m_MovingImageInterpolator = static_cast<InterpolatorType*>(
    interp.GetPointer());

}

/*
 * Standard "PrintSelf" method.
 */
template <class TFixedImage, class TMovingImage, class TDisplacementField>
void
NCCRegistrationFunction<TFixedImage, TMovingImage, TDisplacementField>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{

  Superclass::PrintSelf(os, indent);
  /*
    os << indent << "MovingImageIterpolator: ";
    os << m_MovingImageInterpolator.GetPointer() << std::endl;
    os << indent << "FixedImageGradientCalculator: ";
    os << m_FixedImageGradientCalculator.GetPointer() << std::endl;
    os << indent << "DenominatorThreshold: ";
    os << m_DenominatorThreshold << std::endl;
    os << indent << "IntensityDifferenceThreshold: ";
    os << m_IntensityDifferenceThreshold << std::endl;
  */
}

/*
 * Set the function state values before each iteration
 */
template <class TFixedImage, class TMovingImage, class TDisplacementField>
void
NCCRegistrationFunction<TFixedImage, TMovingImage, TDisplacementField>
::InitializeIteration()
{
  if (!this->m_MovingImage || !this->m_FixedImage || !m_MovingImageInterpolator)
    {
    itkExceptionMacro(<< "MovingImage, FixedImage and/or Interpolator not set");
    }

  // cache fixed image information
  m_FixedImageSpacing    = this->m_FixedImage->GetSignedSpacing();
  m_FixedImageOrigin     = this->m_FixedImage->GetOrigin();

  // setup gradient calculator
  m_FixedImageGradientCalculator->SetInputImage(this->m_FixedImage);

  // setup moving image interpolator
  m_MovingImageInterpolator->SetInputImage(this->m_MovingImage);

  otbMsgDevMacro( " total metric " << m_MetricTotal << " field size " <<
  this->GetDisplacementField()->GetLargestPossibleRegion().GetSize() << " image size " <<
  this->m_FixedImage->GetLargestPossibleRegion().GetSize() );
  m_MetricTotal = 0.0;

}

/*
 * Compute update at a non boundary neighbourhood
 */
template <class TFixedImage, class TMovingImage, class TDisplacementField>
typename NCCRegistrationFunction<TFixedImage, TMovingImage, TDisplacementField>
::PixelType
NCCRegistrationFunction<TFixedImage, TMovingImage, TDisplacementField>
::ComputeUpdate(const NeighborhoodType& it, void * itkNotUsed(globalData),
                const FloatOffsetType& itkNotUsed(offset))
{

  PixelType update;
  update.Fill(0.0);
  unsigned int j;

  IndexType oindex = it.GetIndex();

  typename FixedImageType::SizeType hradius = it.GetRadius();

  FixedImageType* img = const_cast<FixedImageType *>(this->m_FixedImage.GetPointer());

  typename FixedImageType::SizeType imagesize = img->GetLargestPossibleRegion().GetSize();

  itk::NeighborhoodIterator<FixedImageType>
  hoodIt(hradius, img, img->GetRequestedRegion());
  hoodIt.SetLocation(oindex);

  double sff = 0.0;
  double smm = 0.0;
  double sfm = 0.0;
  double fixedValue;
  double movingValue;

  double derivativeF[ImageDimension];
  double derivativeM[ImageDimension];
  for (j = 0; j < ImageDimension; ++j)
    {
    derivativeF[j] = 0;
    derivativeM[j] = 0;
    }

  unsigned int indct;
  unsigned int hoodlen = hoodIt.Size();

  for (indct = 0; indct < hoodlen - 1; indct++)
    {

    IndexType index = hoodIt.GetIndex(indct);

    bool inimage = true;
    for (unsigned int dd = 0; dd < ImageDimension; dd++)
      {
      if (index[dd] < 0 || index[dd] >
          static_cast<typename IndexType::IndexValueType>(imagesize[dd] - 1)) inimage = false;
      }

    if (inimage)
      {

      // Get fixed image related information

      CovariantVectorType fixedGradient;
      double              fixedGradientSquaredMagnitude = 0;

      // Note: no need to check the index is within
      // fixed image buffer. This is done by the external filter.
      fixedValue = (double) this->m_FixedImage->GetPixel(index);

      fixedGradient = m_FixedImageGradientCalculator->EvaluateAtIndex(index);
      for (j = 0; j < ImageDimension; ++j)
        {
        fixedGradientSquaredMagnitude += vnl_math_sqr(fixedGradient[j]) * m_FixedImageSpacing[j];
        }

      // Get moving image related information

      PointType mappedPoint;

      typedef typename TDisplacementField::PixelType DisplacementPixelType;

      // Edited by OTB developers
      DisplacementPixelType vec;
      vec.Fill(0);
      if (this->GetDisplacementField()->GetBufferedRegion().IsInside(index))
        {
        vec = this->GetDisplacementField()->GetPixel(index);
        }
      // End Edited by OTB developers

      for (j = 0; j < ImageDimension; ++j)
        {
        mappedPoint[j] = double(index[j]) * m_FixedImageSpacing[j] +
                         m_FixedImageOrigin[j];
        mappedPoint[j] += vec[j];
        }
      if (m_MovingImageInterpolator->IsInsideBuffer(mappedPoint))
        {
        movingValue = m_MovingImageInterpolator->Evaluate(mappedPoint);
        }
      else
        {
        movingValue = 0.0;
        }

      sff += fixedValue * fixedValue;
      smm += movingValue * movingValue;
      sfm += fixedValue * movingValue;

      for (unsigned int dim = 0; dim < ImageDimension; ++dim)
        {
        double differential = fixedGradient[dim];
        derivativeF[dim] += fixedValue  * differential;
        derivativeM[dim] += movingValue * differential;
        }
      }

    }

  double updatenorm = 0.0;
  if ((sff * smm) != 0.0)
    {
    double factor = 1.0 / std::sqrt(sff * smm);
    for (unsigned int i = 0; i < ImageDimension; ++i)
      {
      update[i] = factor * (derivativeF[i] - (sfm / smm) * derivativeM[i]);
      updatenorm += (update[i] * update[i]);
      }
    updatenorm = std::sqrt(updatenorm);
    m_MetricTotal += sfm * factor;
    this->m_Energy += sfm * factor;
    }
  else
    {
    for (unsigned int i = 0; i < ImageDimension; ++i)
      {
      update[i] = 0.0;
      }
    updatenorm = 1.0;
    }

//  if ( fixedValue > 0.40 && movingValue > 0.40)  std::cout << " update norm " << updatenorm;

  if (this->GetNormalizeGradient() && updatenorm != 0.0)
    {
    update /= (updatenorm);
    }

  return update * this->m_GradientStep;
}

} // end namespace otbs

#endif
