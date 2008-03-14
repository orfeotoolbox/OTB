#include <ossim/projection/ossimTerraSarModel.h>
#include <ossim/projection/SARModel/Ephemeris/GalileanEphemeris.h>
#include <ossim/projection/SARModel/Ephemeris/GeographicEphemeris.h>

#include <ossim/projection/SARModel/DateTime/GMSTDateTime.h>

#include <ossim/projection/SARModel/PlatformPosition.h>
#include <ossim/projection/SARModel/SensorParams.h>
#include <ossim/projection/SARModel/RefPoint.h>
#include <ossim/projection/SARModel/SarSensor.h>

#include <math.h>

RTTI_DEF1(ossimTerraSarModel, "ossimTerraSarModel", ossimSensorModel);

ossimTerraSarModel::ossimTerraSarModel():
	_platformPosition(NULL),
	_sensor(NULL),
	_refPoint(NULL)
{
}

ossimTerraSarModel::~ossimTerraSarModel()
{
	if (_platformPosition != NULL)
	{
		delete _platformPosition;
	}

	if(_sensor != NULL)
	{
		delete _sensor;
	}

	if(_refPoint != NULL)
	{
		delete _refPoint;
	}
}

double ossimTerraSarModel::getSlantRange(double col) const
{
	const double RDR_CLUM        = 2.99792458e+8 ;
	return  _refPoint->get_distance() 
		+ _sensor->get_col_direction() * (col - (_refPoint->get_pix_col())) * ((RDR_CLUM / 2.0) * _sensor->get_nRangeLook() / _sensor->get_sf()) ;
}

JSDDateTime ossimTerraSarModel::getTime(double line) const 
{
	double dt =  _sensor->get_lin_direction() * (line - _refPoint->get_pix_line()) * _sensor->get_nAzimuthLook() / _sensor->get_prf() ;
	JSDDateTime time = _refPoint->get_ephemeris()->get_date();
	time.set_second(time.get_second() + dt);
	time.NormDate();
	return time;
}

double ossimTerraSarModel::getSlantRangeFromGeoreferenced(double col) const
{
	// iterative polynomial inversion
	const double RDR_CLUM        = 2.99792458e+8 ;
	double EPSILON = 0.0000001 ;
	double iterError = 1.0 ; 
	int maxIter = 50, nIter=0 ;
	double estimatedGroundRange, estimatedSlantRangeTime, actualGroundRange, estimatedSlantRange ; 

	
	// actual ground range computation relative to the image near side
	// in the case of Georeferenced images, _refPoint->get_distance() contains the ground range
	actualGroundRange = _refPoint->get_distance() - _sensor->get_col_direction() * (col-_refPoint->get_pix_col()) * _SrToGr_scaling_factor ; 

	estimatedSlantRangeTime = _sceneCenterRangeTime ;
	while ((fabs(iterError)>EPSILON)&& (nIter<maxIter)) {
		// estimated ground range computation from SrToGr
		estimatedGroundRange = 0.0 ; 
		for (int i=0; i<_SrToGr_coeffs.size(); i++) {
			estimatedGroundRange += _SrToGr_coeffs[i]*pow(estimatedSlantRangeTime-_SrToGr_R0,_SrToGr_exponent[i]);
		}
		
		// comparison between the estimated ground range and the actual ground range
		iterError = actualGroundRange - estimatedGroundRange ;

		// estimated slant range update
		estimatedSlantRangeTime += iterError * 2.0 / RDR_CLUM ; 

		nIter++;
	}

	estimatedSlantRange = estimatedSlantRangeTime* RDR_CLUM / 2.0 ;

	return  estimatedSlantRange  ;
}

void ossimTerraSarModel::lineSampleHeightToWorld(const ossimDpt& image_point, const double&   heightEllipsoid, ossimGpt&       worldPoint) const
{
	SarSensor sensor(_sensor,_platformPosition,_refPoint);
	double lon, lat;
	
	double slantRange;
	// Slant range computation, depending on the product type
	if (_isProductGeoreferenced) {
		slantRange = getSlantRangeFromGeoreferenced(image_point.x) ;
	}
	else {
		slantRange = getSlantRange(image_point.x) ;
	}

	JSDDateTime azimuthTime = getTime(image_point.y) ; 

	sensor.ImageToWorld(slantRange, azimuthTime, heightEllipsoid, lon, lat);

	worldPoint.lat = lat;
	worldPoint.lon = lon;
	worldPoint.hgt = heightEllipsoid ;
}

