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

#include "projection/ossimUtmProjection.h"
#include "itkPoint.h"

namespace otb
{

template <class TInputImage, class TOutputImage>
GenericRSResampleImageFilter<TInputImage, TOutputImage>
::GenericRSResampleImageFilter()
{  
  // flags initialization
  m_EstimateInputRpcModel  = false;
  m_EstimateOutputRpcModel = false;
  m_RpcEstimationUpdated   = false;

  // internal filters instanciation
  m_Resampler         = ResamplerType::New();
  m_InputRpcEstimator = InputRpcModelEstimatorType::New();
  m_OutputRpcEstimator= OutputRpcModelEstimatorType::New();
  m_Transform         = GenericRSTransformType::New();
}

template <class TInputImage, class TOutputImage>
void
GenericRSResampleImageFilter<TInputImage, TOutputImage>
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
template <class TInputImage, class TOutputImage>
void
GenericRSResampleImageFilter<TInputImage, TOutputImage>
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
  
  // Get the Output MetaData Dictionary
  itk::MetaDataDictionary& dict = outputPtr->GetMetaDataDictionary();
  
  // Encapsulate the   metadata set by the user
  itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, 
                                        this->GetOutputProjectionRef());
  
  if (this->GetOutputKeywordList().GetSize() > 0)
    {
    itk::EncapsulateMetaData<ImageKeywordlist>(dict, MetaDataKey::OSSIMKeywordlistKey, 
                                               this->GetOutputKeywordList());
    }
  
  // Estimate the output rpc Model if needed
  if (m_EstimateOutputRpcModel)
    this->EstimateOutputRpcModel();
}

/**
 * Method to estimate the rpc model of the output using a temporary image
 */
template <class TInputImage, class TOutputImage>
void
GenericRSResampleImageFilter<TInputImage, TOutputImage>
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
  m_OutputRpcEstimator->UpdateOutputInformation();
  
  // Encapsulate the estimated rpc model in the output
  if (m_OutputRpcEstimator->GetOutput()->GetImageKeywordlist().GetSize() > 0)
    {
    // Fill the output dict
    itk::EncapsulateMetaData<ImageKeywordlist>(dict,
                                               MetaDataKey::OSSIMKeywordlistKey,  
                                               m_OutputRpcEstimator->GetOutput()->GetImageKeywordlist());
    // Fill the transform with the right kwl
    m_Transform->SetInputKeywordList( m_OutputRpcEstimator->GetOutput()->GetImageKeywordlist());
    }
}

/** 
  * Fill with the default dict of the input and the output
  * and instanciate the transform
  */
