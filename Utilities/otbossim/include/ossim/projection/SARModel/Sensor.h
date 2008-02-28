#ifndef Services_h
#define Services_h

#include <ossim/projection/SARModel/DateTime/JSDDateTime.h>

class SensorParams;
class PlatformPosition;
class RefPoint;
/**
 * @ingroup SARModel
 * @brief This class provide basic location services
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 24-01-08
 */
class Sensor
{
public:
	/**
	 * @brief Constructor
	 *
	 * @remarks This constructor create a copy of params, position, and refpoint
	 */
	Sensor(SensorParams* params, PlatformPosition* position, RefPoint* refpoint);
	/**
	 * @brief Destructor
	 */
	~Sensor();
	
	double virtual getDistance(double col) =0;
	JSDDateTime virtual getTime(double line) =0;
	virtual void ImageToWorld(double imX, double imY, double& lon, double& lat) = 0;
protected:
	SensorParams * _params;
	PlatformPosition* _position;
	RefPoint* _refpoint;

	/**
	 * @brief Cette fonction permet de resoudre une equation du 4 iéme degrés
	 * @param degree Degres de l'équation (i.e nombre de coefficients)
	 * @param coefs Coefficients de l'équation
	 * @param nbSol nombre de solutions distinctes
	 * @param sol Solutions de l'équation
	 * @param order Tableau donnant l'ordre des solutions (simple, double ....)
	 */
	//int virtual Solve(int degree, std::complex<double>*coefs, int& nbSol, std::complex<double>* sol, int* order);

	//virtual Normalisation(
private:
};


#endif