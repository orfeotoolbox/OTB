//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// LGPL
// 
// Author:  Garrett Potts
//
// Description:
//
// ossimLensDistortion
//*******************************************************************
//  $Id: ossimLensDistortion.cpp 15929 2009-11-16 18:20:46Z gpotts $
#include <sstream>
#include <ossim/projection/ossimLensDistortion.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimString.h>

RTTI_DEF1(ossimLensDistortion, "ossimLensDistortion", ossim2dTo2dTransform);

bool ossimLensDistortion::saveState(ossimKeywordlist& kwl,
                                    const char* prefix)const
{
   ossim2dTo2dTransform::saveState(kwl, prefix);
   
   kwl.add(prefix,
           "center",
           ossimString::toString(theCenter.x) + " " + ossimString::toString(theCenter.y),
           true);

   return true;
}

bool ossimLensDistortion::loadState(const ossimKeywordlist& kwl,
                                    const char* prefix)
{
   const char* center = kwl.find(prefix, "center");
   theCenter.x = 0.0;
   theCenter.y = 0.0;
   
   if(center)
   {
      std::vector<ossimString> splitString;
      ossimString tempString(center);
      tempString = tempString.trim();
      tempString.split(splitString, " ");
      if(splitString.size() == 2)
      {
         theCenter.x = splitString[0].toDouble();
         theCenter.y = splitString[1].toDouble();
      }
   }

   return ossim2dTo2dTransform::loadState(kwl, prefix);

}

void ossimLensDistortion::distort(const ossimDpt& input, ossimDpt& output)const
{
   int iters = 0;
   
   //***
   // Begin with guess. Forward transform is defined as trasforming left to
   // right. We are therefore solving for left:
   //***
   ossimDpt left (input);
   ossimDpt left_dx;
   ossimDpt left_dy;
   ossimDpt right;
   ossimDpt right_dx;
   ossimDpt right_dy;
   ossimDpt dr_dx;
   ossimDpt dr_dy;
   ossimDpt r_diff;
   ossimDpt l_diff;
   double inverse_norm;
   
   //***
   // Begin iterations:
   //***
   do
   {
      //***
      // establish perturbed image points about the guessed point:
      //***
      left_dx.x = left.x + 1.0;
      left_dx.y = left.y;
      left_dy.x = left.x;
      left_dy.y = left.y + 1.0;
      
      //***
      // Compute numerical partials at current guessed point:
      //***
      undistort(left,    right);
      undistort(left_dx, right_dx);
      undistort(left_dy, right_dy);
      
      dr_dx.x = (right_dx.x - right.x); //e
      dr_dx.y = (right_dx.y - right.y); //g
      dr_dy.x = (right_dy.x - right.x); //f
      dr_dy.y = (right_dy.y - right.y); //h
      
      //***
      // Test for convergence:
      //***
      r_diff = input - right;
      
      //***
      // Compute linearized estimate of image point given gp delta:
      //***
      inverse_norm = dr_dy.u*dr_dx.v - dr_dx.u*dr_dy.v; // fg-eh
      if (inverse_norm != 0)
      {
         l_diff.u = (-dr_dy.v*r_diff.u + dr_dy.u*r_diff.v)/inverse_norm;
         l_diff.v = ( dr_dx.v*r_diff.u - dr_dx.u*r_diff.v)/inverse_norm;
         left += l_diff;
      }
      else
      {
         l_diff.u = 0;
         l_diff.v = 0;
      }
      
      iters++;
      
   } while (((fabs(l_diff.u) > theConvergenceThreshold) ||
             (fabs(l_diff.v) > theConvergenceThreshold)) &&
            (iters < theMaxIterations));
   
   
   output = left;
}

