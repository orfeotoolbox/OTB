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

#include <otb/SensorParams.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimString.h>

namespace ossimplugins
{


static const char PREFIX[]               = "sensor_params.";
static const char PRF_KW[]               = "prf";
static const char SF_KW[]                = "sampling_frequency";
static const char RWL_KW[]               = "radar_wave_length";
static const char COL_DIR_KW[]           = "column_direction";
static const char LIN_DIR_KW[]           = "line_direction";
static const char SIGHT_DIR_KW[]         = "sight_direction";
static const char SEMI_MAJOR_AXIS_KW[]   = "semi_major_axis";
static const char SEMI_MINOR_AXIS_KW[]   = "semi_minor_axis";
static const char NUM_AZIMUTH_LOOKS_KW[] = "number_azimuth_looks";
static const char NUM_RANGE_LOOKS_KW[]   = "number_range_looks";
static const char DOPCEN_KW[]            = "doppler_centroid";
static const char DOPCENLINEAR_KW[]      = "doppler_centroid_linear_term";
static const char RANGETOFIRSTDATA_KW[]  = "range_to_first_data_sample";

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
   _nRangeLook(1),
   _dopcen(0.0),
   _dopcenLinear(0.0)
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
   _nRangeLook(rhs._nRangeLook),
   _dopcen(rhs._dopcen),
   _dopcenLinear(rhs._dopcenLinear)
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
   _dopcen = rhs._dopcen;
   _dopcenLinear = rhs._dopcenLinear;
   return *this;
}

bool SensorParams::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   std::string pfx;
   if (prefix)
   {
      pfx = prefix;
   }
   pfx += PREFIX;

   kwl.add(pfx.c_str(), PRF_KW, _prf);
   kwl.add(pfx.c_str(), SF_KW, _sf);
   kwl.add(pfx.c_str(), RWL_KW, _rwl);
   kwl.add(pfx.c_str(), COL_DIR_KW, _col_direction);
   kwl.add(pfx.c_str(), LIN_DIR_KW, _lin_direction);
   kwl.add(pfx.c_str(), SIGHT_DIR_KW, static_cast<int>(_sightDirection));
   kwl.add(pfx.c_str(), SEMI_MAJOR_AXIS_KW, _semiMajorAxis);
   kwl.add(pfx.c_str(), SEMI_MINOR_AXIS_KW, _semiMinorAxis);
   kwl.add(pfx.c_str(), NUM_AZIMUTH_LOOKS_KW, _nAzimuthLook);
   kwl.add(pfx.c_str(), NUM_RANGE_LOOKS_KW, _nRangeLook);
   kwl.add(pfx.c_str(), DOPCEN_KW, _dopcen);
   kwl.add(pfx.c_str(), DOPCENLINEAR_KW, _dopcenLinear);
   return true;
}

bool SensorParams::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   bool result = true;

   std::string pfx;
   if (prefix)
   {
      pfx = prefix;
   }
   pfx += PREFIX;

   ossimString s;
   const char* lookup = 0;

   lookup = kwl.find(pfx.c_str(), PRF_KW);
   if (lookup)
   {
      s = lookup;
      _prf = s.toDouble();
   }
   else
   {
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), SF_KW);
   if (lookup)
   {
      s = lookup;
      _sf = s.toDouble();
   }
   else
   {
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), RWL_KW);
   if (lookup)
   {
      s = lookup;
      _rwl = s.toDouble();
   }
   else
   {
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), COL_DIR_KW);
   if (lookup)
   {
      s = lookup;
      _col_direction = s.toInt();
   }
   else
   {
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), LIN_DIR_KW);
   if (lookup)
   {
      s = lookup;
      _lin_direction = s.toInt();
   }
   else
   {
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), SIGHT_DIR_KW);
   if (lookup)
   {
      s = lookup;
      if ( s.toInt() == 0 )
      {
         _sightDirection = SensorParams::Left;
      }
      else
      {
         _sightDirection = SensorParams::Right;
      }
   }
   else
   {
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), SEMI_MAJOR_AXIS_KW);
   if (lookup)
   {
      s = lookup;
      _semiMajorAxis = s.toDouble();
   }
   else
   {
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), SEMI_MINOR_AXIS_KW);
   if (lookup)
   {
      s = lookup;
      _semiMinorAxis = s.toDouble();
   }
   else
   {
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), NUM_AZIMUTH_LOOKS_KW);
   if (lookup)
   {
      s = lookup;
      _nAzimuthLook = s.toDouble();
   }
   else
   {
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), NUM_RANGE_LOOKS_KW);
   if (lookup)
   {
      s = lookup;
      _nRangeLook = s.toDouble();
   }
   else
   {
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), DOPCEN_KW);
   if (lookup)
   {
      s = lookup;
      _dopcen = s.toDouble();
   }
   else
   {
      result = false;
   }
   return result;

   lookup = kwl.find(pfx.c_str(), DOPCENLINEAR_KW);
   if (lookup)
   {
      s = lookup;
      _dopcenLinear = s.toDouble();
   }
   else
   {
      result = false;
   }
   return result;

}

}
