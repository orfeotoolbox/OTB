/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbGenericRSResampleImageFilter_hxx
#define otbGenericRSResampleImageFilter_hxx

#include "otbGenericRSResampleImageFilter.h"

#include "itkMetaDataObject.h"

#include "itkProgressAccumulator.h"

#include "itkPoint.h"

#include "ogr_spatialref.h"
#include "cpl_conv.h"

#include "otbSpatialReference.h"
#include "otbImageToGenericRSOutputParameters.h"

namespace otb
{

template <class TInputImage, class TOutputImage>
GenericRSResampleImageFilter<TInputImage, TOutputImage>::GenericRSResampleImageFilter()
{
  // flags initialization
  m_EstimateInputRpcModel  = false;
  m_EstimateOutputRpcModel = false;
  m_RpcEstimationUpdated   = false;

  // internal filters instantiation
  m_Resampler          = ResamplerType::New();
  m_InputRpcEstimator  = InputRpcModelEstimatorType::New();
  m_OutputRpcEstimator = OutputRpcModelEstimatorType::New();
  m_Transform          = GenericRSTransformType::New();

  /** Set number of threads to 1 for Displacement field generator (use for faster access to
    * OSSIM elevation source, which does not handle multithreading when accessing to DEM data) */
  this->SetDisplacementFilterNumberOfThreads(1);
}

template <class TInputImage, class TOutputImage>
void GenericRSResampleImageFilter<TInputImage, TOutputImage>::GenerateData()
{
  // Set up progress reporting
  typename itk::ProgressAccumulator::Pointer progress = itk::ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);
  progress->RegisterInternalFilter(m_Resampler, 1.f);

  m_Resampler->GraftOutput(this->GetOutput());
  m_Resampler->UpdateOutputData(m_Resampler->GetOutput());
  this->GraftOutput(m_Resampler->GetOutput());
}


/**
 *  Generate the right output requested region following the
 *  parameters set by the user
 *
 */
template <class TInputImage, class TOutputImage>
void GenericRSResampleImageFilter<TInputImage, TOutputImage>::GenerateOutputInformation()
{
  // Estimate the output rpc Model if needed
  if (m_EstimateOutputRpcModel)
    this->EstimateOutputRpcModel();

  // Estimate the input rpc model if it is needed
  if (m_EstimateInputRpcModel && !m_RpcEstimationUpdated)
  {
    this->EstimateInputRpcModel();
  }

  // Instantiate the RS transform
  this->UpdateTransform();

  m_Resampler->SetInput(this->GetInput());
  m_Resampler->SetTransform(m_Transform);
  m_Resampler->SetDisplacementFieldSpacing(this->GetDisplacementFieldSpacing());
  m_Resampler->GraftOutput(this->GetOutput());
  m_Resampler->UpdateOutputInformation();
  this->GraftOutput(m_Resampler->GetOutput());

  // Encapsulate output projRef and metadata
  if (this->GetOutputImageMetadata() != nullptr)
    this->GetOutput()->m_Imd.Merge(*(this->GetOutputImageMetadata()));
  this->GetOutput()->m_Imd.Add(MDGeom::ProjectionWKT, this->GetOutputProjectionRef());
}

/**
 * Method to estimate the rpc model of the output using a temporary image
 */
template <class TInputImage, class TOutputImage>
void GenericRSResampleImageFilter<TInputImage, TOutputImage>::EstimateOutputRpcModel()
{
  // Temp image : not allocated but with the same metadata than the
  // output
  typename OutputImageType::Pointer tempPtr = OutputImageType::New();

  typename OutputImageType::RegionType region;
  region.SetSize(this->GetOutputSize());
  region.SetIndex(this->GetOutputStartIndex());
  tempPtr->SetRegions(region);

  // Encapsulate the output metadata in the temp image
  tempPtr->m_Imd.Add(MDGeom::ProjectionWKT, this->GetOutputProjectionRef());
  tempPtr->SetImageMetadata(*(this->GetOutputImageMetadata()));

  // Estimate the rpc model from the temp image
  m_OutputRpcEstimator->SetInput(tempPtr);
  m_OutputRpcEstimator->UpdateOutputInformation();

  // Fill the transform with the right metadata
  m_Transform->SetInputImageMetadata(&(m_OutputRpcEstimator->GetOutput()->GetImageMetadata()));
}

