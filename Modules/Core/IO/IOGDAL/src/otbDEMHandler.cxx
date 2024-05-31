/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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

#include "otbDEMHandler.h"
#include "otbGDALDriverManagerWrapper.h"
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include "gdal_utils.h"

//Warp utility
#include "gdalwarper.h"
#include "vrtdataset.h"

#include "ogr_spatialref.h"

//TODO C++ 17 : use std::optional instead
#include <boost/optional.hpp>

#include <mutex>
#include <thread>
#include <memory>
#include <sstream>

namespace
{ // Anonymous namespace
std::mutex demMutex;
} // Anonymous namespace

namespace otb {

namespace DEMDetails {

// Adapted from boost filesystem documentation:
// https://www.boost.org/doc/libs/1_53_0/libs/filesystem/doc/reference.html
std::vector<std::string> GetFilesInDirectory(const std::string & directoryPath)
{
  std::vector<std::string> fileList;

  if ( !boost::filesystem::exists(directoryPath) )
  {
    return fileList;
  }
  else if (!boost::filesystem::is_directory(directoryPath))
  {
    fileList.push_back(directoryPath);
    return fileList;
  }

  // End iterator: default construction yields past-the-end
  for ( const auto & item : boost::make_iterator_range(boost::filesystem::directory_iterator(directoryPath), {}) )
  {
    try
    {
      if ( boost::filesystem::is_directory(item.status()) )
      {
        auto subDirList = GetFilesInDirectory(item.path().string());
        fileList.insert(fileList.end(), subDirList.begin(), subDirList.end());
      }
      else
      {
        fileList.push_back(item.path().string());
      }
    }
    catch (boost::filesystem::filesystem_error const& e)
    {
      otbLogMacro(Warning, << e.what())
    }
  }

  return fileList;
}

/**
 * Internal RAII wrapper for providing access to `GDALDataset` and
 * caching related information (projection for WGS84 case and geo
 * transformation).
 *
 * \invariant if WGS84 then `(m_Dataset != nullptr) == (m_poCT != nullptr)`
 * \internal
 */
struct DatasetCache
{
  /**@name `GDALDataset` accessors */
  /** @{ */
  GDALDataset * get          () const { return m_Dataset.get();}
  GDALDataset * operator->   () const { return m_Dataset.get();}
  GDALDataset & operator*    () const { assert(m_Dataset); return *m_Dataset;}
  /** @} */

  /// Tells whether a valid dataset is associated.
  explicit      operator bool() const { return bool(m_Dataset); }

  DatasetCache() = default;
  ~DatasetCache() = default;

  /** Constructor that takes over a `GDALDataset` ownership.
   * \post Projection information will have been fetched (if WGS84).
   * \post Geo Transform information will have been fetched
   */
  explicit DatasetCache(GDALDataset *ds)
  { reset(ds); }

  /** Constructor from a filename.
   * \post Projection information will have been fetched (if WGS84).
   * \post Geo Transform information will have been fetched
   */
  explicit DatasetCache(std::string const& file)
    : DatasetCache(static_cast<GDALDataset*>(GDALOpen(file.c_str(), GA_ReadOnly)))
    {}

  /** Release ownership of the dataset.
   * And clear projection related information.
   */
  void release()
  {
    m_Dataset.release();
    m_poCT.release();
    m_isWGS84 = false;
  }

  /** Takes over a `GDALDataset` ownership.
   * \post Projection information will have been fetched (if WGS84).
   * \post Geo Transform information will have been fetched
   */
  void reset(GDALDataset* ds)
  {
    m_Dataset.reset(ds);
    if (m_Dataset)
    {
#if GDAL_VERSION_NUM >= 3000000
      auto srs = ds->GetSpatialRef();
#else
      auto projRef = ds->GetProjectionRef();

      std::unique_ptr<OGRSpatialReference> srsUniquePtr;
      OGRSpatialReference* srs = nullptr;
      // GetProjectionRef() returns an empty non null string if no projection is available
      if (strlen(projRef) != 0 )
      {
        srsUniquePtr = std::make_unique<OGRSpatialReference>(ds->GetProjectionRef());
        srs = srsUniquePtr.get();
      }
#endif

      static auto const& wgs84Srs = OGRSpatialReference::GetWGS84SRS(); // 1%
      m_isWGS84 = false;

      // Convert input lon lat into the coordinates defined by the dataset if needed.
      if (srs && !srs->IsSame(wgs84Srs)) // 10% of the time
      {
        m_isWGS84 = true;
        m_poCT = std::unique_ptr<OGRCoordinateTransformation>
          (OGRCreateCoordinateTransformation(wgs84Srs, srs));
      }

      m_Dataset->GetGeoTransform(m_geoTransform);

      m_NoDataValue = m_Dataset->GetRasterBand(1)->GetNoDataValue();
    }
    else
    {
      m_poCT.release();
    }
  }

