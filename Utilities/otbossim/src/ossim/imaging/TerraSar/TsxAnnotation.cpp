#include <ossim/imaging/TerraSar/TsxAnnotation.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimXmlNode.h>
#include <ossim/base/ossimRefPtr.h>
#include <iostream>

TsxAnnotation::TsxAnnotation()
{
}

TsxAnnotation::~TsxAnnotation()
{
}

bool TsxAnnotation::Parse(ossimXmlDocument docXML) {

	std::vector< ossimRefPtr<ossimXmlNode> > searchResult, secondSearchResult ; 
	ossimRefPtr<ossimXmlNode> foundNode;
	ossimRefPtr<ossimXmlNode> foundSubNode ;
	ossimString * xpathTest ;

	//  Pixel order
	xpathTest = new ossimString("/level1Product/productSpecific/complexImageInfo/imageDataStartWith") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _imageDataStartWith = foundNode->getText() ;

	 //  Orbit direction : Ascending/descending
	xpathTest = new ossimString("/level1Product/productInfo/missionInfo/orbitDirection") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _orbitDirection = foundNode->getText() ;
	 
	// Look Direction
	 xpathTest = new ossimString("/level1Product/productInfo/acquisitionInfo/lookDirection") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _lookDirection = foundNode->getText() ;

	// Input scene centre time
	 xpathTest = new ossimString("/level1Product/productInfo/sceneInfo/sceneCenterCoord/azimuthTimeUTC") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _inp_sctim = foundNode->getText() ;
	 // TSX date format is UTC (xs:dateTime) : yyyy-mm-ddThh:mm:ss(.s+)zzzz (where zzzz is the timeZone).

	//  Processed scene centre latitude
	xpathTest = new ossimString("/level1Product/productInfo/sceneInfo/sceneCenterCoord/lat") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _pro_lat = ( foundNode->getText() ).toDouble() ;
   
	//  Processed scene centre longitude
	xpathTest = new ossimString("/level1Product/productInfo/sceneInfo/sceneCenterCoord/lon") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _pro_long = ( foundNode->getText() ).toDouble() ;
  
	// Processed scene centre heading
	xpathTest = new ossimString("/level1Product/productInfo/sceneInfo/headingAngle") ;
   searchResult.clear();
   docXML.findNodes(*xpathTest, searchResult) ;
   if (searchResult.size() != 1 ) return false ;
   foundNode = searchResult.at(0) ;
   _pro_head = ( foundNode->getText() ).toDouble() ;

 	//  Ellipsoid semi-major axis, km
	_ellip_maj=6378.137;
   // this parameter can be retrieved more precisely from the external file GEOREFERENCE.XML

	//  Ellipsoid semi_minor axis, km
	_ellip_min=6356.752315;
	 // this parameter can be retrieved more precisely from the external file GEOREFERENCE.XML

	 //  Scene centre line number 
	 xpathTest = new ossimString("/level1Product/productInfo/sceneInfo/sceneCenterCoord/refRow") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _sc_lin = ( foundNode->getText() ).toInt() ;
	 _sc_lin = _sc_lin - 1 ; //TSX image coordinates start at (1,1), while OSSIM image coordinates start at (0,0)
        
	 //  Scene centre pixel number
	 xpathTest = new ossimString("/level1Product/productInfo/sceneInfo/sceneCenterCoord/refColumn") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _sc_pix = ( foundNode->getText() ).toInt() ;
	 _sc_pix = _sc_pix - 1 ; //TSX image coordinates start at (1,1), while OSSIM image coordinates start at (0,0)
 
	 //  Radar center frequency
	 xpathTest = new ossimString("/level1Product/instrument/radarParameters/centerFrequency") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _central_freq = ( foundNode->getText() ).toDouble() ;
    
	 //  Range sampling rate
	 xpathTest = new ossimString("/level1Product/productSpecific/complexImageInfo/commonRSF") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _fr = ( foundNode->getText() ).toDouble() ;
	 
	 // Range gate start time
	 xpathTest = new ossimString("/level1Product/productInfo/sceneInfo/rangeTime/firstPixel") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _rng_gate = ( foundNode->getText() ).toDouble() ;

	 // Range gate end time
	 xpathTest = new ossimString("/level1Product/productInfo/sceneInfo/rangeTime/lastPixel") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _rng_gate_end = ( foundNode->getText() ).toDouble() ;

	 // Azimuth start time
	 xpathTest = new ossimString("/level1Product/productInfo/sceneInfo/start/timeUTC") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _azimuthStartTime = ( foundNode->getText() ) ;

	 // Azimuth stop time
	 xpathTest = new ossimString("/level1Product/productInfo/sceneInfo/stop/timeUTC") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _azimuthStopTime = ( foundNode->getText() ) ;

	 // Scene center range time
	 xpathTest = new ossimString("/level1Product/productInfo/sceneInfo/sceneCenterCoord/rangeTime") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _sceneCenterRangeTime = ( foundNode->getText() ).toDouble() ;

	 // Nominal PRF, Hz
	 xpathTest = new ossimString("/level1Product/productSpecific/complexImageInfo/commonPRF") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _fa = ( foundNode->getText() ).toDouble() ;
    
	 // Number of azimuth looks
	 xpathTest = new ossimString("/level1Product/processing/processingParameter/azimuthLooks") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _n_azilok = ( foundNode->getText() ).toDouble() ;
	 
	 // Number of range looks
	 xpathTest = new ossimString("/level1Product/processing/processingParameter/rangeLooks") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _n_rnglok = ( foundNode->getText() ).toDouble() ;

	  // Average terrain height, m
	 xpathTest = new ossimString("/level1Product/productInfo/sceneInfo/sceneAverageHeight") ; 
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _terrain_h = ( foundNode->getText() ).toDouble() ;

	// Type of range data (GROUNDRANGE, SLANTRANGE, MAP, UNDEFINED)
	 xpathTest = new ossimString("/level1Product/productInfo/productVariantInfo/projection") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _rangeProjectionType = foundNode->getText() ;

	// number of lines
	xpathTest = new ossimString("/level1Product/productInfo/imageDataInfo/imageRaster/numberOfRows") ; 
	searchResult.clear();
	docXML.findNodes(*xpathTest, searchResult) ;
	if (searchResult.size() != 1 ) return false ;
	foundNode = searchResult.at(0) ;
	_nbLin = (foundNode->getText()).toDouble() ;

	// Slant Range TO Ground Range Projection
			// number of columns
	xpathTest = new ossimString("/level1Product/productInfo/imageDataInfo/imageRaster/numberOfColumns") ; 
	searchResult.clear();
	docXML.findNodes(*xpathTest, searchResult) ;
	if (searchResult.size() != 1 ) return false ;
	foundNode = searchResult.at(0) ;
	_nbCol = (foundNode->getText()).toDouble() ;
			// mean pixel spacing
	xpathTest = new ossimString("/level1Product/productInfo/imageDataInfo/imageRaster/groundRangeResolution") ; 
	searchResult.clear();
	docXML.findNodes(*xpathTest, searchResult) ;
	if (searchResult.size() != 1 ) return false ;
	foundNode = searchResult.at(0) ;
	_pixel_spacing_mean = (foundNode->getText()).toDouble() ;
		// pixel spacing at near range
	xpathTest = new ossimString("/level1Product/productSpecific/complexImageInfo/projectedSpacingRange/groundNear") ; 
	searchResult.clear();
	docXML.findNodes(*xpathTest, searchResult) ;
	if (searchResult.size() != 1 ) return false ;
	foundNode = searchResult.at(0) ;
	_pixel_spacing_near = (foundNode->getText()).toDouble() ;
		// pixel spacing at far range
	xpathTest = new ossimString("/level1Product/productSpecific/complexImageInfo/projectedSpacingRange/groundFar") ; 
	searchResult.clear();
	docXML.findNodes(*xpathTest, searchResult) ;
	if (searchResult.size() != 1 ) return false ;
	foundNode = searchResult.at(0) ;
	_pixel_spacing_far = (foundNode->getText()).toDouble() ;
			// reference point
	xpathTest = new ossimString("/level1Product/productSpecific/projectedImageInfo/slantToGroundRangeProjection/referencePoint") ; 
	searchResult.clear();
	docXML.findNodes(*xpathTest, searchResult) ;
	if (searchResult.size() == 1 ) {
		foundNode = searchResult.at(0) ;
		_SrToGr_R0 = (foundNode->getText()).toDouble() ;
	}
			// coefficients
	xpathTest = new ossimString("/level1Product/productSpecific/projectedImageInfo/slantToGroundRangeProjection/coefficient") ; 
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 ossimString *xpathExponent = new ossimString("exponent") ; 
	 ossimString exponent_str ;
	 double coefficient;
	 int exponent ;
	 for ( int i=0 ; i<searchResult.size() ; i++) {
		foundNode = searchResult.at(i) ;
		foundNode->getAttributeValue(exponent_str, *xpathExponent);
		exponent = exponent_str.toInt() ;
		coefficient = (foundNode->getText()).toDouble() ;
		_SrToGr_coeffs.push_back(coefficient) ;
		_SrToGr_exps.push_back(exponent) ;
	 }
	
	_ndata=0 ; // Number of state vector data points
	
	// Data point position/velocity
	xpathTest = new ossimString("/level1Product/platform/orbit/stateVec") ; 
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	
	 ossimString qualind ; // TSX ephemerisis quality indicator (1:good, 0: wrong)

	ossimString *xpathQualind = new ossimString("qualInd") ; 
	ossimString *xpathTime = new ossimString("timeUTC") ;
	ossimString *xpathPosX = new ossimString("posX") ;
	ossimString *xpathPosY = new ossimString("posY") ;
	ossimString *xpathPosZ = new ossimString("posZ") ;
	ossimString *xpathVelX = new ossimString("velX") ;
	ossimString *xpathVelY = new ossimString("velY") ;
	ossimString *xpathVelZ = new ossimString("velZ") ;
	_eph_date.clear();
	_posX.clear(); 
	_posY.clear(); 
	_posZ.clear(); 
	_velX.clear(); 
	_velY.clear(); 
	_velZ.clear(); 

	 for (int i=0 ; i<searchResult.size() ; i++) {
		foundNode = searchResult.at(i) ;
		foundNode->getAttributeValue(qualind, *xpathQualind);

		if (qualind=="1") {
			_ndata++;
			secondSearchResult.clear();
			foundSubNode =foundNode->findFirstNode(*xpathTime);
			if (foundSubNode==0) return false;
			_eph_date.push_back(foundSubNode->getText());

			foundSubNode =foundNode->findFirstNode(*xpathPosX);
			if (foundSubNode==0) return false;
			_posX.push_back((foundSubNode->getText()).toDouble());

			foundSubNode =foundNode->findFirstNode(*xpathPosY);
			if (foundSubNode==0) return false;
			_posY.push_back((foundSubNode->getText()).toDouble());

			foundSubNode =foundNode->findFirstNode(*xpathPosZ);
			if (foundSubNode==0) return false;
			_posZ.push_back((foundSubNode->getText()).toDouble());

			foundSubNode =foundNode->findFirstNode(*xpathVelX);
			if (foundSubNode==0) return false;
			_velX.push_back((foundSubNode->getText()).toDouble());

			foundSubNode =foundNode->findFirstNode(*xpathVelY);
			if (foundSubNode==0) return false;
			_velY.push_back((foundSubNode->getText()).toDouble());

			foundSubNode =foundNode->findFirstNode(*xpathVelZ);
			if (foundSubNode==0) return false;
			_velZ.push_back((foundSubNode->getText()).toDouble());
		}
	 }

	 // Scene corners + scene center longitudes
	 xpathTest = new ossimString("/level1Product/productInfo/sceneInfo/sceneCenterCoord/lon") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _cornersLon[0] = ( foundNode->getText() ).toDouble() ;
	 xpathTest = new ossimString("/level1Product/productInfo/sceneInfo/sceneCornerCoord/lon") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 4 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _cornersLon[1] = ( foundNode->getText() ).toDouble() ;
	 foundNode = searchResult.at(1) ;
	 _cornersLon[2] = ( foundNode->getText() ).toDouble() ;
	 foundNode = searchResult.at(2) ;
	 _cornersLon[3] = ( foundNode->getText() ).toDouble() ;
	 foundNode = searchResult.at(3) ;
	 _cornersLon[4] = ( foundNode->getText() ).toDouble() ;

	 // Scene corners + scene center latitudes
	 xpathTest = new ossimString("/level1Product/productInfo/sceneInfo/sceneCenterCoord/lat") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _cornersLat[0] = ( foundNode->getText() ).toDouble() ;
	 xpathTest = new ossimString("/level1Product/productInfo/sceneInfo/sceneCornerCoord/lat") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 4 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _cornersLat[1] = ( foundNode->getText() ).toDouble() ;
	 foundNode = searchResult.at(1) ;
	 _cornersLat[2] = ( foundNode->getText() ).toDouble() ;
	 foundNode = searchResult.at(2) ;
	 _cornersLat[3] = ( foundNode->getText() ).toDouble() ;
	 foundNode = searchResult.at(3) ;
	 _cornersLat[4] = ( foundNode->getText() ).toDouble() ;

	// Scene corners + scene center columns
	 xpathTest = new ossimString("/level1Product/productInfo/sceneInfo/sceneCenterCoord/refColumn") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _cornersCol[0] = ( foundNode->getText() ).toInt() ;
	 _cornersCol[0] = _cornersCol[0] - 1 ; //TSX image coordinates start at (1,1), while OSSIM image coordinates start at (0,0)
	 xpathTest = new ossimString("/level1Product/productInfo/sceneInfo/sceneCornerCoord/refColumn") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 4 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _cornersCol[1] = ( foundNode->getText() ).toInt() ;
	 _cornersCol[1] = _cornersCol[1] - 1 ; //TSX image coordinates start at (1,1), while OSSIM image coordinates start at (0,0)
	 foundNode = searchResult.at(1) ;
	 _cornersCol[2] = ( foundNode->getText() ).toInt() ;
	 _cornersCol[2] = _cornersCol[2] - 1 ; //TSX image coordinates start at (1,1), while OSSIM image coordinates start at (0,0)
	 foundNode = searchResult.at(2) ;
	 _cornersCol[3] = ( foundNode->getText() ).toInt() ;
	 _cornersCol[3] = _cornersCol[3] - 1 ; //TSX image coordinates start at (1,1), while OSSIM image coordinates start at (0,0)
	 foundNode = searchResult.at(3) ;
	 _cornersCol[4] = ( foundNode->getText() ).toInt() ;
	 _cornersCol[4] = _cornersCol[4] - 1 ; //TSX image coordinates start at (1,1), while OSSIM image coordinates start at (0,0)
    
	 // Scene corners + scene center lines
	 xpathTest = new ossimString("/level1Product/productInfo/sceneInfo/sceneCenterCoord/refRow") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _cornersLin[0] = ( foundNode->getText() ).toInt() ;
	 _cornersLin[0] = _cornersLin[0] - 1 ; //TSX image coordinates start at (1,1), while OSSIM image coordinates start at (0,0)
	 	 xpathTest = new ossimString("/level1Product/productInfo/sceneInfo/sceneCornerCoord/refRow") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 4 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _cornersLin[1] = ( foundNode->getText() ).toInt() ;
	 _cornersLin[1] = _cornersLin[1] - 1 ; //TSX image coordinates start at (1,1), while OSSIM image coordinates start at (0,0)
	 foundNode = searchResult.at(1) ;
	 _cornersLin[2] = ( foundNode->getText() ).toInt() ;
	 _cornersLin[2] = _cornersLin[2] - 1 ; //TSX image coordinates start at (1,1), while OSSIM image coordinates start at (0,0)
	 foundNode = searchResult.at(2) ;
	 _cornersLin[3] = ( foundNode->getText() ).toInt() ;
	 _cornersLin[3] = _cornersLin[3] - 1 ; //TSX image coordinates start at (1,1), while OSSIM image coordinates start at (0,0)
	 foundNode = searchResult.at(3) ;
	 _cornersLin[4] = ( foundNode->getText() ).toInt() ;
	 _cornersLin[4] = _cornersLin[4] - 1 ; //TSX image coordinates start at (1,1), while OSSIM image coordinates start at (0,0)

	return true ;
}

