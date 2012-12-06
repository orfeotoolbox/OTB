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
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/elevation/ossimElevationAccuracyInfo.h>


static ossimTrace traceDebug(ossimString("ossimHgtRef:debug"));

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimHgtRef.cpp 21399 2012-07-27 18:19:22Z gpotts $";
#endif



//*****************************************************************************
//  METHOD: ossimHgtRef::ossimHgtRef()
//  
//  Constructor for DEM reference.
//  
//*****************************************************************************
ossimHgtRef::ossimHgtRef(HeightRefType_t cRefType)
:
theCurrentHeightRefType(cRefType)
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
   
}


//*****************************************************************************
//  METHOD: ossimHgtRef::ossimHgtRef()
//  
//  Constructor for ellipsoid reference.
//  
//*****************************************************************************
ossimHgtRef::ossimHgtRef(HeightRefType_t cRefType, const ossim_float64& atHgt)
:
theCurrentHeightRefType(cRefType),
theCurrentRefHeight(atHgt)
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
//  METHOD: ossimHgtRef::getSurfaceInfo()
//  
//  Get reference surface information.
//  
//*****************************************************************************

#if 0
bool ossimHgtRef::getSurfaceInfo(const ossimGpt& pg,
                                       ossimElevationAccuracyInfo& info) const
{
   bool infoOK = false;

  // ossimElevManager::ElevationDatabaseListType dbList;  
  // std::vector<ossimFilename> cellList;


   //double hgt =  ossimElevManager::instance()->getHeightAboveEllipsoid(pg);


/*

   dbList = ossimElevManager::instance()->getElevationDatabaseList();
   ossim_uint32 idx;

   for(idx=0; idx < dbList.size(); ++idx)
   {
      double h = dbList[idx]->getHeightAboveEllipsoid(pg);
      if (!ossim::isnan(h))
      {
         hgt = h;
         break;
      }
   }
  */ 
   /*
   if (!ossim::isnan(hgt))
   {
      ossimElevManager::instance()->getOpenCellList(cellList);

      ossimRefPtr<ossimImageHandler> ih =
            ossimImageHandlerRegistry::instance()->open(cellList[idx]);
      ossimString hanType(ih->getClassName().c_str());

      ossimImageGeometry* geom = ih->getImageGeometry().get();
      gsd = geom->getMetersPerPixel();

      info = hanType;
   }
   else
   {
      info = "No Surface";
   }
   */
   infoOK = true;

   return infoOK;
}
#endif

//*****************************************************************************
//  METHOD: ossimHgtRef::getSurfaceCovMatrix()
//  
//  Get reference surface covariance matrix.
//  
//*****************************************************************************
bool ossimHgtRef::getSurfaceCovMatrix(const ossimGpt& pg, NEWMAT::Matrix& cov) const
{
   
   ossim_float64 refCE;
   ossim_float64 refLE;
   ossimString info;
   ossimDpt gsd;

   switch (theCurrentHeightRefType)
   {
      case AT_HGT:
         // Set the reference CE/LE
         //   Note: currently set to reflect no contribution
         refCE = 0.0;
         refLE = .01;
         
         break;
         
      case AT_DEM:
      {

         // Set the reference CE/LE
         //   Note: currently set to SRTM spec in METERS
         //    refCE = 20.0;
         //    refLE = 16.0;
         //        (ref: www2.jpl.nas.gov/srtm/statistics.html)
         //  "refCE = ossimElevManager::instance()->getAccuracyCE90(pg)" is
         //    the desirable operation here (if it is implemented)
         // ================================================
         //  This is one step closer to automatic
         //  access to elevation surface accuracy
         //   TODO...
         //     [1] load from OSSIM_PREFERENCES?
         //     [2] does DTED header/metadata have info?
         // ================================================
         ossimElevationAccuracyInfo info;
         ossimElevManager::instance()->getAccuracyInfo(info, pg);

         if(info.hasValidAbsoluteError())
         {
            refCE = info.m_absoluteCE;
            refLE = info.m_absoluteLE;
         }
         else
         {
            refCE = 20.0;
            refLE = 16.0;
         }
#if 0
         if (getSurfaceInfo(pg, info, gsd))
         {
            if (info.contains("Srtm"))
            {
               if (gsd.x < 50.0)
               {
                  // SRTM 1 arc
                  refCE = 20.0;
                  refLE = 10.0;
               }
               else
               {
                  // SRTM 3 arc
                  refCE = 20.0;
                  refLE = 16.0;
               }
            }
            else if (info.contains("Dted"))
            {
               if (gsd.x < 50.0)
               {
                  // DTED level 2
                  refCE = 40.0;
                  refLE = 20.0;
               }
               else
               {
                  // DTED level 1
                  refCE = 50.0;
                  refLE = 30.0;
               }
            }
            else
            {
               // Other
               refCE = 20.0;
               refLE = 16.0;
            }
         }
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               //<< "DEBUG: info: " << info
               << " ref: " << refCE << "/" << refLE << endl;
         }
#endif

         break;
      }
      default:
         return false;
         break;
   }
   
   // Form the covariance matrix
   //  A circular error ellipse with no correlation must be assumed.
   cov = 0.0;
   cov(1,1) = refCE/2.146;
   cov(2,2) = refCE/2.146;
   cov(3,3) = refLE/1.6449;
   cov(1,1) *= cov(1,1);
   cov(2,2) *= cov(2,2);
   cov(3,3) *= cov(3,3);
   
   return true;
}