bool ossimTerraSarModel::optimizeModel(std::list<ossimGpt> groundCoordinates, std::list<ossimDpt> imageCoordinates) {

	// Inverse projection of each Ground Control Point
	std::list<ossimGpt>::iterator itGround = groundCoordinates.begin() ; 
	std::list<ossimDpt> inverseLocResults ;
	while (itGround != groundCoordinates.end()) {
		ossimDpt itLoc ;
		this->worldToLineSample(*itGround,itLoc);
		inverseLocResults.push_back(itLoc) ; 
		itGround++;
	}

	// Comparison between the estimated image coordinates of each GCP and its actual image coordinates
	double lineBias = 0.0, columnBias = 0.0 ;
	int nbPtsUsed = 0;

	std::list<ossimDpt>::iterator itActualCoords = imageCoordinates.begin() ;
	std::list<ossimDpt>::iterator itEstimatedCoords = inverseLocResults.begin() ;
	while ((itActualCoords != imageCoordinates.end())&&(itEstimatedCoords != inverseLocResults.end())) {
		
		columnBias += (itActualCoords->x - itEstimatedCoords->x ) ; 
		lineBias += (itActualCoords->y - itEstimatedCoords->y ) ;
		
		nbPtsUsed++;
		itActualCoords++;
		itEstimatedCoords++;
	}

	// Computation of bias in line and column : mean deviations
	lineBias /= nbPtsUsed ; 
	columnBias /= nbPtsUsed ; 

	// Update of the model Reference Point 
	_refPoint->set_pix_col(_refPoint->get_pix_col() - columnBias);
	_refPoint->set_pix_line(_refPoint->get_pix_line() - lineBias);

	return true ;
}


bool ossimTerraSarModel::loadState(const ossimKeywordlist &kwl, const char *prefix)
{
	if(!InitSRGR(kwl, prefix))
	{
		return false;
	}
	if(!InitPlatformPosition(kwl, prefix))
	{
		return false;
	}
	if(!InitSensorParams(kwl, prefix))
	{
		return false;
	}
	if(!InitRefPoint(kwl, prefix))
	{
		return false;
	}
	return true;
}

bool ossimTerraSarModel::InitSensorParams(const ossimKeywordlist &kwl, const char *prefix)
{
	const char* central_freq_str = kwl.find(prefix,"central_freq");
	double central_freq = atof(central_freq_str);
	const char* fr_str = kwl.find(prefix,"fr");
	double fr = atof(fr_str);
	const char* fa_str = kwl.find(prefix,"fa");
	double fa = atof(fa_str);

	//number of different looks 
	const char* n_azilok_str = kwl.find(prefix,"n_azilok");
	double n_azilok = atof(n_azilok_str);
	const char* n_rnglok_str = kwl.find(prefix,"n_rnglok");
	double n_rnglok = atof(n_rnglok_str);

	if(_sensor != NULL)
	{
		delete _sensor;
	}

	_sensor = new SensorParams();


	if (_isProductGeoreferenced) {
		const char* orbitDirection_str = kwl.find(prefix,"orbitDirection");
		std::string orbitDirection(orbitDirection_str) ;
		int orbitDirectionSign ; 
		if (orbitDirection=="DESCENDING") orbitDirectionSign = 1 ;
		else orbitDirectionSign = - 1 ;

		const char* imageDataStartWith_str = kwl.find(prefix,"imageDataStartWith");
		std::string imageDataStartWith(imageDataStartWith_str) ;
		if (imageDataStartWith=="EARLYAZNEARRG") {
				_sensor->set_col_direction(orbitDirectionSign); 
				_sensor->set_lin_direction(orbitDirectionSign);
		} else if (imageDataStartWith=="EARLYAZFARRG") {
				_sensor->set_col_direction(-orbitDirectionSign);
				_sensor->set_lin_direction(orbitDirectionSign);
		} else if (imageDataStartWith=="LATEAZNEARRG") {
				_sensor->set_col_direction(orbitDirectionSign);
				_sensor->set_lin_direction(-orbitDirectionSign);
		} else if (imageDataStartWith=="LATEAZFARRG") {
				_sensor->set_col_direction(-orbitDirectionSign);
				_sensor->set_lin_direction(-orbitDirectionSign);
		} else {
				// COSAR Files are stored starting with early azimuth, near range
				_sensor->set_col_direction(orbitDirectionSign);
				_sensor->set_lin_direction(orbitDirectionSign);
		}
	}
	else {
				_sensor->set_col_direction(1);
				_sensor->set_lin_direction(1);
	}

	_sensor->set_sf(fr);
	const double RDR_CLUM        = 2.99792458e+8 ;
	double wave_length = RDR_CLUM / central_freq ;
	_sensor->set_rwl(wave_length);
	_sensor->set_nAzimuthLook(n_azilok);
	_sensor->set_nRangeLook(n_rnglok);

	// fa is the processing PRF 
	_sensor->set_prf(fa * n_azilok);

	return true;
}

