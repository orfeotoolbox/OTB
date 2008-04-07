#include <ossim/projection/ossimRadarSatModel.h>
#include <ossim/projection/SARModel/Ephemeris/GalileanEphemeris.h>
#include <ossim/projection/SARModel/Ephemeris/GeographicEphemeris.h>

#include <ossim/projection/SARModel/DateTime/JSDDateTime.h>
#include <ossim/projection/SARModel/DateTime/GMSTDateTime.h>
#include <ossim/projection/SARModel/DateTime/CivilDateTime.h>

#include <ossim/projection/SARModel/PlatformPosition.h>
#include <ossim/projection/SARModel/SensorParams.h>
#include <ossim/projection/SARModel/RefPoint.h>
#include <ossim/projection/SARModel/SarSensor.h>

#include <math.h>

RTTI_DEF1(ossimRadarSatModel, "ossimRadarSatModel", ossimGeometricSarSensorModel);

ossimRadarSatModel::ossimRadarSatModel():
	_n_srgr(0),
	_pixel_spacing(0)
{
}

ossimRadarSatModel::~ossimRadarSatModel()
{
}

double ossimRadarSatModel::getSlantRangeFromGeoreferenced(double col) const
{
	if (_n_srgr==0) return(-1) ;

	double relativeGroundRange ; 

	// in the case of Georeferenced images, _refPoint->get_distance() contains the ground range
	relativeGroundRange = _refPoint->get_distance() + _sensor->get_col_direction() * (col-_refPoint->get_pix_col())* _pixel_spacing ; 

	int numSet = FindSRGRSetNumber((_refPoint->get_ephemeris())->get_date()) ;
	/** 
	 * @todo : could be improved (date choice)
	 */

	double slantRange = _srgr_coefset[numSet][0] 
							+ _srgr_coefset[numSet][1]*relativeGroundRange 
							+ _srgr_coefset[numSet][2]*pow(relativeGroundRange,2)
							+ _srgr_coefset[numSet][3]*pow(relativeGroundRange,3) 
							+ _srgr_coefset[numSet][4]*pow(relativeGroundRange,4) 
							+ _srgr_coefset[numSet][5]*pow(relativeGroundRange,5);
	
	return  slantRange ;
}

bool ossimRadarSatModel::InitSensorParams(const ossimKeywordlist &kwl, const char *prefix)
{
	const char* wave_length_str = kwl.find(prefix,"wave_length");
	double wave_length = atof(wave_length_str);
	const char* fr_str = kwl.find(prefix,"fr");
	double fr = atof(fr_str);
	const char* fa_str = kwl.find(prefix,"fa");
	double fa = atof(fa_str);

	/* //number of different looks 
	const char* n_azilok_str = kwl.find(prefix,"n_azilok");
	double n_azilok = atof(n_azilok_str);
	const char* n_rnglok_str = kwl.find(prefix,"n_rnglok");
	double n_rnglok = atof(n_rnglok_str);
	*/
	
	// look bandwidth rate (for localisation purpose)
	const char* bnd_azilok_str = kwl.find(prefix,"bnd_azilok");
	double bnd_azilok = atof(bnd_azilok_str);
	const char* bnd_rnglok_str = kwl.find(prefix,"bnd_rnglok");
	double bnd_rnglok = atof(bnd_rnglok_str);
	const char* bnd_azi_str = kwl.find(prefix,"bnd_azi");
	double bnd_azi = atof(bnd_azi_str);
	const char* bnd_rng_str = kwl.find(prefix,"bnd_rng");
	double bnd_rng = atof(bnd_rng_str);

	double n_azilok = bnd_azi/bnd_azilok;
	double n_rnglok = bnd_rng/bnd_rnglok;

	const char* time_dir_pix = kwl.find(prefix,"time_dir_pix");
	const char* time_dir_lin = kwl.find(prefix,"time_dir_lin");

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

	if(strcmp(time_dir_pix, "INCREASE") == 0)
	{
		_sensor->set_col_direction(1);
	}
	else
	{
		_sensor->set_col_direction(-1);
	}

	if(strcmp(time_dir_lin, "INCREASE") == 0)
	{
		_sensor->set_lin_direction(1);
	}
	else
	{
		_sensor->set_lin_direction(-1);
	}
	
	const char* lookDirection_str = kwl.find(prefix,"lookDirection");
	ossimString lookDirection(lookDirection_str) ;
	lookDirection.trim(" ") ; // eliminates trailing blanks
	if (lookDirection == "NORMAL") _sensor->set_sightDirection(SensorParams::Right) ;
	else _sensor->set_sightDirection(SensorParams::Left) ;

	_sensor->set_prf(fa);
	_sensor->set_sf(fr);
	_sensor->set_rwl(wave_length);
	_sensor->set_nAzimuthLook(n_azilok);
	_sensor->set_nRangeLook(n_rnglok);

	_sensor->set_semiMajorAxis(ellip_maj) ; 
	_sensor->set_semiMinorAxis(ellip_min) ; 

	return true;
}

