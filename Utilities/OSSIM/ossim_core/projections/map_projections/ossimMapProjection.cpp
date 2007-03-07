//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description:
//
// Base class for all map projections.
// 
//*******************************************************************
//  $Id: ossimMapProjection.cpp,v 1.108 2005/12/25 17:11:52 dburken Exp $

#include <iostream>
#include <iomanip>

#include <projections/map_projections/ossimMapProjection.h>
#include <base/common/ossimKeywordNames.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/factory/ossimDatumFactory.h>
#include <base/data_types/ossimDpt.h>
#include <base/data_types/ossimGpt.h>
#include <base/data_types/datum/ossimDatum.h>
#include <base/data_types/ellipse/ossimEllipsoid.h>
#include <base/data_types/ossimString.h>
#include <elevation/ossimElevManager.h>
#include <base/data_types/ossimMatrix3x3.h>
#include <base/misc/ossimUnitConversionTool.h>

// RTTI information for the ossimMapProjection
RTTI_DEF1(ossimMapProjection, "ossimMapProjection" , ossimProjection);


ossimMapProjection::ossimMapProjection(const ossimEllipsoid& ellipsoid,
                                       const ossimGpt& origin)
   :theEllipsoid(ellipsoid),
    theOrigin(origin),
    theDatum(origin.datum()), // force no shifting
    theMetersPerPixel(1, 1),
    theUlGpt(0, 0),
    theUlEastingNorthing(0, 0),
    theDeltaLatPerPixel(OSSIM_DBL_NAN),
    theDeltaLonPerPixel(OSSIM_DBL_NAN)
{
   theUlGpt = theOrigin;
   theUlEastingNorthing.makeNan();

   thePcsCode = 0;
}

ossimMapProjection::ossimMapProjection(const ossimMapProjection& src)
      : ossimProjection(src),
        theEllipsoid(src.theEllipsoid),
        theOrigin(src.theOrigin),
        theDatum(src.theDatum),
        theMetersPerPixel(src.theMetersPerPixel),
        theUlGpt(src.theUlGpt),
        theUlEastingNorthing(src.theUlEastingNorthing),
        theDeltaLatPerPixel(src.theDeltaLatPerPixel),
        theDeltaLonPerPixel(src.theDeltaLonPerPixel),
        thePcsCode(src.thePcsCode)
{
}


ossimMapProjection::~ossimMapProjection()
{
}

ossimGpt ossimMapProjection::origin()const
{
   return theOrigin;
}

void ossimMapProjection::setPcsCode(ossim_uint16 pcsCode)
{
  thePcsCode = pcsCode;
}

ossim_uint16 ossimMapProjection::getPcsCode()const
{
  return thePcsCode;
}

ossimString ossimMapProjection::getProjectionName() const
{
   return getClassName();
}

double ossimMapProjection::getA() const
{
   return theEllipsoid.getA();
}

double ossimMapProjection::getB() const
{
   return theEllipsoid.getB();
}

double ossimMapProjection::getF() const
{
   return theEllipsoid.getFlattening();
}

ossimDpt ossimMapProjection::getMetersPerPixel() const
{
   return theMetersPerPixel;
}

ossimDpt ossimMapProjection::getDecimalDegreesPerPixel() const
{
   return ossimDpt(theDeltaLonPerPixel, theDeltaLatPerPixel);
}

const ossimDpt& ossimMapProjection::getUlEastingNorthing() const
{
   return theUlEastingNorthing;
}

const ossimGpt&   ossimMapProjection::getUlGpt() const
{
   return theUlGpt;
}

const ossimDatum* ossimMapProjection::getDatum() const
{
   return theDatum;
}

bool ossimMapProjection::isGeographic()const
{
   return false;
}

void ossimMapProjection::setEllipsoid(const ossimEllipsoid& ellipsoid)
{
   theEllipsoid = ellipsoid; update();
}

void ossimMapProjection::setAB(double a, double b)
{
   theEllipsoid.setA(a); theEllipsoid.setB(b); update();
}

void ossimMapProjection::setDatum(const ossimDatum* datum)
{
   if(datum)
   {
      theDatum = datum; setEllipsoid( *(datum->ellipsoid()));
   }
}

