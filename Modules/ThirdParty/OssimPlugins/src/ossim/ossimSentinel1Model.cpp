//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL-2
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#include "ossimSentinel1Model.h"
#include "ossimTraceHelpers.h"
#include "ossimXmlTools.h"
#include "ossimKeyWordListUtilities.h"
#include "ossimSarSensorModelPathsAndKeys.h"
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimXmlNode.h>
#include <iostream>
#include <cassert>

#if defined(USE_BOOST_TIME)
            using boost::posix_time::microseconds;
            using boost::posix_time::seconds;
#else
            using ossimplugins::time::microseconds;
            using ossimplugins::time::seconds;
#endif
namespace {// Anonymous namespace
   ossimTrace traceExec  ("ossimSentinel1Model:exec");
   ossimTrace traceDebug ("ossimSentinel1Model:debug");
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
}// Anonymous namespace

namespace ossimplugins
{
   RTTI_DEF1(ossimSentinel1Model, "ossimSentinel1Model", ossimSarSensorModel);

//*************************************************************************************************
// Constructor
//*************************************************************************************************
   ossimSentinel1Model::ossimSentinel1Model()
      : ossimSarSensorModel()
      , theOCN(false)
      , theSLC(false)
      , theTOPSAR(false)
   {
      // theManifestDoc = new ossimXmlDocument();
   }

   void ossimSentinel1Model::clearFields()
   {
      theOCN    = false;
      theSLC    = false;
      theTOPSAR = false;
      theManifestKwl.clear();
      theProductKwl.clear();
      // theManifestFile = ossimFilename::NIL;
      theProductXmlFile = ossimFilename::NIL;
   }

//*************************************************************************************************
// Infamous DUP
//*************************************************************************************************
   ossimObject* ossimSentinel1Model::dup() const
   {
      return new ossimSentinel1Model(*this);
   }

//*************************************************************************************************
// Print
//*************************************************************************************************
   std::ostream& ossimSentinel1Model::print(std::ostream& out) const
   {
      // Capture stream flags since we are going to mess with them.
      std::ios_base::fmtflags f = out.flags();

      out << "\nDump of ossimSentinel1Model at address " << hex << this
          << dec
          << "\n------------------------------------------------"
          << "\n  theImageID            = " << theImageID
          << "\n  theImageSize          = " << theImageSize

          << "\n------------------------------------------------"
          << "\n  " << endl;

      // Set the flags back.
      out.flags(f);
      return ossimSarSensorModel::print(out);
   }

//*************************************************************************************************
// Save State
//*************************************************************************************************
   bool ossimSentinel1Model::saveState(ossimKeywordlist& kwl,
                                      const char* prefix) const
   {
      static const char MODULE[] = "ossimplugins::ossimSentinel1Model::saveState";
      SCOPED_LOG(traceDebug, MODULE);

      kwl.add(prefix,
              ossimKeywordNames::TYPE_KW,
              "ossimSentinel1Model",
              true);

      kwl.add("support_data.",
              "calibration_lookup_flag",
              "true",
              true);

      kwl.addList(theManifestKwl, true);
      kwl.addList(theProductKwl,  true);

      return ossimSarSensorModel::saveState(kwl, prefix);
   }


//*************************************************************************************************
// Load State
//*************************************************************************************************
   bool ossimSentinel1Model::loadState(const ossimKeywordlist& kwl,
                                      const char* prefix)
   {
      static const char MODULE[] = "ossimplugins::ossimSentinel1Model::loadState";
      SCOPED_LOG(traceDebug, MODULE);

      theManifestKwl.addList(kwl, true);

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "theManifestKwl.getSize()" << theManifestKwl.getSize() << "\n";
      }

