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
#include <ossim/base/ossimEnvironmentUtility.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimPolynom.h>

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

   const ossimString attProductInfo          = "productInfo";
   const ossimString attProductVariantInfo   = "productVariantInfo";
   const ossimString attProductVariant       = "productVariant";
   const ossimString attSceneInfo            = "sceneInfo";
   const ossimString attImageDataInfo        = "imageDataInfo";
   const ossimString attImageRaster          = "imageRaster";
   const ossimString attInstrument           = "instrument";
   const ossimString attSettings             = "settings"; // instrument/settings
   const ossimString attProductSpecific      = "productSpecific";
   const ossimString attComplexImageInfo     = "complexImageInfo";
   const ossimString attGeneralHeader        = "generalHeader";
   const ossimString attSceneCenterCoord     = "sceneCenterCoord";
   const ossimString attSceneCornerCoord     = "sceneCornerCoord";
   const ossimString attRangeTime            = "rangeTime";
   const ossimString attPolLayer             = "polLayer";
   const ossimString attImageData            = "/level1Product/productComponents/imageData";
   const ossimString attPolLayerFileName     = "file/location/filename";
   const ossimString attAcquisitionInfo      = "acquisitionInfo";
   const ossimString attProcessingParameter  = "processing/processingParameter";
   const ossimString attMissionInfo          = "missionInfo";

   const std::string ACQUISITION_INFO        = "acquisitionInfo.";
   const std::string POLARISATION_LIST       = "polarisationList";
   const std::string SIZE                    = ".size";
   const std::string KW_POLLIST              = ACQUISITION_INFO + POLARISATION_LIST;
   const std::string CALIBRATION_CALFACTOR   = "calibration.calibrationConstant.calFactor";

   inline bool isnan(ossimGpt const& p) {
      return ossim::isnan(p.latd()) || ossim::isnan(p.lond()) || ossim::isnan(p.height());
   }

}// Anonymous namespace

namespace ossimplugins {
   RTTI_DEF1(ossimTerraSarXSarSensorModel, "ossimTerraSarXSarSensorModel", ossimSarSensorModel);
} // ossimplugins namespace

/** Data aggregates that contains direct access to nodes.
 * While accessing to XML information through an xpath is more user friendly,
 * it's also much less efficient. That's why is approach has been chosen
 * instead.
 */
struct ossimplugins::ossimTerraSarXSarSensorModel::Nodes
{
   Nodes(ossimXmlNode const& xmlRoot)
      : productInfo         (getExpectedFirstNode(xmlRoot,            attProductInfo))
      , productVariantInfo  (getExpectedFirstNode(productInfo,        attProductVariantInfo))
      , sceneInfo           (getExpectedFirstNode(productInfo,        attSceneInfo))
      , sceneCenterCoord    (getExpectedFirstNode(sceneInfo,          attSceneCenterCoord))
      , rangeTime           (getExpectedFirstNode(sceneInfo,          attRangeTime))
      , imageDataInfo       (getExpectedFirstNode(productInfo,        attImageDataInfo))
      , imageRaster         (getExpectedFirstNode(imageDataInfo,      attImageRaster))
      , instrument          (getExpectedFirstNode(xmlRoot,            attInstrument))
      , settings            (getExpectedFirstNode(instrument,         attSettings))
      , productSpecific     (getExpectedFirstNode(xmlRoot,            attProductSpecific))
      , complexImageInfo    (getExpectedFirstNode(productSpecific,    attComplexImageInfo))
      , generalHeader       (getExpectedFirstNode(xmlRoot,            attGeneralHeader))
      , acquisitionInfo     (getExpectedFirstNode(productInfo,        attAcquisitionInfo))
      , processingParameter (getExpectedFirstNode(xmlRoot,            attProcessingParameter))
      , missionInfo         (getExpectedFirstNode(productInfo,        attMissionInfo))
      {}

   ossimXmlNode const& productInfo;
   ossimXmlNode const& productVariantInfo;
   ossimXmlNode const& sceneInfo;
   ossimXmlNode const& sceneCenterCoord;
   ossimXmlNode const& rangeTime;
   ossimXmlNode const& imageDataInfo;
   ossimXmlNode const& imageRaster;
   ossimXmlNode const& instrument;
   ossimXmlNode const& settings;
   ossimXmlNode const& productSpecific;
   ossimXmlNode const& complexImageInfo;
   ossimXmlNode const& generalHeader;
   ossimXmlNode const& acquisitionInfo;
   ossimXmlNode const& processingParameter;
   ossimXmlNode const& missionInfo;
private:
   Nodes(Nodes const&);
   Nodes& operator=(Nodes const&);
};

ossimplugins::ossimTerraSarXSarSensorModel::ossimTerraSarXSarSensorModel()
   : m_polLayerName("UNDEFINED")
{}

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
        //Retrieve Slant Range to Ground range coefficients
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
   if (traceDebug())
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " try to open "<<file<<"\n";

   try
   {
      theGSD.makeNan();

      // -----[ Read product file
      assert(file.exists());
      if ( !this->read(file) )
      {
         if (traceDebug())
            ossimNotify(ossimNotifyLevel_DEBUG) << file << " doesn't have a TerraSarX model.\n";
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

      return true;
   } catch (std::exception const& e) {
      ossimNotify(ossimNotifyLevel_FATAL)
         << "Error while reading " << file << " as having a TerraSarX model: " << e.what() << "\n";
   }
   return false;
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

   out << "\nDump of ossimTerraSarXSarSensorModel at address " << hex << this << dec << endl;

   // Set the flags back.
   out.flags(f);
   return ossimSarSensorModel::print(out);
}

