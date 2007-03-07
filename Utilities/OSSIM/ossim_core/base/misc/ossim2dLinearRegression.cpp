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
// $Id: ossim2dLinearRegression.cpp,v 1.3 2005/12/07 20:24:17 gpotts Exp $
#include "ossim2dLinearRegression.h"

ossim2dLinearRegression::ossim2dLinearRegression()
{
   clear();
}

void ossim2dLinearRegression::solve()
{
   
//    theSumX           = 0.0;
//    theSumXX          = 0.0;
//    theSumY           = 0.0;
//    theSumYY          = 0.0;
//    theIntercept      = 0.0;
//    theSlope          = 0.0;
//    theSumXY          = 0.0;
//    ossim_uint32 idx = 0;
//    for(idx = 0; idx < thePoints.size(); ++idx)
//    {
//       theSumX  += thePoints[idx].x;
//       theSumY  += thePoints[idx].y;
//       theSumXX += thePoints[idx].x*thePoints[idx].x;
//       theSumYY += thePoints[idx].y*thePoints[idx].y;
//       theSumXY += thePoints[idx].x*thePoints[idx].y; 
      
//    }
   
   double numberOfPoints = (double)theNumberOfPoints;

   if(numberOfPoints < 1)
   {
      theSlope = 0.0;
      theIntercept = 0.0;
      return;
   }
   double Sxx = theSumXX - ((theSumX*theSumX)/numberOfPoints);
   double Sxy = theSumXY - ((theSumX*theSumY)/numberOfPoints);

   if(fabs(Sxx) < FLT_EPSILON)
   {
      theSlope = 0.0;
      theIntercept = 0.0;
      return;
   }
   
   theSlope     = Sxy/Sxx;
   theIntercept = (theSumY - theSlope*theSumX)/numberOfPoints;

   theSolvedFlag = true;
}
