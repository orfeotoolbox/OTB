//*****************************************************************************
// FILE: ossimSensorModel.cc
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// AUTHOR: Oscar Kramer (okramer@imagelinks.com)
//
// DESCRIPTION:
//   Contains implementation of class ossimSensorModel. This is the base
//   class to all sensor model-related projections including replacement
//   models such as coarse grids and polynomial models. This base class
//   supports adjustable parameters for registration adjustment.
//
//   Important note to sensor model implementors: In order to avoid a separate
//   set of "initial adjustable parameters," this design assumes ALL initial
//   values are 0. When designing the derived-class model, insure that the
//   adjustable parameters are 0-based. This applies to the adjustable param
//   array declared in this base class only. The derived classes can declare
//   their own adjstable params that are more descriptive and that can be
//   assigned an initial value thet is non-zero, but that are linearly related
//   to the adjustable params of this class. In that case, the updateModel()
//   method will compute the derived-class's parameters based on
//   theAdjustableParams array, after an adjustment is made.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimSensorModel.cpp,v 1.71 2005/09/22 11:58:12 gpotts Exp $
#include <iostream>
#include <sstream>
using namespace std;

#include <stdio.h>

#include <projections/sensor_modeling/ossimSensorModel.h>

RTTI_DEF2(ossimSensorModel, "ossimSensorModel", ossimProjection, ossimAdjustableParameterInterface);

#include <base/common/ossimConstants.h>
#include <base/common/ossimKeywordNames.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/data_types/ossimDrect.h>
#include <base/data_types/datum/ossimDatum.h>
#include <base/context/ossimNotifyContext.h>
#include <base/factory/ossimDatumFactory.h>

#include <elevation/ossimElevManager.h>

//***
// Define Trace flags for use within this file:
//***
#include <base/common/ossimTrace.h>
static ossimTrace traceExec  ("ossimSensorModel:exec");
static ossimTrace traceDebug ("ossimSensorModel:debug");

static const char*       REF_GPT_LAT_KW      = "ref_point_lat";
static const char*       REF_GPT_LON_KW      = "ref_point_lon";
static const char*       REF_GPT_HGT_KW      = "ref_point_hgt";
static const char*       REF_IPT_LINE_KW     = "ref_point_line";
static const char*       REF_IPT_SAMP_KW     = "ref_point_samp";
static const char*       IMAGE_ID_KW         = "image_id";
static const char*       SENSOR_ID_KW        = "sensor";
static const ossimString NULL_STRING         = "NULL";
static const double      RAY_ORIGIN_HEIGHT   = 10000.0; //meters

//*****************************************************************************
//  DEFAULT CONSTRUCTOR: ossimSensorModel()
//*****************************************************************************
ossimSensorModel::ossimSensorModel()
   :
   ossimProjection     (),
   ossimAdjustableParameterInterface(),
   theImageSize        (0, 0),
   theSubImageOffset   (0.0, 0.0),
   theImageID          (),
   theSensorID         (),
   theGSD              (0.0, 0.0),
   theMeanGSD          (0.0),
   theRefGndPt         (0.0, 0.0, 0.0),
   theRefImgPt         (0.0, 0.0),
   theBoundGndPolygon  (),
   theImageClipRect    (),
   theNominalPosError  (0)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::ossimSensorModel(geom_kwl): entering..." << endl;

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::ossimSensorModel(geom_kwl): returning..." << std::endl;
}

//*****************************************************************************
//  COPY CONSTRUCTOR: ossimSensorModel(ossimSensorModel)
//*****************************************************************************
ossimSensorModel::ossimSensorModel(const ossimSensorModel& model)
   :
   ossimProjection(model),
   ossimAdjustableParameterInterface(model),
   theImageSize       (model.theImageSize),
   theSubImageOffset  (model.theSubImageOffset),
   theImageID         (model.theImageID),
   theSensorID        (model.theSensorID),
   theGSD             (model.theGSD),
   theMeanGSD         (model.theMeanGSD),
   theRefGndPt        (model.theRefGndPt),
   theRefImgPt        (model.theRefImgPt),
   theBoundGndPolygon (model.theBoundGndPolygon),
   theImageClipRect   (model.theImageClipRect),
   theNominalPosError (model.theNominalPosError)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::ossimSensorModel(model): entering..." << std::endl;

   theErrorStatus = model.theErrorStatus;
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::ossimSensorModel(model): returning..." << std::endl;
   return;
}

