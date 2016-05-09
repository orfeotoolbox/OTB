#include "ossimSentinel1ProductDoc.h"
#include <cstdio>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimDate.h>
#include "ossimXmlTools.h"

// Define Trace flags for use within this file:
static ossimTrace traceExec  ("ossimSentinel1SupportData:exec");
static ossimTrace traceDebug ("ossimSentinel1SupportData:debug");

namespace ossimplugins
{

    static const char LOAD_FROM_PRODUCT_FILE_KW[] = "load_from_product_file_flag";
    static const char PRODUCT_XML_FILE_KW[] = "product_xml_filename";
    static const char SUPPORT_DATA_PREFIX[] = "support_data.";

    ossimSentinel1ProductDoc::ossimSentinel1ProductDoc ()
        : ossimErrorStatusInterface ()
        , theRangeSpacingTotal (0.0)
        , theAzimuthSpacingTotal (0.0)
        , theSLC (false)
        , theTOPSAR (false)
        {
        }

    ossimSentinel1ProductDoc::~ossimSentinel1ProductDoc ()
    {
    }

    bool ossimSentinel1ProductDoc::openMetadataFile(ossimXmlDocument& doc, ossimString const& file) const
    {
        if ( !doc.openFile( file ) )
        {
            ossimNotify(ossimNotifyLevel_FATAL) << "ossimSentinel1ProductDoc::openMetadataFile" << std::endl;
            return false;
        }

        return true;
    }


    double
    ossimSentinel1ProductDoc
    ::getModifiedJulianDate(ossimString const& utcTimeString)
    {
        std::vector<ossimString> date_time;
        std::vector<ossimString> output;

        utcTimeString.split(date_time, "T");

        date_time[0].split(output, "-");

        if (output.size () != 3)
        {
            ossimNotify(ossimNotifyLevel_DEBUG) << "invalid date\n";
        }

        char* stop;
        int year = std::strtol( output[0].c_str(), &stop, 10);
        if ( *stop != 0 )
        {
            ossimNotify(ossimNotifyLevel_DEBUG) << "invalid year\n";
        }

        stop  = 0;
        int month = std::strtol( output[1].c_str(), &stop, 10);
        if ( *stop != 0 )
        {
            ossimNotify(ossimNotifyLevel_DEBUG) << "invalid month\n";
        }
        stop  = 0;
        int day = std::strtol( output[2].c_str(), &stop, 10);
        if ( *stop != 0 )
        {
            ossimNotify(ossimNotifyLevel_DEBUG) << "invalid day\n";
        }
        /* Parse time */
        output.clear();
        date_time[1].split(output, ":");
        if (output.size () != 3)
        {
            ossimNotify(ossimNotifyLevel_DEBUG) << "invalid time\n";
        }

        stop  = 0;
        int hours = std::strtol( output[0].c_str(), &stop, 10);
        if ( *stop != 0 )
        {
            ossimNotify(ossimNotifyLevel_DEBUG) << "invalid hours\n";
        }

        stop  = 0;
        int minutes = std::strtol( output[1].c_str(), &stop, 10);
        if ( *stop != 0 )
        {
            ossimNotify(ossimNotifyLevel_DEBUG) << "invalid minutes\n";
        }

        stop  = 0;
        int seconds = std::strtod( output[2].c_str(), &stop);
        if ( *stop != 0 )
        {
            ossimNotify(ossimNotifyLevel_DEBUG) << "invalid seconds in float\n";
        }

        /* Below method could replace all above split and convert. But there is a lot of
           checks done in  ossimLocalTm::setIso8601( ) which slows us down. And I dont
           want that. */
        /*
           ossimLocalTm otm;
           if ( !otm.setIso8601(utcString) )
           {
           }
           ossimDate ossimdate(otm);
         */

        ossimDate ossimdate(month, day, year);
        ossimdate.setHour(hours);
        ossimdate.setMin(minutes);
        ossimdate.setFloatSec(seconds);
        return ossimdate.getModifiedJulian();
    }

