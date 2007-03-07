//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
// 
// Description: Container class for a tiff world file data.
//
//********************************************************************
// $Id: ossimTiffWorld.cpp,v 1.7 2004/12/20 11:44:54 dburken Exp $

#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

#include <base/common/ossimConstants.h>
#include <base/common/ossimKeywordNames.h>
#include <base/misc/ossimUnitConversionTool.h>
#include <support_data/tiff/ossimTiffWorld.h>

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
   double xScale = fabs(theXScale);
   double yScale = fabs(theYScale);
   double transX = theTranslateX;
   double transY = theTranslateY;

   if ( (theUnit == OSSIM_FEET) || (theUnit == OSSIM_US_SURVEY_FEET) )
   {
      // Convert to meters.
      xScale = ossimUnitConversionTool(xScale, theUnit).getMeters();
      yScale = ossimUnitConversionTool(yScale, theUnit).getMeters();
      transX = ossimUnitConversionTool(transX, theUnit).getMeters();
      transY = ossimUnitConversionTool(transY, theUnit).getMeters();
   }
   
   // Shift the tie point to be relative to the center of the pixel.
   if(thePixelType == OSSIM_PIXEL_IS_AREA)
   {
      transX += (xScale/2.0);
      transY -= (yScale/2.0);

      // Adjust the keyword list to reflect pixel is point.
      kwl.add(prefix,
              ossimKeywordNames::PIXEL_TYPE_KW,
              "pixel_is_point",
              true);
   }
   
   if(theUnit == OSSIM_DEGREES)
   {
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_LON_KW,
              transX,
              true);
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_LAT_KW,
              transY,
              true);
      kwl.add(prefix,
              ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON,
              xScale,
              true);
      kwl.add(prefix,
              ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT,
              yScale,
              true);
   }
   else
   {
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_EASTING_KW,
              transX,
              true);
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_NORTHING_KW,
              transY,
              true);
      kwl.add(prefix,
              ossimKeywordNames::METERS_PER_PIXEL_X_KW,
              xScale,
              true);
      kwl.add(prefix,
              ossimKeywordNames::METERS_PER_PIXEL_Y_KW,
              yScale,
              true);
   }
   
   return true;
}

bool ossimTiffWorld::loadFromOssimGeom(const ossimKeywordlist& kwl, 
				       const char* prefix)
{
   the2ndValue = 0.0;
   the3rdValue = 0.0;
   
   ossimString xscale   =
      kwl.find(prefix, ossimKeywordNames::METERS_PER_PIXEL_X_KW);
   ossimString yscale   =
      kwl.find(prefix, ossimKeywordNames::METERS_PER_PIXEL_Y_KW);
   ossimString easting  =
      kwl.find(prefix, ossimKeywordNames::TIE_POINT_EASTING_KW);
   ossimString northing =
      kwl.find(prefix, ossimKeywordNames::TIE_POINT_NORTHING_KW);
   
   theXScale      = xscale.toDouble();
   theYScale      = -(yscale.toDouble());
   theTranslateX  = easting.toDouble();
   theTranslateY  = northing.toDouble();
   
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

