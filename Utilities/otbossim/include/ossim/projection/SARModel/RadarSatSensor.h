#ifndef RadarSatSensor_h
#define RadarSatSensor_h

#include <ossim/projection/SARModel/Sensor.h>

class SensorParams;
class PlatformPosition;
class RefPoint;
class GeographicEphemeris;
class RectangularCoordinate;
/**
 * @ingroup SARModel
 * @brief This class provide basic location services for RadarSat sensor
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 29-01-08
 */
class RadarSatSensor : Sensor
{
public:
	/**
	 * @brief Constructor
	 *
	 * @remarks This constructor create a copy of params, position, and refpoint
	 */
	RadarSatSensor(SensorParams* params, PlatformPosition* position, RefPoint* refpoint);
	/**
	 * @brief Destructor
	 */
	~RadarSatSensor();
	
	virtual double getDistance(double col);
	virtual JSDDateTime getTime(double line);

	virtual void ImageToWorld(double imX, double imY, double& lon, double& lat);
protected:

	int localisationApriori ( GeographicEphemeris PosVit , double lambda ,
                        double dist , double fDop , int sensVisee ,
                        double rayonEqu , double rayonPol ,
                        double h , RectangularCoordinate* cart );
private:
};


#endif