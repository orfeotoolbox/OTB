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
        << MODULE << " Keyword not found: " << NUMBER_OF_CORNER_INCIDENCE_ANGLES << " in "<<pfx <<" path.\n";
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
