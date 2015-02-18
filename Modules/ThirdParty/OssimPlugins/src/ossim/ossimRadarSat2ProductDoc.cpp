//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
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

#include <sstream>
#include <vector>

using namespace std;


namespace ossimplugins
{


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

            path = "xPosition";
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

            path = "yPosition";
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

            path = "zPosition";
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

            path = "xVelocity";
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

            path = "yVelocity";
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

            path = "zVelocity";
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
         //sp->set_prf(prf * sp->get_nAzimuthLook() );
         sp->set_prf(prf);
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
      if ( getLineTimeOrdering(xdoc, s) )
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
      if ( getPixelTimeOrdering(xdoc, s) )
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
         imageSize.x = static_cast<ossim_int32>(s.toFloat64());
      }
      else
      {
         result = false;
      }
      if ( getNumberOfLines(xdoc, s) )
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
         << "ossimRadarSat2ProductDoc::initImageSize DEBUG:\nimage size: "
         << imageSize
         << "\nexit status = " << (result?"true":"false")
         << std::endl;
   }

   return result;
}

bool ossimRadarSat2ProductDoc::initGsd(const ossimXmlDocument* xdoc,
                                       ossimDpt& gsd) const
{
   bool result = true;

   if (xdoc)
   {
      ossimString s;
      if ( getSampledPixelSpacing(xdoc, s) )
      {
         gsd.x = s.toFloat64();
      }
      else
      {
         result = false;
      }
      if ( getSampledLineSpacing(xdoc, s) )
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
         << "ossimRadarSat2ProductDoc::initGsd DEBUG:\ngsd: " << gsd
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
                  result = ossim::findFirstNode(ossimString("line"),
                                                icNode, s);
                  if (result)
                  {
                     dpt.y = s.toDouble();
                  }
                  result = ossim::findFirstNode(ossimString("pixel"),
                                                icNode, s);
                  if (result)
                  {
                     dpt.x = s.toDouble();
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
                  result = ossim::findFirstNode(ossimString("latitude"),
                                                gcNode, s);
                  if (result)
                  {
                     gpt.lat = s.toDouble();
                  }
                  result = ossim::findFirstNode(ossimString("longitude"),
                                                gcNode, s);
                  if (result)
                  {
                     gpt.lon = s.toDouble();
                  }
                  result = ossim::findFirstNode(ossimString("height"),
                                                gcNode, s);
                  if (result)
                  {
                     gpt.hgt = s.toDouble();
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


RPCModel ossimRadarSat2ProductDoc::getRpcData(const ossimXmlDocument* xdoc) const
{
   ossimString path = "/product/imageAttributes/geographicInformation/rationalFunctions/satellite";

   RPCModel model;

   ossimString searchbiasError = "/product/imageAttributes/geographicInformation/rationalFunctions/biasError";				//the line (y-coordinate)
   ossimString searchrandomError = "/product/imageAttributes/geographicInformation/rationalFunctions/randomError";			//the line (y-coordinate)
   ossimString searchlineFitQuality = "/product/imageAttributes/geographicInformation/rationalFunctions/lineFitQuality";				//the line (y-coordinate)
   ossimString searchpixelFitQuality = "/product/imageAttributes/geographicInformation/rationalFunctions/pixelFitQuality";				//the line (y-coordinate)
   ossimString searchlineOffset = "/product/imageAttributes/geographicInformation/rationalFunctions/lineOffset";						//the line (y-coordinate)
   ossimString searchpixelOffset = "/product/imageAttributes/geographicInformation/rationalFunctions/pixelOffset";						//the line (y-coordinate)
   ossimString searchlatitudeOffset = "/product/imageAttributes/geographicInformation/rationalFunctions/latitudeOffset";				//the line (y-coordinate)
   ossimString searchlongitudeOffset = "/product/imageAttributes/geographicInformation/rationalFunctions/longitudeOffset";				//the line (y-coordinate)
   ossimString searchheightOffset = "/product/imageAttributes/geographicInformation/rationalFunctions/heightOffset";						//the line (y-coordinate)
   ossimString searchlineScale = "/product/imageAttributes/geographicInformation/rationalFunctions/lineScale";						//the line (y-coordinate)
   ossimString searchpixelScale = "/product/imageAttributes/geographicInformation/rationalFunctions/pixelScale";						//the line (y-coordinate)
   ossimString searchlatitudeScale = "/product/imageAttributes/geographicInformation/rationalFunctions/latitudeScale";						//the line (y-coordinate)
   ossimString searchlongitudeScale = "/product/imageAttributes/geographicInformation/rationalFunctions/longitudeScale";						//the line (y-coordinate)
   ossimString searchheightScale = "/product/imageAttributes/geographicInformation/rationalFunctions/heightScale";						//the line (y-coordinate)

   ossimString searchlineNumeratorCoefficients = "/product/imageAttributes/geographicInformation/rationalFunctions/lineNumeratorCoefficients";						//the line (y-coordinate)
   ossimString searchlineDenominatorCoefficients = "/product/imageAttributes/geographicInformation/rationalFunctions/lineDenominatorCoefficients";						//the line (y-coordinate)
   ossimString searchpixelNumeratorCoefficients = "/product/imageAttributes/geographicInformation/rationalFunctions/pixelNumeratorCoefficients";						//the line (y-coordinate)
   ossimString searchpixelDenominatorCoefficients = "/product/imageAttributes/geographicInformation/rationalFunctions/pixelDenominatorCoefficients";						//the line (y-coordinate)


   // strings to hold values found
   ossimString biasErrorStr;;
   ossimString randomErrorStr;
   ossimString lineFitQualityStr;
   ossimString pixelFitQualityStr;
   ossimString lineOffsetStr;
   ossimString pixelOffsetStr;
   ossimString latitudeOffsetStr;
   ossimString longitudeOffsetStr;
   ossimString heightOffsetStr;
   ossimString lineScaleStr;
   ossimString pixelScaleStr;
   ossimString latitudeScaleStr;
   ossimString longitudeScaleStr;
   ossimString heightScaleStr;

   std::vector<ossimString> lineNumeratorCoefficientsStr;
   std::vector<ossimString> lineDenominatorCoefficientsStr;
   std::vector<ossimString> pixelNumeratorCoefficientsStr;
   std::vector<ossimString> pixelDenominatorCoefficientsStr;


   // doubles to hold values found
   double biasError = 0;
   double randomError = 0;
   double lineFitQuality = 0;
   double pixelFitQuality = 0;
   double lineOffset = 0;
   double pixelOffset = 0;
   double latitudeOffset = 0;
   double longitudeOffset = 0;
   double heightOffset = 0;
   double lineScale = 0;
   double pixelScale = 0;
   double latitudeScale = 0;
   double longitudeScale = 0;
   double heightScale = 0;

   vector<double> lineNumeratorCoefficients = vector<double>(20,0);
   vector<double> lineDenominatorCoefficients = vector<double>(20,0);
   vector<double> pixelNumeratorCoefficients = vector<double>(20,0);
   vector<double> pixelDenominatorCoefficients = vector<double>(20,0);

   //the final string outputs to the text file

   //check if this function is being called on the correct SAR data type
   //function is only applicable for RS2 product.xml files
   bool rs2Check = isRadarSat2(xdoc);
   if (rs2Check)
   {
      if (!ossim::getPath(searchbiasError, xdoc, biasErrorStr))
         ossimNotify(ossimNotifyLevel_WARN) << "ERROR: UNABLE TO FIND RS2 RPC COEFFICIENT INFORMATION" << endl;
      biasError = biasErrorStr.toDouble();

      if (!ossim::getPath(searchrandomError, xdoc, randomErrorStr))
         ossimNotify(ossimNotifyLevel_WARN) << "ERROR: UNABLE TO FIND RS2 RPC COEFFICIENT INFORMATION" << endl;
      randomError = randomErrorStr.toDouble();

      if (!ossim::getPath(searchlineFitQuality, xdoc, lineFitQualityStr))
         ossimNotify(ossimNotifyLevel_WARN) << "ERROR: UNABLE TO FIND RS2 RPC COEFFICIENT INFORMATION" << endl;
      lineFitQuality = lineFitQualityStr.toDouble();

      if (!ossim::getPath(searchpixelFitQuality, xdoc, pixelFitQualityStr))
         ossimNotify(ossimNotifyLevel_WARN) << "ERROR: UNABLE TO FIND RS2 RPC COEFFICIENT INFORMATION" << endl;
      pixelFitQuality = pixelFitQualityStr.toDouble();

      if (!ossim::getPath(searchlineOffset, xdoc, lineOffsetStr))
         ossimNotify(ossimNotifyLevel_WARN) << "ERROR: UNABLE TO FIND RS2 RPC COEFFICIENT INFORMATION" << endl;
      lineOffset = lineOffsetStr.toDouble();

      if (!ossim::getPath(searchpixelOffset, xdoc, pixelOffsetStr))
         ossimNotify(ossimNotifyLevel_WARN) << "ERROR: UNABLE TO FIND RS2 RPC COEFFICIENT INFORMATION" << endl;
      pixelOffset = pixelOffsetStr.toDouble();

      if (!ossim::getPath(searchlatitudeOffset, xdoc, latitudeOffsetStr))
         ossimNotify(ossimNotifyLevel_WARN) << "ERROR: UNABLE TO FIND RS2 RPC COEFFICIENT INFORMATION" << endl;
      latitudeOffset = latitudeOffsetStr.toDouble();

      if (!ossim::getPath(searchlongitudeOffset, xdoc, longitudeOffsetStr))
         ossimNotify(ossimNotifyLevel_WARN) << "ERROR: UNABLE TO FIND RS2 RPC COEFFICIENT INFORMATION" << endl;
      longitudeOffset = longitudeOffsetStr.toDouble();

      if (!ossim::getPath(searchheightOffset, xdoc, heightOffsetStr))
         ossimNotify(ossimNotifyLevel_WARN) << "ERROR: UNABLE TO FIND RS2 RPC COEFFICIENT INFORMATION" << endl;
      heightOffset = heightOffsetStr.toDouble();

      // --------------

      if (!ossim::getPath(searchlineScale, xdoc, lineScaleStr))
         ossimNotify(ossimNotifyLevel_WARN) << "ERROR: UNABLE TO FIND RS2 RPC COEFFICIENT INFORMATION" << endl;
      lineScale = lineScaleStr.toDouble();


      if (!ossim::getPath(searchpixelScale, xdoc, pixelScaleStr))
         ossimNotify(ossimNotifyLevel_WARN) << "ERROR: UNABLE TO FIND RS2 RPC COEFFICIENT INFORMATION" << endl;
      pixelScale = pixelScaleStr.toDouble();


      if (!ossim::getPath(searchlatitudeScale, xdoc, latitudeScaleStr))
         ossimNotify(ossimNotifyLevel_WARN) << "ERROR: UNABLE TO FIND RS2 RPC COEFFICIENT INFORMATION" << endl;
      latitudeScale = latitudeScaleStr.toDouble();

      // -----------------------

      if (!ossim::getPath(searchlongitudeScale, xdoc, longitudeScaleStr))
         ossimNotify(ossimNotifyLevel_WARN) << "ERROR: UNABLE TO FIND RS2 RPC COEFFICIENT INFORMATION" << endl;
      longitudeScale = longitudeScaleStr.toDouble();


      if (!ossim::getPath(searchheightScale, xdoc, heightScaleStr))
         ossimNotify(ossimNotifyLevel_WARN) << "ERROR: UNABLE TO FIND RS2 RPC COEFFICIENT INFORMATION" << endl;
      heightScale = heightScaleStr.toDouble();

      // ---- parameters for reading in coeefs ------------

      double val=0;

      // -------------------------------------


      if (!ossim::getPath(searchlineNumeratorCoefficients, xdoc, lineNumeratorCoefficientsStr))
         ossimNotify(ossimNotifyLevel_WARN) << "ERROR: UNABLE TO FIND RS2 RPC COEFFICIENT INFORMATION" << endl;


      string lineNumeratorCoefficientsStr_N = lineNumeratorCoefficientsStr[0];
      // place into a stream
      std::stringstream LNstream(lineNumeratorCoefficientsStr_N);

      for (int i=0; i < 20; i ++)
      {
         LNstream >> val;
         lineNumeratorCoefficients[i] = val;
      }

      // ------------------

      if (!ossim::getPath(searchlineDenominatorCoefficients, xdoc, lineDenominatorCoefficientsStr))
         ossimNotify(ossimNotifyLevel_WARN) << "ERROR: UNABLE TO FIND RS2 RPC COEFFICIENT INFORMATION" << endl;


      string lineDenominatorCoefficientsStr_N = lineDenominatorCoefficientsStr[0];

      // place into a stream
      std::stringstream LDstream(lineDenominatorCoefficientsStr_N);

      for (int i=0; i < 20; i ++)
      {
         LDstream >> val;
         lineDenominatorCoefficients[i] = val;
      }

      // ------------------

      if (!ossim::getPath(searchpixelNumeratorCoefficients, xdoc, pixelNumeratorCoefficientsStr))
         ossimNotify(ossimNotifyLevel_WARN) << "ERROR: UNABLE TO FIND RS2 RPC COEFFICIENT INFORMATION" << endl;

      string pixelNumeratorCoefficientsStr_N = pixelNumeratorCoefficientsStr[0];

      // place into a stream
      std::stringstream PNstream(pixelNumeratorCoefficientsStr_N);

      for (int i=0; i < 20; i ++)
      {
         PNstream >> val;
         pixelNumeratorCoefficients[i] = val;
      }

      // ------------------

      if (!ossim::getPath(searchpixelDenominatorCoefficients, xdoc, pixelDenominatorCoefficientsStr))
         ossimNotify(ossimNotifyLevel_WARN) << "ERROR: UNABLE TO FIND RS2 RPC COEFFICIENT INFORMATION" << endl;

      string pixelDenominatorCoefficientsStr_N = pixelDenominatorCoefficientsStr[0];

      // place into a stream
      std::stringstream PDstream(pixelDenominatorCoefficientsStr_N);

      for (int i=0; i < 20; i ++)
      {
         PDstream >> val;
         pixelDenominatorCoefficients[i] = val;
      }

      // end character search term

      model.biasError = biasError;
      model.randomError = randomError;
      model.lineFitQuality = lineFitQuality;
      model.pixelFitQuality = pixelFitQuality;
      model.lineOffset = lineOffset;
      model.pixelOffset = pixelOffset;
      model.latitudeOffset = latitudeOffset;
      model.longitudeOffset = longitudeOffset;
      model.heightOffset = heightOffset;
      model.lineScale = lineScale;
      model.pixelScale = pixelScale;
      model.latitudeScale = latitudeScale;
      model.longitudeScale = longitudeScale;
      model.heightScale = heightScale;
      model.lineNumeratorCoefficients = lineNumeratorCoefficients;
      model.lineDenominatorCoefficients = lineDenominatorCoefficients;
      model.pixelNumeratorCoefficients = pixelNumeratorCoefficients;
      model.pixelDenominatorCoefficients = pixelDenominatorCoefficients;
   }

   return model;
}

bool ossimRadarSat2ProductDoc::getSatellite(const ossimXmlDocument* xdoc,
                                            ossimString& s) const
{
   ossimString path = "/product/sourceAttributes/satellite";
   // The saved File is an ossimKeywordlist

   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getSensor(const ossimXmlDocument* xdoc,
                                         ossimString& s) const
{
   ossimString path = "/product/sourceAttributes/sensor";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getBeamModeMnemonic(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path = "/product/sourceAttributes/beamModeMnemonic";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getImageId(const ossimXmlDocument* xdoc,
                                          ossimString& s) const
{
   ossimString path = "/product/sourceAttributes/imageId";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getImageFile(const ossimXmlDocument* xdoc,
                                            ossimString& s) const
{
   bool result = false;
   ossimString fileName;
   
   ossimString path = "/product/imageAttributes/fullResolutionImageData";
   
   if ( ossim::getPath(path, xdoc, fileName) )
   {
      result = true;
      s = fileName;
   }
   
   return result;
}

bool ossimRadarSat2ProductDoc::getAcquisitionType(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/sourceAttributes/radarParameters/acquisitionType";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getRadarCenterFrequency(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/sourceAttributes/radarParameters/radarCenterFrequency";
   return ossim::getPath(path, xdoc, s);
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
   return ossim::getPath(path, xdoc, v);
}

bool ossimRadarSat2ProductDoc::getAntennaPointing(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/sourceAttributes/radarParameters/antennaPointing";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getAdcSamplingRate(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/sourceAttributes/radarParameters/adcSamplingRate";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getPassDirection(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/sourceAttributes/orbitAndAttitude/orbitInformation/passDirection";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getProductType(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageGenerationParameters/generalProcessingInformation/productType";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getZeroDopplerTimeFirstLine(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageGenerationParameters/sarProcessingInformation/zeroDopplerTimeFirstLine";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getNumberOfRangeLooks(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageGenerationParameters/sarProcessingInformation/numberOfRangeLooks";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getNumberOfAzimuthLooks(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageGenerationParameters/sarProcessingInformation/numberOfAzimuthLooks";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getSlantRangeNearEdge(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageGenerationParameters/sarProcessingInformation/slantRangeNearEdge";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getZeroDopplerAzimuthTime(
   const ossimXmlDocument* xdoc, std::vector<ossimString>& v) const
{
   ossimString path =
      "/product/imageGenerationParameters/slantRangeToGroundRange/zeroDopplerAzimuthTime";
   return ossim::getPath(path, xdoc, v);
}

bool ossimRadarSat2ProductDoc::getGroundRangeOrigin(
   const ossimXmlDocument* xdoc, std::vector<ossimString>& v) const
{
   ossimString path =
      "/product/imageGenerationParameters/slantRangeToGroundRange/groundRangeOrigin";
   return ossim::getPath(path, xdoc, v);
}

bool ossimRadarSat2ProductDoc::getGroundToSlantRangeCoefficients(
   const ossimXmlDocument* xdoc, std::vector<ossimString>& v) const
{
   ossimString path =
      "/product/imageGenerationParameters/slantRangeToGroundRange/groundToSlantRangeCoefficients";
   return ossim::getPath(path, xdoc, v);
}

bool ossimRadarSat2ProductDoc::getSemiMajorAxis(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageAttributes/geographicInformation/referenceEllipsoidParameters/semiMajorAxis";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getSemiMinorAxis(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageAttributes/geographicInformation/referenceEllipsoidParameters/semiMinorAxis";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getNumberOfSamplesPerLine(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageAttributes/rasterAttributes/numberOfSamplesPerLine";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getNumberOfLines(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageAttributes/rasterAttributes/numberOfLines";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getSampledPixelSpacing(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageAttributes/rasterAttributes/sampledPixelSpacing";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getSampledLineSpacing(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageAttributes/rasterAttributes/sampledLineSpacing";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getLineTimeOrdering(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageAttributes/rasterAttributes/lineTimeOrdering";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getPixelTimeOrdering(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageAttributes/rasterAttributes/pixelTimeOrdering";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getIncidenceAngleNearRange(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageGenerationParameters/sarProcessingInformation/incidenceAngleNearRange";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getIncidenceAngleFarRange(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageGenerationParameters/sarProcessingInformation/incidenceAngleFarRange ";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getSatelliteHeight(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageGenerationParameters/sarProcessingInformation/satelliteHeight";
   return ossim::getPath(path, xdoc, s);
}

bool ossimRadarSat2ProductDoc::getGeodeticTerrainHeight(
   const ossimXmlDocument* xdoc, ossimString& s) const
{
   ossimString path =
      "/product/imageAttributes/geographicInformation/referenceEllipsoidParameters/geodeticTerrainHeight";
   return ossim::getPath(path, xdoc, s);
}

} // matches: namespace ossimplugins
