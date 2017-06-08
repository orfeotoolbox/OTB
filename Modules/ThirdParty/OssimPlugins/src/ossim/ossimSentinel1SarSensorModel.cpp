/*
 * Copyright (C) 2005-2017 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include <ossimSentinel1SarSensorModel.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossimXmlTools.h>

namespace {// Anonymous namespace
    const ossimString attAzimuthTime      = "azimuthTime";
    const ossimString attFirstValidSample = "firstValidSample";
    const ossimString attGr0              = "gr0";
    const ossimString attGrsrCoefficients = "grsrCoefficients";
    const ossimString attHeight           = "height";
    const ossimString attLatitude         = "latitude";
    const ossimString attLine             = "line";
    const ossimString attLongitude        = "longitude";
    const ossimString attPixel            = "pixel";
    const ossimString attPosition         = "position";
    const ossimString attSlantRangeTime   = "slantRangeTime";
    const ossimString attSr0              = "sr0";
    const ossimString attSrgrCoefficients = "srgrCoefficients";
    const ossimString attTime             = "time";
    const ossimString attVelocity         = "velocity";
    const ossimString attX                = "x";
    const ossimString attY                = "y";
    const ossimString attZ                = "z";
}// Anonymous namespace

#if defined(USE_BOOST_TIME)
            using boost::posix_time::microseconds;
            using boost::posix_time::seconds;
#else
            using ossimplugins::time::microseconds;
            using ossimplugins::time::seconds;
#endif
void ossimplugins::ossimSentinel1SarSensorModel::readCoordinates(
        ossimXmlDocument const& xmlDoc, ossimString const& xpath,
        ossimString const& rg0_xpath, ossimString const& coeffs_xpath,
        std::vector<CoordinateConversionRecordType> & outputRecords)
{
   std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
   xmlDoc.findNodes(xpath, xnodes);

   for(auto itNode = xnodes.begin(); itNode!=xnodes.end();++itNode)
   {
      CoordinateConversionRecordType coordRecord;

      coordRecord.azimuthTime = getTimeFromFirstNode(**itNode, attAzimuthTime);

      coordRecord.rg0 = getDoubleFromFirstNode(**itNode, rg0_xpath);;

      ossimString const& s = getTextFromFirstNode(**itNode, coeffs_xpath);
      std::vector<ossimString> ssplit = s.split(" ");

      if (ssplit.empty())
      {
         throw std::runtime_error(("The "+rg0_xpath+" record has an empty coef vector").string());
      }
      for (std::vector<ossimString>::const_iterator cIt = ssplit.begin(), e = ssplit.end()
            ; cIt != e
            ; ++cIt
          )
      {
         coordRecord.coefs.push_back(cIt->toDouble());
      }
      assert(!coordRecord.coefs.empty()&&"The rg0 record has empty coefs vector.");

      outputRecords.push_back(coordRecord);
   }
}

namespace ossimplugins
{
void ossimSentinel1SarSensorModel::readAnnotationFile(const std::string & annotationXml)
{
    ossimRefPtr<ossimXmlDocument> xmlDoc = new ossimXmlDocument(annotationXml);
    const ossimXmlNode & xmlRoot = *xmlDoc->getRoot();

    //Parse specific metadata for Sentinel1
    //TODO add as members to the Sentinel1SarSensorModel
    const std::string & product_type = getTextFromFirstNode(xmlRoot, "adsHeader/productType");
    const std::string & mode         = getTextFromFirstNode(xmlRoot, "adsHeader/mode");
    const std::string & swath        = getTextFromFirstNode(xmlRoot, "adsHeader/swath");
    const std::string & polarisation = getTextFromFirstNode(xmlRoot, "adsHeader/polarisation");

    theProductType = ProductType(product_type);

    // First, lookup position/velocity records
    std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
    xmlDoc->findNodes("/product/generalAnnotation/orbitList/orbit",xnodes);

    //TODO uncomment and adapt following code from s1_inverse to fill
    //SarSensorModel structure

    for(auto itNode = xnodes.begin(); itNode!=xnodes.end();++itNode)
    {
        OrbitRecordType orbitRecord;

        // Retrieve acquisition time
        orbitRecord.azimuthTime = getTimeFromFirstNode(**itNode, attTime);

        // Retrieve ECEF position
        ossimXmlNode const& positionNode = getExpectedFirstNode(**itNode, attPosition);
        orbitRecord.position[0] = getDoubleFromFirstNode(positionNode, attX);
        orbitRecord.position[1] = getDoubleFromFirstNode(positionNode, attY);
        orbitRecord.position[2] = getDoubleFromFirstNode(positionNode, attZ);

        // Retrieve ECEF velocity
        ossimXmlNode const& velocityNode = getExpectedFirstNode(**itNode, attVelocity);
        orbitRecord.velocity[0] = getDoubleFromFirstNode(velocityNode, attX);
        orbitRecord.velocity[1] = getDoubleFromFirstNode(velocityNode, attY);
        orbitRecord.velocity[2] = getDoubleFromFirstNode(velocityNode, attZ);

        //Add one orbits record
        theOrbitRecords.push_back(orbitRecord);
    }

    //Parse the near range time (in seconds)
    theNearRangeTime = getDoubleFromFirstNode(xmlRoot, "imageAnnotation/imageInformation/slantRangeTime");

    //Parse the range sampling rate
    theRangeSamplingRate = getDoubleFromFirstNode(xmlRoot, "generalAnnotation/productInformation/rangeSamplingRate");

    //Parse the range resolution
    theRangeResolution = getDoubleFromFirstNode(xmlRoot, "imageAnnotation/imageInformation/rangePixelSpacing");

    //Parse the radar frequency
    theRadarFrequency = getDoubleFromFirstNode(xmlRoot, "generalAnnotation/productInformation/radarFrequency");

    //Parse azimuth time interval
    const double azimuthTimeInterval = getDoubleFromFirstNode(xmlRoot, "imageAnnotation/imageInformation/azimuthTimeInterval");
#if defined(USE_BOOST_TIME)
      theAzimuthTimeInterval = boost::posix_time::precise_duration(azimuthTimeInterval * 1000000.);
#else
      theAzimuthTimeInterval = seconds(azimuthTimeInterval);
#endif
    ossimNotify(ossimNotifyLevel_DEBUG) << "theAzimuthTimeInterval " << theAzimuthTimeInterval.total_microseconds() << "us\n";


    // Now read burst records as well
    xnodes.clear();
    xmlDoc->findNodes("/product/swathTiming/burstList/burst",xnodes);

    if(xnodes.empty())
    {
        BurstRecordType burstRecord;

        burstRecord.startLine = 0;
        burstRecord.azimuthStartTime = getTimeFromFirstNode(xmlRoot,"imageAnnotation/imageInformation/productFirstLineUtcTime");

        ossimNotify(ossimNotifyLevel_DEBUG)<< burstRecord.azimuthStartTime<<'\n';

        burstRecord.azimuthStopTime = getTimeFromFirstNode(xmlRoot,"imageAnnotation/imageInformation/productLastLineUtcTime");
        burstRecord.endLine = getTextFromFirstNode(xmlRoot, "imageAnnotation/imageInformation/numberOfLines").toUInt16()-1;

        theBurstRecords.push_back(burstRecord);
    }
    else
    {
        const unsigned int linesPerBurst = xmlRoot.findFirstNode("swathTiming/linesPerBurst")->getText().toUInt16();
        unsigned int burstId(0);

        for(auto itNode = xnodes.begin(); itNode!=xnodes.end();++itNode,++burstId)
        {
            BurstRecordType burstRecord;

            const ossimSarSensorModel::TimeType azTime = getTimeFromFirstNode(**itNode, attAzimuthTime);

            ossimString const& s = getTextFromFirstNode(**itNode, attFirstValidSample);

            long first_valid(0), last_valid(0);
            bool begin_found(false), end_found(false);

            std::vector<ossimString> ssp = s.split(" ");

            for (std::vector<ossimString>::const_iterator sIt = ssp.begin(), e = ssp.end()
                    ; sIt != e && !end_found
                    ; ++sIt
                )
            {
                if(!begin_found)
                {
                    if(*sIt!="-1")
                    {
                        begin_found = true;
                    }
                    else
                    {
                        ++first_valid;
                    }
                    ++last_valid;
                }
                else
                {
                    if(!end_found && *sIt=="-1")
                    {
                        end_found = true;
                        --last_valid;
                    }
                    else
                    {
                        ++last_valid;
                    }
                }
            }

            burstRecord.startLine = burstId*linesPerBurst + first_valid;
            burstRecord.endLine = burstId*linesPerBurst + last_valid;

            burstRecord.azimuthStartTime = azTime + (first_valid*theAzimuthTimeInterval);
            burstRecord.azimuthStopTime = azTime  + (last_valid*theAzimuthTimeInterval);

            theBurstRecords.push_back(burstRecord);
        }
    }

    if(isGRD())
    {
        readCoordinates(*xmlDoc,
                "/product/coordinateConversion/coordinateConversionList/coordinateConversion",
                attSr0, attSrgrCoefficients,
                theSlantRangeToGroundRangeRecords);

        readCoordinates(*xmlDoc,
                "/product/coordinateConversion/coordinateConversionList/coordinateConversion",
                attGr0, attGrsrCoefficients,
                theGroundRangeToSlantRangeRecords);
    }

    xnodes.clear();
    xmlDoc->findNodes("/product/geolocationGrid/geolocationGridPointList/geolocationGridPoint",xnodes);

    for(auto itNode = xnodes.begin(); itNode!=xnodes.end();++itNode)
    {
        GCPRecordType gcpRecord;

        // Retrieve acquisition time
        gcpRecord.azimuthTime = getTimeFromFirstNode(**itNode, attAzimuthTime);

        gcpRecord.slantRangeTime = getDoubleFromFirstNode(**itNode, attSlantRangeTime);

        gcpRecord.imPt.x = getDoubleFromFirstNode(**itNode, attPixel);

        // In TOPSAR products, GCPs are weird (they fall in black lines
        // between burst. This code allows moving them to a valid area of
        // the image.
        if(theBurstRecords.size()>2)
        {
            ossimSarSensorModel::TimeType acqStart;
            bool burstFound(false);
            unsigned long acqStartLine(0);

            for(auto bIt = theBurstRecords.rbegin();bIt!=theBurstRecords.rend() && !burstFound;++bIt)
            {
                if(gcpRecord.azimuthTime >= bIt->azimuthStartTime && gcpRecord.azimuthTime < bIt->azimuthStopTime)
                {
                    burstFound = true;
                    acqStart = bIt->azimuthStartTime;
                    acqStartLine = bIt->startLine;
                }
            }

            if(!burstFound)
            {
                if(gcpRecord.azimuthTime < theBurstRecords.front().azimuthStartTime)
                {
                    acqStart = theBurstRecords.front().azimuthStartTime;
                    acqStartLine = theBurstRecords.front().startLine;
                }
                else if (gcpRecord.azimuthTime >= theBurstRecords.front().azimuthStopTime)
                {
                    acqStart = theBurstRecords.back().azimuthStartTime;
                    acqStartLine = theBurstRecords.back().startLine;
                }
            }
            const DurationType timeSinceStart = gcpRecord.azimuthTime - acqStart;

            gcpRecord.imPt.y= timeSinceStart/theAzimuthTimeInterval + acqStartLine;
            ossimNotify(ossimNotifyLevel_DEBUG) << "timeSinceStart: " << timeSinceStart << " = " << gcpRecord.azimuthTime << " - " << acqStart <<  " (azTime-acqStart)"<< "\n";
            ossimNotify(ossimNotifyLevel_DEBUG) << "imPt_y: " << gcpRecord.imPt.y << " = " << timeSinceStart.total_microseconds() << "/" << theAzimuthTimeInterval.total_microseconds() << "+" << acqStartLine << "\n";
        }
        else
        {
            gcpRecord.imPt.y = getDoubleFromFirstNode(**itNode, attLine);;
        }
        ossimGpt geoPoint;
        gcpRecord.worldPt.lat = getDoubleFromFirstNode(**itNode, attLatitude);
        gcpRecord.worldPt.lon = getDoubleFromFirstNode(**itNode, attLongitude);
        gcpRecord.worldPt.hgt = getDoubleFromFirstNode(**itNode, attHeight);

        theGCPRecords.push_back(gcpRecord);
    }

    this->optimizeTimeOffsetsFromGcps();
}

} // namespace ossimplugins
