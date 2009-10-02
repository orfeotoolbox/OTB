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

#include <AlosSarSignalData.h>


namespace ossimplugins
{

AlosSarSignalData::AlosSarSignalData() : AlosSarRecord("sar_sig_dat_rec")
{
}

AlosSarSignalData::~AlosSarSignalData()
{
}

std::ostream& operator<<(std::ostream& os, const AlosSarSignalData& data)
{
  os << "_slant_range_to_1st_data_sample:"
     << data._slant_range_to_1st_data_sample << std::endl;

  return os;
}

std::istream& operator>>(std::istream& is, AlosSarSignalData& data)
{
  int tmpint;

  char buf4[5];
  buf4[4] = '\0';

  is.read(buf4,4);
  tmpint = buf4;
  std::cout << 'Line number = ' << tmpint;

  is.read(buf4,4);
  tmpint = buf4;
  std::cout << 'SAR image data record index = ' << tmpint;

// TODO Complete modification below


  char buf6[7];
  buf6[6] = '\0';

  char buf168[169];
  buf168[168] = '\0';

  char buf94[95];
  buf94[94] = '\0';

  char buf8[9];
  buf8[8] = '\0';

  is.read(buf168,168);

  is.read(buf6,6);
  data._num_lines = atoi(buf6);

  is.read(buf94,94);

  int tmpval;
  is.read(buf8,8);
  tmpval = atoi(buf8);
  data._num_pix_in_line = tmpval/8; // Assume data always in 8-byte complex format

  char buf432[433];
  buf432[432] = '\0';

  is.read(buf432,432);
  return is;
}

AlosSarSignalData::AlosSarSignalData(const AlosSarSignalData& rhs):
  AlosSarRecord(rhs),
  _num_lines(rhs._num_lines),
  _num_pix_in_line(rhs._num_pix_in_line)
{
}

AlosSarSignalData& AlosSarSignalData::operator=(const AlosSarSignalData& rhs)
{
  _num_lines = rhs._num_lines;
  _num_pix_in_line = rhs._num_pix_in_line;
  return *this;
}
}