bool ossimRadarSatModel::InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix)
{
	const double _PI          = 3.14159265358979323846 ;
	CivilDateTime ref_civil_date;
	/*
	 * Ephemerisis reference date retrieval
	 */
	const char* eph_year_str = kwl.find(prefix,"eph_year");
	int eph_year = atoi(eph_year_str);
	const char* eph_month_str = kwl.find(prefix,"eph_month");
	int eph_month = atoi(eph_month_str);
	const char* eph_day_str = kwl.find(prefix,"eph_day");
	int eph_day = atoi(eph_day_str);
	const char* eph_sec_str = kwl.find(prefix,"eph_sec");
	double eph_sec = atof(eph_sec_str);

	ref_civil_date.set_year(eph_year);
	ref_civil_date.set_month(eph_month);
	ref_civil_date.set_day(eph_day);
	ref_civil_date.set_second((int)eph_sec);
	ref_civil_date.set_decimal( eph_sec-(double)((int)eph_sec));


	JSDDateTime ref_jsd_date(ref_civil_date);
	
	const char* eph_int_str = kwl.find(prefix, "eph_int");
	double eph_int = atof(eph_int_str);
	/*
	 * Ephemerisis number retrieval
	 */
	const char* neph_str = kwl.find(prefix,"neph");
	int neph = atoi(neph_str);

	Ephemeris** ephemeris = new Ephemeris*[neph];

	const char* hr_angle_str = kwl.find(prefix,"hr_angle");
	double greenwich_mha_ref = atof(hr_angle_str);

	GMSTDateTime * greenwich_mha_ref2000 = new GMSTDateTime();
	greenwich_mha_ref2000->set_origine(GMSTDateTime::AN2000);
	ref_civil_date.AsGMSTDateTime(greenwich_mha_ref2000);

	/* 
	 * Ephemerisis retrieval
	 */
	for (int i=0;i<neph;i++)
	{
		double pos[3];
		double vit[3];
		char name[64];

	
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
		vit[0] = atof(vx_str) * 1.0e-3;;

		sprintf(name,"eph%i_velY",i);
		const char* vy_str = kwl.find(prefix,name);
		vit[1] = atof(vy_str) * 1.0e-3;;

		sprintf(name,"eph%i_velZ",i);
		const char* vz_str = kwl.find(prefix,name);
		vit[2] = atof(vz_str) * 1.0e-3;;

		/*
		 * Ephemerisis date
		 */
		JSDDateTime date(ref_jsd_date);
		date.set_second(date.get_second() + i * eph_int);
		date.NormDate();

		/*
		 * Date creation for referential change
		 */
		GMSTDateTime * greenwich_mha = new GMSTDateTime();
		greenwich_mha->set_origine(GMSTDateTime::AN2000);
		date.AsGMSTDateTime(greenwich_mha);

		double angle = greenwich_mha_ref+(greenwich_mha->get_tms()-greenwich_mha_ref2000->get_tms())*180.0/_PI;
		angle = fmod(angle,360.0);

		/*
		 * Referential change
		 */
		GalileanEphemeris * tmpEphemeris = new GalileanEphemeris(date,pos,vit);
		GeographicEphemeris* eph = new GeographicEphemeris();
		
		tmpEphemeris->ToGeographic(angle,eph);
		ephemeris[i] = eph;
		
		delete tmpEphemeris;

	}
	
	/*
	 * Antenna position interpolator creation
	 */
	if (_platformPosition != NULL)
	{
		delete _platformPosition;
	}
	_platformPosition = new PlatformPosition(ephemeris,neph);

	/*
	 * Free of memory used by the ephemerisis list
	 */
	for (int i=0;i<neph;i++)
	{
		delete ephemeris[i];
	}
	delete[] ephemeris;

	return true;
}

