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

#ifndef otbVectorDataToLabelImageFilter_hxx
#define otbVectorDataToLabelImageFilter_hxx

#include "gdal_alg.h"
#include "ogr_srs_api.h"

#include "otbVectorDataToLabelImageFilter.h"
#include "otbOGRIOHelper.h"
#include "otbGdalDataTypeBridge.h"
#include "otbImageMetadataInterfaceBase.h"
#include "otbImageMetadataInterfaceFactory.h"
#include "otbImage.h"

namespace otb
{
template<class TVectorData, class TOutputImage>
VectorDataToLabelImageFilter<TVectorData, TOutputImage>
::VectorDataToLabelImageFilter()
 : m_OGRDataSourcePointer(nullptr),
   m_BandsToBurn(1, 1),
   m_BurnAttribute("FID"),
   m_DefaultBurnValue(1.),
   m_BackgroundValue(0.)
{
  this->SetNumberOfRequiredInputs(1);

  // Output parameters initialization
  m_OutputSpacing.Fill(1.0);
  m_OutputSize.Fill(0);
  m_OutputStartIndex.Fill(0);
}

template<class TVectorData, class TOutputImage>
void
VectorDataToLabelImageFilter<TVectorData, TOutputImage>
::AddVectorData(const VectorDataType* vd)
{
  this->itk::ProcessObject::PushBackInput( vd );
}

template <class TVectorData, class TOutputImage>
const typename VectorDataToLabelImageFilter<TVectorData, TOutputImage>::VectorDataType *
VectorDataToLabelImageFilter<TVectorData, TOutputImage>
::GetInput(unsigned int idx)
{
  return static_cast<const TVectorData *>
           (this->itk::ProcessObject::GetInput(idx));
}

template <class TVectorData, class TOutputImage>
void
VectorDataToLabelImageFilter<TVectorData, TOutputImage>
::SetOutputSpacing(const OutputSpacingType& spacing)
{
  if (this->m_OutputSpacing != spacing)
    {
    this->m_OutputSpacing = spacing;
    this->Modified();
    }
}

template <class TVectorData, class TOutputImage>
void
VectorDataToLabelImageFilter<TVectorData, TOutputImage>
::SetOutputSpacing(const double spacing[2])
{
  OutputSpacingType s(spacing);
  this->SetOutputSpacing(s);
}

template <class TVectorData, class TOutputImage>
void
VectorDataToLabelImageFilter<TVectorData, TOutputImage>
::SetOutputSpacing(const float spacing[2])
{
  itk::Vector<float, 2> sf(spacing);
  OutputSpacingType s;
  s.CastFrom(sf);
  this->SetOutputSpacing(s);
}

template <class TVectorData, class TOutputImage>
void
VectorDataToLabelImageFilter<TVectorData, TOutputImage>
::SetOutputOrigin(const double origin[2])
{
  OutputOriginType p(origin);
  this->SetOutputOrigin(p);
}

template <class TVectorData, class TOutputImage>
void
VectorDataToLabelImageFilter<TVectorData, TOutputImage>
::SetOutputOrigin(const float origin[2])
{
  itk::Point<float, 2> of(origin);
  OutputOriginType p;
  p.CastFrom(of);
  this->SetOutputOrigin(p);
}

template <class TVectorData, class TOutputImage>
void
VectorDataToLabelImageFilter<TVectorData, TOutputImage>
::SetOutputParametersFromImage(const ImageBaseType * src)
{
  this->SetOutputOrigin ( src->GetOrigin() );
  this->SetOutputSpacing ( internal::GetSignedSpacing(src) );
  this->SetOutputSize ( src->GetLargestPossibleRegion().GetSize() );
  ImageMetadataInterfaceBase::Pointer imi = ImageMetadataInterfaceFactory::CreateIMI(src->GetMetaDataDictionary());
  this->SetOutputProjectionRef(imi->GetProjectionRef());
}

template<class TVectorData, class TOutputImage>
void
VectorDataToLabelImageFilter<TVectorData, TOutputImage>
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