    bool ossimSentinel1ProductDoc::readProductMetadata()
    {
        bool commonMetadataRetrieved = false;
        double heightSum = 0.0;
        int numBands = 0;

        ossimDirectory annotationDir( theManifestDirectory.dirCat( "annotation") );
        std::vector<ossimFilename> files;
        annotationDir.findAllFilesThatMatch(files, ".xml");
        std::vector<ossimFilename>::const_iterator it = files.begin();

        /* avoid zero value for numBands. This will result in division by zero below */
        if (files.empty()) numBands = 1;

        for (unsigned int count=0; it != files.end(); ++it,  ++count)
        {
            ossimXmlDocument theProductXmlDocument;
            if (! openMetadataFile(theProductXmlDocument, *it)) {
                continue;
            }

            const ossimXmlNodePtr & productRoot = theProductXmlDocument.getRoot();
            assert(productRoot);

            const ossimXmlNode & adsHeader = getExpectedFirstNode(*productRoot, "adsHeader");

            const ossimString & swath        = getOptionalTextFromFirstNode(adsHeader, "swath");
            const ossimString & polarisation = getOptionalTextFromFirstNode(adsHeader, "polarisation");

            char bandPrefix[256];
            std::snprintf(bandPrefix, sizeof(bandPrefix), "Band[%d].", count);

            theProductKwl.add(bandPrefix,
                    "swath",
                    swath,
                    true);

            theProductKwl.add(bandPrefix,
                    "polarisation",
                    polarisation,
                    true);

            theProductKwl.add(bandPrefix,
                    "annotation",
                    (*it).file(),
                    true);

            theProductKwl.add(bandPrefix,
                    "first_line_time",
                    getOptionalTextFromFirstNode(adsHeader, "startTime"),
                    true);

            theProductKwl.add(bandPrefix,
                    "last_line_time",
                    getOptionalTextFromFirstNode(adsHeader, "stopTime"),
                    true);
            //RK maybe use this->getManifestPrefix()
            theProductKwl.add(SUPPORT_DATA_PREFIX,
                    "mds1_tx_rx_polar",
                    polarisation,
                    true);

            const ossimXmlNode & imageInformation = getExpectedFirstNode(*productRoot, "imageAnnotation/imageInformation");

            const ossimXmlNode & productInformation = getExpectedFirstNode(*productRoot, "generalAnnotation/productInformation");

            theProductKwl.add(SUPPORT_DATA_PREFIX,
                    "data_take_id",
                    getOptionalTextFromFirstNode(adsHeader, "missionDataTakeId"),
                    true);

            theProductKwl.add(SUPPORT_DATA_PREFIX,
                    "slice_num",
                    getOptionalTextFromFirstNode(imageInformation, "sliceNumber"),
                    true);

            theProductKwl.add(SUPPORT_DATA_PREFIX,
                    "line_time_interval",
                    getOptionalTextFromFirstNode(imageInformation, "azimuthTimeInterval"),
                    true);

            theRangeSpacingTotal += getOptionalTextFromFirstNode(imageInformation, "rangePixelSpacing").toFloat64();

            theAzimuthSpacingTotal += getOptionalTextFromFirstNode(imageInformation, "azimuthPixelSpacing").toFloat64();

            theProductKwl.add(bandPrefix,
                    ossimKeywordNames::NUMBER_SAMPLES_KW,
                    getOptionalTextFromFirstNode(imageInformation, "numberOfSamples"),
                    true);

            theProductKwl.add(bandPrefix,
                    ossimKeywordNames::NUMBER_LINES_KW,
                    getOptionalTextFromFirstNode(imageInformation, "numberOfLines"),
                    true);

            theProductKwl.add(bandPrefix,
                    "sample_type", //ossimKeywordNames::PIXEL_TYPE_KW,
                    getOptionalTextFromFirstNode(imageInformation, "pixelValue").upcase(),
                    true);

            heightSum += getBandTerrainHeight(theProductXmlDocument);


            if (!commonMetadataRetrieved)
            {
                // these should be the same for all swaths
                //RK div by oneMillion taken from S1tlbx
                theProductKwl.add(SUPPORT_DATA_PREFIX,
                        "range_sampling_rate",
                        getOptionalTextFromFirstNode(productInformation, "rangeSamplingRate"),
                        true);

                theProductKwl.add(SUPPORT_DATA_PREFIX,
                        "radar_frequency",
                        getOptionalTextFromFirstNode(productInformation, "radarFrequency"),
                        true);

                theProductKwl.add(SUPPORT_DATA_PREFIX,
                        "line_time_interval",
                        getOptionalTextFromFirstNode(imageInformation, "azimuthTimeInterval"),
                        true);

                theProductKwl.add(SUPPORT_DATA_PREFIX,
                        "slant_range_to_first_pixel",
                        getOptionalTextFromFirstNode(imageInformation, "slantRangeTime"),
                        true);

                const ossimXmlNode & downlinkInformation =
                    getExpectedFirstNode(*productRoot, "generalAnnotation/downlinkInformationList/downlinkInformation");

                theProductKwl.add(SUPPORT_DATA_PREFIX,
                        "pulse_repetition_frequency",
                        getOptionalTextFromFirstNode(downlinkInformation, "prf"),
                        true);

                ossimXmlNode const& swathProcParams =
                    getExpectedFirstNode(*productRoot, "imageAnnotation/processingInformation/swathProcParamsList/swathProcParams");
                ossimXmlNode const& rangeProcessingNode = getExpectedFirstNode(swathProcParams, "rangeProcessing");
                ossimXmlNode const& azimuthProcessingNode = getExpectedFirstNode(swathProcParams, "azimuthProcessing");

                theProductKwl.add(SUPPORT_DATA_PREFIX,
                        "azimuth_bandwidth",
                        getOptionalTextFromFirstNode(azimuthProcessingNode, "processingBandwidth"),
                        true);

                theProductKwl.add(SUPPORT_DATA_PREFIX,
                        "range_bandwidth",
                        getOptionalTextFromFirstNode(rangeProcessingNode, "processingBandwidth"),
                        true);

                theProductKwl.add(SUPPORT_DATA_PREFIX,
                        "range_looks",
                        getOptionalTextFromFirstNode(rangeProcessingNode, "numberOfLooks"),
                        true);

                theProductKwl.add(SUPPORT_DATA_PREFIX,
                        "azimuth_looks",
                        getOptionalTextFromFirstNode(azimuthProcessingNode, "numberOfLooks"),
                        true);

                if(!theTOPSAR || !theSLC)
                {
                    theProductKwl.add(SUPPORT_DATA_PREFIX,
                            ossimKeywordNames::NUMBER_SAMPLES_KW,
                            getOptionalTextFromFirstNode(imageInformation, "numberOfSamples"),
                            true);

                    theProductKwl.add(SUPPORT_DATA_PREFIX,
                            ossimKeywordNames::NUMBER_LINES_KW,
                            getOptionalTextFromFirstNode(imageInformation, "numberOfLines"),
                            true);
                }

                ossimXmlNodePtr const& orbitList = productRoot->findFirstNode("generalAnnotation/orbitList");
                if (!orbitList) {
                    ossimNotify(ossimNotifyLevel_DEBUG) << "No orbitVectorList info available in metadata!!\n";
                } else {
                    addOrbitStateVectors(*orbitList);
                }

                ossimXmlNodePtr const& coordinateConversionList = productRoot->findFirstNode("coordinateConversion/coordinateConversionList");
                if (!coordinateConversionList) {
                    ossimNotify(ossimNotifyLevel_DEBUG) << "No coordinate conversion info available in metadata!!\n";
                } else {
                    addSRGRCoefficients(SUPPORT_DATA_PREFIX, *coordinateConversionList);
                }

                ossimXmlNodePtr const& dcEstimateList = productRoot->findFirstNode("dopplerCentroid/dcEstimateList");
                if (!dcEstimateList) {
                    ossimNotify(ossimNotifyLevel_DEBUG) << "No doppler centroid coefficients available in metadata!!\n";
                } else {
                    addDopplerCentroidCoefficients(*dcEstimateList);
                }

                commonMetadataRetrieved = true;
            }

            ++numBands;
        }

        if(theSLC)
        {
            numBands *= 2; // real and imaginary
        }


        theProductKwl.add(SUPPORT_DATA_PREFIX,
                "range_spacing",
                theRangeSpacingTotal / (double)numBands,
                true);

        theProductKwl.add(SUPPORT_DATA_PREFIX,
                "azimuth_spacing",
                theAzimuthSpacingTotal / (double)numBands,
                true);

        theProductKwl.add(SUPPORT_DATA_PREFIX,
                "avg_scene_height",
                heightSum / (double)files.size(),
                true);

        theProductKwl.add(SUPPORT_DATA_PREFIX,
                ossimKeywordNames::NUMBER_BANDS_KW,
                numBands,
                true);
        return true;
    }

