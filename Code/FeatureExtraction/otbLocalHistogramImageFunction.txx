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
#ifndef __otbLocalHistogramImageFunction_txx
#define __otbLocalHistogramImageFunction_txx

#include "otbLocalHistogramImageFunction.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNumericTraits.h"
#include "itkMacro.h"
#include "itkExtractImageFilter.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
LocalHistogramImageFunction<TInputImage, TCoordRep>
::LocalHistogramImageFunction() :
  m_NeighborhoodRadius(1), m_NumberOfHistogramBins(128), m_HistogramMin(0), m_HistogramMax(1)
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
typename LocalHistogramImageFunction<TInputImage,TCoordRep>::OutputType
LocalHistogramImageFunction<TInputImage,TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{

  typename HistogramType::Pointer histogram = HistogramType::New();

  typename HistogramType::SizeType size ;
  size.Fill( this->GetNumberOfHistogramBins() ) ;

  typename HistogramType::MeasurementVectorType lowerBound;
  typename HistogramType::MeasurementVectorType upperBound;

  lowerBound.Fill( static_cast<typename HistogramType::MeasurementType>(this->GetHistogramMin()) ) ;
  upperBound.Fill( static_cast<typename HistogramType::MeasurementType>(this->GetHistogramMax()) ) ;

  histogram->Initialize(size, lowerBound, upperBound ) ;
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

  typename InputImageType::ConstPointer currentImage = this->GetInputImage();

  typename itk::ConstNeighborhoodIterator<InputImageType>::RadiusType radius;
  radius.Fill( this->GetNeighborhoodRadius() );

  typename InputImageType::RegionType region;
  typename InputImageType::IndexType currentIndex;
  typename InputImageType::SizeType currentSize;

  for(unsigned int i = 0 ; i < ImageDimension; ++i )
    {
    currentIndex[i] = index[i] - this->GetNeighborhoodRadius();
    currentSize[i] = 2 * this->GetNeighborhoodRadius() + 1;
    }

  region.SetIndex(currentIndex);
  region.SetSize(currentSize);

  itk::ConstNeighborhoodIterator<InputImageType> it(radius, currentImage, region);

  float numberOfPixelsCounted = 0.0;

  it.GoToBegin();
  while (!it.IsAtEnd())
    {
    typename HistogramType::MeasurementVectorType sample;
    for (unsigned int j = 0; j < sample.Size(); ++j)
      {
      sample[j] = it.GetPixel(j);
      }
    if( histogram->IncreaseFrequency(sample, 1) )
      {
      ++numberOfPixelsCounted;
      }
    ++it;
    }
  return histogram;
}

} // namespace otb

#endif
