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


#ifndef GalileanEphemeride_h
#define GalileanEphemeride_h

#include <ossimPluginConstants.h>
#include <otb/Ephemeris.h>

namespace ossimplugins
{


class GeographicEphemeris;
/**
 * @ingroup Ephemeris
 * @brief This class represents an ephemeris in Galilean coordinates system
 */
class OSSIM_PLUGINS_DLL GalileanEphemeris : public Ephemeris
{
public:
  /**
   * @brief Constructor
   */
  GalileanEphemeris();

  /**
    * @brief Destructor
    */
  ~GalileanEphemeris();

  /**
   * @brief Constructor with initialisations
   */
  GalileanEphemeris(JSDDateTime date, double pos[3], double speed[3]);

  /**
   * @brief Copy constructor
   */
  GalileanEphemeris(const GalileanEphemeris& rhs);

  GalileanEphemeris(GeographicEphemeris& rhs);
  /**
   * @brief Affectation operator
   */
  GalileanEphemeris& operator=(const GalileanEphemeris& rhs);

  /**
   * @brief This function converts the current ephemeris in the Geographic coordinates system
   */
  void ToGeographic(GeographicEphemeris* vGeo);
  void ToGeographic(double greenwich,GeographicEphemeris* vGeo);

  operator GeographicEphemeris();

  /**
   * @brief This function creates a copy of the current class
   */
  virtual Ephemeris* Clone()
  {
    return new GalileanEphemeris(*this);
  };
protected:
  int p2nutt(int newcmb, double greenwich, double day, double p[], double pd[] );
private:
};
}

#endif
