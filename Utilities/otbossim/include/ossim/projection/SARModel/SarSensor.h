#ifndef SarSensor_h
#define SarSensor_h

#include <ossim/projection/SARModel/Sensor.h>

class SensorParams;
class PlatformPosition;
class GeographicEphemeris;
class RectangularCoordinate;
class JSDDateTime;
/**
 * @ingroup SARModel
 * @brief This class provides basic location services for SAR sensors
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 29-01-08
 */
class SarSensor : Sensor
{
public:
	/**
	 * @brief Constructor
	 *
	 * @remarks This constructor creates a copy of params and position
	 */
	SarSensor(SensorParams* params, PlatformPosition* position);
	/**
	 * @brief Destructor
	 */
	~SarSensor();

	/**
	 * @brief This function is able to convert image coordinates into geodetic world coordinates using a geometric SAR sensor model
	 *
	 * @param distance : Slant range of the image point
	 * @param time :		Azimuth time of the image point
	 * @param height :	Altitude of the world point
	 * @retval lon :		Longitude of the world point 
	 * @retval lat :		Latitude of the world point 
	 *
	 * @remark : the doppler frequency is set to zero in this implementation
	 */
	virtual int ImageToWorld(double distance, JSDDateTime time, double height, double& lon, double& lat);
protected:

	/**
	 * @brief This function is able to convert image coordinates into rectangular world coordinates
	 */
	int localisationApriori ( GeographicEphemeris PosVit , double lambda ,
                        double dist , double fDop , int sensVisee ,
                        double rayonEqu , double rayonPol ,
                        double h , RectangularCoordinate* cart );
private:
};


#endif