void ossimMapProjection::setOrigin(const ossimGpt& origin)
{
   theOrigin = origin; update();
}

//*****************************************************************************
//  METHOD: ossimMapProjection::assign
//
//*****************************************************************************
void ossimMapProjection::assign(const ossimProjection &aProjection)
{
   if(&aProjection!=this)
   {
      ossimKeywordlist kwl;

      aProjection.saveState(kwl);
      loadState(kwl);
   }
}

//*****************************************************************************
//  METHOD: ossimMapProjection::update
//
//*****************************************************************************
void ossimMapProjection::update()
{
   // if the delta lat and lon per pixel is set then
   // check to see if the meters were set.
   //
   if((theDeltaLatPerPixel != OSSIM_DBL_NAN)&&
      (theDeltaLonPerPixel != OSSIM_DBL_NAN))
   {
      if(theMetersPerPixel.hasNans())
      {
         computeMetersPerPixel(theDeltaLatPerPixel,
                               theDeltaLonPerPixel,
                               theMetersPerPixel);
      }
   }
   else if(!theMetersPerPixel.hasNans())
   {
      computeDegreesPerPixel(theOrigin,
                             theMetersPerPixel,
                             theDeltaLatPerPixel,
                             theDeltaLonPerPixel);
   }

   // compute the tie points if not already computed
   //
   if((theUlGpt.latd()==OSSIM_DBL_NAN)||
      (theUlGpt.lond()==OSSIM_DBL_NAN))
   {
      theUlGpt = theOrigin;
   }
    if(theUlEastingNorthing.hasNans())
    {
//       theUlEastingNorthing.x = 0.0;
//       theUlEastingNorthing.y = 0.0;
      if((theUlGpt.latd()!=OSSIM_DBL_NAN)&&
 	(theUlGpt.lond()!=OSSIM_DBL_NAN))
      {
         theUlEastingNorthing = forward(theUlGpt);
      }
    }
}

void ossimMapProjection::applyScale(const ossimDpt& scale,
                                    bool recenterTiePoint)
{
   if (!recenterTiePoint)
   {
      theDeltaLonPerPixel *= scale.x;
      theDeltaLatPerPixel *= scale.y;
      theMetersPerPixel.x *= scale.x;
      theMetersPerPixel.y *= scale.y;
   }
   else
   {
      if (isGeographic())
      {
         // Get the current tie.
         ossimGpt tie = getUlGpt();
         
         // Shift it to the upper left edge of the pixel.
         tie.lond( tie.lond() - theDeltaLonPerPixel/2.0);
         tie.latd( tie.latd() + theDeltaLatPerPixel/2.0);
         
         // Apply the scale.
         theDeltaLonPerPixel *= scale.x;
         theDeltaLatPerPixel *= scale.y;
         theMetersPerPixel.x *= scale.x;
         theMetersPerPixel.y *= scale.y;
         
         // Now shift it back to center of the new tie.
         tie.lond( tie.lond() + theDeltaLonPerPixel/2.0);
         tie.latd( tie.latd() - theDeltaLatPerPixel/2.0);
         
         // Set the tie to new point.
         setUlGpt(tie);
      }
      else
      {
         // Get the current tie point.
         ossimDpt tie = getUlEastingNorthing();
         
         // Shift it to the upper left edge of the pixel.
         tie.x -= theMetersPerPixel.x/2.0;
         tie.y += theMetersPerPixel.y/2.0;
         
         // Apply the scale.
         theMetersPerPixel.x *= scale.x;
         theMetersPerPixel.y *= scale.y;
         theDeltaLonPerPixel *= scale.x;
         theDeltaLatPerPixel *= scale.y;
         
         // Now shift it back to center of the new tie.
         tie.x += theMetersPerPixel.x/2.0;
         tie.y -= theMetersPerPixel.y/2.0;
         
         // Set the tie to new point.
         setUlEastingNorthing(tie);
      }
   }
}