/*virtual*/
bool ossimplugins::ossimTerraSarXSarSensorModel::saveState(
      ossimKeywordlist& kwl, const char* prefix_/* = NULL */) const
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarXSarSensorModel::saveState";
   SCOPED_LOG(traceDebug, MODULE);

   std::string const prefix = prefix_ ? prefix_ : "";
   // support_data.calibration_lookup_flag is not mandatory on OTB side, but
   // let's be explicit.
   add(kwl, "support_data.calibration_lookup_flag", "false");

   // polLayers + calfactors
   add(kwl, prefix, KW_POLLIST + SIZE, m_polLayerList.size());
   add(kwl, prefix, ACQUISITION_INFO + "polarisationLayerName", m_polLayerName);
   for (unsigned int i=0, N=m_polLayerList.size(); i!=N ; ++i)
   {
      add(kwl, prefix, KW_POLLIST + '['+ossimString::toString(i)+']', m_polLayerList[i]);
      add(kwl, prefix, CALIBRATION_CALFACTOR, m_calFactor[i]);
   }

   m_sensorParams.saveState(kwl, prefix);
   m_sceneCoord.saveState(kwl, prefix);

   // noise
   if (m_polLayerName != "UNDEFINED")
   {
      const std::size_t polLayerIdx = findPolLayerIdx(m_polLayerName);
      m_noise[polLayerIdx].saveState(kwl, prefix);
   }
   else
   {
      for (std::size_t i=0, N=m_polLayerList.size(); i!=N ; ++i)
      {
         m_noise[i].saveState(kwl, prefix);
      }
   }

   // kwl.addList(theManifestKwl, true);
   kwl.addList(theProductKwl,  true);

   const bool success = ossimSarSensorModel::saveState(kwl, prefix_);
   ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << (success ? " success!" : " failure!") << '\n';
   ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << ": " << kwl.getSize() << " keywords saved.\n";
#if 1
   ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << ":\n" << kwl;
#endif
   return success;
}

/*virtual*/
bool ossimplugins::ossimTerraSarXSarSensorModel::loadState(ossimKeywordlist const& kwl, const char* prefix_/* = NULL */)
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarXSarSensorModel::loadState";
   // traceDebug.setTraceFlag(true);
   // traceExec .setTraceFlag(true);
   SCOPED_LOG(traceDebug, MODULE);
   std::string const prefix = prefix_ ? prefix_ : "";

   theOrbitRecords.clear();
   theGCPRecords.clear();
   theBurstRecords.clear();
   theBoundGndPolygon.clear();

   if (! ossimSarSensorModel::loadState(kwl, prefix_))
   {
      // If the parent class says this is not a TerraSarX product, then no need
      // to process further
      return false;
   }

   std::size_t nb_layers;
   std::string polarization;
   double calFactor;
   get(kwl, prefix, KW_POLLIST + SIZE, nb_layers);
   for (unsigned int i=0; i!=nb_layers ; ++i)
   {
      get(kwl, prefix, KW_POLLIST + '['+ossimString::toString(i)+']', polarization);
      m_polLayerList.push_back(polarization);
      get(kwl, prefix, CALIBRATION_CALFACTOR, calFactor);
      m_calFactor.push_back(calFactor);

   }

   m_sensorParams.loadState(kwl, prefix);
   m_sceneCoord.loadState(kwl, prefix);

   // noise
   get(kwl, prefix, ACQUISITION_INFO + "polarisationLayerName", m_polLayerName);
   if (m_polLayerName != "UNDEFINED")
   {
      m_noise.resize(m_polLayerList.size());
      const std::size_t polLayerIdx = findPolLayerIdx(m_polLayerName);
      m_noise[polLayerIdx].loadState(kwl, prefix);
   }
   else
   {
      Noise noise;
      for (std::size_t i=0, N=m_polLayerList.size(); i!=N ; ++i)
      {
         noise.loadState(kwl, prefix);
         m_noise.push_back(noise);
      }
   }

   return true;
}

