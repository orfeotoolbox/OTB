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
#include <numeric>
#include <algorithm>
#include <clocale> // toupper
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
// ITK includes
#include "itkMacro.h" // itkExceptionMacro
#include "itkMetaDataObject.h"
#include "itkExceptionObject.h"
#include "itksys/SystemTools.hxx"
// OTB includes
#include "otbMacro.h"
#include "otbMetaDataKey.h"
#include "otbOGRDriversInit.h"
#include "otbSystem.h"
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

namespace  { // Anonymous namespace
/**\ingroup GeometryInternals
 * \brief Type for associating filename extension with OGR driver names.
 * \since OTB v 3.14.0
 */
struct ExtensionDriverAssociation
  {
  char const* extension;
  char const* driverName;
  bool Matches(std::string const& ext) const
    {
    return ext == extension;
    }
  };
/**\ingroup GeometryInternals
 * \brief Associative table of filename extension -> OGR driver names.
 * \since OTB v 3.14.0
 */
const ExtensionDriverAssociation k_ExtensionDriverMap[] =
  {
    {"SHP", "ESRI Shapefile"},
    {"TAB", "MapInfo File"},
    {"GML", "GML"},
    {"GPX", "GPX"},
    {"SQLITE", "SQLite"},
    {"KML", "KML"},
  };
/**\ingroup GeometryInternals
 * \brief Returns the OGR driver name associated to a filename.
 * \since OTB v 3.14.0
 * \note Relies on the driver name associated to the filename extension in \c
 * k_ExtensionDriverMap.
 * \note As a special case, filenames starting with "PG:" are bound to
 * "PostgreSQL".
 */
char const* DeduceDriverName(std::string filename)
  {
  std::transform(filename.begin(), filename.end(), filename.begin(), (int (*)(int))toupper);
  if (0 == strncmp(filename.c_str(), "PG:", 3))
    {
    return "PostgreSQL";
    }
  const std::string extension = otb::System::GetExtension(filename);
  ExtensionDriverAssociation const* whichIt =
    std::find_if(
      boost::begin(k_ExtensionDriverMap), boost::end(k_ExtensionDriverMap),
      boost::bind(&ExtensionDriverAssociation::Matches, _1, extension));
  if (whichIt ==  boost::end(k_ExtensionDriverMap))
    {
    return 0; // nothing found
    }
  return whichIt->driverName;
  }
} // Anonymous namespace


otb::ogr::DataSource::DataSource()
: m_DataSource(0)
{
  Drivers::Init();

  OGRSFDriver * d = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName("Memory");
  assert(d && "OGR Memory driver not found");
  m_DataSource = d->CreateDataSource("in-memory");
  if (!m_DataSource) {
    itkExceptionMacro(<< "Failed to create OGRMemDataSource: " << CPLGetLastErrorMsg());
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
  // std::cout << "Opening datasource " << filename << " update=" << update << "\n";
  if (itksys::SystemTools::FileExists(filename.c_str()))
    {
    OGRDataSource * source = OGRSFDriverRegistrar::Open(filename.c_str(), update);
    if (!source)
      {
      itkGenericExceptionMacro(<< "Failed to open OGRDataSource file "
        << filename<<": " << CPLGetLastErrorMsg());
      }
    Pointer res = new DataSource(source);
    res->UnRegister();
    return res;
    }
  else
    {
    if (! update)
      {
      itkGenericExceptionMacro(<< "No DataSource named <"<<filename<<"> exists,"
        " and the file opening mode does not permit updates. DataSource creation is thus aborted.");
      }
    // Hand made factory based on file extension.
    char const* driverName = DeduceDriverName(filename);
    if (!driverName)
      {
      itkGenericExceptionMacro(<< "No OGR driver known to OTB to create and handle a DataSource named <"
        <<filename<<">.");
      }

    OGRSFDriver * d = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(driverName);
    assert(d && "OGR driver not found");
    OGRDataSource * source = d->CreateDataSource(filename.c_str());
    if (!source) {
      itkGenericExceptionMacro(<< "Failed to create OGRDataSource <"<<filename
        <<"> (driver name: " << driverName<<">: " << CPLGetLastErrorMsg());
    }
    source->SetDriver(d);
    Pointer res = new DataSource(source);
    res->UnRegister();
    return res;
    }
}

/*static*/
otb::ogr::DataSource::Pointer
otb::ogr::DataSource::New(OGRDataSource * source)
{
  Pointer res = new DataSource(source);
  res->UnRegister();
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
  return const_iterator(*this, 0);
}

otb::ogr::DataSource::const_iterator otb::ogr::DataSource::cend() const
{
  return const_iterator(*this, GetLayersCount());
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
      << "> in the OGRDataSource file <" << m_DataSource->GetName()
      <<">: " << CPLGetLastErrorMsg());
    }
  Layer l(ol, this);
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
      << "> in the OGRDataSource file <" << m_DataSource->GetName()
      <<">: " << CPLGetLastErrorMsg());
    }
  Layer l(ol, this);
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
      << m_DataSource->GetName() << ">: " << CPLGetLastErrorMsg());
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
      << m_DataSource->GetName() << ">: " << CPLGetLastErrorMsg());
    }
    return otb::ogr::Layer(layer_ptr, this);
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
  return otb::ogr::Layer(layer_ptr, this);
}


