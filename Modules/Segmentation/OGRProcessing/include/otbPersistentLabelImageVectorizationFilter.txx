/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
 
 
#ifndef PersistentLabelImageVectorizationFilter_txx
#define PersistentLabelImageVectorizationFilter_txx

#include "itkDefaultConvertPixelTraits.h"
#include "itkProgressReporter.h"

#include "otbOGRFieldWrapper.h"

#include "otbPersistentLabelImageVectorizationFilter.h"

// --------- otb::PersistentLabelImageVectorizationFilter ---------------------

namespace otb
{
template<class TInputImage>
PersistentLabelImageVectorizationFilter<TInputImage>
::PersistentLabelImageVectorizationFilter() : m_TileNumber(1), m_Enlarge(0), m_Tolerance(0)
{
  this->SetNumberOfRequiredOutputs(2);
}

template<class TInputImage>
void
PersistentLabelImageVectorizationFilter<TInputImage>
::Reset(void)
{
}

template<class TInputImage>
void
PersistentLabelImageVectorizationFilter<TInputImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
}

template<class TInputImage>
void
PersistentLabelImageVectorizationFilter<TInputImage>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
}

  
template<class TInputImage>
typename PersistentLabelImageVectorizationFilter<TInputImage>::OGRDataSourcePointerType
PersistentLabelImageVectorizationFilter<TInputImage>
::ProcessTile()
{  
  ++m_TileNumber;
  // Apply an ExtractImageFilter to avoid problems with filters asking for the LargestPossibleRegion
  typedef itk::ExtractImageFilter<InputImageType, InputImageType> ExtractImageFilterType;
  typename ExtractImageFilterType::Pointer extract = ExtractImageFilterType::New();
  extract->SetInput( this->GetInput() );
  
  SizeType paddedRegionSize = this->GetInput()->GetRequestedRegion().GetSize();
  
  // Expend input region to avoid touching but not overlapping polygons during fusion
  if (m_Enlarge)
  {
    paddedRegionSize[0] += 1;
    paddedRegionSize[1] += 1;
  }
  RegionType paddedRegion(  this->GetInput()->GetRequestedRegion().GetIndex() , paddedRegionSize );
  
  // Crop Enlarged Region to avoid asking for pixels outside the input image
  if (m_Enlarge)
  {
    paddedRegion.Crop(this->GetInput()->GetLargestPossibleRegion());
  }
  extract->SetExtractionRegion( paddedRegion );
  extract->Update();

  // WARNING: itk::ExtractImageFilter does not copy the MetadataDictionary
  extract->GetOutput()->SetMetaDataDictionary(this->GetInput()->GetMetaDataDictionary());

  typename LabelImageToOGRDataSourceFilterType::Pointer labelImageToOGRDataFilter =
                                              LabelImageToOGRDataSourceFilterType::New();
  labelImageToOGRDataFilter->SetInput(extract->GetOutput());
  //labelImageToOGRDataFilter->SetInput(this->GetInput());
  
  labelImageToOGRDataFilter->UpdateLargestPossibleRegion();
 
  labelImageToOGRDataFilter->SetFieldName(m_FieldName);
  labelImageToOGRDataFilter->SetUse8Connected(m_Use8Connected);
  
  labelImageToOGRDataFilter->Update();
  OGRDataSourcePointerType tmpDS = const_cast<OGRDataSourceType *>(labelImageToOGRDataFilter->GetOutput());
  OGRLayerType tmpLayer = tmpDS->GetLayerChecked(0);
  
  OGRDataSourcePointerType outDS = OGRDataSourceType::New();
  OGRLayerType outLayer = outDS->CreateLayer("Layer");
  OGRFieldDefn field(m_FieldName.c_str(),OFTInteger);
  outLayer.CreateField(field, true);
  OGRFieldDefn fieldSize("size",OFTReal);
  outLayer.CreateField(fieldSize, true);
  OGRFieldDefn fieldPerimeter("perimeter",OFTReal);
  outLayer.CreateField(fieldPerimeter, true);
  
  // Write output features
  for(auto featIt = tmpLayer.begin(); featIt!=tmpLayer.end(); ++featIt)
  {
    OGRFeatureType outFeature(outLayer.GetLayerDefn());
    
    // Only geometries whose attributes belong to the selected label list parameter are created. If the list is empty, all geometries are created except if the label is 0 (background)
    if ( (std::find(m_Labels.begin(),m_Labels.end(),(*featIt)[m_FieldName].ogr::Field::template GetValue<int>()) != m_Labels.end() ) || ( (m_Labels.empty() ==true) &&  (*featIt)[m_FieldName].ogr::Field::template GetValue<int>() > 0 ))
    {
      //simplify
      const OGRGeometry * geom = (*featIt).GetGeometry();
      assert(geom && "geometry is NULL ! Can't simplify it.");
      (*featIt).ogr::Feature::SetGeometryDirectly(ogr::Simplify( *geom->Buffer(0) ,m_Tolerance));
     
      outFeature.SetFrom( *featIt, TRUE );
      outLayer.CreateFeature( outFeature );
    }
    else
    {
    }
  }
  return outDS;
}

} // End namespace otb

#endif

