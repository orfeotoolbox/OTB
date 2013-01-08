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

#include "otbImageFileWriter.h"

#include "itkProgressAccumulator.h"

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
  this->SetDeformationFieldSpacing(itk::NumericTraits<SpacingType>::Zero);

  // Wire minipipeline
  m_WarpFilter->SetDeformationField(m_DeformationFilter->GetOutput());
}

template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
void
StreamingResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecisionType>
::GenerateData()
{
  // Set up progress reporting
  typename itk::ProgressAccumulator::Pointer progress = itk::ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);
  progress->RegisterInternalFilter(m_WarpFilter, 1.f);

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
  if(this->GetDeformationFieldSpacing()== itk::NumericTraits<SpacingType>::Zero)
    {
    this->SetDeformationFieldSpacing(2.*this->GetOutputSpacing());
    }
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
  
  for(unsigned int dim = 0; dim < InputImageType::ImageDimension; ++dim)
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
                        this->GetDeformationFieldSpacing()[dim]))) + 1;
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


template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
void
StreamingResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecisionType>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "OutputOrigin: " << this->GetOutputOrigin() << std::endl;
  os << indent << "OutputSpacing: " << this->GetOutputSpacing() << std::endl;
  os << indent << "OutputStartIndex: " << this->GetOutputStartIndex() << std::endl;
  os << indent << "OutputSize: " << this->GetOutputSize() << std::endl;
}


}
#endif