template <class TInputImage, class TOutputImage>
void
GenericRSResampleImageFilter<TInputImage, TOutputImage>
::UpdateTransform()
{
  m_Transform->SetOutputDictionary(this->GetInput()->GetMetaDataDictionary());
  m_Transform->SetOutputProjectionRef(this->GetInput()->GetProjectionRef());
  m_Transform->SetOutputKeywordList(this->GetInput()->GetImageKeywordlist());
  m_Transform->InstanciateTransform();
 }

 /**
  * Generate Input requested region does only propagate the output
  * requested region. 
  */
 template <class TInputImage, class TOutputImage>
 void
 GenericRSResampleImageFilter<TInputImage, TOutputImage>
 ::GenerateInputRequestedRegion()
 {
   // Retrieve output pointer
   OutputImageType * outputPtr = this->GetOutput();

   // Retrieve input pointer
   const InputImageType * inputPtr = this->GetInput();

   // Retrieve output requested region
   RegionType requestedRegion = outputPtr->GetRequestedRegion();

   // Estimate the input rpc model if it is needed
   if (m_EstimateInputRpcModel && !m_RpcEstimationUpdated)
     {
     this->EstimateInputRpcModel();
     }

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
  * Method to estimate the rpc model of the input using a temporary
  * image to avoid adding this rpc estimator filter in the minipipeline.
  * 
  */
 template <class TInputImage, class TOutputImage>
 void
 GenericRSResampleImageFilter<TInputImage, TOutputImage>
 ::EstimateInputRpcModel()
 {
   // Temp image : not allocated but with the sampe metadata as the
   // output 
   typename InputImageType::Pointer tempPtr = InputImageType::New();
   tempPtr->SetRegions(this->GetInput()->GetLargestPossibleRegion());
   tempPtr->CopyInformation(this->GetInput());

   // Estimate the rpc model with the temp image
   m_InputRpcEstimator->SetInput(tempPtr);
   m_InputRpcEstimator->UpdateOutputInformation();

   // No need to override the input kwl, just setup the 
   // transform with the kwl estimated
   if(m_InputRpcEstimator->GetInput()->GetImageKeywordlist().GetSize() > 0)
     m_Transform->SetOutputKeywordList(m_InputRpcEstimator->GetOutput()->GetImageKeywordlist());
  
  // Update the flag for input rpcEstimation in order to not compute
  // the rpc model for each stream
  m_RpcEstimationUpdated = true;
}

/**
 * Method used to copy the parameters of the input image
 * 
 */
template <class TInputImage, class TOutputImage>
void
GenericRSResampleImageFilter<TInputImage, TOutputImage>
::SetOutputParametersFromImage(const ImageBaseType * image)
{
  this->SetOutputOrigin ( image->GetOrigin() );
  this->SetOutputSpacing ( image->GetSpacing() );
  this->SetOutputStartIndex ( image->GetLargestPossibleRegion().GetIndex() );
  this->SetOutputSize ( image->GetLargestPossibleRegion().GetSize() );
  this->SetOutputProjectionRef(image->GetProjectionRef());
  this->SetOutputKeywordList(image->GetImageKeywordlist());
}
/**
 * Method used to copy the parameters of the input image
 * 
 */
template <class TInputImage, class TOutputImage>
void
GenericRSResampleImageFilter<TInputImage, TOutputImage>
::SetOutputParametersFromMap(const std::string map)
{
  // Get the input Image
  const InputImageType* input = this->GetInput();
  
  // Update the transform with input information 
  // Done here because the transform is not instanciated 
  // yet
  this->UpdateTransform();
  
  // The inverse transform is need here
  GenericRSTransformPointerType invTransform = GenericRSTransformType::New();
  m_Transform->GetInverse(invTransform);
  
  // Build the UTM transform : Need the zone & the hemisphere
  // For this we us the geographic coordinate of the input UL corner
  typedef ossimRefPtr<ossimUtmProjection>       OssimMapProjectionPointerType;
  typedef itk::Point<double,2>                  GeoPointType;
  
  // instanciate the projection to get the utm zone
  OssimMapProjectionPointerType  utmMapProjection =  new ossimUtmProjection();
  
  // get the utm zone and hemisphere using the input UL corner
  // geographic coordinates
  typename InputImageType::PointType  pSrc;
  IndexType      index;
  GeoPointType   geoPoint;
  index[0] = input->GetLargestPossibleRegion().GetIndex()[0];
  index[1] = input->GetLargestPossibleRegion().GetIndex()[1];
  input->TransformIndexToPhysicalPoint(index,pSrc);
  
  // The first transform of the inverse transform : input -> WGS84
  geoPoint = invTransform->GetTransform()->GetFirstTransform()->TransformPoint(pSrc);
  
  // Guess the zone and the hemisphere
  ossimGpt point(geoPoint[1],  geoPoint[0]);
  int zone = utmMapProjection->computeZone(point);
  bool hem = (geoPoint[1]>1e-10)?true:false;
  
  // Build the output UTM projection ref 
  OGRSpatialReference oSRS;
  oSRS.SetProjCS("UTM");
  oSRS.SetWellKnownGeogCS("WGS84");
  oSRS.SetUTM(zone, hem);
  
  char * utmRef = NULL;
  oSRS.exportToWkt(&utmRef);
    
  // Update the transform
  this->SetOutputProjectionRef(utmRef);
  this->UpdateTransform();

  // Get the inverse transform again : used later
  m_Transform->GetInverse(invTransform);
  
  // Compute the 4 corners in the cartographic coordinate system
  std::vector<IndexType>       vindex;
  std::vector<OutputPointType> voutput;
  
  IndexType index1, index2, index3, index4;
  SizeType  size;

  // Image size
  size = input->GetLargestPossibleRegion().GetSize();

  // project the 4 corners
  index1 = input->GetLargestPossibleRegion().GetIndex();
  index2 = input->GetLargestPossibleRegion().GetIndex();
  index3 = input->GetLargestPossibleRegion().GetIndex();
  index4 = input->GetLargestPossibleRegion().GetIndex();

  index2[0] += size[0] - 1;
  index3[0] += size[0] - 1;
  index3[1] += size[1] - 1;
  index4[1] += size[1] - 1;

  vindex.push_back(index1);
  vindex.push_back(index2);
  vindex.push_back(index3);
  vindex.push_back(index4);

  for (unsigned int i = 0; i < vindex.size(); i++)
    {
    OutputPointType physicalPoint;
    this->GetInput()->TransformIndexToPhysicalPoint(vindex[i], physicalPoint);
    voutput.push_back(invTransform->TransformPoint(physicalPoint));
    }

  // Compute the boundaries
  double minX = voutput[0][0];
  double maxX = voutput[0][0];
  double minY = voutput[0][1];
  double maxY = voutput[0][1];

  for (unsigned int i = 0; i < voutput.size(); i++)
    {
    // Origins
    if (minX > voutput[i][0])
      minX = voutput[i][0];
    if (minY > voutput[i][1])
      minY = voutput[i][1];

    // Sizes
    if (maxX < voutput[i][0])
      maxX = voutput[i][0];

    if (maxY < voutput[i][1])
      maxY = voutput[i][1];
    }
  
  // Compute the output size
  double sizeCartoX = vcl_abs(maxX - minX);
  double sizeCartoY = vcl_abs(minY - maxY);

  // Set the output orgin in carto 
  // projection
  OriginType   origin;
  origin[0] = minX;
  origin[1] = maxY;
  this->SetOutputOrigin(origin);
  
  // Evaluate output size
  SizeType outputSize;
  outputSize[0] = static_cast<unsigned int>(vcl_floor(vcl_abs(sizeCartoX / this->GetOutputSpacing()[0])));
  outputSize[1] = static_cast<unsigned int>(vcl_floor(vcl_abs(sizeCartoY / this->GetOutputSpacing()[1])));
  this->SetOutputSize(outputSize);
  
  std::cout <<"Output Image params :"
            << " \n OutputSize "   << outputSize 
            << " \n OutputOrigin " << origin 
            << " \n OutputSpacing "<< this->GetOutputSpacing()
            << std::endl;
}

}
#endif
