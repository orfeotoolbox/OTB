#include <ossim/projection/ossimEnvisatAsarModel.h>
#include <ossim/projection/SARModel/Ephemeris/GalileanEphemeris.h>
#include <ossim/projection/SARModel/Ephemeris/GeographicEphemeris.h>

#include <ossim/projection/SARModel/DateTime/GMSTDateTime.h>
#include <ossim/projection/SARModel/DateTime/CivilDateTime.h>
#include <ossim/projection/SARModel/DateTime/MJDDateTime.h>

#include <ossim/projection/SARModel/PlatformPosition.h>
#include <ossim/projection/SARModel/SensorParams.h>
#include <ossim/projection/SARModel/RefPoint.h>
#include <ossim/projection/SARModel/SarSensor.h>

#include <math.h>

RTTI_DEF1(ossimEnvisatAsarModel, "ossimEnvisatAsarModel", ossimGeometricSarSensorModel);

ossimEnvisatAsarModel::ossimEnvisatAsarModel():
	_n_srgr(0),
	_pixel_spacing(0)
{
}

ossimEnvisatAsarModel::~ossimEnvisatAsarModel()
{
}

double ossimEnvisatAsarModel::getSlantRangeFromGeoreferenced(double col) const
{
	const double CLUM        = 2.99792458e+8 ;

	double GR, GR0, relativeGroundRange, slantRange, slantrangetime;

	if (_n_srgr==0) return(-1) ; 

	std::vector<double> coefset = FindSRGRSetNumber((_refPoint->get_ephemeris())->get_date()) ;

	GR = _sensor->get_col_direction() * (col)* _pixel_spacing ; 
	GR0 = coefset[0];
	relativeGroundRange = GR-GR0;
	slantRange = coefset[1] 
							+ coefset[2]*relativeGroundRange 
							+ coefset[3]*pow(relativeGroundRange,2)
							+ coefset[4]*pow(relativeGroundRange,3) 
							+ coefset[5]*pow(relativeGroundRange,4);	

	slantrangetime = 2.0*slantRange / (CLUM);
	return  slantRange ;
}

