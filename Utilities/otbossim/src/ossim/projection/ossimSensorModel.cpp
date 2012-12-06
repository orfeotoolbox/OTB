//*****************************************************************************
// FILE: ossimSensorModel.cc
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// AUTHOR: Oscar Kramer
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
//  $Id: ossimSensorModel.cpp 21808 2012-10-05 14:23:33Z dhicks $
#include <iostream>
#include <sstream>
using namespace std;

// #include <stdio.h>

#include <ossim/projection/ossimSensorModel.h>

RTTI_DEF3(ossimSensorModel, "ossimSensorModel", ossimProjection, ossimOptimizableProjection, ossimAdjustableParameterInterface);

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimDatumFactory.h>

#include <ossim/elevation/ossimElevManager.h>
#include <ossim/base/ossimTieGptSet.h>

#include <ossim/matrix/newmatrc.h>

//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>
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


//DEBUG TBR : output ops
std::ostream& operator<<(std::ostream& os, NEWMAT::GeneralMatrix& mat)
{
   int nr=mat.Nrows();
   int nc=mat.Ncols();

   NEWMAT::MatrixRow crow(&mat,NEWMAT::LoadOnEntry);
//   NEWMAT::MatrixRowCol crow;
   mat.RestoreRow(crow);  
   for (int r=0;r<nr;++r)
   {
      for (int c=0;c<nc;++c)
      {
         os<<*(crow.Data()+c)<<" ";
      }
      os<<std::endl;
      mat.NextRow(crow);
   }
   return os;
}

//*****************************************************************************
//  DEFAULT CONSTRUCTOR: ossimSensorModel()
//*****************************************************************************
ossimSensorModel::ossimSensorModel()
   :
   ossimOptimizableProjection       (),
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
   theRelPosError      (0),
   theNominalPosError  (0),
   theExtrapolateImageFlag(false),
   theExtrapolateGroundFlag(false)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::ossimSensorModel(geom_kwl): entering..." << endl;

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::ossimSensorModel(geom_kwl): returning..." << std::endl;
}

//*****************************************************************************
//  COPY CONSTRUCTOR: ossimSensorModel(ossimSensorModel)
//*****************************************************************************
ossimSensorModel::ossimSensorModel(const ossimSensorModel& model)
   :
   ossimOptimizableProjection(model),
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
   theRelPosError      (model.theRelPosError),
   theNominalPosError (model.theNominalPosError),
   theExtrapolateImageFlag(false),
   theExtrapolateGroundFlag(false)
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
   ossimOptimizableProjection     (),
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
   theRelPosError      (0),
   theNominalPosError      (0),
   theExtrapolateImageFlag(false),
   theExtrapolateGroundFlag(false)
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
   if (!insideImage(image_point)&&(!theExtrapolateImageFlag))
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

   static const double PIXEL_THRESHOLD    = .1; // acceptable pixel error
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
         if(theSeedFunction.valid())
         {
            theSeedFunction->worldToLineSample(worldPoint, ip);
         }
         else if(!theExtrapolateGroundFlag) // if I am not already in the extrapolation routine

         {
         //      recursionFlag = true;
            ip = extrapolate(worldPoint);
         //      recursionFlag = false;
         }
         return;
      }         
   }

   //***
   // Substitute zero for null elevation if present:
   //***
   double height = worldPoint.hgt;
   if ( ossim::isnan(height) )
   {
      height = 0.0;
   }

   //
   // Utilize iterative scheme for arriving at image point. Begin with guess
   // at image center:
   //
   if(theSeedFunction.valid())
   {
      theSeedFunction->worldToLineSample(worldPoint, ip);
   }
   else
   {
      ip.u = theRefImgPt.u;
      ip.v = theRefImgPt.v;
   }
   
   ossimDpt ip_du;
   ossimDpt ip_dv;

   ossimGpt gp, gp_du, gp_dv;
   double dlat_du, dlat_dv, dlon_du, dlon_dv;
   double delta_lat, delta_lon, delta_u, delta_v;
   double inverse_norm;
   bool done = false;
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

      if(gp.isLatNan() || gp.isLonNan())
      {
         gp = extrapolate(ip);
      }
      if(gp_du.isLatNan() || gp_du.isLonNan())
      {
         gp_du = extrapolate(ip_du);
      }
      if(gp_dv.isLatNan() | gp_dv.isLonNan())
      {
         gp_dv = extrapolate(ip_dv);
         
      }
      dlat_du = gp_du.lat - gp.lat; //e
      dlon_du = gp_du.lon - gp.lon; //g
      dlat_dv = gp_dv.lat - gp.lat; //f
      dlon_dv = gp_dv.lon - gp.lon; //h
      
      //
      // Test for convergence:
      //
      delta_lat = worldPoint.lat - gp.lat;
      delta_lon = worldPoint.lon - gp.lon;


      //
      // Compute linearized estimate of image point given gp delta:
      //
      inverse_norm = dlat_dv*dlon_du - dlat_du*dlon_dv; // fg-eh
      
      if (!ossim::almostEqual(inverse_norm, 0.0, DBL_EPSILON))
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
      done = ((fabs(delta_u) < PIXEL_THRESHOLD)&&
              (fabs(delta_v) < PIXEL_THRESHOLD));
      iters++;
   } while ((!done) &&
             (iters < MAX_NUM_ITERATIONS));
