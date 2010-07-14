//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// 
//********************************************************************
// $Id: ossimQuadProjection.cpp 17206 2010-04-25 23:20:40Z dburken $

#include <ossim/projection/ossimQuadProjection.h>
#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimEcefVector.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/elevation/ossimElevManager.h>

RTTI_DEF1(ossimQuadProjection, "ossimQuadProjection", ossimProjection);

ossimQuadProjection::ossimQuadProjection()
      :ossimProjection()
{
   theInputRect.makeNan();
   theUlg.makeNan();
   theUrg.makeNan();
   theLrg.makeNan();
   theLlg.makeNan();
}

ossimQuadProjection::ossimQuadProjection(const ossimQuadProjection& rhs)
   :ossimProjection(rhs),
    theInputRect(rhs.theInputRect),
    theUlg(rhs.theUlg),
    theUrg(rhs.theUrg),
    theLrg(rhs.theLrg),
    theLlg(rhs.theLlg),
    theLatGrid(rhs.theLatGrid),
    theLonGrid(rhs.theLonGrid)
{
}

ossimQuadProjection::ossimQuadProjection(const ossimIrect& rect,
                                         const ossimGpt& ulg,
                                         const ossimGpt& urg,
                                         const ossimGpt& lrg,
                                         const ossimGpt& llg)
      :theInputRect(rect),
       theUlg(ulg),
       theUrg(urg),
       theLrg(lrg),
       theLlg(llg)
{
   initializeGrids();
}
                       
ossimQuadProjection::~ossimQuadProjection()
{
}

ossimObject *ossimQuadProjection::dup()const
{
   return new ossimQuadProjection(*this);
}

ossimGpt ossimQuadProjection::origin()const
{
   ossimGpt result;
   result.makeNan();
   if(theUlg.isLatNan()||
      theUlg.isLonNan()||
      theUrg.isLatNan()||
      theUrg.isLonNan()||
      theLrg.isLatNan()||
      theLrg.isLonNan()||
      theLlg.isLatNan()||
      theLlg.isLonNan())
   {
      return result;
   }

   result.latd( (theUlg.latd() + theUrg.latd() + theLrg.latd() + theLlg.latd())*.25);
   result.lond( (theUlg.lond() + theUrg.lond() + theLrg.lond() + theLlg.lond())*.25);

   result.datum(theUlg.datum());
   
   return result;
}

void ossimQuadProjection::worldToLineSample(const ossimGpt& worldPoint,
                                            ossimDpt&       lineSampPt) const
{
   ossimProjection::worldToLineSample(worldPoint, lineSampPt);
}

void ossimQuadProjection::lineSampleToWorld(const ossimDpt& lineSampPt,
                                            ossimGpt&       worldPt) const
{
   lineSampleHeightToWorld(lineSampPt,
                           ossim::nan(),
                           worldPt);
   
}

void ossimQuadProjection::lineSampleHeightToWorld(const ossimDpt& lineSampPt,
                                                  const double&   heightAboveEllipsoid,
                                                  ossimGpt&       worldPt) const
{
   worldPt.makeNan();
   worldPt.datum(theUlg.datum());
   
   if(theUlg.isLatNan()||
      theUlg.isLonNan()||
      theUrg.isLatNan()||
      theUrg.isLonNan()||
      theLrg.isLatNan()||
      theLrg.isLonNan()||
      theLlg.isLatNan()||
      theLlg.isLonNan()||
      theInputRect.hasNans())
   {
      return;
   }

   if(!theInputRect.pointWithin(lineSampPt))
   {
      worldPt = extrapolate(lineSampPt,
                            heightAboveEllipsoid);
      worldPt.hgt = ossimElevManager::instance()->getHeightAboveEllipsoid(worldPt);
      return;
   }
   
   worldPt.lat = theLatGrid(lineSampPt);
   worldPt.lon = theLonGrid(lineSampPt);
   worldPt.hgt = heightAboveEllipsoid;
   worldPt.hgt = ossimElevManager::instance()->getHeightAboveEllipsoid(worldPt);
}   

