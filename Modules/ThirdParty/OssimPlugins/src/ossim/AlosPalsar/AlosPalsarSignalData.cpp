/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2008-2010 by Centre for Remote Imaging, Sensing and Processing (CRISP)
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


#include <AlosPalsar/AlosPalsarSignalData.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimEndian.h>


namespace ossimplugins
{

AlosPalsarSignalData::AlosPalsarSignalData() : AlosPalsarRecord("sar_sig_dat_rec")
{
}

AlosPalsarSignalData::~AlosPalsarSignalData()
{
}

std::ostream& operator<<(std::ostream& os, const AlosPalsarSignalData& data)
{
  os << "_pulse_repetition_frequency:"
  << data._pulse_repetition_frequency << std::endl;
  os << "_slant_range_to_1st_data_sample:"
  << data._slant_range_to_1st_data_sample << std::endl;

  return os;
}

std::istream& operator>>(std::istream& is, AlosPalsarSignalData& data)
{
  ossim_uint16 tmpuint16;
  ossim_uint32 tmpuint32;
  ossimEndian oe;

  is.read(reinterpret_cast<char*>(&tmpuint32), 4);
  if (oe.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
  {
    oe.swap(tmpuint32);
  }
//  std::cout << "Data line number = " << tmpuint32 << std::endl;

  is.read(reinterpret_cast<char*>(&tmpuint32), 4);
  if (oe.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
  {
    oe.swap(tmpuint32);
  }
//  std::cout << "Data record index = " << tmpuint32 << std::endl;

  is.read(reinterpret_cast<char*>(&tmpuint32), 4);
  if (oe.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
  {
    oe.swap(tmpuint32);
  }
//  std::cout << "Actual count of left-fill pixels = " << tmpuint32 << std::endl;

  is.read(reinterpret_cast<char*>(&tmpuint32), 4);
  if (oe.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
  {
    oe.swap(tmpuint32);
  }
//  std::cout << "Actual count of data pixels = " << tmpuint32 << std::endl;

  is.read(reinterpret_cast<char*>(&tmpuint32), 4);
  if (oe.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
  {
    oe.swap(tmpuint32);
  }
//  std::cout << "Actual count of right-fill pixels = " << tmpuint32 << std::endl;

  is.read(reinterpret_cast<char*>(&tmpuint32), 4);
  if (oe.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
  {
    oe.swap(tmpuint32);
  }
//  std::cout << "Sensor parameters update flag = " << tmpuint32 << std::endl;

  is.read(reinterpret_cast<char*>(&tmpuint32), 4);
  if (oe.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
  {
    oe.swap(tmpuint32);
  }
//  std::cout << "Scene start year = " << tmpuint32 << std::endl;

  is.read(reinterpret_cast<char*>(&tmpuint32), 4);
  if (oe.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
  {
    oe.swap(tmpuint32);
  }
//  std::cout << "Scene start day of year = " << tmpuint32 << std::endl;

  is.read(reinterpret_cast<char*>(&tmpuint32), 4);
  if (oe.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
  {
    oe.swap(tmpuint32);
  }
//  std::cout << "Sensor acquisition milliseconds of day = " << tmpuint32 << std::endl;

  is.read(reinterpret_cast<char*>(&tmpuint16), 2);
  if (oe.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
  {
    oe.swap(tmpuint16);
  }
//  std::cout << "SAR channel indicator = " << tmpuint16 << std::endl;

  is.read(reinterpret_cast<char*>(&tmpuint16), 2);
  if (oe.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
  {
    oe.swap(tmpuint16);
  }
//  std::cout << "SAR channel code = " << tmpuint16 << std::endl;

  is.read(reinterpret_cast<char*>(&tmpuint16), 2);
  if (oe.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
  {
    oe.swap(tmpuint16);
  }
//  std::cout << "Transmitted polarization = " << tmpuint16 << std::endl;

  is.read(reinterpret_cast<char*>(&tmpuint16), 2);
  if (oe.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
  {
    oe.swap(tmpuint16);
  }
//  std::cout << "Received polarization = " << tmpuint16 << std::endl;

  is.read(reinterpret_cast<char*>(&tmpuint32), 4);
  if (oe.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
  {
    oe.swap(tmpuint32);
  }
//  std::cout << "PRF (mHz) = " << tmpuint32 << std::endl;
  data._pulse_repetition_frequency = static_cast<int>(tmpuint32);

  is.read(reinterpret_cast<char*>(&tmpuint32), 4);
  if (oe.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
  {
    oe.swap(tmpuint32);
  }
//  std::cout << "Scan ID for SCAN SAR mode = " << tmpuint32 << std::endl;

  is.read(reinterpret_cast<char*>(&tmpuint16), 2);
  if (oe.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
  {
    oe.swap(tmpuint16);
  }
//  std::cout << "Onboard range compressed flag = " << tmpuint16 << std::endl;

  is.seekg(50, std::ios_base::cur);

  is.read(reinterpret_cast<char*>(&tmpuint32), 4);
  if (oe.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
  {
    oe.swap(tmpuint32);
  }
//  std::cout << "Slant range to 1st data sample (m) = " << tmpuint32 << std::endl;
  data._slant_range_to_1st_data_sample = static_cast<int>(tmpuint32);

  is.read(reinterpret_cast<char*>(&tmpuint32), 4);
  if (oe.getSystemEndianType() == OSSIM_LITTLE_ENDIAN)
  {
    oe.swap(tmpuint32);
  }
//  std::cout << "Data record window position = " << tmpuint32 << std::endl;

//  std::cout << std::endl << "File pointer location = " << is.tellg() << std::endl;
//  std::cout << std::endl;

  return is;
}

AlosPalsarSignalData::AlosPalsarSignalData(const AlosPalsarSignalData& rhs):
    AlosPalsarRecord(rhs),
    _pulse_repetition_frequency(rhs._pulse_repetition_frequency),
    _slant_range_to_1st_data_sample(rhs._slant_range_to_1st_data_sample)
{
}

AlosPalsarSignalData& AlosPalsarSignalData::operator=(const AlosPalsarSignalData& rhs)
{
  _pulse_repetition_frequency = rhs._pulse_repetition_frequency;
  _slant_range_to_1st_data_sample = rhs._slant_range_to_1st_data_sample;
  return *this;
}
}
