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
::PersistentImageToOGRLayerFilter() : m_OGRLayer(NULL, false)
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
    itkExceptionMacro(<<"Spatial reference of input image and target layer do not match!");
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
