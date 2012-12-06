//*****************************************************************************
// FILE: ossimElevSource.cc
//
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//   Contains implementation of class ossimElevSource. This is the base class
//   for all sources of elevation data. it maintains a single common static
//   instance of the geoid for use by all instances of objects derived from
//   this one. 
//
// SOFTWARE HISTORY:
//>
//   18Apr2001  Oscar Kramer
//              Initial coding.
//<
//*****************************************************************************
// $Id: ossimElevSource.cpp 21380 2012-07-25 13:18:31Z gpotts $

#include <ossim/elevation/ossimElevSource.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimEcefRay.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimNotifyContext.h>

RTTI_DEF1(ossimElevSource, "ossimElevSource" , ossimSource)

//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>
static ossimTrace traceExec  ("ossimElevSource:exec");
static ossimTrace traceDebug ("ossimElevSource:debug");


static const char ENABLE_STATS_KW[] = "elevation.compute_statistics.enabled";

ossimElevSource::ossimElevSource()
   :
      theMinHeightAboveMSL (0.0),
      theMaxHeightAboveMSL (0.0),
      theNullHeightValue   (ossim::nan()),
      theSeaLevelValue     (OSSIM_DEFAULT_MEAN_SEA_LEVEL),
      theGroundRect(),
      theComputeStatsFlag(false)
   
{
   if (traceExec())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimElevSource::ossimElevSource: entering..."
         << std::endl;
   }
   
   // User can turn on off with a keyword.
   const char* lookup =
      ossimPreferences::instance()->findPreference(ENABLE_STATS_KW);
   if (lookup)
   {
     theComputeStatsFlag = ossimString(lookup).toBool();
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG:"
         << "\ntheComputeStatsFlag:  "
         << (theComputeStatsFlag?"true":"false")
         << endl;
   }


   if (traceExec())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimElevSource::ossimElevSource: returning..."
         << std::endl;
   }
}

ossimElevSource::ossimElevSource(const ossimElevSource& src)
   :ossimSource(src),
    theMinHeightAboveMSL(src.theMinHeightAboveMSL),
    theMaxHeightAboveMSL(src.theMaxHeightAboveMSL),
    theNullHeightValue(src.theNullHeightValue),
    theSeaLevelValue(src.theSeaLevelValue),
    theGroundRect(src.theGroundRect),
    theComputeStatsFlag(src.theComputeStatsFlag)
{
}

ossimElevSource::~ossimElevSource()
{}

const ossimElevSource& ossimElevSource::operator = (const ossimElevSource& src)
{
   if (this != &src)
   {
      theMinHeightAboveMSL = src.theMinHeightAboveMSL;
      theMaxHeightAboveMSL = src.theMaxHeightAboveMSL;
      theNullHeightValue   = src.theNullHeightValue;
      theSeaLevelValue     = src.theSeaLevelValue;
      theGroundRect        = src.theGroundRect;
      theComputeStatsFlag  = src.theComputeStatsFlag;
      
      ossimSource::operator=(src);
   }
   return *this;
}

double ossimElevSource::getHeightAboveEllipsoid(const ossimGpt& /* argGpt */)
{
   ossimNotify(ossimNotifyLevel_FATAL)
      << "FATAL ossimElevSource::getHeightAboveEllipsoid Not implemented..."
      << std::endl;
   return theNullHeightValue;
}

