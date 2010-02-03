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

#include <otb/RefPoint.h>
#include <otb/Ephemeris.h>
#include <cstring>
#include <ossim/base/ossimKeywordlist.h>

namespace ossimplugins
{


static const char PREFIX[]      = "ref_point.";
static const char DISTANCE_KW[] = "distance";
static const char LINE_KW[]     = "line";
static const char COL_KW[]      = "col";

RefPoint::RefPoint():
   _ephemeris(0),
   _distance(0.0),
   _pix_line(0.0),
   _pix_col(0.0)
{
}

RefPoint::~RefPoint()
{
   if(_ephemeris != 0)
   {
      delete _ephemeris;
   }
}

RefPoint::RefPoint(const RefPoint& rhs):
   _distance(rhs._distance),
   _pix_line(rhs._pix_line),
   _pix_col(rhs._pix_col)
{
   _ephemeris = rhs._ephemeris->Clone();
}

RefPoint& RefPoint::operator=(const RefPoint& rhs)
{
   _distance = rhs._distance;
   _pix_line = rhs._pix_line;
   _pix_col = rhs._pix_col;
   if(_ephemeris != 0)
   {
      delete _ephemeris;
      _ephemeris = 0;
   }
   _ephemeris = rhs._ephemeris->Clone();

   return *this;
}

void RefPoint::set_ephemeris(Ephemeris* ephemeris)
{
   if(_ephemeris != 0)
   {
      delete _ephemeris;
      _ephemeris = 0;
   }
   _ephemeris = ephemeris->Clone();
}

void RefPoint::set_distance(double distance)
{
   _distance = distance;
}

void RefPoint::set_pix_line(double pix_line)
{
   _pix_line = pix_line;
}

void RefPoint::set_pix_col(double pix_col)
{
   _pix_col = pix_col;
}

Ephemeris* RefPoint::get_ephemeris()
{
   return _ephemeris;
}

double RefPoint::get_distance() const
{
   return _distance;
}

double RefPoint::get_pix_line() const
{
   return _pix_line;
}

double RefPoint::get_pix_col() const
{
   return _pix_col;
}

bool RefPoint::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   bool result = true;

   std::string pfx;
   if (prefix)
   {
      pfx = prefix;
   }
   pfx += PREFIX;

   if (_ephemeris)
   {
      result = _ephemeris->saveState(kwl, pfx.c_str());
   }
   else
   {
      result = false;
   }

   kwl.add(pfx.c_str(), DISTANCE_KW, _distance);
   kwl.add(pfx.c_str(), LINE_KW,     _pix_line);
   kwl.add(pfx.c_str(), COL_KW,      _pix_col);

   return result;
}

bool RefPoint::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   bool result = true;

   std::string pfx;
   if (prefix)
   {
      pfx = prefix;
   }
   pfx += PREFIX;


   if (!_ephemeris)
   {
      _ephemeris = new Ephemeris();
   }

   result = _ephemeris->loadState(kwl, pfx.c_str());

   const char* lookup = 0;
   ossimString s;
   double d;

   lookup = kwl.find(pfx.c_str(), DISTANCE_KW);
   if (lookup)
   {
      s = lookup;
      d = s.toDouble();
      _distance = d;
   }
   else
   {
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), LINE_KW);
   if (lookup)
   {
      s = lookup;
      d = s.toDouble();
      _pix_line = d;
   }
   else
   {
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), COL_KW);
   if (lookup)
   {
      s = lookup;
      d = s.toDouble();
      _pix_col = d;
   }
   else
   {
      result = false;
   }

   return result;
}
}