    void ossimSentinel1ProductDoc::addDopplerCentroidCoefficients(ossimXmlNode const& dcEstimateList)
    {
        ossimString count_str;
        dcEstimateList.getAttributeValue(count_str, "count");
        const int count  = count_str.toInt();
        if( count < 1)
        {
            ossimNotify(ossimNotifyLevel_DEBUG) << "No doppler centroid coefficients available in metadata!!\n";
            return;
        }
        else
        {
            ossimXmlNode::ChildListType dcEstimates;
            dcEstimateList.findChildNodes("dcEstimate", dcEstimates);

            ossimXmlNode::ChildListType::const_iterator it = dcEstimates.begin();

            for (int index = 1 ; it != dcEstimates.end() ; ++it, ++index) 
            {
                char prefix[256];
                //Doppler_Centroid_Coefficients.dop_coef_list;
                std::snprintf(prefix, sizeof(prefix), "dopplerCentroid.dop_coef_list%d.", index);

                const ossimXmlNodePtr & dcEstimate = *it;
                assert(dcEstimate);
                theProductKwl.add(prefix,
                        "dop_coef_time",
                        getOptionalTextFromFirstNode(*dcEstimate, "azimuthTime"),
                        true);
                //RK
                const double ref_time = getOptionalTextFromFirstNode(*dcEstimate, "t0").toFloat64() * 1e9; // s to ns

                theProductKwl.add(prefix,
                        "slant_range_time",
                        ref_time,
                        true);

                ossimString const& ns = getOptionalTextFromFirstNode(*dcEstimate, "ns");

                if( !ns.empty() )
                    theProductKwl.add(prefix,
                            "slant_range_time",
                            ns,
                            true);

                ossimString const& coeffStr = getOptionalTextFromFirstNode(*dcEstimate, "geometryDcPolynomial");

                if (!coeffStr.empty())
                {
                    const ossimString separatorList = " ";
                    std::vector<ossimString> result;

                    coeffStr.split(result, separatorList, true);

                    std::vector<ossimString>::const_iterator coeff = result.begin();

                    for (int count = 1 ; coeff != result.end() ; ++count, ++coeff)
                    {
                        char coeff_prefix[256];
                        std::snprintf(coeff_prefix, sizeof(coeff_prefix), "%s%d.", prefix, count);

                        theProductKwl.add(coeff_prefix,
                                "dop_coef",
                                *coeff,
                                true);
                    }

                } //if (!coeffStr.empty())

            } // for each dcEstimate

        } // else count < 1
    }


