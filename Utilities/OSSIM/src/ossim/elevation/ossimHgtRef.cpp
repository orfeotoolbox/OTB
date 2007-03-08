//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Hicks
//
// Description:  Height reference class.
//
//----------------------------------------------------------------------------

#include <ossim/elevation/ossimHgtRef.h>
#include <ossim/elevation/ossimElevManager.h>
#include <ossim/base/ossimLsrSpace.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotify.h>


static ossimTrace traceDebug(ossimString("ossimImageTuple:debug"));

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimHgtRef.cpp,v 1.0 2006/10/02  ?????";
#endif



//*****************************************************************************
//  METHOD: ossimHgtRef::ossimHgtRef()
//  
//  Constructor for DEM reference.
//  
//*****************************************************************************
ossimHgtRef::ossimHgtRef(HeightRefType_t cRefType)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimHgtRef::ossimHgtRef DEBUG:" << std::endl;
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  " << OSSIM_ID << std::endl;
#endif 
   }
   
   theCurrentHeightRefType = cRefType;
   
}


//*****************************************************************************
//  METHOD: ossimHgtRef::ossimHgtRef()
//  
//  Constructor for ellipsoid reference.
//  
//*****************************************************************************
ossimHgtRef::ossimHgtRef(HeightRefType_t cRefType, const ossim_float64& atHgt)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimHgtRef::ossimHgtRef DEBUG:" << std::endl;
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  " << OSSIM_ID << std::endl;
#endif 
   }
   
   theCurrentHeightRefType = cRefType;
   theCurrentRefHeight = atHgt;
   
}



ossimHgtRef::~ossimHgtRef()
{
}


//*****************************************************************************
//  METHOD: ossimHgtRef::getRefHeight()
//  
//  Get current reference height based on state of theCurrentHeightRefType.
//  
//*****************************************************************************
ossim_float64 ossimHgtRef::getRefHeight(const ossimGpt& pg) const
{
   ossim_float64 refHgt;
   
   switch (theCurrentHeightRefType)
   {
      case AT_HGT:
         refHgt = theCurrentRefHeight;
         break;
      case AT_DEM:
         refHgt = ossimElevManager::instance()->getHeightAboveEllipsoid(pg);
         break;
      default:
         refHgt = 0.0;
         break;
   }
         
   return refHgt;
}


//*****************************************************************************
//  METHOD: ossimHgtRef::getHeightCovMatrix()
//  
//  Get height reference covariance matrix.
//  
//*****************************************************************************
bool ossimHgtRef::getHeightCovMatrix(const ossimGpt& pg, NEWMAT::Matrix& cov) const
{
   
   ossim_float64 refCE;
   ossim_float64 refLE;
   NEWMAT::Matrix tnU(3,1);
   ossimLsrSpace enu(pg);
   
   switch (theCurrentHeightRefType)
   {
      case AT_HGT:
         // Set the reference CE/LE
         //   Note: currently set to reflect no contribution
         refCE = 0.0;
         refLE = .01;
         
         break;
         
      case AT_DEM:
         // Set the reference CE/LE
         //   Note: currently set to SRTM spec
         //        (ref: www2.jpl.nas.gov/srtm/statistics.html)
         //  "refCE = ossimElevManager::instance()->getAccuracyCE90(pg)" is
         //    the desirable operation here (if it is implemented)
         refCE = 20.0; //meters
         refLE = 16.0;
         
         break;
         
      default:
         return false;
         break;
   }

   // Set surface normal vector in ENU
   ossimColumnVector3d sn = getLocalTerrainNormal(pg);
   tnU << sn[0] << sn[1] << sn[2];
   
   // Form the reference covariance matrix
   NEWMAT::SymmetricMatrix refCov(3);
   refCov = 0.0;
   refCov(1,1) = refCE/2.146;
   refCov(2,2) = refCE/2.146;
   refCov(3,3) = refLE/1.6449;

   // Propagate to ECF
   NEWMAT::Matrix covEcf;
   covEcf = enu.lsrToEcefRotMatrix() * refCov * enu.lsrToEcefRotMatrix().t();

   // Propagate to terrain normal
   NEWMAT::Matrix ptn;
   NEWMAT::SymmetricMatrix ptns;
   ptn = tnU.t() * covEcf * tnU;
   ptns << ptn;

   cov = tnU * ptns * tnU.t();


   return true;
}


//*****************************************************************************
//  METHOD: ossimHgtRef::getLocalTerrainNormal()
//  
//  Get get local terrain normal unit vector.
//  
//*****************************************************************************
ossimColumnVector3d ossimHgtRef::getLocalTerrainNormal(const ossimGpt& pg) const
{
   ossimColumnVector3d tNorm;
   const ossim_float64 delta = 100.0;
   
   switch (theCurrentHeightRefType)
   {
      case AT_HGT:
         // Use ellipsoid tangent plane mormal
         tNorm = tNorm.zAligned();
         break;
         
      case AT_DEM:
         {
            // Use local 3X3 grid around point to get mean slope
            NEWMAT::Matrix h(3,3);
            ossimDpt mpd;
            mpd = pg.metersPerDegree();
            ossim_float64 dLon = delta/mpd.x;
            ossim_float64 dLat = delta/mpd.y;
            for (ossim_int32 lon=-1; lon<=1; ++lon)
            {
               ossim_float64 clon = pg.lond()+lon*dLon;
               for (ossim_int32 lat=-1; lat<=1; ++lat)
               {
                  ossim_float64 clat = pg.latd()+lat*dLat;
                  ossimGpt p(clat, clon, pg.height());
                  h(lat+2,lon+2) =
                     ossimElevManager::instance()->getHeightAboveEllipsoid(p);
               }
            }
            ossim_float64 dz_dlon =
                  ((h(1,3)+2*h(2,3)+h(3,3))-(h(1,1)+2*h(2,1)+h(3,1)))/(8*delta);
            ossim_float64 dz_dlat =
                  ((h(1,1)+2*h(1,2)+h(1,3))-(h(3,1)+2*h(3,2)+h(3,3)))/(8*delta);
            tNorm[0] = dz_dlon;
            tNorm[1] = dz_dlat;
            tNorm[2] = 1.0 - sqrt(dz_dlon*dz_dlon+dz_dlat*dz_dlat);
         }
         break;
         
      default:
         tNorm = tNorm.zAligned();
         break;
   }

   return tNorm;
}
