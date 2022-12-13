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

#ifndef otbDEMHandler_h
#define otbDEMHandler_h

/// Define to say: open the DEMs only once
#define OPEN_DEM_ONCE

#include "otbImage.h"
#if defined(OPEN_DEM_ONCE)
#  include "otbGDALDatasetWrapper.h"
#endif
#include <string>
#include <list>
#include <set>
#include <vector>

namespace otb
{

/** \class DEMObserverInterface
 *
 * \brief Observer design pattern to keep track of DEM configuration changes
 * \ingroup OTBIOGDAL
 */
class DEMObserverInterface
{
public:
  virtual void Update() = 0;
protected:
  DEMObserverInterface() = default;
  ~DEMObserverInterface() = default;
  DEMObserverInterface(DEMObserverInterface const&) = delete;
  DEMObserverInterface& operator=(DEMObserverInterface const&) = delete;
};

/** \class DEMSubjectInterface
 *
 * \brief Observer design pattern to keep track of DEM configuration changes
 * \ingroup OTBIOGDAL
 */
class DEMSubjectInterface
{
public:
  virtual void AttachObserver(DEMObserverInterface *observer) = 0;
  virtual void DetachObserver(DEMObserverInterface *observer) = 0;
  virtual void Notify() const = 0;
protected:
  DEMSubjectInterface() = default;
  ~DEMSubjectInterface() = default;
  DEMSubjectInterface(DEMSubjectInterface const&) = delete;
  DEMSubjectInterface& operator=(DEMSubjectInterface const&) = delete;
};

class DEMHandlerTLS;

/** \class DEMHandler
 *
 * \brief Single access point for DEM data retrieval
 *
 * This class is the single configuration and access point for
 * elevation handling in images projections and localization
 * functions. Since this class is a singleton, there is no New() method. The
 * DEMHandler::GetInstance() method should be used instead.
 *
 * Please be aware that a proper instantiation and parameter setting
 * of this class is advised before any call to geometric filters or
 * functionalities.
 *
 * The class allows configuring a directory containing DEM tiles
 * (raster tiles will be opened by GDAL drivers) using the OpenDEMDirectory()
 * method. the OpenGeoidFile() method allows inputting a geoid file as well.
 * Last, a default height above ellipsoid can be set using the
 * SetDefaultHeightAboveEllipsoid() method.
 *
 * The class allows retrieving either height above ellipsoid or
 * height above Mean Sea Level (MSL).
 *
 * Here is the complete description of both methods output depending
 * on the class configuration for the SRTM DEM (in the following, no
 * SRTM means DEMDirectory not set, or no coverage for point, or
 * srtm_value is no_data).
 *
 * GetHeightAboveEllipsoid():
 * - SRTM and geoid both available: srtm_value + geoid_offset
 * - No SRTM but geoid available: geoid_offset
 * - SRTM available, but no geoid: srtm_value
 * - No SRTM and no geoid available: default height above ellipsoid
 *
 * GetHeightAboveMSL():
 * - SRTM and geoid both available: srtm_value
 * - No SRTM but geoid available: 0
 * - SRTM available, but no geoid: srtm_value
 * - No SRTM and no geoid available: 0
 *
 * \ingroup OTBIOGDAL
 */
class DEMHandler : public DEMSubjectInterface
{
public:
  using Self      = DEMHandler;
  using PointType = itk::Point<double, 2>;

  /** Retrieve the singleton instance */
  static DEMHandler & GetInstance();

  /** Open all raster in the directory.
   * \param[in] DEMDirectory input directory
   */
  void OpenDEMDirectory(std::string DEMDirectory);

  /** Try to open the DEM directory.
   * \param[in] path input path
   */
  void OpenDEMFile(std::string path);

  /** Tells whether the directory contains a raster
   * \param[in] DEMDirectory input directory
   */
  bool IsValidDEMDirectory(const std::string& DEMDirectory) const;

  /** Try to open a geoid file
   * \param[in] geoidFile input geoid path
   */
  bool OpenGeoidFile(std::string geoidFile);

  /** Return the height above the ellipsoid :
   * - SRTM and geoid both available: srtm_value + geoid_offset
   * - No SRTM but geoid available: geoid_offset
   * - SRTM available, but no geoid: srtm_value
   * - No SRTM and no geoid available: default height above ellipsoid
   * \param[in] lon input longitude
   * \param[in] lat input latitude
   * \return height above ellipsoid
  */
  double GetHeightAboveEllipsoid(double lon, double lat) const;