bool ossimTerraSarModel::InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix)
{
	/*
	 * Retrieval of ephemerisis number
	 */
	const char* neph_str = kwl.find(prefix,"neph");
	int neph = atoi(neph_str);

	Ephemeris** ephemeris = new Ephemeris*[neph];

	/* 
	 * Retrieval of ephemerisis
	 */
	for (int i=0;i<neph;i++)
	{
		double pos[3];
		double vit[3];
		char name[64];

	
		sprintf(name,"eph%i_date",i);
		const char* date_str = kwl.find(prefix,name);

		sprintf(name,"eph%i_posX",i);
		const char* px_str = kwl.find(prefix,name);
		pos[0] = atof(px_str);

		sprintf(name,"eph%i_posY",i);
		const char* py_str = kwl.find(prefix,name);
		pos[1] = atof(py_str);

		sprintf(name,"eph%i_posZ",i);
		const char* pz_str = kwl.find(prefix,name);
		pos[2] = atof(pz_str);


		sprintf(name,"eph%i_velX",i);
		const char* vx_str = kwl.find(prefix,name);
		vit[0] = atof(vx_str) ; //* 1.0e-3;;

		sprintf(name,"eph%i_velY",i);
		const char* vy_str = kwl.find(prefix,name);
		vit[1] = atof(vy_str) ; //* 1.0e-3;;

		sprintf(name,"eph%i_velZ",i);
		const char* vz_str = kwl.find(prefix,name);
		vit[2] = atof(vz_str) ; //* 1.0e-3;;

		/*
		 * Conversion to JSD Date
		 */
		std::string utcString(date_str);
		CivilDateTime eph_civil_date;
		if (! UtcDateTimeStringToCivilDate(utcString, eph_civil_date)) return false;

		JSDDateTime eph_jsd_date(eph_civil_date);

		GeographicEphemeris* eph = new GeographicEphemeris(eph_jsd_date,pos,vit);

		ephemeris[i] = eph;
	}
	
	/*
	 * Creation of the platform position interpolator
	 */
	if (_platformPosition != NULL)
	{
		delete _platformPosition;
	}
	_platformPosition = new PlatformPosition(ephemeris,neph);

	/*
	 * Free of memory used by ephemerisis list : the constructor copies the ephemerisis
	 */
	for (int i=0;i<neph;i++)
	{
		delete ephemeris[i];
	}
	delete[] ephemeris;

	return true;
}

bool ossimTerraSarModel::InitRefPoint(const ossimKeywordlist &kwl, const char *prefix)
{
	const char* sc_lin_str = kwl.find(prefix,"sc_lin");
	double sc_lin = atof(sc_lin_str);

	const char* sc_pix_str = kwl.find(prefix,"sc_pix");
	double sc_pix = atof(sc_pix_str);

	const char* inp_sctim_str = kwl.find(prefix,"inp_sctim");
	std::string inp_sctim_string(inp_sctim_str) ;

	const char* sceneCenterRangeTime_str = kwl.find(prefix,"sc_rng");
	_sceneCenterRangeTime = atof(sceneCenterRangeTime_str);

	if(_refPoint == NULL)
	{
		_refPoint = new RefPoint();
	}

	_refPoint->set_pix_col(sc_pix);
	_refPoint->set_pix_line(sc_lin);

	CivilDateTime * date = new CivilDateTime() ;
	if (! UtcDateTimeStringToCivilDate(inp_sctim_string, *date)) return false ;

	if(_platformPosition != NULL)
	{
		Ephemeris * ephemeris = _platformPosition->Interpolate((JSDDateTime)*date);
	
		_refPoint->set_ephemeris(ephemeris);

		delete ephemeris;
	}
	else
	{
		return false;
	}

	const double RDR_CLUM        = 2.99792458e+8 ;
	double sceneCenterSlantRange = _sceneCenterRangeTime * RDR_CLUM / 2.0 ;

	_refPoint->set_distance(sceneCenterSlantRange);

	// in the case of Georeferenced images, the ground range is stored in place of the slant range
	// (used for SlantRange computation relative to reference point, necessary for optimization)
	if (_isProductGeoreferenced) {
		double estimatedGroundRange = 0.0 ; 
		for (int i=0; i<_SrToGr_coeffs.size(); i++) {
			estimatedGroundRange += _SrToGr_coeffs[i]*pow(_sceneCenterRangeTime-_SrToGr_R0,_SrToGr_exponent[i]);
		}
		// SrToGr update : estimatedGroundRange  = sc_pix * scaling_factor
		_SrToGr_scaling_factor = estimatedGroundRange / sc_pix ;
		_refPoint->set_distance(estimatedGroundRange);
	}

	return true;
}

