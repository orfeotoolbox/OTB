//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Utility class to encapsulate parsing TerraSAR-X product.xml
// file.
// 
//----------------------------------------------------------------------------
// $Id$

#include <ossimTerraSarProductDoc.h>
#include <ossimPluginCommon.h>
#include <otb/CivilDateTime.h>
#include <otb/Ephemeris.h>
#include <otb/GeographicEphemeris.h>
#include <otb/JSDDateTime.h>
#include <otb/PlatformPosition.h>
#include <otb/SarSensor.h>
#include <otb/SensorParams.h>
#include <otb/RefPoint.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimXmlNode.h>

// Static trace for debugging
static ossimTrace traceDebug("ossimTerraSarProductDoc:debug");


ossimplugins::ossimTerraSarProductDoc::ossimTerraSarProductDoc()
{
}

ossimplugins::ossimTerraSarProductDoc::~ossimTerraSarProductDoc()
{
}

bool ossimplugins::ossimTerraSarProductDoc::isTerraSarX(
   const ossimXmlDocument* xdoc) const
{
   bool result = false;
   if (xdoc)
   {
      ossimString s;
      getMission(xdoc, s);
      if (s.upcase() == "TSX-1")
      {
         result = true;
      }
   }
   return result;
}

bool ossimplugins::ossimTerraSarProductDoc::initPlatformPosition(
   const ossimXmlDocument* xdoc, ossimplugins::PlatformPosition* pos) const
{
   static const char MODULE[] =
      "ossimplugins::ossimTerraSarProductDoc::initPlatformPosition";
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
   }  

   bool result = true;
   
   if ( xdoc && pos )
   {
      // Get all the stateVector nodes.
      ossimString path =
         "/level1Product/platform/orbit/stateVec";
      std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
      xdoc->findNodes(path, xnodes);
      if ( xnodes.size() )
      {
         const std::vector<ossimRefPtr<ossimXmlNode> >::size_type COUNT =
            xnodes.size();
         
         std::vector<Ephemeris*> ev;
         
         int nbrData = 0; // to keep track of good stateVector count.

         ossimRefPtr<ossimXmlNode> svNode = 0; // stateVector node

         for (ossim_uint32 i = 0 ; i < COUNT; ++i)
         {
            svNode = xnodes[i];
            if ( !svNode )
            {
               result = false;
               break;
            }

            ossimString s;

            // TSX ephemerisis quality indicator (1:good, 0: wrong)
            svNode->getAttributeValue(s, "qualInd");

            if (s == "1")
            {
               double pos[3];
               double vit[3];
               CivilDateTime eph_civil_date;
               
               path = "timeUTC";
               result = ossim::findFirstNode(path, svNode, s);
               if (result)
               {
                  ossim::iso8601TimeStringToCivilDate(s, eph_civil_date);
               }
               else
               {
                  result = false;
                  ossimNotify(ossimNotifyLevel_WARN)
                     << MODULE << " ERROR:\nNode not found: " << path
                     << std::endl;
                  break;
               }

               path = "posX";
               result = ossim::findFirstNode(path, svNode, s);
               if (result)
               {
                  pos[0] = s.toDouble();
               }
               else
               {
                  result = false;
                  ossimNotify(ossimNotifyLevel_WARN)
                     << MODULE << " ERROR:\nNode not found: " << path
                     << std::endl;
                  break;
               }

               path = "posY";
               result = ossim::findFirstNode(path, svNode, s);
               if (result)
               {
                  pos[1] = s.toDouble();
               }
               else
               {
                  result = false;
                  ossimNotify(ossimNotifyLevel_WARN)
                     << MODULE << " ERROR:\nNode not found: " << path
                     << std::endl;
                  break;
               }

               path = "posZ";
               result = ossim::findFirstNode(path, svNode, s);
               if (result)
               {
                  pos[2] = s.toDouble();
               }
               else
               {
                  result = false;
                  ossimNotify(ossimNotifyLevel_WARN)
                     << MODULE << " ERROR:\nNode not found: " << path
                     << std::endl;
                  break;
               }
               
               path = "velX";
               result = ossim::findFirstNode(path, svNode, s);
               if (result)
               {
                  vit[0] = s.toDouble();
               }
               else
               {
                  result = false;
                  ossimNotify(ossimNotifyLevel_WARN)
                     << MODULE << " ERROR:\nNode not found: " << path
                     << std::endl;
                  break;
               }
               
               path = "velY";
               result = ossim::findFirstNode(path, svNode, s);
               if (result)
               {
                  vit[1] = s.toDouble();
               }
               else
               {
                  result = false;
                  ossimNotify(ossimNotifyLevel_WARN)
                     << MODULE << " ERROR:\nNode not found: " << path
                     << std::endl;
                  break;
               }
               
               path = "velZ";
               result = ossim::findFirstNode(path, svNode, s);
               if (result)
               {
                  vit[2] = s.toDouble();
               }
               else
               {
                  result = false;
                  ossimNotify(ossimNotifyLevel_WARN)
                     << MODULE << " ERROR:\nNode not found: " << path
                     << std::endl;
                  break;
               }
               
               JSDDateTime eph_jsd_date(eph_civil_date);
               GeographicEphemeris* eph =
                  new GeographicEphemeris(eph_jsd_date, pos, vit);
               ev.push_back(eph);
               ++nbrData;
            }  // matches: if (s == "1")
               
   } // matches: for (ossim_uint32 i = 0 ; i < COUNT; ++i)

         if (result && ev.size())
         {
            Ephemeris** ephemeris = new Ephemeris*[ev.size()];
            for ( std::vector<Ephemeris*>::size_type evIdx = 0;
                  evIdx < ev.size();
                  ++evIdx )
            {
               ephemeris[evIdx] = ev[evIdx];
            }
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << " DEBUG\nnbrData:  " << nbrData << "\n";
            } 
            pos->setData(ephemeris, nbrData);
         }
         else
         {
            std::vector<Ephemeris*>::iterator evIdx = ev.begin();
            while (evIdx != ev.end())
            {
               delete (*evIdx);
               (*evIdx) = 0;
            }
            ev.clear();
         }
         
      } // matches: if ( xnodes.size() )
      else
      {
         result = false;
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " ERROR:\nNodes not found: " << path << std::endl;
      }
      
   } // matches: if (xdoc && pos)
   else
   {
      result = false;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true\n":"false\n");
   } 

   return result;
}

