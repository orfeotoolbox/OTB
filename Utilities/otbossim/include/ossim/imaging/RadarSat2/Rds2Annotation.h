#ifndef Rds2Annotation_h
#define Rds2Annotation_h

#include <string>
#include <map>
#include <vector>

class ossimXmlDocument ;

/**
 * @brief This class is able to parse the XML annotation file of a RadarSat2 product
 * @author Magellium, Vincent Martin
 * @version 1.0
 * @date 03-20-08
 */
class Rds2Annotation
{
public:
	/**
	 * @brief Constructor
	 */
	Rds2Annotation();

	/**
	 * @brief Destructor
	 */
	~Rds2Annotation();

	/**
	 * @brief Reads a RadarSat2 XML annotation file
	 *
	 * @param docXML : the XML annotation file to parse
	 */
	bool Parse(ossimXmlDocument docXML);

	/**
	 * @brief Pixel order
	 */
	std::string get_lineTimeOrdering() {
		return _lineTimeOrdering ;
	} ;
	std::string get_pixelTimeOrdering() {
		return _pixelTimeOrdering ;
	} ;

	/**
	 * @brief Orbit direction
	 */
	std::string get_orbitDirection() {
		return _orbitDirection ;
	}
	/**
	 * @brief Look direction
	 */
	std::string get_lookDirection() {
		return _lookDirection ;
	}
  
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
	 * @brief Slant range near edge
	 */
    double   get_slantRangeNearEdge()
	{
		return _slantRangeNearEdge;
	};
    
	/**
	 * @brief Zero Doppler Time First Line
	 */
	 std::string   get_zeroDopplerTimeFirstLine()
	{
		return _zeroDopplerTimeFirstLine;
	};
	 

	/**
	 * @brief ProductType (SLC,SGF,SGX,SGC,SSG,SPG)
	 */
	 std::string  get_productType() {
		return _productType;
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
	 * @brief Number of lines
	 */
	double get_nbLin() {
		return _nbLin;
	}
	/**
	 * @brief Mean Pixel spacing
	 */
	double get_pixel_spacing_mean() {
		return _pixel_spacing_mean;
	}

	/**
	* @brief Slant Range FOR EACH Ground Range Projection reference points
	*/
	std::vector<double> get_SrGr_R0(){
		return _SrGr_R0;
	};

	/**
	* @brief Slant Range FOR EACH Ground Range Projection update times
	*/
	std::vector<std::string> get_SrGr_update(){
		return _SrGr_update;
	};

	/**
	* @brief Slant Range FOR EACH Ground Range Projection coefficients
	*/
	std::vector< std::vector<double> > get_SrGr_coeffs() {
		return _SrGr_coeffs;
	};

	/**
	 * @brief Tie points longitudes
	 */
	std::vector<double>  get_cornersLon() {
		return _cornersLon;
	}
	/**
	 * @brief Tie points latitudes
	 */
	std::vector<double>   get_cornersLat(){
		return _cornersLat;
	}
	/**
	 * @brief Tie points heights
	 */
	std::vector<double>   get_cornersHeight(){
		return _cornersHeight;
	}
	 /**
	 * @brief Tie points columns
	 */
	std::vector<double>   get_cornersCol(){
		return _cornersCol;
	}
	 /**
	 * @brief Tie points lines
	 */
	std::vector<double>  get_cornersLin(){
		return _cornersLin;
	}

protected:

	/**
	 * @brief Pixel order
	 */
	std::string _lineTimeOrdering;
	std::string _pixelTimeOrdering;
	/**
	 * @brief Orbit direction
	 */
	std::string _orbitDirection;
	/**
	 * @brief Look direction
	 */
	std::string _lookDirection;
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
	 * @brief Nominal PRF, Hz
	 */
    double   _fa;
	 /**
	 * @brief Slant range near edge
	 */
    double   _slantRangeNearEdge;
	 /**
	 * @brief Zero Doppler Time First Line
	 */
	 std::string _zeroDopplerTimeFirstLine;
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
	 * @brief ProductType (SLC,SGF,SGX,SGC,SSG,SPG)
	 */
    std::string  _productType;
	 /**
	 * @brief Number of columns
	 */
	double _nbCol;
	/**
	 * @brief Number of lines
	 */
	double _nbLin;
	/**
	 * @brief Mean Pixel spacing
	 */
	double _pixel_spacing_mean;
	/**
	* @brief Slant Range FOR EACH Ground Range Projection reference point
	*/
	std::vector<double> _SrGr_R0;
	/**
	* @brief Slant Range FOR EACH Ground Range Projection update times
	*/
	std::vector<std::string> _SrGr_update;
	/**
	* @brief Slant Range FOR EACH Ground Range Projection coefficients
	*/
	std::vector< std::vector<double> > _SrGr_coeffs;

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
	 * @brief Tie points longitudes
	 */
    std::vector<double>   _cornersLon;
	 /**
	 * @brief Tie points latitudes
	 */
    std::vector<double>   _cornersLat;
	 /**
	 * @brief Tie points heights
	 */
    std::vector<double>   _cornersHeight;
	 /**
	 * @brief Tie points columns
	 */
    std::vector<double>   _cornersCol;
	 /**
	 * @brief Tie points lines
	 */
    std::vector<double>   _cornersLin;

private:

};

#endif