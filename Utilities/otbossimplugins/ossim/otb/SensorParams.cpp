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

#include <SensorParams.h>
#include <ossim/base/ossimKeywordlist.h>

SensorParams::SensorParams():
   _prf(0.0),
   _sf(0.0),
   _rwl(0.0),
   _col_direction(1),
   _lin_direction(1),
   _sightDirection(Right),
   _semiMajorAxis(6378137.0),
   _semiMinorAxis(6356752.3141),        
   _nAzimuthLook(1),
   _nRangeLook(1)
{
}

SensorParams::~SensorParams()
{
}

SensorParams::SensorParams(const SensorParams& rhs):
   _prf(rhs._prf),
   _sf(rhs._sf),
   _rwl(rhs._rwl),
   _col_direction(rhs._col_direction),
   _lin_direction(rhs._lin_direction),
   _sightDirection(rhs._sightDirection),
   _semiMajorAxis(rhs._semiMajorAxis),
   _semiMinorAxis(rhs._semiMinorAxis),
   _nAzimuthLook(rhs._nAzimuthLook),
   _nRangeLook(rhs._nRangeLook)
{
}

SensorParams& SensorParams::operator=(const SensorParams& rhs)
{
   _prf = rhs._prf;
   _sf = rhs._sf;
   _rwl = rhs._rwl;
   _col_direction = rhs._col_direction;
   _lin_direction = rhs._lin_direction;
   _sightDirection = rhs._sightDirection;
   _semiMajorAxis = rhs._semiMajorAxis;
   _semiMinorAxis = rhs._semiMinorAxis;
   _nAzimuthLook = rhs._nAzimuthLook;
   _nRangeLook = rhs._nRangeLook;
   _semiMajorAxis = rhs._semiMajorAxis;
   _semiMinorAxis = rhs._semiMinorAxis;
   return *this;
}

bool SensorParams::saveState(ossimKeywordlist& /*kwl*/, const char* /*prefix*/) const
{
   bool result = true;
   
   return result;
}

bool SensorParams::loadState(const ossimKeywordlist& /*kwl*/, const char* /*prefix*/)
{
   bool result = true;

   return result;
}
