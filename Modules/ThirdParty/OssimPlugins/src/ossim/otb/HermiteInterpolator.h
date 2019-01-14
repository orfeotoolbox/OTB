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


#ifndef HermiteInterpolator_h
#define HermiteInterpolator_h

#include <ossimPluginConstants.h>

namespace ossimplugins
{



/**
 * @brief Abstract interpolator
 * @see Interpolate
 */
class OSSIM_PLUGINS_DLL HermiteInterpolator
{
public:
   /**
    * @brief Constructor
    */
   HermiteInterpolator();

   /**
    * @brief Constructor with initializations
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
   int Interpolate(double x, double& y, double& dy) const;

   /**
    * @brief This function performs the interpolation for the abscissa x
    * @param x Abscissa of the interpolation
    * @param y [out] value of the point at the abscissa x
    * @return Different of 0 if an error occurs
    */
   int Interpolate(double x, double& y) const;

protected:

   void Clear();
   int theNPointsAvailable;
   double* theXValues;
   double* theYValues;
   double* thedYValues;

   mutable double* prodC;
   mutable double* sumC;
   mutable bool isComputed;

   int Precompute() const; // const in a semantic way


private:
};
}

#endif