bool ossimplugins::ossimTerraSarProductDoc::initSensorParams(
   const ossimXmlDocument* xdoc, SensorParams* sp) const
{
   bool result = true;

   if (xdoc && sp)
   {
      ossimString s;

      // Get the number of azimuth looks.
      if ( getNumberOfAzimuthLooks(xdoc, s) )
      {
         sp->set_nAzimuthLook(s.toDouble());
      }
      else
      {
         result = false;
      }

      // Get the number of range looks.
      if ( getNumberOfRangeLooks(xdoc, s) )
      {
         sp->set_nRangeLook(s.toDouble());
      }
      else
      {
         result = false;
      }

      // Get the nominal PRF
      if ( getCommonPrf(xdoc, s) )
      {
         sp->set_prf(s.toDouble());
      }
      else
      {
         result = false;
      }

      // Get the radar wave length.
      if ( getRadarCenterFrequency(xdoc, s) )
      {
         const double CLUM = 2.99792458e+8 ;
         double waveLength = CLUM / s.toDouble();
         sp->set_rwl(waveLength);
      }
      else
      {
         result = false;
      }

      if ( getLookDirection(xdoc, s) )
      {
         if (s.upcase() == "RIGHT")
         {
            sp->set_sightDirection(SensorParams::Right);
         }
         else
         {
            sp->set_sightDirection(SensorParams::Left);
         }
      }
      else
      {
         result = false;
      }

      // Get the Sampling frequency.
      if ( getCommonRsf(xdoc, s) )
      {
         sp->set_sf(s.toDouble());
      }
      else
      {
         result = false;
      }

      // Ellipsoid hard coded to WGS84.
      const double SEMI_MAJOR = 6378137.0;
      const double SEMI_MINOR = 6356752.3142;
      sp->set_semiMajorAxis(SEMI_MAJOR);
      sp->set_semiMinorAxis(SEMI_MINOR);
      
      if ( isProductGeoreferenced(xdoc) )
      {
         if ( getOrbitDirection(xdoc, s) )
         {
            int orbitDirectionSign;
            if ( s.upcase() == "DESCENDING" )
            {
               orbitDirectionSign = 1 ;
            }
            else
            {
               orbitDirectionSign = -1 ;
            }

            if ( getImageDataStrartWith(xdoc, s) )
            {
               if (s=="EARLYAZNEARRG")
               {
                  sp->set_col_direction(orbitDirectionSign); 
                  sp->set_lin_direction(orbitDirectionSign);
               }
               else if (s=="EARLYAZFARRG")
               {
                  sp->set_col_direction(-orbitDirectionSign);
                  sp->set_lin_direction(orbitDirectionSign);
               }
               else if (s=="LATEAZNEARRG")
               {
                  sp->set_col_direction(orbitDirectionSign);
                  sp->set_lin_direction(-orbitDirectionSign);
               }
               else if (s=="LATEAZFARRG") {
                  sp->set_col_direction(-orbitDirectionSign);
                  sp->set_lin_direction(-orbitDirectionSign);
               }
               else
               {
                  //---
                  // COSAR Files are stored starting with early azimuth,
                  // near range
                  //---
                  sp->set_col_direction(orbitDirectionSign);
                  sp->set_lin_direction(orbitDirectionSign);
               }
            }
            else
            {
               result = false;
            }
         }
         else
         {
            result = false;
         }
         
      } // matches: if ( isProductGeoreferenced(xdoc) )
      else
      {
         sp->set_col_direction(1);
         sp->set_lin_direction(1);
      }
      
   } // matches: if (xdoc && sp)
   else
   {
      result = false;
   }
   
   return result;  
}

