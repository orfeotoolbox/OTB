/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbStreamingVectorizedSegmentation_txx
#define __otbStreamingVectorizedSegmentation_txx

#include "otbStreamingVectorizedSegmentation.h"

#include "otbVectorDataTransformFilter.h"
#include "itkAffineTransform.h"

namespace otb
{

template <class TImageType, class TOutputVectorData, class TSegmentationFilter>
PersistentStreamingLabelImageToVectorDataFilter<TImageType, TOutputVectorData, TSegmentationFilter>
::PersistentStreamingLabelImageToVectorDataFilter()
{
   m_SegmentationFilter = SegmentationFilterType::New();
}

template <class TImageType, class TOutputVectorData, class TSegmentationFilter>
PersistentStreamingLabelImageToVectorDataFilter<TImageType, TOutputVectorData, TSegmentationFilter>
::~PersistentStreamingLabelImageToVectorDataFilter()
{
}

template <class TImageType, class TOutputVectorData, class TSegmentationFilter>
void
PersistentStreamingLabelImageToVectorDataFilter<TImageType, TOutputVectorData, TSegmentationFilter>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();

  if (this->GetInput())
    {
    InputImagePointerType input = const_cast<InputImageType *> (this->GetInput());

    typename InputImageType::RegionType region = this->GetOutput()->GetRequestedRegion();

    region.PadByRadius(1);
    region.Crop(input->GetLargestPossibleRegion());

    input->SetRequestedRegion(region);
    }
}

template <class TImageType, class TOutputVectorData, class TSegmentationFilter>
typename PersistentStreamingLabelImageToVectorDataFilter<TImageType, TOutputVectorData, TSegmentationFilter>::OutputVectorDataPointerType
PersistentStreamingLabelImageToVectorDataFilter<TImageType, TOutputVectorData, TSegmentationFilter>
::ProcessTile()
{
  // Apply an ExtractImageFilter to avoid problems with filters asking for the LargestPossibleRegion
  typedef itk::ExtractImageFilter<InputImageType, InputImageType> ExtractImageFilterType;
  typename ExtractImageFilterType::Pointer extract = ExtractImageFilterType::New();
  extract->SetInput( this->GetInput() );
  extract->SetExtractionRegion( this->GetInput()->GetBufferedRegion() );
  extract->Update();

  // WARNING: itk::ExtractImageFilter does not copy the MetadataDictionnary
  extract->GetOutput()->SetMetaDataDictionary(this->GetInput()->GetMetaDataDictionary());
  
  const unsigned int labelImageIndex = LabeledOutputAccessor<SegmentationFilterType>::LabeledOutputIndex;

  typename LabelImageToVectorDataFilterType::Pointer labelImageToVectorDataFilter = LabelImageToVectorDataFilterType::New();
  
  m_SegmentationFilter->SetInput(extract->GetOutput());
  m_SegmentationFilter->UpdateOutputInformation();
  m_SegmentationFilter->Update();
  
  labelImageToVectorDataFilter->SetInput(dynamic_cast<LabelImageType *>(m_SegmentationFilter->GetOutputs().at(labelImageIndex).GetPointer()));
  labelImageToVectorDataFilter->Update();
  

  // return the VectorData in image physical coordinates
  return labelImageToVectorDataFilter->GetOutput();
}


} // end namespace otb
#endif
