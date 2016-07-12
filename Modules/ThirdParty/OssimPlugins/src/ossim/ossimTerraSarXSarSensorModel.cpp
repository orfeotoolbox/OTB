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

#include "ossimTerraSarXSarSensorModel.h"
#include "ossimTraceHelpers.h"
#include "ossimXmlTools.h"
#include "ossimKeyWordListUtilities.h"
#include "ossimSarSensorModelPathsAndKeys.h"
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimKeywordNames.h>
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
   ossimTrace traceExec  ("ossimTerraSarXSarSensorModel:exec");
   ossimTrace traceDebug ("ossimTerraSarXSarSensorModel:debug");
   const ossimString attTimeUTC     = "timeUTC";
   const ossimString attPosX        = "posX";
   const ossimString attPosY        = "posY";
   const ossimString attPosZ        = "posZ";
   const ossimString attVelX        = "velX";
   const ossimString attVelY        = "velY";
   const ossimString attVelZ        = "velZ";
   const ossimString attT           = "t";
   const ossimString attTau         = "tau";
   const ossimString attCol         = "col";
   const ossimString attRow         = "row";
   const ossimString attLat         = "lat";
   const ossimString attLon         = "lon";
   const ossimString attHeight      = "height";
   const ossimString attAzimuthTime = "azimuthTime";

   const ossimString attProductInfo        = "productInfo";
   const ossimString attProductVariantInfo = "productVariantInfo";
   const ossimString attProductVariant     = "productVariant";
   const ossimString attSceneInfo          = "sceneInfo";
   const ossimString attSettings           = "instrument/settings";
   const ossimString attProductSpecific    = "productSpecific";
}// Anonymous namespace

namespace ossimplugins {
   RTTI_DEF1(ossimTerraSarXSarSensorModel, "ossimTerraSarXSarSensorModel", ossimSarSensorModel);
} // ossimplugins namespace