//*****************************************************************************
//  CONSTRUCTOR: ossimSensorModel(geom_kwl)
//  
//  Constructs from a geometry keywordlist.
//  
//*****************************************************************************
ossimSensorModel::ossimSensorModel(const ossimKeywordlist& geom_kwl)
   :
   ossimProjection     (),
   ossimAdjustableParameterInterface(),
   theImageSize        (0, 0),
   theSubImageOffset   (0.0, 0.0),
   theImageID          (),
   theSensorID         (),
   theGSD              (0.0, 0.0),
   theMeanGSD          (0.0),
   theRefGndPt         (0.0, 0.0, 0.0),
   theRefImgPt         (0.0, 0.0),
   theBoundGndPolygon  (),
   theImageClipRect    (),
   theNominalPosError  (0)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::ossimSensorModel(geom_kwl): entering..." << std::endl;

//   ossimElevManager::instance()->loadState(geom_kwl);
   loadState(geom_kwl);

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::ossimSensorModel(geom_kwl): returning..." << std::endl;
   return;
}

//*****************************************************************************
//  DESTRUCTOR: ~ossimSensorModel
//  
//*****************************************************************************
ossimSensorModel::~ossimSensorModel()
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::~ossimSensorModel: entering..." << std::endl;
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::~ossimSensorModel: returning..." << std::endl;
}

ossimObject* ossimSensorModel::getBaseObject()
{
   return this;
}

const ossimObject* ossimSensorModel::getBaseObject()const
{
   return this;
}


//*****************************************************************************
//  METHOD: ossimSensorModel::lineSampleToWorld(image_pt, &gpt)
//  
//  Intersects the ray associated with image_pt with the available elevation
//  model. Returns ground point
//  
//*****************************************************************************
void ossimSensorModel::lineSampleToWorld(const ossimDpt& image_point,
                                         ossimGpt&       gpt) const
{
   bool debug = false;  // setable via interactive debugger
   if (traceExec() || debug)  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::lineSampleToWorld:entering..." << std::endl;
   
   if(image_point.hasNans())
   {
      gpt.makeNan();
      return;
   }
   //***
   // Extrapolate if image point is outside image:
   //***
   if (!insideImage(image_point))
   {
      gpt = extrapolate(image_point);
      return;
   }

   //***
   // Determine imaging ray and invoke elevation source object's services to
   // intersect ray with terrain model:
   //***
   ossimEcefRay ray;
   imagingRay(image_point, ray);
   ossimElevManager::instance()->intersectRay(ray, gpt);

   if (traceDebug() || debug)
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "image_point = " << image_point << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "ray = " << ray << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "gpt = " << gpt << std::endl;
   }

   if (traceExec() || debug)  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::lineSampleToWorld: returning..." << std::endl;
   return;
}

