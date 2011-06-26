//*******************************************************************
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
// $Id: ossimTiffWorld.cpp 19682 2011-05-31 14:21:20Z dburken $

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimUnitConversionTool.h>
#include <ossim/support_data/ossimTiffWorld.h>
#include <ossim/base/ossimNotify.h>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

//**************************************************************************
// ossimTiffWorld::ossimTiffWorld()
//***************************************************************************
ossimTiffWorld::ossimTiffWorld()
   :
      theXform1(1.0),
      theXform2(0.0),
      theXform3(0.0),
      theXform4(-1.0),
      theTranslation(0.0,0.0),
      thePixelType(OSSIM_PIXEL_IS_AREA),
      theUnit(OSSIM_METERS),
      theComputedScale(0.0, 0.0),
      theComputedRotation(0.0)
{}

//**************************************************************************
// ossimTiffWorld::ossimTiffWorld(const char* file, PixelType ptype)
//***************************************************************************
ossimTiffWorld::ossimTiffWorld(const char* file, 
			       ossimPixelType ptype,
			       ossimUnitType  scaleUnits)
   :
      theXform1(1.0),
      theXform2(0.0),
      theXform3(0.0),
      theXform4(-1.0),
      theTranslation(0.0,0.0),
      thePixelType(ptype),
      theUnit(scaleUnits),
      theComputedScale(0.0, 0.0),
      theComputedRotation(0.0)
{
   open(ossimFilename(file), ptype, scaleUnits);
}

bool ossimTiffWorld::open(const ossimFilename& file, ossimPixelType ptype, ossimUnitType  unit)
{
   bool result = false;

   ifstream is;
   is.open(file.c_str());

   if( !is.is_open() )
   {
      // ESH 07/2008, Trac #234: OSSIM is case sensitive 
      // when using worldfile templates during ingest
      // -- If first you don't succeed with the user-specified
      // filename, try again with the results of a case insensitive search.
      ossimFilename fullName(file);
      ossimDirectory directory(fullName.path());
      ossimFilename filename(fullName.file());
      
      std::vector<ossimFilename> result;
      bool bSuccess = directory.findCaseInsensitiveEquivalents(
         filename, result );
      if ( bSuccess == true )
      {
         int numResults = (int)result.size();
         int i;
         for ( i=0; i<numResults && !is.is_open(); ++i )
         {
            is.open( result[i].c_str() );
         }
      }
   }
      
   if ( is.is_open() )
   {
      double x,y;
      is >> theXform1 >> theXform2 >> theXform3 >> theXform4 >> x >> y;
      theTranslation = ossimDpt(x,y);
      
      // Compute the affine parameters from the transform:
      theComputedRotation = atan2(theXform2,theXform1);
      
/*
 * Commented out warning.
 * Used all the time for tie and scale, NOT for affine. We could wrap around trace
 * if we added trace to class. (drb - 20110115)
 */
#if 0
      double angle2 = atan2(theXform4,theXform3);
      if (fabs(theComputedRotation - angle2) > 0.00001)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimTiffWorld -- Non-affine transform encountered."
            << " Use of an affine transform to represent this world file geometry will result in errors."
            << endl;
      }
#endif
      
      double cos_rot = cos(theComputedRotation);
      if (cos_rot != 0.0)
      {
         theComputedScale.x = theXform1/cos_rot;
         theComputedScale.y = theXform4/cos_rot;
      }
      else
      {
         theComputedScale.x = theXform4;
         theComputedScale.y = theXform1;
      }
      thePixelType = ptype;
      theUnit = unit;
      is.close();
      result = true;
   }
   return result;
}

ossimTiffWorld::~ossimTiffWorld()
{
}

void ossimTiffWorld::forward(const ossimDpt& ip,
                             ossimDpt& transformedPoint)
{
   transformedPoint.x = ip.x*theXform1 + ip.y*theXform2 + theTranslation.x;
   transformedPoint.y = ip.x*theXform3 + ip.y*theXform4 + theTranslation.y;
}

bool ossimTiffWorld::saveToOssimGeom(ossimKeywordlist& kwl, const char* prefix)const
{
   ossimDpt scale(fabs(theXform1), fabs(theXform4));
   ossimDpt tie(theTranslation.x, theTranslation.y);

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

bool ossimTiffWorld::loadFromOssimGeom(const ossimKeywordlist& kwl, const char* prefix)
{
   theXform2 = 0.0;
   theXform3 = 0.0;

   const char* lookup;

   // Get the scale...
   lookup = kwl.find(prefix, ossimKeywordNames::PIXEL_SCALE_XY_KW);
   if (lookup)
   {
      ossimDpt scale;
      scale.toPoint(std::string(lookup));
      theXform1      = scale.x;
      theXform4      = -(scale.y);
   }
   else // BACKWARDS COMPATIBILITY LOOKUPS...
   {
      ossimString xscale = kwl.find(prefix, ossimKeywordNames::METERS_PER_PIXEL_X_KW);
      ossimString yscale = kwl.find(prefix, ossimKeywordNames::METERS_PER_PIXEL_Y_KW);
      theXform1 = xscale.toDouble();
      theXform4 = -(yscale.toDouble());
   }

   // Get the tie...
   lookup = kwl.find(prefix, ossimKeywordNames::TIE_POINT_XY_KW);
   if (lookup)
   {
      ossimDpt tie;
      tie.toPoint(std::string(lookup));
      theTranslation.x  = tie.x;
      theTranslation.y  = tie.y;
   }
   else // BACKWARDS COMPATIBILITY LOOKUPS...
   {
      ossimString easting  =
         kwl.find(prefix, ossimKeywordNames::TIE_POINT_EASTING_KW);
      ossimString northing =
         kwl.find(prefix, ossimKeywordNames::TIE_POINT_NORTHING_KW);
      theTranslation.x  = easting.toDouble();
      theTranslation.y  = northing.toDouble(); 
   }
   
   return true;
}

std::ostream& ossimTiffWorld::print(std::ostream& out) const
{
   out << setiosflags(ios::fixed) << setprecision(15)
       << theXform1     << "\n"
       << theXform2     << "\n"
       << theXform3     << "\n"
       << theXform4     << "\n"
       << theTranslation
       << endl;
   return out;
}

std::ostream& operator<<(std::ostream& out, const ossimTiffWorld& obj)
{
   return obj.print(out);
}

