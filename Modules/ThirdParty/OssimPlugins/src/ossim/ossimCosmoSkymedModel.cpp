//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#include <cmath>
#include <cstdio>

#include <ossimCosmoSkymedModel.h>

#include <otb/GalileanEphemeris.h>
#include <otb/GeographicEphemeris.h>
#include <otb/GMSTDateTime.h>

#include <otb/PlatformPosition.h>
#include <otb/SensorParams.h>
#include <otb/RefPoint.h>
#include <otb/SarSensor.h>

namespace ossimplugins
{

RTTI_DEF1(ossimCosmoSkymedModel, "ossimCosmoSkymedModel", ossimGeometricSarSensorModel);

ossimCosmoSkymedModel::ossimCosmoSkymedModel():
  _nbCol(0),
  _SrGr_R0(0),
  _sceneCenterRangeTime(0),
  _pixel_spacing(0)
{
}

ossimCosmoSkymedModel::~ossimCosmoSkymedModel()
{
}

double ossimCosmoSkymedModel::getSlantRangeFromGeoreferenced(double col) const
{
  // in the case of Georeferenced images, _refPoint->get_distance() contains the ground range
  double relativeGroundRange = _refPoint->get_distance() + _sensor->get_col_direction() * (col-_refPoint->get_pix_col())* _pixel_spacing ;

  double slantRange = _SrGr_coeffs[0]
              + _SrGr_coeffs[1]*(relativeGroundRange-_SrGr_R0)
              + _SrGr_coeffs[2]*(pow(relativeGroundRange,2)-_SrGr_R0)
              + _SrGr_coeffs[3]*(pow(relativeGroundRange,3)-_SrGr_R0)
              + _SrGr_coeffs[4]*(pow(relativeGroundRange,4)-_SrGr_R0)
              + _SrGr_coeffs[5]*(pow(relativeGroundRange,5)-_SrGr_R0);

  return  slantRange ;
}

bool ossimCosmoSkymedModel::InitSensorParams(const ossimKeywordlist &kwl, const char *prefix)
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
  double ellip_maj = atof(ellip_maj_str) * 1000.0;  // km -> m
  const char* ellip_min_str = kwl.find(prefix,"ellip_min");
  double ellip_min = atof(ellip_min_str) * 1000.0;  // km -> m

  if(_sensor != NULL)
  {
    delete _sensor;
  }

  _sensor = new SensorParams();


  /**
  * @todo : � voir sur de vrais produits (exemples de DESCENDING et ASCENDING)
  */
  const char* orbitDirection_str = kwl.find(prefix,"orbitDirection");
  std::string orbitDirection(orbitDirection_str) ;
  int orbitDirectionSign ;
  if (orbitDirection=="DESCENDING") orbitDirectionSign = 1 ;
  else orbitDirectionSign = - 1 ;

  const char* lookDirection_str = kwl.find(prefix,"lookDirection");
  std::string lookDirection(lookDirection_str) ;
  if ((lookDirection == "Right")||(lookDirection == "RIGHT")) _sensor->set_sightDirection(SensorParams::Right) ;
  else _sensor->set_sightDirection(SensorParams::Left) ;

