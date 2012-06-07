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
#ifndef __otbPersistentImageToOGRDataFilter_txx
#define __otbPersistentImageToOGRDataFilter_txx

#include "otbPersistentImageToOGRDataFilter.h"
#include "itkTimeProbe.h"
#include <boost/foreach.hpp>
#include <stdio.h>
#include "otbMacro.h"
#include "otbOGRHelpers.h"

namespace otb
{

template<class TImage>
PersistentImageToOGRDataFilter<TImage>
::PersistentImageToOGRDataFilter() : m_FieldName("DN"), m_LayerName("Layer"), m_GeometryType(wkbMultiPolygon)
{
   this->SetNumberOfInputs(2);
   this->SetNumberOfRequiredInputs(2);
   m_StreamSize.Fill(0);
}

template<class TImage>
PersistentImageToOGRDataFilter<TImage>
::~PersistentImageToOGRDataFilter()
{
}

template<class TImage>
void
PersistentImageToOGRDataFilter<TImage>
::SetOGRDataSource( OGRDataSourcePointerType ogrDS )
{
   this->itk::ProcessObject::SetNthInput(1, ogrDS);
}

template<class TImage>
typename PersistentImageToOGRDataFilter<TImage>::OGRDataSourceType *
PersistentImageToOGRDataFilter<TImage>
::GetOGRDataSource( void )
{
   return static_cast<OGRDataSourceType *> (this->itk::ProcessObject::GetInput(1));
}

template<class TImage>
void
PersistentImageToOGRDataFilter<TImage>
::AddOGRLayerCreationOption(const std::string& option)
{
  m_OGRLayerCreationOptions.push_back(option);
  this->Modified();
}

template<class TImage>
void
PersistentImageToOGRDataFilter<TImage>
::ClearOGRLayerCreationOptions()
{
  m_OGRLayerCreationOptions.clear();
  this->Modified();
}

template<class TImage>
void
PersistentImageToOGRDataFilter<TImage>
::SetOGRLayerCreationOptions(const std::vector<std::string>&  options)
{
  m_OGRLayerCreationOptions = options;
  this->Modified();
}


template<class TImage>
void
PersistentImageToOGRDataFilter<TImage>
::AllocateOutputs()
{
  // Nothing that needs to be allocated for the outputs : the output is not meant to be used
}

template<class TImage>
void
PersistentImageToOGRDataFilter<TImage>
::Reset()
{
}

template<class TImage>
void
PersistentImageToOGRDataFilter<TImage>
::Synthetize()
{
}

template<class TImage>
void
PersistentImageToOGRDataFilter<TImage>
::Initialize()
{
   
   std::string projectionRefWkt = this->GetInput()->GetProjectionRef();
   bool projectionInformationAvailable = !projectionRefWkt.empty();
   OGRSpatialReference * oSRS = NULL;
   if(projectionInformationAvailable)
   {
      oSRS = static_cast<OGRSpatialReference *>(OSRNewSpatialReference(projectionRefWkt.c_str()));
   }
   
   
   OGRDataSourcePointerType ogrDS = this->GetOGRDataSource();

   ogrDS->CreateLayer(m_LayerName, oSRS ,m_GeometryType, otb::ogr::StringListConverter(m_OGRLayerCreationOptions).to_ogr());
   OGRFieldDefn field(m_FieldName.c_str(),OFTInteger);
   
   //Handle the case of shapefile. A shapefile is a layer and not a datasource.
   //The layer name in a shapefile is the shapefile's name.
   //This is not the case for a database as sqlite or PG.
   if (ogrDS->GetLayersCount() == 1)
   {
      ogrDS->GetLayer(0).CreateField(field, true);
   }
   else
   {
      ogrDS->GetLayer(m_LayerName).CreateField(field, true);
   }

   //CSLDestroy( options );

}


template<class TImage>
void
PersistentImageToOGRDataFilter<TImage>
::GenerateData()
{
  
  if (this->GetStreamSize()[0]==0 && this->GetStreamSize()[1]==0)
  {
     this->m_StreamSize = this->GetInput()->GetRequestedRegion().GetSize();
  }
  
  // call the processing function for this tile
  OGRDataSourcePointerType currentTileVD = this->ProcessTile();
  OGRLayerType srcLayer = currentTileVD->GetLayerChecked(0);

  OGRDataSourcePointerType ogrDS = this->GetOGRDataSource();
  OGRLayerType dstLayer = ogrDS->GetLayersCount() == 1
                          ? ogrDS->GetLayer(0)
                          : ogrDS->GetLayer(m_LayerName);
  
  
  //Copy features contained in the memory layer (srcLayer) in the output layer
  itk::TimeProbe chrono;
  chrono.Start();
  dstLayer.ogr().StartTransaction();
  OGRLayerType::const_iterator featIt = srcLayer.begin();
  for(; featIt!=srcLayer.end(); ++featIt)
  {
      OGRFeatureType dstFeature(dstLayer.GetLayerDefn());
      dstFeature.SetFrom( *featIt, TRUE );
      dstLayer.CreateFeature( dstFeature );
  }
  
  dstLayer.ogr().CommitTransaction();
  chrono.Stop();
  otbMsgDebugMacro(<< "write ogr tile took " << chrono.GetTotal() << " sec");
  
}

template<class TImage>
void
PersistentImageToOGRDataFilter<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb
#endif
