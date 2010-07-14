//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Base class for all projections.  
//*******************************************************************
//  $Id: ossimProjection.cpp 17206 2010-04-25 23:20:40Z dburken $

#include <iostream>
#include <iomanip>

#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/elevation/ossimElevManager.h>
#include <ossim/base/ossimNotifyContext.h>
//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>
static ossimTrace traceExec  ("ossimProjection:exec");
static ossimTrace traceDebug ("ossimProjection:debug");

using namespace std;

RTTI_DEF1(ossimProjection, "ossimProjection", ossimObject);

//*******************************************************************
// Public constructor:
//*******************************************************************
ossimProjection::ossimProjection()
{
}

//*****************************************************************************
//  METHOD: ossimProjection::saveState()
//*****************************************************************************
bool ossimProjection::saveState(ossimKeywordlist& kwl,
                           const char* prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           TYPE_NAME(this),
           true);

   return true;
}

//*****************************************************************************
//  METHOD: ossimProjection::loadState()
//*****************************************************************************
bool ossimProjection::loadState(const ossimKeywordlist& /* kwl */,
                                const char*             /* prefix */)
{
   return true;
}

//*****************************************************************************
//  METHOD: ossimProjection::worldToLineSample()
//  
//  Performs forward projection of ground point to image space via iterative
//  calls to lineSampleHeightToWorld
//  
//*****************************************************************************
void ossimProjection::worldToLineSample(const ossimGpt& worldPoint,
                                        ossimDpt&       ip) const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimProjection::worldToLineSample: entering..." << std::endl;

   static const double PIXEL_THRESHOLD    = 0.1; // acceptable pixel error
   static const int    MAX_NUM_ITERATIONS = 20;

   //***
   // First check if the world point is inside bounding rectangle:
   //***
   int iters = 0;
   double height = worldPoint.hgt;
   if ( ossim::isnan(height) )
   {
      height = 0.0;
   }

   //***
   // Utilize iterative scheme for arriving at image point. Begin with guess
   // at image center:
   //***
   ip.u = 0;
   ip.v = 0;
   
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

   //***
   // Note that this error mesage appears only if max count was reached while
   // iterating. A linear (no iteration) solution would finish with iters =
   // MAX_NUM_ITERATIONS + 1:
   //***
   if (iters == MAX_NUM_ITERATIONS)
   {
//       ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimProjection::worldToLineSample: Exceeded max number of iterations computing image \n"
//            << "point for ground point: " << worldPoint
//            << "\nCheck the geometry file for valid quantities." << endl;
   }
   
}

void ossimProjection::getRoundTripError(const ossimDpt& imagePoint,
                                        ossimDpt& errorResult)const
{
   ossimGpt world;
   ossimDpt testPt;
   
   lineSampleToWorld(imagePoint, world);
   worldToLineSample(world, testPt);

   errorResult = imagePoint - testPt;
}

void ossimProjection::getRoundTripError(const ossimGpt& groundPoint,
                                        ossimDpt& errorResult)const
{
   ossimDpt tempPt;
   ossimGpt tempGround;

   worldToLineSample(groundPoint, tempPt);
   lineSampleToWorld(tempPt, tempGround);
   
   errorResult = ossimDpt(groundPoint) - ossimDpt(tempGround);
}

void ossimProjection::getGroundClipPoints(ossimGeoPolygon& /* gpts */)const
{
}

std::ostream& ossimProjection::print(std::ostream& out) const
{
   return ossimErrorStatusInterface::print(out);
}
