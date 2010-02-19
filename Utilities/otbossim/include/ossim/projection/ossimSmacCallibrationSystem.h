//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Proecedure for compensation of aerial camera lens distortion
// as computed by the Simultaneous Multiframe Analytical Calibration
// (SMAC) System:
// 
//*******************************************************************
//  $Id$
#ifndef ossimSmacCallibrationSystem_HEADER
#define ossimSmacCallibrationSystem_HEADER
#include <ossim/projection/ossimLensDistortion.h>

/**
 * Proecedure for compensation of aerial camera lens distortion
 * as computed by the Simultaneous Multiframe Analytical Calibration
 * (SMAC) System:
 * <pre>
 *
 * TRANSLATION TO POINT OF SYMMETRY:
 *
 * All measured points (x0, y0) requiring a distortion correction must be first referenced to the principal point of
 * autocollimation (PPA) at (0,0) as are the calibrated fiducials. Once the measured points are in the PPA reference frame, they
 * then must be translated to the calibrated point of symmetry (POS), (Xp, Yp).
 *
 *     X = X0 - Xp
 *     Y = Y0 - Yp
 *
 * Once X and Y are found we need to obtain the radial distance R:
 *     R = sqrt(X^2 + Y^2)
 *
 * SYMMETRIC RADIAL DISTORTION: 
 * To compute the correction for the symmetric radial distortion (DXr, DYr)
 * 
 *     DXr = X(K'0 + K'1*R^2 + K'2*R^4 + K'3*R^6 + K'4*R^8);
 *     DXr = Y(K'0 + K'1*R^2 + K'2*R^4 + K'3*R^6 + K'4*R^8);
 *
 * DECENTERING DISTORTION:
 * To compute the correction for decentering distortion (DXd, DYd) of the measured point use
 * the coefficients of the decentering distortion (P1, .. P4) in the following formula
 *
 *     DXd = (1 + P3*R^2 + P4R^4)*(P1*(R^2 + 2*X^2) + 2*P2*X*Y)
 *     DYd = (1 + P3*R^2 + P4R^4)*(2P1*X*Y + P2*(R^2 + 2*Y^2))
 * 
 * FINAL CORRECTED COORDINATES: 
 * Adding the symmetric radial and decentering distortion corrections to the 
 * translation of the measured point results in a corrected final x and y-coordinate 
 * for the measured point (Xc, Yc).
 *
 *     Xc = X + DXr + DXd
 *     Yc = Y + DYr + DYd 
 *</pre>
 */
class OSSIM_DLL ossimSmacCallibrationSystem : public ossimLensDistortion
{
public:
   ossimSmacCallibrationSystem(const ossimDpt& callibratedCenter = ossimDpt(0.0, 0.0))
   :ossimLensDistortion(callibratedCenter)
   {
      _symmetricRadialDistortionCoefficients[0] = 0;
      _symmetricRadialDistortionCoefficients[1] = 0;
      _symmetricRadialDistortionCoefficients[2] = 0;
      _symmetricRadialDistortionCoefficients[3] = 0;
      _symmetricRadialDistortionCoefficients[4] = 0;
      _decenteringDistortionCoefficients[0]     = 0;
      _decenteringDistortionCoefficients[1]     = 0;
      _decenteringDistortionCoefficients[2]     = 0;
      _decenteringDistortionCoefficients[3]     = 0;
   }
   ossimSmacCallibrationSystem(double k0, double k1, double k2, double k3, double k4,
                               double p1, double p2, double p3, double p4,
                               const ossimDpt& callibratedCenter = ossimDpt(0.0,0.0))
   :ossimLensDistortion(callibratedCenter)
   {
      setSymmetricRadialDistortionCoefficients(k0, k1, k2, k3, k4);
      _decenteringDistortionCoefficients[0]     = p1;
      _decenteringDistortionCoefficients[1]     = p2;
      _decenteringDistortionCoefficients[2]     = p3;
      _decenteringDistortionCoefficients[3]     = p4;
   }
   ossimSmacCallibrationSystem(const ossimSmacCallibrationSystem& src)
   {
      std::copy(src._symmetricRadialDistortionCoefficients,
                src._symmetricRadialDistortionCoefficients+5,
                _symmetricRadialDistortionCoefficients);
      std::copy(src._decenteringDistortionCoefficients,
                src._decenteringDistortionCoefficients+4,
                _decenteringDistortionCoefficients);
   }
   virtual ossimObject* dup()const
   {
      return new ossimSmacCallibrationSystem(*this);
   }
   const double* symmetricRadialDistortionCoefficients()const
   {
      return _symmetricRadialDistortionCoefficients;
   }
   void setSymmetricRadialDistortionCoefficients(double k0, double k1, double k2, double k3, double k4)
   {
      _symmetricRadialDistortionCoefficients[0] = k0;
      _symmetricRadialDistortionCoefficients[1] = k1;
      _symmetricRadialDistortionCoefficients[2] = k2;
      _symmetricRadialDistortionCoefficients[3] = k3;
      _symmetricRadialDistortionCoefficients[4] = k4;
   }
   const double* decenteringDistortionCoefficients()const
   {
      return _decenteringDistortionCoefficients;
   }
   void setDecenteringDistortionCoefficients(double p1, double p2, double p3, double p4)
   {
      _decenteringDistortionCoefficients[0]     = p1;
      _decenteringDistortionCoefficients[1]     = p2;
      _decenteringDistortionCoefficients[2]     = p3;
      _decenteringDistortionCoefficients[3]     = p4;
   }
   const double* radialDistortionCoefficients()const
   {
      return _symmetricRadialDistortionCoefficients;
   }
   virtual void undistort(const ossimDpt& input, ossimDpt& output)const;
   
   static void test();
protected:
   /**
    * The coeeficients are arranged index 0..N-1 where 0 refers to
    * K'0 ... K'(n-1)
    */
   double _symmetricRadialDistortionCoefficients[5];
   
   /**
    * The coeeficients are arranged index 0..N-1 where 0 refers to
    * P1 ... PN
    */
   double _decenteringDistortionCoefficients[4];
   
};

#endif