    double ossimSentinel1ProductDoc::getBandTerrainHeight(ossimXmlDocument const& theProductXmlDocument)
    {
        double heightSum = 0.0;
        vector< ossimXmlNodePtr > heightList;
        theProductXmlDocument.findNodes("/product/generalAnnotation/terrainHeightList/terrainHeight", heightList);
        vector<ossimXmlNodePtr >::const_iterator it = heightList.begin();
        for ( ; it != heightList.end() ; ++it)
        {
            heightSum += getOptionalTextFromFirstNode(**it, "value").toFloat64();
        }
        return heightSum / heightList.size();
    }

    bool ossimSentinel1ProductDoc::initImageSize(ossimIpt& imageSize) const
    {
        const ossimString samples_str = theProductKwl.find(SUPPORT_DATA_PREFIX, ossimKeywordNames::NUMBER_SAMPLES_KW);
        const ossimString lines_str = theProductKwl.find(SUPPORT_DATA_PREFIX, ossimKeywordNames::NUMBER_LINES_KW);

        imageSize.samp =  samples_str.toInt();
        imageSize.line =  lines_str.toInt();

        return true;
    }

    bool ossimSentinel1ProductDoc::initGsd(ossimDpt& gsd) const
    {
        gsd.x =  theRangeSpacingTotal;
        gsd.y =  theAzimuthSpacingTotal;
        return true;
    }

