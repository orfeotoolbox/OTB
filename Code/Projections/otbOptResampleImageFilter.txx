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

#ifndef __otbOptResampleImageFilter_txx
#define __otbOptResampleImageFilter_txx

#include "itkMetaDataDictionary.h"
#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"
#include "itkIdentityTransform.h"

namespace otb
{

template <class TInputImage, class TOutputImage, class TDeformationField>
OptResampleImageFilter<TInputImage, TOutputImage, TDeformationField>
::OptResampleImageFilter() 
{
  // default values for DeformationGrid
  m_DeformationGridSize.Fill(1);
  m_DeformationFieldSpacing.Fill(2);
  
  // default values for Resampled Image
  m_OutputSize.Fill(1);
  m_OutputOrigin.Fill(0);
  m_OutputIndex.Fill(0);
  m_OutputSpacing.Fill(1);
  
  // internal filters instanciation
  m_DeformationFilter = DeformationFieldGeneratorType::New();
  m_WarpFilter        = WarpImageFilterType::New();

  // default identity transform
  m_Transform = itk::IdentityTransform<double ,InputImageType::ImageDimension >::New();
  
  // Setup default interpolator
  typename DefaultInterpolatorType::Pointer interp =  DefaultInterpolatorType::New();
  m_Interpolator = static_cast<InterpolatorType*>( interp.GetPointer() );
}

template <class TInputImage, class TOutputImage, class TDeformationField>
typename OptResampleImageFilter<TInputImage, TOutputImage, TDeformationField>::DeformationFieldType*
OptResampleImageFilter<TInputImage, TOutputImage, TDeformationField>
::GetDeformationField()
{
  return m_DeformationFilter->GetOutput();
}


template <class TInputImage, class TOutputImage, class TDeformationField>
void
OptResampleImageFilter<TInputImage, TOutputImage, TDeformationField>
::GenerateData()
{
  // Get the input image 
  typename InputImageType::Pointer  input  = const_cast<InputImageType *>(this->GetInput());
  typename OutputImageType::Pointer output = this->GetOutput();
  
  
  // Build the deformation field
  m_DeformationFilter->SetTransform(m_Transform);
  m_DeformationFilter->SetOutputIndex(output->GetLargestPossibleRegion().GetIndex());
  m_DeformationFilter->SetOutputOrigin(output->GetOrigin());
  m_DeformationFilter->SetOutputSpacing(m_DeformationFieldSpacing);

  // Build the deformation field size following the
  // m_DeformationFieldSpacing set by the user
  SizeType    size    = output->GetLargestPossibleRegion().GetSize();
  SpacingType spacing = output->GetSpacing();
  
  for (unsigned int dim = 0; dim < InputImageType::ImageDimension;++dim)
    {
    m_DeformationGridSize[dim] = static_cast<unsigned long>(size[dim]*vcl_abs(spacing[dim]/m_DeformationFieldSpacing[dim]));
    }
  m_DeformationFilter->SetOutputSize(m_DeformationGridSize); 
  
  // Apply the streamed warp filter to the input image using the
  // deformation field generated before
  m_WarpFilter->SetInput(input);
  m_WarpFilter->SetOutputOrigin(m_OutputOrigin);
  m_WarpFilter->SetOutputSpacing(m_OutputSpacing);
  m_WarpFilter->SetOutputSize(m_OutputSize);
  m_WarpFilter->SetOutputStartIndex(m_OutputIndex);
  
  // Set the deformation field and the interpolator
  m_WarpFilter->SetDeformationField(m_DeformationFilter->GetOutput());
  m_WarpFilter->SetInterpolator(m_Interpolator);
  
  m_WarpFilter->GraftOutput(this->GetOutput());
  m_WarpFilter->Update();
  this->GraftOutput(m_WarpFilter->GetOutput());
}


/**
 *  
 *
 */
template <class TInputImage, class TOutputImage, class TDeformationField>
void
OptResampleImageFilter<TInputImage, TOutputImage, TDeformationField>
::GenerateOutputInformation()
{
  // call the superclass's implementation of this method
  Superclass::GenerateOutputInformation();
  
  typename OutputImageType::Pointer outputPtr = this->GetOutput();

  outputPtr->SetSpacing( m_OutputSpacing );
  outputPtr->SetOrigin( m_OutputOrigin );
  
  typename OutputImageType::RegionType region;
  region.SetSize(m_OutputSize);
  region.SetIndex(m_OutputIndex);

  outputPtr->SetLargestPossibleRegion(region);
  
//   // Expose the input metadata to the output
//   itk::MetaDataDictionary& dict = const_cast<InputImageType *>(this->GetInput())->GetMetaDataDictionary();
//   std::string              projectionRef = this->GetInput()->GetProjectionRef();
//   itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, projectionRef);
//   outputPtr->SetMetaDataDictionary(dict);
}

template <class TInputImage, class TOutputImage, class TDeformationField>
void
OptResampleImageFilter<TInputImage, TOutputImage, TDeformationField>
::GenerateInputRequestedRegion()
{
  // call the superclass's implementation
  Superclass::GenerateInputRequestedRegion();
  
  // Load Input
  typename InputImageType::Pointer inputPtr = const_cast<InputImageType*>(this->GetInput());
  // Load Output
  typename OutputImageType::Pointer outputPtr = this->GetOutput();

  // Compute the subsampling rate
  SizeType  inputSize  = inputPtr->GetLargestPossibleRegion().GetSize();
  SizeType  outputSize  = outputPtr->GetLargestPossibleRegion().GetSize();

  SpacingType  spacing = inputPtr->GetSpacing();
  
  std::vector<double>   rate(0.);
  rate[0] = static_cast<double>(inputSize[0])/static_cast<double>(outputSize[0]);
  rate[1] = static_cast<double>(inputSize[1])/static_cast<double>(outputSize[1]);

  // Get the output dimensions
  SizeType  lOutputSize  = outputPtr->GetRequestedRegion().GetSize();
  IndexType lOutputIndex = outputPtr->GetRequestedRegion().GetIndex();
  
  // Compute the input informations
  SizeType lInputSize;
  IndexType lInputIndex;
  for (unsigned int i=0; i<InputImageType::ImageDimension ;i ++)
    {
    lInputSize[i] = static_cast<unsigned int>(vcl_ceil( lOutputSize[i] * rate[i]) );
    lInputIndex[i] = static_cast<unsigned int>(vcl_ceil( lOutputIndex[i] * rate[i]) );
    }
  
  RegionType lInputRegion;
  lInputRegion.SetSize(lInputSize);
  lInputRegion.SetIndex(lInputIndex);
  
  lInputRegion.Crop(outputPtr->GetLargestPossibleRegion());

  // Set input resolution input regions
  inputPtr->SetRequestedRegion(lInputRegion);
}

/**
 * Method used to copy the parameters of the input image
 * 
 */
template <class TInputImage, class TOutputImage, class TDeformationField>
void
OptResampleImageFilter<TInputImage, TOutputImage, TDeformationField>
::SetOutputParametersFromImage(const InputImageType * image)
{
  this->SetOutputOrigin ( image->GetOrigin() );
  this->SetOutputSpacing ( image->GetSpacing() );
  this->SetOutputIndex ( image->GetLargestPossibleRegion().GetIndex() );
  this->SetOutputSize ( image->GetLargestPossibleRegion().GetSize() );
}

}
#endif
