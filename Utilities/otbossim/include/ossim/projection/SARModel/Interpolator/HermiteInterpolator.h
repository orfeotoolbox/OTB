#ifndef HermiteInterpolator_h
#define HermiteInterpolator_h


/**
 * @brief Abstract interpolator
 * @see Interpolate
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 14-01-08
 */
class HermiteInterpolator
{
public:
	/**
	 * @brief Constructor
	 */
	HermiteInterpolator();
	
	/**
	 * @brief Constructor with initialisations
	 * @param nbrPoints Number of points used to perform the interpolation
	 * @param x Values of the points abscissa
	 * @param y Values of the points 
	 * @param dy Values of the differential coefficients
	 */
	HermiteInterpolator(int nbrPoints, double* x, double* y, double* dy);
	
	/**
	 * @brief Destructor
	 */
	~HermiteInterpolator();
	
	/**
	 * @brief Copy constructor
	 */
	HermiteInterpolator(const HermiteInterpolator& rhs);
	
	/**
	 * @brief Affectation operator
	 */
	HermiteInterpolator& operator =(const HermiteInterpolator& rhs);
	/**
	 * @brief This function performs the interpolation for the abscissa x
	 * @param x Abscissa of the interpolation
	 * @param y [out] value of the point at the abscissa x
	 * @param dy [out] value of the differential coefficient at abscissa x
	 * @return Different of 0 if an error occurs
	 */
	int Interpolate(const double x, double& y, double& dy);
protected:

	void Clear();
	int _nbrPoints;
	double* _x;
	double* _y;
	double* _dy;
private:
};


#endif