bool ossimplugins::ossimTerraSarXSarSensorModel::read(ossimFilename const& file)
{ 
   SCOPED_LOG(traceDebug, "ossimTerraSarXSarSensorModel::read");

   ossimFilename annotationXml;
   try {
      annotationXml = findTSXLeader(file);
   } catch (std::exception const& e) {
      // not a TSX product: log what has been found and return "not a TSX
      // product"
      if (traceDebug())
         ossimNotify(ossimNotifyLevel_DEBUG) << "Not a TSX product: " << e.what() << "\n";
      return false;
   }

   add(theProductKwl, PRODUCT_XML_FILE_KW, annotationXml.string());

   ossimXmlDocument xmlDoc(annotationXml);
   ossimRefPtr<ossimXmlNode> xRoot = xmlDoc.getRoot();
   if (!xRoot)
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "No root document found for TerraSarX annotation file " << annotationXml << "\n";
      return false;
   }
   const ossimXmlNode & xmlRoot = *xRoot;

   if (xmlRoot.getTag() != "level1Product")
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "Root node is `" << xmlRoot.getTag() << "` and not `level1Product` => this is not a TSX product\n";
      return false;
   }
   // TODO check if there could be other a field that says: "OK this is a TSX
   // product!"

   // From here, we suppose this is really a TSX product, any error is an error
   // in the product annotation file.
   // An exception will be thrown on the first error encountered.
   // The way the projection factory is made (as a _chain factory_), other type
   // of products will be tested, but they are more than likelly to fail.

   Nodes nodes(xmlRoot);

   // Product type
   ossimString  const& productType        = getTextFromFirstNode(nodes.productVariantInfo, "productVariant");
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
   const ossimString & sNearRangeTime = addMandatory(theProductKwl, SUPPORT_DATA_PREFIX, "slant_range_to_first_pixel", nodes.sceneInfo, "rangeTime/firstPixel");

   theNearRangeTime  = to<double>(sNearRangeTime, " extracting slant_range_to_first_pixel from productInfo/sceneInfo/rangeTime/firstPixel field");
   ossimNotify(ossimNotifyLevel_DEBUG) << "theNearRangeTime " << theNearRangeTime << '\n';

   //Parse the range sampling rate
   ossimString const& sRangeSamplingRate = addMandatory(theProductKwl, SUPPORT_DATA_PREFIX, "range_sampling_rate",        nodes.settings, "RSF");
   theRangeSamplingRate = to<double>(sRangeSamplingRate, " extracting range_sampling_rate from instrument/settings/RSF field");


   //Parse the range resolution
   theRangeResolution = getDoubleFromFirstNode(nodes.complexImageInfo, "slantRangeResolution");
   add(theProductKwl, SUPPORT_DATA_PREFIX, "range_spacing", theRangeResolution);
   ossimNotify(ossimNotifyLevel_DEBUG) << "theRangeResolution " << theRangeResolution << '\n';

   //Parse the radar frequency
   ossimString const& sRadarFrequency = addMandatory(theProductKwl, SUPPORT_DATA_PREFIX, "radar_frequency",            nodes.settings, "settingRecord/PRF");
   theRadarFrequency = to<double>(sRadarFrequency, " extracting radar_frequency from instrument/settings/settingRecord/PRF field");
   ossimNotify(ossimNotifyLevel_DEBUG) << "theRadarFrequency " << theRadarFrequency << '\n';

   //Manage only strip map product: i.e. TerraSarX has only one burst unlike S1

   // With Terrasar-X there is only one burst
   add(theProductKwl, BURST_NUMBER_KEY,    ossimString("1"));
   add(theProductKwl, BURST_PREFIX, "[0].start_line", 0);
   // With this way of procedding, the time points are not altered, they are
   // forwarded exactly as written initially
   readAzimuthTimes(nodes); // sets theAzimuthTimeStart and Stop
   const TimeType azimuthTimeStart = time::toModifiedJulianDate(add(theProductKwl, BURST_PREFIX, "[0].azimuth_start_time", theAzimuthTimeStart));
   const TimeType azimuthTimeStop  = time::toModifiedJulianDate(add(theProductKwl, BURST_PREFIX, "[0].azimuth_stop_time",  theAzimuthTimeStop));
   theImageSize.y = getFromFirstNode<unsigned int>(nodes.imageRaster, "numberOfRows");
   const double end_line           = add(theProductKwl, BURST_PREFIX, "[0].end_line", theImageSize.y - 1);

   BurstRecordType burstRecord;
   burstRecord.startLine = 0;
   burstRecord.azimuthStartTime = azimuthTimeStart;
   burstRecord.azimuthStopTime = azimuthTimeStop;
   burstRecord.endLine = end_line;
   theBurstRecords.push_back(burstRecord);

   // theAzimuthTimeInterval needs to be computed from:
   // - azimuth time start/stop difference
   const DurationType td = azimuthTimeStop - azimuthTimeStart;
   // ossimNotify(ossimNotifyLevel_DEBUG) << "td: " << td.total_microseconds() << "us = " << azimuthTimeStop << " - " << azimuthTimeStart << '\n';
   // - numberOfRows
   ossimNotify(ossimNotifyLevel_DEBUG) << "numberOfRows " << (end_line + 1) << '\n';

   // -> azimuth time interval, stored in seconds!
   theAzimuthTimeInterval = td / end_line; // end_line is already a double
   //theAzimuthTimeInterval = seconds(1./static_cast<double>(theRadarFrequency));
   add(theProductKwl, SUPPORT_DATA_PREFIX, "line_time_interval", theAzimuthTimeInterval.total_seconds());
   ossimNotify(ossimNotifyLevel_DEBUG) << "theAzimuthTimeInterval " << theAzimuthTimeInterval.total_microseconds()  << " and 1/prf: " << (1. / theRadarFrequency) * 1000000. << '\n';

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
      const unsigned int polynomialDegree = xmlRoot.findFirstNode("productSpecific/projectedImageInfo/slantToGroundRangeProjection/polynomialDegree")->getText().toUInt16();

      ossimNotify(ossimNotifyLevel_DEBUG) << "Number of coefficients " << polynomialDegree << '\n';

      ossimString path = "/level1Product/productSpecific/projectedImageInfo/slantToGroundRangeProjection/coefficient";
      const ossimString EXP = "exponent";
      ossimString s;

      std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
      
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

      //Estimate GroundRangeToSlantRange coefficients from Slant Range to Ground
      //range coefficients

      //Estimate degree of GRToSR of degree 11
      const unsigned int deg = 11;

      //Parse the last pixel time (last column) available in TSX metadata

      //Parse the last pixel time (in seconds)
      const ossimString & sLastPixelTime = addMandatory(theProductKwl, SUPPORT_DATA_PREFIX, "slant_range_to_last_pixel", nodes.sceneInfo, "rangeTime/lastPixel");

      const double maxRangeTime = to<double>(sLastPixelTime, " extracting slant_range_to_last_pixel from productInfo/sceneInfo/rangeTime/lastPixel field");
      ossimNotify(ossimNotifyLevel_DEBUG) << "maxRangeTime " << maxRangeTime << '\n';

      const double referencePoint = xmlRoot.findFirstNode("productSpecific/projectedImageInfo/slantToGroundRangeProjection/referencePoint")->getText().toDouble();

      ossimNotify(ossimNotifyLevel_DEBUG) << "Reference point " << referencePoint << '\n';
      
      std::cout << "maxRangeTime " << maxRangeTime << '\n';
      //Estimate the polynom
      estimateGRToSRFromSRToGR(deg, maxRangeTime, referencePoint, azimuthTimeStart);

      //Check estimated polynoms
      //std::cout << "GRToSR coefs " << theGroundRangeToSlantRangeRecords[0].coefs[0] << std::endl; 
   }

   // Parse GCPs
   const ossimFilename geoXml = searchGeoRefFile(annotationXml);
   ossimXmlDocument xmlGeo(geoXml);
   ossimRefPtr<ossimXmlNode> geoXmlRoot = xmlGeo.getRoot();
   if (xmlGeo.hasError() || !xRoot) {
      throw std::runtime_error(("Cannot read geo-ref document found for TerraSarX: "+geoXml).string());
   }
   readGeoLocationGrid(*geoXmlRoot, azimuthTimeStart, theNearRangeTime);
   // TODO: metadata file ?

   // Beware, starting from KWL v2, the name follow snake_case
   addMandatory(theProductKwl, "radiometric_correction", nodes.productVariantInfo, "radiometricCorrection");
   // Fill other properties from ossimSensorModel
   initAcquisitionInfo(nodes);
   theSensorID      = getTextFromFirstNode(nodes.generalHeader, "mission");
   // theSensorID is added in parent class

   theImageID       = getTextFromFirstNode(nodes.sceneInfo, "sceneID");
   theImageSize.x   = getFromFirstNode<unsigned int>(nodes.imageRaster, "numberOfColumns");
   // theGSD
   if (isSSC()) {
      theGSD.x = getFromFirstNode<double>(nodes.complexImageInfo, "projectedSpacingRange/slantRange");
      theGSD.y = getFromFirstNode<double>(nodes.complexImageInfo, "projectedSpacingAzimuth");
   } else {
      // TODO: check values whether they aren't inverted
      // x <-> rowSpacing and y <-> columnSpacing is what was done in original
      // code
      theGSD.x = getFromFirstNode<double>(nodes.imageRaster, "rowSpacing");
      theGSD.y = getFromFirstNode<double>(nodes.imageRaster, "columnSpacing");
   }
   theRefGndPt.lat = getFromFirstNode<ossim_float64>(nodes.sceneCenterCoord, "lat");
   theRefGndPt.lon = getFromFirstNode<ossim_float64>(nodes.sceneCenterCoord, "lon");

   initSensorParams(nodes);
   theRefImgPt.x = getFromFirstNode<ossim_float64>(nodes.sceneCenterCoord, "refColumn") - 1.0;
   theRefImgPt.y = getFromFirstNode<ossim_float64>(nodes.sceneCenterCoord, "refRow")    - 1.0;

   // azimuth_start_time/stop
   add(theProductKwl, "azimuth_start_time", theAzimuthTimeStart);
   add(theProductKwl, "azimuth_stop_time",  theAzimuthTimeStop);

   addMandatory(theProductKwl, "range_first_time", nodes.rangeTime, "firstPixel");
   addMandatory(theProductKwl, "range_last_time", nodes.rangeTime, "lastPixel");
   addMandatory(theProductKwl, "generation_time", nodes.generalHeader, "generationTime");

   initNoise(xmlDoc);
   getPolLayerFromImageFile(xmlDoc, file);
   initCalibration(xmlDoc);
   assert(!m_calFactor.empty());
   add(theProductKwl, "calibration.calibrationConstant.calFactor", m_calFactor.back());

   addMandatory(theProductKwl, "radarFrequency", nodes.instrument, "radarParameters/centerFrequency");

   initSceneCoord(nodes);

   theImageClipRect = ossimDrect( 0, 0, theImageSize.x-1, theImageSize.y-1 );
   theSubImageOffset.x = 0.0;
   theSubImageOffset.y = 0.0;

   // automatically loaded/saved into ossimSensorModel
   theMeanGSD = (theGSD.x + theGSD.y)/2.0;

   // Assign the ossimSensorModel::theBoundGndPolygon
   // - TODO: Shouldn't we use the value already computed in /level1Product/productInfo/sceneInfo/sceneCornerCoord?
   //
   // - GCPRecords need to be loaded into C++ data in order to use lineSampleToWorld()
