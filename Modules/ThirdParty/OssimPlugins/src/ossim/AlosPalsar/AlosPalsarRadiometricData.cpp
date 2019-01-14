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


#include <AlosPalsar/AlosPalsarRadiometricData.h>

namespace ossimplugins
{


AlosPalsarRadiometricData::AlosPalsarRadiometricData() : AlosPalsarRecord("radio_data_rec")
{
}

AlosPalsarRadiometricData::~AlosPalsarRadiometricData()
{
}

std::ostream& operator<<(std::ostream& os, const AlosPalsarRadiometricData& data)
{
  os << "calibration_factor:" << data._calibration_factor << std::endl;

  return os;
}

std::istream& operator>>(std::istream& is, AlosPalsarRadiometricData& data)
{
  char buf[9];

  is.seekg(8, std::ios::cur);

  is.read(buf, 8);
  buf[8] = '\0';
  data._calibration_factor = atof(buf);

  is.seekg(9824, std::ios::cur);			//skip the rest of the radiometric data record
  is.seekg(12477692, std::ios::cur);		//skip to the 11th facility-related data record

  return is;
}

AlosPalsarRadiometricData::AlosPalsarRadiometricData(const AlosPalsarRadiometricData& rhs):
    AlosPalsarRecord(rhs)
{
  _calibration_factor = rhs._calibration_factor;
}

AlosPalsarRadiometricData& AlosPalsarRadiometricData::operator=(const AlosPalsarRadiometricData& rhs)
{
  _calibration_factor = rhs._calibration_factor;

  return *this;
}

}
