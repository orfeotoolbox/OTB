#ifndef TsxAnnotation_h
#define TsxAnnotation_h

#include <map>
#include <vector>
#include <string>

class ossimXmlDocument ;

/**
 * @brief This class is able to parse the XML annotation file of a TerraSar level1B product
 * @author Magellium, Vincent Martin
 * @version 1.0
 * @date 02-19-08
 */
class TsxAnnotation
{
public:
	/**
	 * @brief Constructor
	 */
	TsxAnnotation();

	/**
	 * @brief Destructor
	 */
	~TsxAnnotation();

	/**
	 * @brief Reads a TerraSar XML annotation file
	 *
	 * @param docXML : the XML annotation file to parse
	 */
	bool Parse(ossimXmlDocument docXML);

	/**
	 * @brief Pixel order
	 */
	std::string get_imageDataStartWith() {
		return _imageDataStartWith ;
	} ;

	/**
	 * @brief Input scene centre time
	 */
	std::string   get_inp_sctim()
	{
		return _inp_sctim;
	};
	/**
	 * @brief Ascending/descending
	 */
	std::string   get_asc_des()
	{
		return _asc_des;
	};
	/**
	 * @brief Orbit direction
	 */
	std::string get_orbitDirection() {
		return _orbitDirection ;
	}
    /**
	 * @brief Processed scene centre latitude
	 */
	double   get_pro_lat()
	{
		return _pro_lat;
	};
    /**
	 * @brief Processed scene centre longitude
	 */
	double   get_pro_long()
	{
		return _pro_long;
	};
    /**
	 * @brief Processed scene centre headng
	 */
	double   get_pro_head()
	{
		return _pro_head;
	};

    /**
	 * @brief Ellipsoid semi-major axis, km
	 */
	double   get_ellip_maj()
	{
		return _ellip_maj;
	};
    /**
	 * @brief Ellipsoid semi_minor axis, km
	 */
	double  get_ellip_min()
	{
		return _ellip_min;
	};
	 
    /**
	 * @brief Average terrain height, km
	 */
    double   get_terrain_h()
	{
		return _terrain_h;
	};
    /**
	 * @brief Scene centre line number
	 */
    int   get_sc_lin()
	{
		return _sc_lin;
	};
    /**
	 * @brief Scene centre pixel number
	 */
    int   get_sc_pix()
	{
		return _sc_pix;
	};
      
   
    /**
	 * @brief Radar central frequency
	 */
    double   get_central_freq()
	{
		return _central_freq;
	};
       
    /**
	 * @brief Range sampling rate
	 */
    double   get_fr()
	{
		return _fr;
	};
    /**
	 * @brief Range gate start time
	 */
    double   get_rng_gate()
	{
		return _rng_gate;
	};
    
	/**
	 * @brief Scene center range time
	 */
    double  get_sceneCenterRangeTime() 
	{
		return _sceneCenterRangeTime;
	};
	 
    /**
	 * @brief Nominal PRF, Hz
	 */
    double   get_fa()
	{
		return _fa;
	};
    
    /**
	 * @brief Number of azimuth looks
	 */
    double   get_n_azilok()
	{
		return _n_azilok;
	};
    
	/**
	 * @brief Number of range looks
	 */
    double   get_n_rnglok()
	{
		return _n_rnglok;
	};
    
    /**
	 * @brief Along track Doppler frequency terms
	 */
    double*   get_alt_dopcen()
	{
		return _alt_dopcen;
	};
    
    /**
	 * @brief Cross track Doppler frequency terms
	 */
    double*   get_crt_dopcen()
	{
		return _crt_dopcen;
	};

	/**
	 * @brief Type of range data (GROUNDRANGE, SLANTRANGE, MAP, UNDEFINED)
	 */
	 std::string  get_rangeProjectionType() {
		return _rangeProjectionType;
	 };

	/**
	 * @brief Number of state vector data points
	 */
	int  get_ndata()
	{
		return _ndata;
	};
	
	/**
	 * @brief State vector data point date, position, velocity
	 */
	std::vector<std::string> get_eph_date()
	{
		return _eph_date;
	};

	std::vector<double> get_posX()
	{
		return _posX;
	};
	std::vector<double> get_posY()
	{
		return _posY;
	};
	std::vector<double> get_posZ()
	{
		return _posZ;
	};
	std::vector<double> get_velX()
	{
		return _velX;
	};
	std::vector<double> get_velY()
	{
		return _velY;
	};
	std::vector<double> get_velZ()
	{
		return _velZ;
	};
   /**
	 * @brief Number of columns
	 */
	double get_nbCol() {
		return _nbCol;
	}
	/**
	 * @brief Mean Pixel spacing
	 */
	double get_pixel_spacing_mean() {
		return _pixel_spacing_mean;
	}
	/**
	 * @brief Pixel spacing at Near range
	 */
	double get_pixel_spacing_near() {
		return _pixel_spacing_near;
	}

