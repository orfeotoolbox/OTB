/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#ifndef Services_h
#define Services_h

#include <ossimPluginConstants.h>

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
