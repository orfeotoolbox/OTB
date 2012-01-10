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

#include <otb/Ephemeris.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimString.h>

namespace ossimplugins
{


static const char PREFIX[]          = "ephemeris.";
static const char DATE_JULIAN_KW[]  = "date.julian";
static const char DATE_SECOND_KW[]  = "date.second";
static const char DATE_DECIMAL_KW[] = "date.decimal";
static const char POSITION_KW[]     = "position";
static const char VELOCITY_KW[]     = "velocity";

Ephemeris::Ephemeris()
{
  _position[0] = 0.0;
  _position[1] = 0.0;
  _position[2] = 0.0;
  _speed[0] = 0.0;
  _speed[1] = 0.0;
  _speed[2] = 0.0;
}

Ephemeris::~Ephemeris()
{
}

Ephemeris::Ephemeris(JSDDateTime date, double pos[3], double speed[3]):
  _date(date)
{
  _position[0] = pos[0];
  _position[1] = pos[1];
  _position[2] = pos[2];
  _speed[0] = speed[0];
  _speed[1] = speed[1];
  _speed[2] = speed[2];
}

Ephemeris::Ephemeris(const Ephemeris& rhs):
  _date(rhs._date)
{
  _position[0] = rhs._position[0];
  _position[1] = rhs._position[1];
  _position[2] = rhs._position[2];
  _speed[0] = rhs._speed[0];
  _speed[1] = rhs._speed[1];
  _speed[2] = rhs._speed[2];
}

Ephemeris& Ephemeris::operator=(const Ephemeris& rhs)
{
  _position[0] = rhs._position[0];
  _position[1] = rhs._position[1];
  _position[2] = rhs._position[2];
  _speed[0] = rhs._speed[0];
  _speed[1] = rhs._speed[1];
  _speed[2] = rhs._speed[2];
  _date = rhs._date;

  return *this;
}

bool Ephemeris::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   std::string pfx;
   if (prefix)
   {
      pfx = prefix;
   }
   pfx += PREFIX;

   ossimDpt3d pos(_position[0], _position[1], _position[2]);
   ossimDpt3d vit(_speed[0], _speed[1], _speed[2]);
   JulianDate jd = _date.get_day0hTU();

   kwl.add(pfx.c_str(), DATE_JULIAN_KW,  jd.get_julianDate());
   kwl.add(pfx.c_str(), DATE_SECOND_KW,  _date.get_second());
   kwl.add(pfx.c_str(), DATE_DECIMAL_KW, _date.get_decimal());
   kwl.add(pfx.c_str(), POSITION_KW,     pos.toString(15).c_str());
   kwl.add(pfx.c_str(), VELOCITY_KW,     vit.toString(15).c_str());

   return true;
}

bool Ephemeris::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   static const char MODULE[] = "Ephemeris::loadState";

   bool result = true;

   std::string pfx;
   if (prefix)
   {
      pfx = prefix;
   }
   pfx += "ephemeris.";

   const char* lookup = 0;
   ossimString s;
   double d;

   lookup = kwl.find(pfx.c_str(), DATE_JULIAN_KW);
   if (lookup)
   {
      s = lookup;
      d = s.toDouble();
      JulianDate jd(d);
      _date.set_day0hTU(jd);
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << DATE_JULIAN_KW << "\n";

      result = false;
   }

   lookup = kwl.find(pfx.c_str(), DATE_SECOND_KW);
   if (lookup)
   {
      s = lookup;
      d = s.toDouble();
      _date.set_second(d);
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << DATE_SECOND_KW << "\n";
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), DATE_DECIMAL_KW);
   if (lookup)
   {
      s = lookup;
      d = s.toDouble();
      _date.set_decimal(d);
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << DATE_DECIMAL_KW << "\n";
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), POSITION_KW);
   if (lookup)
   {
      std::string ps = lookup;

      ossimDpt3d pt;
      pt.toPoint(ps);

      _position[0] = pt.x;
      _position[1] = pt.y;
      _position[2] = pt.z;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << POSITION_KW << "\n";
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), VELOCITY_KW);
   if (lookup)
   {
      std::string ps = lookup;

      ossimDpt3d pt;
      pt.toPoint(ps);

      _speed[0] = pt.x;
      _speed[1] = pt.y;
      _speed[2] = pt.z;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << VELOCITY_KW << "\n";
      result = false;
   }

   return result;
}
}
