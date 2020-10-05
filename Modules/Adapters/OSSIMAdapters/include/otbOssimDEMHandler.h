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

#ifndef otbOssimDEMHandler_h
#define otbOssimDEMHandler_h

#include <cstdio>

#include "itkIndent.h"
#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkPoint.h"

#include "OTBOSSIMAdaptersExport.h"
#include <string>

class ossimElevManager;

namespace otb
{
/** \class OssimDEMHandler
 *
 * \brief Single access point for DEM data retrieval
 *
 * This class is the single configuration and access point for
 * elevation handling in images projections and localization
 * functions. Since this class is a singleton, there is no New() method. The
 * OssimDEMHandler::Instance() method should be used instead.
 *
 * Please be aware that a proper instantiation and parameter setting
 * of this class is advised before any call to geometric filters or
 * functionalities. Ossim internal accesses to elevation are also
 * configured by this class and this will ensure consistency
 * throughout the library.
 *
 * The class allows configuring a directory containing DEM tiles
 * (DTED or SRTM supported) using the OpenDEMDirectory() method. The
 * OpenGeoidFile() method allows inputting a geoid file as well. Last,
 * a default height above ellipsoid can be set using the
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
 * Please note that OSSIM internal calls for sensor modeling use the
 * height above ellipsoid, and follow the same logic as the
 * GetHeightAboveEllipsoid() method.
 *
 * DEM directory can either contain DTED or SRTM formats.
 * \ingroup Images
 *
 *
 * \ingroup OTBOSSIMAdapters
 */

class OTBOSSIMAdapters_EXPORT OssimDEMHandler : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef OssimDEMHandler                    Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef itk::Point<double, 2> PointType;

  /** Retrieve the singleton instance */
  static Pointer Instance();

  /** Run-time type information (and related methods). */
  itkTypeMacro(OssimDEMHandler, Object);

  /** Try to open the DEM directory. */
  virtual void OpenDEMDirectory(const char* DEMDirectory);
  virtual void OpenDEMDirectory(const std::string& DEMDirectory);

  /** return true if the directory contain DEM */
  virtual bool IsValidDEMDirectory(const char* DEMDirectory);

  /**
   * \brief Open geoid file given its filename or throw an exception
   * if geoid-file could not be loaded.
   *
   * \param geoidFile The filename of the geoid-file.
   *
   * \return <code>true</code> if geoid file has been changed or
   * <code>false</code> if geod-file have been set before and could
   * not be changed.
   */
  virtual bool OpenGeoidFile(const char* geoidFile);

  /**
   * \brief Open geoid file given its filename or throw an exception
   * if geoid-file could not be loaded.
   *
   * \param geoidFile The filename of the geoid-file.
   *
   * \return <code>true</code> if geoid file has been changed or
   * <code>false</code> if geod-file have been set before and could
   * not be changed.
   */
  virtual bool OpenGeoidFile(const std::string& geoidFile);

  /** Compute the height above MSL(Mean Sea Level) of a geographic point. */
  virtual double GetHeightAboveMSL(double lon, double lat) const;
  virtual double GetHeightAboveMSL(const PointType& geoPoint) const;

  /** Compute the height above ellipsoid of a geographic point. */
  virtual double GetHeightAboveEllipsoid(double lon, double lat) const;
  virtual double GetHeightAboveEllipsoid(const PointType& geoPoint) const;

  /** Set the default height above ellipsoid in case no information is available*/
  virtual void SetDefaultHeightAboveEllipsoid(double h);

  /** Set the default height above ellipsoid in case no information is available*/
  double GetDefaultHeightAboveEllipsoid() const;

  /**
   * \return The number of elevation databases in the
   * <code>ossimElevManager</code>.
   */
  unsigned int GetDEMCount() const;

  /** Get DEM directory */
  std::string GetDEMDirectory(unsigned int idx = 0) const;

  /** Get Goid file */
  std::string GetGeoidFile() const;

  /**
   * \brief Remove all the ossimElevationDatabases from the
   * <code>ossimElevManager</code>.
   */
  void ClearDEMs();

protected:
  OssimDEMHandler();
  ~OssimDEMHandler() override
  {
  }

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  // Ossim does not allow retrieving the geoid file path
  // We therefore must keep it on our side
  std::string m_GeoidFile;

  // Ossim does not allow retrieving the default height above
  // ellipsoid We therefore must keep it on our side
  double m_DefaultHeightAboveEllipsoid;

  static Pointer m_Singleton;
};

} // namespace otb

#endif