  /** Convert {lon, lat} coordinates if WGS84 and projection is
   * defined.
   *
   * \todo what about `isWGS84() == true` and projection is not defined?
   * Is it even even possible?
   */
  bool convert_lon_lat(double & lon, double & lat) const
  {
    assert((m_poCT || !isWGS84()) && "Expected: isWGS84() => projection is defined"); // => no need to test isWGS84
    if (/*isWGS84() &&*/ m_poCT) {
      return m_poCT->Transform( 1, &lon, &lat );
    }
    return true;
  }

  /// Apply geo transformation to return pixels associated to {lon, * lat}.
  std::pair<double, double> transform(double lon, double lat) const
  {
    auto const x = (lon - m_geoTransform[0]) / m_geoTransform[1] - 0.5;
    auto const y = (lat - m_geoTransform[3]) / m_geoTransform[5] - 0.5;
    return std::make_pair(x, y);
  }

  bool isWGS84() const noexcept { return m_isWGS84; }

  /// Accessors to Geo Transformation.
  double      * getGeoTransform()       noexcept { return m_geoTransform;}
  double const* getGeoTransform() const noexcept { return m_geoTransform;}

  /// Accessor to No Data value
  double GetNoDataValue() const noexcept { return m_NoDataValue;}


  DatasetCache & operator=(DatasetCache const&) = delete;
  DatasetCache & operator=(DatasetCache &&    ) = default;

private:
  using DatasetUPtr = std::unique_ptr<GDALDataset, void(*)(GDALDataset*)>;
  using OCT_ptr     = std::unique_ptr<OGRCoordinateTransformation>;
  DatasetUPtr m_Dataset = DatasetUPtr(nullptr, [](GDALDataset* DS){if(DS){GDALClose(DS);}});
  OCT_ptr     m_poCT;
  bool        m_isWGS84 = false;
  double      m_geoTransform[6] = {};
  double      m_NoDataValue     = {};
};

/**
 * Obtain DEM value from dataset at specified position.
 *
 * General operation sequence:
 * 1. Convert {lon, lat} in WGS84 case
 * 2. Computed associated pixel coordinates (in floatting point unit)
 *    thanks to geo transformation
 * 3. Extract 4 pixels around the desired position
 * 4. Interpolate the final elevation
 *
 * \return `boost::none` if {lon, lat} conversion cannot be achieved
 * \return `boost::none` if {lon, lat} conversion cannot be achieved
 * \return `boost::none` if pixel extractions from file fails
 * \return elevation according to DEM at {lot, lat} position
 * \internal
 */
boost::optional<double> GetDEMValue(double lon, double lat, DatasetCache const& dsc,ElevationCache const& elevation_cache)
{
  if (!dsc.convert_lon_lat(lon, lat))
  {
    return boost::none;
  }

  // C++17 use: `auto const [x,y] = dsc.transform(lon, lat);`
  auto const xy = dsc.transform(lon, lat);
  auto const x = xy.first;
  auto const y = xy.second;

  auto is_out_raster = [&](auto x, auto y, GDALDataset & ds) {
    return x < 0
      ||   y < 0
      || 1+x > ds.GetRasterXSize()   // no need to test x > size
      || 1+y > ds.GetRasterYSize();  // no need to test Y > size
  };

  if (is_out_raster(x, y, *dsc))
  {
    return boost::none;
  }

  auto const x_int = static_cast<int>(x);
  auto const y_int = static_cast<int>(y);

  auto const deltaX = x - x_int;
  auto const deltaY = y - y_int;

  // Bilinear interpolation.
  std::array<double, 4> elevData;

  if (elevation_cache.holds(x_int, y_int))
  {
    elevation_cache.read(x_int, y_int, elevData);
  }
  else 
  {
    auto const err = dsc->GetRasterBand(1)->RasterIO(
        GF_Read, x_int, y_int, 2, 2,
        elevData.data(), 2, 2, GDT_Float64,
        0, 0, nullptr);

    if (err)
    {
      return boost::none;
    }

  }
  // Test for no data. Don't return a value if any pixel of the
  // interpolation has no data.
  auto const no_data = dsc.GetNoDataValue();
  auto const has_no_data = [=](auto const v){ return  v == no_data; };
  if(std::any_of(std::begin(elevData), std::end(elevData), has_no_data))
  {
    return boost::none;
  }

  // C++20: use std::lerp for better precision (expected to be slower)
  auto const xBil1 = elevData[0] * (1.0 - deltaX) + elevData[1] * deltaX;
  auto const xBil2 = elevData[2] * (1.0 - deltaX) + elevData[3] * deltaX;

  auto const yBil = xBil1 * (1.0 - deltaY) + xBil2 * deltaY;
  return yBil;
}

/**
 * Tells whether the dataset has a Geo Transformation.
 * \internal
 */
bool HasGeoTransform(GDALDataset & gdalds)
{
  std::array<double, 6> a;
  return gdalds.GetGeoTransform(a.data()) == CE_None;
}

}  // namespace DEMDetails


/** Internal actual instance of DEM handling wrapper for current thread.
 * \ingroup OTBIOGDAL
 *
 * `GDALDataset` isn't thread safe. However, several datasets can be
 * opened to the same image. Instances of this class are meant to
 * provide the final access to `GDALDataset` and live as singletons
 * in each thread.
 *
 * \internal This class is not exposed to other code. It's meant to be
 * used through `DEMHandler` singleton.
 *
 * The current design has one `DEMHandlerTLS` per actual thread. In case
 * threads are terminated and restarted multiple time (this is the case
 * with default old ITK 4 architecture that will terminate and respawn
 * thread for new OTB stream), `DEMHandlerTLS` instance will be
 * searched in `DEMHandler::m_tlses` pool.
 */
class DEMHandlerTLS
{
public:
  double GetHeightAboveEllipsoid(double lon, double lat, double defaultHeight,
      ElevationCache const& dem_elevation_cache,
      ElevationCache const& geoid_elevation_cache) const;
  boost::optional<double> GetHeightAboveMSL(double lon, double lat,
      ElevationCache const& dem_elevation_cache) const;
  boost::optional<double> GetGeoidHeight(double lon, double lat,
      ElevationCache const& geoid_elevation_cache) const;