    void ossimSentinel1ProductDoc::addOrbitStateVectors(ossimXmlNode const& orbitList)
    {
        ossimXmlNode::ChildListType stateVectorList;
        orbitList.findChildNodes("orbit", stateVectorList);

        if(stateVectorList.empty())
        {
            ossimNotify(ossimNotifyLevel_DEBUG) << "No orbitVectorList info available in metadata!!\n";
            return;
        }

        std::size_t stateVectorList_size = stateVectorList.size();
        for (std::size_t i = 0; i != stateVectorList_size ; ++i)
        {
            //orbit_state_vectors
            char orbit_prefix[256];
            std::snprintf(orbit_prefix, sizeof(orbit_prefix), "orbitList.orbit[%d].", int(i));

            theProductKwl.add(orbit_prefix,
                    "time",
                    getOptionalTextFromFirstNode(*stateVectorList[i], "time"),
                    true);

            theProductKwl.add(orbit_prefix,
                    "x_pos",
                    getOptionalTextFromFirstNode(*stateVectorList[i], "position/x").toFloat64(),
                    true);

            theProductKwl.add(orbit_prefix,
                    "y_pos",
                    getOptionalTextFromFirstNode(*stateVectorList[i], "position/y").toFloat64(),
                    true);

            theProductKwl.add(orbit_prefix,
                    "z_pos",
                    getOptionalTextFromFirstNode(*stateVectorList[i], "position/z").toFloat64(),
                    true);

            theProductKwl.add(orbit_prefix,
                    "x_vel",
                    getOptionalTextFromFirstNode(*stateVectorList[i], "velocity/x").toFloat64(),
                    true);

            theProductKwl.add(orbit_prefix,
                    "y_vel",
                    getOptionalTextFromFirstNode(*stateVectorList[i], "velocity/y").toFloat64(),
                    true);

            theProductKwl.add(orbit_prefix,
                    "z_vel",
                    getOptionalTextFromFirstNode(*stateVectorList[i], "velocity/z").toFloat64(),
                    true);
        }
    }

    void ossimSentinel1ProductDoc::addSRGRCoefficients(const char* prefix, ossimXmlNode const& coordinateConversionList)
    {
        const ossimString count_str = coordinateConversionList.getAttributeValue("count");
        const int count  = count_str.toInt();
        if( count < 1)
        {
            if(traceDebug())
                ossimNotify(ossimNotifyLevel_DEBUG) << "No coordinate conversion info available in metadata!!\n";
            return;
        }
        else
        {
            ossimNotify(ossimNotifyLevel_FATAL) << "shouldn't reach here!!\n";
            return;
        }
    }

