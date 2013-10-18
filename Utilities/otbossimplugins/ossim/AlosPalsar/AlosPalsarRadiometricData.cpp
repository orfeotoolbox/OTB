//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
// "Copyright Centre for Remote Imaging, Sensing and Processing"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

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
