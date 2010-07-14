//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description: Modeling for lens radial and decentering distortion.
//
//   The default implementation is:
//  
//   Let X and Y be coordinates relative to the center. Let
//   Xc and Yc be the adjusted point so that:
//  
//   Xc = X + delta1X + delta2X
//   Yc = Y + delta1Y + delta2Y
//  
//   
//   delta1X = X*(k0 + k1*r^2 + k2*r^4 + k3*r^6 + k4*r^8)
//   delta1Y = Y*(k0 + k1*r^2 + k2*r^4 + k3*r^6 + k4*r^8)
//   
//   delta2X = (1 + p3*r^2 + p4*r^4)[p1*(r^2 + 2*X^2) + 2*p2*X*Y]
//   delta2Y = (1 + p3*r^2 + p4*r^4)[2*p1*X*Y  + p2*(r^2 + 2*Y^2) ]
//  
//  
//   Note the point that is passed in we will compute:
//        <X, Y> =  position - calibratedPrincipalPoint.
//  
//        so if you compute the delta then make sure that
//        this class is constructed with principal point
//        at 0,0
//  
//   Reference book: Elements of Photogrammetry Paul Wolf
//   
//*******************************************************************
//  $Id: ossimRadialDecentLensDistortion.h 17195 2010-04-23 17:32:18Z dburken $
#ifndef ossimRadialDecentLensDistortion_HEADER
#define ossimRadialDecentLensDistortion_HEADER
#include <iostream>
using namespace std;

#include <ossim/matrix/newmat.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossim2dTo2dTransform.h>

//*****************************************************************************
//  CLASS
//*****************************************************************************
class OSSIMDLLEXPORT ossimRadialDecentLensDistortion
   : public ossim2dTo2dTransform
{
public:
   ossimRadialDecentLensDistortion() {}
   ossimRadialDecentLensDistortion
      (const ossimDpt& calibratedPrincipalPoint,
       const NEWMAT::ColumnVector &radialDistortionParameters,
       const NEWMAT::ColumnVector &decentDistortionParameters)
      : theCalibratedPrincipalPoint(calibratedPrincipalPoint),
        theRadialDistortionParameters(radialDistortionParameters),
        theDecentDistortionParameters(decentDistortionParameters)
      {  }

   ossimRadialDecentLensDistortion(const ossimRadialDecentLensDistortion& copy)
      : theCalibratedPrincipalPoint (copy.theCalibratedPrincipalPoint),
        theRadialDistortionParameters (copy.theRadialDistortionParameters),
        theDecentDistortionParameters (copy.theDecentDistortionParameters) { }

   ossimRadialDecentLensDistortion(const ossimKeywordlist& kwl,
                                   const char* prefix);

   virtual ~ossimRadialDecentLensDistortion(){}
   
   /*!
    * Implementation of base class 2D-to-2D transformation. The "forward"
    * transformation is defined here as going from an undistorted ideal point to
    * a distorted real point, i.e., adding distortion.
    * 
    * Also available (implemented in the base class) are:
    *   inverse(distorted_point_in, undistorted_pt_out)
    *   inverse(undistort_this_pt)
    */
   virtual void forward(const ossimDpt& undistorted_point_in,
                        ossimDpt&       distorted_point_out) const;

   virtual void forward(ossimDpt&  modify_this) const 
      {
         ossimDpt output;
         forward(modify_this, output);
         modify_this = output;
      }
      
   
   /*!
    * Method to save the state of the object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;

   /*!
    * Method to the load (recreate) the state of the object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /*!
    * Set methods provide alternative initialization scheme to loadState:
    */
   void setPrincipalPoint(const ossimDpt pp);
   void setRadialDistortionParams(const NEWMAT::ColumnVector& params);
   void setDecentDistortionParams(const NEWMAT::ColumnVector& params);
   /*!
    * Dumps contents of object to ostream.
    */
   virtual std::ostream& print(std::ostream& out) const;
   
   static const char* PRINCIPAL_POINT_X_KW;
   static const char* PRINCIPAL_POINT_Y_KW;
   static const char* RADIAL_DISTORTION_COEFF_KW;
   static const char* DECENT_DISTORTION_COEFF_KW;
   
private:
   /*!
    * default implementation Will solve the polynomial:
    *
    * k0 + k1*r^2 + k2*r^4 + k3*r^6 + k4*r^8
    */
   virtual double deltaR(double radius)const;
   
   ossimDpt     theCalibratedPrincipalPoint;
   NEWMAT::ColumnVector theRadialDistortionParameters;
   NEWMAT::ColumnVector theDecentDistortionParameters;  

   TYPE_DATA
};

//*****************************************************************************
//  INLINE METHODS
//*****************************************************************************
inline void
ossimRadialDecentLensDistortion::setPrincipalPoint(const ossimDpt pp)
{
   theCalibratedPrincipalPoint = pp;
}

inline void ossimRadialDecentLensDistortion::setRadialDistortionParams
   (const NEWMAT::ColumnVector& params)
{
   theRadialDistortionParameters = params;
}

inline void ossimRadialDecentLensDistortion::setDecentDistortionParams
   (const NEWMAT::ColumnVector& params)
{
   theDecentDistortionParameters = params;
}


#endif
