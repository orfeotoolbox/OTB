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
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossim2dLinearRegression.h,v 1.5 2005/12/07 20:24:17 gpotts Exp $
#ifndef ossim2dLinearRegression_HEADER
#define ossim2dLinearRegression_HEADER
#include "base/data_types/ossimDpt.h"
#include <vector>
#include <iostream>

class ossim2dLinearRegression
{
public:
   friend std::ostream& operator <<(std::ostream& out, const ossim2dLinearRegression& data)
      {
         out << "y = " << data.theSlope << "*x + " << data.theIntercept;

         return out;
      }
   ossim2dLinearRegression();
   void getEquation(double& slope,
                    double& intercept)
   {
      slope     = theSlope;
      intercept = theIntercept;
   }
   void addPoint(const ossimDpt& pt)
      {
//         thePoints.push_back(pt);
         ++theNumberOfPoints;
         theSumX  += pt.x;
         theSumY  += pt.y;
         theSumXX += pt.x*pt.x;
         theSumYY += pt.y*pt.y;
         theSumXY += pt.x*pt.y; 
         theSolvedFlag     = false;
      }
   void clear()
      {
//         thePoints.clear();
         theNumberOfPoints = 0;
         theSumX           = 0.0;
         theSumXX          = 0.0;
         theSumY           = 0.0;
         theSumYY          = 0.0;
         theIntercept      = 0.0;
         theSlope          = 0.0;
         theSumXY          = 0.0;
         theSolvedFlag     = false;
      }
   void solve();
   ossim_uint32 getNumberOfPoints()const
      {
         return theNumberOfPoints;
//         return thePoints.size();
      }
protected:
//   std::vector<ossimDpt> thePoints;
   ossim_uint32 theNumberOfPoints;
   double theSumX;
   double theSumY;
   double theSumXX;
   double theSumYY;
   double theSumXY;
   
   double theIntercept;
   double theSlope;
   bool   theSolvedFlag;
};

#endif
