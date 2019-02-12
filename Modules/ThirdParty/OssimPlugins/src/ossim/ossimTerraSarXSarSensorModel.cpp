/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
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


#include <ossimTerraSarXSarSensorModel.h>
#include <ossim/base/ossimXmlDocument.h>
#include "ossim/ossimXmlTools.h"


namespace {// Anonymous namespace
    const ossimString attTimeUTC = "timeUTC";
    const ossimString attPosX    = "posX";
    const ossimString attPosY    = "posY";
    const ossimString attPosZ    = "posZ";
    const ossimString attVelX    = "velX";
    const ossimString attVelY    = "velY";
    const ossimString attVelZ    = "velZ";
    const ossimString attT       = "t";
    const ossimString attTau     = "tau";
    const ossimString attCol     = "col";
    const ossimString attRow     = "row";
    const ossimString attLat     = "lat";
    const ossimString attLon     = "lon";
    const ossimString attHeight  = "height";
}// Anonymous namespace

void ossimplugins::ossimTerraSarXSarSensorModel::readAnnotationFile(const std::string & annotationXml, const std::string & geoXml)
{
    ossimRefPtr<ossimXmlDocument> xmlDoc = new ossimXmlDocument(annotationXml);
    const ossimXmlNode & xmlRoot = *xmlDoc->getRoot();

    //isGRD parse variant?
    std::string const& product_type = getTextFromFirstNode(xmlRoot, "productInfo/productVariantInfo/productVariant");

    std::cout << "type " <<  product_type << '\n';

    theProductType = ProductType(product_type);

    // First, lookup position/velocity records
    std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
    xmlDoc->findNodes("/level1Product/platform/orbit/stateVec",xnodes);

    std::cout << "Number of states " << xnodes.size() << '\n';

    for(std::vector<ossimRefPtr<ossimXmlNode> >::iterator itNode = xnodes.begin(); itNode!=xnodes.end();++itNode)
    {
        OrbitRecordType orbitRecord;

        // Retrieve acquisition time
        orbitRecord.azimuthTime = getTimeFromFirstNode(**itNode, attTimeUTC);

        // Retrieve ECEF position
        orbitRecord.position[0] = getDoubleFromFirstNode(**itNode, attPosX);
        orbitRecord.position[1] = getDoubleFromFirstNode(**itNode, attPosY);
        orbitRecord.position[2] = getDoubleFromFirstNode(**itNode, attPosZ);

        // Retrieve ECEF velocity
        orbitRecord.velocity[0] = getDoubleFromFirstNode(**itNode, attVelX);
        orbitRecord.velocity[1] = getDoubleFromFirstNode(**itNode, attVelY);
        orbitRecord.velocity[2] = getDoubleFromFirstNode(**itNode, attVelZ);

        //Add one orbits record
        std::cout << "Add theOrbitRecords\n";
        theOrbitRecords.push_back(orbitRecord);
    }

    //Parse the near range time (in seconds)
    theNearRangeTime = getDoubleFromFirstNode(xmlRoot, "productInfo/sceneInfo/rangeTime/firstPixel");

    std::cout << "theNearRangeTime " << theNearRangeTime << '\n';

    //Parse the range sampling rate
    theRangeSamplingRate = getDoubleFromFirstNode(xmlRoot, "instrument/settings/RSF");

    std::cout << "theRangeSamplingRate " << theRangeSamplingRate << '\n';

    //Parse the range resolution
    theRangeResolution = getDoubleFromFirstNode(xmlRoot, "productSpecific/complexImageInfo/slantRangeResolution");

    std::cout << "theRangeResolution " << theRangeResolution << '\n';

    //Parse the radar frequency
    theRadarFrequency = getDoubleFromFirstNode(xmlRoot, "instrument/settings/settingRecord/PRF");

    std::cout << "theRadarFrequency " << theRadarFrequency << '\n';

    //Manage only strip map product for now (one burst)

    //Parse azimuth time start/stop
    const TimeType azimuthTimeStart = getTimeFromFirstNode(xmlRoot, "productInfo/sceneInfo/start/timeUTC");

    std::cout << "azimuthTimeStart " << azimuthTimeStart << '\n';

    const TimeType azimuthTimeStop = getTimeFromFirstNode(xmlRoot, "productInfo/sceneInfo/stop/timeUTC");

    std::cout << "azimuthTimeStop " << azimuthTimeStop << '\n';

    const DurationType td = azimuthTimeStop - azimuthTimeStart;

    // numberOfRows
    unsigned int numberOfRows = xmlRoot.findFirstNode("productInfo/imageDataInfo/imageRaster/numberOfRows")->getText().toUInt16();

    std::cout << "numberOfRows " << numberOfRows << '\n';

    //Compute azimuth time interval
    theAzimuthTimeInterval = td / static_cast<double> (numberOfRows);

    std::cout << "theAzimuthTimeInterval " << theAzimuthTimeInterval.total_microseconds()  << " and 1/prf: " << (1 / theRadarFrequency) * 1000000 << '\n';

    //For Terrasar-X only 1 burst is supported for now
    BurstRecordType burstRecord;

    burstRecord.startLine = 0;
    burstRecord.azimuthStartTime = azimuthTimeStart;
    burstRecord.azimuthStopTime = azimuthTimeStop;

    burstRecord.endLine = numberOfRows - 1;

    theBurstRecords.push_back(burstRecord);

    //GRD (detected product)
    if(isGRD())
    {
        //Retrieve Slant Range to Ground range coeddifcients
        CoordinateConversionRecordType coordRecord;

        //Get azimuth time start (again)
        coordRecord.azimuthTime = azimuthTimeStart;

        //Set ground range origin to 0 (FIXME?)
        coordRecord.rg0 = 0.;

        //Read  coefficients
        xnodes.clear();

        const unsigned int polynomialDegree = xmlRoot.findFirstNode("productSpecific/projectedImageInfo/slantToGroundRangeProjection/polynomialDegree")->getText().toUInt16();

        std::cout << "Number of coefficients " << polynomialDegree << '\n';

        ossimString path = "/level1Product/productSpecific/projectedImageInfo/slantToGroundRangeProjection/coefficient";
        const ossimString EXP = "exponent";
        ossimString s;

        xmlDoc->findNodes(path, xnodes);

        if ( xnodes.size() )
        {
            for (unsigned int i = 0; i < xnodes.size(); ++i)
            {
                if (xnodes[i].valid())
                {
                    xnodes[i]->getAttributeValue(s, EXP);
                    const double coeff = xnodes[i]->getText().toDouble();
                    coordRecord.coefs.push_back(coeff);
                    std::cout << "Coef number " << i << " value: " << coeff << '\n';
                }
            }
        }
        assert(!coordRecord.coefs.empty()&&"The srgr record has empty coefs vector.");

        theSlantRangeToGroundRangeRecords.push_back(coordRecord);
    }

    //Parse GCPs
    ossimRefPtr<ossimXmlDocument> xmlGeo = new ossimXmlDocument(geoXml);

    xnodes.clear();
    xmlGeo->findNodes("/geoReference/geolocationGrid/gridPoint",xnodes);

    std::cout<<"Found "<<xnodes.size()<<" GCPs\n";

    for(std::vector<ossimRefPtr<ossimXmlNode> >::iterator itNode = xnodes.begin(); itNode!=xnodes.end();++itNode)
    {
        GCPRecordType gcpRecord;

        // Get delta acquisition time
        const double deltaAzimuth = getDoubleFromFirstNode(**itNode, attT);
#if defined(USE_BOOST_TIME)
        using boost::posix_time::microseconds;
#else
        using ossimplugins::time::microseconds;
#endif
        gcpRecord.azimuthTime = azimuthTimeStart + microseconds(deltaAzimuth * 1000000);

        //Get delta range time
        gcpRecord.slantRangeTime = theNearRangeTime + getDoubleFromFirstNode(**itNode, attTau);

        gcpRecord.imPt.x = getDoubleFromFirstNode(**itNode, attCol) - 1.;

        gcpRecord.imPt.y = getDoubleFromFirstNode(**itNode, attRow) - 1.;

        ossimGpt geoPoint;
        gcpRecord.worldPt.lat = getDoubleFromFirstNode(**itNode, attLat);
        gcpRecord.worldPt.lon = getDoubleFromFirstNode(**itNode, attLon);
        gcpRecord.worldPt.hgt = getDoubleFromFirstNode(**itNode, attHeight);

        theGCPRecords.push_back(gcpRecord);
    }

    this->optimizeTimeOffsetsFromGcps();
}
