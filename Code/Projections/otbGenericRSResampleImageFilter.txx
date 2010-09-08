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
::GenericRSResampleImageFilter() :m_Interpolator(),m_DeformationFieldSpacing(2),  
                                  m_InterpolatorNeighborhoodRadius(1),m_AddedRadius(2)
{
  // default values for Resampled Image
  m_OutputSize.Fill(1);
  m_OutputOrigin.Fill(0);
  m_OutputIndex.Fill(0);
  m_OutputSpacing.Fill(1);
  
  // internal filters instanciation
  m_Resampler  = ResamplerType::New();

  // default identity transform
  m_Transform = GenericRSTransformType::New();
  
  // Setup default interpolator
  typename DefaultInterpolatorType::Pointer interp =  DefaultInterpolatorType::New();
  m_Interpolator = static_cast<InterpolatorType*>( interp.GetPointer() );
}



template <class TInputImage, class TOutputImage, class TDeformationField>
void
GenericRSResampleImageFilter<TInputImage, TOutputImage, TDeformationField>
::GenerateData()
{
  // Get the input image 
  typename InputImageType::Pointer  input  = const_cast<InputImageType *>(this->GetInput());
  
  this->UpdateTransform();
  
  // Remote Sensing images have y axe flipped
  m_DeformationFieldSpacing[1] = -m_DeformationFieldSpacing[1];
   
  // Use the RSTransform with the OptResampleImageFilter
  m_Resampler->SetInput(input);
  m_Resampler->SetTransform(m_Transform);
  m_Resampler->SetDeformationFieldSpacing(m_DeformationFieldSpacing);
  
  // resampler output parameters
  m_Resampler->SetOutputSize(m_OutputSize);
  m_Resampler->SetOutputSpacing(m_OutputSpacing);
  m_Resampler->SetOutputStartIndex(m_OutputIndex);
  m_Resampler->SetOutputOrigin(m_OutputOrigin);

  // expose the metadata to the output of the resample filter 
  itk::MetaDataDictionary&  dict = m_Resampler->GetOutput()->GetMetaDataDictionary();
  itk::EncapsulateMetaData<std::string>(dict, otb::MetaDataKey::ProjectionRefKey,m_OutputProjectionRef);
  
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

  outputPtr->SetSpacing( m_OutputSpacing );
  outputPtr->SetOrigin( m_OutputOrigin );
  
  typename OutputImageType::RegionType region;
  region.SetSize(m_OutputSize);
  region.SetIndex(m_OutputIndex);

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
  typename InputImageType::Pointer  input  = const_cast<InputImageType *>(this->GetInput());
  
  // Instanciate the RS Transform
  m_Transform->SetOutputProjectionRef(input->GetProjectionRef());
  m_Transform->SetOutputKeywordList(input->GetImageKeywordlist());
  m_Transform->SetInputProjectionRef(m_OutputProjectionRef);
  m_Transform->SetInputKeywordList(m_OutputKeywordList);
  m_Transform->InstanciateTransform();
}

template <class TInputImage, class TOutputImage, class TDeformationField>
void
GenericRSResampleImageFilter<TInputImage, TOutputImage, TDeformationField>
::GenerateInputRequestedRegion()
{
  // Instanciate the transform
  this->UpdateTransform();
  
  // call the superclass's implementation
  Superclass::GenerateInputRequestedRegion();
  
  // Load I/O
  typename InputImageType::Pointer  inputImage = const_cast<InputImageType*>(this->GetInput());
  typename OutputImageType::Pointer outputImage = this->GetOutput();
  
  IndexType index = outputImage->GetRequestedRegion().GetIndex();
  SizeType  size = outputImage->GetRequestedRegion().GetSize();
  
  // Obtain coordinates of upperleft, upperright, lowerleft and lowerright points in the image
  IndexType                                 indexTmp;
  std::vector<IndexType>                    vPoints;
  typename std::vector<IndexType>::iterator it;

  indexTmp[0] = index[0];
  indexTmp[1] = index[1];
  vPoints.push_back(indexTmp);

  indexTmp[0] = index[0] + size[0];
  indexTmp[1] = index[1];
  vPoints.push_back(indexTmp);

  indexTmp[0] = index[0] + size[0];
  indexTmp[1] = index[1] + size[1];
  vPoints.push_back(indexTmp);

  indexTmp[0] = index[0];
  indexTmp[1] = index[1] + size[1];
  vPoints.push_back(indexTmp);

  typedef double CoordRepType; // to maintain compatibility with ITK

  typedef itk::ContinuousIndex<CoordRepType, 2> ContinuousIndexType;
  typename ContinuousIndexType::ValueType minX = itk::NumericTraits<typename ContinuousIndexType::ValueType>::max();
  typename ContinuousIndexType::ValueType maxX = 0;
  typename ContinuousIndexType::ValueType minY = itk::NumericTraits<typename ContinuousIndexType::ValueType>::max();
  typename ContinuousIndexType::ValueType maxY = 0;

  // Coordinates of current output pixel
  PointType outputPoint;
  PointType inputPoint;

  // Transform each "corner" point
  for (it = vPoints.begin(); it != vPoints.end(); ++it)
    {
    // Build the output point location
    // Not good because the matrice indexToPhysicalPoint is identity :
    // not updated.....
    outputImage->TransformIndexToPhysicalPoint(*it, outputPoint);
      
    // Compute corresponding input pixel continuous index
    ContinuousIndexType indexTmpTr;
    inputPoint = m_Transform->TransformPoint(outputPoint);
    inputImage->TransformPhysicalPointToContinuousIndex(inputPoint,indexTmpTr);
      
    //std::cout <<"########: *it" << *it <<" outputPoint " << outputPoint  << " -->inputPoint  " <<inputPoint <<" --> physitoIndex "<< indexTmpTr<< std::endl;

    if (indexTmpTr[0] > maxX) maxX = indexTmpTr[0];

    if (indexTmpTr[0] < minX) minX = indexTmpTr[0];

    if (indexTmpTr[1] > maxY) maxY = indexTmpTr[1];

    if (indexTmpTr[1] < minY) minY = indexTmpTr[1];
    }

  // Create region needed in previous filter in the pipeline, which is the bounding box of previous transformed points
  typename InputImageType::RegionType region;
  index[0] = static_cast<long int>(minX);
  index[1] = static_cast<long int>(minY);
  size[0] = static_cast<long unsigned int>(maxX - minX);
  size[1] = static_cast<long unsigned int>(maxY - minY);

  region.SetSize(size);
  region.SetIndex(index);

  // Grow region to be sure that interpolator can found needed point on image borders
  unsigned int neededRadius =
    StreamingTraits<typename Superclass::InputImageType>::CalculateNeededRadiusForInterpolator(this->GetInterpolator());
    
  if (neededRadius == 0)
    {
    neededRadius = m_InterpolatorNeighborhoodRadius;
    }
  region.PadByRadius(neededRadius + m_AddedRadius);
    
  // If requested region is not contained in input image, then result region is null
  if (!region.Crop(inputImage->GetLargestPossibleRegion()))
    {
    index[0] = 0;
    index[1] = 0;
    size[0] = 0;
    size[1] = 0;
    region.SetIndex(index);
    region.SetSize(size);
    }
  inputImage->SetRequestedRegion(region);
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
  this->SetOutputIndex ( image->GetLargestPossibleRegion().GetIndex() );
  this->SetOutputSize ( image->GetLargestPossibleRegion().GetSize() );
}

}
#endif
