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
 *  Generate the right output requested region following the
 *  parameters set by the user
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
  // GetInputProjectionRef is used here cause the RS transform is
  // inversed : Output -> Input
  itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, 
                                        this->GetInputProjectionRef());
  outputPtr->SetMetaDataDictionary(dict);
}


/**
 * Generate Input requested region does only propagate the output
 * requested region. 
 */
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
  m_Transform->InstanciateTransform();
  
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