//*****************************************************************************
//  METHOD: ossimSensorModel::worldToLineSample()
//  
//  Performs forward projection of ground point to image space.
//  
//*****************************************************************************
void ossimSensorModel::worldToLineSample(const ossimGpt& worldPoint,
                                         ossimDpt&       ip) const
{
   //   static bool recursionFlag = false;

   static const double PIXEL_THRESHOLD    = 0.1; // acceptable pixel error
   static const int    MAX_NUM_ITERATIONS = 20;


   if(worldPoint.isLatNan()||
      worldPoint.isLonNan())
     {
       ip.makeNan();
       return;
     }
      
   //***
   // First check if the world point is inside bounding rectangle:
   //***
   int iters = 0;
   ossimDpt wdp (worldPoint);
   //   if ((!recursionFlag)&&!(theBoundGndPolygon.pointWithin(wdp)))

   if((theBoundGndPolygon.getNumberOfVertices() > 0)&&
      (!theBoundGndPolygon.hasNans()))
   {
      if (!(theBoundGndPolygon.pointWithin(wdp)))
      {
         //      recursionFlag = true;
         ip = extrapolate(worldPoint);
         //      recursionFlag = false;
         return;
      }         
   }
   //***
   // Substitute zero for null elevation if present:
   //***
   double height = worldPoint.hgt;
   if ((height == ossimElevSource::DEFAULT_NULL_HEIGHT)||
       (height == OSSIM_DBL_NAN))
      height = 0.0;

   //***
   // Utilize iterative scheme for arriving at image point. Begin with guess
   // at image center:
   //***
   ip.u = theRefImgPt.u;
   ip.v = theRefImgPt.v;
   
   ossimDpt ip_du;
   ossimDpt ip_dv;

   ossimGpt gp, gp_du, gp_dv;
   double dlat_du, dlat_dv, dlon_du, dlon_dv;
   double delta_lat, delta_lon, delta_u, delta_v;
   double inverse_norm;
   
   //***
   // Begin iterations:
   //***
   do
   {
      //***
      // establish perturbed image points about the guessed point:
      //***
      ip_du.u = ip.u + 1.0;
      ip_du.v = ip.v;
      ip_dv.u = ip.u;
      ip_dv.v = ip.v + 1.0;
      
      //***
      // Compute numerical partials at current guessed point:
      //***
      lineSampleHeightToWorld(ip,    height, gp);
      lineSampleHeightToWorld(ip_du, height, gp_du);
      lineSampleHeightToWorld(ip_dv, height, gp_dv);

      dlat_du = gp_du.lat - gp.lat; //e
      dlon_du = gp_du.lon - gp.lon; //g
      dlat_dv = gp_dv.lat - gp.lat; //f
      dlon_dv = gp_dv.lon - gp.lon; //h

      //***
      // Test for convergence:
      //***
      delta_lat = worldPoint.lat - gp.lat;
      delta_lon = worldPoint.lon - gp.lon;
      
      //***
      // Compute linearized estimate of image point given gp delta:
      //***
      inverse_norm = dlat_dv*dlon_du - dlat_du*dlon_dv; // fg-eh
      if (inverse_norm != 0)
      {
          delta_u = (-dlon_dv*delta_lat + dlat_dv*delta_lon)/inverse_norm;
          delta_v = ( dlon_du*delta_lat - dlat_du*delta_lon)/inverse_norm;
          ip.u += delta_u;
          ip.v += delta_v;
      }
      else
      {
         delta_u = 0;
         delta_v = 0;
      }

      iters++;
    } while (((fabs(delta_u) > PIXEL_THRESHOLD) ||
              (fabs(delta_v) > PIXEL_THRESHOLD)) &&
             (iters < MAX_NUM_ITERATIONS));
//    } while (((fabs(delta_u) > PIXEL_THRESHOLD) ||
//              (fabs(delta_v) > PIXEL_THRESHOLD)) &&
//             (iters < MAX_NUM_ITERATIONS));

   //***
   // Note that this error mesage appears only if max count was reached while
   // iterating. A linear (no iteration) solution would finish with iters =
   // MAX_NUM_ITERATIONS + 1:
   //***
   if (iters == MAX_NUM_ITERATIONS)
   {
   }

   //***
   // The image point computed this way corresponds to full image space.
   // Apply image offset in the case this is a sub-image rectangle:
   //***
      ip -= theSubImageOffset;

   return;
}

//*****************************************************************************
//  METHOD: ossimSensorModel::print(ostream)
//  
//  Dumps contents of object to ostream.
//  
//*****************************************************************************
std::ostream& ossimSensorModel::print(std::ostream& out) const
{
   out << setprecision(15) << setiosflags(ios::fixed)
       << "\n ossimSensorModel base-class data members:\n"
       << "\n         theImageID: " << theImageID
       << "\n        theSensorID: " << theSensorID
       << "\n       theImageSize: " << theImageSize
       << "\n  theSubImageOffset: " << theSubImageOffset
       << "\n             theGSD: " << theGSD
       << "\n         theMeanGSD: " << theMeanGSD
       << "\n        theRefGndPt: " << theRefGndPt
       << "\n        theRefImgPt: " << theRefImgPt
       << "\n theBoundGndPolygon: \n" << theBoundGndPolygon
       << "\n theBoundGndPolygon: " << theBoundGndPolygon
       << "\n   theImageClipRect: " << theImageClipRect
       << "\n theNominalPosError: " << theNominalPosError
       << endl;

   //---
   // Note:
   // need ossimAdjustableParameterInterface::print(os);
   //---

   return ossimProjection::print(out);
}