    void ossimSentinel1ProductDoc::readCalibrationMetadata()
    {
        ossimDirectory calibrationDir( theManifestDirectory.dirCat( "annotation/calibration") );
        std::vector<ossimFilename> files;
        calibrationDir.findAllFilesThatMatch(files, "calibration*");
        std::vector<ossimFilename>::const_iterator it = files.begin();

        std::stringstream strm;
        for (; it != files.end(); ++it)
        {
            ossimXmlDocument calibrationDoc;
            openMetadataFile(calibrationDoc, *it );

            const ossimXmlNodePtr & calibrationRoot = calibrationDoc.getRoot();
            assert(calibrationRoot);
            ossimXmlNode const& adsHeader = getExpectedFirstNode(*calibrationRoot, "adsHeader");
            ossimXmlNode const& calibrationInformation = getExpectedFirstNode(*calibrationRoot, "calibrationInformation");
            ossimXmlNode const& calibrationVectorList  = getExpectedFirstNode(*calibrationRoot, "calibrationVectorList");

            char const calibrationPrefix[] = "calibration.";

            theProductKwl.add(calibrationPrefix,
                    "swath",
                    getOptionalTextFromFirstNode(adsHeader, "swath"),
                    true);

            theProductKwl.add(calibrationPrefix,
                    "polarisation",
                    getOptionalTextFromFirstNode(adsHeader, "polarisation"),
                    true);

            theProductKwl.add(calibrationPrefix,
                    "startTime",
                    getModifiedJulianDate(getOptionalTextFromFirstNode(adsHeader, "startTime")),
                    true);

            theProductKwl.add(calibrationPrefix,
                    "stopTime",
                    getModifiedJulianDate(getOptionalTextFromFirstNode(adsHeader, "stopTime")),
                    true);

            theProductKwl.add(calibrationPrefix,
                    "absoluteCalibrationConstant",
                    getOptionalTextFromFirstNode(calibrationInformation, "absoluteCalibrationConstant"),
                    true);

            theProductKwl.add(calibrationPrefix,
                    "count",
                    calibrationVectorList.getAttributeValue("count"),
                    true);

            std::vector< ossimXmlNodePtr > calibrationVectors;
            calibrationRoot->findChildNodes("calibrationVectorList/calibrationVector", calibrationVectors);
            int idx = 0;
            for (std::vector< ossimXmlNodePtr >::const_iterator b_calibVector = calibrationVectors.begin(), e_calibVector = calibrationVectors.end()
                    ; b_calibVector != e_calibVector
                    ; ++b_calibVector, ++idx
                )
            {
                char calibrationVectorPrefix[256];
                std::snprintf(calibrationVectorPrefix, sizeof(calibrationVectorPrefix), "%scalibrationVector[%d].", calibrationPrefix, idx);
                assert(*b_calibVector);

                ossimXmlNode const& calibrationVector = **b_calibVector;
                const ossimXmlNodePtr & node = calibrationVector.findFirstNode("pixel");

                theProductKwl.add(calibrationVectorPrefix,
                        "pixel_count",
                        node->getAttributeValue("count"),
                        false);

                theProductKwl.add(calibrationVectorPrefix,
                        "azimuthTime",
                        getModifiedJulianDate(getOptionalTextFromFirstNode(calibrationVector, "azimuthTime")),
                        true);

                theProductKwl.add(calibrationVectorPrefix,
                        "line",
                        getOptionalTextFromFirstNode(calibrationVector, "line"),
                        true);

                theProductKwl.add(calibrationVectorPrefix,
                        "pixel",
                        getOptionalTextFromFirstNode(calibrationVector, "pixel"),
                        true);

                theProductKwl.add(calibrationVectorPrefix,
                        "sigmaNought",
                        getOptionalTextFromFirstNode(calibrationVector, "sigmaNought"),
                        true);

                theProductKwl.add(calibrationVectorPrefix,
                        "betaNought",
                        getOptionalTextFromFirstNode(calibrationVector, "betaNought"),
                        true);

                theProductKwl.add(calibrationVectorPrefix,
                        "gamma",
                        getOptionalTextFromFirstNode(calibrationVector, "gamma"),
                        true);

                theProductKwl.add(calibrationVectorPrefix,
                        "dn",
                        getOptionalTextFromFirstNode(calibrationVector, "dn"),
                        true);

                //  calibrationVectors[idx]->toKwl(theProductKwl, "calibrationVectorList_" + ossimString::toString(idx+1) + ".");
            }
        }
    }

