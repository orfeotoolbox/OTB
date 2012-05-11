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
::PersistentStreamingLabelImageToOGRDataFilter() : m_TileMaxLabel(0), m_StartLabel(0), m_Use8Connected(false),
  m_FilterSmallObject(false), m_MinimumObjectSize(1)
{
   this->SetNumberOfInputs(3);
   this->SetNumberOfRequiredInputs(2);
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
::SetInputMask(const LabelImageType *mask)
{
  this->itk::ProcessObject::SetNthInput(2, const_cast<LabelImageType *>(mask));
}

template <class TImageType, class TSegmentationFilter>
const typename PersistentStreamingLabelImageToOGRDataFilter<TImageType, TSegmentationFilter>
::LabelImageType *
PersistentStreamingLabelImageToOGRDataFilter<TImageType, TSegmentationFilter>
::GetInputMask(void)
{
  if (this->GetNumberOfInputs() < 3)
    {
    return 0;
    }

  return static_cast<const LabelImageType *>(this->itk::ProcessObject::GetInput(2));
}

template <class TImageType, class TSegmentationFilter>
void
PersistentStreamingLabelImageToOGRDataFilter<TImageType, TSegmentationFilter>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
}

template <class TImageType, class TSegmentationFilter>
typename PersistentStreamingLabelImageToOGRDataFilter<TImageType, TSegmentationFilter>::OGRDataSourcePointerType
PersistentStreamingLabelImageToOGRDataFilter<TImageType, TSegmentationFilter>
::ProcessTile()
{
  std::cout<< "tile number : " << m_TileNumber <<std::endl;
  m_TileNumber = m_TileNumber + 1;
  itk::TimeProbe tileChrono;
  tileChrono.Start();
  
  // Apply an ExtractImageFilter to avoid problems with filters asking for the LargestPossibleRegion
  typedef itk::ExtractImageFilter<InputImageType, InputImageType> ExtractImageFilterType;
  typename ExtractImageFilterType::Pointer extract = ExtractImageFilterType::New();
  extract->SetInput( this->GetInput() );
  extract->SetExtractionRegion( this->GetInput()->GetRequestedRegion() );
  extract->Update();

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
  
  //RelabelComponentImageFilter to suppress small area
  typename RelabelComponentImageFilterType::Pointer relabelComponentFilter = RelabelComponentImageFilterType::New();
  if(m_FilterSmallObject)
  {
     relabelComponentFilter->SetInput(dynamic_cast<LabelImageType *>(m_SegmentationFilter->GetOutputs().at(labelImageIndex).GetPointer()));
     relabelComponentFilter->SetMinimumObjectSize(m_MinimumObjectSize);
     relabelComponentFilter->Update();
     
     labelImageToOGRDataFilter->SetInputMask(relabelComponentFilter->GetOutput());
  }
  
  itk::TimeProbe chrono2;
  chrono2.Start();
  
  typename LabelImageType::ConstPointer inputMask = this->GetInputMask();
  if (!inputMask.IsNull())
  {
     // Apply an ExtractImageFilter to avoid problems with filters asking for the LargestPossibleRegion
     typedef itk::ExtractImageFilter<LabelImageType, LabelImageType> ExtractLabelImageFilterType;
     typename ExtractLabelImageFilterType::Pointer maskExtract = ExtractLabelImageFilterType::New();
     maskExtract->SetInput( this->GetInputMask() );
     maskExtract->SetExtractionRegion( this->GetInputMask()->GetRequestedRegion() );
     maskExtract->Update();
      
     // WARNING: itk::ExtractImageFilter does not copy the MetadataDictionnary
     maskExtract->GetOutput()->SetMetaDataDictionary(this->GetInputMask()->GetMetaDataDictionary());
     
     if(m_FilterSmallObject)
     {
        typename MultiplyImageFilterType::Pointer multiplyImageFilter = MultiplyImageFilterType::New();
        multiplyImageFilter->SetInput1(maskExtract->GetOutput());
        multiplyImageFilter->SetInput2(relabelComponentFilter->GetOutput());
        multiplyImageFilter->Update();
        labelImageToOGRDataFilter->SetInputMask(multiplyImageFilter->GetOutput());
     }
     else
     {
        labelImageToOGRDataFilter->SetInputMask(maskExtract->GetOutput());
     }
  }
  labelImageToOGRDataFilter->SetInput(dynamic_cast<LabelImageType *>(m_SegmentationFilter->GetOutputs().at(labelImageIndex).GetPointer()));
  labelImageToOGRDataFilter->SetFieldName(this->GetFieldName());
  labelImageToOGRDataFilter->SetUse8Connected(m_Use8Connected);
  labelImageToOGRDataFilter->Update();
  
  chrono2.Stop();
  std::cout<< "vectorization took " << chrono2.GetTotal() << " sec"<<std::endl;


  //Relabeling
  itk::TimeProbe chrono3;
  chrono3.Start();
  OGRDataSourcePointerType tmpDS = const_cast<OGRDataSourceType *>(labelImageToOGRDataFilter->GetOutput());
  OGRLayerType tmpLayer = tmpDS->GetLayer(0);

  unsigned int ind = 0;
  std::map<int,int> relabelMap;
  typename OGRLayerType::const_iterator featIt = tmpLayer.begin();
  for(; featIt!=tmpLayer.end(); ++featIt)
  {
     ogr::Field field = (*featIt)[0];
     int fieldValue = field.GetValue<int>();
     if (relabelMap.find(fieldValue) == relabelMap.end())
     {
         relabelMap[fieldValue] = static_cast<int>(ind);
         ind = ind + 1;
     }
  }
  for(featIt = tmpLayer.begin(); featIt!=tmpLayer.end(); ++featIt)
  {
     ogr::Field field = (*featIt)[0];
     int fieldValue = field.GetValue<int>();
     int newFieldValue = relabelMap[fieldValue] + m_TileMaxLabel;
     field.Unset();
     field.SetValue(newFieldValue);
     //Need to rewrite the feature otherwise changes are not considered.
     tmpLayer.SetFeature(*featIt);
  }
  m_TileMaxLabel = m_TileMaxLabel + relabelMap.size();
  chrono3.Stop();
  std::cout<< "relabel took " << chrono3.GetTotal() << " sec"<<std::endl;
  
  return tmpDS;
}


} // end namespace otb
#endif