bool ossimplugins::ossimTerraSarProductDoc::initImageSize(
   const ossimXmlDocument* xdoc, ossimIpt& imageSize) const
{
   bool result = true;

   if (xdoc)
   {
      ossimString s;
      if ( getNumberOfColumns(xdoc, s) )
      {
         imageSize.x = static_cast<ossim_int32>(s.toFloat64());
      }
      else
      {
         result = false;
      }
      if ( getNumberOfRows(xdoc, s) )
      {
         imageSize.y = static_cast<ossim_int32>(s.toFloat64());
      }
      else
      {
         result = false;
      }
   }
   else
   {
      result = false;
   }
      
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimplugins::ossimTerraSarProductDoc::initImageSize DEBUG:\nimage size: "
         << imageSize
         << "\nexit status = " << (result?"true":"false")
         << std::endl;
   }
   
   return result;
}

bool ossimplugins::ossimTerraSarProductDoc::initGsd(
   const ossimXmlDocument* xdoc, ossimDpt& gsd) const
{
   bool result = true;

   if (xdoc)
   {
      ossimString s;
      if ( getColumnSpacing(xdoc, s) )
      {
         gsd.x = s.toFloat64();
      }
      else
      {
         result = false;
      }
      if ( getRowSpacing(xdoc, s) )
      {
         gsd.y = s.toFloat64(s);
      }
      else
      {
         result = false;
      }
   }
   else
   {
      result = false;
   }
      
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimplugins::ossimTerraSarProductDoc::initGsd DEBUG:\ngsd: "
         << gsd
         << "\nexit status = " << (result?"true":"false")
         << std::endl;
   }
   
   return result;
}

bool ossimplugins::ossimTerraSarProductDoc::initTiePoints(
   const ossimXmlDocument* xdoc,
   std::list<ossimGpt>& gcp,
   std::list<ossimDpt>& icp) const
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarProductDoc::initTiePoints";
   
   bool result = true;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG:\n";
   }
         
   if (xdoc)
   {
      ossimString s;
      ossimDpt dpt;
      ossimGpt gpt;

      // Get the height.
      result = getSceneAverageHeight(xdoc, s);
      if (result)
      {
         gpt.hgt = s.toFloat64();
      }
      
      // Get the scene center.
      result = getSceneCenterRefColumn(xdoc, s);
      if (result)
      {
         dpt.x = s.toFloat64() - 1.0;
      }
      result = getSceneCenterRefRow(xdoc, s);
      if (result)
      {
         dpt.y = s.toFloat64() - 1.0;
      }
      icp.push_back(dpt);

      result = getSceneCenterLat(xdoc, s);
      if (result)
      {
         gpt.lat = s.toFloat64();
      }
      result = getSceneCenterLon(xdoc, s);
      if (result)
      {
         gpt.lon = s.toFloat64();
      }
      gcp.push_back(gpt);
      
      ossimString path =
         "/level1Product/productInfo/sceneInfo/sceneCornerCoord";
      std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
      xdoc->findNodes(path, xnodes);
      if ( xnodes.size() )
      {
         for (ossim_uint32 i = 0; i < xnodes.size(); ++i)
         {
            if (xnodes[i].valid())
            {
               result = ossim::findFirstNode(ossimString("refRow"),
                                             xnodes[i], s);
               if (result)
               {
                  dpt.y = s.toDouble() - 1.0;
               }
               result = ossim::findFirstNode(ossimString("refColumn"),
                                             xnodes[i], s);
               if (result)
               {
                  dpt.x = s.toDouble() - 1.0;
               }
               icp.push_back(dpt);

               if (traceDebug())
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << "dpt" << i << ": " << dpt
                     << "\n";
               }

               result = ossim::findFirstNode(ossimString("lat"),
                                             xnodes[i], s);
               if (result)
               {
                  gpt.lat = s.toFloat64();
               }
               result = ossim::findFirstNode(ossimString("lon"),
                                             xnodes[i], s);
               if (result)
               {
                  gpt.lon = s.toDouble();
               }
               gcp.push_back(gpt);
               
               if (traceDebug())
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << "gpt" << i << ": " << gpt
                     << "\n";
               }
               
            } // matches: if (xnodes[i].valid())
            else
            {
               result = false;
            }
            
         } // mathches: for (ossim_uint32 i = 0; i < xnodes.size(); ++i)
         
      } // matches: if ( xnodes.size() )
      else
      {
         result = false;  // No nodes found.
      }
   } // matches: if (xdoc)
   else
   {
      result = false; // Null pointer passed in.
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG: exit status = " << (result?"true":"false")
         << std::endl;
   }
   
   return result;
}