otb::ogr::Layer otb::ogr::DataSource::GetLayerChecked(std::string const& name)
{
  assert(m_DataSource && "Datasource not initialized");
  OGRLayer * layer_ptr = m_DataSource->GetLayerByName(name.c_str());
  if (!layer_ptr)
    {
    itkExceptionMacro( << "Cannot fetch any layer named <" << name
      << "> in the OGRDataSource <" << m_DataSource->GetName() << ">: "
      << CPLGetLastErrorMsg());
    }
  return otb::ogr::Layer(layer_ptr, this);
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
      << "> in the OGRDataSource <" << m_DataSource->GetName() << ">: " << CPLGetLastErrorMsg());
#else
    // Cannot use the deleter made for result sets obtained from
    // OGRDataSource::ExecuteSQL because it checks for non-nullity....
    // *sigh*
    return otb::ogr::Layer(0, 0);
#endif
    }
  return otb::ogr::Layer(layer_ptr, *m_DataSource);
}


/*===========================================================================*/
/*===============================[ features ]================================*/
/*===========================================================================*/
namespace  { // Anonymous namespace
/**\ingroup GeometryInternals
 * \brief %Functor used to accumulate the sizes of the layers in a \c DataSource.
 * \since OTB v 3.14.0
 */
struct AccuLayersSizes
  {
  AccuLayersSizes(bool doForceComputation)
    : m_doForceComputation(doForceComputation) { }
  int operator()(int accumulated, otb::ogr::Layer const& layer) const
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
  return std::accumulate(begin(),end(), 0, AccuLayersSizes(doForceComputation));
}

/*===========================================================================*/
/*=================================[ Misc ]==================================*/
/*===========================================================================*/

std::string otb::ogr::DataSource::GetGlobalExtent(double & ulx,
                                           double & uly,
                                           double & lrx,
                                           double & lry,
                                           bool force) const
{
  OGREnvelope sExtent;

  const_iterator lit = this->begin();
  
  if(lit==this->end())
    {
    itkGenericExceptionMacro(<< "Cannot compute global extent because there are no layers in the DataSource");
    }

  std::string outwkt = lit->GetProjectionRef();

  const OGRSpatialReference * ref_srs = lit->GetSpatialRef();
  
  const OGRErr res = lit->ogr().GetExtent(&sExtent,force);


  if(res!= OGRERR_NONE)
    {
    itkGenericExceptionMacro(<< "Cannot retrieve extent of layer <"
                              <<lit->GetName()<<">: " << CPLGetLastErrorMsg());
    }
  
  ++lit;

  for(; lit!=this->end(); ++lit)
    {
    OGREnvelope cExtent;
    
     const OGRErr cres = lit->ogr().GetExtent(&cExtent,force);

     if(cres!= OGRERR_NONE)
       {
       itkGenericExceptionMacro(<< "Cannot retrieve extent of layer <"
                                <<lit->GetName()<<">: " << CPLGetLastErrorMsg());
       }
     
     const OGRSpatialReference * current_srs = lit->GetSpatialRef();

     // If both srs are valid and if they are different
     if(ref_srs && current_srs && current_srs->IsSame(ref_srs) == 0)
       {
       // Reproject cExtent in ref_srs
       // OGRCreateCoordinateTransformation is not const-correct
       OGRCoordinateTransformation * coordTransformation = OGRCreateCoordinateTransformation(
         const_cast<OGRSpatialReference *>(current_srs),
         const_cast<OGRSpatialReference *>(ref_srs));

       coordTransformation->Transform(1,&cExtent.MinX,&cExtent.MinY);
       coordTransformation->Transform(1,&cExtent.MaxX,&cExtent.MaxY);

       double real_minx = std::min(cExtent.MinX,cExtent.MaxX);
       double real_miny = std::min(cExtent.MinY,cExtent.MaxY);
       double real_maxx = std::max(cExtent.MinX,cExtent.MaxX);
       double real_maxy = std::max(cExtent.MinY,cExtent.MaxY);

       cExtent.MinX = real_minx;
       cExtent.MinY = real_miny;
       cExtent.MaxX = real_maxx;
       cExtent.MaxY = real_maxy;

       CPLFree(coordTransformation);
       }

     // If srs are invalid, we assume that extent are coherent

     // Merge with previous layer
     sExtent.Merge(cExtent);
    }

  ulx = sExtent.MinX;
  uly = sExtent.MinY;
  lrx = sExtent.MaxX;
  lry = sExtent.MaxY;

  return outwkt;
}


/*virtual*/
void otb::ogr::DataSource::PrintSelf(
  std::ostream& os, itk::Indent indent) const
{
  assert(m_DataSource && "Datasource not initialized");
  BOOST_FOREACH(Layer const& l, *this)
    {
    l.PrintSelf(os, indent);
    }
}

/*virtual*/ void otb::ogr::DataSource::Graft(const itk::DataObject * data)
{
  assert(! "Disabled to check if it makes sense...");
}

bool otb::ogr::DataSource::HasCapability(std::string const& capabilityName) const
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
      << m_DataSource->GetName() << ">: " << CPLGetLastErrorMsg());
    }
}
