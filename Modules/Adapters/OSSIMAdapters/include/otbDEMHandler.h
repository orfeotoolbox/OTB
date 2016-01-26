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
#ifndef __otbDEMHandler_h
#define __otbDEMHandler_h

#include <iostream>
#include <cstdio>

#include "itkIndent.h"
#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkPoint.h"

class ossimElevManager;

namespace otb
{
/** \class DEMHandler
 *
 * \brief Single access point for DEM data retrieval
 *
 * This class is the single configuration and access point for
 * elevation handling in images projections and localization
 * functions. Since this class is a singleton, the New() method is
 * deprecated and will be removed in future release. Please use the
 * DEMHandler::Instance() method instead.
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

class ITK_EXPORT DEMHandler : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef DEMHandler                    Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef itk::Point<double, 2> PointType;

  /** Retrieve the singleton instance */
  static Pointer Instance();

  /** Run-time type information (and related methods). */
  itkTypeMacro(DEMHandler, Object);

  /** Try to open the DEM directory. */
  virtual void OpenDEMDirectory(const char* DEMDirectory);
  virtual void OpenDEMDirectory(const std::string& DEMDirectory);

  /** return true if the directoty contain DEM */
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
  DEMHandler();
  virtual ~DEMHandler() {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

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