/**
  * Fill with the default dict of the input and the output
  * and instantiate the transform
  */
template <class TInputImage, class TOutputImage>
void GenericRSResampleImageFilter<TInputImage, TOutputImage>::UpdateTransform()
{
  if (!m_EstimateInputRpcModel)
  {
    m_Transform->SetOutputImageMetadata(&(this->GetInput()->GetImageMetadata()));
    m_Transform->SetOutputProjectionRef(this->GetInput()->GetProjectionRef());
  }
  m_Transform->InstantiateTransform();
}

template <class TInputImage, class TOutputImage>
void GenericRSResampleImageFilter<TInputImage, TOutputImage>::PropagateRequestedRegion(itk::DataObject* output)
{
  if (this->m_Updating)
    return;

  // Retrieve output requested region
  m_Resampler->GetOutput()->SetRequestedRegion(output);
  m_Resampler->GetOutput()->PropagateRequestedRegion();
}


/**
 * Method to estimate the rpc model of the input using a temporary
 * image to avoid adding this rpc estimator filter in the minipipeline.
 *
 */
template <class TInputImage, class TOutputImage>
void GenericRSResampleImageFilter<TInputImage, TOutputImage>::EstimateInputRpcModel()
{
  // Temp image : not allocated but with the sampe metadata as the
  // output
  typename InputImageType::Pointer tempPtr = InputImageType::New();
  tempPtr->SetRegions(this->GetInput()->GetLargestPossibleRegion());
  tempPtr->CopyInformation(this->GetInput());

  // Estimate the rpc model with the temp image
  m_InputRpcEstimator->SetInput(tempPtr);
  m_InputRpcEstimator->UpdateOutputInformation();

  // setup the transform with the estimated RPC model
  m_Transform->SetOutputImageMetadata(&(m_InputRpcEstimator->GetOutput()->GetImageMetadata()));

  // Update the flag for input rpcEstimation in order to not compute
  // the rpc model for each stream
  m_RpcEstimationUpdated = true;
}

/**
 * Method used to copy the parameters of the input image
 *
 */
template <class TInputImage, class TOutputImage>
void GenericRSResampleImageFilter<TInputImage, TOutputImage>::SetOutputParametersFromImage(const ImageBaseType* image)
{
  const InputImageType* src = dynamic_cast<const InputImageType*>(image);

  this->SetOutputOrigin(src->GetOrigin());
  this->SetOutputSpacing(src->GetSignedSpacing());
  this->SetOutputStartIndex(src->GetLargestPossibleRegion().GetIndex());
  this->SetOutputSize(src->GetLargestPossibleRegion().GetSize());
  this->SetOutputProjectionRef(src->GetProjectionRef());
  this->GetOutput()->SetImageMetadata(src->GetImageMetadata());
}

/**
 * Method used to copy the parameters of the input image
 *
 */
template <class TInputImage, class TOutputImage>
template <class TImageType>
void GenericRSResampleImageFilter<TInputImage, TOutputImage>::SetOutputParametersFromImage(const TImageType* image)
{
  this->SetOutputOrigin(image->GetOrigin());
  this->SetOutputSpacing(image->GetSignedSpacing());
  this->SetOutputStartIndex(image->GetLargestPossibleRegion().GetIndex());
  this->SetOutputSize(image->GetLargestPossibleRegion().GetSize());
  this->SetOutputProjectionRef(image->GetProjectionRef());
  this->GetOutput()->SetImageMetadata(image->GetImageMetadata());
}

/**
 * Method used to project the input image in a defined srs, estimating
 * the output size and origin. The spacing is set by the user. The
 * supported projection are UTM and WGS84.
 *
 */