  DEMHandlerTLS() {
    otbMsgDevMacro(<<std::this_thread::get_id() << " § DEMHandlerTLS::DEMHandlerTLS() --> " << this);
  }
  ~DEMHandlerTLS() {
    otbMsgDevMacro(<<std::this_thread::get_id() << " § DEMHandlerTLS::~DEMHandlerTLS() --> " << this);
  }

  bool OpenDEMVRTFile();
  void CloseDEMVRTFile();

  /** Try to open a geoid file
   * \param geoidFile input geoid path
   */
  bool OpenGeoidFile(std::string const& geoidFile);

  /** Clear the DEM list and geoid filename, close all elevation datasets
   * and reset the default height above ellipsoid */
  void ClearElevationParameters();

  void CreateShiftedDatasetOnce() const;

private:
  DEMHandlerTLS(DEMHandlerTLS const&) = delete;
  void operator=(DEMHandlerTLS const&) = delete;

  using DatasetUPtr = std::unique_ptr<GDALDataset, void(*)(GDALDataset*)>;

  /** Pointer to the DEM dataset */
  DEMDetails::DatasetCache m_DEMDS;

  /** Pointer to the geoid dataset */
  DEMDetails::DatasetCache m_GeoidDS;
};

DEMHandlerTLS& DEMHandler::GetHandlerForCurrentThread() const
{
  static thread_local auto tls = DoFetchOrCreateHandler(); // no need to lock as this is a static TLS
  return *tls;
}

std::shared_ptr<DEMHandlerTLS> DEMHandler::DoFetchOrCreateHandler() const
{
  // DEMHandlerTLS instances are shared between the global DEMHandler
  // singleton and the (unique) thread they are associated to.
  // When a thread terminates, its TLS are destroyed. And as such the
  // shared_ptr DEMHandlerTLS are destroyed, which decrements the owning count
  // by 1.
  // When the DEMHandler singleton is destroyed, each DEMHandlerTLS shared_ptr
  // is destroyed, and the count is decreased by 1.
  // When the count reaches 0, the DEMHandlerTLS is actually destroyed.
  // Infortunately we cannot be sure TLS variables are destroyed before the
  // DEMHandler singleton. Hence this convoluted approach with
  // shared_ptr.

  // With the chosen (shared_ptr based) approach, when the owning count is 1,
  // this means only the DEMHandler singleton owns the TLS handler. And as
  // such, it could be attributed to another thread.
  auto handler_is_not_attributed_to_a_thread = [](auto const& h) { return h.use_count() == 1; };

  std::unique_lock<std::mutex> lock(demMutex); // protecting m_tlses
  for (auto & tls : m_tlses) {
    if (handler_is_not_attributed_to_a_thread(tls)) {
      return tls;
    }
  }
  m_tlses.emplace_back(std::make_shared<DEMHandlerTLS>());
  auto res = m_tlses.back();
  lock.unlock(); // No need to keep the lock any more thanks to the copy
                 // of the shared_ptr `res` from the vector that could
                 // be resized... (which would invalidate any reference
                 // to back())
  RegisterConfigurationInHandler(*res);
  return res;
}

void DEMHandler::RegisterConfigurationInHandler(DEMHandlerTLS & tls) const
{
  otbMsgDevMacro(<<std::this_thread::get_id() << " § DEMHandler::RegisterConfigurationInHandler(" << &tls <<")");
  if (! m_DatasetList.empty()) {
    if (! tls.OpenDEMVRTFile())
    {
      otbLogMacro(Warning, << "An unexpected situation has occurred: trying to open a VRT created from incompatible DEM files");
      assert(false);
      // The following should not be necessary. In the impossible case
      // the VRT file could not be opened, we just clear everything in
      // Release mode (i.e. without assertions)
      // Yes, the clearing has an extremely dirty use of const_cast...
      const_cast<DEMHandler*>(this)->m_DatasetList.clear();
      const_cast<DEMHandler*>(this)->m_DEMDirectories.clear();
    }
  }
  if (!m_GeoidFilename.empty()) {
    tls.OpenGeoidFile(m_GeoidFilename);
  }
  otbMsgDevMacro(<<std::this_thread::get_id() << " § DEMHandler::RegisterConfigurationInHandler(" << &tls <<") END");
}

// Meyer singleton design pattern
DEMHandler & DEMHandler::GetInstance()
{
  static DEMHandler s_instance;
  return s_instance;
}

DEMHandler::DEMHandler()
: m_DefaultHeightAboveEllipsoid(0.0)
{
  GDALAllRegister();
}

DEMHandler::~DEMHandler()
{
  otbMsgDevMacro(<<std::this_thread::get_id() << " § DEMHandler::destructor()");

  // Close all elevation datasets
  ClearElevationParameters();

  // As elevation dataset have been closed, we can remove these files
  VSIUnlink(DEMHandler::DEM_DATASET_PATH);
  VSIUnlink(DEMHandler::DEM_WARPED_DATASET_PATH);
  VSIUnlink(DEMHandler::DEM_SHIFTED_DATASET_PATH);

  { // {} makes sure clearing m_tlses is synchronized
    const std::lock_guard<std::mutex> lock(demMutex);
    m_tlses.clear();
  }
  otbMsgDevMacro(<<std::this_thread::get_id() << " § DEMHandler::destructor() END");
}

void DEMHandlerTLS::CloseDEMVRTFile()
{
  m_DEMDS.release();
}

bool DEMHandlerTLS::OpenDEMVRTFile()
{
  otbMsgDevMacro(<<std::this_thread::get_id() << " § DEMHandlerTLS::OpenDEMVRTFile() --> " << this);
  // As we are reopening the same file: we should close it first!
  assert(! m_DEMDS);
  m_DEMDS = DEMDetails::DatasetCache(DEMHandler::DEM_DATASET_PATH);

  // Note: we don't try to call CreateShiftedDatasetOnce() as the
  // current implementation of DEMHandler::OpenGeoidFile() indirectly
  // makes sure to do so by enforcing the existence of a DEMHandlerTLS
  // instance. This will then trigger the call to
  // CreateShiftedDatasetOnce() either from DEMHandler::OpenGeoidFile()
  // and DEMHandler::OpenDEMDirectory()
  // So... we can just exit
  return !! m_DEMDS;
}

void DEMHandler::CacheElevations()
{
  // Check if the cache has been filled once
  if(m_dem_elevation_cache.size() == 0)
  {
    //Fill the dem cache with the current datasetCache
    int rasterSizeX = m_DatasetList[0]->GetDataSet()->GetRasterXSize();
    int rasterSizeY = m_DatasetList[0]->GetDataSet()->GetRasterYSize();
    //m_DEMCache.reserve(rasterSizeX*rasterSizeY);
    auto ds = m_DatasetList[0]->GetDataSet();
    double elevData[rasterSizeX*rasterSizeY];

    auto err = ds->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, rasterSizeX, rasterSizeY,
      elevData, rasterSizeX, rasterSizeY, GDT_Float64,
      0, 0, nullptr);

    if(err){
      otbLogMacro(Warning, << "Error when opening the DEM Dataset for caching");
      return;
    }

    int lon=0,lat=0;
    double padfTransform[6] = {};
    ds->GetGeoTransform(padfTransform);
    for(auto x = 0; x < rasterSizeX; x++)
    {
      for(auto y = 0; y < rasterSizeY ; y++)
      {
        //Convert x and y to lon lat
        lon = padfTransform[0] + x*padfTransform[1] + y*padfTransform[2];
        lat = padfTransform[3] + x*padfTransform[4] + y*padfTransform[5];
        //Store info
        m_dem_elevation_cache.write(lon,lat,elevData[(x*rasterSizeY) + y]);
      }
    }
    GDALClose(ds);
  }
}

