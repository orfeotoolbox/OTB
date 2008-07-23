#ifndef RectangularCoordinate_h
#define RectangularCoordinate_h

#include <ossim/projection/SARModel/Coordinates/Coordinate.h>

class GeodesicCoordinate;
/**
 * @ingroup Date
 * @brief This class represents a coordinate in a rectangular reference
 * @author @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 04-02-08
 */
 class RectangularCoordinate : public Coordinate
 {
 public:
	/**
	  * @brief Constructor
	  */
	RectangularCoordinate();
	/**
	 * @brief Destructor
	 */
	~RectangularCoordinate();
	/**
	 * @brief Cosntructor with initializations
	 */
	RectangularCoordinate(double x, double y, double z);
	/**
	 * @brief Copy constructor
	 */
	RectangularCoordinate(const RectangularCoordinate& rhs);
	
	/**
	 * @brief Affectation operator
	 */
	RectangularCoordinate& operator=(const RectangularCoordinate& rhs);

	void AsGeodesicCoordinates(double demiGdAxe, double demiPtAxe, GeodesicCoordinate* geod);
 protected:
 private:
 };
 
 #endif