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

#ifndef __otbStreamingResampleImageFilter_txx
#define __otbStreamingResampleImageFilter_txx

#include "itkNumericTraits.h"

namespace otb
{

template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
StreamingResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecisionType>
::StreamingResampleImageFilter() 
{
  // internal filters instanciation
  m_DeformationFilter = DeformationFieldGeneratorType::New();
  m_WarpFilter        = WarpImageFilterType::New();

  // Initialize the deformation field spacing to zero : inconsistant
  // value 
  SpacingType     nullDeformationFieldSpacing;
  nullDeformationFieldSpacing.Fill(0.);
  m_DeformationFilter->SetOutputSpacing(nullDeformationFieldSpacing);

  // Wire minipipeline
  m_WarpFilter->SetDeformationField(m_DeformationFilter->GetOutput());
}

template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
void
StreamingResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecisionType>
::GenerateData()
{
  m_WarpFilter->GraftOutput(this->GetOutput());
  m_WarpFilter->Update();
  this->GraftOutput(m_WarpFilter->GetOutput());
}

/**
 *
 */
template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
void
StreamingResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecisionType>
::GenerateOutputInformation()
{
  // call the superclass's implementation of this method
  Superclass::GenerateOutputInformation();
  
  typename OutputImageType::Pointer outputPtr = this->GetOutput();

  outputPtr->SetSpacing( this->GetOutputSpacing() );
  outputPtr->SetOrigin(  this->GetOutputOrigin() );
  
  typename OutputImageType::RegionType region;
  region.SetSize( this->GetOutputSize() );
  region.SetIndex(this->GetOutputStartIndex() );

  outputPtr->SetLargestPossibleRegion(region);

  // check the output spacing of the deformation field
  // if 0 put an initial value
  if(m_DeformationFilter->GetOutputSpacing() == itk::NumericTraits<SpacingType>::Zero )
    m_DeformationFilter->SetOutputSpacing(2.*this->GetOutputSpacing());
}

template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
void
StreamingResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecisionType>
::GenerateInputRequestedRegion()
{
  // Retrieve output pointer
  OutputImageType * outputPtr = this->GetOutput();

  // Retrieve input pointer
  const InputImageType * inputPtr = this->GetInput();

  // Retrieve output requested region
  RegionType requestedRegion = outputPtr->GetRequestedRegion();
  SizeType largestSize       = outputPtr->GetLargestPossibleRegion().GetSize();

  // Set up deformation field filter
  SizeType deformationFieldLargestSize;
  std::cout <<"DeformationField Spacing" <<m_DeformationFilter->GetOutputSpacing()  << std::endl;
  for(unsigned int dim = 0; dim < InputImageType::ImageDimension;++dim)
    {
    // vcl_ceil to avoid numerical problems due to division of
    // spacings 
    // + 1 :  We need to enlarge the deformation field size cause
    // itk::WarpImageFilter::EvaluateDeformationAtPhysicalPoint needs
    // 4 neighbors and in the edges we can need 1 neighbor pixel
    // outside the field
    deformationFieldLargestSize[dim] = static_cast<unsigned int>(
      vcl_ceil( largestSize[dim]* 
                vcl_abs(this->GetOutputSpacing()[dim] / 
                        this->GetDeformationFieldSpacing()[dim]))) + 1 ;
    }
  m_DeformationFilter->SetOutputSize(deformationFieldLargestSize);
  m_DeformationFilter->SetOutputIndex(this->GetOutputStartIndex());
  
  // Generate input requested region
  m_WarpFilter->SetInput(inputPtr);
  m_WarpFilter->GetOutput()->UpdateOutputInformation();
  m_WarpFilter->GetOutput()->SetRequestedRegion(requestedRegion);
  m_WarpFilter->GetOutput()->PropagateRequestedRegion();
}

/**
 * Method used to copy the parameters of the input image
 * 
 */
template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
void
StreamingResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecisionType>
::SetOutputParametersFromImage(const ImageBaseType * image)
{
  this->SetOutputOrigin ( image->GetOrigin() );
  this->SetOutputSpacing ( image->GetSpacing() );
  this->SetOutputStartIndex ( image->GetLargestPossibleRegion().GetIndex() );
  this->SetOutputSize ( image->GetLargestPossibleRegion().GetSize() );
}

}
#endif
