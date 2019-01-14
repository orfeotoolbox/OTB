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
#include "otbOGRDriversInit.h"
#include "otbSystem.h"
// OGR includes

/*===========================================================================*/
/*=======================[ construction/destruction ]========================*/
/*===========================================================================*/

bool otb::ogr::DataSource::Clear()
{
  Reset(nullptr);
  return true;
}

void otb::ogr::DataSource::Reset(GDALDataset * source)
{
  if (m_DataSource) {
    // OGR makes a pointless check for non-nullity in
    // GDALDataset::DestroyDataSource (pointless because "delete 0" is
    // perfectly valid -> it's a no-op)
    GDALClose(m_DataSource); // void, noexcept
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
    {".GMT", "OGR_GMT"},
    {".GPX", "GPX"},
    {".SQLITE", "SQLite"},
    {".KML", "KML"},
    {".CSV", "CSV"},
    {".GPKG", "GPKG"}
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
  if(filename.compare(0, 3, "PG:") == 0)
    {
    return "PostgreSQL";
    }
  const std::string extension = itksys::SystemTools::GetFilenameLastExtension(filename);
  ExtensionDriverAssociation const* whichIt =
    std::find_if(
      boost::begin(k_ExtensionDriverMap), boost::end(k_ExtensionDriverMap),
      [&](auto const & x) { return x.Matches(extension); } );
  if (whichIt ==  boost::end(k_ExtensionDriverMap))
    {
    return nullptr; // nothing found
    }
  return whichIt->driverName;
  }
} // Anonymous namespace


otb::ogr::DataSource::DataSource()
: m_DataSource(nullptr),
  m_LayerOptions() , 
  m_OpenMode(Modes::Update_LayerUpdate),
  m_FirstModifiableLayerID(0)
{
  Drivers::Init();

  GDALDriver * d = 
    GetGDALDriverManager()->GetDriverByName("Memory");
  assert(d && "OGR Memory driver not found");
  m_DataSource = d->Create( "in-memory" ,
                         0 ,
                         0 ,
                         0 ,
                         GDT_Unknown ,
                         0 );

  if (!m_DataSource) {
    itkExceptionMacro(<< "Failed to create OGRMemDataSource: " 
                      << CPLGetLastErrorMsg());
  }
}

otb::ogr::DataSource::DataSource( 
    GDALDataset * source ,
    Modes::type mode ,
    const std::vector< std::string > & options /*NULL*/ )
: m_DataSource(source) ,
  m_LayerOptions(options) ,
  m_OpenMode(mode) ,
  m_FirstModifiableLayerID(0)
{
  m_FirstModifiableLayerID = GetLayersCount();
}

otb::ogr::DataSource::Pointer otb::ogr::DataSource::OpenDataSource(std::string const& datasourceName, Modes::type mode)
{
  FileNameHelperType::Pointer fileNameHelper = FileNameHelperType::New();
  fileNameHelper->SetExtendedFileName( datasourceName.c_str() );
  std::string simpleFileName = fileNameHelper->GetSimpleFileName();

  bool update = (mode != Modes::Read);
  GDALDataset * source = (GDALDataset *)GDALOpenEx(
      simpleFileName.c_str(), 
      (update? GDAL_OF_UPDATE: GDAL_OF_READONLY) | GDAL_OF_VECTOR,
      NULL,
      otb::ogr::StringListConverter( fileNameHelper->GetGDALOpenOptions() ).to_ogr(),
      NULL);
  if (!source)
    {
    // In read mode, this is a failure
    // In write mode (Overwrite and Update), create the data source transparently
    if (mode == Modes::Read)
      {
      itkGenericExceptionMacro(<< "Failed to open GDALDataset file "
        << simpleFileName<<" : " << CPLGetLastErrorMsg());
      }

    // Hand made factory based on file extension.
    char const* driverName = DeduceDriverName(simpleFileName);
    if (!driverName)
      {
      itkGenericExceptionMacro(<< "No OGR driver known to OTB to create and "
        "handle a DataSource named <"
        <<simpleFileName<<">.");
      }

    GDALDriver * d = 
      GetGDALDriverManager()->GetDriverByName(  driverName  );

    if(!d)
      {
      itkGenericExceptionMacro(<< "Could not create OGR driver " << driverName 
        << ", check your OGR configuration for available drivers." );
      }

    source = d->Create( simpleFileName.c_str() ,
                         0 ,
                         0 ,
                         0 ,
                         GDT_Unknown ,
                         otb::ogr::StringListConverter( 
                          fileNameHelper->GetGDALCreationOptions() ).to_ogr() );
    if (!source) {
      itkGenericExceptionMacro(<< "Failed to create GDALDataset <"
        << simpleFileName << "> (driver name: <" << driverName 
        <<">: " << CPLGetLastErrorMsg());
    }
    }
  return otb::ogr::DataSource::New( source , mode , fileNameHelper->GetGDALLayerOptions() );
}