	/**
	 * @brief Pixel spacing at far range
	 */
	double get_pixel_spacing_far() {
		return _pixel_spacing_far;
	}

	/**
	* @brief Slant Range TO Ground Range Projection reference point
	*/
	double get_SrToGr_R0(){
		return _SrToGr_R0;
	};

	/**
	* @brief Slant Range TO Ground Range Projection coefficients
	*/
	std::vector<double> get_SrToGr_coeffs() {
		return _SrToGr_coeffs;
	};

	/**
	* @brief Slant Range TO Ground Range Projection exponents
	*/
	std::vector<int> get_SrToGr_exps() {
		return _SrToGr_exps;
	};

	/**
	 * @brief Scene corners + scene center longitudes
	 */
	double*  get_cornersLon() {
		return _cornersLon;
	}
	 /**
	 * @brief Scene corners + scene center latitudes
	 */
	double*   get_cornersLat(){
		return _cornersLat;
	}
	 /**
	 * @brief Scene corners + scene center columns
	 */
	double*   get_cornersCol(){
		return _cornersCol;
	}
	 /**
	 * @brief Scene corners + scene center lines
	 */
	double*   get_cornersLin(){
		return _cornersLin;
	}

protected:

	/**
	 * @brief Pixel order
	 */
	std::string _imageDataStartWith;
	/**
	 * @brief Orbit direction
	 */
	std::string _orbitDirection;
	/**
	 * @brief Input scene centre time
	 */
	std::string   _inp_sctim;
	/**
	 * @brief Ascending/descending
	 */
	std::string   _asc_des;
    /**
	 * @brief Processed scene centre latitude
	 */
	double   _pro_lat;
    /**
	 * @brief Processed scene centre longitude
	 */
	double   _pro_long;
    /**
	 * @brief Processed scene centre headng
	 */
	double   _pro_head;
    /**
	 * @brief Ellipsoid semi-major axis, km
	 */
	double   _ellip_maj;
    /**
	 * @brief Ellipsoid semi_minor axis, km
	 */
	double  _ellip_min;
    /**
	 * @brief Scene centre line number
	 */
    int   _sc_lin;
    /**
	 * @brief Scene centre pixel number
	 */
    int   _sc_pix;
    /**
	 * @brief Radar central frequency
	 */
    double   _central_freq;
    /**
	 * @brief Range sampling rate
	 */
    double   _fr;
    /**
	 * @brief Range gate start time
	 */
    double   _rng_gate;

	/**
	 * @brief Scene center range time
	 */
    double   _sceneCenterRangeTime;

    /**
	 * @brief Nominal PRF, Hz
	 */
    double   _fa;
    /**
	 * @brief Number of azimuth looks
	 */
    double   _n_azilok;

	 /**
	 * @brief Number of range looks
	 */
    double   _n_rnglok;

	/**
	 * @brief Average terrain height, km
	 */
    double   _terrain_h;

    /**
	 * @brief Along track Doppler frequency terms
	 */
    double   _alt_dopcen[3];
    /**
	 * @brief Cross track Doppler frequency terms
	 */
    double   _crt_dopcen[3];

	 /**
	 * @brief Type of range data (GROUNDRANGE, SLANTRANGE, MAP, UNDEFINED)
	 */
    std::string  _rangeProjectionType;
	 /**
	 * @brief Number of columns
	 */
	double _nbCol;
	/**
	 * @brief Mean Pixel spacing
	 */
	double _pixel_spacing_mean;
	/**
	 * @brief Pixel spacing at near range
	 */
	double _pixel_spacing_near;
	/**
	 * @brief Pixel spacing at far range
	 */
	double _pixel_spacing_far;
	/**
	* @brief Slant Range TO Ground Range Projection reference point
	*/
	double _SrToGr_R0;
	/**
	* @brief Slant Range TO Ground Range Projection coefficients
	*/
	 std::vector<double> _SrToGr_coeffs;

	/**
	* @brief Slant Range TO Ground Range Projection exponents
	*/
	 std::vector<int> _SrToGr_exps;

	/**
	* @brief Number of data points
	*/
	int   _ndata;

	/**
	* @brief Data point position/velocity
	*/
	std::vector<std::string> _eph_date;
	std::vector<double> _posX; 
	std::vector<double> _posY; 
	std::vector<double> _posZ; 
	std::vector<double> _velX; 
	std::vector<double> _velY; 
	std::vector<double> _velZ; 

	/**
	 * @brief Scene corners + scene center longitudes
	 */
    double   _cornersLon[5];
	 /**
	 * @brief Scene corners + scene center latitudes
	 */
    double   _cornersLat[5];
	 /**
	 * @brief Scene corners + scene center columns
	 */
    double   _cornersCol[5];
	 /**
	 * @brief Scene corners + scene center lines
	 */
    double   _cornersLin[5];

private:

};

#endif
