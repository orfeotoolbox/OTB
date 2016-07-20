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

#include "otb/InfoSceneCoord.h"
#include "ossimKeyWordListUtilities.h"
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
   m_refRow(0),
   m_refColumn(0),
   m_lat(0.0),
   m_lon(0.0),
   m_azimuthTimeUTC(),
   m_rangeTime(0.0),
   m_incidenceAngle(0.)
   {
   }

bool InfoSceneCoord::saveState(ossimKeywordlist& kwl, std::string const& prefix) const
{
   const std::string pfx = prefix + PREFIX;
   add(kwl, pfx, REF_ROW,         m_refRow);
   add(kwl, pfx, REF_COLUMN,      m_refColumn);
   add(kwl, pfx, LAT,             m_lat);
   add(kwl, pfx, LON,             m_lon);
   add(kwl, pfx, AZIMUTH_TIME,    m_azimuthTimeUTC);
   add(kwl, pfx, RANGE_TIME,      m_rangeTime);
   add(kwl, pfx, INCIDENCE_ANGLE, m_incidenceAngle);

   return true;
}

bool InfoSceneCoord::loadState(const ossimKeywordlist& kwl, std::string const& prefix)
{
   static const char MODULE[] = "InfoSceneCoord::loadState";
   const std::string pfx = prefix + PREFIX;

#if 1
   get(kwl, pfx, REF_ROW,         m_refRow);
   get(kwl, pfx, REF_COLUMN,      m_refColumn);
   get(kwl, pfx, LAT,             m_lat);
   get(kwl, pfx, LON,             m_lon);
   get(kwl, pfx, AZIMUTH_TIME,    m_azimuthTimeUTC);
   get(kwl, pfx, RANGE_TIME,      m_rangeTime);
   get(kwl, pfx, INCIDENCE_ANGLE, m_incidenceAngle);
#else
   ossimString s;
   const char* lookup = 0;

   pfx += PREFIX;
   lookup = kwl.find(pfx.c_str(), REF_ROW);
   if (lookup)
   {
      s = lookup;
      m_refRow = s.toUInt32();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << REF_ROW << " in "<<pfx.c_str()<<" path.\n";
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), REF_COLUMN);
   if (lookup)
   {
      s = lookup;
      m_refColumn = s.toUInt32();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << REF_COLUMN << " in "<<pfx.c_str()<<" path.\n";
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), LAT);
   if (lookup)
   {
      s = lookup;
      m_lat = s.toDouble();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << LAT << " in "<<pfx.c_str()<<" path.\n";
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), LON);
   if (lookup)
   {
      s = lookup;
      m_lon = s.toDouble();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << LON << " in "<<pfx.c_str()<<" path.\n";
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), AZIMUTH_TIME);
   if (lookup)
   {
      s = lookup;
      m_azimuthTimeUTC = s;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << AZIMUTH_TIME << " in "<<pfx.c_str()<<" path.\n";
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), RANGE_TIME);
   if (lookup)
   {
      s = lookup;
      m_rangeTime = s.toDouble();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << RANGE_TIME << " in "<<pfx.c_str()<<" path.\n";
      result = false;
   }

   lookup = kwl.find(pfx.c_str(), INCIDENCE_ANGLE);
   if (lookup)
   {
      s = lookup;
      m_incidenceAngle = s.toDouble();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << INCIDENCE_ANGLE << " in "<<pfx.c_str()<<" path.\n";
      result = false;
   }
#endif

   return true;
}
}