void DeleteDataSource(std::string const& datasourceName)
{
  otb::OGRExtendedFilenameToOptions::Pointer fileNameHelper = 
    otb::OGRExtendedFilenameToOptions::New();
  fileNameHelper->SetExtendedFileName( datasourceName.c_str() );
  std::string simpleFileName = fileNameHelper->GetSimpleFileName();

  // Open dataset
  GDALDataset * poDS = (GDALDataset *)GDALOpenEx(
      simpleFileName.c_str(), 
       GDAL_OF_UPDATE | GDAL_OF_VECTOR,
      NULL,
      NULL,
      NULL);

  GDALDriver * poDriver = NULL;
  if(poDS)
    {
    poDriver = poDS->GetDriver();
    GDALClose(poDS);
    }
  else
    {
    itkGenericExceptionMacro(<< "Cannot open data source " << simpleFileName
                             << ": " << CPLGetLastErrorMsg());
    }
  if(poDriver)
    {
    OGRErr ret = poDriver->Delete(simpleFileName.c_str());
    if (ret != OGRERR_NONE)
      {
      itkGenericExceptionMacro(<< "Deletion of data source " << simpleFileName
                             << " failed: " << CPLGetLastErrorMsg());
      }
    }
  else 
    {
    itkGenericExceptionMacro(<< "Cannot get driver associated with data source " << simpleFileName
                             << ": " << CPLGetLastErrorMsg());
    }

}

otb::ogr::DataSource::Pointer
otb::ogr::DataSource::New(std::string const& datasourceName, Modes::type mode)
{
  FileNameHelperType::Pointer fileNameHelper = FileNameHelperType::New();
  fileNameHelper->SetExtendedFileName( datasourceName.c_str() );
  std::string simpleFileName = fileNameHelper->GetSimpleFileName();

  if (mode < Modes::Read || mode >= Modes::MAX__)
    {
    itkGenericExceptionMacro(<< "Wrong mode when opening " << simpleFileName );
    }

  Drivers::Init();
  GDALDataset * ds = (GDALDataset *)GDALOpenEx(
      simpleFileName.c_str(), 
      GDAL_OF_READONLY | GDAL_OF_VECTOR,
      NULL,
      NULL,
      NULL);
  bool ds_exists = (ds!=nullptr);

  GDALClose(ds);


  if (ds_exists && mode == Modes::Overwrite)
    {
    DeleteDataSource(datasourceName);
    }

  return OpenDataSource(datasourceName, mode);
}