      return ossimSarSensorModel::loadState(kwl, prefix);
   }

   ossimFilename ossimSentinel1Model::searchManifestFile(const ossimFilename& file) const
   {
      const ossimFilename manifestFile = ossimFilename(file.path().path() + "/manifest.safe");

      if(!manifestFile.exists())
      {
         ossimNotify(ossimNotifyLevel_WARN) << "manifest.safe " << manifestFile << " doesn't exist...\n";
         return "";
      }
      return manifestFile;
   }

   bool ossimSentinel1Model::open(const ossimFilename& file)
   {
      static const char MODULE[] = "ossimplugins::ossimSentinel1Model::open";
      //traceDebug.setTraceFlag(true);
      SCOPED_LOG(traceDebug, MODULE);

      bool result = false;

      const ossimString ext = file.ext().downcase();
      if ( !file.exists() || (ext != "tiff" && ext != "xml" ))
      {
         return false;
      }
      else
      {
         theGSD.makeNan();

         // -----[ Read manifest file
         const ossimFilename safeFile = searchManifestFile(file);
         theManifestDirectory = safeFile.path();
         if (!safeFile.empty())
         {
            if ( !this->isSentinel1(safeFile))
            {
               ossimNotify(ossimNotifyLevel_FATAL) << MODULE << "Not a Sentinel 1 manifest file " << safeFile << "\n";
               return false;
            }
            ossimXmlDocument manifestDoc;
            if (!manifestDoc.openFile(safeFile))
            {
               ossimNotify(ossimNotifyLevel_FATAL) << MODULE << "Invalid Manifest file " << safeFile << "\n";
               return false;
            }
            ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << "Manifest file " << safeFile << " opened\n";

            theImageID = getImageId(manifestDoc);
            if (theImageID.empty()) {
               ossimNotify(ossimNotifyLevel_FATAL) << MODULE << "Image ID not found in manifest file " << safeFile << "\n";
               return false;
            }

            if (! standAloneProductInformation(manifestDoc))  {
               ossimNotify(ossimNotifyLevel_FATAL) << MODULE << "Cannot load product information from " << safeFile << "\n";
               return false;
            }

            theSensorID = initSensorID(manifestDoc);
            if (theSensorID.empty()) {
               ossimNotify(ossimNotifyLevel_FATAL) << MODULE << "Cannot load sensor ID from " << safeFile << "\n";
               return false;
            }
         }

         // -----[ Read product file
         ossimFilename xmlFileName = file;

         // If this is tiff file, look for corresponding annotation file
         if(ext != "xml")
           {
           const ossimFilename fileNameWihtoutExtension = file.fileNoExtension();
           const ossimFilename path = file.path().path();
           xmlFileName = ossimFilename(path+"/annotation/"+fileNameWihtoutExtension+".xml");
           }

         if ( !xmlFileName.exists() || !this->readProduct(xmlFileName) )
         {
            ossimNotify(ossimNotifyLevel_FATAL) << MODULE << " this->readProduct( safeFile )\n";
            return false;
         }

         if ( !this->initImageSize( theImageSize ) )
         {
           ossimNotify(ossimNotifyLevel_FATAL) << MODULE << " this->initImageSize( theImageSize ) fails\n";
           return false;
         }

         theImageClipRect = ossimDrect( 0, 0, theImageSize.x-1, theImageSize.y-1 );
         theSubImageOffset.x = 0.0;
         theSubImageOffset.y = 0.0;

         // automatically loaded/saved into ossimSensorModel
         theMeanGSD = (theGSD.x + theGSD.y)/2.0;

#if 0
         if ( !this->initSRGR( ) )
         {
         ossimNotify(ossimNotifyLevel_FATAL) << MODULE << " this->initSRGR( )\n";
         return false;
         }
#endif

         // Commit the operation
         theProductXmlFile = file;
         return true;
      }
   }

   ossimString const& ossimSentinel1Model::getImageId(ossimXmlDocument const& manifestDoc) const
   {
      ossimString xpath = "/xfdu:XFDU/metadataSection/metadataObject/metadataWrap/xmlData/s1sarl1:standAloneProductInformation/s1sarl1:missionDataTakeID";
      return getOnlyText(manifestDoc, xpath);
   }

   ossimString ossimSentinel1Model::initSensorID(ossimXmlDocument const& manifestDoc)
   {
      const ossimRefPtr<ossimXmlNode> safePlatform = manifestDoc.getRoot()->findFirstNode("metadataSection/metadataObject/metadataWrap/xmlData/safe:platform");
      ossimString familyName, instrumentId;
      const bool ret1 = safePlatform->getChildTextValue(familyName, "safe:familyName");
      const bool ret2 = safePlatform->getChildTextValue(instrumentId, "safe:number");

      if (ret1 && ret2) {
         theManifestKwl.add("support_data.",
               "instrument",
               "S1" + instrumentId,
               true);
         return familyName + instrumentId;
      }
   }

#if 0
   bool ossimSentinel1Model::getAnnotationFileLocation(const ossimFilename &manifestFile, const char* pattern)
   {
      static const char MODULE[] = "ossimSentinel1SafeManifest::getAnnotationFileLocation";
      //traceDebug.setTraceFlag(true);
      const ossimString prefix = "support_data.";
      const ossimString xpath =  "/xfdu:XFDU/dataObjectSection/dataObject";

      vector<ossimRefPtr<ossimXmlNode> > xml_nodes;

      theManifestDoc->findNodes(xpath, xml_nodes);

      ossimRefPtr<ossimXmlNode> dataObjectNode = xml_nodes[0];
      ossimString dataObjectId;
      dataObjectNode->getAttributeValue(dataObjectId , "ID");

      if ( dataObjectId.match(pattern) )
      {
         ossim::getPath(xpath +  "/byteStream/fileLocation", theManifestDoc.get(), theProductXmlFile);
         xml_nodes.clear();
         theManifestDoc->findNodes(xpath +  "/byteStream/fileLocation", xml_nodes);
         if(xml_nodes.empty())
         {
            ossimNotify(ossimNotifyLevel_FATAL) << MODULE << " getAnnotationFileLocation( )\n";
            return  false;
         }
         xml_nodes[0]->getAttributeValue(theProductXmlFile , "href");
         theProductXmlFile.setPath(manifestFile.path() + "/annotation");

      }
      return true;
   }
