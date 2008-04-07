#include <ossim/projection/ossimErsSarModel.h>
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
#include <cctype> // for toupper
#include <string>
#include <algorithm>

RTTI_DEF1(ossimErsSarModel, "ossimErsSarModel", ossimGeometricSarSensorModel);

ossimErsSarModel::ossimErsSarModel():
	_n_srgr(0),
	_pixel_spacing(0)
{
}

ossimErsSarModel::~ossimErsSarModel()
{
}

double ossimErsSarModel::getSlantRangeFromGeoreferenced(double col) const
{
	const double c =  2.99792458e+8;
	double tn = _srgr_coefset[0][0] + _srgr_coefset[0][1] * col + _srgr_coefset[0][2] * col*col ;
	return tn * (c/2.0);
}

bool ossimErsSarModel::InitSensorParams(const ossimKeywordlist &kwl, const char *prefix)
{
	const char* wave_length_str = kwl.find(prefix,"wave_length");
	double wave_length = atof(wave_length_str);
	const char* fr_str = kwl.find(prefix,"fr");
	double fr = atof(fr_str)*1e6;
	const char* fa_str = kwl.find(prefix,"fa");
	double fa = atof(fa_str);

	std::string time_dir_pix = kwl.find(prefix,"time_dir_pix");
	std::transform(time_dir_pix.begin(), time_dir_pix.end(), time_dir_pix.begin(),(int(*)(int))toupper);
	std::string time_dir_lin = kwl.find(prefix,"time_dir_lin");
	std::transform(time_dir_lin.begin(), time_dir_lin.end(), time_dir_lin.begin(),(int(*)(int))toupper);

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

	if(strcmp(time_dir_pix.c_str(), "INCREASE") == 0)
	{
		_sensor->set_col_direction(1);
	}
	else
	{
		_sensor->set_col_direction(-1);
	}

	if(strcmp(time_dir_lin.c_str(), "INCREASE") == 0)
	{
		_sensor->set_lin_direction(1);
	}
	else
	{
		_sensor->set_lin_direction(-1);
	}

	_sensor->set_sightDirection(SensorParams::Right) ;

	double nlooks_az = atof(kwl.find(prefix,"nlooks_az"));
	_sensor->set_nAzimuthLook(nlooks_az);
	double n_rnglok = atof(kwl.find(prefix,"n_rnglok"));
	_sensor->set_nRangeLook(n_rnglok);

	_sensor->set_prf(fa);
	_sensor->set_sf(fr);
	_sensor->set_rwl(wave_length);

	_sensor->set_semiMajorAxis(ellip_maj) ; 
	_sensor->set_semiMinorAxis(ellip_min) ; 

	return true;
}

