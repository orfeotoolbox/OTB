/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include "otbImage.h"
#include "otbGDALDriverManagerWrapper.h"

namespace otb
{



/** \class DEMObserverInterface
 *
 * \brief Observer design pattern to keep track of DEM configuration changes
 * \ingroup OTBIOGDAL
 */
class DEMObserverInterface {
 public:
  virtual ~DEMObserverInterface() = default;
  virtual void Update() = 0;
};

/** \class DEMSubjectInterface
 *
 * \brief Observer design pattern to keep track of DEM configuration changes
 * \ingroup OTBIOGDAL
 */
class DEMSubjectInterface {
 public:
  virtual ~DEMSubjectInterface() = default;
  virtual void AttachObserver(DEMObserverInterface *observer) = 0;
  virtual void DetachObserver(DEMObserverInterface *observer) = 0;
  virtual void Notify() const = 0;
};


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
  using Self =          DEMHandler;
  using PointType =     itk::Point<double, 2>;

  /** Retrieve the singleton instance */
  static DEMHandler & GetInstance();

  /** Try to open the DEM directory. 
  * \param path input path
  */
  void OpenDEMFile(const std::string & path);

  /** Open all raster in the directory. 
  * \param DEMDirectory input directory
  */
  void OpenDEMDirectory(const std::string& DEMDirectory);

  /** Tells whether the directory contains a raster
  * \param DEMDirectory input directory
  */
  bool IsValidDEMDirectory(const std::string& DEMDirectory) const;

  /** Try to open a geoid file 
  * \param geoidFile input geoid path
  */
  bool OpenGeoidFile(const std::string& geoidFile);

  /** Return the height above the ellipsoid :
   * - SRTM and geoid both available: srtm_value + geoid_offset
   * - No SRTM but geoid available: geoid_offset
   * - SRTM available, but no geoid: srtm_value
   * - No SRTM and no geoid available: default height above ellipsoid
   * \param lon input longitude
   * \param lat input latitude
   * \return height above ellipsoid
  */
  double GetHeightAboveEllipsoid(double lon, double lat) const;

  double GetHeightAboveEllipsoid(const PointType& geoPoint) const;
 
  /** Return the height above the mean sea level :
   * - SRTM and geoid both available: srtm_value
   * - No SRTM but geoid available: 0
   * - SRTM available, but no geoid: srtm_value
   * - No SRTM and no geoid available: 0
   * \param lon input longitude
   * \param lat input latitude
   * \return height above mean sea level
  */
  double GetHeightAboveMSL(double lon, double lat) const;

  double GetHeightAboveMSL(const PointType& geoPoint) const;

  /** Return the number of DEM opened */
  unsigned int GetDEMCount() const;
  
  double GetDefaultHeightAboveEllipsoid() const;

  void SetDefaultHeightAboveEllipsoid(double height);

  /** Get DEM directory 
   * \param idx directory index
   * \return the DEM directory corresponding to index idx
   */
  std::string GetDEMDirectory(unsigned int idx = 0) const;

  /** Get Geoid file */
  std::string GetGeoidFile() const;

  /** Clear the DEM list and close all DEM datasets */
  void ClearDEMs();
  
  /** Add an element to the current list of observers. The obsever will be updated whenever the DEM configuration
  is modified*/
  void AttachObserver(DEMObserverInterface *observer) override {m_ObserverList.push_back(observer);};
  
  /** Remove an element of the current list of observers. */
  void DetachObserver(DEMObserverInterface *observer) override {m_ObserverList.remove(observer);};
  
  /** Update all observers */
  void Notify() const override;

  /** Path to the in-memory vrt */
  const std::string DEM_DATASET_PATH = "/vsimem/otb_dem_dataset.vrt";
  const std::string DEM_WARPED_DATASET_PATH = "/vsimem/otb_dem_warped_dataset.vrt";
  const std::string DEM_SHIFTED_DATASET_PATH = "/vsimem/otb_dem_shifted_dataset.vrt";

protected: 
  DEMHandler(); 

  ~DEMHandler();


private:
  DEMHandler(const Self&) = delete;
  void operator=(const Self&) = delete;  

  void CreateShiftedDataset();

  /** List of RAII capsules on all opened DEM datasets for memory management */
  std::vector<otb::GDALDatasetWrapper::Pointer> m_DatasetList;
  
  /** Pointer to the DEM dataset */
  GDALDataset * m_Dataset;

  /** Pointer to the geoid dataset */
  GDALDataset* m_GeoidDS;
  
  /** Pointer to the sifted dataset */
  GDALDataset* m_ShiftedDS;
  
  /** Default height above elliposid, used when no DEM or geoid height is available. */
  double m_DefaultHeightAboveEllipsoid;

  /** List of the DEM directories currently opened */
  std::vector<std::string> m_DEMDirectories;

  /** Filename of the current geoid */
  std::string m_GeoidFilename;

  /** Observers on the DEM */
  std::list<DEMObserverInterface *> m_ObserverList;
};

}
#endif
