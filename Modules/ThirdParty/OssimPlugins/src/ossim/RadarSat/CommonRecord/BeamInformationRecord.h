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


#ifndef BeamInformationRecord_h
#define BeamInformationRecord_h


#include <RadarSat/RadarSatRecord.h>
#include <string>

namespace ossimplugins
{
/**
 * @ingroup ProcessingParametersRecord
 * @brief This class is able to read a Beam information record
 */
class BeamInformationRecord
{
public:
  /**
   * @brief Constructor
   */
  BeamInformationRecord();

  /**
   * @brief Destructor
   */
  ~BeamInformationRecord();

  /**
   * @brief Copy constructor
   */
  BeamInformationRecord(const BeamInformationRecord& rhs);

  /**
   * @brief Copy operator
   */
  BeamInformationRecord& operator=(const BeamInformationRecord& rhs);

  /**
   * @brief This function writes the BeamInformationRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const BeamInformationRecord& data);

  /**
   * @brief This function reads a BeamInformationRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, BeamInformationRecord& data);

  /**
   * @brief Beam type
   */
  std::string   get_beam_type()
  {
    return _beam_type;
  };

  /**
   * @brief Elevation beam look angle source
   */
  std::string   get_beam_look_src()
  {
    return _beam_look_src;
  };

    /**
   * @brief Applied elevation beam look angle (deg)
   */
  double   get_beam_look_ang()
  {
    return _beam_look_ang;
  };

  /**
   * @brief Actual PRF (Hz)
   */
  double   get_prf()
  {
    return _prf;
  };
protected:

  /**
   * @brief Beam type
   */
  std::string  _beam_type;

  /**
   * @brief Elevation beam look angle source
   */
  std::string  _beam_look_src;

    /**
   * @brief Applied elevation beam look angle (deg)
   */
  double   _beam_look_ang;

  /**
   * @brief Actual PRF (Hz)
   */
  double   _prf;
private:

};
}
#endif
