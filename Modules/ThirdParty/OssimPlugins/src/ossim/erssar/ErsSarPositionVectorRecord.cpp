//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#include "erssar/ErsSarPositionVectorRecord.h"


namespace ossimplugins
{

ErsSarPositionVectorRecord::ErsSarPositionVectorRecord()
{
}

ErsSarPositionVectorRecord::~ErsSarPositionVectorRecord()
{
}

std::ostream& operator<<(std::ostream& os, const ErsSarPositionVectorRecord& data)
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

std::istream& operator>>(std::istream& is, ErsSarPositionVectorRecord& data)
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

ErsSarPositionVectorRecord::ErsSarPositionVectorRecord(const ErsSarPositionVectorRecord& rhs)
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

ErsSarPositionVectorRecord& ErsSarPositionVectorRecord::operator=(const ErsSarPositionVectorRecord& rhs)
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
