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


#ifndef BeamPixelCountRecord_h
#define BeamPixelCountRecord_h


#include <RadarSat/RadarSatRecord.h>
#include <string>

namespace ossimplugins
{
/**
 * @ingroup ProcessingParametersRecord
 * @brief This class is able to read a Beam pixel count record
 */
class BeamPixelCountRecord
{
public:
  /**
   * @brief Constructor
   */
  BeamPixelCountRecord();

  /**
   * @brief Destructor
   */
  ~BeamPixelCountRecord();

  /**
   * @brief Copy constructor
   */
  BeamPixelCountRecord(const BeamPixelCountRecord& rhs);

  /**
   * @brief Copy operator
   */
  BeamPixelCountRecord& operator=(const BeamPixelCountRecord& rhs);

  /**
   * @brief This function writes the BeamPixelCountRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const BeamPixelCountRecord& data);

  /**
   * @brief This function reads a BeamPixelCountRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, BeamPixelCountRecord& data);

  /**
   * @brief Pixel count update date/time
   */
  std::string   get_pix_update()
  {
    return _pix_update;
  };

    /**
   * @brief Count of image pixels in beams
   */
  int*   get_n_pix()
  {
    return _n_pix;
  };
protected:

  /**
   * @brief Pixel count update date/time
   */
  std::string  _pix_update;
    /**
   * @brief Count of image pixels in beams
   */
  int   _n_pix[4];
private:

};
}
#endif