#if 0
   ossimGpt ul;
   ossimGpt ur;
   ossimGpt lr;
   ossimGpt ll;
   lineSampleToWorld(theImageClipRect.ul(), ul);
   lineSampleToWorld(theImageClipRect.ur(), ur);
   lineSampleToWorld(theImageClipRect.lr(), lr);
   lineSampleToWorld(theImageClipRect.ll(), ll);
#else
   assert(m_sceneCoord.size() == 4);
   ossimGpt ul(m_sceneCoord[0].get_lat(), m_sceneCoord[0].get_lon());
   ossimGpt ur(m_sceneCoord[1].get_lat(), m_sceneCoord[1].get_lon());
   ossimGpt ll(m_sceneCoord[2].get_lat(), m_sceneCoord[2].get_lon());
   ossimGpt lr(m_sceneCoord[3].get_lat(), m_sceneCoord[3].get_lon());
#endif
   // std::clog << "UL: " << theImageClipRect.ul() << " -> " << ul << "\n";
   // std::clog << "UR: " << theImageClipRect.ur() << " -> " << ur << "\n";
   // std::clog << "LR: " << theImageClipRect.lr() << " -> " << lr << "\n";
   // std::clog << "LL: " << theImageClipRect.ll() << " -> " << ll << "\n";
   if (!isnan(ul) && !isnan(ur) && !isnan(lr) && !isnan(ll)) {
      setGroundRect(ul, ur, lr, ll);  // ossimSensorModel method.
      // TODO: shouldn't we return false?
      // even if we'are able to orthorectify geometries
   }

   return true;
}

