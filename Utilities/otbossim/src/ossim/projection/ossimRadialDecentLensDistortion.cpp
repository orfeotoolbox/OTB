//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts 
//
// Description:
//
// MeanRadialLens distortion.
//
//*******************************************************************
//  $Id: ossimRadialDecentLensDistortion.cpp 15766 2009-10-20 12:37:09Z gpotts $

#include <cstdlib>
#include <ossim/projection/ossimRadialDecentLensDistortion.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimCommon.h>

RTTI_DEF1(ossimRadialDecentLensDistortion, "ossimRadialDecentLensDistortion",
          ossim2dTo2dTransform);

const char* ossimRadialDecentLensDistortion::PRINCIPAL_POINT_X_KW
                               = "principal_point_x";
const char* ossimRadialDecentLensDistortion::PRINCIPAL_POINT_Y_KW
                               = "principal_point_y";
const char* ossimRadialDecentLensDistortion::RADIAL_DISTORTION_COEFF_KW
                               = "radial_distortion_coeff_K";
const char* ossimRadialDecentLensDistortion::DECENT_DISTORTION_COEFF_KW
                               = "decent_distortion_coeff_P";

ossimRadialDecentLensDistortion::ossimRadialDecentLensDistortion
(const ossimKeywordlist& kwl, const char* prefix)
   :
   theCalibratedPrincipalPoint (0, 0),
   theRadialDistortionParameters(5),
   theDecentDistortionParameters(5)
{
   loadState(kwl, prefix);
}

bool ossimRadialDecentLensDistortion::loadState(const ossimKeywordlist& kwl,
                                                const char* prefix)
{
   const char* value;

   theCalibratedPrincipalPoint = ossimDpt(0, 0);

   value = kwl.find(prefix, PRINCIPAL_POINT_X_KW);
   if (value)
   {
      theCalibratedPrincipalPoint.x = atof(value);
   }

   value = kwl.find(prefix, PRINCIPAL_POINT_Y_KW);
   if (value)
   {
      theCalibratedPrincipalPoint.y = atof(value);
   }
   
   for (int i=0; i<5; ++i)
   {
      ossimString key = RADIAL_DISTORTION_COEFF_KW;
      key += ossimString::toString(i);
      value = kwl.find(prefix, key.c_str());
      if (!value)
      {
         theRadialDistortionParameters[i] = 0.0;
      }
      else
      {
         theRadialDistortionParameters[i] = ossimString::toDouble(value);
      }

      key = DECENT_DISTORTION_COEFF_KW;
      key += ossimString::toString(i);
      value = kwl.find(prefix, key.c_str());
      if (!value)
      {
         theDecentDistortionParameters[i] = 0.0;
      }
      else
      {
         theDecentDistortionParameters[i] = ossimString::toDouble(value);
      }
   }

   //***
   // Pass on to base class:
   //***
   bool rtn_val = ossim2dTo2dTransform::loadState(kwl, prefix);

   return rtn_val;
}

bool ossimRadialDecentLensDistortion::saveState(ossimKeywordlist& kwl,
                                                const char* prefix) const
{
   //***
   // Principal Point:
   //***
   kwl.add(prefix, PRINCIPAL_POINT_X_KW, theCalibratedPrincipalPoint.x);
   kwl.add(prefix, PRINCIPAL_POINT_Y_KW, theCalibratedPrincipalPoint.y);
   
   for(int i=0; i<5; ++i)
   {
      //---
      // Radial distortion coefficients:
      //---
      ossimString key = RADIAL_DISTORTION_COEFF_KW;
      key += ossimString::toString(i);
      kwl.add(prefix,
              key.c_str(),
              theRadialDistortionParameters[i],
              true,  // overwrite
              15); // scientific

      //---
      // Decentering distortion coefficients:
      //---
      key = DECENT_DISTORTION_COEFF_KW;
      key += ossimString::toString(i);
      kwl.add(prefix,
              key.c_str(),
              theDecentDistortionParameters[i],
              true,  // overwrite
              15); // scientific
   }

   //***
   // Pass on to base class:
   //***
   bool rtn_val = ossim2dTo2dTransform::saveState(kwl, prefix);

   return rtn_val;
}
   
//*****************************************************************************
//  METHOD
//*****************************************************************************
double ossimRadialDecentLensDistortion::deltaR(double radius)const
{
  double rSquare = radius*radius;

  return theRadialDistortionParameters[0] +
     rSquare*(theRadialDistortionParameters[1] +
              rSquare*(theRadialDistortionParameters[2] +
                       rSquare*(theRadialDistortionParameters[3] +
                                rSquare*theRadialDistortionParameters[4])));
}

//*****************************************************************************
//  METHOD
//*****************************************************************************
void ossimRadialDecentLensDistortion::forward(const ossimDpt& input_pt,
                                              ossimDpt&       output_pt)
   const
{
   ossimDpt deltaPosition(input_pt.x - theCalibratedPrincipalPoint.x,
                          input_pt.y - theCalibratedPrincipalPoint.y);
   double r = sqrt(ossim::square(deltaPosition.x) + 
                   ossim::square(deltaPosition.y));
   double dr = deltaR(r);
   double deltaX1 = deltaPosition.x*dr;
   double deltaY1 = deltaPosition.y*dr;

   double rSquare = r*r;
   double deltaX2 =  (1 + theDecentDistortionParameters[3]*rSquare +
                      theDecentDistortionParameters[4]*rSquare*rSquare)*
                     (theDecentDistortionParameters[1]*(rSquare + 2*deltaPosition.x*deltaPosition.x) +
                      2*theDecentDistortionParameters[2]*deltaPosition.x*deltaPosition.y);
   
   double deltaY2 =  (1 + theDecentDistortionParameters[3]*rSquare +  theDecentDistortionParameters[4]*rSquare*rSquare)*
                     (theDecentDistortionParameters[2]*(rSquare + 2*deltaPosition.y*deltaPosition.y) +
                      2*theDecentDistortionParameters[1]*deltaPosition.x*deltaPosition.y);

   output_pt.x = deltaPosition.x + deltaX1 + deltaX2;
   output_pt.y = deltaPosition.y + deltaY1 + deltaY2;
}

//*****************************************************************************
//  METHOD: ossimRadialDecentLensDistortion::print()
//  
//*****************************************************************************
std::ostream& ossimRadialDecentLensDistortion::print(std::ostream& os) const
{
   os << "\nDump of ossimRadialDecentLensDistortion object" << endl;
   
   os << "  theCalibratedPrincipalPoint: " << theCalibratedPrincipalPoint;
   
   for (unsigned int i=0; i<5; i++)
      os << "\n  K["<<i<<"]: " << theRadialDistortionParameters[i];  
   
   for (unsigned int i=0; i<5; i++)
      os << "\n  P["<<i<<"]: " << theDecentDistortionParameters[i];  
   
   os << endl;

   // call base class print???
   
   return os;
}
