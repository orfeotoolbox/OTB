#include <ossim/projection/ossimTerraSarModel.h>

#include <ossim/projection/SARModel/Ephemeris/GalileanEphemeris.h>
#include <ossim/projection/SARModel/Ephemeris/GeographicEphemeris.h>
#include <ossim/projection/SARModel/DateTime/GMSTDateTime.h>

#include <ossim/projection/SARModel/PlatformPosition.h>
#include <ossim/projection/SARModel/SensorParams.h>
#include <ossim/projection/SARModel/RefPoint.h>
#include <ossim/projection/SARModel/SarSensor.h>

#include <math.h>

RTTI_DEF1(ossimTerraSarModel, "ossimTerraSarModel", ossimGeometricSarSensorModel);

ossimTerraSarModel::ossimTerraSarModel():
	_nbCol(0),
	_SrToGr_R0(0),
	_sceneCenterRangeTime(0),
	_SrToGr_scaling_factor(0)
{
}

ossimTerraSarModel::~ossimTerraSarModel()
{
}

// Note : ground range to slant range coputation could be performed in three ways : 
//		(1) Slant Range to Ground Range polynomial inversion (coefficients given at mid-scene)
//		(2) use of a parabolic model from three geolocated points
//		(3) interpolation from the geolocation grid (separate file, most precise technique) 
// In this version, (1) and (2) were implemented but (1) is imprecise on the test products. 
double ossimTerraSarModel::getSlantRangeFromGeoreferenced(double col) const
{
	const double c =  2.99792458e+8;
	double tn = _alt_srgr_coefset[0] + _alt_srgr_coefset[1] * col + _alt_srgr_coefset[2] * col*col ;
	return tn * (c/2.0);
}

/*
double ossimTerraSarModel::getSlantRangeFromGeoreferenced(double col) const
{
	// iterative polynomial inversion
	const double CLUM        = 2.99792458e+8 ;
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
		estimatedSlantRangeTime += iterError * 2.0 / CLUM ; 

		nIter++;
	}

	estimatedSlantRange = estimatedSlantRangeTime* CLUM / 2.0 ;

	return  estimatedSlantRange  ;
}
*/

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

	//ellipsoid parameters
	const char* ellip_maj_str = kwl.find(prefix,"ellip_maj");
	double ellip_maj = atof(ellip_maj_str) * 1000.0;	// km -> m
	const char* ellip_min_str = kwl.find(prefix,"ellip_min");
	double ellip_min = atof(ellip_min_str) * 1000.0;  // km -> m


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

	const char* lookDirection_str = kwl.find(prefix,"lookDirection");
	std::string lookDirection(lookDirection_str) ;
	if ((lookDirection == "Right")||(lookDirection == "RIGHT")) _sensor->set_sightDirection(SensorParams::Right) ;
	else _sensor->set_sightDirection(SensorParams::Left) ;

	_sensor->set_sf(fr);
	const double CLUM        = 2.99792458e+8 ;
	double wave_length = CLUM / central_freq ;
	_sensor->set_rwl(wave_length);
	_sensor->set_nRangeLook(n_rnglok);
	_sensor->set_prf(fa) ; 
	// fa is the processing PRF 
	//_sensor->set_prf(fa * n_azilok); // number of looks disabled

	_sensor->set_semiMajorAxis(ellip_maj) ; 
	_sensor->set_semiMinorAxis(ellip_min) ; 

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
		vit[0] = atof(vx_str) ;

		sprintf(name,"eph%i_velY",i);
		const char* vy_str = kwl.find(prefix,name);
		vit[1] = atof(vy_str) ;

		sprintf(name,"eph%i_velZ",i);
		const char* vz_str = kwl.find(prefix,name);
		vit[2] = atof(vz_str) ;

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

	const double CLUM        = 2.99792458e+8 ;
	double sceneCenterSlantRange = _sceneCenterRangeTime * CLUM / 2.0 ;

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

	// in order to use ossimSensorModel::lineSampleToWorld
	const char* nbCol_str = kwl.find(prefix,"nbCol");
	const char* nbLin_str = kwl.find(prefix,"nbLin");
	theImageSize.x      = atoi(nbCol_str);
   theImageSize.y      = atoi(nbLin_str);
   theImageClipRect    = ossimDrect(0, 0, theImageSize.x-1, theImageSize.y-1);

	if (_isProductGeoreferenced) {
		std::string azimuthStartTime(kwl.find("azimuthStartTime"));
		std::string azimuthStopTime(kwl.find("azimuthStopTime"));
		CivilDateTime * dateStart = new CivilDateTime() ;
		if (! UtcDateTimeStringToCivilDate(azimuthStartTime, *dateStart)) return false ;
		CivilDateTime * dateStop = new CivilDateTime() ;
		if (! UtcDateTimeStringToCivilDate(azimuthStopTime, *dateStop)) return false ;
		double acq_msec_first = (double) dateStart->get_second()+dateStart->get_decimal();
		double acq_msec_last = (double) dateStop->get_second()+dateStop->get_decimal();

		double actualPRF = theImageSize.y/(acq_msec_last-acq_msec_first) ;
		_sensor->set_nAzimuthLook(_sensor->get_prf()/actualPRF); 
	}
	else 	
		_sensor->set_nAzimuthLook(1.0);

	// Ground Control Points extracted from the model : scene center and corners
	std::list<ossimGpt> groundGcpCoordinates ; 
	std::list<ossimDpt> imageGcpCoordinates ; 
	char name[64];
	for (int k=0 ; k<5 ; k++) {
		sprintf(name,"cornersCol%i",k);
		const char* i_str = kwl.find(name);
		int i = atoi(i_str);
		sprintf(name,"cornersLin%i",k);
		const char* j_str = kwl.find(name);
		int j = atoi(j_str);
		sprintf(name,"cornersLon%i",k);
		const char* lon_str = kwl.find(name);
		double lon = atof(lon_str);
		sprintf(name,"cornersLat%i",k);
		const char* lat_str = kwl.find(name);
		double lat = atof(lat_str);
		const char* height_str = kwl.find("terrain_h");
		double height = atof(height_str) ;

		ossimDpt imageGCP(i,j);
		ossimGpt groundGCP(lat ,lon , height);
		groundGcpCoordinates.push_back(groundGCP) ; 
		imageGcpCoordinates.push_back(imageGCP) ;
	}

	// Default optimization 
	optimizeModel(groundGcpCoordinates, imageGcpCoordinates) ;

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

	// ALTERNATIVE to provided coefficients

	// Range time for first mid and last pixel
	std::string orbitDirection(kwl.find(prefix,"orbitDirection")) ;
	double t1, t2, t3 ; 
	if (orbitDirection=="DESCENDING") {
		t3 = atof(kwl.find("start_rng"));
		t2 = atof(kwl.find("sc_rng"));
		t1 = atof(kwl.find("end_rng"));
	}
	else {
		t1 = atof(kwl.find("start_rng"));
		t2 = atof(kwl.find("sc_rng"));
		t3 = atof(kwl.find("end_rng"));
	}

	// Range pixels numbers corresponding
	double x1 = 0.0;
	double x2 = atof(kwl.find("sc_pix")); 
	double x3 = 2.0*(x2+1.0) -1.0 ; 

	_alt_srgr_coefset[0] = t1;
	_alt_srgr_coefset[1] = ((t2-t1)/(x2*x2)+(t1-t3)/(x3*x3))/((1.0/x2)-(1.0/x3));
	_alt_srgr_coefset[2] = ((t2-t1)/x2 + (t1-t3)/x3)/(x2-x3);
	
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

	
	 
	 