//*****************************************************************************
//  METHOD: intersectRay()
//  
//  Service method for intersecting a ray with the elevation surface to
//  arrive at a ground point. The ray is expected to originate ABOVE the
//  surface and pointing down.
//
//  NOTE: the gpt argument is expected to be initialized with the desired
//  datum, including ellipsoid, for the proper intersection point to be
//  computed.
//
//  LIMITATION: This release supports only single valued solutions, i.e., it
//  is possible a ray passing through one side of a mountain and out the other
//  will return an intersection with the far side. Eventually, a more robust
//  algorithm will be employed.
//
//*****************************************************************************
bool ossimElevSource::intersectRay(const ossimEcefRay& ray, ossimGpt& gpt, double defaultElevValue)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimElevSource::intersectRay: entering..." << std::endl;

   static const double CONVERGENCE_THRESHOLD = 0.001; // meters
   static const int    MAX_NUM_ITERATIONS    = 50;
   
   double          h_ellips; // height above ellipsoid
   bool            intersected;
   ossimEcefPoint  prev_intersect_pt (ray.origin());
   ossimEcefPoint  new_intersect_pt;
   double          distance;
   bool            done = false;
   int             iteration_count = 0;

   if(ray.hasNans()) 
   {
      gpt.makeNan();
      return false;
   }
   //***
   // Set the initial guess for horizontal intersect position as the ray's
   // origin, and establish the datum and ellipsoid:
   //***
   const ossimDatum*     datum     = gpt.datum();
   const ossimEllipsoid* ellipsoid = datum->ellipsoid();
//    double lat, lon, h;

//    ellipsoid->XYZToLatLonHeight(ray.origin().x(),
//                                 ray.origin().y(),
//                                 ray.origin().z(),
//                                 lat, lon, h);
//    ossimGpt nadirGpt(lat, lon, h);

//    std::cout << "nadir pt = " << nadirGpt << std::endl;
   
   gpt = ossimGpt(prev_intersect_pt, datum);

   //
   // Loop to iterate on ray intersection with variable elevation surface:
   //
   do
   {
      //
      // Intersect ray with ellipsoid inflated by h_ellips:
      //
      h_ellips = getHeightAboveEllipsoid(gpt);
      if ( ossim::isnan(h_ellips) ) h_ellips = defaultElevValue;
      
      intersected = ellipsoid->nearestIntersection(ray,
                                                   h_ellips,
                                                   new_intersect_pt);
      if (!intersected)
      {
         //
         // No intersection (looking over the horizon), so set ground point
         // to NaNs:
         //
         gpt.makeNan();
         done = true;
      }
      else
      {
         //
         // Assign the ground point to the latest iteration's intersection
         // point:
         //
         gpt = ossimGpt(new_intersect_pt, datum);
         
         //
         // Determine if convergence achieved:
         //
         distance = (new_intersect_pt - prev_intersect_pt).magnitude();
         if (distance < CONVERGENCE_THRESHOLD)
            done = true;
         else
         {
            prev_intersect_pt = new_intersect_pt;
         }
      }

      iteration_count++;

   } while ((!done) && (iteration_count < MAX_NUM_ITERATIONS));

   if (iteration_count == MAX_NUM_ITERATIONS)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimElevSource::intersectRay: Max number of iterations reached solving for ground "
                                            << "point. Result is probably inaccurate." << std::endl;
      }
   }
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimElevSource::intersectRay: returning..." << std::endl;
   return intersected;
}

double ossimElevSource::getMinHeightAboveMSL() const
{
   return theMinHeightAboveMSL;
}

double ossimElevSource::getMaxHeightAboveMSL() const
{
   return theMaxHeightAboveMSL;
}

double ossimElevSource::getNullHeightValue() const
{
   return theNullHeightValue;
}

double ossimElevSource::getSeaLevelValue() const
{
   return theSeaLevelValue;
}

const ossimGrect& ossimElevSource::getBoundingGndRect() const
{
   return theGroundRect;
}

bool ossimElevSource::canConnectMyInputTo(ossim_int32 /* inputIndex */,
                                          const ossimConnectableObject* /* object */)const
{
   return false;
}

void ossimElevSource::initialize()
{}


bool ossimElevSource::getComputeStatisticsFlag() const
{
   return theComputeStatsFlag;
}

void ossimElevSource::setComputeStatisticsFlag(bool flag)
{
   theComputeStatsFlag = flag;
}
