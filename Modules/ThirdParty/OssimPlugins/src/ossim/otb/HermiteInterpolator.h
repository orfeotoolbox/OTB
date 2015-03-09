//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

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


