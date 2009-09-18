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

#include <AlosSarDataFileDescriptor.h>


namespace ossimplugins
{

AlosSarDataFileDescriptor::AlosSarDataFileDescriptor() : AlosSarRecord("sar_desc_rec")
{
}

AlosSarDataFileDescriptor::~AlosSarDataFileDescriptor()
{
}

std::ostream& operator<<(std::ostream& os, const AlosSarDataFileDescriptor& data)
{
  os<<"_num_lines:"<<data._num_lines<<std::endl;
  os<<"_num_lines:"<<data._num_lines<<std::endl;
  return os;
}

std::istream& operator>>(std::istream& is, AlosSarDataFileDescriptor& data)
{
  char buf6[7];
  buf6[6] = '\0';

  char buf180[181];
  buf180[180] = '\0';

  is.read(buf180,180);

  is.read(buf6,6);
  data._num_lines = atoi(buf6);

  is.read(buf6,6);
  data._num_pix_in_line = atoi(buf6);

  char buf528[528];
  buf528[528] = '\0';

  is.read(buf528,528);
  return is;
}

AlosSarDataFileDescriptor::AlosSarDataFileDescriptor(const AlosSarDataFileDescriptor& rhs):
  AlosSarRecord(rhs),
  _num_lines(rhs._num_lines),
  _num_pix_in_line(rhs._num_pix_in_line)
{
}

AlosSarDataFileDescriptor& AlosSarDataFileDescriptor::operator=(const AlosSarDataFileDescriptor& rhs)
{
  _num_lines = rhs._num_lines;
  _num_pix_in_line = rhs._num_pix_in_line;
  return *this;
}
}
