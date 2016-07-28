//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef SarSensor_h
#define SarSensor_h

#include <ossimPluginConstants.h>
#include "otb/Sensor.h"

namespace ossimplugins
{
class GeographicEphemeris;
class RectangularCoordinate;
class JSDDateTime;
/**
 * @ingroup SARModel
 * @brief This class provides basic location services for SAR sensors
 */
class OSSIM_PLUGINS_DLL SarSensor : Sensor
{
public:
  /**
   * @brief Constructor
   *
   * @remarks This constructor creates a copy of params and position
   */
  SarSensor(SensorParams const& params, PlatformPosition const& position);
  /**
   * @brief Destructor
   */
  virtual ~SarSensor();

  /**
   * @brief This function is able to convert image coordinates into geodetic world coordinates using a geometric SAR sensor model
   *
   * @param[in] distance Slant range of the image point
   * @param[in] time    Azimuth time of the image point
   * @param[in] height  Altitude of the world point
   * @param[out] lon    Longitude of the world point
   * @param[out] lat    Latitude of the world point
   *
   * @note the doppler frequency is set to zero in this implementation
   */
  virtual int ImageToWorld(double distance, JSDDateTime time, double height, double& lon, double& lat) const;
private:

  /**
   * @brief This function is able to convert image coordinates into rectangular world coordinates
   */
  int localisationSAR ( GeographicEphemeris posSpeed , double lambda ,
                        double dist , double fDop , int sensVisee ,
                        double equRadius , double polRadius ,
                        double h , RectangularCoordinate* cart ) const;
};

}

#endif