    void ossimSentinel1ProductDoc::readNoiseMetadata()
    {
        ossimDirectory calibrationDir( theManifestDirectory.dirCat( "annotation/calibration") );
        std::vector<ossimFilename> files;
        calibrationDir.findAllFilesThatMatch(files, "noise*");
        std::vector<ossimFilename>::const_iterator it = files.begin();
        const char noisePrefix[] = "noise.";

        for (; it != files.end(); ++it)
        {
            ossimXmlDocument noiseDoc;
            openMetadataFile(noiseDoc, *it );

            const ossimXmlNodePtr noiseRoot = noiseDoc.getRoot();
            std::vector< ossimXmlNodePtr > noiseVectors;
            noiseRoot->findChildNodes("noiseVectorList/noiseVector", noiseVectors);
            int idx = 0;
            for (std::vector< ossimXmlNodePtr >::const_iterator b_noiseVector = noiseVectors.begin(), e_noiseVector = noiseVectors.end()
                    ; b_noiseVector != e_noiseVector
                    ; ++b_noiseVector, ++idx
                )
            {
                char noiseVectorPrefix [256];
                std::snprintf(noiseVectorPrefix, sizeof(noiseVectorPrefix), "%snoiseVector[%d].", noisePrefix, idx);
                assert(*b_noiseVector);

                ossimXmlNode const& noiseVector = **b_noiseVector;

                const ossimXmlNodePtr & node = noiseVector.findFirstNode("pixel");

                theProductKwl.add(noiseVectorPrefix,
                        "pixel_count",
                        node->getAttributeValue("count"),
                        false);

                theProductKwl.add(noiseVectorPrefix,
                        "azimuthTime",
                        getModifiedJulianDate(getOptionalTextFromFirstNode(noiseVector, "azimuthTime")),
                        true);

                theProductKwl.add(noiseVectorPrefix,
                        "line",
                        getOptionalTextFromFirstNode(noiseVector, "line"),
                        true);

                theProductKwl.add(noiseVectorPrefix,
                        "pixel",
                        getOptionalTextFromFirstNode(noiseVector, "pixel"),
                        true);

                theProductKwl.add(noiseVectorPrefix,
                        "noiseLut",
                        getOptionalTextFromFirstNode(noiseVector, "noiseLut"),
                        true);

                //noiseVectorList[idx]->toKwl(theProductKwl, "noiseVectorList_" + ossimString::toString(idx+1) + ".");
            }
        }
    }


    void ossimSentinel1ProductDoc::clearFields( )
    {
        theProductKwl.clear();
    }
    //RK
    // bool ossimSentinel1ProductDoc::saveState(ossimKeywordlist& kwl,
    //                                               const char* prefix)const
    // {
    //    kwl.addList(theProductKwl);

    //    kwl.add(prefix,
    //            "sample_type",
    //            theSampleType,
    //            true);

    //    return true;
    // }

    // bool ossimSentinel1ProductDoc::loadState(const ossimKeywordlist& kwl,
    //                                               const char* prefix)
    // {

    //    return true;

    // }
}