bool ossimRadarSatModel::InitRefPoint(const ossimKeywordlist &kwl, const char *prefix)
{
	const char* sc_lin_str = kwl.find(prefix,"sc_lin");
	double sc_lin = atof(sc_lin_str);

	const char* sc_pix_str = kwl.find(prefix,"sc_pix");
	double sc_pix = atof(sc_pix_str);

	const char* inp_sctim_str = kwl.find(prefix,"inp_sctim");

	const char* rng_gate_str = kwl.find(prefix,"rng_gate");
	double rng_gate = atof(rng_gate_str);

	if(_refPoint == NULL)
	{
		_refPoint = new RefPoint();
	}

	_refPoint->set_pix_col(sc_pix);
	_refPoint->set_pix_line(sc_lin);

	char year_str[5];
	for (int i=0;i<4;i++)
	{
		year_str[i] = inp_sctim_str[i];
	}
	year_str[4] = '\0';

	char month_str[3];
	for (int i=4;i<6;i++)
	{
		month_str[i-4] = inp_sctim_str[i];
	}
	month_str[2] = '\0';

	char day_str[3];
	for (int i=6;i<8;i++)
	{
		day_str[i-6] = inp_sctim_str[i];
	}
	day_str[2] = '\0';

	char hour_str[3];
	for (int i=8;i<10;i++)
	{
		hour_str[i-8] = inp_sctim_str[i];
	}
	hour_str[2] = '\0';

	char min_str[3];
	for (int i=10;i<12;i++)
	{
		min_str[i-10] = inp_sctim_str[i];
	}
	min_str[2] = '\0';

	char sec_str[3];
	for (int i=12;i<14;i++)
	{
		sec_str[i-12] = inp_sctim_str[i];
	}
	sec_str[2] = '\0';

	char mili_str[4];
	for (int i=14;i<17;i++)
	{
		mili_str[i-14] = inp_sctim_str[i];
	}
	mili_str[3] = '\0';

	int year = atoi(year_str);
	int month = atoi(month_str);
	int day = atoi(day_str);
	int hour = atoi(hour_str);
	int min = atoi(min_str);
	int sec = atoi(sec_str);
	double mili = atof(mili_str);


	CivilDateTime date(year, month, day, hour * 3600 + min * 60 + sec, mili/1000.0);
	
	if(_platformPosition != NULL)
	{
		Ephemeris * ephemeris = _platformPosition->Interpolate((JSDDateTime)date);
	
		_refPoint->set_ephemeris(ephemeris);

		delete ephemeris;
	}
	else
	{
		return false;
	}

	double c = 2.99792458e+8;

	double distance = (rng_gate + ((double)sc_pix)*_sensor->get_nRangeLook()/_sensor->get_sf()) * (c/2.0);
	
	// in the case of Georeferenced images, the "relative" ground range is stored in place of the slant range
	// (used for SlantRange computation relative to reference point, necessary for optimization)
	// here, the pixelDirection is ignored since the RDS reference point is always at the scene centre
	if (_isProductGeoreferenced) {
		distance = _refPoint->get_pix_col() * _pixel_spacing ; 
	}

	_refPoint->set_distance(distance);


//// essai : line spacing et vitesse satellite
//	const char* line_spacing_str = kwl.find(prefix,"line_spacing");
//	double line_spacing = atof(line_spacing_str);
//	Ephemeris * ephemeris = _refPoint->get_ephemeris() ; 
//	double velSat = sqrt(pow(ephemeris->get_vitesse()[0], 2)+ pow(ephemeris->get_vitesse()[1], 2)+ pow(ephemeris->get_vitesse()[2], 2)); 
//	double prfeq = velSat / line_spacing ; 
//	_sensor->set_prf(prfeq);
//	_sensor->set_nAzimuthLook(1.0);
///**
// * @todo : effacer 
// */

	// in order to use ossimSensorModel::lineSampleToWorld
	const char* nbCol_str = kwl.find(prefix,"nbCol");
	const char* nbLin_str = kwl.find(prefix,"nbLin");
	theImageSize.x      = atoi(nbCol_str);
   theImageSize.y      = atoi(nbLin_str);
   theImageClipRect    = ossimDrect(0, 0, theImageSize.x-1, theImageSize.y-1);

	// sensor PRF update in the case of ground projected products
	if (_isProductGeoreferenced) {
		const char* acq_msec_first_str = kwl.find("acq_msec_first");
		double acq_msec_first = atof(acq_msec_first_str);
		const char* acq_msec_last_str = kwl.find("acq_msec_last");
		double acq_msec_last = atof(acq_msec_last_str);

		double actualPRF = 1000.0*theImageSize.y/(acq_msec_last-acq_msec_first) ;
		_sensor->set_nAzimuthLook(_sensor->get_prf()/actualPRF); 
	}

	// Ground Control Points extracted from the model : scene center
	std::list<ossimGpt> groundGcpCoordinates ; 
	std::list<ossimDpt> imageGcpCoordinates ; 
	const char* lon_str = kwl.find("pro_long");
	double lon = atof(lon_str);
	const char* lat_str = kwl.find("pro_lat");
	double lat = atof(lat_str);
	const char* height_str = kwl.find("terrain_height");
	double height = 0 ; //atof(height_str) ;

	ossimDpt imageGCP(sc_pix,sc_lin);
	ossimGpt groundGCP(lat, lon, height);
	groundGcpCoordinates.push_back(groundGCP) ; 
	imageGcpCoordinates.push_back(imageGCP) ;

	// Ground Control Points extracted from the model : scene corners
	// upper left corner
	lon = atof(kwl.find("cornersLon0")); 
	lat = atof(kwl.find("cornersLat0"));
	ossimDpt imageGCP0(0,0);
	ossimGpt groundGCP0(lat, lon, height);
	groundGcpCoordinates.push_back(groundGCP0) ; 
	imageGcpCoordinates.push_back(imageGCP0) ;
	// upper right corner
	lon = atof(kwl.find("cornersLon1")); 
	lat = atof(kwl.find("cornersLat1"));
	ossimDpt imageGCP1(theImageSize.x-1,0);
	ossimGpt groundGCP1(lat, lon, height);
	groundGcpCoordinates.push_back(groundGCP1) ; 
	imageGcpCoordinates.push_back(imageGCP1) ;
	// lower left corner
	lon = atof(kwl.find("cornersLon2")); 
	lat = atof(kwl.find("cornersLat2"));
	ossimDpt imageGCP2(0,theImageSize.y-1);
	ossimGpt groundGCP2(lat, lon, height);
	groundGcpCoordinates.push_back(groundGCP2) ; 
	imageGcpCoordinates.push_back(imageGCP2) ;
	// lower right corner
	lon = atof(kwl.find("cornersLon3")); 
	lat = atof(kwl.find("cornersLat3"));
	ossimDpt imageGCP3(theImageSize.x-1,theImageSize.y-1);
	ossimGpt groundGCP3(lat, lon, height);
	groundGcpCoordinates.push_back(groundGCP3) ; 
	imageGcpCoordinates.push_back(imageGCP3) ;

	// Default optimization 
	optimizeModel(groundGcpCoordinates, imageGcpCoordinates) ;

	return true;
}