std::size_t ossimplugins::ossimTerraSarXSarSensorModel::addOrbitStateVectors(
      ossimXmlNode const& orbitList)
{
   theOrbitRecords.clear();
   std::vector<ossimRefPtr<ossimXmlNode> > stateVectorList;
   orbitList.findChildNodes("stateVec",stateVectorList);
   ossimNotify(ossimNotifyLevel_DEBUG) << "Number of states " << stateVectorList.size() << '\n';

   const std::size_t stateVectorList_size = stateVectorList.size();
   char orbit_prefix_[256];
   for (std::size_t i = 0; i != stateVectorList_size ; ++i)
   {
      OrbitRecordType orbitRecord;

      //orbit_state_vectors
      const int pos = s_printf(orbit_prefix_, "orbitList.orbit[%d].", int(i));
      assert(pos > 0 && pos < sizeof(orbit_prefix_));
      const std::string orbit_prefix(orbit_prefix_, pos);

      // Store acquisition time
      orbitRecord.azimuthTime = time::toModifiedJulianDate
         (addMandatory(theProductKwl, orbit_prefix + keyTime,  *stateVectorList[i], attTimeUTC));

      // Retrieve ECEF position
      orbitRecord.position[0] = to<double>(
            addMandatory(theProductKwl, orbit_prefix + keyPosX, *stateVectorList[i], attPosX),
            " extracting X position in orbit");
      orbitRecord.position[1] = to<double>(
            addMandatory(theProductKwl, orbit_prefix + keyPosY, *stateVectorList[i], attPosY),
            " extracting Y position in orbit");
      orbitRecord.position[2] = to<double>(
            addMandatory(theProductKwl, orbit_prefix + keyPosZ, *stateVectorList[i], attPosZ),
            " extracting Z position in orbit");

      // Retrieve ECEF velocity
      orbitRecord.velocity[0] = to<double>(
            addMandatory(theProductKwl, orbit_prefix + keyVelX, *stateVectorList[i], attVelX),
            " extracting X velocity in orbit");
      orbitRecord.velocity[1] = to<double>(
            addMandatory(theProductKwl, orbit_prefix + keyVelY, *stateVectorList[i], attVelY),
            " extracting Y velocity in orbit");
      orbitRecord.velocity[2] = to<double>(
            addMandatory(theProductKwl, orbit_prefix + keyVelZ, *stateVectorList[i], attVelZ),
            " extracting Z velocity in orbit");

      theOrbitRecords.push_back(orbitRecord);
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
   theGCPRecords.clear();
   char prefix[1024];
   std::vector<ossimRefPtr<ossimXmlNode> > xnodes;

   productRoot.findChildNodes("geolocationGrid/gridPoint",xnodes);

   ossimNotify(ossimNotifyLevel_DEBUG)<<"Found "<<xnodes.size()<<" GCPs\n";

   unsigned int idx = 0;
   for(std::vector<ossimRefPtr<ossimXmlNode> >::iterator itNode = xnodes.begin(); itNode!=xnodes.end();++itNode, ++idx)
   {
      GCPRecordType gcpRecord;
      const int pos = s_printf(prefix, "%s[%d].", GCP_PREFIX.c_str(), idx);
      assert(pos >= sizeof(SR_PREFIX)+4 && pos < sizeof(prefix));

      // Get delta acquisition time
      const double deltaAzimuth = getDoubleFromFirstNode(**itNode, attT);
      const TimeType azimuthTime = azimuthTimeStart + seconds(deltaAzimuth);
      add(theProductKwl, prefix, attAzimuthTime, azimuthTime);

      //Get delta range time
      // is seconds
      gcpRecord.slantRangeTime = theNearRangeTime + getDoubleFromFirstNode(**itNode, attTau);
      add(theProductKwl, prefix, keySlantRangeTime, gcpRecord.slantRangeTime);

      gcpRecord.imPt.x = getDoubleFromFirstNode(**itNode, attCol) - 1.;
      gcpRecord.imPt.y = getDoubleFromFirstNode(**itNode, attRow) - 1.;
      add(theProductKwl, prefix, keyImPtX, gcpRecord.imPt.x);
      add(theProductKwl, prefix, keyImPtY, gcpRecord.imPt.y);

      gcpRecord.worldPt.lat = to<double>(addMandatory(theProductKwl, prefix, keyWorldPtLat, **itNode, attLat),    " extracting latitude from " + attLat + " node");
      gcpRecord.worldPt.lon = to<double>(addMandatory(theProductKwl, prefix, keyWorldPtLon, **itNode, attLon),    " extracting longitude from " + attLon + " node");
      gcpRecord.worldPt.hgt = to<double>(addMandatory(theProductKwl, prefix, keyWorldPtHgt, **itNode, attHeight), " extracting height from " + attHeight + " node");

      theGCPRecords.push_back(gcpRecord);
   }
   add(theProductKwl, GCP_NUMBER_KEY, idx);
}

ossimFilename ossimplugins::ossimTerraSarXSarSensorModel::searchGeoRefFile(ossimFilename const& file) const
{
   ossimFilename geoRefFile(file.path() + "/ANNOTATION/GEOREF.xml");
   if (!geoRefFile.exists()) {
      // May be in same directory
      geoRefFile = file.path() + "/GEOREF.xml";
      if (!geoRefFile.exists()) {
         throw std::runtime_error(("Cannot find GEOREF.xml file alongside "+file).string());
      }
   }
   ossimNotify(ossimNotifyLevel_DEBUG) << "Found georef file: " << geoRefFile << '\n';
   return geoRefFile;
}

void ossimplugins::ossimTerraSarXSarSensorModel::readAzimuthTimes(Nodes const& nodes)
{
   theAzimuthTimeStart = getOptionalTextFromFirstNode(nodes.sceneInfo, "start/timeUTC").string();
   if (theAzimuthTimeStart.empty())
   {
      theAzimuthTimeStart = getOptionalTextFromFirstNode(nodes.settings, "rxGainSetting/startTimeUTC").string();
   }

   theAzimuthTimeStop = getOptionalTextFromFirstNode(nodes.sceneInfo, "stop/timeUTC").string();
   if (theAzimuthTimeStop.empty())
   {
      theAzimuthTimeStop = getOptionalTextFromFirstNode(nodes.settings, "rxGainSetting/stopTimeUTC").string();
   }
}

ossimFilename ossimplugins::ossimTerraSarXSarSensorModel::findTSXLeader(ossimFilename const& file)
{
   if ( file.exists() && (file.ext().downcase() == "xml") )
   {
      return file;
   }
   else if (!file.exists())
   {
      throw std::runtime_error(("Cannot find TSX metadata file from the non-existing file: "+file).string());
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimplugins::ossimTerraSarXSarSensorModel::findTSXLeader "
            << " directory scan turned off.  This is killing the factory open."
            << " We should never scan a directory.  Need to resolve.\n";
      }

#if 1
      ossimFilename imagePath = file.path();
      if (imagePath.empty())
         imagePath = ossimEnvironmentUtility::instance()->getCurrentWorkingDir();

      ossimDirectory directory(imagePath.path());
      std::vector<ossimFilename> vectName;
      const ossimString reg = ".xml";
      directory.findAllFilesThatMatch( vectName, reg, 1 );

      for (std::size_t loop = 0, N=vectName.size() ; loop!=N ; ++loop)
      {
         ossimFilename const& curFile = vectName[loop];
         if (starts_with(curFile.file(), "TSX"))
         {
            return curFile;
         }
      }
#endif
   }
   throw std::runtime_error(("Cannot find TSX metadata file from: "+file).string());
}

ossimString const& ossimplugins::ossimTerraSarXSarSensorModel::findPolLayerName(
      ossimXmlNode const& node, ossimString const& xpath) const
{
   std::vector<ossimRefPtr<ossimXmlNode> > sub_nodes; // TODO: reallocating this is not efficient
   node.findChildNodes(attPolLayer, sub_nodes);
   if (sub_nodes.empty())
   {
      throw std::runtime_error(("Could not find " + xpath + "/" + attPolLayer + " in TSX xml file.").string());
   }
   ossimString const& polLayerName = sub_nodes[0]->getText();
   return polLayerName;
}

