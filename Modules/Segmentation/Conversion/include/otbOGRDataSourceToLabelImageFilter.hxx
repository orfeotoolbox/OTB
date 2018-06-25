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

#ifndef otbOGRDataSourceToLabelImageFilter_hxx
#define otbOGRDataSourceToLabelImageFilter_hxx

#include "otbOGRDataSourceToLabelImageFilter.h"
#include "otbOGRIOHelper.h"
#include "otbGdalDataTypeBridge.h"
#include "otbImageMetadataInterfaceFactory.h"
#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"
#include "otbImage.h"

#include "gdal_alg.h"
#include "stdint.h" //needed for uintptr_t

namespace otb
{
template< class TOutputImage>
OGRDataSourceToLabelImageFilter<TOutputImage>
::OGRDataSourceToLabelImageFilter() : m_BurnAttribute("DN"),
                                      m_BackgroundValue(0),
                                      m_ForegroundValue(255),
                                      m_BurnAttributeMode(true)
{
  this->SetNumberOfRequiredInputs(1);

  // Output parameters initialization
  m_OutputSpacing.Fill(1.0);
  m_OutputSize.Fill(0);
  m_OutputStartIndex.Fill(0);
  m_BandsToBurn.clear();
  m_BandsToBurn.push_back(1);

}

template< class TOutputImage>
void
OGRDataSourceToLabelImageFilter<TOutputImage>
::AddOGRDataSource(const OGRDataSourceType* ds)
{
  this->itk::ProcessObject::PushBackInput( ds );
}

template < class TOutputImage>
const typename OGRDataSourceToLabelImageFilter<TOutputImage>::OGRDataSourceType *
OGRDataSourceToLabelImageFilter<TOutputImage>
::GetInput(unsigned int idx)
{
  return static_cast<const OGRDataSourceType *>
           (this->itk::ProcessObject::GetInput(idx));
}

template < class TOutputImage>
void
OGRDataSourceToLabelImageFilter<TOutputImage>
::SetOutputSpacing(const OutputSpacingType& spacing)
{
  if (this->m_OutputSpacing != spacing)
    {
    this->m_OutputSpacing = spacing;
    this->Modified();
    }
}

template < class TOutputImage>
void
OGRDataSourceToLabelImageFilter<TOutputImage>
::SetOutputSpacing(const double spacing[2])
{
  OutputSpacingType s(spacing);
  this->SetOutputSpacing(s);
}

template < class TOutputImage>
void
OGRDataSourceToLabelImageFilter<TOutputImage>
::SetOutputSpacing(const float spacing[2])
{
  itk::Vector<float, 2> sf(spacing);
  OutputSpacingType s;
  s.CastFrom(sf);
  this->SetOutputSpacing(s);
}

template < class TOutputImage>
void
OGRDataSourceToLabelImageFilter<TOutputImage>
::SetOutputOrigin(const double origin[2])
{
  OutputOriginType p(origin);
  this->SetOutputOrigin(p);
}

template < class TOutputImage>
void
OGRDataSourceToLabelImageFilter<TOutputImage>
::SetOutputOrigin(const float origin[2])
{
  itk::Point<float, 2> of(origin);
  OutputOriginType p;
  p.CastFrom(of);
  this->SetOutputOrigin(p);
}

template < class TOutputImage>
void
OGRDataSourceToLabelImageFilter<TOutputImage>
::SetOutputParametersFromImage(const ImageBaseType * image)
{
    this->SetOutputOrigin ( image->GetOrigin() );
    this->SetOutputSpacing ( internal::GetSignedSpacing( image ) );
    this->SetOutputSize ( image->GetLargestPossibleRegion().GetSize() );
    
    ImageMetadataInterfaceBase::Pointer imi = ImageMetadataInterfaceFactory::CreateIMI(image->GetMetaDataDictionary());

    this->SetOutputProjectionRef(imi->GetProjectionRef());
}

template< class TOutputImage>
void
OGRDataSourceToLabelImageFilter<TOutputImage>
::GenerateOutputInformation()
{
  // get pointer to the output
  OutputImagePointer outputPtr = this->GetOutput();
  if (!outputPtr)
    {
    return;
    }

  // Set the size of the output region
  typename TOutputImage::RegionType outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize(m_OutputSize);
  //outputLargestPossibleRegion.SetIndex(m_OutputStartIndex);
  outputPtr->SetLargestPossibleRegion(outputLargestPossibleRegion);

  // Set spacing and origin
  outputPtr->SetSignedSpacing(m_OutputSpacing);
  outputPtr->SetOrigin(m_OutputOrigin);

  itk::MetaDataDictionary& dict = outputPtr->GetMetaDataDictionary();
  itk::EncapsulateMetaData<std::string> (dict, MetaDataKey::ProjectionRefKey,
                                         static_cast<std::string>(this->GetOutputProjectionRef()));

  // Generate the OGRLayers from the input OGRDataSource
  for (unsigned int idx = 0; idx < this->GetNumberOfInputs(); ++idx)
    {
    OGRDataSourcePointerType ogrDS = dynamic_cast<OGRDataSourceType*>(this->itk::ProcessObject::GetInput(idx));
    const unsigned int nbLayers = ogrDS->GetLayersCount();

    for (unsigned int layer = 0; layer < nbLayers; ++layer)
      {
         m_SrcDataSetLayers.push_back( &(ogrDS->GetLayer(layer).ogr()) );
      }
    }

  // Set the NoData value using the background
  const unsigned int & nbBands =  outputPtr->GetNumberOfComponentsPerPixel();
  std::vector<bool> noDataValueAvailable;
  noDataValueAvailable.resize(nbBands,true);
  std::vector<double> noDataValue;
  noDataValue.resize(nbBands,static_cast<double>(m_BackgroundValue));
  itk::EncapsulateMetaData<std::vector<bool> >(dict,MetaDataKey::NoDataValueAvailable,noDataValueAvailable);
  itk::EncapsulateMetaData<std::vector<double> >(dict,MetaDataKey::NoDataValue,noDataValue);
}

template< class TOutputImage>
void
OGRDataSourceToLabelImageFilter<TOutputImage>::GenerateData()
{
  // Call Superclass GenerateData
  this->AllocateOutputs();

  // Get the buffered region
  OutputImageRegionType bufferedRegion = this->GetOutput()->GetBufferedRegion();

  // nb bands
  const unsigned int & nbBands =  this->GetOutput()->GetNumberOfComponentsPerPixel();

  // register drivers
  GDALAllRegister();

  std::ostringstream stream;
  stream << "MEM:::"
         <<  "DATAPOINTER=" << (uintptr_t)(this->GetOutput()->GetBufferPointer()) << ","
         <<  "PIXELS=" << bufferedRegion.GetSize()[0] << ","
         <<  "LINES=" << bufferedRegion.GetSize()[1]<< ","
         <<  "BANDS=" << nbBands << ","
         <<  "DATATYPE=" << GDALGetDataTypeName(GdalDataTypeBridge::GetGDALDataType<OutputImageInternalPixelType>()) << ","
         <<  "PIXELOFFSET=" << sizeof(OutputImageInternalPixelType) *  nbBands << ","
         <<  "LINEOFFSET=" << sizeof(OutputImageInternalPixelType)*nbBands*bufferedRegion.GetSize()[0] << ","
         <<  "BANDOFFSET=" << sizeof(OutputImageInternalPixelType);

  GDALDatasetH dataset = GDALOpen(stream.str().c_str(), GA_Update);

  // Add the projection ref to the dataset
  GDALSetProjection (dataset, this->GetOutput()->GetProjectionRef().c_str());

  // Set the nodata value
  for(unsigned int band = 0; band < nbBands; ++band)
    {
     GDALRasterBandH hBand = GDALGetRasterBand(dataset, band + 1);
     GDALFillRaster(hBand, m_BackgroundValue, 0);
    }

  // add the geoTransform to the dataset
  itk::VariableLengthVector<double> geoTransform(6);

  // Reporting origin and spacing of the buffered region
  // the spacing is unchanged, the origin is relative to the buffered region
  OutputIndexType  bufferIndexOrigin = bufferedRegion.GetIndex();
  OutputOriginType bufferOrigin;
  this->GetOutput()->TransformIndexToPhysicalPoint(bufferIndexOrigin, bufferOrigin);
  geoTransform[0] = bufferOrigin[0] - 0.5 * this->GetOutput()->GetSignedSpacing()[0];
  geoTransform[3] = bufferOrigin[1] - 0.5 * this->GetOutput()->GetSignedSpacing()[1];
  geoTransform[1] = this->GetOutput()->GetSignedSpacing()[0];
  geoTransform[5] = this->GetOutput()->GetSignedSpacing()[1];

  // FIXME: Here component 1 and 4 should be replaced by the orientation parameters
  geoTransform[2] = 0.;
  geoTransform[4] = 0.;
  GDALSetGeoTransform(dataset,const_cast<double*>(geoTransform.GetDataPointer()));

  // Burn the geometries into the dataset
   if (dataset != nullptr)
     {
     std::vector<std::string> options;

     std::vector<double> foreground(nbBands*m_SrcDataSetLayers.size(),m_ForegroundValue);

     if(m_BurnAttributeMode)
       {
       options.push_back("ATTRIBUTE="+m_BurnAttribute);
       }

     GDALRasterizeLayers( dataset, nbBands,
                          &m_BandsToBurn[0],
                          m_SrcDataSetLayers.size(),
                          &(m_SrcDataSetLayers[0]),
                          nullptr, nullptr, &foreground[0],
                          ogr::StringListConverter(options).to_ogr(),
                          nullptr, nullptr );
     // release the dataset
     GDALClose( dataset );
     }
}

template< class TOutputImage>
void
OGRDataSourceToLabelImageFilter<TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
