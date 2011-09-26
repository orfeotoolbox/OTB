//*******************************************************************
// Copyright (C) 2002 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
//
// Contains class definition for ossimLlxy.  This is a simple "latitude /
// longitude to x / y" projection.
// 
//*******************************************************************
//  $Id: ossimLlxyProjection.cpp 20060 2011-09-07 12:33:46Z gpotts $

#include <ossim/projection/ossimLlxyProjection.h>
#include <ossim/elevation/ossimElevManager.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimDatum.h>

// RTTI information for the ossimMapProjection
RTTI_DEF1(ossimLlxyProjection, "ossimLlxyProjection" , ossimMapProjection);

// About 1 meter.
static const ossim_float64 DEFAULT_DEGREES_PER_PIXEL = 8.9831528412e-006;

ossimLlxyProjection::ossimLlxyProjection()
{
  // set to about 1 meter per pixel
   theDegreesPerPixel.y = DEFAULT_DEGREES_PER_PIXEL;
   theDegreesPerPixel.x = DEFAULT_DEGREES_PER_PIXEL;
   theUlEastingNorthing.y = ossim::nan();
   theUlEastingNorthing.x = ossim::nan();
   computeMetersPerPixel(theOrigin, 
			 theDegreesPerPixel.y,
			 theDegreesPerPixel.x,
			 theMetersPerPixel);
}

ossimLlxyProjection::ossimLlxyProjection(const ossimLlxyProjection& rhs)
   :
      ossimMapProjection(rhs)
{
   theOrigin              = rhs.theOrigin;
   theUlGpt               = rhs.theUlGpt;
   theUlEastingNorthing.y = ossim::nan();
   theUlEastingNorthing.x = ossim::nan();
   theDatum               = theOrigin.datum();
   theEllipsoid           = *(theDatum->ellipsoid());
   theDegreesPerPixel.y    = rhs.theDegreesPerPixel.y;
   theDegreesPerPixel.x    = rhs.theDegreesPerPixel.x;
   computeMetersPerPixel(theOrigin, 
			 theDegreesPerPixel.y,
			 theDegreesPerPixel.x,
			 theMetersPerPixel);
}

ossimLlxyProjection::ossimLlxyProjection(const ossimGpt& origin,
                                         double latSpacing,
                                         double lonSpacing)
   :
      ossimMapProjection()
{
   theOrigin              = origin;
   theUlGpt               = origin;
   theUlEastingNorthing.y = 0.0;
   theUlEastingNorthing.x = 0.0;
   theDatum               = theOrigin.datum();
   theEllipsoid           = *(theDatum->ellipsoid());
   theDegreesPerPixel.y    = latSpacing;
   theDegreesPerPixel.x    = lonSpacing;
   computeMetersPerPixel(theOrigin, 
			 theDegreesPerPixel.y,
			 theDegreesPerPixel.x,
			 theMetersPerPixel);
}

ossimLlxyProjection::ossimLlxyProjection(const ossimEllipsoid& ellipsoid,
					 const ossimGpt& origin)
  :ossimMapProjection(ellipsoid, origin)
{
   theDegreesPerPixel.y = 1.0;
   theDegreesPerPixel.x = 1.0;
   theUlEastingNorthing.y = ossim::nan();
   theUlEastingNorthing.x = ossim::nan();
   computeMetersPerPixel(theOrigin, 
			 theDegreesPerPixel.y,
			 theDegreesPerPixel.x,
			 theMetersPerPixel);
}

ossimLlxyProjection::~ossimLlxyProjection()
{
}

ossimObject* ossimLlxyProjection::dup()const
{
   return new ossimLlxyProjection(*this);
}

//*****************************************************************************
//  METHOD: ossimMapProjection::computeDegreesPerPixel
//  
//*****************************************************************************
void ossimLlxyProjection::computeDegreesPerPixel(const ossimGpt& ground,
                                                const ossimDpt& metersPerPixel,
                                                double &deltaLat,
                                                double &deltaLon)
{
   ossimDpt mpd = ground.metersPerDegree();
   ossimDpt dpm(1.0/mpd.x,
                1.0/mpd.y);
   deltaLat = metersPerPixel.y*dpm.y;
   deltaLon = metersPerPixel.x*dpm.x;
}

//*****************************************************************************
//  METHOD: ossimMapProjection::computeMetersPerPixel
//  
//*****************************************************************************
void ossimLlxyProjection::computeMetersPerPixel(const ossimGpt& center,
						  double deltaDegreesPerPixelLat,
						  double deltaDegreesPerPixelLon,
						  ossimDpt &metersPerPixel)
{
  metersPerPixel = center.metersPerDegree();
  metersPerPixel.x *= deltaDegreesPerPixelLon;
  metersPerPixel.y *= deltaDegreesPerPixelLat;
}

void ossimLlxyProjection::worldToLineSample(const ossimGpt& worldPoint,
                                            ossimDpt&       lineSampPt) const
{
   ossimGpt gpt = worldPoint;
   
   if (*theOrigin.datum() != *gpt.datum())
   {
      // Apply datum shift if it's not the same.
      gpt.changeDatum(theOrigin.datum());
   }

   lineSampPt.line = (theUlGpt.latd() - gpt.latd()) / theDegreesPerPixel.y;
   lineSampPt.samp = (gpt.lond() - theUlGpt.lond()) / theDegreesPerPixel.x;
}