void ossimSensorModel::setImageRect(const ossimDrect& imageRect)
{
   theImageClipRect = imageRect;
   theRefImgPt = imageRect.midPoint();
}
void ossimSensorModel::setGroundRect(const ossimGpt& ul,
                                     const ossimGpt& ur,
                                     const ossimGpt& lr,
                                     const ossimGpt& ll)
{
   theBoundGndPolygon.clear();
   theBoundGndPolygon.addPoint(ul);
   theBoundGndPolygon.addPoint(ur);
   theBoundGndPolygon.addPoint(lr);
   theBoundGndPolygon.addPoint(ll);
}


//*****************************************************************************
//  METHOD: ossimSensorModel::saveState()
//  
//  Saves the state of this object to KWL.
//  
//*****************************************************************************
bool ossimSensorModel::saveState(ossimKeywordlist& kwl,
                                 const char*       prefix) const 
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::saveState: entering..." << std::endl;

   kwl.add(prefix, IMAGE_ID_KW, theImageID.chars());
   kwl.add(prefix, SENSOR_ID_KW, theSensorID.chars());
   
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_LINES_KW,
           theImageSize.line,
           true);
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_SAMPLES_KW,
           theImageSize.samp,
           true);
   
   kwl.add(prefix,
           REF_GPT_LAT_KW,
           theRefGndPt.lat,
           true);
   
   kwl.add(prefix,
           REF_GPT_LON_KW,
           theRefGndPt.lon,
           true);
   
   kwl.add(prefix,
           REF_GPT_HGT_KW,
           theRefGndPt.hgt,
           true);
   
   kwl.add(prefix,
           REF_IPT_LINE_KW,
           theRefImgPt.line,
           true);
      
   kwl.add(prefix,
           REF_IPT_SAMP_KW,
           theRefImgPt.samp,
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::METERS_PER_PIXEL_Y_KW,
           theGSD.line,
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::METERS_PER_PIXEL_X_KW,
           theGSD.samp,
           true);
   
   ossimDpt corner;
   if(!theBoundGndPolygon.vertex(0, corner))
   {
      corner = ossimDpt(0,0);
   }
   
   kwl.add(prefix,
           ossimKeywordNames::UL_LAT_KW,
           corner.lat,
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::UL_LON_KW,
           corner.lon,
           true);
   
   if(!theBoundGndPolygon.nextVertex(corner))
   {
      corner = ossimDpt(0,0);
   }
   kwl.add(prefix,
           ossimKeywordNames::UR_LAT_KW,
           corner.lat,
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::UR_LON_KW,
           corner.lon,
           true);
   
   if(!theBoundGndPolygon.nextVertex(corner))
   {
      corner = ossimDpt(0,0);
   }
   kwl.add(prefix,
           ossimKeywordNames::LR_LAT_KW,
           corner.lat,
           true);
   kwl.add(prefix,
           ossimKeywordNames::LR_LON_KW,
           corner.lon,
           true);
   
   
   if(!theBoundGndPolygon.nextVertex(corner))
   {
      corner = ossimDpt(0,0);
   }
   kwl.add(prefix,
           ossimKeywordNames::LL_LAT_KW,
           corner.lat,
           true);
   kwl.add(prefix,
           ossimKeywordNames::LL_LON_KW,
           corner.lon,
           true);


   kwl.add(prefix,
           "rect",
           ossimString::toString(theImageClipRect.ul().x)
           + " " + ossimString::toString(theImageClipRect.ul().y) + " " +
           ossimString::toString(theImageClipRect.lr().x) + " " +
           ossimString::toString(theImageClipRect.lr().y),
           true);

   saveAdjustments(kwl, prefix);
   //
   // Also save the state of the elevation object:
   //
//   ossimElevManager::instance()->saveState(kwl, prefix);
   
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::saveState:returning..." << std::endl;
   return ossimProjection::saveState(kwl, prefix);;
}

//*****************************************************************************
//  METHOD: ossimSensorModel::loadState()
//  
//  Loads the state of this object from KWL.
//  
//*****************************************************************************
bool ossimSensorModel::loadState(const ossimKeywordlist& kwl,
                                 const char*       prefix)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::loadState: entering..." << std::endl;

   const char* keyword;
   const char* value;
