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
#include "itkExceptionObject.h"
#include "itksys/SystemTools.hxx"
// OTB includes
#include "otbMacro.h"
#include "otbOGRDriversInit.h"
#include "otbSystem.h"
// OGR includes
#include "ogrsf_frmts.h"

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
    {".SHP", "ESRI Shapefile"},
    {".TAB", "MapInfo File"},
    {".GML", "GML"},
    {".GPX", "GPX"},
    {".SQLITE", "SQLite"},
    {".KML", "KML"},
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
  const std::string extension = itksys::SystemTools::GetFilenameLastExtension(filename);
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
: m_DataSource(0),
  m_OpenMode(Modes::Update_LayerUpdate),
  m_FirstModifiableLayerID(0)
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

otb::ogr::DataSource::DataSource(OGRDataSource * source, Modes::type mode)
: m_DataSource(source),
  m_OpenMode(mode),
  m_FirstModifiableLayerID(0)
{
  m_FirstModifiableLayerID = GetLayersCount();
}

otb::ogr::DataSource::Pointer otb::ogr::DataSource::OpenDataSource(std::string const& datasourceName, Modes::type mode)
{
  bool update = (mode != Modes::Read);

  OGRDataSource * source = OGRSFDriverRegistrar::Open(datasourceName.c_str(), update);
  if (!source)
    {
    // In read mode, this is a failure
    // In write mode (Overwrite and Update), create the data source transparently
    if (mode == Modes::Read)
      {
      itkGenericExceptionMacro(<< "Failed to open OGRDataSource file "
        << datasourceName<<" : " << CPLGetLastErrorMsg());
      }

    // Hand made factory based on file extension.
    char const* driverName = DeduceDriverName(datasourceName);
    if (!driverName)
      {
      itkGenericExceptionMacro(<< "No OGR driver known to OTB to create and handle a DataSource named <"
        <<datasourceName<<">.");
      }

    OGRSFDriver * d = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(driverName);

    if(!d)
      {
      itkGenericExceptionMacro(<<"Could not create OGR driver "<<driverName<<", check your OGR configuration for available drivers.");
      }

    source = d->CreateDataSource(datasourceName.c_str());
    if (!source) {
      itkGenericExceptionMacro(<< "Failed to create OGRDataSource <"<<datasourceName
        <<"> (driver name: <" << driverName<<">: " << CPLGetLastErrorMsg());
    }
    source->SetDriver(d);
    }
  return otb::ogr::DataSource::New(source, mode);
}

void DeleteDataSource(std::string const& datasourceName)
{
  // Attempt to delete the datasource if it already exists
  OGRDataSource * poDS = OGRSFDriverRegistrar::Open(datasourceName.c_str(), TRUE);

  if (poDS != NULL)
    {
    OGRSFDriver * ogrDriver = poDS->GetDriver();
    OGRDataSource::DestroyDataSource(poDS);
    //Erase the data if possible
    if (ogrDriver->TestCapability(ODrCDeleteDataSource))
      {
      //Delete datasource
      OGRErr ret = ogrDriver->DeleteDataSource(datasourceName.c_str());
      if (ret != OGRERR_NONE)
        {
        itkGenericExceptionMacro(<< "Deletion of data source " << datasourceName
                        << " failed: " << CPLGetLastErrorMsg());
        }
      }
    else
      {
      itkGenericExceptionMacro(<< "Cannot delete data source " << datasourceName);
      }
    } // if (poDS != NULL)
}

otb::ogr::DataSource::Pointer
otb::ogr::DataSource::New(std::string const& datasourceName, Modes::type mode)
{
  if (mode < Modes::Read || mode >= Modes::MAX__)
    {
    itkGenericExceptionMacro(<< "Wrong mode when opening " << datasourceName);
    }

  Drivers::Init();

  if (mode == Modes::Overwrite)
    {
    DeleteDataSource(datasourceName);
    }

  return OpenDataSource(datasourceName, mode);
}