//*****************************************************************************
//  METHOD: ossimMapProjection::worldToLineSample
//
//*****************************************************************************
ossimDpt ossimMapProjection::worldToLineSample(const ossimGpt &worldPoint)const
{
   ossimDpt result;

   worldToLineSample(worldPoint, result);

   return result;
}

//*****************************************************************************
//  METHOD: ossimMapProjection::lineSampleToWorld
//
//*****************************************************************************
ossimGpt ossimMapProjection::lineSampleToWorld(const ossimDpt &lineSample)const
{
   ossimGpt result;

   lineSampleToWorld(lineSample, result);

   return result;
}

//*****************************************************************************
//  METHOD: ossimMapProjection::worldToLineSample
//
//*****************************************************************************
void ossimMapProjection::worldToLineSample(const ossimGpt &worldPoint,
                                           ossimDpt&       lineSample)const
{
   // make sure our tie point is good and world point
   // is good.
   //
   if(theUlEastingNorthing.isNan()||
      worldPoint.isLatNan() || worldPoint.isLonNan())
   {
      lineSample.makeNan();
      return;
   }
   // initialize line sample
   //   lineSample = ossimDpt(0,0);

   // I am commenting this code out because I am going to
   // move it to the ossimImageViewProjectionTransform.
   //
   // see if we have a datum set and if so
   // shift the world to our datum.  If not then
   // find the easting northing value for the world
   // point.
    if(theDatum)
    {
       ossimGpt gpt = worldPoint;

       gpt.changeDatum(theDatum);

       // lineSample is currently in easting northing
       // and will need to be converted to line sample.
       lineSample = forward(gpt);
    }
    else
    {
       // lineSample is currently in easting northing
       // and will need to be converted to line sample.
       lineSample = forward(worldPoint);
    }

   // check the final result to make sure there were no
   // problems.
   //
   if(!lineSample.isNan())
   {
//       if(!isIdentityMatrix())
//       {
//          ossimDpt temp = lineSample;

//          lineSample.x = theInverseTrans[0][0]*temp.x+
//                         theInverseTrans[0][1]*temp.y+
//                         theInverseTrans[0][2];

//          lineSample.y = theInverseTrans[1][0]*temp.x+
//                         theInverseTrans[1][1]*temp.y+
//                         theInverseTrans[1][2];
//       }
//       else
      {
         lineSample.x = ((lineSample.x  - theUlEastingNorthing.x)/theMetersPerPixel.x);

         // We must remember that the Northing is negative since the positive
         // axis for an image is assumed to go down since it's image space.
         lineSample.y = (-(lineSample.y - theUlEastingNorthing.y)/theMetersPerPixel.y);
      }
   }
}

//*****************************************************************************
//  METHOD: ossimMapProjection::lineSampleHeightToWorld
//
//*****************************************************************************
void ossimMapProjection::lineSampleHeightToWorld(const ossimDpt &lineSample,
                                                 const double&  hgtEllipsoid,
                                                 ossimGpt&      gpt)const
{
   //
   // make sure that the passed in lineSample is good and
   // check to make sure our easting northing is good so
   // we can compute the line sample.
   //
   //
   if(lineSample.hasNans()||theUlEastingNorthing.hasNans())
   {
      gpt.makeNan();
      return;
   }
   ossimDpt eastingNorthing;

   eastingNorthing = (theUlEastingNorthing);
   
   eastingNorthing.x += (lineSample.x*theMetersPerPixel.x);

   //
   // Note:  the Northing is positive up.  In image space
   // the positive axis is down so we must multiply by
   // -1
   //
   eastingNorthing.y += (-lineSample.y*theMetersPerPixel.y);


   //
   // now invert the meters into a ground point.
   //
   gpt = inverse(eastingNorthing);
   gpt.datum(theDatum);

   if(gpt.isLatNan() && gpt.isLonNan())
   {
      gpt.makeNan();
   }
   else
   {
      gpt.clampLat(-90, 90);
      gpt.clampLon(-180, 180);

      // Finally assign the specified height:
      gpt.hgt = hgtEllipsoid;
   }
}

