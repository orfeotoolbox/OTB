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
#include "ogrsf_frmts.h"

namespace otb
{




template<class TImage>
PersistentImageToOGRDataFilter<TImage>
::PersistentImageToOGRDataFilter() : m_FileName(""), m_TileNum(0)
{
   // OGR factory registration
   OGRRegisterAll();
}

template<class TImage>
PersistentImageToOGRDataFilter<TImage>
::~PersistentImageToOGRDataFilter()
{
   if (m_DataSource != NULL)
    {
      OGRDataSource::DestroyDataSource(m_DataSource);
    }
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
   if (m_DataSource != NULL)
    {
      OGRDataSource::DestroyDataSource(m_DataSource);
    }
   
   std::string projectionRefWkt = this->GetInput()->GetProjectionRef();
   bool projectionInformationAvailable = !projectionRefWkt.empty();
   OGRSpatialReference * oSRS = NULL;
   if (projectionInformationAvailable)
    {
    oSRS = static_cast<OGRSpatialReference *>(OSRNewSpatialReference(projectionRefWkt.c_str()));
    }
   OGRSFDriver * ogrDriver;
   //OGRSFDriverRegistrar::Open(this->m_FileName.c_str(), FALSE, &ogrDriver);
   ogrDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName("ESRI Shapefile");
   m_DataSource = ogrDriver->CreateDataSource(this->m_FileName.c_str(), NULL);
   m_DataSource->CreateLayer("titi", oSRS ,wkbMultiPolygon, NULL);
   
   OGRFieldDefn field("DN",OFTInteger);
   m_DataSource->GetLayer(0)->CreateField(&field, true);
   
   std::cout<<"bon ok "<<std::endl;
}

template<class TImage>
void
PersistentImageToOGRDataFilter<TImage>
::SetFileName(const std::string & filename)
{
  m_FileName = filename;
  this->Initialize();
}

template<class TImage>
void
PersistentImageToOGRDataFilter<TImage>
::GenerateData()
{
  // call the processing function for this tile
  OGRDataSourceObjectPointerType currentTileVD = this->ProcessTile();
  OGRLayer * poSrcLayer = currentTileVD->Get()->GetDataSource()->GetLayer(0);
  
  std::ostringstream stream;
  stream << m_TileNum;
  
  //m_DataSource->CopyLayer(tileLayer,stream.str().c_str(),NULL);
  //create the layer corresponding to the tile processed !
  OGRLayer * poDstLayer;
  poDstLayer = m_DataSource->GetLayer(0);
  

  //poDstLayer = m_DataSource->CreateLayer(stream.str().c_str(),poSrcLayer->GetSpatialRef() ,poSrcLayer->GetGeomType(), NULL);
  //Copy features
  poSrcLayer->ResetReading();
  OGRFieldDefn * layerDefn = poSrcLayer->GetLayerDefn()->GetFieldDefn(0);
  /*std::cout<<"ouaip "<< layerDefn->GetNameRef() <<std::endl;
  poDstLayer->CreateField(layerDefn, true);
  std::cout<<"create field ok"<<std::endl;*/
  unsigned int nbFeatures = poSrcLayer->GetFeatureCount(true);
  //std::cout<<"nb Features : "<<nbFeatures <<std::endl;
  unsigned int i = 0;
  OGRFeature  *poFeature;
  while (i<nbFeatures)
  {
      OGRFeature  *poDstFeature = NULL;
      poFeature = poSrcLayer->GetNextFeature();
      
      if( poFeature == NULL )
            break;
      
      poDstFeature = OGRFeature::CreateFeature( poDstLayer->GetLayerDefn() );
      poDstFeature->SetFrom( poFeature, TRUE );
      
      poDstLayer->CreateFeature( poDstFeature );
      
      OGRFeature::DestroyFeature( poDstFeature );
      OGRFeature::DestroyFeature( poFeature );
      
      i++;
  }
  
  
//   OGRDataSource::DestroyDataSource(currentTileVD->Get()->GetDataSource());
  
  
  m_TileNum++;
  
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