/*static*/
otb::ogr::DataSource::Pointer
otb::ogr::DataSource::New(OGRDataSource * source, Modes::type mode)
{
  Pointer res = new DataSource(source, mode);
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
  std::vector<std::string> const& papszOptions/* = NULL */)
{
  assert(m_DataSource && "Datasource not initialized");

  // In read mode, try to return an existing layer.
  // and throw if not found
  if (m_OpenMode == Modes::Read)
    {
    otb::ogr::Layer l = GetLayerChecked(name); // will throw if not existing
    itkGenericOutputMacro(<< "Requesting layer creation in read-only OGRDataSource. Returning the existing layer");
    return l;
    }

  // Other mode : Check if the layer already exists.
  otb::ogr::Layer layer = GetLayer(name); // won't throw on failure

  switch (m_OpenMode)
  {
  case Modes::Update_LayerOverwrite:
    {
    // First delete the layer
    if (layer)
      {
      DeleteLayer(GetLayerID(name));
      }

    // Then create it
    OGRLayer * ol = m_DataSource->CreateLayer(
      name.c_str(), poSpatialRef, eGType, otb::ogr::StringListConverter(papszOptions).to_ogr());
    if (!ol)
      {
      itkGenericExceptionMacro(<< "Failed to create the layer <"<<name
        << "> in the OGRDataSource file <" << m_DataSource->GetName()
        <<">: " << CPLGetLastErrorMsg());
      }

    const bool modifiable = true;
    return Layer(ol, modifiable);
    }
    break;
  case Modes::Overwrite:
  case Modes::Update_LayerUpdate:
    {
    if (layer)
      {
      return layer;
      }
    else
      {
      // Then create it
      OGRLayer * ol = m_DataSource->CreateLayer(
        name.c_str(), poSpatialRef, eGType, otb::ogr::StringListConverter(papszOptions).to_ogr());
      if (!ol)
        {
        itkGenericExceptionMacro(<< "Failed to create the layer <"<<name
          << "> in the OGRDataSource file <" << m_DataSource->GetName()
          <<">: " << CPLGetLastErrorMsg());
        }

      const bool modifiable = true;
      return Layer(ol, modifiable);
      }
    }
    break;
  case Modes::Update_LayerCreateOnly:
    {
    // The layer exists. Returns it
    // It will be non-modifiable if already existing at OGRDataSource creation time
    if (layer)
      {
      return layer;
      }

    // Case where the layer does not exists
    OGRLayer * ol = m_DataSource->CreateLayer(
      name.c_str(), poSpatialRef, eGType, otb::ogr::StringListConverter(papszOptions).to_ogr());
    if (!ol)
      {
      itkGenericExceptionMacro(<< "Failed to create the layer <"<<name
        << "> in the OGRDataSource file <" << m_DataSource->GetName()
        <<">: " << CPLGetLastErrorMsg());
      }

    const bool modifiable = true;
    return Layer(ol, modifiable);
    }
    break;
  default :
    assert(false && "Should never be there");
    itkGenericExceptionMacro(<< "OGRDataSource opening mode not supported");
    break;
  }

  itkGenericExceptionMacro(<< "OGRDataSource opening mode not supported");
  return Layer(0, false); // keep compiler happy
}

otb::ogr::Layer otb::ogr::DataSource::CopyLayer(
  Layer & srcLayer,
  std::string const& newName,
  char ** papszOptions/* = NULL */)
{
  assert(m_DataSource && "Datasource not initialized");

  switch (m_OpenMode)
  {
    case Modes::Invalid:
      assert(false && "Invalid OGRDataSource opening mode");
      itkGenericExceptionMacro(<< "Invalid OGRDataSource opening mode");
      break;
    case Modes::Read:
      itkGenericExceptionMacro(<< "OGRDataSource is opened in Read mode : cannot create a layer");
      break;
    default:
      break;
  }

  OGRLayer * l0 = &srcLayer.ogr();
  OGRLayer * ol = m_DataSource->CopyLayer(l0, newName.c_str(), papszOptions);
  if (!ol)
    {
    itkGenericExceptionMacro(<< "Failed to copy the layer <"
      << srcLayer.GetName() << "> into the new layer <" <<newName
      << "> in the OGRDataSource file <" << m_DataSource->GetName()
      <<">: " << CPLGetLastErrorMsg());
    }
  const bool modifiable = true;
  Layer l(ol, modifiable);
  return l;
}

