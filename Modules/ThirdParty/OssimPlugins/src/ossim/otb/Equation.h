/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#ifndef Equation_h
#define Equation_h

#include <complex>
#include <vector>
#include <ossimPluginConstants.h>

namespace ossimplugins
{


/**
 * @brief This class manages and solves an equation of the fourth degree
 */
class OSSIM_PLUGINS_DLL Equation
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

   int get_nbrSol() const
   {
      return _nbrSol;
   };

   const std::complex<double>* get_solutions() const
   {
      return _solutions;
   };

   std::vector<int> get_order() const
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
   std::vector<int> _order;
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
}

#endif