/*static*/
otb::ogr::DataSource::Pointer
otb::ogr::DataSource::New(GDALDataset * source , Modes::type mode , const std::vector< std::string > & layerOptions )
{
  Pointer res = new DataSource( source , mode , layerOptions );
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
    itkGenericOutputMacro(<< "Requesting layer creation in read-only "
                             "GDALDataset. Returning the existing layer");
    return l;
    }

  // Other mode : Check if the layer already exists.
  otb::ogr::Layer layer = GetLayer(name); // won't throw on failure

  FileNameHelperType::Pointer layerOptionHelper = 
    FileNameHelperType::GetGDALLayerOptionsHelper( m_LayerOptions );
  layerOptionHelper->AddGDALLayerOptions( papszOptions );
  std::vector<std::string> layerOptions = 
    layerOptionHelper->GetGDALLayerOptions();

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
      name.c_str() ,
      poSpatialRef ,
      eGType ,
      otb::ogr::StringListConverter( layerOptions ).to_ogr() );

    if (!ol)
      {
      itkGenericExceptionMacro(<< "Failed to create the layer <"<<name
        << "> in the GDALDataset file <" << GetDatasetDescription()
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
        name.c_str() ,
        poSpatialRef ,
        eGType ,
        otb::ogr::StringListConverter( layerOptions ).to_ogr() );

      if (!ol)
        {
        itkGenericExceptionMacro(<< "Failed to create the layer <"<<name
          << "> in the GDALDataset file <" <<  GetDatasetDescription()
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
    // It will be non-modifiable if already existing at GDALDataset creation time
    if (layer)
      {
      return layer;
      }

    // Case where the layer does not exists
    OGRLayer * ol = m_DataSource->CreateLayer(
          name.c_str() ,
          poSpatialRef ,
          eGType ,
          otb::ogr::StringListConverter( layerOptions ).to_ogr() );

    if (!ol)
      {
      itkGenericExceptionMacro(<< "Failed to create the layer <"<<name
        << "> in the GDALDataset file <" <<  GetDatasetDescription()
        <<">: " << CPLGetLastErrorMsg());
      }

    const bool modifiable = true;
    return Layer(ol, modifiable);
    }
    break;
  default :
    assert(false && "Should never be there");
    itkGenericExceptionMacro(<< "GDALDataset opening mode not supported");
    break;
  }

  itkGenericExceptionMacro(<< "GDALDataset opening mode not supported");
  return Layer(nullptr, false); // keep compiler happy
}

otb::ogr::Layer otb::ogr::DataSource::CopyLayer(
  Layer & srcLayer,
  std::string const& newName,
  std::vector<std::string> const& papszOptions/* = NULL */)
{
  assert(m_DataSource && "Datasource not initialized");

  switch (m_OpenMode)
  {
    case Modes::Invalid:
      assert(false && "Invalid GDALDataset opening mode");
      itkGenericExceptionMacro(<< "Invalid GDALDataset opening mode");
      break;
    case Modes::Read:
      itkGenericExceptionMacro(<< "GDALDataset is opened in Read mode : "
                                  "cannot create a layer");
      break;
    default:
      break;
  }
  
  FileNameHelperType::Pointer layerOptionHelper = 
    FileNameHelperType::GetGDALLayerOptionsHelper( m_LayerOptions );
  layerOptionHelper->AddGDALLayerOptions( papszOptions );
  std::vector<std::string> layerOptions = 
    layerOptionHelper->GetGDALLayerOptions();

  OGRLayer * l0 = &srcLayer.ogr();
  OGRLayer * ol = m_DataSource->CopyLayer( 
                  l0 ,
                  newName.c_str() ,
                  otb::ogr::StringListConverter( layerOptions ).to_ogr() );
  if (!ol)
    {
    itkGenericExceptionMacro(<< "Failed to copy the layer <"
      << srcLayer.GetName() << "> into the new layer <" << newName
      << "> in the GDALDataset file <" <<  GetDatasetDescription()
      <<">: " << CPLGetLastErrorMsg());
    }
  const bool modifiable = true;
  Layer l( ol , modifiable );
  return l;
}