//   int nconv;
   ossimDpt v[4]; // temporarily holds vertices for ground polygon

   //***
   // Read each keyword, breaking out if error occurs:
   //***
   keyword = IMAGE_ID_KW;
   value = kwl.find(prefix, keyword);
   if (value)
      theImageID = value;
   else
      theImageID = NULL_STRING;
   
   keyword = SENSOR_ID_KW;
   value = kwl.find(prefix, keyword);
   if (value)
      theSensorID = value;
   else
      theSensorID = NULL_STRING;
      
   keyword = ossimKeywordNames::NUMBER_LINES_KW;//IMAGE_SIZE_LINES_KW;
   value = kwl.find(prefix, keyword);
   if (value)
   {
      theImageSize.line = ossimString(value).toLong();
   }

   keyword = ossimKeywordNames::NUMBER_SAMPLES_KW;// IMAGE_SIZE_SAMPS_KW;
   value = kwl.find(prefix, keyword);
   if (value)
   {
      theImageSize.samp = ossimString(value).toLong();
   }

   keyword = REF_IPT_LINE_KW;
   value = kwl.find(prefix, keyword);
   if (value)
   {
      theRefImgPt.line = ossimString(value).toDouble();
   }

   keyword = REF_IPT_SAMP_KW;
   value = kwl.find(prefix, keyword);
   if (value)
   {
      theRefImgPt.samp = ossimString(value).toDouble();
   }

   keyword = REF_GPT_LAT_KW;
   value = kwl.find(prefix, keyword);
   if(value)
   {
      theRefGndPt.latd(ossimString(value).toDouble());
   }
   

   keyword = REF_GPT_LON_KW;
   value = kwl.find(prefix, keyword);
   if(value)
   {
      theRefGndPt.lond(ossimString(value).toDouble());
   }
   

   keyword = REF_GPT_HGT_KW;
   value = kwl.find(prefix, keyword);
   if(value)
   {
      theRefGndPt.hgt = ossimString(value).toDouble();
   }
   

   keyword = ossimKeywordNames::METERS_PER_PIXEL_Y_KW;// GSD_LINE_DIR_KW;
   value = kwl.find(prefix, keyword);
   if (value)
   {
      theGSD.line = ossimString(value).toDouble();
   }

   keyword = ossimKeywordNames::METERS_PER_PIXEL_X_KW;//GSD_SAMP_DIR_KW;
   value = kwl.find(prefix, keyword);
   if (value)
   {
      theGSD.samp = ossimString(value).toDouble();
   }

   theMeanGSD = (fabs(theGSD.line) + fabs(theGSD.samp))/2.0;

   keyword = ossimKeywordNames::UL_LAT_KW;
   value = kwl.find(prefix, keyword);
   if (value)
   {
      v[0].lat = ossimString(value).toDouble();
   }

   keyword = ossimKeywordNames::UL_LON_KW;
   value = kwl.find(prefix, keyword);
   if (value)
   {
      v[0].lon = ossimString(value).toDouble();
   }

   keyword = ossimKeywordNames::UR_LAT_KW;
   value = kwl.find(prefix, keyword);
   if (value)
   {
      v[1].lat = ossimString(value).toDouble();
   }

   keyword = ossimKeywordNames::UR_LON_KW;
   value = kwl.find(prefix, keyword);
   if (value)
   {
      v[1].lon = ossimString(value).toDouble();
   }

   keyword = ossimKeywordNames::LR_LAT_KW;
   value = kwl.find(prefix, keyword);
   if (value)
   {
      v[2].lat = ossimString(value).toDouble();
   }

   keyword = ossimKeywordNames::LR_LON_KW;
   value = kwl.find(prefix, keyword);
   if (value)
   {
      v[2].lon = ossimString(value).toDouble();
   }

   keyword = ossimKeywordNames::LL_LAT_KW;
   value = kwl.find(prefix, keyword);
   if (value)
   {
      v[3].lat = ossimString(value).toDouble();
   }

   keyword = ossimKeywordNames::LL_LON_KW;
   value = kwl.find(prefix, keyword);
   if (value)
   {
      v[3].lon = ossimString(value).toDouble();
   }

   //***
   // Initialize other data members given quantities read in KWL:
   //***
   theBoundGndPolygon = ossimPolygon(4, v);

   const char* rect = kwl.find(prefix, "rect");
   if(rect)
   {
      std::istringstream in(rect);
      double ulx, uly, lrx, lry;
      in >> ulx >> uly >> lrx >> lry;
      theImageClipRect = ossimDrect(ulx, uly, lrx, lry);
   }
   else
   {
      theImageClipRect = ossimDrect(0.0, 0.0,
                                    theImageSize.samp-1, theImageSize.line-1);
   }
   loadAdjustments(kwl, prefix);

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::loadState: returning..." << std::endl;
   return ossimProjection::loadState(kwl, prefix);;
}

