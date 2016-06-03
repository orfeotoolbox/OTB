//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$
#if 0
#include "ossimSentinel1ProductDoc.h"
#include <cstdio>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimDate.h>
#include "ossimXmlTools.h"
#include "ossimTimeUtilities.h"

// Define Trace flags for use within this file:

namespace {// Anonymous namespace
   ossimTrace traceExec  ("ossimSentinel1SupportData:exec");
   ossimTrace traceDebug ("ossimSentinel1SupportData:debug");
   const ossimString attAdsHeader        = "adsHeader";
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

   // const char LOAD_FROM_PRODUCT_FILE_KW[] = "load_from_product_file_flag";
   // const char PRODUCT_XML_FILE_KW[] = "product_xml_filename";
   const char SUPPORT_DATA_PREFIX[] = "support_data.";
   const char GEOM_PREFIX[]         = "support_data.geom";
   const char SRD_PREFIX[]          = "support_data.geom.srd";
   const char SR_PREFIX[]           = "support_data.geom.srd.coords.sr";
   const char GR_PREFIX[]           = "support_data.geom.srd.coords.gr";
   const char BURST_PREFIX[]        = "support_data.geom.bursts.burst";
   const char GCP_PREFIX[]          = "support_data.geom.gcp";
}// Anonymous namespace

namespace ossimplugins
{
   ossimSentinel1ProductDoc::ossimSentinel1ProductDoc ()
      : ossimErrorStatusInterface ()
      // , theRangeSpacingTotal (0.0)
      // , theAzimuthSpacingTotal (0.0)
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

