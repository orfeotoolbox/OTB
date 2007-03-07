//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author:  Garrett Potts (gpotts@imagelinks.com
//
// Description: Source code produced by Dave Knopp
//
//*******************************************************************
//  $Id: ossimLeastSquaresBilin.h,v 1.4 2004/04/02 13:49:19 gpotts Exp $
#ifndef    ossimLeastSquaresBilin_INCLUDE
#define    ossimLeastSquaresBilin_INCLUDE
#include <base/common/ossimConstants.h>
#include <matrix/newmat.h>

/** 
 * @brief Provide 2D Least Squares Bilinear model fitting
 * The math model is that of a bilinear surface of the form:
@code
z(x,y) = a + b*x + c*y + d*x*y
@endcode
 * The getLSParms() method returns parameter values which are the least
 * squares solution associated with the samples added via addSample(). Note
 * that it is necessary to add at least four sample to obtain a solution.
 */

class OSSIMDLLEXPORT ossimLeastSquaresBilin
{
public:

   ossimLeastSquaresBilin(const ossimLeastSquaresBilin &);
   /** 
    * Instantiate as zero surface.
    */
   ossimLeastSquaresBilin();
   
   ossimLeastSquaresBilin & operator = (const ossimLeastSquaresBilin &);
   
   /**
    * Free internal storage.
    */
   virtual ~ossimLeastSquaresBilin();
   

   /**
    * Will clear everything and set it up to
    * for another solve.  Just add points
    * and call the solve method.
    */
   virtual void clear();

   /**
    * add a single data sample.
    *
    * @param xx "x" coordinate of sample location.
    * @param yy "y"  "y" coordinate of sample location.
    * @param zmea sample value measured at (xx,yy)
    */
   virtual void addSample(double x, double yy, double zmea);
   
   /**
    * return LS solution parameters.
    *
    * @param pa set to constant coefficient.
    * @param pb_x set to linear coefficient of "x"
    * @param pc_y set to linear coefficient of "y"
    * @param pd_xy set to cross coefficient of "x*y"
    */
   virtual bool getLSParms(double& pa, double& pb_x, double& pc_y, double& pd_xy)const;

   /**
    * @param pa set to constant coefficient.
    * @param pb_x set to linear coefficient of "x"
    * @param pc_y set to linear coefficient of "y"
    * @param pd_xy set to cross coefficient of "x*y"
    */
   virtual void setLSParams(double pa, //constant
                            double pb_x, // linear coefficient of x,
                            double pc_y, // linear coefficient of y
                            double pd_xy); // coefficient of x*y
                           
   /**
    * interpolate LS-fit value at location (xx,yy) - returns z(xx,yy).
    *
    * @param xx "x" coordinate at which to interpolate.
    * @param yy "y" "y" coordinate at which to interpolate.
    * 
    */
   virtual inline double lsFitValue(double xx, double yy) const
      {
         return (bl_a + bl_b*xx + bl_c*yy + bl_d*xx*yy);
      }
   
   /**
    * compute least squares parameter solution - true if succesfull.
    */
   bool solveLS();
   
private:
   /**
    * constant term.
    */
   double bl_a;  

   /**
    * linear-X term.
    */
   double bl_b;

   /**
    * linear-Y term.
    */
   double bl_c;

   /**
    * cross-XY term
    */
   double bl_d;   

   /**
    * Normal system coefficient matrix.
    */
   NEWMAT::Matrix*  AtA;

   /**
    * Normal system RHS vector
    */
   NEWMAT::Matrix*  Atb;
};

#endif //  LeastSquaresBilinilin_INCL_

