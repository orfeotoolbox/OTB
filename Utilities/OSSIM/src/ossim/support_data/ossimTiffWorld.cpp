//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Ken Melero
// 
// Description: Container class for a tiff world file data.
//
//********************************************************************
// $Id: ossimTiffWorld.cpp 9464 2006-08-28 18:53:04Z dburken $

#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimUnitConversionTool.h>
#include <ossim/support_data/ossimTiffWorld.h>

//**************************************************************************
// ossimTiffWorld::ossimTiffWorld()
//***************************************************************************
ossimTiffWorld::ossimTiffWorld()
   :
      theXScale(1.0),
      the2ndValue(0.0),
      the3rdValue(0.0),
      theYScale(1.0),
      theTranslateX(0.0),
      theTranslateY(0.0),
      thePixelType(OSSIM_PIXEL_IS_AREA),
      theUnit(OSSIM_METERS)
{}

//**************************************************************************
// ossimTiffWorld::ossimTiffWorld(const char* file, PixelType ptype)
//***************************************************************************
ossimTiffWorld::ossimTiffWorld(const char* file, 
			       ossimPixelType ptype,
			       ossimUnitType  scaleUnits)
   :
      theXScale(1.0),
      the2ndValue(0.0),
      the3rdValue(0.0),
      theYScale(-1.0),
      theTranslateX(0.0),
      theTranslateY(0.0),
      thePixelType(ptype),
      theUnit(scaleUnits)
{
   ifstream is;
   is.open(file);

   if(!is)
   {
      return;
   }

   is >> theXScale
      >> the2ndValue
      >> the3rdValue
      >> theYScale
      >> theTranslateX
      >> theTranslateY;

   is.close();
}

ossimTiffWorld::~ossimTiffWorld()
{
}

void ossimTiffWorld::forward(const ossimDpt& imagePoint,
                             ossimDpt& transformedPoint)
{
   transformedPoint.x =
      imagePoint.x*theXScale   + imagePoint.y*the3rdValue + theTranslateX;
   transformedPoint.y =
      imagePoint.x*the2ndValue + imagePoint.y*theYScale   + theTranslateY;
}


bool ossimTiffWorld::saveToOssimGeom(ossimKeywordlist& kwl, 
				     const char* prefix)const
{
   ossimDpt scale(fabs(theXScale), fabs(theYScale));
   ossimDpt tie(theTranslateX, theTranslateY);

   if ( (theUnit == OSSIM_FEET) || (theUnit == OSSIM_US_SURVEY_FEET) )
   {
      // Convert to meters.
      scale.x = ossimUnitConversionTool(scale.x, theUnit).getMeters();
      scale.y = ossimUnitConversionTool(scale.y, theUnit).getMeters();
      tie.x   = ossimUnitConversionTool(tie.x, theUnit).getMeters();
      tie.y   = ossimUnitConversionTool(tie.y, theUnit).getMeters();
   }
   
   // Shift the tie point to be relative to the center of the pixel.
   if(thePixelType == OSSIM_PIXEL_IS_AREA)
   {
      tie.x += (scale.x/2.0);
      tie.y -= (scale.y/2.0);

      // Adjust the keyword list to reflect pixel is point.
      kwl.add(prefix,
              ossimKeywordNames::PIXEL_TYPE_KW,
              "pixel_is_point",
              true);
   }

   // Units in feet converted to meters up above.
   ossimString units = "meters";
   if(theUnit == OSSIM_DEGREES)
   {
      units = "degrees";
   }
   
   kwl.add(prefix,
           ossimKeywordNames::TIE_POINT_XY_KW,
           tie.toString().c_str(),
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::TIE_POINT_UNITS_KW,
           units.c_str(),
           true);

   kwl.add(prefix,
           ossimKeywordNames::PIXEL_SCALE_XY_KW,
           scale.toString().c_str(),
           true);

   kwl.add(prefix,
           ossimKeywordNames::PIXEL_SCALE_UNITS_KW,
           units.c_str(),
           true);

   return true;
}

bool ossimTiffWorld::loadFromOssimGeom(const ossimKeywordlist& kwl, 
				       const char* prefix)
{
   the2ndValue = 0.0;
   the3rdValue = 0.0;

   const char* lookup;

   // Get the scale...
   lookup = kwl.find(prefix, ossimKeywordNames::PIXEL_SCALE_XY_KW);
   if (lookup)
   {
      ossimDpt scale;
      scale.toPoint(std::string(lookup));
      theXScale      = scale.x;
      theYScale      = -(scale.y);
   }
   else // BACKWARDS COMPATIBILITY LOOKUPS...
   {
      ossimString xscale   =
         kwl.find(prefix, ossimKeywordNames::METERS_PER_PIXEL_X_KW);
      ossimString yscale   =
         kwl.find(prefix, ossimKeywordNames::METERS_PER_PIXEL_Y_KW);
      theXScale      = xscale.toDouble();
      theYScale      = -(yscale.toDouble());
   }

   // Get the tie...
   lookup = kwl.find(prefix, ossimKeywordNames::TIE_POINT_XY_KW);
   if (lookup)
   {
      ossimDpt tie;
      tie.toPoint(std::string(lookup));
      theTranslateX  = tie.x;
      theTranslateY  = tie.y;
   }
   else // BACKWARDS COMPATIBILITY LOOKUPS...
   {
      ossimString easting  =
         kwl.find(prefix, ossimKeywordNames::TIE_POINT_EASTING_KW);
      ossimString northing =
         kwl.find(prefix, ossimKeywordNames::TIE_POINT_NORTHING_KW);
      theTranslateX  = easting.toDouble();
      theTranslateY  = northing.toDouble(); 
   }
   
   return true;
}

std::ostream& ossimTiffWorld::print(std::ostream& out) const
{
   out << setiosflags(ios::fixed) << setprecision(15)
       << theXScale     << "\n"
       << the2ndValue   << "\n"
       << the3rdValue   << "\n"
       << theYScale     << "\n"
       << theTranslateX << "\n"
       << theTranslateY
       << endl;
   return out;
}

std::ostream& operator<<(std::ostream& out, const ossimTiffWorld& obj)
{
   return obj.print(out);
}

