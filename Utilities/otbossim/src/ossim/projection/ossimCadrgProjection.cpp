//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimCadrgProjection.cpp 17815 2010-08-03 13:23:14Z dburken $
#include <ossim/projection/ossimCadrgProjection.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/elevation/ossimElevManager.h>
#include <ossim/base/ossimDatum.h>

RTTI_DEF1(ossimCadrgProjection, "ossimCadrgProjection", ossimMapProjection)

double ossimCadrgProjection::theOldZoneExtents[] = {0.0, 32.0, 48.0, 56.0, 64.0,
                                                    68.0, 72.0, 76.0, 80.0, 90.0};
double ossimCadrgProjection::theCadrgArcA[] = { 369664, 302592, 245760, 199168,
					       163328, 137216, 110080, 82432 };

double ossimCadrgProjection::theNorthLimit = 90.0*M_PI/180.0;
double ossimCadrgProjection::theSouthLimit = -90.0*M_PI/180.0;

ossimCadrgProjection::ossimCadrgProjection()
   :ossimMapProjection(ossimEllipsoid(), ossimGpt()),
    theCadrgZone(1),
    theMapScale(5000000),
    theWidth(0.0),
    theHeight(0.0)
{
   computeParameters();
}

ossimCadrgProjection::~ossimCadrgProjection()
{
   
}

ossimObject *ossimCadrgProjection::dup()const
{
   return new ossimCadrgProjection(*this);
}


ossimDpt ossimCadrgProjection::worldToLineSample(const ossimGpt &worldPoint)    const
{
   ossimDpt lineSample;

   worldToLineSample(worldPoint, lineSample);

   return lineSample;
}

ossimGpt ossimCadrgProjection::inverse(const ossimDpt& /* eastingNorthing */)const
{
   double lat=0.0;
   double lon=0.0;

   return ossimGpt(lat*DEG_PER_RAD, lon*DEG_PER_RAD, 0.0, theDatum);  
}

ossimDpt ossimCadrgProjection::forward(const ossimGpt &latLon)const
{
   double easting  = 0.0;
   double northing = 0.0;
   ossimGpt gpt = latLon;
   
   if (theDatum)
   {
      if (theDatum->code() != latLon.datum()->code())
      {
         gpt.changeDatum(theDatum); // Shift to our datum.
      }
   }
   
   return ossimDpt(easting, northing);
}

ossimGpt ossimCadrgProjection::lineSampleToWorld(const ossimDpt &projectedPoint)const
{
   ossimGpt worldPoint;

   lineSampleToWorld(projectedPoint, worldPoint);

   return worldPoint;
}

void ossimCadrgProjection::worldToLineSample(const ossimGpt &worldPoint,
                                            ossimDpt&       lineSample)const
{
   double lat = worldPoint.latd();
   double lon = worldPoint.lond();
   double centerLat = theOrigin.latd();
   double centerLon = theOrigin.lond()*DEG_PER_RAD;
   
   lineSample.y = (centerLat - lat)/90.0*thePixelConstant.y;
   lineSample.x = (lon - centerLon)/360.0*thePixelConstant.x;

   lineSample = lineSample - theUlLineSample;
}

void ossimCadrgProjection::lineSampleToWorld(const ossimDpt &projectedPoint,
                                            ossimGpt& gpt)const
{
   gpt = theOrigin;


   ossimDpt adjustedPixel(projectedPoint.x + theUlLineSample.x,
                          projectedPoint.y + theUlLineSample.y);

//   double lat = gpt.latd() - (90/thePixelConstant.y)*adjustedPixel.y;
//   double lon = gpt.lond() + (360/thePixelConstant.x)*adjustedPixel.x;
   double lat = gpt.latd() - (90/thePixelConstant.y)*adjustedPixel.y;
   double lon = gpt.lond() + (360/thePixelConstant.x)*adjustedPixel.x;
   
   gpt.latd(lat);
   gpt.lond(lon);

   gpt.clampLat(-90, 90);
   gpt.clampLon(-180, 180);
}

double ossimCadrgProjection::computeXPixConstant(double scale,
                                                 long zone)const
{
   double adrgscale = 1000000/scale;
   
   // E-W pixel constant
   double x_pix = (double) adrgscale*theCadrgArcA[zone-1] / 512.0;
   
   // Increase, if necessary, to the next highest integer value
   x_pix = ceil(x_pix);
   x_pix = x_pix * 1.33333;//(512*100)/(150*256);
   
   // Round the final result.
   x_pix = ossim::round<int>(x_pix);
   
   return x_pix*256.0;
   
}

double ossimCadrgProjection::computeYPixConstant(double scale)const
{
   double adrgscale = 1000000/scale;
   const long CADRG_ARC_B = 400384;
   
   double y_pix = (double) adrgscale * CADRG_ARC_B / 512.0;
   
   // Increase, if necessary, to the next highest integer value
   y_pix = ceil(y_pix);

   y_pix = y_pix * 0.33333;//(512*100)/(4*150*256);
   
   // Round the final result.
   y_pix = ossim::round<int>(y_pix);
   
   return y_pix*256.0;
}

void ossimCadrgProjection::computeParameters()
{
   theUlLineSample = ossimDpt(0,0);

   thePixelConstant.y = computeYPixConstant(theMapScale);
   thePixelConstant.x = computeXPixConstant(theMapScale,
                                            theCadrgZone);
   double height = theHeight;
   double width  = theWidth;
   
   if(width > thePixelConstant.x)
   {
      width  = thePixelConstant.x;
   }
   
   if(height > thePixelConstant.y)
   {
      height = thePixelConstant.y;
   }


   theUlLineSample.x = -width/2.0;
   theUlLineSample.y = -height/2.0;
}

