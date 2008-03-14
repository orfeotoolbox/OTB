#ifndef GeographicEphemeride_h
#define GeographicEphemeride_h

#include <ossim/projection/SARModel/Ephemeris/Ephemeris.h>

class GalileanEphemeris;
/**
 * @ingroup Ephemeris
 * @brief This class represent an ephemerie in Geographic coordinates system
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 14-01-08
 */
class GeographicEphemeris : public Ephemeris
{
public:
	/**
	 * @brief Constructor
	 */
	GeographicEphemeris();
	/**
	  * @brief Destructor
	  */
	~GeographicEphemeris();
	
	/**
	 * @brief Constructor with initialisations
	 */
	GeographicEphemeris(JSDDateTime date, double pos[3], double vitesse[3]);
	/**
	 * @brief Copy constructor
	 */
	GeographicEphemeris(const GeographicEphemeris& rhs);
	
	GeographicEphemeris(GalileanEphemeris& rhs);
	/**
	 * @brief Affectation operator
	 */
	GeographicEphemeris& operator=(const GeographicEphemeris& rhs);
	
	/**
	 * @brief This fonction convert the current ephemeris in the Galilean coordinates system
	 */
	void ToGalilean(GalileanEphemeris* vGal);
	
	operator GalileanEphemeris();
	
	/**
	 * @brief This function create a copy of the current class
	 */
	virtual Ephemeris* Clone()
	{
		return new GeographicEphemeris(*this);
	};
protected:
private:

};
#endif