void otb::ogr::DataSource::DeleteLayer(size_t i)
{
  assert(m_DataSource && "Datasource not initialized");

  switch (m_OpenMode)
  {
    case Modes::Invalid:
      assert(false && "Invalid GDALDataset opening mode");
      itkGenericExceptionMacro(<< "Invalid GDALDataset opening mode");
      break;
    case Modes::Read:
      itkGenericExceptionMacro(<< "GDALDataset is opened in Read mode : "
        "cannot delete a layer");
      break;
    case Modes::Update_LayerCreateOnly:
      itkGenericExceptionMacro(<< "GDALDataset is opened in "
        "Update_LayerCreateOnly mode : cannot delete a layer");
      break;
    default:
      break;
  }

  const int nb_layers = GetLayersCount();
  if (int(i) >= nb_layers)
    {
    itkExceptionMacro(<< "Cannot delete " << i 
      << "th layer in the GDALDataset <" <<  GetDatasetDescription() 
      << "> as it contains only " << nb_layers << "layers.");
    }
  const OGRErr err = m_DataSource->DeleteLayer(int(i));
  if (err != OGRERR_NONE)
    {
    itkExceptionMacro(<< "Cannot delete " << i 
      << "th layer in the GDALDataset <" <<  GetDatasetDescription() 
      << ">: " << CPLGetLastErrorMsg());
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
      << "> in the GDALDataset <" <<  GetDatasetDescription() << ">: "
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
    itkExceptionMacro(<< "Cannot fetch " << i << "th layer in the GDALDataset <"
      << GetDatasetDescription() << "> as it contains only " << nb_layers 
      << "layers.");
    }
  OGRLayer * layer_ptr = m_DataSource->GetLayer(int(i));
  if (!layer_ptr)
    {
    itkExceptionMacro( << "Unexpected error: cannot fetch " << i 
      << "th layer in the GDALDataset <" << GetDatasetDescription() 
      << ">: " << CPLGetLastErrorMsg());
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
      << "> in the GDALDataset <" << GetDatasetDescription() << ">: "
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
    itkExceptionMacro( << "Unexpected error: cannot execute the SQL request <" 
      << statement << "> in the GDALDataset <" <<  GetDatasetDescription() 
      << ">: " << CPLGetLastErrorMsg());
#else
    // Cannot use the deleter made for result sets obtained from
    // GDALDataset::ExecuteSQL because it checks for non-nullity....
    // *sigh*
    return otb::ogr::Layer(nullptr, modifiable);
#endif
    }
  return otb::ogr::Layer(layer_ptr, *m_DataSource, modifiable);
}

void
otb::ogr::DataSource::
SetLayerCreationOptions( const std::vector< std::string > & options )
{
  FileNameHelperType::Pointer helper = FileNameHelperType::New();
  helper->SetGDALLayerOptions( options );
  m_LayerOptions = helper->GetGDALLayerOptions();
  // perf : do we move code from  helper->SetGDALLayerOptions in here?
}

void
otb::ogr::DataSource::
AddLayerCreationOptions( std::vector< std::string > options )
{
  FileNameHelperType::Pointer helper = FileNameHelperType::New();
  helper->SetGDALLayerOptions( m_LayerOptions );
  helper->AddGDALLayerOptions( options );
  m_LayerOptions = helper->GetGDALLayerOptions();
  // perf : do we move code from helper->AddGDALLayerOptions in here?
}

const std::vector< std::string > &
otb::ogr::DataSource::
GetLayerCreationOptions() const
{
  return m_LayerOptions;
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
    itkGenericExceptionMacro(<< "Cannot compute global extent because there "
      "are no layers in the DataSource");
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

      OGRCoordinateTransformation::DestroyCT(coordTransformation);
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
  m_DataSource->FlushCache();
}


std::string otb::ogr::DataSource::GetDatasetDescription() const
{
  std::vector<std::string> files = 
    otb::ogr::GetFileListAsStringVector( m_DataSource );
  std::string description = "";
  for( std::vector<std::string>::const_iterator it = files.begin() ; 
       it!=files.end() ; ++it )
    description+=(*it)+", ";

  return description;
}