void ossimplugins::ossimTerraSarXSarSensorModel::readAnnotationFile(const std::string & annotationXml, const std::string & geoXml)
{
    ossimXmlDocument xmlDoc(annotationXml);
    ossimRefPtr<ossimXmlNode> xRoot = xmlDoc.getRoot();
    if (!xRoot) {
       throw std::runtime_error("No root document found for TerraSarX annotation file "+annotationXml);
    }
    const ossimXmlNode & xmlRoot = *xRoot;

   // Product type
    ossimString const& product_type = getTextFromFirstNode(xmlRoot, "productInfo/productVariantInfo/productVariant");
    ossimNotify(ossimNotifyLevel_DEBUG) << "Product type: " <<  product_type << '\n';
    theProductType = ProductType(product_type);

    // First, lookup position/velocity records
    std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
    xmlDoc.findNodes("/level1Product/platform/orbit/stateVec",xnodes);

    ossimNotify(ossimNotifyLevel_DEBUG) << "Number of states " << xnodes.size() << '\n';

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
        ossimNotify(ossimNotifyLevel_DEBUG) << "Add theOrbitRecords\n";
        theOrbitRecords.push_back(orbitRecord);
    }

    //Parse the near range time (in seconds)
    theNearRangeTime = getDoubleFromFirstNode(xmlRoot, "productInfo/sceneInfo/rangeTime/firstPixel");
    ossimNotify(ossimNotifyLevel_DEBUG) << "theNearRangeTime " << theNearRangeTime << '\n';

    //Parse the range sampling rate
    theRangeSamplingRate = getDoubleFromFirstNode(xmlRoot, "instrument/settings/RSF");
    ossimNotify(ossimNotifyLevel_DEBUG) << "theRangeSamplingRate " << theRangeSamplingRate << '\n';

    //Parse the range resolution
    theRangeResolution = getDoubleFromFirstNode(xmlRoot, "productSpecific/complexImageInfo/slantRangeResolution");
    ossimNotify(ossimNotifyLevel_DEBUG) << "theRangeResolution " << theRangeResolution << '\n';

    //Parse the radar frequency
    theRadarFrequency = getDoubleFromFirstNode(xmlRoot, "instrument/settings/settingRecord/PRF");
    ossimNotify(ossimNotifyLevel_DEBUG) << "theRadarFrequency " << theRadarFrequency << '\n';

    //Manage only strip map product for now (one burst)

    //Parse azimuth time start/stop
    const TimeType azimuthTimeStart = getTimeFromFirstNode(xmlRoot, "productInfo/sceneInfo/start/timeUTC");

    ossimNotify(ossimNotifyLevel_DEBUG) << "azimuthTimeStart " << azimuthTimeStart << '\n';

    const TimeType azimuthTimeStop = getTimeFromFirstNode(xmlRoot, "productInfo/sceneInfo/stop/timeUTC");

    ossimNotify(ossimNotifyLevel_DEBUG) << "azimuthTimeStop " << azimuthTimeStop << '\n';

    const DurationType td = azimuthTimeStop - azimuthTimeStart;

    ossimNotify(ossimNotifyLevel_DEBUG) << "td " << td.total_microseconds() << '\n';

    // numberOfRows
    const unsigned int numberOfRows = getFromFirstNode<unsigned int>(xmlRoot, "productInfo/imageDataInfo/imageRaster/numberOfRows");

    ossimNotify(ossimNotifyLevel_DEBUG) << "numberOfRows " << numberOfRows << '\n';

    //Compute azimuth time interval
    theAzimuthTimeInterval = td / static_cast<double> (numberOfRows - 1);
    //theAzimuthTimeInterval = seconds(1./static_cast<double>(theRadarFrequency));

    ossimNotify(ossimNotifyLevel_DEBUG) << "theAzimuthTimeInterval " << theAzimuthTimeInterval.total_microseconds()  << " and 1/prf: " << (1. / theRadarFrequency) * 1000000. << '\n';

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

        ossimNotify(ossimNotifyLevel_DEBUG) << "Number of coefficients " << polynomialDegree << '\n';

        ossimString path = "/level1Product/productSpecific/projectedImageInfo/slantToGroundRangeProjection/coefficient";
        const ossimString EXP = "exponent";
        ossimString s;

        xmlDoc.findNodes(path, xnodes);

        if ( xnodes.size() )
        {
            for (unsigned int i = 0; i < xnodes.size(); ++i)
            {
                if (xnodes[i].valid())
                {
                    xnodes[i]->getAttributeValue(s, EXP);
                    const double coeff = xnodes[i]->getText().toDouble();
                    coordRecord.coefs.push_back(coeff);
                    ossimNotify(ossimNotifyLevel_DEBUG) << "Coef number " << i << " value: " << coeff << '\n';
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

    ossimNotify(ossimNotifyLevel_DEBUG)<<"Found "<<xnodes.size()<<" GCPs\n";

    for(std::vector<ossimRefPtr<ossimXmlNode> >::iterator itNode = xnodes.begin(); itNode!=xnodes.end();++itNode)
    {
        GCPRecordType gcpRecord;

        // Get delta acquisition time
        const double deltaAzimuth = getDoubleFromFirstNode(**itNode, attT);
        gcpRecord.azimuthTime = azimuthTimeStart + microseconds(deltaAzimuth * 1000000.);

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

/*virtual*/
bool ossimplugins::ossimTerraSarXSarSensorModel::open(const ossimFilename& file)
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarXSarSensorModel::open";
   // traceDebug.setTraceFlag(true);
   // traceExec .setTraceFlag(true);
   SCOPED_LOG(traceDebug, MODULE);
   const ossimString ext = file.ext().downcase();
   if ( !file.exists() || (ext != "tiff" && ext != "xml" ))
   {
      return false;
   }
   else
   {
      theGSD.makeNan();

      // -----[ Read product file
      ossimFilename xmlFileName = file;

      // If this is tiff file, look for corresponding annotation file
      if(ext != "xml")
      {
         // TODO: update relative path to annotation file
         const ossimFilename fileNameWihtoutExtension = file.fileNoExtension();
         const ossimFilename path = file.path().path();
         xmlFileName = ossimFilename(path+"/annotation/"+fileNameWihtoutExtension+".xml");
      }

      if ( !xmlFileName.exists() || !this->readProduct(xmlFileName) )
      {
         ossimNotify(ossimNotifyLevel_FATAL) << MODULE << " open "<<xmlFileName<<" failed\n";
         return false;
      }

#if 0
      // TODO: check about theMeanGSD and theGSD
      if ( !this->initImageSize( theImageSize ) )
      {
         ossimNotify(ossimNotifyLevel_FATAL) << MODULE << " this->initImageSize( theImageSize ) fails\n";
         return false;
      }
#endif

      theImageClipRect = ossimDrect( 0, 0, theImageSize.x-1, theImageSize.y-1 );
      theSubImageOffset.x = 0.0;
      theSubImageOffset.y = 0.0;

      // automatically loaded/saved into ossimSensorModel
      theMeanGSD = (theGSD.x + theGSD.y)/2.0;
   }
   return true;
}

/*virtual*/
ossimObject* ossimplugins::ossimTerraSarXSarSensorModel::dup() const
{
   return new ossimTerraSarXSarSensorModel(*this);
}

/*virtual*/
std::ostream& ossimplugins::ossimTerraSarXSarSensorModel::print(std::ostream& out) const
{
   // Capture stream flags since we are going to mess with them.
   std::ios_base::fmtflags f = out.flags();

   out << "\nDump of ossimTerraSarXSarSensorModel at address " << hex << this
      << dec
      // TODO factorize these data back into ossimSarSensorModel (update
      // ossimSentinel1Model in consequence)
      << "\n------------------------------------------------"
      << "\n  theImageID            = " << theImageID
      << "\n  theImageSize          = " << theImageSize
      << "\n------------------------------------------------"
      << "\n" << endl;

   // Set the flags back.
   out.flags(f);
   return ossimSarSensorModel::print(out);
}

/*virtual*/
bool ossimplugins::ossimTerraSarXSarSensorModel::saveState(
      ossimKeywordlist& kwl, const char* prefix/* = NULL */) const
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarXSarSensorModel::saveState";
   SCOPED_LOG(traceDebug, MODULE);

   add(kwl, prefix, ossimKeywordNames::TYPE_KW, "ossimTerraSarXSarSensorModel");
   add(kwl, "support_data.calibration_lookup_flag", "false");

   // kwl.addList(theManifestKwl, true);
   kwl.addList(theProductKwl,  true);

   const bool success = ossimSarSensorModel::saveState(kwl, prefix);
   ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << (success ? " success!" : " failure!") << '\n';
   ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << ": " << kwl.getSize() << " keywords saved.\n";
   return success;
}

/*virtual*/
bool ossimplugins::ossimTerraSarXSarSensorModel::loadState(ossimKeywordlist const& kwl, const char* prefix/* = NULL */)
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarXSarSensorModel::loadState";
   SCOPED_LOG(traceDebug, MODULE);

#if 0
   theManifestKwl.addList(kwl, true);

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "theManifestKwl.getSize()" << theManifestKwl.getSize() << "\n";
   }
#endif

   return ossimSarSensorModel::loadState(kwl, prefix);
}

