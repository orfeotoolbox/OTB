//*******************************************************************
// Copyright (C) 2002 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Contains class definition for ossimLlxy.  This is a simple "latitude /
// longitude to x / y" projection.
// 
//*******************************************************************
//  $Id: ossimLlxyProjection.cpp,v 1.24 2005/09/09 19:23:58 gpotts Exp $

#include <projections/map_projections/ossimLlxyProjection.h>
#include <elevation/ossimElevManager.h>
#include <base/common/ossimKeywordNames.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/data_types/datum/ossimDatum.h>

// RTTI information for the ossimMapProjection
RTTI_DEF1(ossimLlxyProjection, "ossimLlxyProjection" , ossimMapProjection);

ossimLlxyProjection::ossimLlxyProjection()
{
   theDeltaLatPerPixel = 1.0;
   theDeltaLonPerPixel = 1.0;
   theUlEastingNorthing.y = OSSIM_DBL_NAN;
   theUlEastingNorthing.x = OSSIM_DBL_NAN;
   computeMetersPerPixel(theOrigin, 
			 theDeltaLatPerPixel,
			 theDeltaLonPerPixel,
			 theMetersPerPixel);
}

ossimLlxyProjection::ossimLlxyProjection(const ossimLlxyProjection& rhs)
   :
      ossimMapProjection(rhs)
{
   theOrigin              = rhs.theOrigin;
   theUlGpt               = rhs.theUlGpt;
   theUlEastingNorthing.y = OSSIM_DBL_NAN;
   theUlEastingNorthing.x = OSSIM_DBL_NAN;
   theDatum               = theOrigin.datum();
   theEllipsoid           = *(theDatum->ellipsoid());
   theDeltaLatPerPixel    = rhs.theDeltaLatPerPixel;
   theDeltaLonPerPixel    = rhs.theDeltaLonPerPixel;
   computeMetersPerPixel(theOrigin, 
			 theDeltaLatPerPixel,
			 theDeltaLonPerPixel,
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
   theDeltaLatPerPixel    = latSpacing;
   theDeltaLonPerPixel    = lonSpacing;
   computeMetersPerPixel(theOrigin, 
			 theDeltaLatPerPixel,
			 theDeltaLonPerPixel,
			 theMetersPerPixel);
}

ossimLlxyProjection::ossimLlxyProjection(const ossimEllipsoid& ellipsoid,
					 const ossimGpt& origin)
  :ossimMapProjection(ellipsoid, origin)
{
   theDeltaLatPerPixel = 1.0;
   theDeltaLonPerPixel = 1.0;
   theUlEastingNorthing.y = OSSIM_DBL_NAN;
   theUlEastingNorthing.x = OSSIM_DBL_NAN;
   computeMetersPerPixel(theOrigin, 
			 theDeltaLatPerPixel,
			 theDeltaLonPerPixel,
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
   
   if (theOrigin.datum() != gpt.datum())
   {
      // Apply datum shift if it's not the same.
      gpt.changeDatum(theOrigin.datum());
   }

   lineSampPt.line = (theUlGpt.latd() - gpt.latd()) / theDeltaLatPerPixel;
   lineSampPt.samp = (gpt.lond() - theUlGpt.lond()) / theDeltaLonPerPixel;
}

void ossimLlxyProjection::lineSampleToWorld(const ossimDpt& lineSampPt,
                                            ossimGpt&       worldPt) const
{
   // Start with the origin.  This will keep the origin's datum.
   worldPt = theUlGpt;
   
   double lat = theUlGpt.latd() - (lineSampPt.line * theDeltaLatPerPixel);
   double lon = theUlGpt.lond() + (lineSampPt.samp * theDeltaLonPerPixel);

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
   worldPt.hgt = ossimElevManager::instance()->getHeightAboveEllipsoid(worldPt);
}

std::ostream& ossimLlxyProjection::print(std::ostream& out) const
{
   out << setiosflags(ios::fixed) << setprecision(15)
       << "ossimLlxyProjection dump:"
       << "\norigin:  " << theOrigin
       << "\nlatitude spacing in decimal degrees:   " << theDeltaLatPerPixel
       << "\nlongitude spacing in decimal degrees:  " << theDeltaLonPerPixel
       << "\n\nossimMapProjection dump:\n" << endl;

   return ossimMapProjection::print(out);
}

void ossimLlxyProjection::setMetersPerPixel(const ossimDpt& pt)
{
  ossimMapProjection::setMetersPerPixel(pt);
  computeDegreesPerPixel(theOrigin,
			 theMetersPerPixel,
			 theDeltaLatPerPixel,
			 theDeltaLonPerPixel);
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

    const char* deltaLat = kwl.find(prefix, ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT);
    const char* deltaLon = kwl.find(prefix, ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON);
    const char* deltaX   = kwl.find(prefix, ossimKeywordNames::METERS_PER_PIXEL_X_KW);
    const char* deltaY   = kwl.find(prefix, ossimKeywordNames::METERS_PER_PIXEL_Y_KW);
    
    if (deltaLat&&deltaLon)
    {
       theDeltaLatPerPixel = ossimString(deltaLat).toDouble();
       theDeltaLonPerPixel = ossimString(deltaLon).toDouble();
       computeMetersPerPixel(theOrigin, 
                             theDeltaLatPerPixel,
                             theDeltaLonPerPixel,
                             theMetersPerPixel);
    }
    else if(deltaX&&deltaY)
    {
       theMetersPerPixel.x = ossimString(deltaX).toDouble();
       theMetersPerPixel.y = ossimString(deltaY).toDouble();
       
       computeDegreesPerPixel(theOrigin, 
                              theMetersPerPixel,
                              theDeltaLatPerPixel,
                              theDeltaLonPerPixel);
    }
    if(!deltaLat&&!deltaLon&&!deltaX&&!deltaY)
    {
       theDeltaLatPerPixel = 1.0;
       theDeltaLonPerPixel = 1.0;
       computeMetersPerPixel(theOrigin,
                             theDeltaLatPerPixel,
                             theDeltaLonPerPixel,
                             theMetersPerPixel);
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
            (theDeltaLatPerPixel == proj->theDeltaLatPerPixel) &&
            (theDeltaLonPerPixel == proj->theDeltaLonPerPixel) );
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
   theDeltaLatPerPixel = spacing;

   // Update the meters per pixel.
   ossimDpt pt = ossimGpt(0.0, 0.0).metersPerDegree();
   theMetersPerPixel.y = pt.y * theDeltaLatPerPixel;
}

void ossimLlxyProjection::setLonSpacing(double spacing)
{
   theDeltaLonPerPixel = spacing;

   // Update the meters per pixel.
   ossimDpt pt = ossimGpt(0.0, 0.0).metersPerDegree();
   theMetersPerPixel.x = pt.x * theDeltaLonPerPixel;
}

bool ossimLlxyProjection::isGeographic() const
{
   return true;
}

void ossimLlxyProjection::setOrigin(const ossimGpt& origin)
{
   theOrigin = origin;
}

double ossimLlxyProjection::getLatSpacing() const
{
   return theDeltaLatPerPixel;
}

double ossimLlxyProjection::getLonSpacing() const
{
   return theDeltaLonPerPixel;
}
