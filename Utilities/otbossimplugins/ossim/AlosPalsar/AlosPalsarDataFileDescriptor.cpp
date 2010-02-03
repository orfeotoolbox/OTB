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

#include <AlosPalsar/AlosPalsarDataFileDescriptor.h>


namespace ossimplugins
{

AlosPalsarDataFileDescriptor::AlosPalsarDataFileDescriptor() : AlosPalsarRecord("sar_desc_rec")
{
}

AlosPalsarDataFileDescriptor::~AlosPalsarDataFileDescriptor()
{
}

std::ostream& operator<<(std::ostream& os, const AlosPalsarDataFileDescriptor& data)
{
  os << "_num_lines:" << data._num_lines << std::endl;
  os << "_num_pix_in_line:" << data._num_pix_in_line << std::endl;
  return os;
}

std::istream& operator>>(std::istream& is, AlosPalsarDataFileDescriptor& data)
{
  char buf6[7];
  buf6[6] = '\0';

  char buf168[169];
  buf168[168] = '\0';

  char buf94[95];
  buf94[94] = '\0';

  char buf8[9];
  buf8[8] = '\0';

  is.read(buf168, 168);

  is.read(buf6, 6);
  data._num_lines = atoi(buf6);

  is.read(buf94, 94);

  int tmpval;
  is.read(buf8, 8);
  tmpval = atoi(buf8);
  data._num_pix_in_line = tmpval / 8; // Assume data always in 8-byte complex format

  char buf432[433];
  buf432[432] = '\0';

  is.read(buf432, 432);
  return is;
}

AlosPalsarDataFileDescriptor::AlosPalsarDataFileDescriptor(const AlosPalsarDataFileDescriptor& rhs):
    AlosPalsarRecord(rhs),
    _num_pix_in_line(rhs._num_pix_in_line),
    _num_lines(rhs._num_lines)
{
}

AlosPalsarDataFileDescriptor& AlosPalsarDataFileDescriptor::operator=(const AlosPalsarDataFileDescriptor& rhs)
{
  _num_lines = rhs._num_lines;
  _num_pix_in_line = rhs._num_pix_in_line;
  return *this;
}
}
