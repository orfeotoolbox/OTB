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

#ifndef __otbGenericRSResampleImageFilter_txx
#define __otbGenericRSResampleImageFilter_txx

#include "itkMetaDataDictionary.h"
#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"
#include "itkIdentityTransform.h"
#include "itkContinuousIndex.h"

namespace otb
{

template <class TInputImage, class TOutputImage, class TDeformationField>
GenericRSResampleImageFilter<TInputImage, TOutputImage, TDeformationField>
::GenericRSResampleImageFilter() 
{  
  // internal filters instanciation
  m_Resampler  = ResamplerType::New();
  m_Transform = GenericRSTransformType::New();

  // Initialize the deformation field spacing 
  SpacingType   initSpacing;
  initSpacing[0]= 2.;
  initSpacing[1]=-2.;
  this->SetDeformationFieldSpacing(initSpacing);
}



template <class TInputImage, class TOutputImage, class TDeformationField>
void
GenericRSResampleImageFilter<TInputImage, TOutputImage, TDeformationField>
::GenerateData()
{
  m_Resampler->GraftOutput(this->GetOutput());
  m_Resampler->Update();
  this->GraftOutput(m_Resampler->GetOutput());
}


/**
 *  
 *
 */
template <class TInputImage, class TOutputImage, class TDeformationField>
void
GenericRSResampleImageFilter<TInputImage, TOutputImage, TDeformationField>
::GenerateOutputInformation()
{
  // call the superclass's implementation of this method
  Superclass::GenerateOutputInformation();
  
  typename OutputImageType::Pointer outputPtr = this->GetOutput();

  outputPtr->SetSpacing( this->GetOutputSpacing() );
  outputPtr->SetOrigin( this->GetOutputOrigin() );
  
  typename OutputImageType::RegionType region;
  region.SetSize(this->GetOutputSize());
  region.SetIndex(this->GetOutputStartIndex() );

  outputPtr->SetLargestPossibleRegion(region);
  
  // Expose the input metadata to the output
  itk::MetaDataDictionary& dict = this->GetOutput()->GetMetaDataDictionary();
  itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, m_OutputProjectionRef);
  outputPtr->SetMetaDataDictionary(dict);
}

template <class TInputImage, class TOutputImage, class TDeformationField>
void
GenericRSResampleImageFilter<TInputImage, TOutputImage, TDeformationField>
::UpdateTransform()
{
  // Get the input image 
  const InputImageType* input  = this->GetInput();
  
  // Instanciate the RS Transform
  m_Transform->SetOutputProjectionRef(input->GetProjectionRef());
  m_Transform->SetInputProjectionRef(m_OutputProjectionRef);
  m_Transform->SetOutputKeywordList(input->GetImageKeywordlist());
  m_Transform->SetInputKeywordList(this->GetOutputKeywordList());
  m_Transform->InstanciateTransform();
}

template <class TInputImage, class TOutputImage, class TDeformationField>
void
GenericRSResampleImageFilter<TInputImage, TOutputImage, TDeformationField>
::GenerateInputRequestedRegion()
{
  // Retrieve output pointer
  OutputImageType * outputPtr = this->GetOutput();

  // Retrieve input pointer
  const InputImageType * inputPtr = this->GetInput();
  
  // Retrieve output requested region
  RegionType requestedRegion = outputPtr->GetRequestedRegion();
  
  // Instanciate the RS transform 
  this->UpdateTransform();
  
  // Generate input requested region
  m_Resampler->SetInput(inputPtr);
  m_Resampler->SetTransform(m_Transform);
  m_Resampler->SetDeformationFieldSpacing(this->GetDeformationFieldSpacing());
  m_Resampler->GetOutput()->UpdateOutputInformation();
  m_Resampler->GetOutput()->SetRequestedRegion(requestedRegion);
  m_Resampler->GetOutput()->PropagateRequestedRegion();
}

/**
 * Method used to copy the parameters of the input image
 * 
 */
template <class TInputImage, class TOutputImage, class TDeformationField>
void
GenericRSResampleImageFilter<TInputImage, TOutputImage, TDeformationField>
::SetOutputParametersFromImage(const InputImageType * image)
{
  this->SetOutputOrigin ( image->GetOrigin() );
  this->SetOutputSpacing ( image->GetSpacing() );
  this->SetOutputStartIndex ( image->GetLargestPossibleRegion().GetIndex() );
  this->SetOutputSize ( image->GetLargestPossibleRegion().GetSize() );
}

}
#endif
