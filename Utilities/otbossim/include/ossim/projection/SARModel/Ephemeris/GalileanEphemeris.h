#ifndef GalileanEphemeride_h
#define GalileanEphemeride_h

#include <ossim/projection/SARModel/Ephemeris/Ephemeris.h>

class GeographicEphemeris;
/**
 * @ingroup Ephemeris
 * @brief This class represent an ephemeris in Galilean coordinates system
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 14-01-08
 */
class GalileanEphemeris : public Ephemeris
{
public:
	/**
	 * @brief Constructor
	 */
	GalileanEphemeris();
	
	/**
	  * @brief Destructor
	  */
	~GalileanEphemeris();
	
	/**
	 * @brief Constructor with initialisations
	 */
	GalileanEphemeris(JSDDateTime date, double pos[3], double vitesse[3]);
	
	/**
	 * @brief Copy constructor
	 */
	GalileanEphemeris(const GalileanEphemeris& rhs);
	
	GalileanEphemeris(GeographicEphemeris& rhs);
	/**
	 * @brief Affectation operator
	 */
	GalileanEphemeris& operator=(const GalileanEphemeris& rhs);
	
	/**
	 * @brief This fonction convert the current ephemeris in the Geographic coordinates systèm
	 */
	void ToGeographic(GeographicEphemeris* vGeo);
	void ToGeographic(double greenwich,GeographicEphemeris* vGeo);

	operator GeographicEphemeris();
	
	/**
	 * @brief This function create a copy of the current class
	 */
	virtual Ephemeris* Clone()
	{
		return new GalileanEphemeris(*this);
	};
protected:
	int p2nutt(int newcmb, double greenwich, double day, double p[], double pd[] );
private:
};
#endif