bool ossimplugins::ossimTerraSarXSarSensorModel::readProduct(ossimFilename const & productXmlFile)
{
   try
   {
      const bool ret = read(productXmlFile);
      if ( ret )
      {
#if 0
         readCalibrationMetadata();
         readNoiseMetadata();
#endif
         return true;
      }
   } catch (std::exception const& e) {
      if (traceExec())
         ossimNotify(ossimNotifyLevel_INFO)
            << "Error while reading " << productXmlFile << " as TerraSarX model: " << e.what() << "\n";
   }
   if (traceExec())
      ossimNotify(ossimNotifyLevel_FATAL)
         << "Cannot read " << productXmlFile << " as TerraSarX model.\n";
   return false;
}

bool ossimplugins::ossimTerraSarXSarSensorModel::read(ossimFilename const& annotationXml)
{
   ossimXmlDocument xmlDoc(annotationXml);
   ossimRefPtr<ossimXmlNode> xRoot = xmlDoc.getRoot();
   if (!xRoot) {
      throw std::runtime_error("No root document found for TerraSarX annotation file "+annotationXml);
   }
   const ossimXmlNode & xmlRoot = *xRoot;

   ossimXmlNode const& productInfo        = getExpectedFirstNode(xmlRoot, attProductInfo);
   ossimXmlNode const& productVariantInfo = getExpectedFirstNode(productInfo, attProductVariantInfo);
   ossimXmlNode const& sceneInfo          = getExpectedFirstNode(productInfo, attSceneInfo);
   ossimXmlNode const& settings           = getExpectedFirstNode(xmlRoot, attSettings);
   ossimXmlNode const& productSpecific    = getExpectedFirstNode(xmlRoot, attProductSpecific);

   // Product type
   ossimString  const& productType        = getTextFromFirstNode(productVariantInfo, "productVariant");
   ossimNotify(ossimNotifyLevel_DEBUG) << "Product type: " <<  productType << '\n';
   theProductType = ProductType(productType);
   add(theProductKwl, SUPPORT_DATA_PREFIX, "product_type", productType);

   // First, lookup position/velocity records
   ossimRefPtr<ossimXmlNode> orbitList = xmlRoot.findFirstNode("platform/orbit");
   if (!orbitList || 0 == addOrbitStateVectors(*orbitList)) {
      add(theProductKwl,"orbitList.nb_orbits", "0");
      ossimNotify(ossimNotifyLevel_DEBUG) << "No orbitVectorList info available in metadata!!\n";
   }

   //Parse the near range time (in seconds)
   const ossimString & sNearRangeTime = addMandatory(theProductKwl, SUPPORT_DATA_PREFIX, "slant_range_to_first_pixel", sceneInfo, "rangeTime/firstPixel");

   theNearRangeTime  = to<double>(sNearRangeTime, " extracting slant_range_to_first_pixel from productInfo/sceneInfo/rangeTime/firstPixel field");
   ossimNotify(ossimNotifyLevel_DEBUG) << "theNearRangeTime " << theNearRangeTime << '\n';

   //Parse the range sampling rate
   ossimString const& sRangeSamplingRate = addMandatory(theProductKwl, SUPPORT_DATA_PREFIX, "range_sampling_rate",        settings, "RSF");
   theRangeSamplingRate = to<double>(sRangeSamplingRate, " extracting range_sampling_rate from instrument/settings/RSF field");


   //Parse the range resolution
   theRangeResolution = getDoubleFromFirstNode(productSpecific, "complexImageInfo/slantRangeResolution");
   add(theProductKwl, SUPPORT_DATA_PREFIX, "range_spacing", theRangeResolution);
   theGSD.x = theRangeResolution; // TODO: check this must be maintained
   // Originally, theGSD.x was obtained from
   // "/level1Product/productSpecific/complexImageInfo/projectedSpacingRange/slantRange";
   ossimNotify(ossimNotifyLevel_DEBUG) << "theRangeResolution " << theRangeResolution << '\n';

   //Parse the radar frequency
   ossimString const& sRadarFrequency = addMandatory(theProductKwl, SUPPORT_DATA_PREFIX, "radar_frequency",            settings, "settingRecord/PRF");
   theRadarFrequency = to<double>(sRadarFrequency, " extracting radar_frequency from instrument/settings/settingRecord/PRF field");
   ossimNotify(ossimNotifyLevel_DEBUG) << "theRadarFrequency " << theRadarFrequency << '\n';

   //Manage only strip map product: i.e. TerraSarX has only one burst unlike S1

   // With Terrasar-X there is only one burst
   add(theProductKwl, BURST_NUMBER_KEY,    ossimString("1"));
   add(theProductKwl, BURST_PREFIX, "[0].start_line", 0);
   // With this way of procedding, the time points are not altered, they are
   // forwarded exactly as written initially
   const TimeType azimuthTimeStart = time::toModifiedJulianDate(addMandatory(theProductKwl, BURST_PREFIX, "[0].azimuth_start_time", sceneInfo, "start/timeUTC"));
   const TimeType azimuthTimeStop  = time::toModifiedJulianDate(addMandatory(theProductKwl, BURST_PREFIX, "[0].azimuth_stop_time",  sceneInfo, "stop/timeUTC"));
   const double end_line           = add(theProductKwl, BURST_PREFIX, "[0].end_line",           getFromFirstNode<unsigned int>(productInfo, "imageDataInfo/imageRaster/numberOfRows") - 1);

   // theAzimuthTimeInterval needs to be computed from:
   // - azimuth time start/stop difference
   const DurationType td = azimuthTimeStop - azimuthTimeStart;
   ossimNotify(ossimNotifyLevel_DEBUG) << "td: " << td.total_microseconds() << "us = " << azimuthTimeStop << " - " << azimuthTimeStart << '\n';
   // - numberOfRows
   ossimNotify(ossimNotifyLevel_DEBUG) << "numberOfRows " << (end_line + 1) << '\n';

   // -> azimuth time interval, stored in seconds!
   theAzimuthTimeInterval = td / end_line; // end_line is already a double
   //theAzimuthTimeInterval = seconds(1./static_cast<double>(theRadarFrequency));
   add(theProductKwl, SUPPORT_DATA_PREFIX, "line_time_interval", theAzimuthTimeInterval.total_seconds());
   ossimNotify(ossimNotifyLevel_DEBUG) << "theAzimuthTimeInterval " << theAzimuthTimeInterval.total_microseconds()  << " and 1/prf: " << (1. / theRadarFrequency) * 1000000. << '\n';

   //GRD (detected product)
#if 0
   if(isGRD())
   {
      //Retrieve Slant Range to Ground range coeddifcients
      CoordinateConversionRecordType coordRecord;

      //Get azimuth time start (again)
      coordRecord.azimuthTime = azimuthTimeStart;

      //Set ground range origin to 0 (FIXME?)
      coordRecord.rg0 = 0.;

      //Read  coefficients
      const unsigned int polynomialDegree = xmlRoot.findFirstNode("productSpecific/projectedImageInfo/slantToGroundRangeProjection/polynomialDegree")->getText().toUInt16();

      ossimNotify(ossimNotifyLevel_DEBUG) << "Number of coefficients " << polynomialDegree << '\n';

      ossimString path = "/level1Product/productSpecific/projectedImageInfo/slantToGroundRangeProjection/coefficient";
      const ossimString EXP = "exponent";
      ossimString s;

      xmlDoc.findNodes(path, xnodes);

      if ( xnodes.size() )
      {
         for (unsigned int i = 0; i < xnodes.size(); ++i)
         {
            if (xnodes[i].valid())
            {
               xnodes[i]->getAttributeValue(s, EXP);
               const double coeff = xnodes[i]->getText().toDouble();
               coordRecord.coefs.push_back(coeff);
               ossimNotify(ossimNotifyLevel_DEBUG) << "Coef number " << i << " value: " << coeff << '\n';
            }
         }
      }
      assert(!coordRecord.coefs.empty()&&"The srgr record has empty coefs vector.");

      theSlantRangeToGroundRangeRecords.push_back(coordRecord);
   }
#endif

   //Parse GCPs
   const ossimFilename geoXml = searchGeoRefFile(annotationXml);
   ossimXmlDocument xmlGeo(geoXml);
   ossimRefPtr<ossimXmlNode> geoXmlRoot = xmlGeo.getRoot();
   if (xmlGeo.hasError() || !xRoot) {
      throw std::runtime_error("Cannot read geo-ref document found for TerraSarX: "+geoXml);
   }
   readGeoLocationGrid(*geoXmlRoot, azimuthTimeStart, theNearRangeTime);
   // TODO: metadata file ?
   return true;
}

