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


#ifndef AntennaElevationPatterns_h
#define AntennaElevationPatterns_h

#include <EnvisatAsar/EnvisatAsarRecord.h>
#include <string>

namespace ossimplugins
{
/**
 * @ingroup AntennaElevationPatternsRecord
 * @brief This class is able to read the ASAR AntennaElevationPatterns record
 */
class AntennaElevationPatterns : public EnvisatAsarRecord
{
public:
  /**
   * @brief Constructor
   */
  AntennaElevationPatterns();

  /**
   * @brief Destructor
   */
  virtual ~AntennaElevationPatterns();

  /**
   * @brief This function writes the AntennaElevationPatterns in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const AntennaElevationPatterns& data);

  /**
   * @brief This function reads a AntennaElevationPatterns from a stream
   */
  friend std::istream& operator>>(std::istream& is, AntennaElevationPatterns& data);

  /**
   * @brief Copy constructor
   */
  AntennaElevationPatterns(const AntennaElevationPatterns& rhs);

  /**
   * @brief Copy operator
   */
  AntennaElevationPatterns& operator=(const AntennaElevationPatterns& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  EnvisatAsarRecord* Instantiate()
  {
    return new AntennaElevationPatterns();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  EnvisatAsarRecord* Clone()
  {
    return new AntennaElevationPatterns(*this);
  };

  /**
   * @brief Reads the class data from a stream
   */
  void Read(std::istream& is)
  {
    is>>*this;
  };

  /**
   * @brief Writes the class to a stream
   */
  void Write(std::ostream& os)
  {
    os<<*this;
  };


  /**
  * @brief zero_doppler_time
  */
  std::string   get_zero_doppler_time()
  {
    return _zero_doppler_time;
  };
  /**
  * @brief attach_flag
  */
  bool   get_attach_flag()
  {
    return _attach_flag;
  };
  /**
  * @brief swath
  */
  std::string    get_swath()
  {
    return _swath;
  };
  /**
  * @brief Elevation Pattern
  */
  float *    get_elevation_pattern()
  {
    return _elevation_pattern;
  };


  /**
  * @brief zero_doppler_time
  */
  std::string   _zero_doppler_time;
  /**
  * @brief attach_flag
  */
  bool   _attach_flag;
  /**
  * @brief swath
  */
  std::string   _swath;
  /**
  * @brief Elevation Pattern
  */
  float  _elevation_pattern[33];


private:

};
}
#endif