//*****************************************************************************
//  METHOD: ossimMapProjection::lineSampleToWorld
//
//  Implements the base class pure virtual. Simply calls lineSampleToWorld()
//  and assigns argument height to the resultant groundpoint.
//
//*****************************************************************************
void ossimMapProjection::lineSampleToWorld (const ossimDpt& lineSampPt,
                                            ossimGpt&       worldPt) const
{
   double null_elev = OSSIM_DBL_NAN;

   lineSampleHeightToWorld(lineSampPt, null_elev, worldPt);

   worldPt.hgt =  ossimElevManager::instance()->getHeightAboveEllipsoid(worldPt);
}

//*****************************************************************************
//  METHOD: ossimMapProjection::lineSampleToEastingNorthing
//
//*****************************************************************************
void ossimMapProjection::lineSampleToEastingNorthing(const ossimDpt& lineSample,
                                                     ossimDpt&       eastingNorthing)const
{
   // make sure that the passed in lineSample is good and
   // check to make sure our easting northing is good so
   // we can compute the line sample.
   //
   if(lineSample.hasNans()||theUlEastingNorthing.hasNans())
   {
      eastingNorthing.makeNan();
      return;
   }
   ossimDpt deltaPoint = lineSample;

   eastingNorthing.x = theUlEastingNorthing.x + deltaPoint.x*theMetersPerPixel.x;
   eastingNorthing.y = theUlEastingNorthing.y + (-deltaPoint.y)*theMetersPerPixel.y ;

   //   eastingNorthing.x += (lineSample.x*theMetersPerPixel.x);

   // Note:  the Northing is positive up.  In image space
   // the positive axis is down so we must multiply by
   // -1
   //   eastingNorthing.y += (-lineSample.y*theMetersPerPixel.y);
}

void ossimMapProjection::setMetersPerPixel(const ossimDpt& gsd)
{
   theMetersPerPixel=gsd;
   computeDegreesPerPixel(theOrigin,
                          theMetersPerPixel,
                          theDeltaLatPerPixel,
                          theDeltaLonPerPixel);
}

void ossimMapProjection::setDecimalDegreesPerPixel(const ossimDpt& gsd)
{
   theDeltaLatPerPixel = gsd.lat;
   theDeltaLonPerPixel = gsd.lon;

   computeMetersPerPixel(theOrigin,
                         gsd.lat,
                         gsd.lon,
                         theMetersPerPixel);
}

void ossimMapProjection::eastingNorthingToWorld(const ossimDpt& eastingNorthing,
                                                ossimGpt&       worldPt)const
{
   ossimDpt lineSample;
   eastingNorthingToLineSample(eastingNorthing, lineSample);
   lineSampleToWorld(lineSample, worldPt);
}


//*****************************************************************************
//  METHOD: ossimMapProjection::eastingNorthingToLineSample
//
//*****************************************************************************
void ossimMapProjection::eastingNorthingToLineSample(const ossimDpt& eastingNorthing,
                                                     ossimDpt&       lineSample)const
{
   if(eastingNorthing.hasNans())
   {
      lineSample.makeNan();
      return;
   }
   // check the final result to make sure there were no
   // problems.
   //
   if(!eastingNorthing.isNan())
   {
     lineSample.x = (eastingNorthing.x - theUlEastingNorthing.x)/theMetersPerPixel.x;

     // We must remember that the Northing is negative since the positive
     // axis for an image is assumed to go down since it's image space.
     lineSample.y = (-(eastingNorthing.y-theUlEastingNorthing.y))/theMetersPerPixel.y;
   }
}

//*****************************************************************************
//  METHOD: ossimMapProjection::setUlEastingNorthing
//
//*****************************************************************************
void ossimMapProjection::setUlEastingNorthing(const ossimDpt& ulEastingNorthing)
{
   theUlEastingNorthing = ulEastingNorthing;
}

//*****************************************************************************
//  METHOD: ossimMapProjection::setUlGpt
//
//*****************************************************************************
void ossimMapProjection::setUlGpt(const ossimGpt& ulGpt)
{
   theUlGpt = ulGpt;
}

