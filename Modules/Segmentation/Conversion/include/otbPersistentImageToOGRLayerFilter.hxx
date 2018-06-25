/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbPersistentImageToOGRLayerFilter_hxx
#define otbPersistentImageToOGRLayerFilter_hxx

#include "otbPersistentImageToOGRLayerFilter.h"
#include "otbStopwatch.h"
#include <boost/foreach.hpp>
#include <stdio.h>
#include "otbMacro.h"
#include "otbOGRHelpers.h"

namespace otb
{

template<class TImage>
PersistentImageToOGRLayerFilter<TImage>
::PersistentImageToOGRLayerFilter() : m_OGRLayer(nullptr, false)
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
  // Make sure input projection ref is set
  const_cast<InputImageType*> (this->GetInput())->UpdateOutputInformation();

  // Ensure that spatial reference of the output layer matches with
  // the spatial reference of the input image
  OGRSpatialReference oSRS(this->GetInput()->GetProjectionRef().c_str());

  // when dealing with .shp OGRSPatialreference is morphed to ESRI WKT,
  // which results in small difference in WKT with some projection reference
  // so the comparison must be done with WKT and ESRI WKT Mantis #ref567
  OGRSpatialReference oSRSESRI(this->GetInput()->GetProjectionRef().c_str());

  oSRSESRI.morphToESRI();
  oSRSESRI.morphFromESRI();
  if (m_OGRLayer.GetSpatialRef() && (!oSRS.IsSame(m_OGRLayer.GetSpatialRef())
      && !oSRSESRI.IsSame(m_OGRLayer.GetSpatialRef())))
    {
    if ((oSRS.Validate() != OGRERR_NONE) && (oSRSESRI.Validate() != OGRERR_NONE))
      {
      itkExceptionMacro(<<"Input projection ref is not valid");
      }
    itkExceptionMacro(<<"Spatial reference of input image and target layer do not match! "<< std::endl
                      << "Input image : "<< this->GetInput()->GetProjectionRef()<< std::endl
                      << "Target layer : "<<m_OGRLayer.GetProjectionRef());
    }
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


  // Check spatial reference matches
  if(srcLayer.GetSpatialRef() && m_OGRLayer.GetSpatialRef() && !srcLayer.GetSpatialRef()->IsSame(m_OGRLayer.GetSpatialRef()))
     {
     itkExceptionMacro(<<"Spatial reference of internal and target layers do not match!");
     }

  //Copy features contained in the memory layer (srcLayer) in the output layer
  otb::Stopwatch chrono = otb::Stopwatch::StartNew();

  OGRErr err = m_OGRLayer.ogr().StartTransaction();
  if (err != OGRERR_NONE)
    {
    itkExceptionMacro(<< "Unable to start transaction for OGR layer " << m_OGRLayer.ogr().GetName() << ".");
    }

  OGRLayerType::const_iterator featIt = srcLayer.begin();
  for(; featIt!=srcLayer.end(); ++featIt)
    {
    OGRFeatureType dstFeature(m_OGRLayer.GetLayerDefn());
    dstFeature.SetFrom( *featIt, TRUE );
    m_OGRLayer.CreateFeature( dstFeature );
    }

  err = m_OGRLayer.ogr().CommitTransaction();

  if (err != OGRERR_NONE)
    {
    itkExceptionMacro(<< "Unable to commit transaction for OGR layer " << m_OGRLayer.ogr().GetName() << ".");
    }

  chrono.Stop();
  otbMsgDebugMacro(<< "Writing OGR tile took " << chrono.GetElapsedMilliseconds() << " ms");

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
