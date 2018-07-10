/*
 * Copyright (C) 2005-2017 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include <otb/InfoSceneCoord.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimString.h>

namespace ossimplugins
{


static const char PREFIX[]          = ".";
static const char REF_ROW[]         = "refRow";
static const char REF_COLUMN[]      = "refColumn";
static const char LAT[]             = "lat";
static const char LON[]             = "lon";
static const char AZIMUTH_TIME[]    = "azimuthTimeUTC";
static const char RANGE_TIME[]      = "rangeTime";
static const char INCIDENCE_ANGLE[] = "incidenceAngle";

InfoSceneCoord::InfoSceneCoord():
     _refRow(0),
     _refColumn(0),
     _lat(0.0),
     _lon(0.0),
     _azimuthTimeUTC(),
     _rangeTime(0.0),
     _incidenceAngle(0.)
{
}

InfoSceneCoord::~InfoSceneCoord()
{
}


InfoSceneCoord::InfoSceneCoord(const InfoSceneCoord& rhs):
     _refRow(rhs._refRow),
     _refColumn(rhs._refColumn),
     _lat(rhs._lat),
     _lon(rhs._lon),
     _azimuthTimeUTC(rhs._azimuthTimeUTC),
     _rangeTime(rhs._rangeTime),
     _incidenceAngle(rhs._incidenceAngle)
{
}

InfoSceneCoord& InfoSceneCoord::operator=(const InfoSceneCoord& rhs)
{
     _refRow = rhs._refRow;
     _refColumn = rhs._refColumn;
     _lat = rhs._lat;
     _lon = rhs._lon;
     _azimuthTimeUTC = rhs._azimuthTimeUTC;
     _rangeTime =rhs._rangeTime;
     _incidenceAngle = rhs._incidenceAngle;
  return *this;
}

bool InfoSceneCoord::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
  std::string pfx;
  if (prefix)
  {
     pfx = prefix;
  }
  pfx += PREFIX;
  kwl.add(pfx.c_str(), REF_ROW,  _refRow);
  kwl.add(pfx.c_str(), REF_COLUMN, _refColumn);
  kwl.add(pfx.c_str(), LAT, _lat);
  kwl.add(pfx.c_str(), LON, _lon);
  kwl.add(pfx.c_str(), AZIMUTH_TIME, _azimuthTimeUTC);
  kwl.add(pfx.c_str(), RANGE_TIME, _rangeTime);
  kwl.add(pfx.c_str(), INCIDENCE_ANGLE, _incidenceAngle);

  return true;
}

bool InfoSceneCoord::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
  static const char MODULE[] = "InfoSceneCoord::loadState";
  bool result = true;
  std::string pfx;
  if (prefix)
  {
    pfx = prefix;
  }
   ossimString s;
   const char* lookup = 0;

  pfx += PREFIX;
  lookup = kwl.find(pfx.c_str(), REF_ROW);
  if (lookup)
  {
    s = lookup;
    _refRow = s.toUInt32();
  }
  else
  {
    ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << REF_ROW << " in "<<pfx <<" path.\n";
    result = false;
  }
  
  lookup = kwl.find(pfx.c_str(), REF_COLUMN);
  if (lookup)
  {
    s = lookup;
    _refColumn = s.toUInt32();
  }
  else
  {
    ossimNotify(ossimNotifyLevel_WARN)
	<< MODULE << " Keyword not found: " << REF_COLUMN << " in "<<pfx <<" path.\n";
    result = false;
  }

  lookup = kwl.find(pfx.c_str(), LAT);
  if (lookup)
  {
    s = lookup;
    _lat = s.toDouble();
  }
  else
  {
    ossimNotify(ossimNotifyLevel_WARN)
	<< MODULE << " Keyword not found: " << LAT << " in "<<pfx <<" path.\n";
    result = false;
  }

  lookup = kwl.find(pfx.c_str(), LON);
  if (lookup)
  {
    s = lookup;
    _lon = s.toDouble();
  }
  else
  {
    ossimNotify(ossimNotifyLevel_WARN)
	<< MODULE << " Keyword not found: " << LON << " in "<<pfx <<" path.\n";
    result = false;
  }

  lookup = kwl.find(pfx.c_str(), AZIMUTH_TIME);
  if (lookup)
  {
    s = lookup;
    _azimuthTimeUTC = s;
  }
  else
  {
    ossimNotify(ossimNotifyLevel_WARN)
	<< MODULE << " Keyword not found: " << AZIMUTH_TIME << " in "<<pfx <<" path.\n";
    result = false;
  }

  lookup = kwl.find(pfx.c_str(), RANGE_TIME);
  if (lookup)
  {
    s = lookup;
    _rangeTime = s.toDouble();
  }
  else
  {
    ossimNotify(ossimNotifyLevel_WARN)
	<< MODULE << " Keyword not found: " << RANGE_TIME << " in "<<pfx <<" path.\n";
    result = false;
  }

  lookup = kwl.find(pfx.c_str(), INCIDENCE_ANGLE);
  if (lookup)
  {
    s = lookup;
    _incidenceAngle = s.toDouble();
  }
  else
  {
    ossimNotify(ossimNotifyLevel_WARN)
	<< MODULE << " Keyword not found: " << INCIDENCE_ANGLE << " in "<<pfx <<" path.\n";
    result = false;
  }

   return result;
}
}