//*****************************************************************************
//  METHOD: ossimMapProjection::saveState
//
//*****************************************************************************
bool ossimMapProjection::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   ossimProjection::saveState(kwl, prefix);

   kwl.add(prefix,
           ossimKeywordNames::ORIGIN_LATITUDE_KW,
           theOrigin.latd(),
           true);

   kwl.add(prefix,
           ossimKeywordNames::CENTRAL_MERIDIAN_KW,
           theOrigin.lond(),
           true);

   if(isGeographic())
   {
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_LAT_KW,
              theUlGpt.latd(),
              true);

      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_LON_KW,
              theUlGpt.lond(),
              true);
   }

   theEllipsoid.saveState(kwl, prefix);

   if(!ossimIsNan(theDeltaLatPerPixel)&&isGeographic())
   {
      kwl.add(prefix,
              ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT,
              theDeltaLatPerPixel,
              true);
   }

   if(!ossimIsNan(theDeltaLonPerPixel)&&isGeographic())
   {
      kwl.add(prefix,
              ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON,
              theDeltaLonPerPixel,
              true);
   }

   if(theDatum)
   {
      kwl.add(prefix,
              ossimKeywordNames::DATUM_KW,
              theDatum->code(),
              true);
   }

   if(!theMetersPerPixel.isNan()&&!isGeographic())
   {
      kwl.add(prefix,
              ossimKeywordNames::METERS_PER_PIXEL_X_KW,
              theMetersPerPixel.x,
              true);

      kwl.add(prefix,
              ossimKeywordNames::METERS_PER_PIXEL_Y_KW,
              theMetersPerPixel.y,
              true);
   }

   if(!theUlEastingNorthing.isNan()&&!isGeographic())
   {
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_EASTING_KW,
              theUlEastingNorthing.x,
              true);

      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_NORTHING_KW,
              theUlEastingNorthing.y,
              true);
   }

   if(thePcsCode != 0)
     {
       kwl.add(prefix,
	       ossimKeywordNames::PCS_CODE_KW,
	       thePcsCode,
	       true);
     }

   return true;
}