void DEMHandler::OpenDEMFile(std::string path)
{
  otbMsgDevMacro(<<std::this_thread::get_id() << " § DEMHandler::OpenDEMFile("<<path<<")");

  std::unique_lock<std::mutex> lock(demMutex); // protecting m_tlses
  m_DatasetList.push_back(otb::GDALDriverManagerWrapper::GetInstance().Open(path));
  for (auto tls : m_tlses) {
    tls->CloseDEMVRTFile();
  }
  // TODO: why [0] and not back()???
  std::array<GDALDatasetH, 1> vrtDatasetList { m_DatasetList[0]->GetDataSet() };
  auto close_me = GDALBuildVRT(DEMHandler::DEM_DATASET_PATH, 1, vrtDatasetList.data(),
      nullptr, nullptr, nullptr);
  // Need to close the dataset, so it is flushed into memory.
  GDALClose(close_me);
  for (auto tls : m_tlses)
  {
    if (! tls->OpenDEMVRTFile())
    {
      // Sometimes ungeoreferenced images are pushed into the VRT files,
      // but as they are not supported, no VRT is produced
      m_DatasetList.clear();
      return ; // no push_back, no notification
    }
  }
  m_DEMDirectories.push_back(move(path));
  lock.unlock();
  Notify();
}

void DEMHandler::OpenDEMDirectory(std::string DEMDirectory)
{
  otbMsgDevMacro(<<std::this_thread::get_id() << " § DEMHandler::OpenDEMDirectory("<<DEMDirectory<<")");
  auto isSameDirectory = [&DEMDirectory](std::string const& s)
  {
    return s == DEMDirectory;
  };
  if(std::any_of(std::begin(m_DEMDirectories), std::end(m_DEMDirectories), isSameDirectory))
  {
    otbLogMacro(Info, << "DEM Directory '"<< DEMDirectory << "' is already opened.");
    return;
  }

  auto demFiles = DEMDetails::GetFilesInDirectory(DEMDirectory);
  std::size_t nb_new_DEM_opened = 0;
  // Check and open every dem files found in DEMDirectory
  for (const auto & file : demFiles)
  {
    otbMsgDevMacro("Try to open DEM image: " << file);
    auto ds = otb::GDALDriverManagerWrapper::GetInstance().Open(file);
    if (ds)
    {
      if (DEMDetails::HasGeoTransform(*ds->GetDataSet()))
      {
        m_DatasetList.push_back(ds);
        ++nb_new_DEM_opened;
      }
      else
      {
        // GDAL VRT files cannot contain references to ungeoreferenced
        // images, otherwise GDAL will issue messages like:
        // > 662: Warning 1: gdalbuildvrt does not support ungeoreferenced image. Skipping BLA/BLA.TIF
        // > ERROR 4: No such file or directory
        otbLogMacro(Debug, << "Input image "<< file << " will not be used because it is ungeoreferenced.");
      }
    }
    else
    {
      otbMsgDevMacro("Ignoring DEM image: " << file);
    }
  }

  // Don't build a vrt if there is no dataset
  if (nb_new_DEM_opened == 0)
  {
    otbLogMacro(Warning, << "No DEM found in "<< DEMDirectory)
  }
  else
  {
    otbLogMacro(Info, << nb_new_DEM_opened << " DEM found in "<< DEMDirectory)
    m_DEMDirectories.push_back(move(DEMDirectory)); // => parameter voluntary taken by value

    // Clean before anything else: Free the previous in-memory dataset (if any)
    if (m_DatasetList.size() != nb_new_DEM_opened)
    {
      const std::lock_guard<std::mutex> lock(demMutex);
      for (auto tls : m_tlses) {
        tls->CloseDEMVRTFile();
      }
      VSIUnlink(DEMHandler::DEM_DATASET_PATH);
    }

    // (Re-)Create the VRT from a list of opened dataset
    std::size_t const vrtSize = m_DatasetList.size();
    std::vector<GDALDatasetH> vrtDatasetList(vrtSize);
    for (std::size_t i = 0; i < vrtSize; i++)
    {
      vrtDatasetList[i] = m_DatasetList[i]->GetDataSet();
    }

    otbMsgDevMacro("Building VRT from " << vrtSize << " datasets: " << DEMHandler::DEM_DATASET_PATH);
    auto close_me = GDALBuildVRT(DEMHandler::DEM_DATASET_PATH, vrtSize, vrtDatasetList.data(),
        nullptr, nullptr, nullptr);
    // Need to close the dataset, so it is flushed into memory.
    GDALClose(close_me);

    const std::lock_guard<std::mutex> lock(demMutex);
    {
      // Check all requirements are satisfied to open de the VRT file.
      // If not, clear everything.
      DEMDetails::DatasetCache vrt(DEMHandler::DEM_DATASET_PATH);
      // +--> TODO: DatasetCache will also try to check the
      // projection... Can't we do better?
      if (!vrt)
      {
        // Note: DEMDirectory has been moved into m_DEMDirectories
        otbLogMacro(Warning, << "Input images from '"<< m_DEMDirectories.back() <<"' cannot be used to create a DEM VRT file, and as such they will be ignored.");
        m_DatasetList.clear();
        m_DEMDirectories.clear();
        return ;
      }
    } // make sure to close the vrt (thanks to RAII)

    for (auto tls : m_tlses)
    {
      if (! tls->OpenDEMVRTFile())
      {
        assert(false); // Should have been handled by the previous tests
                       // (HasGeoTransform + VRT validity tests)
        // Sometimes ungeoreferenced images are pushed into the VRT files,
        // but as they are not supported, no VRT is produced
        m_DatasetList.clear();
        m_DEMDirectories.clear();
        return ;
      }
    }

    if(!m_GeoidFilename.empty()) // still locked
    { // Should be done from only one thread, and ... once!!
      assert(! m_tlses.empty()); // Given OpenGeoidFile() implementation
                                 // (where an instance of DEMHandlerTLS
                                 // is fetched locally (to be sure there
                                 // is at least one), we know that
                                 // !m_GeoidFilename.empty() =>
                                 // !m_tlses.empty()
      m_tlses.front()->CreateShiftedDatasetOnce();
    }
  }

  Notify();
  otbMsgDevMacro(<<std::this_thread::get_id() << " § DEMHandler::OpenDEMDirectory() END");
}