//    } while (((fabs(delta_u) > PIXEL_THRESHOLD) ||
//              (fabs(delta_v) > PIXEL_THRESHOLD)) &&
//             (iters < MAX_NUM_ITERATIONS));

   //***
   // Note that this error mesage appears only if max count was reached while
   // iterating. A linear (no iteration) solution would finish with iters =
   // MAX_NUM_ITERATIONS + 1:
   //***
   if (iters >= MAX_NUM_ITERATIONS)
   {
//       std::cout << "MAX ITERATION!!!" << std::endl;
//       std::cout << "delta_u = "   << delta_u
//                 << "\ndelta_v = " << delta_v << "\n";
   }
   else
   {
//       std::cout << "ITERS === " << iters << std::endl;
   }
//    std::cout << "iters = " << iters << "\n";
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
       << "\n   theImageClipRect: " << theImageClipRect
       << "\n theNominalPosError: " << theNominalPosError
       << "\n     theNominalPosError: " << theNominalPosError
       << "\n     theRelPosError: " << theRelPosError
       << endl;

   //---
   // Note:
   // need ossimAdjustableParameterInterface::print(os);
   //---
   return ossimProjection::print(out);
}

void ossimSensorModel::setRefImgPt(const ossimDpt& pt)
{
   theRefImgPt = pt;
}

