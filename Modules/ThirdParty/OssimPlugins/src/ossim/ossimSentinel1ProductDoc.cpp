#include "ossimSentinel1ProductDoc.h"
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimDate.h>

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

   bool ossimSentinel1ProductDoc::openMetadataFile(ossimRefPtr<ossimXmlDocument>& doc, const ossimString file) const
   {
      if ( !doc->openFile( file ) )
      {
         std::cerr << "ossimSentinel1ProductDoc::openMetadataFile() failed." << std::endl;
         return false;
      }

      return true;
   }


   double
   ossimSentinel1ProductDoc
   ::getModifiedJulianDate(const ossimString utcTimeString)
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



      for (int count=0; it != files.end(); ++it,  ++count)
      {
         ossimRefPtr<ossimXmlDocument> theProductXmlDocument = new ossimXmlDocument();
         openMetadataFile(theProductXmlDocument, (*it) );

         const ossimRefPtr<ossimXmlNode> adsHeader = theProductXmlDocument->getRoot()->findFirstNode("adsHeader");

         const ossimString swath = adsHeader->getChildTextValue("swath");
         const ossimString polarisation = adsHeader->getChildTextValue("polarisation");

         std::stringstream _tempstrm;
         _tempstrm << "Band[" << count << "].";
         const ossimString bandPrefix = _tempstrm.str();

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
                         adsHeader->getChildTextValue("startTime"),
                         true);

         theProductKwl.add(bandPrefix,
                         "last_line_time",
                         adsHeader->getChildTextValue("stopTime"),
                         true);
         //RK maybe use this->getManifestPrefix()
         theProductKwl.add(SUPPORT_DATA_PREFIX,
                      "mds1_tx_rx_polar",
                      polarisation,
                      true);

         const ossimRefPtr<ossimXmlNode> imageInformation = theProductXmlDocument->getRoot()->findFirstNode("imageAnnotation/imageInformation");

         const ossimRefPtr<ossimXmlNode> productInformation = theProductXmlDocument->getRoot()->findFirstNode("generalAnnotation/productInformation");

         theProductKwl.add(SUPPORT_DATA_PREFIX,
                         "data_take_id",
                         adsHeader->getChildTextValue("missionDataTakeId"),
                         true);

         theProductKwl.add(SUPPORT_DATA_PREFIX,
                         "slice_num",
                         imageInformation->getChildTextValue("sliceNumber"),
                         true);

         theProductKwl.add(SUPPORT_DATA_PREFIX,
                         "line_time_interval",
                         imageInformation->getChildTextValue("azimuthTimeInterval"),
                         true);

         theRangeSpacingTotal += imageInformation->getChildTextValue("rangePixelSpacing").toFloat64();

         theAzimuthSpacingTotal += imageInformation->getChildTextValue("azimuthPixelSpacing").toFloat64();

         theProductKwl.add(bandPrefix,
                         ossimKeywordNames::NUMBER_SAMPLES_KW,
                         imageInformation->getChildTextValue("numberOfSamples"),
                         true);

         theProductKwl.add(bandPrefix,
                         ossimKeywordNames::NUMBER_LINES_KW,
                         imageInformation->getChildTextValue("numberOfLines"),
                         true);

         theProductKwl.add(bandPrefix,
                         "sample_type", //ossimKeywordNames::PIXEL_TYPE_KW,
                         imageInformation->getChildTextValue("pixelValue").upcase(),
                         true);

         heightSum += getBandTerrainHeight(theProductXmlDocument);


         if (!commonMetadataRetrieved)
         {
            // these should be the same for all swaths
            //RK div by oneMillion taken from S1tlbx
            theProductKwl.add(SUPPORT_DATA_PREFIX,
                              "range_sampling_rate",
                              productInformation->getChildTextValue("rangeSamplingRate"),
                              true);

            theProductKwl.add(SUPPORT_DATA_PREFIX,
                              "radar_frequency",
                              productInformation->getChildTextValue("radarFrequency"),
                              true);

            theProductKwl.add(SUPPORT_DATA_PREFIX,
                            "line_time_interval",
                            imageInformation->getChildTextValue("azimuthTimeInterval"),
                            true);

            theProductKwl.add(SUPPORT_DATA_PREFIX,
                              "slant_range_to_first_pixel",
                              imageInformation->getChildTextValue("slantRangeTime"),
                              true);

            const ossimRefPtr<ossimXmlNode> downlinkInformation =
               theProductXmlDocument->getRoot()->findFirstNode("generalAnnotation/downlinkInformationList/downlinkInformation");

            theProductKwl.add(SUPPORT_DATA_PREFIX,
                            "pulse_repetition_frequency",
                            downlinkInformation->getChildTextValue("prf"),
                            true);

            const ossimRefPtr<ossimXmlNode> swathProcParams =
               theProductXmlDocument->getRoot()->findFirstNode("imageAnnotation/processingInformation/swathProcParamsList/swathProcParams");
            const ossimRefPtr<ossimXmlNode> rangeProcessingNode = swathProcParams->findFirstNode("rangeProcessing");
            const ossimRefPtr<ossimXmlNode> azimuthProcessingNode = swathProcParams->findFirstNode("azimuthProcessing");

            theProductKwl.add(SUPPORT_DATA_PREFIX,
                            "azimuth_bandwidth",
                              azimuthProcessingNode->getChildTextValue("processingBandwidth"),
                            true);

            theProductKwl.add(SUPPORT_DATA_PREFIX,
                            "range_bandwidth",
                              rangeProcessingNode->getChildTextValue("processingBandwidth"),
                            true);

            theProductKwl.add(SUPPORT_DATA_PREFIX,
                            "range_looks",
                            rangeProcessingNode->getChildTextValue("numberOfLooks"),
                            true);

            theProductKwl.add(SUPPORT_DATA_PREFIX,
                            "azimuth_looks",
                            azimuthProcessingNode->getChildTextValue("numberOfLooks"),
                            true);

            if(!theTOPSAR || !theSLC)
            {
               theProductKwl.add(SUPPORT_DATA_PREFIX,
                               ossimKeywordNames::NUMBER_SAMPLES_KW,
                               imageInformation->getChildTextValue("numberOfSamples"),
                               true);

               theProductKwl.add(SUPPORT_DATA_PREFIX,
                               ossimKeywordNames::NUMBER_LINES_KW,
                               imageInformation->getChildTextValue("numberOfLines"),
                            true);
            }

            const ossimRefPtr<ossimXmlNode> orbitList = theProductXmlDocument->getRoot()->findFirstNode("generalAnnotation/orbitList");
            addOrbitStateVectors(orbitList);

            const ossimRefPtr<ossimXmlNode> coordinateConversionList = theProductXmlDocument->getRoot()->findFirstNode("coordinateConversion/coordinateConversionList");
            addSRGRCoefficients(SUPPORT_DATA_PREFIX, coordinateConversionList);

            const ossimRefPtr<ossimXmlNode> dcEstimateList = theProductXmlDocument->getRoot()->findFirstNode("dopplerCentroid/dcEstimateList");
            addDopplerCentroidCoefficients(dcEstimateList);

            commonMetadataRetrieved = true;
         }

         ++numBands;
      }

      if(theSLC)
      {
         numBands = numBands * 2; // real and imaginary
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

   void ossimSentinel1ProductDoc::addDopplerCentroidCoefficients(const ossimRefPtr<ossimXmlNode>& dcEstimateList)
   {
      if (!dcEstimateList.get())
         return;

      ossimString count_str;
      dcEstimateList->getAttributeValue(count_str, "count");
      const int count  = count_str.toInt();
      if( count < 1)
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "No doppler centroid coefficients available in metadata!!\n";
         return;
      }
      else
      {
         ossimXmlNode::ChildListType dcEstimates;
         dcEstimateList->findChildNodes("dcEstimate", dcEstimates);

         ossimXmlNode::ChildListType::const_iterator it = dcEstimates.begin();

         int index = 1;

         while( it != dcEstimates.end())
         {
            //Doppler_Centroid_Coefficients.dop_coef_list;
            const ossimString prefix = "dopplerCentroid.dop_coef_list" + ossimString::toString(index) + ".";

            ++index;

            const ossimRefPtr<ossimXmlNode> dcEstimate = (*it);
            theProductKwl.add(prefix,
                            "dop_coef_time",
                            dcEstimate->getChildTextValue("azimuthTime"),
                            true);
            //RK
            const double ref_time = dcEstimate->getChildTextValue("t0").toFloat64() * 1e9; // s to ns

            theProductKwl.add(prefix,
                            "slant_range_time",
                            ref_time,
                            true);

            const ossimString ns = dcEstimate->getChildTextValue("ns");

            if( !ns.empty() )
               theProductKwl.add(prefix,
                               "slant_range_time",
                               ns,
                               true);

            const ossimString coeffStr = dcEstimate->getChildTextValue("geometryDcPolynomial");

            if (!coeffStr.empty())
            {
               const ossimString separatorList = " ";
               std::vector<ossimString> result;

               coeffStr.split(result, separatorList, true);

               std::vector<ossimString>::const_iterator coeff = result.begin();

               int count = 1;

               while (coeff != result.end())
               {
                  const ossimString coeff_prefix =  prefix + ossimString::toString(count) + ".";

                  theProductKwl.add(coeff_prefix,
                                  "dop_coef",
                                  (*coeff),
                                  true);

                  ++coeff;

                  ++count;
               }

            } //if (!coeffStr.empty())

            ++it;

         } // while( it != dcEstimates.end())

      } // else count < 1
    }


   double ossimSentinel1ProductDoc::getBandTerrainHeight( const ossimRefPtr<ossimXmlDocument>& theProductXmlDocument )
   {
      double heightSum = 0.0;
      vector< ossimRefPtr<ossimXmlNode> > heightList;
      theProductXmlDocument->findNodes("/product/generalAnnotation/terrainHeightList/terrainHeight", heightList);
      vector<ossimRefPtr<ossimXmlNode> >::const_iterator it = heightList.begin();
      while(it != heightList.end())
      {
         heightSum += (*it)->getChildTextValue("value").toFloat64();
         ++it;
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

   bool ossimSentinel1ProductDoc::initGsd( ossimDpt& gsd) const
   {
      gsd.x =  theRangeSpacingTotal;
      gsd.y =  theAzimuthSpacingTotal;
      return true;
   }

   void ossimSentinel1ProductDoc::addOrbitStateVectors(const ossimRefPtr<ossimXmlNode>& orbitList)
   {

      if (!orbitList.get())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "No orbitVectorList info available in metadata!!\n";
         return;
      }

      ossimXmlNode::ChildListType stateVectorList;
      orbitList->findChildNodes("orbit", stateVectorList);

      if(stateVectorList.size() < 1)
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "No orbitVectorList info available in metadata!!\n";
         return;
      }

      for (int i = 0; i < stateVectorList.size(); ++i)
      {
         //orbit_state_vectors
         const ossimString orbit_prefix = "orbitList.orbit[" + ossimString::toString(i)  + "].";

         theProductKwl.add(orbit_prefix,
                         "time",
                         stateVectorList[i]->getChildTextValue("time"),
                         true);

         theProductKwl.add(orbit_prefix,
                         "x_pos",
                         stateVectorList[i]->getChildTextValue("position/x").toFloat64(),
                         true);

         theProductKwl.add(orbit_prefix,
                         "y_pos",
                         stateVectorList[i]->getChildTextValue("position/y").toFloat64(),
                         true);

         theProductKwl.add(orbit_prefix,
                         "z_pos",
                         stateVectorList[i]->getChildTextValue("position/z").toFloat64(),
                         true);

         theProductKwl.add(orbit_prefix,
                         "x_vel",
                         stateVectorList[i]->getChildTextValue("velocity/x").toFloat64(),
                         true);

         theProductKwl.add(orbit_prefix,
                         "y_vel",
                         stateVectorList[i]->getChildTextValue("velocity/y").toFloat64(),
                         true);

         theProductKwl.add(orbit_prefix,
                         "z_vel",
                         stateVectorList[i]->getChildTextValue("velocity/z").toFloat64(),
                         true);
      }
   }

   void ossimSentinel1ProductDoc::addSRGRCoefficients(const char* prefix, const ossimRefPtr<ossimXmlNode>& coordinateConversionList)
   {
      if (!coordinateConversionList.get())
         return;

      ossimString count_str;
      coordinateConversionList->getAttributeValue(count_str, "count");
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
         ossimRefPtr<ossimXmlDocument> calibrationDoc = new ossimXmlDocument();
         openMetadataFile(calibrationDoc, (*it) );

         ossimRefPtr<ossimXmlNode> adsHeader = calibrationDoc->getRoot()->findFirstNode("adsHeader");
         ossimRefPtr<ossimXmlNode> calibrationInformation = calibrationDoc->getRoot()->findFirstNode("calibrationInformation");
         ossimRefPtr<ossimXmlNode> calibrationVectorList = calibrationDoc->getRoot()->findFirstNode("calibrationVectorList");

         const ossimString calibrationPrefix = "calibration.";

         theProductKwl.add(calibrationPrefix,
                         "swath",
                         adsHeader->getChildTextValue("swath"),
                         true);

         theProductKwl.add(calibrationPrefix,
                         "polarisation",
                         adsHeader->getChildTextValue("polarisation"),
                         true);

         theProductKwl.add(calibrationPrefix,
                         "startTime",
                         getModifiedJulianDate(adsHeader->getChildTextValue("startTime")),
                         true);

         theProductKwl.add(calibrationPrefix,
                         "stopTime",
                         getModifiedJulianDate(adsHeader->getChildTextValue("stopTime")),
                         true);

         theProductKwl.add(calibrationPrefix,
                         "absoluteCalibrationConstant",
                         calibrationInformation->getChildTextValue("absoluteCalibrationConstant"),
                         true);

         theProductKwl.add(calibrationPrefix,
                         "count",
                         calibrationVectorList->getAttributeValue("count"),
                         true);

         const ossimRefPtr<ossimXmlNode> calibrationRoot = calibrationDoc->getRoot();
         std::vector< ossimRefPtr<ossimXmlNode> > calibrationVectors;
         calibrationRoot->findChildNodes("calibrationVectorList/calibrationVector", calibrationVectors);
         for( unsigned int idx = 0; idx < calibrationVectors.size(); ++idx)
         {

         const ossimString calibrationVectorPrefix = calibrationPrefix + "calibrationVector[" + ossimString::toString(idx) + "].";

         const ossimRefPtr<ossimXmlNode> node = calibrationVectors[idx]->findFirstNode("pixel");

         theProductKwl.add(calibrationVectorPrefix,
                         "pixel_count",
                         node->getAttributeValue("count"),
                         false);

         theProductKwl.add(calibrationVectorPrefix,
                         "azimuthTime",
                         getModifiedJulianDate(calibrationVectors[idx]->getChildTextValue("azimuthTime")),
                         true);

         theProductKwl.add(calibrationVectorPrefix,
                         "line",
                         calibrationVectors[idx]->getChildTextValue("line"),
                         true);

         theProductKwl.add(calibrationVectorPrefix,
                         "pixel",
                         calibrationVectors[idx]->getChildTextValue("pixel"),
                         true);

         theProductKwl.add(calibrationVectorPrefix,
                         "sigmaNought",
                         calibrationVectors[idx]->getChildTextValue("sigmaNought"),
                         true);

         theProductKwl.add(calibrationVectorPrefix,
                         "betaNought",
                         calibrationVectors[idx]->getChildTextValue("betaNought"),
                         true);

         theProductKwl.add(calibrationVectorPrefix,
                         "gamma",
                         calibrationVectors[idx]->getChildTextValue("gamma"),
                         true);

         theProductKwl.add(calibrationVectorPrefix,
                         "dn",
                         calibrationVectors[idx]->getChildTextValue("dn"),
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
      const ossimString noisePrefix = "noise.";

      for (; it != files.end(); ++it)
      {
         ossimRefPtr<ossimXmlDocument> noiseDoc = new ossimXmlDocument();
         openMetadataFile(noiseDoc, (*it) );

         const ossimRefPtr<ossimXmlNode> noiseRoot = noiseDoc->getRoot();
         std::vector< ossimRefPtr<ossimXmlNode> > noiseVectors;
         noiseRoot->findChildNodes("noiseVectorList/noiseVector", noiseVectors);
         for( unsigned int idx = 0; idx < noiseVectors.size(); ++idx)
         {
            const ossimString noiseVectorPrefix = noisePrefix + "noiseVector[" + ossimString::toString(idx) + "].";

            const ossimRefPtr<ossimXmlNode> node = noiseVectors[idx]->findFirstNode("pixel");

            theProductKwl.add(noiseVectorPrefix,
                              "pixel_count",
                              node->getAttributeValue("count"),
                              false);

            theProductKwl.add(noiseVectorPrefix,
                              "azimuthTime",
                              getModifiedJulianDate(noiseVectors[idx]->getChildTextValue("azimuthTime")),
                              true);

            theProductKwl.add(noiseVectorPrefix,
                              "line",
                              noiseVectors[idx]->getChildTextValue("line"),
                              true);

            theProductKwl.add(noiseVectorPrefix,
                              "pixel",
                              noiseVectors[idx]->getChildTextValue("pixel"),
                              true);

            theProductKwl.add(noiseVectorPrefix,
                              "noiseLut",
                              noiseVectors[idx]->getChildTextValue("noiseLut"),
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
