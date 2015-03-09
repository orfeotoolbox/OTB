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
