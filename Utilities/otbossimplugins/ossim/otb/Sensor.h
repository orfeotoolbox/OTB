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

#ifndef Services_h
#define Services_h

#include <ossimPluginConstants.h>
#include <otb/JSDDateTime.h>

namespace ossimplugins
{


class SensorParams;
class PlatformPosition;
class JSDDateTime;

/**
 * @ingroup SARModel
 * @brief This class provides basic location services
 */
class OSSIM_PLUGINS_DLL Sensor
{
public:
  /**
   * @brief Constructor
   *
   * @remarks This constructor creates a copy of the sensor model (params and position)
   */
  Sensor(SensorParams* params, PlatformPosition* position);
  /**
   * @brief Destructor
   */
  virtual ~Sensor();

  /**
   * @brief This function is able to convert image coordinates into geodetic world coordinates using a sensor model
   *
   * @param distance : Slant range of the image point
   * @param time :    Azimuth time of the image point
   * @param height :  Altitude of the world point
   * @retval lon :    Longitude of the world point
   * @retval lat :    Latitude of the world point
   */
  virtual int ImageToWorld(double distance, JSDDateTime time, double height, double& lon, double& lat) const = 0;
protected:
  SensorParams * _params;
  PlatformPosition* _position;

private:
};

}
#endif