bool ossimRadarSatModel::InitSRGR(const ossimKeywordlist &kwl, const char *prefix)
{
	// product type : format "RSAT-1-SAR-{SLC,SGG,SGC,...}-..."
	const char* file_name_str = kwl.find(prefix,"file_name");
	char format[4] ;
	// product type (SLC,SGG,SGC,...) extraction
	for (int j=11;j<14;j++)
	{
		format[j-11] = file_name_str[j];
	}
	format[3] = '\0';
	std::string format_str(format);

	_isProductGeoreferenced = (format_str=="SGX") || (format_str=="SGF");

	// pixel spacing
	const char* pixel_spacing_str = kwl.find(prefix,"pixel_spacing");
	_pixel_spacing = atof(pixel_spacing_str);

	// number of SRGR sets
	const char* n_srgr_str = kwl.find(prefix,"n_srgr");
	_n_srgr = atoi(n_srgr_str);

	char name[64];
	for (int i=0;i<_n_srgr;i++)
	{
		// SRGR update time, format : YYYY-DDD-HH:MM:SS.SSS
		sprintf(name,"srgr_update%i",i);
		const char* srgr_update_str = kwl.find(prefix,name);
		
		// years and days are ignored in the following (conversion from CivilDate to JSDDate requires a YYYY-MM-DD format)
		// could be improved
		char hour_str[3];
		for (int j=9;j<11;j++)
		{
			hour_str[j-9] = srgr_update_str[j];
		}
		hour_str[2] = '\0';

		char min_str[3];
		for (int j=12;j<14;j++)
		{
			min_str[j-12] = srgr_update_str[j];
		}
		min_str[2] = '\0';

		char sec_str[3];
		for (int j=15;j<17;j++)
		{
			sec_str[j-15] = srgr_update_str[j];
		}
		sec_str[2] = '\0';

		char mili_str[4];
		for (int j=18;j<21;j++)
		{
			mili_str[j-18] = srgr_update_str[j];
		}
		mili_str[3] = '\0';

		int hour = atoi(hour_str);
		int min = atoi(min_str);
		int sec = atoi(sec_str);
		double mili = atof(mili_str);
		_srgr_update[i] = hour * 3600 + min * 60 + sec + mili/1000.0 ;

		// SRGR coefficients
		sprintf(name,"srgr_coef%iA",i);
		const char* srgr_coef_strA = kwl.find(prefix,name);
		_srgr_coefset[i][0] = atof(srgr_coef_strA);
		sprintf(name,"srgr_coef%iB",i);
		const char* srgr_coef_strB = kwl.find(prefix,name);
		_srgr_coefset[i][1] = atof(srgr_coef_strB);
		sprintf(name,"srgr_coef%iC",i);
		const char* srgr_coef_strC = kwl.find(prefix,name);
		_srgr_coefset[i][2] = atof(srgr_coef_strC);
		sprintf(name,"srgr_coef%iD",i);
		const char* srgr_coef_strD = kwl.find(prefix,name);
		_srgr_coefset[i][3] = atof(srgr_coef_strD);
		sprintf(name,"srgr_coef%iE",i);
		const char* srgr_coef_strE = kwl.find(prefix,name);
		_srgr_coefset[i][4] = atof(srgr_coef_strE);
		sprintf(name,"srgr_coef%iF",i);
		const char* srgr_coef_strF = kwl.find(prefix,name);
		_srgr_coefset[i][5] = atof(srgr_coef_strF);
	}
	return true;
}

int ossimRadarSatModel::FindSRGRSetNumber(JSDDateTime date) const
{
	if (_n_srgr==0) return(-1) ;

	double delays[20];
	for (int i=0;i<_n_srgr;i++)
	{	
		delays[i] = fabs(date.get_second()+date.get_decimal()-_srgr_update[i]) ;   
	}
	
	int setNumber = 0 ; 
	double min_delay = delays[0] ; 
	for (int i=1;i<_n_srgr;i++)
	{
		if (delays[i]<min_delay) {
			setNumber = i ;
			min_delay = delays[i] ;
		}
	}
	return setNumber ;
}