bool ossimQuadProjection::saveState(ossimKeywordlist& kwl,
                                    const char* prefix)const
{
   ossimProjection::saveState(kwl, prefix);
   kwl.add(prefix,
           ossimKeywordNames::DATUM_KW,
           theUlg.datum()->code(),
           true);
   
   if(theInputRect.hasNans())
   {
      kwl.add(prefix,
              ossimKeywordNames::UL_X_KW,
              "nan",
              true);
      kwl.add(prefix,
              ossimKeywordNames::UL_Y_KW,
              "nan",
              true);
      kwl.add(prefix,
              "width",
              0,
              true);
      kwl.add(prefix,
              "height",
              0,
              true);
   }
   else
   {
      kwl.add(prefix,
              ossimKeywordNames::UL_X_KW,
              theInputRect.ul().x,
              true);
      kwl.add(prefix,
              ossimKeywordNames::UL_Y_KW,
              theInputRect.ul().y,
              true);
      kwl.add(prefix,
              "width",
              theInputRect.width(),
              true);
      kwl.add(prefix,
              "height",
              theInputRect.height(),
              true);
   }
   
   if(theUlg.isLatNan()||
      theUlg.isLonNan()||
      theUrg.isLatNan()||
      theUrg.isLonNan()||
      theLrg.isLatNan()||
      theLrg.isLonNan()||
      theLlg.isLatNan()||
      theLlg.isLonNan())
   {
      kwl.add(prefix,
              ossimKeywordNames::UL_LAT_KW,
              "nan",
              true);
      kwl.add(prefix,
              ossimKeywordNames::UL_LON_KW,
              "nan",
              true);
      kwl.add(prefix,
              ossimKeywordNames::UR_LAT_KW,
              "nan",
              true);
      kwl.add(prefix,
              ossimKeywordNames::UR_LON_KW,
              "nan",
              true);
      kwl.add(prefix,
              ossimKeywordNames::LR_LAT_KW,
              "nan",
              true);
      kwl.add(prefix,
              ossimKeywordNames::LR_LON_KW,
              "nan",
              true);
      kwl.add(prefix,
              ossimKeywordNames::LL_LAT_KW,
              "nan",
              true);
      kwl.add(prefix,
              ossimKeywordNames::LL_LON_KW,
              "nan",
              true);
   }
   else
   {
      kwl.add(prefix,
              ossimKeywordNames::UL_LAT_KW,
              theUlg.latd(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::UL_LON_KW,
              theUlg.lond(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::UR_LAT_KW,
              theUrg.latd(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::UR_LON_KW,
              theUrg.lond(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::LR_LAT_KW,
              theLrg.latd(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::LR_LON_KW,
              theLrg.lond(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::LL_LAT_KW,
              theLlg.latd(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::LL_LON_KW,
              theLlg.lond(),
              true);
   }
   return true;
}

bool ossimQuadProjection::loadState(const ossimKeywordlist& kwl,
                                    const char* prefix)
{
   ossimProjection::loadState(kwl, prefix);
   theUlg.makeNan();
   theUrg.makeNan();
   theLrg.makeNan();
   theLlg.makeNan();
   theInputRect.makeNan();

   ossimString ulLat = kwl.find(prefix, ossimKeywordNames::UL_LAT_KW);
   ossimString ulLon = kwl.find(prefix, ossimKeywordNames::UL_LON_KW);
   ossimString urLat = kwl.find(prefix, ossimKeywordNames::UR_LAT_KW);
   ossimString urLon = kwl.find(prefix, ossimKeywordNames::UR_LON_KW);
   ossimString lrLat = kwl.find(prefix, ossimKeywordNames::LR_LAT_KW);
   ossimString lrLon = kwl.find(prefix, ossimKeywordNames::LR_LON_KW);
   ossimString llLat = kwl.find(prefix, ossimKeywordNames::LL_LAT_KW);
   ossimString llLon = kwl.find(prefix, ossimKeywordNames::LL_LON_KW);
   ossimString datum = kwl.find(prefix, ossimKeywordNames::DATUM_KW);
   ossimString ulX    = kwl.find(prefix, ossimKeywordNames::UL_X_KW);
   ossimString ulY    = kwl.find(prefix, ossimKeywordNames::UL_Y_KW);
   ossimString width  = kwl.find(prefix, "width");
   ossimString height = kwl.find(prefix, "height");

   if(ulLat == "nan")
   {
      theUlg.latd(ossim::nan());
   }
   else
   {
      theUlg.latd(ulLat.toDouble());
   }
   if(ulLon == "nan")
   {
      theUlg.lond(ossim::nan());
   }
   else
   {
      theUlg.lond(ulLon.toDouble());
   }
   
   if(urLat == "nan")
   {
      theUrg.latd(ossim::nan());
   }
   else
   {
      theUrg.latd(urLat.toDouble());
   }
   if(urLon == "nan")
   {
      theUrg.lond(ossim::nan());
   }
   else
   {
      theUrg.lond(urLon.toDouble());
   }
   
   if(lrLat == "nan")
   {
      theLrg.latd(ossim::nan());
   }
   else
   {
      theLrg.latd(lrLat.toDouble());
   }
   if(lrLon == "nan")
   {
      theLrg.lond(ossim::nan());
   }
   else
   {
      theLrg.lond(lrLon.toDouble());
   }
   
   if(llLat == "nan")
   {
      theLlg.latd(ossim::nan());
   }
   else
   {
      theLlg.latd(llLat.toDouble());
   }
   if(llLon == "nan")
   {
      theLlg.lond(ossim::nan());
   }
   else
   {
      theLlg.lond(llLon.toDouble());
   }

   ossimIpt ul;
   
   ul.makeNan();
   
   if((ulX != "nan") &&
      (ulY != "nan"))
   {
      ul.x = ulX.toInt32();
      ul.y = ulY.toInt32();
   }
   ossim_uint32 w = width.toUInt32();
   ossim_uint32 h = height.toUInt32();
   if(datum == "")
   {
      datum = "WGE";
   }
   const ossimDatum* datumPtr = ossimDatumFactory::instance()->create(datum);
   theUlg.datum(datumPtr);
   theUrg.datum(datumPtr);
   theLrg.datum(datumPtr);
   theLlg.datum(datumPtr);
   
   if(w&&h)
   {
      theInputRect = ossimIrect(ul.x,
                                ul.y,
                                ul.x + w - 1,
                                ul.y + h - 1);
   }
   else
   {
      theInputRect.makeNan();
   }

   initializeGrids();

   return true;
}

bool ossimQuadProjection::operator==(const ossimProjection& /* projection */) const
{
   return false;
}

ossimDpt ossimQuadProjection::getMetersPerPixel() const
{
   ossimGpt centerG;
   ossimGpt rightG;
   ossimGpt topG;

   lineSampleToWorld(theInputRect.midPoint(), centerG);
   lineSampleToWorld(theInputRect.midPoint()+ossimDpt(1,0), rightG);
   lineSampleToWorld(theInputRect.midPoint()+ossimDpt(0,-1), topG);
   
   ossimEcefPoint centerP = centerG;
   ossimEcefPoint rightP  = rightG;
   ossimEcefPoint topP    = topG;

   ossimEcefVector horizontal = rightP-centerP;
   ossimEcefVector vertical   = topP-centerP;

   ossimDpt result(horizontal.magnitude(),
                   vertical.magnitude());

   result.x = (result.x + result.y)/2.0;
   result.y = result.x;

   
   return result;
}

void ossimQuadProjection::initializeGrids()
{
   ossimIpt gridSize(2,2);
   
   ossimDpt spacing = ossimDpt((double)(theInputRect.width()-1)/(gridSize.x-1),
                               (double)(theInputRect.height()-1)/(gridSize.y-1));
   
   theLatGrid.setNullValue(ossim::nan());
   theLonGrid.setNullValue(ossim::nan());
   theLatGrid.initialize(gridSize, theInputRect.ul(), spacing);
   theLonGrid.initialize(gridSize, theInputRect.ul(), spacing);

   theLatGrid.setNode(0,0, theUlg.latd());
   theLatGrid.setNode(1,0, theUrg.latd());
   theLatGrid.setNode(1,1, theLrg.latd());
   theLatGrid.setNode(0,1, theLlg.latd());

   theLonGrid.setNode(0,0, theUlg.lond());
   theLonGrid.setNode(1,0, theUrg.lond());
   theLonGrid.setNode(1,1, theLrg.lond());
   theLonGrid.setNode(0,1, theLlg.lond());
}

ossimGpt ossimQuadProjection::extrapolate(const ossimDpt& imagePoint,
                                          const double&   height) const
{
   //---
   // If image point supplied has NaN components, return now with a NaN point.
   // This prevents an infinite recursion between model worldToLineSample
   // and this method:
   //---
   if (imagePoint.hasNans())
   {
      return ossimGpt(ossim::nan(), ossim::nan(), ossim::nan());
   }

   //***
   // Determine which edge is intersected by the radial, and establish
   // intersection:
   //***
   ossimGpt gpt;
   ossimDpt edgePt (imagePoint);
   ossimDpt image_center (theInputRect.midPoint());
   ossimDrect clipRect = theInputRect;
   clipRect.clip(image_center, edgePt);

   //***
   // Need edgePt relative to image center. Compute an epsilon perturbation in
   // the direction of edgePt for later computing directional derivative,
   // and back out the offset to origin:
   //***
   ossimDpt deltaPt (edgePt - image_center);
   ossimDpt epsilon (deltaPt/deltaPt.length());
   edgePt -= epsilon;  // insure that we are inside the image
   ossimDpt edgePt_prime (edgePt - epsilon); // epsilon=1pixel
       
   //***
   // Establish ground point corresponding to edge point and edgePt+epsilon:
   //***
   ossimGpt edgeGP;
   ossimGpt edgeGP_prime;

   if (ossim::isnan(height))
   {
      lineSampleToWorld(edgePt, edgeGP);
      lineSampleToWorld(edgePt_prime, edgeGP_prime);
   }
   else
   {
      lineSampleHeightToWorld(edgePt, height, edgeGP);
      lineSampleHeightToWorld(edgePt_prime, height, edgeGP_prime);
   }
   
   //***
   // Compute approximate directional derivatives of lat and lon along radial
   // at the edge:
   //***
   double dpixel    = (edgePt-edgePt_prime).length();
   double dlat_drad = (edgeGP.lat - edgeGP_prime.lat)/dpixel;
   double dlon_drad = (edgeGP.lon - edgeGP_prime.lon)/dpixel;

   //***
   // Now extrapolate to image point of interest:
   //***
   double delta_pixel = (imagePoint - edgePt).length();

   gpt.lat = edgeGP.lat + dlat_drad*delta_pixel;
   gpt.lon = edgeGP.lon + dlon_drad*delta_pixel;
//    if (height == OSSIM_DBL_NAN)
//    {
//       gpt.hgt = theElevation->getHeightAboveMSL(gpt);
//    }
//    else
   gpt.hgt = height;
   
   return gpt;
   
}