bool ossimTerraSarModel::InitSRGR(const ossimKeywordlist &kwl, const char *prefix)
{
	const char* rangeProjectionType_str = kwl.find(prefix,"rangeProjectionType");
	std::string rangeProjectionType(rangeProjectionType_str);

	_isProductGeoreferenced = (rangeProjectionType=="GROUNDRANGE") ;

	// Number of columns
	const char* nbCol_str = kwl.find(prefix,"nbCol");
	_nbCol = atoi(nbCol_str);

	// SRtoGR polynomial reference
	const char* SrToGr_R0_str = kwl.find(prefix,"SrToGr_R0");
	_SrToGr_R0 = atof(SrToGr_R0_str);

	// number of SRtoGR coefficients
	const char* SrToGr_coeffs_number_str = kwl.find(prefix,"SrToGr_coeffs_number");
	int srToGr_coeffs_number = atoi(SrToGr_coeffs_number_str);

	// SRtoGR coefficients and exponents
	char name[64];
	double coeff ; 
	int exponent ; 
	for(int i=0;i<srToGr_coeffs_number;i++)
	{
		sprintf(name,"SrToGr_coeffs_%i",i);
		const char* coeff_str = kwl.find(prefix,name);
		coeff = atof(coeff_str);
		_SrToGr_coeffs.push_back(coeff);
		sprintf(name,"SrToGr_exps_%i",i);
		const char* exp_str = kwl.find(prefix,name);
		exponent = atoi(exp_str);
		_SrToGr_exponent.push_back(exponent);
	}
	
	return true;
}


bool ossimTerraSarModel::UtcDateTimeStringToCivilDate(const std::string &utcString, CivilDateTime &outputDate) {
	// conversion :
	// ossimRadarSatModel handles string civil dates of the form yyyymmddhhmmssddd, 
	// while TSX date format is UTC (xs:dateTime) : yyyy-mm-ddThh:mm:ss(.s+)zzzz (where zzzz is the timeZone).
	// Here, the timeZone is ignored. 
	 if (utcString.size() < 19) return false ;
	 const char* stringUTCDate =  utcString.c_str() ;

 	char year_str[5];
	for (int i=0;i<4;i++)
	{
		year_str[i] = stringUTCDate[i];
	}
	year_str[4] = '\0';

	char month_str[3];
	for (int i=5;i<7;i++)
	{
		month_str[i-5] = stringUTCDate[i];
	}
	month_str[2] = '\0';

	char day_str[3];
	for (int i=8;i<10;i++)
	{
		day_str[i-8] = stringUTCDate[i];
	}
	day_str[2] = '\0';

	char hour_str[3];
	for (int i=11;i<13;i++)
	{
		hour_str[i-11] = stringUTCDate[i];
	}
	hour_str[2] = '\0';

	char min_str[3];
	for (int i=14;i<16;i++)
	{
		min_str[i-14] = stringUTCDate[i];
	}
	min_str[2] = '\0';

	char sec_str[3];
	for (int i=17;i<19;i++)
	{
		sec_str[i-17] = stringUTCDate[i];
	}
	sec_str[2] = '\0';

	char mili_str[4]={'0','0','0','\0'};
	if ((utcString.size() > 19)&& (stringUTCDate[19] == '.')) {
		mili_str[0]=stringUTCDate[20];
		if ((utcString.size() > 21)&&(stringUTCDate[21] != ':')&&(stringUTCDate[21] != 'Z')) {
			mili_str[1]=stringUTCDate[21];
			if ((utcString.size() > 22)&&(stringUTCDate[22] != ':')&&(stringUTCDate[22] != 'Z')) {
				mili_str[2]=stringUTCDate[22];
			}
		}
	 }

	int hour = atoi(hour_str);
	int min = atoi(min_str);
	int sec = atoi(sec_str);
	double mili = atof(mili_str);

	outputDate.set_year(atoi(year_str));
	outputDate.set_month(atoi(month_str));
	outputDate.set_day(atoi(day_str));
	outputDate.set_second(hour * 3600 + min * 60 + sec);
	outputDate.set_decimal(mili/1000.0);

	return true ;
}