  // Generate the OGRLayers from the input VectorDatas
  // iteration begin from 1 cause the 0th input is a image
  for (unsigned int idx = 0; idx < this->GetNumberOfInputs(); ++idx)
    {
    const VectorDataType* vd = dynamic_cast< const VectorDataType*>(this->itk::ProcessObject::GetInput(idx));

    // Get the projection ref of the current VectorData
    std::string projectionRefWkt = vd->GetProjectionRef();
    bool        projectionInformationAvailable = !projectionRefWkt.empty();
    OGRSpatialReference * oSRS = nullptr;

    if (projectionInformationAvailable)
      {
      oSRS = static_cast<OGRSpatialReference *>(OSRNewSpatialReference(projectionRefWkt.c_str()));
      }
    else
      {
      otbMsgDevMacro(<< "Projection information unavailable");
      }

    // Retrieving root node
    DataTreeConstPointerType tree = vd->GetDataTree();

    // Get the input tree root
    InternalTreeNodeType * inputRoot = const_cast<InternalTreeNodeType *>(tree->GetRoot());

    // Iterative method to build the layers from a VectorData
    OGRLayer *   ogrCurrentLayer = nullptr;
    std::vector<OGRLayer *> ogrLayerVector;
    otb::OGRIOHelper::Pointer IOConversion = otb::OGRIOHelper::New();

    // The method ConvertDataTreeNodeToOGRLayers create the
    // OGRDataSource but don t release it. Destruction is done in the
    // desctructor
    m_OGRDataSourcePointer = nullptr;
    ogrLayerVector = IOConversion->ConvertDataTreeNodeToOGRLayers(inputRoot,
                                                                  m_OGRDataSourcePointer,
                                                                  ogrCurrentLayer,
                                                                  oSRS);

    // From OGRLayer* to OGRGeometryH vector
    for (unsigned int idx2 = 0; idx2 < ogrLayerVector.size(); ++idx2)
      {
      // test if the layers contain a field m_BurnField;
      int burnField = -1;

      if( !m_BurnAttribute.empty() )
        {
        burnField = OGR_FD_GetFieldIndex( OGR_L_GetLayerDefn( (OGRLayerH)(ogrLayerVector[idx2]) ),
                                           m_BurnAttribute.c_str() );

      // Get the geometries of the layer
      OGRFeatureH hFeat;
      OGR_L_ResetReading( (OGRLayerH)(ogrLayerVector[idx2]) );
      while( ( hFeat = OGR_L_GetNextFeature( (OGRLayerH)(ogrLayerVector[idx2]) )) != nullptr )
        {
        OGRGeometryH hGeom;
        if( OGR_F_GetGeometryRef( hFeat ) == nullptr )
          {
          OGR_F_Destroy( hFeat );
          continue;
          }

        hGeom = OGR_G_Clone( OGR_F_GetGeometryRef( hFeat ) );
        m_SrcDataSetGeometries.push_back( hGeom );

        if (burnField == -1 )
          {
          // TODO : if no burnAttribute available, warning or raise an exception??
          m_FullBurnValues.push_back(m_DefaultBurnValue++);
          itkWarningMacro(<<"Failed to find attribute "<<m_BurnAttribute << " in layer "
                          << OGR_FD_GetName( OGR_L_GetLayerDefn( (OGRLayerH)(ogrLayerVector[idx2]) ))
                          <<" .Setting burn value to default =  "
                          << m_DefaultBurnValue);
          }
        else
          {
          m_FullBurnValues.push_back( OGR_F_GetFieldAsDouble( hFeat, burnField ) );
          }

        OGR_F_Destroy( hFeat );
        }
        }

    // Destroy the oSRS
    if (oSRS != nullptr)
      {
      OSRRelease(oSRS);
      }
      }
    }
}

template<class TVectorData, class TOutputImage>
void
VectorDataToLabelImageFilter<TVectorData, TOutputImage>::GenerateData()
{
  // Call Superclass GenerateData
  this->AllocateOutputs();

  // Get the buffered region
  OutputImageRegionType bufferedRegion = this->GetOutput()->GetBufferedRegion();

  // Fill the buffer with the background value
  this->GetOutput()->FillBuffer(m_BackgroundValue);
  
  // nb bands
  unsigned int nbBands =  this->GetOutput()->GetNumberOfComponentsPerPixel();

  // register drivers
  GDALAllRegister();

  std::ostringstream stream;
  stream << "MEM:::"
         <<  "DATAPOINTER=" << (unsigned long)(GUIntBig)(this->GetOutput()->GetBufferPointer()) << ","
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
     GDALRasterizeGeometries( dataset, m_BandsToBurn.size(),
                          &(m_BandsToBurn[0]),
                          m_SrcDataSetGeometries.size(),
                          &(m_SrcDataSetGeometries[0]),
                          nullptr, nullptr, &(m_FullBurnValues[0]),
                          nullptr,
                          GDALDummyProgress, nullptr );

     // release the dataset
     GDALClose( dataset );
     }
}

template<class TVectorData, class TOutputImage>
void
VectorDataToLabelImageFilter<TVectorData, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif

