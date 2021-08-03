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
#include <ossim/base/ossimDirectory.h>
#include "ossim/ossimXmlTools.h"
#include "itksys/SystemTools.hxx"

#include "ossim/ossimTraceHelpers.h"
#include "ossim/ossimKeyWordListUtilities.h"
#include "ossim/ossimSarSensorModelPathsAndKeys.h"

#include "cpl_minixml.h"
#include "cpl_string.h"

namespace {// Anonymous namespace
  ossimTrace traceDebug ("ossimTerraSarXSarSensorModel:debug");
  const ossimString attTimeUTC  = "timeUTC";
  const ossimString attPosX     = "posX";
  const ossimString attPosY     = "posY";
  const ossimString attPosZ     = "posZ";
  const ossimString attVelX     = "velX";
  const ossimString attVelY     = "velY";
  const ossimString attVelZ     = "velZ";
  const ossimString attT        = "t";
  const ossimString attTau      = "tau";
  const ossimString attCol      = "col";
  const ossimString attRow      = "row";
  const ossimString attLat      = "lat";
  const ossimString attLon      = "lon";
  const ossimString attHeight   = "height";
  const ossimString attPolDeg   = "polynomialDegree";
  const ossimString attPolCoef  = "coefficient";
  const ossimString attRefPoint = "referencePoint";
  const ossimString attRefRow   = "refRow";
  const ossimString attRefCol   = "refColumn";
}// Anonymous namespace


namespace ossimplugins
{
  bool ossimTerraSarXSarSensorModel::saveState(ossimKeywordlist& kwl,
					       const char* prefix) const
  {
    static const char MODULE[] = "ossimplugins::ossimTerraSarXSarSensorModel::saveState";
    SCOPED_LOG(traceDebug, MODULE);

    kwl.add(prefix,
    	    ossimKeywordNames::TYPE_KW,
    	    "ossimTerraSarXSarSensorModel",
    	    true);

   
    // kwl.add("support_data.",
    // 	    "calibration_lookup_flag",
    // 	    "false",
    // 	    false);

    kwl.addList(theProductKwl,  true);

    return ossimSarSensorModel::saveState(kwl, prefix);
  }


//*************************************************************************************************
// Load State
//*************************************************************************************************
  bool ossimTerraSarXSarSensorModel::loadState(const ossimKeywordlist& kwl,
					       const char* prefix)
  {
    // Specify the looking flag (can be left or right for Cosmo)
    std::string look_side;
    get(kwl, SUPPORT_DATA_PREFIX, "look_side", look_side);
    
    if (look_side != "RIGHT")
      {
    	theRightLookingFlag = false;
      }

    return ossimSarSensorModel::loadState(kwl, prefix);
  }
 

  bool ossimTerraSarXSarSensorModel::readAnnotationFile(const std::string & annotationXml, const std::string & geoXml)
  {
    ossimRefPtr<ossimXmlDocument> xmlDoc = new ossimXmlDocument(annotationXml);
    const ossimXmlNode & xmlRoot = *xmlDoc->getRoot();

    // First, get polarization from image name (only). 
    // The annotationXml contains metadata for the whole product => possibly for several polarizations
    std::string tmp_name = m_imageName.substr(m_imageName.find("_") + 1, m_imageName.size()-1); 
    std::string polarisation = tmp_name.substr(0, tmp_name.find("_")); 

    add(theProductKwl, HEADER_PREFIX, "polarisation", polarisation);

    // Get product_type
    std::string product_type = getTextFromFirstNode(xmlRoot, "productInfo/productVariantInfo/productVariant");

    if (product_type == "SSC")
      {
	product_type = "SLC";
      }

    ////////////////// Add General Parameters ///////////////// 
    theProductType = ProductType(product_type);

    std::string sampleType = getTextFromFirstNode(xmlRoot, "productInfo/imageDataInfo/imageDataType");
    std::string orbitDirection = getTextFromFirstNode(xmlRoot, "productInfo/missionInfo/orbitDirection");
    std::string absOrbit = getTextFromFirstNode(xmlRoot, "productInfo/missionInfo/absOrbit");
    std::string orbitCycle = getTextFromFirstNode(xmlRoot, "productInfo/missionInfo/orbitCycle");
    
    std::string acquisitionMode = getTextFromFirstNode(xmlRoot, "productInfo/acquisitionInfo/imagingMode");
    std::string lookDirection = getTextFromFirstNode(xmlRoot, "productInfo/acquisitionInfo/lookDirection");
    std::string sceneId = getTextFromFirstNode(xmlRoot, "productInfo/sceneInfo/sceneID");

    const double sceneAvgHeight = getDoubleFromFirstNode(xmlRoot, "productInfo/sceneInfo/sceneAverageHeight");

    std::string takeId = getTextFromFirstNode(xmlRoot, "setup/inputData/uniqueDataTakeID");

    add(theProductKwl, "sample_type", sampleType);
    add(theProductKwl, SUPPORT_DATA_PREFIX, "slice_num", "1");
    add(theProductKwl, SUPPORT_DATA_PREFIX, "orbit_pass", orbitDirection);
    add(theProductKwl, SUPPORT_DATA_PREFIX, "abs_orbit", absOrbit);
    add(theProductKwl, SUPPORT_DATA_PREFIX, "orbit_cycle", orbitCycle);
    add(theProductKwl, SUPPORT_DATA_PREFIX, "acquisition_mode", acquisitionMode);
    add(theProductKwl, SUPPORT_DATA_PREFIX, "avg_scene_height", sceneAvgHeight);
    add(theProductKwl, SUPPORT_DATA_PREFIX, "data_take_id", takeId);
    add(theProductKwl, SUPPORT_DATA_PREFIX, "look_side", lookDirection);

    // Parse the near range time (in seconds)
    theNearRangeTime = getDoubleFromFirstNode(xmlRoot, "productInfo/sceneInfo/rangeTime/firstPixel");

    const double lastRangePixel = getDoubleFromFirstNode(xmlRoot, "productInfo/sceneInfo/rangeTime/lastPixel");
    add(theProductKwl, "support_data.slant_range_to_last_pixel", lastRangePixel);

    // Parse the range sampling rate
    theRangeSamplingRate = getDoubleFromFirstNode(xmlRoot, "instrument/settings/RSF");

    // Parse the range resolution
    theRangeResolution = getDoubleFromFirstNode(xmlRoot, "productSpecific/complexImageInfo/projectedSpacingRange/slantRange");

    add(theProductKwl, "meters_per_pixel_x", theRangeResolution);

    // Parse the radar frequency
    theRadarFrequency = getDoubleFromFirstNode(xmlRoot, "instrument/radarParameters/centerFrequency");

    // Parse the PRF
    const double azimuthFrequency = getDoubleFromFirstNode(xmlRoot, "instrument/settings/settingRecord/PRF");
    add(theProductKwl, SUPPORT_DATA_PREFIX, "pulse_repetition_frequency", azimuthFrequency);

    
    // Parse azimuth and range number of looks
    const unsigned int nbAziLooks = xmlRoot.findFirstNode("productInfo/imageDataInfo/imageRaster/azimuthLooks")->getText().toUInt16();
    const unsigned int nbRanLooks = xmlRoot.findFirstNode("productInfo/imageDataInfo/imageRaster/rangeLooks")->getText().toUInt16();

    add(theProductKwl, SUPPORT_DATA_PREFIX, "range_looks", nbRanLooks);
    add(theProductKwl, SUPPORT_DATA_PREFIX, "azimuth_looks", nbAziLooks);

    // Parse spectral window coeff and id
    const double ranWinCoeff = getDoubleFromFirstNode(xmlRoot, "processing/processingParameter/rangeWindowCoefficient");
    const double aziWinCoeff = getDoubleFromFirstNode(xmlRoot, "processing/processingParameter/azimuthWindowCoefficient");
    std::string ranWinId = getTextFromFirstNode(xmlRoot, "processing/processingParameter/rangeWindowID");
    std::string aziWinId = getTextFromFirstNode(xmlRoot, "processing/processingParameter/azimuthWindowID");

    add(theProductKwl, SUPPORT_DATA_PREFIX, "range_window_coeff", ranWinCoeff);
    add(theProductKwl, SUPPORT_DATA_PREFIX, "azimuth_window_coeff", aziWinCoeff);
    add(theProductKwl, SUPPORT_DATA_PREFIX, "range_spectral_window", ranWinId);
    add(theProductKwl, SUPPORT_DATA_PREFIX, "azimuth_spectral_window", aziWinId);

    // Parse processing/acquisition mode : if TDM => bistatic
    const std::string processingFacility = getTextFromFirstNode(xmlRoot, "productInfo/generationInfo/level0ProcessingFacility");
    std::string processingMode = "monostatic";

    if (processingFacility.find("TDM") != std::string::npos )
      {
	processingMode = "bistatic";
      }

    add(theProductKwl, SUPPORT_DATA_PREFIX, "processing_mode", processingMode);

    //Manage only strip map product for now (one burst)
    // Parse azimuth time start/stop
    const TimeType azimuthTimeStart = getTimeFromFirstNode(xmlRoot, "productInfo/sceneInfo/start/timeUTC");
    const TimeType azimuthTimeStop = getTimeFromFirstNode(xmlRoot, "productInfo/sceneInfo/stop/timeUTC");

    const DurationType td = azimuthTimeStop - azimuthTimeStart;

    // numberOfRows and numberOfColumns
    unsigned int numberOfRows = xmlRoot.findFirstNode("productInfo/imageDataInfo/imageRaster/numberOfRows")->getText().toUInt16();
    
    unsigned int numberOfColumns = xmlRoot.findFirstNode("productInfo/imageDataInfo/imageRaster/numberOfColumns")->getText().toUInt16();

    add(theProductKwl, SUPPORT_DATA_PREFIX, "number_samples", numberOfColumns);
    add(theProductKwl, SUPPORT_DATA_PREFIX, "number_lines", numberOfRows);
    add(theProductKwl,"number_samples", numberOfColumns);  // Does not override the metadata (Why ?????)
    add(theProductKwl, "number_lines", numberOfRows);  // Does not override the metadata (Why ?????)

    //Compute azimuth time interval
    theAzimuthTimeInterval = td / static_cast<double> (numberOfRows);
    
    add(theProductKwl, SUPPORT_DATA_PREFIX, "first_line_time", 
	getTextFromFirstNode(xmlRoot, "productInfo/sceneInfo/start/timeUTC"));
    add(theProductKwl, SUPPORT_DATA_PREFIX, "last_line_time", 
	getTextFromFirstNode(xmlRoot, "productInfo/sceneInfo/stop/timeUTC"));

    // Parse the azimut resolution/spacing
    const double aziSpacing = getDoubleFromFirstNode(xmlRoot, "productSpecific/complexImageInfo/projectedSpacingAzimuth");

    add(theProductKwl, SUPPORT_DATA_PREFIX, "azimuth_spacing", aziSpacing);
    add(theProductKwl, "meters_per_pixel_y", aziSpacing);

    // Parse the azimut/range bandwidth
    const double aziBandwidth = getDoubleFromFirstNode(xmlRoot, "processing/processingParameter/totalProcessedAzimuthBandwidth");
    const double ranBandwidth = getDoubleFromFirstNode(xmlRoot, "processing/processingParameter/totalProcessedRangeBandwidth");
    
    add(theProductKwl, SUPPORT_DATA_PREFIX, "azimuth_bandwidth", aziBandwidth);
    add(theProductKwl, SUPPORT_DATA_PREFIX, "range_bandwidth", ranBandwidth);

    ////////////////// GRD (detected product) ///////////////// 
    std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
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
		  }
	      }
	  }
        assert(!coordRecord.coefs.empty()&&"The srgr record has empty coefs vector.");

        theSlantRangeToGroundRangeRecords.push_back(coordRecord);
      }


    /////////////////// Fill vectors (orbit, gcps, ...) ///////////////////
    //For Terrasar-X only 1 burst is supported for now
    BurstRecordType burstRecord;

    burstRecord.startLine = 0;
    burstRecord.azimuthStartTime = azimuthTimeStart;
    burstRecord.azimuthStopTime = azimuthTimeStop;
    burstRecord.endLine = numberOfRows - 1;
    burstRecord.startSample = 0;
    burstRecord.endSample = numberOfColumns - 1;
    burstRecord.azimuthAnxTime = 0;
    theBurstRecords.push_back(burstRecord);

    // ul/ur/ll/lr lon/lat coords
    readSceneCoord(xmlDoc, numberOfRows, numberOfColumns, "sceneCornerCoord");
    readSceneCoord(xmlDoc, numberOfRows, numberOfColumns, "sceneCenterCoord");

    // Lookup position/velocity records
    readOrbitVector(xmlDoc);

    // Parse GCPs
    readGCPs(geoXml);

    // Parse Doppler Rate
    readDopplerRate(xmlDoc);

    // Parse Doppler Centroid
    readDopplerCentroid(xmlDoc, polarisation);

    // Parse calibration factor
    readCalibrationFactor(xmlDoc, polarisation);

    // Parse noise
    readNoise(xmlDoc, polarisation);

    // Ensure that superclass members are initialized
    loadState(theProductKwl);

    // SensorId and ImageId
    theSensorID = getTextFromFirstNode(xmlRoot, "generalHeader/mission");
    theImageID = sceneId;

    return true;
  }


  bool ossimTerraSarXSarSensorModel::open(const ossimFilename& file)
  {
    // We expect the filename given here to be the image file (.tif or .cos) found in the IMAGEDATA directory
    // Reference: TerraSar-X Ground Segment Level 1b Product Format Specification

    // Get the path to the root of the image product
    using namespace itksys;
    const auto product_root = SystemTools::GetParentDirectory(SystemTools::GetParentDirectory(SystemTools::GetRealPath(file)));

    // Get image name (to retrieve specific information such as polarization)
    m_imageName = SystemTools::GetFilenameName(file);

    // Get paths to the two xml we need, and check they exist
    std::vector<std::string> components;
    SystemTools::SplitPath(product_root, components);

    std::vector<std::string> components_georef = components;
    components_georef.push_back("ANNOTATION");
    components_georef.push_back("GEOREF.xml");
    const auto georef = SystemTools::JoinPath(components_georef);

    std::vector<std::string> components_annotation = components;
    components_annotation.push_back(components.back() + std::string(".xml"));
    std::string annotation = SystemTools::JoinPath(components_annotation);

    if (!SystemTools::FileExists(annotation))
      {
	ossimDirectory directory = ossimDirectory(product_root);

        std::vector<ossimFilename> vectName;
        ossimString reg = "^T(S|D)X1_SAR__.*\\.xml$";
        directory.findAllFilesThatMatch( vectName, reg, 1 );
        if (vectName.size())
	  {
	    const ossimFilename annotationName = vectName[0];
	    std::vector<std::string> components_annotation_xml = components;
	    components_annotation_xml.push_back(SystemTools::GetFilenameName(annotationName));
	    annotation = SystemTools::JoinPath(components_annotation_xml);
	  }
      }

    if (!SystemTools::FileExists(georef) || !SystemTools::FileExists(annotation))
      {
	return false;
      }

    // Try to read annotation file
    return readAnnotationFile(annotation, georef);
  }

  bool ossimTerraSarXSarSensorModel::readSceneCoord(const ossimRefPtr<ossimXmlDocument> xmlDoc,
						    const unsigned int numberOfRows,
						    const unsigned int numberOfColumns,
						    const std::string scenePosition)
  {
    std::vector<ossimRefPtr<ossimXmlNode> > xnodes;

    if (scenePosition == "sceneCornerCoord")
      {
	xmlDoc->findNodes("/level1Product/productInfo/sceneInfo/sceneCornerCoord",xnodes);

	add(theProductKwl, "sceneCoord.numberOfSceneCornerCoord", xnodes.size());
      }
    else if (scenePosition == "sceneCenterCoord")
      {
	xmlDoc->findNodes("/level1Product/productInfo/sceneInfo/sceneCenterCoord",xnodes);
      }
    else
      {
	return false;
      }

    unsigned int index = 0;

    for(std::vector<ossimRefPtr<ossimXmlNode> >::iterator itNode = xnodes.begin(); itNode!=xnodes.end();++itNode)
      {
	// Retrieve for the current corner : row and col
	const unsigned int row = getTextFromFirstNode(**itNode, attRefRow).toUInt16();
	const unsigned int col = getTextFromFirstNode(**itNode, attRefCol).toUInt16();

	// Retrieve lat/lon
	const double lat = getDoubleFromFirstNode(**itNode, attLat);
	const double lon = getDoubleFromFirstNode(**itNode, attLon);

	// Retrive azimuth/range time and incidence angle
	const std::string aziTimeUTC = getTextFromFirstNode(**itNode, "azimuthTimeUTC");
	const double rangeTime = getDoubleFromFirstNode(**itNode, "rangeTime");
	const double incAngle = getDoubleFromFirstNode(**itNode, "incidenceAngle");

	if (scenePosition == "sceneCornerCoord")
	  {
	    // Define which corner to add lat/lon
	    std::string kwl_lat_key = "ul_lat";
	    std::string kwl_lon_key = "ul_lon";
	    if (row == 1 && col > 1)
	      {
		kwl_lat_key = "ur_lat";
		kwl_lon_key = "ur_lon";
	      }
	    if (row == numberOfRows && col > 1)
	      {
		kwl_lat_key = "lr_lat";
		kwl_lon_key = "lr_lon";
	      }
	    if (row == numberOfRows && col == 1)
	      {
		kwl_lat_key = "ll_lat";
		kwl_lon_key = "ll_lon";
	      }


	    // Add information into ossimkeywordlist
	    add(theProductKwl, kwl_lat_key, "", lat);
	    add(theProductKwl, kwl_lon_key, "", lon);
	  }

	// Add the metadata into our keywordlist
	char prefix[256];
	if (scenePosition == "sceneCornerCoord")
	  s_printf(prefix, "sceneCoord.sceneCornerCoord[%d].", index);
	else
	  s_printf(prefix, "sceneCoord.sceneCenterCoord.");

	add(theProductKwl, prefix, "azimuthTimeUTC", aziTimeUTC);
	add(theProductKwl, prefix, "incidenceAngle", incAngle);
	add(theProductKwl, prefix, "lat", lat);
	add(theProductKwl, prefix, "lon", lon);
	add(theProductKwl, prefix, "rangeTime", rangeTime);
	add(theProductKwl, prefix, "refColumn", col);
	add(theProductKwl, prefix, "refRow", row);

	++index;
      }

    return true;
  }

  
  bool ossimTerraSarXSarSensorModel::readOrbitVector(const ossimRefPtr<ossimXmlDocument> xmlDoc)
  {
    std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
    xmlDoc->findNodes("/level1Product/platform/orbit/stateVec",xnodes);

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
        theOrbitRecords.push_back(orbitRecord);
      }

    return true;
  }

  bool ossimTerraSarXSarSensorModel::readCalibrationFactor(const ossimRefPtr<ossimXmlDocument> xmlDoc, const std::string polarisation)
  {
    // Retrieve calibrationConstant nodes (may have several calibration nodes following some layers :
    // polaristaions
    std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
    xmlDoc->findNodes("/level1Product/calibration/calibrationConstant", xnodes);

    // Loop on nodes with a check on layer (must match to our image)
    for(std::vector<ossimRefPtr<ossimXmlNode> >::iterator itCalConstnantNode = xnodes.begin(); 
	itCalConstnantNode!=xnodes.end();++itCalConstnantNode)
      {
	// Get the polLayer
	std::string polarisationLayer  = getTextFromFirstNode(**itCalConstnantNode, "polLayer");

	// Check if the current polLayer match with our image (if not then pass the iteration)
	if (polarisationLayer != polarisation)
	  {
	    continue;
	  }

	const double calFactor = getDoubleFromFirstNode(**itCalConstnantNode, "calFactor");

	// Add value into keywordlist
	add(theProductKwl, "calibration.calibrationConstant.calFactor", calFactor);
      }

    return true;
  }

  bool ossimTerraSarXSarSensorModel::readDopplerRate(const ossimRefPtr<ossimXmlDocument> xmlDoc)
  {
    std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
    xmlDoc->findNodes("/level1Product/processing/geometry/dopplerRate", xnodes);

    // Vector to store coefficient for the current polynomial
    std::vector<double> polCoefVector;

    std::vector<ossimRefPtr<ossimXmlNode> > coeffNodes;
    ossimRefPtr<ossimXmlNode>  polNode;

    unsigned int index = 1;

    for(std::vector<ossimRefPtr<ossimXmlNode> >::iterator itNode = xnodes.begin(); itNode!=xnodes.end();++itNode)
      {
	// Clear the current coefficients
	polCoefVector.clear();
	coeffNodes.clear();

	// Retieve acquisition time
	const TimeType timeUTC = getTimeFromFirstNode(**itNode, attTimeUTC);

	// Retrive the dopplerRatePolynomial Node
	polNode = itNode[0]->findFirstNode("dopplerRatePolynomial");
	
	// Retrieve reference point
	const double refPoint = getDoubleFromFirstNode(*polNode, attRefPoint);
	
	// Retrieve polynomial degree
	const unsigned int degree = getTextFromFirstNode(*polNode, attPolDeg).toUInt16();
	
	const ossimString EXP = "exponent";
        ossimString s;
	polNode->findChildNodes("coefficient", coeffNodes);


        if ( coeffNodes.size() )
	  {
            for (unsigned int i = 0; i < coeffNodes.size(); ++i)
	      {
                if (coeffNodes[i].valid())
		  {
                    coeffNodes[i]->getAttributeValue(s, EXP);
                    const double coeff = coeffNodes[i]->getText().toDouble();
                    polCoefVector.push_back(coeff);
		  }
	      }
	  }

	assert((polCoefVector.size() == degree) && "The doppler rate record has an incoherent size.");

	// Add the metadata inot our keywordlist
	char prefix[256];
	//Doppler_Centroid_Coefficients.dop_coef_list;
	s_printf(prefix, "azimuthFmRate.azi_fm_rate_coef_list%d.", index);
	
	add(theProductKwl, prefix, "azi_fm_rate_coef_time", timeUTC);
	add(theProductKwl, prefix, "slant_range_time", refPoint);
	
	for (int count = 1 ; count < (polCoefVector.size()+1) ; ++count)
	  {
	    char coeff_prefix[256];
	    s_printf(coeff_prefix, "%s%d.azi_fm_rate_coef", prefix, count);
	    
	    add(theProductKwl, coeff_prefix, polCoefVector[count-1]);
	  }
	
	++index;
      }
    
    add(theProductKwl, "azimuthFmRate.azi_fm_rate_coef_nb_list", xnodes.size());

    return true;
  }
  
  bool ossimTerraSarXSarSensorModel::readDopplerCentroid(const ossimRefPtr<ossimXmlDocument> xmlDoc, 
							 const std::string polarisation)
  {
    // Retrieve doppler Centroid nodes (may have several doppler centroid nodes following some layers (polarisations)
    std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
    xmlDoc->findNodes("/level1Product/processing/doppler/dopplerCentroid", xnodes);

    // Loop on nodes with a check on layer (must match to our image)
    for(std::vector<ossimRefPtr<ossimXmlNode> >::iterator itDopplerNode = xnodes.begin(); 
	itDopplerNode!=xnodes.end();++itDopplerNode)
      {  
	// Get the polLayer
	std::string polarisationLayer  = getTextFromFirstNode(**itDopplerNode, "polLayer");

	// Check if the current polLayer match with our image (if not then pass the iteration)
	if (polarisationLayer != polarisation)
	  {
	    continue;
	  }
	
	std::vector<ossimRefPtr<ossimXmlNode> > dopplerEstimateNodes;
	// Get dopplerEstimate nodes
	(*itDopplerNode)->findChildNodes("dopplerEstimate", dopplerEstimateNodes);

	// Vector to store coefficient for the current polynomial
	std::vector<double> polCoefVector;

	std::vector<ossimRefPtr<ossimXmlNode> > coeffNodes;
	ossimRefPtr<ossimXmlNode>  polNode;

	unsigned int index = 1;

	for(std::vector<ossimRefPtr<ossimXmlNode> >::iterator itNode = dopplerEstimateNodes.begin(); 
	    itNode!=dopplerEstimateNodes.end();++itNode)
	  {
	    // Clear the current coefficients
	    polCoefVector.clear();
	    coeffNodes.clear();
	    
	    // Retieve acquisition time
	    const TimeType timeUTC = getTimeFromFirstNode(**itNode, attTimeUTC);
	
	    // Retrtive he combinedDoppler Node
	    polNode = itNode[0]->findFirstNode("combinedDoppler");
	
	    // Retrieve reference point
	    const double refPoint = getDoubleFromFirstNode(*polNode, attRefPoint);
	
	    // Retrieve polynomial degree
	    const unsigned int degree = getTextFromFirstNode(*polNode, attPolDeg).toUInt16();
	
	    const ossimString EXP = "exponent";
	    ossimString s;
	    polNode->findChildNodes("coefficient", coeffNodes);

	    
	    if ( coeffNodes.size() )
	      {
		for (unsigned int i = 0; i < coeffNodes.size(); ++i)
		  {
		    if (coeffNodes[i].valid())
		      {
			coeffNodes[i]->getAttributeValue(s, EXP);
			const double coeff = coeffNodes[i]->getText().toDouble();
			polCoefVector.push_back(coeff);
		      }
		  }
	      }

	    assert((polCoefVector.size() == degree) && "The doppler rate record has an incoherent size.");
    
	    // Add the metadata inot our keywordlist
	    char prefix[256];
	    //Doppler_Centroid_Coefficients.dop_coef_list;
	    s_printf(prefix, "dopplerCentroid.dop_coef_list%d.", index);
	
	    add(theProductKwl, prefix, "dop_coef_time", timeUTC);
	    add(theProductKwl, prefix, "slant_range_time", refPoint);
	    
	    for (int count = 1 ; count < (polCoefVector.size()+1) ; ++count)
	      {
		char coeff_prefix[256];
		s_printf(coeff_prefix, "%s%d.dop_coef", prefix, count);
		
		add(theProductKwl, coeff_prefix, polCoefVector[count-1]);
	      }

	    ++index;

	  }
	
	add(theProductKwl, "dopplerCentroid.dop_coef_nb_list", dopplerEstimateNodes.size());
      }

    return true;
  }

 bool ossimTerraSarXSarSensorModel::readNoise(const ossimRefPtr<ossimXmlDocument> xmlDoc,
							 const std::string polarisation)
  {
    // Retrieve doppler Centroid nodes (may have several doppler centroid nodes following some layers (polarisations)
    std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
    xmlDoc->findNodes("/level1Product/noise", xnodes);

    // Loop on nodes with a check on layer (must match to our image)
    for(std::vector<ossimRefPtr<ossimXmlNode> >::iterator itNoiseNode = xnodes.begin();
	itNoiseNode!=xnodes.end();++itNoiseNode)
      {
	// Get the polLayer
	std::string polarisationLayer  = getTextFromFirstNode(**itNoiseNode, "polLayer");

	// Check if the current polLayer match with our image (if not then pass the iteration)
	if (polarisationLayer != polarisation)
	  {
	    continue;
	  }

	std::vector<ossimRefPtr<ossimXmlNode> > imageNoiseNodes;
	// Get dopplerEstimate nodes
	(*itNoiseNode)->findChildNodes("imageNoise", imageNoiseNodes);

	// Vector to store coefficient for the current polynomial
	std::vector<double> polCoefVector;

	std::vector<ossimRefPtr<ossimXmlNode> > coeffNodes;
	ossimRefPtr<ossimXmlNode>  polNode;

	unsigned int index = 0;

	for(std::vector<ossimRefPtr<ossimXmlNode> >::iterator itNode = imageNoiseNodes.begin();
	    itNode!=imageNoiseNodes.end();++itNode)
	  {
	    // Clear the current coefficients
	    polCoefVector.clear();
	    coeffNodes.clear();

	    // Retieve acquisition time
	    //const TimeType timeUTC = getTimeFromFirstNode(**itNode, attTimeUTC);
	    std::string timeUTC = getTextFromFirstNode(**itNode, attTimeUTC);

	    // Retrtive he combinedDoppler Node
	    polNode = itNode[0]->findFirstNode("noiseEstimate");

	    // Retrieve reference point and validaty range
	    const double refPoint = getDoubleFromFirstNode(*polNode, attRefPoint);
	    const double validityMin = getDoubleFromFirstNode(*polNode, "validityRangeMin");
	    const double validityMax = getDoubleFromFirstNode(*polNode, "validityRangeMax");

	    // Retrieve polynomial degree
	    const unsigned int degree = getTextFromFirstNode(*polNode, attPolDeg).toUInt16();

	    const ossimString EXP = "exponent";
	    ossimString s;
	    polNode->findChildNodes("coefficient", coeffNodes);

	    if ( coeffNodes.size() )
	      {
		for (unsigned int i = 0; i < coeffNodes.size(); ++i)
		  {
		    if (coeffNodes[i].valid())
		      {
			coeffNodes[i]->getAttributeValue(s, EXP);
			const double coeff = coeffNodes[i]->getText().toDouble();
			polCoefVector.push_back(coeff);
		      }
		  }
	      }

	    assert((polCoefVector.size() == degree) && "The doppler rate record has an incoherent size.");

	    // Add the metadata inot our keywordlist
	    char prefix[256];
	    //Doppler_Centroid_Coefficients.dop_coef_list;
	    s_printf(prefix, "noise[%d]imageNoise.", index);

	    add(theProductKwl, prefix, "timeUTC", timeUTC);
	    add(theProductKwl, prefix, "noiseEstimate.referencePoint", refPoint);
	    add(theProductKwl, prefix, "noiseEstimate.validityRangeMax", validityMax);
	    add(theProductKwl, prefix, "noiseEstimate.validityRangeMin", validityMin);
	    add(theProductKwl, prefix, "noiseEstimate.polynomialDegree", degree);

	    for (int count = 0 ; count < polCoefVector.size() ; ++count)
	      {
		char coeff_prefix[256];
		s_printf(coeff_prefix, "%snoiseEstimate.coefficient[%d]", prefix, count);

		add(theProductKwl, coeff_prefix, polCoefVector[count]);
	      }

	    ++index;

	  }

	add(theProductKwl, "noise.numberOfNoiseRecords", imageNoiseNodes.size());
	add(theProductKwl, "noise.nameOfNoisePolarisation", polarisation);
      }

    return true;
  }


  bool ossimTerraSarXSarSensorModel::readGCPs(const std::string & geoXml)
  {
    ossimRefPtr<ossimXmlDocument> xmlGeo = new ossimXmlDocument(geoXml);
    const ossimXmlNode & xmlGeoRoot = *xmlGeo->getRoot();

    std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
    xmlGeo->findNodes("/geoReference/geolocationGrid/gridPoint",xnodes);

    // Retrieve reference azimuth and range Time for geo grid
    const TimeType tReferenceTime = getTimeFromFirstNode(xmlGeoRoot, "geolocationGrid/gridReferenceTime/tReferenceTimeUTC");

    const double tauReference = getDoubleFromFirstNode(xmlGeoRoot, "geolocationGrid/gridReferenceTime/tauReferenceTime");

    // Loop on points
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
	gcpRecord.azimuthTime = tReferenceTime + microseconds(deltaAzimuth * 1000000);

        //Get delta range time
	gcpRecord.slantRangeTime = tauReference + getDoubleFromFirstNode(**itNode, attTau);


        gcpRecord.imPt.x = getDoubleFromFirstNode(**itNode, attCol) - 1.;

        gcpRecord.imPt.y = getDoubleFromFirstNode(**itNode, attRow) - 1.;

        ossimGpt geoPoint;
        gcpRecord.worldPt.lat = getDoubleFromFirstNode(**itNode, attLat);
        gcpRecord.worldPt.lon = getDoubleFromFirstNode(**itNode, attLon);
        gcpRecord.worldPt.hgt = getDoubleFromFirstNode(**itNode, attHeight);

        theGCPRecords.push_back(gcpRecord);
      }

    add(theProductKwl, GCP_NUMBER_KEY, static_cast<ossim_uint32>(theGCPRecords.size()));

    this->optimizeTimeOffsetsFromGcps();

    return true;
  }
  
}
