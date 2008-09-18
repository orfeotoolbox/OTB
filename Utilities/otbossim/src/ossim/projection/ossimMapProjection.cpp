//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Base class for all map projections.
// 
//*******************************************************************
//  $Id: ossimMapProjection.cpp 13025 2008-06-13 17:06:30Z sbortman $

#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <sstream>

#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimStatePlaneProjectionFactory.h>
#include <ossim/projection/ossimStatePlaneProjectionInfo.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimString.h>
#include <ossim/elevation/ossimElevManager.h>
#include <ossim/base/ossimMatrix3x3.h>
#include <ossim/base/ossimUnitConversionTool.h>
#include <ossim/base/ossimUnitTypeLut.h>

// RTTI information for the ossimMapProjection
RTTI_DEF1(ossimMapProjection, "ossimMapProjection" , ossimProjection);

ossimMapProjection::ossimMapProjection(const ossimEllipsoid& ellipsoid,
                                       const ossimGpt& origin)
   :theEllipsoid(ellipsoid),
    theOrigin(origin),
    theDatum(origin.datum()), // force no shifting
    theMetersPerPixel(1, 1),
    theDegreesPerPixel(),
    theUlGpt(0, 0),
    theUlEastingNorthing(0, 0),
    theFalseEastingNorthing(0, 0),
    thePcsCode(0),
    theElevationLookupFlag(true),
    theModelTransformUnitType(OSSIM_UNIT_UNKNOWN)
{
   theUlGpt = theOrigin;
   theUlEastingNorthing.makeNan();
   theDegreesPerPixel.makeNan();
}

