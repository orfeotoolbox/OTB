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
#ifndef __otbStreamingVectorizedSegmentationOGR_txx
#define __otbStreamingVectorizedSegmentationOGR_txx

#include "otbStreamingVectorizedSegmentationOGR.h"

#include "otbVectorDataTransformFilter.h"
#include "itkAffineTransform.h"

#include "itkTimeProbe.h"

namespace otb
{

template <class TImageType, class TSegmentationFilter>
PersistentStreamingLabelImageToOGRDataFilter<TImageType, TSegmentationFilter>
::PersistentStreamingLabelImageToOGRDataFilter() : m_FieldName("DN"), m_TileMaxLabel(0), m_StartLabel(0)
{
   m_SegmentationFilter = SegmentationFilterType::New();
   m_TileNumber = 1;
}

template <class TImageType, class TSegmentationFilter>
PersistentStreamingLabelImageToOGRDataFilter<TImageType, TSegmentationFilter>
::~PersistentStreamingLabelImageToOGRDataFilter()
{
}

template <class TImageType, class TSegmentationFilter>
void
PersistentStreamingLabelImageToOGRDataFilter<TImageType, TSegmentationFilter>
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

template <class TImageType, class TSegmentationFilter>
typename PersistentStreamingLabelImageToOGRDataFilter<TImageType, TSegmentationFilter>::OGRDataSourceObjectPointerType
PersistentStreamingLabelImageToOGRDataFilter<TImageType, TSegmentationFilter>
::ProcessTile()
{
  std::cout<< "tile number : " << m_TileNumber <<std::endl;
  m_TileNumber = m_TileNumber + 1;
  itk::TimeProbe tileChrono;
  tileChrono.Start();
  
  
  itk::TimeProbe chrono;
  chrono.Start();
  // Apply an ExtractImageFilter to avoid problems with filters asking for the LargestPossibleRegion
  typedef itk::ExtractImageFilter<InputImageType, InputImageType> ExtractImageFilterType;
  typename ExtractImageFilterType::Pointer extract = ExtractImageFilterType::New();
  extract->SetInput( this->GetInput() );
  extract->SetExtractionRegion( this->GetInput()->GetBufferedRegion() );
  extract->Update();
  
  chrono.Stop();
  //std::cout<< "extract took " << chrono.GetTotal() << " sec"<<std::endl;

  // WARNING: itk::ExtractImageFilter does not copy the MetadataDictionnary
  extract->GetOutput()->SetMetaDataDictionary(this->GetInput()->GetMetaDataDictionary());
  
  const unsigned int labelImageIndex = LabeledOutputAccessor<SegmentationFilterType>::LabeledOutputIndex;

  typename LabelImageToOGRDataSourceFilterType::Pointer labelImageToOGRDataFilter =
                                              LabelImageToOGRDataSourceFilterType::New();
  
  
  itk::TimeProbe chrono1;
  chrono1.Start();
  m_SegmentationFilter->SetInput(extract->GetOutput());
  //m_SegmentationFilter->ResetPipeline();
  //m_SegmentationFilter->Modified();
  m_SegmentationFilter->UpdateLargestPossibleRegion();
  m_SegmentationFilter->Update();
  
  chrono1.Stop();
  std::cout<< "segmentation took " << chrono1.GetTotal() << " sec"<<std::endl;
  
  itk::TimeProbe chrono2;
  chrono2.Start();
  //labelImageToVectorDataFilter->SetInput(dynamic_cast<LabelImageType *>(segFilter->GetOutputs().at(labelImageIndex).GetPointer()));
  labelImageToOGRDataFilter->SetInput(dynamic_cast<LabelImageType *>(m_SegmentationFilter->GetOutputs().at(labelImageIndex).GetPointer()));
  labelImageToOGRDataFilter->SetFieldName(m_FieldName);
  labelImageToOGRDataFilter->Update();
  
  chrono2.Stop();
  std::cout<< "vectorization took " << chrono2.GetTotal() << " sec"<<std::endl;

  //Relabel the vector data
  /*itk::TimeProbe chrono3;
  chrono3.Start();
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
  
  chrono3.Stop();
  //std::cout<< "relabel took " << chrono3.GetTotal() << " sec"<<std::endl;


  tileChrono.Stop();
  std::cout<< "tile processing took " << tileChrono.GetTotal() << " sec"<<std::endl;*/
  // return the VectorData in image physical coordinates
  return const_cast<OGRDataSourceObjectType *>(labelImageToOGRDataFilter->GetOutput());
}


} // end namespace otb
#endif
