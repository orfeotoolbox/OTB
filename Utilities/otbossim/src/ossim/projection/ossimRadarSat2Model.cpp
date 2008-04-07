#include <ossim/projection/ossimRadarSat2Model.h>

#include <ossim/projection/SARModel/Ephemeris/GalileanEphemeris.h>
#include <ossim/projection/SARModel/Ephemeris/GeographicEphemeris.h>
#include <ossim/projection/SARModel/DateTime/GMSTDateTime.h>

#include <ossim/projection/SARModel/PlatformPosition.h>
#include <ossim/projection/SARModel/SensorParams.h>
#include <ossim/projection/SARModel/RefPoint.h>
#include <ossim/projection/SARModel/SarSensor.h>

#include <math.h>

RTTI_DEF1(ossimRadarSat2Model, "ossimRadarSat2Model", ossimGeometricSarSensorModel);

ossimRadarSat2Model::ossimRadarSat2Model():
	_nbCol(0),
	_pixel_spacing(0),
	_n_srgr(0)
{
}

ossimRadarSat2Model::~ossimRadarSat2Model()
{
}

double ossimRadarSat2Model::getSlantRangeFromGeoreferenced(double col) const
{
	if (_n_srgr==0) return(-1) ;

	double relativeGroundRange, slantRange = 0.0 ; 

	// in the case of Georeferenced images, _refPoint->get_distance() contains the ground range
	relativeGroundRange = _refPoint->get_distance() + _sensor->get_col_direction() * (col-_refPoint->get_pix_col())* _pixel_spacing ; 

	int numSet = FindSRGRSetNumber((_refPoint->get_ephemeris())->get_date()) ;
	/** 
	 * @todo : could be improved (date choice)
	 */

	for (int i=0 ; i<_SrGr_coeffs[numSet].size() ; i++) {
		slantRange += _SrGr_coeffs[numSet][i]*pow(relativeGroundRange,i) ;
	}
	
	return  slantRange ;
}

bool ossimRadarSat2Model::InitSensorParams(const ossimKeywordlist &kwl, const char *prefix)
{
	// sensor frequencies
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

	const char* lineTimeOrdering_str = kwl.find(prefix,"lineTimeOrdering");
	std::string lineTimeOrdering(lineTimeOrdering_str) ;
	const char* pixelTimeOrdering_str = kwl.find(prefix,"pixelTimeOrdering");
	std::string pixelTimeOrdering(pixelTimeOrdering_str) ;
	if (pixelTimeOrdering == "Increasing") _sensor->set_col_direction(1);
	else _sensor->set_col_direction(- 1);
	if (lineTimeOrdering == "Increasing") _sensor->set_lin_direction(1);
	else _sensor->set_lin_direction(- 1);

	const char* lookDirection_str = kwl.find(prefix,"lookDirection");
	std::string lookDirection(lookDirection_str) ;
	if ((lookDirection == "Right")||(lookDirection == "RIGHT")) _sensor->set_sightDirection(SensorParams::Right) ;
	else _sensor->set_sightDirection(SensorParams::Left) ;

	_sensor->set_sf(fr);
	const double CLUM        = 2.99792458e+8 ;
	double wave_length = CLUM / central_freq ;
	_sensor->set_rwl(wave_length);
	_sensor->set_nAzimuthLook(n_azilok);
	_sensor->set_nRangeLook(n_rnglok);

	// fa is the processing PRF 
	_sensor->set_prf(fa * n_azilok);

	_sensor->set_semiMajorAxis(ellip_maj) ; 
	_sensor->set_semiMinorAxis(ellip_min) ; 

	return true;
}

bool ossimRadarSat2Model::InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix)
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