std::size_t ossimplugins::ossimTerraSarXSarSensorModel::addOrbitStateVectors(
      ossimXmlNode const& orbitList)
{
   std::vector<ossimRefPtr<ossimXmlNode> > stateVectorList;
   orbitList.findChildNodes("stateVec",stateVectorList);
   ossimNotify(ossimNotifyLevel_DEBUG) << "Number of states " << stateVectorList.size() << '\n';

   const std::size_t stateVectorList_size = stateVectorList.size();
   char orbit_prefix_[256];
   for (std::size_t i = 0; i != stateVectorList_size ; ++i)
   {
      //orbit_state_vectors
      const int pos = s_printf(orbit_prefix_, "orbitList.orbit[%d].", int(i));
      assert(pos > 0 && pos < sizeof(orbit_prefix_));
      const std::string orbit_prefix(orbit_prefix_, pos);

      // Store acquisition time
      addMandatory(theProductKwl, orbit_prefix + keyTime,  *stateVectorList[i], attTimeUTC);

      // Retrieve ECEF position
      addMandatory(theProductKwl, orbit_prefix + keyPosX, *stateVectorList[i], attPosX);
      addMandatory(theProductKwl, orbit_prefix + keyPosY, *stateVectorList[i], attPosY);
      addMandatory(theProductKwl, orbit_prefix + keyPosZ, *stateVectorList[i], attPosZ);

      // Retrieve ECEF velocity
      addMandatory(theProductKwl, orbit_prefix + keyVelX, *stateVectorList[i], attVelX);
      addMandatory(theProductKwl, orbit_prefix + keyVelY, *stateVectorList[i], attVelY);
      addMandatory(theProductKwl, orbit_prefix + keyVelZ, *stateVectorList[i], attVelZ);
   }

   add(theProductKwl,"orbitList.nb_orbits", stateVectorList_size);
   return stateVectorList_size;
}

