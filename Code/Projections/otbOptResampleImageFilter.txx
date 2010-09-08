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
  // internal filters instanciation
  m_DeformationFilter = DeformationFieldGeneratorType::New();
  m_WarpFilter        = WarpImageFilterType::New();

  // Wire minipipeline
  m_WarpFilter->SetDeformationField(m_DeformationFilter->GetOutput());
}

template <class TInputImage, class TOutputImage, class TDeformationField>
void
OptResampleImageFilter<TInputImage, TOutputImage, TDeformationField>
::GenerateData()
{
  m_WarpFilter->GraftOutput(this->GetOutput());
  m_WarpFilter->Update();
  this->GraftOutput(m_WarpFilter->GetOutput());
}

/**
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

  outputPtr->SetSpacing( this->GetOutputSpacing() );
  outputPtr->SetOrigin(  this->GetOutputOrigin() );
  
  typename OutputImageType::RegionType region;
  region.SetSize( this->GetOutputSize() );
  region.SetIndex(this->GetOutputStartIndex() );

  outputPtr->SetLargestPossibleRegion(region);
}

template <class TInputImage, class TOutputImage, class TDeformationField>
void
OptResampleImageFilter<TInputImage, TOutputImage, TDeformationField>
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
  for(unsigned int dim = 0; dim < InputImageType::ImageDimension;++dim)
    {
    deformationFieldLargestSize[dim] = static_cast<unsigned long>(largestSize[dim]
                                       *vcl_abs(this->GetOutputSpacing()[dim]
                                       /this->GetDeformationFieldSpacing()[dim]));
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
template <class TInputImage, class TOutputImage, class TDeformationField>
void
OptResampleImageFilter<TInputImage, TOutputImage, TDeformationField>
::SetOutputParametersFromImage(const InputImageType * image)
{
  this->SetOutputOrigin ( image->GetOrigin() );
  this->SetOutputSpacing ( image->GetSpacing() );
  this->SetOutputStartIndex ( image->GetLargestPossibleRegion().GetIndex() );
  this->SetOutputSize ( image->GetLargestPossibleRegion().GetSize() );
}

}
#endif