//*****************************************************************************
///  METHOD: ossimHgtRef::getSurfaceCovMatrix()
//  
//  Get reference surface covariance matrix.
// 
//*****************************************************************************
bool ossimHgtRef::getSurfaceCovMatrix(const ossim_float64   refCE, 
                                      const ossim_float64   refLE,
                                      NEWMAT::Matrix& cov) const
{
   ossim_float64 useCE;
   ossim_float64 useLE;
   
   if (refCE<0.0 || refLE<0.0)
   {
      return false;
   }
   
   switch (theCurrentHeightRefType)
   {
      case AT_HGT:
         // Set the reference CE/LE
         //   Note: currently set to reflect no contribution
         useCE = 0.0;
         useLE = .01;
         
         break;
         
      case AT_DEM:
         // Set the reference CE/LE
         useCE = refCE;
         useLE = refLE;
         
         break;
         
      default:
         return false;
         break;
   }
   
   // Form the covariance matrix
   //  A circular error ellipse with no correlation must be assumed.
   cov = 0.0;
   cov(1,1) = useCE/2.146;
   cov(2,2) = useCE/2.146;
   cov(3,3) = useLE/1.6449;
   cov(1,1) *= cov(1,1);
   cov(2,2) *= cov(2,2);
   cov(3,3) *= cov(3,3);
   
   return true;
}


//*****************************************************************************
//  METHOD: ossimHgtRef::getSurfaceNormalCovMatrix()
//  
//  Form surface normal ECF covariance matrix from ENU surface covariance.
//  
//*****************************************************************************
bool ossimHgtRef::
getSurfaceNormalCovMatrix
   (const ossimGpt&       pg, 
    const NEWMAT::Matrix& surfCov, 
          NEWMAT::Matrix& normCov) const
{
   // Set the local frame
   ossimLsrSpace enu(pg);

   // Propagate the reference covariance matrix to ECF
   NEWMAT::Matrix covEcf;
   covEcf = enu.lsrToEcefRotMatrix() * surfCov * enu.lsrToEcefRotMatrix().t();

   // Get surface normal vector in ENU
   ossimColumnVector3d sn = getLocalTerrainNormal(pg);
   NEWMAT::Matrix tnU(3,1);
   tnU << sn[0] << sn[1] << sn[2];

   // Rotate surface normal to ECF
   NEWMAT::Matrix tnUecf(3,1);
   tnUecf = enu.lsrToEcefRotMatrix() * tnU;

   // Propagate to terrain normal
   NEWMAT::Matrix ptn;
   NEWMAT::SymmetricMatrix ptns;
   ptn = tnUecf.t() * covEcf * tnUecf;
   ptns << ptn;

   // And back to ECF
   normCov = tnUecf * ptns * tnUecf.t();


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
                  h(2-lat,lon+2) =
                     ossimElevManager::instance()->getHeightAboveEllipsoid(p);
               }
            }
            
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  <<"DEBUG: getLocalTerrainNormal...  3X3 grid"<<endl;
               for (ossim_int32 lat=-1; lat<=1; ++lat)
               {
                  for (ossim_int32 lon=-1; lon<=1; ++lon)
                    ossimNotify(ossimNotifyLevel_DEBUG)<<"  "<<h(lat+2,lon+2);
                  ossimNotify(ossimNotifyLevel_DEBUG)<<endl;
               }
            }

            ossim_float64 dz_dlon =
                  ((h(1,3)+2*h(2,3)+h(3,3))-(h(1,1)+2*h(2,1)+h(3,1)))/(8*delta);
            ossim_float64 dz_dlat =
                  ((h(1,1)+2*h(1,2)+h(1,3))-(h(3,1)+2*h(3,2)+h(3,3)))/(8*delta);
            tNorm[0] = -dz_dlon;
            tNorm[1] = -dz_dlat;
            tNorm[2] = 1.0 - sqrt(dz_dlon*dz_dlon+dz_dlat*dz_dlat);
         }
         
         // If error condition, return z-aligned vector to allow continuation
         if (ossim::isnan(tNorm[0]) ||
             ossim::isnan(tNorm[1]) ||
             ossim::isnan(tNorm[2]))
         {
            tNorm = tNorm.zAligned();
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "WARNING: ossimHgtRef::getLocalTerrainNormal(): "
                  << "\n   error... terrain normal set to vertical..."
                  << std::endl;
               
            }
         }
         break;
         
      default:
         tNorm = tNorm.zAligned();
         break;
   }

   return tNorm;
}