  double GetHeightAboveEllipsoid(const PointType& geoPoint) const;

  /** Return the height above the mean sea level :
   * - SRTM and geoid both available: srtm_value
   * - No SRTM but geoid available: 0
   * - SRTM available, but no geoid: srtm_value
   * - No SRTM and no geoid available: 0
   * \param[in] lon input longitude
   * \param[in] lat input latitude
   * \return height above mean sea level
  */
  double GetHeightAboveMSL(double lon, double lat) const;

  double GetHeightAboveMSL(const PointType& geoPoint) const;

  double GetGeoidHeight(double lon, double lat) const;
  double GetGeoidHeight(const PointType& geoPoint) const;

  /** Return the number of DEM opened */
  unsigned int GetDEMCount() const;

  double GetDefaultHeightAboveEllipsoid() const;

  void SetDefaultHeightAboveEllipsoid(double height);

  /** Get DEM directory
   * \param[in] idx directory index
   * \return the DEM directory corresponding to index idx
   */
  std::string const& GetDEMDirectory(unsigned int idx = 0) const;

  /** Get Geoid file */
  std::string const& GetGeoidFile() const;

  /** Clear the DEM list and geoid filename, close all elevation datasets
   * and reset the default height above ellipsoid */
  void ClearElevationParameters();

  /** Add an element to the current list of observers.
   * The obsever will be updated whenever the DEM configuration is
   * modified
   */
  void AttachObserver(DEMObserverInterface *observer) override {m_ObserverList.push_back(observer);};

  /** Remove an element of the current list of observers. */
  void DetachObserver(DEMObserverInterface *observer) override {m_ObserverList.remove(observer);};

  /** Update all observers */
  void Notify() const override;

  /** Path to the in-memory vrt */
  static constexpr char const* DEM_DATASET_PATH         = "/vsimem/otb_dem_dataset.vrt";
  static constexpr char const* DEM_WARPED_DATASET_PATH  = "/vsimem/otb_dem_warped_dataset.vrt";
  static constexpr char const* DEM_SHIFTED_DATASET_PATH = "/vsimem/otb_dem_shifted_dataset.vrt";

  DEMHandlerTLS const& GetHandlerForCurrentThread() const;
protected:
  DEMHandler();
  ~DEMHandler();

  friend class DEMHandlerTLS;
  void RegisterTLS(DEMHandlerTLS* tls);
  void UnregisterTLS(DEMHandlerTLS* tls);

private:
  /** Internal accessor to the current (thread-wise) DEM handler.
   * If no handler has been attributed to the current thread, one will be
   * picked from the `m_tlses` pool, or created on the fly thanks to
   * `DoFetchOrCreateHandler`.
   *
   * This function is just a facade that caches the static thread local
   * `DEMHandlerTLS` attributed to the current thread.
   */
  DEMHandlerTLS & DoGetHandlerForCurrentThread() const;

  DEMHandler(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Default height above elliposid, used when no DEM or geoid height is available. */
  double m_DefaultHeightAboveEllipsoid;

  /** List of the DEM directories currently opened */
  std::vector<std::string> m_DEMDirectories;

#if defined(OPEN_DEM_ONCE)
  /** List of RAII capsules on all opened DEM datasets for memory management */
  std::vector<otb::GDALDatasetWrapper::Pointer> m_DatasetList;
#endif

  /** Filename of the current geoid */
  std::string m_GeoidFilename;

  /** Observers on the DEM */
  std::list<DEMObserverInterface *> m_ObserverList;

  std::set<DEMHandlerTLS*> m_tlses;
};


double GetHeightAboveEllipsoid(DEMHandlerTLS const&, double lon, double lat);
double GetHeightAboveMSL      (DEMHandlerTLS const&, double lon, double lat);
double GetGeoidHeight         (DEMHandlerTLS const&, double lon, double lat);
double GetHeightAboveEllipsoid(DEMHandlerTLS const&, itk::Point<double, 2> geoPoint);
double GetHeightAboveMSL      (DEMHandlerTLS const&, itk::Point<double, 2> geoPoint);
double GetGeoidHeight         (DEMHandlerTLS const&, itk::Point<double, 2> geoPoint);

}
#endif
