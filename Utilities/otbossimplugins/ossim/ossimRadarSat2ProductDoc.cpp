//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Utility class to encapsulate parsing RadarSat2 product.xml
// file.
// 
//----------------------------------------------------------------------------
// $Id$

#include <ossimRadarSat2ProductDoc.h>
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
static ossimTrace traceDebug("ossimRadarSat2ProductDoc:debug");


ossimRadarSat2ProductDoc::ossimRadarSat2ProductDoc()
{
}

ossimRadarSat2ProductDoc::~ossimRadarSat2ProductDoc()
{
}

bool ossimRadarSat2ProductDoc::isRadarSat2(const ossimXmlDocument* xdoc) const
{
   bool result = false;
   if (xdoc)
   {
      ossimString s;
      getSatellite(xdoc, s);
      if (s.upcase() == "RADARSAT-2")
      {
         result = true;
      }
   }
   return result;
}

bool ossimRadarSat2ProductDoc::initPlatformPosition(
   const ossimXmlDocument* xdoc, PlatformPosition* pos) const
{
   static const char MODULE[] =
      "ossimRadarSat2ProductDoc::initPlatformPosition";
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
   }  

   bool result = true;
   
   if ( xdoc && pos )
   {
      // Get all the stateVector nodes.
      ossimString path =
         "/product/sourceAttributes/orbitAndAttitude/orbitInformation/stateVector";
      std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
      xdoc->findNodes(path, xnodes);
      if ( xnodes.size() )
      {
         const std::vector<ossimRefPtr<ossimXmlNode> >::size_type COUNT =
            xnodes.size();
         Ephemeris** ephemeris = new Ephemeris*[COUNT];
         
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

            double pos[3];
            double vit[3];
            CivilDateTime eph_civil_date;
            ossimString s;

            path = "timeStamp";
            result = findFirstNode(path, svNode, s);
            if (result)
            {
               ossim::UtcDateTimeStringToCivilDate(s, eph_civil_date);
            }
            else
            {
               result = false;
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE << " ERROR:\nNode not found: " << path
                  << std::endl;
               break;
            }

            path = "xPosition";
            result = findFirstNode(path, svNode, s);
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

            path = "yPosition";
            result = findFirstNode(path, svNode, s);
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

            path = "zPosition";
            result = findFirstNode(path, svNode, s);
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

            path = "xVelocity";
            result = findFirstNode(path, svNode, s);
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

            path = "yVelocity";
            result = findFirstNode(path, svNode, s);
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

            path = "zVelocity";
            result = findFirstNode(path, svNode, s);
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
            ephemeris[i] = eph;
            ++nbrData;
               
	 } // matches:  for (int i = 0 ; i < nbrData; ++i)

         if (result)
         {
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << " DEBUG\nnbrData:  " << nbrData << "\n";
            } 
            pos->setData(ephemeris, nbrData);
         }
         else
         {
            for (int idx = 0; idx < nbrData; ++idx)
            {
               delete ephemeris[idx];
            }
            delete [] ephemeris;
            ephemeris = 0;
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

bool ossimRadarSat2ProductDoc::initSensorParams(const ossimXmlDocument* xdoc,
                                                SensorParams* sp) const
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

      //---
      // drb ???
      // Get the nominal PRF
      //---
      double prf;
      if ( getNominalPrf(xdoc, prf) )
      {
         sp->set_prf(prf * sp->get_nAzimuthLook() );
         // sp->set_prf(prf);         
      }
      else
      {
         result = false;
      }

      // Get the Sampling frequency.
      if ( getAdcSamplingRate(xdoc, s) )
      {
         sp->set_sf(s.toDouble());
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

      // Get columns direction (1=increasing, -1=decreasing).
      if ( getPixelTimeOrdering(xdoc, s) )
      {
         if (s.downcase() == "increasing")
         {
            sp->set_col_direction(1);
         }
         else
         {
            sp->set_col_direction(-1);
         }
      }
      else
      {
         result = false;
      }

      // Get lines direction (1=increasing, -1=decreasing).
      if ( getLineTimeOrdering(xdoc, s) )
      {
         if (s.downcase() == "increasing")
         {
            sp->set_lin_direction(1);
         }
         else
         {
            sp->set_lin_direction(-1);
         }
      }
      else
      {
         result = false;
      }

      // Get the antenna pointing direction.
      if ( getAntennaPointing(xdoc, s) )
      {
         if ( s.downcase() == "right")
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

      // Get the ellipsoid semi_major axis, m, Default : WGS84
      if ( getSemiMajorAxis(xdoc, s) )
      {
         sp->set_semiMajorAxis(s.toDouble());
      }
      else
      {
         result = false;
      }

      //  Get the ellipsoid semi_minor axis, m, Default : WGS84
      if ( getSemiMinorAxis(xdoc, s) )
      {
         sp->set_semiMinorAxis(s.toDouble());
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
      
   } // matches: if (xdoc && sp)
   else
   {
      result = false;
   }
   
   return result;  
}

bool ossimRadarSat2ProductDoc::initImageSize(const ossimXmlDocument* xdoc,
                                             ossimIpt& imageSize) const
{
   bool result = true;

   if (xdoc)
   {
      ossimString s;
      if ( getNumberOfSamplesPerLine(xdoc, s) )
      {
         imageSize.x = s.toFloat64();
      }
      else
      {
         result = false;
      }
      if ( getNumberOfLines(xdoc, s) )
      {
         imageSize.y = s.toFloat64();
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
         << "ossimRadarSat2ProductDoc::initImageSize DEBUG:\nimage size: "
         << imageSize
         << "\nexit status = " << (result?"true":"false")
         << std::endl;
   }
   
   return result;
}

bool ossimRadarSat2ProductDoc::initTiePoints(const ossimXmlDocument* xdoc,
                                             std::list<ossimGpt>& gcp,
                                             std::list<ossimDpt>& icp) const
{
   static const char MODULE[] = "ossimRadarSat2ProductDoc::initTiePoints";
   
   bool result = true;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG:\n";
   }
         
   if (xdoc)
   {
      ossimString path = "/product/imageAttributes/geographicInformation/geolocationGrid/imageTiePoint";
      std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
      xdoc->findNodes(path, xnodes);
      if ( xnodes.size() )
      {
         for (ossim_uint32 i = 0; i < xnodes.size(); ++i)
         {
            if (xnodes[i].valid())
            {
               ossimRefPtr<ossimXmlNode> icNode = 0; // imageCoordinate
               icNode =
                  xnodes[i]->findFirstNode(ossimString("imageCoordinate"));
               if (icNode.valid())
               {
                  ossimString s;
                  ossimDpt dpt;
                  result = findFirstNode(ossimString("line"), icNode, s);
                  if (result)
                  {
                     dpt.y = s.toDouble();
                  }
                  else
                  {
                     result = false;
                  }
                  result = findFirstNode(ossimString("pixel"), icNode, s);
                  if (result)
                  {
                     dpt.x = s.toDouble();
                  }
                  else
                  {
                     result = false;
                  }

                  icp.push_back(dpt);

                  if (traceDebug())
                  {
                     ossimNotify(ossimNotifyLevel_DEBUG)
                        << "dpt" << i << ": " << dpt
                        << "\n";
                  }
               }
               else
               {
                  result = false;
               }

               ossimRefPtr<ossimXmlNode> gcNode = 0; // geodeticCoordinate
               gcNode = xnodes[i]->findFirstNode(
                  ossimString("geodeticCoordinate"));
               if (gcNode.valid())
               {
                  ossimString s;
                  ossimGpt gpt;
                  result = findFirstNode(ossimString("latitude"), gcNode, s);
                  if (result)
                  {
                     gpt.lat = s.toDouble();
                  }
                  else
                  {
                     result = false;
                  }
                  result = findFirstNode(ossimString("longitude"), gcNode, s);
                  if (result)
                  {
                     gpt.lon = s.toDouble();
                  }
                  else
                  {
                     result = false;
                  }
                  result = findFirstNode(ossimString("height"), gcNode, s);
                  if (result)
                  {
                     gpt.hgt = s.toDouble();
                  }
                  else
                  {
                     result = false;
                  }

                  gcp.push_back(gpt);

                  if (traceDebug())
                  {
                     ossimNotify(ossimNotifyLevel_DEBUG)
                        << "gpt" << i << ": " << gpt
                        << "\n";
                  }
               
               } // matches: if (gcNode.valid())
               else
               {
                  result = false;
               }
               
            } // if (xnodes[i].valid())
            
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

bool ossimRadarSat2ProductDoc::getSatellite(const ossimXmlDocument* xdoc,
                                            ossimString& s) const
{
   ossimString path = "/product/sourceAttributes/satellite";
   return getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getSensor(const ossimXmlDocument* xdoc,
                                         ossimString& s) const
{
   ossimString path = "/product/sourceAttributes/sensor";
   return getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getImageId(const ossimXmlDocument* xdoc,
                                          ossimString& s) const
{
   ossimString path = "/product/sourceAttributes/imageId";
   return getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getRadarCenterFrequency(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/sourceAttributes/radarParameters/radarCenterFrequency";
   return getPath(path, xdoc, s);
}

//---
// drb ???
// 
// Temporary until we decide how to get nominal prf from multiple nodes.
// drb - 15 April 2009
//---
bool ossimRadarSat2ProductDoc::getNominalPrf(const ossimXmlDocument* xdoc,
                                             double& prf) const
{
   bool result = false;
   std::vector<ossimString> v;
   if ( getPulseRepetitionFrequency(xdoc, v) )
   {
      if (v.size())
      {
         double d = 0;
         std::vector<ossimString>::const_iterator i = v.begin();
         while (i < v.end())
         {
            d += (*i).toDouble();
            ++i;
         }
         prf = d / v.size();
         result = true;
      }
   }
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimRadarSat2ProductDoc::getNominalPrf DEBUG:\nprf = "
         << prf << "\nexit status = " << (result?"true":"false")
         << std::endl;
   }
   return result;
}

bool ossimRadarSat2ProductDoc::getPulseRepetitionFrequency(
   const ossimXmlDocument* xdoc, std::vector<ossimString>& v) const
{
   ossimString path =
         "/product/sourceAttributes/radarParameters/pulseRepetitionFrequency";
   return getPath(path, xdoc, v);
}

bool ossimRadarSat2ProductDoc::getAntennaPointing(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/sourceAttributes/radarParameters/antennaPointing";
   return getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getAdcSamplingRate(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/sourceAttributes/radarParameters/adcSamplingRate";
   return getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getPassDirection(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/sourceAttributes/orbitAndAttitude/orbitInformation/passDirection";
   return getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getProductType(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageGenerationParameters/generalProcessingInformation/productType";
   return getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getZeroDopplerTimeFirstLine(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageGenerationParameters/sarProcessingInformation/zeroDopplerTimeFirstLine";
   return getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getNumberOfRangeLooks(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageGenerationParameters/sarProcessingInformation/numberOfRangeLooks";
   return getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getNumberOfAzimuthLooks(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageGenerationParameters/sarProcessingInformation/numberOfAzimuthLooks";
   return getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getSlantRangeNearEdge(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageGenerationParameters/sarProcessingInformation/slantRangeNearEdge";
   return getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getZeroDopplerAzimuthTime(
   const ossimXmlDocument* xdoc, std::vector<ossimString>& v) const
{
   ossimString path =
      "/product/imageGenerationParameters/slantRangeToGroundRange/zeroDopplerAzimuthTime";
   return getPath(path, xdoc, v);
}

bool ossimRadarSat2ProductDoc::getGroundRangeOrigin(
   const ossimXmlDocument* xdoc, std::vector<ossimString>& v) const
{
   ossimString path =
      "/product/imageGenerationParameters/slantRangeToGroundRange/groundRangeOrigin";
   return getPath(path, xdoc, v);
}

bool ossimRadarSat2ProductDoc::getGroundToSlantRangeCoefficients(
   const ossimXmlDocument* xdoc, std::vector<ossimString>& v) const
{
   ossimString path =
      "/product/imageGenerationParameters/slantRangeToGroundRange/groundToSlantRangeCoefficients";
   return getPath(path, xdoc, v);
}

bool ossimRadarSat2ProductDoc::getSemiMajorAxis(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageAttributes/geographicInformation/referenceEllipsoidParameters/semiMajorAxis";
   return getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getSemiMinorAxis(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageAttributes/geographicInformation/referenceEllipsoidParameters/semiMinorAxis";
   return getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getNumberOfSamplesPerLine(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageAttributes/rasterAttributes/numberOfSamplesPerLine";
   return getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getNumberOfLines(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageAttributes/rasterAttributes/numberOfLines";
   return getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getSampledPixelSpacing(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageAttributes/rasterAttributes/sampledPixelSpacing";
   return getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getSampledLineSpacing(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageAttributes/rasterAttributes/sampledLineSpacing";
   return getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getLineTimeOrdering(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageAttributes/rasterAttributes/lineTimeOrdering";
   return getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getPixelTimeOrdering(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageAttributes/rasterAttributes/pixelTimeOrdering";
   return getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getGeodeticTerrainHeight(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageAttributes/geographicInformation/referenceEllipsoidParameters/geodeticTerrainHeight";
   return getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getPath(const ossimString& path,
                                       const ossimXmlDocument* xdoc,
                                       ossimString& s) const
{
   bool result = false;
   if (xdoc)
   {
      std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
      xdoc->findNodes(path, xnodes);
      if ( xnodes.size() == 1 ) // Error if more than one.
      {
         if ( xnodes[0].valid() )
         {
            s = xnodes[0]->getText();
            result = true;
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimRadarSat2ProductDoc::getPath ERROR:\n"
               << "Node not found: " << path
               << std::endl; 
         }
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
               << "ossimRadarSat2ProductDoc::getPath ERROR:\n"
               << "Multiple nodes found: " << path
               << std::endl; 
      }
   }
   if (!result)
   {
      s.clear();
   }
   return result; 
}

bool ossimRadarSat2ProductDoc::getPath(const ossimString& path,
                                       const ossimXmlDocument* xdoc,
                                       std::vector<ossimString>& v) const
{
   bool result = false;
   if (xdoc)
   {
      std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
      xdoc->findNodes(path, xnodes);
      if ( xnodes.size() )
      {
         std::vector<ossimRefPtr<ossimXmlNode> >::const_iterator i =
            xnodes.begin();
         while ( i != xnodes.end() )
         {
            v.push_back( (*i)->getText() );
            ++i;
         }
         result = true;
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimRadarSat2ProductDoc::getPath ERROR:\n"
            << "Nodes not found: " << path
            << std::endl;  
      }
   }
   if (!result)
   {
      v.clear();
   }
   return result;
}

bool ossimRadarSat2ProductDoc::findFirstNode(const ossimString& path,
                                             ossimRefPtr<ossimXmlNode> node,
                                             ossimString& s) const
{
   bool result = false;
   if ( node.valid() )
   {
      ossimRefPtr<ossimXmlNode> n = node->findFirstNode(path);
      if ( n.valid() )
      {
         s = n->getText();
         if ( s.size() )
         {
            result = true;
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimRadarSat2ProductDoc::findFirstNode ERROR:\n"
               << "Node empty: " << path
               << std::endl;
         }
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimRadarSat2ProductDoc::findFirstNode ERROR:\n"
            << "Node not found: " << path
            << std::endl;   
      }
   }
   return result;
}