void ossimSensorModel::setRefGndPt(const ossimGpt& pt)
{
   theRefGndPt = pt;
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
           ossimKeywordNames::CE90_ABSOLUTE_KW,
           theNominalPosError,
           true, 20);

   kwl.add(prefix,
           ossimKeywordNames::CE90_RELATIVE_KW,
           theRelPosError,
           true, 20);

   kwl.add(prefix,
           "rect",
           ossimString::toString(theImageClipRect.ul().x)
           + " " + ossimString::toString(theImageClipRect.ul().y) + " " +
           ossimString::toString(theImageClipRect.lr().x) + " " +
           ossimString::toString(theImageClipRect.lr().y),
           true);

   // Avoid passing null char* to method that takes an ossimString.
   ossimString tmpStr;
   if (prefix)
   {
      tmpStr = prefix;
   }
   saveAdjustments(kwl, tmpStr);
      
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

   keyword = ossimKeywordNames::CE90_ABSOLUTE_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      // Try old keyword for legacy purposes:
      keyword = ossimKeywordNames::IMAGE_CE90_KW;
      value = kwl.find(prefix, keyword);
   }
   if (value)
      theNominalPosError = atof(value);
   else
      theNominalPosError = 0.0;

   keyword = ossimKeywordNames::CE90_RELATIVE_KW;
   value = kwl.find(prefix, keyword);
   if (value)
      theRelPosError = atof(value);
   else
      theRelPosError = theNominalPosError;

   //***
   // Initialize other data members given quantities read in KWL:
   //***
   theBoundGndPolygon = ossimPolygon(4, v);

   const char* rect = kwl.find(prefix, "rect");
   if(rect)
   {
      std::vector<ossimString> splitArray;
      ossimString rectString(rect);
      rectString = rectString.trim();
      rectString.split(splitArray, " ");
      if(splitArray.size() == 4)
      {
         theImageClipRect = ossimDrect(splitArray[0].toDouble(),
                                       splitArray[1].toDouble(),
                                       splitArray[2].toDouble(),
                                       splitArray[3].toDouble());
      }
      else
      {
         theImageClipRect = ossimDrect(0.0, 0.0,
                                       theImageSize.samp-1, theImageSize.line-1);
      }
   }
   else
   {
      theImageClipRect = ossimDrect(0.0, 0.0,
                                    theImageSize.samp-1, theImageSize.line-1);
   }
   
   // Avoid passing null char* to method that takes an ossimString.
   ossimString tmpStr;
   if (prefix)
   {
      tmpStr = prefix;
   }
   loadAdjustments(kwl, tmpStr);

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
   theExtrapolateImageFlag = true;
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) <<  "DEBUG ossimSensorModel::extrapolate: entering... " << std::endl;

   //---
   // If image point supplied has NaN components, return now with a NaN point.
   // This prevents an infinite recursion between model worldToLineSample
   // and this method:
   //---
   if (imagePoint.hasNans())
   {
      theExtrapolateImageFlag = false;
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::extrapolate: returning..." << std::endl;
      return ossimGpt(ossim::nan(), ossim::nan(), ossim::nan());
   }

   if(theSeedFunction.valid())
   {
      ossimGpt wpt;

      theSeedFunction->lineSampleToWorld(imagePoint, wpt);
      theExtrapolateImageFlag = false;
      return wpt;
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
   if ( ossim::isnan(height) )
   {
      gpt.hgt = ossimElevManager::instance()->getHeightAboveEllipsoid(gpt);
   }
   else
   {
      gpt.hgt = height;
   }
   theExtrapolateImageFlag = false;
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
   theExtrapolateGroundFlag = true;
   double height = 0.0;
   //---
   // If ground point supplied has NaN components, return now with a NaN point.
   //---
   if ( (ossim::isnan(gpt.lat)) || (ossim::isnan(gpt.lon)) )
//       (gpt.hgt==OSSIM_DBL_NAN))
   {
      theExtrapolateGroundFlag = false;
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::extrapolate: returning..." << std::endl;
      return ossimDpt(ossim::nan(), ossim::nan());
   }
   if(ossim::isnan(gpt.hgt) == false)
   {
      height = gpt.hgt;
   }
   
   if(theSeedFunction.valid())
   {
      ossimDpt ipt;

      theSeedFunction->worldToLineSample(gpt, ipt);

      theExtrapolateGroundFlag = false;
     return ipt;
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

   theExtrapolateGroundFlag = false;
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
//  METHOD: ossimSensorModel::getObsCovMat()
//  
//  Default implementation for forming observation covariance matrix.
//
//    Note: At this base class level, the only error source currently
//          considered is mensuration error.  This is obviously optimistic,
//          but is included as a placeholder/example, and is presently
//          the trivial case.
//  
//*****************************************************************************
ossimSensorModel::CovMatStatus ossimSensorModel::getObsCovMat(
   const ossimDpt& /* ipos */ , NEWMAT::SymmetricMatrix& Cov, const ossim_float64 defPointingSigma)
{
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // Mensuration error contribution
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   //  Sensitivity matrix
   NEWMAT::SymmetricMatrix B(2);
   B = 0.0;
   B(1,1) = 1.0;
   B(2,2) = B(1,1);

   //  Pointing covariance matrix
   NEWMAT::SymmetricMatrix P(2);
   P = 0.0;
   P(1,1) = defPointingSigma*defPointingSigma;
   P(2,2) = P(1,1);

   //  Propagate
   NEWMAT::SymmetricMatrix Cm;
   Cm << B * P * B.t();

   // ~~~~~~~~~~~~~~~~~~~~
   // Sum total covariance
   // ~~~~~~~~~~~~~~~~~~~~
   NEWMAT::SymmetricMatrix Ctot = Cm; //+ other contributors as identified

   // ~~~~~~~~~~~~~~~~~~
   // Propagate to image
   // ~~~~~~~~~~~~~~~~~~
   NEWMAT::SymmetricMatrix Bi(2);
   Bi = 0.0;
   Bi(1,1) = 1.0;
   Bi(2,2) = Bi(1,1);

   Cov << Bi * Ctot * Bi.t();

   return ossimSensorModel::COV_PARTIAL;
}

void ossimSensorModel::computeGsd()
{
   static const char MODULE[] = "ossimSensorModel::computeGsd";

   if (theImageSize.hasNans())
   {
      std::string e = MODULE;
      e += "Error image size has nans!";
      throw ossimException(e);
   }

   //---
   // Compute gsd in the x direction from left to right points across the middle of image.
   // Compute gsd in the y direction from top to bottom points across the middle of image.
   //---

   ossim_float64 midLine = 0.0;
   ossim_float64 midSamp = 0.0;
   ossim_float64 endLine = 1.0;
   ossim_float64 endSamp = 1.0;
   if (theImageSize.x > 2)
   {
      midSamp = (theImageSize.x-1)/2.0;
      endSamp = theImageSize.x-1;
         
   }
   if (theImageSize.y > 2)
   {
      midLine = (theImageSize.y-1)/2.0;
      endLine = theImageSize.y-1;
   }
   
   ossimDpt leftDpt  (0.0,     midLine);
   ossimDpt rightDpt (endSamp, midLine);
   ossimDpt topDpt   (midSamp, 0.0);
   ossimDpt bottomDpt(midSamp, endLine);
   
   ossimGpt leftGpt;
   ossimGpt rightGpt;
   ossimGpt topGpt;
   ossimGpt bottomGpt;

   //---
   // Left point.
   // For the first point use lineSampleToWorld to get the height.
   //---
   lineSampleToWorld(leftDpt, leftGpt);
   if (leftGpt.hasNans())
   {
      std::string e = MODULE;
      e += "Error leftGpt has nans!";
      throw ossimException(e);
   }

   //---
   // Right point:
   // Use lineSampleHeightToWorld using the left height since we want the horizontal distance.
   //---
   lineSampleHeightToWorld(rightDpt, leftGpt.hgt, rightGpt);
   if (rightGpt.hasNans())
   {
      std::string e = MODULE;
      e += "Error rightGpt has nans!";
      throw ossimException(e);
   }

   //---
   // Top point:
   // Use lineSampleHeightToWorld using the left height since we want the horizontal distance.
   //---
   lineSampleHeightToWorld(topDpt, leftGpt.hgt, topGpt);
   if (topGpt.hasNans())
   {
      std::string e = MODULE;
      e += "Error topGpt has nans!";
      throw ossimException(e);
   }
   
   //---
   // Bottom point:
   // Use lineSampleHeightToWorld using the left height since we want the horizontal distance.
   //---
   lineSampleHeightToWorld(bottomDpt, leftGpt.hgt, bottomGpt);
   if (bottomGpt.hasNans())
   {
      std::string e = MODULE;
      e += "Error bottomGpt has nans!";
      throw ossimException(e);
   }

#if 0 /* Please leave for debug. (drb) */
   ossimNotify(ossimNotifyLevel_DEBUG)
      << "image size:    " << theImageSize
      << "\nleftDpt:   " << leftDpt
      << "\nrightDpt:  " << rightDpt
      << "\ntopDpt:    " << topDpt
      << "\nbottomDpt: " << bottomDpt      
      << "\nleftGpt:   " << leftGpt
      << "\nrightGpt:  " << rightGpt
      << "\ntopGpt:    " << topGpt
      << "\nbottomGpt: " << bottomGpt      
      << "\n";
#endif
      
   theGSD.x   = leftGpt.distanceTo(rightGpt)/(rightDpt.x-leftDpt.x);
   theGSD.y   = topGpt.distanceTo(bottomGpt)/(bottomDpt.y-topDpt.y);
   theMeanGSD = (theGSD.x + theGSD.y)/2.0;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimSensorModel::computGsd DEBUG:"
         << "\ntheGSD:     " << theGSD
         << "\ntheMeanGSD: " << theMeanGSD << std::endl;
   }
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

ossim_uint32 
ossimSensorModel::degreesOfFreedom()const
{
   ossim_uint32 dof = 0;
   ossim_uint32 idx = 0;
   ossim_uint32 numAdj = getNumberOfAdjustableParameters();
   for(idx = 0; idx < numAdj; ++idx)
   {
      if(!isParameterLocked(idx))
      {
         ++dof;
      }
   }
   
   return dof;
}

double
ossimSensorModel::optimizeFit(const ossimTieGptSet& tieSet, double* /* targetVariance */)
{
   //use a simple Levenberg-Marquardt non-linear optimization
   //note : please limit the number of tie points
   //
   //INPUTS: requires Jacobian matrix (partial derivatives with regards to parameters)
   //OUPUTS: will also compute parameter covariance matrix
   //
   //TBD: use targetVariance!
 
   int np = getNumberOfAdjustableParameters();
   int nobs = tieSet.size();

   //setup initail values
   int iter=0;
   int iter_max = 200;
   double minResidue = 1e-10; //TBC
   double minDelta = 1e-10; //TBC

   //build Least Squares initial normal equation
   // don't waste memory, add samples one at a time
   NEWMAT::SymmetricMatrix A;
   NEWMAT::ColumnVector residue;
   NEWMAT::ColumnVector projResidue;
   double deltap_scale = 1e-4; //step_Scale is 1.0 because we expect parameters to be between -1 and 1
   buildNormalEquation(tieSet, A, residue, projResidue, deltap_scale);
   double ki2=residue.SumSquare();

   //get current adjustment (between -1 and 1 normally) and convert to ColumnVector
   ossimAdjustmentInfo cadj;
   getAdjustment(cadj);
   std::vector< ossimAdjustableParameterInfo >& parmlist = cadj.getParameterList();
   NEWMAT::ColumnVector cparm(np), nparm(np);
   for(int n=0;n<np;++n)
   {
      cparm(n+1) = parmlist[n].getParameter();
   }

   double damping_speed = 2.0;
   //find max diag element for A
   double maxdiag=0.0;
   for(int d=1;d<=np;++d) {
      if (maxdiag < A(d,d)) maxdiag=A(d,d);
   }
   double damping = 1e-3 * maxdiag;
   double olddamping = 0.0;
   bool found = false;

   //DEBUG TBR
   // cout<<"rms="<<sqrt(ki2/nobs)<<" ";
   // cout.flush();

   while ( (!found) && (iter < iter_max) ) //non linear optimization loop
   {
      bool decrease = false;

      do
      {
         //add damping update to normal matrix
         for(int d=1;d<=np;++d) A(d,d) += damping - olddamping;
         olddamping = damping;

         NEWMAT::ColumnVector deltap = solveLeastSquares(A, projResidue);

         if (deltap.NormFrobenius() <= minDelta) 
         {
            found = true;
         } else {
            //update adjustment
            nparm = cparm + deltap;
            for(int n=0;n<np;++n)
            {
               setAdjustableParameter(n, nparm(n+1), false); //do not update now, wait
            }
            updateModel();

            //check residue is reduced
            NEWMAT::ColumnVector newresidue = getResidue(tieSet);
            double newki2=newresidue.SumSquare();
            double res_reduction = (ki2 - newki2) / (deltap.t()*(deltap*damping + projResidue)).AsScalar();
 //DEBUG TBR
       cout<<sqrt(newki2/nobs)<<" ";
       cout.flush();

            if (res_reduction > 0)
            {
               //accept new parms
               cparm = nparm;
               ki2=newki2;

               deltap_scale = max(1e-15, deltap.NormInfinity()*1e-4);

               buildNormalEquation(tieSet, A, residue, projResidue, deltap_scale);
               olddamping = 0.0;

               found = ( projResidue.NormInfinity() <= minResidue );
               //update damping factor
               damping *= std::max( 1.0/3.0, 1.0-std::pow((2.0*res_reduction-1.0),3));
               damping_speed = 2.0;
               decrease = true;
            } else {
               //cancel parameter update
               for(int n=0;n<np;++n)
               {
                  setAdjustableParameter(n, nparm(n+1), false); //do not update right now
               }
               updateModel();

               damping *= damping_speed;
               damping_speed *= 2.0;
            }
         }
      } while (!decrease && !found);
      ++iter;
   }

//DEBUG TBR
cout<<endl;

   //compute parameter correlation
   // use normal matrix inverse
   //TBD

   return ki2/nobs;
}

void
ossimSensorModel::buildNormalEquation(const ossimTieGptSet& tieSet,
                                      NEWMAT::SymmetricMatrix& A,
                                      NEWMAT::ColumnVector& residue,
                                      NEWMAT::ColumnVector& projResidue,
                                      double pstep_scale)
{
   //goal:       build Least Squares system
   //constraint: never store full Jacobian matrix in memory (can be huge)
   //            so we build the matrices incrementally
   // the system can be built using forward() or inverse() depending on the projection capabilities : useForward()
   //
   //TBD : add covariance matrix for each tie point

   //init
   int np = getNumberOfAdjustableParameters();
   int dimObs;
   bool useImageObs = useForward(); //caching
   if (useImageObs)
   {
      dimObs = 2; //image observation
   } else {
      dimObs = 3; //ground observations
   }
   int no = dimObs * tieSet.size(); //number of observations

   A.ReSize(np);
   residue.ReSize(no);
   projResidue.ReSize(np);
   //Zeroify matrices that will be accumulated
   A           = 0.0;
   projResidue = 0.0;

   const vector<ossimRefPtr<ossimTieGpt> >& theTPV = tieSet.getTiePoints();
   vector<ossimRefPtr<ossimTieGpt> >::const_iterator tit;
   unsigned long c=1;

   if (useImageObs)
   { 
     //image observations 
      std::vector<ossimDpt> imDerp(np);
     ossimDpt resIm;
     // loop on tie points
      for (tit = theTPV.begin() ; tit != theTPV.end() ; ++tit)
      {
         //compute residue
         resIm = (*tit)->tie - forward(*(*tit));
         residue(c++) = resIm.x;
         residue(c++) = resIm.y;

         //compute all image derivatives regarding parametres for the tie point position
         for(int p=0;p<np;++p)
         {
            imDerp[p] = getForwardDeriv( p , *(*tit) , pstep_scale);
         }

         //compute influence of tie point on all sytem elements
         for(int p1=0;p1<np;++p1)
         {        
            //proj residue: J * residue
            projResidue.element(p1) += imDerp[p1].x * resIm.x + imDerp[p1].y * resIm.y;

            //normal matrix A = transpose(J)*J
            for(int p2=p1;p2<np;++p2)
            {
               A.element(p1,p2) += imDerp[p1].x * imDerp[p2].x + imDerp[p1].y * imDerp[p2].y;
            }
         }
      }
   }
   else
   {
      // ground observations
      std::vector<ossimGpt>  gdDerp(np);
      ossimGpt gd, resGd;
      // loop on tie points
      for (tit = theTPV.begin() ; tit != theTPV.end() ; ++tit)
      {
         //compute residue
         gd = inverse((*tit)->tie);
         residue(c++) = resGd.lon = ((*tit)->lon - gd.lon) * 100000.0;
         residue(c++) = resGd.lat = ((*tit)->lat - gd.lat) * 100000.0 * cos(gd.lat / 180.0 * M_PI);
         residue(c++) = resGd.hgt = (*tit)->hgt - gd.hgt; //TBD : normalize to meters?

         //compute all image derivatives regarding parametres for the tie point position
         for(int p=0;p<np;++p)
         {
            gdDerp[p] = getInverseDeriv( p , (*tit)->tie, pstep_scale);
         }

         //compute influence of tie point on all sytem elements
         for(int p1=0;p1<np;++p1)
         {        
            //proj residue: J * residue
            projResidue.element(p1) += gdDerp[p1].lon * resGd.lon + gdDerp[p1].lat * resGd.lat + gdDerp[p1].hgt * resGd.hgt; //TBC

            //normal matrix A = transpose(J)*J
            for(int p2=p1;p2<np;++p2)
            {
               A.element(p1,p2) += gdDerp[p1].lon * gdDerp[p2].lon + gdDerp[p1].lat * gdDerp[p2].lat + gdDerp[p1].hgt * gdDerp[p2].hgt;
            }
         }
      }

   } //end of if (useImageObs)
}

//give inverse() partial derivative regarding parameter parmIdx (>=0)
ossimGpt
ossimSensorModel::getInverseDeriv(int parmIdx, const ossimDpt& ipos, double hdelta)
{   
   double den = 0.5/hdelta;
   ossimGpt res,gd;

   double middle = getAdjustableParameter(parmIdx);
   //set parm to high value
   setAdjustableParameter(parmIdx, middle + hdelta, true);
   res = inverse(ipos);
   //set parm to low value and gte difference
   setAdjustableParameter(parmIdx, middle - hdelta, true);
   gd = inverse(ipos);

   //reset parm
   setAdjustableParameter(parmIdx, middle, true);

   res.lon = den*(res.lon - gd.lon) * 100000.0; //TBC : approx meters
   res.lat = den*(res.lat - gd.lat) * 100000.0 * cos(gd.lat / 180.0 * M_PI);
   res.hgt = den*(res.hgt - gd.hgt);

   return res;
}

//give forward() partial derivative regarding parameter parmIdx (>=0)
ossimDpt
ossimSensorModel::getForwardDeriv(int parmIdx, const ossimGpt& gpos, double hdelta)
{   
   static double den = 0.5/hdelta;
   ossimDpt res;

   double middle = getAdjustableParameter(parmIdx);
   //set parm to high value
   setAdjustableParameter(parmIdx, middle + hdelta, true);
   res = inverse(gpos);
   //set parm to low value and gte difference
   setAdjustableParameter(parmIdx, middle - hdelta, true);
   res -= inverse(gpos);
   //get partial derivative
   res = res*den;

   //reset parm
   setAdjustableParameter(parmIdx, middle, true);

   return res;
}

NEWMAT::ColumnVector
ossimSensorModel::getResidue(const ossimTieGptSet& tieSet)
{
   //init
   NEWMAT::ColumnVector residue;
   int dimObs;

   bool useImageObs = useForward(); //caching
   if (useImageObs)
   {
      dimObs = 2; //image observation
   } else {
      dimObs = 3; //ground observations
   }
   int no = dimObs * tieSet.size(); //number of observations

   residue.ReSize(no);

   const vector<ossimRefPtr<ossimTieGpt> >& theTPV = tieSet.getTiePoints();
   vector<ossimRefPtr<ossimTieGpt> >::const_iterator tit;
   unsigned long c=1;

   if (useImageObs)
   { 
     //image observations 
     ossimDpt resIm;
     // loop on tie points
      for (tit = theTPV.begin() ; tit != theTPV.end() ; ++tit)
      {
         //compute residue
         resIm = (*tit)->tie - forward(**tit);
         residue(c++) = resIm.x;
         residue(c++) = resIm.y;
      }
   } else {
      // ground observations
      ossimGpt gd;
      // loop on tie points
      for (tit = theTPV.begin() ; tit != theTPV.end() ; ++tit)
      {
         //compute residue
         gd = inverse((*tit)->tie);
         residue(c++) = ((*tit)->lon - gd.lon) * 100000.0; //approx meters //TBC TBD
         residue(c++) = ((*tit)->lat - gd.lat) * 100000.0 * cos(gd.lat / 180.0 * M_PI);
         residue(c++) = (*tit)->hgt - gd.hgt; //meters
      }
   } //end of if (useImageObs)

   return residue;
}

/*!
 * solves Ax = r , with A symmetric positive definite
 * A can be rank deficient
 * size of A is typically between 10 and 100 rows
 */
NEWMAT::ColumnVector 
ossimSensorModel::solveLeastSquares(NEWMAT::SymmetricMatrix& A,  NEWMAT::ColumnVector& r)const
{
   NEWMAT::ColumnVector x = invert(A)*r;
   return x;
}

/** 
 * stable invert stolen from ossimRpcSolver
 */
NEWMAT::Matrix 
ossimSensorModel::invert(const NEWMAT::Matrix& m)const
{
   ossim_uint32 idx = 0;
   NEWMAT::DiagonalMatrix d;
   NEWMAT::Matrix u;
   NEWMAT::Matrix v;

   // decompose m.t*m which is stored in Temp into the singular values and vectors.
   //
   NEWMAT::SVD(m, d, u, v, true, true);
   
   // invert the diagonal
   // this is just doing the reciprical fo all diagonal components and store back int
   // d.  ths compute d inverse.
   //
   for(idx=0; idx < (ossim_uint32)d.Ncols(); ++idx)
   {
      if(d[idx] > 1e-14) //TBC : use DBL_EPSILON ?
      {
         d[idx] = 1.0/d[idx];
      }
      else
      {
         d[idx] = 0.0;

//DEBUG TBR
cout<<"warning: singular matrix in SVD"<<endl;

      }
   }

   //compute inverse of decomposed m;
   return v*d*u.t();
}
