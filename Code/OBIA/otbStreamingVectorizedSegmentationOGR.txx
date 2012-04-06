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
::PersistentStreamingLabelImageToOGRDataFilter() : m_TileMaxLabel(0), m_StartLabel(0), m_Use8Connected(false)
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
  
  //std::cout<< "extract region " << extract->GetOutput()->GetLargestPossibleRegion()<<std::endl;
  
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
  m_SegmentationFilter->UpdateLargestPossibleRegion();
  m_SegmentationFilter->Update();
  
  chrono1.Stop();
  std::cout<< "segmentation took " << chrono1.GetTotal() << " sec"<<std::endl;
  
  
  itk::TimeProbe chrono2;
  chrono2.Start();

  labelImageToOGRDataFilter->SetInput(dynamic_cast<LabelImageType *>(m_SegmentationFilter->GetOutputs().at(labelImageIndex).GetPointer()));
  labelImageToOGRDataFilter->SetFieldName(this->GetFieldName());
  labelImageToOGRDataFilter->SetUse8Connected(m_Use8Connected);
  labelImageToOGRDataFilter->Update();
  
  chrono2.Stop();
  std::cout<< "vectorization took " << chrono2.GetTotal() << " sec"<<std::endl;


  //Relabeling
  itk::TimeProbe chrono3;
  chrono3.Start();
  OGRDataSourceObjectType * output = const_cast<OGRDataSourceObjectType *>(labelImageToOGRDataFilter->GetOutput());
  OGRDataSource * poDS = output->Get()->GetDataSource();
  OGRLayer * poLayer = poDS->GetLayer(0);
  unsigned int nbFeatures = poLayer->GetFeatureCount(true);
  unsigned int i = 0;
  unsigned int ind = 0;
  OGRFeature  *poFeature;
  std::map<int,int> relabelMap;
  poLayer->ResetReading();
  while (i<nbFeatures)
  {
     poFeature = poLayer->GetNextFeature();
     int fieldValue = poFeature->GetFieldAsInteger(0);
     if (relabelMap.find(fieldValue) == relabelMap.end())
     {
         relabelMap[fieldValue] = static_cast<int>(ind);
         ind = ind + 1;
     }
     OGRFeature::DestroyFeature( poFeature );
     i++;
  }
  i = 0;
  poLayer->ResetReading();
  while (i<nbFeatures)
  {
     poFeature = poLayer->GetNextFeature();
     int fieldValue = poFeature->GetFieldAsInteger(0);
     int newFieldValue = relabelMap[fieldValue] + m_TileMaxLabel;
     poFeature->SetField(0,newFieldValue);
     poFeature->UnsetField(0);
     poFeature->SetField(this->GetFieldName().c_str(),newFieldValue);
     //Need to rewrite the feature otherwise changes are not considered.
     poLayer->SetFeature(poFeature);
     
     OGRFeature::DestroyFeature( poFeature );
     i++;
  }
  m_TileMaxLabel = m_TileMaxLabel + relabelMap.size();
  std::cout<< "relabel took " << chrono3.GetTotal() << " sec"<<std::endl;
  
  return output;
  return const_cast<OGRDataSourceObjectType *>(labelImageToOGRDataFilter->GetOutput());
}


} // end namespace otb
#endif
