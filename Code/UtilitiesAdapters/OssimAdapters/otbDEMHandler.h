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

// This macro is here to replace deprecated elevation setup in all
// classes at once
#define otbLegacyElevationMacro()                                       \
  itkLegacyMacro(void SetDEMDirectory(const std::string & dem)          \
  {                                                                     \
    otb::DEMHandler::Instance()->OpenDEMDirectory(dem);                 \
  })                                                                    \
  itkLegacyMacro(std::string GetDEMDirectory() const                    \
  {                                                                     \
    return otb::DEMHandler::Instance()->GetDEMDirectory();              \
  })                                                                    \
  itkLegacyMacro(void SetGeoidFile(const std::string & geoid)           \
  {                                                                     \
    otb::DEMHandler::Instance()->OpenGeoidFile(geoid);                  \
  })                                                                    \
  itkLegacyMacro(std::string GetGeoidFile() const                       \
  {                                                                     \
    return otb::DEMHandler::Instance()->GetGeoidFile();                 \
  })                                                                    \
  itkLegacyMacro(void SetAverageElevation(double elevation)             \
  {                                                                     \
  otb::DEMHandler::Instance()->SetDefaultHeightAboveEllipsoid(elevation);  \
  })                                                                    \
  itkLegacyMacro(double GetAverageElevation() const                     \
  {                                                                     \
  return otb::DEMHandler::Instance()->GetDefaultHeightAboveEllipsoid(); \
  })                                                                    \

namespace otb
{
/** \class DEMHandler
 *
 * \brief Class for Reading a DEM data
 * 
 * This class is the single configuration and access point for
 * elevation handling in images projections and localisation functions.
 *
 * It is based on ossimElevManager.
 * It allows to obtain height above Ellipsoid or MSL(Mean Sea Level)
 * of a geographic point.
 *
 * It handles both DTED and SRTM formats.
 * \ingroup Images
 *
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

  /** Method for creation through the object factory. 
   *  \deprecated DEMHandler is now a singleton class, use Instance method instead.
   */
  itkLegacyMacro(static Self * New());

  /** Retrieve the singleton instance */
  static Pointer Instance();

  /** Run-time type information (and related methods). */
  itkTypeMacro(DEMHandler, Object);

  /** Try to open the DEM directory. */
  virtual void OpenDEMDirectory(const char* DEMDirectory);
  virtual void OpenDEMDirectory(const std::string& DEMDirectory);

  /** return true if the directoty contain DEM */
  virtual bool IsValidDEMDirectory(const char* DEMDirectory);

  /** Open geoid file. */
  virtual void OpenGeoidFile(const char* geoidFile);
  virtual void OpenGeoidFile(const std::string& geoidFile);

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


  /** Get DEM directory */
  std::string GetDEMDirectory(unsigned int idx = 0) const;

  /** Get Goid file */
  std::string GetGeoidFile() const;

protected:
  DEMHandler();
  virtual ~DEMHandler() {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  ossimElevManager* m_ElevManager;

  // Ossim does not allow to retrieve the geoid file path
  // We therefore must keep it on our side
  std::string m_GeoidFile;

  // Ossim does not allow to retrieve the default height above
  // ellipsoid We therefore must keep it on our side
  double m_DefaultHeightAboveEllipsoid;

  static Pointer m_Singleton;

};

} // namespace otb

#endif