   bool ossimSentinel1ProductDoc::read(ossimFilename const& annotationXml)
   {
      ossimXmlDocument productXmlDocument;
      if (! openMetadataFile(productXmlDocument, annotationXml)) {
         ossimNotify(ossimNotifyLevel_FATAL) << "Cannot open Sar Sentinel1 model from XML file: "<<annotationXml<< std::endl;

         return false;
      }

      const ossimXmlNodePtr & productRoot = productXmlDocument.getRoot();
      assert(productRoot.get());

      const ossimXmlNode & adsHeader = getExpectedFirstNode(*productRoot,attAdsHeader);
      const ossimString & polarisation = getTextFromFirstNode(adsHeader, "polarisation");
      const ossimString & productType  = getTextFromFirstNode(adsHeader, "productType");
      const bool          isGRD        = productType == "GRD";

      char const* headerPrefix = "header.";

      theProductKwl.add(headerPrefix, "product_type", productType);
      theProductKwl.add(headerPrefix, "is_GRD",       productType == "GRD"); // TODO: check how to store a bool

      addMandatory(theProductKwl, headerPrefix, "swath",        adsHeader, "swath");
      addMandatory(theProductKwl, headerPrefix, "polarisation", adsHeader, "polarisation");
      theProductKwl.add(headerPrefix, "polarisation", polarisation);
      theProductKwl.add(headerPrefix, "annotation",   annotationXml.file());

      addMandatory(theProductKwl, headerPrefix, "first_line_time", adsHeader, "startTime");
      addMandatory(theProductKwl, headerPrefix, "last_line_time",  adsHeader, "stopTime");

      //RK maybe use this->getManifestPrefix()

      theProductKwl.add(SUPPORT_DATA_PREFIX, "mds1_tx_rx_polar", polarisation, true);

      const ossimXmlNode & imageInformation   = getExpectedFirstNode(*productRoot, "imageAnnotation/imageInformation");
      const ossimXmlNode & productInformation = getExpectedFirstNode(*productRoot, "generalAnnotation/productInformation");

      addOptional (theProductKwl, SUPPORT_DATA_PREFIX, "data_take_id",       adsHeader,        "missionDataTakeId");
      addOptional (theProductKwl, SUPPORT_DATA_PREFIX, "slice_num",          imageInformation, "sliceNumber");

      ossimXmlNode const& xAzimtuTimeInterval = getExpectedFirstNode(imageInformation, "azimuthTimeInterval");
      theAzimuthTimeInterval = to<double>(xAzimtuTimeInterval.getText());
      add(theProductKwl, SUPPORT_DATA_PREFIX, "line_time_interval", xAzimtuTimeInterval.getText());
      // addMandatory(theProductKwl, SUPPORT_DATA_PREFIX, "line_time_interval", imageInformation, "azimuthTimeInterval");

      const double rangeSpacing = getDoubleFromFirstNode(imageInformation, "rangePixelSpacing");
      theProductKwl.add(SUPPORT_DATA_PREFIX, "range_spacing", rangeSpacing);

      const double azimuthSpacing = getDoubleFromFirstNode(imageInformation, "azimuthPixelSpacing");
      theProductKwl.add(SUPPORT_DATA_PREFIX, "azimuth_spacing", azimuthSpacing);

      addOptional(theProductKwl, ossimKeywordNames::NUMBER_SAMPLES_KW, imageInformation, "numberOfSamples");
      addOptional(theProductKwl, ossimKeywordNames::NUMBER_LINES_KW,   imageInformation, "numberOfLines");

      theProductKwl.add(
            "sample_type" /*ossimKeywordNames::PIXEL_TYPE_KW*/,
            getOptionalTextFromFirstNode(imageInformation, "pixelValue").upcase());

      // TODO: check if it makes sense when only one file is considered
      const double heightSum = getBandTerrainHeight(productXmlDocument);
      theProductKwl.add(SUPPORT_DATA_PREFIX, "avg_scene_height", heightSum);


      // these should be the same for all swaths
      //RK div by oneMillion taken from S1tlbx
      addOptional(theProductKwl, SUPPORT_DATA_PREFIX, "range_sampling_rate",        productInformation, "rangeSamplingRate");
      addOptional(theProductKwl, SUPPORT_DATA_PREFIX, "radar_frequency",            productInformation, "radarFrequency");
      addOptional(theProductKwl, SUPPORT_DATA_PREFIX, "line_time_interval",         imageInformation,   "azimuthTimeInterval");
      addOptional(theProductKwl, SUPPORT_DATA_PREFIX, "slant_range_to_first_pixel", imageInformation,   "slantRangeTime");

      const ossimXmlNode & downlinkInformation =
         getExpectedFirstNode(*productRoot, "generalAnnotation/downlinkInformationList/downlinkInformation");

      addOptional(theProductKwl, SUPPORT_DATA_PREFIX, "pulse_repetition_frequency", downlinkInformation, "prf");

      ossimXmlNode const& swathProcParams =
         getExpectedFirstNode(*productRoot, "imageAnnotation/processingInformation/swathProcParamsList/swathProcParams");
      ossimXmlNode const& rangeProcessingNode   = getExpectedFirstNode(swathProcParams, "rangeProcessing");
      ossimXmlNode const& azimuthProcessingNode = getExpectedFirstNode(swathProcParams, "azimuthProcessing");

      addOptional(theProductKwl, SUPPORT_DATA_PREFIX, "azimuth_bandwidth", azimuthProcessingNode, "processingBandwidth");
      addOptional(theProductKwl, SUPPORT_DATA_PREFIX, "range_bandwidth",   rangeProcessingNode,   "processingBandwidth");
      addOptional(theProductKwl, SUPPORT_DATA_PREFIX, "range_looks",       rangeProcessingNode,   "numberOfLooks");
      addOptional(theProductKwl, SUPPORT_DATA_PREFIX, "azimuth_looks",     azimuthProcessingNode, "numberOfLooks");

      if(!theTOPSAR || !theSLC)
      {
         addOptional(theProductKwl, SUPPORT_DATA_PREFIX, ossimKeywordNames::NUMBER_SAMPLES_KW, imageInformation, "numberOfSamples");
         addOptional(theProductKwl, SUPPORT_DATA_PREFIX, ossimKeywordNames::NUMBER_LINES_KW,   imageInformation, "numberOfLines");
      }

      ossimXmlNodePtr const& orbitList = productRoot->findFirstNode("generalAnnotation/orbitList");
      if (!orbitList) {
         ossimNotify(ossimNotifyLevel_DEBUG) << "No orbitVectorList info available in metadata!!\n";
      } else {
         addOrbitStateVectors(*orbitList);
      }

      if (isGRD) {
         ossimXmlNodePtr const& coordinateConversionList = productRoot->findFirstNode("coordinateConversion/coordinateConversionList/");
         if (!coordinateConversionList) {
            ossimNotify(ossimNotifyLevel_WARN) << "No coordinate conversion info available in metadata!!\n";
         } else {
            addSRGRCoefficients(*coordinateConversionList);
         }
      }

      ossimXmlNodePtr const& dcEstimateList = productRoot->findFirstNode("dopplerCentroid/dcEstimateList");
      if (!dcEstimateList) {
         ossimNotify(ossimNotifyLevel_DEBUG) << "No doppler centroid coefficients available in metadata!!\n";
      } else {
         addDopplerCentroidCoefficients(*dcEstimateList);
      }

      readBurstRecords(*productRoot, imageInformation);

      readGeoLocationGrid(*productRoot);

#if 0
      theProductKwl.add(SUPPORT_DATA_PREFIX,
            ossimKeywordNames::NUMBER_BANDS_KW,
            numBands,
            true);
#endif
      return true;
   }