//*****************************************************************************
// PROTECTED METHOD: Model_type::extrapolate(image_point)
//
//  This method computes extrapolated values of latitude and longitude for
//  points which are outside the actual image boundaries.
//*****************************************************************************
ossimGpt ossimSensorModel::extrapolate (const ossimDpt& imagePoint,
                                        const double&   height) const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) <<  "DEBUG ossimSensorModel::extrapolate: entering... " << std::endl;

   //***
   // If image point supplied has NaN components, return now with a NaN point.
   // This prevents an infinite recursion between model worldToLineSample
   // and this method:
   //***
   if ((imagePoint.line == OSSIM_DBL_NAN) || (imagePoint.samp == OSSIM_DBL_NAN))
   {
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::extrapolate: returning..." << std::endl;
      return ossimGpt(OSSIM_DBL_NAN, OSSIM_DBL_NAN, OSSIM_DBL_NAN);
   }

   //***
   // Determine which edge is intersected by the radial, and establish
   // intersection:
   //***
   ossimGpt gpt;
   ossimDpt edgePt (imagePoint);
   ossimDpt image_center (theRefImgPt);
   theImageClipRect.clip(image_center, edgePt);

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

   if (height == OSSIM_DBL_NAN)
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
   if (height == ossimElevSource::DEFAULT_NULL_HEIGHT)
   {
      gpt.hgt = ossimElevManager::instance()->getHeightAboveEllipsoid(gpt);
   }
   else
      gpt.hgt = height;
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::extrapolate: returning..." << std::endl;
   return gpt;
}


//*****************************************************************************
// PROTECTED METHOD: Model_type::extrapolate(ground_point)
//
//  This method computes extrapolated values of line and sample for
//  points which are outside the actual image boundaries.
//*****************************************************************************
ossimDpt ossimSensorModel::extrapolate (const ossimGpt& gpt) const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) <<  "DEBUG ossimSensorModel::extrapolate: entering... " << std::endl;

   double height = 0.0;
   //***
   // If ground point supplied has NaN components, return now with a NaN point.
   //***
   if ((gpt.lat==OSSIM_DBL_NAN) ||
       (gpt.lon==OSSIM_DBL_NAN)) //||
//       (gpt.hgt==OSSIM_DBL_NAN))
   {
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::extrapolate: returning..." << std::endl;
      return ossimDpt(OSSIM_DBL_NAN, OSSIM_DBL_NAN);
   }
   if(gpt.hgt != OSSIM_DBL_NAN)
   {
      height = gpt.hgt;
   }
   //***
   // Determine which edge is intersected by the radial, and establish
   // intersection:
   //***
   ossimDpt edgePt (gpt);
   ossimDpt image_center (theRefGndPt);
   theBoundGndPolygon.clipLineSegment(image_center, edgePt);

   //---
   // Compute an epsilon perturbation in the direction away from edgePt for
   // later computing directional derivative:
   //---
   const double  DEG_PER_MTR =  8.983152841e-06; // Equator WGS-84...
   double epsilon = theMeanGSD*DEG_PER_MTR; //degrees (latitude) per pixel
   ossimDpt deltaPt (edgePt-image_center);
   ossimDpt epsilonPt (deltaPt*epsilon/deltaPt.length());
   edgePt -= epsilonPt;
   ossimDpt edgePt_prime (edgePt - epsilonPt);
       
   //***
   // Establish image point corresponding to edge point and edgePt+epsilon:
   //***
      ossimGpt edgeGP       (edgePt.lat,       edgePt.lon,       height);//gpt.hgt);
      ossimGpt edgeGP_prime (edgePt_prime.lat, edgePt_prime.lon, height);//gpt.hgt);

   worldToLineSample(edgeGP, edgePt);
   worldToLineSample(edgeGP_prime, edgePt_prime);

   //***
   // Compute approximate directional derivatives of line and sample along
   // radial at the edge:
   //***
   double dsamp_drad = (edgePt.samp - edgePt_prime.samp)/epsilon;
   double dline_drad = (edgePt.line - edgePt_prime.line)/epsilon;

   //***
   // Now extrapolate to point of interest:
   //***
   double delta = (ossimDpt(gpt) - ossimDpt(edgeGP)).length();

   
   ossimDpt extrapolated_ip (edgePt.samp + delta*dsamp_drad,
                             edgePt.line + delta*dline_drad);

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::extrapolate: returning..." << std::endl;

   return extrapolated_ip;
}