bool ossimErsSarModel::InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix)
{
	const double PI          = 3.14159265358979323846 ;
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
	
	/*
	 * Ephemerisis time interval retrieval
	 */
	const char* eph_int_str = kwl.find(prefix, "eph_int");
	double eph_int = atof(eph_int_str);
	/*
	 * Ephemerisis number retrieval
	 */
	const char* neph_str = kwl.find(prefix,"neph");
	int neph = atoi(neph_str);

	Ephemeris** ephemeris = new Ephemeris*[neph];

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
		vit[0] = atof(vx_str);

		sprintf(name,"eph%i_velY",i);
		const char* vy_str = kwl.find(prefix,name);
		vit[1] = atof(vy_str);

		sprintf(name,"eph%i_velZ",i);
		const char* vz_str = kwl.find(prefix,name);
		vit[2] = atof(vz_str);

		/*
		 * Ephemerisis date
		 */
		JSDDateTime date(ref_jsd_date);
		date.set_second(date.get_second() + i * eph_int);
		date.NormDate();

		GeographicEphemeris* eph = new GeographicEphemeris(date, pos, vit);

		ephemeris[i] = eph;
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

bool ossimErsSarModel::InitRefPoint(const ossimKeywordlist &kwl, const char *prefix)
{
	const char* sc_lin_str = kwl.find(prefix,"sc_lin");
	double sc_lin = atof(sc_lin_str);

	const char* sc_pix_str = kwl.find(prefix,"sc_pix");
	double sc_pix = atof(sc_pix_str);

	const char* inp_sctim_str = kwl.find(prefix,"inp_sctim");

	const char* rng_gate_str = kwl.find(prefix,"zero_dop_range_time_f_pixel");
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

	double distance = (rng_gate*1e-3 + ((double)sc_pix)*_sensor->get_nRangeLook()/_sensor->get_sf()) * (c/2.0);

	_refPoint->set_distance(distance);

	// in order to use ossimSensorModel::lineSampleToWorld
	const char* nbCol_str = kwl.find(prefix,"num_pix");
	const char* nbLin_str = kwl.find(prefix,"num_lines");
	theImageSize.x      = atoi(nbCol_str);
	theImageSize.y      = atoi(nbLin_str);
	theImageClipRect    = ossimDrect(0, 0, theImageSize.x-1, theImageSize.y-1);

	// Ground Control Points extracted from the model : corner points
	std::list<ossimGpt> groundGcpCoordinates ; 
	std::list<ossimDpt> imageGcpCoordinates ;
	// first line first pix
	const char* lon_str = kwl.find("first_line_first_pixel_lon");
	double lon = atof(lon_str);
	const char* lat_str = kwl.find("first_line_first_pixel_lat");
	double lat = atof(lat_str);
	if (lon > 180.0) lon -= 360.0;
	ossimDpt imageGCP1(0,0);
	ossimGpt groundGCP1(lat, lon, 0.0);
	groundGcpCoordinates.push_back(groundGCP1) ; 
	imageGcpCoordinates.push_back(imageGCP1) ;
	// first line last pix
	lon_str = kwl.find("first_line_last_pixel_lon");
	lon = atof(lon_str);
	lat_str = kwl.find("first_line_last_pixel_lat");
	lat = atof(lat_str);
	if (lon > 180.0) lon -= 360.0;
	ossimDpt imageGCP2(theImageSize.x-1, 0);
	ossimGpt groundGCP2(lat, lon, 0.0);
	groundGcpCoordinates.push_back(groundGCP2) ; 
	imageGcpCoordinates.push_back(imageGCP2) ;
	// last line last pix
	lon_str = kwl.find("last_line_last_pixel_lon");
	lon = atof(lon_str);
	lat_str = kwl.find("last_line_last_pixel_lat");
	lat = atof(lat_str);
	if (lon > 180.0) lon -= 360.0;
	ossimDpt imageGCP3(theImageSize.x-1,theImageSize.y-1);
	ossimGpt groundGCP3(lat, lon, 0.0);
	groundGcpCoordinates.push_back(groundGCP3) ; 
	imageGcpCoordinates.push_back(imageGCP3) ;
	// last line first pix
	lon_str = kwl.find("last_line_first_pixel_lon");
	lon = atof(lon_str);
	lat_str = kwl.find("last_line_first_pixel_lat");
	lat = atof(lat_str);
	if (lon > 180.0) lon -= 360.0;
	ossimDpt imageGCP4(0,theImageSize.y-1);
	ossimGpt groundGCP4(lat, lon, 0.0);
	groundGcpCoordinates.push_back(groundGCP4) ; 
	imageGcpCoordinates.push_back(imageGCP4) ;

	// Default optimization 
	optimizeModel(groundGcpCoordinates, imageGcpCoordinates) ;

	return true;
}

bool ossimErsSarModel::InitSRGR(const ossimKeywordlist &kwl, const char *prefix)
{
	// Product type = PRI
	std::string filename(kwl.find("filename"));
	std::transform(filename.begin(), filename.end(), filename.begin(),(int(*)(int))toupper);
	string::size_type loc = filename.find("PRI");
	if( loc != string::npos ) {
     _isProductGeoreferenced = true;
   } else {
     _isProductGeoreferenced = false;
   } 

	// Number of SRGR Coef
	_n_srgr = 3;

	// Range time for first mid and last pixel
	double t1 = atof(kwl.find("zero_dop_range_time_f_pixel"))*1e-3;
	double t2 = atof(kwl.find("zero_dop_range_time_c_pixel"))*1e-3;
	double t3 = atof(kwl.find("zero_dop_range_time_l_pixel"))*1e-3;

	// Range pixels numbers corresponding
	// Todo : check if it works with "DECREASING LINE TIME"
	double x1 = 0.0;
	double x2 = atof(kwl.find("sc_pix")) - 1.0; 
	double x3 = 2.0*(x2+1.0) -1.0 ; 

	_srgr_coefset[0][0] = t1;
	_srgr_coefset[0][1] = ((t2-t1)/(x2*x2)+(t1-t3)/(x3*x3))/((1.0/x2)-(1.0/x3));
	_srgr_coefset[0][2] = ((t2-t1)/x2 + (t1-t3)/x3)/(x2-x3);

	return true;
}