   void ossimSentinel1ProductDoc::readBurstRecords(ossimXmlNode const& productRoot, ossimXmlNode const& imageInformation)
   {
      std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
      productRoot.findChildNodes("swathTiming/burstList/burst",xnodes);
      if (xnodes.empty())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "No burst records available in metadata!!\n";
         add(theProductKwl, BURST_PREFIX, "[0].start_line", 0);
         add(theProductKwl, BURST_PREFIX, "[0].azimuth_start_time", getModifiedJulianDateFromFirstNode(imageInformation,  "productFirstLineUtcTime"));
         add(theProductKwl, BURST_PREFIX, "[0].azimuth_stop_time",  getModifiedJulianDateFromFirstNode(imageInformation,  "productLastLineUtcTime"));
         add(theProductKwl, BURST_PREFIX, "[0].end_line",           getFromFirstNode<unsigned int>(imageInformation, "numberOfLines")-1);
      }
      else
      {
         char burstPrefix[1024];

         const unsigned int linesPerBurst = getFromFirstNode<unsigned int>(productRoot, "swathTiming/linesPerBurst");
         unsigned int burstId(0);

         for(std::vector<ossimRefPtr<ossimXmlNode> >::iterator itNode = xnodes.begin(); itNode!=xnodes.end();++itNode,++burstId)
         {
            const TimeType azTime = getModifiedJulianDateFromFirstNode(**itNode, attAzimuthTime);
            ossimString const& s = getTextFromFirstNode(**itNode, attFirstValidSample);

            ossim_int64 first_valid(0), last_valid(0);
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
                  }
                  else
                  {
                     ++last_valid;
                  }
               }
            }

            std::snprintf(burstPrefix, sizeof(burstPrefix), "%s[%d].", BURST_PREFIX, burstId);
            add(theProductKwl,burstPrefix, "start_line",         burstId*linesPerBurst + first_valid);
            add(theProductKwl,burstPrefix, "end_line",           burstId*linesPerBurst + last_valid);
            // TODO: check units.
            // using boost::posix_time::microseconds;
            using ossimplugins::time::microseconds;
            add(theProductKwl,burstPrefix, "azimuth_start_time", azTime + microseconds(first_valid*theAzimuthTimeInterval));
            add(theProductKwl,burstPrefix, "azimuth_stop_time",  azTime + microseconds(last_valid*theAzimuthTimeInterval));
         }
      }
   }

   void ossimSentinel1ProductDoc::readGeoLocationGrid(ossimXmlNode const& productRoot)
   {
      char prefix[1024];
      std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
      productRoot.findChildNodes("geolocationGrid/geolocationGridPointList/geolocationGridPoint", xnodes);

      unsigned int idx = 0;
      for(std::vector<ossimRefPtr<ossimXmlNode> >::iterator itNode = xnodes.begin(); itNode!=xnodes.end();++itNode,++idx)
      {
         int pos = std::snprintf(prefix, sizeof(prefix), "%s[%d].", GCP_PREFIX, idx);
         assert(pos >= sizeof(SR_PREFIX)+4 && pos < 1024);
         const TimeType azimuthTime = time::toModifiedJulianDate(
               addMandatory(theProductKwl, prefix, attAzimuthTime,     **itNode, attAzimuthTime)); // acquisition time
         addMandatory(theProductKwl, prefix, "slant_range_time", **itNode, attSlantRangeTime);
         addMandatory(theProductKwl, prefix, "im_pt.x",          **itNode, attPixel);

         // In TOPSAR products, GCPs are weird (they fall in black lines
         // between burst. This code allows to move them to a valid area of
         // the image.
         if(theBurstRecords.size()>2)
         {
            TimeType acqStart(0);
            unsigned long acqStartLine(0);

#if 0
            bool burstFound(false);
            for(std::vector<BurstRecordType>::reverse_iterator bIt = theBurstRecords.rbegin();bIt!=theBurstRecords.rend() && !burstFound;++bIt)
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
#else
            const std::vector<BurstRecordType>::const_reverse_iterator bIt
               = std::find_if(theBurstRecords.rbegin(), theBurstRecords.rend(), DoesContain(azimuthTime));
            if (bIt != theBurstRecords.rend())
            {
               acqStart = bIt->azimuthStartTime;
               acqStartLine = bIt->startLine;
            }
            else if(azimuthTime < theBurstRecords.front().azimuthStartTime)
            {
               acqStart = theBurstRecords.front().azimuthStartTime;
               acqStartLine = theBurstRecords.front().startLine;
            }
            else if (azimuthTime >= theBurstRecords.front().azimuthStopTime)
            {
               acqStart = theBurstRecords.back().azimuthStartTime;
               acqStartLine = theBurstRecords.back().startLine;
            }
            else
            {
               assert(!"unexpected case");
            }
#endif

            const DurationType timeSinceStart = azimuthTime - acqStart; // in sec

            const double timeSinceStartInMicroSeconds = timeSinceStart.total_microseconds();
            const double imPt_y= timeSinceStartInMicroSeconds/theAzimuthTimeInterval + acqStartLine;
            add(theProductKwl, prefix, "im_pt.y", imPt_y);
         }
         else
         {
            addMandatory(theProductKwl, prefix, "im_pt.y", **itNode, attLine);
         }
         addMandatory(theProductKwl, prefix, "wolrd_pt.lat", **itNode, attLatitude);
         addMandatory(theProductKwl, prefix, "wolrd_pt.lon", **itNode, attLongitude);
         addMandatory(theProductKwl, prefix, "wolrd_pt.hgt", **itNode, attHeight);
      }
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
            assert(dcEstimate.get());
            addOptional(theProductKwl, prefix, "dop_coef_time",  *dcEstimate, "azimuthTime");
            //RK
            const double ref_time = getOptionalTextFromFirstNode(*dcEstimate, "t0").toFloat64() * 1e9; // s to ns
            theProductKwl.add(prefix, "slant_range_time", ref_time);

            ossimString const& ns = getOptionalTextFromFirstNode(*dcEstimate, "ns");

            if( !ns.empty() )
               theProductKwl.add(prefix, "slant_range_time", ns);

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

                  theProductKwl.add(coeff_prefix, "dop_coef", *coeff);
               }

            } //if (!coeffStr.empty())

         } // for each dcEstimate

      } // else count < 1
   }


   double ossimSentinel1ProductDoc::getBandTerrainHeight(ossimXmlDocument const& productXmlDocument)
   {
      double heightSum = 0.0;
      vector< ossimXmlNodePtr > heightList;
      productXmlDocument.findNodes("/product/generalAnnotation/terrainHeightList/terrainHeight", heightList);
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

      imageSize.samp = samples_str.toInt();
      imageSize.line = lines_str.toInt();

      return true;
   }

   bool ossimSentinel1ProductDoc::initGsd(ossimDpt& gsd) const
   {
      // TODO: cache the value, or store it in a variable?
#if 0
      gsd.x =  theRangeSpacingTotal;
      gsd.y =  theAzimuthSpacingTotal;
#endif
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

         addMandatory(theProductKwl, orbit_prefix, "time",  *stateVectorList[i], "time");

         addMandatory(theProductKwl, orbit_prefix, "x_pos", *stateVectorList[i], "position/x");
         addMandatory(theProductKwl, orbit_prefix, "y_pos", *stateVectorList[i], "position/y");
         addMandatory(theProductKwl, orbit_prefix, "z_pos", *stateVectorList[i], "position/z");

         addMandatory(theProductKwl, orbit_prefix, "x_vel", *stateVectorList[i], "velocity/x");
         addMandatory(theProductKwl, orbit_prefix, "y_vel", *stateVectorList[i], "velocity/y");
         addMandatory(theProductKwl, orbit_prefix, "z_vel", *stateVectorList[i], "velocity/z");
      }
   }

   void ossimSentinel1ProductDoc::readCoordinates(
         ossimXmlNode const& node,
         ossimString const& rg0_xpath, ossimString const& coeffs_xpath,
         char const * sr_gr_prefix)
   {
      char prefix[1024];
      std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
      node.findChildNodes("coordinateConversion", xnodes);

      unsigned int idx = 0;
      for(std::vector<ossimRefPtr<ossimXmlNode> >::iterator itNode = xnodes.begin(); itNode!=xnodes.end();++itNode, ++idx)
      {
         int pos = std::snprintf(prefix, sizeof(prefix), "%s[%d].", sr_gr_prefix, idx);
         assert(pos >= sizeof(SR_PREFIX)+4 && pos < 1024);
         addMandatory(theProductKwl, prefix, attAzimuthTime,**itNode, attAzimuthTime);
         addMandatory(theProductKwl, prefix, rg0_xpath,     **itNode, rg0_xpath);

         ossimString const& s = getTextFromFirstNode(**itNode, coeffs_xpath);
         std::vector<ossimString> ssplit = s.split(" ");

         if (ssplit.empty())
         {
            throw std::runtime_error("The "+rg0_xpath+" record has an empty coef vector");
         }
         unsigned int coeff_idx;
         for (std::vector<ossimString>::const_iterator cIt = ssplit.begin(), e = ssplit.end()
               ; cIt != e
               ; ++cIt, ++coeff_idx
             )
         {
            // append to current prefix
            std::snprintf(prefix+pos, sizeof(prefix)-pos, ".coeff[%d]", coeff_idx);
            add(theProductKwl, prefix, *cIt); // Don't check this is really a double.
         }
         assert(coeff_idx>0 &&"The rg0 record has empty coefs vector.");
      }
   }

   void ossimSentinel1ProductDoc::addSRGRCoefficients(ossimXmlNode const& coordinateConversionList)
   {
      readCoordinates(coordinateConversionList, attSr0, attSrgrCoefficients, SR_PREFIX);
      readCoordinates(coordinateConversionList, attGr0, attGrsrCoefficients, GR_PREFIX);
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
         assert(calibrationRoot.get());
         ossimXmlNode const& adsHeader = getExpectedFirstNode(*calibrationRoot, "adsHeader");
         ossimXmlNode const& calibrationInformation = getExpectedFirstNode(*calibrationRoot, "calibrationInformation");
         ossimXmlNode const& calibrationVectorList  = getExpectedFirstNode(*calibrationRoot, "calibrationVectorList");

         char const calibrationPrefix[] = "calibration.";

         addMandatory(theProductKwl, calibrationPrefix, "swath", adsHeader, "swath");
         addMandatory(theProductKwl, calibrationPrefix, "polarisation", adsHeader, "polarisation");

         add(theProductKwl, calibrationPrefix, "startTime",
               time::toModifiedJulianDate(getTextFromFirstNode(adsHeader, "startTime")));

         add(theProductKwl, calibrationPrefix, "stopTime",
               time::toModifiedJulianDate(getTextFromFirstNode(adsHeader, "stopTime")));

         addMandatory(theProductKwl, calibrationPrefix, "absoluteCalibrationConstant", calibrationInformation, "absoluteCalibrationConstant");

         theProductKwl.add(calibrationPrefix,
               "count",
               calibrationVectorList.getAttributeValue("count"));

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
            assert(b_calibVector->get());

            ossimXmlNode const& calibrationVector = **b_calibVector;
            const ossimXmlNodePtr & node = calibrationVector.findFirstNode("pixel");

            theProductKwl.add(calibrationVectorPrefix,
                  "pixel_count",
                  node->getAttributeValue("count"));

            add(theProductKwl, calibrationVectorPrefix, "azimuthTime",
                  time::toModifiedJulianDate(getOptionalTextFromFirstNode(calibrationVector, "azimuthTime")));

            addMandatory(theProductKwl, calibrationVectorPrefix, "line",        calibrationVector, "line");
            addMandatory(theProductKwl, calibrationVectorPrefix, "pixel",       calibrationVector, "pixel");
            addMandatory(theProductKwl, calibrationVectorPrefix, "sigmaNought", calibrationVector, "sigmaNought");
            addMandatory(theProductKwl, calibrationVectorPrefix, "betaNought",  calibrationVector, "betaNought");
            addMandatory(theProductKwl, calibrationVectorPrefix, "gamma",       calibrationVector, "gamma");
            addMandatory(theProductKwl, calibrationVectorPrefix, "dn",          calibrationVector, "dn");

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
            assert(b_noiseVector->get());

            ossimXmlNode const& noiseVector = **b_noiseVector;

            const ossimXmlNodePtr & node = noiseVector.findFirstNode("pixel");

            theProductKwl.add(noiseVectorPrefix,
                  "pixel_count",
                  node->getAttributeValue("count"),
                  false);

            add(theProductKwl, noiseVectorPrefix, "azimuthTime",
                  time::toModifiedJulianDate(getTextFromFirstNode(noiseVector, "azimuthTime")));

            addMandatory(theProductKwl, noiseVectorPrefix, "line",     noiseVector, "line");
            addMandatory(theProductKwl, noiseVectorPrefix, "pixel",    noiseVector, "pixel");
            addMandatory(theProductKwl, noiseVectorPrefix, "noiseLut", noiseVector, "noiseLut");
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
#endif