void otb::ogr::DataSource::DeleteLayer(size_t i)
{
  assert(m_DataSource && "Datasource not initialized");

  switch (m_OpenMode)
  {
    case Modes::Invalid:
      assert(false && "Invalid OGRDataSource opening mode");
      itkGenericExceptionMacro(<< "Invalid OGRDataSource opening mode");
      break;
    case Modes::Read:
      itkGenericExceptionMacro(<< "OGRDataSource is opened in Read mode : cannot delete a layer");
      break;
    case Modes::Update_LayerCreateOnly:
      itkGenericExceptionMacro(<< "OGRDataSource is opened in Update_LayerCreateOnly mode : cannot delete a layer");
      break;
    default:
      break;
  }

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

bool otb::ogr::DataSource::IsLayerModifiable(size_t i) const
{
  assert(m_DataSource && "Datasource not initialized");
  switch(m_OpenMode)
  {
    case Modes::Read:
      return false;
    case Modes::Update_LayerCreateOnly:
      return int(i) >= m_FirstModifiableLayerID;
    default:
      return true;
  }
}

bool otb::ogr::DataSource::IsLayerModifiable(std::string const& layername) const
{
  assert(m_DataSource && "Datasource not initialized");
  switch(m_OpenMode)
  {
    case Modes::Read:
      return false;
    case Modes::Update_LayerCreateOnly:
        {
        const int id = this->GetLayerIDUnchecked(layername);
        return id >= m_FirstModifiableLayerID;
        }
    default:
      return true;
  }
}

int otb::ogr::DataSource::GetLayerIDUnchecked(std::string const& name) const
{
  assert(m_DataSource && "Datasource not initialized");
  for (int i = 0, N = GetLayersCount(); i < N; i++)
    {
    OGRLayer * raw_layer = GetLayerUnchecked(i);
    // wrapping to give access to GetName() that doesn't exist with all version of gdal/ogr
    Layer layer(raw_layer, false);
    if (layer.GetName() == name)
      {
      return i;
      }
    }
  return -1;
}

size_t otb::ogr::DataSource::GetLayerID(std::string const& name) const
{
  int const id = GetLayerIDUnchecked(name);
  if (id < 0)
    {
    itkExceptionMacro( << "Cannot fetch any layer named <" << name
      << "> in the OGRDataSource <" << m_DataSource->GetName() << ">: "
      << CPLGetLastErrorMsg());
    }
  return 0; // keep compiler happy
}

otb::ogr::Layer otb::ogr::DataSource::GetLayerChecked(size_t i)
{
  assert(m_DataSource && "Datasource not initialized");
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
  return otb::ogr::Layer(layer_ptr, IsLayerModifiable(i));
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
  return otb::ogr::Layer(layer_ptr, layer_ptr ? IsLayerModifiable(name) : false);
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
  return otb::ogr::Layer(layer_ptr, IsLayerModifiable(name));
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
  const bool modifiable = false;
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
    return otb::ogr::Layer(0, modifiable);
#endif
    }
  return otb::ogr::Layer(layer_ptr, *m_DataSource, modifiable);
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

OGREnvelope otb::ogr::DataSource::GetGlobalExtent(bool force/* = false */, std::string *outwkt) const
{
  assert(m_DataSource && "Datasource not initialized");
  const_iterator lit = this->begin();

  if(lit==this->end())
    {
    itkGenericExceptionMacro(<< "Cannot compute global extent because there are no layers in the DataSource");
    }

  const OGRSpatialReference * ref_srs = lit->GetSpatialRef();
  OGREnvelope sExtent = lit->GetExtent(force);

  if (outwkt)
    {
    *outwkt = lit->GetProjectionRef();
    }

  ++lit;

  for(; lit!=this->end(); ++lit)
    {
    OGREnvelope cExtent = lit->GetExtent(force); // may throw

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

      const double real_minx = std::min(cExtent.MinX,cExtent.MaxX);
      const double real_miny = std::min(cExtent.MinY,cExtent.MaxY);
      const double real_maxx = std::max(cExtent.MinX,cExtent.MaxX);
      const double real_maxy = std::max(cExtent.MinY,cExtent.MaxY);

      cExtent.MinX = real_minx;
      cExtent.MinY = real_miny;
      cExtent.MaxX = real_maxx;
      cExtent.MaxY = real_maxy;

#if GDAL_VERSION_NUM >= 1700
      OGRCoordinateTransformation::DestroyCT(coordTransformation);
#else
#warning the following resource release may crash, please update your version of GDAL
      delete coordTransformation; // note there is no garanty
#endif
      }
    // else: If srs are invalid, we assume that extent are coherent

    // Merge with previous layers' extent
    sExtent.Merge(cExtent);
    } // for each layer

  return sExtent;
}

std::string otb::ogr::DataSource::GetGlobalExtent(double & ulx,
                                           double & uly,
                                           double & lrx,
                                           double & lry,
                                           bool force) const
{
  std::string outwkt;
  const OGREnvelope sExtent = GetGlobalExtent(force, &outwkt);
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

/*virtual*/ void otb::ogr::DataSource::Graft(const itk::DataObject * itkNotUsed(data))
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