//*****************************************************************************
//  METHOD: ossimMapProjection::loadState
//
//*****************************************************************************
bool ossimMapProjection::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   ossimProjection::loadState(kwl, prefix);

   const char* scale_x  = kwl.find(prefix, ossimKeywordNames::SCALE_X_KW);
   const char* scale_y  = kwl.find(prefix, ossimKeywordNames::SCALE_Y_KW);
   const char* trans_x  = kwl.find(prefix, ossimKeywordNames::TRANSLATION_X_KW);
   const char* trans_y  = kwl.find(prefix, ossimKeywordNames::TRANSLATION_Y_KW);
   const char* rotation = kwl.find(prefix, ossimKeywordNames::ROTATION_KW);
   const char* pcs_code = kwl.find(prefix, ossimKeywordNames::PCS_CODE_KW);
   ossimDpt scale(1.0, 1.0);
   ossimDpt trans(0.0, 0.0);
   double   rot = 0.0;

   if(scale_x)
      scale.x = ossimString(scale_x).toDouble();

   if(scale_y)
      scale.y = ossimString(scale_y).toDouble();

   if(trans_x)
      trans.x = ossimString(trans_x).toDouble();

   if(trans_y)
      trans.y = ossimString(trans_y).toDouble();

   if(rotation)
      rot = ossimString(rotation).toDouble();

   if(pcs_code)
     {
       thePcsCode = (ossim_uint16)ossimString(pcs_code).toULong();
     }
   else
     {
       thePcsCode = 0;
     }

   const char *lookup;

   // Get the latitude of the origin.
   lookup = kwl.find(prefix, ossimKeywordNames::ORIGIN_LATITUDE_KW);
   if (lookup)
   {
      theOrigin.latd(ossimString(lookup).toDouble());
   }

   // Get the central meridian.
   lookup = kwl.find(prefix, ossimKeywordNames::CENTRAL_MERIDIAN_KW);
   if (lookup)
   {
      theOrigin.lond(ossimString(lookup).toDouble());
   }

   theEllipsoid.loadState(kwl, prefix);

   //***
   // Call the virtual method "updateState" to allow derived classes to
   // verify/update their state.
   //***
   lookup = kwl.find(prefix, ossimKeywordNames::DATUM_KW);
   if(lookup)
   {
      theDatum = ossimDatumFactory::instance()->create(lookup);
      if(theDatum)
      {
         const ossimEllipsoid* ellipse = theDatum->ellipsoid();
         if(ellipse)
         {
            theEllipsoid = *ellipse;
         }
      }
   }
   else
   {
      theDatum = ossimDatumFactory::instance()->wgs84();
   }

   if(theDatum)
   {
      theOrigin.datum(theDatum);
      theUlGpt.datum(theDatum);
   }

   lookup =  kwl.find(prefix, ossimKeywordNames::METERS_PER_PIXEL_X_KW);
   if(lookup)
   {
      theMetersPerPixel.x = fabs(ossimString(lookup).toDouble());
   }
   else
   {
      theMetersPerPixel.x = OSSIM_DBL_NAN;
   }

   lookup =  kwl.find(prefix, ossimKeywordNames::METERS_PER_PIXEL_Y_KW);
   if(lookup)
   {
      theMetersPerPixel.y = fabs(ossimString(lookup).toDouble());
   }
   else
   {
      theMetersPerPixel.y = OSSIM_DBL_NAN;
   }

   theUlEastingNorthing.makeNan();
   theUlGpt.makeNan();

   const char* easting =  kwl.find(prefix, ossimKeywordNames::TIE_POINT_EASTING_KW);
   if(easting)
   {
      theUlEastingNorthing.x = (ossimString(easting).toDouble());
   }

   const char* northing =  kwl.find(prefix, ossimKeywordNames::TIE_POINT_NORTHING_KW);
   if(northing)
   {
      theUlEastingNorthing.y = (ossimString(northing).toDouble());
   }
   const char* tie_lat     = NULL;
   const char* tie_lon     = NULL;


   tie_lat      = kwl.find(prefix, ossimKeywordNames::TIE_POINT_LAT_KW);
   tie_lon      = kwl.find(prefix, ossimKeywordNames::TIE_POINT_LON_KW);

   if(tie_lat && tie_lon )
   {
      theUlGpt.latd(ossimString(tie_lat).toDouble());
      theUlGpt.lond(ossimString(tie_lon).toDouble());
   }
   else
   {
      theUlGpt.makeNan();
   }

   const char* deg_per_lat = kwl.find(prefix, ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT);
   const char* deg_per_lon = kwl.find(prefix, ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON);

   if(deg_per_lat)
   {
      theDeltaLatPerPixel = fabs(ossimString(deg_per_lat).toDouble());
   }
   else
   {
      theDeltaLatPerPixel = OSSIM_DBL_NAN;
   }
   if(deg_per_lon)
   {
      theDeltaLonPerPixel = fabs(ossimString(deg_per_lon).toDouble());
   }
   else
   {
      theDeltaLonPerPixel = OSSIM_DBL_NAN;
   }

   if((theDeltaLonPerPixel!=OSSIM_DBL_NAN)&&
      (theDeltaLatPerPixel!=OSSIM_DBL_NAN)&&
      theMetersPerPixel.hasNans())
   {
      theMetersPerPixel    = theOrigin.metersPerDegree();
      theMetersPerPixel.x *= theDeltaLonPerPixel;
      theMetersPerPixel.y *= theDeltaLatPerPixel;
   }
   
   ossimString pixelType = kwl.find(prefix, ossimKeywordNames::PIXEL_TYPE_KW);
   pixelType=pixelType.trim();
   if(pixelType!="")
   {
      pixelType.downcase();
      if(pixelType.contains("area"))
      {
         if((theMetersPerPixel.x != OSSIM_DBL_NAN)&&
            (theMetersPerPixel.y != OSSIM_DBL_NAN))
         {
            if(!theUlEastingNorthing.hasNans())
            {
               theUlEastingNorthing.x += theMetersPerPixel.x;
               theUlEastingNorthing.y -= theMetersPerPixel.y;
            }
         }
         if((theDeltaLatPerPixel != OSSIM_DBL_NAN)&&
            (theDeltaLonPerPixel != OSSIM_DBL_NAN))
         {
            theUlGpt.latd( theUlGpt.latd() - theDeltaLatPerPixel);
            theUlGpt.lond( theUlGpt.lond() + theDeltaLonPerPixel);
         }
      }
   }
   return true;
}

