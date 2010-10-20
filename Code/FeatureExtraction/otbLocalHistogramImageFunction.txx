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
#include "itkImageToHistogramGenerator.h"
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
       m_NeighborhoodRadius(1), m_NumberOfHistogramBins(128)
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
}

template <class TInputImage, class TCoordRep>
typename LocalHistogramImageFunction<TInputImage,TCoordRep>::OutputType
LocalHistogramImageFunction<TInputImage,TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  typename HistogramGeneratorType::Pointer histogramGenerator = HistogramGeneratorType::New();
  histogramGenerator->SetNumberOfBins( this->GetNumberOfHistogramBins() );

  // Check for input image
  if( !this->GetInputImage() )
    {
    return histogramGenerator->GetOutput();
    }

  // Check for out of buffer
  if ( !this->IsInsideBuffer( index ) )
    {
    return histogramGenerator->GetOutput();
    }

  typename InputImageType::Pointer currentImage;
  currentImage = InputImageType::New();

  currentImage = const_cast<InputImageType *>(this->GetInputImage());

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

  if(region.Crop(currentImage->GetRequestedRegion()) )
    {
      typedef itk::ExtractImageFilter<InputImageType,InputImageType> ExtractFilterType;
      typename ExtractFilterType::Pointer extractFilter = ExtractFilterType::New();

      extractFilter->SetInput(this->GetInputImage());
      extractFilter->SetExtractionRegion(region);
      extractFilter->Update();

      histogramGenerator->SetInput(  extractFilter->GetOutput()  );
      histogramGenerator->Compute();
    }

  return histogramGenerator->GetOutput();
}

} // namespace otb

#endif