  const char* colsOrder_str = kwl.find(prefix,"colsOrder");
  std::string colsOrder(colsOrder_str) ;
  const char* linsOrder_str = kwl.find(prefix,"linsOrder");
  std::string linsOrder(linsOrder_str) ;
  if (colsOrder=="NEAR-FAR")
    _sensor->set_col_direction(orbitDirectionSign);
  else _sensor->set_col_direction(-orbitDirectionSign);
  if (linsOrder=="NEAR-FAR")
    _sensor->set_lin_direction(orbitDirectionSign);
  else _sensor->set_lin_direction(-orbitDirectionSign);

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

bool ossimCosmoSkymedModel::InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix)
{
  /*
   * Retrieval of ephemerisis number
   */
  const char* neph_str = kwl.find(prefix,"neph");
  int neph = atoi(neph_str);

  Ephemeris** ephemeris = new Ephemeris*[neph];

  /*
   * Retrieval of reference date
   */
  const char* referenceUTC_str = kwl.find(prefix,"referenceUTC");
  std::string referenceUTC(referenceUTC_str) ;
  CivilDateTime ref_civil_date;
  if (! UtcDateTimeStringToCivilDate(referenceUTC, ref_civil_date)) return false;

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
    float relative_date = atof(date_str);

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
    int second = (int) relative_date ;
    double decimal = relative_date - second ;
    CivilDateTime eph_civil_date(ref_civil_date.get_year(), ref_civil_date.get_month(), ref_civil_date.get_day(), second, decimal);
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

bool ossimCosmoSkymedModel::InitRefPoint(const ossimKeywordlist &kwl, const char *prefix)
{
  const char* sc_lin_str = kwl.find(prefix,"sc_lin");
  double sc_lin = atof(sc_lin_str);

  const char* sc_pix_str = kwl.find(prefix,"sc_pix");
  double sc_pix = atof(sc_pix_str);

  // const char* pixel_spacing_str = kwl.find(prefix,"pixel_spacing");
  // double pixel_spacing = atof(pixel_spacing_str);

  const char* azimuthStartTime_str = kwl.find(prefix,"azimuthStartTime");
  double azimuthStartTime = atof(azimuthStartTime_str);

  const char* rng_gate_str = kwl.find(prefix,"rng_gate");
  double rng_gate = atof(rng_gate_str);

  const char* referenceUTC_str = kwl.find(prefix,"referenceUTC");
  std::string referenceUTC(referenceUTC_str) ;
  CivilDateTime ref_civil_date;
  if (! UtcDateTimeStringToCivilDate(referenceUTC, ref_civil_date)) return false;

  if(_refPoint == NULL)
  {
    _refPoint = new RefPoint();
  }

  _refPoint->set_pix_col(sc_pix);
  _refPoint->set_pix_line(sc_lin);

  double relative_date = (azimuthStartTime + sc_lin/_sensor->get_prf());
  int second = (int) relative_date ;
  double decimal = relative_date - second ;
  CivilDateTime * date = new CivilDateTime(ref_civil_date.get_year(), ref_civil_date.get_month(), ref_civil_date.get_day(), second, decimal);

  if(_platformPosition != NULL)
  {
    Ephemeris * ephemeris = _platformPosition->Interpolate((JSDDateTime)*date);
    if (ephemeris == NULL) return false ;

    _refPoint->set_ephemeris(ephemeris);

    delete ephemeris;
  }
  else
  {
    return false;
  }

  double c = 2.99792458e+8;
  double distance = (rng_gate + sc_pix*_sensor->get_nRangeLook()/_sensor->get_sf()) * (c/2.0);

  // in the case of Georeferenced images, the "relative" ground range is stored in place of the slant range
  // (used for SlantRange computation relative to reference point, necessary for optimization)
  // here, the pixelDirection is ignored since the CSKS reference point is always at the scene centre
  if (_isProductGeoreferenced) {
    distance = _refPoint->get_pix_col() * _pixel_spacing ;
  }

  _refPoint->set_distance(distance);


  // in order to use ossimSensorModel::lineSampleToWorld
  const char* nbCol_str = kwl.find(prefix,"nbCol");
  const char* nbLin_str = kwl.find(prefix,"nbLin");
  theImageSize.x      = atoi(nbCol_str);
   theImageSize.y      = atoi(nbLin_str);
   theImageClipRect    = ossimDrect(0, 0, theImageSize.x-1, theImageSize.y-1);


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
    sprintf(name,"cornersHeight%i",k);
    const char* height_str = kwl.find(name);
    double height = atof(height_str) ;

    ossimDpt imageGCP(i,j);
    ossimGpt groundGCP(lat, lon, height);
    groundGcpCoordinates.push_back(groundGCP) ;
    imageGcpCoordinates.push_back(imageGCP) ;
  }

  // Default optimization
  optimizeModel(groundGcpCoordinates, imageGcpCoordinates) ;

  return true;
}

bool ossimCosmoSkymedModel::InitSRGR(const ossimKeywordlist &kwl, const char *prefix)
{
  const char* rangeProjectionType_str = kwl.find(prefix,"rangeProjectionType");
  std::string rangeProjectionType(rangeProjectionType_str);

  const char* pixel_spacing_str = kwl.find(prefix,"pixel_spacing");
  _pixel_spacing= atof(pixel_spacing_str);

  _isProductGeoreferenced = (rangeProjectionType=="GROUNDRANGE") ;

  // Number of columns
  const char* nbCol_str = kwl.find(prefix,"nbCol");
  _nbCol = atoi(nbCol_str);

  // SRGR polynomial reference
  const char* SrGr_R0_str = kwl.find(prefix,"SrGr_R0");
  _SrGr_R0 = atof(SrGr_R0_str);

  // SRGR coefficients
  char name[64];
  double coeff ;
  for(int i=0;i<6;i++)
  {
    sprintf(name,"SrToGr_coeffs_%i",i);
    const char* coeff_str = kwl.find(prefix,name);
    coeff = atof(coeff_str);
    _SrGr_coeffs.push_back(coeff);
  }

  return true;
}


bool ossimCosmoSkymedModel::UtcDateTimeStringToCivilDate(const std::string &utcString, CivilDateTime &outputDate) {
  // conversion :
  // try with date format yyyymmdd

  if (utcString.size() < 8) return false ;
  const char* stringUTCDate =  utcString.c_str() ;

   char year_str[5];
  for (int i=0;i<4;i++)
  {
    year_str[i] = stringUTCDate[i];
  }
  year_str[4] = '\0';

  char month_str[3];
  for (int i=4;i<6;i++)
  {
    month_str[i-4] = stringUTCDate[i];
  }
  month_str[2] = '\0';

  char day_str[3];
  for (int i=6;i<8;i++)
  {
    day_str[i-6] = stringUTCDate[i];
  }
  day_str[2] = '\0';

  outputDate.set_year(atoi(year_str));
  outputDate.set_month(atoi(month_str));
  outputDate.set_day(atoi(day_str));
  outputDate.set_second(0);
  outputDate.set_decimal(0.0);

  return true ;
}


}


