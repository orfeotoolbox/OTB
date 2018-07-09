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

#ifndef otbLocalHistogramImageFunction_hxx
#define otbLocalHistogramImageFunction_hxx

#include "otbLocalHistogramImageFunction.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkExtractImageFilter.h"
#include "otbMath.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
LocalHistogramImageFunction<TInputImage, TCoordRep>
::LocalHistogramImageFunction() :
  m_NeighborhoodRadius(1), m_NumberOfHistogramBins(128), m_HistogramMin(0), m_HistogramMax(1), m_GaussianSmoothing(true)
{
}

template <class TInputImage, class TCoordRep>
void
LocalHistogramImageFunction<TInputImage, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << " Neighborhood radius value   : "  << this->GetNeighborhoodRadius() << std::endl;
  os << indent << " Number Of Histogram Bins    : "  << this->GetNumberOfHistogramBins() << std::endl;
  os << indent << " Histogram Minimum           : "  << this->GetHistogramMin() << std::endl;
  os << indent << " Histogram Maximum           : "  << this->GetHistogramMax() << std::endl;
}

template <class TInputImage, class TCoordRep>
typename LocalHistogramImageFunction<TInputImage, TCoordRep>::OutputType
LocalHistogramImageFunction<TInputImage, TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  typename HistogramType::Pointer histogram = HistogramType::New();

  typename HistogramType::SizeType size(this->GetInputImage()->GetNumberOfComponentsPerPixel());
  size.Fill(this->GetNumberOfHistogramBins());

  typename HistogramType::MeasurementVectorType lowerBound(this->GetInputImage()->GetNumberOfComponentsPerPixel());
  typename HistogramType::MeasurementVectorType upperBound(this->GetInputImage()->GetNumberOfComponentsPerPixel());

  lowerBound.Fill( static_cast<typename HistogramType::MeasurementType>(this->GetHistogramMin()) );
  upperBound.Fill( static_cast<typename HistogramType::MeasurementType>(this->GetHistogramMax()) );

  histogram->SetMeasurementVectorSize(this->GetInputImage()->GetNumberOfComponentsPerPixel());
  histogram->Initialize(size, lowerBound, upperBound );
  histogram->SetToZero();

  // Check for input image
  if( !this->GetInputImage() )
    {
    return histogram;
    }

  // Check for out of buffer
  if ( !this->IsInsideBuffer( index ) )
    {
    return histogram;
    }

  // Create an N-d neighborhood kernel, using a zeroflux boundary condition
  typename InputImageType::SizeType kernelSize;
  kernelSize.Fill( m_NeighborhoodRadius );

  itk::ConstNeighborhoodIterator<InputImageType>
    it(kernelSize, this->GetInputImage(), this->GetInputImage()->GetBufferedRegion());

  // Set the iterator at the desired location
  it.SetLocation(index);

  // Define a gaussian kernel around the center location
  double squaredRadius = m_NeighborhoodRadius * m_NeighborhoodRadius;
  double squaredSigma = 0.25 * squaredRadius;

  // Offset to be used in the loops
  typename InputImageType::OffsetType offset;

  // Fill the histogram
  for(int i = -(int)m_NeighborhoodRadius; i< (int)m_NeighborhoodRadius; ++i)
    {
    for(int j = -(int)m_NeighborhoodRadius; j< (int)m_NeighborhoodRadius; ++j)
      {
      // Check if the current pixel lies within a disc of radius m_NeighborhoodRadius
      double currentSquaredRadius = i*i+j*j;
      if(currentSquaredRadius < squaredRadius)
        {
        // If so, compute the gaussian weighting (this could be
        // computed once for all for the sake of optimisation) if necessary
        double gWeight = 1.;
        if(m_GaussianSmoothing)
          {
          gWeight = (1/std::sqrt(otb::CONST_2PI*squaredSigma)) * std::exp(- currentSquaredRadius/(2*squaredSigma));
          }

        // Compute pixel location
        offset[0]=i;
        offset[1]=j;

        // Get the current value
        typename HistogramType::MeasurementVectorType sample(this->GetInputImage()->GetNumberOfComponentsPerPixel());
        sample[0] = it.GetPixel(offset);

        // Populate histogram
        histogram->IncreaseFrequencyOfMeasurement(sample, gWeight);
        }
      }
    }
  return histogram;
}

} // namespace otb

#endif