bool ossimCadrgProjection::saveState(ossimKeywordlist& kwl,
                                    const char* prefix)const
{
   bool result = ossimProjection::saveState(kwl, prefix);

   kwl.add(prefix,
           ossimKeywordNames::ZONE_KW,
           theCadrgZone,
           true);
   
   kwl.add(prefix,
           "map_scale",
           theMapScale,
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_LINES_KW,
           theHeight,
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_SAMPLES_KW,
           theWidth,
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::UL_LAT_KW,
           theUlGpt.latd(),
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::UL_LON_KW,
           theUlGpt.lond(),
           true);

   kwl.add(prefix,
           ossimKeywordNames::LL_LAT_KW,
           theLlGpt.latd(),
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::LL_LON_KW,
           theLlGpt.lond(),
           true);

   kwl.add(prefix,
           ossimKeywordNames::LR_LAT_KW,
           theLrGpt.latd(),
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::LR_LON_KW,
           theLrGpt.lond(),
           true);

   kwl.add(prefix,
           ossimKeywordNames::UR_LAT_KW,
           theUrGpt.latd(),
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::UR_LON_KW,
           theUrGpt.lond(),
           true);

  
   
   if(theDatum)
   {
      kwl.add(prefix,
              ossimKeywordNames::DATUM_KW,
              theDatum->code(),
              true);
   }

   return result;
}


bool ossimCadrgProjection::loadState(const ossimKeywordlist& kwl,
                                    const char* prefix)
{
   ossimProjection::loadState(kwl, prefix);

   
   const char* lookup = kwl.find(prefix, ossimKeywordNames::UL_LAT_KW);
   if(lookup)
   {
      theUlGpt.latd(ossimString(lookup).toDouble());
   }
   else
    {
       theUlGpt.latd(90.0);
    }

    lookup = kwl.find(prefix, ossimKeywordNames::UL_LON_KW);
    if(lookup)
    {
       theUlGpt.lond(ossimString(lookup).toDouble());
    }
    else
    {
       theUlGpt.lond(-180.0);
    }

    lookup = kwl.find(prefix, ossimKeywordNames::LL_LAT_KW);
    if(lookup)
    {
       theLlGpt.latd(ossimString(lookup).toDouble());
    }
    else
    {
       theLlGpt.latd(0.0);
    }

    lookup = kwl.find(prefix, ossimKeywordNames::LL_LON_KW);
    if(lookup)
    {
       theLlGpt.lond(ossimString(lookup).toDouble());
    }
   else
   {
      theLlGpt.lond(-180.0);
   }

   lookup = kwl.find(prefix, ossimKeywordNames::LR_LAT_KW);
   if(lookup)
   {
      theLrGpt.latd(ossimString(lookup).toDouble());
   }
   else
   {
      theLrGpt.latd(0.0);
   }

   lookup = kwl.find(prefix, ossimKeywordNames::LR_LON_KW);
   if(lookup)
   {
      theLrGpt.lond(ossimString(lookup).toDouble());
   }
   else
   {
      theLrGpt.lond(180.0);
   }

   lookup = kwl.find(prefix, ossimKeywordNames::LR_LAT_KW);
   if(lookup)
   {
      theLrGpt.latd(ossimString(lookup).toDouble());
   }
   else
   {
      theLrGpt.latd(0.0);
   }

   lookup = kwl.find(prefix, ossimKeywordNames::LR_LON_KW);
   if(lookup)
   {
      theLrGpt.lond(ossimString(lookup).toDouble());
   }
   else
   {
      theLrGpt.lond(180.0);
   }

   lookup = kwl.find(prefix, ossimKeywordNames::UR_LAT_KW);
   if(lookup)
   {
      theUrGpt.latd(ossimString(lookup).toDouble());
   }
   else
   {
      theUrGpt.latd(90.0);
   }

   lookup = kwl.find(prefix, ossimKeywordNames::UR_LON_KW);
   if(lookup)
   {
      theUrGpt.lond(ossimString(lookup).toDouble());
   }
   else
   {
      theUrGpt.lond(180.0);
   }

   const char* zone = kwl.find(prefix,
                               ossimKeywordNames::ZONE_KW);
   if(zone)
   {
      theCadrgZone = ossimString(zone).toLong();
   }
   const char* mapScale = kwl.find(prefix,
                                   "map_scale");
   if(mapScale)
   {
      theMapScale = ossimString(mapScale).toDouble();
   }
   const char *height = kwl.find(prefix,
                                 ossimKeywordNames::NUMBER_LINES_KW);
   const char *width  = kwl.find(prefix,
                                 ossimKeywordNames::NUMBER_SAMPLES_KW);
   if(height)
   {
      theHeight = ossimString(height).toDouble();
   }
   
   if(width)
   {
      theWidth  = ossimString(width).toDouble();
   }

   computeParameters();

   return true;
}

//*************************************************************************************************
//! Returns TRUE if principal parameters are within epsilon tolerance.
//*************************************************************************************************
bool ossimCadrgProjection::operator==(const ossimProjection& proj) const
{
   if (!ossimMapProjection::operator==(proj))
      return false;

   ossimCadrgProjection* p = PTR_CAST(ossimCadrgProjection, &proj);
   if (!p) return false;

   if (theUlGpt != p->theUlGpt) return false;
   if (theLlGpt != p->theLlGpt) return false;
   if (theLrGpt != p->theLrGpt) return false;
   if (theUrGpt != p->theUrGpt) return false;
   if (!ossim::almostEqual(theWidth,p->theWidth)) return false;
   if (!ossim::almostEqual(theHeight,p->theHeight)) return false;
   if (theCadrgZone != p->theCadrgZone) return false;

   return true;
}