template <class TInputImage, class TOutputImage>
void GenericRSResampleImageFilter<TInputImage, TOutputImage>::SetOutputParametersFromMap(const std::string map, const SpacingType& spacing)
{
  // Get the input Image
  const InputImageType* input = this->GetInput();

  // Update the transform with input information
  // Done here because the transform is not instantiated
  // yet
  this->UpdateTransform();

  // Needed variable
  std::string projectionRef;
  // The inverse transform is need here
  GenericRSTransformPointerType invTransform = GenericRSTransformType::New();
  m_Transform->GetInverse(invTransform);

  if (map == "UTM")
  {
    // Build the UTM transform : Need the zone & the hemisphere
    // For this we us the geographic coordinate of the input UL corner
    typedef itk::Point<double, 2> GeoPointType;

    // get the utm zone and hemisphere using the input UL corner
    // geographic coordinates
    typename InputImageType::PointType pSrc;
    IndexType                          index;
    GeoPointType                       geoPoint;
    index[0] = input->GetLargestPossibleRegion().GetIndex()[0];
    index[1] = input->GetLargestPossibleRegion().GetIndex()[1];
    input->TransformIndexToPhysicalPoint(index, pSrc);

    // The first transform of the inverse transform : input -> WGS84
    geoPoint = invTransform->GetTransform()->GetFirstTransform()->TransformPoint(pSrc);

    // Guess the zone and the hemisphere
    unsigned int                      zone(0);
    otb::SpatialReference::hemisphere hem;
    otb::SpatialReference::UTMFromGeoPoint(geoPoint[0], geoPoint[1], zone, hem);

    otb::SpatialReference oSRS = otb::SpatialReference::FromUTM(zone, hem);

    projectionRef = oSRS.ToWkt();
  }
  else if (map == "WGS84")
  {
    projectionRef = otb::SpatialReference::FromWGS84().ToWkt(); // WGS84
  }
  else
  {
    itkExceptionMacro("The output map " << map << "is not supported, please try UTM or WGS84");
  }

  // Compute the output parameters
  typedef otb::ImageToGenericRSOutputParameters<InputImageType> OutputParametersEstimatorType;
  typename OutputParametersEstimatorType::Pointer               genericRSEstimator = OutputParametersEstimatorType::New();

  genericRSEstimator->SetInput(input);
  genericRSEstimator->SetOutputProjectionRef(projectionRef);
  genericRSEstimator->ForceSpacingTo(spacing);
  genericRSEstimator->Compute();

  // Update the Output Parameters
  this->SetOutputProjectionRef(projectionRef);
  this->SetOutputOrigin(genericRSEstimator->GetOutputOrigin());
  this->SetOutputSpacing(genericRSEstimator->GetOutputSpacing());
  this->SetOutputSize(genericRSEstimator->GetOutputSize());
  this->UpdateTransform();
}

/**
 * Used to project the input image in a srs defined by its WKT
 * projectionRef (as parameter) only. estimating the output size, spacing
 * and origin.
 *
 */
template <class TInputImage, class TOutputImage>
void GenericRSResampleImageFilter<TInputImage, TOutputImage>::SetOutputParametersFromMap(const std::string projectionRef)
{
  const InputImageType* input = this->GetInput();

  // Compute the output parameters
  typedef otb::ImageToGenericRSOutputParameters<InputImageType> OutputParametersEstimatorType;
  typename OutputParametersEstimatorType::Pointer               genericRSEstimator = OutputParametersEstimatorType::New();

  genericRSEstimator->SetInput(input);
  genericRSEstimator->SetOutputProjectionRef(projectionRef);
  genericRSEstimator->Compute();

  // Update the Output Parameters
  this->SetOutputProjectionRef(projectionRef);
  this->SetOutputOrigin(genericRSEstimator->GetOutputOrigin());
  this->SetOutputSpacing(genericRSEstimator->GetOutputSpacing());
  this->SetOutputSize(genericRSEstimator->GetOutputSize());
  this->UpdateTransform();
}

template <class TInputImage, class TOutputImage>
void GenericRSResampleImageFilter<TInputImage, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "EstimateInputRpcModel:" << (m_EstimateInputRpcModel ? "On" : "Off") << std::endl;
  os << indent << "EstimateOutputRpcModel:" << (m_EstimateOutputRpcModel ? "On" : "Off") << std::endl;
  os << indent << "RpcEstimationUpdated:" << (m_RpcEstimationUpdated ? "True" : "False") << std::endl;
  os << indent << "OutputOrigin: " << m_Resampler->GetOutputOrigin() << std::endl;
  os << indent << "OutputSpacing: " << m_Resampler->GetOutputSpacing() << std::endl;
  os << indent << "OutputStartIndex: " << m_Resampler->GetOutputStartIndex() << std::endl;
  os << indent << "OutputSize: " << m_Resampler->GetOutputSize() << std::endl;
  os << indent << "GenericRSTransform: " << std::endl;
  m_Transform->Print(os, indent.GetNextIndent());
}
}
#endif
