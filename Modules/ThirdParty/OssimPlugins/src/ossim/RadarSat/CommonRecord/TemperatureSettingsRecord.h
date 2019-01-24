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


#ifndef TemperatureSettingsRecord_h
#define TemperatureSettingsRecord_h

#include <RadarSat/RadarSatRecord.h>

namespace ossimplugins
{
/**
 * @ingroup ProcessingParametersRecord
 * @brief This class is able to read a Temperature Settings record
 */
class TemperatureSettingsRecord
{
public:
  /**
   * @brief Constructor
   */
  TemperatureSettingsRecord();

  /**
   * @brief Destructor
   */
  ~TemperatureSettingsRecord();

  /**
   * @brief Copy constructor
   */
  TemperatureSettingsRecord(const TemperatureSettingsRecord& rhs);

  /**
   * @brief Copy operator
   */
  TemperatureSettingsRecord& operator=(const TemperatureSettingsRecord& rhs);

  /**
   * @brief This function writes the TemperatureSettingsRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const TemperatureSettingsRecord& data);

  /**
   * @brief This function reads a TemperatureSettingsRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, TemperatureSettingsRecord& data);

  /**
   * @brief Temperature settings
   */
  int*   get_temp_set()
  {
    return _temp_set;
  };

protected:

  /**
   * @brief Temperature settings
   */
  int _temp_set[4];
private:

};
}
#endif