ossimMapProjection::ossimMapProjection(const ossimMapProjection& src)
      : ossimProjection(src),
        theEllipsoid(src.theEllipsoid),
        theOrigin(src.theOrigin),
        theDatum(src.theDatum),
        theMetersPerPixel(src.theMetersPerPixel),
        theDegreesPerPixel(src.theDegreesPerPixel),
        theUlGpt(src.theUlGpt),
        theUlEastingNorthing(src.theUlEastingNorthing),
        theFalseEastingNorthing(src.theFalseEastingNorthing),
        thePcsCode(src.thePcsCode),
        theElevationLookupFlag(true),
        theModelTransform(src.theModelTransform),
        theInverseModelTransform(src.theInverseModelTransform),
        theModelTransformUnitType(src.theModelTransformUnitType)
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
   return theDegreesPerPixel;
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
   //---
   // Set the origin and since the origin has a datum which in turn has
   // an ellipsoid, sync them up.
   //---
   theOrigin    = origin;
   theDatum     = theOrigin.datum();
   theEllipsoid = *(theDatum->ellipsoid());
      
   update();
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
   if(theModelTransformUnitType != OSSIM_UNIT_UNKNOWN)
   {
      const NEWMAT::Matrix& m = theModelTransform.getData();
      ossimDpt ls1(0, 0);
      ossimDpt ls2(1, 0);
      ossimDpt ls3(0, 1);
      ossimGpt wpt1;
      ossimGpt wpt2;
      lineSampleToWorld(ls1, wpt1);
      ossimDpt mpt1(m[0][0]*ls1.x + m[0][1]*ls1.y + m[0][3],
                    m[1][0]*ls1.x + m[1][1]*ls1.y + m[1][3]);
      ossimDpt mpt2(m[0][0]*ls2.x + m[0][1]*ls2.y + m[0][3],
                    m[1][0]*ls2.x + m[1][1]*ls2.y + m[1][3]);
      ossimDpt mpt3(m[0][0]*ls3.x + m[0][1]*ls3.y + m[0][3],
                    m[1][0]*ls3.x + m[1][1]*ls3.y + m[1][3]);
      double len = 1.0;
      double len2 = 1.0;
      switch(theModelTransformUnitType)
      {
         case OSSIM_DEGREES:
         case OSSIM_MINUTES:
         case OSSIM_SECONDS:
         case OSSIM_RADIANS:
         {
            ossimUnitConversionTool ut;
            len  = (mpt1-mpt2).length();
            len2  = (mpt1-mpt3).length();
			ut.setValue((len+len2)*.5, theModelTransformUnitType);
			len = ut.getValue(OSSIM_DEGREES);
            theDegreesPerPixel = ossimDpt(len, len);
			theUlGpt = wpt1;
            computeMetersPerPixel(theDegreesPerPixel.y,
                                  theDegreesPerPixel.x,
                                  theMetersPerPixel);
         break;
         }
         default:
         {
            ossimUnitConversionTool ut;
            len  = (mpt1-mpt2).length();
            len2  = (mpt1-mpt3).length();
            ut.setValue(mpt1.x, theModelTransformUnitType);
            mpt1.x = ut.getValue(OSSIM_METERS);
            ut.setValue(mpt1.y, theModelTransformUnitType);
            mpt1.y = ut.getValue(OSSIM_METERS);
			ut.setValue((len+len2)*.5, theModelTransformUnitType);
			len = ut.getValue(OSSIM_METERS);
            theMetersPerPixel = ossimDpt(len, len);
            theUlEastingNorthing = mpt1;
            computeDegreesPerPixel(theOrigin,
                                   theMetersPerPixel,
                                   theDegreesPerPixel.y,
                                   theDegreesPerPixel.x);
            break;
         }
      }
      theUlGpt = wpt1;
   }
   else if( theDegreesPerPixel.hasNans() == false )
   {
      if(theMetersPerPixel.hasNans())
      {
         computeMetersPerPixel(theDegreesPerPixel.y,
                               theDegreesPerPixel.x,
                               theMetersPerPixel);
      }
   }
   else if(!theMetersPerPixel.hasNans())
   {
      computeDegreesPerPixel(theOrigin,
                             theMetersPerPixel,
                             theDegreesPerPixel.y,
                             theDegreesPerPixel.x);
   }
   // compute the tie points if not already computed
   //
   if( (ossim::isnan(theUlGpt.latd())) ||
       (ossim::isnan(theUlGpt.lond())) )
   {
      theUlGpt = theOrigin;
   }
   if(theUlEastingNorthing.hasNans())
   {
      if((ossim::isnan(theUlGpt.latd()) == false) &&
         (ossim::isnan(theUlGpt.lond()) == false))
      {
         theUlEastingNorthing = forward(theUlGpt);
      }
   }

   if (thePcsCode)
   {
      verifyPcsCodeMatches();
   }
}

