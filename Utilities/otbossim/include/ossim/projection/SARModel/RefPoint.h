#ifndef RefPoint_h
#define RefPoint_h

class Ephemeris;

/**
 * @brief This class handle the referencial point
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 15-01-08
 */
class RefPoint
{
public:
	/**
	 * @brief Constuctor
	 */
	RefPoint();
	/**
	 * @brief Destructor
	 */
	~RefPoint();
	/**
	 * @brief Copy constructor
	 */
	RefPoint(const RefPoint& rhs);
	/**
	 * @brief Affectation operator
	 */
	RefPoint& operator=(const RefPoint& rhs);

	void set_ephemeris(Ephemeris* ephemeris);
	void set_distance(double distance);
	void set_pix_line(double pix_line);
	void set_pix_col(double pix_col);

	Ephemeris* get_ephemeris();
	double get_distance();
	double get_pix_line();
	double get_pix_col();

	virtual RefPoint* Clone()
	{
		return new RefPoint(*this);
	};
protected:
	Ephemeris* _ephemeris;
	double _distance;
	double _pix_line;
	double _pix_col;
private:
};
#endif