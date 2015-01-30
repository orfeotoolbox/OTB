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

#include <otb/GMSTDateTime.h>
#include <otb/CivilDateTime.h>

namespace ossimplugins
{


GMSTDateTime::GMSTDateTime():
  _tms(0),
  // TODO: Check this initialisation
  _origine(GMSTDateTime::AN1900)
{
}

GMSTDateTime::~GMSTDateTime()
{
}

GMSTDateTime::GMSTDateTime(const GMSTDateTime& rhs):
  _tms(rhs._tms),
  _origine(rhs._origine)
{
}

GMSTDateTime::GMSTDateTime(double tms, Ori_TSM_Green origine):
  _tms(tms),
  _origine(origine)
{
}

GMSTDateTime::GMSTDateTime(CivilDateTime& rhs)
{
  rhs.AsGMSTDateTime(this);
}

GMSTDateTime& GMSTDateTime::operator=(const GMSTDateTime& rhs)
{
  _tms = rhs._tms;
  _origine = rhs._origine;
  return *this;
}
}
