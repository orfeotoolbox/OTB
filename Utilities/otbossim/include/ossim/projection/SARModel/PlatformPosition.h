#ifndef PlatformPosition_h
#define PlatformPosition_h

#include <ossim/projection/SARModel/DateTime/JSDDateTime.h>

class Ephemeris;


/**
 * @ingroup SARModel
 * @brief This class handle the platform position
 * @author @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 15-01-08
 */
class PlatformPosition
{
public:
	/**
	 * @brief Constructor
	 */
	PlatformPosition();
	/**
	 * @brief Destructor
	 */
	~PlatformPosition();
	/**
	 * @brief Copy constructor
	 */
	PlatformPosition(const PlatformPosition& rhs);
	
	/**
	 * @brief Constructor with initialisation
	 * @param data Ephemerids
	 * @param nbrData Number of Ephemeris in data
	 * @remarks All the ephemeris have to be in the same coordinates system
	 * @remarks This constructor copy the content of data, so it's the calling fonction that have to manage the memory of data.
	 */
	PlatformPosition(Ephemeris** data, int nbrData);
	
	/**
	 * @brief Affectation operator
	 */
	PlatformPosition& operator=(const PlatformPosition& rhs);
	
	/**
	 * @brief This function interpolate its ephemeris to create a new ephemeris at the given date and time
	 * @param date Date and time at wich the interpolation have to be done
	 * @return The ephemeris at the given date, or NULL if an error occur
	 */
	Ephemeris* Interpolate(JSDDateTime date);
	
	virtual PlatformPosition* Clone()
	{
		return new PlatformPosition(*this);
	};
protected:

	/**
	 * @brief Number of platforme positions
	 * @see _data
	 */
	int _nbrData;
	
	/**
	 * @brief Platform positions
	 */
	Ephemeris** _data;
	
	/**
	 * @brief This function delete all the contents of the class
	 */
	void Clear();
private:
};

#endif