//*****************************************************************************
//  METHOD: ossimMapProjection::print
//*****************************************************************************
std::ostream& ossimMapProjection::print(std::ostream& out) const
{
   const char MODULE[] = "ossimMapProjection::print";

   out << setiosflags(ios::fixed) << setprecision(15)
       << "\n// " << MODULE
       << "\n" << ossimKeywordNames::TYPE_KW               << ":  "
       << getClassName()
       << "\n" << ossimKeywordNames::MAJOR_AXIS_KW         << ":  "
       << theEllipsoid.getA()
       << "\n" << ossimKeywordNames::MINOR_AXIS_KW         << ":  "
       << theEllipsoid.getB()
       << "\n" << ossimKeywordNames::ORIGIN_LATITUDE_KW    << ":  "
       << theOrigin.latd()
       << "\n" << ossimKeywordNames::CENTRAL_MERIDIAN_KW   << ":  "
       << theOrigin.lond()
       << "\n" << ossimKeywordNames::DATUM_KW              << ":  "
       << (theDatum?theDatum->code().c_str():"unknown")
       << "\n" << ossimKeywordNames::METERS_PER_PIXEL_X_KW << ":  "
       << (ossimIsNan(theMetersPerPixel.x)?ossimString("nan"):ossimString::toString(theMetersPerPixel.x, 15))
       << "\n" << ossimKeywordNames::METERS_PER_PIXEL_Y_KW << ":  "
       << (ossimIsNan(theMetersPerPixel.y)?ossimString("nan"):ossimString::toString(theMetersPerPixel.y, 15))
       << std::endl;
   return ossimProjection::print(out);
}

//*****************************************************************************
//  METHOD: ossimMapProjection::computeDegreesPerPixel
//
//*****************************************************************************
void ossimMapProjection::computeDegreesPerPixel(const ossimGpt& ground,
                                                const ossimDpt& metersPerPixel,
                                                double &deltaLat,
                                                double &deltaLon)
{
//    ossimDpt mpd = ground.metersPerDegree();
//    ossimDpt dpm(1.0/mpd.x,
//                 1.0/mpd.y);
//    deltaLat = metersPerPixel.y*dpm.y;
//    deltaLon = metersPerPixel.x*dpm.x;
   ossimDpt eastNorthGround = forward(ground);
   ossimDpt rightEastNorth  =  eastNorthGround;
   ossimDpt downEastNorth   =  eastNorthGround;
   rightEastNorth.x += metersPerPixel.x;
   downEastNorth.y  -= metersPerPixel.y;

   ossimGpt rightGpt = inverse(rightEastNorth);
   ossimGpt downGpt  = inverse(downEastNorth);

   // use euclidean distance to get length along the horizontal (lon)
   // and vertical (lat) directions
   //
   double tempDeltaLat = rightGpt.latd() - ground.latd();
   double tempDeltaLon = rightGpt.lond() - ground.lond();
   deltaLon = sqrt(tempDeltaLat*tempDeltaLat + tempDeltaLon*tempDeltaLon);

   tempDeltaLat = downGpt.latd() - ground.latd();
   tempDeltaLon = downGpt.lond() - ground.lond();
   deltaLat = sqrt(tempDeltaLat*tempDeltaLat + tempDeltaLon*tempDeltaLon);

}

//*****************************************************************************
//  METHOD: ossimMapProjection::computeMetersPerPixel
//
//*****************************************************************************
void ossimMapProjection::computeMetersPerPixel(const ossimGpt& center,
                                               double deltaDegreesPerPixelLat,
                                               double deltaDegreesPerPixelLon,
                                               ossimDpt &metersPerPixel)
{
//     metersPerPixel = center.metersPerDegree();
//     metersPerPixel.x *= deltaDegreesPerPixelLon;
//     metersPerPixel.y *= deltaDegreesPerPixelLat;

   ossimGpt right=center;
   ossimGpt down=center;

   down.latd(center.latd()  + deltaDegreesPerPixelLat);
   right.lond(center.lond() + deltaDegreesPerPixelLon);

   ossimDpt centerMeters = forward(center);
   ossimDpt rightMeters = forward(right);
   ossimDpt downMeters  = forward(down);


   metersPerPixel.x = (rightMeters - centerMeters).length();
   metersPerPixel.y = (downMeters  - centerMeters).length();

}

