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
::PersistentStreamingLabelImageToVectorDataFilter() : m_FieldName("DN"), m_TileMaxLabel(0), m_StartLabel(0)
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
  labelImageToVectorDataFilter->SetFieldName(m_FieldName);
  labelImageToVectorDataFilter->Update();

  
  //Relabel the vector data
  typename TreeNodeType::Pointer rootNode = const_cast<TreeNodeType *>(labelImageToVectorDataFilter->GetOutput()->GetDataTree()->GetRoot());
  ChildrenListType childList = rootNode->GetChildrenList()[0]->GetChildrenList();

  std::vector<int> fieldValueVector;
  for (typename ChildrenListType::iterator it = childList.begin(); it != childList.end(); ++it)
    {
       typename OutputVectorDataType::DataNodePointerType dataNode = (*it)->Get();
       fieldValueVector.push_back(dataNode->GetFieldAsInt(m_FieldName));
    }
  std::sort(fieldValueVector.begin(), fieldValueVector.end());
  std::map<int,int> relabelMap;
  unsigned int i = 0;
  unsigned int ind = 0;
  for(i = 0; i < fieldValueVector.size(); i++)
  {
     if (relabelMap.find(fieldValueVector.at(i)) == relabelMap.end())
     {
         relabelMap[fieldValueVector.at(i)] = static_cast<int>(ind);
         ind = ind + 1;
     }
  }
  for (typename ChildrenListType::iterator it = childList.begin(); it != childList.end(); ++it)
  {
     typename OutputVectorDataType::DataNodePointerType dataNode = (*it)->Get();
     int newLabel = relabelMap[dataNode->GetFieldAsInt(m_FieldName)] + m_TileMaxLabel;
     dataNode->SetFieldAsInt(m_FieldName, newLabel);
  }
  
  m_TileMaxLabel = m_TileMaxLabel + fieldValueVector.size();

  // return the VectorData in image physical coordinates
  return labelImageToVectorDataFilter->GetOutput();
}


} // end namespace otb
#endif
