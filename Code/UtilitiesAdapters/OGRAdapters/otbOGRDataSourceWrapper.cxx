/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbOGRDataSourceWrapper.h"
// standard includes
#include <cassert>
#include <boost/bind.hpp>
// ITK includes
#include "itkMacro.h" // itkExceptionMacro
#include "itkMetaDataObject.h"
#include "itkExceptionObject.h"
// OTB includes
#include "otbMacro.h"
#include "otbMetaDataKey.h"
#include "otbOGRDriversInit.h"
// OGR includes
#include "ogrsf_frmts.h"

void
otb::ogr::DataSource
::SetProjectionRef(const std::string& projectionRef)
{
  itk::MetaDataDictionary& dict = this->GetMetaDataDictionary();
  itk::EncapsulateMetaData<std::string>(
    dict, MetaDataKey::ProjectionRefKey, projectionRef);
  this->Modified();
}

std::string
otb::ogr::DataSource
::GetProjectionRef() const
{
  const itk::MetaDataDictionary& dict = this->GetMetaDataDictionary();

  std::string projectionRef;
  itk::ExposeMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, projectionRef);

  return projectionRef;
}

/*===========================================================================*/
/*=======================[ construction/destruction ]========================*/
/*===========================================================================*/
bool otb::ogr::DataSource::Clear()
{
  Reset(0);
  return true;
}

void otb::ogr::DataSource::Reset(OGRDataSource * source)
{
  if (m_DataSource) {
    // OGR makes a pointless check for non-nullity in
    // OGRDataSource::DestroyDataSource (pointless because "delete 0" is
    // perfectly valid -> it's a no-op)
    OGRDataSource::DestroyDataSource(m_DataSource); // void, noexcept
  }
  m_DataSource = source;
}

otb::ogr::DataSource::DataSource()
: m_DataSource(0)
{
  Drivers::Init();

  OGRSFDriver * d = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName("Memory");
  assert(d && "OGR Memory driver not found");
  m_DataSource = d->CreateDataSource("in-memory");
  if (!m_DataSource) {
    itkExceptionMacro(<< "Failed to create OGRMemDataSource");
  }
  m_DataSource->SetDriver(d);
}

otb::ogr::DataSource::DataSource(OGRDataSource * source)
: m_DataSource(source)
{
}

otb::ogr::DataSource::Pointer
otb::ogr::DataSource::New(std::string const& filename, Modes::type mode)
{
  Drivers::Init();

  const bool update = mode & Modes::write;
  OGRDataSource * source = OGRSFDriverRegistrar::Open(filename.c_str(), update);
  if (!source) {
    itkGenericExceptionMacro(<< "Failed to open OGRDataSource file " << filename);
  }
  Pointer res = new DataSource(source);
  return res;
}

/*static*/
otb::ogr::DataSource::Pointer
otb::ogr::DataSource::New(OGRDataSource * source)
{
  Pointer res = new DataSource(source);
  return res;
}

/*virtual*/  otb::ogr::DataSource::~DataSource()
{
  Clear();
}

/*===========================================================================*/
/*================================[ layers ]=================================*/
/*===========================================================================*/
OGRLayer& otb::ogr::DataSource::GetLayerChecked(size_t i)
{
  const int nb_layers = GetLayersCount();
  if (int(i) >= nb_layers)
    {
    itkExceptionMacro(<< "Cannot fetch " << i << "th layer in the OGRDataSource as it contains only "
      << nb_layers << "layers.");
    }
  OGRLayer * layer_ptr = m_DataSource->GetLayer(int(i));
  if (!layer_ptr)
    {
    itkExceptionMacro( << "Unexpected error: cannot fetch " << i << "th layer in the OGRDataSource.");
    }
    return *layer_ptr;
}

OGRLayer const& otb::ogr::DataSource::GetLayerChecked(size_t i) const
{
  return const_cast <DataSource*>(this)->GetLayerChecked(i);
}

OGRLayer* otb::ogr::DataSource::GetLayerUnchecked(size_t i)
{
  OGRLayer * layer_ptr = m_DataSource->GetLayer(int(i));
  return layer_ptr;
}

int otb::ogr::DataSource::GetLayersCount() const
{
  assert(m_DataSource);
  return m_DataSource->GetLayerCount();
}

/*===========================================================================*/
/*===============================[ features ]================================*/
/*===========================================================================*/
namespace  { // Anonymous namespace
  struct AccuLayersSizes
    {
    AccuLayersSizes(bool doForceComputation)
      : m_doForceComputation(doForceComputation) { }
    int operator()(OGRLayer& layer, int accumulated) const
      {
      const int loc_size = layer.GetFeatureCount(m_doForceComputation);
      return loc_size < 0 ? loc_size : loc_size+accumulated;
      }
  private:
    bool m_doForceComputation;
    };
} // Anonymous namespace

int otb::ogr::DataSource::Size(bool doForceComputation) const
{
  return AccumulateOnLayers(AccuLayersSizes(doForceComputation), 0);
}

/*===========================================================================*/
/*=================================[ Misc ]==================================*/
/*===========================================================================*/

/*virtual*/
void otb::ogr::DataSource::PrintSelf(
  std::ostream& os, itk::Indent indent) const
{
  // ForEachLayer(boost::bind(&OGRLayer::PrintSelf, _1, indent++));
}

/*virtual*/ void otb::ogr::DataSource::Graft(const itk::DataObject * data)
{
  assert(! "Disabled to check if it makes sense...");
}