std::size_t ossimplugins::ossimTerraSarXSarSensorModel::findPolLayerIdx(
      std::string const& polLayerName) const
{
   const std::vector<ossimString>::const_iterator b  = m_polLayerList.begin();
   const std::vector<ossimString>::const_iterator e  = m_polLayerList.end();
   const std::vector<ossimString>::const_iterator wh = std::find(b, e, polLayerName);

   if(wh == e)
   {
      throw std::runtime_error("Unable to found polLayer in polLayer List");
   }
   const std::size_t polLayerIdx = std::distance(b, wh);
   return polLayerIdx;
}

void ossimplugins::ossimTerraSarXSarSensorModel::getNoiseAtGivenNode(
   ossimXmlNode const& noiseNode, ossimplugins::Noise& noise)
{
   std::vector<ImageNoise> tabImageNoise;

   static const char MODULE[] = "ossimplugins::ossimTerraSarXSarSensorModel::getNoise";
   SCOPED_LOG(traceDebug, MODULE);

   static const ossimString attNumberOfNoiseRecords = "numberOfNoiseRecords";
   const std::size_t nbOfNoiseRecords = getFromFirstNode<std::size_t>(noiseNode, attNumberOfNoiseRecords);
   noise.set_numberOfNoiseRecords(nbOfNoiseRecords);

   static const ossimString attImageNoise           = "imageNoise";
   std::vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
   noiseNode.findChildNodes(attImageNoise, xml_nodes);
   if(xml_nodes.empty())
   {
      throw std::runtime_error("Could not find imageNoise node");
   }

   std::vector<ossimRefPtr<ossimXmlNode> >::iterator node = xml_nodes.begin();
   std::vector<ossimRefPtr<ossimXmlNode> > sub_nodes;
   while (node != xml_nodes.end())
   {
      ImageNoise ev;
      ev.set_timeUTC(getTextFromFirstNode(**node, attTimeUTC));

      sub_nodes.clear();
      static const ossimString attValidityRangeMin = "noiseEstimate/validityRangeMin";
      static const ossimString attValidityRangeMax = "noiseEstimate/validityRangeMax";
      static const ossimString attReferencePoint   = "noiseEstimate/referencePoint";
      static const ossimString attPolynomialDegree = "noiseEstimate/polynomialDegree";
      ev.set_validityRangeMin(getFromFirstNode<double>      (**node, attValidityRangeMin));
      ev.set_validityRangeMax(getFromFirstNode<double>      (**node, attValidityRangeMax));
      ev.set_referencePoint  (getFromFirstNode<double>      (**node, attReferencePoint));
      ev.set_polynomialDegree(getFromFirstNode<ossim_uint32>(**node, attPolynomialDegree));

      sub_nodes.clear();
      ossimXmlNode::ChildListType nodelist;
      (*node)->findChildNodes("noiseEstimate/coefficient",nodelist);
      ossimXmlNode::ChildListType::const_iterator child_iter = nodelist.begin();
      std::vector<double> polynomialCoefficients;
      while(child_iter != nodelist.end())
      {
         double coefficient = ((*child_iter)->getText()).toDouble() ;
         polynomialCoefficients.push_back(coefficient);
         ++child_iter;
      }
      ev.set_polynomialCoefficients( polynomialCoefficients );

      tabImageNoise.push_back(ev);

      ++node;
   }

   noise.set_imageNoise(tabImageNoise);
}

void ossimplugins::ossimTerraSarXSarSensorModel::initNoise(
      ossimXmlDocument const& xmlDocument)
{
   ossimString polLayerName;
   std::vector<ossimRefPtr<ossimXmlNode> > sub_nodes;

   static const char MODULE[] = "ossimplugins::ossimTerraSarXSarSensorModel::initNoise";
   SCOPED_LOG(traceDebug, MODULE);

   m_noise.resize(m_polLayerList.size());

   const ossimString xpath = "/level1Product/noise";

   std::vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
   xmlDocument.findNodes(xpath, xml_nodes);
   if (xml_nodes.empty())
   {
      throw std::runtime_error(("Could not find " + xpath + " in TSX xml file.").string());
   }

   std::vector<ossimRefPtr<ossimXmlNode> >::const_iterator node = xml_nodes.begin();
   for ( ; node != xml_nodes.end() ; ++node)
   {
      sub_nodes.clear();
      (*node)->findChildNodes(attPolLayer, sub_nodes);
      if (sub_nodes.empty())
      {
         throw std::runtime_error(("Could not find " + xpath + "/" + attPolLayer + " in TSX xml file.").string());
      }

      ossimString const& polLayerName = sub_nodes[0]->getText();
      const std::size_t polLayerIdx   = findPolLayerIdx(polLayerName);

      m_noise[polLayerIdx].set_imagePolarisation(polLayerName);

      getNoiseAtGivenNode(**node, m_noise[polLayerIdx]);
   }
}

void ossimplugins::ossimTerraSarXSarSensorModel::getPolLayerFromImageFile(
   ossimXmlDocument const& xmlDocument, const ossimFilename& imageFilename)
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarXSarSensorModel::getPolLayerFromImageFile";
   SCOPED_LOG(traceDebug, MODULE);

   std::vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
   xmlDocument.findNodes(attImageData, xml_nodes);
   if(xml_nodes.empty())
   {
      throw std::runtime_error(("Could not find " + attImageData + " node in XML metadata.").string());
   }

   std::vector<ossimRefPtr<ossimXmlNode> > sub_nodes;
   std::vector<ossimRefPtr<ossimXmlNode> >::const_iterator node = xml_nodes.begin();
   std::vector<ossimRefPtr<ossimXmlNode> >::const_iterator end  = xml_nodes.end();
   for ( ; node != xml_nodes.end() ; ++node)
   {
      ossimString const& polLayerFileName = getTextFromFirstNode(**node, attPolLayerFileName);

      if (polLayerFileName == imageFilename.file())
      {
         ossimString const& polLayerName = getTextFromFirstNode(**node, attPolLayer);
         m_polLayerName = polLayerName.string();
      }
   }
}

