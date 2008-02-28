#ifndef Coordinate_h
#define Coordinate_h

/**
 * @ingroup Date
 * @brief This class represente a coordinate
 * @author @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 04-02-08
 */
 
 class Coordinate
 {
 public:
	/**
	  * @brief Constructor
	  */
	Coordinate();
	/**
	 * @brief Destructor
	 */
	~Coordinate();
	/**
	 * @brief Cosntructor with initializations
	 */
	Coordinate(double x, double y, double z);
	/**
	 * @brief Copy constructor
	 */
	Coordinate(const Coordinate& rhs);
	
	/**
	 * @brief Affectation operator
	 */
	Coordinate& operator=(const Coordinate& rhs);
	
	void set_coordinates(double x, double y, double z);
	
	double get_x()
	{
		return _x;
	};
	
	double get_y()
	{
		return _y;
	};
	
	double get_z()
	{
		return _z;
	};
 protected:
	double _x;
	double _y;
	double _z;
 private:
 };
 
 #endif