#endif

   bool ossimSentinel1Model::standAloneProductInformation(ossimXmlDocument const& manifestDoc)
   {
      static const char MODULE[] = "ossimSentinel1ProductDoc::parseSafe";

      const ossimString prefix = "support_data.";

      const ossimRefPtr<ossimXmlNode> safeProcessing = manifestDoc.getRoot()->findFirstNode("metadataSection/metadataObject/metadataWrap/xmlData/safe:processing");
      const ossimRefPtr<ossimXmlNode> facility       = safeProcessing->findFirstNode("safe:facility");
      const ossimRefPtr<ossimXmlNode> software       = facility->findFirstNode("safe:software");
      const ossimString org     = facility->getAttributeValue("organisation");
      const ossimString name    = software->getAttributeValue("name");
      const ossimString version = software->getAttributeValue("version");

      theManifestKwl.add(prefix,
            "Processing_system_identifier",
            org + " " + name + " " + version,
            true);

      theManifestKwl.add(prefix,
            ossimKeywordNames::DATE_KW,
            safeProcessing->getAttributeValue("start"),
            true);

      const ossimRefPtr<ossimXmlNode> acquisitionPeriod = manifestDoc.getRoot()->findFirstNode("metadataSection/metadataObject/metadataWrap/xmlData/safe:acquisitionPeriod");
      ossimString acqStartTime = acquisitionPeriod->getChildTextValue("safe:startTime");

      theManifestKwl.add(prefix,
            "first_line_time",
            acqStartTime,
            true);

      theManifestKwl.add(prefix,
            "last_line_time",
            acquisitionPeriod->getChildTextValue("safe:stopTime"),
            true);
      //RK
      theManifestKwl.add(prefix,
            ossimKeywordNames::IMAGE_DATE_KW,
            acqStartTime,
            true);

      const ossimRefPtr<ossimXmlNode> instrumentNode =
         manifestDoc.getRoot()->findFirstNode("metadataSection/metadataObject/metadataWrap/xmlData/safe:platform/safe:instrument");
      ossimString swath            =  instrumentNode->getChildTextValue("s1sarl1:swath");
      ossimString acquisition_mode =  instrumentNode->getChildTextValue("s1sarl1:mode");

      if( acquisition_mode.empty())
      {
         ossimRefPtr<ossimXmlNode>  instrumentModeNode =  instrumentNode->findFirstNode("safe:extension/s1sarl1:instrumentMode");
         if(instrumentModeNode.get())
         {
            acquisition_mode = instrumentModeNode->getChildTextValue("s1sarl1:mode");
            swath            = instrumentModeNode->getChildTextValue("s1sarl1:swath");
         }
      }

      theManifestKwl.add(prefix,
            "acquisition_mode",
            acquisition_mode,
            true);

      theManifestKwl.add(prefix,
            "swath",
            swath,
            true);

      if (acquisition_mode == "IW" || acquisition_mode == "EW")
         theTOPSAR = true;

      const ossimRefPtr<ossimXmlNode> orbitReference =
         manifestDoc.getRoot()->findFirstNode("metadataSection/metadataObject/metadataWrap/xmlData/safe:orbitReference");

      std::vector<ossimRefPtr<ossimXmlNode> > orbitNumberNodes;
      const ossimString orbitReference_xpath = "/xfdu:XFDU/metadataSection/metadataObject/metadataWrap/xmlData/safe:orbitReference";
      manifestDoc.findNodes(orbitReference_xpath + "/safe:orbitNumber", orbitNumberNodes);

      std::vector<ossimRefPtr<ossimXmlNode> >::const_iterator it = orbitNumberNodes.begin();
      while( it != orbitNumberNodes.end())
      {
         const ossimRefPtr<ossimXmlAttribute> attribute =    (*it)->findAttribute("type");
         if( attribute.get() )
         {
            if( attribute->getValue() == "start" )
            {
               theManifestKwl.add(prefix, "abs_orbit", (*it)->getText(), true);
               break;
            }
         }
         ++it;
      }

      orbitNumberNodes.clear();
      manifestDoc.findNodes(orbitReference_xpath + "/safe:relativeOrbitNumber", orbitNumberNodes);

      std::vector<ossimRefPtr<ossimXmlNode> >::const_iterator it2 = orbitNumberNodes.begin();
      while( it2 != orbitNumberNodes.end())
      {
         const ossimRefPtr<ossimXmlAttribute> attribute =    (*it2)->findAttribute("start");
         if( attribute.get() )
         {
            if( attribute->getValue() == "start" )
            {
               theManifestKwl.add(prefix, "rel_orbit", (*it2)->getText(), true);
               break;
            }
         }
         ++it2;
      }

      theManifestKwl.add(prefix, "orbit_cycle", orbitReference->getChildTextValue("safe:cycleNumber"), true);

      ossimString orbit_pass = "";
      orbit_pass = orbitReference->getChildTextValue("s1:pass");

      if( orbit_pass.empty() )
      {
         orbit_pass = orbitReference->getChildTextValue("safe:extension/s1:orbitProperties/s1:pass");
      }

      theManifestKwl.add(prefix,
            "orbit_pass",
            orbit_pass,
            true);

      ossimString productType = "unknown";
      const ossimRefPtr<ossimXmlNode> standAloneProductInformation =
         manifestDoc.getRoot()->findFirstNode("metadataSection/metadataObject/metadataWrap/xmlData/s1sarl1:standAloneProductInformation");

      if (theOCN)
      {
         productType = "OCN";
      }
      else
      {
         if (standAloneProductInformation.get())
            productType  = standAloneProductInformation->getChildTextValue("s1sarl1:productType");
      }

      theManifestKwl.add(prefix,
            "product_type",
            productType,
            true);

      if( productType.contains("SLC" ) )
      {
         //ossimKeywordNames::PIXEL_TYPE_KW;  RK
         theManifestKwl.add("sample_type", "COMPLEX",  true);
         theSLC = true;
      }
      else
      {
         theManifestKwl.add("sample_type", "DETECTED",  true);
         theManifestKwl.add(prefix, "srgr_flag", "true",  true);
      }

      return true;
   }

   bool ossimSentinel1Model::isSentinel1(const ossimFilename &manifestFile )
   {
      theOCN = isLevel2(manifestFile);
      return isLevel1(manifestFile) || theOCN  || isLevel0(manifestFile);
   }

   bool ossimSentinel1Model::isLevel1(const ossimFilename& file) const
   {
      bool productXmlCheck = checkDirectory(file, "annotation", ".xml");

      return productXmlCheck && checkDirectory(file, "measurement", ".tiff");
   }

   bool ossimSentinel1Model::isLevel2(const ossimFilename& file) const
   {
      return checkDirectory(file, "measurement",".nc");
   }

   bool ossimSentinel1Model::isLevel0(const ossimFilename& file) const
   {
      return checkDirectory(file, "measurement",".dat");
   }

   bool ossimSentinel1Model::checkDirectory(const ossimFilename& file, const char* d, const char* ext) const
   {
      //check dir is valid first
      ossimDirectory dir(file.path() + '/' + d + '/');
      std::vector<ossimFilename> result;
      dir.findAllFilesThatMatch(result, ext);
      if ( result.empty() )
      {
         if (traceExec())
         {
            ossimNotify(ossimNotifyLevel_FATAL)
               << " DEBUG:" << " checkDirectory failed for: " << file.path()   << "/" << d << " with ext ="<< ext << "\n";
         }
         return false;
      }

      return true;
   }

   bool ossimSentinel1Model::readProduct(const ossimFilename &productXmlFile)
   {
      try
      {
         const bool ret = read(productXmlFile);
         if ( ret )
         {
            readCalibrationMetadata();
            readNoiseMetadata();
            return true;
         }
      } catch (std::exception const& e) {
         if (traceExec())
            ossimNotify(ossimNotifyLevel_INFO)
               << "Error while reading " << productXmlFile << " as Sentinel1 model: " << e.what() << "\n";
      }
      if (traceExec())
         ossimNotify(ossimNotifyLevel_FATAL)
               << "Cannot read " << productXmlFile << " as Sentinel1 model.\n";
      return false;
   }

   bool ossimSentinel1Model::read(ossimFilename const& annotationXml)
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
      theProductType = ProductType(productType);

      add(theProductKwl, SUPPORT_DATA_PREFIX, "product_type", productType.string());

      addMandatory(theProductKwl, HEADER_PREFIX, "swath",        adsHeader, "swath");
      addMandatory(theProductKwl, HEADER_PREFIX, "polarisation", adsHeader, "polarisation");
      add(theProductKwl, HEADER_PREFIX, "polarisation", polarisation.string());
      add(theProductKwl, HEADER_PREFIX, "annotation",   annotationXml.file().string());

      addMandatory(theProductKwl, HEADER_PREFIX, "first_line_time", adsHeader, "startTime");
      addMandatory(theProductKwl, HEADER_PREFIX, "last_line_time",  adsHeader, "stopTime");

      //RK maybe use this->getManifestPrefix()

      add(theProductKwl, SUPPORT_DATA_PREFIX, "mds1_tx_rx_polar", polarisation);

      const ossimXmlNode & imageInformation   = getExpectedFirstNode(*productRoot, "imageAnnotation/imageInformation");
      const ossimXmlNode & productInformation = getExpectedFirstNode(*productRoot, "generalAnnotation/productInformation");

      addOptional (theProductKwl, SUPPORT_DATA_PREFIX, "data_take_id",       adsHeader,        "missionDataTakeId");
      addOptional (theProductKwl, SUPPORT_DATA_PREFIX, "slice_num",          imageInformation, "sliceNumber");

      ossimXmlNode const& xAzimutTimeInterval = getExpectedFirstNode(imageInformation, "azimuthTimeInterval");
      const double azimuthTimeInterval = to<double>(xAzimutTimeInterval.getText(), "decoding imageInformation/azimuthTimeInterval");