bool HasInputProjection(GDALDataset const& gdalds)
{
#if GDAL_VERSION_NUM >= 3000000
  return gdalds.GetSpatialRef() && ! gdalds.GetSpatialRef()->IsEmpty();
#else
  return gdalds.GetProjectionRef()[0] != '\0';  // strlen(gdalds.GetProjectionRef()) != 0;
#endif
}

bool DEMHandlerTLS::OpenGeoidFile(const std::string& geoidFile)
{
  otbMsgDevMacro(<<std::this_thread::get_id() << " § DEMHandlerTLS::OpenGeoidFile("<<geoidFile<<") --> " << this);

  DEMDetails::DatasetCache gdalds(geoidFile);
  if (!gdalds)
  {
    otbLogMacro(Warning, << "Cannot open geoid file "<< geoidFile);
    return false;
  }

  if (! HasInputProjection(*gdalds))
  {
    otbLogMacro(Warning, << "Input geoid file "<< geoidFile << " will not be used because it has no input projection.");
    return false;
  }

  m_GeoidDS = std::move(gdalds);

  return true;
}

bool DEMHandler::OpenGeoidFile(std::string geoidFile)
{
  otbMsgDevMacro(<<std::this_thread::get_id() << " § DEMHandler::OpenGeoidFile("<<geoidFile<<")");

  // In case the geoid is not valid, we still try to open it for real,
  // even if the DEMHandlerTLS will not serve to anything...
  // tls will also serve to call CreateShiftedDatasetOnce()
  auto & tls = GetHandlerForCurrentThread();
  (void) tls;

  int nb_success = 0;
  {
    const std::lock_guard<std::mutex> lock(demMutex);
    for (auto tls : m_tlses)
    {
      nb_success += tls->OpenGeoidFile(geoidFile) ? 1 : 0;
      // if any is true, all should be!
    }
  } // <- release lock

  if (nb_success != 0)
  {
    m_GeoidFilename = move(geoidFile); // => parameter voluntary taken by value
    if (! m_DatasetList.empty())
    { // In that case, we could expect TLS.m_DEMDS to be non-null
      const std::lock_guard<std::mutex> lock(demMutex);
      assert(!m_tlses.empty()); //
      if (! m_DatasetList.empty())
      {
        // We just need to create it once, in any thread. The one created
        // locally being good enough...
        tls.CreateShiftedDatasetOnce();
      }
    }
  }
  Notify();
  return nb_success != 0;
}