bool ossimRadarSat2Model::InitRefPoint(const ossimKeywordlist &kwl, const char *prefix)
{
	// in order to use ossimSensorModel::lineSampleToWorld
	const char* nbCol_str = kwl.find(prefix,"nbCol");
	const char* nbLin_str = kwl.find(prefix,"nbLin");
	theImageSize.x      = atoi(nbCol_str);
   theImageSize.y      = atoi(nbLin_str);
   theImageClipRect    = ossimDrect(0, 0, theImageSize.x-1, theImageSize.y-1);

	// no reference point (e.g. scene center) is given in the annotation file
	// we choose to use the upper left corner as reference point
	if(_refPoint == NULL)
	{
		_refPoint = new RefPoint();
	}

	_refPoint->set_pix_col(0);		// upper left corner
	_refPoint->set_pix_line(0);	// upper left corner

	const char* zeroDopplerTimeFirstLine_str = kwl.find(prefix,"zeroDopplerTimeFirstLine");
	std::string zeroDopplerTimeFirstLine(zeroDopplerTimeFirstLine_str);
	
	CivilDateTime * date = new CivilDateTime() ;
	if (! UtcDateTimeStringToCivilDate(zeroDopplerTimeFirstLine, *date)) return false ;

	if (_sensor->get_lin_direction() == -1) {
		double time = (double) date->get_second() + date->get_decimal() ;  // upper left corner
		time += theImageSize.y / _sensor->get_prf() ;
		date->set_second((int) floor(time)) ;
		date->set_decimal(time - floor(time)) ; 
	}

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

	const char* slantRangeNearEdge_str = kwl.find(prefix,"slantRangeNearEdge");
	double distance = atof(slantRangeNearEdge_str);

	// in the case of Georeferenced images, the ground range is stored in place of the slant range
	// (used for SlantRange computation relative to reference point, necessary for optimization)
	if (_isProductGeoreferenced) {
		if (_sensor->get_col_direction() == 1)
			distance += 0 ; // upper left corner
		else 
			distance += _nbCol * _pixel_spacing ; 
	}

	_refPoint->set_distance(distance);

	// Ground Control Points extracted from the model : scene center and corners
	std::list<ossimGpt> groundGcpCoordinates ; 
	std::list<ossimDpt> imageGcpCoordinates ; 
	const char* nTiePoints_str = kwl.find(prefix,"nTiePoints");
	int nTiePoints = atoi(nTiePoints_str);
	char name[64];
	for (int k=0 ; k<nTiePoints ; k++) {
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
		sprintf(name,"cornersHeight%i",k);
		const char* height_str = kwl.find(name);
		double height = atof(height_str);

		ossimDpt imageGCP(i,j);
		ossimGpt groundGCP(lat ,lon , height);
		groundGcpCoordinates.push_back(groundGCP) ; 
		imageGcpCoordinates.push_back(imageGCP) ;
	}

	// Default optimization 
	optimizeModel(groundGcpCoordinates, imageGcpCoordinates) ;

	return true;
}

bool ossimRadarSat2Model::InitSRGR(const ossimKeywordlist &kwl, const char *prefix)
{
	const char* productType_str = kwl.find(prefix,"productType");
	std::string productType(productType_str);

	_isProductGeoreferenced = (productType != "SLC") ;

	// Pixel spacing
	const char* pixel_spacing_str = kwl.find(prefix,"pixel_spacing_mean");
	_pixel_spacing = atof(pixel_spacing_str);

	// Number of columns
	const char* nbCol_str = kwl.find(prefix,"nbCol");
	_nbCol = atoi(nbCol_str);

	// number of SRGR coefficient sets
	const char* SrGr_coeffs_number_str = kwl.find(prefix,"SrGr_coeffs_number");
	_n_srgr = atoi(SrGr_coeffs_number_str);

	// SRGR coefficients and exponents
	char name[64];
	for(int i=0;i<_n_srgr;i++)
	{
		std::vector<double> srgr_set ; 
		for(int j=0;j<4;j++)
		{
			sprintf(name,"SrGr_coeffs_%i_%i",i,j);
			const char* coeff_str = kwl.find(prefix,name);
			double coeff = atof(coeff_str);
			srgr_set.push_back(coeff);
		}
		_SrGr_coeffs.push_back(srgr_set);
		// SRGR polynomial reference
		sprintf(name,"SrGr_R0_%i",i);
		const char* SrGr_R0_str = kwl.find(prefix,name);
		_SrGr_R0.push_back(atof(SrGr_R0_str));
		// SRGR update time
		sprintf(name,"SrGr_update_%i",i);
		const char* SrGr_update_str = kwl.find(prefix,name);
		CivilDateTime SrGr_update_date ; 
		UtcDateTimeStringToCivilDate(std::string(SrGr_update_str), SrGr_update_date) ; 
		_srgr_update.push_back((double) SrGr_update_date.get_second()+ SrGr_update_date.get_decimal());
	}
	
	return true;
}


bool ossimRadarSat2Model::UtcDateTimeStringToCivilDate(const std::string &utcString, CivilDateTime &outputDate) {
	// conversion :
	// ossimRadarSatModel handles string civil dates of the form yyyymmddhhmmssddd, 
	// while Rds2 date format is UTC (xs:dateTime) : yyyy-mm-ddThh:mm:ss(.s+)zzzz (where zzzz is the timeZone).
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

int ossimRadarSat2Model::FindSRGRSetNumber(JSDDateTime date) const
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
	
	 
	 
