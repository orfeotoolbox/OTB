#include <ossim/projection/ossimRadarSatModel.h>
#include <ossim/projection/SARModel/Ephemeris/GalileanEphemeris.h>
#include <ossim/projection/SARModel/Ephemeris/GeographicEphemeris.h>

#include <ossim/projection/SARModel/DateTime/GMSTDateTime.h>
#include <ossim/projection/SARModel/DateTime/CivilDateTime.h>

#include <ossim/projection/SARModel/PlatformPosition.h>
#include <ossim/projection/SARModel/SensorParams.h>
#include <ossim/projection/SARModel/RefPoint.h>
#include <ossim/projection/SARModel/RadarSatSensor.h>

RTTI_DEF1(ossimRadarSatModel, "ossimRadarSatModel", ossimSensorModel);

ossimRadarSatModel::ossimRadarSatModel():
	_platformPosition(NULL),
	_sensor(NULL),
	_refPoint(NULL)
{
}

ossimRadarSatModel::~ossimRadarSatModel()
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

void ossimRadarSatModel::lineSampleHeightToWorld(const ossimDpt& image_point, const double&   heightEllipsoid, ossimGpt&       worldPoint) const
{
	

	RadarSatSensor sensor(_sensor,_platformPosition,_refPoint);
	double lon, lat;
	sensor.ImageToWorld(image_point.x, image_point.y,lon, lat);

	worldPoint.lat = lat;
	worldPoint.lon = lon;
}

bool ossimRadarSatModel::loadState(const ossimKeywordlist &kwl, const char *prefix)
{
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

bool ossimRadarSatModel::InitSensorParams(const ossimKeywordlist &kwl, const char *prefix)
{
	const char* wave_length_str = kwl.find(prefix,"wave_length");
	double wave_length = atof(wave_length_str);
	const char* fr_str = kwl.find(prefix,"fr");
	double fr = atof(fr_str);
	const char* fa_str = kwl.find(prefix,"fa");
	double fa = atof(fa_str);

	const char* time_dir_pix = kwl.find(prefix,"time_dir_pix");
	const char* time_dir_lin = kwl.find(prefix,"time_dir_lin");

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
	
	_sensor->set_prf(fa);
	_sensor->set_sf(fr);
	_sensor->set_rwl(wave_length);

	return true;
}

bool ossimRadarSatModel::InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix)
{
	const double RDR_PI          = 3.14159265358979323846 ;
	CivilDateTime ref_civil_date;
	/*
	 * On recupere la date de reference des ephemerides
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
	 * On recupere le nombre d'ephemerides
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
	 * On recupere les ephemerides
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
		 * Date de l'ehphemeride
		 */
		JSDDateTime date(ref_jsd_date);
		date.set_second(date.get_second() + i * eph_int);
		date.NormDate();

		/*
		 * Creation de la date pour le changement de repere
		 */
		GMSTDateTime * greenwich_mha = new GMSTDateTime();
		greenwich_mha->set_origine(GMSTDateTime::AN2000);
		date.AsGMSTDateTime(greenwich_mha);

		double angle = greenwich_mha_ref+(greenwich_mha->get_tms()-greenwich_mha_ref2000->get_tms())*180.0/RDR_PI;
		angle = fmod(angle,360.0);

		/*
		 * Changement de repere
		 */
		GalileanEphemeris * tmpEphemeris = new GalileanEphemeris(date,pos,vit);
		GeographicEphemeris* eph = new GeographicEphemeris();
		
		tmpEphemeris->ToGeographic(angle,eph);
		ephemeris[i] = eph;
		
		delete tmpEphemeris;

	}
	
	/*
	 * Creation de l'interpolateur de position de l'antenne
	 */
	if (_platformPosition != NULL)
	{
		delete _platformPosition;
	}
	_platformPosition = new PlatformPosition(ephemeris,neph);

	/*
	 * Liberation de la memoire utilisée par la liste des ephemerides
	 * ->Le constructeur effectue une copie des ephemerides
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
	double distance = (rng_gate + ((double)sc_pix)/_sensor->get_sf()) * (c/2.0);
	
	_refPoint->set_distance(distance);
	return true;
}