#if defined(USE_BOOST_TIME)
      theAzimuthTimeInterval = boost::posix_time::precise_duration(azimuthTimeInterval * 1000000.);
#else
      theAzimuthTimeInterval = seconds(azimuthTimeInterval);
#endif
      // std::clog << "theAzimuthTimeInterval " << theAzimuthTimeInterval.total_microseconds() << "\n";
      add(theProductKwl, SUPPORT_DATA_PREFIX, "line_time_interval", xAzimutTimeInterval.getText());
      // addMandatory(theProductKwl, SUPPORT_DATA_PREFIX, "line_time_interval", imageInformation, "azimuthTimeInterval");

      const double rangeSpacing = getDoubleFromFirstNode(imageInformation, "rangePixelSpacing");
      add(theProductKwl, SUPPORT_DATA_PREFIX, "range_spacing", rangeSpacing);
      theGSD.x = rangeSpacing;

      const double azimuthSpacing = getDoubleFromFirstNode(imageInformation, "azimuthPixelSpacing");
      add(theProductKwl, SUPPORT_DATA_PREFIX, "azimuth_spacing", azimuthSpacing);
      theGSD.y = azimuthSpacing;
      // theGSD and theMeanGSD will be saved and reloaded automatically in
      // ossimSensorModel::load/saveState()

      // TODO: Why duplicting these two information elsewhere? they're already
      // under root prefix
      addOptional(theProductKwl, ossimKeywordNames::NUMBER_SAMPLES_KW, imageInformation, "numberOfSamples");
      addOptional(theProductKwl, ossimKeywordNames::NUMBER_LINES_KW,   imageInformation, "numberOfLines");

      add(theProductKwl,
            "sample_type" /*ossimKeywordNames::PIXEL_TYPE_KW*/,
            getOptionalTextFromFirstNode(imageInformation, "pixelValue").upcase());

      // TODO: check if it makes sense when only one file is considered
      const double heightSum = getBandTerrainHeight(productXmlDocument);
      add(theProductKwl, SUPPORT_DATA_PREFIX, "avg_scene_height", heightSum);


      // these should be the same for all swaths
      //RK div by oneMillion taken from S1tlbx
      addOptional(theProductKwl, SUPPORT_DATA_PREFIX, "range_sampling_rate",        productInformation, "rangeSamplingRate");
      addOptional(theProductKwl, SUPPORT_DATA_PREFIX, "radar_frequency",            productInformation, "radarFrequency");
      // TODO: store the line_time_interval * 1000000
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

      if (isGRD()) {
         ossimXmlNodePtr const& coordinateConversionList = productRoot->findFirstNode("coordinateConversion/coordinateConversionList");
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
      add(theProductKwl, SUPPORT_DATA_PREFIX,
            ossimKeywordNames::NUMBER_BANDS_KW,
            numBands,
            true);
#endif
      return true;
   }

   void ossimSentinel1Model::readCalibrationMetadata()
   {
      ossimDirectory calibrationDir( theManifestDirectory.dirCat( "annotation/calibration") );
      std::vector<ossimFilename> files;
      if (calibrationDir.isOpened())
        {
        calibrationDir.findAllFilesThatMatch(files, "calibration*");
        }
      std::vector<ossimFilename>::const_iterator it = files.begin();

      ossimNotify(ossimNotifyLevel_INFO) << files.size() << " calibration files found in " << theManifestDirectory << "\n";
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

         // store as a string
         addMandatory(theProductKwl, calibrationPrefix, "startTime", adsHeader, "startTime");
         addMandatory(theProductKwl, calibrationPrefix, "stopTime",  adsHeader, "stopTime");

         addMandatory(theProductKwl, calibrationPrefix, "absoluteCalibrationConstant", calibrationInformation, "absoluteCalibrationConstant");

         add(theProductKwl, calibrationPrefix,
               "count",
               calibrationVectorList.getAttributeValue("count").string());

         std::vector< ossimXmlNodePtr > calibrationVectors;
         calibrationRoot->findChildNodes("calibrationVectorList/calibrationVector", calibrationVectors);
         int idx = 0;
         for (std::vector< ossimXmlNodePtr >::const_iterator b_calibVector = calibrationVectors.begin(), e_calibVector = calibrationVectors.end()
               ; b_calibVector != e_calibVector
               ; ++b_calibVector, ++idx
             )
         {
            char calibrationVectorPrefix[256];
            s_printf(calibrationVectorPrefix, "%scalibrationVector[%d].", calibrationPrefix, idx);
            assert(b_calibVector->get());

            ossimXmlNode const& calibrationVector = **b_calibVector;
            const ossimXmlNodePtr & node = calibrationVector.findFirstNode("pixel");

            add(theProductKwl, calibrationVectorPrefix,
                  "pixel_count",
                  node->getAttributeValue("count").string());

            // store as a string
            addMandatory(theProductKwl, calibrationVectorPrefix, keyAzimuthTime, calibrationVector, "azimuthTime");

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

   void ossimSentinel1Model::readNoiseMetadata()
   {
      ossimDirectory calibrationDir( theManifestDirectory.dirCat( "annotation/calibration") );
      std::vector<ossimFilename> files;
      if (calibrationDir.isOpened())
        {
        calibrationDir.findAllFilesThatMatch(files, "noise*");
        }
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
            s_printf(noiseVectorPrefix, "%snoiseVector[%d].", noisePrefix, idx);
            assert(b_noiseVector->get());

            ossimXmlNode const& noiseVector = **b_noiseVector;

            const ossimXmlNodePtr & node = noiseVector.findFirstNode("pixel");

            add(theProductKwl, noiseVectorPrefix,
                  "pixel_count",
                  node->getAttributeValue("count"),
                  ShallOverwrite::no);

            // store as a string
            addMandatory(theProductKwl, noiseVectorPrefix, keyAzimuthTime, noiseVector, "azimuthTime");

            addMandatory(theProductKwl, noiseVectorPrefix, "line",     noiseVector, "line");
            addMandatory(theProductKwl, noiseVectorPrefix, "pixel",    noiseVector, "pixel");
            addMandatory(theProductKwl, noiseVectorPrefix, "noiseLut", noiseVector, "noiseLut");
            //noiseVectorList[idx]->toKwl(theProductKwl, "noiseVectorList_" + ossimString::toString(idx+1) + ".");
         }
      }
   }

   void ossimSentinel1Model::readBurstRecords(ossimXmlNode const& productRoot, ossimXmlNode const& imageInformation)
   {
      std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
      productRoot.findChildNodes("swathTiming/burstList/burst",xnodes);
      if (xnodes.empty())
      {
         // Appart from TopSAR products, there won't be any burst
         //records, so this warning is unnecessary
         //ossimNotify(ossimNotifyLevel_DEBUG) << "No burst records available in metadata!!\n";
         add(theProductKwl, BURST_NUMBER_KEY,                       ossimString("1"));
         BurstRecordType burstRecord;
         burstRecord.startLine        = add(theProductKwl, BURST_PREFIX, "[0].start_line", 0);
         burstRecord.azimuthStartTime = add(theProductKwl, BURST_PREFIX, "[0].azimuth_start_time", getTimeFromFirstNode(imageInformation,  "productFirstLineUtcTime"));
         burstRecord.azimuthStopTime  = add(theProductKwl, BURST_PREFIX, "[0].azimuth_stop_time",  getTimeFromFirstNode(imageInformation,  "productLastLineUtcTime"));
         burstRecord.endLine          = add(theProductKwl, BURST_PREFIX, "[0].end_line",           getFromFirstNode<unsigned int>(imageInformation, "numberOfLines")-1);
         theBurstRecords.push_back(burstRecord);
      }
      else
      {
         char burstPrefix[1024];

         const unsigned int linesPerBurst = getFromFirstNode<unsigned int>(productRoot, "swathTiming/linesPerBurst");
         unsigned int burstId(0);

         for(std::vector<ossimRefPtr<ossimXmlNode> >::iterator itNode = xnodes.begin(); itNode!=xnodes.end();++itNode,++burstId)
         {
            BurstRecordType burstRecord;
            const TimeType azTime = getTimeFromFirstNode(**itNode, attAzimuthTime);
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

            s_printf(burstPrefix, "%s[%d].", BURST_PREFIX.c_str(), burstId);
            burstRecord.startLine = add(theProductKwl,burstPrefix + keyStartLine,         burstId*linesPerBurst + first_valid);
            burstRecord.endLine   = add(theProductKwl,burstPrefix + keyEndLine,           burstId*linesPerBurst + last_valid);
            // TODO: check units.
            burstRecord.azimuthStartTime = add(theProductKwl,burstPrefix + keyAzimuthStartTime, azTime + (first_valid*theAzimuthTimeInterval));
            burstRecord.azimuthStopTime  = add(theProductKwl,burstPrefix + keyAzimuthStopTime,  azTime + (last_valid*theAzimuthTimeInterval));

            theBurstRecords.push_back(burstRecord);
         }
         add(theProductKwl, BURST_NUMBER_KEY, burstId);
      }
   }

   void ossimSentinel1Model::addSRGRCoefficients(ossimXmlNode const& coordinateConversionList)
   {
      readCoordinates(coordinateConversionList, attSr0, attSrgrCoefficients, SR_PREFIX);
      readCoordinates(coordinateConversionList, attGr0, attGrsrCoefficients, GR_PREFIX);
   }

   void ossimSentinel1Model::readCoordinates(
         ossimXmlNode const& node,
         ossimString const& rg0_xpath, ossimString const& coeffs_xpath,
         std::string const& sr_gr_prefix)
   {
      char prefix[1024];
      std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
      node.findChildNodes("coordinateConversion", xnodes);

      add(theProductKwl,sr_gr_prefix+"."+NUMBER_KEY, static_cast<ossim_uint32>(xnodes.size()));

      unsigned int idx = 0;
      for(std::vector<ossimRefPtr<ossimXmlNode> >::iterator itNode = xnodes.begin(); itNode!=xnodes.end();++itNode, ++idx)
      {
         int pos = s_printf(prefix, "%s[%d].", sr_gr_prefix.c_str(), idx);
         assert(pos >= sizeof(SR_PREFIX)+4 && pos < sizeof(prefix));

         addMandatory(theProductKwl, prefix + keyAzimuthTime,**itNode, attAzimuthTime);
         addMandatory(theProductKwl, prefix + rg0_xpath,     **itNode, rg0_xpath);

         ossimString const& s = getTextFromFirstNode(**itNode, coeffs_xpath);
         std::vector<ossimString> ssplit = s.split(" ");

         if (ssplit.empty())
         {
            throw std::runtime_error("The "+rg0_xpath.string()+" record has an empty coef vector");
         }
         add(theProductKwl,prefix+ NUMBER_KEY, ssplit.size());

         unsigned int coeff_idx = 0;
         for (std::vector<ossimString>::const_iterator cIt = ssplit.begin(), e = ssplit.end()
               ; cIt != e
               ; ++cIt, ++coeff_idx
             )
         {
            // append to current prefix
            s_printf(prefix+pos, sizeof(prefix)-pos, "coeff[%d]", coeff_idx);
            add(theProductKwl, prefix, *cIt); // Don't check this is really a double.
         }
         assert(coeff_idx>0 &&"The rg0 record has empty coefs vector.");
      }
   }

   void ossimSentinel1Model::readGeoLocationGrid(ossimXmlNode const& productRoot)
   {
      SCOPED_LOG(traceDebug, "ossimSentinel1Model::readGeoLocationGrid");
      char prefix[1024];
      std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
      productRoot.findChildNodes("geolocationGrid/geolocationGridPointList/geolocationGridPoint", xnodes);

      unsigned int idx = 0;
      for(std::vector<ossimRefPtr<ossimXmlNode> >::iterator itNode = xnodes.begin(); itNode!=xnodes.end();++itNode,++idx)
      {
         int pos = s_printf(prefix, "%s[%d].", GCP_PREFIX.c_str(), idx);
         assert(pos >= sizeof(SR_PREFIX)+4 && pos < 1024);
#if defined(USE_BOOST_TIME)
         const TimeType azimuthTime = getTimeFromFirstNode(**itNode, attAzimuthTime);
         add(theProductKwl, prefix, attAzimuthTime, azimuthTime);
#else
         const TimeType azimuthTime = time::toModifiedJulianDate(
               addMandatory(theProductKwl, prefix, attAzimuthTime,     **itNode, attAzimuthTime)); // acquisition time
#endif
         addMandatory(theProductKwl, prefix, keySlantRangeTime, **itNode, attSlantRangeTime);
         addMandatory(theProductKwl, prefix, keyImPtX,          **itNode, attPixel);

         // In TOPSAR products, GCPs are weird (they fall in black lines
         // between burst. This code allows to move them to a valid area of
         // the image.
         if(theBurstRecords.size()>2)
         {
#if defined(USE_BOOST_TIME)
            TimeType acqStart;
#else
            TimeType acqStart(0);
#endif
            unsigned long acqStartLine(0);

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

            const DurationType timeSinceStart = azimuthTime - acqStart; // in day frac

            const double imPt_y= timeSinceStart/theAzimuthTimeInterval + acqStartLine;
            // std::clog << "timeSinceStart: " << timeSinceStart << "(" << timeSinceStart.total_microseconds() << "us) = " << azimuthTime << " - " << acqStart <<  " (azTime-acqStart)"<< "\n";
            // std::clog << "imPt_y: " << imPt_y << " = " << timeSinceStart.total_microseconds() << "/" << theAzimuthTimeInterval.total_microseconds() << "+" << acqStartLine << "\n";
            add(theProductKwl, prefix, keyImPtY, imPt_y);
         }
         else
         {
            addMandatory(theProductKwl, prefix, keyImPtY, **itNode, attLine);
         }
         addMandatory(theProductKwl, prefix, keyWorldPtLat, **itNode, attLatitude);
         addMandatory(theProductKwl, prefix, keyWorldPtLon, **itNode, attLongitude);
         addMandatory(theProductKwl, prefix, keyWorldPtHgt, **itNode, attHeight);
      }
      add(theProductKwl, GCP_NUMBER_KEY, static_cast<ossim_uint32>(idx));
   }

   void ossimSentinel1Model::addOrbitStateVectors(ossimXmlNode const& orbitList)
   {
      ossimXmlNode::ChildListType stateVectorList;
      orbitList.findChildNodes("orbit", stateVectorList);

      if(stateVectorList.empty())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "No orbitVectorList info available in metadata!!\n";
         return;
      }

      std::size_t stateVectorList_size = stateVectorList.size();
      char orbit_prefix_[256];
      for (std::size_t i = 0; i != stateVectorList_size ; ++i)
      {
         //orbit_state_vectors
         const int pos = s_printf(orbit_prefix_, "orbitList.orbit[%d].", int(i));
         assert(pos > 0 && pos < 256);
         const std::string orbit_prefix(orbit_prefix_, pos);

         addMandatory(theProductKwl, orbit_prefix + keyTime,  *stateVectorList[i], "time");

         addMandatory(theProductKwl, orbit_prefix + keyPosX, *stateVectorList[i], "position/x");
         addMandatory(theProductKwl, orbit_prefix + keyPosY, *stateVectorList[i], "position/y");
         addMandatory(theProductKwl, orbit_prefix + keyPosZ, *stateVectorList[i], "position/z");

         addMandatory(theProductKwl, orbit_prefix + keyVelX, *stateVectorList[i], "velocity/x");
         addMandatory(theProductKwl, orbit_prefix + keyVelY, *stateVectorList[i], "velocity/y");
         addMandatory(theProductKwl, orbit_prefix + keyVelZ, *stateVectorList[i], "velocity/z");
      }

      add(theProductKwl,"orbitList.nb_orbits", static_cast<ossim_uint32>(stateVectorList_size));
   }

   bool ossimSentinel1Model::initImageSize(ossimIpt& imageSize) const
   {
      std::string const& samples_cstr = theProductKwl.findKey(SUPPORT_DATA_PREFIX, ossimKeywordNames::NUMBER_SAMPLES_KW);
      std::string const& lines_cstr   = theProductKwl.findKey(SUPPORT_DATA_PREFIX, ossimKeywordNames::NUMBER_LINES_KW);

      imageSize.samp = to<int>(samples_cstr, "decoding sample number from KWL");
      imageSize.line = to<int>(lines_cstr, "decoding line number from KWL");

      return true;
   }

   double ossimSentinel1Model::getBandTerrainHeight(ossimXmlDocument const& productXmlDocument)
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

   void ossimSentinel1Model::addDopplerCentroidCoefficients(ossimXmlNode const& dcEstimateList)
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
            s_printf(prefix, "dopplerCentroid.dop_coef_list%d.", index);

            const ossimXmlNodePtr & dcEstimate = *it;
            assert(dcEstimate.get());
            addOptional(theProductKwl, prefix, "dop_coef_time",  *dcEstimate, "azimuthTime");
            //RK
            const double ref_time = getOptionalTextFromFirstNode(*dcEstimate, "t0").toFloat64() * 1e9; // s to ns
            add(theProductKwl, prefix, keySlantRangeTime, ref_time);

            ossimString const& ns = getOptionalTextFromFirstNode(*dcEstimate, "ns");

            if( !ns.empty() )
               add(theProductKwl, prefix, keySlantRangeTime, ns);

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
                  s_printf(coeff_prefix, "%s%d.dop_coef", prefix, count);

                  add(theProductKwl, coeff_prefix, coeff->string());
               }

            } //if (!coeffStr.empty())

         } // for each dcEstimate

      } // else count < 1
   }

   bool ossimSentinel1Model::openMetadataFile(ossimXmlDocument& doc, ossimString const& file) const
   {
      if ( !doc.openFile( file ) )
      {
         ossimNotify(ossimNotifyLevel_FATAL) << "Cannot open metadata file `" << file << "`\n";
         return false;
      }

      return true;
   }

   void ossimSentinel1Model::imagingRay(const ossimDpt& image_point, ossimEcefRay&   image_ray) const
   {
      // NOT YET IMPLEMENTED
      setErrorStatus();
   }

} //end namespace