//*****************************************************************************
//  METHOD: ossimSensorModel::imagingRay()
//  
//  Default implementation for computing imaging ray from image point.
//  
//*****************************************************************************
void ossimSensorModel::imagingRay(const ossimDpt& image_point,
                                  ossimEcefRay&   image_ray) const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::imagingRay: entering..." << std::endl;

   ossimGpt start;
   ossimGpt end;

   lineSampleHeightToWorld(image_point, RAY_ORIGIN_HEIGHT, start);
   lineSampleHeightToWorld(image_point, 0.0, end);

   image_ray = ossimEcefRay(start, end);

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::imagingRay: returning..." << std::endl;
   return;
}

//*****************************************************************************
//  STATIC METHOD: ossimSensorModel::writeGeomTemplate
//  
//  Outputs a sample geometry KWL to stream provided.
//  
//*****************************************************************************
void ossimSensorModel::writeGeomTemplate(ostream& os)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::writeGeomTemplate:entering..." << std::endl;
   
   os << "//***\n"
      << "// Base-class ossimSensorModel Keywords:\n"
      << "//***\n"
      << ossimKeywordNames::ID_KW << ":  <string>\n"
      << SENSOR_ID_KW << ": <string>\n"
      << ossimKeywordNames::NUMBER_LINES_KW << ": <int>\n"
      << ossimKeywordNames::NUMBER_SAMPLES_KW << ": <int>\n"
      << REF_GPT_LAT_KW << ": <decimal degrees>\n"
      << REF_GPT_LON_KW << ": <decimal degrees>\n"
      << REF_GPT_HGT_KW << ": <float meters>\n"
      << REF_IPT_LINE_KW << ": <float>\n"
      << REF_IPT_SAMP_KW << ": <float>\n"
      << ossimKeywordNames::METERS_PER_PIXEL_Y_KW << ": <float meters>\n"
      << ossimKeywordNames::METERS_PER_PIXEL_X_KW << ": <float meters>\n"
      << ossimKeywordNames::UL_LAT_KW << ": <decimal degrees>\n"
      << ossimKeywordNames::UL_LON_KW << ": <decimal degrees>\n"
      << ossimKeywordNames::UR_LAT_KW << ": <decimal degrees>\n"
      << ossimKeywordNames::UR_LON_KW << ": <decimal degrees>\n"
      << ossimKeywordNames::LR_LAT_KW << ": <decimal degrees>\n"
      << ossimKeywordNames::LR_LON_KW << ": <decimal degrees>\n"
      << ossimKeywordNames::LL_LAT_KW << ": <decimal degrees>\n"
      << ossimKeywordNames::LL_LON_KW << ": <decimal degrees>\n"
      << "\n"
      << "//***\n"
      << "// Repeat following four entries for each adjustable parameter:\n"
      << "//***\n"
//       << PARAM_PREFIX << "N." << PARAM_NAME_KW << ": <string>\n" 
//       << PARAM_PREFIX << "N." << PARAM_UNITS_KW << ": <string>\n" 
//       << PARAM_PREFIX << "N." << PARAM_VALUE_KW << ": <float>\n" 
//       << PARAM_PREFIX << "N." << PARAM_SIGMA_KW << ": <float>\n"
      << std::endl;

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::writeGeomTemplate: returning..." << std::endl;
   return;
}