void DEMHandlerTLS::CreateShiftedDatasetOnce() const
{
  otbMsgDevMacro(<<std::this_thread::get_id() << " § DEMHandlerTLS::CreateShiftedDatasetOnce() --> " << this);

  assert(m_DEMDS);
  assert(m_GeoidDS);
  // WIP : no data is not handled at the moment

  // Warp geoid dataset
  auto warpOptions = GDALCreateWarpOptions();;
  warpOptions->hSrcDS           = m_GeoidDS.get();
  //warpOptions->hDstDS         = m_Dataset;
  warpOptions->eResampleAlg     = GRA_Bilinear;
  warpOptions->eWorkingDataType = GDT_Float64;
  warpOptions->nBandCount       = 1;
  warpOptions->panSrcBands      = (int *) CPLMalloc(sizeof(int) * warpOptions->nBandCount );
  warpOptions->panSrcBands[0]   = 1;
  warpOptions->panDstBands      = (int *) CPLMalloc(sizeof(int) * warpOptions->nBandCount );
  warpOptions->panDstBands[0]   = 1;

  // Establish reprojection transformer.
  warpOptions->pTransformerArg =
    GDALCreateGenImgProjTransformer(
        m_GeoidDS.get(), GDALGetProjectionRef(m_GeoidDS.get()),
        m_DEMDS.get(),   GDALGetProjectionRef(m_DEMDS.get()),
        FALSE, 0.0, 1 );
  warpOptions->pfnTransformer = GDALGenImgProjTransform;

  auto ds = static_cast<GDALDataset *> (GDALCreateWarpedVRT(m_GeoidDS.get(),
        m_DEMDS->GetRasterXSize(),
        m_DEMDS->GetRasterYSize(),
        const_cast<double*>(m_DEMDS.getGeoTransform()), // not const-correct?
        warpOptions));

  ds->SetDescription(DEMHandler::DEM_WARPED_DATASET_PATH);
  GDALClose(ds);

  GDALDriver *poDriver = (GDALDriver *) GDALGetDriverByName( "VRT" );
  GDALDataset *poVRTDS;

  poVRTDS = poDriver->Create( DEMHandler::DEM_SHIFTED_DATASET_PATH, m_DEMDS->GetRasterXSize(), m_DEMDS->GetRasterYSize(), 0, GDT_Float64, NULL );

  poVRTDS->SetGeoTransform(const_cast<double*>(m_DEMDS.getGeoTransform()));

  poVRTDS->SetProjection(m_DEMDS->GetProjectionRef());

  char** derivedBandOptions = nullptr;

  derivedBandOptions = CSLAddNameValue(derivedBandOptions, "subclass", "VRTDerivedRasterBand");
  derivedBandOptions = CSLAddNameValue(derivedBandOptions, "PixelFunctionType", "sum");
  poVRTDS->AddBand(GDT_Float64, derivedBandOptions);

  GDALRasterBand *poBand = poVRTDS->GetRasterBand( 1 );

  // TODO use std string (and boost format ?) or stream
  char demVrtXml[10000];
  snprintf( demVrtXml, sizeof(demVrtXml),
      "<SimpleSource>"
      "  <SourceFilename>%s</SourceFilename>"
      "  <SourceBand>1</SourceBand>"
      "</SimpleSource>",
      DEMHandler::DEM_DATASET_PATH);

  poBand->SetMetadataItem( "source_0", demVrtXml, "new_vrt_sources" );

  char geoidVrtXml[10000];
  snprintf( geoidVrtXml, sizeof(geoidVrtXml),
      "<SimpleSource>"
      "  <SourceFilename>%s</SourceFilename>"
      "  <SourceBand>1</SourceBand>"
      "</SimpleSource>",
      DEMHandler::DEM_WARPED_DATASET_PATH);

  poBand->SetMetadataItem( "source_1", geoidVrtXml, "new_vrt_sources" );

  GDALClose(poVRTDS);
}