void ossimplugins::ossimTerraSarXSarSensorModel::readGeoLocationGrid(
      ossimXmlNode const& productRoot,
      TimeType     const& azimuthTimeStart,
      double       const  nearRangeTime)
{
   SCOPED_LOG(traceDebug, "ossimTerraSarXSarSensorModel::readGeoLocationGrid");
   char prefix[1024];
   std::vector<ossimRefPtr<ossimXmlNode> > xnodes;

   productRoot.findChildNodes("geolocationGrid/gridPoint",xnodes);

   ossimNotify(ossimNotifyLevel_DEBUG)<<"Found "<<xnodes.size()<<" GCPs\n";

   unsigned int idx = 0;
   for(std::vector<ossimRefPtr<ossimXmlNode> >::iterator itNode = xnodes.begin(); itNode!=xnodes.end();++itNode, ++idx)
   {
      const int pos = s_printf(prefix, "%s[%d].", GCP_PREFIX.c_str(), idx);
      assert(pos >= sizeof(SR_PREFIX)+4 && pos < sizeof(prefix));

      // Get delta acquisition time
      const double deltaAzimuth = getDoubleFromFirstNode(**itNode, attT);
      const TimeType azimuthTime = azimuthTimeStart + seconds(deltaAzimuth);
      add(theProductKwl, prefix, attAzimuthTime, azimuthTime);

      //Get delta range time
      // is seconds
      add(theProductKwl, prefix, keySlantRangeTime, nearRangeTime + getDoubleFromFirstNode(**itNode, attTau));

      add(theProductKwl, prefix, keyImPtX, getDoubleFromFirstNode(**itNode, attCol) - 1.);
      add(theProductKwl, prefix, keyImPtY, getDoubleFromFirstNode(**itNode, attRow) - 1.);

      addMandatory(theProductKwl, prefix, keyWorldPtLat, **itNode, attLat);
      addMandatory(theProductKwl, prefix, keyWorldPtLon, **itNode, attLon);
      addMandatory(theProductKwl, prefix, keyWorldPtHgt, **itNode, attHeight);
   }
   add(theProductKwl, GCP_NUMBER_KEY, idx);
}

ossimFilename ossimplugins::ossimTerraSarXSarSensorModel::searchGeoRefFile(ossimFilename const& file) const
{
   const ossimFilename geoRefFile = ossimFilename(file.path() + "/GEOREF.xml");
   if (!geoRefFile.exists()) {
      throw std::runtime_error("Cannot find GEOREF.xml file alongside "+file);
   }
   ossimNotify(ossimNotifyLevel_DEBUG) << "Found georef file: " << geoRefFile << '\n';
   return geoRefFile;
}