//*****************************************************************************
//  METHOD: ossimMapProjection::computeMetersPerPixel
//
//*****************************************************************************
void ossimMapProjection::computeMetersPerPixel(double deltaDegreesPerPixelLat,
                                               double deltaDegreesPerPixelLon,
                                               ossimDpt &metersPerPixel)
{
   computeMetersPerPixel(theOrigin,
                         deltaDegreesPerPixelLat,
                         deltaDegreesPerPixelLon,
                         metersPerPixel);
}

//*****************************************************************************
//  METHOD: ossimMapProjection::operator==
//
//*****************************************************************************
bool ossimMapProjection::operator==(const ossimProjection& projection) const
{
   const ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection, &projection);

   if(!mapProj)
   {
      return false;
   }

   if( (theDatum        == mapProj->theDatum) &&
       (getClassName()  == mapProj->getClassName())&&
       (theOrigin       == mapProj->theOrigin))
   {
      if(isGeographic())
      {
         return ( (theDeltaLatPerPixel == mapProj->theDeltaLatPerPixel) &&
                  (theDeltaLonPerPixel == mapProj->theDeltaLonPerPixel) &&
                  (theUlGpt            == mapProj->theUlGpt));
      }
      else
      {
         return ( (theMetersPerPixel    == mapProj->theMetersPerPixel) &&
                  (theUlEastingNorthing == mapProj->theUlEastingNorthing));
      }
   }

   return false;
}

double ossimMapProjection::getFalseEasting() const
{
   return 0.0;
}

double ossimMapProjection::getFalseNorthing() const
{
   return 0.0;
}

double ossimMapProjection::getStandardParallel1() const
{
   return 0.0;
}

double ossimMapProjection::getStandardParallel2() const
{
   return 0.0;
}

void ossimMapProjection::snaptTiePointTo(ossim_float64 multiple,
                                         ossimUnitType unitType)
{
   ossim_float64 convertedMultiple = multiple;
   
   if (isGeographic() && (unitType != OSSIM_DEGREES) )
   {
      // Convert to degrees.
      ossimUnitConversionTool convertor;
      convertor.setOrigin(theOrigin);
      convertor.setValue(multiple, unitType);
      convertedMultiple = convertor.getDegrees();
   }
   else if ( !isGeographic() && (unitType != OSSIM_METERS) )
   {
      // Convert to meters.
      ossimUnitConversionTool convertor;
      convertor.setOrigin(theOrigin);
      convertor.setValue(multiple, unitType);
      convertedMultiple = convertor.getMeters();
   }

   // cout << "multiple: " << convertedMultiple << endl;
   
   // Convert the tie point.
   if (isGeographic())
   {
      // Snap the latitude.
      ossim_float64 d = theUlGpt.latd();
      d = irint(d / convertedMultiple) * convertedMultiple;
      theUlGpt.latd(d);

      // Snap the longitude.
      d = theUlGpt.lond();
      d = irint(d / convertedMultiple) * convertedMultiple;
      theUlGpt.lond(d);

      // Adjust the stored easting / northing.
      theUlEastingNorthing = forward(theUlGpt);
   }
   else
   {
      // Snap the easting.
      ossim_float64 d = theUlEastingNorthing.x;
      d = irint(d / convertedMultiple) * convertedMultiple;
      theUlEastingNorthing.x = d;

      // Snap the northing.
      d = theUlEastingNorthing.y;
      d = irint(d / convertedMultiple) * convertedMultiple;
      theUlEastingNorthing.y = d;

      // Adjust the stored upper left ground point.
      theUlGpt = inverse(theUlEastingNorthing);
   }
}