void ossimMapProjection::applyScale(const ossimDpt& scale,
                                    bool recenterTiePoint)
{
   if (!recenterTiePoint)
   {
      theDegreesPerPixel.x *= scale.x;
      theDegreesPerPixel.y *= scale.y;
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
         tie.lond( tie.lond() - theDegreesPerPixel.x/2.0);
         tie.latd( tie.latd() + theDegreesPerPixel.y/2.0);
         
         // Apply the scale.
         theDegreesPerPixel.x *= scale.x;
         theDegreesPerPixel.y *= scale.y;
         theMetersPerPixel.x *= scale.x;
         theMetersPerPixel.y *= scale.y;
         
         // Now shift it back to center of the new tie.
         tie.lond( tie.lond() + theDegreesPerPixel.x/2.0);
         tie.latd( tie.latd() - theDegreesPerPixel.y/2.0);
         
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
         theDegreesPerPixel.x *= scale.x;
         theDegreesPerPixel.y *= scale.y;
         
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

   if(theModelTransformUnitType != OSSIM_UNIT_UNKNOWN)
   {
      ossimGpt gpt = worldPoint;
      if(theDatum)
      {
         gpt.changeDatum(theDatum);
      }
      switch(theModelTransformUnitType)
      {
         case OSSIM_METERS:
         case OSSIM_FEET:
         case OSSIM_US_SURVEY_FEET:
         {
            ossimDpt modelPoint = forward(gpt);
            ossimUnitConversionTool ut;
            ut.setValue(modelPoint.x, OSSIM_METERS);
            modelPoint.x = ut.getValue(theModelTransformUnitType);
            ut.setValue(modelPoint.y, OSSIM_METERS);
            modelPoint.y = ut.getValue(theModelTransformUnitType);
            const NEWMAT::Matrix& m = theInverseModelTransform.getData();
            
            lineSample.x = m[0][0]*modelPoint.x + m[0][1]*modelPoint.y + m[0][3];
            lineSample.y = m[1][0]*modelPoint.x + m[1][1]*modelPoint.y + m[1][3];
            
            return;
         }
         case OSSIM_DEGREES:
         case OSSIM_RADIANS:
         case OSSIM_MINUTES:
         case OSSIM_SECONDS:
         {
            ossimUnitConversionTool ut;
            ossimDpt modelPoint;
            modelPoint.lat = gpt.latd();
            modelPoint.lon = gpt.lond();
            ut.setValue(modelPoint.lat, OSSIM_DEGREES);
            modelPoint.lat = ut.getValue(theModelTransformUnitType);
            ut.setValue(modelPoint.lon, OSSIM_DEGREES);
            modelPoint.lon = ut.getValue(theModelTransformUnitType);
            const NEWMAT::Matrix& m = theInverseModelTransform.getData();
            
            lineSample.x = m[0][0]*modelPoint.x + m[0][1]*modelPoint.y + m[0][3];
            lineSample.y = m[1][0]*modelPoint.x + m[1][1]*modelPoint.y + m[1][3];
            return;
         }
         default:
         {
            lineSample.makeNan();
            return;
         }
      }
   }
   else if(isGeographic())
   {
      ossimGpt gpt = worldPoint;
      
      if (theOrigin.datum() != gpt.datum())
      {
         // Apply datum shift if it's not the same.
         gpt.changeDatum(theOrigin.datum());
      }
      
      lineSample.line = (theUlGpt.latd() - gpt.latd()) / theDegreesPerPixel.y;
      lineSample.samp = (gpt.lond() - theUlGpt.lond()) / theDegreesPerPixel.x;
   }
   else
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
   if(lineSample.hasNans())
   {
      gpt.makeNan();
      return;
   }
   if(theModelTransformUnitType != OSSIM_UNIT_UNKNOWN)
   {
      const NEWMAT::Matrix& m = theModelTransform.getData();
      // map transforms can only be 2-D for now so we will look at
      // the first 2 rows only
      ossimDpt modelPoint(m[0][0]*lineSample.x + m[0][1]*lineSample.y + m[0][3],
                          m[1][0]*lineSample.x + m[1][1]*lineSample.y + m[1][3]);
      switch(theModelTransformUnitType)
      {
         case OSSIM_DEGREES:
         {
            gpt.latd(modelPoint.lat);
            gpt.lond(modelPoint.lon);
            gpt.datum(theDatum);
            return;
         }
         case OSSIM_MINUTES:
         case OSSIM_SECONDS:
         case OSSIM_RADIANS:
         {
            ossimUnitConversionTool ut;
            ut.setValue(modelPoint.x, theModelTransformUnitType);
            modelPoint.x = ut.getValue(OSSIM_DEGREES);
            ut.setValue(modelPoint.y, theModelTransformUnitType);
            modelPoint.y = ut.getValue(OSSIM_DEGREES);
            gpt.latd(modelPoint.lat);
            gpt.lond(modelPoint.lon);
            gpt.datum(theDatum);
            return;
         }
         default:
         {
            ossimUnitConversionTool ut;
            ut.setValue(modelPoint.x, theModelTransformUnitType);
            modelPoint.x = ut.getValue(OSSIM_METERS);
            ut.setValue(modelPoint.y, theModelTransformUnitType);
            modelPoint.y = ut.getValue(OSSIM_METERS);
            gpt = inverse(modelPoint);
            break;
         }
      }
      gpt.datum(theDatum);
   }
   else if(isGeographic())
   {
      double lat = theUlGpt.latd() - (lineSample.line * theDegreesPerPixel.y);
      double lon = theUlGpt.lond() + (lineSample.samp * theDegreesPerPixel.x);
      
      gpt.latd(lat);
      gpt.lond(lon);
      gpt.hgt = hgtEllipsoid;
   }
   else
   {
      if(theUlEastingNorthing.hasNans())
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
   if(theElevationLookupFlag)
   {
      gpt.hgt = ossimElevManager::instance()->getHeightAboveEllipsoid(gpt);
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
   double elev = ossim::nan();

//    if(theElevationLookupFlag)
//    {
//       elev =  ossimElevManager::instance()->getHeightAboveEllipsoid(worldPt);
//    }

   lineSampleHeightToWorld(lineSampPt, elev, worldPt);

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
                          theDegreesPerPixel.y,
                          theDegreesPerPixel.x);
}

void ossimMapProjection::setDecimalDegreesPerPixel(const ossimDpt& gsd)
{
   theDegreesPerPixel.y = gsd.lat;
   theDegreesPerPixel.x = gsd.lon;

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

   theEllipsoid.saveState(kwl, prefix);

   if(theDatum)
   {
      kwl.add(prefix,
              ossimKeywordNames::DATUM_KW,
              theDatum->code(),
              true);
   }

   if(thePcsCode != 0)
   {
      kwl.add(prefix,
              ossimKeywordNames::PCS_CODE_KW,
              thePcsCode,
              true);
   }
   
   if(isGeographic())
   {
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_XY_KW,
              ossimDpt(theUlGpt).toString().c_str(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_UNITS_KW,
              ossimUnitTypeLut::instance()->getEntryString(OSSIM_DEGREES),
              true);
      kwl.add(prefix,
              ossimKeywordNames::PIXEL_SCALE_XY_KW,
              theDegreesPerPixel.toString().c_str(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::PIXEL_SCALE_UNITS_KW,
              ossimUnitTypeLut::instance()->getEntryString(OSSIM_DEGREES),
              true);
   }
   else
   {
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_XY_KW,
              theUlEastingNorthing.toString().c_str(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_UNITS_KW,
              ossimUnitTypeLut::instance()->getEntryString(OSSIM_METERS),
              true);
      kwl.add(prefix,
              ossimKeywordNames::PIXEL_SCALE_XY_KW,
              theMetersPerPixel.toString().c_str(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::PIXEL_SCALE_UNITS_KW,
              ossimUnitTypeLut::instance()->getEntryString(OSSIM_METERS),
              true);  
   }

   kwl.add(prefix,
           ossimKeywordNames::FALSE_EASTING_NORTHING_KW,
           theFalseEastingNorthing.toString().c_str(),
           true);
   kwl.add(prefix,
           ossimKeywordNames::FALSE_EASTING_NORTHING_UNITS_KW,
           ossimUnitTypeLut::instance()->getEntryString(OSSIM_METERS),
           true);
   kwl.add(prefix,
           ossimKeywordNames::ELEVATION_LOOKUP_FLAG_KW,
           ossimString::toString(theElevationLookupFlag),
           true);

   if(theModelTransformUnitType != OSSIM_UNIT_UNKNOWN)
   {
      const NEWMAT::Matrix& m = theModelTransform.getData();
      ostringstream out;
      ossim_uint32 row, col;
      for(row = 0; row < 4; ++row)
      {
         for(col = 0; col < 4; ++col)
         {
            out << std::setprecision(20) << m[row][col] << " ";
         }
      }
      kwl.add(prefix,
              ossimKeywordNames::IMAGE_MODEL_TRANSFORM_MATRIX_KW,
              out.str().c_str(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::IMAGE_MODEL_TRANSFORM_UNIT_KW,
              ossimUnitTypeLut::instance()->getEntryString(theModelTransformUnitType),
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

   const char* elevLookupFlag = kwl.find(prefix, ossimKeywordNames::ELEVATION_LOOKUP_FLAG_KW);
   if(elevLookupFlag)
   {
      theElevationLookupFlag = ossimString(elevLookupFlag).toBool();
   }
   // Get the ellipsoid.
   theEllipsoid.loadState(kwl, prefix);

   const char *lookup;

   // Get the datum.  If not there default to WGS 84
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
   
   // Get the latitude of the origin.
   lookup = kwl.find(prefix, ossimKeywordNames::ORIGIN_LATITUDE_KW);
   if (lookup)
   {
      theOrigin.latd(ossimString(lookup).toFloat64());
   }
   // else ???

   // Get the central meridian.
   lookup = kwl.find(prefix, ossimKeywordNames::CENTRAL_MERIDIAN_KW);
   if (lookup)
   {
      theOrigin.lond(ossimString(lookup).toFloat64());
   }
   // else ???


   // Get the pixel scale.
   theMetersPerPixel.makeNan();
   theDegreesPerPixel.makeNan();
   lookup = kwl.find(prefix, ossimKeywordNames::PIXEL_SCALE_UNITS_KW);
   if (lookup)
   {
      ossimUnitType units =
         static_cast<ossimUnitType>(ossimUnitTypeLut::instance()->
                                    getEntryNumber(lookup));
      
      lookup = kwl.find(prefix, ossimKeywordNames::PIXEL_SCALE_XY_KW);
      if (lookup)
      {
         ossimDpt scale;
         scale.toPoint(std::string(lookup));

         switch (units)
         {
            case OSSIM_METERS:
            {
               theMetersPerPixel = scale;
               break;
            }
            case OSSIM_DEGREES:
            {
               theDegreesPerPixel.x = scale.x;
               theDegreesPerPixel.y = scale.y;
               break;
            }
            case OSSIM_FEET:
            case OSSIM_US_SURVEY_FEET:
            {
               ossimUnitConversionTool ut;
               ut.setValue(scale.x, units);
               theMetersPerPixel.x = ut.getValue(OSSIM_METERS);
               ut.setValue(scale.y, units);
               theMetersPerPixel.y = ut.getValue(OSSIM_METERS);
               break;
            }
            default:
            {
               // Unhandled unit type!
               ossimNotify(ossimNotifyLevel_WARN)
                  << "ossimMapProjection::loadState WARNING!"
                  << "Unhandled unit type for "
                  << ossimKeywordNames::PIXEL_SCALE_UNITS_KW << ":  "
                  << ( ossimUnitTypeLut::instance()->
                       getEntryString(units).c_str() )
                  << endl;
               break;
            }
         } // End of switch (units)
         
      }  // End of if (PIXEL_SCALE_XY)

   } // End of if (PIXEL_SCALE_UNITS)
   else
   {
      // BACKWARDS COMPATIBILITY LOOKUPS...
      lookup =  kwl.find(prefix, ossimKeywordNames::METERS_PER_PIXEL_X_KW);
      if(lookup)
      {
         theMetersPerPixel.x = fabs(ossimString(lookup).toFloat64());
      }
      
      lookup =  kwl.find(prefix, ossimKeywordNames::METERS_PER_PIXEL_Y_KW);
      if(lookup)
      {
         theMetersPerPixel.y = fabs(ossimString(lookup).toFloat64());
      }
      
      lookup = kwl.find(prefix,
                        ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT);
      if(lookup)
      {
         theDegreesPerPixel.y = fabs(ossimString(lookup).toFloat64());
      }
      
      lookup = kwl.find(prefix,
                        ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON);
      if(lookup)
      {
         theDegreesPerPixel.x = fabs(ossimString(lookup).toFloat64());
      }
   }            

   // Get the tie point.
   theUlGpt.makeNan();

    // Since this won't be picked up from keywords set to 0 to keep nan out.
   theUlGpt.hgt = 0.0;
   
   theUlEastingNorthing.makeNan();
   lookup = kwl.find(prefix, ossimKeywordNames::TIE_POINT_UNITS_KW);
   if (lookup)
   {
      ossimUnitType units =
         static_cast<ossimUnitType>(ossimUnitTypeLut::instance()->
                                    getEntryNumber(lookup));
      
      lookup = kwl.find(prefix, ossimKeywordNames::TIE_POINT_XY_KW);
      if (lookup)
      {
         ossimDpt tie;
         tie.toPoint(std::string(lookup));

         switch (units)
         {
            case OSSIM_METERS:
            {
               theUlEastingNorthing = tie;
               break;
            }
            case OSSIM_DEGREES:
            {
               theUlGpt.lond(tie.x);
               theUlGpt.latd(tie.y);
               break;
            }
            case OSSIM_FEET:
            case OSSIM_US_SURVEY_FEET:
            {
               ossimUnitConversionTool ut;
               ut.setValue(tie.x, units);
               theUlEastingNorthing.x = ut.getValue(OSSIM_METERS);
               ut.setValue(tie.y, units);
               theUlEastingNorthing.y = ut.getValue(OSSIM_METERS);
               break;
            }
            default:
            {
               // Unhandled unit type!
               ossimNotify(ossimNotifyLevel_WARN)
                  << "ossimMapProjection::loadState WARNING!"
                  << "Unhandled unit type for "
                  << ossimKeywordNames::TIE_POINT_UNITS_KW << ": " 
                  << ( ossimUnitTypeLut::instance()->
                       getEntryString(units).c_str() )
                  << endl;
               break;
            }
         } // End of switch (units)
         
      }  // End of if (TIE_POINT_XY)

   } // End of if (TIE_POINT_UNITS)
   else
   {
      // BACKWARDS COMPATIBILITY LOOKUPS...
      lookup =  kwl.find(prefix, ossimKeywordNames::TIE_POINT_EASTING_KW);
      if(lookup)
      {
         theUlEastingNorthing.x = (ossimString(lookup).toFloat64());
      }

      lookup =  kwl.find(prefix, ossimKeywordNames::TIE_POINT_NORTHING_KW);
      if(lookup)
      {
         theUlEastingNorthing.y = (ossimString(lookup).toFloat64());
      }

      lookup = kwl.find(prefix, ossimKeywordNames::TIE_POINT_LAT_KW);
      if (lookup)
      {
         theUlGpt.latd(ossimString(lookup).toFloat64());
      }

      lookup = kwl.find(prefix, ossimKeywordNames::TIE_POINT_LON_KW);
      if (lookup)
      {
         theUlGpt.lond(ossimString(lookup).toFloat64());
      }
   }
   
   // Get the false easting northing.
   theFalseEastingNorthing.x = 0.0;
   theFalseEastingNorthing.y = 0.0;
   lookup = kwl.find(prefix,
                     ossimKeywordNames::FALSE_EASTING_NORTHING_UNITS_KW);
   if (lookup)
   {
      ossimUnitType units =
         static_cast<ossimUnitType>(ossimUnitTypeLut::instance()->
                                    getEntryNumber(lookup));
      
      lookup = kwl.find(prefix, ossimKeywordNames::FALSE_EASTING_NORTHING_KW);
      if (lookup)
      {
         ossimDpt eastingNorthing;
         eastingNorthing.toPoint(std::string(lookup));

         switch (units)
         {
            case OSSIM_METERS:
            {
               theFalseEastingNorthing = eastingNorthing;
               break;
            }
            case OSSIM_FEET:
            case OSSIM_US_SURVEY_FEET:
            {
               ossimUnitConversionTool ut;
               ut.setValue(eastingNorthing.x, units);
               theFalseEastingNorthing.x = ut.getValue(OSSIM_METERS);
               ut.setValue(eastingNorthing.y, units);
               theFalseEastingNorthing.y = ut.getValue(OSSIM_METERS);
               break;
            }
            default:
            {
               // Unhandled unit type!
               ossimNotify(ossimNotifyLevel_WARN)
                  << "ossimMapProjection::loadState WARNING!"
                  << "Unhandled unit type for "
                  << ossimKeywordNames::FALSE_EASTING_NORTHING_UNITS_KW
                  << ":  " 
                  << ( ossimUnitTypeLut::instance()->
                       getEntryString(units).c_str() )
                  << endl;
               break;
            }
         } // End of switch (units)
         
      }  // End of if (FALSE_EASTING_NORTHING_KW)

   } // End of if (FALSE_EASTING_NORTHING_UNITS_KW)
   else
   {
      // BACKWARDS COMPATIBILITY LOOKUPS...
      lookup =  kwl.find(prefix, ossimKeywordNames::FALSE_EASTING_KW);
      if(lookup)
      {
         theFalseEastingNorthing.x = fabs(ossimString(lookup).toFloat64());
      }
      
      lookup =  kwl.find(prefix, ossimKeywordNames::FALSE_NORTHING_KW);
      if(lookup)
      {
         theFalseEastingNorthing.y = fabs(ossimString(lookup).toFloat64());
      }
   }            

   // Get the Projection Coordinate System(PCS) code.
   lookup = kwl.find(prefix, ossimKeywordNames::PCS_CODE_KW);
   if(lookup)
   {
      thePcsCode = (ossim_uint16)ossimString(lookup).toUInt32();
   }
   else
   {
      thePcsCode = 0;
   }
   
//    if((theDegreesPerPixel.x!=OSSIM_DBL_NAN)&&
//       (theDegreesPerPixel.y!=OSSIM_DBL_NAN)&&
//       theMetersPerPixel.hasNans())
//    {
//       theMetersPerPixel    = theOrigin.metersPerDegree();
//       theMetersPerPixel.x *= theDegreesPerPixel.x;
//       theMetersPerPixel.y *= theDegreesPerPixel.y;
//    }

   lookup = kwl.find(prefix, ossimKeywordNames::PIXEL_TYPE_KW);
   if (lookup)
   {
      ossimString pixelType = lookup;
      pixelType=pixelType.trim();
      if(pixelType!="")
      {
         pixelType.downcase();
         if(pixelType.contains("area"))
         {
            if( theMetersPerPixel.hasNans() == false)
            {
               if(!theUlEastingNorthing.hasNans())
               {
                  theUlEastingNorthing.x += (theMetersPerPixel.x*0.5);
                  theUlEastingNorthing.y -= (theMetersPerPixel.y*0.5);
               }
            }
            if(theDegreesPerPixel.hasNans() == false)
            {
               theUlGpt.latd( theUlGpt.latd() - (theDegreesPerPixel.y*0.5) );
               theUlGpt.lond( theUlGpt.lond() + (theDegreesPerPixel.x*0.5) );
            }
         }
      }
   }
   
   theModelTransformUnitType = OSSIM_UNIT_UNKNOWN;
   const char* modelTransform = kwl.find(prefix, ossimKeywordNames::IMAGE_MODEL_TRANSFORM_MATRIX_KW);
   const char* modelTransformUnit = kwl.find(prefix, ossimKeywordNames::IMAGE_MODEL_TRANSFORM_UNIT_KW);
   if(modelTransform&&modelTransformUnit) // row ordered 4x4 matrix.  Should be 16 values
   {
      
      NEWMAT::Matrix& m = theModelTransform.getData();
      istringstream in(modelTransform);
      ossim_uint32 row, col;
      for(row = 0; row < 4; ++row)
      {
         for(col = 0; col < 4; ++col)
         {
            in >> m[row][col];
         }
      }
      // make sure these have the identity and all unused are 0.0
      m[2][2] = 1.0;
      m[2][0] = 0.0;
      m[2][1] = 0.0;
      m[2][3] = 0.0;
      m[3][3] = 1.0;
      m[3][2] = 0.0;
      m[3][1] = 0.0;
      m[3][0] = 0.0;
      
      if(!in.fail())
      {
         theInverseModelTransform = theModelTransform;
         theInverseModelTransform.i();
         theModelTransformUnitType = static_cast<ossimUnitType>(ossimUnitTypeLut::instance()->
                                                                getEntryNumber(modelTransformUnit));
      }
  }
   //---
   // Set the datum of the origin and tie point.
   // Use method that does NOT perform a shift.
   //---
   if(theDatum)
   {
      theOrigin.datum(theDatum);
      theUlGpt.datum(theDatum);
   }

   if(theMetersPerPixel.hasNans() &&
      theDegreesPerPixel.hasNans())
   {
      ossimDpt mpd = ossimGpt().metersPerDegree();
      if(isGeographic())
      {
         theDegreesPerPixel.lat = 1.0/mpd.y;
         theDegreesPerPixel.lon = 1.0/mpd.y;
      }
      else
      {
         theMetersPerPixel.x = 1.0;
         theMetersPerPixel.y = 1.0;
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
       << ((ossim::isnan(theMetersPerPixel.x))?ossimString("nan"):ossimString::toString(theMetersPerPixel.x, 15))
       << "\n" << ossimKeywordNames::METERS_PER_PIXEL_Y_KW << ":  "
       << ((ossim::isnan(theMetersPerPixel.y))?ossimString("nan"):ossimString::toString(theMetersPerPixel.y, 15))
       << "\n" << ossimKeywordNames::FALSE_EASTING_NORTHING_KW << ": "
       << theFalseEastingNorthing.toString().c_str()
       << "\n" << ossimKeywordNames::FALSE_EASTING_NORTHING_UNITS_KW << ": "
       << ossimUnitTypeLut::instance()->getEntryString(OSSIM_METERS)
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
         return ( (theDegreesPerPixel.y == mapProj->theDegreesPerPixel.y) &&
                  (theDegreesPerPixel.x == mapProj->theDegreesPerPixel.x) &&
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
   return theFalseEastingNorthing.x;
}

double ossimMapProjection::getFalseNorthing() const
{
   return theFalseEastingNorthing.y;
}

double ossimMapProjection::getStandardParallel1() const
{
   return 0.0;
}

double ossimMapProjection::getStandardParallel2() const
{
   return 0.0;
}

void ossimMapProjection::snapTiePointTo(ossim_float64 multiple,
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
      d = ossim::round<int>(d / convertedMultiple) * convertedMultiple;
      theUlGpt.latd(d);

      // Snap the longitude.
      d = theUlGpt.lond();
      d = ossim::round<int>(d / convertedMultiple) * convertedMultiple;
      theUlGpt.lond(d);

      // Adjust the stored easting / northing.
      theUlEastingNorthing = forward(theUlGpt);
   }
   else
   {
      // Snap the easting.
      ossim_float64 d = theUlEastingNorthing.x - getFalseEasting();
      d = ossim::round<int>(d / convertedMultiple) * convertedMultiple;
      theUlEastingNorthing.x = d + getFalseEasting();

      // Snap the northing.
      d = theUlEastingNorthing.y - getFalseNorthing();
      d = ossim::round<int>(d / convertedMultiple) * convertedMultiple;
      theUlEastingNorthing.y = d + getFalseNorthing();

      // Adjust the stored upper left ground point.
      theUlGpt = inverse(theUlEastingNorthing);
   }
}

void ossimMapProjection::setElevationLookupFlag(bool flag)
{
   theElevationLookupFlag = flag;
}

bool ossimMapProjection::getElevationLookupFlag()const
{
   return theElevationLookupFlag;
}
   

bool ossimMapProjection::verifyPcsCodeMatches()
{
   if (thePcsCode == 0)
   {
      return false;
   }

   ossim_int32 code = static_cast<ossim_int32>(thePcsCode);
   
   const ossimStatePlaneProjectionInfo* info =
      ossimStatePlaneProjectionFactory::instance()->getInfo(code);
   if (info)
   {
      if (info->matchesProjection(this))
      {
         return true;
      }
   }

   // Reset to zero since it doesn't belong to this projection.
   thePcsCode = 0;
   
   return false;
}
