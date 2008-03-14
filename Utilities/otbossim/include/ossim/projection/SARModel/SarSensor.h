#ifndef SarSensor_h
#define SarSensor_h

#include <ossim/projection/SARModel/Sensor.h>

class SensorParams;
class PlatformPosition;
class RefPoint;
class GeographicEphemeris;
class RectangularCoordinate;
class JSDDateTime;
/**
 * @ingroup SARModel
 * @brief This class provide basic location services for RadarSat sensor
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
	 * @remarks This constructor create a copy of params, position, and refpoint
	 */
	SarSensor(SensorParams* params, PlatformPosition* position, RefPoint* refpoint);
	/**
	 * @brief Destructor
	 */
	~SarSensor();

	virtual void ImageToWorld(double distance, JSDDateTime time, double height, double& lon, double& lat);
protected:

	int localisationApriori ( GeographicEphemeris PosVit , double lambda ,
                        double dist , double fDop , int sensVisee ,
                        double rayonEqu , double rayonPol ,
                        double h , RectangularCoordinate* cart );
private:
};


#endif