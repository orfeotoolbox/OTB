#ifndef GeodesicCoordinate_h
#define GeodesicCoordinate_h

#include <ossim/projection/SARModel/Coordinates/Coordinate.h>

/**
 * @ingroup Date
 * @brief This class represente a coordinate in a geodesic reference
 * @author @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 04-02-08
 */
 class GeodesicCoordinate : public Coordinate
 {
 public:
	/**
	  * @brief Constructor
	  */
	GeodesicCoordinate();
	/**
	 * @brief Destructor
	 */
	~GeodesicCoordinate();
	/**
	 * @brief Cosntructor with initializations
	 */
	GeodesicCoordinate(double x, double y, double z);
	/**
	 * @brief Copy constructor
	 */
	GeodesicCoordinate(const GeodesicCoordinate& rhs);
	
	/**
	 * @brief Affectation operator
	 */
	GeodesicCoordinate& operator=(const GeodesicCoordinate& rhs);
 protected:
 private:
 };
 
 #endif