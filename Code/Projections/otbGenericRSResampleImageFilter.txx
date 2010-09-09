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
::GenericRSResampleImageFilter():m_EstimateInputRpcModel(false),
                                 m_EstimateOutputRpcModel(false)
{  
  // internal filters instanciation
  m_Resampler         = ResamplerType::New();
  m_InputRpcEstimator = RpcModelEstimatorType::New();
  m_OutputRpcEstimator = RpcModelEstimatorType::New();
  m_Transform         = GenericRSTransformType::New();
  
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
  std::cout <<"GenerateOutputInformation" << std::endl;
  // call the superclass's implementation of this method
  Superclass::GenerateOutputInformation();
  
  typename OutputImageType::Pointer outputPtr = this->GetOutput();

  outputPtr->SetSpacing( this->GetOutputSpacing() );
  outputPtr->SetOrigin( this->GetOutputOrigin() );
  
  typename OutputImageType::RegionType region;
  region.SetSize(this->GetOutputSize());
  region.SetIndex(this->GetOutputStartIndex() );

  outputPtr->SetLargestPossibleRegion(region);
  
  // Get the Output MetaData Dictionary
  itk::MetaDataDictionary& dict = outputPtr->GetMetaDataDictionary();
  // Encapsulate the output  metadata 
  itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, 
                                        this->GetOutputProjectionRef());
  
  if (this->GetOutputKeywordList().GetSize() > 0)
    {
    itk::EncapsulateMetaData<ImageKeywordlist>(dict, MetaDataKey::OSSIMKeywordlistKey, 
                                               this->GetOutputKeywordList());
    }
  
  // Estimate the rpc Model 
  if (m_EstimateOutputRpcModel)
    this->EstimateOutputRpcModel();
}

/**
 * Method to estimate the rpc model of the output using a temporary image
 */
template <class TInputImage, class TOutputImage, class TDeformationField>
void
GenericRSResampleImageFilter<TInputImage, TOutputImage, TDeformationField>
::EstimateOutputRpcModel()
{
  // Get the output dictionary
  itk::MetaDataDictionary& dict = this->GetOutput()->GetMetaDataDictionary();
  
  // Temp image : not allocated but with the sampe metadata as the
  // output 
  typename OutputImageType::Pointer tempPtr = OutputImageType::New();
  tempPtr->SetRegions(this->GetOutput()->GetLargestPossibleRegion());

  // Encapsulate the output metadata in the temp image
  itk::MetaDataDictionary& tempDict = tempPtr->GetMetaDataDictionary();
  itk::EncapsulateMetaData<std::string>(tempDict, MetaDataKey::ProjectionRefKey, 
                                        this->GetOutputProjectionRef() );
  itk::EncapsulateMetaData<ImageKeywordlist>(tempDict, MetaDataKey::OSSIMKeywordlistKey, 
                                             this->GetOutputKeywordList());
  
  // Estimate the rpc model from the temp image
  m_OutputRpcEstimator->SetInput(tempPtr);
  m_OutputRpcEstimator->GenerateOutputInformation();
  
  // Encapsulate the estimated rpc model in the output
  if(m_OutputRpcEstimator->GetOutput()->GetImageKeywordlist().GetSize() > 0)
    itk::EncapsulateMetaData<ImageKeywordlist>(dict,
                                               MetaDataKey::OSSIMKeywordlistKey,  
                                               m_OutputRpcEstimator->GetOutput()->GetImageKeywordlist());
}



/**
 * Use the rpc keywordlist if any rpc model estimated
 */
template <class TInputImage, class TOutputImage, class TDeformationField>
void
GenericRSResampleImageFilter<TInputImage, TOutputImage, TDeformationField>
::UpdateTransform()
{
  if(m_EstimateInputRpcModel)
    {
    m_Transform->SetOutputKeywordList( m_InputRpcEstimator->GetOutput()->GetImageKeywordlist());
    }

  if(m_EstimateOutputRpcModel)
    {
    m_Transform->SetInputKeywordList( m_OutputRpcEstimator->GetOutput()->GetImageKeywordlist());
    }
  
  m_Transform->InstanciateTransform();
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
  
  // Generate input requested region
  if (m_EstimateInputRpcModel)
    {
    m_InputRpcEstimator->SetInput(this->GetInput());
    m_Resampler->SetInput(m_InputRpcEstimator->GetOutput());
    }
  else
    {
    m_Resampler->SetInput(this->GetInput());
    }
  
  // Instanciate the RS transform 
  this->UpdateTransform();
  
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