void ossimplugins::ossimTerraSarXSarSensorModel::initCalibration(
      ossimXmlDocument const& xmlDocument)
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarXSarSensorModel::initCalibration";
   SCOPED_LOG(traceDebug, MODULE);

   m_calFactor.resize(m_polLayerList.size());

   const ossimString xpath      = "/level1Product/calibration/calibrationConstant";
   const ossimString xCalFactor = "calFactor";

   std::vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
   xmlDocument.findNodes(xpath, xml_nodes);
   if(xml_nodes.empty())
   {
      throw std::runtime_error(("Could not find " + xpath + " in TSX xml file.").string());
   }

   std::vector<ossimRefPtr<ossimXmlNode> > sub_nodes;
   std::vector<ossimRefPtr<ossimXmlNode> >::const_iterator node = xml_nodes.begin();
   for ( ; node != xml_nodes.end() ; ++node)
   {
      ossimString const& polLayerName = findPolLayerName(**node, xpath);
      const std::size_t polLayerIdx   = findPolLayerIdx(polLayerName);

      sub_nodes.clear();
      (*node)->findChildNodes(xCalFactor, sub_nodes);
      if (sub_nodes.empty())
      {
         throw std::runtime_error(("Could not find " + xpath + "/" + attPolLayer + " in TSX xml file.").string());
      }
      m_calFactor[polLayerIdx] = sub_nodes[0]->getText().toDouble();
   }
}

void ossimplugins::ossimTerraSarXSarSensorModel::initSceneCoord(Nodes const& nodes)
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarXSarSensorModel::initSceneCoord";
   SCOPED_LOG(traceDebug, MODULE);

   InfoSceneCoord isc;

   isc.set_refRow        (getFromFirstNode<ossim_uint32>(nodes.sceneCenterCoord, "refRow"));
   isc.set_refColumn     (getFromFirstNode<ossim_uint32>(nodes.sceneCenterCoord, "refColumn"));
   isc.set_lat           (getFromFirstNode<double>      (nodes.sceneCenterCoord, "lat"));
   isc.set_lon           (getFromFirstNode<double>      (nodes.sceneCenterCoord, "lon"));
   isc.set_azimuthTimeUTC(getTextFromFirstNode          (nodes.sceneCenterCoord, "azimuthTimeUTC"));
   isc.set_rangeTime     (getFromFirstNode<double>      (nodes.sceneCenterCoord, "rangeTime"));
   isc.set_incidenceAngle(getFromFirstNode<double>      (nodes.sceneCenterCoord, "incidenceAngle"));

   m_sceneCoord.set_centerSceneCoord(isc);

   std::vector<ossimRefPtr<ossimXmlNode> > xnodes2;
   nodes.sceneInfo.findChildNodes(attSceneCornerCoord, xnodes2);

   if ( ! xnodes2.empty() )
   {
      std::vector<InfoSceneCoord> tabIsc;

      for (ossim_uint32 i = 0; i < xnodes2.size(); ++i)
      {
         assert (xnodes2[i].valid());
         ossimXmlNode const& node = *xnodes2[i];

         InfoSceneCoord isc2;

         isc2.set_refRow        (getFromFirstNode<ossim_uint32>(node, "refRow"));
         isc2.set_refColumn     (getFromFirstNode<ossim_uint32>(node, "refColumn"));
         isc2.set_lat           (getFromFirstNode<double>      (node, "lat"));
         isc2.set_lon           (getFromFirstNode<double>      (node, "lon"));
         isc2.set_azimuthTimeUTC(getTextFromFirstNode          (node, "azimuthTimeUTC"));
         isc2.set_rangeTime     (getFromFirstNode<double>      (node, "rangeTime"));
         isc2.set_incidenceAngle(getFromFirstNode<double>      (node, "incidenceAngle"));

         tabIsc.push_back(isc2);
      }

      m_sceneCoord.set_cornersSceneCoord( tabIsc );
      m_sceneCoord.set_numberOfSceneCoord( tabIsc.size() );
   }
}

bool ossimplugins::ossimTerraSarXSarSensorModel::isProductGeoreferenced(Nodes const& nodes) const
{
   // getProjection() == GROUNDRANGE
   ossimString const& projection = getTextFromFirstNode(nodes.productVariantInfo, "projection");
   return projection == "GROUNDRANGE";
}

