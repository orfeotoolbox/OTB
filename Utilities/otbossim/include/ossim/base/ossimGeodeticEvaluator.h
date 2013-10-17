//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Hicks
//
// Description: GeodeticEvaluator class.
//----------------------------------------------------------------------------
#ifndef ossimGeodeticEvaluator_HEADER
#define ossimGeodeticEvaluator_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimGpt.h>


class OSSIM_DLL ossimGeodeticEvaluator
{
public:

   /**
    * @brief constructor.
    *    Defaults to WGS-84
    */
   ossimGeodeticEvaluator(const ossimEllipsoid& ell = ossimEllipsoid());

   /**
    * @brief virtual destructor.
    */
   ~ossimGeodeticEvaluator();
   

   /**
    * @brief Evaluate direct problem.
    * @brief Given: Point 1 position, azimuth & distance to point 2.
    * @brief Find:  Point 2 position, azimuth from point 2 to point 1.
    *
    * @param  p1 Point 1.
    * @param az1 Azimuth from point 1 to point 2.
    * @param   d Distance between points 1 & 2.
    * @param  p2 Point 2.
    * @param az2 Azimuth from point 2 to point 1.
    */
   bool direct(
      const ossimGpt& p1, const double& az1, const double& d, ossimGpt& p2, double& az2);
   

   /**
    * @brief Evaluate Vincenty inverse problem.
    * @brief Given: Point 1 & point 2 positions.
    * @brief Find:  Distance and azimuths between points.
    *
    * @param   p1 Point 1.
    * @param   p2 Point 2.
    * @param    d Distance between points 1 & 2.
    * @param az12 Azimuth from point 1 to point 2.
    * @param az21 Azimuth from point 2 to point 1.
    */
   bool inverse(
      const ossimGpt& p1, const ossimGpt& p2, double& d, double& az12, double& az21);

protected:
   double m_A;
   double m_B;
   double m_F;
   double m_A2;
   double m_B2;
   double m_Ecc2;
   double m_2ndEcc2;

private:

};

#endif // #ifndef ossimGeodeticEvaluator_HEADER