/*===============================[ DEMHandlerTLS data accessors ]============*/
boost::optional<double> DEMHandlerTLS::GetHeightAboveMSL(double lon, double lat, ElevationCache const& dem_elevation_cache) const
 {
   if (m_DEMDS)
   {
    return DEMDetails::GetDEMValue(lon, lat, m_DEMDS, dem_elevation_cache);
   }
   return boost::none;
 }

boost::optional<double> DEMHandlerTLS::GetGeoidHeight(double lon, double lat, ElevationCache const& geoid_elevation_cache) const
{
  if (m_GeoidDS)
  {
    return DEMDetails::GetDEMValue(lon, lat, m_GeoidDS, geoid_elevation_cache);
  }
  return boost::none;
}

double DEMHandlerTLS::GetHeightAboveEllipsoid(double lon, double lat, double defaultHeight, 
    ElevationCache const& dem_elevation_cache,
    ElevationCache const& geoid_elevation_cache) const
{
  boost::optional<double> DEMresult   = GetHeightAboveMSL(lon, lat, dem_elevation_cache);
  boost::optional<double> geoidResult = GetGeoidHeight(lon, lat, geoid_elevation_cache);

  if (DEMresult || geoidResult)
  {
    double result = 0.;
    if (DEMresult)
    {
      result += *DEMresult;
    }

    if (geoidResult)
    {
      result += *geoidResult;
    }
    return result;
  }
  else
    return defaultHeight;
}

double DEMHandler::GetHeightAboveEllipsoid(double lon, double lat)
{
  auto & tls = GetHandlerForCurrentThread();
  return tls.GetHeightAboveEllipsoid(lon, lat, m_DefaultHeightAboveEllipsoid,m_dem_elevation_cache, m_geoid_elevation_cache);
}
double DEMHandler::GetHeightAboveEllipsoid(const PointType& geoPoint)
{
  return GetHeightAboveEllipsoid(geoPoint[0], geoPoint[1]);
}

double DEMHandler::GetGeoidHeight(double lon, double lat) const
{
  auto & tls = GetHandlerForCurrentThread();
  return tls.GetGeoidHeight(lon, lat, m_geoid_elevation_cache).value_or(0);
}
double DEMHandler::GetGeoidHeight(const PointType& geoPoint) const
{
  return GetGeoidHeight(geoPoint[0], geoPoint[1]);
}