void ossimLlxyProjection::lineSampleToWorld(const ossimDpt& lineSampPt,
                                            ossimGpt&       worldPt) const
{
   worldPt.makeNan();
   // Start with the origin.  This will keep the origin's datum.
   worldPt.datum(theOrigin.datum());
   
   double lat = theUlGpt.latd() - (lineSampPt.line * theDegreesPerPixel.y);
   double lon = theUlGpt.lond() + (lineSampPt.samp * theDegreesPerPixel.x);

   //---
   // Assuming the origin had a lon between -180 and 180 and lat between -90
   // and 90.
   //---
//    if (lon > 180.0)
//    {
//       lon -= 360.0;
//    }
//    else if (lon < -180.0)
//    {
//       lon += 360.0;
//    }
//    if (lat > 90.0)
//    {
//       lat -= 90.0;
//    }
//    else if (lat < -90.0)
//    {
//       lat = -180.0 - lat;
//    }

   worldPt.latd(lat);
   worldPt.lond(lon);
   if(theElevationLookupFlag)
   {
      worldPt.hgt = ossimElevManager::instance()->getHeightAboveEllipsoid(worldPt);
   }
}

std::ostream& ossimLlxyProjection::print(std::ostream& out) const
{
   out << setiosflags(ios::fixed) << setprecision(15)
       << "ossimLlxyProjection dump:"
       << "\norigin:  " << theOrigin
       << "\nlatitude spacing in decimal degrees:   " << theDegreesPerPixel.y
       << "\nlongitude spacing in decimal degrees:  " << theDegreesPerPixel.x
       << "\n\nossimMapProjection dump:\n" << endl;

   return ossimMapProjection::print(out);
}

void ossimLlxyProjection::setMetersPerPixel(const ossimDpt& pt)
{
  ossimMapProjection::setMetersPerPixel(pt);
  computeDegreesPerPixel(theOrigin,
			 theMetersPerPixel,
			 theDegreesPerPixel.y,
			 theDegreesPerPixel.x);
}

bool ossimLlxyProjection::saveState(ossimKeywordlist& kwl,
                                    const char* prefix) const
{
   // Base class...
   ossimMapProjection::saveState(kwl, prefix);

   return true;
}

bool ossimLlxyProjection::loadState(const ossimKeywordlist& kwl,
                                    const char* prefix)
{
   // Base class...
   ossimMapProjection::loadState(kwl, prefix);

   if (theOrigin.hasNans() == false)
   {
      if ( (theDegreesPerPixel.hasNans() == false) &&
           theMetersPerPixel.hasNans() )
      {
         // Compute meters per pixel from origin and decimal degrees.
         computeMetersPerPixel(theOrigin, 
                               theDegreesPerPixel.y,
                               theDegreesPerPixel.x,
                               theMetersPerPixel);
      }
      else if( (theMetersPerPixel.hasNans() == false) &&
               theDegreesPerPixel.hasNans() )
      {
         // Compute decimal degrees per pixel from origin and meters.
         computeDegreesPerPixel(theOrigin, 
                                theMetersPerPixel,
                                theDegreesPerPixel.y,
                                theDegreesPerPixel.x);
      }
      else
      {
         // Assign some value.
         theDegreesPerPixel.y = DEFAULT_DEGREES_PER_PIXEL;
         theDegreesPerPixel.x = DEFAULT_DEGREES_PER_PIXEL;
         computeMetersPerPixel(theOrigin,
                               theDegreesPerPixel.y,
                               theDegreesPerPixel.x,
                               theMetersPerPixel);
      }
   }
    
   return true;
}

bool ossimLlxyProjection::operator==(const ossimProjection& projection) const
{
   const ossimLlxyProjection* proj
      = PTR_CAST(ossimLlxyProjection, &projection);

   if(!proj)
   {
      return false;
   }

   return ( // (theOrigin     == proj->theOrigin)     &&  // tmp... fix gpt!
            (theDegreesPerPixel.y == proj->theDegreesPerPixel.y) &&
            (theDegreesPerPixel.x == proj->theDegreesPerPixel.x) );
}


ossimDpt ossimLlxyProjection::forward(const ossimGpt &worldPoint) const
{
   ossimDpt result;

   worldToLineSample(worldPoint, result);

   return result;
}

ossimGpt ossimLlxyProjection::inverse(const ossimDpt &projectedPoint) const
{
   ossimGpt result;
   
   lineSampleToWorld(projectedPoint, result);
   
   return result;
}

void ossimLlxyProjection::setLatSpacing(double spacing)
{
   theDegreesPerPixel.y = spacing;

   // Update the meters per pixel.
   ossimDpt pt = ossimGpt(0.0, 0.0).metersPerDegree();
   theMetersPerPixel.y = pt.y * theDegreesPerPixel.y;
}

void ossimLlxyProjection::setLonSpacing(double spacing)
{
   theDegreesPerPixel.x = spacing;

   // Update the meters per pixel.
   ossimDpt pt = ossimGpt(0.0, 0.0).metersPerDegree();
   theMetersPerPixel.x = pt.x * theDegreesPerPixel.x;
}

bool ossimLlxyProjection::isGeographic() const
{
   return true;
}

double ossimLlxyProjection::getLatSpacing() const
{
   return theDegreesPerPixel.y;
}

double ossimLlxyProjection::getLonSpacing() const
{
   return theDegreesPerPixel.x;
}
