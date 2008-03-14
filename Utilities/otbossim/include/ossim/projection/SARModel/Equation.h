#ifndef Equation_h
#define Equation_h

#include <complex>
/**
 * @brief This class manage and solve an equation of the forth degree
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
	 * @brief Test dans une equation du troisieme degres l'existance d'une racine triple
	 */
	int TestDegree3Triple(std::complex<double>* a, double epsilon);
	/**
	 * @brief Test dans une equation du troisieme degres l'existance d'une racine simple et d'une racine double
	 */
	int TestDegree3SimpleDouble(std::complex<double>* a, double epsilon);

	/**
	 * @brief Retourne l'indice du plus petit élement de la liste list
	 */
	int IndiceMin ( int n , double *list);

	/**
	 * @brief Retourne l'indice du plus grand élement de la liste list
	 */
	int IndiceMax ( int n , double *list);

	/**
	 * @brief Test dans une equation du quatrieme degres l'existance d'une racine quadruple
	 */
	int TestDegree4Quad ( std::complex<double> *a , double epsilon );
	/**
	 * @brief Test dans une equation du quatrieme degres l'existance de deux racines doubles
	 */
	int TestDegree4DoubleDouble ( std::complex<double> *a , std::complex<double> *k , double epsilon );
	/**
	 * @brief Test dans une equation du quatrieme degres l'existance d'une racine sipmple et d'une racine triple
	 */
	int TestDegree4SimpleTriple ( std::complex<double> *a , std::complex<double> *k , double epsilon );
	/**
	 * @brief Test dans une equation du quatrieme degres l'existance de deux racines simples et d'une racine double
	 */
	int TestDegreeSimpleSimpleDouble( std::complex<double> *a , double epsilon );
;
};
#endif