bool ossimEnvisatAsarModel::InitSensorParams(const ossimKeywordlist &kwl, const char *prefix)
{
	const double CLUM = 2.99792458e+8;
	const char* wave_length_str = kwl.find(prefix,"wave_length");
	double wave_length = CLUM / atof(wave_length_str);
	const char* fr_str = kwl.find(prefix,"range_sampling_rate");
	double fr = atof(fr_str);

	// product type : Slant Range or Ground Range
	_isProductGeoreferenced = atoi(kwl.find(prefix,"is_groundrange"));
	double fa;
	if (!_isProductGeoreferenced) 
	{
		const char* fa_str = kwl.find(prefix,"prf");
		fa = atof(fa_str);
	}
	else
	{
		const char* fa_str = kwl.find(prefix,"line_time_interval");
		fa = 1.0/atof(fa_str);
	}

	// TODO : vérifier que ceci est ok pour les produits IMP
	const char* time_dir_pix = "INCREASE";
	const char* time_dir_lin = "INCREASE";

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

bool ossimEnvisatAsarModel::InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix)
{
	const int neph		= 5;
	long val_long;
	unsigned long val_ulong1;
	unsigned long val_ulong2;
	CivilDateTime ref_civil_date;
	
	Ephemeris** ephemeris = new Ephemeris*[neph];

	for (int i=1;i<=neph;i++)
	{	
		double pos[3];
		double vel[3];
		char name[64];

		/*
		 * Date de l'ephemeride
		*/
		sprintf(name,"state_vector_time_%i_day",i);
		long day = (long) atof(kwl.find(prefix,name));
		sprintf(name,"state_vector_time_%i_sec",i);
		unsigned long sec = (unsigned long) atof(kwl.find(prefix,name));
		sprintf(name,"state_vector_time_%i_microsec",i);
		unsigned long microsec = (unsigned long) atof(kwl.find(prefix,name));
		MJDDateTime mjdDate(day, sec, microsec);
		JSDDateTime jsd_date(mjdDate);
		/*
		 * Position de l'ephemeride
		*/
		sprintf(name,"x_pos_%i",i);
		pos[0] = (double) atoi(kwl.find(prefix,name)) * 1.0e-2;
		sprintf(name,"y_pos_%i",i);
		pos[1] = (double) atoi(kwl.find(prefix,name)) * 1.0e-2;
		sprintf(name,"z_pos_%i",i);
		pos[2] = (double) atoi(kwl.find(prefix,name)) * 1.0e-2;
		/*
		 * Vitesse de l'ephemeride
		*/
		sprintf(name,"x_vel_%i",i);
		vel[0] = (double) atoi(kwl.find(prefix,name)) * 1.0e-5;
		sprintf(name,"y_vel_%i",i);
		vel[1] = (double) atoi(kwl.find(prefix,name)) * 1.0e-5;
		sprintf(name,"z_vel_%i",i);
		vel[2] = (double) atoi(kwl.find(prefix,name)) * 1.0e-5;

		GeographicEphemeris* eph = new GeographicEphemeris(jsd_date, pos, vel);

		ephemeris[i-1] = eph;
	}

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

	

bool ossimEnvisatAsarModel::InitRefPoint(const ossimKeywordlist &kwl, const char *prefix)
{

	// Reference image position
	if(_refPoint == NULL)
	{
		_refPoint = new RefPoint();
	}
	double sc_pix = atof(kwl.find(prefix,"samp_num")) - 1.0;
	_refPoint->set_pix_col(sc_pix);
	double sc_lin = atof(kwl.find(prefix,"line_num")) - 1.0;
	_refPoint->set_pix_line(sc_lin);

	// Reference Point distance	
	double c = 2.99792458e+8; 
	double distance = atof(kwl.find(prefix,"slant_range_time")) * (c/2.0) * 1.0e-9;	
	_refPoint->set_distance(distance);


	// Reference Point Ephemeris
	long day_ref = (long)atof(kwl.find(prefix,"first_zero_doppler_time_day"));
	unsigned long sec_ref = (unsigned long)atof(kwl.find(prefix,"first_zero_doppler_time_sec"));
	unsigned long microsec_ref = (unsigned long)atof(kwl.find(prefix,"first_zero_doppler_time_microsec"));

	MJDDateTime mjdDate_ref(day_ref, sec_ref, microsec_ref);
	JSDDateTime jsd_date_ref(mjdDate_ref);

	if(_platformPosition != NULL)
	{
		Ephemeris * ephemeris = _platformPosition->Interpolate(jsd_date_ref);
		_refPoint->set_ephemeris(ephemeris);
		delete ephemeris;
	}
	else return false;

	// Adjustement of the model using the four corner points
	// in order to use ossimSensorModel::lineSampleToWorld
	const char* nbCol_str = kwl.find(prefix,"num_pix");
	const char* nbLin_str = kwl.find(prefix,"num_lines");
	theImageSize.x      = atoi(nbCol_str);
	theImageSize.y      = atoi(nbLin_str);
	theImageClipRect    = ossimDrect(0, 0, theImageSize.x-1, theImageSize.y-1);

	// Ground Control Points extracted from the model : corner points
	std::list<ossimGpt> groundGcpCoordinates ; 
	std::list<ossimDpt> imageGcpCoordinates ;
	double h = atof(kwl.find("avg_scene_height"));

	// first line first pix
	double line = atof(kwl.find("UL_line")) - 1;
	double col  = atof(kwl.find("UL_col"))  - 1;	
	double lon  = atof(kwl.find("UL_lon")) * 1e-6;
	double lat  = atof(kwl.find("UL_lat")) * 1e-6;
	if (lon > 180.0) lon -= 360.0;
	ossimDpt imageGCP1(col,line);
	ossimGpt groundGCP1(lat, lon, h);
	groundGcpCoordinates.push_back(groundGCP1) ; 
	imageGcpCoordinates.push_back(imageGCP1) ;
	// first line last pix
	line = atof(kwl.find("UR_line")) - 1;
	col  = atof(kwl.find("UR_col"))  - 1;	
	lon  = atof(kwl.find("UR_lon")) * 1e-6;
	lat  = atof(kwl.find("UR_lat")) * 1e-6;
	if (lon > 180.0) lon -= 360.0;
	ossimDpt imageGCP2(col,line);
	ossimGpt groundGCP2(lat, lon, h);
	groundGcpCoordinates.push_back(groundGCP2) ; 
	imageGcpCoordinates.push_back(imageGCP2) ;
	// last line last pix
	line = atof(kwl.find("LR_line")) - 1;
	col  = atof(kwl.find("LR_col"))  - 1;	
	lon  = atof(kwl.find("LR_lon")) * 1e-6;
	lat  = atof(kwl.find("LR_lat")) * 1e-6;
	if (lon > 180.0) lon -= 360.0;
	ossimDpt imageGCP3(col,line);
	ossimGpt groundGCP3(lat, lon, h);
	groundGcpCoordinates.push_back(groundGCP3) ; 
	imageGcpCoordinates.push_back(imageGCP3) ;
	// last line first pix
	line = atof(kwl.find("LL_line")) - 1;
	col  = atof(kwl.find("LL_col"))  - 1;	
	lon  = atof(kwl.find("LL_lon")) * 1e-6;
	lat  = atof(kwl.find("LL_lat")) * 1e-6;
	if (lon > 180.0) lon -= 360.0;
	ossimDpt imageGCP4(col,line);
	ossimGpt groundGCP4(lat, lon, h);
	groundGcpCoordinates.push_back(groundGCP4) ; 
	imageGcpCoordinates.push_back(imageGCP4) ;

	// Default optimization 
	optimizeModel(groundGcpCoordinates, imageGcpCoordinates) ;

	return true;
}
bool ossimEnvisatAsarModel::InitSRGR(const ossimKeywordlist &kwl, const char *prefix)
{
	// product type : Slant Range or Ground Range
	_isProductGeoreferenced = atoi(kwl.find(prefix,"is_groundrange"));

	// pixel spacing
	_pixel_spacing = atof(kwl.find(prefix,"pixel_spacing"));

	// number of SRGR sets
	_n_srgr = atoi(kwl.find(prefix,"n_srgr"));


	char name[64];
	for (int i=0;i<_n_srgr;i++)
	{	
		// SRGR update time
		sprintf(name,"srgr_update_day%i",i);
		long day_ref = (long)atof(kwl.find(prefix,name));
		sprintf(name,"srgr_update_sec%i",i);
		unsigned long sec_ref = (unsigned long)atof(kwl.find(prefix,name));
		sprintf(name,"srgr_update_microsec%i",i);
		unsigned long microsec_ref = (unsigned long)atof(kwl.find(prefix,name));
		MJDDateTime mjdDate_ref(day_ref, sec_ref, microsec_ref);
		JSDDateTime jsd_date_ref(mjdDate_ref);
		_srgr_update.push_back(jsd_date_ref);

		std::vector<double> srgr_set;
		// SRGR coefficients
		sprintf(name,"srgr_coef%iA",i);
		const char* srgr_coef_strA = kwl.find(prefix,name);
		srgr_set.push_back(atof(srgr_coef_strA));
		sprintf(name,"srgr_coef%iB",i);
		const char* srgr_coef_strB = kwl.find(prefix,name);
		srgr_set.push_back(atof(srgr_coef_strB));
		sprintf(name,"srgr_coef%iC",i);
		const char* srgr_coef_strC = kwl.find(prefix,name);
		srgr_set.push_back(atof(srgr_coef_strC));
		sprintf(name,"srgr_coef%iD",i);
		const char* srgr_coef_strD = kwl.find(prefix,name);
		srgr_set.push_back(atof(srgr_coef_strD));
		sprintf(name,"srgr_coef%iE",i);
		const char* srgr_coef_strE = kwl.find(prefix,name);
		srgr_set.push_back(atof(srgr_coef_strE)*1e-10);
		sprintf(name,"srgr_coef%iF",i);
		const char* srgr_coef_strF = kwl.find(prefix,name);
		srgr_set.push_back(atof(srgr_coef_strF)*1e-10);

		_srgr_coefset.push_back(srgr_set);
	}


	return true;
}


std::vector<double> ossimEnvisatAsarModel::FindSRGRSetNumber(JSDDateTime date) const
{
	//if (_n_srgr==0) return NULL ;

	std::vector<double> delays;
	double delay;
	for (int i=0;i<_n_srgr;i++)
	{	
		JSDDateTime datetmp(_srgr_update.at(i));
		delay = date.get_second()+date.get_decimal() - (datetmp.get_second() + datetmp.get_decimal());
		delays.push_back( fabs(delay) );   
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

	return _srgr_coefset[setNumber];
}