bool ossimplugins::ossimTerraSarProductDoc::isProductGeoreferenced(
   const ossimXmlDocument* xdoc) const
{
   bool result = false;
   
   ossimString s;
   if ( getProjection(xdoc, s) )
   {
      result = (s == "GROUNDRANGE");
   }

   return result;
}

bool ossimplugins::ossimTerraSarProductDoc::getMission(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path = "/level1Product/generalHeader/mission";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getImageFile(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   bool result = false;
   ossimString subDir;
   ossimString fileName;
   
   ossimString path = "/level1Product/productComponents/imageData/file/location/path";

   if ( ossim::getPath(path, xdoc, subDir) )
   {
      path = "/level1Product/productComponents/imageData/file/location/filename";
      if (  ossim::getPath(path, xdoc, fileName) )
      {
         result = true;
         s = subDir;
         s += "/";
         s += fileName;
      }
   }
   return result;
}

bool ossimplugins::ossimTerraSarProductDoc::getSceneId(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path = "/level1Product/productInfo/sceneInfo/sceneID";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getProjection(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path = "/level1Product/productInfo/productVariantInfo/projection";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getReferencePoint(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path = "/level1Product/productSpecific/projectedImageInfo/slantToGroundRangeProjection/referencePoint";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getImageDataStrartWith(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/level1Product/productSpecific/complexImageInfo/imageDataStartWith";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getOrbitDirection(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path = "/level1Product/productInfo/missionInfo/orbitDirection";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getLookDirection(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/level1Product/productInfo/acquisitionInfo/lookDirection";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getRangeGateFirstPixel(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path = "/level1Product/productInfo/sceneInfo/rangeTime/firstPixel";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getRangeGateLastPixel(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path = "/level1Product/productInfo/sceneInfo/rangeTime/lastPixel";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getSceneCenterAzimuthTime(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path = "/level1Product/productInfo/sceneInfo/sceneCenterCoord/azimuthTimeUTC";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getSceneCenterRangeTime(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path = "/level1Product/productInfo/sceneInfo/sceneCenterCoord/rangeTime";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getSceneCenterRefColumn(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path = "/level1Product/productInfo/sceneInfo/sceneCenterCoord/refColumn";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getSceneCenterRefRow(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path = "/level1Product/productInfo/sceneInfo/sceneCenterCoord/refRow";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getSceneCenterLat(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path = "/level1Product/productInfo/sceneInfo/sceneCenterCoord/lat";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getSceneCenterLon(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path = "/level1Product/productInfo/sceneInfo/sceneCenterCoord/lon";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getSceneAverageHeight(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/level1Product/productInfo/sceneInfo/sceneAverageHeight";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getRadarCenterFrequency(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/level1Product/instrument/radarParameters/centerFrequency";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getAzimuthStartTime(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/level1Product/productInfo/sceneInfo/start/timeUTC";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getAzimuthStopTime(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/level1Product/productInfo/sceneInfo/stop/timeUTC";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getCommonPrf(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
         "/level1Product/productSpecific/complexImageInfo/commonPRF";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getCommonRsf(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path
      = "/level1Product/productSpecific/complexImageInfo/commonRSF";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getNumberOfRangeLooks(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/level1Product/processing/processingParameter/rangeLooks";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getNumberOfAzimuthLooks(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/level1Product/processing/processingParameter/azimuthLooks";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getNumberOfColumns(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/level1Product/productInfo/imageDataInfo/imageRaster/numberOfColumns";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getNumberOfRows(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/level1Product/productInfo/imageDataInfo/imageRaster/numberOfRows";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getColumnSpacing(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/level1Product/productInfo/imageDataInfo/imageRaster/columnSpacing";
   return ossim::getPath(path, xdoc, s);
}

bool ossimplugins::ossimTerraSarProductDoc::getRowSpacing(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/level1Product/productInfo/imageDataInfo/imageRaster/rowSpacing";
   return ossim::getPath(path, xdoc, s);
}
