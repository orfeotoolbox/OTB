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

#include <otb/MJDDateTime.h>

namespace ossimplugins
{


MJDDateTime::MJDDateTime():
  _day(0),
  _second(0),
  _microsecond(0)
{
}

MJDDateTime::~MJDDateTime()
{
}

MJDDateTime::MJDDateTime(const MJDDateTime& rhs):
  _day(rhs._day),
  _second(rhs._second),
  _microsecond(rhs._microsecond)
{
}

MJDDateTime::MJDDateTime(long day, unsigned long second, unsigned long microsecond):
  _day(day),
  _second(second),
  _microsecond(microsecond)
{
}

MJDDateTime& MJDDateTime::operator=(const MJDDateTime& rhs)
{
  _day = rhs._day;
  _second = rhs._second;
  _microsecond = rhs._microsecond;

  return *this;
}
}
