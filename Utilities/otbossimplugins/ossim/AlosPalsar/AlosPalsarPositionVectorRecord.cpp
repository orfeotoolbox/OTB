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

#include <AlosPalsar/AlosPalsarPositionVectorRecord.h>

namespace ossimplugins
{


AlosPalsarPositionVectorRecord::AlosPalsarPositionVectorRecord()
{
}

AlosPalsarPositionVectorRecord::~AlosPalsarPositionVectorRecord()
{
}

std::ostream& operator<<(std::ostream& os, const AlosPalsarPositionVectorRecord& data)
{
  for (int i = 0; i < 3; i++)
  {
    os << "pos[" << i << "]:" << data._pos[i] << std::endl;
  }

  for (int i = 0; i < 3; i++)
  {
    os << "vel[" << i << "]:" << data._vel[i] << std::endl;
  }
  return os;
}

std::istream& operator>>(std::istream& is, AlosPalsarPositionVectorRecord& data)
{
  char buf[23];
  buf[22] = '\0';

  for (int i = 0; i < 3; i++)
  {
    is.read(buf, 22);
    data._pos[i] = atof(buf);
  }

  for (int i = 0; i < 3; i++)
  {
    is.read(buf, 22);
    data._vel[i] = atof(buf);
  }
  return is;
}

AlosPalsarPositionVectorRecord::AlosPalsarPositionVectorRecord(const AlosPalsarPositionVectorRecord& rhs)
{
  for (int i = 0; i < 3; i++)
  {
    _pos[i] = rhs._pos[i];
  }

  for (int i = 0; i < 3; i++)
  {
    _vel[i] = rhs._vel[i];
  }
}

AlosPalsarPositionVectorRecord& AlosPalsarPositionVectorRecord::operator=(const AlosPalsarPositionVectorRecord& rhs)
{
  for (int i = 0; i < 3; i++)
  {
    _pos[i] = rhs._pos[i];
  }

  for (int i = 0; i < 3; i++)
  {
    _vel[i] = rhs._vel[i];
  }
  return *this;
}

}
