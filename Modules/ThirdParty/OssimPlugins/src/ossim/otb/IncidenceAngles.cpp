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

#include <otb/IncidenceAngles.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimString.h>


namespace ossimplugins
{
static const char INCIDENCE_ANGLES[] = "incidenceAngles";
static const char NUMBER_OF_CORNER_INCIDENCE_ANGLES[] = "numberOfCornerIncidenceAngles";
static const char CENTER_INCIDENCE_ANGLE[] = "centerInfoIncidenceAngle";
static const char CORNERS_INCIDENCE_ANGLE[] = "cornersInfoIncidenceAngle";

IncidenceAngles::IncidenceAngles():
  _numberOfCornerIncidenceAngles(0),
  _centerInfoIncidenceAngle(),
  _tabCornersInfoIncidenceAngle()
{
}

IncidenceAngles::~IncidenceAngles()
{
}


IncidenceAngles::IncidenceAngles(const IncidenceAngles& rhs):
   _numberOfCornerIncidenceAngles(rhs._numberOfCornerIncidenceAngles),
   _centerInfoIncidenceAngle(rhs._centerInfoIncidenceAngle),
   _tabCornersInfoIncidenceAngle(rhs._tabCornersInfoIncidenceAngle)
{
}

IncidenceAngles& IncidenceAngles::operator=(const IncidenceAngles& rhs)
{
  _numberOfCornerIncidenceAngles = rhs._numberOfCornerIncidenceAngles;
  _centerInfoIncidenceAngle = rhs._centerInfoIncidenceAngle;
  _tabCornersInfoIncidenceAngle = rhs._tabCornersInfoIncidenceAngle;
  return *this;
}

bool IncidenceAngles::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   std::string pfx("");
   std::string pfx2("");
   if (prefix)
   {
      pfx = prefix;
   }
   pfx += INCIDENCE_ANGLES;
   pfx2 = pfx;
   pfx2 += ".";
   
   kwl.add(pfx2.c_str(), NUMBER_OF_CORNER_INCIDENCE_ANGLES, _numberOfCornerIncidenceAngles);

   std::string s = pfx + "." + CENTER_INCIDENCE_ANGLE;
   _centerInfoIncidenceAngle.saveState(kwl, s.c_str());

   std::string s2 =pfx + "." + CORNERS_INCIDENCE_ANGLE;
   for (unsigned int i = 0; i < _tabCornersInfoIncidenceAngle.size(); ++i)
   {
      std::string s3 = s2 + "[" + ossimString::toString(i).c_str() + "]";
      _tabCornersInfoIncidenceAngle[i].saveState(kwl, s3.c_str());
   }

   return true;
}

bool IncidenceAngles::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
  static const char MODULE[] = "IncidenceAngles::loadState";

   bool result = true;

   ossimString s;
   const char* lookup = 0;
   
   std::string pfx("");
   if (prefix)
   {
      pfx = prefix;
   }
   pfx += INCIDENCE_ANGLES;
   pfx += ".";
 

   lookup = kwl.find(pfx.c_str(), NUMBER_OF_CORNER_INCIDENCE_ANGLES);

   if (lookup)
   {
     s = lookup;
     _numberOfCornerIncidenceAngles = s.toUInt32();
   }
   else
   {
     ossimNotify(ossimNotifyLevel_WARN)
        << MODULE << " Keyword not found: " << NUMBER_OF_CORNER_INCIDENCE_ANGLES << " in "<<pfx.c_str()<<" path.\n";
     result = false;
   }
   
   std::string s1 = pfx + CENTER_INCIDENCE_ANGLE;
   
   result = _centerInfoIncidenceAngle.loadState(kwl, s1.c_str());

   _tabCornersInfoIncidenceAngle.clear();
   std::string s2 = pfx + CORNERS_INCIDENCE_ANGLE;
   for (unsigned int i = 0; i < _numberOfCornerIncidenceAngles; ++i)
   {
      std::string s3 = s2 + "[" + ossimString::toString(i).c_str() + "]";
      InfoIncidenceAngle iia;
      result = iia.loadState(kwl, s3.c_str());
      _tabCornersInfoIncidenceAngle.push_back(iia);
   }
   if( _numberOfCornerIncidenceAngles != _tabCornersInfoIncidenceAngle.size() )
   {
    ossimNotify(ossimNotifyLevel_WARN)
	<< MODULE << " Keyword " << NUMBER_OF_CORNER_INCIDENCE_ANGLES << " is different with the number of ImageNoise nodes \n";
   }

  return result;
}


std::ostream& IncidenceAngles::print(std::ostream& out) const
{
   out << setprecision(15) << setiosflags(ios::fixed)
       << "\n IncidentAngles class data members:\n";

   const char* prefix = 0;
   ossimKeywordlist kwl;
   ossimString pfx;
   pfx += INCIDENCE_ANGLES;
   ossimString s = pfx + "." + NUMBER_OF_CORNER_INCIDENCE_ANGLES;
   kwl.add(prefix, s.c_str(), _numberOfCornerIncidenceAngles);
   
   ossimString s1 = pfx + "." + CENTER_INCIDENCE_ANGLE;
   _centerInfoIncidenceAngle.saveState(kwl, s1.c_str());

   ossimString s2 =pfx + "." + CORNERS_INCIDENCE_ANGLE;
   for (unsigned int i = 0; i < _tabCornersInfoIncidenceAngle.size(); ++i)
   {
     ossimString s3 = s2 + "[" + ossimString::toString(i) + "]";
     _tabCornersInfoIncidenceAngle[i].saveState(kwl, s3.c_str());
   }
   out << kwl;

   return out;
}

}
