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
#ifndef __otbPersistentImageToOGRLayerFilter_txx
#define __otbPersistentImageToOGRLayerFilter_txx

#include "otbPersistentImageToOGRLayerFilter.h"
#include "itkTimeProbe.h"
#include <boost/foreach.hpp>
#include <stdio.h>
#include "otbMacro.h"
#include "otbOGRHelpers.h"

namespace otb
{

template<class TImage>
PersistentImageToOGRLayerFilter<TImage>
::PersistentImageToOGRLayerFilter() : m_OGRLayer(NULL)
{
   m_StreamSize.Fill(0);
}

template<class TImage>
PersistentImageToOGRLayerFilter<TImage>
::~PersistentImageToOGRLayerFilter()
{
}

template<class TImage>
void
PersistentImageToOGRLayerFilter<TImage>
::SetOGRLayer(const OGRLayerType & ogrLayer)
{
  m_OGRLayer = ogrLayer;
  this->Modified();
}

template<class TImage>
const typename PersistentImageToOGRLayerFilter<TImage>::OGRLayerType&
PersistentImageToOGRLayerFilter<TImage>
::GetOGRLayer( void ) const
{
  return m_OGRLayer;
}

template<class TImage>
void
PersistentImageToOGRLayerFilter<TImage>
::AllocateOutputs()
{
  // Nothing that needs to be allocated for the outputs : the output is not meant to be used
}

template<class TImage>
void
PersistentImageToOGRLayerFilter<TImage>
::Reset()
{
}

template<class TImage>
void
PersistentImageToOGRLayerFilter<TImage>
::Synthetize()
{
}

template<class TImage>
void
PersistentImageToOGRLayerFilter<TImage>
::Initialize()
{
   
   // std::string projectionRefWkt = this->GetInput()->GetProjectionRef();
   // bool projectionInformationAvailable = !projectionRefWkt.empty();
   // OGRSpatialReference * oSRS = NULL;
   // if(projectionInformationAvailable)
   // {
   //    oSRS = static_cast<OGRSpatialReference *>(OSRNewSpatialReference(projectionRefWkt.c_str()));
   // }
   
   
   // OGRLayerPointerType ogrDS = this->GetOGRLayer();

   // ogrDS->CreateLayer(m_LayerName, oSRS ,m_GeometryType, otb::ogr::StringListConverter(m_OGRLayerCreationOptions).to_ogr());
   // OGRFieldDefn field(m_FieldName.c_str(),OFTInteger);
   
   // //Handle the case of shapefile. A shapefile is a layer and not a datasource.
   // //The layer name in a shapefile is the shapefile's name.
   // //This is not the case for a database as sqlite or PG.
   // if (ogrDS->GetLayersCount() == 1)
   // {
   //    ogrDS->GetLayer(0).CreateField(field, true);
   // }
   // else
   // {
   //    ogrDS->GetLayer(m_LayerName).CreateField(field, true);
   // }

   // //CSLDestroy( options );

}


template<class TImage>
void
PersistentImageToOGRLayerFilter<TImage>
::GenerateData()
{
  if(!m_OGRLayer)
    {
    itkExceptionMacro(<<"Output OGRLayer is null.");
    }

  if (this->GetStreamSize()[0]==0 && this->GetStreamSize()[1]==0)
  {
     this->m_StreamSize = this->GetInput()->GetRequestedRegion().GetSize();
  }


  // call the processing function for this tile
  OGRDataSourcePointerType currentTileVD = this->ProcessTile();
  OGRLayerType srcLayer = currentTileVD->GetLayerChecked(0);  
  
  //Copy features contained in the memory layer (srcLayer) in the output layer
  itk::TimeProbe chrono;
  chrono.Start();
  m_OGRLayer.ogr().StartTransaction();
  OGRLayerType::const_iterator featIt = srcLayer.begin();
  for(; featIt!=srcLayer.end(); ++featIt)
  {
      OGRFeatureType dstFeature(m_OGRLayer.GetLayerDefn());
      dstFeature.SetFrom( *featIt, TRUE );
      m_OGRLayer.CreateFeature( dstFeature );
  }
  
  m_OGRLayer.ogr().CommitTransaction();
  chrono.Stop();
  otbMsgDebugMacro(<< "write ogr tile took " << chrono.GetTotal() << " sec");
  
}

template<class TImage>
void
PersistentImageToOGRLayerFilter<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb
#endif
