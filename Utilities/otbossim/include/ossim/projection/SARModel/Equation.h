#ifndef Equation_h
#define Equation_h

#include <complex>
/**
 * @brief This class manages and solves an equation of the fourth degree
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 30-01-08
 */
class Equation
{
public:
	/**
	 * @brief Constructor
	 */
	Equation();
	/**
	 * @brief Destructor
	 */
	~Equation();

	Equation(const Equation& rhs);

	Equation& operator=(const Equation& rhs);

	Equation(int degree, std::complex<double>* coefficients);

	void CreateEquation(int degree, std::complex<double>* coefficients);

	void Solve();

	int get_nbrSol()
	{
		return _nbrSol;
	};

	std::complex<double>* get_solutions()
	{
		return _solutions;
	};
	
	int * get_order()
	{
		return _order;
	};

protected:

	/**
	 * @brief Equation coefficients
	 */
	std::complex<double>* _coefficients;
	/**
	 * @brief Equation degree
	 */
	int _degree;

	void ComputeTrueDegree();

	void Normalisation();
	void DeNormalisation();
	static const double Epsilon;

	int _nbrSol;
	int * _order;
	std::complex<double>* _solutions;
private:

	enum NormalisationType
	{
		GreatValues,
		SmallValues
	};

	NormalisationType _normalisationType;
	double _normalisationCoefficient;
	int _trueDegree;

	void Solve1();
	void Solve2();
	void Solve3(int d4 =0);
	void Solve4();

	/**
	 * ??
	 */
	std::complex<double> Proche(std::complex<double> z, double epsilon);

	/** 
	 * @brief Tests whether a triple root exists in a third degree equation
	 */
	int TestDegree3Triple(std::complex<double>* a, double epsilon);
	/**
	 * @brief Tests whether a double root and a simple root exist in a third degree equation
	 */
	int TestDegree3SimpleDouble(std::complex<double>* a, double epsilon);

	/**
	 * @brief Returns the index of the smaller list element
	 */
	int IndiceMin ( int n , double *list);

	/**
	 * @brief Returns the index of the greater list element
	 */
	int IndiceMax ( int n , double *list);

	/**
	 * @brief Tests whether a quadruple root exists in a fourth degree equation
	 */
	int TestDegree4Quad ( std::complex<double> *a , double epsilon );
	/**
	 * @brief Tests whether two double roots exist in a fourth degree equation
	 */
	int TestDegree4DoubleDouble ( std::complex<double> *a , std::complex<double> *k , double epsilon );
	/**
	 * @brief Tests whether one simple root and one triple root exist in a fourth degree equation
	 */
	int TestDegree4SimpleTriple ( std::complex<double> *a , std::complex<double> *k , double epsilon );
	/**
	 * @brief Tests whether two simple root and one double root exist in a fourth degree equation
	 */
	int TestDegreeSimpleSimpleDouble( std::complex<double> *a , double epsilon );
;
};
#endif