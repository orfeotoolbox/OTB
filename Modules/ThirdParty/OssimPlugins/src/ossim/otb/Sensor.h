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
#include "otb/SensorParams.h"
#include "otb/PlatformPosition.h"

namespace ossimplugins
{
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
  Sensor(SensorParams const& params, PlatformPosition const& position);
  /**
   * @brief Destructor
   */
  virtual ~Sensor();

  /**
   * @brief This function is able to convert image coordinates into geodetic world coordinates using a sensor model
   *
   * @param[in] distance Slant range of the image point
   * @param[in] time     Azimuth time of the image point
   * @param[in] height  Altitude of the world point
   * @param[out] lon    Longitude of the world point
   * @param[out] lat    Latitude of the world point
   */
  virtual int ImageToWorld(double distance, JSDDateTime time, double height, double& lon, double& lat) const = 0;

protected:
  SensorParams     m_params;
  PlatformPosition m_position;

private:
  Sensor(Sensor const&);
  Sensor& operator=(Sensor const&);
};

}
#endif