double DEMHandler::GetHeightAboveMSL(double lon, double lat) const
{
  auto & tls = GetHandlerForCurrentThread();
  return tls.GetHeightAboveMSL(lon, lat,m_dem_elevation_cache).value_or(0);
}
double DEMHandler::GetHeightAboveMSL(const PointType& geoPoint) const
{
  return GetHeightAboveMSL(geoPoint[0], geoPoint[1]);
}

std::size_t DEMHandler::GetDEMCount() const noexcept
{
  return m_DatasetList.size();
}

bool DEMHandler::IsValidDEMDirectory(const std::string& DEMDirectory) const
{
  for (const auto & filename : DEMDetails::GetFilesInDirectory(DEMDirectory))
  {
    // test if a driver can identify this dataset
    auto identifyDriverH = GDALIdentifyDriver(filename.c_str(), nullptr);
    if (identifyDriverH)
    {
      return true;
    }
  }

  return false;
}

std::string const& DEMHandler::GetDEMDirectory(std::size_t idx) const
{
  if (idx >= m_DEMDirectories.size())
  {
    std::ostringstream oss;
    oss << "Requested DEM directory " << idx
      << ", but only "<< m_DEMDirectories.size() << " have been set.";
    // TODO: std::out_of_range is a logic (i.e. a programming) error =>
    // use another exception type!
    throw std::out_of_range (oss.str());
  }

  return m_DEMDirectories[idx];
}

std::string const& DEMHandler::GetGeoidFile() const noexcept
{
  return m_GeoidFilename;
}

void DEMHandlerTLS::ClearElevationParameters()
{
  m_GeoidDS.release();
  m_DEMDS.release();
}

void DEMHandler::ClearElevationParameters()
{
  m_DefaultHeightAboveEllipsoid = 0.;
  m_GeoidFilename.clear();

  // This will call GDALClose on all datasets
  m_DatasetList.clear();
  {
    const std::lock_guard<std::mutex> lock(demMutex);
    for (auto tls : m_tlses) {
      tls->ClearElevationParameters();
    }
    m_DEMDirectories.clear();
  }
  // TODO: ¿ shouldn't we erase the files as well ?
  Notify();
}


void DEMHandler::SetDefaultHeightAboveEllipsoid(double height)
{
  m_DefaultHeightAboveEllipsoid = height;
  Notify();
}

double DEMHandler::GetDefaultHeightAboveEllipsoid() const noexcept
{
  return m_DefaultHeightAboveEllipsoid;
}

void DEMHandler::Notify() const
{
  for (const auto & observer: m_ObserverList)
  {
    observer->Update();
  }
};

double GetHeightAboveEllipsoid(DEMHandlerTLS& tls, double lon, double lat)
{
  auto const& dem_handler = DEMHandler::GetInstance();
  return tls.GetHeightAboveEllipsoid(lon, lat,
    dem_handler.GetDefaultHeightAboveEllipsoid(),
    dem_handler.GetDemElevationCache(),
    dem_handler.GetGeoidElevationCache());
}

double GetHeightAboveMSL      (DEMHandlerTLS& tls, double lon, double lat)
{ return tls.GetHeightAboveMSL(lon, lat, DEMHandler::GetInstance().GetDemElevationCache()).value_or(0); }

double GetGeoidHeight         (DEMHandlerTLS& tls, double lon, double lat)
{ return tls.GetGeoidHeight(lon, lat, DEMHandler::GetInstance().GetGeoidElevationCache()).value_or(0); }

double GetHeightAboveEllipsoid(DEMHandlerTLS& tls, itk::Point<double, 2> geoPoint)
{ return GetHeightAboveEllipsoid(tls, geoPoint[0], geoPoint[1]); }

double GetHeightAboveMSL      (DEMHandlerTLS& tls, itk::Point<double, 2> geoPoint)
{ return GetHeightAboveMSL(tls, geoPoint[0], geoPoint[1]); }

double GetGeoidHeight         (DEMHandlerTLS& tls, itk::Point<double, 2> geoPoint)
{ return GetGeoidHeight(tls, geoPoint[0], geoPoint[1]); }

bool ElevationCache::reset(int x_min, int x_max, int y_min, int y_max, GDALDataset & ds)
{
    int const dx = x_max - x_min + 1;
    int const dy = y_max - y_min + 1;
    m_buffer.resize(dx * dy);
    auto const err = ds.GetRasterBand(1)->RasterIO(
        GF_Read, x_min, y_min, dx, dy,
        m_buffer.data(), dx, dy, GDT_Float64,
        0, 0, nullptr);
    if (err != CE_None) {
      m_elevations = elevations_view_t();
      m_x0 = 0;
      m_y0 = 0;
      return false;
    }
    m_elevations = elevations_view_t(m_buffer.data(), narrow{}, dy, dx);
    m_x0 = x_min;
    m_y0 = y_min;
}

} // namespace otb
