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


#ifndef RadarSatRecord_h
#define RadarSatRecord_h


#include <RadarSat/RadarSatRecordHeader.h>
#include <cstdlib>
#include <string>

namespace ossimplugins
{
/**
 * @ingroup RadarSat
 * @brief This class is the base class of all the record classes
 */
class RadarSatRecord
{
public:
  /**
   * @brief Constructor
   * @param mnemonic Name of the record
   */
  RadarSatRecord(std::string mnemonic);
  /**
   * @brief Destructor
   */
  virtual ~RadarSatRecord();

  /**
   * @brief Copy constructor
   */
  RadarSatRecord(const RadarSatRecord& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  virtual RadarSatRecord* Instantiate() =0;

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  virtual RadarSatRecord* Clone()=0;

  /**
   * @brief Reads the class data from a stream
   */
  virtual void Read(std::istream& is) =0;

  /**
   * @brief Writes the class to a stream
   */
  virtual void Write(std::ostream& os)=0;

  std::string get_mnemonic()
  {
    return _mnemonic;
  };

protected:

  std::string _mnemonic;

private:
};
}
#endif
