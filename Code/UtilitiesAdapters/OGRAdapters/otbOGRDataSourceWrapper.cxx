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
otb::ogr::DataSource::const_iterator otb::ogr::DataSource::cbegin() const
{
  // return const_iterator(*this, 0);
  assert(!"not-ready");
  return const_iterator();
}

otb::ogr::DataSource::const_iterator otb::ogr::DataSource::cend() const
{
  // return const_iterator(*this, GetLayersCount());
  assert(!"not-ready");
  return const_iterator();
}

otb::ogr::DataSource::iterator otb::ogr::DataSource::begin()
{
  return iterator(*this, 0);
}

otb::ogr::DataSource::iterator otb::ogr::DataSource::end()
{
  return iterator(*this, GetLayersCount());
}

otb::ogr::Layer otb::ogr::DataSource::CreateLayer(
  std::string const& name,
  OGRSpatialReference * poSpatialRef/* = NULL */,
  OGRwkbGeometryType eGType/* = wkbUnknown */,
  char ** papszOptions/* = NULL */)
{
  assert(m_DataSource && "Datasource not initialized");
  OGRLayer * ol = m_DataSource->CreateLayer(
    name.c_str(), poSpatialRef, eGType, papszOptions);
  if (!ol)
    {
    itkGenericExceptionMacro(<< "Failed to create the layer <"<<name
      << "> in the OGRDataSource file " << m_DataSource->GetName());
    }
  Layer l(ol);
  return l;
}

otb::ogr::Layer otb::ogr::DataSource::CopyLayer(
  Layer & srcLayer,
  std::string const& newName,
  char ** papszOptions/* = NULL */)
{
  assert(m_DataSource && "Datasource not initialized");
  OGRLayer * l0 = &srcLayer.ogr();
  OGRLayer * ol = m_DataSource->CopyLayer(l0, newName.c_str(), papszOptions);
  if (!ol)
    {
    itkGenericExceptionMacro(<< "Failed to copy the layer <"
      << srcLayer.GetName() << "> into the new layer <" <<newName
      << "> in the OGRDataSource file " << m_DataSource->GetName());
    }
  Layer l(ol);
  return l;
}

void otb::ogr::DataSource::DeleteLayer(size_t i)
{
  const int nb_layers = GetLayersCount();
  if (int(i) >= nb_layers)
    {
    itkExceptionMacro(<< "Cannot delete " << i << "th layer in the OGRDataSource <"
      << m_DataSource->GetName() << "> as it contains only " << nb_layers << "layers.");
    }
  const OGRErr err = m_DataSource->DeleteLayer(int(i));
  if (err != OGRERR_NONE)
    {
    itkExceptionMacro(<< "Cannot delete " << i << "th layer in the OGRDataSource <"
      << m_DataSource->GetName() << ">.");
    }
}


otb::ogr::Layer otb::ogr::DataSource::GetLayerChecked(size_t i)
{
  const int nb_layers = GetLayersCount();
  if (int(i) >= nb_layers)
    {
    itkExceptionMacro(<< "Cannot fetch " << i << "th layer in the OGRDataSource <"
      << m_DataSource->GetName() << "> as it contains only " << nb_layers << "layers.");
    }
  OGRLayer * layer_ptr = m_DataSource->GetLayer(int(i));
  if (!layer_ptr)
    {
    itkExceptionMacro( << "Unexpected error: cannot fetch " << i << "th layer in the OGRDataSource <"
      << m_DataSource->GetName() << ">.");
    }
    return otb::ogr::Layer(layer_ptr);
}

OGRLayer* otb::ogr::DataSource::GetLayerUnchecked(size_t i)
{
  assert(m_DataSource && "Datasource not initialized");
  OGRLayer * layer_ptr = m_DataSource->GetLayer(int(i));
  return layer_ptr;
}

otb::ogr::Layer otb::ogr::DataSource::GetLayer(std::string const& name)
{
  assert(m_DataSource && "Datasource not initialized");
  OGRLayer * layer_ptr = m_DataSource->GetLayerByName(name.c_str());
  return otb::ogr::Layer(layer_ptr);
}


otb::ogr::Layer otb::ogr::DataSource::GetLayerChecked(std::string const& name)
{
  assert(m_DataSource && "Datasource not initialized");
  OGRLayer * layer_ptr = m_DataSource->GetLayerByName(name.c_str());
  if (!layer_ptr)
    {
    itkExceptionMacro( << "Cannot fetch any layer named <" << name
      << "> in the OGRDataSource <" << m_DataSource->GetName() << ">.");
    }
  return otb::ogr::Layer(layer_ptr);
}

int otb::ogr::DataSource::GetLayersCount() const
{
  assert(m_DataSource && "Datasource not initialized");
  return m_DataSource->GetLayerCount();
}

otb::ogr::Layer otb::ogr::DataSource::ExecuteSQL(
  std::string const& statement,
  OGRGeometry * poSpatialFilter,
  char        const* pszDialect)
{
  assert(m_DataSource && "Datasource not initialized");
  OGRLayer * layer_ptr = m_DataSource->ExecuteSQL(
    statement.c_str(), poSpatialFilter, pszDialect);
  if (!layer_ptr)
    {
#if defined(PREFER_EXCEPTION)
    itkExceptionMacro( << "Unexpected error: cannot execute the SQL request <" << statement
      << "> in the OGRDataSource <" << m_DataSource->GetName() << ">.");
#else
    // Cannot use the deleter made for result sets obtained from
    // OGRDataSource::ExecuteSQL because it checks for non-nullity....
    // *sigh*
    return otb::ogr::Layer(0);
#endif
    }
  return otb::ogr::Layer(layer_ptr, m_DataSource);

}


/*===========================================================================*/
/*===============================[ features ]================================*/
/*===========================================================================*/
namespace  { // Anonymous namespace
  struct AccuLayersSizes
    {
    AccuLayersSizes(bool doForceComputation)
      : m_doForceComputation(doForceComputation) { }
    int operator()(otb::ogr::Layer const& layer, int accumulated) const
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
  assert(m_DataSource && "Datasource not initialized");
  ForEachLayer(boost::bind(&Layer::PrintSelf, _1, boost::ref(os), indent.GetNextIndent()));
}

/*virtual*/ void otb::ogr::DataSource::Graft(const itk::DataObject * data)
{
  assert(! "Disabled to check if it makes sense...");
}

bool otb::ogr::DataSource::HasCapability(std::string const& capabilityName)
{
  assert(m_DataSource && "Datasource not initialized");
  return m_DataSource->TestCapability(capabilityName.c_str());
}

void otb::ogr::DataSource::SyncToDisk()
{
  assert(m_DataSource && "Datasource not initialized");
  const OGRErr res= m_DataSource->SyncToDisk();
  if (res != OGRERR_NONE)
    {
    itkExceptionMacro( << "Cannot flush the pending of the OGRDataSource <"
      << m_DataSource->GetName() << ">.");
    }
}