void ossimplugins::ossimTerraSarXSarSensorModel::initSensorParams(Nodes const& nodes)
{
   static const char MODULE[] = "ossimplugins::ossimTerraXSarSensorModel::initSensorParams";
   SCOPED_LOG(traceDebug, MODULE);

   m_sensorParams.set_nAzimuthLook(getFromFirstNode<double>(nodes.processingParameter, "azimuthLooks"));
   m_sensorParams.set_nRangeLook  (getFromFirstNode<double>(nodes.processingParameter, "rangeLooks"));
   m_sensorParams.set_prf         (getFromFirstNode<double>(nodes.complexImageInfo,    "commonPRF"));
   const double CLUM = 2.99792458e+8 ;
   const double rcf = getFromFirstNode<double>(nodes.instrument, "radarParameters/centerFrequency");
   if (rcf == 0.0)
   {
      throw std::runtime_error("Null Radar Center Frequency found in TSX annotation file, cannot compute the wavelenght");
   }
   m_sensorParams.set_rwl         (CLUM / rcf);
   ossimString const& rawDirection = getTextFromFirstNode(nodes.acquisitionInfo, "lookDirection");
   char buffer[10]; // 10 is more than enough for "right" and "left"
   const string_view direction = to_lower_to(rawDirection, buffer);
   if (direction == "right")
   {
      m_sensorParams.set_sightDirection(SensorParams::Right);
   } else if (direction == "left")
   {
      m_sensorParams.set_sightDirection(SensorParams::Left);
   } else
   {
      throw std::runtime_error(("Invalid sensor direction found in TSX file: "+rawDirection).string());
   }
   m_sensorParams.set_sf          (getFromFirstNode<double>(nodes.complexImageInfo, "commonRSF"));

   // Ellipsoid hard coded to WGS84.
   const double SEMI_MAJOR = 6378137.0;
   const double SEMI_MINOR = 6356752.3142;
   m_sensorParams.set_semiMajorAxis(SEMI_MAJOR);
   m_sensorParams.set_semiMinorAxis(SEMI_MINOR);

   if (isProductGeoreferenced(nodes)) // getProjection() == GROUNDRANGE
   {
      ossimString const& rawOrbitDirection = getTextFromFirstNode(nodes.missionInfo, "orbitDirection");
      ossimString const& imageDataStartWith = getTextFromFirstNode(nodes.complexImageInfo, "imageDataStartWith");
      char orbitDirection[50];
      to_lower_to(rawOrbitDirection, orbitDirection);

      const int orbitDirectionSign = strcmp(orbitDirection, "descending") == 0
         ?  1
         : -1;

      if (imageDataStartWith=="EARLYAZNEARRG") {
         m_sensorParams.set_col_direction(orbitDirectionSign);
         m_sensorParams.set_lin_direction(orbitDirectionSign);
      }
      else if (imageDataStartWith=="EARLYAZFARRG") {
         m_sensorParams.set_col_direction(-orbitDirectionSign);
         m_sensorParams.set_lin_direction(orbitDirectionSign);
      }
      else if (imageDataStartWith=="LATEAZNEARRG") {
         m_sensorParams.set_col_direction(orbitDirectionSign);
         m_sensorParams.set_lin_direction(-orbitDirectionSign);
      }
      else if (imageDataStartWith=="LATEAZFARRG") {
         m_sensorParams.set_col_direction(-orbitDirectionSign);
         m_sensorParams.set_lin_direction(-orbitDirectionSign);
      }
      else { // COSAR Files are stored ing with early azimuth, near range
         m_sensorParams.set_col_direction(orbitDirectionSign);
         m_sensorParams.set_lin_direction(orbitDirectionSign);
      }
   } // matches: if ( isProductGeoreferenced(xdoc) )
   else
   {
      m_sensorParams.set_col_direction(1);
      m_sensorParams.set_lin_direction(1);
   }
}

void ossimplugins::ossimTerraSarXSarSensorModel::initAcquisitionInfo(Nodes const& nodes)
{
   static const char MODULE[] = "ossimplugins::ossimTerraXSarSensorModel::initAcquisitionInfo";
   SCOPED_LOG(traceDebug, MODULE);

   // number of layers and m_polLayerList
   const std::size_t numberOfLayers = getFromFirstNode<std::size_t>(nodes.imageDataInfo, "numberOfLayers");
   m_polLayerList   = getTextNodes(nodes.acquisitionInfo, "polarisationList/polLayer");
   if (numberOfLayers != m_polLayerList.size())
   {
      throw std::runtime_error("Number of layers found mismatch with the number of layer registered");
   }

   addMandatory(theProductKwl, "acquisitionInfo.imagingMode",      nodes.acquisitionInfo, "imagingMode");
   addMandatory(theProductKwl, "acquisitionInfo.sensor",           nodes.acquisitionInfo, "sensor");
   addMandatory(theProductKwl, "acquisitionInfo.lookDirection",    nodes.acquisitionInfo, "lookDirection");
   addMandatory(theProductKwl, "acquisitionInfo.polarisationMode", nodes.acquisitionInfo, "polarisationMode");
}

void ossimplugins::ossimTerraSarXSarSensorModel::estimateGRToSRFromSRToGR(const unsigned int degree, const double maxRangeTime, const double referencePoint, const TimeType azimuthTimeStart)
{
  //TODO Implement polynomial inversion to estimate slant range to ground
  //range coefficients from slant range to ground range coefficients

  //const unsigned int m = degree;
    
  //Polynoms use for estimation

  //TODO: i don't understand how to initialize theExpSet used in LMSFit
  ossimPolynom< ossim_float64 , 1 >::EXPT_SET expSet;
  ossimPolynom< ossim_float64 , 1 >::EXP_TUPLE orders;

  //Store estimated polynom in an ossimPolynom
  ossimPolynom< ossim_float64 , 1 >  poly;

  orders.push_back(degree);
  
  expSet = poly.builExpSet(orders);

  std::vector< ossimPolynom< ossim_float64 , 1 >::VAR_TUPLE > inputs;
  inputs.resize(degree+1);
  std::vector< ossim_float64 > outputs;
  outputs.resize(degree+1);
  
  vector< ossimPolynom< ossim_float64 , 1 >::VAR_TUPLE >::iterator pit;
  vector< ossim_float64 >::iterator it;

  int i;
  for (pit=inputs.begin(), i=0; pit!=inputs.end(); ++pit,++i)
    {
      double input_time = theNearRangeTime + (maxRangeTime - theNearRangeTime) * i / degree;
      pit->push_back( input_time );
      //Compute polynomial values using slantrangetogroundrange coefficients
      double acc = 0.;
      unsigned int j = theSlantRangeToGroundRangeRecords[0].coefs.size() - 1;
      while ( j > 0) {
        acc = (acc + theSlantRangeToGroundRangeRecords[0].coefs[j--]) * (input_time - referencePoint);
      }
      outputs[i] = acc + theSlantRangeToGroundRangeRecords[0].coefs[0];
    }
    
  double rms=0.0;
  bool resfit = poly.LMSfit(expSet, inputs, outputs, &rms);
  if (!resfit)
    {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimSarSensorModel::estimateGRToSRFromSRToGR():  polynom LMS fit failed "<< std::endl;
    }

  std::cout << "Estimated polynom: " << poly << std::endl;
  std::cout << "RMS: " << rms << std::endl;

  //Now fill coefficients
  //Retrieve Slant Range to Ground range coefficients
  CoordinateConversionRecordType coordRecord;

  //Get azimuth time start (again)
  coordRecord.azimuthTime = azimuthTimeStart;

  //Set ground range origin to 0 (FIXME?)
  coordRecord.rg0 = 0.;

  //Fill coefficients
  //loop on monoms
  for ( typename ossimPolynom< ossim_float64 , 1 >::MONOM_MAP::const_iterator it = poly.getMonoms().begin(); it != poly.getMonoms().end() ; ++it )
    {
      coordRecord.coefs.push_back(it->second);
    }
  theGroundRangeToSlantRangeRecords.push_back(coordRecord);
}
