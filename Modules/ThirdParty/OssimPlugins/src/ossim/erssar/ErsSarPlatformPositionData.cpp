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


#include "erssar/ErsSarPlatformPositionData.h"


namespace ossimplugins
{

ErsSarPlatformPositionData::ErsSarPlatformPositionData() : ErsSarRecord("pos_data_rec")
{
}

ErsSarPlatformPositionData::~ErsSarPlatformPositionData()
{
}

std::ostream& operator<<(std::ostream& os, const ErsSarPlatformPositionData& data)
{
  os << "orbit_ele_desg:" << data._orbit_ele_desg << std::endl;

  for (int i = 0; i < 6; i++)
  {
    os << "orbit_ele[" << i << "]:" << data._orbit_ele[i] << std::endl;
  }

  os << "ndata:" << data._ndata << std::endl;

  os << "year:" << data._year << std::endl;

  os << "month:" << data._month << std::endl;

  os << "day:" << data._day << std::endl;

  os << "gmt_day:" << data._gmt_day << std::endl;

  os << "gmt_sec:" << data._gmt_sec << std::endl;

  os << "data_int:" << data._data_int << std::endl;

  os << "ref_coord:" << data._ref_coord << std::endl;

  os << "hr_angle:" << data._hr_angle << std::endl;

  os << "alt_poserr:" << data._alt_poserr << std::endl;

  os << "crt_poserr:" << data._crt_poserr << std::endl;

  os << "rad_poserr:" << data._rad_poserr << std::endl;

  os << "alt_velerr:" << data._alt_velerr << std::endl;

  os << "crt_velerr:" << data._crt_velerr << std::endl;

  os << "rad_velerr:" << data._rad_velerr << std::endl;

  for (int i = 0; i < 64; i++)
  {
    os << "pos_vect[" << i << "]:" << data._pos_vect[i] << std::endl;
  }

  return os;
}

std::istream& operator>>(std::istream& is, ErsSarPlatformPositionData& data)
{
  char buf[1207];
  buf[1206] = '\0';

  is.read(buf, 32);
  buf[32] = '\0';
  data._orbit_ele_desg = buf;

  for (int i = 0; i < 6; i++)
  {
    is.read(buf, 16);
    buf[16] = '\0';
    data._orbit_ele[i] = atof(buf);
  }

  is.read(buf, 4);
  buf[4] = '\0';
  data._ndata = atoi(buf);

  is.read(buf, 4);
  buf[4] = '\0';
  data._year = atoi(buf);

  is.read(buf, 4);
  buf[4] = '\0';
  data._month = atoi(buf);

  is.read(buf, 4);
  buf[4] = '\0';
  data._day = atoi(buf);

  is.read(buf, 4);
  buf[4] = '\0';
  data._gmt_day = atoi(buf);

  is.read(buf, 22);
  buf[22] = '\0';
  data._gmt_sec = atof(buf);

  is.read(buf, 22);
  buf[22] = '\0';
  data._data_int = atof(buf);

  is.read(buf, 64);
  buf[64] = '\0';
  data._ref_coord = buf;

  is.read(buf, 22);
  buf[22] = '\0';
  data._hr_angle = atof(buf);

  is.read(buf, 16);
  buf[16] = '\0';
  data._alt_poserr = atof(buf);

  is.read(buf, 16);
  buf[16] = '\0';
  data._crt_poserr = atof(buf);

  is.read(buf, 16);
  buf[16] = '\0';
  data._rad_poserr = atof(buf);

  is.read(buf, 16);
  buf[16] = '\0';
  data._alt_velerr = atof(buf);

  is.read(buf, 16);
  buf[16] = '\0';
  data._crt_velerr = atof(buf);

  is.read(buf, 16);
  buf[16] = '\0';
  data._rad_velerr = atof(buf);

  for (int i = 0; i < data._ndata; i++)
  {
    is >> data._pos_vect[i];
  }

  return is;
}

ErsSarPlatformPositionData::ErsSarPlatformPositionData(const ErsSarPlatformPositionData& rhs):
    ErsSarRecord(rhs)
{
  _orbit_ele_desg = rhs._orbit_ele_desg;

  for (int i = 0; i < 6; i++)
  {
    _orbit_ele[i] = rhs._orbit_ele[i];
  }

  _ndata = rhs._ndata;

  _year = rhs._year;

  _month = rhs._month;

  _day = rhs._day;

  _gmt_day = rhs._gmt_day;

  _gmt_sec = rhs._gmt_sec;

  _data_int = rhs._data_int;

  _ref_coord = rhs._ref_coord;

  _hr_angle = rhs._hr_angle;

  _alt_poserr = rhs._alt_poserr;

  _crt_poserr = rhs._crt_poserr;

  _rad_poserr = rhs._rad_poserr;

  _alt_velerr = rhs._alt_velerr;

  _crt_velerr = rhs._crt_velerr;

  _rad_velerr = rhs._rad_velerr;

  for (int i = 0; i < 64; i++)
  {
    _pos_vect[i] = rhs._pos_vect[i];
  }

}

ErsSarPlatformPositionData& ErsSarPlatformPositionData::operator=(const ErsSarPlatformPositionData& rhs)
{
  _orbit_ele_desg = rhs._orbit_ele_desg;

  for (int i = 0; i < 6; i++)
  {
    _orbit_ele[i] = rhs._orbit_ele[i];
  }

  _ndata = rhs._ndata;

  _year = rhs._year;

  _month = rhs._month;

  _day = rhs._day;

  _gmt_day = rhs._gmt_day;

  _gmt_sec = rhs._gmt_sec;

  _data_int = rhs._data_int;

  _ref_coord = rhs._ref_coord;

  _hr_angle = rhs._hr_angle;

  _alt_poserr = rhs._alt_poserr;

  _crt_poserr = rhs._crt_poserr;

  _rad_poserr = rhs._rad_poserr;

  _alt_velerr = rhs._alt_velerr;

  _crt_velerr = rhs._crt_velerr;

  _rad_velerr = rhs._rad_velerr;

  for (int i = 0; i < 64; i++)
  {
    _pos_vect[i] = rhs._pos_vect[i];
  }

  return *this;
}
}
