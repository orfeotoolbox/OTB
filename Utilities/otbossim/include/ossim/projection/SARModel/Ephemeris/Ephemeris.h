#ifndef Ephemeride_h
#define Ephemeride_h

#include <ossim/projection/SARModel/DateTime/JSDDateTime.h>

/**
 * @ingroup Ephemeris
 * @brief This class represents an ephemeris
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 14-01-08
 */
 class Ephemeris
 {
public:
	/**
	 * @brief Constructor
	 */
	Ephemeris();
	/**
	  * @brief Destructor
	  */
	~Ephemeris();
	
	/**
	 * @brief Constructor with initialisations
	 */
	Ephemeris(JSDDateTime date, double pos[3], double vitesse[3]);
	/**
	 * @brief Copy constructor
	 */
	Ephemeris(const Ephemeris& rhs);
	
	/**
	 * @brief Affectation operator
	 */
	Ephemeris& operator=(const Ephemeris& rhs);
	
	/**
	 * @brief This function creatse a copy of the current class
	 */
	virtual Ephemeris* Clone()
	{
		return new Ephemeris(*this);
	};
	
	JSDDateTime get_date()
	{
		return _date;
	};
	
	double* get_position()
	{
		return _position;
	};
	
	double* get_vitesse()
	{
		return _vitesse;
	};
	
	void set_date(JSDDateTime date)
	{
		_date = date;
	};
	
	void set_position(double position[3])
	{
		_position[0] = position[0];
		_position[1] = position[1];
		_position[2] = position[2];
	};
	
	void set_vitesse(double vitesse[3])
	{
		_vitesse[0] = vitesse[0];
		_vitesse[1] = vitesse[1];
		_vitesse[2] = vitesse[2];
	};
protected:

	JSDDateTime _date;
	double _position[3];
	double _vitesse[3];
private:
 };

 #endif