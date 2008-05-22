#include <ossim/imaging/RadarSat2/Rds2Annotation.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimXmlNode.h>
#include <ossim/base/ossimRefPtr.h>
#include <iostream>

Rds2Annotation::Rds2Annotation()
{
}

Rds2Annotation::~Rds2Annotation()
{
}

bool Rds2Annotation::Parse(ossimXmlDocument docXML) {

	// Rds2 date format is UTC (xs:dateTime) : yyyy-mm-ddThh:mm:ss(.s+)zzzz (where zzzz is the timeZone).

	std::vector< ossimRefPtr<ossimXmlNode> > searchResult, secondSearchResult ; 
	ossimRefPtr<ossimXmlNode> foundNode;
	ossimRefPtr<ossimXmlNode> foundSubNode ;
	ossimString * xpathTest ;

	//  Pixel order
	xpathTest = new ossimString("/product/imageAttributes/rasterAttributes/lineTimeOrdering") ;
	searchResult.clear();
	docXML.findNodes(*xpathTest, searchResult) ;
	if (searchResult.size() != 1 ) return false ;
	foundNode = searchResult.at(0) ;
	_lineTimeOrdering = foundNode->getText() ;

	xpathTest = new ossimString("/product/imageAttributes/rasterAttributes/pixelTimeOrdering") ;
	searchResult.clear();
	docXML.findNodes(*xpathTest, searchResult) ;
	if (searchResult.size() != 1 ) return false ;
	foundNode = searchResult.at(0) ;
	_pixelTimeOrdering = foundNode->getText() ;

	//  Orbit direction : Ascending/descending
	xpathTest = new ossimString("/product/sourceAttributes/orbitAndAttitude/orbitInformation/passDirection") ;
	searchResult.clear();
	docXML.findNodes(*xpathTest, searchResult) ;
	if (searchResult.size() != 1 ) return false ;
	foundNode = searchResult.at(0) ;
	_orbitDirection = foundNode->getText() ;

	//  Look direction 
	xpathTest = new ossimString("/product/sourceAttributes/radarParameters/antennaPointing") ;
	searchResult.clear();
	docXML.findNodes(*xpathTest, searchResult) ;
	if (searchResult.size() != 1 ) return false ;
	foundNode = searchResult.at(0) ;
	_lookDirection = foundNode->getText() ;

 	//  Ellipsoid semi-major axis, km
	xpathTest = new ossimString("/product/imageAttributes/geographicInformation/referenceEllipsoidParameters/semiMajorAxis") ;
   searchResult.clear();
   docXML.findNodes(*xpathTest, searchResult) ;
   if (searchResult.size() != 1 ) return false ;
   foundNode = searchResult.at(0) ;
   _ellip_maj = ( foundNode->getText() ).toDouble() / 1000.0 ;

	//  Ellipsoid semi_minor axis, km
	xpathTest = new ossimString("/product/imageAttributes/geographicInformation/referenceEllipsoidParameters/semiMinorAxis") ;
   searchResult.clear();
   docXML.findNodes(*xpathTest, searchResult) ;
   if (searchResult.size() != 1 ) return false ;
   foundNode = searchResult.at(0) ;
   _ellip_min = ( foundNode->getText() ).toDouble() / 1000.0 ;

	 //  Radar center frequency
	 xpathTest = new ossimString("/product/sourceAttributes/radarParameters/radarCenterFrequency") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _central_freq = ( foundNode->getText() ).toDouble() ;
    
	 //  Range sampling rate
	 xpathTest = new ossimString("/product/sourceAttributes/radarParameters/adcSamplingRate") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _fr = ( foundNode->getText() ).toDouble() ;

	 //  Slant Range Near Edge
	 xpathTest = new ossimString("/product/imageGenerationParameters/sarProcessingInformation/slantRangeNearEdge") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _slantRangeNearEdge = ( foundNode->getText() ).toDouble() ;

	 //  Zero Doppler Time First Line
	 xpathTest = new ossimString("/product/imageGenerationParameters/sarProcessingInformation/zeroDopplerTimeFirstLine") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _zeroDopplerTimeFirstLine = foundNode->getText() ;

	 // Nominal PRF, Hz
	 xpathTest = new ossimString("/product/sourceAttributes/radarParameters/pulseRepetitionFrequency") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _fa = ( foundNode->getText() ).toDouble() ;
    
	 // Number of azimuth looks
	 xpathTest = new ossimString("/product/imageGenerationParameters/sarProcessingInformation/numberOfAzimuthLooks") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _n_azilok = ( foundNode->getText() ).toDouble() ;
	 
	 // Number of range looks
	 xpathTest = new ossimString("/product/imageGenerationParameters/sarProcessingInformation/numberOfRangeLooks") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _n_rnglok = ( foundNode->getText() ).toDouble() ;

	 // Average terrain height, m
	 xpathTest = new ossimString("/product/imageAttributes/geographicInformation/referenceEllipsoidParameters/geodeticTerrainHeight") ; 
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _terrain_h = ( foundNode->getText() ).toDouble() ;

	// ProductType (SLC,SGF,SGX,SGC,SSG,SPG)
	 xpathTest = new ossimString("/product/imageGenerationParameters/generalProcessingInformation/productType") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 if (searchResult.size() != 1 ) return false ;
	 foundNode = searchResult.at(0) ;
	 _productType = foundNode->getText() ;

	// number of lines
	xpathTest = new ossimString("/product/imageAttributes/rasterAttributes/numberOfLines") ; 
	searchResult.clear();
	docXML.findNodes(*xpathTest, searchResult) ;
	if (searchResult.size() != 1 ) return false ;
	foundNode = searchResult.at(0) ;
	_nbLin = (foundNode->getText()).toDouble() ;

	// Slant Range FOR EACH Ground Range Projection
			// number of columns
	xpathTest = new ossimString("/product/imageAttributes/rasterAttributes/numberOfSamplesPerLine") ; 
	searchResult.clear();
	docXML.findNodes(*xpathTest, searchResult) ;
	if (searchResult.size() != 1 ) return false ;
	foundNode = searchResult.at(0) ;
	_nbCol = (foundNode->getText()).toDouble() ;
			// mean pixel spacing
	xpathTest = new ossimString("/product/imageAttributes/rasterAttributes/sampledPixelSpacing") ; 
	searchResult.clear();
	docXML.findNodes(*xpathTest, searchResult) ;
	if (searchResult.size() != 1 ) return false ;
	foundNode = searchResult.at(0) ;
	_pixel_spacing_mean = (foundNode->getText()).toDouble() ;

			// update times
	xpathTest = new ossimString("/product/imageGenerationParameters/slantRangeToGroundRange/zeroDopplerAzimuthTime") ; 
	searchResult.clear();
	docXML.findNodes(*xpathTest, searchResult) ;
	for ( unsigned int i=0 ; i<searchResult.size() ; i++) {
		foundNode = searchResult.at(i) ;
		_SrGr_update.push_back(foundNode->getText()) ;
	}

			// reference points
	xpathTest = new ossimString("/product/imageGenerationParameters/slantRangeToGroundRange/groundRangeOrigin") ; 
	searchResult.clear();
	docXML.findNodes(*xpathTest, searchResult) ;
	for ( unsigned int i=0 ; i<searchResult.size() ; i++) {
		foundNode = searchResult.at(i) ;
		_SrGr_R0.push_back(foundNode->getText().toDouble()) ;
	}
			// coefficients
	xpathTest = new ossimString("/product/imageGenerationParameters/slantRangeToGroundRange/groundToSlantRangeCoefficients") ; 
	searchResult.clear();
	docXML.findNodes(*xpathTest, searchResult) ;
	std::string coefficients;
	double coeff ;
	int indexDeb, indexFin ; 
	for ( unsigned int i=0 ; i<searchResult.size() ; i++) {
		foundNode = searchResult.at(i) ;
		coefficients = (foundNode->getText()) ; // list of coefficients ; separator : ' '
		std::vector<double> coefficientVector;
		indexDeb = 0 ;
		indexFin = 0 ;
		while (indexFin != std::string::npos) { 
			indexFin = coefficients.find(' ', indexDeb) ; 
			coeff = ossimString(coefficients.substr(indexDeb,indexFin-indexDeb)).toDouble() ; 
			coefficientVector.push_back(coeff) ; 
			indexDeb = indexFin+1 ;
		}
		_SrGr_coeffs.push_back(coefficientVector) ;
	}
	
	_ndata=0 ; // Number of state vector data points
	
	// Data point position/velocity
	xpathTest = new ossimString("/product/sourceAttributes/orbitAndAttitude/orbitInformation/stateVector") ; 
	searchResult.clear();
	docXML.findNodes(*xpathTest, searchResult) ;

	ossimString *xpathTime = new ossimString("timeStamp") ;
	ossimString *xpathPosX = new ossimString("xPosition") ;
	ossimString *xpathPosY = new ossimString("yPosition") ;
	ossimString *xpathPosZ = new ossimString("zPosition") ;
	ossimString *xpathVelX = new ossimString("xVelocity") ;
	ossimString *xpathVelY = new ossimString("yVelocity") ;
	ossimString *xpathVelZ = new ossimString("zVelocity") ;
	_eph_date.clear();
	_posX.clear(); 
	_posY.clear(); 
	_posZ.clear(); 
	_velX.clear(); 
	_velY.clear(); 
	_velZ.clear(); 

	 for (unsigned int i=0 ; i<searchResult.size() ; i++) {
		foundNode = searchResult.at(i) ;
		
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
    
	  // Tie points longitudes
	 xpathTest = new ossimString("/product/imageAttributes/geographicInformation/geolocationGrid/imageTiePoint/geodeticCoordinate/longitude") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 for (unsigned int i=0 ; i<searchResult.size() ; i++ ) {
		foundNode = searchResult.at(i) ;
		_cornersLon.push_back( (foundNode->getText()).toDouble() ) ;
	 }

	 // Tie points latitudes
	 xpathTest = new ossimString("/product/imageAttributes/geographicInformation/geolocationGrid/imageTiePoint/geodeticCoordinate/latitude") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 for (unsigned int i=0 ; i<searchResult.size() ; i++ ) {
		foundNode = searchResult.at(i) ;
		_cornersLat.push_back( (foundNode->getText()).toDouble() ) ;
	 }

	 // Tie points heights
	 xpathTest = new ossimString("/product/imageAttributes/geographicInformation/geolocationGrid/imageTiePoint/geodeticCoordinate/height") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 for (unsigned int i=0 ; i<searchResult.size() ; i++ ) {
		foundNode = searchResult.at(i) ;
		_cornersHeight.push_back( (foundNode->getText()).toDouble() ) ;
	 }

	// Tie points columns
	 xpathTest = new ossimString("/product/imageAttributes/geographicInformation/geolocationGrid/imageTiePoint/imageCoordinate/pixel") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 for (unsigned int i=0 ; i<searchResult.size() ; i++ ) {
		foundNode = searchResult.at(i) ;
		_cornersCol.push_back( (foundNode->getText()).toDouble() ) ;
	 }
    
	 // Tie points lines
	 xpathTest = new ossimString("/product/imageAttributes/geographicInformation/geolocationGrid/imageTiePoint/imageCoordinate/line") ;
	 searchResult.clear();
	 docXML.findNodes(*xpathTest, searchResult) ;
	 for (unsigned int i=0 ; i<searchResult.size() ; i++ ) {
		foundNode = searchResult.at(i) ;
		_cornersLin.push_back( (foundNode->getText()).toDouble() ) ;
	 }

	return true ;
}

